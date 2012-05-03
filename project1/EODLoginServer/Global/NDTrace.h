#pragma	  once

#include "Global_Function.h"
#include "../myServer/MY_TRACE.H"

// Trace title index info
enum { 
	_TRACE_CLASS_NONE=0,
	_TRACE_CLASS_ERROR, 
	_TRACE_CLASS_DB, 
	_TRACE_CLASS_LOGIN, 
	_TRACE_CLASS_LOGIN_SUCCESS, 
	_TRACE_CLASS_SYSTEM,
	_TRACE_CLASS_BILLING,
	_TRACE_CLASS_MAX 
};

#define _TRACE_BUF_MAX_SIZE	1024

class CMYTrace : public CSystemTrace, public CLinePrintFile
{
public:
	char * strBuf ;

	CMYTrace() : CSystemTrace( _TRACE_CLASS_MAX, _ns_trace_::_FORMAT_TEXT )
	{		
		CLinePrintFile::Create( "LogFiles\\systemTrace", "w+", "txt") ;
		strBuf = new char[_TRACE_BUF_MAX_SIZE] ;
	}
	~CMYTrace()
	{		
		delete [] strBuf ;
		CLinePrintFile::FileClose() ;
	}

	void OutputDevice( const char * title, const char * str )
	{		
		::PrintConsole("[%s] %s ", title, str ) ;		
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