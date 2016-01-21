// SciTE - Scintilla based Text Editor
/** @file StringHelpers.cxx
 ** Implementation of widely useful string functions.
 **/
// Copyright 2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <string.h>
#include <stdio.h>

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Scintilla.h"

#include "GUI.h"
#include "StringHelpers.h"

bool StartsWith(GUI::gui_string const &s, GUI::gui_string const &start) {
	return (s.size() >= start.size()) &&
		(std::equal(s.begin(), s.begin() + start.size(), start.begin()));
}

bool StartsWith(std::string const &s, const char *start) {
	const size_t startSize = strlen(start);
	std::string sStart(start);
	return (s.size() >= startSize) &&
		(std::equal(s.begin(), s.begin() + startSize, sStart.begin()));
}

bool EndsWith(GUI::gui_string const &s, GUI::gui_string const &end) {
	return (s.size() >= end.size()) &&
		(std::equal(s.begin() + s.size() - end.size(), s.end(), end.begin()));
}

bool Contains(std::string const &s, char ch) {
	return s.find(ch) != std::string::npos;
}

int Substitute(std::wstring &s, const std::wstring &sFind, const std::wstring &sReplace) {
	int c = 0;
	size_t lenFind = sFind.size();
	size_t lenReplace = sReplace.size();
	size_t posFound = s.find(sFind);
	while (posFound != std::wstring::npos) {
		s.replace(posFound, lenFind, sReplace);
		posFound = s.find(sFind, posFound + lenReplace);
		c++;
	}
	return c;
}

int Substitute(std::string &s, const std::string &sFind, const std::string &sReplace) {
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

bool RemoveStringOnce(std::string &s, const char *marker) {
	size_t modText = s.find(marker);
	if (modText != std::string::npos) {
		s.erase(modText, strlen(marker));
		return true;
	}
	return false;
}

std::string StdStringFromInteger(int i) {
	char number[32];
	sprintf(number, "%0d", i);
	return std::string(number);
}

std::string StdStringFromSizeT(size_t i) {
	std::ostringstream strstrm;
	strstrm << i;
	return strstrm.str();
}

std::string StdStringFromDouble(double d, int precision) {
	char number[32];
	sprintf(number, "%.*f", precision, d);
	return std::string(number);
}

static int LowerCaseAZ(int c) {
	if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 'a';
	} else {
		return c;
	}
}

void LowerCaseAZ(std::string &s) {
	std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(LowerCaseAZ));
}

int CompareNoCase(const char *a, const char *b) {
	while (*a && *b) {
		if (*a != *b) {
			char upperA = MakeUpperCase(*a);
			char upperB = MakeUpperCase(*b);
			if (upperA != upperB)
				return upperA - upperB;
		}
		a++;
		b++;
	}
	// Either *a or *b is nul
	return *a - *b;
}

bool EqualCaseInsensitive(const char *a, const char *b) {
	return 0 == CompareNoCase(a, b);
}

bool isprefix(const char *target, const char *prefix) {
	while (*target && *prefix) {
		if (*target != *prefix)
			return false;
		target++;
		prefix++;
	}
	if (*prefix)
		return false;
	else
		return true;
}

unsigned int UTF32Character(const unsigned char *utf8) {
	unsigned char ch = utf8[0];
	unsigned int u32Char;
	if (ch < 0x80) {
		u32Char = ch;
	} else if (ch < 0x80 + 0x40 + 0x20) {
		u32Char = (ch & 0x1F) << 6;
		ch = utf8[1];
		u32Char += ch & 0x7F;
	} else if (ch < 0x80 + 0x40 + 0x20 + 0x10) {
		u32Char = (ch & 0xF) << 12;
		ch = utf8[1];
		u32Char += (ch & 0x7F) << 6;
		ch = utf8[2];
		u32Char += ch & 0x7F;
	} else {
		u32Char = (ch & 0x7) << 18;
		ch = utf8[1];
		u32Char += (ch & 0x3F) << 12;
		ch = utf8[2];
		u32Char += (ch & 0x3F) << 6;
		ch = utf8[3];
		u32Char += ch & 0x3F;
	}
	return u32Char;
}

/**
 * Convert a string into C string literal form using \a, \b, \f, \n, \r, \t, \v, and \ooo.
 */
std::string Slash(const std::string &s, bool quoteQuotes) {
	std::string oRet;
	for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
		if (*it == '\a') {
			oRet.append("\\a");
		} else if (*it == '\b') {
			oRet.append("\\b");
		} else if (*it == '\f') {
			oRet.append("\\f");
		} else if (*it == '\n') {
			oRet.append("\\n");
		} else if (*it == '\r') {
			oRet.append("\\r");
		} else if (*it == '\t') {
			oRet.append("\\t");
		} else if (*it == '\v') {
			oRet.append("\\v");
		} else if (*it == '\\') {
			oRet.append("\\\\");
		} else if (quoteQuotes && (*it == '\'')) {
			oRet.append("\\\'");
		} else if (quoteQuotes && (*it == '\"')) {
			oRet.append("\\\"");
		} else if (IsASCII(*it) && (*it < ' ')) {
			oRet.push_back('\\');
			oRet.push_back(static_cast<char>((*it >> 6) + '0'));
			oRet.push_back(static_cast<char>((*it >> 3) + '0'));
			oRet.push_back(static_cast<char>((*it & 0x7) + '0'));
		} else {
			oRet.push_back(*it);
		}
	}
	return oRet;
}

/**
 * Is the character an octal digit?
 */
static bool IsOctalDigit(char ch) {
	return ch >= '0' && ch <= '7';
}

/**
 * If the character is an hexa digit, get its value.
 */
static int GetHexaDigit(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	}
	if (ch >= 'A' && ch <= 'F') {
		return ch - 'A' + 10;
	}
	if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	}
	return -1;
}

/**
 * Convert C style \a, \b, \f, \n, \r, \t, \v, \ooo and \xhh into their indicated characters.
 */
unsigned int UnSlash(char *s) {
	char *sStart = s;
	char *o = s;

	while (*s) {
		if (*s == '\\') {
			s++;
			if (*s == 'a') {
				*o = '\a';
			} else if (*s == 'b') {
				*o = '\b';
			} else if (*s == 'f') {
				*o = '\f';
			} else if (*s == 'n') {
				*o = '\n';
			} else if (*s == 'r') {
				*o = '\r';
			} else if (*s == 't') {
				*o = '\t';
			} else if (*s == 'v') {
				*o = '\v';
			} else if (IsOctalDigit(*s)) {
				int val = *s - '0';
				if (IsOctalDigit(*(s + 1))) {
					s++;
					val *= 8;
					val += *s - '0';
					if (IsOctalDigit(*(s + 1))) {
						s++;
						val *= 8;
						val += *s - '0';
					}
				}
				*o = static_cast<char>(val);
			} else if (*s == 'x') {
				s++;
				int val = 0;
				int ghd = GetHexaDigit(*s);
				if (ghd >= 0) {
					s++;
					val = ghd;
					ghd = GetHexaDigit(*s);
					if (ghd >= 0) {
						s++;
						val *= 16;
						val += ghd;
					}
				}
				*o = static_cast<char>(val);
			} else {
				*o = *s;
			}
		} else {
			*o = *s;
		}
		o++;
		if (*s) {
			s++;
		}
	}
	*o = '\0';
	return static_cast<unsigned int>(o - sStart);
}

std::string UnSlashString(const char *s) {
	std::string sCopy(s, strlen(s) + 1);
	unsigned int len = UnSlash(&sCopy[0]);
	return sCopy.substr(0, len);
}

/**
 * Convert C style \0oo into their indicated characters.
 * This is used to get control characters into the regular expresion engine.
 */
static unsigned int UnSlashLowOctal(char *s) {
	char *sStart = s;
	char *o = s;
	while (*s) {
		if ((s[0] == '\\') && (s[1] == '0') && IsOctalDigit(s[2]) && IsOctalDigit(s[3])) {
			*o = static_cast<char>(8 * (s[2] - '0') + (s[3] - '0'));
			s += 3;
		} else {
			*o = *s;
		}
		o++;
		if (*s)
			s++;
	}
	*o = '\0';
	return static_cast<unsigned int>(o - sStart);
}

std::string UnSlashLowOctalString(const char *s) {
	std::string sCopy(s, strlen(s) + 1);
	unsigned int len = UnSlashLowOctal(&sCopy[0]);
	return sCopy.substr(0, len);
}
