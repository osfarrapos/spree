#ifndef __INPUT_H__
#define __INPUT_H__
#ifdef __cplusplus

#include "xbasedefs.h"
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>

#define KBBUFFER_SIZE			256*sizeof(unsigned char)

#define _XDEF_USEDIRECTINPUT

// Mouse button
typedef enum _XMouseButton
{
	_XmbLeft = 0,
		_XmbMiddle = 1,
		_XmbRight = 2,
} _XMouseButton;

typedef struct MouseState
{
	long AX;
	long AY;
	long AZ;
	
	bool bButton[3];
} MouseState;

typedef struct ScrnPos
{
	long x;
	long y;
	long z;
} ScrnPos;

extern class _XInput *gpInput;	// Create by the graphic mode setting functions

class _XInput  
{	
public:	
	_XInput(HWND hwnd, HINSTANCE hInst, BOOL bExclusive);
	BOOL CreateKeyboard();
	BOOL CreateMouse(int x, int y);
	BOOL CreateJoystick(long lJoyIndex);
	
	void Update();
	BOOL CheckKeyPress(DWORD Key);
	BOOL CheckKeyDown(DWORD Key); //Author : ¾çÈñ¿Õ //breif : »ç¿ë¾ÈÇÔ

	MouseState * GetMouseState();
	ScrnPos* GetMouseVirScreenPos();
	ScrnPos* GetMouseScreenPos();
	ScrnPos* GetMousePrevScreenPos();
	
	void SetMouseResolution(long lMouseRes);
	void SetScrnResolution(long width, long height);
	BOOL CheckMousePos( int x1, int y1, int x2, int y2 );
	
	void ClearKeyboardStack( void ){ memset( m_KbBuffer, 0, 256 ); }
	void ClearKey( DWORD Key, BOOL State ){ m_KbBuffer[Key] = State; }
	virtual ~_XInput();

	void SetMousePosition( int x, int y )
	{ 
		m_PrevScrnPos = m_ScrnPos;

		m_ScrnPos.x = x; 
		//m_ScrnPos.y = 0; 
		m_ScrnPos.z = y;
		
		m_MouseState.AX = m_ScrnPos.x - m_PrevScrnPos.x;
		m_MouseState.AY = m_ScrnPos.y - m_PrevScrnPos.y;
		m_MouseState.AZ = m_ScrnPos.z - m_PrevScrnPos.z;
	}
	void SetMouseLButton( bool click ){  m_MouseState.bButton[0] = click; }
	void SetMouseMButton( bool click ){  m_MouseState.bButton[2] = click; }
	void SetMouseRButton( bool click ){  m_MouseState.bButton[1] = click; }
	BOOL CheckMouseMovePos( void ){ return (m_ScrnPos.x == m_PrevScrnPos.x && 
											m_ScrnPos.y == m_PrevScrnPos.y &&
											m_ScrnPos.z == m_PrevScrnPos.z ); }

    LPDIRECTINPUTDEVICE8 GetMouse() { return m_lpDIMouse; }
private:
	unsigned char	m_KbBuffer[256];
	BYTE			m_byKeyTemp[256];
	MouseState		m_MouseState;
	ScrnPos			m_ScrnPos;
	ScrnPos			m_PrevScrnPos;
	long			m_lMouseRes;
	BOOL			m_bExclusive;
	HWND			m_hwnd;
		
	BOOL			m_bMouse;
	BOOL			m_bKeyboard;
	
	long			m_ScreenWidth;
	long			m_ScreenHeight;
	
	HINSTANCE		m_DIDLL;
	HRESULT			(WINAPI *m_dllDirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

	LPDIRECTINPUT8			m_lpDI;
	LPDIRECTINPUTDEVICE8	m_lpDIMouse;
	LPDIRECTINPUTDEVICE8	m_lpDIKeyboard;
	// add the joystick later;
	
	// Helper functions 
	void pShutdownKb();
	void pShutdownMouse();

	BOOL GetKeyboardState(DWORD& dwKey); //Author : ¾çÈñ¿Õ //breif : »ç¿ë¾ÈÇÔ
};

#endif // __cplusplus
#endif // __INPUT_H__
