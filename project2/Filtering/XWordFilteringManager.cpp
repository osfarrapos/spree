#include "stdafx.h"
#include "XKernel.h"
#include "XMLPaser.h"
#include "XWordFilteringManager.h"
#include "XFileEx.h"
#include "XProc_MainGameDef.h"
#include "XUnicodeConv.h"

#define _XDEF_YOCK_FILLTER_SAMPLE					"fillteringsample.txt"	//필터링 샐플링 파일

extern void ReplaceCharacter( LPTSTR _string, TCHAR srcchar, TCHAR changechar );
extern _XPackageArchive		g_ScriptArchive;

_XWordFilteringManager::_XWordFilteringManager()
{
	memset( m_LastFilteredWord, 0, sizeof(TCHAR) * 256 );	
}

_XWordFilteringManager::~_XWordFilteringManager()
{
	RemoveAll();
}

void _XWordFilteringManager::RemoveAll()
{
	m_WordFilteringList.RemoveAll();
}

BYTE _XGetFilterStringAttrib( LPTSTR srcstring )
{
	BYTE attrib = 0;
	
	LPTSTR pNameFilteringIndicator = strchr( srcstring, _T('!') );
	LPTSTR pGuildFilteringIndicator = strchr( srcstring, _T('@') );
	LPTSTR pNickFilteringIndicator = strchr( srcstring, _T('#') );
	LPTSTR pAlwaysFilteringIndicator = strchr( srcstring, _T('~') );
	
	if( pNameFilteringIndicator  ) attrib |= _XDEF_WORDFILETERINGTYPE_NAME;
	if( pGuildFilteringIndicator ) attrib |= _XDEF_WORDFILETERINGTYPE_GUILD;
	if( pNickFilteringIndicator  ) attrib |= _XDEF_WORDFILETERINGTYPE_CHANGE;
	if( pAlwaysFilteringIndicator) attrib |= _XDEF_WORDFILETERINGTYPE_ALWAYS;

	if( attrib == 0 ) 
		attrib |= _XDEF_WORDFILETERINGTYPE_ALWAYS;

	return attrib;
}

BOOL _XWordFilteringManager::DataInit(void)
{
	char	strData[256];
	
	int		cbData		= 0;
	int		cbChangeData = 0;
	int		nMaxWord	= 0;

	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* pScriptFile = g_ScriptArchive.GetPackedFile( _XDEF_YOCK_FILLTER_SAMPLE );
#ifdef _XDEF_NEWVERSONFILETERING
	TCHAR	AttribString[8];
	TCHAR	DataString[128];
	TCHAR	DataString1[128];
	
	if(pScriptFile)
	{
		nMaxWord = sizeof(strData);
		memset(strData, 0x00, 256);
		while( 1 )	
		{	
			if( feof(pScriptFile) ) break;
			
			if( !fgets(strData, nMaxWord, pScriptFile) ) break;
			
			if(strData[0] == ';' || strData[0] == '\n' || !strlen(strData)) continue;
			
			if( strncmp( strData, _T("_XM_EOS"), 7 ) == 0 ) break;
			
			memset( AttribString, 0, sizeof(TCHAR) * 8 );
			memset( DataString, 0, sizeof(TCHAR) * 128 );
			memset( DataString1, 0, sizeof(TCHAR) * 128 );
			sscanf( strData, "%s", AttribString );
			
			if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
			{
				TCHAR	TempConvString[128];
				memset( TempConvString, 0, sizeof(TCHAR) * 128 );
				int inputlen = strlen(strData);
				int maxlength = 128;

				if( _XGetFilterStringAttrib( AttribString ) & _XDEF_WORDFILETERINGTYPE_ALWAYS )
				{
					_XCP1258Conv( (LPBYTE)strData, (LPBYTE)TempConvString, inputlen, maxlength, CONV_LOWERCASE /*| CONV_STRIPACCENT*/ );
				}
				else
				{
					_XCP1258Conv( (LPBYTE)strData, (LPBYTE)TempConvString, inputlen, maxlength, CONV_LOWERCASE | CONV_STRIPACCENT );
				}

				memset( strData, 0, sizeof(TCHAR) * 128 );
				strcpy( strData, TempConvString );
			}
			else // 알파벳일 경우 대문자를 소문자로 변환 
			{
				if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
				{
					int stringlength  = strlen( strData );
					for( int i = 0; i < stringlength; i++ )
					{
						if( strData[i] >= 0x0041 && strData[i] <= 0x005A ) // Alphabet 
						{
							strData[i] = strData[i] + 32;
						}
					}
				}	
				else
				{
					strlwr(strData);
				}
			}
			
			if( _XGetFilterStringAttrib( AttribString ) & _XDEF_WORDFILETERINGTYPE_CHANGE )
			{
				int indexDataString = 0;
				bool bcompletedatastring = false;
				bool bstart = false;
				for( int i = 0; i < 256; ++i )
				{
					if( !bcompletedatastring )
					{
						if( !bstart )
						{
							if( strData[i] == 9 ) 
								bstart = true;
						}
						else
						{
							if( strData[i] == ';' ) 
							{
								bstart = false;
								bcompletedatastring = true;
								indexDataString  = 0;
							}
							else
							{
								DataString[indexDataString] = strData[i];
								++indexDataString;
							}
						}				

					}
					else
					{
						if( !bstart )
						{
							if( strData[i] == 9 ) 
								bstart = true;
						}
						else
						{
							if( strData[i] == ';' ) 
							{
								break;
							}
							else
							{
								DataString1[indexDataString] = strData[i];
								++indexDataString;
							}
						}					
					}
				}
				cbData = strlen(DataString);
				cbChangeData = strlen(DataString1);
				if( cbData > 0 && cbChangeData > 0 )
				{
					TCHAR Data[64];
					TCHAR ChangeData[64];
					memset(Data, 0x00, 64);
					memset(ChangeData, 0x00, 64);
					strncpy(Data,DataString,cbData);
					strncpy(ChangeData,DataString1,cbChangeData);

					for(int i = 0; i < cbChangeData; i++ )
					{
						if( ChangeData[i] == _T('_') ) // underbar to blank
								ChangeData[i] = _T(' ');
					}

					strlwr(ChangeData);				

					for(i = 0; i < cbData; i++ )
					{
						if( Data[i] == _T('_') ) // underbar to blank
								Data[i] = _T(' ');
					}			

					m_WordFilteringList.AddChangeData(Data, strlen(Data), ChangeData, strlen(ChangeData), _XGetFilterStringAttrib( AttribString ) );
					memset(strData, 0x00, 256);
				}
			}
			else
			{
				int indexDataString = 0;
				int startindex = 0;
				int endindex = 0;
				bool bstart = false;
				for( int i = 0; i < 256; ++i )
				{
					if( !bstart )
					{
						if( strData[i] == 9 ) 
							bstart = true;
					}
					else
					{
						if( strData[i] == ';' ) 
						{
							bstart = false;
							break;
						}
						else
						{
							DataString[indexDataString] = strData[i];
							++indexDataString;
						}
					}				
				}
				cbData = strlen(DataString);
				if( cbData > 0 )
				{
					TCHAR Data[64];
					memset(Data, 0x00, 64);
					strncpy(Data,DataString,cbData);

					for(int i = 0; i < cbData; i++ )
					{
						if( Data[i] == _T('_') ) // underbar to blank
								Data[i] = _T(' ');
					}

					m_WordFilteringList.AddData(Data, strlen(Data), _XGetFilterStringAttrib( AttribString ) );

					memset(strData, 0x00, 256);
				}
			}
			
		}		
	}
	else
	{
		_XFatalError( "Loading word filtering database [%s]", _XDEF_YOCK_FILLTER_SAMPLE );
		return false;
	}

#else
	TCHAR	AttribString[8];
	TCHAR	DataString[128];
	
	if(pScriptFile)
	{
		nMaxWord = sizeof(strData);
		memset(strData, 0x00, 256);
		while( 1 )	
		{	
			if( feof(pScriptFile) ) break;
			
			if( !fgets(strData, nMaxWord, pScriptFile) ) break;
			
			if(strData[0] == ';' || strData[0] == '\n' || !strlen(strData)) continue;
			
			if( strncmp( strData, _T("_XM_EOS"), 7 ) == 0 ) break;
			
			memset( AttribString, 0, sizeof(TCHAR) * 8 );
			memset( DataString, 0, sizeof(TCHAR) * 128 );
			sscanf( strData, "%s %s", AttribString, DataString );
			
			cbData = strlen(DataString);
			if( cbData > 0 )
			{
				TCHAR Data[64];
				memset(Data, 0x00, 64);
				strncpy(Data,DataString,cbData);
				m_WordFilteringList.AddData(Data, strlen(Data), _XGetFilterStringAttrib( AttribString ) );				

				memset(strData, 0x00, 256);
			}
		}		
	}
	else
	{
		_XFatalError( "Loading word filtering database [%s]", _XDEF_YOCK_FILLTER_SAMPLE );
		return false;
	}

#endif

	g_ScriptArchive.ClosePackage();
	
	return true;
}

BOOL _XWordFilteringManager::DataInit(XMLPaser& m_xXML) //Author : 양희왕 추가
{
	char	strData[256];
	
	int		cbData		= 0;
	int		cbChangeData = 0;
	int		nMaxWord	= 0;

	if( !m_xXML.ND_isThereTable( _T("Dictionary") ) )
	{
		_XLog("Gameinfo.xml Error seet:Dictionary");
		return FALSE;
	}

	int TableRowCount = m_xXML.ND_nGetTableRowCount( _T("Dictionary") );

	if(TableRowCount <=0)
	{
		_XLog("Gameinfo.xml RowCount Error seet:Dictionary");
		return FALSE;
	}

	TableRowCount -= 1;

	nMaxWord = sizeof(strData);
	memset(strData, 0x00, sizeof(strData));

#ifdef _XDEF_NEWVERSONFILETERING
	TCHAR	AttribString[8];
	TCHAR	DataString[128];
	TCHAR	DataString1[128];

	for( int i = 0; i < TableRowCount; i++ )
	{
		memset( AttribString, 0, sizeof(AttribString));
		memset( DataString, 0, sizeof(DataString));
		memset( DataString1, 0, sizeof(DataString1));
	}
	
	/*if(pScriptFile)
	{
		
		while( 1 )	
		{	
			if( feof(pScriptFile) ) break;
			
			if( !fgets(strData, nMaxWord, pScriptFile) ) break;
			
			if(strData[0] == ';' || strData[0] == '\n' || !strlen(strData)) continue;
			
			if( strncmp( strData, _T("_XM_EOS"), 7 ) == 0 ) break;
			
			memset( AttribString, 0, sizeof(TCHAR) * 8 );
			memset( DataString, 0, sizeof(TCHAR) * 128 );
			memset( DataString1, 0, sizeof(TCHAR) * 128 );
			sscanf( strData, "%s", AttribString );
			
			if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
			{
				TCHAR	TempConvString[128];
				memset( TempConvString, 0, sizeof(TCHAR) * 128 );
				int inputlen = strlen(strData);
				int maxlength = 128;

				if( _XGetFilterStringAttrib( AttribString ) & _XDEF_WORDFILETERINGTYPE_ALWAYS )
				{
					_XCP1258Conv( (LPBYTE)strData, (LPBYTE)TempConvString, inputlen, maxlength, CONV_LOWERCASE /*| CONV_STRIPACCENT );
				}
				else
				{
					_XCP1258Conv( (LPBYTE)strData, (LPBYTE)TempConvString, inputlen, maxlength, CONV_LOWERCASE | CONV_STRIPACCENT );
				}

				memset( strData, 0, sizeof(TCHAR) * 128 );
				strcpy( strData, TempConvString );
			}
			else // 알파벳일 경우 대문자를 소문자로 변환 
			{
				if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
				{
					int stringlength  = strlen( strData );
					for( int i = 0; i < stringlength; i++ )
					{
						if( strData[i] >= 0x0041 && strData[i] <= 0x005A ) // Alphabet 
						{
							strData[i] = strData[i] + 32;
						}
					}
				}	
				else
				{
					strlwr(strData);
				}
			}
			
			if( _XGetFilterStringAttrib( AttribString ) & _XDEF_WORDFILETERINGTYPE_CHANGE )
			{
				int indexDataString = 0;
				bool bcompletedatastring = false;
				bool bstart = false;
				for( int i = 0; i < 256; ++i )
				{
					if( !bcompletedatastring )
					{
						if( !bstart )
						{
							if( strData[i] == 9 ) 
								bstart = true;
						}
						else
						{
							if( strData[i] == ';' ) 
							{
								bstart = false;
								bcompletedatastring = true;
								indexDataString  = 0;
							}
							else
							{
								DataString[indexDataString] = strData[i];
								++indexDataString;
							}
						}				

					}
					else
					{
						if( !bstart )
						{
							if( strData[i] == 9 ) 
								bstart = true;
						}
						else
						{
							if( strData[i] == ';' ) 
							{
								break;
							}
							else
							{
								DataString1[indexDataString] = strData[i];
								++indexDataString;
							}
						}					
					}
				}
				cbData = strlen(DataString);
				cbChangeData = strlen(DataString1);
				if( cbData > 0 && cbChangeData > 0 )
				{
					TCHAR Data[64];
					TCHAR ChangeData[64];
					memset(Data, 0x00, 64);
					memset(ChangeData, 0x00, 64);
					strncpy(Data,DataString,cbData);
					strncpy(ChangeData,DataString1,cbChangeData);

					for(int i = 0; i < cbChangeData; i++ )
					{
						if( ChangeData[i] == _T('_') ) // underbar to blank
								ChangeData[i] = _T(' ');
					}

					strlwr(ChangeData);				

					for(i = 0; i < cbData; i++ )
					{
						if( Data[i] == _T('_') ) // underbar to blank
								Data[i] = _T(' ');
					}			

					m_WordFilteringList.AddChangeData(Data, strlen(Data), ChangeData, strlen(ChangeData), _XGetFilterStringAttrib( AttribString ) );
					memset(strData, 0x00, 256);
				}
			}
			else
			{
				int indexDataString = 0;
				int startindex = 0;
				int endindex = 0;
				bool bstart = false;
				for( int i = 0; i < 256; ++i )
				{
					if( !bstart )
					{
						if( strData[i] == 9 ) 
							bstart = true;
					}
					else
					{
						if( strData[i] == ';' ) 
						{
							bstart = false;
							break;
						}
						else
						{
							DataString[indexDataString] = strData[i];
							++indexDataString;
						}
					}				
				}
				cbData = strlen(DataString);
				if( cbData > 0 )
				{
					TCHAR Data[64];
					memset(Data, 0x00, 64);
					strncpy(Data,DataString,cbData);

					for(int i = 0; i < cbData; i++ )
					{
						if( Data[i] == _T('_') ) // underbar to blank
								Data[i] = _T(' ');
					}

					m_WordFilteringList.AddData(Data, strlen(Data), _XGetFilterStringAttrib( AttribString ) );

					memset(strData, 0x00, 256);
				}
			}
			
		}		
	}
	else
	{
		_XFatalError( "Loading word filtering database [%s]", _XDEF_YOCK_FILLTER_SAMPLE );
		return false;
	}*/
#endif

	return TRUE;
}


//=============================================================================================================
//리스트에서 키값을 이용해서.. 리스트에서 검색
//=============================================================================================================
BOOL _XWordFilteringManager::Filtering(LPTSTR strCmpData, BYTE filteringtype, BOOL checkwordonly )
{
	/*
	if( g_LanguageType != _XLANGUAGE_TYPE_KOREAN )
	{
		return TRUE;
	}
	*/
	
#ifdef _XDEF_NEWVERSONFILETERING
	
	TCHAR					szTempStrCmpData[1024];
	memset( szTempStrCmpData, 0, 1024 );
	strcpy( szTempStrCmpData, strCmpData );

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		TCHAR	TempConvString[1024];
		memset( TempConvString, 0, sizeof(TCHAR) * 1024 );
		int inputlen = strlen(szTempStrCmpData);
		int maxlength = 1024;

		if( filteringtype & _XDEF_WORDFILETERINGTYPE_ALWAYS )
		{
			_XCP1258Conv( (LPBYTE)szTempStrCmpData, (LPBYTE)TempConvString, inputlen, maxlength, CONV_LOWERCASE );
		}
		else
		{
			_XCP1258Conv( (LPBYTE)szTempStrCmpData, (LPBYTE)TempConvString, inputlen, maxlength, CONV_LOWERCASE | CONV_STRIPACCENT );
		}
		memset( szTempStrCmpData, 0, sizeof(TCHAR) * 1024 );
		strcpy( szTempStrCmpData, TempConvString );
	}
	else
	{
		if(g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
		{
			LPTSTR pStr = szTempStrCmpData;
			while( *pStr )
			{
		        if( (BYTE)*pStr > 127 )
				{
					++pStr;
				}
				else
				{
					if( *pStr >= 'A' && *pStr <= 'Z' )
					{
						*pStr += 0x20;
					}					
				}	
				
				++pStr;
			}
		}
		else
		{
			strlwr(szTempStrCmpData);
		}
	}

	BOOL					bIsFiller	= TRUE; // 필터링 유무에 상관없이 사용해도 되는지 사용하면 안되는지를 나타낸다.
	
	char*					pCmpData	= szTempStrCmpData;
	char*					pLastData	= pCmpData + strlen(szTempStrCmpData);
	char*					pData;
	BYTE					retfilteringtype = filteringtype;
	

	if( filteringtype & _XDEF_WORDFILETERINGTYPE_CHANGE)
	{
		
		int		TargetDataLength = 0;
		int		DataLength = 0;
		int		TempDataBufferLength = 0;
		TCHAR	TotalDataBuffer[1024];
		TCHAR	TempDataBuffer[1024];
		memset( TotalDataBuffer, 0, sizeof(TCHAR) * 1024 );
		memset( TempDataBuffer, 0, sizeof(TCHAR) * 1024 );
		strncpy( TotalDataBuffer,  szTempStrCmpData, strlen(szTempStrCmpData));
		bool	findchangedata = false;

		int length = 0;
		int offset = 0;
		for (; pCmpData < pLastData; pCmpData++)
		{
			
			if( pCmpData == '\0') return	bIsFiller;
			
			
			if((pData = m_WordFilteringList.SearchChangeData(pCmpData,TargetDataLength,retfilteringtype)) != NULL)
			{
				if( checkwordonly )
				{
					if( retfilteringtype != _XDEF_WORDFILETERINGTYPE_ALWAYS )
					{
						if( strcmp( szTempStrCmpData, pData ) != 0 )
						{
							return FALSE;
						}
				
					}
				}
				
				if( pCmpData )
				{
					memset( m_LastFilteredWord, 0, sizeof(TCHAR) * 256 );
					strcpy( m_LastFilteredWord,  pCmpData );
				}
				DataLength = 0;
				DataLength = strlen(pData);
				
				length = strlen(szTempStrCmpData) - strlen(pCmpData);
				
				strcpy(TempDataBuffer, pCmpData + TargetDataLength );				
				
				TempDataBufferLength = strlen(TempDataBuffer);
				strncpy(TotalDataBuffer+length+offset,pData,DataLength);
				
				if( TempDataBufferLength == 0)
				{
					int DeleteLength = TargetDataLength - strlen(pData);
					if( DeleteLength > 0 )
						memset(TotalDataBuffer+length+offset+DataLength, 0, DeleteLength);
				}
				else
				{
					memset(TotalDataBuffer+length+offset+DataLength, 0, strlen(TempDataBuffer));
				}
				strcat(TotalDataBuffer,TempDataBuffer);
				offset += (strlen(pData)- TargetDataLength);
				
				bIsFiller = TRUE;
				
				pCmpData	= pCmpData + TargetDataLength - 1;
				
				if((unsigned)*pCmpData > 127)
				{
					pCmpData--;
				}
				findchangedata = true;
			}
						
			if ((unsigned)*pCmpData > 127)
			{
				pCmpData++;
			}
		}
		memset( szTempStrCmpData, 0, strlen(szTempStrCmpData) );
		strncpy( szTempStrCmpData,  TotalDataBuffer, strlen(TotalDataBuffer) );
		
		// 베트남은 전부 소문자로 바꾼다. 그 스트링을 복사하면 안됨.
		if(g_LanguageType != _XLANGUAGE_TYPE_VIETNAMESE )
		{
			// 변경할 데이타를 찾았을때만 데이타를 변경한다.
			if( findchangedata )
			{
				memset( strCmpData, 0, strlen(strCmpData) );
				strncpy( strCmpData,  TotalDataBuffer, strlen(TotalDataBuffer) );
			}
		}				
	}
	
	pCmpData	= szTempStrCmpData;
	pLastData	= pCmpData + strlen(szTempStrCmpData);
		
	for (; pCmpData < pLastData; pCmpData++)
	{
		
		if( pCmpData == '\0') return	bIsFiller;
		
		if((pData = m_WordFilteringList.SearchData(pCmpData,retfilteringtype)) != NULL)
		{
			if( checkwordonly )
			{
				if( retfilteringtype != _XDEF_WORDFILETERINGTYPE_ALWAYS )
				{
					if( strcmp( szTempStrCmpData, pData ) != 0 )
					{
						memset( m_LastFilteredWord, 0, sizeof(TCHAR) * 256 );
						strcpy( m_LastFilteredWord,  pData );
						return FALSE;
					}
				
				}
			}
			
			if( pCmpData )
			{
				memset( m_LastFilteredWord, 0, sizeof(TCHAR) * 256 );
				strcpy( m_LastFilteredWord,  pCmpData );
			}
			
			memset(pCmpData,'*' , strlen(pData));
			bIsFiller = TRUE;
			
			pCmpData	= pCmpData + strlen(pData) - 1;
			
			if((unsigned)*pCmpData > 127)
			pCmpData--;
			
		}
		
		if ((unsigned)*pCmpData > 127)
			pCmpData++;
	}
	
	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		int orgstrlen = strlen(szTempStrCmpData);

		int asteriskcount = 0;
		for( int i = 0; i < orgstrlen; i++ )
		{
			if( szTempStrCmpData[i] == '*' )
			{
				asteriskcount++;
			}
			else
			{
				if( asteriskcount > 1 && asteriskcount < 4 )
				{
					BOOL leftspace = FALSE;
					BOOL rightspace = FALSE;

					// check left
					if( i - asteriskcount > 0 )
					{
						if( szTempStrCmpData[i - asteriskcount - 1] == ' ' )
						{
							leftspace = TRUE;
						}
					}
					else
					{
						leftspace = TRUE;
					}

					if( i < orgstrlen ) // check right
					{
						if( szTempStrCmpData[i] == ' ' )
						{
							rightspace = TRUE;
						}
					}
					else
					{
						rightspace = TRUE;
					}

					if( leftspace && rightspace )
					{
						for( int j = 0; j < asteriskcount; j++ )
						{
							strCmpData[i-asteriskcount+j] = '*';
						}
					}

					asteriskcount = 0;
				}
				else
				{
					if( asteriskcount >= 4 )
					{
						for( int j = 0; j < asteriskcount; j++ )
						{
							strCmpData[i-asteriskcount+j] = '*';
						}
					}

					asteriskcount = 0;
				}
			}			
		}

		// case of all asterisk
		if( asteriskcount > 1 && asteriskcount < 4 )
		{
			BOOL leftspace = FALSE;
			BOOL rightspace = FALSE;
			
			// check left
			if( i - asteriskcount > 0 )
			{
				if( szTempStrCmpData[i - asteriskcount - 1] == ' ' )
				{
					leftspace = TRUE;
				}
			}
			else
			{
				leftspace = TRUE;
			}
			
			if( i < orgstrlen ) // check right
			{
				if( szTempStrCmpData[i] == ' ' )
				{
					rightspace = TRUE;
				}
			}
			else
			{
				rightspace = TRUE;
			}
			
			if( leftspace && rightspace )
			{
				for( int j = 0; j < asteriskcount; j++ )
				{
					strCmpData[i-asteriskcount+j] = '*';
				}
			}
			
			asteriskcount = 0;
		}
		else
		{
			if( asteriskcount >= 4 )
			{
				for( int j = 0; j < asteriskcount; j++ )
				{
					strCmpData[i-asteriskcount+j] = '*';
				}
			}
		}
	}
	else
	{
		int orgstrlen = strlen(szTempStrCmpData);
		for( int i = 0; i < orgstrlen; i++ )
		{
			if( szTempStrCmpData[i] == '*' )
			{
				strCmpData[i] = '*';
			}
		}
	}
	
	return bIsFiller;
#else
	
	BOOL					bIsFiller	= FALSE;
	
	char*					pCmpData	= strCmpData;
	char*					pLastData	= pCmpData + strlen(strCmpData);
	char*					pData;
	BYTE					retfilteringtype = filteringtype;
	
	for (; pCmpData < pLastData; pCmpData++)
	{
		
		if( pCmpData == '\0') return	bIsFiller;
		
		if((pData = m_WordFilteringList.SearchData(pCmpData,retfilteringtype)) != NULL)
		{
			if( checkwordonly )
			{
				if( retfilteringtype != _XDEF_WORDFILETERINGTYPE_ALWAYS )
				{
					if( strcmp( strCmpData, pData ) != 0 )
					{
						return FALSE;
					}
				}
			}
			
			if( pCmpData )
			{
				memset( m_LastFilteredWord, 0, sizeof(TCHAR) * 256 );
				strcpy( m_LastFilteredWord,  pCmpData );
			}
		
			memset(pCmpData,'*' , strlen(pData));
			bIsFiller = TRUE;
			
			pCmpData	= pCmpData + strlen(pData) - 1;
			
			if((unsigned)*pCmpData > 127)
				pCmpData--;
			
		}
		
		if ((unsigned)*pCmpData > 127)
			pCmpData++;
	}
	
	return bIsFiller;
#endif

}

void _XWordFilteringManager::SafeDelete()
{
	m_WordFilteringList.RemoveAll();
}

