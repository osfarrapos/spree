// XExceptionHandler.h : declaration of the _XExceptionHandler namespace
//
// Created Date : 2003.08.12
// Author		: Sohyun, Park
// DESC			: Exception Handler
//
//////////////////////////////////////////////////////////////////////

#ifndef __XEXCEPTION_HANDLER_H__
#define __XEXCEPTION_HANDLER_H__

#include <windows.h>

/** 예외처리 실행을 위한 namespace.
 *
 * 예외처리 기록.
 * \author Sohyun, Park
 * \date 2003.08.18
 */

namespace _XExceptionHandler
{

	/** 예외처리의 main 함수.
	 * Exception이 일어 났을때, Exception code와 Exception address를 받아서 상세 정보를 EOD.log에 기록한다.
	 * EOD.log가 먼저 열려 있어야 한다.
	 * \param [IN] DWORD _code : 예외 Code.
	 * \param [IN] DWORD _address : 예외가 일어난 곳의 논리주소.
	 * \todo Call Stack과 Register 정보 기록.
	 */
	void	GenerateExceptionInformation(DWORD _code, const void* _address);

	/** 예외처리 String 기록.
	 * 예외코드에 따른 String을 EOD.log에 기록한다.
	 * \param [IN] DWORD _code : 예외 Code.
	 */
	void	GenerateExceptionString(DWORD _code);

	/** 예외가 일어난 곳의 상세주소 얻기.
	 * 예외가 일어난 곳의 주소를 받아서 module name, section, offset을 구한다.
	 * \param [IN] const void* _address : 예외가 일어난 곳의 논리주소.
	 * \param [IN/OUT] TCHAR* _modulename : 예외가 일어난 곳의 모듈 이름.
	 * \param [IN] DWORD len : _modulename의 길이.
	 * \param [OUT] DWORD& section : 상세 주소.
	 * \param [OUT] DWORD& offset : 상세 주소.
	 */
	BOOL	GetExceptionAddress(const void* _address, TCHAR* _modulename, DWORD len, DWORD& section, DWORD& offset);
}

#endif