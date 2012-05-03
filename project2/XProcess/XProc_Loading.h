// XProc_Loading.h: interface for the XProc_Loading class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPROC_LOADING
#define _XPROC_LOADING

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum _XACCLAIMADPHASE
{
	_XACCLAIMADPHASE_BILLBOARD = 0,
	_XACCLAIMADPHASE_WATCHANDWINBUTTON,
	_XACCLAIMADPHASE_WATCHANDWINADVIEW,
	_XACCLAIMADPHASE_WATCHANDWINRESULTVIEW,
	_XACCLAIMADPHASE_END
};

class XProc_Loading : public _XProcess  
{
public:
	XProc_Loading();
	virtual ~XProc_Loading();
							
public:
	BOOL InitializeProcess( void );
	void DestroyProcess( void );

	bool InitDeviceObject( void );
	void ReleaseDeviceObject( void );
	
	void PauseProcess( void );
	void UnPauseProcess( void );

	void Draw( void );
	bool Process( void );
	
	void MessageProc( UINT message, WPARAM wparam, LPARAM lparam );	
	void OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );	
	bool OnKeyboardPoll(_XInput *pInput);
	bool OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool OnKeyUp(WPARAM wparam, LPARAM lparam);
	bool OnMousePoll(MouseState *pState, ScrnPos *pPos);
	bool OnMouseButton(_XMouseButton p_MB, bool bPushed);
	void OnPaint(WPARAM wparam, LPARAM lparam);

	void UpdateProgress( int increase );

	void SetMessage( LPTSTR message );

	// Loading process private data section....	    	
	_XTextureManager	m_TextureArchive;

	_XCamera			m_Player;
	_XStatic			m_BackgroundStatic;
	_XStatic			m_ProgressStatic;
	_XStatic			m_ProgressBarStatic;

	POINT				messagecoord;
	TCHAR				m_Message[256];
	
	BOOL				m_DataLoaded;
	
	int					m_BacgroundTextureIndex;
	int					m_ProgressTextureIndex;

	int					m_Progress;

	DWORD				m_SiganalLastSendedTime;
	
#ifdef _ACCLAIM_IGAADSYSTEM
	_XACCLAIMADPHASE	m_AcclaimADPhase;
	_XButton			m_WatchAndWinButton;
	int					m_WatchAndWinResultViewTimer;
	FLOAT				m_MessageBoxAlphaLevel;

	
	_XImageStatic		m_TitleLeftBar;	
	_XImageStatic		m_TitleCenterBar;	
	_XImageStatic		m_TitleRightBar;
	_XImageStatic		m_ItemIconStatic;

	DWORD				m_AutoCancelCounter;

	BOOL				m_RecievedADResult;
	DWORD               m_LoadingAD_Gold;			// 지급 금액
	UCHAR               m_LoadingAD_ItemCnt;		// 아이템 개수 
	_ADItemInfo*		m_pLoadingAD_ADIteminfo;	// ucItemCnt 개수만큼 반복
	DWORD				m_ADRecievedResultAutoCancelCounter;
#endif
};

#endif // !defined(_XPROC_LOADING)
