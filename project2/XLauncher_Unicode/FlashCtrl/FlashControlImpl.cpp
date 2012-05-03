#include "FlashControlImpl.h"

CFlashControlInterfaceImpl::CFlashControlInterfaceImpl()
{

}

CFlashControlInterfaceImpl::~CFlashControlInterfaceImpl()
{

}

void CFlashControlInterfaceImpl::initialize( HWND hwnd )
{
	m_ctrPlayer.initialize(hwnd);
}

HDC  CFlashControlInterfaceImpl::GetNWindowDC() 
{
	return m_ctrPlayer.GetNWindowDC();
}

void CFlashControlInterfaceImpl::SetDC( HDC dc ) 
{
	m_ctrPlayer.SetDC(dc);
}

void CFlashControlInterfaceImpl::PlayTimerFire() 
{
	m_ctrPlayer.PlayTimerFire();
}

BOOL CFlashControlInterfaceImpl::UpdateCursor() 
{
	return m_ctrPlayer.UpdateCursor();
}

void CFlashControlInterfaceImpl::CursorTimerFire()
{
	m_ctrPlayer.CursorTimerFire();
}


void CFlashControlInterfaceImpl::Repaint(RECT rc) 
{
	SRECT rect;
	rect.xmin = rc.left;
	rect.ymin = rc.top;
	rect.xmax = rc.right;
	rect.ymax = rc.bottom;

	m_ctrPlayer.Repaint(&rect);
}

void CFlashControlInterfaceImpl::HandleKeyDown(int key, int modifiers) 
{
//	m_ctrPlayer.HandleKeyDown(key, modifiers);
}

void CFlashControlInterfaceImpl::MouseMove(int x, int y, BOOL mouseIsDown) 
{
	m_ctrPlayer.MouseMove(x, y, mouseIsDown);
}

void CFlashControlInterfaceImpl::ControlOpen(char* filename ) 
{
	m_ctrPlayer.ControlOpen(filename);
}

void CFlashControlInterfaceImpl::FreeBuffer() 
{
	m_ctrPlayer.FreeBuffer();
}

void CFlashControlInterfaceImpl::MouseUp(int x, int y ) 
{
	m_ctrPlayer.MouseUp(x, y);
}

void CFlashControlInterfaceImpl::MouseDown(int x, int y )
{
	m_ctrPlayer.MouseDown( x, y);
}

CFlashControlInterface* GetFlashControl()
{ 
	return new CFlashControlInterfaceImpl;
}


void CFlashControlInterfaceImpl::ControlZoomIn()
{
	m_ctrPlayer.ControlZoomIn();
}

void CFlashControlInterfaceImpl::ControlZoomOut()
{
	m_ctrPlayer.ControlZoomOut();
}
void CFlashControlInterfaceImpl::ControlViewAll()
{
	m_ctrPlayer.ControlViewAll();
}
void CFlashControlInterfaceImpl::ControlHighQuality()
{
	m_ctrPlayer.ControlHighQuality();
}

void CFlashControlInterfaceImpl::SetLowQuality()
{
	m_ctrPlayer.SetLowQuality();
}

void CFlashControlInterfaceImpl::ControlPlay()
{
	m_ctrPlayer.ControlPlay();
}
void CFlashControlInterfaceImpl::ControlLoop()
{
	m_ctrPlayer.ControlLoop();
}
void CFlashControlInterfaceImpl::ControlRewind()
{
	m_ctrPlayer.ControlRewind();
}
void CFlashControlInterfaceImpl::ControlForward()
{
	m_ctrPlayer.ControlForward();
}
void CFlashControlInterfaceImpl::ControlBack()
{
	m_ctrPlayer.ControlBack();
}

void CFlashControlInterfaceImpl::ControlClose()
{
	m_ctrPlayer.ControlClose();
}