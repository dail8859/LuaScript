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

#include <WindowsX.h>
#include "SettingsDialog.h"
#include "PluginDefinition.h"
#include "Hyperlinks.h"

const wchar_t *msg = TEXT("An option is blank (or all whitespace). If this is desired, it is recommended that you disable Active Commenting! Continue anyways?");
const wchar_t *help = TEXT("\
Format Keywords:\r\n\
$FILENAME - The current file name.\r\n\
$FUNCTION - The name of the function/method.\r\n\
$PARAM - Expands to a single function/method parameter. Any line containing this will get repeated for each parameter.\r\n\
$@ - Expands to the prefix character for Doxygen commands.\r\n\
$| - Marks the alignment position. This is only valid for lines containing $PARAM.\r\n");


INT_PTR CALLBACK inputDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwndDlg, IDC_EDIT_LANG));
		Edit_LimitText(GetDlgItem(hwndDlg, IDC_EDIT_LANG), 30);
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDOK:
			{
				wchar_t *text;
				int len = Edit_GetTextLength(GetDlgItem(hwndDlg, IDC_EDIT_LANG));

				text = new wchar_t[len + 1];
				Edit_GetText(GetDlgItem(hwndDlg, IDC_EDIT_LANG), text, len + 1);
				EndDialog(hwndDlg, (INT_PTR) text);
				return true;
			}
			default:
				return false;
		}
	case WM_CLOSE:
	case WM_DESTROY:
		EndDialog(hwndDlg, NULL);
		return true;
	}

	return false;
}

void SettingsDialog::init(HINSTANCE hInst, NppData nppData)
{
	_nppData = nppData;
	Window::init(hInst, nppData._nppHandle);
}

void SettingsDialog::doDialog()
{
	if (!isCreated()) create(IDD_SETTINGSDLG);
	goToCenter();

	//initParserSettings();
	//loadParserSettings();
	//updatePreview();
}

BOOL CALLBACK SettingsDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) 
	{
	case WM_INITDIALOG:
		{
			return true;
		}
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			switch(LOWORD(wParam))
			{
			case IDOK:
				display(false);
				return true;
			case IDCANCEL:
				display(false);
				return true;
			}
		}
	default:
		return StaticDialog::dlgProc(_HSource, message, wParam, lParam);
	}

	return false;
}
