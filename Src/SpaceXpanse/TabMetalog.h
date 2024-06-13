// Copyright (c) Martin Schweiger
// Licensed under the MIT License

//-----------------------------------------------------------------------------
// Launchpad tab definition: class MetalogTab
// Tab for "metalog" page
//-----------------------------------------------------------------------------

#ifndef __TABMETALOG_H
#define __TABMETALOG_H

#include "LpadTab.h"

namespace spacexpanse {

	class MetalogTab : public LaunchpadTab {
	public:
		MetalogTab(const LaunchpadDialog* lp);

		void Create();
		bool OpenHelp();

		INT_PTR TabProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		static INT_PTR CALLBACK MetalogProc(HWND, UINT, WPARAM, LPARAM);
		static INT_PTR CALLBACK MetalogHello(HWND, UINT, WPARAM, LPARAM);
	};

}

#endif // !__TABMETALOG_H
