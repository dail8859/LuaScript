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

#include "IFaceTable.h"
#include "Notepad_plus_msgs.h"

static IFaceConstant ifaceConstants[] = {
	{ "WV_95", WV_95 },
	{ "WV_98", WV_98 },
	{ "WV_ME", WV_ME },
	{ "WV_NT", WV_NT },
	{ "WV_S2003", WV_S2003 },
	{ "WV_UNKNOWN", WV_UNKNOWN },
	{ "WV_VISTA", WV_VISTA },
	{ "WV_W2K", WV_W2K },
	{ "WV_WIN32S", WV_WIN32S },
	{ "WV_WIN7", WV_WIN7 },
	{ "WV_WIN8", WV_WIN8 },
	{ "WV_WIN81", WV_WIN81 },
	{ "WV_XP", WV_XP },
	{ "WV_XPX64", WV_XPX64 },
};

static IFaceFunction ifaceFunctions[] = {
	{ "SaveCurrentFile", NPPM_SAVECURRENTFILE, iface_void, { iface_void, iface_void } }
};

static IFaceProperty ifaceProperties[] = {
	{ "CurrentLangType", NPPM_GETCURRENTLANGTYPE, NPPM_SETCURRENTLANGTYPE, iface_int, iface_void },
	{ "WindowsVersion", NPPM_GETWINDOWSVERSION, 0, iface_int, iface_void }
};

IFaceTable NppIFaceTable("NPPM_", ifaceFunctions, ELEMENTS(ifaceFunctions), ifaceConstants, ELEMENTS(ifaceConstants), ifaceProperties, ELEMENTS(ifaceProperties));
