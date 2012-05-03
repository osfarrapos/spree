// XLauncherDlg.h : header file
//

#if !defined(AFX_XLAUNCHERDLG_H__94D3F57F_8321_4EB8_8382_4DD5C10F5CA2__INCLUDED_)
#define AFX_XLAUNCHERDLG_H__94D3F57F_8321_4EB8_8382_4DD5C10F5CA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXLauncherDlg dialog

#include "DIB.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "Label.h"
#include "BitmapDialog.h"
#include "XRegistry.h"
#include "NoticeDialog.h"
#include "TextProgressCtrl.h"
#include "GradientProgressCtrl.h"
#include "BitmapSlider.h"
#include "FlashStatic.h"
#include "BitComboBox.h"
#include "SkinListCtrl.h"
#include "XBrowser.h"
#include "webbro.h"

typedef enum _XPATCHDOWNPROCESS
{
	_XPATCHDOWNPROCESS_PATCHSCRIPT = 0,
	_XPATCHDOWNPROCESS_PATCHFILE
};

typedef enum _XTABMODE
{
	_XTABMODE_UPDATEMESSAGE = 0,
	_XTABMODE_OPTIONVIEW,
	_XTABMODE_SELECTSERVER
};

#define _COUNT_MAX_GROUP_				18
#define _FOREST_STATE_STOP				-2
#define _FOREST_STATE_OFF				-1



#ifdef _XTESTSERVER

#define _XDEF_TESTSERVERSTRUCTINDICATOR		0xFD4E2A4C

typedef struct _XSERVERINFOSTRUCT
{
	DWORD	testserverindicator;
	char	cdnserverurl[64];
	DWORD	serverport;
	char	serveraddress[32];
	DWORD	patchserverport;
	char	patchserveraddress[32];	
} _XSERVERINFOSTRUCT;

#else

typedef struct _XSERVERINFOSTRUCT
{
	char	patchserveraddress[32];
	DWORD	patchserverport;
	char	cdnserverurl[64];
	char	serveraddress[32];
	DWORD	serverport;
} _XSERVERINFOSTRUCT;

#endif


#define _XADDRESSENCODEKEY	0xDC

class _XServerAddressManager
{
	
public:
	_XSERVERINFOSTRUCT	m_ServerInfo;
	
public:
	BOOL GetServerInfo( void );
	
	void SetServerAddress( LPSTR address );
	void SetPatchServerAddress( LPSTR address );
	
	void SetServerPort( DWORD port );
	void SetPatchServerPort( DWORD port );
	
	void SetCDNServerUrl( LPSTR url );
	
	void XOREncrypt( void );
};

extern _XServerAddressManager	g_ServerAddressManager;

#define _XPATCHVERSIONENCODEKEY		0xBF

#ifdef _XTESTSERVER

typedef struct _XPATCHVERSIONINFOSTRUCT
{
	DWORD	testserverindicator;
	DWORD	patchversion_2 : 16;	
	DWORD	patchversion_1 : 16;		
	DWORD	exeversion_3;
	DWORD	exeversion_2;
	DWORD	exeversion_1;	
	DWORD	exeversion_4;	
} _XPATCHVERSIONINFOSTRUCT;

#else

typedef struct _XPATCHVERSIONINFOSTRUCT
{
	DWORD	patchversion_1 : 16;
	DWORD	patchversion_2 : 16;	
	DWORD	exeversion_4;
	DWORD	exeversion_1;	
	DWORD	exeversion_3;
	DWORD	exeversion_2;
} _XPATCHVERSIONINFOSTRUCT;

#endif

class _XPatchVersionManager
{
	
public:
	_XPATCHVERSIONINFOSTRUCT	m_VersionInfo;
	
public:
	BOOL	GetVersionInfo( void );
	DWORD	GetExeVersion( void );
	
	void	SetPatchNumber( DWORD version );
	
	void	SetExeVersionnumber( DWORD v1, DWORD v2, DWORD v3, DWORD v4 );	
	
	void	XOREncrypt( void );
};


extern _XPatchVersionManager	g_PatchVersionManager;

class CXLauncherDlg : public CDialog
{

public:
	CXLauncherDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL			m_Initialized;

	CNoticeDialog	m_NoticeDialog;

	CString			m_PatchServerIPAddress;
	int				m_PatchServerPortNumber;

	BOOL            m_bPasv;
	BOOL            m_bUsePreconfig;
	BOOL            m_bUseProxy;
	CString         m_sProxy;
	double		    m_dbLimit;

	CLabel			m_CompanyStatic;
		
	BOOL			m_SerialPassed;

	CString         m_PackageVersionName;
	CString         m_ServerPackageVersionName;
	DWORD			m_Version;
	DWORD			m_ServerVersion;
	
	DWORD			m_LauncherVersion;
	DWORD			m_ServerLauncherVersion;

	BOOL			m_VersionCheck;
	BOOL			m_PatchDownloadComplete;
	CString         m_PatchFilename;
	CString         m_PatchInfoFilename;
	DWORD           m_PatchNumber;
	DWORD           m_ServerPatchNumber;

	//CString         m_Message;

	CString         m_PatchMySelf;
	BOOL            m_SelfPatchReady;
	
	WORD			m_BannerIndex;
	WORD			m_ServerBannerIndex;
	BOOL			m_BannerCheck;
	BOOL			m_BannerDownloadComplete;
	CString         m_BannerFilename;


	CDIB			m_BackBufferImage;
	CDIB			m_BorderImage;

	CDIB			m_NoticeSmallBorderImage;
	CDIB			m_UpdateSmallBorderImage;
	
	CDIB			m_UpdateTitleImage;
	BOOL			m_ViewUpdateInformationMode;

	CDIB			m_OptionBorderImage;
	CDIB			m_ServerSelectBorderImage;

	CDIB			m_PopupBorderImage;
	CDIB			m_ProgressBarBackBorderImage;
	CDIB			m_ProgressbarBackBorderTitleImage;

	CDIB			m_UpdatedMessageImage;

	int				m_MaxServerCount;
	CDIB			m_ServerNameImage[_COUNT_MAX_GROUP_];
	CDIB			m_ServerFullImage;
	CDIB			m_SelectedServerNameImage;
	int				m_ServerTrafficFactor[_COUNT_MAX_GROUP_];	
	int				m_NextServerTrafficFactor[_COUNT_MAX_GROUP_];
	int				m_SelectedServerIndex;
	BOOL			m_ViewServerList;

	_XTABMODE		m_CurrentTabMode;

	BOOL			m_UpdateComplete;
	BOOL			m_DownloadComplete;

	HBRUSH			m_brushHollow;
	
	int				m_ControlClickedCount;
	int				m_ShiftClickedCount;

	BOOL			m_TestServerConnectMode;

public:	
	int				CheckSerialNumber();
		
	int             VersionCheck( void );	
	int				ConnectVersionServer( BOOL viewmessage = TRUE );

#ifdef _XACCLAIM
	BOOL			LoadLanguageList( void );
#endif

	BOOL			ProcessParsingPatchScript( void );
	int             ParsingPatchScript( void );
	void            SplitMessage( CString& sourceString, CString& destString );	
	int             Update( void );
	int             PatchProcess( int patchindex );

	void			Run();

	void			InsertStringToProgressMessageListCtrl( LPCTSTR message, BOOL insert = TRUE );
	void			InsertStringToServerMessageListCtrl( LPCTSTR message, BOOL insert = TRUE );

	static STDMETHODIMP _XGetExeFileVersion(LPTSTR sFilename, DWORD& h_1,DWORD& h_2,DWORD& l_1,DWORD& l_2 );

	BOOL            SplitSerialnumber( LPCTSTR serialstring );

	void            ResetRegistry( void );

	void			RedrawAllControls( void );

	void			ChangeTab( _XTABMODE tabmode );

	void			RebuildServerTrafficGraph( void );
	void			SelectServer( int index );

	void			CheckInnerTester( void );
	void			SetTestServerMode( void );
	
	void			StartPatch( void );

	virtual bool	_OnBeforeNavigate2Noticeexplorer(CString	URL,CString	frame,void *post_data,int post_data_size, CString headers);
	virtual bool	_OnBeforeNavigate2Updateexplorer(CString	URL,CString	frame,void *post_data,int post_data_size, CString headers);

	void			CreateDeskTopShortCut( void );
	void			CreateLink(LPCTSTR lpszItem, LPCTSTR lpszFile, LPCTSTR lpszWorkingDir, int nFolder, BOOL seticonlocation );

public:	
// thread downloader...	
	bool			mf_downloading;
	bool			mf_continue;
	ULONG			m_progmax;
	int				m_percent;
	CString			m_statusstring;
	
	CString			m_downloadfilename;	
	CString			m_downloadtargetfilename;	

	FLOAT			m_OnePatchIncreaseFactor;

	_XPATCHDOWNPROCESS	m_patchprocess;

	static UINT		DownloadThread(void* param);

	void			StartDownload_ScriptFile( void );
	bool			XFileDownload_Method1( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName );
	bool			XFileDownload_Method2( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName );
	bool			XFileDownload_Method3( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName );
	bool			XFileDownload_Method4( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName );
	bool			XFileDownload_Method5( const TCHAR* IP, LPCTSTR URL, LPCTSTR FileName );

	void			LoadString( void );
	
// Dialog Data
	//{{AFX_DATA(CXLauncherDlg)
	enum { IDD = IDD_XLAUNCHER_DIALOG };
	CLabel			m_AutoRunStatic;
	CBitComboBox	m_AutoRunComboBox;
	CBitComboBox	m_LanguageComboBox;
	CLabel	m_LanguageStatic;
	CShadeButtonST	m_ChangeResolution;
	CLabel	m_DownloadMethodStatic;
	CBitComboBox	m_DownloadMethodCombo;
	CStatic	m_CologoStatic;
	CLabel	m_SoundOptionTitle;
	CLabel	m_GraphicOptionTitle;
	CLabel	m_OptionTitleStatic;
	CLabel	m_SoundSampleRateOptionStatic;
	CLabel	m_SoundDeviceOptionStatic;
	CLabel	m_SoundOutputOptionStatic;
	CLabel	m_CursorOptionStatic;
	CLabel	m_ShadowOptionStatic;
	CLabel	m_SightOptionStatic;
	CLabel	m_ModelOptionStatic;
	CLabel	m_TerrainOptionStatic;
	CLabel	m_UpdateTitleStatic;
	CLabel	m_UpdateStatic;
	CLabel	m_NoticeStatic;
	CBitComboBox		m_SoundMixRateCombo;
	CBitComboBox		m_SoundDriverTypeCombo;
	CBitComboBox		m_SoundOutputTypeCombo;
	CWebBrowser2		m_NoticeListBrowser;
	CWebBrowser2		m_UpdateListBrowser;
	CLabel				m_SelectedServerStatic;
	CBitComboBox		m_UseHardWareCursorCombo;
	CBitComboBox		m_ShadowQualityCombo;
	CBitComboBox		m_ViewSightRangeCombo;
	CBitComboBox		m_TerrainQualityComboBox;
	CBitComboBox		m_CharacterQualityComboBox;
	CFlashStatic		m_FlashStatic;
	CShadeButtonST			m_HidePatchInfoButton;
	CShadeButtonST			m_ShowPatchInfoButton;
	CShadeButtonST			m_ServerListRefreshButton;
	CShadeButtonST			m_ServerListCloseButton;
	CShadeButtonST			m_OptionOKButton;
	CShadeButtonST			m_OptionMiddleButton;
	CShadeButtonST			m_OptionLowButton;
	CShadeButtonST			m_OptionHighButton;
	CShadeButtonST			m_OptionCancelButton;
	CLabel				m_ProgressRateStatic;
	CLabel				m_LauncherTitleStatic;
	CLabel				m_UpdateMoreStatic;
	CLabel				m_NoticeMoreStatic;
	CShadeButtonST			m_CloseButton;
	CBitmapSlider		m_DownloadProgress;
	CBitmapSlider		m_TotalUpdateProgress;
	CBitmapSlider		m_UpdateProgress;
	CShadeButtonST			m_CancelButton;
	CShadeButtonST			m_SetupButton;
	CShadeButtonST			m_RunButton;
	CShadeButtonST			m_SelectServerButton;
	CLabel				m_ctlUpdateText;
	CShadeButtonST		m_JoinButton;	
	CSkinListCtrl		m_ServerMessageList;
	int		m_TextureQuality_Terrain;
	int		m_TextureQuality_Character;
	int		m_SightRange;
	int		m_TextureQuality_Shadow;
	int		m_UseHardwareCursor;	
	int		m_SoundOutPutType;
	int		m_SoundDriverType;
	int		m_SoundMixRate;
	int		m_DownloadMethod;
	int		m_SubLanguageIndex;
	int		m_AutoRunIndex;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXLauncherDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXLauncherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnCancel();
	afx_msg void OnSetupbutton();
	afx_msg void OnRunbutton();
	afx_msg void OnJoinbutton();
	afx_msg void OnSelectServerbutton();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClosebutton();
	afx_msg void OnNoticemorestatic();
	afx_msg void OnUpdatemorestatic();
	afx_msg void OnOptionHighButton();
	afx_msg void OnOptionMiddleButton();
	afx_msg void OnOptionLowButton();
	afx_msg void OnOptionOkButton();
	afx_msg void OnOptionCancelButton();
	afx_msg void OnServerrefreshButton();
	afx_msg void OnShowDetailPatchInfoButton();
	afx_msg void OnHideDetailPatchInfoButton();
	afx_msg void OnSelendokTerrainqualitycombo();
	afx_msg void OnSelendokCharacterqualitycombo();
	afx_msg void OnSelendokViewsightcombo();
	afx_msg void OnSelendokShadowqualitycombo();
	afx_msg void OnSelendokUsehardwarecursorcombo();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBeforeNavigate2Noticeexplorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	afx_msg void OnBeforeNavigate2Updateexplorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	//afx_msg void OnDocumentCompleteNoticeexplorer( LPDISPATCH pDisp, VARIANT FAR* URL );
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);	
	afx_msg void OnSelendokSoundoutputtypecombo();
	afx_msg void OnSelendokSounddrivercombo();
	afx_msg void OnSelendokMixratecombo();
	afx_msg void OnSelendokDownloadmethodcombo();
	afx_msg void OnChangeresolution();
	afx_msg void OnSelendokLanguagecombo();
	afx_msg void OnSelendokAutoruncombo();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


class CCallback : public IBindStatusCallback  
{
public:
	CCallback();
	~CCallback();
	
    // Pointer to the download progress dialog.
    CXLauncherDlg* m_pDlg;

	CString		   m_DownloadData;
	
    // The time when the download should timeout.
    BOOL  m_bUseTimeout;
    CTime m_timeToStop;
	
    // IBindStatusCallback methods.  Note that the only method called by IE
    // is OnProgress(), so the others just return E_NOTIMPL.
	
    STDMETHOD(OnStartBinding)(
        /* [in] */ DWORD dwReserved,
        /* [in] */ IBinding __RPC_FAR *pib)
	{ return E_NOTIMPL; }
	
    STDMETHOD(GetPriority)(
        /* [out] */ LONG __RPC_FAR *pnPriority)
	{ return E_NOTIMPL; }
	
    STDMETHOD(OnLowResource)(
        /* [in] */ DWORD reserved)
	{ return E_NOTIMPL; }
	
    STDMETHOD(OnProgress)(
        /* [in] */ ULONG ulProgress,
        /* [in] */ ULONG ulProgressMax,
        /* [in] */ ULONG ulStatusCode,
        /* [in] */ LPCWSTR wszStatusText);
		
		STDMETHOD(OnStopBinding)(
        /* [in] */ HRESULT hresult,
        /* [unique][in] */ LPCWSTR szError)
	{ return E_NOTIMPL; }
	
    STDMETHOD(GetBindInfo)(
        /* [out] */ DWORD __RPC_FAR *grfBINDF,
        /* [unique][out][in] */ BINDINFO __RPC_FAR *pbindinfo)
	{ return E_NOTIMPL; }
	
    STDMETHOD(OnDataAvailable)(
        /* [in] */ DWORD grfBSCF,
        /* [in] */ DWORD dwSize,
        /* [in] */ FORMATETC __RPC_FAR *pformatetc,
        /* [in] */ STGMEDIUM __RPC_FAR *pstgmed)
	{ return E_NOTIMPL; }
	
    STDMETHOD(OnObjectAvailable)(
        /* [in] */ REFIID riid,
        /* [iid_is][in] */ IUnknown __RPC_FAR *punk)
	{ return E_NOTIMPL; }
	
    // IUnknown methods.  Note that IE never calls any of these methods, since
    // the caller owns the IBindStatusCallback interface, so the methods all
    // return zero/E_NOTIMPL.
	
    STDMETHOD_(ULONG,AddRef)()
	{ return 0; }
	
    STDMETHOD_(ULONG,Release)()
	{ return 0; }
	
    STDMETHOD(QueryInterface)(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
	{ return E_NOTIMPL; }
};

void _XLog( BOOL insertlistview, LPCTSTR msg, ... );



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLAUNCHERDLG_H__94D3F57F_8321_4EB8_8382_4DD5C10F5CA2__INCLUDED_)
