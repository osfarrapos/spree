// SndTableList.cpp: implementation of the CSndTableList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XPacker.h"
#include "SndTableList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSndTableList::CSndTableList()
{
	m_sndScnTable = NULL;
	m_ItemCount = 0;

}

CSndTableList::~CSndTableList()
{
	if( m_sndScnTable ) 
	{
		delete[] m_sndScnTable;
		m_sndScnTable = NULL;
	}
}

void CSndTableList::resetSndList( void )
{
	m_ItemCount = 0;

	if( m_sndScnTable ) 
	{
		for( DWORD i=0; i < m_ItemCount; i++ )
		{
			if( m_sndScnTable[i].table )
			{
				for( DWORD j=0; j < m_sndScnTable[i].nSnd; j++ )
					 delete[] m_sndScnTable[i].table[j];				
			}		
		}

		delete[] m_sndScnTable;
		m_sndScnTable = NULL;
	}
}

const LPSNDSCENE CSndTableList::getSndScene( DWORD index )
{
	if( m_ItemCount <= 0 || index >= m_ItemCount ) return NULL;
		
	return &m_sndScnTable[index];
}

BOOL CSndTableList::loadSndList( LPCSTR filename )
{	
	resetSndList();

	FILE* sndlistfile = NULL;
	
	sndlistfile = fopen(filename,"rb");	

	if( !sndlistfile ) return FALSE;

	char head[sizeof(SNDLISTHEADER)+1];

	if( fread(head, sizeof(char), strlen(SNDLISTHEADER)+1, sndlistfile ) < 1 )
	{
		fclose( sndlistfile );
		return FALSE;
	}
			
	if(!strcmp(head,SNDLISTHEADER))
	{
		if( fread(&m_ItemCount, sizeof(DWORD), 1, sndlistfile) < 1 )
		{
			resetSndList();
			fclose( sndlistfile );
			return FALSE;
		}
	}
	else
	{
		fclose( sndlistfile );
		return FALSE;
	}

	m_sndScnTable = new SNDSCENE[m_ItemCount]; 
	
	if( !m_sndScnTable ) 
	{
		resetSndList();
		fclose( sndlistfile );
		return FALSE;
	}

	memset( m_sndScnTable, 0, sizeof(SNDSCENE) * m_ItemCount );

	for( DWORD i=0; i < m_ItemCount; i++)
	{
		memset( &m_sndScnTable[i].li, 0, sizeof(SNDLISTITEM) );
		fread( &m_sndScnTable[i].li, sizeof(SNDLISTITEM), 1, sndlistfile );	
		m_sndScnTable[i].nSnd=0;
		m_sndScnTable[i].table=NULL;
	}
	
	fclose( sndlistfile );

	return TRUE;
}


BOOL CSndTableList::saveSndList( LPCSTR filename )
{
	FILE* sndlistfile = NULL;
	
	sndlistfile = fopen(filename,"wb");	

	if( !sndlistfile ) return FALSE;

	char head[sizeof(SNDLISTHEADER)+1];
	memset( head, 0, sizeof(SNDLISTHEADER)+1 ); 
	strcpy( head, SNDLISTHEADER );

	if( fwrite(head, sizeof(char), strlen(SNDLISTHEADER)+1, sndlistfile ) < 1 )
	{
		fclose( sndlistfile );
		return FALSE;
	}
			
	if( fwrite(&m_ItemCount, sizeof(DWORD), 1, sndlistfile) < 1 )
	{
		fclose( sndlistfile );
		return FALSE;
	}	
	
	for( DWORD i=0; i < m_ItemCount; i++)
	{		
		fwrite( &m_sndScnTable[i].li, sizeof(SNDLISTITEM), 1, sndlistfile );
	}
	
	fclose( sndlistfile );


	return TRUE;
}


BOOL CSndTableList::insertSceneList( DWORD index )
{
	if( !m_sndScnTable || m_ItemCount <= 0 )
	{		
		m_sndScnTable =  new SNDSCENE;

		if( !m_sndScnTable )
		{
			AfxMessageBox( "ERROR : Allocation memory" );
			return FALSE;
		}

		memset( m_sndScnTable, 0, sizeof(SNDSCENE));		

		m_ItemCount++;		
	}
	else
	{
		if( m_ItemCount <= 0 || index > m_ItemCount ) return FALSE;
		if( !m_sndScnTable ) return FALSE;

		LPSNDSCENE t_psndScnTable;

		t_psndScnTable = new SNDSCENE[m_ItemCount+1];

		if( !t_psndScnTable )
		{
			AfxMessageBox( "ERROR : Allocation memory" );
			return FALSE;
		}

		memset( t_psndScnTable, 0, sizeof(SNDSCENE) * (m_ItemCount+1) );

		if( index == 0 )
		{
			memcpy( (t_psndScnTable+1), m_sndScnTable, sizeof(SNDSCENE) * (m_ItemCount) );
		}
		else if( index == m_ItemCount )
		{
			memcpy( t_psndScnTable, m_sndScnTable, sizeof(SNDSCENE) * (m_ItemCount) );
		}
		else
		{
			memcpy( t_psndScnTable, m_sndScnTable, sizeof(SNDSCENE) * (index) );

			memcpy( (t_psndScnTable+index+1), m_sndScnTable+index, sizeof(SNDSCENE) * (m_ItemCount-index) );
		}
		
		delete[] m_sndScnTable;
		m_sndScnTable = t_psndScnTable;

		m_ItemCount++;		
	}

	return TRUE;
}

BOOL CSndTableList::deleteSceneList( DWORD index )
{
	if( m_ItemCount <= 0 || index >= m_ItemCount ) return FALSE;
	if( !m_sndScnTable ) return FALSE;


	LPSNDSCENE t_psndScnTable;

	t_psndScnTable = new SNDSCENE[m_ItemCount-1];

	if( !t_psndScnTable )
	{
		AfxMessageBox( "ERROR : Allocation memory" );
		return FALSE;
	}

	memset( t_psndScnTable, 0, sizeof(SNDSCENE) * (m_ItemCount-1) );

	if( index == 0 )
	{
		memcpy( t_psndScnTable, m_sndScnTable+1, sizeof(SNDSCENE) * (m_ItemCount-1) );
	}
	else if( index == m_ItemCount )
	{
		memcpy( t_psndScnTable, m_sndScnTable, sizeof(SNDSCENE) * (m_ItemCount-1) );
	}
	else
	{
		memcpy( t_psndScnTable, m_sndScnTable, sizeof(SNDSCENE) * (index) );

		memcpy( (t_psndScnTable+index), m_sndScnTable+index+1, sizeof(SNDSCENE) * (m_ItemCount-index-1) );
	}
	
	delete[] m_sndScnTable;
	m_sndScnTable = t_psndScnTable;

	m_ItemCount--;

	return TRUE;
}


BOOL CSndTableList::loadSoundScene( LPCSTR filename, LPSNDSCENE psndscene, LPSNDINFO& psndinfo )
{
	if( !psndscene || !filename || !filename[0] ) return FALSE;
	FILE *sslfileptr    = NULL;

	if( psndinfo )
	{
		delete[] psndinfo;
		psndinfo = NULL;
	}
	
	sslfileptr = fopen( filename, "rb" );

	if( !sslfileptr )
	{
		CString tMessageStr; 
		tMessageStr.Format( "ERROR : %s file open !", filename );
		AfxMessageBox( tMessageStr );
		return FALSE;
	}

	char scnHeader[sizeof(SNDSCNHEADER)+1];	

	// laod header indicator...
	fread(&scnHeader,sizeof(char),strlen(SNDSCNHEADER)+1,sslfileptr);	

	if( strcmp(scnHeader,SNDSCNHEADER) )	
	{
		AfxMessageBox( "ERROR : Not supported file format" );
		fclose( sslfileptr );
		return FALSE;
	}
		
	fread(&(psndscene->nSnd),sizeof(DWORD),1,sslfileptr);
	fread(psndscene->loadBlock,sizeof(LOADHANDLE),1,sslfileptr);
	fread(psndscene->loadPath,sizeof(LOADPATH),1,sslfileptr);

	psndinfo = new SNDINFO[psndscene->nSnd];

	if( !psndinfo )
	{
		psndscene->nSnd = 0;
		memset( psndscene->loadBlock, 0, sizeof( LOADHANDLE ) );
		memset( psndscene->loadPath, 0, sizeof( LOADPATH ) );
		AfxMessageBox( "ERROR : Allocation memory" );
		fclose( sslfileptr );
		return FALSE;
	}
/*	sndscene->table = new LPSNDOBJ[ sndscene->nSnd ];

	if( !sndscene->table )
	{
		sndscene->nSnd = 0;
		memset( sndscene->loadBlock, 0, sizeof( LOADHANDLE ) );
		memset( sndscene->loadPath, 0, sizeof( LOADPATH ) );
		AfxMessageBox( "ERROR : Allocation memory" );
		fclose( sslfileptr );
		return FALSE;
	}
*/
	for(DWORD i=0; i<psndscene->nSnd; i++)
	{			
		fread(&(psndinfo[i]), sizeof(SNDINFO), 1, sslfileptr);
		// I substitute the correct drive letter
		//sndInfo.loadPath[0]=sndDriveContext;		
//		sndscene->table[i]=NULL;
		//sndscene->table[i]=SndObjCreate(&sndInfo,sndscene->loadBlock,sndscene->loadPath);
		//myassert(sndScnTable[hFile]!=NULL,NULL,sndInfo.loadID);		
	}		
	
	fclose( sslfileptr );

	return TRUE;
}


/*

LPSNDOBJ SndObjCreate(SNDINFO *pInfo, LOADHANDLE lBlock, LOADPATH lPath)
{
	LPSNDOBJ lpSndObj=NULL;		
	DWORD lengthData;
	DWORD bytesPerSec;

	if(!HwSndGetCardDetect())
		return NULL;
	
	lpSndObj=MyMalloc(SNDOBJ,1);
	ASSERTOUTMEMORY(lpSndObj);
	lpSndObj->info=*pInfo;
	if(lpSndObj->info.nInstRequired<1){
		DEBUGASSERT(lpSndObj->info.nInstRequired>=1);
		lpSndObj->info.nInstRequired=1;
	}
	lpSndObj->nInst=0;
	lpSndObj->lpMem=NULL;
	strcpy(lpSndObj->loadInfo.loadBlock,lBlock);
	strcpy(lpSndObj->loadInfo.loadPath,lPath);
	if(!(lpSndObj->info.sndType & SND_STREAMING) || 
										(lpSndObj->info.sndType & SND_LOADRAM)){
		lpSndObj->lpMem=HwSndObjLoad(lpSndObj->loadInfo.loadPath,
							lpSndObj->loadInfo.loadBlock,lpSndObj->info.loadID);
		ASSERTFILE(lpSndObj->lpMem!=NULL,lpSndObj->info.loadID);
	}
	if(!(lpSndObj->info.sndType & SND_STREAMING)){
		SndObjInitInstVector(lpSndObj, &lengthData);
	}
	else {
		 SndObjGetFmt(lpSndObj->lpMem,lpSndObj->loadInfo.loadPath, 
						lpSndObj->loadInfo.loadBlock,lpSndObj->info.loadID,&lpSndObj->fmt,&lengthData);	
		lpSndObj->instVector=NULL;
	}
	if(!(lpSndObj->info.sndType & SND_LOADRAM) && lpSndObj->lpMem){
		MyFree(lpSndObj->lpMem);
		lpSndObj->lpMem=NULL;
	}
	bytesPerSec=lpSndObj->fmt.nChannels*lpSndObj->fmt.nSamplesPerSec*
												(lpSndObj->fmt.wBitsPerSample/8);
	lpSndObj->timeLength=PDIV(INTTOP(lengthData),INTTOP(bytesPerSec));
	
	return lpSndObj;
}

*/



