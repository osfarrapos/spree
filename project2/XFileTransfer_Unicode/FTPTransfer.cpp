// FTPTransfer.cpp: implementation of the CFTPTransfer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include <Windows.h>
#include <mmsystem.h>
#include "FTPTransfer.h"
#include "Registry.h"
#include "RegistryDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFTPTransfer::CFTPTransfer()
{	
	m_sServer		= _T("");
	m_sProxyServer	= _T("");
	m_sUserName		= _T("");
	m_sPassWord		= _T("");

	m_bUsePassive	= FALSE;
	m_bUseProxy		= FALSE;
	m_bUsePreconfig = TRUE;

	m_bResult		= TRUE;
	m_sInputSpec	= _T("");
	m_sOutputSpec	= _T("");

	m_hInternetSession	= NULL;
	m_hFTPSession		= NULL;
	m_FileConnection	= NULL;
}

CFTPTransfer::~CFTPTransfer()
{

}

//#define TEST_VERSION

// ========================================
// Connection Initialize function
// ========================================
BOOL CFTPTransfer::Initialze(void)
{
	TCHAR		registrybuffer[_REGSTRING_MAXLENGTH];
	memset(registrybuffer, 0, _REGSTRING_MAXLENGTH);

#ifdef TEST_VERSION
	m_sServer = _T("0.0.0.0");		// test
#else
	g_RegManager._XRegReadString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LOGFTPSERVERADDRESS, _T("0.0.0.0"), registrybuffer, _REGSTRING_MAXLENGTH, TRUE);
	
	m_sServer = registrybuffer;	
#endif

	m_bUsePassive = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_PASV, 0, TRUE);
	m_bUseProxy = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEPROXY, 0, TRUE);
	m_bUsePreconfig = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_PRECONFIGUREDSETTINGS, 1, TRUE);

	memset(registrybuffer, 0, _REGSTRING_MAXLENGTH);
	if(g_RegManager._XRegReadString(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_PROXYSERVERADDRESS, _T(""), registrybuffer, _REGSTRING_MAXLENGTH, TRUE))
		m_sProxyServer = registrybuffer;
	else
		m_sProxyServer = _T("");
	
	m_sUserName = _T("anonymous");
	m_sPassWord = _T("");

	return TRUE;
}

// ======================================
// Connect server
// ======================================
BOOL CFTPTransfer::ConnetServer(void)
{
	if(m_bUsePreconfig)
		m_hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	else if(m_bUseProxy)
		m_hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PROXY, m_sProxyServer, NULL, 0);
	else
		m_hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	
	if(m_hInternetSession == NULL)
	{
		return FALSE;
	}
	
	m_hFTPSession = ::InternetConnect(m_hInternetSession, m_sServer, INTERNET_INVALID_PORT_NUMBER, m_sUserName, m_sPassWord, 
		INTERNET_SERVICE_FTP, m_bUsePassive ? INTERNET_FLAG_PASSIVE : 0, 0);
	if(m_hFTPSession == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

// =======================================
// Set Server Directory Path
// =======================================
void CFTPTransfer::SetServerPath( CString senderid )
{
	TCHAR		directoryname[256];
	unsigned long	sizename = 256;
		
	if( senderid.GetLength() )
		wsprintf(directoryname, _T("%s_%d"), senderid, timeGetTime());
	else
		wsprintf(directoryname, _T("%s_%d"), _T("UNKNOWNUSER"), timeGetTime());

	::FtpCreateDirectory(m_hFTPSession, directoryname);
	::FtpSetCurrentDirectory(m_hFTPSession, directoryname);
}

// =======================================
// Send file
// =======================================
BOOL CFTPTransfer::SendFile(void)
{
	DWORD		errorcode;
	m_bResult = ::FtpPutFile(m_hFTPSession, m_sInputSpec, m_sOutputSpec, FTP_TRANSFER_TYPE_BINARY, 0);
	if(m_bResult == FALSE)
	{
		errorcode = GetLastError();
	}
	return m_bResult;
}
