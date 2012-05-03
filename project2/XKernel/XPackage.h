// XPackage.h: interface for the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPACKAGE_H_
#define _XPACKAGE_H_

/*#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000*/

#define _HEADERINDICATOR_PACKAGEFILE              0xFEFA
#define _HEADERINDICATOR_PACKAGEFILEVERSION		  0xA001
#define _ENCODEKEY								  0xEF
/*
BOOL CheckExistFile( LPCSTR filename, BOOL resetAttrib );
BOOL WriteDirectory( CString dd );
*/
extern HWND gHWnd;

#include "XList.h"
#include <assert.h>

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
		assert( filename );
		if( m_filename ) delete[] m_filename;
		int length = strlen( filename );
		
		m_filename = new TCHAR[ length+1 ];
		if( m_filename )
		{
			m_length = length;
			//m_filename[m_length] = 0;
			memset( m_filename, 0, m_length+1 );
			strcpy( m_filename, filename );

			for( int i = 0; i < m_length; i++ ) // make lower class
			{
				if( m_filename[i] >= 97 && m_filename[i] <= 122 )
				{
					m_filename[i] -= 32;
				}
			}
		}
		else
		{
			MessageBox( gHWnd, "ERROR : Allocate memory !", "TripleX Core", MB_OK );			
		}
	}
};

class _XFileInfoPtr : public _XItem
{
public:
	int			m_Index;
public:
	_XFileInfoPtr( int Index )
	{
				m_Index = Index;
	}
};

#pragma warning(disable:4786)

#include <map>
#include <string>
#include <vector>
#include <functional>

struct FILENAME_COMPARE : public std::binary_function<std::string, std::string, bool>
{
	static bool nocase_compare ( char c1, char c2)
	{
		return toupper(c1) < toupper(c2);
	}
	bool operator () (const std::string& lhs, const std::string& rhs) const
	{
		if (rhs.find(".d") != std::string::npos || rhs.find(".D") != std::string::npos)
		{
			return std::lexicographical_compare(lhs.begin(), lhs.end() - 4,
											rhs.begin(), rhs.end() - 4,
											nocase_compare);
		}
		return std::lexicographical_compare(lhs.begin(), lhs.end(),
										rhs.begin(), rhs.end(),
										nocase_compare);
	}
};

//#define _XDEF_PACK_EXPORTUSEDFILES

struct XFILE_INFO
{
	std::string strFileName;
	int			nLength;
	int			nFilePosition;

#ifdef _XDEF_PACK_EXPORTUSEDFILES
	BOOL	m_Used;
#endif

#ifdef _XDEF_PACK_EXPORTUSEDFILES
	XFILE_INFO()
	{
		m_Used = FALSE;
	}
#endif

};

class _XPackageArchive
{
private:
	typedef std::vector<XFILE_INFO*>							svdef_XFILE_INFO;
	typedef std::map<std::string, int, FILENAME_COMPARE>		smdef_FileTable;

	svdef_XFILE_INFO		m_svFileInfo;
	smdef_FileTable			m_smFileTable;

	TCHAR		m_ArchiveName[_MAX_PATH];
	
	FILE*		m_PackageFileptr;

	FILE*		m_EncryptPackageFileptr;

	int			m_PackedFileCount;

	int			m_PackefFileStartPosition;

	BOOL		m_UseVirtualMemory;
	LONG		m_PackageVirtualMemorySize;
	LPBYTE		m_PackageVirtualMemory;

	LONG		m_PackageFileSize;

	BOOL		m_EncryptedPackage;
	
public:
	BOOL		IsLock()	{ return m_PackageFileptr ? TRUE : FALSE; }
	_XPackageArchive();

	virtual ~_XPackageArchive();

	BOOL		OpenPackage( LPCTSTR filename );
	BOOL		ReOpenPackage( void );
	BOOL		ReOpenPackage_Thread( void );
	void		ClosePackage( void );
	void		FinalizePackage(void);
	
	void		SetEncryptedPackage( BOOL encrypted ){ m_EncryptedPackage = encrypted; }

	LPCTSTR		GetArchieveName( void ){ return m_ArchiveName; }

	inline BOOL	GetUseVirtualMemory( void ){ return m_UseVirtualMemory; }
	void		EnableVirtualMemory( BOOL enable )
	{
				m_UseVirtualMemory = enable;
				if(!m_UseVirtualMemory)
				{
					if(m_PackageVirtualMemory) 
					{ 
						delete[] (m_PackageVirtualMemory); 
						m_PackageVirtualMemory=NULL;
					}
				}
	};

	int			FindResource( LPCTSTR resourcename );
	LPCTSTR		GetResourceName( int file_id );
	int			GetResourceCount( void );
	
	LPBYTE		GetPackedMemory( int file_id );

	FILE*		GetPackedFile( int file_id );
	FILE*		GetPackedFile( int file_id, int offset );
	FILE*		GetPackedFile( LPSTR file_name);	
	LONG		GetPackedFileSize( int file_id );
	LONG		GetPackedFilePosition( int file_id );
	BOOL		GetPackedFileInfo( int file_id, DWORD& position, DWORD& size );
	LPBYTE		LoadPackedFileToMemory( LPSTR file_name, LONG* buffersize = NULL, BOOL reporterror = TRUE );
	LPBYTE		LoadPackedFileToMemory( DWORD resourceindex, LONG* buffersize = NULL );

	BOOL		ExportFile( int file_id, LPCTSTR filename );
	BOOL		ExportEncryptedFile( FILE* pPackedFilePtr, LONG filesize, LPCTSTR filename );

	BOOL		Patch( LPCTSTR packagefilename, LPCTSTR patchinfofilename );
	
	BOOL		CreateDecryptFile( LPTSTR orgpackagefilename, LPTSTR newpackagefilename );

#ifdef _XDEF_PACK_EXPORTUSEDFILES
	BOOL		ExportUsedFiles( void );
	BOOL		ExportUsedFilesList( void );
#endif
	
};

extern void _EncodeData( LPSTR string, BYTE key );

#endif
