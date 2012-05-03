#pragma	  once

#include "Global_Function.h"
#include "../Mylibrary/MY_TRACE.H"


enum  {
	_TRACE_CLASS_ERROR,
	_TRACE_CLASS_BACKUP,
	_TRACE_CLASS_UPDATE,
	_TRACE_CLASS_LOGIN,
	_TRACE_CLASS_CHARAC,
	_TRACE_CLASS_GMS,	
	_TRACE_CLASS_SYSTEM,
	_TRACE_CLASS_DB,
	_TRACE_CLASS_MESSAGE,
	_TRACE_CLASS_BILLING,
	_TRACE_CLASS_MAX,
	
} ;


class CMYTrace : public CSystemTrace
{
public:

	CMYTrace() : CSystemTrace( _TRACE_CLASS_MAX )
	{		
	}


	void OutputDevice( const char * title, const char * str )
	{
		/*
		SYSTEMTIME	time ;
		GetLocalTime(&time ) ;
		::PrintConsole("[%u:%u:%u][%s] %s ", time.wHour, time.wMinute, time.wSecond, title, str ) ;
		*/
		::PrintConsole( "%s%s", title, str ) ;
	}

	void OutputDevice( const char * str )
	{
		::PrintConsole( str ) ;
	}

	const char * HTMLFormatingString( const char * title, const char * str, const int iTraceClassNum ) ;

	void SetAllTrace( bool b )
	{
		for( int i = 0 ; i < _TRACE_CLASS_MAX ; i++ )
		{
			ShowTraceString( i, b ) ;
		}		
	}

protected:
private:
};
