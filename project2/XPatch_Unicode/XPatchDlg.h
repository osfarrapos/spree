// XPatchDlg.h : header file
//

#if !defined(AFX_XPATCHDLG_H__149E8C4B_13EC_469B_8FD8_0A613614F0AC__INCLUDED_)
#define AFX_XPATCHDLG_H__149E8C4B_13EC_469B_8FD8_0A613614F0AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BitmapDialog.h"
#include "Label.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CXPatchDlg dialog

class CXPatchDlg : public CBitmapDialog
{
// Construction
public:
	void Patch( void );
	CXPatchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXPatchDlg)
	enum { IDD = IDD_XPATCH_DIALOG };
	CString	m_PatchPackageFilename;
	CString	m_ProgressString;
	//}}AFX_DATA

	CButtonST m_CloseButton;
	CButtonST m_MinimizeButton;

	CButtonST m_PackageOpenButton;
	CButtonST m_PatchButton;

	CLabel    m_SelectedPackageStatic;
	CLabel    m_ProgressStatic;
	
	CString   m_Message;

	DWORD     m_PatchNumber;
	DWORD     m_ServerPatchNumber;
	
	DWORD	  m_Version;
	DWORD	  m_ServerVersion;

	CString   m_ServerPackageVersionName;

	int       ParsingPatchScript( void );
	void      SplitMessage( CString& sourceString, CString& destString );
	void      SetProgressString( CString progressmessage );

	static STDMETHODIMP _XGetExeFileVersion(LPTSTR sFilename, DWORD& h_1,DWORD& h_2,DWORD& l_1,DWORD& l_2 );
	
	void	  LoadString( void );

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXPatchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXPatchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelectpackagebutton();
	afx_msg void OnUpdatebutton();	
	afx_msg void OnMinimizebutton();
	afx_msg void OnClosebutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPATCHDLG_H__149E8C4B_13EC_469B_8FD8_0A613614F0AC__INCLUDED_)
