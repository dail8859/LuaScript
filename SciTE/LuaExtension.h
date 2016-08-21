// SciTE - Scintilla based Text Editor
// LuaExtension.h - Lua scripting extension
// Copyright 1998-2000 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#pragma once

#include "NppExtensionAPI.h"

#include <vector>

struct LuaFuncItem
{
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
	bool Clear();
	bool Load(const char *filename);

	bool RunString(const char *s);
	bool RunFile(const wchar_t *filename);

	bool OnReady();
	bool OnBeforeOpen(const char *filename, uptr_t bufferid);
	bool OnOpen(const char *filename, uptr_t bufferid);
	bool OnSwitchFile(const char *filename, uptr_t bufferid);
	bool OnBeforeSave(const char *filename, uptr_t bufferid);
	bool OnSave(const char *filename, uptr_t bufferid);
	bool OnFileRenamed(const char *filename, uptr_t bufferid);
	bool OnFileDeleted(const char *filename, uptr_t bufferid);
	bool OnChar(char ch);
	bool OnExecute(const char *s);
	bool OnSavePointReached();
	bool OnSavePointLeft();
	bool OnStyle(unsigned int startPos, int lengthDoc, int initStyle, StyleWriter *styler);
	bool OnDoubleClick();
	bool OnUpdateUI(const SCNotification *sc);
	bool OnMarginClick();
	bool OnUserListSelection(int listType, const char *selection);
	bool OnKey(int keyval, int modifiers);
	bool OnDwellStart(int pos, const char *word);
	bool OnLangChange();
	bool OnBeforeClose(const char *filename, uptr_t bufferid);
	bool OnClose(const char *filename, uptr_t bufferid);
	bool OnBeforeShutdown();
	bool OnCancelShutdown();
	bool OnShutdown();
	bool OnModification(const SCNotification *sc);
	bool NeedsOnClose();

	void CallShortcut(int id);
};
