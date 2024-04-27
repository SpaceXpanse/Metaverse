// Copyright (c) Martin Schweiger
// Licensed under the MIT License

//=============================================================================
// AboutTab class
//=============================================================================

#include <windows.h>
#include <commctrl.h>
#include <io.h>
#include "SpaceXpanse.h"
#include "TabAbout.h"
#include "Util.h"
#include "Help.h"
#include "resource.h"
#include "cryptstring.h"
#include <string>
#include <vector>
#include <sstream>
//-----------------------------------------------------------------------------
// AboutTab class
HWND g_hListBox;
DWORD WINAPI RedirectToListBox(LPVOID lpParameter);

spacexpanse::AboutTab::AboutTab (const LaunchpadDialog *lp): LaunchpadTab (lp)
{
}

//-----------------------------------------------------------------------------

bool spacexpanse::AboutTab::OpenHelp ()
{
	OpenTabHelp ("tab_about");
	return true;
}

//-----------------------------------------------------------------------------

void spacexpanse::AboutTab::Create ()
{
	hTab = CreateTab (IDD_PAGE_ABT);

	char cbuf[256];
	SetWindowText (GetDlgItem (hTab, IDC_ABT_TXT_NAME), uscram(NAME1));
	SetWindowText (GetDlgItem (hTab, IDC_ABT_TXT_BUILDDATE), uscram(SIG4));
	strcpy (cbuf, uscram(SIG1B));
	SetWindowText (GetDlgItem (hTab, IDC_ABT_TXT_CPR), cbuf);
	strcpy (cbuf, uscram(SIG2));
	strcat (cbuf, "\r\n");
	strcat (cbuf, uscram(SIG5));
	strcat (cbuf, "\r\n");
	strcat (cbuf, uscram(SIG6));
	SetWindowText (GetDlgItem (hTab, IDC_ABT_TXT_WEBADDR), cbuf);
#ifndef INLINEGRAPHICS
	strcpy(cbuf, "D3D9Client module by Jarmo Nikkanen and Peter Schneider");
	SendDlgItemMessage(hTab, IDC_ABT_LBOX_COMPONENT, LB_ADDSTRING, 0, (LPARAM)cbuf);
#endif
	strcpy(cbuf, "XRSound module Copyright (c) Doug Beachy");
	SendDlgItemMessage(hTab, IDC_ABT_LBOX_COMPONENT, LB_ADDSTRING, 0, (LPARAM)cbuf);

	static int item[] = {
		IDC_ABT_GRP_SPACEXPANSE, IDC_ABT_GRP_WEB, IDC_ABT_ICON_DG, IDC_ABT_TXT_NAME,
		IDC_ABT_TXT_WEB, IDC_ABT_TXT_WEBADDR, IDC_ABT_TXT_CPR, IDC_ABT_TXT_LICENSE,
		IDC_ABT_GRP_COMPONENT, IDC_ABT_WEB, IDC_ABT_DISCLAIM, IDC_ABT_HELLO, IDC_ABT_CREDIT,
		IDC_ABT_TXT_BUILDDATE, IDC_ABT_LBOX_COMPONENT
	};

	RegisterItemPositions (item, 14);
}

//-----------------------------------------------------------------------------

INT_PTR spacexpanse::AboutTab::TabProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_ABT_WEB:
			ShellExecute (NULL, "open", "http://spacexpanse.org/", NULL, NULL, SW_SHOWNORMAL);
			return true;
		case IDC_ABT_DISCLAIM:
			DialogBoxParam (AppInstance(), MAKEINTRESOURCE(IDD_MSG), LaunchpadWnd(), AboutProc,
				IDT_DISCLAIMER);
			return TRUE;
		case IDC_ABT_HELLO:
			DialogBoxParam (AppInstance(), MAKEINTRESOURCE(IDD_HELLO), LaunchpadWnd(), AboutHello,
				IDT_DISCLAIMER);
			return TRUE;
		case IDC_ABT_CREDIT:
			::OpenHelp(hWnd, "html\\Credit.chm", "Credit");
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: AboutProc()
// Desc: Minimal message proc function for the about box
//-----------------------------------------------------------------------------
INT_PTR CALLBACK spacexpanse::AboutTab::AboutProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		SetWindowText(GetDlgItem(hWnd, IDC_MSG),
			uscram((char*)LockResource(LoadResource(NULL,
				FindResource(NULL, MAKEINTRESOURCE(lParam), "TEXT")))));
		return TRUE;
	case WM_COMMAND:
		if (IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam))
			EndDialog(hWnd, TRUE);
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: AboutHello()
// Desc: Minimal message proc function for the about box
//-----------------------------------------------------------------------------
INT_PTR CALLBACK spacexpanse::AboutTab::AboutHello(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		g_hListBox = GetDlgItem(hWnd, IDC_LIST1);
		CreateThread(NULL, 0, RedirectToListBox, (LPVOID)hWnd, 0, NULL);
		return TRUE;
	case WM_COMMAND:
		if (IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam))
			EndDialog(hWnd, TRUE);
		return TRUE;
	}
	return FALSE;
}


DWORD WINAPI RedirectToListBox(LPVOID lpParameter) {
	HINSTANCE hDLL = LoadLibrary("libhello.dll");
	if (hDLL != NULL) {
		//        MessageBox(NULL, L"load DLL!", L"Message", MB_OK | MB_ICONINFORMATION);
		typedef void (*SetListBoxHandle)(HWND hListBox);
		SetListBoxHandle setList = (SetListBoxHandle)GetProcAddress(hDLL, "SetListBoxHandle");
		if (setList != NULL) {
			setList(g_hListBox);
		}
		else {
			MessageBox(NULL, "Failed to find function in DLL!", "Message", MB_OK | MB_ICONINFORMATION);
		}
		typedef int (*dmain)(int, char**);
		dmain dMain = (dmain)GetProcAddress(hDLL, "dmain");
		if (dMain != NULL) {
			//  --spacexpanse_rpc_url = \"http://user1:pass1@127.0.0.1:18399\" --game_rpc_port = 28332 --storage_type = memory
			std::string input = "alabala --spacexpanse_rpc_url=http://user1:pass1@127.0.0.1:18399 --game_rpc_port=28332 --storage_type=memory";
			std::istringstream iss(input);
			std::vector<std::string> tokens;
			std::string token;
			while (iss >> token) {
				tokens.push_back(token);
			}

			std::vector<char*> argv1;
			for (const auto& token : tokens) {
				argv1.push_back(const_cast<char*>(token.c_str()));
			}
			argv1.push_back(nullptr); // Последният аргумент трябва да бъде nullptr, за да се отбележи края на масива

			int argc1 = argv1.size() - 1;
			char** argv_array = argv1.data();
			//            MessageBox(NULL, L"Point3!", L"Message", MB_OK | MB_ICONINFORMATION);
			dMain(argc1, argv_array);
			//            MessageBox(NULL, L"Point4!", L"Message", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(NULL, "Failed to find function in DLL!", "Message", MB_OK | MB_ICONINFORMATION);
		}
		FreeLibrary(hDLL);
	}
	else {
		MessageBox(NULL, "Failed to load DLL!", "Message", MB_OK | MB_ICONINFORMATION);
	}
	return 0;
}
