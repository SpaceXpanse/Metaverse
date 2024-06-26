// Copyright (c) Martin Schweiger
// Licensed under the MIT License

//=============================================================================
// MetalogTab class
//=============================================================================

#include <windows.h>
#include <commctrl.h>
#include <io.h>
#include "SpaceXpanse.h"
#include "TabMetalog.h"
#include "Util.h"
#include "Help.h"
#include "resource.h"
#include "cryptstring.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/curl/include/curl.h"
//-----------------------------------------------------------------------------
// MetalogTab class
HWND g_hListBoxM;
DWORD WINAPI RedirectToListBoxM(LPVOID lpParameter);
void SentRodMessageM(HWND hWnd);

spacexpanse::MetalogTab::MetalogTab (const LaunchpadDialog *lp): LaunchpadTab (lp)
{
}

//-----------------------------------------------------------------------------

bool spacexpanse::MetalogTab::OpenHelp ()
{
	OpenTabHelp ("tab_metalog");
	return true;
}

//-----------------------------------------------------------------------------

void spacexpanse::MetalogTab::Create ()
{
	HWND hEdit1;
	HWND hEdit2;
	hTab = CreateTab (IDD_PAGE_MTL);
	g_hListBoxM = GetDlgItem(hTab, IDC_LIST1);
	CreateThread(NULL, 0, RedirectToListBoxM, (LPVOID)hTab, 0, NULL);
	hEdit1 = GetDlgItem(hTab, IDC_SCN_SAVE);
	SetWindowText(hEdit1, "p/");
	hEdit2 = GetDlgItem(hTab, IDC_SAVE_NAME);
	Edit_SetCueBannerText(hEdit2, L"Your message here");
	SendMessage(hEdit2, EM_LIMITTEXT, 160, 0);
//	SetWindowText(hEdit2, "Your message here");
}

//-----------------------------------------------------------------------------

BOOL spacexpanse::MetalogTab::Size (int w, int h)
{
	SetWindowPos (GetDlgItem (hTab, IDC_LIST1), NULL,
		7, 7, w - 20, h - 50,
		SWP_NOZORDER);
	SetWindowPos (GetDlgItem (hTab, IDC_SCN_SAVE), NULL,
		7, h-30, 70, 20,
		SWP_NOZORDER);
	SetWindowPos (GetDlgItem (hTab, IDC_SAVE_NAME), NULL,
		97, h-30, w - 160, 20,
		SWP_NOZORDER);
	SetWindowPos (GetDlgItem (hTab, IDC_MSG), NULL,
		w - 60, h-33, 50, 24,
		SWP_NOZORDER);

	return NULL;
}

INT_PTR spacexpanse::MetalogTab::TabProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_MSG:
			SentRodMessageM(hWnd);
			return TRUE;
		case IDC_MTL_CREDIT:
			::OpenHelp(hWnd, "html\\Credit.chm", "Credit");
			return TRUE;
		}
		break;
	}
	return FALSE;
}

struct MemoryStruct {
	char* memory;
	size_t size;
};

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	char* ptr = (char*) realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL) {
		// Out of memory!
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

void SentRodMessageM(HWND hWnd)
{
	CURL* curl;
	CURLcode res;
	struct MemoryStruct chunk;

	chunk.memory = (char*) malloc(1); 
	chunk.size = 0; 

	HWND hEdit1 = GetDlgItem(hWnd, IDC_SCN_SAVE);
	char currentUser[256];
	GetWindowText(hEdit1, currentUser, 256);
	HWND hEdit2 = GetDlgItem(hWnd, IDC_SAVE_NAME);
	char currentMess[1024];
	GetWindowText(hEdit2, currentMess, 1024);

	const char* url = "http://user1:pass1@127.0.0.1:18399/";
	std::string json_data = "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"name_update\", \"params\": [\"";
	json_data = json_data + currentUser;
	json_data = json_data + "\", \"{\\\"g\\\":{\\\"helloworld\\\":{\\\"m\\\":\\\"";
	json_data = json_data + currentMess;
	json_data = json_data + "\\\"}}}\"]}";

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl) {
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json;");

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_1_0);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			MessageBox(hWnd, "Request Failed!", "Error", MB_OK | MB_ICONERROR);
		}
		else {
			MessageBox(hWnd, chunk.memory, "JSON-RPC Response", MB_OK | MB_ICONINFORMATION);
		}

		// Cleanup
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
		free(chunk.memory);
	}

	curl_global_cleanup();
}

DWORD WINAPI RedirectToListBoxM(LPVOID lpParameter) {
	HINSTANCE hDLL = LoadLibrary("libhello.dll");
	if (hDLL != NULL) {
		//        MessageBox(NULL, L"load DLL!", L"Message", MB_OK | MB_ICONINFORMATION);
		typedef void (*SetListBoxHandle)(HWND hListBoxM);
		SetListBoxHandle setList = (SetListBoxHandle)GetProcAddress(hDLL, "SetListBoxHandle");
		if (setList != NULL) {
			setList(g_hListBoxM);
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
