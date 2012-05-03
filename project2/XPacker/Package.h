// Package.h: interface for the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGE_H__9692596A_6C98_11D5_A852_00022A009AE8__INCLUDED_)
#define AFX_PACKAGE_H__9692596A_6C98_11D5_A852_00022A009AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

BOOL CheckExistFile( LPCSTR filename, BOOL resetAttrib );
BOOL WriteDirectory( CString dd );

class _XFileInfo
{
public:
	WORD	m_length;
	LPSTR	m_filename;
	
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

	void Set( LPSTR filename )
	{
		ASSERT( filename );
		if( m_filename ) delete[] m_filename;
		int length = strlen( filename );
		
		m_filename = new char[ length ];
		if( m_filename ) m_length = length;
		else
		{
			AfxMessageBox( "ERROR : Allocate memory !" );
		}
	}
};

class CPackageArchive
{

	FILE*		m_PackageFileptr;

	int			m_PackedFileCount;

	int			m_PackefFileStartPosition;

	int*		m_PackageFilePositionArray;

	_XFileInfo* m_PackageFilenameArray;

public:
	CPackageArchive();

	virtual ~CPackageArchive();

	BOOL Open( LPCTSTR filename );

	BOOL Close( void );

	int  FindResource( LPSTR resourcename );

	FILE* GetPackedFile( int file_id );
	FILE* GetPackedFile( LPSTR file_name);

	BOOL  ExportFile( int file_id, LPCTSTR filename );

	BOOL  Patch( LPCTSTR packagefilename, LPCTSTR patchinfofilename );

};

#endif // !defined(AFX_PACKAGE_H__9692596A_6C98_11D5_A852_00022A009AE8__INCLUDED_)
