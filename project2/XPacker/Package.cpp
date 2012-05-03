// Package.cpp: implementation of the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Package.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define _HEADERINDICATOR_PACKAGEFILE              0xFEFA
#define _HEADERINDICATOR_PACKAGEFILEVERSION		  0xA001


typedef struct 
{

	WORD HeaderIndicator;
	WORD FileVersion;	
	int  PackedFileCount;

} _PACKAGEFILE_HEADER;


BOOL CheckExistFile( LPCSTR filename, BOOL resetAttrib )
{
	HANDLE      hFind;
	WIN32_FIND_DATA rFind;	

	hFind = FindFirstFile ( filename, &rFind );

	if ( INVALID_HANDLE_VALUE == hFind )
	{
		FindClose ( hFind );
		return FALSE;
	}

	FindClose ( hFind );

	if( resetAttrib )
	{
		CFileStatus filestatus;
		CFile::GetStatus( filename, filestatus );
		filestatus.m_attribute = 0x00; // normal file attrib setting
		CFile::SetStatus( filename, filestatus );
	}

	return TRUE;
}


BOOL WriteDirectory(CString dd)
{


	HANDLE		fFile;					// File Handle
	WIN32_FIND_DATA fileinfo;			// File Information Structure
	CStringArray	m_arr;				// CString Array to hold Directory Structures
	BOOL tt;							// BOOL used to test if Create Directory was successful
	int x1 = 0;							// Counter
	CString tem = "";					// Temporary CString Object

	// Before we go to a lot of work.  
	// Does the file exist

	fFile = FindFirstFile(dd,&fileinfo);

	// if the file exists and it is a directory
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		//  Directory Exists close file and return
		FindClose(fFile);
		return TRUE;
	}

	m_arr.RemoveAll();					// Not really necessary - Just habit
	for(x1=0;x1<dd.GetLength();x1++)	// Parse the supplied CString Directory String
	{									
		if(dd.GetAt(x1) != '\\')		// if the Charachter is not a \ 
			tem += dd.GetAt(x1);		// else add character to Temp String
		else
		{
			m_arr.Add(tem);				// if the Character is a \ Add the Temp String to the CString Array
			tem += "\\";				// Now add the \ to the temp string
		}
		if(x1 == dd.GetLength()-1)		// If we reached the end of the file add the remaining string
			m_arr.Add(tem);
	}


	// Close the file
	FindClose(fFile);
	
	// Now lets cycle through the String Array and create each directory in turn
	for(x1 = 1;x1<m_arr.GetSize();x1++)
	{
		tem = m_arr.GetAt(x1);
		tt = CreateDirectory(tem,NULL);

		// If the Directory exists it will return a false
		if(tt)
			SetFileAttributes(tem,FILE_ATTRIBUTE_NORMAL);
		// If we were successful we set the attributes to normal
	}
	m_arr.RemoveAll();
	//  Now lets see if the directory was successfully created
	fFile = FindFirstFile(dd,&fileinfo);

	// if the file exists and it is a directory
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		//  Directory Exists close file and return
		FindClose(fFile);
		return TRUE;
	}
	else
	{
		FindClose(fFile);
		return FALSE;
	}
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPackageArchive::CPackageArchive()
{
	m_PackageFileptr = NULL;

	m_PackedFileCount = 0;

	m_PackefFileStartPosition = 0;

	m_PackageFilenameArray		= NULL;

	m_PackageFilePositionArray	= NULL;
}

CPackageArchive::~CPackageArchive()
{
	if( m_PackageFileptr ) fclose( m_PackageFileptr );

	if( m_PackageFilePositionArray )
	{
		delete[] m_PackageFilePositionArray;
		m_PackageFilePositionArray = NULL;
	}

	if( m_PackageFilenameArray )
	{
		delete[] m_PackageFilenameArray;
		m_PackageFilenameArray = NULL;
	}
}


BOOL CPackageArchive::Open( LPCTSTR filename )
{
	if( !filename[0] ) return FALSE;

	Close();

	this->m_PackageFileptr = fopen( filename, "rb" );

	if( !this->m_PackageFileptr )
	{
		AfxMessageBox( "ERROR : Package openning" );
		return FALSE;
	}

	_PACKAGEFILE_HEADER packagefileheader;

	if( fread( &packagefileheader, sizeof( _PACKAGEFILE_HEADER ), 1, m_PackageFileptr ) < 1 )
	{
		AfxMessageBox( "ERROR : Package header loading" );
		Close();
		return FALSE;
	}

	if( packagefileheader.HeaderIndicator != _HEADERINDICATOR_PACKAGEFILE )
	{
		AfxMessageBox( "ERROR : Package header loading" );
		Close();
		return FALSE;
	}

	if( packagefileheader.FileVersion != _HEADERINDICATOR_PACKAGEFILEVERSION )
	{
		AfxMessageBox( "ERROR : Package version" );
		Close();
		return FALSE;
	}

	this->m_PackedFileCount = packagefileheader.PackedFileCount;

	m_PackageFilePositionArray = new int[ this->m_PackedFileCount ];

	if( !m_PackageFilePositionArray )
	{
		AfxMessageBox( "ERROR : Allocate position array" );
		Close();
		return FALSE;
	}

	// Alloc.. & Read filename informations...
	m_PackageFilenameArray = new _XFileInfo[ this->m_PackedFileCount ];

	if( !m_PackageFilenameArray )
	{
		AfxMessageBox( "ERROR : Allocate resource descrpition array" );
		Close();
		return FALSE;
	}

	WORD length;
	char tempbuffer[_MAX_PATH];

	for( int i = 0; i < this->m_PackedFileCount; i++ )
	{
		length = 0;
		memset( tempbuffer, 0, _MAX_PATH );

		if( fread( &length, sizeof(WORD), 1, m_PackageFileptr ) < 1 )
		{
			AfxMessageBox( "ERROR : Read file descrpition array size" );
			Close();
			return FALSE;
		}

		if( fread( &tempbuffer , length, 1, m_PackageFileptr ) < 1 )
		{
			AfxMessageBox( "ERROR : Read file descrpition array" );
			Close();
			return FALSE;
		}

		m_PackageFilenameArray[i].Set( tempbuffer );
	}

	if( fread( m_PackageFilePositionArray, sizeof(int)*this->m_PackedFileCount, 1, m_PackageFileptr ) < 1 )
	{
		AfxMessageBox( "ERROR : Read position array" );
		Close();
		return FALSE;
	}

	m_PackefFileStartPosition =  sizeof( _PACKAGEFILE_HEADER ) + (sizeof(int)*this->m_PackedFileCount);	
			
	return TRUE;
}

BOOL CPackageArchive::Close( void )
{
	if( m_PackageFileptr )
	{
		fclose( m_PackageFileptr );
		m_PackageFileptr = NULL;
	}

	m_PackedFileCount = 0;

	m_PackefFileStartPosition = 0;	

	if( m_PackageFilenameArray )
	{
		delete[] m_PackageFilenameArray;
		m_PackageFilenameArray = NULL;
	}

	if( m_PackageFilePositionArray )
	{
		delete[] m_PackageFilePositionArray;
		m_PackageFilePositionArray = NULL;
	}

	return TRUE;
}


FILE* CPackageArchive::GetPackedFile( int file_id )
{
	if( !m_PackageFileptr )		
	{
		AfxMessageBox( "ERROR : Not ready package" );
		return NULL;
	}

	if( m_PackedFileCount <= file_id )
	{
		AfxMessageBox( "ERROR : Invalid Package ID" );
		return NULL;
	}
	
	if( fseek( this->m_PackageFileptr, m_PackageFilePositionArray[file_id], SEEK_SET ) == 0 )
	{
		return this->m_PackageFileptr;
	}
	else
	{
		AfxMessageBox( "ERROR : Package not found" );
		return NULL;
	}

	return NULL;
}

FILE* CPackageArchive::GetPackedFile( LPSTR file_name)
{
	ASSERT( file_name );
	if( !file_name ) return NULL;

	int resourceindex = FindResource( file_name );

	if( resourceindex == -1 )
	{
		char message[256];
		sprintf( message, "ERROR : Can't find resource [%s]", file_name );
		AfxMessageBox( message );
		return NULL;
	}

	return GetPackedFile( resourceindex );
}

int  CPackageArchive::FindResource( LPSTR resourcename )
{
	ASSERT( resourcename );
	if( !resourcename ) return -1;
	
	ASSERT( m_PackageFilenameArray );
	if( m_PackageFilenameArray ) return -1;

	for( int i = 0; i < m_PackedFileCount; i++ )
	{
		if( 0 == strcmp( resourcename, m_PackageFilenameArray->m_filename ) )
		{
			return i;
		}
	}

	return -1;
}


#include "imagehlp.h" 

BOOL CPackageArchive::ExportFile( int file_id, LPCTSTR filename )
{	
	if( !filename || !filename[0] ) return FALSE;

	FILE* pExportFile = NULL;
	FILE* pPackageFile = this->GetPackedFile( file_id );	
	if( !pPackageFile )	return FALSE;

	char FilePath[ _MAX_PATH+1 ];				
	memset( FilePath, 0, _MAX_PATH+1 );
	strncpy( FilePath, filename, _MAX_PATH );
		
	for( int i = strlen(FilePath)-1; i > 0; i-- )
	{
		if( FilePath[i] == '\\' ) break;
		else FilePath[i] = NULL;
	}

	if(FilePath[0]) MakeSureDirectoryPathExists( FilePath );
	
	
	if( CheckExistFile( filename, TRUE ) ) CFile::Remove( filename );

	
	pExportFile = fopen( filename, "wb" );

	if( !pExportFile )
	{
		AfxMessageBox( "ERROR : Export file open" );
		return FALSE;
	}
	
	long filesize = 0L;
	long currentfilepos = 0L;
	int  databuffer = 0;
		
	if( file_id < m_PackedFileCount-1 )
	{		
		filesize = m_PackageFilePositionArray[file_id+1] - m_PackageFilePositionArray[file_id];
	}	
	else // 팩키지의 맨끝 파일이면...
	{
		filesize = -1;		
	}


	do
	{
		if( feof( pPackageFile ) ) break;

		databuffer = fgetc( pPackageFile );

		if( databuffer == EOF ) break;

		if( EOF == fputc( databuffer , pExportFile ) )
		{
			AfxMessageBox( "ERROR : Export data" );
			break;
		}
		
		currentfilepos++;

		//한개 파일의 끝이면 종료..
		if( filesize != -1 )
		{
			if( filesize == currentfilepos ) break;
		}

	}while( !feof( pPackageFile ) );

	fclose( pExportFile );

	return TRUE;
}


BOOL CPackageArchive::Patch( LPCTSTR packagefilename, LPCTSTR patchinfofilename )
{
	if( !packagefilename[0] || !patchinfofilename[0] ) return FALSE;

	if( m_PackageFileptr ) this->Close();
	
	if( !this->Open( packagefilename ) )
	{		
		return FALSE;
	}

	FILE* pPatchinfoFile = NULL;

	pPatchinfoFile = fopen( patchinfofilename, "rt" );

	if( !pPatchinfoFile )
	{
		AfxMessageBox( "ERROR : Upgrade information file open" );
		this->Close();
		return FALSE;
	}

	char  parsingString[256];
	int   packageindexcount = 0;
	FILE* pCurrentpackagefile = NULL;

	char  currentPathString[_MAX_PATH];
	CString tFilenamestring;
	CString tMessagestring;

	::GetCurrentDirectory( _MAX_PATH, currentPathString );

	while( !feof( pPatchinfoFile ) )
	{
		if( feof( pPatchinfoFile ) ) break;

		memset( parsingString, 0, 256 );
		fgets( parsingString, 255, pPatchinfoFile );

		if( strlen(parsingString) && parsingString[0] != '\n' && parsingString[0] != ';' )
		{
			if( this->GetPackedFile( packageindexcount ) )
			{				
				tFilenamestring = currentPathString;
				
				tFilenamestring += parsingString;

				tFilenamestring.Remove( '\n' );				

				if( !this->ExportFile( packageindexcount, tFilenamestring ) ) break;

			}
			packageindexcount++;
		}
	}
	
	fclose( pPatchinfoFile );

	this->Close();

	return TRUE;
}


