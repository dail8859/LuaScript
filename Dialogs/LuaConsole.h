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

#ifndef LUACONSOLE_H
#define LUACONSOLE_H

#include "ConsoleInterface.h"
#include "ConsoleDialog.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static lua_State *L = 0;

class LuaConsole final : public ConsoleInterface
{
public:
	explicit LuaConsole(HWND hNotepad) :
		mp_consoleDlg(new ConsoleDialog()),
		m_hNotepad(hNotepad),
		m_nppData(new NppData)
	{
	}

	~LuaConsole() {
		delete mp_consoleDlg;
		delete m_nppData;
		m_hNotepad = nullptr;
	}

	void init(HINSTANCE hInst, NppData& nppData) {
		mp_consoleDlg->initDialog(hInst, nppData, this);
		*m_nppData = nppData;
	}

	void showDialog() {mp_consoleDlg->doDialog();}

	void runStatement(const char *statement) {};
	void stopStatement() {};
	void openFile(const char *filename, size_t lineNumber) {};

private:
	ConsoleDialog *mp_consoleDlg;
	NppData* m_nppData;
	HWND m_hNotepad;
};



#endif
