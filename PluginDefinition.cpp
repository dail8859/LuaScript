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

#include "PluginDefinition.h"
#include "Version.h"
#include "AboutDialog.h"

#include "LuaConsole.h"
#include "LuaExtension.h"
#include "NppExtensionAPI.h"
#include "WcharMbcsConverter.h"

// --- Local variables ---
static NppData nppData;
static SciFnDirect pSciMsg;			// For direct scintilla call
static sptr_t pSciWndData;			// For direct scintilla call
static HANDLE _hModule;				// For dialog initialization
static LuaConsole *g_console;
static bool isReady = false;

// --- Menu callbacks ---
static void showConsole();
static void editStartupScript();
static void runCurrentFile();
static void showAbout();

// --- Global variables ---
ShortcutKey sk = {true, false, false, '`'};
FuncItem funcItem[nbFunc] = {
	{TEXT("Show Console"),        showConsole,       0, false, NULL},
	{TEXT("Edit Startup Script"), editStartupScript, 0, false, NULL},
	{TEXT("Run Current File"),    runCurrentFile,    0, false, NULL},
	{TEXT(""),                    NULL,              0, false, NULL}, // separator
	{TEXT("About..."),            showAbout,         0, false, NULL}
};

inline LRESULT SendScintilla(UINT Msg, WPARAM wParam, LPARAM lParam) {
	return pSciMsg(pSciWndData, Msg, wParam, lParam);
}

inline LRESULT SendNpp(UINT Msg, WPARAM wParam, LPARAM lParam) {
	return SendMessage(nppData._nppHandle, Msg, wParam, lParam);
}

bool updateScintilla() {
	HWND curScintilla;

	// Get the current scintilla
	int which = -1;
	SendNpp(NPPM_GETCURRENTSCINTILLA, SCI_UNUSED, (LPARAM)&which);
	if(which == -1) return false;
	curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	// Get the function and pointer to it for more efficient calls
	pSciMsg = (SciFnDirect) SendMessage(curScintilla, SCI_GETDIRECTFUNCTION, 0, 0);
	pSciWndData = (sptr_t) SendMessage(curScintilla, SCI_GETDIRECTPOINTER, 0, 0);

	return true;
}

std::shared_ptr<char> getStartupScriptFilePath(wchar_t *buff, size_t size) {
	SendNpp(NPPM_GETPLUGINSCONFIGDIR, size, (LPARAM)buff);
	wcscat_s(buff, size, TEXT("\\"));
	wcscat_s(buff, size, TEXT("startup"));
	wcscat_s(buff, size, TEXT(".lua"));
	return WcharMbcsConverter::wchar2char(buff);
}

void pluginInit(HANDLE hModule) {
	_hModule = hModule;
}

void pluginCleanUp() {
}

void setNppInfo(NppData notepadPlusData) {
	nppData = notepadPlusData;
	g_console = new LuaConsole(nppData._nppHandle);
	g_console->init((HINSTANCE)_hModule, nppData);
	LuaExtension::Instance().Initialise(new NppExtensionAPI(g_console->mp_consoleDlg, &nppData));
}

// --- Menu call backs ---

void showConsole() {
	g_console->showDialog();
	// If the console is shown automatically at startup, don't give it focus
	if (isReady) g_console->mp_consoleDlg->giveInputFocus();
}

void editStartupScript() {
	wchar_t buff[MAX_PATH];
	SendNpp(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)buff);
	wcscat_s(buff, MAX_PATH, TEXT("\\"));
	wcscat_s(buff, MAX_PATH, TEXT("startup"));
	wcscat_s(buff, MAX_PATH, TEXT(".lua"));
	if (PathFileExists(buff) == 0) {
		const char *s = "-- Startup script\r\n-- Changes will take effect once Notepad++ is restarted\r\n\r\n";
		DWORD dwBytesWritten;
		HANDLE h = CreateFile(buff, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(h, s, strlen(s), &dwBytesWritten, NULL);
		CloseHandle(h);
	}
	SendNpp(NPPM_DOOPEN, 0, (LPARAM)buff);
}


static void runCurrentFile() {
	updateScintilla();
	const char *doc = (const char *)SendScintilla(SCI_GETCHARACTERPOINTER);
	if (LuaExtension::Instance().RunString(doc) == false) {
		g_console->showDialog();
	}
}

void showAbout() {
	CreateDialog((HINSTANCE) _hModule, MAKEINTRESOURCE(IDD_ABOUTDLG), nppData._nppHandle, abtDlgProc);
}

void handleNotification(SCNotification *notifyCode) {
	static TCHAR static_fname[MAX_PATH];
	TCHAR fname[MAX_PATH];
	NotifyHeader nh = notifyCode->nmhdr;

	switch(nh.code)
	{
	case SCN_CHARADDED:
		LuaExtension::Instance().OnChar(notifyCode->ch);
		break;
	case SCN_SAVEPOINTREACHED:
		LuaExtension::Instance().OnSavePointReached();
		break;
	case SCN_SAVEPOINTLEFT:
		LuaExtension::Instance().OnSavePointLeft();
		break;
	case NPPN_READY:
		isReady = true;
		// Run the startup script
		wchar_t buff[MAX_PATH];
		SendNpp(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)buff);
		wcscat_s(buff, MAX_PATH, TEXT("\\"));
		wcscat_s(buff, MAX_PATH, TEXT("startup"));
		wcscat_s(buff, MAX_PATH, TEXT(".lua"));
		if (LuaExtension::Instance().RunFile(WcharMbcsConverter::wchar2char(buff).get()) == false)
			g_console->showDialog();
		break;
	case NPPN_FILEBEFOREOPEN:
		SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
		LuaExtension::Instance().OnBeforeOpen(WcharMbcsConverter::wchar2char(fname).get());
		break;
	case NPPN_FILEOPENED:
		SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
		LuaExtension::Instance().OnOpen(WcharMbcsConverter::wchar2char(fname).get());
		break;
	case NPPN_BUFFERACTIVATED:
		SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
		LuaExtension::Instance().OnSwitchFile(WcharMbcsConverter::wchar2char(fname).get());
		break;
	case NPPN_FILEBEFORESAVE:
		SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
		LuaExtension::Instance().OnBeforeSave(WcharMbcsConverter::wchar2char(fname).get());
		break;
	case NPPN_FILESAVED:
		SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
		LuaExtension::Instance().OnSave(WcharMbcsConverter::wchar2char(fname).get());
		break;
	case NPPN_FILEBEFORECLOSE:
		SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)static_fname);
		LuaExtension::Instance().OnBeforeClose(WcharMbcsConverter::wchar2char(static_fname).get());
		break;
	case NPPN_FILECLOSED:
		// NOTE: cannot use idFrom to get the path since it is no longer valid
		LuaExtension::Instance().OnClose(WcharMbcsConverter::wchar2char(static_fname).get());
		break;
	case NPPN_SHUTDOWN:
		LuaExtension::Instance().Finalise();
		break;
	}
	return;
}