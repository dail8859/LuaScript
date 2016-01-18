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

#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H

#include <sstream>
#include "PluginInterface.h"

#define SCI_UNUSED 0

const wchar_t NPP_PLUGIN_NAME[] = TEXT("LuaScript");
const int nbFunc = 2;

// --- Helper functions ---
LRESULT SendScintilla(UINT Msg, WPARAM wParam=SCI_UNUSED, LPARAM lParam=SCI_UNUSED);
LRESULT SendNpp(UINT Msg, WPARAM wParam=SCI_UNUSED, LPARAM lParam=SCI_UNUSED);

// Calls from LuaScript.cpp
void pluginInit(HANDLE hModule);						// Called from DllMain, DLL_PROCESS_ATTACH
void pluginCleanUp();									// Called from DllMain, DLL_PROCESS_DETACH
void setNppInfo(NppData notepadPlusData);				// Called from setInfo()
void handleNotification(SCNotification *notifyCode);	// Called from beNotified()

#endif //PLUGINDEFINITION_H
