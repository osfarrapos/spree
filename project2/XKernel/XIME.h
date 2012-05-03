// XIME.h: interface for the _XIME class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_XIME_H_)
#define _XIME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* IME Header files */ 
#include <windows.h>
#include <imm.h>
#include <objbase.h>
#include <dimm.h>

#include <string>
#include <vector>

#include "XGUIObject.h"
#include "XGUIControlCenter.h"
#include "XFont.h"


class _XIMEContainer;
typedef	void (__stdcall *_XIMESpecialKeyCallback)( _XIMEContainer* pIMEContainer );
typedef	BOOL (__stdcall *_XIMEStatusCallback)( _XIMEContainer* pIMEContainer, DWORD lastkey );

#define MAX_IMEBUFFERLENGTH			512
#define MAX_MULTILINE_COUNT			256
#define MAX_MULTILINE_LENGTH		256
#define MAX_CANDLE_COUNT			512
#define MAX_CANDLE_LENGTH			32
#define MAX_CANDLE_VIEWCOUNT		9
#define _XDEF_CARETTYPE				"_"
#define _XDEF_MAXPWSTRINGSIZE		32

#define CHT_IMEFILENAME1    "TINTLGNT.IME" // New Phonetic
#define CHT_IMEFILENAME2    "CINTLGNT.IME" // New Chang Jie
#define CHT_IMEFILENAME3    "MSTCIPHA.IME" // Phonetic 5.1
#define CHS_IMEFILENAME1    "PINTLGNT.IME" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    "MSSCIPYA.IME" // MSPY3 for OfficeXP

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor ) ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )	// New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )	// New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )	// New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )	// New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )	// New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )	// New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )	// New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
#define IMEID_CHS_VER41	( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )	// MSPY1.5	// SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42	( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )	// MSPY2	// Win2k/WinME
#define IMEID_CHS_VER53	( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )	// MSPY3	// WinXP

enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };
enum { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };

#define LCID_INVARIANT MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT)

#define GETPROCADDRESS( Module, APIName, Temp ) Temp = GetProcAddress( Module, #APIName ); if( Temp ) *(FARPROC*)&_##APIName = Temp 
#define PLACEHOLDERPROC( APIName ) _##APIName = Dummy_##APIName 

int GetCharsetFromLang(LANGID);
int	GetCodePageFromLang( LANGID langid );

class _XIMEKernel
{	
public:
	static _XIMEKernel& GetInstance( void );
		
	_XIMEKernel();
	~_XIMEKernel();
	
	_XIMEContainer*		GetIMEContainer( void );
	void				SetFocus(_XIMEContainer* imecontainer);	
	
	void				Clear( void );
	void				ResetCombinationBuffer( void );
	bool				GetKey(WPARAM wparam, LPARAM lparam);
	void				CheckToggleState(HWND hWnd);
	void				SetupImeApi(HWND hWnd);
	void				SetNativeMode( BOOL native );

	//CJK method
public:

	std::wstring		m_input;
	std::wstring		m_comp;
	std::wstring		m_reading;
	std::vector<BYTE>	m_candidate;
	
	int					m_ulStart;
	int					m_ulEnd;
	
	HKL					m_hkl;
	LANGID				m_langId;
	WORD				m_codePage;
	
	bool				m_bUnicodeIME;
	bool				m_bVerticalReading;
	bool				m_bVerticalCandidate;
	int					m_ImeState;
	wchar_t*			m_wszCurrIndicator;
	
	DWORD				m_dwId[2];

	wchar_t				m_IMEDescription[128];
	
	void				InitInput();	
	/* messages */ 
	void				OnInputLanguageChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool				OnComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool				OnEndComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool				OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool				OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam);
			
	int					GetComp(char* text, int len);
	int					GetReading(char* text, int len);
	int					GetReading(wchar_t* text, int len);
	
	int					GetCandidate(DWORD index, char* text, int len);
	int					GetCandidateCount();
	int					GetCandidateSelection();
	int					GetCandidatePageSize();
	int					GetCandidatePageStart();
	void				GetUnderLine(int* start, int* end);
	
	/* property */ 
	bool				IsVerticalReading(){ return m_bVerticalReading; }
	bool				IsVerticalCandidate(){ return m_bVerticalCandidate; }
	WORD				GetCodePage() { return m_codePage; }
	LANGID				GetLangId() { return m_langId; }
	int					GetCharSet() { return GetCharsetFromLang(m_langId); }
	wchar_t*			GetIndicator() { return m_wszCurrIndicator; }
	int					GetIndicatorMBCS(char* text, int len);
	int					GetImeState() { return m_ImeState; }

protected:	
	DWORD				GetImeId( UINT uIndex = 0 );
	bool				GetReadingWindowOrientation( DWORD dwId );
	void				GetPrivateReadingString(HWND hWnd);	
	void				CheckInputLocale( void );
private:
	_XIMEContainer*		m_pIMEContainer;
	
	static _XIMEKernel* ms_pThis;
};

#define _XIMEKERNEL		_XIMEKernel::GetInstance()


class _XIMM
{
public:
	_XIMM();
	virtual ~_XIMM();
	
	void			Initialize();
	
	INPUTCONTEXT*	LockIMC(HIMC hImc);
	BOOL			UnlockIMC(HIMC hImc);
	LPVOID			LockIMCC( HIMCC );
	BOOL			UnlockIMCC( HIMCC );
	
public:
	
	HINSTANCE		m_hDllImm32;
	HINSTANCE		m_hDllIme;

	// Empty implementation of the IMM32 API
    static	INPUTCONTEXT*	WINAPI Dummy_ImmLockIMC( HIMC ) { return NULL; }
    static BOOL				WINAPI Dummy_ImmUnlockIMC( HIMC ) { return FALSE; }
    static LPVOID			WINAPI Dummy_ImmLockIMCC( HIMCC ) { return NULL; }
    static BOOL				WINAPI Dummy_ImmUnlockIMCC( HIMCC ) { return FALSE; }
    static BOOL				WINAPI Dummy_ImmDisableTextFrameService( DWORD ) { return TRUE; }
    static LONG 			WINAPI Dummy_ImmGetCompositionStringW( HIMC, DWORD, LPVOID, DWORD ) { return IMM_ERROR_GENERAL; }
    static DWORD 			WINAPI Dummy_ImmGetCandidateListW( HIMC, DWORD, LPCANDIDATELIST, DWORD ) { return 0; }
    static HIMC 			WINAPI Dummy_ImmGetContext( HWND ) { return NULL; }
    static BOOL 			WINAPI Dummy_ImmReleaseContext( HWND, HIMC ) { return FALSE; }
    static HIMC 			WINAPI Dummy_ImmAssociateContext( HWND, HIMC ) { return NULL; }
    static BOOL 			WINAPI Dummy_ImmGetOpenStatus( HIMC ) { return 0; }
    static BOOL 			WINAPI Dummy_ImmSetOpenStatus( HIMC, BOOL ) { return 0; }
    static BOOL 			WINAPI Dummy_ImmGetConversionStatus( HIMC, LPDWORD, LPDWORD ) { return 0; }
    static HWND 			WINAPI Dummy_ImmGetDefaultIMEWnd( HWND ) { return NULL; }
    static UINT 			WINAPI Dummy_ImmGetIMEFileNameA( HKL, LPSTR, UINT ) { return 0; }
    static UINT 			WINAPI Dummy_ImmGetVirtualKey( HWND ) { return 0; }
    static BOOL 			WINAPI Dummy_ImmNotifyIME( HIMC, DWORD, DWORD, DWORD ) { return FALSE; }
    static BOOL 			WINAPI Dummy_ImmSetConversionStatus( HIMC, DWORD, DWORD ) { return FALSE; }
    static BOOL 			WINAPI Dummy_ImmSimulateHotKey( HWND, DWORD ) { return FALSE; }
    static BOOL 			WINAPI Dummy_ImmIsIME( HKL ) { return FALSE; }
	static UINT 			WINAPI Dummy_ImmGetDescriptionW( HKL, LPWSTR, UINT ) { return 0; }

    // Traditional Chinese IME
    static UINT				WINAPI Dummy_GetReadingString( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT ) { return 0; }
    static BOOL 			WINAPI Dummy_ShowReadingWindow( HIMC, BOOL ) { return FALSE; }
	
	// Function pointers: IMM32
    static INPUTCONTEXT*	(WINAPI * _ImmLockIMC)( HIMC );
    static BOOL				(WINAPI * _ImmUnlockIMC)( HIMC );
    static LPVOID			(WINAPI * _ImmLockIMCC)( HIMCC );
    static BOOL				(WINAPI * _ImmUnlockIMCC)( HIMCC );
    static BOOL 			(WINAPI * _ImmDisableTextFrameService)( DWORD );
    static LONG 			(WINAPI * _ImmGetCompositionStringW)( HIMC, DWORD, LPVOID, DWORD );
    static DWORD 			(WINAPI * _ImmGetCandidateListW)( HIMC, DWORD, LPCANDIDATELIST, DWORD );
    static HIMC 			(WINAPI * _ImmGetContext)( HWND );
    static BOOL 			(WINAPI * _ImmReleaseContext)( HWND, HIMC );
    static HIMC 			(WINAPI * _ImmAssociateContext)( HWND, HIMC );
    static BOOL 			(WINAPI * _ImmGetOpenStatus)( HIMC );
    static BOOL 			(WINAPI * _ImmSetOpenStatus)( HIMC, BOOL );
    static BOOL 			(WINAPI * _ImmGetConversionStatus)( HIMC, LPDWORD, LPDWORD );
    static HWND 			(WINAPI * _ImmGetDefaultIMEWnd)( HWND );
    static UINT 			(WINAPI * _ImmGetIMEFileNameA)( HKL, LPSTR, UINT );
    static UINT 			(WINAPI * _ImmGetVirtualKey)( HWND );
    static BOOL 			(WINAPI * _ImmNotifyIME)( HIMC, DWORD, DWORD, DWORD );
    static BOOL 			(WINAPI * _ImmSetConversionStatus)( HIMC, DWORD, DWORD );
    static BOOL 			(WINAPI * _ImmSimulateHotKey)( HWND, DWORD );
    static BOOL 			(WINAPI * _ImmIsIME)( HKL );
	static UINT 			(WINAPI * _ImmGetDescriptionW)( HKL, LPWSTR, UINT );

    // Function pointers: Traditional Chinese IME
    static UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
    static BOOL (WINAPI * _ShowReadingWindow)( HIMC, BOOL );
};

class _XIMEContainer
{
//private:
public:
	int			m_cursorX;

	wchar_t		m_text[MAX_IMEBUFFERLENGTH + 1];
	int			m_length;
	int			m_MaxLength;
	
	TCHAR		m_candleText[MAX_CANDLE_COUNT][MAX_CANDLE_LENGTH];
	int			m_candleLength;
	int			m_candleNum;
	
	wchar_t		m_comText[MAX_IMEBUFFERLENGTH + 1];			// combination buffer
	int			m_comLength;
		
	bool		m_bNewLine;
	bool		m_active;

	int			m_VKeyboardTypedCount;
	
public:
	
	_XIMEContainer();
	~_XIMEContainer();
	
	void		ResetContainer( void );
	void		ClearBuffer( void );
	void		SetFocus( void );
	BOOL		IsFocused( void );
	
	void		AddString(char* string, int _length, BOOL callchangedcallback = TRUE);
	void		AddUnicode(wchar_t w, BOOL callchangedcallback = TRUE);
	void		AddUnicodeString(wchar_t* string,int _length, BOOL callchangedcallback = TRUE);
	
	// input handling...
	void		ProcessDelKey( void );
	void		ProcessHomeKey( void );
	void		ProcessEndKey( void );
	void		ProcessBackSpaceKey( void );
	void		ProcessLeftKey( void );
	void		ProcessRightKey( void );
	void		ProcessEscapeKey( void );
	void		ProcessTabKey( void );
	void		ResetCandleList( void );
	
	// get..
	int			GetMaxLength( void );
	TCHAR*		GetText( void );
	wchar_t*	GetUnicodeText( void );
	wchar_t*	GetComText( void );
	int			GetComTextlength( void );
	TCHAR*		GetCandle( int candleindex );	
	int			GetCandleTextlength( void );
	int			GetCandleScrollIndex( void ){ return m_candleNum;	}
	int			GetLength( void );
	int			GetLength_ByMBCSSize( void );
	int			GetCursorX( void );
	int			GetCursorX_ByMBCSSize( void );
	bool		GetAvtive( void );
	bool		GetFocus( void );
	bool		IsCandle( void );
	bool		GetNewLine( void );
	int			GetWidth( wchar_t* pText );

	// set..
	void		SetComText(const wchar_t* text,int length);
	void		SetComTextBlank( void );
	
	void		SetCandleText(int candleindex, const TCHAR *candles);
	void		SetCandleNum(int num);
	void		SetCandleLength(int len);
	
	void		SetCursorX( int cursorx );
	void		SetCursorX_ByMBCSSize( int cursorx );

	void		SetMaxLength( int length )
	{
				if( length > MAX_IMEBUFFERLENGTH ) m_MaxLength = MAX_IMEBUFFERLENGTH;				
				else m_MaxLength = length;
	}
	void		SetNewLine( bool newline ){ m_bNewLine = newline; }

	void		SetHorizScrollMode( BOOL scroll )
	{
				m_HorizScrolling = scroll;
	}
	
	BOOL		CheckMousePosition( void );

	// Edit window control
private:
	BOOL					m_ShowWindow;
	BOOL					m_NoFocusedDraw;
	BOOL					m_ShowWindowRectangle;
	BOOL					m_PasswordEdit;
	BOOL					m_NumericEditMode;
	BOOL					m_NumericSeparationMode;
	BOOL					m_bWhiteLineRectagle;
	POINT					m_WindowPosition;
	SIZE					m_WindowSize;
	_XFONT_ALIGNTYPE		m_AlignType;
	D3DCOLOR				m_TextColor;
	D3DCOLOR				m_CaretColor;
	DWORD					m_CommandID;
	BOOL					m_HorizScrolling;
	int						m_ScrollPosition;
	_XIMEContainer*			m_pNextIMEContainer;
	_XIMESpecialKeyCallback	m_pReturnKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pEscapeKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pTabKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pUpArrowKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pDownArrowKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pLeftArrowKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pRightArrowKeyCallbackFunction;
	_XIMESpecialKeyCallback	m_pBackSpaceKeyCallbackFunction;
	_XIMESpecialKeyCallback m_pCapsLockKeyCallbackFunction;
	
	_XIMEStatusCallback		m_pDataChangeCallbackFunction;
	
public:	
	void		ShowWindow( BOOL show )
	{ 				
				m_ShowWindow = show; 
				if( !m_ShowWindow && (_XIMEKERNEL.GetIMEContainer() == this) )
				{
					_XIMEKERNEL.SetFocus( NULL );
				}
	}
	BOOL		GetShowStatus( void ){ return m_ShowWindow; }
	void		SetNoFocuseDraw( BOOL draw ){ m_NoFocusedDraw = draw; }
	void		ShowWindowRectAngle( BOOL show ){ m_ShowWindowRectangle = show;	}
	void		SetWhiteRectAngle( BOOL white ){ m_bWhiteLineRectagle = white;	}
	void		MoveWindow( int X, int Y );
	void		ResizeWindow( int width, int height );
	void		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	POINT		GetWindowPosition( void ){ return m_WindowPosition;	}
	SIZE		GetWindowSize( void ){ return m_WindowSize; }
	void		SetAlignType( _XFONT_ALIGNTYPE	aligntype ){ m_AlignType = aligntype; }
	void		SetTextColor( D3DCOLOR textcolor ){ m_TextColor = textcolor; }
	void		SetCaretColor( D3DCOLOR caretcolor ){ m_CaretColor = caretcolor; }
	void		SetCommandID( DWORD cmdid ){ m_CommandID = cmdid; }
	DWORD		GetCommandID( void ){ return m_CommandID; }	
	void		SetNumericEditMode( BOOL numericedit, BOOL numericseparationmode = FALSE )
	{
				m_NumericEditMode		= numericedit;
				m_NumericSeparationMode	= numericseparationmode;
	}
	BOOL		GetNumericEditMode( void ){ return m_NumericEditMode; }
	BOOL		GetNumericSeparationMode( void ){ return m_NumericSeparationMode; }
	
	_XIMEStatusCallback GetDataChangeCallBack( void )
	{
				return m_pDataChangeCallbackFunction;
	}
	void		SetDataChangeCallBack( _XIMEStatusCallback pDataChangeCallback )
	{	
				m_pDataChangeCallbackFunction = pDataChangeCallback; 
	}

	_XIMESpecialKeyCallback GetReturnKeyCallBack( void )
	{
				return m_pReturnKeyCallbackFunction;
	}
	void		SetReturnKeyCallback( _XIMESpecialKeyCallback pRetCallback )
	{	
				m_pReturnKeyCallbackFunction = pRetCallback; 
	}
	
	_XIMESpecialKeyCallback GetEscapeKeyCallBack( void )
	{
				return m_pEscapeKeyCallbackFunction;
	}
	void		SetEscapeKeyCallback( _XIMESpecialKeyCallback pESCCallback )
	{	
				m_pEscapeKeyCallbackFunction = pESCCallback; 
	}

	_XIMESpecialKeyCallback GetTabKeyCallBack( void )
	{
				return m_pTabKeyCallbackFunction;
	}
	void		SetTabKeyCallback( _XIMESpecialKeyCallback pTabCallback )
	{	
				m_pTabKeyCallbackFunction = pTabCallback; 
	}
	_XIMESpecialKeyCallback GetUpArrowKeyCallBack( void )
	{
		return m_pUpArrowKeyCallbackFunction;
	}
	void		SetUpArrowKeyCallback( _XIMESpecialKeyCallback pUpArrowCallback )
	{	
		m_pUpArrowKeyCallbackFunction = pUpArrowCallback; 
	}
	
	_XIMESpecialKeyCallback GetDownArrowKeyCallBack( void )
	{
		return m_pDownArrowKeyCallbackFunction;
	}
	void		SetDownArrowKeyCallback( _XIMESpecialKeyCallback pDownArrowCallback )
	{	
		m_pDownArrowKeyCallbackFunction = pDownArrowCallback; 
	}
	_XIMESpecialKeyCallback GetLeftArrowKeyCallBack( void )
	{
		return m_pLeftArrowKeyCallbackFunction;
	}
	void		SetLeftArrowKeyCallback( _XIMESpecialKeyCallback pLeftArrowCallback )
	{	
		m_pLeftArrowKeyCallbackFunction = pLeftArrowCallback; 
	}
	
	_XIMESpecialKeyCallback GetRightArrowKeyCallBack( void )
	{
		return m_pRightArrowKeyCallbackFunction;
	}
	void		SetRightArrowKeyCallback( _XIMESpecialKeyCallback pRightArrowCallback )
	{	
		m_pRightArrowKeyCallbackFunction = pRightArrowCallback; 
	}
	_XIMESpecialKeyCallback GetBackSpaceKeyCallBack( void )
	{
		return m_pBackSpaceKeyCallbackFunction;
	}
	void		SetBackSpaceKeyCallback( _XIMESpecialKeyCallback pBackSpaceCallback )
	{	
		m_pBackSpaceKeyCallbackFunction = pBackSpaceCallback; 
	}
	_XIMESpecialKeyCallback GetCapsLockKeyCallBack( void )
	{
		return m_pCapsLockKeyCallbackFunction;
	}
	void		SetCapsLockKeyCallback( _XIMESpecialKeyCallback pCapsLockCallback )
	{	
		m_pCapsLockKeyCallbackFunction = pCapsLockCallback; 
	}
	void		SetNextIMEContainerHandle( _XIMEContainer* nextimecontainer )
	{
				m_pNextIMEContainer = nextimecontainer;
	}
	void		SetPasswordMode( BOOL mode )
	{
				m_PasswordEdit = mode;
	}
	BOOL		GetPasswordMode( void )
	{
				return m_PasswordEdit;
	}
	

private:
	// character.. process.
	int			GetLeftString(wchar_t* target);		// 커서를 기반으로 왼쪽 문자들을 얻는다
	int			GetRightString(wchar_t* target);		// 커서를 기반으로 오른쪽 문자들을 얻는다.
	
};

#endif // !defined(_XIME_H_)
