#include "..\stdafx.h"
#include "Global_LogFile.h"




bool _j_XFile::xFileCreate(char * cFileName, char * cMode)
{
	char cFile[512] ={0};	
	GetLocalTime( &curTime ) ;
	
	BOOL success ;
	success = CreateDirectory( "logfile", 0 ) ;
	if( !success )
	{
		int err = GetLastError() ;
		if( err != ERROR_ALREADY_EXISTS )
		{
			printf( "Create Directory Failed : %d\n", err ) ;
			return false ;
		}
	}

	sprintf(cFile,"logfile\\%s_%d_%d %d %d_%d.txt", 
		cFileName, curTime.wMonth,curTime.wDay,curTime.wYear,curTime.wHour,curTime.wMinute);

	try{
		m_pStream = fopen(cFile,cMode);
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_XFile::xFileCreate() -fopen() \n");
		return false;
	}
	

	if(m_pStream == NULL){
		return false;
	}

	xFileWrite(cFileName);
	xFileWrite("\n");
	xFileWrite("============================================================\n");	
	xFileWrite(cFile);	
	xFileWrite("============================================================\n");		


	return true;
}

void _j_XFile::xFileWrite(char * pvWriteString)
{
	try{		
		fputs(pvWriteString, m_pStream) ;
		fflush(m_pStream) ;
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_XFile::xFileWrite() \n");
		return;
	}	
}

bool _j_XFile::xFileClose()
{
	if( m_pStream )
	{
		char buf[512]={0};
		
		sprintf(buf,"Close : %d/%d %d %d:%d\n", 
			curTime.wMonth,curTime.wDay,curTime.wYear,curTime.wHour,curTime.wMinute);		
		xFileWrite(buf) ;

		if(fclose(m_pStream) == 0)		
			return true;
	}

	return false;
}

bool _j_GlobalLog::LogFileCreate(char * fileName, char * mode)
{	
	
	iLineCount = 0;	//log count 초기화

	memset(m_cFileName, 0, sizeof(m_cFileName)) ;
	sprintf(m_cFileName, fileName) ;
	
	if(!xFileCreate(m_cFileName,mode)){
		return false;
	}		

	return true;
}
void _j_GlobalLog::LogFileWrite( const char *format,... )
{
	memset(m_szBuffer, 0, sizeof(m_szBuffer)) ;

	try{
		if(iLineCount > LOG_MAX_LINE){
			LogFileClose();			// 쓰던 로그파일 닫고
			LogFileCreate(m_cFileName,"w+");		// 새 로그파일 생성
			iLineCount = 0;			
		}
		
		va_list arglist ;
		va_start(arglist, format) ;
		vsprintf(m_szBuffer, format, arglist) ;
		va_end(arglist) ;
		xFileWrite(m_szBuffer);
		iLineCount++;			// 로그 라인 카운터 증가
		
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] _j_UserLoginLog::LogFileWrite(PUSER pUser, int mode)\n ");
		return;
	}
	
}
bool _j_GlobalLog::LogFileClose()
{
	return xFileClose();
}
