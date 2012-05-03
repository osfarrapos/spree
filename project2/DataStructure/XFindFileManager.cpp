// XFindFileManager.cpp: implementation of the _XFindFileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XFindFileManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XFindFileManager::_XFindFileManager()
{
	m_FoundFileCount = 0;
}

_XFindFileManager::~_XFindFileManager()
{
	m_smFindFileTable.clear();
	
	std::for_each( m_svFindFileNameList.begin(), m_svFindFileNameList.end(), DELETEOBJECT() );
	m_svFindFileNameList.clear();
}

void _XFindFileManager::FindAllFile( LPCTSTR rootpath )
{
	m_smFindFileTable.clear();

	std::for_each(m_svFindFileNameList.begin(), m_svFindFileNameList.end(), DELETEOBJECT());
	m_svFindFileNameList.clear();

	m_FoundFileCount = 0;

	FindFile( rootpath );
}

void _XFindFileManager::FindFile( LPCTSTR rootpath )
{
	if( NULL == rootpath ) return;
	
	WIN32_FIND_DATA		hFinder;
    BOOL				result;
    char				buf[512];
    HANDLE				hFindFile;
    int					nFileCount = 0;
	
    _snprintf( buf, sizeof(buf), "%s\\*.*", rootpath );
    hFindFile = FindFirstFile(buf, &hFinder);
	
    if(hFindFile == INVALID_HANDLE_VALUE)
        return;
	
    result = TRUE;
    while(result)
    {
        if((result = FindNextFile(hFindFile, &hFinder)) == 0)
            break;
		
        if(hFinder.cFileName[0] == '.')
            continue;
		
		if( (hFinder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  ==    FILE_ATTRIBUTE_DIRECTORY )
        {
            _snprintf(buf, sizeof(buf), "%s\\%s", rootpath, hFinder.cFileName);
			FindFile( buf );
            continue;
        }
		else
		{
			_XFINDFILE_INFO* pFindFileInfo = new _XFINDFILE_INFO;
			pFindFileInfo->strPathfileName  = rootpath;
			pFindFileInfo->strPathfileName += _T('\\');
			pFindFileInfo->strPathfileName += hFinder.cFileName;
			pFindFileInfo->strFileName = hFinder.cFileName;
			pFindFileInfo->referenceindex = -1;
						
			m_svFindFileNameList.push_back( pFindFileInfo );
			m_smFindFileTable.insert(smdef_FileNameTable::value_type(pFindFileInfo->strFileName, m_FoundFileCount));
			m_FoundFileCount++;
		}
    }

    FindClose(hFindFile);
}	


int _XFindFileManager::FindFileInList( LPCTSTR filename )
{
	smdef_FileNameTable::iterator it = m_smFindFileTable.find(filename);
	if ( it != m_smFindFileTable.end() )
	{
		return (*it).second;
	}
	return -1;
}

LPCTSTR _XFindFileManager::GetPathName( int index )
{
	if( index > -1 )
	{
		if( m_svFindFileNameList.size() <= index )
		{
			return NULL;
		}
		
		return m_svFindFileNameList[ index ]->strPathfileName.c_str();
	}
	return NULL;
}

LPCTSTR _XFindFileManager::GetPathName( LPCTSTR filename )
{
	int index = FindFileInList( filename );
	if( index > -1 )
	{
		if( m_svFindFileNameList.size() <= index )
		{
			return NULL;
		}
		
		return m_svFindFileNameList[ index ]->strPathfileName.c_str();
	}
	return NULL;
}

_XFINDFILE_INFO* _XFindFileManager::GetInfo( int index )
{	
	if( index > -1 )
	{
		if( m_svFindFileNameList.size() <= index )
		{
			return NULL;
		}
		
		return m_svFindFileNameList[ index ];
	}
	return NULL;
}

int	_XFindFileManager::GetReferenceData( LPCTSTR filename )
{
	int index = FindFileInList( filename );

	if( index > -1 )
	{
		if( m_svFindFileNameList.size() <= index )
		{
			return -1;
		}
		else
		{
			return m_svFindFileNameList[ index ]->referenceindex;
		}
	}

	return -1;
}


void _XFindFileManager::SetInfo( int index, int data )
{
	if( index > -1 )
	{
		if( m_svFindFileNameList.size() > index )
		{		
			m_svFindFileNameList[ index ]->referenceindex = data;
		}
	}
}