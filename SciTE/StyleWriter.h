// SciTE - Scintilla based Text Editor
/** @file StyleWriter.h
 ** Simple buffered interface to the text and styles of a document held by Scintilla.
 **/
// Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef STYLEWRITER_H
#define STYLEWRITER_H

// Read only access to a document, its styles and other data
class TextReader {
	// Deleted so TextReader objects can not be copied
	TextReader(const TextReader &source) = delete;
	TextReader &operator=(const TextReader &) = delete;
protected:
	enum {extremePosition=0x7FFFFFFF};
	/** @a bufferSize is a trade off between time taken to copy the characters
	 * and retrieval overhead.
	 * @a slopSize positions the buffer before the desired position
	 * in case there is some backtracking. */
	enum {bufferSize=4000, slopSize=bufferSize/8};
	char buf[bufferSize+1];
	int startPos;
	int endPos;
	int codePage;

	GUI::ScintillaWindow &sw;
	int lenDoc;

	bool InternalIsLeadByte(char ch) const;
	void Fill(int position);
public:
	explicit TextReader(GUI::ScintillaWindow &sw_);
	char operator[](int position) {
		if (position < startPos || position >= endPos) {
			Fill(position);
		}
		return buf[position - startPos];
	}
	/** Safe version of operator[], returning a defined value for invalid position. */
	char SafeGetCharAt(int position, char chDefault=' ') {
		if (position < startPos || position >= endPos) {
			Fill(position);
			if (position < startPos || position >= endPos) {
				// Position is outside range of document
				return chDefault;
			}
		}
		return buf[position - startPos];
	}
	bool IsLeadByte(char ch) const {
		return codePage && InternalIsLeadByte(ch);
	}
	void SetCodePage(int codePage_) {
		codePage = codePage_;
	}
	bool Match(int pos, const char *s);
	int StyleAt(int position);
	int GetLine(int position);
	int LineStart(int line);
	int LevelAt(int line);
	int Length();
	int GetLineState(int line);
};

// Adds methods needed to write styles and folding
class StyleWriter : public TextReader {
	// Deleted so StyleWriter objects can not be copied
	StyleWriter(const StyleWriter &source) = delete;
	StyleWriter &operator=(const StyleWriter &) = delete;
protected:
	char styleBuf[bufferSize];
	int validLen;
	unsigned int startSeg;
public:
	explicit StyleWriter(GUI::ScintillaWindow &sw_);
	void Flush();
	int SetLineState(int line, int state);

	void StartAt(unsigned int start, char chMask=31);
	unsigned int GetStartSegment() const { return startSeg; }
	void StartSegment(unsigned int pos);
	void ColourTo(unsigned int pos, int chAttr);
	void SetLevel(int line, int level);
};

#endif
