#if !defined(AFX_FTPOPTIONSDIALOG_H__01EC86FC_852E_4F80_B41D_A0F2D989D8C7__INCLUDED_)
#define AFX_FTPOPTIONSDIALOG_H__01EC86FC_852E_4F80_B41D_A0F2D989D8C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FTPOptionsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFTPOptionsDialog dialog

class CFTPOptionsDialog : public CDialog
{
// Construction
public:
	CFTPOptionsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFTPOptionsDialog)
	enum { IDD = IDD_FTPOPTIONDIALOG };
	CButton	m_UseHardwareCursorButton;
	BOOL	m_UseHardwareCursor;
	int		m_TextureQuality_Terrain;
	int		m_TextureQuality_Building;
	int		m_TextureQuality_Character;	
	int		m_MipLevel_Terrain;
	int		m_MipLevel_Building;
	int		m_MipLevel_Character;
	int		m_TextureQuality_Shadow;
	BOOL	m_ViewCharacterShadow;
	BOOL	m_ViewCloudShadow;
	int		m_SightRange;	
	int		m_ZBufferFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFTPOptionsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFTPOptionsDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnUsehardwarecursorcheck();
	afx_msg void OnSelendokTerrainqualitycombo();
	afx_msg void OnSelendokBuildingqualitycombo();
	afx_msg void OnSelendokCharacterqualitycombo();	
	afx_msg void OnSelendokTerrainmiplevelcombo();
	afx_msg void OnSelendokBuildingmiplevelcombo();
	afx_msg void OnSelendokCharactermiplevelcombo();
	afx_msg void OnSelendokCharactershadowqualitycombo();
	afx_msg void OnUsecharactershadowcheck();
	afx_msg void OnUsecloudshadowcheck();
	afx_msg void OnSetveryhighlevelbutton();
	afx_msg void OnSetmiddlelevelbutton();
	afx_msg void OnSetlowlevelbutton();
	afx_msg void OnSelendokSightrangecombo();	
	afx_msg void OnSelendokZbuffercombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPOPTIONSDIALOG_H__01EC86FC_852E_4F80_B41D_A0F2D989D8C7__INCLUDED_)
