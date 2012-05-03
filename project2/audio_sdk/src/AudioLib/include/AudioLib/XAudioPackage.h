// XPackage.h: interface for the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XAUDIOPACKAGE_H_
#define _XAUDIOPACKAGE_H_

/*#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000*/

#define _HEADERINDICATOR_AUDIOPACKAGEFILE              0xFEFA
#define _HEADERINDICATOR_AUDIOPACKAGEFILEVERSION	   0xA001
#define _AUDIOENCODEKEY								   0xEF

#include <windows.h>
#include <stdio.h>

extern void _XFatalError (TCHAR *str, ...);

typedef struct 
{

	WORD HeaderIndicator;
	WORD FileVersion;	
	int  PackedFileCount;

} _AUDIOPACKAGEFILE_HEADER;

class _XAudioFileInfo
{
public:
	WORD	m_length;
	LPSTR	m_filename;
	
	_XAudioFileInfo()	
	{
		m_length	= 0;
		m_filename	= NULL;
	}

	~_XAudioFileInfo()	
	{
		if( m_filename ) 
		{
			delete[] m_filename;
			m_filename = NULL;
		}
		m_length = 0;
	}

	void Set( LPSTR filename )
	{		
		if( m_filename ) delete[] m_filename;
		int length = strlen( filename );
		
		m_filename = new TCHAR[ length+1 ];
		if( m_filename )
		{
			m_length = length;
			memset( m_filename, 0, length+1 );
			strcpy( m_filename, filename );
		}
		else
		{
			_XFatalError( "Allocate memory !" );
		}
	}
};

class _XAudioPackageArchive
{
	TCHAR		m_ArchiveName[_MAX_PATH];
	
	FILE*		m_PackageFileptr;

	int			m_PackedFileCount;

	int			m_PackefFileStartPosition;

	int*		m_PackageFilePositionArray;

	_XAudioFileInfo* m_PackageFilenameArray;

public:
	_XAudioPackageArchive();

	virtual ~_XAudioPackageArchive();

	BOOL		Open( LPCTSTR filename );
	BOOL		ReOpen( void );
	BOOL		Close( BOOL resetfilelist = TRUE );

	int			FindResource( LPSTR resourcename );
	LPSTR		GetResourceName( int file_id );
	int			GetResourceCount( void );
	
	FILE*		GetPackedFile( int file_id );
	FILE*		GetPackedFile( LPSTR file_name);	
	LONG		GetPackedFileSize( int file_id );
	LPBYTE		LoadPackedFileToMemory( LPSTR file_name, LONG* buffersize = NULL);
	LPBYTE		LoadPackedFileToMemory( DWORD resourceindex, LONG* buffersize = NULL);

	BOOL		ExportFile( int file_id, LPCTSTR filename );

	BOOL		Patch( LPCTSTR packagefilename, LPCTSTR patchinfofilename );

};

extern void _XAudioEncodeData( LPSTR string, BYTE key );

#endif
