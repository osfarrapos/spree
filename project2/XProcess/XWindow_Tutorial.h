// XWindow_Tutorial.h: interface for the _XWindow_Tutorial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TUTORIAL_H__73E27646_DEB0_47FD_9FBE_AE13E3C7DD31__INCLUDED_)
#define AFX_XWINDOW_TUTORIAL_H__73E27646_DEB0_47FD_9FBE_AE13E3C7DD31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

// 추가될 때마다 삽입 해야 함 ------------------=
// 추가시 m_TutorialTypeName과 _XTUTORIALTYPE 항상 비교
// 추가시 저장되는 함수도 처리 _XWindow_TutorialIcon::SaveTutorialData

typedef struct tagTutorialInfo
{
	TCHAR CharacterName[13];

	BOOL _XTutorial_001 : 1;
	BOOL _XTutorial_002 : 1;
	BOOL _XTutorial_003 : 1;
	BOOL _XTutorial_004 : 1;
	BOOL _XTutorial_005 : 1;
	BOOL _XTutorial_006 : 1;
	BOOL _XTutorial_007 : 1;
	BOOL _XTutorial_008 : 1;
	BOOL _XTutorial_009 : 1;
	BOOL _XTutorial_010 : 1;
	BOOL _XTutorial_011 : 1;
	BOOL _XTutorial_012 : 1;
	BOOL _XTutorial_013 : 1;
	BOOL _XTutorial_014 : 1;
	BOOL _XTutorial_015 : 1;
	BOOL _XTutorial_016 : 1;
	BOOL _XTutorial_017 : 1;
	BOOL _XTutorial_018 : 1;
	BOOL _XTutorial_019 : 1;
	BOOL _XTutorial_020 : 1;
	BOOL _XTutorial_021 : 1;
	BOOL _XTutorial_022 : 1;
	BOOL _XTutorial_023 : 1;
	BOOL _XTutorial_024 : 1;
	BOOL _XTutorial_025 : 1;
	BOOL _XTutorial_026 : 1;
	BOOL _XTutorial_027 : 1;
	BOOL _XTutorial_028 : 1;
	BOOL _XTutorial_029 : 1;
	BOOL _XTutorial_030 : 1;
	BOOL _XTutorial_031 : 1;
	BOOL _XTutorial_032 : 1;

	BOOL _XTutorial_033 : 1;
	BOOL _XTutorial_034 : 1;
	BOOL _XTutorial_035 : 1;
	BOOL _XTutorial_036 : 1;
	BOOL _XTutorial_037 : 1;
	BOOL _XTutorial_038 : 1;
	BOOL _XTutorial_039 : 1;
	BOOL _XTutorial_040 : 1;
	BOOL _XTutorial_041 : 1;	
	BOOL _XTutorial_042 : 1;
	BOOL _XTutorial_043 : 1;	
	BOOL _XTutorial_044 : 1;
	BOOL _XTutorial_045 : 1;
	BOOL _XTutorial_046 : 1;
	BOOL _XTutorial_047 : 1;
	BOOL _XTutorial_048 : 1;
	BOOL _XTutorial_049 : 1;
	BOOL _XTutorial_050 : 1;
	BOOL _XTutorial_051 : 1;
	BOOL _XTutorial_052 : 1;
	BOOL _XTutorial_Reserved : 12;

	void ConvertToByte( LPBOOL pByteMemory, int size )
	{
		memset( pByteMemory, 0, sizeof(size) );

		pByteMemory[0]  = (_XTutorial_001 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_002 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_003 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_004 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_005 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_006 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_007 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_008 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_009 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_010 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_011 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_012 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_013 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_014 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_015 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_016 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_017 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_018 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_019 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_020 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_021 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_022 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_023 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_024 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_025 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_026 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_027 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_028 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_029 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_030 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_031 ? 1 : 0); pByteMemory[0] <<= 1;
		pByteMemory[0] |= (_XTutorial_032 ? 1 : 0); 
		
		pByteMemory[1]  = (_XTutorial_033 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_034 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_035 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_036 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_037 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_038 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_039 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_040 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_041 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_042 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_043 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_044 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_045 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_046 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_047 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_048 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_049 ? 1 : 0); pByteMemory[1] <<= 1;
		pByteMemory[1] |= (_XTutorial_050 ? 1 : 0); pByteMemory[1] <<= 1;	
		pByteMemory[1] |= (_XTutorial_051 ? 1 : 0); pByteMemory[1] <<= 1;	
		pByteMemory[1] |= (_XTutorial_052 ? 1 : 0); 
	}

	void ConvertToBit( LPBOOL pByteMemory )
	{
		Reset();

		BOOL tempbyte = pByteMemory[0];
		_XTutorial_032 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_031 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_030 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_029 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_028 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_027 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_026 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_025 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_024 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_023 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_022 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_021 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_020 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_019 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_018 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_017 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_016 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_015 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_014 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_013 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_012 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_011 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_010 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_009 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_008 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_007 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_006 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_005 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_004 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_003 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_002 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_001 = tempbyte & 0x00001;   

		tempbyte = pByteMemory[1];
		_XTutorial_052 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_051 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_050 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_049 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_048 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_047 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_046 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_045 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_044 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_043 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_042 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_041 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_040 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_039 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_038 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_037 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_036 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_035 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_034 = tempbyte & 0x00001;   tempbyte >>= 1;
		_XTutorial_033 = tempbyte & 0x00001;   
	}

	void Reset()
	{
		memset( CharacterName, 0, sizeof(TCHAR)*13 );
		
		_XTutorial_001 = FALSE;
		_XTutorial_002 = FALSE;
		_XTutorial_003 = FALSE;
		_XTutorial_004 = FALSE;
		_XTutorial_005 = FALSE;
		_XTutorial_006 = FALSE;
		_XTutorial_007 = FALSE;
		_XTutorial_008 = FALSE;
		_XTutorial_009 = FALSE;
		_XTutorial_010 = FALSE;
		_XTutorial_011 = FALSE;
		_XTutorial_012 = FALSE;
		_XTutorial_013 = FALSE;
		_XTutorial_014 = FALSE;
		_XTutorial_015 = FALSE;
		_XTutorial_016 = FALSE;
		_XTutorial_017 = FALSE;
		_XTutorial_018 = FALSE;
		_XTutorial_019 = FALSE;
		_XTutorial_020 = FALSE;
		_XTutorial_021 = FALSE;
		_XTutorial_022 = FALSE;
		_XTutorial_023 = FALSE;
		_XTutorial_024 = FALSE;
		_XTutorial_025 = FALSE;
		_XTutorial_026 = FALSE;
		_XTutorial_027 = FALSE;
		_XTutorial_028 = FALSE;
		_XTutorial_029 = FALSE;
		_XTutorial_030 = FALSE;
		_XTutorial_031 = FALSE;
		_XTutorial_032 = FALSE;
		_XTutorial_033 = FALSE;
		_XTutorial_034 = FALSE;
		_XTutorial_035 = FALSE;
		_XTutorial_036 = FALSE;
		_XTutorial_037 = FALSE;
		_XTutorial_038 = FALSE;
		_XTutorial_039 = FALSE;
		_XTutorial_040 = FALSE;
		_XTutorial_041 = FALSE;
		_XTutorial_042 = FALSE;
		_XTutorial_043 = FALSE;
		_XTutorial_044 = FALSE;
		_XTutorial_045 = FALSE;
		_XTutorial_046 = FALSE;
		_XTutorial_047 = FALSE;
		_XTutorial_048 = FALSE;
		_XTutorial_049 = FALSE;
		_XTutorial_050 = FALSE;
		_XTutorial_051 = FALSE;
		_XTutorial_052 = FALSE;
		
		_XTutorial_Reserved = FALSE;
	};

} _XTUTORIALINFO;
 
typedef enum tagTutorialType
{
	_XTUTORIAL_NONE = 0,
	_XTUTORIAL_001,
	_XTUTORIAL_002,
	_XTUTORIAL_003,
	_XTUTORIAL_004,
	_XTUTORIAL_005,
	_XTUTORIAL_006,
	_XTUTORIAL_007,
	_XTUTORIAL_008,
	_XTUTORIAL_009,
	_XTUTORIAL_010,
	_XTUTORIAL_011,
	_XTUTORIAL_012,
	_XTUTORIAL_013,
	_XTUTORIAL_014,
	_XTUTORIAL_015,
	_XTUTORIAL_016,
	_XTUTORIAL_017,
	_XTUTORIAL_018,
	_XTUTORIAL_019,
	_XTUTORIAL_020,
	_XTUTORIAL_021,
	_XTUTORIAL_022,
	_XTUTORIAL_023,
	_XTUTORIAL_024,
	_XTUTORIAL_025,
	_XTUTORIAL_026,
	_XTUTORIAL_027,
	_XTUTORIAL_028,
	_XTUTORIAL_029,
	_XTUTORIAL_030,
	_XTUTORIAL_031,
	_XTUTORIAL_032,
	_XTUTORIAL_033,
	_XTUTORIAL_034,
	_XTUTORIAL_035,
	_XTUTORIAL_036,
	_XTUTORIAL_037,
	_XTUTORIAL_038,
	_XTUTORIAL_039,
	_XTUTORIAL_040,
	_XTUTORIAL_041,
	_XTUTORIAL_042,
	_XTUTORIAL_043,
	_XTUTORIAL_044,
	_XTUTORIAL_045,
	_XTUTORIAL_046,
	_XTUTORIAL_047,
	_XTUTORIAL_048,
	_XTUTORIAL_049,
	_XTUTORIAL_050,
	_XTUTORIAL_051,
	_XTUTORIAL_052,
	_XTUTORIAL_MAX
} _XTUTORIALTYPE;
// ---------------------------------------------=

#define _MAX_TUTORIALCOUNT 32

class _XWindow_Tutorial : public _XWindow  
{
protected:	
	_XTUTORIALTYPE		m_TutorialType;
	TCHAR				m_CurTutorialText[_MAX_TUTORIALCOUNT][128];
	_XImageStatic*		m_RoundEdgeImage[4];
	_XImageStatic*		m_LTitleImage;
	_XImageStatic*		m_MTitleImage;
	_XImageStatic*		m_RTitleImage;
	_XImageStatic*		m_Horizontal;
	_XImageStatic*		m_Vertically;
	_XImageStatic*		m_Mark;
	_XButton*			m_btnTutorialClose;
	_XButton*			m_pHelpbookCallButton;
	
	int					m_TextWidth;
	int					m_TextHeight;
	FLOAT				m_ScaleX;
	FLOAT				m_ScaleY;
	FLOAT				m_ScaleTitleBar;
	
	
	BOOL				LoadTutorialData();
public:
	_XWindow_Tutorial();
	virtual ~_XWindow_Tutorial();

	static _XWindow_Tutorial*	CreateTutorialDialogBox( _XWindowManager* pWindowManager );
	
	virtual void		Create( _XWindowManager* pWindowManager, DWORD windowid );	

	virtual BOOL		Initialize( void );
	virtual void		DestroyWindow( void );
	
	virtual void		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	

	void				ShowWindow(BOOL show);
	void				MoveWindow(int X, int Y);
	BOOL				SetTutorialType( _XTUTORIALTYPE type );
	
};

// 필수 튜토리얼 창 --------------------------------------------------------------------------=
class _XWindow_TutorialEx : public _XWindow_Tutorial
{
public:
	_XWindow_TutorialEx();
	virtual ~_XWindow_TutorialEx();

	static _XWindow_TutorialEx*	CreateTutorialDialogBox( _XWindowManager* pWindowManager );
	
	void				Create( _XWindowManager* pWindowManager, DWORD windowid );
	BOOL				Initialize( void );	
	void				DestroyWindow( void );
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
};
#endif // !defined(AFX_XWINDOW_TUTORIAL_H__73E27646_DEB0_47FD_9FBE_AE13E3C7DD31__INCLUDED_)
