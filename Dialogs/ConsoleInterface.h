// This file is part of LuaScript.
// 
// Original work Copyright (C)2010, 2011, 2014 Dave Brotherstone
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

class ConsoleInterface
{
public:
	ConsoleInterface() {};
	virtual ~ConsoleInterface() {};
	virtual void runStatement(const char *) = 0;
	virtual void openFile(const char *filename, size_t lineNumber) = 0;
};
