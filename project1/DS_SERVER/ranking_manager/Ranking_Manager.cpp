#include "..\stdafx.h"
#include "Ranking_Manager.h"



void CRankInfo::GetRankInfo( _Rank * pRank, size_t size )
{
	rank_lock() ;
	try{

		for( u_int i = 0 ;  i < _MAX_RANK ; ++i )
		{
			if( size <= i )
			{
				break ;
			}

			pRank[i] = m_vRankInfo[i] ;
		}
	}
	catch (...) {
		::PrintConsole("[EXCEPTION] %s, %d \n", __FILE__, __LINE__ ) ;
	}		
	rank_unlock() ;
}