#include "ConsoleDialog.h"
#include "Docking.h"
#include "Notepad_plus_msgs.h"
#include "PluginInterface.h"
#include "resource.h"
#include "SciLexer.h"
#include "Scintilla.h"
#include "WcharMbcsConverter.h"

#include <Commctrl.h>

#pragma comment(lib, "comctl32.lib") 

// Do this for now instead of including windowsx.h just for these two
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

ConsoleDialog::ConsoleDialog() :
	DockingDlgInterface(IDD_CONSOLE),
	m_console(NULL),
	m_prompt("> "),
	m_hTabIcon(NULL),
	m_currentHistory(0),
	m_hContext(NULL)
{
}

ConsoleDialog::ConsoleDialog(const ConsoleDialog& other) :
	DockingDlgInterface(other),
	m_data(other.m_data),
	m_console(other.m_console),
	m_prompt(other.m_prompt),
	m_hTabIcon(NULL),
	m_history(other.m_history),
	m_currentHistory(other.m_currentHistory),
	m_hContext(NULL)
{
}

ConsoleDialog::~ConsoleDialog()
{
	if (m_sciOutput.GetID())
	{
		::SendMessage(_hParent, NPPM_DESTROYSCINTILLAHANDLE, 0, reinterpret_cast<LPARAM>(m_sciOutput.GetID()));
		m_sciOutput.SetID(NULL);
	}

	if (m_sciInput.GetID())
	{
		::SendMessage(_hParent, NPPM_DESTROYSCINTILLAHANDLE, 0, reinterpret_cast<LPARAM>(m_sciInput.GetID()));
		m_sciInput.SetID(NULL);
	}

	if (m_hTabIcon)
	{
		::DestroyIcon(m_hTabIcon);
		m_hTabIcon = NULL;
	}

	if (m_hContext)
	{
		::DestroyMenu(m_hContext);
		m_hContext = NULL;
	}

	m_console = NULL;
}

void ConsoleDialog::initDialog(HINSTANCE hInst, NppData& nppData, ConsoleInterface* console)
{
	DockingDlgInterface::init(hInst, nppData._nppHandle);

	Window::init(hInst, nppData._nppHandle);
	createOutputWindow(nppData._nppHandle);
	createInputWindow(nppData._nppHandle);

	m_console = console;

	ZeroMemory(&m_data, sizeof(m_data));

	// Set up the context menu
	m_hContext = CreatePopupMenu();
	MENUITEMINFO mi;
	mi.cbSize = sizeof(mi);
	mi.fMask = MIIM_ID | MIIM_STRING;
	mi.fType = MFT_STRING;
	mi.fState = MFS_ENABLED;

	mi.wID = 1;
	mi.dwTypeData = _T("Select all");
	InsertMenuItem(m_hContext, 0, TRUE, &mi);

	mi.wID = 2;
	mi.dwTypeData = _T("Copy");
	InsertMenuItem(m_hContext, 1, TRUE, &mi);

	mi.wID = 3;
	mi.dwTypeData = _T("Clear");
	InsertMenuItem(m_hContext, 3, TRUE, &mi);
}

BOOL CALLBACK ConsoleDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
			SetParent((HWND)m_sciOutput.GetID(), _hSelf);
			ShowWindow((HWND)m_sciOutput.GetID(), SW_SHOW);
			SetParent((HWND)m_sciInput.GetID(), _hSelf);
			ShowWindow((HWND)m_sciInput.GetID(), SW_SHOW);

			// Subclass some stuff
			SetWindowSubclass((HWND)m_sciInput.GetID(), ConsoleDialog::inputWndProc, 0, reinterpret_cast<DWORD_PTR>(this));
			SetWindowSubclass((HWND)m_sciOutput.GetID(), ConsoleDialog::scintillaWndProc, 0, reinterpret_cast<DWORD_PTR>(this));
			return FALSE;
		case WM_SIZE:
			MoveWindow((HWND)m_sciOutput.GetID(), 0, 0, LOWORD(lParam), HIWORD(lParam) - 30, TRUE);
			MoveWindow(::GetDlgItem(_hSelf, IDC_PROMPT), 0, HIWORD(lParam)-25, 30, 25, TRUE);
			MoveWindow((HWND)m_sciInput.GetID(), 30, HIWORD(lParam) - 30, LOWORD(lParam) - 85, 25, TRUE);
			MoveWindow(::GetDlgItem(_hSelf, IDC_RUN), LOWORD(lParam) - 50, HIWORD(lParam) - 30, 50, 25, TRUE);  
			return FALSE;
		case WM_CONTEXTMENU:
			{
				MENUITEMINFO mi;
				mi.cbSize = sizeof(mi);
				mi.fMask = MIIM_STATE;

				mi.fState = m_sciOutput.Send(SCI_GETSELECTIONEMPTY) ? MFS_DISABLED : MFS_ENABLED;

				SetMenuItemInfo(m_hContext, 2, FALSE, &mi);

				// Thanks MS for corrupting the value of BOOL. :-/
				// From the documentation (http://msdn.microsoft.com/en-us/library/ms648002.aspx):
				//
				//    If you specify TPM_RETURNCMD in the uFlags parameter, the return value is the menu-item 
				//    identifier of the item that the user selected. If the user cancels the menu without making 
				//    a selection, or if an error occurs, then the return value is zero.
				INT cmdID = (INT)TrackPopupMenu(m_hContext, TPM_RETURNCMD, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, _hSelf, NULL);

				switch (cmdID)
				{
				case 1: // Select All
					m_sciOutput.Send(SCI_SELECTALL);
					break;
				case 2: // Copy
					m_sciOutput.Send(SCI_COPY);
					break;
				case 3: // Clear
					clearText();
					break;
				default:
					break;
				}
			}
			break;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDC_RUN)
			{
				runStatement();
				giveInputFocus();
				return FALSE;
			}
			break;
		default:
			break;
	}

	return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
}

void ConsoleDialog::historyPrevious() {
	if (m_currentHistory > 0) {
		if (m_currentHistory == m_history.size()) { // Yes, this is mean to be "=="
			m_curLine = WcharMbcsConverter::char2tchar((const char *)m_sciInput.Send(SCI_GETCHARACTERPOINTER)).get();
		}
		--m_currentHistory;

		m_sciInput.SendPointer(SCI_SETTEXT, 0, WcharMbcsConverter::tchar2char(m_history[m_currentHistory].c_str()).get());
		m_sciInput.Send(SCI_DOCUMENTEND);
		m_sciInput.Send(SCI_EMPTYUNDOBUFFER);
	}
}

void ConsoleDialog::historyNext() {
	if (m_history.empty()) return;

	if (m_currentHistory < m_history.size() - 1) {
		++m_currentHistory;
		m_sciInput.SendPointer(SCI_SETTEXT, 0, WcharMbcsConverter::tchar2char(m_history[m_currentHistory].c_str()).get());
	}
	else if (m_currentHistory == m_history.size() - 1) {
		++m_currentHistory;
		m_sciInput.SendPointer(SCI_SETTEXT, 0, WcharMbcsConverter::tchar2char(m_curLine.c_str()).get());
	}
	m_sciInput.Send(SCI_DOCUMENTEND);
	m_sciInput.Send(SCI_EMPTYUNDOBUFFER);
}

void ConsoleDialog::historyAdd(const TCHAR *line)
{
	if (line && line[0] && (m_history.empty() || line != m_history.back()))
		m_history.emplace_back(line);
	m_currentHistory = m_history.size();
	m_curLine.clear();
}

void ConsoleDialog::historyEnd()
{
	m_currentHistory = m_history.size();
	m_curLine.clear();
	m_sciInput.Send(SCI_CLEARALL);
	m_sciInput.Send(SCI_EMPTYUNDOBUFFER);
}

LRESULT CALLBACK ConsoleDialog::inputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	ConsoleDialog *cd = reinterpret_cast<ConsoleDialog *>(dwRefData);
	switch (uMsg) {
		case WM_GETDLGCODE:
			return DLGC_WANTARROWS | DLGC_WANTCHARS;
		case WM_KEYDOWN:
			switch (wParam) {
				case VK_UP:
					cd->historyPrevious();
					return FALSE;
				case VK_DOWN:
					cd->historyNext();
					return FALSE;
			}
		case WM_KEYUP:
			switch (wParam) {
				case VK_RETURN:
					cd->runStatement();
					return FALSE;
				case VK_ESCAPE:
					cd->historyEnd();
					return FALSE;
			}
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void ConsoleDialog::runStatement()
{
	assert(m_console != NULL);
	if (m_console)
	{
		Sci_TextRange tr;
		tr.chrg.cpMin = 0;
		tr.chrg.cpMax = m_sciInput.Send(SCI_GETLENGTH);
		tr.lpstrText = new char[2 * (tr.chrg.cpMax - tr.chrg.cpMin) + 2]; // See documentation
		m_sciInput.SendPointer(SCI_GETSTYLEDTEXT, 0, &tr);

		writeText(m_prompt.size(), m_prompt.c_str());
		m_sciOutput.Send(SCI_SETREADONLY, 0);
		m_sciOutput.SendPointer(SCI_ADDSTYLEDTEXT, 2 * (tr.chrg.cpMax - tr.chrg.cpMin), tr.lpstrText);
		m_sciOutput.Send(SCI_SETREADONLY, 1);
		writeText(2, "\r\n");

		delete[] tr.lpstrText;

		const char *text = (const char *)m_sciInput.Send(SCI_GETCHARACTERPOINTER);
		historyAdd(WcharMbcsConverter::char2tchar(text).get());
		m_console->runStatement(text);
		m_sciInput.Send(SCI_CLEARALL);
		m_sciInput.Send(SCI_EMPTYUNDOBUFFER);
	}
}

void ConsoleDialog::setPrompt(const char *prompt)
{
	m_prompt = prompt;
	// NOTE: This doesn't seem to work at all
	::SetWindowTextA(::GetDlgItem(_hSelf, IDC_PROMPT), prompt);
}

void ConsoleDialog::createOutputWindow(HWND hParentWindow)
{
	HWND sci = (HWND)::SendMessage(_hParent, NPPM_CREATESCINTILLAHANDLE, 0, reinterpret_cast<LPARAM>(hParentWindow));
	LONG currentStyle = GetWindowLong(sci, GWL_STYLE);
	SetWindowLong(sci, GWL_STYLE, currentStyle | WS_TABSTOP);

	m_sciOutput.SetID(sci);
	m_sciOutput.Send(SCI_SETUNDOCOLLECTION, 0);
	m_sciOutput.Send(SCI_SETREADONLY, 1);
	m_sciOutput.Send(SCI_USEPOPUP, 0);
	m_sciOutput.Send(SCI_SETLEXER, SCLEX_NULL);

	// Set up the styles
	setStyles((HWND)m_sciOutput.GetID());

	// Also add any additional styles
	m_sciOutput.Send(SCI_STYLESETFORE, 39, 0x0000FF);
}

void ConsoleDialog::createInputWindow(HWND hParentWindow) {
	HWND sci = (HWND)::SendMessage(_hParent, NPPM_CREATESCINTILLAHANDLE, 0, reinterpret_cast<LPARAM>(hParentWindow));
	LONG currentStyle = GetWindowLong(sci, GWL_STYLE);
	SetWindowLong(sci, GWL_STYLE, currentStyle | WS_TABSTOP);

	m_sciInput.SetID(sci);

	// Have it actually do the lexing
	m_sciInput.Send(SCI_SETLEXER, SCLEX_LUA);

	// Set up the styles
	setStyles((HWND)m_sciInput.GetID());

	m_sciInput.Send(SCI_SETHSCROLLBAR, 0);
}

void ConsoleDialog::setStyles(HWND sci) {
	SendMessage(sci, SCI_SETEOLMODE, SC_EOL_CRLF, 0);

	SendMessage(sci, SCI_STYLESETFORE, STYLE_DEFAULT, 0x000000);
	SendMessage(sci, SCI_STYLESETBACK, STYLE_DEFAULT, 0xFFFFFF);
	SendMessage(sci, SCI_STYLECLEARALL, 0, 0);

	// Remove all margins
	SendMessage(sci, SCI_SETMARGINWIDTHN, 0, 0);
	SendMessage(sci, SCI_SETMARGINWIDTHN, 1, 0);
	SendMessage(sci, SCI_SETMARGINWIDTHN, 2, 0);
	SendMessage(sci, SCI_SETMARGINWIDTHN, 3, 0);
	SendMessage(sci, SCI_SETMARGINWIDTHN, 4, 0);

	SendMessage(sci, SCI_SETMARGINLEFT, 0, 2);

	SendMessage(sci, SCI_SETCODEPAGE, 65001, 0);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_COMMENT, 0x008000);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_COMMENTLINE, 0x008000);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_COMMENTDOC, 0x808000);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_LITERALSTRING, 0x4A0095);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_PREPROCESSOR, 0x004080);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_WORD, 0xFF0000);
	SendMessage(sci, SCI_STYLESETBOLD, SCE_LUA_WORD, 1); // keywordClass="instre1"
	SendMessage(sci, SCI_SETKEYWORDS, 0, (LPARAM)"and break do else elseif end false for function goto if in local nil not or repeat return then true until while");
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_NUMBER, 0x0080FF);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_STRING, 0x808080);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_CHARACTER, 0x808080);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_OPERATOR, 0x800000);
	SendMessage(sci, SCI_STYLESETBOLD, SCE_LUA_OPERATOR, 1);
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_WORD2, 0xC08000);
	SendMessage(sci, SCI_STYLESETBOLD, SCE_LUA_WORD2, 1); // keywordClass="instre2"
	SendMessage(sci, SCI_SETKEYWORDS, 1, (LPARAM)"_ENV _G _VERSION assert collectgarbage dofile error getfenv getmetatable ipairs load loadfile loadstring module next pairs pcall print rawequal rawget rawlen rawset require select setfenv setmetatable tonumber tostring type unpack xpcall string table math bit32 coroutine io os debug package __index __newindex __call __add __sub __mul __div __mod __pow __unm __concat __len __eq __lt __le __gc __mode");
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_WORD3, 0xFF0080);
	SendMessage(sci, SCI_STYLESETBOLD, SCE_LUA_WORD3, 1); // keywordClass="type1"
	SendMessage(sci, SCI_SETKEYWORDS, 2, (LPARAM)"byte char dump find format gmatch gsub len lower rep reverse sub upper abs acos asin atan atan2 ceil cos cosh deg exp floor fmod frexp ldexp log log10 max min modf pow rad random randomseed sin sinh sqrt tan tanh arshift band bnot bor btest bxor extract lrotate lshift replace rrotate rshift shift string.byte string.char string.dump string.find string.format string.gmatch string.gsub string.len string.lower string.match string.rep string.reverse string.sub string.upper table.concat table.insert table.maxn table.pack table.remove table.sort table.unpack math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos math.cosh math.deg math.exp math.floor math.fmod math.frexp math.huge math.ldexp math.log math.log10 math.max math.min math.modf math.pi math.pow math.rad math.random math.randomseed math.sin math.sinh math.sqrt math.tan math.tanh bit32.arshift bit32.band bit32.bnot bit32.bor bit32.btest bit32.bxor bit32.extract bit32.lrotate bit32.lshift bit32.replace bit32.rrotate bit32.rshift");
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_WORD4, 0xA00000);
	SendMessage(sci, SCI_STYLESETBOLD, SCE_LUA_WORD4, 1);
	SendMessage(sci, SCI_STYLESETITALIC, SCE_LUA_WORD4, 1); // keywordClass="type2"
	SendMessage(sci, SCI_SETKEYWORDS, 3, (LPARAM)"close flush lines read seek setvbuf write clock date difftime execute exit getenv remove rename setlocale time tmpname coroutine.create coroutine.resume coroutine.running coroutine.status coroutine.wrap coroutine.yield io.close io.flush io.input io.lines io.open io.output io.popen io.read io.tmpfile io.type io.write io.stderr io.stdin io.stdout os.clock os.date os.difftime os.execute os.exit os.getenv os.remove os.rename os.setlocale os.time os.tmpname debug.debug debug.getfenv debug.gethook debug.getinfo debug.getlocal debug.getmetatable debug.getregistry debug.getupvalue debug.getuservalue debug.setfenv debug.sethook debug.setlocal debug.setmetatable debug.setupvalue debug.setuservalue debug.traceback debug.upvalueid debug.upvaluejoin package.cpath package.loaded package.loaders package.loadlib package.path package.preload package.seeall");
	SendMessage(sci, SCI_STYLESETFORE, SCE_LUA_LABEL, 0x008080);
	SendMessage(sci, SCI_STYLESETBOLD, SCE_LUA_LABEL, 1);
}

LRESULT CALLBACK ConsoleDialog::scintillaWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	// No idea what this does, but it seems to help a bit.
	if (uMsg == WM_GETDLGCODE) return DLGC_WANTARROWS | DLGC_WANTCHARS;
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void ConsoleDialog::writeText(size_t length, const char *text)
{
	m_sciOutput.Send(SCI_SETREADONLY, 0);
	m_sciOutput.Send(SCI_APPENDTEXT, length, (LPARAM)text);
	m_sciOutput.Send(SCI_SETREADONLY, 1);
	m_sciOutput.Send(SCI_DOCUMENTEND);
}

void ConsoleDialog::writeError(size_t length, const char *text)
{
	typedef struct {
		unsigned char c;
		unsigned char style;
	} cell;

	std::vector<cell> cells(length + 1);

	for (size_t i = 0; i < length; ++i) {
		cells[i].c = text[i];
		cells[i].style = 39;
	}
	cells[length].c = 0;
	cells[length].style = 0;

	m_sciOutput.Send(SCI_SETREADONLY, 0);
	m_sciOutput.Send(SCI_DOCUMENTEND); // make sure it's at the end
	m_sciOutput.Send(SCI_ADDSTYLEDTEXT, length * 2, (LPARAM)cells.data());
	m_sciOutput.Send(SCI_SETREADONLY, 1);
	m_sciOutput.Send(SCI_DOCUMENTEND);
}

void ConsoleDialog::doDialog()
{
	if (!isCreated())
	{
		//create(m_data);
		StaticDialog::create(_dlgID);

		m_hTabIcon = (HICON)::LoadImage(_hInst, MAKEINTRESOURCE(IDI_LUA), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT);
		m_data.hIconTab = m_hTabIcon;
		m_data.hClient = _hSelf;
		m_data.uMask = DWS_DF_CONT_BOTTOM | DWS_ICONTAB;
		m_data.pszName = TEXT("LuaScript");
		m_data.pszAddInfo = NULL;
		m_data.pszModuleName = TEXT("LuaScript.dll");
		m_data.dlgID = 0; // The index of the menu command to run
		m_data.iPrevCont = -1;

		::SendMessage(_hParent, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&m_data);
	}

	display(true);
}

void ConsoleDialog::hide()
{
	display(false);
}

void ConsoleDialog::clearText()
{
	m_sciOutput.Send(SCI_SETREADONLY, 0);
	m_sciOutput.Send(SCI_CLEARALL);
	m_sciOutput.Send(SCI_SETREADONLY, 1);
}
