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
#include "PluginInterface.h"

class StyleWriter;

inline sptr_t SptrFromPointer(void *p) {
	return reinterpret_cast<sptr_t>(p);
}

inline sptr_t SptrFromString(const char *cp) {
	return reinterpret_cast<sptr_t>(cp);
}

inline uptr_t UptrFromString(const char *cp) {
	return reinterpret_cast<uptr_t>(cp);
}


class NppExtensionAPI final {
public:
	enum Pane { paneEditor = 1, paneEditorMain = 2, paneEditorSecondary = 3, paneOutput = 4, paneFindOutput = 5 };

	NppExtensionAPI(ConsoleDialog *cd_, const NppData *nppData) : cd(cd_) { m_nppData = nppData; }
	~NppExtensionAPI();

	sptr_t Send(Pane p, unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	char *Range(Pane p, int start, int end);
	void Remove(Pane p, int start, int end);
	void Insert(Pane p, int pos, const char *s);
	void Trace(const char *s);
	std::string Property(const char *key);
	void SetProperty(const char *key, const char *val);
	void UnsetProperty(const char *key);
	uptr_t GetInstance();
	void ShutDown();
	void Perform(const char *actions);
	void DoMenuCommand(int cmdID);
	void UpdateStatusBar(bool bUpdateSlowData);
	void UserStripShow(const char *description);
	void UserStripSet(int control, const char *value);
	void UserStripSetList(int control, const char *value);
	const char *UserStripValue(int control);

private:
	ConsoleDialog *cd;
	const NppData* m_nppData;
	HWND getScintillaHandle(Pane p);
};

