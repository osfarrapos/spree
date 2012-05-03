// XStringDataBase.cpp: implementation of the _XStringDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XUnicodeConv.h"
#include "XStringDataBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TCHAR	g_NoneString[] = _T("NULL");


_XSTRINGSECTIONINFO::~_XSTRINGSECTIONINFO()
{
	std::for_each( m_StringVector.begin(), m_StringVector.end(), DELETEOBJECT() );
	m_StringVector.clear();

	m_StringMap.clear();
}

_XStringDataBase::_XStringDataBase()
{
	m_TotalSectionCount =	0;
	m_pSectionInfo		=	NULL;
}

_XStringDataBase::~_XStringDataBase()
{
	Release();
}

BOOL _XStringDataBase::Load( LPCTSTR filename, int languageindex )
{
	Release();

	FILE* pFile = fopen( filename, "rb" );

	if( !pFile )
	{
		_XFatalError( _T("Could not open %s file"), filename );
		return FALSE;
	}

	DWORD header = 0x0000;
	fread( &header, sizeof(DWORD), 1, pFile );

	if( header != _XDEF_XSTRINGDBHEADER )
	{
		_XFatalError( _T("Could not support %s file : [%x]"), filename, header );
		fclose( pFile );
		return FALSE;
	}

	DWORD version = 0x0000;
	fread( &version, sizeof(DWORD), 1, pFile );

	if( version != _XDEF_XSTRINGDBVERSION )
	{
		_XFatalError( _T("Could not support this format [%x:%s]: "), version, filename );
		fclose( pFile );
		return FALSE;
	}

	m_TotalSectionCount = 0;
	fread( &m_TotalSectionCount, sizeof(int), 1, pFile );

	m_pSectionInfo = new _XSTRINGSECTIONINFO[ m_TotalSectionCount ];
	
	typedef struct _XSTRINGSECTIONHEADERINFO
	{
		int		stringcnt;
		wchar_t	sectioname[64];
	} XSTRINGSECTIONREADINFO;

	_XSTRINGSECTIONHEADERINFO	sectionstchunk;
	wchar_t						readstring[1024];
	char						mbcsreadstring[1024];
	int							readstringlength;
	
	for( int i = 0; i < m_TotalSectionCount; i++ )
	{
		fread( &sectionstchunk, sizeof(_XSTRINGSECTIONHEADERINFO), 1, pFile );

		m_pSectionInfo[i].m_stringcnt = sectionstchunk.stringcnt;
		memset( m_pSectionInfo[i].m_sectioname, 0, sizeof(TCHAR) * 64 );
		//strcpy( m_pSectionInfo[i].m_sectioname, sectionstchunk.sectioname );
	
		_XTEXTSTRING_INFO*	pTextstringItem;

		for( int j = 0; j < sectionstchunk.stringcnt; j++ )
		{
			pTextstringItem = new _XTEXTSTRING_INFO;

			fread( &pTextstringItem->iResourceIndex, sizeof(int), 1, pFile );

			for( int lancnt = 0; lancnt < _XDEF_MAXLANGUAGECOUNT; lancnt++ )
			{			
				if( languageindex == lancnt )
				{			
					fread( &pTextstringItem->iParameterOrder, sizeof(int), 1, pFile );
				}
				else
				{
					fseek( pFile, sizeof(int), SEEK_CUR );
				}
			}		
			
			for( lancnt = 0; lancnt < _XDEF_MAXLANGUAGECOUNT; lancnt++ )
			{			
				if( languageindex == lancnt )
				{					
					fread( &readstringlength, sizeof(int), 1, pFile );
					
					memset( readstring, 0, sizeof(wchar_t) * 1024 );
					fread( readstring, sizeof(wchar_t) * readstringlength, 1, pFile );


					memset( mbcsreadstring, 0, sizeof(char) * 1024 );

					if( languageindex == _XLANGUAGE_TYPE_VIETNAMESE )
					{
						int strbyteslength = (wcslen(readstring) << 1);
						int outputmaxlength = 1024;
						_XUniCodeCompToCP1258( (LPBYTE)readstring, (LPBYTE)mbcsreadstring, strbyteslength, outputmaxlength );
						
						if( outputmaxlength > 0 )
						{
							pTextstringItem->strTextString = new TCHAR[outputmaxlength+1];
							//strcpy( pTextstringItem->strTextString, mbcsreadstring );
							memcpy( pTextstringItem->strTextString, mbcsreadstring, outputmaxlength );
							pTextstringItem->strTextString[outputmaxlength] = NULL;
						}
						else
						{
							pTextstringItem->strTextString = new TCHAR[5];
							memset( pTextstringItem->strTextString, 0, sizeof(TCHAR) * 5 );
							strcpy( pTextstringItem->strTextString, _T("NULL"));
						}
						//int requirelength = WideCharToMultiByte(1258, WC_SEPCHARS, readstring, -1, mbcsreadstring, 1024, NULL, NULL);
						//pTextstringItem->strTextString = new TCHAR[requirelength];
					}
					else
					{
						int requirelength = WideCharToMultiByte(CP_ACP, 0, readstring, -1, mbcsreadstring, 1024, NULL, NULL);
						pTextstringItem->strTextString = new TCHAR[requirelength];
						//memset( pTextstringItem->strTextString, 0, sizeof( TCHAR ) * requirelength );

						if( requirelength > 0 )
						{
							pTextstringItem->strTextString = new TCHAR[requirelength];
							strcpy( pTextstringItem->strTextString, mbcsreadstring );
						}
						else
						{
							pTextstringItem->strTextString = new TCHAR[5];
							memset( pTextstringItem->strTextString, 0, sizeof(TCHAR) * 5 );
							strcpy( pTextstringItem->strTextString, _T("NULL"));
						}
					}					

					//pTextstringItem->strTextString = new TCHAR[5];
					//memset( pTextstringItem->strTextString, 0, sizeof(TCHAR) * 5 );
					//strcpy( pTextstringItem->strTextString, _T("NULL"));
					

					m_pSectionInfo[i].m_StringVector.push_back( pTextstringItem );
					m_pSectionInfo[i].m_StringMap.insert(smdef_XTEXTSTRING_INFOMAP::value_type(pTextstringItem->iResourceIndex, pTextstringItem));
				}
				else
				{
					fread( &readstringlength, sizeof(int), 1, pFile );
					fseek( pFile, sizeof(wchar_t) * readstringlength, SEEK_CUR );					
				}
			}
		}
	}

	fclose( pFile );

	return TRUE;
}

void _XStringDataBase::Release( void )
{
	SAFE_DELETE_ARRAY( m_pSectionInfo );
	m_pSectionInfo		=	NULL;

	m_TotalSectionCount =	0;	
}

LPTSTR _XStringDataBase::GetTextString( _XSTRINGDB_SECTIONDEFINE sectionindex, int resourceid )
{
	if( m_TotalSectionCount <= sectionindex )
	{
		return g_NoneString;
	}


	smdef_XTEXTSTRING_INFOMAP::iterator it  = m_pSectionInfo[sectionindex].m_StringMap.find(resourceid);	
	if(it != m_pSectionInfo[sectionindex].m_StringMap.end())
	{
		_XTEXTSTRING_INFO* pStringItem = (*it).second;
		if(pStringItem)
		{
			if( pStringItem->strTextString[0] == 0 )
				return g_NoneString;
			else
				return pStringItem->strTextString;
		}
	}
	
	//_XLog("[string] === Not Found %d ===]", resourceid);
	return g_NoneString;
}
