// This file is part of LuaScript.
// 
// Original work Copyright (C)2010, 2011, 2014 Dave Brotherstone
// Derived work Copyright (C)2016 Justin Dailey <dail8859@yahoo.com>
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

#include "ConsoleDialog.h"
#include "resource.h"
#include "LuaConsole.h"

#include <Commctrl.h>

#pragma comment(lib, "comctl32.lib") 

// Do this for now instead of including windowsx.h just for these two
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

ConsoleDialog::ConsoleDialog() :
	m_console(NULL),
	m_prompt("> "),
	m_currentHistory(0),
	m_hContext(NULL) {}

ConsoleDialog::~ConsoleDialog() {
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

	if (m_data.hIconTab)
	{
		::DestroyIcon(m_data.hIconTab);
		m_data.hIconTab = NULL;
	}

	if (m_hContext)
	{
		::DestroyMenu(m_hContext);
		m_hContext = NULL;
	}

	m_console = NULL;
}

void ConsoleDialog::initDialog(HINSTANCE hInst, NppData& nppData, LuaConsole* console) {
	StaticDialog::init(hInst, nppData._nppHandle);

	m_console = console;

	createOutputWindow(nppData._nppHandle);
	createInputWindow(nppData._nppHandle);

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

INT_PTR CALLBACK ConsoleDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_INITDIALOG:
			SetParent((HWND)m_sciInput.GetID(), _hSelf);
			ShowWindow((HWND)m_sciInput.GetID(), SW_SHOW);
			SetParent((HWND)m_sciOutput.GetID(), _hSelf);
			ShowWindow((HWND)m_sciOutput.GetID(), SW_SHOW);

			// Subclass some stuff
			SetWindowSubclass((HWND)m_sciInput.GetID(), ConsoleDialog::inputWndProc, 0, reinterpret_cast<DWORD_PTR>(this));
			SetWindowSubclass((HWND)m_sciOutput.GetID(), ConsoleDialog::scintillaWndProc, 0, reinterpret_cast<DWORD_PTR>(this));
			return FALSE;
		case WM_SIZE: {
			RECT rect = { 0, 0, LOWORD(lParam), HIWORD(lParam) };
			int h = min(m_sciInput.Call(SCI_GETLINECOUNT), 8) * m_sciInput.Call(SCI_TEXTHEIGHT, 1);
			MoveWindow((HWND)m_sciOutput.GetID(), 0, 0, rect.right, rect.bottom - h - 14, TRUE);
			MoveWindow((HWND)m_sciInput.GetID(), 0, rect.bottom - h - 14, rect.right - 50, h + 9, TRUE);
			m_sciOutput.Call(SCI_DOCUMENTEND);

			MoveWindow(::GetDlgItem(_hSelf, IDC_RUN), rect.right - 50, rect.bottom - 30, 50, 25, TRUE);
			return FALSE;
		}
		case WM_CONTEXTMENU: {
				MENUITEMINFO mi;
				mi.cbSize = sizeof(mi);
				mi.fMask = MIIM_STATE;

				mi.fState = m_sciOutput.Call(SCI_GETSELECTIONEMPTY) ? MFS_DISABLED : MFS_ENABLED;

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
					m_sciOutput.Call(SCI_SELECTALL);
					break;
				case 2: // Copy
					m_sciOutput.Call(SCI_COPY);
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
			if (LOWORD(wParam) == IDC_RUN) {
				runStatement();
				giveInputFocus();
				return FALSE;
			}
			break;
		case WM_NOTIFY: {
			LPNMHDR nmhdr = reinterpret_cast<LPNMHDR>(lParam);
			if (m_sciInput.GetID() == nmhdr->hwndFrom) {
				SCNotification* scn = reinterpret_cast<SCNotification*>(lParam);
				m_console->processNotification(scn);

				switch (nmhdr->code) {
					case SCN_MODIFIED: {
						SCNotification* scn = reinterpret_cast<SCNotification*>(lParam);
						if ((scn->modificationType & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT))) {
							if (scn->linesAdded != 0) {
								RECT rect;
								GetClientRect(_hSelf, &rect);
								int h = min(m_sciInput.Call(SCI_GETLINECOUNT), 8) * m_sciInput.Call(SCI_TEXTHEIGHT, 1);
								MoveWindow((HWND)m_sciOutput.GetID(), 0, 0, rect.right, rect.bottom - h - 14, TRUE);
								MoveWindow((HWND)m_sciInput.GetID(), 0, rect.bottom - h - 14, rect.right - 50, h + 9, TRUE);
								m_sciOutput.Call(SCI_DOCUMENTEND);
							}

							// Not the most efficient way but by far the easiest to do it here
							int startLine = 0;
							int endLine = m_sciInput.Call(SCI_GETLINECOUNT);
							for (int i = startLine; i < endLine; ++i) {
								m_sciInput.CallString(SCI_MARGINSETTEXT, i, ">");
								m_sciInput.Call(SCI_MARGINSETSTYLE, i, STYLE_LINENUMBER);
							}
						}
						break;
					}
				}
			}
			else if (nmhdr->hwndFrom == _hParent) {
				switch (LOWORD(nmhdr->code)) {
					case DMN_CLOSE:
						updateConsoleCheckMark(false);
						break;
				}
			}
			break;
		}
		default:
			break;
	}

	return FALSE;
}

void ConsoleDialog::historyPrevious() {
	if (m_currentHistory > 0) {
		if (m_currentHistory == m_history.size()) { // Yes, this is mean to be "=="
			m_curLine = GUI::StringFromUTF8((const char *)m_sciInput.CallReturnPointer(SCI_GETCHARACTERPOINTER));
		}
		--m_currentHistory;

		m_sciInput.CallString(SCI_SETTEXT, 0, GUI::UTF8FromString(m_history[m_currentHistory]).c_str());
		m_sciInput.Call(SCI_DOCUMENTEND);
		m_sciInput.Call(SCI_EMPTYUNDOBUFFER);
		m_sciInput.Call(SCI_SCROLLTOEND);
	}
}

void ConsoleDialog::historyNext() {
	if (m_history.empty()) return;

	if (m_currentHistory < m_history.size() - 1) {
		++m_currentHistory;
		m_sciInput.CallString(SCI_SETTEXT, 0, GUI::UTF8FromString(m_history[m_currentHistory]).c_str());
	}
	else if (m_currentHistory == m_history.size() - 1) {
		++m_currentHistory;
		m_sciInput.CallString(SCI_SETTEXT, 0, GUI::UTF8FromString(m_curLine).c_str());
	}
	m_sciInput.Call(SCI_DOCUMENTEND);
	m_sciInput.Call(SCI_EMPTYUNDOBUFFER);
	m_sciInput.Call(SCI_SCROLLTOEND);
}

void ConsoleDialog::historyAdd(const TCHAR *line) {
	if (line && line[0] && (m_history.empty() || line != m_history.back()))
		m_history.emplace_back(line);
	m_currentHistory = m_history.size();
	m_curLine.clear();
}

void ConsoleDialog::historyEnd() {
	m_currentHistory = m_history.size();
	m_curLine.clear();
	m_sciInput.Call(SCI_EMPTYUNDOBUFFER); // Empty first incase it was a mistake
	m_sciInput.CallString(SCI_SETTEXT, 0, "");
}

void ConsoleDialog::updateConsoleCheckMark(bool check) const {
	if (cmdID != nullptr)
		SendMessage(_hParent, NPPM_SETMENUITEMCHECK, *cmdID, check);
}

LRESULT CALLBACK ConsoleDialog::inputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	ConsoleDialog *cd = reinterpret_cast<ConsoleDialog *>(dwRefData);
	switch (uMsg) {
		case WM_GETDLGCODE:
			return DLGC_WANTARROWS | DLGC_WANTCHARS | DLGC_WANTTAB;
		case WM_KEYDOWN:
			switch (wParam) {
				case VK_UP:
					if (cd->m_sciInput.Call(SCI_AUTOCACTIVE)) break; // Pass this along to the Scintilla control
					if (cd->m_sciInput.Call(SCI_GETLINECOUNT) == 1 || cd->m_sciInput.Call(SCI_GETCURRENTPOS) == 0) {
						cd->historyPrevious(); // Only go to the next one if the cursor is at the beginning of the text (or has a single line)
						return FALSE;
					}
					break;
				case VK_DOWN:
					if (cd->m_sciInput.Call(SCI_AUTOCACTIVE)) break; // Pass this along to the Scintilla control
					if (cd->m_sciInput.Call(SCI_GETLINECOUNT) == 1 || cd->m_sciInput.Call(SCI_GETCURRENTPOS) == cd->m_sciInput.Call(SCI_GETLENGTH)) {
						cd->historyNext(); // Only go to the next one if the cursor is at the end of the text (or has a single line)
						return FALSE;
					}
					break;
			}
			break;
		case WM_KEYUP:
			switch (wParam) {
				case VK_RETURN:
					if (cd->m_sciInput.Call(SCI_AUTOCACTIVE)) cd->m_sciInput.Call(SCI_AUTOCCOMPLETE);
					else if (GetKeyState(VK_SHIFT) & (1 << 15)) cd->m_sciInput.Call(SCI_NEWLINE);
					else cd->runStatement();
					return FALSE;
				case VK_ESCAPE:
					if (cd->m_sciInput.Call(SCI_AUTOCACTIVE)) cd->m_sciInput.Call(SCI_AUTOCCANCEL);
					else cd->historyEnd();
					return FALSE;
			}
			break;
		case WM_CHAR:
			if (wParam == VK_SPACE && GetKeyState(VK_CONTROL) & (1 << 15)) {
				cd->m_console->showAutoCompletion();
				return FALSE;
			}
			break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void ConsoleDialog::runStatement() {
	int prevLastLine = m_sciOutput.Call(SCI_GETLINECOUNT);
	int newLastLine = 0;

	Sci_TextRange tr;
	tr.chrg.cpMin = 0;
	tr.chrg.cpMax = m_sciInput.Call(SCI_GETLENGTH);
	tr.lpstrText = new char[2 * (tr.chrg.cpMax - tr.chrg.cpMin) + 2]; // See documentation
	m_sciInput.CallPointer(SCI_GETSTYLEDTEXT, 0, &tr);

	m_sciOutput.Call(SCI_DOCUMENTEND);
	m_sciOutput.Call(SCI_SETREADONLY, 0);
	m_sciOutput.CallString(SCI_ADDSTYLEDTEXT, 2 * (tr.chrg.cpMax - tr.chrg.cpMin), tr.lpstrText);
	writeText(2, "\r\n");
	m_sciOutput.Call(SCI_SETREADONLY, 1);

	delete[] tr.lpstrText;

	newLastLine = m_sciOutput.Call(SCI_GETLINECOUNT);

	for (int i = prevLastLine; i < newLastLine; ++i) {
		m_sciOutput.CallString(SCI_MARGINSETTEXT, i - 1, ">");
		m_sciOutput.Call(SCI_MARGINSETSTYLE, i - 1, STYLE_LINENUMBER);
	}

	const char *text = (const char *)m_sciInput.CallReturnPointer(SCI_GETCHARACTERPOINTER);
	historyAdd(GUI::StringFromUTF8(text).c_str());
	m_console->runStatement(text);
	m_sciInput.Call(SCI_CLEARALL);
	m_sciInput.Call(SCI_EMPTYUNDOBUFFER);
	m_sciInput.CallString(SCI_MARGINSETTEXT, 0, ">");
	m_sciInput.Call(SCI_MARGINSETSTYLE, 0, STYLE_LINENUMBER);
}

void ConsoleDialog::setPrompt(const char *prompt) {
	m_prompt = prompt;
	// NOTE: This doesn't seem to work at all
	//::SetWindowTextA(::GetDlgItem(_hSelf, IDC_PROMPT), prompt);
}

void ConsoleDialog::createOutputWindow(HWND hParentWindow) {
	HWND sci = (HWND)::SendMessage(_hParent, NPPM_CREATESCINTILLAHANDLE, 0, reinterpret_cast<LPARAM>(hParentWindow));
	SetWindowLongPtr(sci, GWL_STYLE, GetWindowLongPtr(sci, GWL_STYLE) | WS_TABSTOP);

	m_sciOutput.SetID(sci);

	m_sciOutput.Call(SCI_USEPOPUP, 0); 

	// Set up the styles
	m_console->setupOutput(m_sciOutput);

	// Also add any additional styles
	m_sciOutput.Call(SCI_STYLESETFORE, 39, 0x0000FF); // Red error message

	// Margin for the prompt
	m_sciOutput.Call(SCI_SETMARGINWIDTHN, 1, m_sciOutput.CallString(SCI_TEXTWIDTH, STYLE_DEFAULT, ">") * 2);
	m_sciOutput.Call(SCI_SETMARGINTYPEN, 1, SC_MARGIN_RTEXT);
	m_sciOutput.Call(SCI_STYLESETBOLD, STYLE_LINENUMBER, true);
}

void ConsoleDialog::createInputWindow(HWND hParentWindow) {
	HWND sci = (HWND)::SendMessage(_hParent, NPPM_CREATESCINTILLAHANDLE, 0, reinterpret_cast<LPARAM>(hParentWindow));
	SetWindowLongPtr(sci, GWL_STYLE, GetWindowLongPtr(sci, GWL_STYLE) | WS_TABSTOP);

	m_sciInput.SetID(sci);

	m_console->setupInput(m_sciInput);

	m_sciInput.Call(SCI_SETHSCROLLBAR, 0);

	// Margin for the prompt
	m_sciInput.Call(SCI_SETMARGINWIDTHN, 1, m_sciInput.CallString(SCI_TEXTWIDTH, STYLE_DEFAULT, ">") * 2);
	m_sciInput.Call(SCI_SETMARGINTYPEN, 1, SC_MARGIN_RTEXT);
	m_sciInput.Call(SCI_STYLESETBOLD, STYLE_LINENUMBER, true);

	m_sciInput.Call(SCI_SETMARGINLEFT, 0, 2);

	// Remove several command keys. This is not all inclusive but attempts to clear the 
	// obvious ones. Some insert ascii codes, others are not desired for the dialog box
	m_sciInput.Call(SCI_CLEARCMDKEY, 'Q' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'W' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'E' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'R' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'T' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'O' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'P' + (SCMOD_CTRL << 16));

	m_sciInput.Call(SCI_CLEARCMDKEY, 'S' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'D' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'F' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'G' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'H' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'J' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'K' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'L' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'B' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'N' + (SCMOD_CTRL << 16));
	m_sciInput.Call(SCI_CLEARCMDKEY, 'M' + (SCMOD_CTRL << 16));

	m_sciInput.Call(SCI_CLEARCMDKEY, SCK_RETURN); // don't allow normal new lines

	m_sciInput.CallString(SCI_MARGINSETTEXT, 0, ">");
	m_sciInput.Call(SCI_MARGINSETSTYLE, 0, STYLE_LINENUMBER);
}

LRESULT CALLBACK ConsoleDialog::scintillaWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	// No idea what this does, but it seems to help a bit.
	if (uMsg == WM_GETDLGCODE) return DLGC_WANTARROWS | DLGC_WANTCHARS;
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void ConsoleDialog::writeText(size_t length, const char *text) {
	m_sciOutput.Call(SCI_SETREADONLY, 0);
	m_sciOutput.CallString(SCI_APPENDTEXT, length, text);
	m_sciOutput.Call(SCI_SETREADONLY, 1);
	m_sciOutput.Call(SCI_DOCUMENTEND);
}

void ConsoleDialog::writeError(size_t length, const char *text) {
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

	m_sciOutput.Call(SCI_SETREADONLY, 0);
	m_sciOutput.Call(SCI_DOCUMENTEND); // make sure it's at the end
	m_sciOutput.CallString(SCI_ADDSTYLEDTEXT, length * 2, (const char *)cells.data());
	m_sciOutput.Call(SCI_SETREADONLY, 1);
	m_sciOutput.Call(SCI_DOCUMENTEND);
}

void ConsoleDialog::display(bool toShow) const {
	updateConsoleCheckMark(toShow);
	SendMessage(_hParent, toShow ? NPPM_DMMSHOW : NPPM_DMMHIDE, 0, reinterpret_cast<LPARAM>(_hSelf));
}

void ConsoleDialog::setCommandID(int *_cmdID) {
	 cmdID = _cmdID;

	 // Check it for the first time
	 if (isVisible())
		 updateConsoleCheckMark(true);
}

void ConsoleDialog::doDialog() {
	if (!isCreated()) {
		StaticDialog::create(IDD_CONSOLE);

		m_data.hIconTab = (HICON)::LoadImage(_hInst, MAKEINTRESOURCE(IDI_LUA), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT);
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

void ConsoleDialog::hide() {
	display(false);
}

void ConsoleDialog::clearText() {
	m_sciOutput.Call(SCI_SETREADONLY, 0);
	m_sciOutput.Call(SCI_CLEARALL);
	m_sciOutput.Call(SCI_SETSCROLLWIDTH, 1);
	m_sciOutput.Call(SCI_SETREADONLY, 1);
}
