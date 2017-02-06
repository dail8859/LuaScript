// This file is part of LuaScript.
// 
// Copyright (C)2017 Justin Dailey <dail8859@yahoo.com>
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

#ifndef NPPLUASCRIPT_H
#define NPPLUASCRIPT_H

#define LS_START (WM_USER + 3500)

// Commands that can be sent to LuaScript

#define LS_EXECUTE_SCRIPT (LS_START + 0)
#define LS_EXECUTE_STATEMENT (LS_START + 1)

struct LuaScriptInfo
{
	// The structure's version. This must be 1.
	int structVersion;

	// Configurable flags
	int flags;

	// Script to run. This has different meaning based on the command.
	const char *script;
};

#endif
