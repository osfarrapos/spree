// Package.cpp: implementation of the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Package.h"
#include "resource.h"
#include "XLauncherDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef  _LOGFILE_
extern void _XLog( BOOL insertlistview, LPCTSTR msg, ... );
#endif

extern TCHAR g_Installedpath[_REGSTRING_MAXLENGTH+1];

void _XMessage( TCHAR* argv, ... )
{
	TCHAR szBuff[1024];
	int retValue;
	va_list argptr;
	
	va_start( argptr, argv );
	retValue = vswprintf( szBuff, argv, argptr );
	va_end( argptr );
	
	_XLog( FALSE, szBuff );

	AfxMessageBox( szBuff );	
}

void _EncodeData( LPSTR string, BYTE key )
{
	if( !string ) return;
	ASSERT( string );

	int length = strlen( string );

	LPSTR tptr = string;

	for( int i = 0; i < length; i++ )
	{
		*tptr ^= key;
		tptr++;
	}
}

#define OTHER_PATH_SEPARATOR _T('\\')
#define DEVICE_SEPARATOR _T(':')

LPCTSTR GetFileExtension(LPCTSTR filename)
{
 int pos, end;

 pos = end = wcslen(filename);

 while ((pos>0) && (filename[pos-1] != _T('.') ) && (filename[pos-1] != _T('/') )
	    && (filename[pos-1] != OTHER_PATH_SEPARATOR)
        && (filename[pos-1] != DEVICE_SEPARATOR))
  pos--;

 if (filename[pos-1] == _T('.'))
  return filename+pos;

 return filename+end;
}

BOOL CheckExistFile( LPCTSTR filename, BOOL resetAttrib )
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
		SetFileAttributes( filename, FILE_ATTRIBUTE_ARCHIVE );
		/*CFileStatus filestatus;
		CFile::GetStatus( filename, filestatus );
		filestatus.m_attribute = CFile::Attribute::normal; // normal file attrib setting
		CFile::SetStatus( filename, filestatus );*/
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
	CString tem = _T("");					// Temporary CString Object

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
		if( dd.GetAt(x1) != _T('\\') )		// if the Charachter is not a \ 
		{
			tem += dd.GetAt(x1);		// else add character to Temp String
		}
		else
		{
			m_arr.Add(tem);				// if the Character is a \ Add the Temp String to the CString Array
			tem += _T("\\");				// Now add the \ to the temp string
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

CPackage::CPackage()
{
	m_PackageFileptr = NULL;

	m_PackedFileCount = 0;

	m_PackefFileStartPosition = 0;

	m_PackageFilenameArray		= NULL;

	m_PackageFilePositionArray = NULL;
}

CPackage::~CPackage()
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


BOOL CPackage::Open( LPCTSTR filename, BOOL skipcreateindextable )
{
	if( !filename[0] ) return FALSE;

	Close();

#ifdef _XUSEXPIDXTABLE	

	CString indexfilename = filename;
	indexfilename += _T("N");

	if( !skipcreateindextable )
	{
		if( !CheckExistIndexFile( filename ) )
		{
			if( !SaveIndexTable( filename ) )
			{
				return FALSE;
			}
		}
	}

	FILE* pIDXFilePtr = NULL;

	pIDXFilePtr = _wfopen( indexfilename, _T("rb") );
	
	if( !pIDXFilePtr )
	{
		_XMessage( _T("ERROR : Package openning [%s]"), indexfilename );
		return FALSE;
	}
	
	_XLog( FALSE, _T("Package Message : Package opened [%s]\n"), indexfilename );
	
	_PACKAGEFILE_HEADER packagefileheader;
	
	if( fread( &packagefileheader, sizeof( _PACKAGEFILE_HEADER ), 1, pIDXFilePtr ) < 1 )
	{
		_XMessage( "ERROR : Package header loading [%s]", indexfilename );
		fclose( pIDXFilePtr );
		return FALSE;
	}
	
	if( packagefileheader.HeaderIndicator != _HEADERINDICATOR_PACKAGEFILE )
	{
		_XMessage( "ERROR : Package header check [%s]", indexfilename );
		fclose( pIDXFilePtr );
		return FALSE;
	}
	
	if( packagefileheader.FileVersion != _HEADERINDICATOR_PACKAGEFILEVERSION )
	{
		_XMessage( "ERROR : Package version [%s]", indexfilename );
		fclose( pIDXFilePtr );
		return FALSE;
	}
	
	this->m_PackedFileCount = packagefileheader.PackedFileCount;
	
	m_PackageFilePositionArray = new int[ this->m_PackedFileCount ];
	
	if( !m_PackageFilePositionArray )
	{
		_XMessage( "ERROR : Allocate position array [%s]", indexfilename );
		fclose( pIDXFilePtr );
		return FALSE;
	}
	
	// Alloc.. & Read filename informations...
	m_PackageFilenameArray = new _XFileInfo[ this->m_PackedFileCount ];
	
	if( !m_PackageFilenameArray )
	{
		_XMessage( "ERROR : Allocate resource description array [%s]", indexfilename );
		fclose( pIDXFilePtr );
		return FALSE;
	}
	
	WORD length;
	char tempbuffer[512];
	TCHAR widestring[1024];

	for( int i = 0; i < this->m_PackedFileCount; i++ )
	{
		length = 0;
		memset( tempbuffer, 0, 512 );
		
		if( fread( &length, sizeof(WORD), 1, pIDXFilePtr ) < 1 )
		{
			_XMessage( "ERROR : Read file description array size [%s]", indexfilename );
			fclose( pIDXFilePtr );
			return FALSE;
		}
		
		if( length > 0  )
		{
			if( fread( &tempbuffer , length, 1, pIDXFilePtr ) < 1 )
			{
				_XMessage( "ERROR : Read file description array [%s]", indexfilename );
				fclose( pIDXFilePtr );
				return FALSE;
			}
		}
		
		_EncodeData( tempbuffer, _ENCODEKEY );
				
		memset( widestring, 0, sizeof(TCHAR) * 1024 );
		int requirelength = MultiByteToWideChar(CP_ACP, 0, tempbuffer, strlen(tempbuffer), widestring, 1024);
		
		m_PackageFilenameArray[i].Set( widestring );
	}
	
	if( this->m_PackedFileCount > 0 )
	{
		if( fread( m_PackageFilePositionArray, sizeof(int)*this->m_PackedFileCount, 1, pIDXFilePtr ) < 1 )
		{
			_XMessage( "ERROR : Read position array [%s]", indexfilename );
			fclose( pIDXFilePtr );
			return FALSE;
		}
	}
	
	m_PackefFileStartPosition =  sizeof( _PACKAGEFILE_HEADER ) + (sizeof(int)*this->m_PackedFileCount);	

	this->m_PackageFileptr = _wfopen( filename, _T("rb") );
	
	if( !this->m_PackageFileptr )
	{
		_XMessage( "ERROR : Package openning [%s]", filename );
		return FALSE;
	}

#else

	this->m_PackageFileptr = _wfopen( filename, _T("rb") );

	if( !this->m_PackageFileptr )
	{
		_XMessage( _T("ERROR : Package openning [%s]"), filename );
		return FALSE;
	}

	_XLog( FALSE, _T("Package Message : Package opened [%s]\n"), filename );

	_PACKAGEFILE_HEADER packagefileheader;

	if( fread( &packagefileheader, sizeof( _PACKAGEFILE_HEADER ), 1, m_PackageFileptr ) < 1 )
	{
		_XMessage( _T("ERROR : Package header loading [%s]"), filename );
		Close();
		return FALSE;
	}
	
	if( packagefileheader.HeaderIndicator != _HEADERINDICATOR_PACKAGEFILE )
	{
		_XMessage( _T("ERROR : Package header check [%s]"), filename );
		Close();
		return FALSE;
	}

	if( packagefileheader.FileVersion != _HEADERINDICATOR_PACKAGEFILEVERSION )
	{
		_XMessage( _T("ERROR : Package version [%s]"), filename );
		Close();
		return FALSE;
	}

	this->m_PackedFileCount = packagefileheader.PackedFileCount;

	m_PackageFilePositionArray = new int[ this->m_PackedFileCount ];

	if( !m_PackageFilePositionArray )
	{
		_XMessage( _T("ERROR : Allocate position array [%s]"), filename );
		Close();
		return FALSE;
	}

	// Alloc.. & Read filename informations...
	m_PackageFilenameArray = new _XFileInfo[ this->m_PackedFileCount ];

	if( !m_PackageFilenameArray )
	{
		_XMessage( _T("ERROR : Allocate resource description array [%s]"), filename );
		Close();
		return FALSE;
	}

	WORD length;
	char tempbuffer[512];
	TCHAR widestring[1024];

	for( int i = 0; i < this->m_PackedFileCount; i++ )
	{
		length = 0;
		memset( tempbuffer, 0, 512 );

		if( fread( &length, sizeof(WORD), 1, m_PackageFileptr ) < 1 )
		{
			_XMessage( _T("ERROR : Read file description array size [%s]"), filename );
			Close();
			return FALSE;
		}

		if( length > 0 )
		{		
			if( fread( &tempbuffer , length, 1, m_PackageFileptr ) < 1 )
			{
				_XMessage( _T("ERROR : Read file description array [%s]"), filename );
				Close();
				return FALSE;
			}
		}

		_EncodeData( tempbuffer, _ENCODEKEY );

		memset( widestring, 0, sizeof(TCHAR) * 1024 );
		int requirelength = MultiByteToWideChar(GetACP(), 0, tempbuffer, strlen(tempbuffer), widestring, 1024);
		
		m_PackageFilenameArray[i].Set( widestring );
	}

	if( this->m_PackedFileCount > 0 )
	{
		if( fread( m_PackageFilePositionArray, sizeof(int)*this->m_PackedFileCount, 1, m_PackageFileptr ) < 1 )
		{
			_XMessage( _T("ERROR : Read position array [%s]"), filename );
			Close();
			return FALSE;
		}
	}

	m_PackefFileStartPosition =  sizeof( _PACKAGEFILE_HEADER ) + (sizeof(int)*this->m_PackedFileCount);	
#endif
	
	return TRUE;
}

BOOL CPackage::Close( void )
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


FILE* CPackage::GetPackedFile( int file_id )
{
	if( !m_PackageFileptr )		
	{
		_XMessage( _T("ERROR : Not ready package") );
		return NULL;
	}

	if( m_PackedFileCount <= file_id )
	{
		_XMessage( _T("ERROR : Invalid Package ID") );
		return NULL;
	}
	
	if( fseek( this->m_PackageFileptr, m_PackageFilePositionArray[file_id], SEEK_SET ) == 0 )
	{
		return this->m_PackageFileptr;
	}
	else
	{
		_XMessage( _T("ERROR : Package not found") );
		return NULL;
	}

	return NULL;
}

FILE* CPackage::GetPackedFile( LPCTSTR file_name )
{
	ASSERT( file_name );
	if( !file_name ) return NULL;

	if( !m_PackageFileptr )		
	{
		_XMessage( _T("ERROR : Not ready package") );
		return NULL;
	}

	int resourceindex = FindResource( file_name );

	if( resourceindex == -1 )
	{
		_XMessage( _T("ERROR : Can't find resource [%s]"), file_name );
		return NULL;
	}

	return GetPackedFile( resourceindex );
}

int  CPackage::FindResource( LPCTSTR resourcename )
{
	ASSERT( resourcename );
	if( !resourcename ) return -1;
	
	ASSERT( m_PackageFilenameArray );
	if( !m_PackageFilenameArray ) return -1;

	for( int i = 0; i < m_PackedFileCount; i++ )
	{
		if( 0 == _wcsicmp( resourcename, m_PackageFilenameArray[i].m_filename ) )
		{
			return i;
		}
	}

	return -1;
}

LPCTSTR CPackage::GetResourceName( int file_id )
{
	if( !m_PackageFileptr )		
	{
		_XMessage( _T("ERROR : Not ready package") );
		return NULL;
	}

	if( m_PackedFileCount <= file_id )
	{
		_XMessage( _T("ERROR : Invalid Package ID") );
		return NULL;
	}
	
	return m_PackageFilenameArray[ file_id ].m_filename;
}

int	CPackage::GetResourceCount( void )
{
	if( !m_PackageFileptr ) return 0;

	return m_PackedFileCount;
}


#include "imagehlp.h" 
#define _XDEF_MEMORYALLOCATION_COPY

BOOL CPackage::ExportFile( int file_id, LPCTSTR filename )
{	
	if( !filename || !filename[0] ) return FALSE;

	_XLog( FALSE, _T("Package Message : Export file [%d:%s]\n"), file_id, filename );

	FILE* pExportFile = NULL;
	FILE* pPackageFile = this->GetPackedFile( file_id );
	if( !pPackageFile )	return FALSE;

	TCHAR FilePath[ 1024 ];
	memset( FilePath, 0, sizeof(TCHAR) * 1024 );
	wcscpy( FilePath, filename );
		
	for( int i = wcslen(FilePath)-1; i > 0; i-- )
	{
		if( FilePath[i] == _T('\\') ) break;
		else FilePath[i] = NULL;
	}

	_XLog( FALSE, _T("Package Message : Check folder and create [%d:%s]\n"), file_id, filename );
	
	char mbcsstring[1024];		
	memset( mbcsstring, 0, 1024 );
	WideCharToMultiByte(GetACP(), 0, FilePath, -1, mbcsstring, 1024, NULL, NULL);
	if(FilePath[0]) MakeSureDirectoryPathExists( mbcsstring );
	
	CString newfilename = filename;
	newfilename += _T("_new");
	
	pExportFile = _wfopen( newfilename.GetBuffer(512), _T("wb") );

	if( !pExportFile )
	{
		_XMessage( _T("ERROR : Export file open [%s]"), newfilename );
		return FALSE;
	}
	
	long filesize = 0L;
	long currentfilepos = 0L;
	int  databuffer = 0;
		
	if( file_id < m_PackedFileCount-1 )
	{		
		filesize = m_PackageFilePositionArray[file_id+1] - m_PackageFilePositionArray[file_id];
	}	
	else // Last file in package
	{
		filesize = -1;		
	}

	//_XLog( FALSE, _T("Package Message : Exporting [%d:%s]\n"), file_id, filename );

#ifndef _XDEF_MEMORYALLOCATION_COPY
	do
	{
		if( feof( pPackageFile ) ) break;

		databuffer = fgetc( pPackageFile );

		if( databuffer == EOF ) break;

		if( EOF == fputc( databuffer , pExportFile ) )
		{
			_XMessage( _T("ERROR : Exporting packed data [%s]"), newfilename );
			if( CheckExistFile( filename, TRUE ) ) ::DeleteFile( newfilename );
			fclose( pExportFile );
			break;
		}
		
		currentfilepos++;

		// end of file
		if( filesize != -1 )
		{
			if( filesize == currentfilepos ) break;
		}

	}while( !feof( pPackageFile ) );
#else 
	
	if( filesize == -1 )
	{
		fseek( pPackageFile, 0L, SEEK_END );
		LONG endptr = ftell( pPackageFile );

		filesize = endptr - m_PackageFilePositionArray[file_id];

		pPackageFile = this->GetPackedFile( file_id );
	}

	LPBYTE pBuffer = NULL;
	
	pBuffer = new BYTE[ filesize ];

	if( !pBuffer )
	{
		_XMessage( _T("ERROR : Allocation copy buffer [%s]"), newfilename );
		fclose( pExportFile );
		if( CheckExistFile( filename, TRUE ) ) ::DeleteFile( newfilename );		
		return FALSE;
	}

	if( fread( pBuffer, sizeof( BYTE ) * filesize, 1, pPackageFile ) < 1 )
	{
		_XMessage( _T("ERROR : Read exporting data [%s]"), newfilename );
		fclose( pExportFile );
		if( CheckExistFile( filename, TRUE ) ) ::DeleteFile( newfilename );		
		if( pBuffer ) delete[] pBuffer;
		return FALSE;
	}

	if( fwrite( pBuffer, sizeof( BYTE ) * filesize, 1, pExportFile ) < 1 )
	{
		_XMessage( _T("ERROR : Write exporting data [%s]"), newfilename );
		fclose( pExportFile );
		if( CheckExistFile( filename, TRUE ) ) ::DeleteFile( newfilename );
		if( pBuffer ) delete[] pBuffer;
		return FALSE;
	}
	
	if( pBuffer ) delete[] pBuffer;

#endif

	fclose( pExportFile );

	_XLog( FALSE, _T("Package Message : Check old file [%d:%s]\n"), file_id, filename );

	if( CheckExistFile( filename, TRUE ) )
	{	
		::DeleteFile( filename );
	}

	_XLog( FALSE, _T("Package Message : Rename new file [%s->%s]\n"), newfilename, filename );
	CFile::Rename( newfilename, filename );

	return TRUE;
}

BOOL CPackage::ExportFile( int file_id, FILE* pExportFile )
{
	if( !pExportFile ) return FALSE;
	
	FILE* pPackageFile = this->GetPackedFile( file_id );
	if( !pPackageFile )	return FALSE;
	
	//_XLog( FALSE, _T("Package Message : Search Export file [%d]\n"), file_id );

	long filesize = 0L;
	long currentfilepos = 0L;
	int  databuffer = 0;
		
	if( file_id < m_PackedFileCount-1 )
	{		
		filesize = m_PackageFilePositionArray[file_id+1] - m_PackageFilePositionArray[file_id];
	}	
	else // if last file in package
	{
		filesize = -1;
	}

	//_XLog( FALSE, _T("Package Message : Exporting [%d]\n"), file_id );
	
#ifndef _XDEF_MEMORYALLOCATION_COPY
	do
	{
		if( feof( pPackageFile ) ) break;

		databuffer = fgetc( pPackageFile );

		if( databuffer == EOF ) break;

		if( EOF == fputc( databuffer , pExportFile ) )
		{
			_XMessage( "ERROR : Exporting packed data" );
			break;
		}
		
		currentfilepos++;

		// if end of file		
		if( filesize != -1 )
		{
			if( filesize == currentfilepos ) break;
		}
		
	}while( !feof( pPackageFile ) );

#else

	if( filesize == -1 )
	{
		fseek( pPackageFile, 0L, SEEK_END );
		LONG endptr = ftell( pPackageFile );

		filesize = endptr - m_PackageFilePositionArray[file_id];

		pPackageFile = this->GetPackedFile( file_id );
	}

	LPBYTE pBuffer = NULL;
	
	pBuffer = new BYTE[ filesize ];

	if( !pBuffer )
	{
		_XMessage( _T("ERROR : Allocation copy buffer") );
		return FALSE;
	}

	//_XLog( FALSE, _T("Package Message : Read org file [%d]\n"), file_id );

	if( fread( pBuffer, sizeof( BYTE ) * filesize, 1, pPackageFile ) < 1 )
	{
		_XMessage( _T("ERROR : Read exporting data") );
		return FALSE;
	}

	//_XLog( FALSE, _T("Package Message : write new file [%d]\n"), file_id );

	if( fwrite( pBuffer, sizeof( BYTE ) * filesize, 1, pExportFile ) < 1 )
	{
		_XMessage( _T("ERROR : Write exporting data") );
		return FALSE;
	}
	
	if( pBuffer ) delete[] pBuffer;

#endif
	
	return TRUE;
}


BOOL CPackage::Patch( LPCTSTR packagefilename, LPCTSTR patchinfofilename, CXLauncherDlg* pMainWindow )
{
	if( !packagefilename[0] || !patchinfofilename[0] ) return FALSE;

	if( m_PackageFileptr ) this->Close();
	
	if( !this->Open( packagefilename ) )
	{		
		return FALSE;
	}

	FILE* pPatchinfoFile = NULL;

	pPatchinfoFile = _wfopen( patchinfofilename, _T("rt") );

	if( !pPatchinfoFile )
	{
		AfxMessageBox( _T("ERROR : patch information file open") );
		this->Close();
		return FALSE;
	}

	_XLog( FALSE, _T("Package Message : Open patch information file [%s:%s]\n"), packagefilename, patchinfofilename );

	char  parsingString[256];	
	FILE* pCurrentpackagefile = NULL;
	TCHAR szPatchFileString[512][256];
	int	  patchfilecount = 0;

	memset( szPatchFileString, 0, sizeof(TCHAR) * 512 * 256 );
		
	CString currentPathString = g_Installedpath;
	CString tFilenamestring;
	CString tMessagestring;
	TCHAR widestring[1024];
	
	while( !feof( pPatchinfoFile ) )
	{
		if( feof( pPatchinfoFile ) ) break;

		memset( parsingString, 0, 256 );
		fgets( parsingString, 255, pPatchinfoFile );

		if( strlen(parsingString) && parsingString[0] != '\n' && parsingString[0] != ';' )
		{
			memset( widestring, 0, sizeof(TCHAR) * 1024 );
			int requirelength = MultiByteToWideChar(CP_ACP, 0, parsingString, strlen(parsingString), widestring, 1024);
			
			wcscpy( szPatchFileString[patchfilecount], widestring );
			patchfilecount++;
		}
	}	
	fclose( pPatchinfoFile );

	int		orgtotalprogress = 0;
	FLOAT	totalprogress = 0.0f;
	FLOAT   increasefactor = 0.0f;

	if( pMainWindow ) 
	{
		orgtotalprogress = pMainWindow->m_TotalUpdateProgress.GetPos();
		increasefactor   = (pMainWindow->m_OnePatchIncreaseFactor/2.0f) / (FLOAT)patchfilecount;
	}

	for( int packageindexcount = 0; packageindexcount < patchfilecount; packageindexcount++ )
	{
		if( this->GetPackedFile( packageindexcount ) )
		{				
			tFilenamestring = currentPathString;
			
			tFilenamestring += szPatchFileString[packageindexcount];
			
			tFilenamestring.Remove( _T('\n') );
			
			tMessagestring.Format( _T("Update : %s\n"), szPatchFileString[packageindexcount] );
			if( pMainWindow ) 
				pMainWindow->InsertStringToProgressMessageListCtrl(tMessagestring);
			_XLog( TRUE, tMessagestring.GetBuffer(256) );
			
			if( _wcsicmp( GetFileExtension(tFilenamestring), _T("XP") ) == 0 )
			{
				// Patch for packed file
				if( CheckExistFile(tFilenamestring, TRUE) )
				{
					CString packedfile = tFilenamestring;
					packedfile += _T("_Merge");
					if( !this->ExportFile( packageindexcount, packedfile ) )
					{
						tMessagestring.Format( _T("Export failed. [%s_Merge]\n"), szPatchFileString[packageindexcount] );
						if( pMainWindow ) 
							pMainWindow->InsertStringToProgressMessageListCtrl(tMessagestring);
						_XLog( TRUE, tMessagestring.GetBuffer(256) );
						return FALSE;
					}
					
					if( !this->PatchXPToXP( tFilenamestring ) )
					{
						tMessagestring.Format( _T("XP vs XP merge failed. [%s]\n"), szPatchFileString[packageindexcount] );
						if( pMainWindow ) 
							pMainWindow->InsertStringToProgressMessageListCtrl(tMessagestring);
						_XLog( TRUE,  tMessagestring.GetBuffer(256) );
						return FALSE;
					}
				}
				else
				{
					if( !this->ExportFile( packageindexcount, tFilenamestring ) )
					{
						tMessagestring.Format( _T("Export failed. [%s]\n"), szPatchFileString[packageindexcount] );
						if( pMainWindow ) 
							pMainWindow->InsertStringToProgressMessageListCtrl(tMessagestring);
						_XLog(  TRUE, tMessagestring.GetBuffer(256) );
						return FALSE;
					}
				}
			}
			else if( !this->ExportFile( packageindexcount, tFilenamestring ) )
			{
				tMessagestring.Format( _T("Export failed. [%s]\n"), szPatchFileString[packageindexcount] );
				if( pMainWindow ) 
					pMainWindow->InsertStringToProgressMessageListCtrl(tMessagestring);
				_XLog(  TRUE, tMessagestring.GetBuffer(256) );
				return FALSE;
			}
			
			
			tMessagestring.Format( _T("Update complete.[%s]\n"), szPatchFileString[packageindexcount] );
			if( pMainWindow ) 
				pMainWindow->InsertStringToProgressMessageListCtrl(tMessagestring);
			_XLog(  TRUE, tMessagestring.GetBuffer(256) );
		}

		if( pMainWindow ) 
		{
			pMainWindow->m_UpdateProgress.SetPos( 30 + (int)(((FLOAT)packageindexcount / (FLOAT)patchfilecount) * 65.0f) );

			totalprogress += increasefactor;
			pMainWindow->m_TotalUpdateProgress.SetPos( orgtotalprogress + (int)totalprogress );
		}
	}

	this->Close();

	return TRUE;
}

/*
BOOL CPackage::Patch( LPCTSTR packagefilename, LPCTSTR patchinfofilename )
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
		AfxMessageBox( "ERROR : Patch information file open" );
		this->Close();
		return FALSE;
	}

	_XLog( FALSE, "Package Message : Open patch information file [%s:%s]\n", packagefilename, patchinfofilename );

	char  parsingString[256];
	int   packageindexcount = 0;
	FILE* pCurrentpackagefile = NULL;

	CString currentPathString = g_Installedpath;
	CString tFilenamestring;
	CString tMessagestring;

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

				tMessagestring.Format( "Patch : %s\n", tFilenamestring );
				//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
				_XLog( TRUE, tMessagestring.GetBuffer(256) );
				
				if( strcmpi( GetFileExtension(tFilenamestring), "XP" ) == 0 )
				{
					// 패킹된 파일의 패치 
					if( CheckExistFile(tFilenamestring, TRUE) )
					{
						CString packedfile = tFilenamestring;
						packedfile += _T("_Merge");
						if( !this->ExportFile( packageindexcount, packedfile ) )
						{
							tMessagestring.Format( "Export failed. [%s]\n", packedfile );
							//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
							_XLog( TRUE, tMessagestring.GetBuffer(256) );
							break;
						}

						if( !this->PatchXPToXP( tFilenamestring ) )
						{
							tMessagestring.Format( "XP vs XP merge failed. [%s]\n", tFilenamestring );
							//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
							_XLog( TRUE,  tMessagestring.GetBuffer(256) );
							break;
						}
					}
					else
					{
						if( !this->ExportFile( packageindexcount, tFilenamestring ) )
						{
							tMessagestring.Format( "Export failed. [%s]\n", tFilenamestring );
							//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
							_XLog(  TRUE, tMessagestring.GetBuffer(256) );
							break;
						}
					}
				}
				else if( !this->ExportFile( packageindexcount, tFilenamestring ) )
				{
					tMessagestring.Format( "Export failed. [%s]\n", tFilenamestring );
					//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
					_XLog(  TRUE, tMessagestring.GetBuffer(256) );
					break;
				}


				tMessagestring.Format( "Patch complete.[%s]\n", tFilenamestring );
				//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
				_XLog(  TRUE, tMessagestring.GetBuffer(256) );
			}
			packageindexcount++;
		}
	}
	
	fclose( pPatchinfoFile );

	this->Close();

	return TRUE;
}
 

*/

BOOL CPackage::PatchXPToXP( LPCTSTR packagefilename )
{
	CPackage	OrgPackage;
	CPackage	MergePackage;

	CString tMessagestring;
	tMessagestring.Format( _T("Modifing package file.\n") );
	//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
	_XLog(  TRUE, tMessagestring.GetBuffer(256) );
	tMessagestring.Format( _T("    %s\n"), packagefilename );
	//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
	_XLog(  TRUE, tMessagestring.GetBuffer(256) );
	
	CString newpackagename = packagefilename;
	newpackagename += _T("_New");	
	//CFile::Rename( packagefilename, orgname );

	CString mergename = packagefilename;
	mergename += _T("_Merge");

	//if( CheckExistFile(newpackagename,   TRUE) ) CFile::Remove( newpackagename );
	//if( CheckExistFile(mergename, TRUE) ) CFile::Remove( mergename );
	
	if( !OrgPackage.Open( packagefilename ) )
	{
		return FALSE;
	}

	if( !MergePackage.Open( mergename ) )
	{
		OrgPackage.Close();
		return FALSE;
	}

	tMessagestring.Format( _T("Calculate package informations.\n") );
	//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
	_XLog(  TRUE, tMessagestring.GetBuffer(256) );

	///////////////////////////////////////////////////////////////////////////////////
	// calculate new insert file count
	int MergedCount = 0;
	int MaxCount = 0;
	
	for( int i = 0; i < MergePackage.GetResourceCount(); i++ )
	{
		BOOL find = FALSE;
		for( int j = 0; j < OrgPackage.GetResourceCount(); j++ )
		{
			if( _wcsicmp( OrgPackage.m_PackageFilenameArray[j].m_filename,
						  MergePackage.m_PackageFilenameArray[i].m_filename ) == 0 )
			{
				find = TRUE;		
				break;
			}
		}	

		if( !find ) MergedCount++;
	}

	MaxCount = OrgPackage.GetResourceCount() + MergedCount;

	///////////////////////////////////////////////////////////////////////////////////
	// Create new package and merge process

	FILE* newpackagefile;
	_XFileInfo* fileinfo = NULL;
	int* PackageFilePositionArray = NULL;
	_PACKAGEFILE_HEADER	packageheader;

	packageheader.HeaderIndicator	=	_HEADERINDICATOR_PACKAGEFILE;
	packageheader.FileVersion		=	_HEADERINDICATOR_PACKAGEFILEVERSION;
	packageheader.PackedFileCount	=	MaxCount;

	newpackagefile	= _wfopen( newpackagename, _T("wb") );

	if( !newpackagefile )
	{
		AfxMessageBox( _T("ERROR : New package file open") );
		MergePackage.Close();
		OrgPackage.Close();
		return FALSE;
	}

	PackageFilePositionArray = new int[ MaxCount ];
	fileinfo = new _XFileInfo[ MaxCount ];
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// file name list ...
	for( i = 0; i < OrgPackage.GetResourceCount(); i++ )
	{
		// 기존 파일 리스트 유지
		fileinfo[i].Set( OrgPackage.m_PackageFilenameArray[i].m_filename );
	}
	
	// merge new file informations[file name]...
	int newfilecount = 0;
	for( i = 0; i < MergePackage.GetResourceCount(); i++ )
	{		
		BOOL find = FALSE;
		for( int j = 0; j < OrgPackage.GetResourceCount(); j++ )
		{
			if( _wcsicmp( OrgPackage.m_PackageFilenameArray[j].m_filename,
						  MergePackage.m_PackageFilenameArray[i].m_filename ) == 0 )
			{
				find = TRUE;
				break;
			}
		}		
		
		if( !find ) // 기존 리스트에 없는 새로운 파일 정보를 추가
		{			
			fileinfo[OrgPackage.GetResourceCount()+newfilecount].Set( MergePackage.m_PackageFilenameArray[i].m_filename );
								
			newfilecount++;
		}
	}

	int FileinfoSize = sizeof(_PACKAGEFILE_HEADER) + (MaxCount * sizeof(int));
	LONG fileposition = 0L;
	
	for( i=0; i < MaxCount; i++ )
	{
		FileinfoSize += sizeof( WORD );
		FileinfoSize += fileinfo[i].m_length;
	}

	// Accumulate file position...
	for( i = 0; i < OrgPackage.GetResourceCount(); i++ )
	{
		BOOL find = FALSE;
		for( int j = 0; j < MergePackage.GetResourceCount(); j++ )
		{
			if( _wcsicmp( OrgPackage.m_PackageFilenameArray[i].m_filename,
						 MergePackage.m_PackageFilenameArray[j].m_filename ) == 0 )
			{
				find = TRUE;		
				break;
			}
		}	

		PackageFilePositionArray[i]	= FileinfoSize + fileposition;
						
		if( find )
		{		
			// 업데이트될 파일의 파일 포인터 재계산...
			if( j < MergePackage.GetResourceCount()-1 )
			{			
				fileposition += MergePackage.m_PackageFilePositionArray[j+1] - MergePackage.m_PackageFilePositionArray[j];
			}
			else
			{
				fseek( MergePackage.m_PackageFileptr, 0L, SEEK_END );
				LONG endptr = ftell( MergePackage.m_PackageFileptr );

				fileposition += endptr - MergePackage.m_PackageFilePositionArray[j];
			}
		}
		else
		{				
			if( i < OrgPackage.GetResourceCount()-1 )
			{			
				fileposition += OrgPackage.m_PackageFilePositionArray[i+1] - OrgPackage.m_PackageFilePositionArray[i];
			}
			else
			{
				fseek( OrgPackage.m_PackageFileptr, 0L, SEEK_END );
				LONG endptr = ftell( OrgPackage.m_PackageFileptr );

				fileposition += endptr - OrgPackage.m_PackageFilePositionArray[i];
			}
		}		
	}

	// New file position...
	int addpos = 0;
	for( int j = 0; j < MergePackage.GetResourceCount(); j++ )
	{
		BOOL find = FALSE;
		for( i = 0; i < OrgPackage.GetResourceCount(); i++ )
		{
			if( _wcsicmp( OrgPackage.m_PackageFilenameArray[i].m_filename,
						  MergePackage.m_PackageFilenameArray[j].m_filename ) == 0 )
			{
				find = TRUE;		
				break;
			}
		}

		if( !find )
		{
			PackageFilePositionArray[ OrgPackage.GetResourceCount() + addpos ]	= FileinfoSize + fileposition;

			if( j < MergePackage.GetResourceCount()-1 )
			{			
				fileposition += MergePackage.m_PackageFilePositionArray[j+1] - MergePackage.m_PackageFilePositionArray[j];
			}
			else
			{
				fseek( MergePackage.m_PackageFileptr, 0L, SEEK_END );
				LONG endptr = ftell( MergePackage.m_PackageFileptr );

				fileposition += endptr - MergePackage.m_PackageFilePositionArray[j];
			}

			addpos++;
		}
	}

	fwrite( &packageheader, sizeof(_PACKAGEFILE_HEADER), 1, newpackagefile );

	// Save file informations....
	tMessagestring = _T("Rebuild package\n");
	//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
	_XLog(  TRUE, tMessagestring.GetBuffer(256) );
			
	for( i = 0; i < MaxCount; i++ )
	{					
		if( fwrite( &fileinfo[i].m_length, sizeof(WORD), 1, newpackagefile ) < 1 )
		{
			_XMessage( _T("ERROR : Save file description array size [%s]"), packagefilename );			
			fclose( newpackagefile );
			delete[] PackageFilePositionArray;
			delete[] fileinfo;
			MergePackage.Close();
			OrgPackage.Close();
			return FALSE;
		}
		
		char mbcsstring[1024];		
		memset( mbcsstring, 0, 1024 );
		WideCharToMultiByte(GetACP(), 0, fileinfo[i].m_filename, -1, mbcsstring, 1024, NULL, NULL);

		_EncodeData( mbcsstring, _ENCODEKEY );

		if( fileinfo[i].m_length > 0 )
		{
			if( fwrite( mbcsstring , fileinfo[i].m_length, 1, newpackagefile ) < 1 )
			{
				_XMessage( _T("ERROR : Save file description array [%s]"), packagefilename );			
				fclose( newpackagefile );
				delete[] PackageFilePositionArray;
				delete[] fileinfo;
				MergePackage.Close();
				OrgPackage.Close();
				return FALSE;
			}
		}
		//_EncodeData( fileinfo[i].m_filename, _ENCODEKEY );
	}

	if( fwrite( PackageFilePositionArray, sizeof(int)*MaxCount, 1, newpackagefile ) < 1 )
	{
		_XMessage( _T("ERROR : Read position array [%s]"), packagefilename );
		fclose( newpackagefile );
		delete[] PackageFilePositionArray;
		delete[] fileinfo;
		MergePackage.Close();
		OrgPackage.Close();
		return FALSE;		
	}	


	// Rebuild package...

	for( i = 0; i < OrgPackage.GetResourceCount(); i++ )
	{
		BOOL find = FALSE;
		for( int j = 0; j < MergePackage.GetResourceCount(); j++ )
		{
			if( _wcsicmp( OrgPackage.m_PackageFilenameArray[i].m_filename,
						  MergePackage.m_PackageFilenameArray[j].m_filename ) == 0 )
			{
				find = TRUE;
				break;
			}
		}	

		if( find ) // 수정된 파일을 패킹 
		{
			if( !MergePackage.ExportFile( MergePackage.FindResource( OrgPackage.m_PackageFilenameArray[i].m_filename ), newpackagefile ) )
			{
				fclose( newpackagefile );
				delete[] PackageFilePositionArray;
				delete[] fileinfo;
				MergePackage.Close();
				OrgPackage.Close();				
				return FALSE;
			}
		}
		else // 기존 파일 보존 
		{
			if( !OrgPackage.ExportFile( i, newpackagefile ) )
			{
				fclose( newpackagefile );
				delete[] PackageFilePositionArray;
				delete[] fileinfo;
				MergePackage.Close();
				OrgPackage.Close();				
				return FALSE;
			}
		}			
	}

	// 추가된 파일을 패킹 
	for( i = 0; i < MergePackage.GetResourceCount(); i++ )
	{
		BOOL find = FALSE;
		for( int j = 0; j < OrgPackage.GetResourceCount(); j++ )
		{
			if( _wcsicmp( OrgPackage.m_PackageFilenameArray[j].m_filename,
						 MergePackage.m_PackageFilenameArray[i].m_filename ) == 0 )
			{
				find = TRUE;
				break;
			}
		}	

		if( !find ) // 새로운 파일을 패킹 
		{
			if( !MergePackage.ExportFile( MergePackage.FindResource( MergePackage.m_PackageFilenameArray[i].m_filename ), newpackagefile ) )
			{
				fclose( newpackagefile );
				delete[] PackageFilePositionArray;
				delete[] fileinfo;
				MergePackage.Close();
				OrgPackage.Close();
				return FALSE;
			}
		}		
	}
			
	fclose( newpackagefile );
	delete[] PackageFilePositionArray;
	delete[] fileinfo;
	MergePackage.Close();
	OrgPackage.Close();

	tMessagestring.Format( _T("Delete temporary patch files\n") );
	//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
	_XLog(  TRUE, tMessagestring.GetBuffer(256) );

	if( CheckExistFile(packagefilename,   TRUE) ) ::DeleteFile( packagefilename );
	CFile::Rename( newpackagename, packagefilename );
	if( CheckExistFile(mergename, TRUE) ) ::DeleteFile( mergename );

//#define _TEST_EXPORT_
#ifdef _TEST_EXPORT_
	CPackage testpac;
	testpac.Open( packagefilename );

	for( i = 0; i < testpac.GetResourceCount(); i++ )
	{
		testpac.ExportFile( i, testpac.GetResourceName(i) );
	}

	testpac.Close();
#endif

	tMessagestring.Format( _T("Complete rebuild\n") );
	//((CXLauncherDlg*)AfxGetMainWnd())->InsertStringToProgressMessageListCtrl(tMessagestring);
	_XLog(  TRUE, tMessagestring.GetBuffer(256) );

	return TRUE;
}

BOOL CPackage::CheckExistIndexFile( LPCTSTR filename )
{
	CString indexfilename = filename;
	indexfilename+=_T("N");
	
	if( CheckExistFile( indexfilename, FALSE ) )
	{
		return TRUE;
	}	
	return FALSE;
}

BOOL CPackage::SaveIndexTable( LPCTSTR filename )
{
	CString indexfilename = filename;
	indexfilename += _T("N");
	
	if( !Open( filename, TRUE ) )
	{
		_XMessage( _T("ERROR : Org pack file open [%s]"), filename );
		return FALSE;
	}

	FILE* pFilePtr = _wfopen( indexfilename, _T("wb") );
	
	if( !pFilePtr )
	{
		_XMessage( _T("ERROR : IDXTBL File open [%s]"), indexfilename );
		return FALSE;
	}
	
	_PACKAGEFILE_HEADER packagefileheader;
	
	packagefileheader.HeaderIndicator = _HEADERINDICATOR_PACKAGEFILE;
	packagefileheader.FileVersion = _HEADERINDICATOR_PACKAGEFILEVERSION;
	packagefileheader.PackedFileCount = this->m_PackedFileCount;

	if( fwrite( &packagefileheader, sizeof( _PACKAGEFILE_HEADER ), 1, pFilePtr ) < 1 )
	{
		_XMessage( _T("ERROR : IDXTBL Package header write [%s]"), indexfilename );
		fclose( pFilePtr );
		return FALSE;
	}
	
	WORD length;
	char tempbuffer[512];
	char mbcsstring[1024];		
		
	for( int i = 0; i < this->m_PackedFileCount; i++ )
	{
		length = 0;
		memset( tempbuffer, 0, 512 );
		
		if( fwrite( &m_PackageFilenameArray[i].m_length, sizeof(WORD), 1, pFilePtr ) < 1 )
		{
			_XMessage( _T("ERROR : IDXTBL Write file description array size [%s]"), indexfilename );
			fclose( pFilePtr );
			return FALSE;
		}
		
		memset( tempbuffer, 0, sizeof(TCHAR) * 512 );

		memset( mbcsstring, 0, 1024 );
		WideCharToMultiByte(GetACP(), 0, m_PackageFilenameArray[i].m_filename, -1, mbcsstring, 1024, NULL, NULL);

		strcpy( tempbuffer, mbcsstring );
		_EncodeData( tempbuffer, _ENCODEKEY );

		if( fwrite( &tempbuffer, m_PackageFilenameArray[i].m_length, 1, pFilePtr ) < 1 )
		{
			_XMessage( _T("ERROR : IDXTBL Write file description array [%s]"), indexfilename );
			fclose( pFilePtr );
			return FALSE;
		}
	}
	
	if( this->m_PackedFileCount > 0 )
	{
		if( fwrite( m_PackageFilePositionArray, sizeof(int)*this->m_PackedFileCount, 1, pFilePtr ) < 1 )
		{
			_XMessage( _T("ERROR : IDXTBL Write position array [%s]"), indexfilename );
			fclose( pFilePtr );
			return FALSE;
		}
	}	

	fclose( pFilePtr );

	return TRUE;
}
