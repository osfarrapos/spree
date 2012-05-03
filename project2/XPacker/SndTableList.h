// SndTableList.h: interface for the CSndTableList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNDTABLELIST_H__40D09599_D674_4801_BAD6_2AC834368A56__INCLUDED_)
#define AFX_SNDTABLELIST_H__40D09599_D674_4801_BAD6_2AC834368A56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SndStructure.h"

class CSndTableList  
{

private:
	DWORD m_ItemCount;

	LPSNDSCENE m_sndScnTable;

public:
	
	CSndTableList();
	virtual ~CSndTableList();

	void resetSndList( void );

	BOOL loadSndList( LPCSTR filename );
	BOOL saveSndList( LPCSTR filename );

	const LPSNDSCENE getSndScene( DWORD index );
	DWORD      getItemCount( void ){ return m_ItemCount; }

	BOOL  insertSceneList( DWORD index );
	BOOL  deleteSceneList( DWORD index );

	BOOL  loadSoundScene( LPCSTR filename, LPSNDSCENE psndscene, LPSNDINFO& psndinfo );

};

#endif // !defined(AFX_SNDTABLELIST_H__40D09599_D674_4801_BAD6_2AC834368A56__INCLUDED_)
