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

#include "Extender.h"

class NppExtensionAPI : public ExtensionAPI
{
public:
	NppExtensionAPI();
	~NppExtensionAPI();

	virtual sptr_t Send(Pane p, unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	virtual char *Range(Pane p, int start, int end);
	virtual void Remove(Pane p, int start, int end);
	virtual void Insert(Pane p, int pos, const char *s);
	virtual void Trace(const char *s);
	virtual std::string Property(const char *key);
	virtual void SetProperty(const char *key, const char *val);
	virtual void UnsetProperty(const char *key);
	virtual uptr_t GetInstance();
	virtual void ShutDown();
	virtual void Perform(const char *actions);
	virtual void DoMenuCommand(int cmdID);
	virtual void UpdateStatusBar(bool bUpdateSlowData);
	virtual void UserStripShow(const char *description);
	virtual void UserStripSet(int control, const char *value);
	virtual void UserStripSetList(int control, const char *value);
	virtual const char *UserStripValue(int control);

};

