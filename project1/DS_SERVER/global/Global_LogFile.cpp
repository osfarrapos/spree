#include "..\stdafx.h"
#include "Global_LogFile.h"

bool _j_XFile::xFileCreate(char * cFileName, char * cMode)
{
	char cFile[256] ={0};	
	GetLocalTime( &curTime ) ;
	

	sprintf(cFile,"%s__FOREST_No_%d___%02d%02d%02d_%02dh%02dm.txt", 
		cFileName, g_ForestNum, curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute);

	TRY{
		m_pStream = fopen(cFile,cMode);
		if( m_pStream == NULL )
		{
			return false ;
		}
	}
	CATCH_ALL {
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
	TRY{		
		fputs(pvWriteString, m_pStream) ;
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_XFile::xFileWrite() \n");
	}	
}

bool _j_XFile::xFileClose()
{
	char buf[256]={0};
	
	sprintf(buf,"Close Time : %04d_%02d/%02d %02d:%02d %02dsec\n", 
		curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute,curTime.wSecond);		
	xFileWrite(buf) ;

	TRY{
		if (m_pStream ) {
			if(fclose(m_pStream) == 0)		
				return true;
		}		
	}
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_XFile::xFileClose() \n");
		return false;
	}

	return false;
}

bool _j_GlobalLog::LogFileCreate(char * fileName, char * mode)
{	
	
	iLineCount = 0;	//log count 초기화
	
	sprintf(m_cFileName, fileName) ;	
	if(!xFileCreate(m_cFileName,mode)){
		return false;
	}		

	return true;
}
void _j_GlobalLog::LogFileWrite( const char *format,... )
{
	memset(m_szBuffer, 0, sizeof(m_szBuffer)) ;

	TRY{
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
	CATCH_ALL {
		::PrintConsole("[EXCEPTION] _j_UserLoginLog::LogFileWrite(PUSER pUser, int mode)\n ");
		return;
	}
	
}
bool _j_GlobalLog::LogFileClose()
{
	return xFileClose();
}
