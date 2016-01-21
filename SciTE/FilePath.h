// SciTE - Scintilla based Text Editor
/** @file FilePath.h
 ** Definition of platform independent base class of editor.
 **/
// Copyright 1998-2005 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

extern const GUI::gui_char pathSepString[];
extern const GUI::gui_char pathSepChar;
extern const GUI::gui_char listSepString[];
extern const GUI::gui_char configFileVisibilityString[];
extern const GUI::gui_char fileRead[];
extern const GUI::gui_char fileWrite[];

#if defined(__unix__)
#include <limits.h>
#ifdef PATH_MAX
#define MAX_PATH PATH_MAX
#else
#define MAX_PATH 260
#endif
#endif

class FilePath;

typedef std::vector<FilePath> FilePathSet;

class FilePath {
	GUI::gui_string fileName;
public:
	FilePath(const GUI::gui_char *fileName_ = GUI_TEXT(""));
	FilePath(const GUI::gui_string &fileName_);
	FilePath(FilePath const &directory, FilePath const &name);
	void Set(const GUI::gui_char *fileName_);
	void Set(FilePath const &other);
	void Set(FilePath const &directory, FilePath const &name);
	void SetDirectory(FilePath directory);
	void Init();
	bool SameNameAs(const GUI::gui_char *other) const;
	bool SameNameAs(const FilePath &other) const;
	bool operator==(const FilePath &other) const;
	bool operator<(const FilePath &other) const;
	bool IsSet() const;
	bool IsUntitled() const;
	bool IsAbsolute() const;
	bool IsRoot() const;
	static int RootLength();
	const GUI::gui_char *AsInternal() const;
	std::string AsUTF8() const;
	FilePath Name() const;
	FilePath BaseName() const;
	FilePath Extension() const;
	FilePath Directory() const;
	void FixName();
	FilePath AbsolutePath() const;
	FilePath NormalizePath() const;
	static FilePath GetWorkingDirectory();
	bool SetWorkingDirectory() const;
	void List(FilePathSet &directories, FilePathSet &files);
	FILE *Open(const GUI::gui_char *mode) const;
	std::vector<char> Read() const;
	void Remove() const;
	time_t ModifiedTime() const;
	long GetFileLength() const;
	bool Exists() const;
	bool IsDirectory() const;
	bool Matches(const GUI::gui_char *pattern) const;
};

std::string CommandExecute(const GUI::gui_char *command, const GUI::gui_char *directoryForRun);
