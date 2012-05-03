#include "ADBalloon.h"
#include "BrowserControl.h"

ADAD::BrowserControlPane* bcp;
const ADBallonParam* padp;

LRESULT CALLBACK ADBallonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CREATE:
			RECT r;
			r.left = 0;	r.right = padp->width;
			r.top = 0;	r.bottom = padp->height;
			bcp = new ADAD::BrowserControlPane(hwnd, padp->hInst, &r);
			bcp->NavigateTo(padp->URL);
			bcp->ActivateWindow(TRUE);

			SetTimer(hwnd, 0xAA, padp->timeout * 1000, NULL);
			break;

		case WM_NCHITTEST:
			return HTCLIENT;
		
		case WM_TIMER:
			if (wParam == 0xFF) KillTimer(hwnd, 0xFF);
			KillTimer(hwnd, 0xAA);
			DestroyWindow(hwnd);
			break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) DestroyWindow(hwnd);
			break;

		case WM_USER:
			SetTimer(hwnd, 0xFF, 10, NULL);
			break;

		case WM_CLOSE:
			bcp->ActivateWindow(FALSE);
			DestroyWindow(hwnd);
			break;
		
		case WM_DESTROY:
			delete bcp;
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	
	return 0;
}

int ShowADBalloon(const ADBallonParam& adp)
{
	::OleInitialize( NULL );
	padp = &adp;
	
	// Register Class

	WNDCLASSEX wcl =
	{ 
		sizeof(wcl), CS_OWNDC | CS_DBLCLKS, ADBallonProc, NULL, NULL, adp.hInst, NULL, NULL,
			(HBRUSH)COLOR_BACKGROUND, NULL, "NexonADBallon", NULL 
	};
	if (!RegisterClassEx(&wcl)) return 0;

	// Create and Show Window

	int		width  = adp.width + GetSystemMetrics(SM_CXDLGFRAME) * 2;
	int		height = adp.height + GetSystemMetrics(SM_CYDLGFRAME) * 2;
	RECT	sRect; GetWindowRect(GetDesktopWindow(), &sRect);

	HWND hwnd = 
		CreateWindowEx(WS_EX_TOPMOST, "NexonADBallon", "ad.", WS_POPUP | WS_THICKFRAME, 
		(sRect.right - width)/2, (sRect.bottom - height)/2, width, height, NULL, NULL, adp.hInst, NULL);

	if (!hwnd) return 0;
	
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// Message Loop

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	::OleUninitialize();
	return 1;
}
