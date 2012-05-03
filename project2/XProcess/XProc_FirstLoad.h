// XProc_FirstLoad.h: interface for the XProc_FirstLoad class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPROC_FIRSTLOAD_H_
#define _XPROC_FIRSTLOAD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class XMLPaser;

class XProc_FirstLoad : public _XProcess  
{

private: 
	POINT				messagecoord;
	TCHAR				m_Message[256];
	_XStatic			m_BackgroundStatic;
	_XTextureManager	m_TextureArchive;
	_XCamera			m_Player;
	
#ifdef _ACCLAIM_IGAADSYSTEM
	BOOL				m_AcclaimADMode;
#endif
	
public:
	XProc_FirstLoad();
	virtual ~XProc_FirstLoad();

	BOOL InitializeProcess( void );
	void DestroyProcess( void );
	
	bool InitDeviceObject( void );
	void ReleaseDeviceObject( void );
	
	void PauseProcess( void );
	void UnPauseProcess( void );
	
	void Draw( void );
	bool Process( void );
	
	void MessageProc( UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_UDPSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	bool OnKeyboardPoll(_XInput *pInput);
	bool OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool OnKeyUp(WPARAM wparam, LPARAM lparam);
	bool OnMousePoll(MouseState *pState, ScrnPos *pPos);
	bool OnMouseButton(_XMouseButton p_MB, bool bPushed);	
	void OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam );
	void OnPaint(WPARAM wparam, LPARAM lparam);
		
	BOOL InitialzeZoneTable( void );
	BOOL InitializeSpeedTreeWrapper( void );
	BOOL LoadCharacterModel( void );	
	BOOL LoadMonsterModel( void );
	BOOL LoadObjectModel( void );
	BOOL LoadEffectScript( void );
	BOOL LoadSceneList( void );
	BOOL LoadLifeSkillScript( void );
	BOOL LoadCashItemPackage( void );
	BOOL CreateMainInterface( void );

	BOOL LoadDummyObjectModel(void);

	BOOL LoadMonsterTable(void);

	bool Initialize3DSound( void );
	void Relase3DSound( void );
	BOOL PreLoadSoundEffect( void );
	void ReleaseSoundEffect( void );

	void SetMessage( LPTSTR message );

	BOOL InitializeBGMList(void);

	BOOL LoadQuestScript(void);
	
	BOOL LoadMatchScript(void);
	BOOL LoadNPCBuffScript(void);
	BOOL LoadFameScript(void);
};

#endif // !defined(_XPROC_FIRSTLOAD_H_)
