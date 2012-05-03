#pragma once
#include <windows.h>

struct ADBallonParam
{
	ADBallonParam() { URL[0] = 0; }
	
	TCHAR		URL[512];
	int			width;
	int			height;
	DWORD		timeout;
	HINSTANCE	hInst;
};

int ShowADBalloon(const ADBallonParam& adp);
