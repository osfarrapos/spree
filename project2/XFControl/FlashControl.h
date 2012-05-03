#include "stdafx.h"
#include "d3dx9.h"

class CFlashControlInterface
{
public:
	CFlashControlInterface(){};
	~CFlashControlInterface(){};

public:
	// 앞으로 사용될 모든 가상함수들을 정의합니다.

	virtual	void initialize( HWND hwnd, HINSTANCE hinst, LPDIRECT3DTEXTURE9	pOffScreenTexture ) = 0;
	virtual	void SetOffScreenTextureObject( LPDIRECT3DTEXTURE9	pOffScreenTexture ) = 0;
	virtual	void SetResourceFilePtr( FILE* pResourcePointer, long iResourceSize ) = 0;
	virtual	HDC  GetNWindowDC() = 0;				// This is a call for the Flash code to use
	virtual	void SetDC( HDC dc ) = 0;  // Only for use by the Window Message Handler
	virtual	void PlayTimerFire() = 0;
	virtual BOOL UpdateCursor() = 0;	
	virtual	void CursorTimerFire() = 0;
	virtual void UpdateFullTexture() = 0;

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
	
	virtual	BOOL IsPlaying() = 0;
	virtual const char*	 GetTargetName() = 0;

};



