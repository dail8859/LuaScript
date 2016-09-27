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

#ifndef IFACETABLEMIXER_H
#define IFACETABLEMIXER_H

#include "IFaceTable.h"
#include <vector>

class IFaceTableMixer : public IFaceTableInterface {
private:
	std::vector<IFaceTableInterface *> ifaces;

public:
	void AddIFaceTable(IFaceTableInterface *iface) { ifaces.push_back(iface); }

	// IFaceTableInterface
	const IFaceConstant *FindConstant(const char *name) const;
	const IFaceFunction *FindFunction(const char *name) const;
	const IFaceFunction *FindFunctionByConstantName(const char *name) const;
	const IFaceProperty *FindProperty(const char *name) const;
	int GetConstantName(int value, char *nameOut, unsigned nameBufferLen, const char *hint) const;
	const IFaceFunction *GetFunctionByMessage(int message) const;
	IFaceFunction GetPropertyFuncByMessage(int message) const;
};

#endif
