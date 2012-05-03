// EnterSerialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XLauncher.h"
#include "EnterSerialDlg.h"

#include "XEncryption.h"
#include "XEncryptiondata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnterSerialDlg dialog

// Virtual key define..
#define VK_PAGE_UP   0x21
#define VK_PAGE_DOWN 0x22

#define VK_0    0x30
#define VK_1    0x31
#define VK_2    0x32
#define VK_3    0x33
#define VK_4    0x34
#define VK_5    0x35
#define VK_6    0x36
#define VK_7    0x37
#define VK_8    0x38
#define VK_9    0x39


#define VK_A    0x41
#define VK_B    0x42
#define VK_C    0x43
#define VK_D    0x44
#define VK_E    0x45
#define VK_F    0x46
#define VK_G    0x47
#define VK_H    0x48
#define VK_I    0x49
#define VK_J    0x4A
#define VK_K    0x4B
#define VK_L    0x4C
#define VK_M    0x4D
#define VK_N    0x4E
#define VK_O    0x4F
#define VK_P    0x50
#define VK_Q    0x51
#define VK_R    0x52
#define VK_S    0x53
#define VK_T    0x54
#define VK_U    0x55
#define VK_V    0x56
#define VK_W    0x57
#define VK_X    0x58
#define VK_Y    0x59
#define VK_Z    0x5A


CEnterSerialDlg::CEnterSerialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnterSerialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnterSerialDlg)
	m_SerialField1 = _T("");
	m_SerialField2 = _T("");
	m_SerialField3 = _T("");
	m_SerialField4 = _T("");
	m_SerialField5 = _T("");
	//}}AFX_DATA_INIT

	easterneggcdkeyinsertLevel = 0;
	easterneggcdkeyinsertKey = FALSE;
}


void CEnterSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnterSerialDlg)
	DDX_Text(pDX, IDC_SERIALFIELDEDIT1, m_SerialField1);
	DDV_MaxChars(pDX, m_SerialField1, 5);
	DDX_Text(pDX, IDC_SERIALFIELDEDIT2, m_SerialField2);
	DDV_MaxChars(pDX, m_SerialField2, 5);
	DDX_Text(pDX, IDC_SERIALFIELDEDIT3, m_SerialField3);
	DDV_MaxChars(pDX, m_SerialField3, 5);
	DDX_Text(pDX, IDC_SERIALFIELDEDIT4, m_SerialField4);
	DDV_MaxChars(pDX, m_SerialField4, 5);
	DDX_Text(pDX, IDC_SERIALFIELDEDIT5, m_SerialField5);
	DDV_MaxChars(pDX, m_SerialField5, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnterSerialDlg, CDialog)
	//{{AFX_MSG_MAP(CEnterSerialDlg)
	ON_EN_CHANGE(IDC_SERIALFIELDEDIT1, OnChangeSerialfieldedit1)
	ON_EN_CHANGE(IDC_SERIALFIELDEDIT2, OnChangeSerialfieldedit2)
	ON_EN_CHANGE(IDC_SERIALFIELDEDIT3, OnChangeSerialfieldedit3)
	ON_EN_CHANGE(IDC_SERIALFIELDEDIT4, OnChangeSerialfieldedit4)
	ON_EN_CHANGE(IDC_SERIALFIELDEDIT5, OnChangeSerialfieldedit5)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnterSerialDlg message handlers

void CEnterSerialDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CEnterSerialDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CEnterSerialDlg::OnChangeSerialfieldedit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	if( this->m_SerialField1.GetLength() == 5 )
	{
		GetDlgItem( IDC_SERIALFIELDEDIT2 )->SetFocus(); 
		((CEdit*)GetDlgItem( IDC_SERIALFIELDEDIT2 ))->SetSel( 5 );
	}

	if(  this->m_SerialField1.GetLength() == 5 && 
		 this->m_SerialField2.GetLength() == 5 && 
		 this->m_SerialField3.GetLength() == 5 && 
		 this->m_SerialField4.GetLength() == 5 && 
		 this->m_SerialField5.GetLength() == 6 )
		 GetDlgItem( IDOK )->EnableWindow( TRUE );
	else GetDlgItem( IDOK )->EnableWindow( FALSE );

}

void CEnterSerialDlg::OnChangeSerialfieldedit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	if( this->m_SerialField2.GetLength() == 5 )
	{
		GetDlgItem( IDC_SERIALFIELDEDIT3 )->SetFocus(); 
		((CEdit*)GetDlgItem( IDC_SERIALFIELDEDIT3 ))->SetSel( 5 );
	}

	if(  this->m_SerialField1.GetLength() == 5 && 
		 this->m_SerialField2.GetLength() == 5 && 
		 this->m_SerialField3.GetLength() == 5 && 
		 this->m_SerialField4.GetLength() == 5 && 
		 this->m_SerialField5.GetLength() == 6 )
		 GetDlgItem( IDOK )->EnableWindow( TRUE );
	else GetDlgItem( IDOK )->EnableWindow( FALSE );
}

void CEnterSerialDlg::OnChangeSerialfieldedit3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	if( this->m_SerialField3.GetLength() == 5 )
	{
		GetDlgItem( IDC_SERIALFIELDEDIT4 )->SetFocus(); 
		((CEdit*)GetDlgItem( IDC_SERIALFIELDEDIT4 ))->SetSel( 5 );
	}

	if(  this->m_SerialField1.GetLength() == 5 && 
		 this->m_SerialField2.GetLength() == 5 && 
		 this->m_SerialField3.GetLength() == 5 && 
		 this->m_SerialField4.GetLength() == 5 && 
		 this->m_SerialField5.GetLength() == 6 )
		 GetDlgItem( IDOK )->EnableWindow( TRUE );
	else GetDlgItem( IDOK )->EnableWindow( FALSE );
}

void CEnterSerialDlg::OnChangeSerialfieldedit4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( this->m_SerialField4.GetLength() == 5 )
	{
		GetDlgItem( IDC_SERIALFIELDEDIT5 )->SetFocus(); 
		((CEdit*)GetDlgItem( IDC_SERIALFIELDEDIT5 ))->SetSel( 6 );
	}

	if(  this->m_SerialField1.GetLength() == 5 && 
		 this->m_SerialField2.GetLength() == 5 && 
		 this->m_SerialField3.GetLength() == 5 && 
		 this->m_SerialField4.GetLength() == 5 && 
		 this->m_SerialField5.GetLength() == 6 )
		 GetDlgItem( IDOK )->EnableWindow( TRUE );
	else GetDlgItem( IDOK )->EnableWindow( FALSE );
}

void CEnterSerialDlg::OnChangeSerialfieldedit5() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	
	if( this->m_SerialField5.GetLength() == 6 )
	{
		GetDlgItem( IDOK )->SetFocus(); 
	}

	if(  this->m_SerialField1.GetLength() == 5 && 
		 this->m_SerialField2.GetLength() == 5 && 
		 this->m_SerialField3.GetLength() == 5 && 
		 this->m_SerialField4.GetLength() == 5 && 
		 this->m_SerialField5.GetLength() == 6 )
		 GetDlgItem( IDOK )->EnableWindow( TRUE );
	else GetDlgItem( IDOK )->EnableWindow( FALSE );
}


BOOL CEnterSerialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(  this->m_SerialField1.GetLength() == 5 && 
		 this->m_SerialField2.GetLength() == 5 && 
		 this->m_SerialField3.GetLength() == 5 && 
		 this->m_SerialField4.GetLength() == 5 && 
		 this->m_SerialField5.GetLength() == 6 )
		 GetDlgItem( IDOK )->EnableWindow( TRUE );
	else GetDlgItem( IDOK )->EnableWindow( FALSE );

	RECT rect;
	GetWindowRect( &rect );

	//SetActiveWindow();
	//SetFocus();
	SetForegroundWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CEnterSerialDlg::makeSerial( LPSTR field1, LPSTR field2, LPSTR field3, LPSTR field4, LPSTR field5 )
{	
	// Randomize...
	srand( (unsigned)time( NULL ) );
	
	char	strSerialData[256];
	char	strSerialMergedtData[256] = "A2FCFX3DQ4ISC1J9R3X2E";
	char	strEncryptKeyData[256] = "9E2GH4FWDWKC2MBL4EBNKUV63T";
	char    strSerialIndexString[7];
		
	_XEncryption encrypt_factory;
	char encryptserial[_ENCRYPTION_FIELD_LENGTH+1];
	
	// Encrypt key setting ///////////////////////////////////////////////////////////////////////////////////////////
	encrypt_factory.setKey( 10, 31, strEncryptKeyData );
	
	memset( strSerialIndexString, 0, 7 );
	memcpy( strSerialIndexString, "9R3X2E", 6 );

	// Random index 를 만들기 위한 테이블 준비..
	int  randomStartRange[16];
	char randomindexstring[17];
	memset( randomindexstring, 0, 17 );
	memcpy( randomindexstring, strSerialMergedtData, 16 );
	
	for( int i = 0; i < 16; i++ )
	{
		randomStartRange[i] = _SERIALFIELD_ASCIILUT[ randomindexstring[ i ] ];
	}
	
	// generate serial number loop ///////////////////////////////////////////////////////////////////////////////////
	CString strEncryptString;
	CString strDecryptString;	
	
	// create random serial number -------------------------------------------------------------------------------		
	memset( randomindexstring, 0, 17 );

	int randomvalue;
	for( i = 0; i < 16; i++ )
	{
		randomvalue = (35 - randomStartRange[i]);
		if( randomvalue ) randomvalue = rand() % randomvalue;
		else randomvalue = 35;

		randomindexstring[i] = _SERIALFIELD_CHARACTERDATA[randomvalue];
	}

	// joint serial field ----------------------------------------------------------------------------------------
	//strSerialData = strSerialMergedtData;		
	memset( strSerialData , 0, 256 );
	strcpy( strSerialData, randomindexstring );
	strcat( strSerialData, strSerialIndexString );


	// serial number encryption ----------------------------------------------------------------------------------
	encrypt_factory.enciperSerial( strSerialData, encryptserial );
	char serialfield[5][7];
	memset( serialfield, 0, sizeof(char) * 5 * 7 );

	for( i = 0; i < 5; i++ )
	{
		memcpy( serialfield[i], encryptserial+ (i*5), (i == 4) ? 6 : 5 );
	}			
	
	memcpy( field1, serialfield[0], 5);
	memcpy( field2, serialfield[1], 5);
	memcpy( field3, serialfield[2], 5);
	memcpy( field4, serialfield[3], 5);
	memcpy( field5, serialfield[4], 6);

}

void CEnterSerialDlg::generateSerial()
{	
	char field[5][7];
	memset( field, 0, 5*7 );

	char field2[5][7];
	memset( field2, 0, 5*7 );

	m_SerialField1 = _T("");
	m_SerialField2 = _T("");
	m_SerialField3 = _T("");
	m_SerialField4 = _T("");
	m_SerialField5 = _T("");

	makeSerial( field[0], field[1], field[2], field[3], field[4] );

	UpdateData( FALSE );

	int fieldnumber;
	int fieldindex;

	for( int i = 0; i < 26; i++ )
	{
		if( i < 25 )
		{
			fieldnumber = i / 5;
			fieldindex  = i % 5;
		}
		else
		{
			fieldnumber = 4;
			fieldindex  = 5;
		}

		for( int j = (int)'0'; j <= (int)field[ fieldnumber ][fieldindex]; j++ )
		{
			if( fieldnumber == 0 )
			{
				field2[0][fieldindex] = (char)j;
				m_SerialField1 = field2[0];
			}
			else if( fieldnumber == 1 )
			{
				field2[1][fieldindex] = (char)j;

				m_SerialField2 = field2[1];
			}
			else if( fieldnumber == 2 )
			{
				field2[2][fieldindex] = (char)j;

				m_SerialField3 = field2[2];
			}
			else if( fieldnumber == 3 )
			{
				field2[3][fieldindex] = (char)j;

				m_SerialField4 = field2[3];
			}
			else if( fieldnumber == 4 )
			{
				field2[4][fieldindex] = (char)j;

				m_SerialField5 = field2[4];
			}

			UpdateData( FALSE );

			UpdateWindow();

			Sleep( 1 );
		}
	}
	

	if(  this->m_SerialField1.GetLength() == 5 && 
		 this->m_SerialField2.GetLength() == 5 && 
		 this->m_SerialField3.GetLength() == 5 && 
		 this->m_SerialField4.GetLength() == 5 && 
		 this->m_SerialField5.GetLength() == 6 )
		 GetDlgItem( IDOK )->EnableWindow( TRUE );
	else GetDlgItem( IDOK )->EnableWindow( FALSE );
}

BOOL CEnterSerialDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
    {	   
		if( HIBYTE( GetAsyncKeyState( VK_CONTROL ) ) )
		{
		   if( HIBYTE( GetAsyncKeyState( VK_F ) ) && 
			   HIBYTE( GetAsyncKeyState( VK_S ) )   )
		   {
			   if( this->m_SerialField1.CompareNoCase( _T("asdfg" ) ) == 0 && 
				   this->m_SerialField2.CompareNoCase( _T("yuio" ) ) == 0 )
			   {				   
				   easterneggcdkeyinsertKey = TRUE;
				   easterneggcdkeyinsertLevel = 2;
			   }			   
		   }
		}		
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CEnterSerialDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( easterneggcdkeyinsertLevel <= 5 )
	{
		if( HIBYTE( GetAsyncKeyState( VK_CONTROL ) ) )
		{
			if( easterneggcdkeyinsertKey && easterneggcdkeyinsertLevel >= 2 )
			{
				easterneggcdkeyinsertLevel++;
			}

			if( easterneggcdkeyinsertLevel == 5 ) easterneggcdkeyinsertKey = FALSE;
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CEnterSerialDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( HIBYTE( GetAsyncKeyState( VK_CONTROL ) ) )
	{
		if( !easterneggcdkeyinsertKey && easterneggcdkeyinsertLevel >= 5 )
		{
			easterneggcdkeyinsertLevel++;
		}


		if( easterneggcdkeyinsertLevel == 9 )
		{
			easterneggcdkeyinsertKey = FALSE;
			easterneggcdkeyinsertLevel = 0;
			generateSerial();
		}
	}

	CDialog::OnRButtonDown(nFlags, point);
}
