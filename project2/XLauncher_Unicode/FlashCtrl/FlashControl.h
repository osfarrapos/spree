#ifndef _FLASHCONTROLINTERFACE_
#define _FLASHCONTROLINTERFACE_

// Flash Control 의 클래스를 호출하기 위한 Virtual 함수 모음집 ( 버전 1 )
// 아.. 100만장 돌파??
#include "stdafx.h"

class CFlashControlInterface
{
public:
	CFlashControlInterface(){};
	virtual	~CFlashControlInterface(){};

public:
	// 앞으로 사용될 모든 가상함수들을 정의합니다.

	virtual	void initialize( HWND hwnd ) = 0;
	virtual	HDC  GetNWindowDC() = 0;				// This is a call for the Flash code to use
	virtual	void SetDC( HDC dc ) = 0;  // Only for use by the Window Message Handler
	virtual	void PlayTimerFire() = 0;
	virtual BOOL UpdateCursor() = 0;	
	virtual	void CursorTimerFire() = 0;

	virtual void Repaint(RECT sr) = 0;
	virtual void HandleKeyDown(int key, int modifiers) = 0;
	virtual void MouseMove(int x, int y, BOOL mouseIsDown) = 0;
	virtual void ControlOpen(char* filename ) = 0;
	virtual void FreeBuffer() = 0;
	virtual void MouseUp(int x, int y ) = 0;
	virtual void MouseDown(int x, int y ) = 0;

	// Context 메뉴를 위한 pure virtual 함수들
	virtual void ControlZoomIn() = 0;
	virtual void ControlZoomOut() = 0;
	virtual void ControlViewAll() = 0;
	virtual void ControlHighQuality() = 0;
	virtual void SetLowQuality() = 0;
	virtual void ControlPlay() = 0;
	virtual void ControlLoop() = 0;
	virtual void ControlRewind() = 0;
	virtual void ControlForward() = 0;
	virtual void ControlBack() = 0;
	virtual void ControlClose() = 0;
};

extern CFlashControlInterface* GetFlashControl();

struct LOGPALETTE256 {
    WORD			palVersion;
    WORD			palNumEntries;
    PALETTEENTRY	palEntry[256];
};

#endif
