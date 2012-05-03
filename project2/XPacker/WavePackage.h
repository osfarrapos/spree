// WavePackage.h: interface for the CWavePackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEPACKAGE_H__9CBFFE52_A948_4857_997C_99D3768F3DBF__INCLUDED_)
#define AFX_WAVEPACKAGE_H__9CBFFE52_A948_4857_997C_99D3768F3DBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WP_INDICATOR  "PACK02W"

typedef struct
{
   char  filename[16];
   DWORD filesize;
   DWORD packedoffset;
} _WAVEPACKED_INFOHEADER;



class CWavePackage  
{
	int    m_packedfile_count;

	FILE*  m_pPackageFile;
	
	_WAVEPACKED_INFOHEADER* m_infoheader_list;

public:
	CWavePackage();
	virtual ~CWavePackage();

	void Reset( void );

	BOOL Open( LPCSTR filename );
	void Close( void );

	int  GetPackedFileCount( void ){ return m_packedfile_count; }
	
	_WAVEPACKED_INFOHEADER* GetPackedFileInfo( int index );

	BOOL ExportPackedFile( int index, LPCTSTR extractfolder );
	BOOL ExportPackedAllFile( LPCTSTR extractfolder );
	
};

#endif // !defined(AFX_WAVEPACKAGE_H__9CBFFE52_A948_4857_997C_99D3768F3DBF__INCLUDED_)
