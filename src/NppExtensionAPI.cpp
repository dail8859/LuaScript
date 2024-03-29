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

#include <memory>
#include "NppExtensionAPI.h"
#include "Window.h"

NppExtensionAPI::~NppExtensionAPI() {}

NppExtensionAPI::Pane NppExtensionAPI::getCurrentPane() {
	int which = 0;
	SendMessage(m_nppData->_nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);

	// Luckily Notepad++'s index matches up with the scis array, it will either be 0 or 1
	return static_cast<NppExtensionAPI::Pane>(which);
}

sptr_t NppExtensionAPI::Send(NppExtensionAPI::Pane p, unsigned int msg, uptr_t wParam, sptr_t lParam) {
	// Just use SendMessage() for Notepad++, for any Scintilla handle use the ScintillaWindow wrapper
	// that allows for more effecient calls.
	if (p == NppExtensionAPI::application)
		return SendMessage(m_nppData->_nppHandle, msg, wParam, lParam);
	else
		return scis[p].Call(msg, wParam, lParam);
}

char *NppExtensionAPI::Range(NppExtensionAPI::Pane p, intptr_t start, intptr_t end) {
	if (end <= start) return nullptr;

	char *dest = new char[end - start + 1];
	Sci_TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = dest;
	this->Send(p, SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));

	return dest;
}

void NppExtensionAPI::Remove(NppExtensionAPI::Pane p, intptr_t start, intptr_t end) {
	if (end <= start) return;

	intptr_t deleteLength = end - start;
	this->Send(p, SCI_DELETERANGE, start, deleteLength);
}

void NppExtensionAPI::Insert(NppExtensionAPI::Pane p, intptr_t pos, const char *s) {
	this->Send(p, SCI_INSERTTEXT, pos, reinterpret_cast<LPARAM>(s));
}

void NppExtensionAPI::SetTextDirection(NppExtensionAPI::Pane p, bool rtl) {
	HWND hwnd = reinterpret_cast<HWND>(scis[p].GetID());
	long exStyle = static_cast<long>(::GetWindowLongPtr(hwnd, GWL_EXSTYLE));
	exStyle = rtl ? (exStyle | WS_EX_LAYOUTRTL) : (exStyle & (~WS_EX_LAYOUTRTL));
	::SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);

	// Toggle the wrap mode back and forth to fix the problem of mirrored characters
	// This was taken from N++
	bool isWrapped = this->Send(p, SCI_GETWRAPMODE) == SC_WRAP_WORD;
	this->Send(p, SCI_SETWRAPMODE, !isWrapped);
	this->Send(p, SCI_SETWRAPMODE, isWrapped);
}

void NppExtensionAPI::Trace(const char *s) {
	cd->writeText(strlen(s), s);
}

void NppExtensionAPI::TraceError(const char *s) {
	cd->writeError(strlen(s), s);
}

void NppExtensionAPI::Tracef(const char *fmt, ...) {
	char buffer[512];
	va_list arg;

	va_start(arg, fmt);
	vsnprintf(buffer, 512, fmt, arg);
	va_end(arg);

	cd->writeText(strlen(buffer), buffer);
}

void NppExtensionAPI::ClearConsole() {
	cd->clearText();
}

std::string NppExtensionAPI::Property(const char *key) {
	if (strcmp(key, "ext.lua.debug.traceback") == 0) return std::string("1");

	this->Tracef("TODO: NppExtensionAPI::Property(%s)\r\n", key);
	return std::string("NppExtensionAPI::Property");
}

void NppExtensionAPI::SetProperty(const char *key, const char *val) {
	this->Tracef("TODO: NppExtensionAPI::SetProperty(%s, %s)\r\n", key, val);
}

void NppExtensionAPI::UnsetProperty(const char *key) {
	this->Tracef("TODO: NppExtensionAPI::UnsetProperty(%s)\r\n", key);
}

uptr_t NppExtensionAPI::GetInstance() {
	this->Trace("TODO: NppExtensionAPI::GetInstance()\r\n");
	return NULL;
}

void NppExtensionAPI::ShutDown() {
	this->Trace("TODO: NppExtensionAPI::ShutDown()\r\n");
}

void NppExtensionAPI::DoMenuCommand(int cmdID) {
	SendMessage(m_nppData->_nppHandle, NPPM_MENUCOMMAND, 0, (LPARAM)cmdID);
}
