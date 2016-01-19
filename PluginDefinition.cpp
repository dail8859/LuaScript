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
#include "Utils.h"
#include "Version.h"
#include "SettingsDialog.h"
#include "AboutDialog.h"

#include "LuaConsole.h"

// --- Local variables ---
static bool do_active_commenting;	// active commenting - create or extend a document block
//static bool do_active_wrapping;	// active wrapping - wrap text inside of document blocks...todo

static NppData nppData;
static SciFnDirect pSciMsg;			// For direct scintilla call
static sptr_t pSciWndData;			// For direct scintilla call
static SettingsDialog sd;			// The settings dialog
static HANDLE _hModule;				// For dialog initialization

// --- Menu callbacks ---
static void doxyItFunction();
static void doxyItFile();
static void activeCommenting();
//static void activeWrapping();
static void showSettings();
static void showAbout();

static LuaConsole *g_console;


// --- Global variables ---
//ShortcutKey sk = {true, true, true, 'D'};
FuncItem funcItem[nbFunc] = {
	//{TEXT("DoxyIt - Function"), doxyItFunction,   0, false, &sk},
	//{TEXT("DoxyIt - File"),     doxyItFile,       0, false, NULL},
	//{TEXT(""),                  NULL,             0, false, NULL}, // separator
	//{TEXT("Active commenting"), activeCommenting, 0, false, NULL},
	//{TEXT(""),                  NULL,             0, false, NULL}, // separator
	{TEXT("Settings..."),       showSettings,     0, false, NULL},
	{TEXT("About..."),          showAbout,        0, false, NULL}
};


inline LRESULT SendScintilla(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return pSciMsg(pSciWndData, Msg, wParam, lParam);
}

inline LRESULT SendNpp(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return SendMessage(nppData._nppHandle, Msg, wParam, lParam);
}

bool updateScintilla()
{
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

// --- Configuration ---

void getIniFilePath(wchar_t *iniPath, int size)
{
	SendNpp(NPPM_GETPLUGINSCONFIGDIR, size, (LPARAM) iniPath);
	wcscat_s(iniPath, size, TEXT("\\"));
	wcscat_s(iniPath, size, NPP_PLUGIN_NAME);
	wcscat_s(iniPath, size, TEXT(".ini"));
}

void configSave()
{
}

void configLoad()
{
}

void pluginInit(HANDLE hModule)
{
	_hModule = hModule;
}

void pluginCleanUp()
{
}

void setNppInfo(NppData notepadPlusData)
{
	nppData = notepadPlusData;

	sd.init((HINSTANCE) _hModule, nppData);
}


// --- Menu call backs ---

void showSettings()
{
	if(!updateScintilla()) return;
	sd.doDialog();
}

void showAbout()
{
	//CreateDialog((HINSTANCE) _hModule, MAKEINTRESOURCE(IDD_ABOUTDLG), nppData._nppHandle, abtDlgProc);
	g_console->showDialog();
}


// --- Notification callbacks ---


void handleNotification(SCNotification *notifyCode)
{
	static bool do_newline = false;
	NotifyHeader nh = notifyCode->nmhdr;
	int ch = notifyCode->ch;

	switch(nh.code)
	{
	case SCN_UPDATEUI:
		break;
	case SCN_CHARADDED:
		break;
	case NPPN_READY:
		g_console = new LuaConsole(nppData._nppHandle);
		g_console->init((HINSTANCE)_hModule, nppData);
		//g_console->initDialog((HINSTANCE)_hModule, nppData, NULL);
		break;
	case NPPN_SHUTDOWN:
		break;
	case NPPN_BUFFERACTIVATED:
	case NPPN_LANGCHANGED:
		break;
	}
	return;
}