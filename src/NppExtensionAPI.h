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
	enum Pane {
		paneEditorMain,
		paneEditorSecondary,
		paneOutput,
		paneInput,
		application
	};

	GUI::ScintillaWindow scis[4];

	NppExtensionAPI(ConsoleDialog *cd_, const NppData *nppData) : cd(cd_) { 
		m_nppData = nppData;

		scis[paneEditorMain].SetID(m_nppData->_scintillaMainHandle);
		scis[paneEditorSecondary].SetID(m_nppData->_scintillaSecondHandle);
		scis[paneOutput].SetID(cd->getSciOutputHwnd());
		scis[paneInput].SetID(cd->getSciInputHwnd());
	}

	~NppExtensionAPI();

	Pane getCurrentPane();

	sptr_t Send(Pane p, unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	char *Range(Pane p, intptr_t start, intptr_t end);
	void Remove(Pane p, intptr_t start, intptr_t end);
	void Insert(Pane p, intptr_t pos, const char *s);
	void SetTextDirection(Pane p, bool rtl);
	void Trace(const char *s);
	void TraceError(const char *s);
	void Tracef(const char *fmt, ...);
	void ClearConsole();
	std::string Property(const char *key);
	void SetProperty(const char *key, const char *val);
	void UnsetProperty(const char *key);
	uptr_t GetInstance();
	void ShutDown();
	void DoMenuCommand(int cmdID);

private:
	ConsoleDialog *cd;
	const NppData* m_nppData;
};
