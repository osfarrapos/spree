// PackFileItem.cpp: implementation of the CPackFileItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XPacker.h"
#include "PackFileItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPackFileItem::CPackFileItem()
{
	memset( &rFileInfo, 0, sizeof( SHFILEINFO ) );
	memset( &m_OrgFileName, 0, 512 );
	rFileSize = -1;
}

CPackFileItem::CPackFileItem( LPCTSTR filename )
{
	memset( &rFileInfo, 0, sizeof( SHFILEINFO ) );
	memset( &m_OrgFileName, 0, 512 );
	rFileSize = -1;

	setFileInformations( filename );
}

CPackFileItem::~CPackFileItem()
{

}

void CPackFileItem::setFileInformations( LPCTSTR filename )
{	 
	HANDLE      hFind;
	WIN32_FIND_DATA rFind;

	hFind = FindFirstFile ( filename, &rFind );

    if ( INVALID_HANDLE_VALUE != hFind )
    {
		this->rFileSize = rFind.nFileSizeLow;
		FindClose ( hFind );

		SHGetFileInfo ( filename, 0, &rFileInfo, sizeof(rFileInfo),
					SHGFI_SYSICONINDEX | SHGFI_ATTRIBUTES | SHGFI_TYPENAME );

		memset( rFileInfo.szDisplayName, 0, _MAX_PATH );

		if( _MAX_PATH <= strlen(filename) )
		{
			 strncpy( rFileInfo.szDisplayName, filename, _MAX_PATH-1 );	
		}
		else strcpy( rFileInfo.szDisplayName, filename );
    }
	else
	{
		memset( &rFileInfo, 0, sizeof( SHFILEINFO ) );
		rFileSize = -1;
	}
}
