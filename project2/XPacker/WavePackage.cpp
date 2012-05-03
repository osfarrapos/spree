// WavePackage.cpp: implementation of the CWavePackage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XPacker.h"
#include "WavePackage.h"

#include "Package.h"

#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWavePackage::CWavePackage()
{
	m_packedfile_count = 0;

	m_pPackageFile = NULL;
	
	m_infoheader_list = NULL;
}

CWavePackage::~CWavePackage()
{
	Reset();
}


void CWavePackage::Reset( void )
{
	if( m_pPackageFile ) fclose( m_pPackageFile );
	m_packedfile_count = 0;
	
	if( m_infoheader_list )
	{
		delete[] m_infoheader_list;
		m_infoheader_list = NULL;
	}
}

BOOL CWavePackage::Open( LPCSTR filename )
{
	Reset();
		
	m_pPackageFile = fopen( filename, "rb" );
	if( !m_pPackageFile )
	{
		AfxMessageBox( "Error : wave package file open" );
		return FALSE;
	}

	char indicator[8];
	if( fread( indicator, 8, 1, m_pPackageFile ) < 1 )
	{
		AfxMessageBox( "Error : wave package check" );
		Reset();
		return FALSE;
	}

	if( strncmp( indicator, _WP_INDICATOR, strlen(_WP_INDICATOR) ) != 0 )
	{
		AfxMessageBox( "Error : Not supported file type" );
		Reset();
		return FALSE;
	}
		
	if( fread( &m_packedfile_count, sizeof(int), 1, m_pPackageFile ) < 1 )
	{
		AfxMessageBox( "Error : get packed wave file count" );
		Reset();
		return FALSE;
	}

	m_infoheader_list = new _WAVEPACKED_INFOHEADER[m_packedfile_count];

	if( !m_infoheader_list )
	{
		AfxMessageBox( "Error : Memory allocation" );
		Reset();
		return FALSE;
	}

	for( int i = 0; i < m_packedfile_count; i++ )
	{
		if( fread( &(m_infoheader_list[i]), sizeof(_WAVEPACKED_INFOHEADER), 1, m_pPackageFile ) < 1 )
		{
			AfxMessageBox( "Error : Read wave package information" );
			Reset();
			return FALSE;
		}
	}

	return TRUE;
}

void CWavePackage::Close( void )
{
	Reset();
}

_WAVEPACKED_INFOHEADER* CWavePackage::GetPackedFileInfo( int index )
{ 
	if( index >= m_packedfile_count ) return NULL;

	if( m_infoheader_list )
	{
		return &(m_infoheader_list[index]);
	}
	else return NULL;
}

int  g_AllOverwrite = 0;
BOOL g_OverwriteQuestion = TRUE;
BOOL g_AllOverwriteQuestion = TRUE;

BOOL CWavePackage::ExportPackedFile( int index, LPCTSTR extractfolder )
{
	if( index >= m_packedfile_count )
	{
		AfxMessageBox( "Error : Package file / overflow boundary" );
		return FALSE;
	}

	if( !m_pPackageFile )
	{
		AfxMessageBox( "Error : Package file / not ready" );
		return FALSE;
	}
	
	if( extractfolder && extractfolder[0] )
	{
		if( _chdir( extractfolder ) == -1 )
		{
			AfxMessageBox( "Error : change extract folder" );
			return FALSE;
		}
	}
	
	if( g_OverwriteQuestion )
	{
		if( CheckExistFile( m_infoheader_list[index].filename, FALSE ) )
		{
			if( g_AllOverwrite == 1 ) return TRUE;
			if( g_AllOverwrite == 0 )
			{
				CString strMessage;
				strMessage.Format( "%s file already exist. Overwrite ?", m_infoheader_list[index].filename );
				
				if( AfxMessageBox( strMessage, MB_YESNO ) == IDNO )
				{
					if( g_AllOverwriteQuestion )
					{
						if( AfxMessageBox( "Skip all exist files ?", MB_YESNO ) == IDYES )
						{							
							g_AllOverwrite = 1;
						}				
						
						g_AllOverwriteQuestion = FALSE;
					}
					return TRUE;
				}
				if( g_AllOverwriteQuestion )
				{
					if( AfxMessageBox( "Overwrite all files ?", MB_YESNO ) == IDYES )
					{					
						g_AllOverwrite = 2;
					}				
					g_AllOverwriteQuestion = FALSE;
				}
			}
		}	
	}

	FILE* ExportFileptr = NULL;
	ExportFileptr = fopen( m_infoheader_list[index].filename, "wb" );

	if( !ExportFileptr )
	{
		AfxMessageBox( "Error : extract file open" );
		return FALSE;
	}

	int data = 0;

	//rewind( m_pPackageFile );
	fseek( m_pPackageFile, m_infoheader_list[index].packedoffset, SEEK_SET );

	for( DWORD i = 0; i < m_infoheader_list[index].filesize; i++ )
	{
		if( feof( m_pPackageFile ) ) break;

		data = fgetc( m_pPackageFile );

		data = fputc( data, ExportFileptr );
	}

	fclose( ExportFileptr );


	return TRUE;
}


BOOL CWavePackage::ExportPackedAllFile( LPCTSTR extractfolder )
{
	if( m_packedfile_count <= 0 )
	{
		AfxMessageBox( "Error : Empty package" );
		return FALSE;
	}

	if( !m_pPackageFile )
	{
		AfxMessageBox( "Error : Package file / not ready" );
		return FALSE;
	}

	for( int i = 0; i < m_packedfile_count; i++ )
	{
		if( ExportPackedFile( i, extractfolder ) == FALSE )
		{			
			return FALSE;
		}
	}
    
	return TRUE;
}
