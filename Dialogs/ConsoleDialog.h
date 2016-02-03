#ifndef _CONSOLEDIALOG_H
#define _CONSOLEDIALOG_H

#include "DockingDlgInterface.h"
#include "ConsoleInterface.h"

#include <list>
#include <map>

struct SCNotification;
struct NppData;

struct LineDetails;

class ConsoleDialog : public DockingDlgInterface
{
public:
	ConsoleDialog();
	ConsoleDialog(const ConsoleDialog& other);
	~ConsoleDialog();

	void initDialog(HINSTANCE hInst, NppData& nppData, ConsoleInterface *console);

	void doDialog();
	void hide();

	void writeText(size_t length, const char *text);
	void writeError(size_t length, const char *text);
	void clearText();
	void setPrompt(const char *prompt);
	HWND getScintillaHwnd() { return m_sciOutput; }

	void giveInputFocus() { SetFocus(m_sciInput); }

	void runEnabled(bool enabled);

protected:
	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);

private:
	ConsoleDialog& operator = (const ConsoleDialog&); // assignment operator disabled

	void createOutputWindow(HWND hParentWindow);
	void createInputWindow(HWND hParentWindow);
	void runStatement();
	void stopStatement();

	static LRESULT CALLBACK inputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static LRESULT CALLBACK scintillaWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

	void historyNext();
	void historyPrevious();
	void historyAdd(const TCHAR *line);
	void historyEnd();

	LRESULT callScintilla(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) {	return ::SendMessage(m_sciOutput, message, wParam, lParam); }

	/* Styler functions */
	void onStyleNeeded(SCNotification* notification);
	void onHotspotClick(SCNotification* notification);
	bool parseLine(LineDetails *lineDetails);

	tTbData* m_data;
	HWND m_sciOutput;
	HWND m_sciInput;
	ConsoleInterface *m_console;
	std::string m_prompt;
	HICON m_hTabIcon;

	std::list<tstring> m_history;
	std::list<tstring>::iterator m_historyIter;
	std::map<idx_t, tstring> m_changes;
	idx_t m_currentHistory;
	bool m_runButtonIsRun;

	HMENU m_hContext;
};

enum ErrorLevel
{
	EL_UNSET,
	EL_INFO,
	EL_WARNING,
	EL_ERROR
};

struct LineDetails
{
public:
	char *line;
	size_t lineLength;
	idx_t errorLineNo;
	idx_t filenameStart;
	idx_t filenameEnd;
	ErrorLevel errorLevel;
};

#endif
