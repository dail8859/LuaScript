// This file is part of LuaScript.
// 
// Copyright (C)2016 Justin Dailey <dail8859@yahoo.com>
// 
// LuaScript is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include <sstream>
#include <algorithm>
#include "LuaConsole.h"
#include "SciIFaceTable.h"
#include "NppIFaceTable.h"
#include "SciLexer.h"


#define INDIC_BRACEHIGHLIGHT INDIC_CONTAINER
#define INDIC_BRACEBADLIGHT INDIC_CONTAINER + 1

// Extra helper functions not from the IFace tables
static const std::vector<std::string> moreSciFuncs = { "append", "findtext", "match", "remove", "set_text_direction", "textrange" };
static const std::vector<std::string> moreNppFuncs = { "AddEventHandler", "AddShortcut", "ClearConsole", "ConstantName", "RemoveAllEventHandlers", "RemoveEventHandler", "SendEditor", "StartTimer", "StopTimer", "WriteError" };
static const std::vector<std::string> moreNppProps = { "UniMode" };

// Copied from Scintilla
inline int MakeUpperCase(int ch) {
	if (ch < 'a' || ch > 'z')
		return ch;
	else
		return static_cast<char>(ch - 'a' + 'A');
}

// Copied from Scintilla
int CompareNCaseInsensitive(const char *a, const char *b, size_t len) {
	while (*a && *b && len) {
		if (*a != *b) {
			char upperA = static_cast<char>(MakeUpperCase(*a));
			char upperB = static_cast<char>(MakeUpperCase(*b));
			if (upperA != upperB)
				return upperA - upperB;
		}
		a++;
		b++;
		len--;
	}
	if (len == 0)
		return 0;
	else
		// Either *a or *b is nul
		return *a - *b;
}

// Copied (and modified) from Scintilla
struct Sorter {
	bool ignoreCase;

	Sorter(bool ignoreCase_) : ignoreCase(ignoreCase_) {}

	inline bool operator()(const std::string& a, const std::string& b) {
		auto lenA = a.length();
		auto lenB = b.length();
		auto len = min(lenA, lenB);
		int cmp;
		if (ignoreCase)
			cmp = CompareNCaseInsensitive(a.c_str(), b.c_str(), len);
		else
			cmp = strncmp(a.c_str(), b.c_str(), len);
		if (cmp == 0)
			cmp = static_cast<int>(lenA - lenB);
		return cmp < 0;
	}
};

static std::vector<std::string> &sortCaseInsensitive(std::vector<std::string> &strings) {
	std::sort(strings.begin(), strings.end(), Sorter(true));
	return strings;
}

static bool inline isBrace(int ch) {
	return strchr("[]{}()", ch) != NULL;
}

static std::string getRange(GUI::ScintillaWindow *sw, intptr_t start, intptr_t end) {
	if (end <= start) return std::string();

	std::vector<char> buffer(end - start + 1);
	Sci_TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = buffer.data();

	sw->CallPointer(SCI_GETTEXTRANGE, 0, &tr);

	return std::string(buffer.begin(), buffer.end() - 1); // don't copy the null
}

static std::string getWordAt(GUI::ScintillaWindow *sw, intptr_t pos) {
	intptr_t word_start = sw->Call(SCI_WORDSTARTPOSITION, pos, true);
	intptr_t word_end = sw->Call(SCI_WORDENDPOSITION, pos, true);
	return getRange(sw, word_start, word_end);
}

static std::string getLuaIdentifierAt(GUI::ScintillaWindow *sw, intptr_t pos) {
	const intptr_t line = sw->Call(SCI_LINEFROMPOSITION);

	Sci_TextToFind ttf = {
		{
			// Search backwards
			pos,
			sw->Call(SCI_POSITIONFROMLINE, line)
		},
		"[a-z_][a-z0-9_]*(\\.[a-z_][a-z0-9_]*)*",
	};

	// Only return the range if it ends at pos
	if (sw->CallPointer(SCI_FINDTEXT, SCFIND_REGEXP, &ttf) != -1 && ttf.chrgText.cpMax == pos)
		return getRange(sw, ttf.chrgText.cpMin, ttf.chrgText.cpMax);
	else
		return std::string();
}

template <typename T, typename U>
static std::string join(const std::vector<T> &v, const U &delim) {
	std::stringstream ss;
	for (size_t i = 0; i < v.size(); ++i) {
		if (i != 0) ss << delim;
		ss << v[i];
	}
	return ss.str();
}

static void setStyles(GUI::ScintillaWindow &sci) {
	sci.Call(SCI_SETEOLMODE, SC_EOL_CRLF, 0);

	sci.Call(SCI_STYLESETFORE, STYLE_DEFAULT, 0x000000);
	sci.Call(SCI_STYLESETBACK, STYLE_DEFAULT, 0xFFFFFF);
	sci.Call(SCI_STYLECLEARALL, 0, 0);

	// Setup the margins
	sci.Call(SCI_SETMARGINWIDTHN, 0, 0);
	sci.Call(SCI_SETMARGINWIDTHN, 2, 0);
	sci.Call(SCI_SETMARGINWIDTHN, 3, 0);
	sci.Call(SCI_SETMARGINWIDTHN, 4, 0);

	sci.Call(SCI_SETCODEPAGE, CP_UTF8);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_COMMENT, 0x008000);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_COMMENTLINE, 0x008000);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_COMMENTDOC, 0x808000);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_LITERALSTRING, 0x4A0095);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_PREPROCESSOR, 0x004080); // Technically not used since this is lua 5+
	sci.Call(SCI_STYLESETFORE, SCE_LUA_WORD, 0xFF0000);
	sci.Call(SCI_STYLESETBOLD, SCE_LUA_WORD, 1); // for SCI_SETKEYWORDS, 0
	sci.Call(SCI_STYLESETFORE, SCE_LUA_NUMBER, 0x0080FF);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_STRING, 0x808080);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_CHARACTER, 0x808080);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_OPERATOR, 0x800000);
	sci.Call(SCI_STYLESETBOLD, SCE_LUA_OPERATOR, 1);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_WORD2, 0xC08000);
	sci.Call(SCI_STYLESETBOLD, SCE_LUA_WORD2, 1); // for SCI_SETKEYWORDS, 1
	sci.Call(SCI_STYLESETFORE, SCE_LUA_WORD3, 0xFF0080);
	sci.Call(SCI_STYLESETBOLD, SCE_LUA_WORD3, 1); // for SCI_SETKEYWORDS, 2
	sci.Call(SCI_STYLESETFORE, SCE_LUA_WORD4, 0xA00000);
	sci.Call(SCI_STYLESETBOLD, SCE_LUA_WORD4, 1);
	sci.Call(SCI_STYLESETITALIC, SCE_LUA_WORD4, 1); // for SCI_SETKEYWORDS, 3
	sci.Call(SCI_STYLESETFORE, SCE_LUA_LABEL, 0x008080);
	sci.Call(SCI_STYLESETBOLD, SCE_LUA_LABEL, 1);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_WORD5, 0x004080); // for SCI_SETKEYWORDS, 4, Scintilla defines
	sci.Call(SCI_STYLESETBOLD, SCE_LUA_WORD5, 1);
	sci.Call(SCI_STYLESETFORE, SCE_LUA_WORD6, 0x004080); // for SCI_SETKEYWORDS, 5, Notepad++ defines
	sci.Call(SCI_STYLESETBOLD, SCE_LUA_WORD6, 1);
}

LuaConsole::LuaConsole(HWND hNotepad) : mp_consoleDlg(new ConsoleDialog()), m_hNotepad(hNotepad), m_nppData(new NppData) {
	// Scintilla properties
	sciProperties = join(sortCaseInsensitive(SciIFaceTable.GetAllPropertyNames()), ' ');

	// Scintilla methods
	auto sciFuncNames = SciIFaceTable.GetAllFunctionNames();
	sciFuncNames.insert(sciFuncNames.end(), moreSciFuncs.begin(), moreSciFuncs.end());
	sciFunctions = join(sortCaseInsensitive(sciFuncNames), ' ');

	// Notepad++ properties
	auto nppPropNames = NppIFaceTable.GetAllPropertyNames();
	nppPropNames.insert(nppPropNames.end(), moreNppFuncs.begin(), moreNppFuncs.end());
	nppPropNames.insert(nppPropNames.end(), moreNppProps.begin(), moreNppProps.end());
	nppProperties = join(sortCaseInsensitive(nppPropNames), ' ');

	// Notepad++ functions
	nppFunctions = join(sortCaseInsensitive(NppIFaceTable.GetAllFunctionNames()), ' ');

	auto nppConsts = NppIFaceTable.GetAllConstantNames();
	auto sciConsts = SciIFaceTable.GetAllConstantNames();
	nppConsts.insert(nppConsts.end(), sciConsts.begin(), sciConsts.end());
	globalConsts = nppConsts;
}

void LuaConsole::setupInput(GUI::ScintillaWindow &sci) {
	// Have it actually do the lexing
	sci.Call(SCI_SETLEXER, SCLEX_LUA);

	// Set up the styles
	setStyles(sci);

	// Setup Keywords
	sci.CallString(SCI_SETKEYWORDS, 0, "and break do else elseif end false for function goto if in local nil not or repeat return then true until while");
	sci.CallString(SCI_SETKEYWORDS, 1, "_ENV _G _VERSION assert collectgarbage dofile error getfenv getmetatable ipairs load loadfile loadstring module next pairs pcall print rawequal rawget rawlen rawset require select setfenv setmetatable tonumber tostring type unpack xpcall string table math bit32 coroutine io os debug package __index __newindex __call __add __sub __mul __div __mod __pow __unm __concat __len __eq __lt __le __gc __mode");
	sci.CallString(SCI_SETKEYWORDS, 2, "byte char dump find format gmatch gsub len lower rep reverse sub upper abs acos asin atan atan2 ceil cos cosh deg exp floor fmod frexp ldexp log log10 max min modf pow rad random randomseed sin sinh sqrt tan tanh arshift band bnot bor btest bxor extract lrotate lshift replace rrotate rshift shift string.byte string.char string.dump string.find string.format string.gmatch string.gsub string.len string.lower string.match string.rep string.reverse string.sub string.upper table.concat table.insert table.maxn table.pack table.remove table.sort table.unpack math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos math.cosh math.deg math.exp math.floor math.fmod math.frexp math.huge math.ldexp math.log math.log10 math.max math.min math.modf math.pi math.pow math.rad math.random math.randomseed math.sin math.sinh math.sqrt math.tan math.tanh bit32.arshift bit32.band bit32.bnot bit32.bor bit32.btest bit32.bxor bit32.extract bit32.lrotate bit32.lshift bit32.replace bit32.rrotate bit32.rshift");
	sci.CallString(SCI_SETKEYWORDS, 3, "close flush lines read seek setvbuf write clock date difftime execute exit getenv remove rename setlocale time tmpname coroutine.create coroutine.resume coroutine.running coroutine.status coroutine.wrap coroutine.yield io.close io.flush io.input io.lines io.open io.output io.popen io.read io.tmpfile io.type io.write io.stderr io.stdin io.stdout os.clock os.date os.difftime os.execute os.exit os.getenv os.remove os.rename os.setlocale os.time os.tmpname debug.debug debug.getfenv debug.gethook debug.getinfo debug.getlocal debug.getmetatable debug.getregistry debug.getupvalue debug.getuservalue debug.setfenv debug.sethook debug.setlocal debug.setmetatable debug.setupvalue debug.setuservalue debug.traceback debug.upvalueid debug.upvaluejoin package.cpath package.loaded package.loaders package.loadlib package.path package.preload package.seeall");

	// Highlight constants
	sci.CallString(SCI_SETKEYWORDS, 4, join(SciIFaceTable.GetAllConstantNames(), ' ').c_str());
	sci.CallString(SCI_SETKEYWORDS, 5, join(NppIFaceTable.GetAllConstantNames(), ' ').c_str());

	// Set up some autocomplete junk
	sci.Call(SCI_AUTOCSETIGNORECASE, true);
	sci.Call(SCI_AUTOCSETMAXHEIGHT, 8);
	sci.Call(SCI_AUTOCSETCANCELATSTART, false);

	sci.Call(SCI_INDICSETSTYLE, INDIC_BRACEHIGHLIGHT, INDIC_STRAIGHTBOX);
	sci.Call(SCI_INDICSETUNDER, INDIC_BRACEHIGHLIGHT, true);
	sci.Call(SCI_BRACEHIGHLIGHTINDICATOR, true, INDIC_BRACEHIGHLIGHT);

	sci.Call(SCI_SETINDENTATIONGUIDES, SC_IV_LOOKBOTH);

	m_sciInput = &sci;
}

void LuaConsole::setupOutput(GUI::ScintillaWindow &sci) {
	sci.Call(SCI_SETUNDOCOLLECTION, 0);
	sci.Call(SCI_SETREADONLY, 1);
	sci.Call(SCI_SETLEXER, SCLEX_NULL);

	// Set up the styles
	setStyles(sci);
}

bool LuaConsole::processNotification(const SCNotification *scn) {
	enum AutoCFinish { acf_none, acf_brackets, acf_parens };
	static AutoCFinish acf = acf_none;
	static bool triggerAc = false;

	switch (scn->nmhdr.code) {
		case SCN_CHARADDED: {
			if ((scn->ch == '.' || scn->ch == ':') && m_sciInput->Call(SCI_GETCURRENTPOS) > 1) {
				showAutoCompletion();
			}
			else if (scn->ch == '\n') {
				maintainIndentation();
			}
			break;
		}
		case SCN_UPDATEUI: {
			braceMatch();
			if (acf != acf_none) {
				m_sciInput->CallString(SCI_ADDTEXT, 2, acf == acf_brackets ? "[]" : "()");
				m_sciInput->Call(SCI_CHARLEFT);
				acf = acf_none;
			}

			if (triggerAc) {
				triggerAc = false;
				m_sciInput->Call(SCI_AUTOCCANCEL);
				showAutoCompletion();
			}
			break;
		}
		case SCN_MODIFIED:
			// Detect certain modifcations (e.g. pasting) when auto-complete is active
			if (scn->modificationType & SC_MOD_INSERTTEXT && scn->length > 1 && scn->linesAdded == 0 && m_sciInput->Call(SCI_AUTOCACTIVE)) {
				// Auto-complete needs delayed since caret position information is not updated yet
				triggerAc = true;
			}
			break;
		case SCN_AUTOCSELECTION: {
			// See if it's a Sci or Npp property
			auto prop = SciIFaceTable.FindProperty(scn->text);
			if (prop != nullptr && prop->paramType != iface_void) {
				acf = acf_brackets;
				break;
			}
			else {
				prop = NppIFaceTable.FindProperty(scn->text);
				if (prop != nullptr && prop->paramType != iface_void) {
					acf = acf_brackets;
					break;
				}
			}

			// See if it's a Sci or Npp function
			if (SciIFaceTable.FindFunction(scn->text) != nullptr || NppIFaceTable.FindFunction(scn->text) != nullptr) {
				acf = acf_parens;
				break;
			}

			// May be one of the extra functions
			if (std::binary_search(moreNppFuncs.begin(), moreNppFuncs.end(), scn->text) ||
				std::binary_search(moreSciFuncs.begin(), moreSciFuncs.end(), scn->text)) {
				acf = acf_parens;
				break;
			}

			break;
		}

	}
	return true;
}

void LuaConsole::maintainIndentation() {
	intptr_t curPos = m_sciInput->Call(SCI_GETCURRENTPOS);
	intptr_t curLine = m_sciInput->Call(SCI_LINEFROMPOSITION, curPos);
	intptr_t prevIndent = m_sciInput->Call(SCI_GETLINEINDENTATION, curLine - 1);
	m_sciInput->Call(SCI_SETLINEINDENTATION, curLine, prevIndent);
	curPos = m_sciInput->Call(SCI_GETLINEINDENTPOSITION, curLine);
	m_sciInput->Call(SCI_SETEMPTYSELECTION, curPos);
}

void LuaConsole::braceMatch() {
	intptr_t curPos = m_sciInput->Call(SCI_GETCURRENTPOS);
	intptr_t bracePos = INVALID_POSITION;

	// Check on both sides
	if (isBrace(static_cast<int>(m_sciInput->Call(SCI_GETCHARAT, curPos - 1)))) {
		bracePos = curPos - 1;
	}
	else if (isBrace(static_cast<int>(m_sciInput->Call(SCI_GETCHARAT, curPos)))) {
		bracePos = curPos;
	}

	// See if we are next to a brace
	if (bracePos != INVALID_POSITION) {
		intptr_t otherPos = m_sciInput->Call(SCI_BRACEMATCH, bracePos, 0);
		if (otherPos != INVALID_POSITION) {
			m_sciInput->Call(SCI_BRACEHIGHLIGHT, bracePos, otherPos);
		}
		else {
			m_sciInput->Call(SCI_BRACEHIGHLIGHT, INVALID_POSITION, INVALID_POSITION);
		}
	}
	else {
		m_sciInput->Call(SCI_BRACEHIGHLIGHT, INVALID_POSITION, INVALID_POSITION);
	}
}

void LuaConsole::showAutoCompletion() {
	std::string partialWord;
	intptr_t curPos = m_sciInput->Call(SCI_GETCURRENTPOS);
	int prevCh = static_cast<int>(m_sciInput->Call(SCI_GETCHARAT, curPos - 1));

	// The cursor could be at the end of a partial word e.g. editor.Sty|
	if (isalpha(prevCh) || prevCh == '_') {
		partialWord = getWordAt(m_sciInput, curPos - 1);

		// Back up past the partial word
		prevCh = static_cast<int>(m_sciInput->Call(SCI_GETCHARAT, curPos - 1 - partialWord.size()));
		curPos = curPos - static_cast<int>(partialWord.size());
	}

	if (prevCh == '.' || prevCh == ':') {
		std::string prev = getLuaIdentifierAt(m_sciInput, curPos - 1);

		if (!prev.empty()) {
			auto udataname = LuaExtension::Instance().GetUserDataName(prev.c_str());
			if (udataname == "Npp_MT_Application") {
				if (prevCh == '.') {
					m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), nppProperties.c_str());
				}
				else if (prevCh == ':') {
					m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), nppFunctions.c_str());
				}
			}
			else if (udataname == "Npp_MT_Pane") {
				if (prevCh == '.') {
					m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), sciProperties.c_str());
				}
				else if (prevCh == ':') {
					m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), sciFunctions.c_str());
				}
			}
			else if (prev.length() > 0) {
				auto list = LuaExtension::Instance().GetObjectAttributes(prev.c_str(), prevCh == ':');
				if (list.size() > 0)
					m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), join(sortCaseInsensitive(list), ' ').c_str());
			}
		}
	}
	else if (m_sciInput->Call(SCI_AUTOCACTIVE) == false) {
		auto list = LuaExtension::Instance().GetObjectAttributes("_G", false);
		list.insert(list.end(), globalConsts.begin(), globalConsts.end());
		m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), join(sortCaseInsensitive(list), ' ').c_str());
	}
}

std::string LuaConsole::getGlobalConstants() {
	return join(sortCaseInsensitive(globalConsts), ' ');
}
