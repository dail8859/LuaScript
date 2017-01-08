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

#include <Windows.h>
#include <Shellapi.h>
#include "AboutDialog.h"
#include "resource.h"
#include "Hyperlinks.h"
#include "Version.h"

INT_PTR CALLBACK abtDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
			ConvertStaticToHyperlink(hwndDlg, IDC_GITHUB);
			ConvertStaticToHyperlink(hwndDlg, IDC_README);
			SetWindowText(GetDlgItem(hwndDlg, IDC_VERSION), TEXT("LuaScript v") VERSION_TEXT TEXT(" ") VERSION_STAGE);
			return true;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
					DestroyWindow(hwndDlg);
					return true;
				case IDC_GITHUB:
					ShellExecute(hwndDlg, TEXT("open"), TEXT("https://github.com/dail8859/LuaScript/"), NULL, NULL, SW_SHOWNORMAL);
					return true;
				case IDC_README:
					ShellExecute(hwndDlg, TEXT("open"), TEXT("https://dail8859.github.io/LuaScript/"), NULL, NULL, SW_SHOWNORMAL);
					return true;
			}
		case WM_DESTROY:
			DestroyWindow(hwndDlg);
			return true;
	}
	return false;
}

void ShowAboutDialog(HINSTANCE hInstance, const wchar_t *lpTemplateName, HWND hWndParent) {
	HWND hSelf = CreateDialogParam((HINSTANCE)hInstance, lpTemplateName, hWndParent, abtDlgProc, NULL);

	// Go to center
	RECT rc;
	GetClientRect(hWndParent, &rc);
	POINT center;
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	center.x = rc.left + w / 2;
	center.y = rc.top + h / 2;
	ClientToScreen(hWndParent, &center);

	RECT dlgRect;
	GetClientRect(hSelf, &dlgRect);
	int x = center.x - (dlgRect.right - dlgRect.left) / 2;
	int y = center.y - (dlgRect.bottom - dlgRect.top) / 2;

	SetWindowPos(hSelf, HWND_TOP, x, y, (dlgRect.right - dlgRect.left), (dlgRect.bottom - dlgRect.top), SWP_SHOWWINDOW);
}
