#include "..\stdafx.h"
#include "VMManager.h"
#include "..\Global\Global_LogFile.h"

extern _j_GlobalLog * g_pSystemLog ;

bool _h_VMManager::Create( unsigned int MegaSize )
{
	if( m_pStartAddr )		// create 함수 중복 호출 체크
	{

		::PrintConsole( "[ME_ERR] 가상 메모리의 Create 를 2번 이상 호출하였습니다.\n" ) ;


		return false ;
	}

	if( MegaSize > 3000U )	// 너무 큰 메모리 사용 체크
		return false ;
	// 가상 메모리를 예약해 둔다. 
	m_pStartAddr = VirtualAlloc( NULL, MegaSize * _MEGA_, MEM_RESERVE, PAGE_READWRITE ) ;

	
	// VirtualAlloc에 의하여 실제로 할당된 메모리 영역에 대한 세부 ////
	// 정보를 얻어와 확인한다. 여기서는 할당된 크기만을 확인하였다.
	MEMORY_BASIC_INFORMATION memoryinfo;
	VirtualQuery(m_pStartAddr, &memoryinfo, sizeof(memoryinfo));	
	printf("-> Memory zone Reserved (Bytes:%d Mega:%d Pages:%d )\n",
		(UINT)(memoryinfo.RegionSize),(UINT)(memoryinfo.RegionSize)/_MEGA_,(UINT)(memoryinfo.RegionSize/4096));
	g_pSystemLog->LogFileWrite("-> Memory zone Reserved (Bytes:%d Mega:%d Pages:%d )\n",
		(UINT)(memoryinfo.RegionSize),(UINT)(memoryinfo.RegionSize)/_MEGA_,(UINT)(memoryinfo.RegionSize/4096));
	////////////////////////////////////////////////////////////////////

	if( m_pStartAddr )
	{		
		m_uiReserveSize = MegaSize * _MEGA_ ;
		m_pCurrAddr = m_pStartAddr ;
		return true ;
	}
	else
		DWORD dError = GetLastError() ;
		return false ;
}

void * _h_VMManager::MemoryCommit( unsigned int Size )
{
	void * pVoid = NULL ;
	UINT realSize ;			// Size 와 비교하여, 실제로 커밋할 메모리를 계산하고, 이 값을 사용한다. 
	int temp ;
	UINT checkSize ;
	
	temp = Size % _PAGE_SIZE_ ;
	
	if( temp != 0 )
	{
		realSize = Size - temp + _PAGE_SIZE_ ;
	}
	else
	{
		realSize = Size ;
	}

	checkSize = m_uiCurrSize + realSize ;
	if( m_uiReserveSize < checkSize )
	{
		printf( "[ME_ERR] 예약된 사이즈를 모두 사용하였습니다.\n" ) ;

		return NULL ;
	}

	pVoid = VirtualAlloc( m_pCurrAddr, realSize, MEM_COMMIT, PAGE_READWRITE ) ;
	if( pVoid )
	{
		memset( pVoid, 0, realSize ) ;
		m_pCurrAddr = (PBYTE)m_pCurrAddr + realSize ;
		m_uiCurrSize = checkSize ;

		printf( "[INFO] MEM_COMMIT : Reserve = %u, Used = %u, CurrAddr = 0x%X\n", m_uiReserveSize, m_uiCurrSize, pVoid ) ;		
		
	}
	
	return pVoid ;		// 리턴값이 null 이면, 에러다. 
}

void _h_VMManager::Destroy()
{
	if( m_pStartAddr )
	{
		if( VirtualFree( m_pStartAddr, m_uiReserveSize * _MEGA_, MEM_DECOMMIT ) == 0 )
			return ;

		VirtualFree( m_pStartAddr, 0, MEM_RELEASE ) ;
	}
}