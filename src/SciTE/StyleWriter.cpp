// SciTE - Scintilla based Text Editor
/** @file StyleWriter.cxx
 ** Simple buffered interface to the text and styles of a document held by Scintilla.
 **/
// Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <string>

#include "Scintilla.h"
#include "GUI.h"
#include "StyleWriter.h"

TextReader::TextReader(GUI::ScintillaWindow &sw_) :
	startPos(extremePosition),
	endPos(0),
	codePage(0),
	sw(sw_),
	lenDoc(-1) {
	buf[0] = 0;
}

bool TextReader::InternalIsLeadByte(char ch) const {
	return GUI::IsDBCSLeadByte(codePage, ch);
}

void TextReader::Fill(intptr_t position) {
	if (lenDoc == -1)
		lenDoc = sw.Call(SCI_GETTEXTLENGTH, 0, 0);
	startPos = position - slopSize;
	if (startPos + bufferSize > lenDoc)
		startPos = lenDoc - bufferSize;
	if (startPos < 0)
		startPos = 0;
	endPos = startPos + bufferSize;
	if (endPos > lenDoc)
		endPos = lenDoc;
	sw.Call(SCI_SETTARGETRANGE, startPos, endPos);
	sw.CallPointer(SCI_GETTARGETTEXT, 0, buf);
}

bool TextReader::Match(int pos, const char *s) {
	for (int i=0; *s; i++) {
		if (*s != SafeGetCharAt(pos+i))
			return false;
		s++;
	}
	return true;
}

int TextReader::StyleAt(intptr_t position) {
	return static_cast<unsigned char>(sw.Call(SCI_GETSTYLEAT, position, 0));
}

intptr_t TextReader::GetLine(intptr_t position) {
	return sw.Call(SCI_LINEFROMPOSITION, position, 0);
}

intptr_t TextReader::LineStart(intptr_t line) {
	return sw.Call(SCI_POSITIONFROMLINE, line, 0);
}

int TextReader::LevelAt(intptr_t line) {
	return static_cast<int>(sw.Call(SCI_GETFOLDLEVEL, line, 0));
}

intptr_t TextReader::Length() {
	if (lenDoc == -1)
		lenDoc = sw.Call(SCI_GETTEXTLENGTH, 0, 0);
	return lenDoc;
}

int TextReader::GetLineState(intptr_t line) {
	return static_cast<int>(sw.Call(SCI_GETLINESTATE, line));
}

StyleWriter::StyleWriter(GUI::ScintillaWindow &sw_) :
	TextReader(sw_),
	validLen(0),
	startSeg(0) {
	styleBuf[0] = 0;
}

int StyleWriter::SetLineState(intptr_t line, int state) {
	return static_cast<int>(sw.Call(SCI_SETLINESTATE, line, state));
}

void StyleWriter::StartAt(uintptr_t start, char chMask) {
	sw.Call(SCI_STARTSTYLING, start, chMask);
}

void StyleWriter::StartSegment(uintptr_t pos) {
	startSeg = pos;
}

void StyleWriter::ColourTo(uintptr_t pos, int chAttr) {
	// Only perform styling if non empty range
	if (pos != startSeg - 1) {
		if (validLen + (pos - startSeg + 1) >= bufferSize)
			Flush();
		if (validLen + (pos - startSeg + 1) >= bufferSize) {
			// Too big for buffer so send directly
			sw.Call(SCI_SETSTYLING, pos - startSeg + 1, chAttr);
		} else {
			for (uintptr_t i = startSeg; i <= pos; i++) {
				styleBuf[validLen++] = static_cast<char>(chAttr);
			}
		}
	}
	startSeg = pos+1;
}

void StyleWriter::SetLevel(intptr_t line, int level) {
	sw.Call(SCI_SETFOLDLEVEL, line, level);
}

void StyleWriter::Flush() {
	startPos = extremePosition;
	lenDoc = -1;
	if (validLen > 0) {
		sw.SendPointer(SCI_SETSTYLINGEX, validLen, styleBuf);
		validLen = 0;
	}
}
