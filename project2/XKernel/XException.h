// XException.h: interface for the _XException class.
//
// Created Date : 2003.08.12
// Author		: Sohyun, Park
// DESC			: Structure Exception Handler Class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XEXCEPTION_H__7D41895A_BED5_482A_99FE_2A4200AA8E2F__INCLUDED_)
#define AFX_XEXCEPTION_H__7D41895A_BED5_482A_99FE_2A4200AA8E2F__INCLUDED_

#include <exception>
#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/** 예외처리 클래스.
 *
 * Structure Exception Handler 설치.
 * EXCEPTION_POINTERS로 부터 Exception Code와 Exception Address 추출.
 * \author Sohyun, Park
 * \date 2003.08.18
 */
class _XException
{
public:
	/** Constructor.
	 */
	_XException(EXCEPTION_POINTERS const &info)  throw();

	/** Destructor.
	 */
	virtual ~_XException();

	/** Structure Exception Handler 설치.
	 */
	static	void			Install()	throw();

	/** Exception Code 추출.
	 * \return DWORD Exception Code.
	 */
	DWORD					What()		const throw();

	/** Exception Address 추출.
	 * \return void * Exception Address.
	 */
	void	const			*Where()	const throw();

private :
	/// Exception Address
	void	const			*m_Address;

	/// Exception Code
	DWORD					m_Code;

	/// Context Record
	PCONTEXT				m_ContextRecord;
};

#endif // !defined(AFX_XEXCEPTION_H__7D41895A_BED5_482A_99FE_2A4200AA8E2F__INCLUDED_)
