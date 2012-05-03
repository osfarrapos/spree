#include "..\stdafx.h"
#include "IP_Filter.h"


bool CIpFilter::IpListFileRead( const char * cFileName )
{
	
	char cLineStr[256] ;
	char cIP[32] ;
	FILE * fStream = NULL ;
	if( (fStream = fopen(cFileName, "r")) == NULL )
	{
		g_pErrMsg->LinePrint( "[FAIL] ip list file <%s> read failed", cFileName ) ;
		return false ;
	}
	
	while (1) {
		memset( cLineStr, 0, 256 ) ;	
		if( fgets( cLineStr, 256, fStream ) == NULL ) {
			break ;
		}
		sscanf(cLineStr, "%s", cIP) ;
		
		InsertIp(cIP) ;
	}
	
	fclose(fStream) ;
	return true ;
}