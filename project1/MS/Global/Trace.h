#pragma	  once

#include "Global_Function.h"
#include "../MyLib/MY_TRACE.H"

#define __TPOS  "==> %s(%d)",__FILE__, __LINE__


enum  {
	_TRACE_CLASS_ERROR,
	_TRACE_CLASS_BACKUP,
	_TRACE_CLASS_UPDATE,
	_TRACE_CLASS_LOGIN,		
	_TRACE_CLASS_MESSAGE,
	_TRACE_CLASS_SYSTEM,
	_TRACE_CLASS_DB,
	_TRACE_CLASS_MAX,
} ;


class CMYTrace : public CSystemTrace
{
	
public:

	CMYTrace() : CSystemTrace( _TRACE_CLASS_MAX )
	{		
	}

	virtual void OutputDevice( const char * title, const char * str )
	{
		SYSTEMTIME	time ;
		GetLocalTime(&time ) ;
		::PrintConsole("[%d:%d:%d][%s] %s ", time.wHour, time.wMinute, time.wSecond, title, str ) ;
	}

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
