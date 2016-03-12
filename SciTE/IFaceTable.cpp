// SciTE - Scintilla based Text Editor
/** @file IFaceTable.cxx
 ** SciTE iface function and constant descriptors.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include "IFaceTable.h"
#include <string.h>
#include <ctype.h>

int IFaceTable::FindConstant(const char *name) {
	int lo = 0;
	int hi = constantCount - 1;
	do {
		int idx = (lo+hi)/2;
		int cmp = strcmp(name, constants[idx].name);

		if (cmp > 0) {
			lo = idx + 1;
		} else if (cmp < 0) {
			hi = idx - 1;
		} else {
			return idx;
		}
	} while (lo <= hi);

	return -1;
}

int IFaceTable::FindFunction(const char *name) {
	int lo = 0;
	int hi = functionCount - 1;
	do {
		int idx = (lo+hi)/2;
		int cmp = strcmp(name, functions[idx].name);
		if (cmp > 0) {
			lo = idx + 1;
		} else if (cmp < 0) {
			hi = idx - 1;
		} else {
			return idx;
		}
	} while (lo <= hi);

	return -1;
}

int IFaceTable::FindFunctionByConstantName(const char *name) {
	if (strncmp(name, prefix, strlen(prefix))==0) {
		// This looks like a constant for an iface function.  This requires
		// a sequential search.  Take special care since the function names
		// are mixed case, whereas the constants are all-caps.

		for (int idx = 0; idx < functionCount; ++idx) {
			const char *nm = name+4;
			const char *fn = functions[idx].name;
			while (*nm && *fn && (*nm == toupper(*fn))) {
				++nm;
				++fn;
			}
			if (!*nm && !*fn) {
				return idx;
			}
		}
	}
	return -1;
}

int IFaceTable::FindProperty(const char *name) {
	int lo = 0;
	int hi = propertyCount - 1;
	do {
		int idx = (lo+hi)/2;
		int cmp = strcmp(name, properties[idx].name);

		if (cmp > 0) {
			lo = idx + 1;
		} else if (cmp < 0) {
			hi = idx - 1;
		} else {
			return idx;
		}
	} while (lo <= hi);

	return -1;
}

int IFaceTable::GetConstantName(int value, char *nameOut, unsigned nameBufferLen, const char *hint) {
	if (nameOut && nameBufferLen > 0) {
		*nameOut = '\0';
	}

	// Look in both the constants table and the functions table.  Start with functions.
	for (int funcIdx = 0; funcIdx < functionCount; ++funcIdx) {
		if (functions[funcIdx].value == value) {
			int len = static_cast<int>(strlen(functions[funcIdx].name)) + 4;
			if (nameOut && (static_cast<int>(nameBufferLen) > len)) {
				strcpy(nameOut, prefix);
				strcat(nameOut, functions[funcIdx].name);
				// fix case
				for (char *nm = nameOut + 4; *nm; ++nm) {
					if (*nm >= 'a' && *nm <= 'z') {
						*nm = static_cast<char>(*nm - 'a' + 'A');
					}
				}
				return len;
			} else {
				return -1 - len;
			}
		}
	}

	for (int constIdx = 0; constIdx < constantCount; ++constIdx) {
		if (constants[constIdx].value == value && (hint == NULL || strncmp(hint, constants[constIdx].name, strlen(hint)) == 0)) {
			int len = static_cast<int>(strlen(constants[constIdx].name));
			if (nameOut && (static_cast<int>(nameBufferLen) > len)) {
				strcpy(nameOut, constants[constIdx].name);
				return len;
			} else {
				return -1 - len;
			}
		}
	}

	return 0;
}
