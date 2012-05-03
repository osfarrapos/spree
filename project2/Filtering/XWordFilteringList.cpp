#include "stdafx.h"
#include "XKernel.h"
#include "XWordFilteringList.h"

#ifndef	MAX_DATA_LENGTH
	#define	MAX_DATA_LENGTH		256
#endif

_XWORDFILTERNODE::_XWORDFILTERNODE()
{
	pNext				= (_XWORDFILTERNODE*)0;

	pData				= (char*)0;
#ifdef _XDEF_NEWVERSONFILETERING
	pChangeData			= (char*)0;
#endif
	FilteringType		= _XDEF_WORDFILETERINGTYPE_ALWAYS;
}

_XWORDFILTERNODE::~_XWORDFILTERNODE()
{
	pNext		= (_XWORDFILTERNODE*)0;

	if( pData != NULL)	delete[] pData; //(char*)0;
	pData		=(char*)0;
#ifdef _XDEF_NEWVERSONFILETERING
	if( pChangeData != NULL)	delete[] pChangeData;
	pChangeData			= (char*)0;
#endif
}

_XWordFilteringList::_XWordFilteringList()
{
	m_pTop				= (_XWORDFILTERNODE*)0;
}

_XWordFilteringList::~_XWordFilteringList()
{
	if( m_pTop != NULL)
	{
		RemoveAll();
		delete[] m_pTop;
		m_pTop	= (_XWORDFILTERNODE*)0;
	}
}

void _XWordFilteringList::AddData(char* pData, int nDataSize, BYTE filteringtype)
{
	register int			i;

	_XWORDFILTERNODE*			pTmp	= (_XWORDFILTERNODE*)0;
	_XWORDFILTERNODE*			pThis	= (_XWORDFILTERNODE*)0;
	
	if(m_pTop == (_XWORDFILTERNODE*)0)
	{
		pThis			= new _XWORDFILTERNODE[MAX_DATA_LENGTH];
		m_pTop			= pThis;
	}
	else
		pThis			= m_pTop;
	
	for(i = 0; i < nDataSize; i++)
	{
		if(pThis == (_XWORDFILTERNODE*)0)
		{
			pThis		= new _XWORDFILTERNODE[MAX_DATA_LENGTH];
			pTmp->pNext	= pThis;
		}
		
		pTmp			= &pThis[(unsigned char)pData[i]];
		pThis			= pTmp->pNext;
	}
	
	if(pTmp->pData == (char*)0)
	{
		pTmp->pData	= new CHAR[nDataSize+1];
		memset(pTmp->pData, 0x00, nDataSize+1 );
		strcpy(pTmp->pData, pData);
		pTmp->FilteringType = filteringtype;

		//_XLog( "%s", pTmp->pData );
	}
}

#ifdef _XDEF_NEWVERSONFILETERING
void _XWordFilteringList::AddChangeData(char* pData, int nDataSize, char* pChangeData, int nChangeDataSize, BYTE filteringtype)
{
	register int			i;
	
	_XWORDFILTERNODE*			pTmp	= (_XWORDFILTERNODE*)0;
	_XWORDFILTERNODE*			pThis	= (_XWORDFILTERNODE*)0;
	
	if(m_pTop == (_XWORDFILTERNODE*)0)
	{
		pThis			= new _XWORDFILTERNODE[MAX_DATA_LENGTH];
		m_pTop			= pThis;
	}
	else
		pThis			= m_pTop;
	
	for(i = 0; i < nDataSize; i++)
	{
		if(pThis == (_XWORDFILTERNODE*)0)
		{
			pThis		= new _XWORDFILTERNODE[MAX_DATA_LENGTH];
			pTmp->pNext	= pThis;
		}
		
		pTmp			= &pThis[(unsigned char)pData[i]];
		pThis			= pTmp->pNext;
	}
	
	if(pTmp->pData == (char*)0 && pTmp->pChangeData == (char*)0)
	{
		pTmp->pData	= new CHAR[nDataSize+1];
		memset(pTmp->pData, 0x00, nDataSize+1 );
		strcpy(pTmp->pData, pData);
		
		// 바뀐 데이타를 넣는다.	
		pTmp->pChangeData	= new CHAR[nChangeDataSize+1];
		memset(pTmp->pChangeData, 0x00, nChangeDataSize+1 );
		strcpy(pTmp->pChangeData, pChangeData);

		pTmp->FilteringType = filteringtype;
	}	
}
#endif

#ifdef _XDEF_NEWVERSONFILETERING
char* _XWordFilteringList::SearchChangeData(char* pData, int& nTargetDataLength ,BYTE& filteringtype)
{
	char*				pTmpData= (char*)0;
	char*				pBuffer	= (char*)0;
	char*				pChangeBuffer	= (char*)0;
	BYTE				lastfilteringtype = 0;
	
	_XWORDFILTERNODE*			pTmp	= (_XWORDFILTERNODE*)0;
	_XWORDFILTERNODE*			pThis	= (_XWORDFILTERNODE*)0;
	
	pThis				= m_pTop;
	pTmpData			= pData;
	
	while((pThis != (_XWORDFILTERNODE*)0))
	{
		pTmp			= &pThis[(unsigned char)pTmpData[0]];
		
		if(pTmp->pData != (char*)0 && pTmp->pChangeData != (char*)0)
		{
			pBuffer				= pTmp->pData;
			pChangeBuffer		= pTmp->pChangeData;

			lastfilteringtype	= pTmp->FilteringType;
		}
		
		pThis			= pThis[(unsigned char)pTmpData[0]].pNext;
		
		pTmpData++;
	}
	
	if( pBuffer && pTmp && pChangeBuffer)
	{		
		if( filteringtype & _XDEF_WORDFILETERINGTYPE_CHANGE )
		{
			if( lastfilteringtype & _XDEF_WORDFILETERINGTYPE_CHANGE )
			{
			//	filteringtype = lastfilteringtype;
				nTargetDataLength = strlen(pBuffer);
				return pChangeBuffer;					
			}
		}
	}
	
	return NULL;
}
#endif
char* _XWordFilteringList::SearchData(char* pData,BYTE& filteringtype)
{
	char*				pTmpData= (char*)0;
	char*				pBuffer	= (char*)0;
	BYTE				lastfilteringtype = 0;
	
	_XWORDFILTERNODE*			pTmp	= (_XWORDFILTERNODE*)0;
	_XWORDFILTERNODE*			pThis	= (_XWORDFILTERNODE*)0;
	
	pThis				= m_pTop;
	pTmpData			= pData;
	
	while((pThis != (_XWORDFILTERNODE*)0))
	{
		pTmp			= &pThis[(unsigned char)pTmpData[0]];
		
		if(pTmp->pData != (char*)0)
		{
			pBuffer				= pTmp->pData;
			lastfilteringtype	= pTmp->FilteringType;
		}
		
		pThis			= pThis[(unsigned char)pTmpData[0]].pNext;
		
		pTmpData++;
	}

	if( pBuffer && pTmp )
	{
		if( filteringtype & _XDEF_WORDFILETERINGTYPE_ALWAYS )
		{
			if( lastfilteringtype & _XDEF_WORDFILETERINGTYPE_ALWAYS )
			{
				filteringtype = lastfilteringtype;
				return pBuffer;
			}
		}
		
		if( filteringtype & _XDEF_WORDFILETERINGTYPE_NAME )
		{
			if( lastfilteringtype & _XDEF_WORDFILETERINGTYPE_NAME )
			{
				filteringtype = lastfilteringtype;
				return pBuffer;
			}
		}
		
		if( filteringtype & _XDEF_WORDFILETERINGTYPE_GUILD )
		{
			if( lastfilteringtype & _XDEF_WORDFILETERINGTYPE_GUILD )
			{
				filteringtype = lastfilteringtype;
				return pBuffer;
			}
		}
		
		if( filteringtype & _XDEF_WORDFILETERINGTYPE_CHANGE )
		{
			if( lastfilteringtype & _XDEF_WORDFILETERINGTYPE_CHANGE )
			{
				filteringtype = lastfilteringtype;
				return pBuffer;
			}
		}
	}
	
	return NULL;
}

void _XWordFilteringList::RemoveAll()
{
	_XWORDFILTERNODE*			pTmp	= (_XWORDFILTERNODE*)0;
	_XWORDFILTERNODE*			pThis	= (_XWORDFILTERNODE*)0;

	register int			i;

	pThis				= m_pTop;

	while((pThis != (_XWORDFILTERNODE*)0))
	{
		for(i = 0; i < MAX_DATA_LENGTH; i++)
		{
			if(pThis[i].pNext != (_XWORDFILTERNODE*)0)
			{
				pTmp	= &pThis[i];
				pThis	= pThis[i].pNext;

				i	= 0;

				continue;
			}

			if(pThis[i].pData != (char*)0)
			{
				SAFE_DELETE_ARRAY( pThis[i].pData);
			}

#ifdef _XDEF_NEWVERSONFILETERING
			if(pThis[i].pChangeData != (char*)0)
			{
				SAFE_DELETE_ARRAY( pThis[i].pChangeData );
			}			
#endif
		}

		if(pThis != m_pTop)
		{
			SAFE_DELETE_ARRAY( pThis);
			pTmp->pNext = (_XWORDFILTERNODE*)0;
		}
		else
		{
			delete[] pThis->pData;
			pThis->pData	= (CHAR*)0;
			pThis->pNext	= (_XWORDFILTERNODE*)0;

			delete[] pThis; 
			pThis			= (_XWORDFILTERNODE*)0;
			m_pTop			= (_XWORDFILTERNODE*)0;			

		}
		pThis				= m_pTop;
	}
	
	if( m_pTop != NULL )
	{
		delete[] m_pTop;
		m_pTop	= (_XWORDFILTERNODE*)0;
	}
	
}
