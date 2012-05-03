// FTPTransfer.h: interface for the CFTPTransfer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FTPTRANSFER_H_
#define _FTPTRANSFER_H_

//#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wininet.h>


/** FTP Transfer class.
 *
 * 실제로 file을 FTP 프로토콜을 사용하여 전송하는 클래스.
 * \author Suhyun, Park
 * \date 2003.08.18
 * \see XFileTransferDlg.cpp
 */
class CFTPTransfer  
{
protected :

	/// FTP 서버 주소
	CString			m_sServer;
	/// Proxy 서버 주소
	CString			m_sProxyServer;

	/// User name : default로 anonymous 사용
	CString			m_sUserName;
	/// Password : anonymous인 경우 password 없음
	CString			m_sPassWord;

	/// Passive setting을 사용하는지 여부
	BOOL			m_bUsePassive;
	/// Proxy server를 사용하는지 여부
	BOOL			m_bUseProxy;
	/// Preconfig option을 사용하는지 여부
	BOOL			m_bUsePreconfig;

	/// Internet session
	HINTERNET		m_hInternetSession;
	/// FTP session
	HINTERNET		m_hFTPSession;
	/// File connection - 현재 사용하지 않음
	HINTERNET		m_FileConnection;

	/// WIN32_FIND_DATA - 현재 사용하지 않음
	WIN32_FIND_DATA	m_sWFD;
	/// 전송 결과
	BOOL			m_bResult;
	/// 전송할 파일의 Local Full Path
	CString			m_sInputSpec;
	/// 전송할 파일이 저장될 FTP server full path
	CString			m_sOutputSpec;

public:
	/** Constructor.
	 */
	CFTPTransfer();
	/** Desctructor.
	 */
	virtual ~CFTPTransfer();

	/** 초기화 함수.
	 * 
	 * 서버주소, user name, password 세팅, Passive/Proxy/Preconfig 사용여부 체크
	 * \return 성공 여부
	 */
	BOOL			Initialze(void);
	/** FTP 서버로 연결하는 함수.
	 *
	 * \return 성공 여부
	 */
	BOOL			ConnetServer(void);
	/** FTP 서버에 Path 설정하는 함수.
	 *
	 * 전송될 파일이 저장될 디렉토리를 생성하고, 그 디렉토리로 이동한다.<br>
	 * 디렉토리 이름은 EmperorOfDragons에서 사용되는 유저이름_LocalTime() 이다.
	 */
	void			SetServerPath( CString senderid );
	/** 파일 전송.
	 *
	 * \return 성공 여부
	 */
	BOOL			SendFile(void);

	/** m_sInputSpec setting.
	 *
	 * 전송할 파일의 Local Full Path setting
	 */
	inline void		SetInputSpec(TCHAR* inputfilename)	{	m_sInputSpec = inputfilename;	}
	/** m_sInputSpec concatenation.
	 *
	 * 전송할 파일의 Local Full Path setting
	 */
	inline void		ConcatInputSpec(TCHAR* filename)	{	m_sInputSpec += filename;		}
	/** m_sOutputSpec setting.
	 *
	 * 전송할 파일이 저장될 FTP server full path
	 */
	inline void		SetOutputSpec(TCHAR* outputfilename)	{	m_sOutputSpec = outputfilename;	}
	
};

#endif 
