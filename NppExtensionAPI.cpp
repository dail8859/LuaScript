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

#include "NppExtensionAPI.h"
#include "Window.h"

NppExtensionAPI::NppExtensionAPI() {

}


NppExtensionAPI::~NppExtensionAPI() {

}

sptr_t NppExtensionAPI::Send(ExtensionAPI::Pane p, unsigned int msg, uptr_t wParam, sptr_t lParam) {
	return 0;
}

char *NppExtensionAPI::Range(ExtensionAPI::Pane p, int start, int end) {
	char *arr = new char[120];
	strcpy(arr, "NppExtensionAPI::Range");
	return arr;
}

void NppExtensionAPI::Remove(ExtensionAPI::Pane p, int start, int end) {

}
void NppExtensionAPI::Insert(ExtensionAPI::Pane p, int pos, const char *s) {

}

void NppExtensionAPI::Trace(const char *s) {
	MessageBoxA(NULL, s, "Trace", MB_OK);
}

std::string NppExtensionAPI::Property(const char *key) {
	return std::string("NppExtensionAPI::Property");
}

void NppExtensionAPI::SetProperty(const char *key, const char *val) {

}

void NppExtensionAPI::UnsetProperty(const char *key) {

}

uptr_t NppExtensionAPI::GetInstance() {
	return NULL;
}

void NppExtensionAPI::ShutDown() {

}

void NppExtensionAPI::Perform(const char *actions) {
	MessageBoxA(NULL, actions, "Perform", MB_OK);
}

void NppExtensionAPI::DoMenuCommand(int cmdID) {

}

void NppExtensionAPI::UpdateStatusBar(bool bUpdateSlowData) {

}

void NppExtensionAPI::UserStripShow(const char *description) {

}

void NppExtensionAPI::UserStripSet(int control, const char *value) {

}

void NppExtensionAPI::UserStripSetList(int control, const char *value) {

}

const char *NppExtensionAPI::UserStripValue(int control) {
	return "NppExtensionAPI::UserStripValue";
}
