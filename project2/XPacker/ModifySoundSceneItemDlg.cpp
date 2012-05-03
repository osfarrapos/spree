// ModifySoundSceneItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XPacker.h"
#include "ModifySoundSceneItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifySoundSceneItemDlg dialog


CModifySoundSceneItemDlg::CModifySoundSceneItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModifySoundSceneItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifySoundSceneItemDlg)
	m_LoadIDString = _T("");
	m_SoundClass = -1;
	m_Normal = FALSE;
	m_3D = FALSE;
	m_UseCpuRam = FALSE;
	m_Streaming = FALSE;
	m_LoadRam = FALSE;
	m_dwInsideConeAngle = 0;
	m_dwOutsideConeAngle = 0;
	m_OutsideVolumePerCent = 0;
	m_flMinDistance = 0.0f;
	m_flMaxDistance = 0.0f;
	m_InsideConAngleFlag = FALSE;
	m_OutsideConAngleFlag = FALSE;
	m_OutSideVolumePercentFlag = FALSE;
	m_MinimumDistanceFlag = FALSE;
	m_MaximumDistanceFlag = FALSE;
	//}}AFX_DATA_INIT
}


void CModifySoundSceneItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifySoundSceneItemDlg)
	DDX_CBString(pDX, IDC_LOADIDCOMBO, m_LoadIDString);
	DDV_MaxChars(pDX, m_LoadIDString, 8);
	DDX_CBIndex(pDX, IDC_SOUNDCLASSCOMBO, m_SoundClass);
	DDX_Check(pDX, IDC_NORMALCHECK, m_Normal);
	DDX_Check(pDX, IDC_3DCHECK, m_3D);
	DDX_Check(pDX, IDC_USECPURAMCHECK, m_UseCpuRam);
	DDX_Check(pDX, IDC_STREAMINGCHECK, m_Streaming);
	DDX_Check(pDX, IDC_LOADRAMCHECK, m_LoadRam);
	DDX_Text(pDX, IDC_INSIDECONEEDIT, m_dwInsideConeAngle);
	//DDV_MinMaxDWord(pDX, m_dwInsideConeAngle, 0, 360);
	DDX_Text(pDX, IDC_OUTSIDECONEEDIT, m_dwOutsideConeAngle);
	//DDV_MinMaxDWord(pDX, m_dwOutsideConeAngle, 0, 360);
	DDX_Text(pDX, IDC_OUTSIDEVOLUMEPERCENT, m_OutsideVolumePerCent);
	DDX_Text(pDX, IDC_MINIMUMDISTANCEEDIT, m_flMinDistance);
	DDX_Text(pDX, IDC_MAXIMUMDISTANCEEDIT, m_flMaxDistance);
	DDX_Check(pDX, IDC_INSIDECONANGLECHECK, m_InsideConAngleFlag);
	DDX_Check(pDX, IDC_OUTSIDECONANGLECHECK, m_OutsideConAngleFlag);
	DDX_Check(pDX, IDC_OUTSIDEVOLUMEPERCENTCHECK, m_OutSideVolumePercentFlag);
	DDX_Check(pDX, IDC_MINIMUMVOLUMEDISTANCECHECK, m_MinimumDistanceFlag);
	DDX_Check(pDX, IDC_MAXIMUMDISTANCECHECK, m_MaximumDistanceFlag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifySoundSceneItemDlg, CDialog)
	//{{AFX_MSG_MAP(CModifySoundSceneItemDlg)
	ON_BN_CLICKED(IDC_NORMALCHECK, OnNormalcheck)
	ON_BN_CLICKED(IDC_3DCHECK, On3dcheck)
	ON_BN_CLICKED(IDC_USECPURAMCHECK, OnUsecpuramcheck)
	ON_BN_CLICKED(IDC_STREAMINGCHECK, OnStreamingcheck)
	ON_BN_CLICKED(IDC_LOADRAMCHECK, OnLoadramcheck)
	ON_EN_CHANGE(IDC_INSIDECONEEDIT, OnChangeInsideconeedit)
	ON_EN_CHANGE(IDC_OUTSIDECONEEDIT, OnChangeOutsideconeedit)
	ON_EN_CHANGE(IDC_OUTSIDEVOLUMEPERCENT, OnChangeOutsidevolumepercent)
	ON_EN_CHANGE(IDC_MINIMUMDISTANCEEDIT, OnChangeMinimumdistanceedit)
	ON_EN_CHANGE(IDC_MAXIMUMDISTANCEEDIT, OnChangeMaximumdistanceedit)
	ON_BN_CLICKED(IDC_INSIDECONANGLECHECK, OnInsideconanglecheck)
	ON_BN_CLICKED(IDC_OUTSIDECONANGLECHECK, OnOutsideconanglecheck)
	ON_BN_CLICKED(IDC_OUTSIDEVOLUMEPERCENTCHECK, OnOutsidevolumepercentcheck)
	ON_BN_CLICKED(IDC_MINIMUMVOLUMEDISTANCECHECK, OnMinimumvolumedistancecheck)
	ON_BN_CLICKED(IDC_MAXIMUMDISTANCECHECK, OnMaximumdistancecheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifySoundSceneItemDlg message handlers

void CModifySoundSceneItemDlg::OnNormalcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
}

void CModifySoundSceneItemDlg::On3dcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );	
}

void CModifySoundSceneItemDlg::OnUsecpuramcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
}

void CModifySoundSceneItemDlg::OnStreamingcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
}

void CModifySoundSceneItemDlg::OnLoadramcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
}

void CModifySoundSceneItemDlg::OnChangeInsideconeedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CModifySoundSceneItemDlg::OnChangeOutsideconeedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CModifySoundSceneItemDlg::OnChangeOutsidevolumepercent() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CModifySoundSceneItemDlg::OnChangeMinimumdistanceedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CModifySoundSceneItemDlg::OnChangeMaximumdistanceedit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CModifySoundSceneItemDlg::OnInsideconanglecheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CModifySoundSceneItemDlg::OnOutsideconanglecheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CModifySoundSceneItemDlg::OnOutsidevolumepercentcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CModifySoundSceneItemDlg::OnMinimumvolumedistancecheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CModifySoundSceneItemDlg::OnMaximumdistancecheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
