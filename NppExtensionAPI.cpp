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

std::shared_ptr<char> getStartupScritFilePath(wchar_t *buff, size_t size);

NppExtensionAPI::~NppExtensionAPI() {
}

HWND NppExtensionAPI::getScintillaHandle(NppExtensionAPI::Pane p) {
	if (p == NppExtensionAPI::paneEditor) {// Get the current scintilla
		int which = -1;
		SendMessage(m_nppData->_nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
		if (which == -1) return nullptr;
		return (which == 0) ? m_nppData->_scintillaMainHandle : m_nppData->_scintillaSecondHandle;
	}
	if (p == NppExtensionAPI::paneEditorMain) return m_nppData->_scintillaMainHandle;
	if (p == NppExtensionAPI::paneEditorSecondary) return m_nppData->_scintillaSecondHandle;
	if (p == NppExtensionAPI::paneOutput) return this->cd->getScintillaHwnd();

	return nullptr;
	
}

sptr_t NppExtensionAPI::Send(NppExtensionAPI::Pane p, unsigned int msg, uptr_t wParam, sptr_t lParam) {
	HWND sci = getScintillaHandle(p);

	if (sci == nullptr) {
		// This is bad :(
		MessageBox(NULL, TEXT("sci == nullptr"), TEXT("sci == nullptr"), MB_OK);
		return 0;
	}

	return SendMessage(sci, msg, wParam, lParam);
}

char *NppExtensionAPI::Range(NppExtensionAPI::Pane p, int start, int end) {
	if (end <= start) return nullptr;

	char *dest = new char[end - start + 1];
	TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = dest;
	this->Send(p, SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));

	return dest;
}

void NppExtensionAPI::Remove(NppExtensionAPI::Pane p, int start, int end) {
	if (end <= start) return;

	int deleteLength = end - start;
	this->Send(p, SCI_DELETERANGE, start, deleteLength);
}
void NppExtensionAPI::Insert(NppExtensionAPI::Pane p, int pos, const char *s) {
	this->Send(p, SCI_INSERTTEXT, pos, reinterpret_cast<LPARAM>(s));
}

void NppExtensionAPI::Trace(const char *s) {
	cd->writeText(strlen(s), s);
}

void NppExtensionAPI::Tracef(const char *fmt, ...) {
	char buffer[512];
	va_list arg;

	va_start(arg, fmt);
	vsnprintf(buffer, 512, fmt, arg);
	va_end(arg);

	cd->writeText(strlen(buffer), buffer);
}

std::string NppExtensionAPI::Property(const char *key) {
	if (strcmp(key, "ext.lua.debug.traceback") == 0) return std::string("1");
	if (strcmp(key, "ext.lua.startup.script") == 0) {
		wchar_t buff[MAX_PATH];
		return std::string(getStartupScritFilePath(buff, MAX_PATH).get());
	}

	this->Trace("TODO: NppExtensionAPI::Property(");
	this->Trace(key);
	this->Trace(")\r\n");
	return std::string("NppExtensionAPI::Property");
}

void NppExtensionAPI::SetProperty(const char *key, const char *val) {
	this->Trace("TODO: NppExtensionAPI::SetProperty()\r\n");
}

void NppExtensionAPI::UnsetProperty(const char *key) {
	this->Trace("TODO: NppExtensionAPI::UnsetProperty()\r\n");
}

uptr_t NppExtensionAPI::GetInstance() {
	this->Trace("TODO: NppExtensionAPI::GetInstance()\r\n");
	return NULL;
}

void NppExtensionAPI::ShutDown() {
	this->Trace("TODO: NppExtensionAPI::ShutDown()\r\n");
}

void NppExtensionAPI::Perform(const char *actions) {
	this->Trace("TODO: NppExtensionAPI::Perform(");
	this->Trace(actions);
	this->Trace(")\r\n");
}

void NppExtensionAPI::DoMenuCommand(int cmdID) {
	SendMessage(m_nppData->_nppHandle, NPPM_MENUCOMMAND, 0, (LPARAM)cmdID);
}

void NppExtensionAPI::UpdateStatusBar(bool bUpdateSlowData) {
	this->Trace("TODO: NppExtensionAPI::UpdateStatusBar()\r\n");
}

void NppExtensionAPI::UserStripShow(const char *description) {
	this->Trace("TODO: NppExtensionAPI::UserStripShow()\r\n");
}

void NppExtensionAPI::UserStripSet(int control, const char *value) {
	this->Trace("TODO: NppExtensionAPI::UserStripSet()\r\n");
}

void NppExtensionAPI::UserStripSetList(int control, const char *value) {
	this->Trace("TODO: NppExtensionAPI::UserStripSetList()\r\n");
}

const char *NppExtensionAPI::UserStripValue(int control) {
	this->Trace("TODO: NppExtensionAPI::UserStripValue()\r\n");
	return "NppExtensionAPI::UserStripValue";
}
