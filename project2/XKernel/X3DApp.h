#ifndef __X3DAPP_H__
#define __X3DAPP_H__
#ifdef __cplusplus

#include <D3D9.h>
#include "DXUtil.h"
#include "XKernel.h"

enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESETFAILED         0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d


class X3DApp
{
// public functions ////////////////////////////////////////////////////////
public:
	X3DApp();
	~X3DApp();

	virtual HRESULT			Create( HINSTANCE hInstance );

	// Run the application
	void					Run();
	
	// Change the app title
	void					SetTitle(TCHAR *title);

	// First initialisation - called one time when the app start
	virtual bool			Startup() { return true; }
	
	// Last initialisation - called one time when the app finish
	virtual void			Shutdown() { }
	
	// Initialisation of the device objects, called when the device is lost
	virtual bool			InitDeviceObjects() { return true; }
			
	// Internal error handling function
    HRESULT					DisplayErrorMsg( HRESULT hr, DWORD dwType );

	// GFX MODE SETUP ///////////////////////////////////////////////////////
    // Internal functions to manage and render the 3D scene
	void					CheckDeviceConfirm( void );
	virtual HRESULT			ConfirmDevice(D3DCAPS9*,DWORD,D3DFORMAT)   { return S_OK; }
    static bool				ConfirmDeviceHelper( D3DCAPS9* pCaps, 
												 VertexProcessingType vertexProcessingType, 
												 D3DFORMAT backBufferFormat );
    void					BuildPresentParamsFromSettings();
    bool					FindBestWindowedMode( bool bRequireHAL, bool bRequireREF );
    bool					FindBestFullscreenMode( bool bRequireHAL, bool bRequireREF );

	bool					FindDefaultFullscreenMode( bool bRequireHAL, bool bRequireREF );

    HRESULT					ChooseInitialD3DSettings();
    HRESULT					Initialize3DEnvironment();
    HRESULT					HandlePossibleSizeChange();
    HRESULT					Reset3DEnvironment();
    HRESULT					ToggleFullscreen();
    HRESULT					ForceWindowed();
    HRESULT					UserSelectNewDevice();
    void					Cleanup3DEnvironment();
    HRESULT					Render3DEnvironment();
    virtual HRESULT			AdjustWindowForChange();    
	virtual HRESULT			RestorePrevResolution();
	
    virtual HRESULT			RestoreDeviceObjects()                     { return S_OK; }
	virtual HRESULT			InvalidateDeviceObjects()                  { return S_OK; }
    virtual HRESULT			DeleteDeviceObjects()                      { return S_OK; }

	// EVENT MANAGEMENT //////////////////////////////////
	virtual bool			MousePoll(MouseState *pState, ScrnPos *pPos) { return true; }
	virtual bool			MouseButton(_XMouseButton p_MB, bool bPushed) { return true; }	
	virtual void			OnMouseMove( WPARAM wparam, LPARAM lparam ){}
	virtual bool			MouseWheel( WPARAM wparam ){ return true; }
	virtual void			MouseLBtnDblClick( WPARAM wparam ){}
	virtual void			MouseRBtnDblClick( WPARAM wparam ){}
	virtual void			MouseMBtnDblClick( WPARAM wparam ){}

	virtual bool			KeyboardPoll(_XInput *pInput) { return true; }
	virtual bool			KeyDown(WPARAM wparam, LPARAM lparam) { return true; }
	virtual bool			KeyUp(WPARAM wparam, LPARAM lparam) { return true; }
	virtual void			MessageHandler( UINT message, WPARAM wparam, LPARAM lparam ){}
	virtual bool			WndMessageHandler( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ){ return true; }
		
	virtual void			OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam ){};
	virtual void			OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam ){};
	virtual void			OnUserMessage_UDPSocket(  UINT message, WPARAM wparam, LPARAM lparam ){};
	virtual void			OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam ){};
	virtual void			OnNexonMessangerEvent( WPARAM wparam, LPARAM lparam ){};
	
	virtual void			OnTimer(WPARAM wparam, LPARAM lparam) {}
	virtual void			OnPaint(WPARAM wparam, LPARAM lparam) {}
	virtual void			OnMove(WPARAM wparam, LPARAM lparam) {}

	virtual void			PaintHandler( void ){};

	virtual void			MCINotify( UINT message, WPARAM wparam, LPARAM lparam ){};	

	virtual void			OnWindowSizeChangeNotify( void ){};

	// SCENE MANAGEMENT //////////////////////////////////
	virtual void			Pause( bool bPause );
	virtual bool			Process() { return true; }
	virtual bool			Render() { return true; }

	// Cursor ////////////////////////////////////////////	
	virtual void			SetCursor( DWORD type, BOOL forcesetting = FALSE ){};
	virtual void			SetClickCursor( void ){};
	virtual void			ReleaseClickCursor( void ){};
	virtual void			ResetReadyCursor(  DWORD type  ){};
	virtual void			ReadyCursor(  DWORD type  ){};


	virtual BOOL			GetGammaRamp(void);	
	virtual void			ApplyGammaRamp(void);
	virtual void			RestoreGammaRamp( void );

// public members //////////////////////////////////////////////////////////
public:		
	_XDeviceEnum			m_d3dEnumeration;
	_X3DSettings 			m_d3dSettings;

	// Internal variables for the state of the app
    bool					m_bWindowed;    
	BOOL					m_bHasFocus;
    bool					m_bDeviceLost;
    bool					m_bMinimized;
    bool					m_bMaximized;
    bool					m_bIgnoreSizeChange;
    bool					m_bDeviceObjectsInited;
    bool					m_bDeviceObjectsRestored;
	
    // Internal variables used for timing
    bool					m_bFrameMoving;
    bool					m_bSingleStep;

	// Main objects used for creating and rendering the 3D scene
    D3DPRESENT_PARAMETERS	m_d3dpp;         // Parameters for CreateDevice/Reset
    HWND					m_hWnd;              // The main app window            
    D3DSURFACE_DESC			m_d3dsdBackBuffer;   // Surface desc of the backbuffer
    DWORD					m_dwCreateFlags;     // Indicate sw or hw vertex processing
    DWORD					m_dwWindowStyle;     // Saved window style for mode switches
    RECT					m_rcWindowBounds;    // Saved window bounds for mode switches
    RECT					m_rcWindowClient;    // Saved client area size for mode switches
	
    // Variables for timing    
    TCHAR					m_strDeviceStats[90];// String to hold D3D device stats
    TCHAR					m_strFrameStats[90]; // String to hold frame stats
	
    // Overridable variables for the app
    TCHAR					m_strWindowTitle[64];    // Title for the app's window    
    bool					m_bShowCursorWhenFullscreen; // Whether to show cursor when fullscreen
    bool					m_bClipCursorWhenFullscreen; // Whether to limit cursor pos when fullscreen
    bool					m_bStartFullscreen;  // Whether to start up the app in fullscreen mode
		
// protected functions /////////////////////////////////////////////////////
protected:

// protected members ///////////////////////////////////////////////////////
protected:
	bool					m_bDone;
	bool					m_bDeviceInitialised;

// private functions ///////////////////////////////////////////////////////
private:

// private members /////////////////////////////////////////////////////////
private:

};

#endif // __cplusplus
#endif // __X3DAPP_H__
