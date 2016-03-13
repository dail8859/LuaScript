// SciTE - Scintilla based Text Editor
/** @file IFaceTable.h
 ** SciTE iface function and constant descriptors.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef IFACETABLE_H
#define IFACETABLE_H

#define ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

enum IFaceType {
	iface_void,
	iface_int,
	iface_length,
	iface_position,
	iface_colour,
	iface_bool,
	iface_keymod,
	iface_string,
	iface_stringresult,
	iface_cells, // This and everything else below is not "scriptable"
	iface_textrange,
	iface_findtext,
	iface_formatrange
};	

struct IFaceConstant {
	const char *name;
	int value;
};

struct IFaceFunction {
	const char *name;
	int value;
	IFaceType returnType;
	IFaceType paramType[2];
};

struct IFaceProperty {
	const char *name;
	int getter;
	int setter;
	IFaceType valueType;
	IFaceType paramType;
	
	IFaceFunction GetterFunction() const {
		IFaceFunction result = {"(property getter)",getter,valueType,{paramType,iface_void}};
		return result;
	}
	
	IFaceFunction SetterFunction() const {
		IFaceFunction result = {"(property setter)",setter,iface_void,{valueType, iface_void}};
		if (paramType != iface_void) {
			result.paramType[0] = paramType;
			if (valueType == iface_stringresult)
				result.paramType[1] = iface_string;
			else
				result.paramType[1] = valueType;
		}
		if ((paramType == iface_void) && ((valueType == iface_string) || (valueType == iface_stringresult))) {
			result.paramType[0] = paramType;
			if (valueType == iface_stringresult)
				result.paramType[1] = iface_string;
			else
				result.paramType[1] = valueType;
		}
		return result;
	}
};

inline bool IFaceTypeIsScriptable(IFaceType t, int index) {
	return t < iface_stringresult || (index == 1 && t == iface_stringresult);
}

inline bool IFaceTypeIsNumeric(IFaceType t) {
	return (t > iface_void && t < iface_bool);
}

inline bool IFaceFunctionIsScriptable(const IFaceFunction &f) {
	return IFaceTypeIsScriptable(f.paramType[0], 0) && IFaceTypeIsScriptable(f.paramType[1], 1);
}

inline bool IFacePropertyIsScriptable(const IFaceProperty &p) {
	return (((p.valueType > iface_void) && (p.valueType <= iface_stringresult) && (p.valueType != iface_keymod)) &&
		((p.paramType < iface_colour) || (p.paramType == iface_string) ||
		(p.paramType == iface_bool)) && (p.getter || p.setter));
}

class IFaceTableInterface {
public:
	virtual const IFaceConstant *FindConstant(const char *name) = 0;
	virtual const IFaceFunction *FindFunction(const char *name) = 0;
	virtual const IFaceFunction *FindFunctionByConstantName(const char *name) = 0;
	virtual const IFaceProperty *FindProperty(const char *name) = 0;
	virtual int GetConstantName(int value, char *nameOut, unsigned nameBufferLen, const char *hint) = 0;
	virtual const IFaceFunction *GetFunctionByMessage(int message) = 0;
	virtual const IFaceFunction *GetPropertyFuncByMessage(int message) = 0;
};

class IFaceTable : public IFaceTableInterface {
public:

public:
	IFaceTable(const char *_prefix,
		const IFaceFunction *const _functions, int _functionCount,
		const IFaceConstant *const _constants, int _constantCount,
		const IFaceProperty *const _properties, int _propertyCount) :
		prefix(_prefix),
		functions(_functions), functionCount(_functionCount),
		constants(_constants), constantCount(_constantCount),
		properties(_properties), propertyCount(_propertyCount)
	{}

	const char *prefix;

	const IFaceFunction *const functions;
	const IFaceConstant *const constants;
	const IFaceProperty *const properties;

	const int functionCount;
	const int constantCount;
	const int propertyCount;

	// IFaceTableInterface
	const IFaceConstant *FindConstant(const char *name);
	const IFaceFunction *FindFunction(const char *name);
	const IFaceFunction *FindFunctionByConstantName(const char *name);
	const IFaceProperty *FindProperty(const char *name);
	int GetConstantName(int value, char *nameOut, unsigned nameBufferLen, const char *hint);
	const IFaceFunction *GetFunctionByMessage(int message);
	const IFaceFunction *GetPropertyFuncByMessage(int message);
};

#endif
