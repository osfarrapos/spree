// Package.h: interface for the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGE_H__9692596A_6C98_11D5_A852_00022A009AE8__INCLUDED_)
#define AFX_PACKAGE_H__9692596A_6C98_11D5_A852_00022A009AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

BOOL CheckExistFile( LPCTSTR filename, BOOL resetAttrib );
BOOL WriteDirectory( CString dd );

#define _HEADERINDICATOR_PACKAGEFILE              0xFEFA
#define _HEADERINDICATOR_PACKAGEFILEVERSION		  0xA001
#define _ENCODEKEY								  0xEF

typedef struct 
{

	WORD HeaderIndicator;
	WORD FileVersion;	
	int  PackedFileCount;

} _PACKAGEFILE_HEADER;

class _XFileInfo
{
public:
	WORD	m_length;
	LPTSTR	m_filename;
	
	_XFileInfo()	
	{
		m_length	= 0;
		m_filename	= NULL;
	}

	~_XFileInfo()	
	{
		if( m_filename ) 
		{
			delete[] m_filename;
			m_filename = NULL;
		}
		m_length = 0;
	}

	void Set( LPCTSTR filename )
	{
		ASSERT( filename );
		if( m_filename ) delete[] m_filename;
		int length = wcslen( filename );
		
		m_filename = new TCHAR[ length+1 ];
		if( m_filename )
		{
			m_length = length;
			memset( m_filename, 0, sizeof(TCHAR) * (length+1) );
			wcscpy( m_filename, filename );
		}
		else
		{
			AfxMessageBox( _T("ERROR : Allocation memory !") );
		}
	}	
};

class CPackage  
{

public:
	FILE* m_PackageFileptr;

	int   m_PackedFileCount;

	int   m_PackefFileStartPosition;

	int*  m_PackageFilePositionArray;

	_XFileInfo* m_PackageFilenameArray;


public:
	CPackage();

	virtual ~CPackage();

	BOOL		Open( LPCTSTR filename, BOOL skipcreateindextable = FALSE );

	BOOL		Close( void );
	
	BOOL		CheckExistIndexFile( LPCTSTR filename );
	BOOL		SaveIndexTable( LPCTSTR filename );
	
	int			FindResource( LPCTSTR resourcename );
	LPCTSTR		GetResourceName( int file_id );
	int			GetResourceCount( void );
	
	FILE*		GetPackedFile( int file_id );
	FILE*		GetPackedFile( LPCTSTR file_name);
	
	BOOL		ExportFile( int file_id, LPCTSTR filename );
	BOOL		ExportFile( int file_id, FILE* pExportFile );
	
	BOOL		Patch( LPCTSTR packagefilename, LPCTSTR patchinfofilename );
	
	BOOL		PatchXPToXP( LPCTSTR packagefilename );
};

extern void _EncodeData( LPSTR string, BYTE key );

#endif // !defined(AFX_PACKAGE_H__9692596A_6C98_11D5_A852_00022A009AE8__INCLUDED_)
