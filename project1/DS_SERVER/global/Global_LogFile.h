#pragma	   once

class _j_XFile
{
public:
	FILE *	m_pStream;
	SYSTEMTIME curTime ;
	
	
	bool	xFileCreate(char * cFileName, char * cMode);
	void	xFileWrite(char * write);
	bool	xFileClose();
	
	
protected:
private:
};

class _j_GlobalLog : public _j_XFile
{
public:	
	
	_j_GlobalLog() : iLineCount(0)
	{	
		memset( m_cFileName, 0, 256 ) ;
	}

	~_j_GlobalLog() {
		Reflush() ;
		if ( m_pStream ) {
			xFileClose() ;
		}			
	}
	
	int		iLineCount;
	char	m_szBuffer[2048] ;
	
	char	m_cFileName[256] ;
	
	bool LogFileCreate(char * fileName, char * mode);
	void LogFileWrite(const char *format,... );
	bool LogFileClose();
	void Reflush() 
	{ 
		if( m_pStream )
		{
			fflush( m_pStream ) ; 
		}
	}	
};
