/*----------------------------------------------------------------------
Copyright (c) 1998-present Russ Freeman. All Rights Reserved.
Web site: http://www.gipsysoft.com/

This software is provided 'as-is', without any express or implied warranty.

In no event will the author be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject
to the following restrictions: 

1) This source code may not be distributed as part of a commercial library
   without written permission from the author
2) The origin of this software must not be misrepresented; you must not claim
   that you wrote the original software. If you use this software in a product,
	 an acknowledgment in the product documentation is requested but not required.
3) Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
4) Altered source is encouraged to be submitted back to the original author so
   it can be shared with the community. Please share your changes. Don't make me
	 use GPL!
5) This notice may not be removed or altered from any source distribution.
6) If you have any confusion then ask

File:	DebugHlp.h
Owner:	russf@gipsysoft.com
Purpose:	Debugging helpers
----------------------------------------------------------------------*/
#ifndef DEBUGHLP_H
#define DEBUGHLP_H

#ifndef _WINDOWS_
	#include <Windows.h>
#endif	//	_WINDOWS_

#ifndef _INC_TCHAR
	#include <tchar.h>
#endif	//	_INC_TCHAR

#undef ASSERT
#undef VERIFY
#undef TRACE

#pragma warning( disable : 4127 )	//	: conditional expression is constant

#ifndef _DEBUG_HELP_NO_LINK_LIBS
	#ifdef _DEBUG
		#ifdef	_UNICODE
			#pragma comment( lib, "DebugHLPUD.lib" )
		#else		//	 _UNICODE
			#pragma comment( lib, "DebugHLPD.lib" )
		#endif	//	 _UNICODE

	#else	//	_DEBUG

		#ifdef	_UNICODE
			#pragma comment( lib, "DebugHLPU.lib" )
		#else		//	 _UNICODE
			#pragma comment( lib, "DebugHLP.lib" )
		#endif	//	 _UNICODE
	#endif	//	_DEBUG
#endif	//	_DEBUG_HELP_NO_LINK_LIBS

#ifndef LPCSTR
	typedef const char * LPCSTR;
#endif	//	LPCSTR

#ifndef LPCWSTR
	/*lint -e18 */
	typedef const unsigned short * LPCWSTR;
	/*lint +e18 */
#endif	//	LPCSTR
#ifndef BOOL
	typedef int BOOL;
#endif	//	BOOL
	void _cdecl DebugTraceToFileA( LPCSTR pcszFilename );
	void _cdecl DebugTraceToFileW( LPCWSTR pcszFilename );

	void _cdecl DebugTraceA( LPCSTR pcszFormat, ... );
	void _cdecl DebugTraceW( LPCWSTR pcszFormat, ... );

	void _cdecl DebugTraceLineAndFileA( LPCSTR pcszFilename, int nLine );
	void _cdecl DebugTraceLineAndFileW( LPCWSTR pcszFilename, int nLine );

	void _cdecl DebugTraceMemory( LPVOID lp, UINT uSize );

	//	Return TRUE if the program should retry, use STOPHRE to do the retry.
	BOOL _cdecl AssertFailed( LPCSTR pcszFilename, int nLine, LPCSTR pcszExpression );
	BOOL _cdecl ApiFailure( LPCSTR pcszFilename, int nLine, LPCSTR pcszExpression, int nGetLastError );

	#ifndef DEBUGHLP_BARE_TRACE
		#define TRACEA ::DebugTraceLineAndFileA( __FILE__, __LINE__ ), ::DebugTraceA
		#define TRACEW ::DebugTraceLineAndFileW( _T(__FILE__), __LINE__ ), ::DebugTraceW
		#define TRACENLA ::DebugTraceA
		#define TRACENLW ::DebugTraceW
		#define TRACE_MEMORYA	DebugTraceLineAndFileA( _T(__FILE__), __LINE__ ), ::DebugTraceMemory
		#define TRACE_MEMORYW	DebugTraceLineAndFileA( _T(__FILE__), __LINE__ ), ::DebugTraceMemory

	#else	//	DEBUGHLP_BARE_TRACE
		#define TRACEA ::DebugTraceA
		#define TRACEW ::DebugTraceW
		#define TRACE_MEMORY	::DebugTraceMemory
	#endif	//	DEBUGHLP_BARE_TRACE

#ifdef _DEBUG
	#define SETLOGFILEA( pcszFilename )	::DebugTraceToFileA( pcszFilename )
	#define SETLOGFILEW( pcszFilename )	::DebugTraceToFileW( pcszFilename )

	#define STOPHERE()	_asm{ int 3 }
	#define VERIFY( exp )	ASSERT( exp )
	#define ASSERT( exp ) \
		/*lint -e717 -e506 -e774*/ \
		do \
		{ \
			if( !( exp ) )\
			{\
				TRACEA( "Assert Failed with expression %s\n", #exp);\
				if( ::AssertFailed( __FILE__, __LINE__, #exp ) ) \
				{\
					STOPHERE();	\
				}\
			}\
		} while (0) \
		/*lint +e717 +e506 +e774*/ \

	#define VAPI(exp) \
		/*lint -e717 -e506 -e774*/ \
		do \
		{ \
			if( !(exp) )\
			{\
				int nGetLastError = GetLastError();\
				TRACEA( "VAPI Failed with expression %s\n", #exp);\
				if( ApiFailure( __FILE__, __LINE__, #exp, nGetLastError ) ) \
				{\
					STOPHERE();	\
				}\
			}\
		} while (0) \
		/*lint +e717 +e506 +e774*/ \

	#define ASSERT_VALID_HWND( hwnd )						ASSERT( ::IsWindow( hwnd ) )
	#define ASSERT_VALID_STR_LEN( str, n )			ASSERT( !::IsBadStringPtr( str, n ) )
	#define ASSERT_VALID_STR( str )							ASSERT_VALID_STR_LEN( str, 0xfffff )
	#define ASSERT_VALID_WRITEPTR( obj, n )			ASSERT( !::IsBadWritePtr( obj, n ) )
	#define ASSERT_VALID_READPTR( obj, n )			ASSERT( !::IsBadReadPtr( obj, n ) )
	#define ASSERT_VALID_WRITEOBJPTR( obj )			ASSERT_VALID_WRITEPTR( obj, sizeof(*obj ) )
	#define ASSERT_VALID_READOBJPTR( obj )			ASSERT_VALID_READPTR( obj, sizeof(*obj ) )

	#ifdef _UNICODE
		#define TRACE TRACEW
		#define TRACENL	TRACENLW
		#define TRACE_MEMORY TRACE_MEMORYW
		#define SETLOGFILE SETLOGFILEW
	#else	//	_UNICODE
		#define TRACE TRACEA
		#define TRACENL	TRACENLA
		#define TRACE_MEMORY TRACE_MEMORYA
		#define SETLOGFILE SETLOGFILEA
	#endif	//	_UNICODE

#else	//	_DEBUG
	#define ASSERT_VALID_HWND( hwnd )						((void)0)
	#define ASSERT_VALID_STR_LEN( str, n )			((void)0)
	#define ASSERT_VALID_STR( str )							((void)0)
	#define ASSERT_VALID_WRITEPTR( obj, n )			((void)0)
	#define ASSERT_VALID_READPTR( obj, n )			((void)0)
	#define ASSERT_VALID_WRITEOBJPTR( obj )			((void)0)
	#define ASSERT_VALID_READOBJPTR( obj )			((void)0)

	#ifdef _UNICODE
		#define TRACE							1 ? (void)0 : ::DebugTraceW
		#define TRACENL						1 ? (void)0 : ::DebugTraceW
	#else	//	_UNICODE
		#define TRACE							1 ? (void)0 : ::DebugTraceA
		#define TRACENL						1 ? (void)0 : ::DebugTraceA
	#endif	//	_UNICODE

	#define ASSERT(exp)					((void)0)
	#define STOPHERE					((void)0)
	#define VERIFY(exp)					((void)(exp))
	#define VAPI(exp)					((void)(exp))
	#define SETLOGFILE				((void)0)
	#define TRACE_MEMORY				1 ? (void)0 : ::DebugTraceMemory
#endif	//	_DEBUG

#endif //DEBUGHLP_H
