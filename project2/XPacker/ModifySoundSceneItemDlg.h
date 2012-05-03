#if !defined(AFX_MODIFYSOUNDSCENEITEMDLG_H__DB8B9CBA_5068_4012_8779_7E1AA9BF9BB7__INCLUDED_)
#define AFX_MODIFYSOUNDSCENEITEMDLG_H__DB8B9CBA_5068_4012_8779_7E1AA9BF9BB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifySoundSceneItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModifySoundSceneItemDlg dialog

class CModifySoundSceneItemDlg : public CDialog
{
// Construction
public:
	CModifySoundSceneItemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModifySoundSceneItemDlg)
	enum { IDD = IDD_EDITSOUNDSCENEDIALOG };
	CString	m_LoadIDString;
	int		m_SoundClass;
	BOOL	m_Normal;
	BOOL	m_3D;
	BOOL	m_UseCpuRam;
	BOOL	m_Streaming;
	BOOL	m_LoadRam;
	DWORD	m_dwInsideConeAngle;
	DWORD	m_dwOutsideConeAngle;
	DWORD	m_OutsideVolumePerCent;
	float	m_flMinDistance;
	float	m_flMaxDistance;
	BOOL	m_InsideConAngleFlag;
	BOOL	m_OutsideConAngleFlag;
	BOOL	m_OutSideVolumePercentFlag;
	BOOL	m_MinimumDistanceFlag;
	BOOL	m_MaximumDistanceFlag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifySoundSceneItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModifySoundSceneItemDlg)
	afx_msg void OnNormalcheck();
	afx_msg void On3dcheck();
	afx_msg void OnUsecpuramcheck();
	afx_msg void OnStreamingcheck();
	afx_msg void OnLoadramcheck();
	afx_msg void OnChangeInsideconeedit();
	afx_msg void OnChangeOutsideconeedit();
	afx_msg void OnChangeOutsidevolumepercent();
	afx_msg void OnChangeMinimumdistanceedit();
	afx_msg void OnChangeMaximumdistanceedit();
	afx_msg void OnInsideconanglecheck();
	afx_msg void OnOutsideconanglecheck();
	afx_msg void OnOutsidevolumepercentcheck();
	afx_msg void OnMinimumvolumedistancecheck();
	afx_msg void OnMaximumdistancecheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYSOUNDSCENEITEMDLG_H__DB8B9CBA_5068_4012_8779_7E1AA9BF9BB7__INCLUDED_)
