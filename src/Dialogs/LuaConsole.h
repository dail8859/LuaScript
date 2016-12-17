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

#pragma once

#include "ConsoleDialog.h"
#include "LuaExtension.h"
#include "PluginInterface.h"

#include "lua.hpp"

class LuaConsole final {
public:
	explicit LuaConsole(HWND hNotepad);

	~LuaConsole() {
		delete mp_consoleDlg;
		delete m_nppData;
		m_hNotepad = nullptr;
	}

	void init(HINSTANCE hInst, NppData& nppData) {
		mp_consoleDlg->initDialog(hInst, nppData, this);
		*m_nppData = nppData;
	}

	void showDialog() { mp_consoleDlg->doDialog(); }
	void runStatement(const char *statement) { LuaExtension::Instance().OnExecute(statement); }

	void setupInput(GUI::ScintillaWindow &sci);
	void setupOutput(GUI::ScintillaWindow &sci);

	bool processNotification(const SCNotification *scn);

	void showAutoCompletion();

	const std::string &getGlobalConstants() const { return globalConsts; }

	ConsoleDialog *mp_consoleDlg;
private:
	NppData* m_nppData;
	HWND m_hNotepad;

	// Autocomplete lists
	std::string sciProperties;
	std::string sciFunctions;
	std::string nppProperties;
	std::string nppFunctions;
	std::string globalConsts;

	GUI::ScintillaWindow *m_sciInput;

	void maintainIndentation();
	void braceMatch();
};

