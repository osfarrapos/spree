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
	~_j_GlobalLog() {
		if ( m_pStream ) {
			xFileClose() ;
		}
	}
	
	int		iLineCount;
	char	m_szBuffer[1024] ;

	char	m_cFileName[512] ;

	bool LogFileCreate(char * fileName, char * mode);
	void LogFileWrite(const char *format,... );
	bool LogFileClose();
};
