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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>

#include <string>
#include <vector>
#include <fstream>

#include "Scintilla.h"
#include "GUI.h"
#include "StyleWriter.h"
#include "NppExtensionAPI.h"
#include "LuaExtension.h"

#include "IFaceTableMixer.h"
#include "SciIFaceTable.h"
#include "NppIFaceTable.h"

#include "lua.hpp"

IFaceTableMixer ifacemixer;

// From lua.c
#define EOFMARK         "<eof>"
#define marklen         (sizeof(EOFMARK)/sizeof(char) - 1)

std::vector<LuaFuncItem> luaShortcuts;

const char *callbacks[] = {
	"OnStyle",
	"OnChar",
	"OnSavePointReached",
	"OnSavePointLeft",
	"OnDoubleClick",
	"OnUpdateUI",
	"OnModification", // Npp specific (text has been modified; added or deleted)
	//"OnMarginClick",
	//"OnUserListSelection",
	//"OnDwellStart",

	"OnReady", // Npp specific
	"OnBeforeOpen", // Npp specific
	"OnOpen",
	"OnSwitchFile",
	"OnBeforeSave",
	"OnSave",
	"OnFileRenamed", // Npp specific
	"OnFileDeleted", // Npp specific
	"OnLangChange", // Npp specific
	"OnBeforeClose", // Npp specific
	"OnClose",
	"OnBeforeShutdown", // Npp specific
	"OnCancelShutdown", // Npp specific
	"OnShutdown", // Npp specific
};


// Helper function from SciTE
static int Substitute(std::string &s, const std::string &sFind, const std::string &sReplace) {
	int c = 0;
	size_t lenFind = sFind.size();
	size_t lenReplace = sReplace.size();
	size_t posFound = s.find(sFind);
	while (posFound != std::string::npos) {
		s.replace(posFound, lenFind, sReplace);
		posFound = s.find(sFind, posFound + lenReplace);
		c++;
	}
	return c;
}

// Helper function from SciTE
static bool Exists(const char *fileName) {
	bool ret = false;
	if (fileName && fileName[0]) {
		FILE *fp = fopen(fileName, "rb");
		if (fp) {
			ret = true;
			fclose(fp);
		}
	}
	return ret;
}


// A note on naming conventions:
// I've gone back and forth on this a bit, trying different styles.
// It isn't easy to get something that feels consistent, considering
// that the Lua API uses lower case, underscore-separated words and
// Scintilla of course uses mixed case with no underscores.

// What I've settled on is that functions that require you to think
// about the Lua stack are likely to be mixed with Lua API functions,
// so these should using a naming convention similar to Lua itself.
// Functions that don't manipulate Lua at a low level should follow
// the normal SciTE convention.  There is some grey area of course,
// and for these I just make a judgement call


static NppExtensionAPI *host = 0;
static lua_State *luaState = 0;
static bool luaDisabled = false;
static std::string extensionScript;
static bool tracebackEnabled = true;

static int GetPropertyInt(const char *propName) {
	int propVal = 0;
	if (host) {
		std::string sPropVal = host->Property(propName);
		if (sPropVal.length()) {
			propVal = atoi(sPropVal.c_str());
		}
	}
	return propVal;
}

// Forward declarations
static NppExtensionAPI::Pane check_pane_object(lua_State *L, int index);
static void push_pane_object(lua_State *L, NppExtensionAPI::Pane p);
static int iface_function_helper(lua_State *L, const IFaceFunction &func);

inline void raise_error(lua_State *L, const char *errMsg=NULL) {
	luaL_where(L, 1);
	if (errMsg) {
		lua_pushstring(L, errMsg);
	} else {
		lua_insert(L, -2);
	}
	lua_concat(L, 2);
	lua_error(L);
}

inline void raise_ferror(lua_State *L, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	lua_pushvfstring(L, fmt, args);
	va_end(args);
	raise_error(L);
}

inline int absolute_index(lua_State *L, int index) {
	return ((index < 0) && (index != LUA_REGISTRYINDEX)) ? (lua_gettop(L) + index + 1) : index;
}

// Lua 5.1's checkudata throws an error on failure, we don't want that, we want NULL
static void *checkudata(lua_State *L, int ud, const char *tname) {
	void *p = lua_touserdata(L, ud);
	if (p != NULL) { // value is a userdata?
		if (lua_getmetatable(L, ud)) { // does it have a metatable?
			lua_getfield(L, LUA_REGISTRYINDEX, tname); // get correct metatable
			if (lua_rawequal(L, -1, -2)) { // does it have correct mt?
				lua_pop(L, 2);
				return p;
			}
			else {
				lua_pop(L, 2);
			}
		}
	}
	return NULL;
}

static int cf_npp_send(lua_State *L) {
	// This is reinstated as a replacement for the old <pane>:send, which was removed
	// due to safety concerns.  Is now exposed as npp.SendEditor / npp.SendOutput.
	// It is rewritten to be typesafe, checking the arguments against the metadata in
	// IFaceTable in the same way that the object interface does.

	IFaceFunction propfunc;
	int paneIndex = lua_upvalueindex(1);
	check_pane_object(L, paneIndex);
	int message = (int)luaL_checkinteger(L, 1);

	lua_pushvalue(L, paneIndex);
	lua_replace(L, 1);

	const IFaceFunction *func;
	func = SciIFaceTable.GetFunctionByMessage(message);

	if (func == nullptr) {
		propfunc = SciIFaceTable.GetPropertyFuncByMessage(message);
		if (propfunc.value != -1) func = &propfunc;
	}

	if (func != nullptr) {
		if (IFaceFunctionIsScriptable(*func)) {
			return iface_function_helper(L, *func);
		} else {
			raise_error(L, "Cannot call send for this function: not scriptable.");
			return 0;
		}
	} else {
		raise_error(L, "Message number does not match any published Scintilla / Notepad++ function or property");
		return 0;
	}
}

static int cf_npp_constname(lua_State *L) {
	char constName[100] = "";
	const char *hint = nullptr;
	int message = (int)luaL_checkinteger(L, 1);

	hint = luaL_optstring(L, 2, nullptr);

	if (ifacemixer.GetConstantName(message, constName, 100, hint) > 0) {
		lua_pushstring(L, constName);
		return 1;
	} else {
		raise_error(L, "Argument does not match any Scintilla / Notepad++ constant");
		return 0;
	}
}

void stackdump(lua_State* l)
{
	int i;
	int top = lua_gettop(l);

	for (i = 1; i <= top; i++)
	{  /* repeat for each level */
		int t = lua_type(l, i);
		host->Trace(lua_typename(l, t));
		host->Trace("  ");  /* put a separator */
	}
	host->Trace("\r\n");  /* end the listing */
}

static bool isValidCallback(const char *cb) {
	for (int i = 0; i < ELEMENTS(callbacks); ++i) {
		if (strcmp(callbacks[i], cb) == 0) return true;
	}
	return false;
}

static int cf_npp_add_callback(lua_State *L) {
	const char *callback = luaL_checkstring(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	if (!isValidCallback(callback)) {
		lua_pushfstring(L, "Unrecognized callback name '%s'", callback);
		raise_error(L);
	}

	lua_pushliteral(L, "Npp_Callbacks");
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_getfield(L, -1, callback);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1); // the nil value
		lua_newtable(L);
		lua_setfield(L, -2, callback);
		lua_getfield(L, -1, callback); // get the table back on top of the stack
	}

	// Get the length of the table
	size_t len = lua_rawlen(L, -1);

	lua_pushvalue(L, -3); // copy the callback function to the top of the stack
	lua_seti(L, -2, len + 1);

	lua_pushboolean(L, 1);
	return 1;
}

static int cf_npp_remove_callback(lua_State *L) {
	const char *callback = luaL_checkstring(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	int idx = -1;

	if (!isValidCallback(callback)) {
		lua_pushfstring(L, "Unrecognized callback name '%s'", callback);
		raise_error(L);
	}

	lua_pushliteral(L, "Npp_Callbacks");
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_getfield(L, -1, callback);
	if (lua_isnil(L, -1)) {
		// There haven't been any callbacks registered yet
		lua_pushboolean(L, 0);
		return 1;
	}

	// Iterate the callback table to see if the function is registered
	lua_pushnil(L);
	while (lua_next(luaState, -2) != 0) {
		if (lua_rawequal(L, -1, 2) == 1) {
			idx = (int)lua_tointeger(L, -2);
			// remove the key and value
			lua_pop(L, 2);
			break;
		}
		// remove the value; keep 'key' for next iteration
		lua_pop(L, 1);
	}

	if (idx != -1) {
		lua_getglobal(L, "table");
		lua_getfield(L, -1, "remove");
		lua_remove(L, -2);
		lua_pushvalue(L, -2); // the callback table
		lua_pushinteger(L, idx);
		lua_call(L, 2, 0); // call "table.remove(callback, idx)" and ignore return

		lua_pushboolean(L, 1); // Yay it was removed
	}
	else {
		lua_pushboolean(L, 0); // Not found, oh well
	}

	return 1;
}

static int cf_npp_removeall_callbacks(lua_State *L) {
	const char *callback = luaL_checkstring(L, 1);

	if (!isValidCallback(callback)) {
		lua_pushfstring(L, "Unrecognized callback name '%s'", callback);
		raise_error(L);
	}

	lua_pushliteral(L, "Npp_Callbacks");
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_getfield(L, -1, callback);
	if (lua_istable(L, -1)) {
		lua_pushstring(L, callback);
		lua_pushnil(L);
		lua_settable(L, -4);
	}

	return 0;
}

static int cf_npp_add_shortcut(lua_State *L) {
	ShortcutKey *sk = NULL;
	const char *name = luaL_checkstring(L, 1);
	const char *shortcut = luaL_optstring(L, 2, NULL);
	luaL_checktype(L, 3, LUA_TFUNCTION);

	// Try to parse the string into a shortcut structure
	if (shortcut && shortcut[0] != '\0') {
		const char *cur = shortcut;
		sk = new ShortcutKey({ false, false, false, 0 });
		do {
			if (_strnicmp(cur, "Ctrl+", 5) == 0) {
				if (sk->_isCtrl) raise_ferror(L, "Invalid shortcut key \"%s\"", shortcut);
				cur += 5;
				sk->_isCtrl = true;
			}
			else if (_strnicmp(cur, "Alt+", 4) == 0) {
				if (sk->_isAlt) raise_ferror(L, "Invalid shortcut key \"%s\"", shortcut);
				cur += 4;
				sk->_isAlt = true;
			}
			else if (_strnicmp(cur, "Shift+", 6) == 0) {
				if (sk->_isShift) raise_ferror(L, "Invalid shortcut key \"%s\"", shortcut);
				cur += 6;
				sk->_isShift = true;
			}
			else {
				// A - Z, 0 - 9
				if (isalnum(toupper(cur[0])) && cur[1] == NULL) {
					sk->_key = toupper(*cur++);
				}
				// F1 - F12
				else if (toupper(cur[0]) == 'F' && strlen(cur) <= 3) {
					char *endptr;
					cur++;
					long int num = strtol(cur, &endptr, 10);

					// Make Sure it was:
					//  An actual number
					//  Didn't start with 0 (e.g. 05)
					//  Is between 1 and 12
					if (endptr != cur && cur[0] != '0' && num >= 1 && num <= 12) sk->_key = 0x6F + static_cast<UCHAR>(num);
					else raise_ferror(L, "Invalid shortcut key \"%s\"", shortcut);;
				}
				// Other keys
				else if (_stricmp(cur, ";") == 0) sk->_key = VK_OEM_1;
				else if (_stricmp(cur, "/") == 0) sk->_key = VK_OEM_2;
				else if (_stricmp(cur, "~") == 0) sk->_key = VK_OEM_3;
				else if (_stricmp(cur, "[") == 0) sk->_key = VK_OEM_4;
				else if (_stricmp(cur, "\\") == 0) sk->_key = VK_OEM_5;
				else if (_stricmp(cur, "]") == 0) sk->_key = VK_OEM_6;
				else if (_stricmp(cur, "\'") == 0) sk->_key = VK_OEM_7;
				else if (_stricmp(cur, ",") == 0) sk->_key = VK_OEM_COMMA;
				else if (_stricmp(cur, "-") == 0) sk->_key = VK_OEM_MINUS;
				else if (_stricmp(cur, ".") == 0) sk->_key = VK_OEM_PERIOD;
				else if (_stricmp(cur, "=") == 0) sk->_key = VK_OEM_PLUS;
				else if (_stricmp(cur, "up") == 0) sk->_key = VK_UP;
				else if (_stricmp(cur, "down") == 0) sk->_key = VK_DOWN;
				else if (_stricmp(cur, "left") == 0) sk->_key = VK_LEFT;
				else if (_stricmp(cur, "right") == 0) sk->_key = VK_RIGHT;
				else if (_stricmp(cur, "space") == 0) sk->_key = VK_SPACE;
				else if (_stricmp(cur, "pageup") == 0) sk->_key = VK_PRIOR;
				else if (_stricmp(cur, "pagedown") == 0) sk->_key = VK_NEXT;
				else if (_stricmp(cur, "backspace") == 0) sk->_key = VK_BACK;
				else if (_stricmp(cur, "delete") == 0) sk->_key = VK_DELETE;
				else if (_stricmp(cur, "escape") == 0) sk->_key = VK_ESCAPE;
				// These appear to get registered but can't be called from N++
				//else if (_stricmp(cur, "enter") == 0) sk->_key = VK_RETURN;
				//else if (_stricmp(cur, "insert") == 0) sk->_key = VK_INSERT;
				// Numpad numbers
				//else if (_strnicmp(cur, "Numpad", 6) == 0) {
				//	cur += 6;
				//	if (isdigit(cur[0]) && cur[1] == NULL) sk->_key = VK_NUMPAD0 + atoi(cur);
				//	else raise_error(L, "Cannot parse shortcut key");
				//}
				else raise_ferror(L, "Invalid shortcut key \"%s\"", shortcut);

				// Each if block makes sure the string has been completely consumed, else an 
				// error will be raised by this point so we can go ahead and stop.
				break;
			}
		} while (*cur != NULL);
		// Do a quick sanity check?
	}

	lua_pushliteral(L, "Npp_Shortcuts");
	lua_gettable(L, LUA_REGISTRYINDEX);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1); // the nil value
		lua_newtable(L);
		lua_setfield(L, LUA_REGISTRYINDEX, "Npp_Shortcuts");
		lua_getfield(L, LUA_REGISTRYINDEX, "Npp_Shortcuts"); // get the table back on top of the stack
	}

	// Get the length of the table
	size_t len = lua_rawlen(L, -1);
	lua_pushvalue(L, 3); // the function
	lua_seti(L, -2, len + 1);

	luaShortcuts.emplace_back();
	strncpy(luaShortcuts.back()._itemName, name, nbChar);
	luaShortcuts.back()._pShKey = sk;

	return 0;
}

static int cf_npp_write_error(lua_State *L) {
	int nargs = lua_gettop(L);

	lua_getglobal(L, "tostring");

	for (int i = 1; i <= nargs; ++i) {
		if (i > 1)
			host->TraceError("\t");

		const char *argStr = lua_tostring(L, i);
		if (argStr) {
			host->TraceError(argStr);
		}
		else {
			lua_pushvalue(L, -1); // tostring
			lua_pushvalue(L, i);
			lua_call(L, 1, 1);
			argStr = lua_tostring(L, -1);
			if (argStr) {
				host->TraceError(argStr);
			}
			else {
				raise_error(L, "tostring (called from print) returned a non-string");
			}
			lua_settop(L, nargs + 1);
		}
	}

	host->TraceError("\r\n");
	return 0;
}

static int cf_npp_clear_console(lua_State *L) {
	host->ClearConsole();
	return 0;
}

static NppExtensionAPI::Pane check_pane_object(lua_State *L, int index) {
	NppExtensionAPI::Pane *pPane = static_cast<NppExtensionAPI::Pane *>(checkudata(L, index, "Npp_MT_Pane"));

	if ((!pPane) && lua_istable(L, index)) {
		// so that nested objects have a convenient way to do a back reference
		int absIndex = absolute_index(L, index);
		lua_pushliteral(L, "pane");
		lua_gettable(L, absIndex);
		pPane = static_cast<NppExtensionAPI::Pane *>(checkudata(L, -1, "Npp_MT_Pane"));
	}

	if (pPane) {
		return *pPane;
	}

	pPane = static_cast<NppExtensionAPI::Pane *>(checkudata(L, index, "Npp_MT_Application"));

	// NOTE: I'm not sure what the above comment about the "back reference" means. This may or
	// may not apply in thise case. So that if statement may need pasted/modified in this case

	if (pPane) {
		return *pPane;
	}

	if (index == 1)
		lua_pushliteral(L, "Self object is missing in pane method or property access.");
	else if (index == lua_upvalueindex(1))
		lua_pushliteral(L, "Internal error: pane object expected in closure.");
	else
		lua_pushliteral(L, "Pane object expected.");

	raise_error(L);
	return host->paneOutput; // this line never reached
}

static int cf_pane_textrange(lua_State *L) {
	NppExtensionAPI::Pane p = check_pane_object(L, 1);

	if (lua_gettop(L) >= 3) {
		int cpMin = static_cast<int>(luaL_checkinteger(L, 2));
		int cpMax = static_cast<int>(luaL_checkinteger(L, 3));

		if (cpMax >= 0) {
			char *range = host->Range(p, cpMin, cpMax);
			if (range) {
				lua_pushstring(L, range);
				delete []range;
				return 1;
			}
		} else {
			raise_error(L, "Invalid argument 2 for <pane>:textrange.  Positive number or zero expected.");
		}
	} else {
		raise_error(L, "Not enough arguments for <pane>:textrange");
	}

	return 0;
}

static int cf_pane_insert(lua_State *L) {
	NppExtensionAPI::Pane p = check_pane_object(L, 1);
	int pos = (int)luaL_checkinteger(L, 2);
	const char *s = luaL_checkstring(L, 3);
	host->Insert(p, pos, s);
	return 0;
}

static int cf_pane_remove(lua_State *L) {
	NppExtensionAPI::Pane p = check_pane_object(L, 1);
	int cpMin = static_cast<int>(luaL_checkinteger(L, 2));
	int cpMax = static_cast<int>(luaL_checkinteger(L, 3));
	host->Remove(p, cpMin, cpMax);
	return 0;
}

static int cf_pane_append(lua_State *L) {
	NppExtensionAPI::Pane p = check_pane_object(L, 1);
	const char *s = luaL_checkstring(L, 2);
	host->Insert(p, static_cast<int>(host->Send(p, SCI_GETLENGTH, 0, 0)), s);
	return 0;
}

static int cf_pane_findtext(lua_State *L) {
	NppExtensionAPI::Pane p = check_pane_object(L, 1);

	int nArgs = lua_gettop(L);

	const char *t = luaL_checkstring(L, 2);
	bool hasError = (!t);

	if (!hasError) {
		Sci_TextToFind ft = {{0, 0}, 0, {0, 0}};

		ft.lpstrText = const_cast<char *>(t);

		int flags = (nArgs > 2) ? (int)luaL_checkinteger(L, 3) : 0;
		hasError = (flags == 0 && lua_gettop(L) > nArgs);

		if (!hasError) {
			if (nArgs > 3) {
				ft.chrg.cpMin = static_cast<int>(luaL_checkinteger(L, 4));
				hasError = (lua_gettop(L) > nArgs);
			}
		}

		if (!hasError) {
			if (nArgs > 4) {
				ft.chrg.cpMax = static_cast<int>(luaL_checkinteger(L, 5));
				hasError = (lua_gettop(L) > nArgs);
			} else {
				ft.chrg.cpMax = static_cast<long>(host->Send(p, SCI_GETLENGTH, 0, 0));
			}
		}

		if (!hasError) {
			sptr_t result = host->Send(p, SCI_FINDTEXT, static_cast<uptr_t>(flags), SptrFromPointer(&ft));
			if (result >= 0) {
				lua_pushinteger(L, static_cast<LUA_INTEGER>(ft.chrgText.cpMin));
				lua_pushinteger(L, static_cast<LUA_INTEGER>(ft.chrgText.cpMax));
				return 2;
			} else {
				lua_pushnil(L);
				return 1;
			}
		}
	}

	if (hasError) {
		raise_error(L, "Invalid arguments for <pane>:findtext");
	}

	return 0;
}

// Pane match generator.  This was prototyped in about 30 lines of Lua.
// I hope the C++ version is more robust at least, e.g. prevents infinite
// loops and is more tamper-resistant.

struct PaneMatchObject {
	NppExtensionAPI::Pane pane;
	int startPos;
	int endPos;
	int flags; // this is really part of the state, but is kept here for convenience
	int endPosOrig; // has to do with preventing infinite loop on a 0-length match
};

static int cf_match_replace(lua_State *L) {
	PaneMatchObject *pmo = static_cast<PaneMatchObject *>(checkudata(L, 1, "Npp_MT_PaneMatchObject"));
	if (!pmo) {
		raise_error(L, "Self argument for match:replace() should be a pane match object.");
		return 0;
	} else if ((pmo->startPos < 0) || (pmo->endPos < pmo->startPos) || (pmo->endPos < 0)) {
		raise_error(L, "Blocked attempt to use invalidated pane match object.");
		return 0;
	}
	const char *replacement = luaL_checkstring(L, 2);

	// If an option were added to process \d back-references, it would just
	// be an optional boolean argument, i.e. m:replace([[\1]], true), and
	// this would just change SCI_REPLACETARGET to SCI_REPLACETARGETRE.
	// The problem is, even if SCFIND_REGEXP was used, it's hard to know
	// whether the back references are still valid.  So for now this is
	// left out.

	host->Send(pmo->pane, SCI_SETTARGETSTART, pmo->startPos, 0);
	host->Send(pmo->pane, SCI_SETTARGETEND, pmo->endPos, 0);
	host->Send(pmo->pane, SCI_REPLACETARGET, lua_rawlen(L, 2), SptrFromString(replacement));
	pmo->endPos = static_cast<int>(host->Send(pmo->pane, SCI_GETTARGETEND, 0, 0));
	return 0;
}

static int cf_match_metatable_index(lua_State *L) {
	PaneMatchObject *pmo = static_cast<PaneMatchObject *>(checkudata(L, 1, "Npp_MT_PaneMatchObject"));
	if (!pmo) {
		raise_error(L, "Internal error: pane match object is missing.");
		return 0;
	} else if ((pmo->startPos < 0) || (pmo->endPos < pmo->startPos) || (pmo->endPos < 0)) {
		raise_error(L, "Blocked attempt to use invalidated pane match object.");
		return 0;
	}

	if (lua_isstring(L, 2)) {
		const char *key = lua_tostring(L, 2);

		if (0 == strcmp(key, "pos")) {
			lua_pushinteger(L, pmo->startPos);
			return 1;
		} else if (0 == strcmp(key, "len")) {
			lua_pushinteger(L, pmo->endPos - pmo->startPos);
			return 1;
		} else if (0 == strcmp(key, "text")) {
			// If the document is changed while in the match loop, this will be broken.
			// Exception: if the changes are made exclusively through match:replace,
			// everything will be fine.
			char *range = host->Range(pmo->pane, pmo->startPos, pmo->endPos);
			if (range) {
				lua_pushstring(L, range);
				delete []range;
				return 1;
			} else {
				return 0;
			}
		} else if (0 == strcmp(key, "replace")) {
			int replaceMethodIndex = lua_upvalueindex(1);
			if (lua_iscfunction(L, replaceMethodIndex)) {
				lua_pushvalue(L, replaceMethodIndex);
				return 1;
			} else {
				return 0;
			}
		}
	}

	raise_error(L, "Invalid property / method name for pane match object.");
	return 0;
}

static int cf_match_metatable_tostring(lua_State *L) {
	PaneMatchObject *pmo = static_cast<PaneMatchObject *>(checkudata(L, 1, "Npp_MT_PaneMatchObject"));
	if (!pmo) {
		raise_error(L, "Internal error: pane match object is missing.");
		return 0;
	} else if ((pmo->startPos < 0) || (pmo->endPos < pmo->startPos) || (pmo->endPos < 0)) {
		lua_pushliteral(L, "match(invalidated)");
		return 1;
	} else {
		lua_pushfstring(L, "match{pos=%d,len=%d}", pmo->startPos, pmo->endPos - pmo->startPos);
		return 1;
	}
}

static int cf_pane_match(lua_State *L) {
	int nargs = lua_gettop(L);

	NppExtensionAPI::Pane p = check_pane_object(L, 1);
	luaL_checkstring(L, 2);

	int generatorIndex = lua_upvalueindex(1);
	if (!lua_isfunction(L, generatorIndex)) {
		raise_error(L, "Internal error: match generator is missing.");
		return 0;
	}

	lua_pushvalue(L, generatorIndex);

	// I'm putting some of the state in the match userdata for more convenient
	// access.  But, the search string is going in state because that part is
	// more convenient to leave in Lua form.
	lua_pushvalue(L, 2);

	PaneMatchObject *pmo = static_cast<PaneMatchObject *>(lua_newuserdata(L, sizeof(PaneMatchObject)));
	if (pmo) {
		pmo->pane = p;
		pmo->startPos = -1;
		pmo->endPos = pmo->endPosOrig = 0;
		pmo->flags = 0;
		if (nargs >= 3) {
			pmo->flags = (int)luaL_checkinteger(L, 3);
			if (nargs >= 4) {
				pmo->endPos = pmo->endPosOrig = (int)luaL_checkinteger(L, 4);
				if (pmo->endPos < 0) {
					raise_error(L, "Invalid argument 3 for <pane>:match.  Positive number or zero expected.");
					return 0;
				}
			}
		}
		if (luaL_newmetatable(L, "Npp_MT_PaneMatchObject")) {
			lua_pushliteral(L, "__index");
			lua_pushcfunction(L, cf_match_replace);
			lua_pushcclosure(L, cf_match_metatable_index, 1);
			lua_settable(L, -3);

			lua_pushliteral(L, "__tostring");
			lua_pushcfunction(L, cf_match_metatable_tostring);
			lua_settable(L, -3);
		}
		lua_setmetatable(L, -2);

		return 3;
	} else {
		raise_error(L, "Internal error: could not create match object.");
		return 0;
	}
}

static int cf_pane_match_generator(lua_State *L) {
	const char *text = lua_tostring(L, 1);
	PaneMatchObject *pmo = static_cast<PaneMatchObject *>(checkudata(L, 2, "Npp_MT_PaneMatchObject"));

	if (!(text)) {
		raise_error(L, "Internal error: invalid state for <pane>:match generator.");
		return 0;
	} else if (!pmo) {
		raise_error(L, "Internal error: invalid match object initializer for <pane>:match generator");
		return 0;
	}

	if ((pmo->endPos < 0) || (pmo->endPos < pmo->startPos)) {
		raise_error(L, "Blocked attempt to use invalidated pane match object.");
		return 0;
	}

	int searchPos = pmo->endPos;
	if ((pmo->startPos == pmo->endPosOrig) && (pmo->endPos == pmo->endPosOrig)) {
		// prevent infinite loop on zero-length match by stepping forward
		searchPos++;
	}

	Sci_TextToFind ft = { {0,0}, 0, {0,0} };
	ft.chrg.cpMin = searchPos;
	ft.chrg.cpMax = static_cast<long>(host->Send(pmo->pane, SCI_GETLENGTH, 0, 0));
	ft.lpstrText = const_cast<char *>(text);

	if (ft.chrg.cpMax > ft.chrg.cpMin) {
		sptr_t result = host->Send(pmo->pane, SCI_FINDTEXT, static_cast<uptr_t>(pmo->flags), SptrFromPointer(&ft));
		if (result >= 0) {
			pmo->startPos = static_cast<int>(ft.chrgText.cpMin);
			pmo->endPos = pmo->endPosOrig = static_cast<int>(ft.chrgText.cpMax);
			lua_pushvalue(L, 2);
			return 1;
		}
	}

	// One match object is used throughout the entire iteration.
	// This means it's bad to try to save the match object for later
	// reference.
	pmo->startPos = pmo->endPos = pmo->endPosOrig = -1;
	lua_pushnil(L);
	return 1;
}

static int cf_global_print(lua_State *L) {
	int nargs = lua_gettop(L);

	lua_getglobal(L, "tostring");

	for (int i = 1; i <= nargs; ++i) {
		if (i > 1)
			host->Trace("\t");

		const char *argStr = lua_tostring(L, i);
		if (argStr) {
			host->Trace(argStr);
		} else {
			lua_pushvalue(L, -1); // tostring
			lua_pushvalue(L, i);
			lua_call(L, 1, 1);
			argStr = lua_tostring(L, -1);
			if (argStr) {
				host->Trace(argStr);
			} else {
				raise_error(L, "tostring (called from print) returned a non-string");
			}
			lua_settop(L, nargs + 1);
		}
	}

	host->Trace("\r\n");
	return 0;
}

static bool call_function(lua_State *L, int nargs, bool ignoreFunctionReturnValue=false) {
	bool handled = false;
	if (L) {
		int traceback = 0;
		if (tracebackEnabled) {
			lua_getglobal(L, "debug");
			lua_getfield(L, -1, "traceback");
			lua_remove(L, -2);
			if (lua_isfunction(L, -1)) {
				traceback = lua_gettop(L) - nargs - 1;
				lua_insert(L, traceback);
			} else {
				lua_pop(L, 1);
			}
		}

		int result = lua_pcall(L, nargs, ignoreFunctionReturnValue ? 0 : 1, traceback);

		if (traceback) {
			lua_remove(L, traceback);
		}

		if (result == LUA_OK) {
			if (ignoreFunctionReturnValue) {
				handled = true;
			} else {
				handled = (0 != lua_toboolean(L, -1));
				lua_pop(L, 1);
			}
		} else if (result == LUA_ERRRUN) {
			lua_getglobal(L, "print");
			lua_insert(L, -2); // use pushed error message
			lua_pcall(L, 1, 0, 0);
		} else {
			lua_pop(L, 1);
			if (result == LUA_ERRMEM) {
				host->TraceError("Memory allocation error\r\n");
			} else if (result == LUA_ERRERR) {
				host->TraceError("An error occurred, but cannot be reported due to failure in _TRACEBACK\r\n");
			} else {
				host->TraceError("Unexpected error\r\n");
			}
		}
	}
	return handled;
}

static bool HasNamedFunction(const char *name) {
	bool hasFunction = false;
	if (luaState) {
		lua_getglobal(luaState, name);
		hasFunction = lua_isfunction(luaState, -1);
		lua_pop(luaState, 1);
	}
	return hasFunction;
}

// Generic function to pass parameters to the callbacks
// varfmt is a string defining what type the following arguments are
static bool CallNamedFunction(const char *name, const char *varfmt, ...) {
	bool handled = false;
	if (luaState) {
		lua_pushstring(luaState, "Npp_Callbacks");
		lua_gettable(luaState, LUA_REGISTRYINDEX);
		lua_getfield(luaState, -1, name);
		if (lua_istable(luaState, -1)) {
			lua_pushnil(luaState); /* first key */
			while (lua_next(luaState, -2) != 0) {
				if (varfmt) {
					// Push all the variables
					va_list vl;
					va_start(vl, varfmt);
					const char *type = varfmt;
					while (*type) {
						switch (*type++) {
						case 's': lua_pushstring(luaState, va_arg(vl, char *)); break;
						case 'i': lua_pushinteger(luaState, va_arg(vl, int)); break;
						default: raise_error(luaState, varfmt);
						}
					}
					va_end(vl);
				}
				handled = call_function(luaState, varfmt != NULL ? strlen(varfmt) : 0);
				// call_function removes the function for us, the key stays on the stack
			}
		}
		lua_pop(luaState, 2); // the Npp_Callbacks table and the callback table
	}
	return handled;
}

static int iface_function_helper(lua_State *L, const IFaceFunction &func) {
	NppExtensionAPI::Pane p = check_pane_object(L, 1);

	int arg = 2;

	sptr_t params[2] = {0,0};

	char *stringResult = 0;
	wchar_t *wstringResult = 0;
	tstring nppstrparam;
	enum stringResultType { none, string, tstring } needStringResult = none;
	int loopParamCount = 2;

	if (func.paramType[0] == iface_length && func.paramType[1] == iface_string) {
		params[0] = lua_rawlen(L, arg);
		params[1] = SptrFromString(params[0] ? lua_tostring(L, arg) : "");
		loopParamCount = 0;
	} else if ((func.paramType[1] == iface_stringresult) || (func.returnType == iface_stringresult) ||
				(func.paramType[1] == iface_tstringresult) || (func.returnType == iface_tstringresult)) {
		needStringResult = (func.paramType[1] == iface_stringresult) || (func.returnType == iface_stringresult) ? string : tstring;
		// The buffer will be allocated later, so it won't leak if Lua does
		// a longjmp in response to a bad arg.
		if (func.paramType[0] == iface_length) {
			loopParamCount = 0;
		} else {
			loopParamCount = 1;
		}
	}

	for (int i=0; i<loopParamCount; ++i) {
		if (func.paramType[i] == iface_string) {
			const char *s = lua_tostring(L, arg++);
			params[i] = SptrFromString(s ? s : "");
		} else if (func.paramType[i] == iface_tstring) {
			const char *s = lua_tostring(L, arg++);
			nppstrparam = GUI::StringFromUTF8(s ? s : "");
			params[i] = reinterpret_cast<sptr_t>(nppstrparam.c_str());
		} else if (func.paramType[i] == iface_keymod) {
			int keycode = static_cast<int>(luaL_checkinteger(L, arg++)) & 0xFFFF;
			int modifiers = static_cast<int>(luaL_checkinteger(L, arg++)) & (SCMOD_SHIFT|SCMOD_CTRL|SCMOD_ALT);
			params[i] = keycode | (modifiers<<16);
		} else if (func.paramType[i] == iface_bool) {
			params[i] = lua_toboolean(L, arg++);
		} else if (IFaceTypeIsNumeric(func.paramType[i])) {
			params[i] = static_cast<long>(luaL_checkinteger(L, arg++));
		}
	}

	if (needStringResult != none) {
		sptr_t stringResultLen;
		if (needStringResult == string) {
			stringResultLen = host->Send(p, func.value, params[0], 0);
			if (stringResultLen > 0) {
				// not all string result methods are guaranteed to add a null terminator
				stringResult = new char[stringResultLen+1];
				stringResult[stringResultLen]='\0';
				params[1] = SptrFromPointer(stringResult);
			} else {
				// Is this an error?  Are there any cases where it's not an error,
				// and where the right thing to do is just return a blank string?
				return 0;
			}
		} else {
			// Per the Notepad++ documentation buffers should be MAX_PATH in length
			// There aren't any Notepad++ messages currently that use iface_length
			// so this would always report the length as MAX_PATH instead of the actual len
			stringResultLen = MAX_PATH;
			wstringResult = new wchar_t[stringResultLen];
			params[1] = SptrFromPointer(wstringResult);
		}
		if (func.paramType[0] == iface_length) {
			params[0] = stringResultLen;
		}
	}

	// Now figure out what to do with the param types and return type.
	// - stringresult gets inserted at the start of return tuple.
	// - numeric return type gets returned to lua as a number (following the stringresult)
	// - other return types e.g. void get dropped.

	sptr_t result = host->Send(p, func.value, params[0], params[1]);

	int resultCount = 0;

	if (stringResult) {
		lua_pushstring(L, stringResult);
		delete[] stringResult;
		resultCount++;
	}
	if (wstringResult) {
		// lua makes its own copy of the string so we only need the shared_ptr in this block
		std::string res = GUI::UTF8FromString(wstringResult);
		lua_pushstring(L, res.c_str());
		delete[] wstringResult;
		resultCount++;
	}

	if (func.returnType == iface_bool) {
		lua_pushboolean(L, static_cast<int>(result));
		resultCount++;
	} else if (IFaceTypeIsNumeric(func.returnType)) {
		lua_pushinteger(L, static_cast<int>(result));
		resultCount++;
	}

	return resultCount;
}

struct IFacePropertyBinding {
	NppExtensionAPI::Pane pane;
	const IFaceProperty *prop;
};

static int cf_ifaceprop_metatable_index(lua_State *L) {
	// if there is a getter, __index calls it
	// otherwise, __index raises "property 'name' is write-only".
	IFacePropertyBinding *ipb = static_cast<IFacePropertyBinding *>(checkudata(L, 1, "Npp_MT_IFacePropertyBinding"));
	if (!(ipb && IFacePropertyIsScriptable(*(ipb->prop)))) {
		raise_error(L, "Internal error: property binding is improperly set up");
		return 0;
	}
	if (ipb->prop->getter == 0) {
		raise_error(L, "Attempt to read a write-only indexed property");
		return 0;
	}
	IFaceFunction func = ipb->prop->GetterFunction();

	// rewrite the stack to match what the function expects.  put pane at index 1; param is already at index 2.
	push_pane_object(L, ipb->pane);
	lua_replace(L, 1);
	lua_settop(L, 2);
	return iface_function_helper(L, func);
}

static int cf_ifaceprop_metatable_newindex(lua_State *L) {
	IFacePropertyBinding *ipb = static_cast<IFacePropertyBinding *>(checkudata(L, 1, "Npp_MT_IFacePropertyBinding"));
	if (!(ipb && IFacePropertyIsScriptable(*(ipb->prop)))) {
		raise_error(L, "Internal error: property binding is improperly set up");
		return 0;
	}
	if (ipb->prop->setter == 0) {
		raise_error(L, "Attempt to write a read-only indexed property");
		return 0;
	}
	IFaceFunction func = ipb->prop->SetterFunction();

	// rewrite the stack to match what the function expects.
	// pane at index 1; param at index 2, value at index 3
	push_pane_object(L, ipb->pane);
	lua_replace(L, 1);
	lua_settop(L, 3);
	return iface_function_helper(L, func);
}

static int cf_pane_iface_function(lua_State *L) {
	int funcidx = lua_upvalueindex(1);
	const IFaceFunction *func = static_cast<IFaceFunction *>(lua_touserdata(L, funcidx));
	if (func) {
		return iface_function_helper(L, *func);
	} else {
		raise_error(L, "Internal error - bad upvalue in iface function closure");
		return 0;
	}
}

static int push_iface_function(lua_State *L, const char *name, IFaceTableInterface *iface) {
	auto func = iface->FindFunction(name);
	if (func != nullptr) {
		if (IFaceFunctionIsScriptable(*func)) {
			lua_pushlightuserdata(L, (void*)func);
			lua_pushcclosure(L, cf_pane_iface_function, 1);

			// Since Lua experts say it is inefficient to create closures / cfunctions
			// in an inner loop, I tried caching the closures in the metatable, and looking
			// for them there first.  However, it made very little difference and did not
			// seem worth the added complexity. - WBD

			return 1;
		}
	}
	return -1; // signal to try next pane index handler
}

static int push_iface_propval(lua_State *L, const char *name, IFaceTableInterface *iface) {
	// this function doesn't raise errors, but returns 0 if the function is not handled.

	auto prop = iface->FindProperty(name);
	if (prop != nullptr) {
		if (!IFacePropertyIsScriptable(*prop)) {
			raise_error(L, "Error: iface property is not scriptable.");
			return -1;
		}

		if (prop->paramType == iface_void) {
			if (prop->getter) {
				lua_settop(L, 1);
				return iface_function_helper(L, prop->GetterFunction());
			}
		} else if (prop->paramType == iface_bool) {
			// The bool getter is untested since there are none in the iface.
			// However, the following is suggested as a reference protocol.
			NppExtensionAPI::Pane p = check_pane_object(L, 1);

			if (prop->getter) {
				if (host->Send(p, prop->getter, 1, 0)) {
					lua_pushnil(L);
					return 1;
				} else {
					lua_settop(L, 1);
					lua_pushboolean(L, 0);
					return iface_function_helper(L, prop->GetterFunction());
				}
			}
		} else {
			// Indexed property.  These return an object with the following behavior:
			// if there is a getter, __index calls it
			// otherwise, __index raises "property 'name' is write-only".
			// if there is a setter, __newindex calls it
			// otherwise, __newindex raises "property 'name' is read-only"

			IFacePropertyBinding *ipb = static_cast<IFacePropertyBinding *>(lua_newuserdata(L, sizeof(IFacePropertyBinding)));
			if (ipb) {
				ipb->pane = check_pane_object(L, 1);
				ipb->prop = prop;
				if (luaL_newmetatable(L, "Npp_MT_IFacePropertyBinding")) {
					lua_pushliteral(L, "__index");
					lua_pushcfunction(L, cf_ifaceprop_metatable_index);
					lua_settable(L, -3);
					lua_pushliteral(L, "__newindex");
					lua_pushcfunction(L, cf_ifaceprop_metatable_newindex);
					lua_settable(L, -3);
				}
				lua_setmetatable(L, -2);
				return 1;
			} else {
				raise_error(L, "Internal error: failed to allocate userdata for indexed property");
				return -1;
			}
		}
	}

	return -1; // signal to try next pane index handler
}

static int cf_pane_metatable_index(lua_State *L) {
	IFaceTableInterface *iface = static_cast<IFaceTableInterface *>(lua_touserdata(L, lua_upvalueindex(1)));
	if (lua_isstring(L, 2)) {
		const char *name = lua_tostring(L, 2);

		// these return the number of values pushed (possibly 0), or -1 if no match
		int results = push_iface_function(L, name, iface);
		if (results < 0)
			results = push_iface_propval(L, name, iface);

		if (results >= 0) {
			return results;
		} else if (name[0] != '_') {
			lua_getmetatable(L, 1);
			if (lua_istable(L, -1)) {
				lua_pushvalue(L, 2);
				lua_gettable(L, -2);
				if (!lua_isnil(L, -1))
					return 1;
			}
		}
	}

	raise_error(L, "Pane function / readable property / indexed writable property name expected");
	return 0;
}

static int cf_pane_metatable_newindex(lua_State *L) {
	IFaceTableInterface *iface = static_cast<IFaceTableInterface *>(lua_touserdata(L, lua_upvalueindex(1)));
	if (lua_isstring(L, 2)) {
		auto prop = iface->FindProperty(lua_tostring(L, 2));
		if (prop != nullptr) {
			if (IFacePropertyIsScriptable(*prop)) {
				if (prop->setter) {
					// stack needs to be rearranged to look like an iface function call
					lua_remove(L, 2);
					if (prop->paramType == iface_void) {
						return iface_function_helper(L, prop->SetterFunction());
					} else if ((prop->paramType == iface_bool)) {
						if (!lua_isnil(L, 3)) {
							lua_pushboolean(L, 1);
							lua_insert(L, 2);
						} else {
							// the nil will do as a false value.
							// just push an arbitrary numeric value that Scintilla will ignore
							lua_pushinteger(L, 0);
						}
						return iface_function_helper(L, prop->SetterFunction());

					} else {
						raise_error(L, "Error - (pane object) cannot assign directly to indexed property");
					}
				} else {
					raise_error(L, "Error - (pane object) cannot assign to a read-only property");
				}
			}
		}
	}

	raise_error(L, "Error - (pane object) expected the name of a writable property");
	return 0;
}

void push_pane_object(lua_State *L, NppExtensionAPI::Pane p) {
	*static_cast<NppExtensionAPI::Pane *>(lua_newuserdata(L, sizeof(p))) = p;
	if (luaL_newmetatable(L, "Npp_MT_Pane")) {
		lua_pushlightuserdata(L, &SciIFaceTable);
		lua_pushcclosure(L, cf_pane_metatable_index, 1);
		lua_setfield(L, -2, "__index");
		lua_pushlightuserdata(L, &SciIFaceTable);
		lua_pushcclosure(L, cf_pane_metatable_newindex, 1);
		lua_setfield(L, -2, "__newindex");

		// Push built-in functions into the metatable, where the custom
		// __index metamethod will find them.

		lua_pushcfunction(L, cf_pane_findtext);
		lua_setfield(L, -2, "findtext");
		lua_pushcfunction(L, cf_pane_textrange);
		lua_setfield(L, -2, "textrange");
		lua_pushcfunction(L, cf_pane_insert);
		lua_setfield(L, -2, "insert");
		lua_pushcfunction(L, cf_pane_remove);
		lua_setfield(L, -2, "remove");
		lua_pushcfunction(L, cf_pane_append);
		lua_setfield(L, -2, "append");

		lua_pushcfunction(L, cf_pane_match_generator);
		lua_pushcclosure(L, cf_pane_match, 1);
		lua_setfield(L, -2, "match");
	}
	lua_setmetatable(L, -2);
}

static int cf_global_metatable_index(lua_State *L) {
	if (lua_isstring(L, 2)) {
		const char *name = lua_tostring(L, 2);
		if ((name[0] < 'A') || (name[0] > 'Z') || ((name[1] >= 'a') && (name[1] <= 'z'))) {
			// short circuit; iface constants are always upper-case and start with a letter
			return 0;
		}

		auto con = ifacemixer.FindConstant(name);
		if (con != nullptr) {
			lua_pushinteger(L, con->value);
			return 1;
		} else {
			auto func = ifacemixer.FindFunctionByConstantName(name);
			if (func != nullptr) {
				lua_pushinteger(L, func->value);

				// FindFunctionByConstantName is slow, so cache the result into the
				// global table.  My tests show this gives an order of magnitude
				// improvement.
				lua_pushvalue(L, 2);
				lua_pushvalue(L, -2);
				lua_rawset(L, 1);

				return 1;
			}
		}
	}

	return 0; // global namespace access should not raise errors
}

static int LuaPanicFunction(lua_State *L) {
	if (L == luaState) {
		lua_close(luaState);
		luaState = NULL;
		luaDisabled = true;
	}
	host->TraceError("\r\nError occurred in unprotected call.  This is very bad.\r\n");
	return 1;
}

static bool InitGlobalScope() {
	tracebackEnabled = (GetPropertyInt("ext.lua.debug.traceback") == 1);

	if (!luaDisabled) {
		luaState = luaL_newstate();
		if (!luaState) {
			luaDisabled = true;
			host->TraceError("Scripting engine failed to initialise\r\n");
			return false;
		}
		lua_atpanic(luaState, LuaPanicFunction);

	}
	else {
		return false;
	}

	ifacemixer.AddIFaceTable(&SciIFaceTable);
	ifacemixer.AddIFaceTable(&NppIFaceTable);

	// ...register standard libraries
	luaL_openlibs(luaState);

	// override a library function whose default impl uses stdout
	lua_register(luaState, "print", cf_global_print);

	// pane objects
	push_pane_object(luaState, host->paneEditorMain);
	lua_setglobal(luaState, "editor1");

	push_pane_object(luaState, host->paneEditorSecondary);
	lua_setglobal(luaState, "editor2");

	lua_getglobal(luaState, "editor1");
	lua_setglobal(luaState, "editor");

	push_pane_object(luaState, host->paneOutput);
	lua_setglobal(luaState, "console");

	// scite
	*static_cast<NppExtensionAPI::Pane *>(lua_newuserdata(luaState, sizeof(host->application))) = host->application;
	if (luaL_newmetatable(luaState, "Npp_MT_Application")) {
		lua_pushlightuserdata(luaState, &NppIFaceTable);
		lua_pushcclosure(luaState, cf_pane_metatable_index, 1);
		lua_setfield(luaState, -2, "__index");
		lua_pushlightuserdata(luaState, &NppIFaceTable);
		lua_pushcclosure(luaState, cf_pane_metatable_newindex, 1);
		lua_setfield(luaState, -2, "__newindex");

		// Push built-in functions into the metatable, where the custom
		// __index metamethod will find them.
		lua_getglobal(luaState, "editor");
		lua_pushcclosure(luaState, cf_npp_send, 1);
		lua_setfield(luaState, -2, "SendEditor");

		lua_pushcfunction(luaState, cf_npp_constname);
		lua_setfield(luaState, -2, "ConstantName");

		lua_pushcfunction(luaState, cf_npp_add_shortcut);
		lua_setfield(luaState, -2, "AddShortcut");

		lua_pushcfunction(luaState, cf_npp_write_error);
		lua_setfield(luaState, -2, "WriteError");

		lua_pushcfunction(luaState, cf_npp_clear_console);
		lua_setfield(luaState, -2, "ClearConsole");

		lua_pushcfunction(luaState, cf_npp_add_callback);
		lua_setfield(luaState, -2, "AddEventHandler");

		lua_pushcfunction(luaState, cf_npp_remove_callback);
		lua_setfield(luaState, -2, "RemoveEventHandler");

		lua_pushcfunction(luaState, cf_npp_removeall_callbacks);
		lua_setfield(luaState, -2, "RemoveAllEventHandlers");
	}
	lua_setmetatable(luaState, -2);

	// Keep "scite" for backwards compatibility but also allow "npp"
	lua_setglobal(luaState, "scite");
	lua_getglobal(luaState, "scite");
	lua_setglobal(luaState, "npp");

	// Create the registry table to hold callbacks
	lua_newtable(luaState);
	lua_setfield(luaState, LUA_REGISTRYINDEX, "Npp_Callbacks");

	// get global environment table from registry
	lua_pushglobaltable(luaState);
	// Metatable for global namespace, to publish iface constants
	if (luaL_newmetatable(luaState, "Npp_MT_GlobalScope")) {
		lua_pushcfunction(luaState, cf_global_metatable_index);
		lua_setfield(luaState, -2, "__index");
	}
	// set global index callback hook
	lua_setmetatable(luaState, -2);
	// remove the global environment table from the stack
	lua_pop(luaState, 1);

	return true;
}


LuaExtension::LuaExtension() {}
LuaExtension::~LuaExtension() {}

LuaExtension &LuaExtension::Instance() {
	static LuaExtension singleton;
	return singleton;
}

bool LuaExtension::Initialise(NppExtensionAPI *host_) {
	host = host_;

	return false;
}

bool LuaExtension::Finalise() {
	if (luaState) {
		lua_close(luaState);
	}

	luaState = NULL;
	host = NULL;

	return false;
}

bool LuaExtension::RunString(const char *s) {
	if (luaState || InitGlobalScope()) {
		int status = luaL_loadbuffer(luaState, s, strlen(s), "=File");

		if (status == LUA_OK) {
			status = lua_pcall(luaState, 0, LUA_MULTRET, 0);
		}

		if (status != LUA_OK) {
			// Print an error message
			host->TraceError(lua_tostring(luaState, -1));
			host->TraceError("\r\n");
			lua_settop(luaState, 0); /* clear stack */
			return false;
		}

		lua_settop(luaState, 0); /* clear stack */
	}

	return true;
}

bool LuaExtension::RunFile(const wchar_t *filename) {
	std::ifstream filestream(filename, std::ios::in | std::ios::binary);
	std::string buff;

	if (!filestream.is_open()) return true; // this is ok since nothing actually "failed"

	filestream.seekg(0, std::ios::end);
	buff.reserve((size_t)filestream.tellg());
	filestream.seekg(0, std::ios::beg);

	buff.assign(std::istreambuf_iterator<char>(filestream), std::istreambuf_iterator<char>());
	filestream.close();

	// Skip the UTF-8-BOM
	int idx = 0;
	if (buff.compare(0, 3, "\xEF\xBB\xBF") == 0) idx = 3;

	return RunString(&buff.c_str()[idx]);
}

bool LuaExtension::OnExecute(const char *s) {
	static bool isFirstLine = true;
	static std::string chunk;
	int status = 0;

	if (luaState || InitGlobalScope()) {
		if (isFirstLine) {
			// First try to compile the chunk as a return statement
			const char *retline = lua_pushfstring(luaState, "return %s;", s);
			status = luaL_loadbuffer(luaState, retline, strlen(retline), "=Console");
			if (status == 0) lua_remove(luaState, -2);
			else lua_pop(luaState, 2);

			if (status == LUA_OK) {
				// It worked, let's call it
				status = lua_pcall(luaState, 0, LUA_MULTRET, 0);
			}
			else {
				// Else let's just try it as is
				status = luaL_loadbuffer(luaState, s, strlen(s), "=Console");
				if (status == LUA_OK) {
					status = lua_pcall(luaState, 0, LUA_MULTRET, 0);
				}
				else if (status == LUA_ERRSYNTAX) {
					size_t lmsg;
					const char *msg = lua_tolstring(luaState, -1, &lmsg);
					if (lmsg >= marklen && strcmp(msg + lmsg - marklen, EOFMARK) == 0) {
						lua_pop(luaState, 1);
						isFirstLine = false;
						chunk = s;
						return false;
					}
				}
			}
		}
		else {
			// Append the new line to what we've gotten so far
			chunk.append("\r\n");
			chunk.append(s);
			status = luaL_loadbuffer(luaState, chunk.c_str(), chunk.length(), "=Console");
			if (status == LUA_OK) {
				status = lua_pcall(luaState, 0, LUA_MULTRET, 0);
			}
			else if (status == LUA_ERRSYNTAX) {
				size_t lmsg;
				const char *msg = lua_tolstring(luaState, -1, &lmsg);
				if (lmsg >= marklen && strcmp(msg + lmsg - marklen, EOFMARK) == 0) {
					lua_pop(luaState, 1);
					return false;
				}
			}
		}

		// At this point *something* ran so clear out some data
		chunk.clear();
		isFirstLine = true;

		if (status == LUA_OK) {
			if (lua_gettop(luaState) > 0) {  /* any result to print? */
				lua_getglobal(luaState, "print");
				lua_insert(luaState, 1);
				if (lua_pcall(luaState, lua_gettop(luaState) - 1, 0, 0) != 0)
					host->TraceError("error calling " LUA_QL("print"));
			}
			// else everything finished fine but had no return value
		}
		else {
			// Print an error message if possible
			const char *errmsg = lua_tostring(luaState, -1);
			if (errmsg) {
				host->TraceError(errmsg);
				host->TraceError("\r\n");
			}
			else {
				host->TraceError("error\r\n");
			}
		}
		lua_settop(luaState, 0); /* clear stack */
	}
	return true;
}

void LuaExtension::CallShortcut(int id) {
	lua_pushliteral(luaState, "Npp_Shortcuts");
	lua_gettable(luaState, LUA_REGISTRYINDEX);
	if (lua_isnil(luaState, -1)) {
		raise_error(luaState, "Attempting to call unregistered shortcut");
	}

	lua_geti(luaState, -1, id);
	call_function(luaState, 0, true);
	lua_settop(luaState, 0); // Make sure it is cleared

	return;
}

// Similar to StyleContext class in Scintilla
struct StylingContext {
	unsigned int startPos;
	int lengthDoc;
	int initStyle;
	StyleWriter *styler;

	unsigned int endPos;
	unsigned int endDoc;

	unsigned int currentPos;
	bool atLineStart;
	bool atLineEnd;
	int state;

	char cursor[3][8];
	int cursorPos;
	int codePage;
	int lenCurrent;
	int lenNext;

	static StylingContext *Context(lua_State *L) {
		return static_cast<StylingContext *>(lua_touserdata(L, lua_upvalueindex(1)));
	}

	void Colourize() {
		int end = currentPos - 1;
		if (end >= static_cast<int>(endDoc))
			end = static_cast<int>(endDoc)-1;
		styler->ColourTo(end, state);
	}

	static int Line(lua_State *L) {
		StylingContext *context = Context(L);
		int position = (int)luaL_checkinteger(L, 2);
		lua_pushinteger(L, context->styler->GetLine(position));
		return 1;
	}

	static int CharAt(lua_State *L) {
		StylingContext *context = Context(L);
		int position = (int)luaL_checkinteger(L, 2);
		lua_pushinteger(L, context->styler->SafeGetCharAt(position));
		return 1;
	}

	static int StyleAt(lua_State *L) {
		StylingContext *context = Context(L);
		int position = (int)luaL_checkinteger(L, 2);
		lua_pushinteger(L, context->styler->StyleAt(position));
		return 1;
	}

	static int LevelAt(lua_State *L) {
		StylingContext *context = Context(L);
		int line = (int)luaL_checkinteger(L, 2);
		lua_pushinteger(L, context->styler->LevelAt(line));
		return 1;
	}

	static int SetLevelAt(lua_State *L) {
		StylingContext *context = Context(L);
		int line = (int)luaL_checkinteger(L, 2);
		int level = (int)luaL_checkinteger(L, 3);
		context->styler->SetLevel(line, level);
		return 0;
	}

	static int LineState(lua_State *L) {
		StylingContext *context = Context(L);
		int line = (int)luaL_checkinteger(L, 2);
		lua_pushinteger(L, context->styler->GetLineState(line));
		return 1;
	}

	static int SetLineState(lua_State *L) {
		StylingContext *context = Context(L);
		int line = (int)luaL_checkinteger(L, 2);
		int stateOfLine = (int)luaL_checkinteger(L, 3);
		context->styler->SetLineState(line, stateOfLine);
		return 0;
	}


	void GetNextChar() {
		lenCurrent = lenNext;
		lenNext = 1;
		int nextPos = currentPos + lenCurrent;
		unsigned char byteNext = static_cast<unsigned char>(styler->SafeGetCharAt(nextPos));
		unsigned int nextSlot = (cursorPos + 1) % 3;
		memcpy(cursor[nextSlot], "\0\0\0\0\0\0\0\0", 8);
		cursor[nextSlot][0] = byteNext;
		if (codePage) {
			if (codePage == SC_CP_UTF8) {
				if (byteNext >= 0x80) {
					cursor[nextSlot][1] = styler->SafeGetCharAt(nextPos + 1);
					lenNext = 2;
					if (byteNext >= 0x80 + 0x40 + 0x20) {
						lenNext = 3;
						cursor[nextSlot][2] = styler->SafeGetCharAt(nextPos + 2);
						if (byteNext >= 0x80 + 0x40 + 0x20 + 0x10) {
							lenNext = 4;
							cursor[nextSlot][3] = styler->SafeGetCharAt(nextPos + 3);
						}
					}
				}
			}
			else {
				if (styler->IsLeadByte(byteNext)) {
					lenNext = 2;
					cursor[nextSlot][1] = styler->SafeGetCharAt(nextPos + 1);
				}
			}
		}

		// End of line?
		// Trigger on CR only (Mac style) or either on LF from CR+LF (Dos/Win)
		// or on LF alone (Unix). Avoid triggering two times on Dos/Win.
		char ch = cursor[(cursorPos) % 3][0];
		atLineEnd = (ch == '\r' && cursor[nextSlot][0] != '\n') ||
			(ch == '\n') ||
			(currentPos >= endPos);
	}

	void StartStyling(unsigned int startPos_, unsigned int length, int initStyle_) {
		endDoc = styler->Length();
		endPos = startPos_ + length;
		if (endPos == endDoc)
			endPos = endDoc + 1;
		currentPos = startPos_;
		atLineStart = true;
		atLineEnd = false;
		state = initStyle_;
		cursorPos = 0;
		lenCurrent = 0;
		lenNext = 0;
		memcpy(cursor[0], "\0\0\0\0\0\0\0\0", 8);
		memcpy(cursor[1], "\0\0\0\0\0\0\0\0", 8);
		memcpy(cursor[2], "\0\0\0\0\0\0\0\0", 8);
		styler->StartAt(startPos_, static_cast<char>(0xffu));
		styler->StartSegment(startPos_);

		GetNextChar();
		cursorPos++;
		GetNextChar();
	}

	static int EndStyling(lua_State *L) {
		StylingContext *context = Context(L);
		context->Colourize();
		return 0;
	}

	static int StartStyling(lua_State *L) {
		StylingContext *context = Context(L);
		unsigned int startPosStyle = (int)luaL_checkinteger(L, 2);
		unsigned int lengthStyle = (int)luaL_checkinteger(L, 3);
		int initialStyle = (int)luaL_checkinteger(L, 4);
		context->StartStyling(startPosStyle, lengthStyle, initialStyle);
		return 0;
	}

	static int More(lua_State *L) {
		StylingContext *context = Context(L);
		lua_pushboolean(L, context->currentPos < context->endPos);
		return 1;
	}

	void Forward() {
		if (currentPos < endPos) {
			atLineStart = atLineEnd;
			currentPos += lenCurrent;
			cursorPos++;
			GetNextChar();
		}
		else {
			atLineStart = false;
			memcpy(cursor[0], "\0\0\0\0\0\0\0\0", 8);
			memcpy(cursor[1], "\0\0\0\0\0\0\0\0", 8);
			memcpy(cursor[2], "\0\0\0\0\0\0\0\0", 8);
			atLineEnd = true;
		}
	}

	static int Forward(lua_State *L) {
		StylingContext *context = Context(L);
		context->Forward();
		return 0;
	}

	static int Position(lua_State *L) {
		StylingContext *context = Context(L);
		lua_pushinteger(L, context->currentPos);
		return 1;
	}

	static int AtLineStart(lua_State *L) {
		StylingContext *context = Context(L);
		lua_pushboolean(L, context->atLineStart);
		return 1;
	}

	static int AtLineEnd(lua_State *L) {
		StylingContext *context = Context(L);
		lua_pushboolean(L, context->atLineEnd);
		return 1;
	}

	static int State(lua_State *L) {
		StylingContext *context = Context(L);
		lua_pushinteger(L, context->state);
		return 1;
	}

	static int SetState(lua_State *L) {
		StylingContext *context = Context(L);
		context->Colourize();
		context->state = (int)luaL_checkinteger(L, 2);
		return 0;
	}

	static int ForwardSetState(lua_State *L) {
		StylingContext *context = Context(L);
		context->Forward();
		context->Colourize();
		context->state = (int)luaL_checkinteger(L, 2);
		return 0;
	}

	static int ChangeState(lua_State *L) {
		StylingContext *context = Context(L);
		context->state = (int)luaL_checkinteger(L, 2);
		return 0;
	}

	static int Current(lua_State *L) {
		StylingContext *context = Context(L);
		lua_pushstring(L, context->cursor[context->cursorPos % 3]);
		return 1;
	}

	static int Next(lua_State *L) {
		StylingContext *context = Context(L);
		lua_pushstring(L, context->cursor[(context->cursorPos + 1) % 3]);
		return 1;
	}

	static int Previous(lua_State *L) {
		StylingContext *context = Context(L);
		lua_pushstring(L, context->cursor[(context->cursorPos + 2) % 3]);
		return 1;
	}

	static int Token(lua_State *L) {
		StylingContext *context = Context(L);
		int start = context->styler->GetStartSegment();
		int end = context->currentPos - 1;
		int len = end - start + 1;
		if (len <= 0)
			len = 1;
		char *sReturn = new char[len + 1];
		for (int i = 0; i < len; i++) {
			sReturn[i] = context->styler->SafeGetCharAt(start + i);
		}
		sReturn[len] = '\0';
		lua_pushstring(L, sReturn);
		delete[]sReturn;
		return 1;
	}

	bool Match(const char *s) {
		for (int n = 0; *s; n++) {
			if (*s != styler->SafeGetCharAt(currentPos + n))
				return false;
			s++;
		}
		return true;
	}

	static int Match(lua_State *L) {
		StylingContext *context = Context(L);
		const char *s = luaL_checkstring(L, 2);
		lua_pushboolean(L, context->Match(s));
		return 1;
	}

	void PushMethod(lua_State *L, lua_CFunction fn, const char *name) {
		lua_pushlightuserdata(L, this);
		lua_pushcclosure(L, fn, 1);
		lua_setfield(luaState, -2, name);
	}
};

bool LuaExtension::OnStyle(unsigned int startPos, int lengthDoc, int initStyle, StyleWriter *styler) {
	if (luaState) {
		lua_pushstring(luaState, "Npp_Callbacks");
		lua_gettable(luaState, LUA_REGISTRYINDEX);
		lua_getfield(luaState, -1, "OnStyle");
		if (lua_istable(luaState, -1)) {
			lua_pushnil(luaState); /* first key */
			while (lua_next(luaState, -2) != 0) {
				StylingContext sc;
				sc.startPos = startPos;
				sc.lengthDoc = lengthDoc;
				sc.initStyle = initStyle;
				sc.styler = styler;
				sc.codePage = static_cast<int>(host->Send(host->getCurrentPane(), SCI_GETCODEPAGE));

				lua_newtable(luaState);

				lua_pushstring(luaState, "startPos");
				lua_pushinteger(luaState, startPos);
				lua_settable(luaState, -3);

				lua_pushstring(luaState, "lengthDoc");
				lua_pushinteger(luaState, lengthDoc);
				lua_settable(luaState, -3);

				lua_pushstring(luaState, "initStyle");
				lua_pushinteger(luaState, initStyle);
				lua_settable(luaState, -3);

				//lua_pushstring(luaState, "language");
				//std::string lang = host->Property("Language");
				//lua_pushstring(luaState, lang.c_str());
				//lua_settable(luaState, -3);

				sc.PushMethod(luaState, StylingContext::Line, "Line");
				sc.PushMethod(luaState, StylingContext::CharAt, "CharAt");
				sc.PushMethod(luaState, StylingContext::StyleAt, "StyleAt");
				sc.PushMethod(luaState, StylingContext::LevelAt, "LevelAt");
				sc.PushMethod(luaState, StylingContext::SetLevelAt, "SetLevelAt");
				sc.PushMethod(luaState, StylingContext::LineState, "LineState");
				sc.PushMethod(luaState, StylingContext::SetLineState, "SetLineState");

				sc.PushMethod(luaState, StylingContext::StartStyling, "StartStyling");
				sc.PushMethod(luaState, StylingContext::EndStyling, "EndStyling");
				sc.PushMethod(luaState, StylingContext::More, "More");
				sc.PushMethod(luaState, StylingContext::Forward, "Forward");
				sc.PushMethod(luaState, StylingContext::Position, "Position");
				sc.PushMethod(luaState, StylingContext::AtLineStart, "AtLineStart");
				sc.PushMethod(luaState, StylingContext::AtLineEnd, "AtLineEnd");
				sc.PushMethod(luaState, StylingContext::State, "State");
				sc.PushMethod(luaState, StylingContext::SetState, "SetState");
				sc.PushMethod(luaState, StylingContext::ForwardSetState, "ForwardSetState");
				sc.PushMethod(luaState, StylingContext::ChangeState, "ChangeState");
				sc.PushMethod(luaState, StylingContext::Current, "Current");
				sc.PushMethod(luaState, StylingContext::Next, "Next");
				sc.PushMethod(luaState, StylingContext::Previous, "Previous");
				sc.PushMethod(luaState, StylingContext::Token, "Token");
				sc.PushMethod(luaState, StylingContext::Match, "Match");

				call_function(luaState, 1);
				// call_function removes the function for us, the key stays on the stack
				lua_pop(luaState, 1); // remove the key

				break; // Only use the first styler, all others are ignored
			}
		}
		lua_pop(luaState, 2); // the Npp_Callbacks table and the callback table
	}
	return false;
}

bool LuaExtension::OnChar(char ch) {
	char chs[2] = { ch, '\0' };
	return CallNamedFunction("OnChar", "s", chs);
}

bool LuaExtension::OnSavePointReached() {
	return CallNamedFunction("OnSavePointReached", NULL);
}

bool LuaExtension::OnSavePointLeft() {
	return CallNamedFunction("OnSavePointLeft", NULL);
}

bool LuaExtension::OnDoubleClick(const SCNotification *sc) {
	return CallNamedFunction("OnDoubleClick", "iii", sc->position, sc->line, sc->modifiers);
}

bool LuaExtension::OnUpdateUI(const SCNotification *sc) {
	return CallNamedFunction("OnUpdateUI", "i", sc->updated);
}

bool LuaExtension::OnModification(const SCNotification *sc) {
	int modType = sc->modificationType;

	return CallNamedFunction("OnModification", "iiisi", modType, sc->position, sc->length,
		sc->text != NULL ? std::string(sc->text, sc->length).c_str() : "", sc->linesAdded);
}

bool LuaExtension::OnMarginClick() {
	return CallNamedFunction("OnMarginClick", NULL);
}

bool LuaExtension::OnUserListSelection(int listType, const char *selection) {
	return CallNamedFunction("OnUserListSelection", "is", listType, selection);
}

bool LuaExtension::OnDwellStart(int pos, const char *word) {
	return CallNamedFunction("OnDwellStart", "is", pos, word);
}



bool LuaExtension::OnReady() {
	return CallNamedFunction("OnReady", NULL);
}
bool LuaExtension::OnBeforeOpen(const char *filename, uptr_t bufferid) {
	return CallNamedFunction("OnBeforeOpen", "si", filename, bufferid);
}

bool LuaExtension::OnOpen(const char *filename, uptr_t bufferid) {
	return CallNamedFunction("OnOpen", "si", filename, bufferid);
}

bool LuaExtension::OnSwitchFile(const char *filename, uptr_t bufferid) {
	if (!luaState) return false;

	// Switching to a new file also means it could update the current scintilla handle
	// so update "editor" to point to the right instance
	if (host->getCurrentPane() == host->paneEditorMain) {
		lua_getglobal(luaState, "editor1");
	}
	else {
		lua_getglobal(luaState, "editor2");
	}
	lua_setglobal(luaState, "editor");

	return CallNamedFunction("OnSwitchFile", "si", filename, bufferid);
}

bool LuaExtension::OnBeforeSave(const char *filename, uptr_t bufferid) {
	return CallNamedFunction("OnBeforeSave", "si", filename, bufferid);
}

bool LuaExtension::OnSave(const char *filename, uptr_t bufferid) {
	return CallNamedFunction("OnSave", "si", filename, bufferid);
}

bool LuaExtension::OnFileRenamed(const char *filename, uptr_t bufferid) {
	return CallNamedFunction("OnFileRenamed", "si", filename, bufferid);
}

bool LuaExtension::OnFileDeleted(const char *filename, uptr_t bufferid) {
	return CallNamedFunction("OnFileDeleted", "si", filename, bufferid);
}

bool LuaExtension::OnLangChange() {
	return CallNamedFunction("OnLangChange", NULL);
}

bool LuaExtension::OnBeforeClose(const char *filename, uptr_t bufferid) {
	return CallNamedFunction("OnBeforeClose", "si", filename, bufferid);
}

bool LuaExtension::OnClose(const char *filename, uptr_t bufferid) {
	return CallNamedFunction("OnClose", "si", filename, bufferid);
}

bool LuaExtension::OnBeforeShutdown() {
	return CallNamedFunction("OnBeforeShutdown", NULL);
}

bool LuaExtension::OnCancelShutdown() {
	return CallNamedFunction("OnCancelShutdown", NULL);
}

bool LuaExtension::OnShutdown() {
	return CallNamedFunction("OnShutdown", NULL);
}
