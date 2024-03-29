// This file is part of LuaScript.
// 
// Original work Copyright 1998-2000 by Neil Hodgson <neilh@scintilla.org>
// Derived work Copyright (C)2016 Justin Dailey <dail8859@yahoo.com>
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

#include "NppExtensionAPI.h"

#include <vector>

struct LuaFuncItem {
	char _itemName[nbChar];
	ShortcutKey *_pShKey;
};

extern std::vector<LuaFuncItem> luaShortcuts;

class LuaExtension final {
private:
	LuaExtension(); // Singleton
	LuaExtension(const LuaExtension &);   // Disable copy ctor
	void operator=(const LuaExtension &); // Disable operator=

public:
	static LuaExtension &Instance();

	~LuaExtension();

	bool Initialise(NppExtensionAPI *host_);
	bool Finalise();

	// Helper methods
	bool RunString(const char *s, bool clearStack = true);
	bool RunFile(const wchar_t *filename);
	bool OnExecute(const char *s);
	std::vector<std::string> ExecuteAndReturnList(const char *s);
	std::string ExecuteAndReturnString(const char * s);
	std::vector<std::string> GetObjectAttributes(const char * object, bool onlyFuncs);
	std::string GetUserDataName(const char *object);
	void CallShortcut(int id);

	// Scintilla callbacks
	bool OnStyle(uintptr_t startPos, intptr_t lengthDoc, int initStyle, StyleWriter *styler);
	bool OnChar(const SCNotification *sc);
	bool OnSavePointReached(const SCNotification *sc);
	bool OnSavePointLeft(const SCNotification *sc);
	bool OnModifyAttemptRO(const SCNotification *sc);
	bool OnDoubleClick(const SCNotification *sc);
	bool OnUpdateUI(const SCNotification *sc);
	bool OnModification(const SCNotification *sc);
	bool OnMacroRecord(const SCNotification *sc);
	bool OnMarginClick(const SCNotification *sc);
	bool OnNeedShown(const SCNotification *sc);
	bool OnPainted(const SCNotification *sc);
	bool OnUserListSelection(const SCNotification *sc);
	bool OnDwellStart(const SCNotification *sc);
	bool OnDwellEnd(const SCNotification *sc);
	bool OnZoom(const SCNotification *sc);
	bool OnHotSpotClick(const SCNotification *sc);
	bool OnHotSpotDoubleClick(const SCNotification *sc);
	bool OnHotSpotReleaseClick(const SCNotification *sc);
	bool OnIndicatorClick(const SCNotification *sc);
	bool OnIndicatorRelease(const SCNotification *sc);
	bool OnCallTipClick(const SCNotification *sc);
	bool OnAutoCSelection(const SCNotification *sc);
	bool OnAutoCCancelled(const SCNotification *sc);
	bool OnAutoCCharDeleted(const SCNotification *sc);
	bool OnFocusIn(const SCNotification *sc);
	bool OnFocusOut(const SCNotification *sc);

	// Notepad++ callbacks
	bool OnReady();
	bool OnToolBarModification();
	bool OnBeforeClose(const char *filename, uptr_t bufferid);
	bool OnOpen(const char *filename, uptr_t bufferid);
	bool OnClose();
	bool OnBeforeOpen(const char *filename, uptr_t bufferid);
	bool OnBeforeSave(const char *filename, uptr_t bufferid);
	bool OnSave(const char *filename, uptr_t bufferid);
	bool OnShutdown();
	bool OnSwitchFile(const char *filename, uptr_t bufferid);
	bool OnLangChange();
	bool OnFileBeforeLoad();
	bool OnFileLoadFailed();
	bool OnReadOnlyChanged(const char *filename, uptr_t bufferid, int status);
	bool OnDocOrderChanged(const char *filename, uptr_t bufferid, int newIndex);
	bool OnSnapshotDirtyFileLoaded(const char *filename, uptr_t bufferid);
	bool OnBeforeShutdown();
	bool OnCancelShutdown();
	bool OnFileBeforeRename(const char *filename, uptr_t bufferid);
	bool OnFileRenameCancel(const char *filename, uptr_t bufferid);
	bool OnFileRenamed(const char *filename, uptr_t bufferid);
	bool OnFileBeforeDelete(const char *filename, uptr_t bufferid);
	bool OnFileDeleteFailed(const char *filename, uptr_t bufferid);
	bool OnFileDeleted();
};
