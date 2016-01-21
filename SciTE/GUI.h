// SciTE - Scintilla based Text Editor
/** @file GUI.h
 ** Interface to platform GUI facilities.
 ** Split off from Scintilla's Platform.h to avoid SciTE depending on implementation of Scintilla.
 ** Implementation in win32/GUIWin.cxx for Windows and gtk/GUIGTK.cxx for GTK+.
 **/
// Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef GUI_H
#define GUI_H

#define ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

namespace GUI {

class Point {
public:
	int x;
	int y;

	explicit Point(int x_=0, int y_=0) : x(x_), y(y_) {
	}
};

class Rectangle {
public:
	int left;
	int top;
	int right;
	int bottom;

	Rectangle(int left_=0, int top_=0, int right_=0, int bottom_ = 0) :
		left(left_), top(top_), right(right_), bottom(bottom_) {
	}
	bool Contains(Point pt) const {
		return (pt.x >= left) && (pt.x <= right) &&
			(pt.y >= top) && (pt.y <= bottom);
	}
	int Width() const { return right - left; }
	int Height() const { return bottom - top; }
	bool operator==(const Rectangle &other) const {
		return (left == other.left) &&
			(top == other.top) &&
			(right == other.right) &&
			(bottom == other.bottom);
	}
};

#if defined(GTK) || defined(__APPLE__)

// On GTK+ and OS X use UTF-8 char strings

typedef char gui_char;
typedef std::string gui_string;

#define GUI_TEXT(q) q

#else

// On Win32 use UTF-16 wide char strings

typedef wchar_t gui_char;
typedef std::wstring gui_string;

#define GUI_TEXT(q) L##q

#endif

gui_string StringFromUTF8(const char *s);
gui_string StringFromUTF8(const std::string &s);
std::string UTF8FromString(const gui_string &s);
gui_string StringFromInteger(long i);
gui_string HexStringFromInteger(long i);

typedef void *WindowID;
class Window {
protected:
	WindowID wid;
public:
	Window() : wid(0) {
	}
	Window &operator=(WindowID wid_) {
		wid = wid_;
		return *this;
	}
	WindowID GetID() const {
		return wid;
	}
	void SetID(WindowID wid_) {
		wid = wid_;
	}
	bool Created() const {
		return wid != 0;
	}
	void Destroy();
	bool HasFocus();
	Rectangle GetPosition();
	void SetPosition(Rectangle rc);
	Rectangle GetClientPosition();
	void Show(bool show=true);
	void InvalidateAll();
	void SetTitle(const gui_char *s);
};

typedef void *MenuID;
class Menu {
	MenuID mid;
public:
	Menu() : mid(0) {
	}
	MenuID GetID() const {
		return mid;
	}
	void CreatePopUp();
	void Destroy();
	void Show(Point pt, Window &w);
};

class ElapsedTime {
	long bigBit;
	long littleBit;
public:
	ElapsedTime();
	double Duration(bool reset=false);
};

struct ScintillaFailure {
	sptr_t status;
	explicit ScintillaFailure(sptr_t status_) : status(status_) {
	}
};

class ScintillaWindow : public Window {
	// Deleted so ScintillaWindow objects can not be copied
	ScintillaWindow(const ScintillaWindow &source) = delete;
	ScintillaWindow &operator=(const ScintillaWindow &) = delete;
	SciFnDirect fn;
	sptr_t ptr;
public:
	sptr_t status;
	ScintillaWindow() : fn(0), ptr(0), status() {
	}
	void SetID(WindowID wid_) {
		wid = wid_;
		fn = 0;
		ptr = 0;
		if (wid) {
			fn = reinterpret_cast<SciFnDirect>(
				Send(SCI_GETDIRECTFUNCTION, 0, 0));
			ptr = Send(SCI_GETDIRECTPOINTER, 0, 0);
		}
	}
	bool CanCall() const {
		return wid && fn && ptr;
	}
	int Call(unsigned int msg, uptr_t wParam=0, sptr_t lParam=0) {
		switch (msg) {
		case SCI_CREATEDOCUMENT:
		case SCI_CREATELOADER:
		case SCI_PRIVATELEXERCALL:
		case SCI_GETDIRECTFUNCTION:
		case SCI_GETDIRECTPOINTER:
		case SCI_GETDOCPOINTER:
		case SCI_GETCHARACTERPOINTER:
			throw ScintillaFailure(SC_STATUS_FAILURE);
		}
		if (!fn)
			throw ScintillaFailure(SC_STATUS_FAILURE);
		sptr_t retVal = fn(ptr, msg, wParam, lParam);
		status = fn(ptr, SCI_GETSTATUS, 0, 0);
		if (status > 0 && status < SC_STATUS_WARN_START)
			throw ScintillaFailure(status);
		return static_cast<int>(retVal);
	}
	sptr_t CallReturnPointer(unsigned int msg, uptr_t wParam=0, sptr_t lParam=0) {
		sptr_t retVal = fn(ptr, msg, wParam, lParam);
		status = fn(ptr, SCI_GETSTATUS, 0, 0);
		if (status > 0 && status < SC_STATUS_WARN_START)
			throw ScintillaFailure(status);
		return retVal;
	}
	int CallPointer(unsigned int msg, uptr_t wParam, void *s) {
		return Call(msg, wParam, reinterpret_cast<sptr_t>(s));
	}
	int CallString(unsigned int msg, uptr_t wParam, const char *s) {
		return Call(msg, wParam, reinterpret_cast<sptr_t>(s));
	}
	sptr_t Send(unsigned int msg, uptr_t wParam=0, sptr_t lParam=0);
	sptr_t SendPointer(unsigned int msg, uptr_t wParam=0, void *lParam=0);
};

bool IsDBCSLeadByte(int codePage, char ch);

}

#if defined(SCI_NAMESPACE)

// Scintilla namespace may or may not be turned on.
// If it is turned on, then make the structures usable without the Scintilla:: prefix

using Scintilla::Sci_CharacterRange;
using Scintilla::Sci_TextRange;
using Scintilla::Sci_TextToFind;
using Scintilla::SCNotification;

#endif

#endif
