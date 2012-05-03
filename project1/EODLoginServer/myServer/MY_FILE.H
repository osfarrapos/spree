/*================================================================================
 by ozzywow
=================================================================================*/


#pragma	   once


class CLinePrintFile
{

protected:

	char	m_buf[512] ;

	FILE *	m_pStream;	
	CRITICAL_SECTION	cs ;
	
	int		m_print_line;

public:

	CLinePrintFile() : m_pStream(NULL)
	{
#if (_WIN32_WINNT > 0x0403)
		if( FALSE == InitializeCriticalSectionAndSpinCount( &cs, (0x80000000)|2000 ) )
		{
			throw ;
		}
#else
		InitializeCriticalSection( &cs ) ;
#endif
	}
	~CLinePrintFile() 
	{
		fflush(m_pStream);
		FileClose() ;
		DeleteCriticalSection( &cs ) ;
	}

	bool	Create(const char * cFileName , const char * cMode, const char * cFormat = "txt")
	{
		bool bRet = true ;	
		SYSTEMTIME curTime ;
		GetLocalTime( &curTime ) ;

		m_print_line = 0;
		
		EnterCriticalSection( &cs ) ;
		sprintf(m_buf,"%s__%d-%d-%d__%dh%dm%ds.%s", 
			cFileName, curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute,curTime.wSecond, cFormat);		
		
		m_pStream = fopen(m_buf,cMode);
		if(m_pStream == NULL)
		{
			bRet = false ;
		}
			
		LinePrint(m_buf, strlen( m_buf ) );
		LeaveCriticalSection( &cs ) ;
		
		return bRet ;
	}

	void	LinePrint(const char * str,...)
	{
		SYSTEMTIME curTime ;
		GetLocalTime( &curTime ) ;

		EnterCriticalSection( &cs ) ;		
		va_list arglist ;
		va_start(arglist, str) ;
		vsprintf( m_buf, str, arglist) ;
		va_end(arglist) ;
		size_t writenSize = fwrite( m_buf, 1, strlen(m_buf), m_pStream);
		if(writenSize > 0){
			fflush(m_pStream);		
		}
		++m_print_line;

		if( m_print_line > 20,000 )
		{
			FileClose();
			Create( "LogFiles\\system_error", "at", "TXT" );
		}
		LeaveCriticalSection( &cs ) ;
	}


	bool	FileClose()
	{
		
		SYSTEMTIME curTime ;
		GetLocalTime( &curTime ) ;
		
		bool bRet  = false;
		
		EnterCriticalSection( &cs ) ;
		sprintf(m_buf,"End time : %d-%d-%d_%d:%d:%d  \n", 
			curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute,curTime.wSecond);
		fwrite(m_buf, 1, strlen(m_buf), m_pStream);
		
		if(fclose(m_pStream) == 0)		
		{	
			bRet = true ;
		}	
		
		LeaveCriticalSection( &cs ) ;
		return bRet ;		
	}
	


private:
};
