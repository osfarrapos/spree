// ResChangeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "xlauncher.h"
#include "ResChangeDialog.h"


#include <D3D9.h>
#include "DXUtil.h"
#include "D3DUtil.h"
#include "D3DEnumeration.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern TCHAR g_InterfaceString[100][1024];
extern void	_XLog( BOOL insertlistview, LPCTSTR msg, ... );

/////////////////////////////////////////////////////////////////////////////
// CResChangeDialog dialog

CResChangeDialog::CResChangeDialog(CWnd* pParent)
	: CBitmapDialog(CResChangeDialog::IDD, pParent)
{

}


void CResChangeDialog::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResChangeDialog)
	DDX_Control(pDX, IDC_WINDOWSMODESTATIC2, m_Title);
	DDX_Control(pDX, IDC_WINDOWMODECOMBO, m_WindowModeComboBox);
	DDX_Control(pDX, IDC_WINDOWSMODESTATIC, m_WindowModeStatic);	
	DDX_Control(pDX, IDC_REFRESHRATESTATIC, m_RefreshRate);
	DDX_Control(pDX, IDC_RESOLUTIONSTATIC, m_ResolutionStatic);
	DDX_Control(pDX, IDC_REFRESHRATECOMBO, m_RefreshRateCombo);
	DDX_Control(pDX, IDC_RESOLUTIONCOMBO, m_ResolutionCombo);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_CBIndex(pDX, IDC_WINDOWMODECOMBO, m_FullScreenMode);
	DDX_CBIndex(pDX, IDC_RESOLUTIONCOMBO, m_ResolutionIndex);
	DDX_CBIndex(pDX, IDC_REFRESHRATECOMBO, m_RefreshRateIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResChangeDialog, CBitmapDialog)
	//{{AFX_MSG_MAP(CResChangeDialog)
	ON_WM_ERASEBKGND()	
	ON_BN_CLICKED(IDOK, OnOk)
	ON_CBN_SELENDOK(IDC_WINDOWMODECOMBO, OnSelendokWindowmodecombo)
	ON_CBN_SELENDOK(IDC_RESOLUTIONCOMBO, OnSelendokResolutionCombo)
	ON_CBN_SELENDOK(IDC_REFRESHRATECOMBO, OnSelendokRefreshratecombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResChangeDialog message handlers

void CResChangeDialog::OnOk() 
{
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SCREENMODE, m_FullScreenMode );

	CString lbtext;
	m_ResolutionCombo.GetLBText( m_ResolutionIndex, lbtext );

	int width  = 0;
	int height = 0;
	wchar_t dummy;
	swscanf( lbtext, _T("%d %c %d"), &width, &dummy, &height );

	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SCREENWIDTH, width );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SCREENHEIGHT, height );


	int refreshrate = 60;
	m_RefreshRateCombo.GetLBText( m_RefreshRateIndex, lbtext );

#ifdef _XRUSSIAN
	lbtext.Replace( g_InterfaceString[74], _T("") );
#else
	lbtext.Replace( _T("Mhz"), _T("") );
#endif

	swscanf( lbtext, _T("%d"), &refreshrate );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_SCREENREFRESHRATE, refreshrate );

	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_CHANGEDSCREENMODE, TRUE );

	BOOL fff = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_CHANGEDSCREENMODE, 0, FALSE );

	//_XLog( FALSE, _T("update flag : %d\n"), fff );

	EndDialog( IDOK );
}

LPDIRECT3D9 g_pD3D = NULL;
CD3DEnumeration g_d3dEnumeration;
D3DAdapterInfo* g_pBestAdapterInfo = NULL;
D3DDeviceInfo*  g_pBestDeviceInfo = NULL;
D3DDeviceCombo* g_pBestDeviceCombo = NULL;

HRESULT ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
					   D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat )
{
    if( dwBehavior & D3DCREATE_PUREDEVICE )
        return E_FAIL; // GetTransform doesn't work on PUREDEVICE
	
    // Need to support post-pixel processing (for alpha blending)
    if( FAILED( g_pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
        adapterFormat, D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
        D3DRTYPE_SURFACE, backBufferFormat ) ) )
    {
        return E_FAIL;
    }
	
    // This sample uses alpha textures and/or straight alpha. Make sure the
    // device supports them
    if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
        return S_OK;
    if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHA )
        return S_OK;
	
    return E_FAIL;
}


bool ConfirmDeviceHelper( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
						  D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat )
{
    DWORD dwBehavior;
	
    if (vertexProcessingType == SOFTWARE_VP)
        dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    else if (vertexProcessingType == MIXED_VP)
        dwBehavior = D3DCREATE_MIXED_VERTEXPROCESSING;
    else if (vertexProcessingType == HARDWARE_VP)
        dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else if (vertexProcessingType == PURE_HARDWARE_VP)
        dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    else
        dwBehavior = 0; // TODO: throw exception
    
    return SUCCEEDED( ConfirmDevice( pCaps, dwBehavior, adapterFormat, backBufferFormat ) );
}

BOOL CResChangeDialog::OnInitDialog() 
{
	CBitmapDialog::OnInitDialog();
	
	//m_OKButton.MoveWindow( 403, 181, 77, 20 );	
	m_OKButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_OKButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_OKButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_OKButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_OKButton.SetWindowText( g_InterfaceString[41] );

	m_CancelButton.SetShade(CShadeButtonST::SHS_HSHADE);
	m_CancelButton.SetColor( CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255) );
	m_CancelButton.SetColor( CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255) );
	m_CancelButton.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255) );
	m_CancelButton.SetWindowText( g_InterfaceString[38] );

	this->SetWindowText( g_InterfaceString[65] );

	m_WindowModeComboBox.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_WindowModeComboBox.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_WindowModeComboBox.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_WindowModeComboBox.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));

	m_WindowModeComboBox.ResetContent();
	m_WindowModeComboBox.AddString( g_InterfaceString[67] );
	m_WindowModeComboBox.AddString( g_InterfaceString[68] );

	m_WindowModeComboBox.SetCurSel( (int)m_regfullscreenmode );
	m_FullScreenMode = m_regfullscreenmode;

	//m_ResolutionCombo.EnableWindow( !m_FullScreenMode );
	//m_RefreshRateCombo.EnableWindow( !m_FullScreenMode );

	m_RefreshRateCombo.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_RefreshRateCombo.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_RefreshRateCombo.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_RefreshRateCombo.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));

	m_ResolutionCombo.SetComboBitmap(IDB_COMBO_LEFT,IDB_COMBO_RIGHT,IDB_COMBO_CEN,IDB_COMBO_RIGHTDOWN);
	m_ResolutionCombo.SetComboListBitmap(IDB_LIST_LEFT, IDB_LIST_RIGHT,IDB_LIST_TOP,IDB_LIST_BOT);
	m_ResolutionCombo.SetHighlightColor(RGB(64,64,64),RGB(255,255,255));
	m_ResolutionCombo.SetNormalPositionColor(RGB(0,0,0),RGB(255,255,255));

	m_RefreshRate.SetTransparent(TRUE);
	m_RefreshRate.SetTextColor(RGB(227, 227, 227));
#ifdef _XRUSSIAN
	m_RefreshRate.SetWindowText( g_InterfaceString[72] );
#endif
	
	m_ResolutionStatic.SetTransparent(TRUE);
	m_ResolutionStatic.SetTextColor(RGB(227, 227, 227));
#ifdef _XRUSSIAN
	m_ResolutionStatic.SetWindowText( g_InterfaceString[71] );
#endif

	m_WindowModeStatic.SetTransparent(TRUE);
	m_WindowModeStatic.SetTextColor(RGB(227, 227, 227));
#ifdef _XRUSSIAN
	m_WindowModeStatic.SetWindowText( g_InterfaceString[70] );
#endif
	

	m_Title.SetWindowText( g_InterfaceString[65] );
	m_Title.SetTransparent(TRUE);
	m_Title.SetTextColor(RGB(227, 227, 227));

	HRESULT hr;
	
    // Create the Direct3D object	
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
    if( g_pD3D == NULL )
	{
		_XLog( FALSE, _T("Could not initialize d3d object") );
        return FALSE;	
	}

	g_d3dEnumeration.SetD3D( g_pD3D );
    g_d3dEnumeration.ConfirmDeviceCallback = ConfirmDeviceHelper;
    if( FAILED( hr = g_d3dEnumeration.Enumerate() ) )
    {
        SAFE_RELEASE( g_pD3D );
		_XLog( FALSE, _T("Could not enumerate device") );
        //return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

	FindBestFullscreenMode( true, false );

	SAFE_RELEASE( g_pD3D );

	CString resstring;

#ifdef _XRUSSIAN
	resstring.Format( _T("%d X %d 32%s"), m_screenwidth, m_screenheight, g_InterfaceString[73] );
#else
	resstring.Format( _T("%d X %d 32bit color"), m_screenwidth, m_screenheight );
#endif

	CString lbtext;

	m_ResolutionCombo.SetCurSel(0);

	for( int i = 0; i < m_ResolutionCombo.GetCount(); i++ )
	{
		m_ResolutionCombo.GetLBText( i, lbtext );

		if( lbtext.CompareNoCase( resstring ) == 0 )
		{
			m_ResolutionCombo.SetCurSel( i );
			break;
		}
	}

	CString refreshstring;
#ifdef _XRUSSIAN
	refreshstring.Format( _T("%d%s"), m_screenrefreshrate, g_InterfaceString[74] );
#else
	refreshstring.Format( _T("%dMhz"), m_screenrefreshrate );
#endif
	m_RefreshRateCombo.SetCurSel(0);

	for( i = 0; i < m_RefreshRateCombo.GetCount(); i++ )
	{
		m_RefreshRateCombo.GetLBText( i, lbtext );

		if( lbtext.CompareNoCase( refreshstring ) == 0 )
		{
			m_RefreshRateCombo.SetCurSel( i );
			break;
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


bool CResChangeDialog::FindBestFullscreenMode( bool bRequireHAL, bool bRequireREF )
{

    // For fullscreen, default to first HAL DeviceCombo that supports the current desktop 
    // display mode, or any display mode if HAL is not compatible with the desktop mode, or 
    // non-HAL if no HAL is available
    D3DDISPLAYMODE adapterDesktopDisplayMode;
    D3DDISPLAYMODE bestAdapterDesktopDisplayMode;
    //D3DDISPLAYMODE bestDisplayMode;
    bestAdapterDesktopDisplayMode.Width = 0;
    bestAdapterDesktopDisplayMode.Height = 0;
    bestAdapterDesktopDisplayMode.Format = D3DFMT_UNKNOWN;
    bestAdapterDesktopDisplayMode.RefreshRate = 0;
	
    for( UINT iai = 0; iai < g_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
    {
        D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)g_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
        g_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &adapterDesktopDisplayMode );
        for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
        {
            D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
            if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
                continue;
            if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
                continue;
            for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
            {
                D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
                bool bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
                bool bAdapterMatchesDesktop = (pDeviceCombo->AdapterFormat == adapterDesktopDisplayMode.Format);
                if (pDeviceCombo->IsWindowed)
                    continue;
                // If we haven't found a compatible set yet, or if this set
                // is better (because it's a HAL, and/or because formats match better),
                // save it
                if (g_pBestDeviceCombo == NULL ||
                    g_pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceInfo->DevType == D3DDEVTYPE_HAL ||
                    pDeviceCombo->DevType == D3DDEVTYPE_HAL && g_pBestDeviceCombo->AdapterFormat != adapterDesktopDisplayMode.Format && bAdapterMatchesDesktop ||
                    pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB )
                {
                    bestAdapterDesktopDisplayMode = adapterDesktopDisplayMode;
                    g_pBestAdapterInfo = pAdapterInfo;
                    g_pBestDeviceInfo = pDeviceInfo;
                    g_pBestDeviceCombo = pDeviceCombo;
                    if (pDeviceInfo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB)
                    {
                        // This fullscreen device combo looks great -- take it
                        goto EndFullscreenDeviceComboSearch;
                    }
                    // Otherwise keep looking for a better fullscreen device combo
                }
            }
        }
    }
EndFullscreenDeviceComboSearch:

	m_ResolutionCombo.ResetContent();

    if (g_pBestDeviceCombo == NULL)
        return false;

    // Need to find a display mode on the best adapter that uses pBestDeviceCombo->AdapterFormat
    // and is as close to bestAdapterDesktopDisplayMode's res as possible
    //bestDisplayMode.Width = 0;
    //bestDisplayMode.Height = 0;
    //bestDisplayMode.Format = D3DFMT_UNKNOWN;
    //bestDisplayMode.RefreshRate = 0;
	int ressize[128][2];
	int resizelength = 0;
	memset( ressize, 0, sizeof(int) * 128 * 2 );

    for( UINT idm = 0; idm < g_pBestAdapterInfo->pDisplayModeList->Count(); idm++ )
    {
        D3DDISPLAYMODE* pdm = (D3DDISPLAYMODE*)g_pBestAdapterInfo->pDisplayModeList->GetPtr(idm);
        //if( pdm->Format != g_pBestDeviceCombo->AdapterFormat )
		if( pdm->Format != D3DFMT_X8R8G8B8 )
            continue;
        	
		BOOL aleadyexist = FALSE;
		for( int i = 0; i < resizelength; i++ )
		{
			if( ressize[i][0] == pdm->Width && ressize[i][1] == pdm->Height )
			{
				aleadyexist = TRUE;
				break;
			}
		}

		if( !aleadyexist )
		{
			ressize[resizelength][0] = pdm->Width;
			ressize[resizelength][1] = pdm->Height;
			++resizelength;
		}

		/*
		if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
            pdm->Height == bestAdapterDesktopDisplayMode.Height && 
            pdm->RefreshRate == bestAdapterDesktopDisplayMode.RefreshRate )
        {
            // found a perfect match, so stop
            bestDisplayMode = *pdm;
            break;
        }
        else if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
                 pdm->Height == bestAdapterDesktopDisplayMode.Height && 
                 pdm->RefreshRate > bestDisplayMode.RefreshRate )
        {
            // refresh rate doesn't match, but width/height match, so keep this
            // and keep looking
            bestDisplayMode = *pdm;
        }
        else if( pdm->Width == bestAdapterDesktopDisplayMode.Width )
        {
            // width matches, so keep this and keep looking
            bestDisplayMode = *pdm;
        }
        else if( bestDisplayMode.Width == 0 )
        {
            // we don't have anything better yet, so keep this and keep looking
            bestDisplayMode = *pdm;
        }
		*/
    }

	CString resolutionstring;
	m_ResolutionCombo.ResetContent();

	for( int i = 0; i < resizelength; i++ )
	{	
#ifdef _XRUSSIAN		
		resolutionstring.Format( _T("%d X %d 32%s"), ressize[i][0], ressize[i][1], g_InterfaceString[73] );
#else
		resolutionstring.Format( _T("%d X %d 32bit color"), ressize[i][0], ressize[i][1] );
#endif
		m_ResolutionCombo.AddString(resolutionstring);
	}

	m_ResolutionIndex = 0;
	m_ResolutionCombo.SetCurSel( 0 );
	
	if( !g_pBestAdapterInfo || !g_pBestDeviceCombo )
	{
		return false;
	}

	CString lbtext;
	m_ResolutionCombo.GetLBText( m_ResolutionIndex, lbtext );

	int width  = 0;
	int height = 0;
	wchar_t dummy;
	swscanf( lbtext, _T("%d %c %d"), &width, &dummy, &height );

	int RefreshRateArray[100];
	memset( RefreshRateArray, 0, sizeof(int) * 100 );
	int arraysize = 0;

	for( idm = 0; idm < g_pBestAdapterInfo->pDisplayModeList->Count(); idm++ )
    {
        D3DDISPLAYMODE* pdm = (D3DDISPLAYMODE*)g_pBestAdapterInfo->pDisplayModeList->GetPtr(idm);
        //if( pdm->Format != g_pBestDeviceCombo->AdapterFormat )
		if( pdm->Format != D3DFMT_X8R8G8B8 )
            continue;
        		
		if( pdm->Width == width && pdm->Height == height )
		{
			BOOL aleadyexist = FALSE;
			for( int i = 0; i < arraysize; i++ )
			{
				if( pdm->RefreshRate == RefreshRateArray[i] )
				{
					aleadyexist = TRUE;
					break;
				}
			}

			if( !aleadyexist )
			{
				RefreshRateArray[arraysize] = pdm->RefreshRate;
				++arraysize;
			}
		}		
	}

	CString newlbtext;
	m_RefreshRateCombo.ResetContent();

	for( i = 0; i < arraysize; i++ )
	{
#ifdef _XRUSSIAN
		newlbtext.Format( _T("%d%s"), RefreshRateArray[i], g_InterfaceString[74] );
#else
		newlbtext.Format( _T("%dMhz"), RefreshRateArray[i] );
#endif
		m_RefreshRateCombo.AddString( newlbtext );
	}		

	m_RefreshRateCombo.SetCurSel( 0 );

    return true;
}

void CResChangeDialog::OnSelendokWindowmodecombo() 
{
	UpdateData();

	//m_ResolutionCombo.EnableWindow( !m_FullScreenMode );
	//m_RefreshRateCombo.EnableWindow( !m_FullScreenMode );
}

void CResChangeDialog::OnSelendokResolutionCombo() 
{
	UpdateData();

	if( !g_pBestAdapterInfo || !g_pBestDeviceCombo )
	{
		return;
	}

	CString lbtext;
	m_ResolutionCombo.GetLBText( m_ResolutionIndex, lbtext );

	int width  = 0;
	int height = 0;
	wchar_t dummy;
	swscanf( lbtext, _T("%d %c %d"), &width, &dummy, &height );

	int RefreshRateArray[100];
	memset( RefreshRateArray, 0, sizeof(int) * 100 );
	int arraysize = 0;

	for( UINT idm = 0; idm < g_pBestAdapterInfo->pDisplayModeList->Count(); idm++ )
    {
        D3DDISPLAYMODE* pdm = (D3DDISPLAYMODE*)g_pBestAdapterInfo->pDisplayModeList->GetPtr(idm);
        //if( pdm->Format != g_pBestDeviceCombo->AdapterFormat )
		if( pdm->Format != D3DFMT_X8R8G8B8 )
            continue;
        		
		if( pdm->Width == width && pdm->Height == height )
		{
			BOOL aleadyexist = FALSE;
			for( int i = 0; i < arraysize; i++ )
			{
				if( pdm->RefreshRate == RefreshRateArray[i] )
				{
					aleadyexist = TRUE;
					break;
				}
			}

			if( !aleadyexist )
			{
				RefreshRateArray[arraysize] = pdm->RefreshRate;
				++arraysize;
			}
		}		
	}

	CString newlbtext;
	m_RefreshRateCombo.ResetContent();

	for( int i = 0; i < arraysize; i++ )
	{
#ifdef _XRUSSIAN
		newlbtext.Format( _T("%d%s"), RefreshRateArray[i], g_InterfaceString[74] );
#else
		newlbtext.Format( _T("%dMhz"), RefreshRateArray[i] );
#endif
		m_RefreshRateCombo.AddString( newlbtext );
	}		

	m_RefreshRateCombo.SetCurSel( 0 );
}

void CResChangeDialog::OnSelendokRefreshratecombo() 
{
	UpdateData();	
}


BOOL CResChangeDialog::OnEraseBkgnd(CDC* pDC)
{
	return CBitmapDialog::OnEraseBkgnd( pDC );
} 
