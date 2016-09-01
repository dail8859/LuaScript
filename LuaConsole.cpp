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

#include "LuaConsole.h"
#include "SciIFaceTable.h"
#include "NppIFaceTable.h"


#define INDIC_BRACEHIGHLIGHT INDIC_CONTAINER
#define INDIC_BRACEBADLIGHT INDIC_CONTAINER + 1

static bool inline isBrace(int ch) {
	return strchr("[]{}()", ch) != NULL;
}

static std::string getRange(GUI::ScintillaWindow *sw, int start, int end) {
	if (end <= start) return std::string();

	std::vector<char> buffer(end - start + 1);
	TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = buffer.data();

	sw->CallPointer(SCI_GETTEXTRANGE, 0, &tr);

	return std::string(buffer.begin(), buffer.end() - 1); // don't copy the null
}

static std::string getWordAt(GUI::ScintillaWindow *sw, int pos) {
	int word_start = sw->Call(SCI_WORDSTARTPOSITION, pos, true);
	int word_end = sw->Call(SCI_WORDENDPOSITION, pos, true);
	return getRange(sw, word_start, word_end);
}


static void setStyles(GUI::ScintillaWindow &sci, bool is_input) {
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

	// Only the input window needs the wordlists
	if (is_input) {
		sci.CallString(SCI_SETKEYWORDS, 0, "and break do else elseif end false for function goto if in local nil not or repeat return then true until while");
		sci.CallString(SCI_SETKEYWORDS, 1, "_ENV _G _VERSION assert collectgarbage dofile error getfenv getmetatable ipairs load loadfile loadstring module next pairs pcall print rawequal rawget rawlen rawset require select setfenv setmetatable tonumber tostring type unpack xpcall string table math bit32 coroutine io os debug package __index __newindex __call __add __sub __mul __div __mod __pow __unm __concat __len __eq __lt __le __gc __mode");
		sci.CallString(SCI_SETKEYWORDS, 2, "byte char dump find format gmatch gsub len lower rep reverse sub upper abs acos asin atan atan2 ceil cos cosh deg exp floor fmod frexp ldexp log log10 max min modf pow rad random randomseed sin sinh sqrt tan tanh arshift band bnot bor btest bxor extract lrotate lshift replace rrotate rshift shift string.byte string.char string.dump string.find string.format string.gmatch string.gsub string.len string.lower string.match string.rep string.reverse string.sub string.upper table.concat table.insert table.maxn table.pack table.remove table.sort table.unpack math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos math.cosh math.deg math.exp math.floor math.fmod math.frexp math.huge math.ldexp math.log math.log10 math.max math.min math.modf math.pi math.pow math.rad math.random math.randomseed math.sin math.sinh math.sqrt math.tan math.tanh bit32.arshift bit32.band bit32.bnot bit32.bor bit32.btest bit32.bxor bit32.extract bit32.lrotate bit32.lshift bit32.replace bit32.rrotate bit32.rshift");
		sci.CallString(SCI_SETKEYWORDS, 3, "close flush lines read seek setvbuf write clock date difftime execute exit getenv remove rename setlocale time tmpname coroutine.create coroutine.resume coroutine.running coroutine.status coroutine.wrap coroutine.yield io.close io.flush io.input io.lines io.open io.output io.popen io.read io.tmpfile io.type io.write io.stderr io.stdin io.stdout os.clock os.date os.difftime os.execute os.exit os.getenv os.remove os.rename os.setlocale os.time os.tmpname debug.debug debug.getfenv debug.gethook debug.getinfo debug.getlocal debug.getmetatable debug.getregistry debug.getupvalue debug.getuservalue debug.setfenv debug.sethook debug.setlocal debug.setmetatable debug.setupvalue debug.setuservalue debug.traceback debug.upvalueid debug.upvaluejoin package.cpath package.loaded package.loaders package.loadlib package.path package.preload package.seeall");

		// Highlight Scintilla constants
		std::string scicon;
		scicon.reserve(SciIFaceTable.constants.size() * 10);
		for (const auto &con : SciIFaceTable.constants) {
			scicon.append(con.name);
			scicon.append(" ");
		}
		sci.CallString(SCI_SETKEYWORDS, 4, scicon.c_str());

		// Highlight Notepad++ constants
		std::string nppcon;
		nppcon.reserve(NppIFaceTable.constants.size() * 10);
		for (const auto &con : NppIFaceTable.constants) {
			nppcon.append(con.name);
			nppcon.append(" ");
		}
		sci.CallString(SCI_SETKEYWORDS, 5, nppcon.c_str());
	}
}


void LuaConsole::setupInput(GUI::ScintillaWindow &sci) {
	// Have it actually do the lexing
	sci.Call(SCI_SETLEXER, SCLEX_LUA);

	// Set up the styles
	setStyles(sci, true);

	// Set up some autocomplete junk
	sci.Call(SCI_AUTOCSETIGNORECASE, true);
	sci.Call(SCI_AUTOCSETMAXHEIGHT, 8);
	sci.Call(SCI_AUTOCSETCANCELATSTART, false);

	sci.Call(SCI_INDICSETSTYLE, INDIC_BRACEHIGHLIGHT, INDIC_STRAIGHTBOX);
	sci.Call(SCI_INDICSETUNDER, INDIC_BRACEHIGHLIGHT, true);
	sci.Call(SCI_BRACEHIGHLIGHTINDICATOR, true, INDIC_BRACEHIGHLIGHT);

	m_sciInput = &sci;
}

void LuaConsole::setupOutput(GUI::ScintillaWindow &sci) {
	sci.Call(SCI_SETUNDOCOLLECTION, 0);
	sci.Call(SCI_SETREADONLY, 1);
	sci.Call(SCI_SETLEXER, SCLEX_NULL);

	// Set up the styles
	setStyles(sci, false);
}

bool LuaConsole::processNotification(const SCNotification *scn) {
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
			break;
		}
	}
	return true;
}

void LuaConsole::maintainIndentation() {
	int curPos = m_sciInput->Call(SCI_GETCURRENTPOS);
	int curLine = m_sciInput->Call(SCI_LINEFROMPOSITION, curPos);
	int prevIndent = m_sciInput->Call(SCI_GETLINEINDENTATION, curLine - 1);
	m_sciInput->Call(SCI_SETLINEINDENTATION, curLine, prevIndent);
	curPos = m_sciInput->Call(SCI_GETLINEINDENTPOSITION, curLine);
	m_sciInput->Call(SCI_SETEMPTYSELECTION, curPos);
}

void LuaConsole::braceMatch() {
	int curPos = m_sciInput->Call(SCI_GETCURRENTPOS);
	int bracePos = INVALID_POSITION;

	// Check on both sides
	if (isBrace(m_sciInput->Call(SCI_GETCHARAT, curPos - 1))) {
		bracePos = curPos - 1;
	}
	else if (isBrace(m_sciInput->Call(SCI_GETCHARAT, curPos))) {
		bracePos = curPos;
	}

	// See if we are next to a brace
	if (bracePos != INVALID_POSITION) {
		int otherPos = m_sciInput->Call(SCI_BRACEMATCH, bracePos, 0);
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
	int curPos = m_sciInput->Call(SCI_GETCURRENTPOS);
	int prevCh = m_sciInput->Call(SCI_GETCHARAT, curPos - 1);

	// The cursor could be at the end of a partial word e.g. editor.Sty|
	if (isalpha(prevCh)) {
		partialWord = getWordAt(m_sciInput, curPos - 1);

		// Back up past the partial word
		prevCh = m_sciInput->Call(SCI_GETCHARAT, curPos - 1 - partialWord.size());
		curPos = curPos - partialWord.size();
	}

	if (prevCh == '.' || prevCh == ':') {
		std::string prev = getWordAt(m_sciInput, curPos - 1);

		// TODO: all these are hard coded but they could either be pulled from the IFaceTable or some generated from Lua: for i, v in pairs(npp) do print(i, v) end
		if (prev.compare("npp") == 0) {
			if (prevCh == '.') {
				m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), "AddEventHandler AddShortcut AppDataPluginsAllowed BufferLangType ClearConsole ConstantName CurrentBufferID CurrentColumn CurrentLine CurrentView DefaultBackgroundColor DefaultForegroundColor LanguageDescription LanguageName RemoveAllEventHandlers RemoveEventHandler SendEditor Version WindowsVersion WriteError");
			}
			else if (prevCh == ':') {
				m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), "ActivateDoc DoOpen GetBufferIDFromPos GetCurrentDirectory GetCurrentDocIndex GetCurrentWord GetExtPart GetFileName GetFullCurrentPath GetNamePart GetNbOpenFiles GetNppDirectory GetPluginsConfigDir MenuCommand ReloadFile SaveAllFiles SaveCurrentFile SaveCurrentFileAs SwitchToFile");
			}
		}
		else if (prev.compare("editor") == 0 || prev.compare("editor1") == 0 || prev.compare("editor2") == 0 || prev.compare("console") == 0) {
			if (prevCh == '.') {
				m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), "AdditionalCaretFore AdditionalCaretsBlink AdditionalCaretsVisible AdditionalSelAlpha AdditionalSelBack AdditionalSelectionTyping AdditionalSelFore AllLinesVisible Anchor AnnotationLines AnnotationStyle AnnotationStyleOffset AnnotationStyles AnnotationText AnnotationVisible AutoCAutoHide AutoCCancelAtStart AutoCCaseInsensitiveBehaviour AutoCChooseSingle AutoCCurrent AutoCCurrentText AutoCDropRestOfWord AutoCFillUps AutoCIgnoreCase AutoCMaxHeight AutoCMaxWidth AutoCMulti AutoCOrder AutoCSeparator AutoCTypeSeparator AutomaticFold BackSpaceUnIndents BufferedDraw CallTipBack CallTipFore CallTipForeHlt CallTipPosition CallTipPosStart CallTipUseStyle CaretFore CaretLineBack CaretLineBackAlpha CaretLineVisible CaretLineVisibleAlways CaretPeriod CaretSticky CaretStyle CaretWidth CharacterPointer CharAt CodePage Column ControlCharSymbol CurrentPos Cursor DirectFunction DirectPointer DistanceToSecondaryStyles DocPointer EdgeColour EdgeColumn EdgeMode EndAtLastLine EndStyled EOLMode ExtraAscent ExtraDescent FirstVisibleLine Focus FoldExpanded FoldFlags FoldLevel FoldParent FontQuality GapPosition HighlightGuide HotspotActiveUnderline HotspotSingleLine HScrollBar Identifier Identifiers IdleStyling IMEInteraction Indent IndentationGuides IndicAlpha IndicatorCurrent IndicatorValue IndicFlags IndicFore IndicHoverFore IndicHoverStyle IndicOutlineAlpha IndicStyle IndicUnder KeyWords LayoutCache Length Lexer LexerLanguage LineCount LineEndPosition LineEndTypesActive LineEndTypesAllowed LineEndTypesSupported LineIndentation LineIndentPosition LinesOnScreen LineState LineVisible MainSelection MarginCursorN MarginLeft MarginMaskN MarginOptions MarginRight MarginSensitiveN MarginStyle MarginStyleOffset MarginStyles MarginText MarginTypeN MarginWidthN MarkerAlpha MarkerBack MarkerBackSelected MarkerFore MaxLineState ModEventMask Modify MouseDownCaptures MouseDwellTime MouseSelectionRectangularSwitch MultiPaste MultipleSelection Overtype PasteConvertEndings PhasesDraw PositionCache PrimaryStyleFromStyle PrintColourMode PrintMagnification PrintWrapMode Property PropertyExpanded PropertyInt PunctuationChars ReadOnly RectangularSelectionAnchor RectangularSelectionAnchorVirtualSpace RectangularSelectionCaret RectangularSelectionCaretVirtualSpace RectangularSelectionModifier Representation RGBAImageHeight RGBAImageScale RGBAImageWidth ScrollWidth ScrollWidthTracking SearchFlags SelAlpha SelectionEmpty SelectionEnd SelectionIsRectangle SelectionMode SelectionNAnchor SelectionNAnchorVirtualSpace SelectionNCaret SelectionNCaretVirtualSpace SelectionNEnd SelectionNStart Selections SelectionStart SelEOLFilled Status StyleAt StyleBack StyleBits StyleBitsNeeded StyleBold StyleCase StyleChangeable StyleCharacterSet StyleEOLFilled StyleFont StyleFore StyleFromSubStyle StyleHotSpot StyleItalic StyleSize StyleSizeFractional StyleUnderline StyleVisible StyleWeight SubStyleBases SubStylesLength SubStylesStart TabIndents TabWidth Tag TargetEnd TargetStart TargetText Technology TextLength TwoPhaseDraw UndoCollection UseTabs ViewEOL ViewWS VirtualSpaceOptions VScrollBar WhitespaceChars WhitespaceSize WordChars WrapIndentMode WrapMode WrapStartIndent WrapVisualFlags WrapVisualFlagsLocation XOffset Zoom");
			}
			else if (prevCh == ':') {
				m_sciInput->CallString(SCI_AUTOCSHOW, partialWord.size(), "AddRefDocument AddSelection AddStyledText AddTabStop AddText AddUndoAction Allocate AllocateExtendedStyles AllocateSubStyles AnnotationClearAll append AppendText AssignCmdKey AutoCActive AutoCCancel AutoCComplete AutoCPosStart AutoCSelect AutoCShow AutoCStops BackTab BeginUndoAction BraceBadLight BraceBadLightIndicator BraceHighlight BraceHighlightIndicator BraceMatch CallTipActive CallTipCancel CallTipPosStart CallTipSetHlt CallTipShow Cancel CanPaste CanRedo CanUndo ChangeInsertion ChangeLexerState CharLeft CharLeftExtend CharLeftRectExtend CharPositionFromPoint CharPositionFromPointClose CharRight CharRightExtend CharRightRectExtend ChooseCaretX Clear ClearAll ClearAllCmdKeys ClearCmdKey ClearDocumentStyle ClearRegisteredImages ClearRepresentation ClearSelections ClearTabStops Colourise ContractedFoldNext ConvertEOLs Copy CopyAllowLine CopyRange CopyText CountCharacters CreateDocument CreateLoader Cut DeleteBack DeleteBackNotLine DeleteRange DelLineLeft DelLineRight DelWordLeft DelWordRight DelWordRightEnd DescribeKeyWordSets DescribeProperty DocLineFromVisible DocumentEnd DocumentEndExtend DocumentStart DocumentStartExtend DropSelectionN EditToggleOvertype EmptyUndoBuffer EncodedFromUTF8 EndUndoAction EnsureVisible EnsureVisibleEnforcePolicy ExpandChildren FindColumn FindIndicatorFlash FindIndicatorHide FindIndicatorShow FindText findtext FoldAll FoldChildren FoldLine FormatRange FormFeed FreeSubStyles GetCurLine GetHotspotActiveBack GetHotspotActiveFore GetLastChild GetLine GetLineSelEndPosition GetLineSelStartPosition GetNextTabStop GetRangePointer GetSelText GetStyledText GetText GetTextRange GotoLine GotoPos GrabFocus HideLines HideSelection Home HomeDisplay HomeDisplayExtend HomeExtend HomeRectExtend HomeWrap HomeWrapExtend IndicatorAllOnFor IndicatorClearRange IndicatorEnd IndicatorFillRange IndicatorStart IndicatorValueAt insert InsertText IsRangeWord LineCopy LineCut LineDelete LineDown LineDownExtend LineDownRectExtend LineDuplicate LineEnd LineEndDisplay LineEndDisplayExtend LineEndExtend LineEndRectExtend LineEndWrap LineEndWrapExtend LineFromPosition LineLength LineScroll LineScrollDown LineScrollUp LinesJoin LinesSplit LineTranspose LineUp LineUpExtend LineUpRectExtend LoadLexerLibrary LowerCase MarginTextClearAll MarkerAdd MarkerAddSet MarkerDefine MarkerDefinePixmap MarkerDefineRGBAImage MarkerDelete MarkerDeleteAll MarkerDeleteHandle MarkerEnableHighlight MarkerGet MarkerLineFromHandle MarkerNext MarkerPrevious MarkerSymbolDefined match MoveCaretInsideView MoveSelectedLinesDown MoveSelectedLinesUp MultipleSelectAddEach MultipleSelectAddNext NewLine Null PageDown PageDownExtend PageDownRectExtend PageUp PageUpExtend PageUpRectExtend ParaDown ParaDownExtend ParaUp ParaUpExtend Paste PointXFromPosition PointYFromPosition PositionAfter PositionBefore PositionFromLine PositionFromPoint PositionFromPointClose PositionRelative PrivateLexerCall PropertyNames PropertyType Redo RegisterImage RegisterRGBAImage ReleaseAllExtendedStyles ReleaseDocument remove ReplaceSel ReplaceTarget ReplaceTargetRE RotateSelection ScrollCaret ScrollRange ScrollToEnd ScrollToStart SearchAnchor SearchInTarget SearchNext SearchPrev SelectAll SelectionDuplicate SetCharsDefault SetEmptySelection SetFoldMarginColour SetFoldMarginHiColour SetHotspotActiveBack SetHotspotActiveFore SetLengthForEncode SetSavePoint SetSel SetSelBack SetSelection SetSelFore SetStyling SetStylingEx SetTargetRange SetText SetVisiblePolicy SetWhitespaceBack SetWhitespaceFore SetXCaretPolicy SetYCaretPolicy ShowLines StartRecord StartStyling StopRecord StutteredPageDown StutteredPageDownExtend StutteredPageUp StutteredPageUpExtend StyleClearAll StyleResetDefault SwapMainAnchorCaret Tab TargetAsUTF8 TargetFromSelection TargetWholeDocument TextHeight textrange TextWidth ToggleCaretSticky ToggleFold Undo UpperCase UsePopUp UserListShow VCHome VCHomeDisplay VCHomeDisplayExtend VCHomeExtend VCHomeRectExtend VCHomeWrap VCHomeWrapExtend VerticalCentreCaret VisibleFromDocLine WordEndPosition WordLeft WordLeftEnd WordLeftEndExtend WordLeftExtend WordPartLeft WordPartLeftExtend WordPartRight WordPartRightExtend WordRight WordRightEnd WordRightEndExtend WordRightExtend WordStartPosition WrapCount ZoomIn ZoomOut");
			}
		}
	}
}
