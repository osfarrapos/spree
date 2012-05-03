// FTPOptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XLauncher.h"
#include "FTPOptionsDialog.h"

#include "XRegistry.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFTPOptionsDialog dialog

///////////////////////////////////////////////////////////////////////////////////////////////////
// Game Options
#define ID_REGSTR_BGMENABLE								  "Bgm"
#define ID_REGSTR_INTERFACESOUNDENABLE					  "InterfaceSoundEffect"
#define ID_REGSTR_SOUNDEFFECTENABLE					      "SoundEffect"
#define ID_REGSTR_3DSOUNDALGORITHM					      "3D Sound algorithm"
#define ID_REGSTR_3DSOUNDENABLE							  "Use 3d sound"

CFTPOptionsDialog::CFTPOptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFTPOptionsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFTPOptionsDialog)
	m_UseHardwareCursor = FALSE;
	m_TextureQuality_Terrain = 3;
	m_TextureQuality_Building = 3;
	m_TextureQuality_Character = 3;	
	m_MipLevel_Terrain = 1;
	m_MipLevel_Building = 1;
	m_MipLevel_Character = 1;
	m_TextureQuality_Shadow = 2;
	m_ViewCharacterShadow = FALSE;
	m_ViewCloudShadow = FALSE;
	m_SightRange = 1;	
	m_ZBufferFormat = 1;
	//}}AFX_DATA_INIT
}


void CFTPOptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFTPOptionsDialog)
	DDX_Control(pDX, IDC_USEHARDWARECURSORCHECK, m_UseHardwareCursorButton);
	DDX_Check(pDX, IDC_USEHARDWARECURSORCHECK, m_UseHardwareCursor);
	DDX_CBIndex(pDX, IDC_TERRAINQUALITYCOMBO, m_TextureQuality_Terrain);
	DDX_CBIndex(pDX, IDC_BUILDINGQUALITYCOMBO, m_TextureQuality_Building);
	DDX_CBIndex(pDX, IDC_CHARACTERQUALITYCOMBO, m_TextureQuality_Character);	
	DDX_CBIndex(pDX, IDC_TERRAINMIPLEVELCOMBO, m_MipLevel_Terrain);
	DDX_CBIndex(pDX, IDC_BUILDINGMIPLEVELCOMBO, m_MipLevel_Building);
	DDX_CBIndex(pDX, IDC_CHARACTERMIPLEVELCOMBO, m_MipLevel_Character);
	DDX_CBIndex(pDX, IDC_CHARACTERSHADOWQUALITYCOMBO, m_TextureQuality_Shadow);
	DDX_Check(pDX, IDC_USECHARACTERSHADOWCHECK, m_ViewCharacterShadow);
	DDX_Check(pDX, IDC_USECLOUDSHADOWCHECK, m_ViewCloudShadow);
	DDX_CBIndex(pDX, IDC_SIGHTRANGECOMBO, m_SightRange);	
	DDX_CBIndex(pDX, IDC_ZBUFFERCOMBO, m_ZBufferFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFTPOptionsDialog, CDialog)
	//{{AFX_MSG_MAP(CFTPOptionsDialog)
	ON_BN_CLICKED(IDC_USEHARDWARECURSORCHECK, OnUsehardwarecursorcheck)
	ON_CBN_SELENDOK(IDC_TERRAINQUALITYCOMBO, OnSelendokTerrainqualitycombo)
	ON_CBN_SELENDOK(IDC_BUILDINGQUALITYCOMBO, OnSelendokBuildingqualitycombo)
	ON_CBN_SELENDOK(IDC_CHARACTERQUALITYCOMBO, OnSelendokCharacterqualitycombo)	
	ON_CBN_SELENDOK(IDC_TERRAINMIPLEVELCOMBO, OnSelendokTerrainmiplevelcombo)
	ON_CBN_SELENDOK(IDC_BUILDINGMIPLEVELCOMBO, OnSelendokBuildingmiplevelcombo)
	ON_CBN_SELENDOK(IDC_CHARACTERMIPLEVELCOMBO, OnSelendokCharactermiplevelcombo)
	ON_CBN_SELENDOK(IDC_CHARACTERSHADOWQUALITYCOMBO, OnSelendokCharactershadowqualitycombo)
	ON_BN_CLICKED(IDC_USECHARACTERSHADOWCHECK, OnUsecharactershadowcheck)
	ON_BN_CLICKED(IDC_USECLOUDSHADOWCHECK, OnUsecloudshadowcheck)
	ON_BN_CLICKED(ID_SETVERYHIGHLEVELBUTTON, OnSetveryhighlevelbutton)
	ON_BN_CLICKED(ID_SETMIDDLELEVELBUTTON2, OnSetmiddlelevelbutton)
	ON_BN_CLICKED(ID_SETLOWLEVELBUTTON, OnSetlowlevelbutton)
	ON_CBN_SELENDOK(IDC_SIGHTRANGECOMBO, OnSelendokSightrangecombo)
	ON_CBN_SELENDOK(IDC_ZBUFFERCOMBO, OnSelendokZbuffercombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFTPOptionsDialog message handlers


void CFTPOptionsDialog::OnOK() 
{
	// TODO: Add extra validation here	

	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_USEHARDWARECURSOR, m_UseHardwareCursor );

	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_TERRAIN, m_TextureQuality_Terrain );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_WORLDOBJECT, m_TextureQuality_Building );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTER, m_TextureQuality_Character );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_MONSTER, m_TextureQuality_Character );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_NPC, m_TextureQuality_Character );
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MIPLEVEL_DETAILTTILE, m_MipLevel_Terrain );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MIPLEVEL_WORLDOBJECT, m_MipLevel_Building );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MIPLEVEL_CHARACTER, m_MipLevel_Character );
	
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTERSHADOW, m_TextureQuality_Shadow );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_VIEWCHARACTERSHADOW, m_ViewCharacterShadow );
	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_VIEWCLOUDSHADOW, m_ViewCloudShadow );	

	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SIGHTLEVEL_RANGE, m_SightRange );	

	g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_ZDEPTHFORMAT, (m_ZBufferFormat == 0) ? 1 : 2 );

	CDialog::OnOK();
}

BOOL CFTPOptionsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here		
	m_UseHardwareCursor			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_USEHARDWARECURSOR, 1, TRUE );
	
	m_TextureQuality_Terrain	= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_TERRAIN, 3, TRUE );
	m_TextureQuality_Building	= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_WORLDOBJECT, 3, TRUE );
	m_TextureQuality_Character	= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTER, 3, TRUE );
	m_MipLevel_Terrain			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MIPLEVEL_DETAILTTILE, 2, TRUE );
	m_MipLevel_Building			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MIPLEVEL_WORLDOBJECT, 1, TRUE );
	m_MipLevel_Character		= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_MIPLEVEL_CHARACTER, 1, TRUE );
	
	m_TextureQuality_Shadow		= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_QUALITYLEVEL_CHARACTERSHADOW, 2, TRUE );
	m_ViewCharacterShadow		= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_VIEWCHARACTERSHADOW, 1, TRUE );
	m_ViewCloudShadow			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_VIEWCLOUDSHADOW, 1, TRUE );	
	
	m_SightRange				= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SIGHTLEVEL_RANGE, 1, TRUE );
		
	m_ZBufferFormat				= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_ZDEPTHFORMAT, 2, TRUE );
	m_ZBufferFormat				-= 1;
	
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFTPOptionsDialog::OnUsehardwarecursorcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CFTPOptionsDialog::OnSelendokTerrainqualitycombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CFTPOptionsDialog::OnSelendokBuildingqualitycombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CFTPOptionsDialog::OnSelendokCharacterqualitycombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CFTPOptionsDialog::OnSelendokTerrainmiplevelcombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CFTPOptionsDialog::OnSelendokBuildingmiplevelcombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CFTPOptionsDialog::OnSelendokCharactermiplevelcombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CFTPOptionsDialog::OnSelendokCharactershadowqualitycombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CFTPOptionsDialog::OnUsecharactershadowcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CFTPOptionsDialog::OnUsecloudshadowcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CFTPOptionsDialog::OnSetveryhighlevelbutton() 
{
	// TODO: Add your control notification handler code here
	m_TextureQuality_Terrain = 3;
	m_TextureQuality_Building = 3;
	m_TextureQuality_Character = 3;	
	m_MipLevel_Terrain = 2;
	m_MipLevel_Building = 2;
	m_MipLevel_Character = 2;
	m_TextureQuality_Shadow = 3;	

	UpdateData(FALSE);
}

void CFTPOptionsDialog::OnSetmiddlelevelbutton() 
{
	// TODO: Add your control notification handler code here
	m_TextureQuality_Terrain = 1;
	m_TextureQuality_Building = 1;
	m_TextureQuality_Character = 1;	
	m_MipLevel_Terrain = 1;
	m_MipLevel_Building = 1;
	m_MipLevel_Character = 1;
	m_TextureQuality_Shadow = 1;	
	
	UpdateData(FALSE);	
}

void CFTPOptionsDialog::OnSetlowlevelbutton() 
{
	// TODO: Add your control notification handler code here
	m_TextureQuality_Terrain = 0;
	m_TextureQuality_Building = 0;
	m_TextureQuality_Character = 0;	
	m_MipLevel_Terrain = 0;
	m_MipLevel_Building = 0;
	m_MipLevel_Character = 0;
	m_TextureQuality_Shadow = 0;

	UpdateData(FALSE);	
}

void CFTPOptionsDialog::OnSelendokSightrangecombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CFTPOptionsDialog::OnSelendokZbuffercombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
