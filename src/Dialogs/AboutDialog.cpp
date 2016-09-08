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
#include "PluginInterface.h"
#include "AboutDialog.h"
#include "resource.h"
#include "Hyperlinks.h"
#include "Version.h"

INT_PTR CALLBACK abtDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DECLARE_HANDLE(HDROP);
	switch(uMsg)
	{
	case WM_INITDIALOG:
		ConvertStaticToHyperlink(hwndDlg, IDC_GITHUB);
		ConvertStaticToHyperlink(hwndDlg, IDC_README);
		SetWindowText(GetDlgItem(hwndDlg, IDC_VERSION), TEXT("LuaScript v") VERSION_TEXT TEXT(" ") VERSION_STAGE);
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
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