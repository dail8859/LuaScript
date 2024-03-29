﻿// This file is part of LuaScript.
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

#include <Windows.h>
#include <Shellapi.h>
#include <shlwapi.h>
#include <vector>

#include "PluginInterface.h"
#include "AboutDialog.h"
#include "LuaConsole.h"
#include "LuaExtension.h"
#include "NppExtensionAPI.h"
#include "GUI.h"
#include "StyleWriter.h"
#include "NppLuaScript.h"
#include "SciLexer.h"

// --- Menu callbacks ---
static void showConsole();
static void editStartupScript();
static void executeCurrentFile();
static void showDocumentation();
static void showAbout();

// --- Local variables ---
static NppData nppData;
static HWND curScintilla;
static SciFnDirect pSciMsg;			// For direct scintilla call
static sptr_t pSciWndData;			// For direct scintilla call
static HANDLE _hModule;				// For dialog initialization
static LuaConsole *g_console;
static bool isReady = false;
static std::vector<FuncItem> funcItems;
static toolbarIcons tbiConsoleButton;
static PFUNCPLUGINCMD LuaShortcutWrappers[] = {
	[]() { LuaExtension::Instance().CallShortcut(1); },
	[]() { LuaExtension::Instance().CallShortcut(2); },
	[]() { LuaExtension::Instance().CallShortcut(3); },
	[]() { LuaExtension::Instance().CallShortcut(4); },
	[]() { LuaExtension::Instance().CallShortcut(5); },
	[]() { LuaExtension::Instance().CallShortcut(6); },
	[]() { LuaExtension::Instance().CallShortcut(7); },
	[]() { LuaExtension::Instance().CallShortcut(8); },
	[]() { LuaExtension::Instance().CallShortcut(9); },
	[]() { LuaExtension::Instance().CallShortcut(10); },
	[]() { LuaExtension::Instance().CallShortcut(11); },
	[]() { LuaExtension::Instance().CallShortcut(12); },
	[]() { LuaExtension::Instance().CallShortcut(13); },
	[]() { LuaExtension::Instance().CallShortcut(14); },
	[]() { LuaExtension::Instance().CallShortcut(15); },
	[]() { LuaExtension::Instance().CallShortcut(16); },
	[]() { LuaExtension::Instance().CallShortcut(17); },
	[]() { LuaExtension::Instance().CallShortcut(18); },
	[]() { LuaExtension::Instance().CallShortcut(19); },
	[]() { LuaExtension::Instance().CallShortcut(20); },
	[]() { LuaExtension::Instance().CallShortcut(21); },
	[]() { LuaExtension::Instance().CallShortcut(22); },
	[]() { LuaExtension::Instance().CallShortcut(23); },
	[]() { LuaExtension::Instance().CallShortcut(24); },
	[]() { LuaExtension::Instance().CallShortcut(25); },
	[]() { LuaExtension::Instance().CallShortcut(26); },
	[]() { LuaExtension::Instance().CallShortcut(27); },
	[]() { LuaExtension::Instance().CallShortcut(28); },
	[]() { LuaExtension::Instance().CallShortcut(29); },
	[]() { LuaExtension::Instance().CallShortcut(30); },
	[]() { LuaExtension::Instance().CallShortcut(31); },
	[]() { LuaExtension::Instance().CallShortcut(32); },
	[]() { LuaExtension::Instance().CallShortcut(33); },
	[]() { LuaExtension::Instance().CallShortcut(34); },
	[]() { LuaExtension::Instance().CallShortcut(35); },
	[]() { LuaExtension::Instance().CallShortcut(36); },
	[]() { LuaExtension::Instance().CallShortcut(37); },
	[]() { LuaExtension::Instance().CallShortcut(38); },
	[]() { LuaExtension::Instance().CallShortcut(39); },
	[]() { LuaExtension::Instance().CallShortcut(40); },
	[]() { LuaExtension::Instance().CallShortcut(41); },
	[]() { LuaExtension::Instance().CallShortcut(42); },
	[]() { LuaExtension::Instance().CallShortcut(43); },
	[]() { LuaExtension::Instance().CallShortcut(44); },
	[]() { LuaExtension::Instance().CallShortcut(45); },
	[]() { LuaExtension::Instance().CallShortcut(46); },
	[]() { LuaExtension::Instance().CallShortcut(47); },
	[]() { LuaExtension::Instance().CallShortcut(48); },
	[]() { LuaExtension::Instance().CallShortcut(49); },
	[]() { LuaExtension::Instance().CallShortcut(50); },
	[]() { LuaExtension::Instance().CallShortcut(51); },
	[]() { LuaExtension::Instance().CallShortcut(52); },
	[]() { LuaExtension::Instance().CallShortcut(53); },
	[]() { LuaExtension::Instance().CallShortcut(54); },
	[]() { LuaExtension::Instance().CallShortcut(55); },
	[]() { LuaExtension::Instance().CallShortcut(56); },
	[]() { LuaExtension::Instance().CallShortcut(57); },
	[]() { LuaExtension::Instance().CallShortcut(58); },
	[]() { LuaExtension::Instance().CallShortcut(59); },
	[]() { LuaExtension::Instance().CallShortcut(60); },
	[]() { LuaExtension::Instance().CallShortcut(61); },
	[]() { LuaExtension::Instance().CallShortcut(62); },
	[]() { LuaExtension::Instance().CallShortcut(63); },
	[]() { LuaExtension::Instance().CallShortcut(64); },
	[]() { LuaExtension::Instance().CallShortcut(65); },
	[]() { LuaExtension::Instance().CallShortcut(66); },
	[]() { LuaExtension::Instance().CallShortcut(67); },
	[]() { LuaExtension::Instance().CallShortcut(68); },
	[]() { LuaExtension::Instance().CallShortcut(69); },
	[]() { LuaExtension::Instance().CallShortcut(70); },
	[]() { LuaExtension::Instance().CallShortcut(71); },
	[]() { LuaExtension::Instance().CallShortcut(72); },
	[]() { LuaExtension::Instance().CallShortcut(73); },
	[]() { LuaExtension::Instance().CallShortcut(74); },
	[]() { LuaExtension::Instance().CallShortcut(75); },
	[]() { LuaExtension::Instance().CallShortcut(76); },
	[]() { LuaExtension::Instance().CallShortcut(77); },
	[]() { LuaExtension::Instance().CallShortcut(78); },
	[]() { LuaExtension::Instance().CallShortcut(79); },
	[]() { LuaExtension::Instance().CallShortcut(80); },
	[]() { LuaExtension::Instance().CallShortcut(81); },
	[]() { LuaExtension::Instance().CallShortcut(82); },
	[]() { LuaExtension::Instance().CallShortcut(83); },
	[]() { LuaExtension::Instance().CallShortcut(84); },
	[]() { LuaExtension::Instance().CallShortcut(85); },
	[]() { LuaExtension::Instance().CallShortcut(86); },
	[]() { LuaExtension::Instance().CallShortcut(87); },
	[]() { LuaExtension::Instance().CallShortcut(88); },
	[]() { LuaExtension::Instance().CallShortcut(89); },
	[]() { LuaExtension::Instance().CallShortcut(90); },
	[]() { LuaExtension::Instance().CallShortcut(91); },
	[]() { LuaExtension::Instance().CallShortcut(92); },
	[]() { LuaExtension::Instance().CallShortcut(93); },
	[]() { LuaExtension::Instance().CallShortcut(94); },
	[]() { LuaExtension::Instance().CallShortcut(95); },
	[]() { LuaExtension::Instance().CallShortcut(96); },
	[]() { LuaExtension::Instance().CallShortcut(97); },
	[]() { LuaExtension::Instance().CallShortcut(98); },
	[]() { LuaExtension::Instance().CallShortcut(99); },
	[]() { LuaExtension::Instance().CallShortcut(100); },
};

static inline LRESULT SendScintilla(UINT Msg, WPARAM wParam = SCI_UNUSED, LPARAM lParam = SCI_UNUSED) {
	return pSciMsg(pSciWndData, Msg, wParam, lParam);
}

static inline LRESULT SendNpp(UINT Msg, WPARAM wParam = SCI_UNUSED, LPARAM lParam = SCI_UNUSED) {
	return SendMessage(nppData._nppHandle, Msg, wParam, lParam);
}

static bool updateScintilla() {
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

const wchar_t *getStartupScriptFilePath() {
	static wchar_t iniPath[MAX_PATH];
	SendNpp(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)iniPath);
	wcscat_s(iniPath, MAX_PATH, TEXT("\\"));
	wcscat_s(iniPath, MAX_PATH, TEXT("startup"));
	wcscat_s(iniPath, MAX_PATH, TEXT(".lua"));
	return iniPath;
}

bool fileIsInConfigDir() {
	wchar_t configDir[MAX_PATH];
	wchar_t filePath[MAX_PATH];

	SendNpp(NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM)filePath);
	SendNpp(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configDir);

	return _wcsnicmp(filePath, configDir, wcslen(configDir)) == 0;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  reasonForCall, LPVOID lpReserved) {
	switch (reasonForCall) {
		case DLL_PROCESS_ATTACH:
			_hModule = hModule;
			break;
		case DLL_PROCESS_DETACH:
			if (tbiConsoleButton.hToolbarBmp) ::DeleteObject(tbiConsoleButton.hToolbarBmp);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}

	return TRUE;
}

extern "C" __declspec(dllexport) void setInfo(NppData notepadPlusData) {
	nppData = notepadPlusData;

	// Initialize the dialogs
	g_console = new LuaConsole(nppData._nppHandle);
	g_console->init((HINSTANCE)_hModule, nppData);
	LuaExtension::Instance().Initialise(new NppExtensionAPI(g_console->mp_consoleDlg, &nppData));

	// Set up the short cuts that we know of
	funcItems.emplace_back(FuncItem{ TEXT("Show Console"), showConsole, 0, false, NULL });
	funcItems.emplace_back(FuncItem{ TEXT("Edit Startup Script"), editStartupScript, 0, false, NULL });
	funcItems.emplace_back(FuncItem{ TEXT("Execute Current File"), executeCurrentFile, 0, false, NULL });

	// Run the startup script
	const wchar_t *buff = getStartupScriptFilePath();

	if (LuaExtension::Instance().RunFile(buff) == true) {
		if (luaShortcuts.size() > 0) funcItems.emplace_back(FuncItem{ TEXT(""), NULL, 0, false, NULL });

		const int max_shortcuts = sizeof(LuaShortcutWrappers) / sizeof(LuaShortcutWrappers[0]);
		if (luaShortcuts.size() > max_shortcuts) {
			luaShortcuts.resize(max_shortcuts);
			MessageBox(NULL, TEXT("Too many Lua shortcuts have been registered.\r\nThis is an arbitrary limit and can be increased if you ask nicely on Github."), NPP_PLUGIN_NAME, MB_OK | MB_ICONERROR);
		}

		for (size_t i = 0; i < luaShortcuts.size(); ++i) {
			funcItems.emplace_back();
			_tcscpy_s(funcItems.back()._itemName, 64, GUI::StringFromUTF8(luaShortcuts[i]._itemName).c_str());
			funcItems.back()._pFunc = LuaShortcutWrappers[i];
			funcItems.back()._cmdID = 0;
			funcItems.back()._init2Check = false;
			funcItems.back()._pShKey = luaShortcuts[i]._pShKey;
		}

		luaShortcuts.clear(); // No longer need anything from it
	}
	else {
		// Something went wrong :(
		const char *c = "Error occurred while loading startup script\r\n";
		g_console->mp_consoleDlg->doDialog();
		g_console->mp_consoleDlg->writeError(strlen(c), c);
	}

	funcItems.emplace_back(FuncItem{ TEXT(""), NULL, 0, false, NULL });
	funcItems.emplace_back(FuncItem{ TEXT("Documentation..."), showDocumentation, 0, false, NULL });
	funcItems.emplace_back(FuncItem{ TEXT("About..."), showAbout, 0, false, NULL });
}

extern "C" __declspec(dllexport) const wchar_t *getName() {
	return NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem *getFuncsArray(int *nbF) {
	*nbF = static_cast<int>(funcItems.size());
	return funcItems.data();
}

extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode) {
	TCHAR fname[MAX_PATH];
	Sci_NotifyHeader nh = notifyCode->nmhdr;

	// Handle these specific codes since nh.hwndFrom comes from unknown hwnds
	// and *hope* these only come from the 2 "real" scintilla instances
	switch (nh.code) {
		case NPPN_READONLYCHANGED:
			// Note: hwndFrom is the bufferID
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, (WPARAM)nh.hwndFrom, (LPARAM)fname);
			LuaExtension::Instance().OnReadOnlyChanged(GUI::UTF8FromString(fname).c_str(), (uptr_t)nh.hwndFrom, (int)nh.idFrom);
			return;
		case NPPN_DOCORDERCHANGED:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnDocOrderChanged(GUI::UTF8FromString(fname).c_str(), nh.idFrom, static_cast<int>(reinterpret_cast<intptr_t>(nh.hwndFrom)));
			return;
		case NPPN_SNAPSHOTDIRTYFILELOADED:
			// Note: hwndFrom is NULL
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnSnapshotDirtyFileLoaded(GUI::UTF8FromString(fname).c_str(), (uptr_t)nh.idFrom);
			return;
	}

	// We only want notifications from Notepad++ and it's 2 scintilla handles
	if (nh.hwndFrom != nppData._nppHandle && nh.hwndFrom != nppData._scintillaMainHandle && nh.hwndFrom != nppData._scintillaSecondHandle)
		return;

	switch (nh.code) {
		case SCN_STYLENEEDED: {
			updateScintilla();
			// Copied from SciTE
			GUI::ScintillaWindow wEditor;
			wEditor.SetID(curScintilla);
			intptr_t lineEndStyled = wEditor.Call(SCI_LINEFROMPOSITION, wEditor.Call(SCI_GETENDSTYLED));
			intptr_t endStyled = wEditor.Call(SCI_POSITIONFROMLINE, lineEndStyled);
			StyleWriter styler(wEditor);
			int styleStart = 0;
			if (endStyled > 0) styleStart = styler.StyleAt(endStyled - 1);
			styler.SetCodePage(static_cast<int>(wEditor.Call(SCI_GETCODEPAGE)));
			LuaExtension::Instance().OnStyle(endStyled, notifyCode->position - endStyled, styleStart, &styler);
			styler.Flush();
			break;
		}
		case SCN_CHARADDED:
			LuaExtension::Instance().OnChar(notifyCode);
			break;
		case SCN_SAVEPOINTREACHED:
			LuaExtension::Instance().OnSavePointReached(notifyCode);
			break;
		case SCN_SAVEPOINTLEFT:
			LuaExtension::Instance().OnSavePointLeft(notifyCode);
			break;
		case SCN_MODIFYATTEMPTRO:
			LuaExtension::Instance().OnModifyAttemptRO(notifyCode);
			break;
		case SCN_DOUBLECLICK:
			LuaExtension::Instance().OnDoubleClick(notifyCode);
			break;
		case SCN_UPDATEUI:
			LuaExtension::Instance().OnUpdateUI(notifyCode);
			break;
		case SCN_MODIFIED:
			if (notifyCode->modificationType & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT))
				LuaExtension::Instance().OnModification(notifyCode);
			break;
		case SCN_MACRORECORD:
			// NOTE: do not call!
			//LuaExtension::Instance().OnMacroRecord(notifyCode);
			break;
		case SCN_MARGINCLICK:
			LuaExtension::Instance().OnMarginClick(notifyCode);
			break;
		case SCN_NEEDSHOWN:
			LuaExtension::Instance().OnNeedShown(notifyCode);
			break;
		case SCN_PAINTED:
			LuaExtension::Instance().OnPainted(notifyCode);
			break;
		case SCN_USERLISTSELECTION:
			LuaExtension::Instance().OnUserListSelection(notifyCode);
			break;
		case SCN_DWELLSTART:
			LuaExtension::Instance().OnDwellStart(notifyCode);
			break;
		case SCN_DWELLEND:
			LuaExtension::Instance().OnDwellEnd(notifyCode);
			break;
		case SCN_ZOOM:
			LuaExtension::Instance().OnZoom(notifyCode);
			break;
		case SCN_HOTSPOTCLICK:
			LuaExtension::Instance().OnHotSpotClick(notifyCode);
			break;
		case SCN_HOTSPOTDOUBLECLICK:
			LuaExtension::Instance().OnHotSpotDoubleClick(notifyCode);
			break;
		case SCN_HOTSPOTRELEASECLICK:
			LuaExtension::Instance().OnHotSpotReleaseClick(notifyCode);
			break;
		case SCN_INDICATORCLICK:
			LuaExtension::Instance().OnIndicatorClick(notifyCode);
			break;
		case SCN_INDICATORRELEASE:
			LuaExtension::Instance().OnIndicatorRelease(notifyCode);
			break;
		case SCN_CALLTIPCLICK:
			LuaExtension::Instance().OnCallTipClick(notifyCode);
			break;
		case SCN_AUTOCSELECTION:
			LuaExtension::Instance().OnAutoCSelection(notifyCode);
			break;
		case SCN_AUTOCCANCELLED:
			LuaExtension::Instance().OnAutoCCancelled(notifyCode);
			break;
		case SCN_AUTOCCHARDELETED:
			LuaExtension::Instance().OnAutoCCharDeleted(notifyCode);
			break;
		case SCN_FOCUSIN:
			LuaExtension::Instance().OnFocusIn(notifyCode);
			break;
		case SCN_FOCUSOUT:
			LuaExtension::Instance().OnFocusOut(notifyCode);
			break;

		// Notepad++ messages

		case NPPN_READY: {
			// Ensure it is using the correct version of Notepad++
			int version = static_cast<int>(SendNpp(NPPM_GETNPPVERSION));
			int major = (version >> 16) & 0xFF;
			int minor = version & 0xFF;

			if (major < 7 || (major == 7 && minor < 7)) {
				MessageBox(NULL, TEXT("This version of LuaScript should only be used with Notepad++ v7.7 and later."), NPP_PLUGIN_NAME, MB_OK | MB_ICONWARNING);
			}

			const char *msg = LUA_COPYRIGHT "\r\n\r\n";
			g_console->mp_consoleDlg->writeText(strlen(msg), msg);
			g_console->mp_consoleDlg->setCommandID(&funcItems[0]._cmdID);
			isReady = true;
			LuaExtension::Instance().OnReady();
			break;
		}
		case NPPN_TBMODIFICATION:
			tbiConsoleButton.hToolbarIcon = NULL;
			tbiConsoleButton.hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)_hModule, MAKEINTRESOURCE(IDI_LUABMP), IMAGE_BITMAP, 0, 0, (LR_SHARED | LR_LOADTRANSPARENT | LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS));
			SendNpp(NPPM_ADDTOOLBARICON, (WPARAM)funcItems[0]._cmdID, (LPARAM)&tbiConsoleButton);

			LuaExtension::Instance().OnToolBarModification();
			break;
		case NPPN_FILEBEFORECLOSE:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnBeforeClose(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_FILEOPENED:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnOpen(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_FILECLOSED:
			// Note: a valid BufferID is technically provided but it is not able to be used at all
			LuaExtension::Instance().OnClose();
			break;
		case NPPN_FILEBEFOREOPEN:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnBeforeOpen(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_FILEBEFORESAVE:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnBeforeSave(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_FILESAVED:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnSave(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_SHUTDOWN:
			LuaExtension::Instance().OnShutdown();
			LuaExtension::Instance().Finalise();
			break;
		case NPPN_BUFFERACTIVATED:
			// If opening the startup.lua file add the constants to a keyword list
			SendNpp(NPPM_GETFILENAME, MAX_PATH, (LPARAM)fname);
			if (SendNpp(NPPM_GETBUFFERLANGTYPE, nh.idFrom) == L_LUA && fileIsInConfigDir()) {
				updateScintilla();
				SendScintilla(SCI_SETKEYWORDS, 4, (LPARAM)g_console->getGlobalConstants().c_str());
				SendScintilla(SCI_STYLESETFORE, SCE_LUA_WORD5, SendScintilla(SCI_STYLEGETFORE, SCE_LUA_PREPROCESSOR));
				SendScintilla(SCI_STYLESETBOLD, SCE_LUA_WORD5, true);
			}

			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnSwitchFile(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_LANGCHANGED:
			LuaExtension::Instance().OnLangChange();
			break;
		case NPPN_WORDSTYLESUPDATED:
			break;
		case NPPN_SHORTCUTREMAPPED:
			break;
		case NPPN_FILEBEFORELOAD:
			LuaExtension::Instance().OnFileBeforeLoad();
			break;
		case NPPN_FILELOADFAILED:
			LuaExtension::Instance().OnFileLoadFailed();
			break;
		case NPPN_BEFORESHUTDOWN:
			LuaExtension::Instance().OnBeforeShutdown();
			break;
		case NPPN_CANCELSHUTDOWN:
			LuaExtension::Instance().OnCancelShutdown();
			break;
		case NPPN_FILEBEFORERENAME:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnFileBeforeRename(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_FILERENAMECANCEL:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnFileRenameCancel(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_FILERENAMED:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnFileRenamed(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_FILEBEFOREDELETE:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnFileBeforeDelete(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_FILEDELETEFAILED:
			SendNpp(NPPM_GETFULLPATHFROMBUFFERID, nh.idFrom, (LPARAM)fname);
			LuaExtension::Instance().OnFileDeleteFailed(GUI::UTF8FromString(fname).c_str(), nh.idFrom);
			break;
		case NPPN_FILEDELETED:
			LuaExtension::Instance().OnFileDeleted();
			break;
	}
	return;
}

// NOTE: this function's return value doesn't actually matter since N++ never uses it
extern "C" __declspec(dllexport) LRESULT messageProc(UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == NPPM_MSGTOPLUGIN) {
		auto ci = reinterpret_cast<const CommunicationInfo*>(lParam);
		auto lsi = reinterpret_cast<const LuaScriptInfo*>(ci->info);

		if (lsi && lsi->structVersion == 1) {
			if (ci->internalMsg == LS_EXECUTE_SCRIPT && lsi->script != nullptr) {
				LuaExtension::Instance().RunFile(GUI::StringFromUTF8(lsi->script).c_str());
			}
			else if (ci->internalMsg == LS_EXECUTE_STATEMENT && lsi->script != nullptr) {
				LuaExtension::Instance().RunString(lsi->script);
			}
		}
	}

	return TRUE;
}

extern "C" __declspec(dllexport) BOOL isUnicode() {
	return TRUE;
}

// --- Menu call backs ---

void showConsole() {
	if (g_console->mp_consoleDlg->isVisible())
		g_console->mp_consoleDlg->hide();
	else
		g_console->mp_consoleDlg->doDialog();

	// If the console is shown automatically at startup, don't give it focus
	if (isReady) g_console->mp_consoleDlg->giveInputFocus();
}

void editStartupScript() {
	const wchar_t *buff = getStartupScriptFilePath();

	if (PathFileExists(buff) == 0) {
		const char *s = "-- Startup script\r\n-- Changes will take effect once Notepad++ is restarted\r\n\r\n";
		DWORD dwBytesWritten;
		HANDLE h = CreateFile(buff, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(h, s, static_cast<DWORD>(strlen(s)), &dwBytesWritten, NULL);
		CloseHandle(h);
	}
	SendNpp(NPPM_DOOPEN, 0, (LPARAM)buff);
}

static void executeCurrentFile() {
	updateScintilla();
	const char *doc = (const char *)SendScintilla(SCI_GETCHARACTERPOINTER);
	if (LuaExtension::Instance().RunString(doc) == false) {
		g_console->mp_consoleDlg->doDialog();
	}
}

static void showDocumentation() {
	ShellExecute(nppData._nppHandle, TEXT("open"), TEXT("https://dail8859.github.io/LuaScript/"), NULL, NULL, SW_SHOWNORMAL);
}

static void showAbout() {
	ShowAboutDialog((HINSTANCE)_hModule, MAKEINTRESOURCE(IDD_ABOUTDLG), nppData._nppHandle);
}
