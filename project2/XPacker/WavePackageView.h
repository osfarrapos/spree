#if !defined(AFX_WAVEPACKAGEVIEW_H__DA42A627_3281_45D7_B447_3004D89B13E7__INCLUDED_)
#define AFX_WAVEPACKAGEVIEW_H__DA42A627_3281_45D7_B447_3004D89B13E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WavePackageView.h : header file
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "WavePackage.h"

#include "SndTableList.h"

#include "ProgressDlg.h"

#include "XDSound.h"

/////////////////////////////////////////////////////////////////////////////
// CWavePackageView dialog

class CWavePackageView : public CDialog
{
// Construction
public:

	CImageList    m_imglist;

	CMenu		  m_SoundListMenu;

	CString       m_PathString;

	int			  m_SelectedSoundTableItem;
		
	CWavePackageView(CWnd* pParent = NULL);   // standard constructor

	CWavePackage  m_WavePackage;

	CSndTableList m_sndtablelist;

	LPSNDINFO	  m_psndInfo;
	DWORD         m_sndinfocount;

	BOOL loadWavePackage( LPCSTR filename );

	BOOL loadSoundListFile( LPCSTR filename );

	void setSoundList( void );

	void setSoundSceneInfo( DWORD index );

	BOOL saveSoundSceneFile( LPCSTR filename );

	_XWave m_WaveFile;
	
// Dialog Data
	//{{AFX_DATA(CWavePackageView)
	enum { IDD = IDD_WAVEPACKERVIEW };
	CListCtrl	m_SoundSceneFileListCtrl;	
	CListCtrl	m_SoundListControl;
	CListCtrl	m_WaveFileListCtrl;
	CString	m_Count;
	CString	m_TotalSize;
	CString	m_WavePackageFilename;
	CString	m_SoundListFileString;
	CString	m_SoundListCount;	
	CString	m_PackageFilename;
	CString	m_PackagePath;
	DWORD 	m_SceneCount;
	CString	m_SceneNameString;
	BOOL	m_AutoLoadWavePack;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWavePackageView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWavePackageView)
	afx_msg void OnItemchangedPackedfilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLoadwavpackfile();
	virtual BOOL OnInitDialog();
	afx_msg void OnUnpackallwavefile();
	afx_msg void OnUnpackwavefile();
	afx_msg void OnLoadwavlistfile();
	afx_msg void OnSavewavlistfile();
	afx_msg void OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInsertsoundlistitem();
	afx_msg void OnDeletesoundlistitem();
	afx_msg void OnKeydownFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOpensoundscenefile();
	afx_msg void OnItemchangedFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLoadlinkedwavpackfile();
	afx_msg void OnAutoloadwavepackcheck();
	afx_msg void OnDblclkSoundscenefilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnInsertsoundsceneitem();
	afx_msg void OnDeletesoundsceneitem();
	afx_msg void OnSavescenefile();
	afx_msg void OnDblclkPackedfilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangePackagefilenameedit();
	afx_msg void OnChangePathedit();
	afx_msg void OnSetworkforder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEPACKAGEVIEW_H__DA42A627_3281_45D7_B447_3004D89B13E7__INCLUDED_)
