#pragma		once 
#include <vector>

using namespace std;

namespace _ranking
{
	enum
	{
		_RANK_TYPE_VIMU,		// 비무랭킹
		_RANK_TYPE_PART,		// 문파랭킹
		_RANK_TYPE_LEVEL,		// 성취단계 랭킹
		_RANK_TYPE_MARTIAL,		// 무공랭킹
		_RANK_TYPE_MAX,
	} ;
}

#define _MAX_RANK	100
struct _Rank ;

class CRankInfo
{
private:
	short				m_sRankType ;	// 랭킹 종류
	u_int				m_uiMaxRanking ;
	_Rank				m_vRankInfo[_MAX_RANK] ;	// 랭킹 리스트	

	CTraceAbleLock		m_csRank ;

#define rank_lock()		m_csRank.tlock( __FILE__, __LINE__ ) ;
#define rank_unlock()	m_csRank.tunlock( __FILE__, __LINE__ ) ;	

public:
	CRankInfo() : m_sRankType(0)
	{
		memset( m_vRankInfo, 0 , sizeof(m_vRankInfo) ) ;
	}

	~CRankInfo()
	{		
	}



	inline void SetTypeNum(short sType, u_int uiMaxCount )
	{
		m_sRankType = sType ;
		m_uiMaxRanking = uiMaxCount ;
	}

	inline void Release()
	{
		rank_lock() ;
		try{
			memset( m_vRankInfo, 0 , sizeof(m_vRankInfo) ) ;
		}
		catch(...){
		}		
		rank_unlock() ;
	}



/*
#ifdef _PD_LEVEL_RANK_OVERLAP_

	inline void InsertRank( _Rank & rank, u_int slot )
	{
		if( slot > m_uiMaxRanking )
			return ;

		rank_lock() ;
		try{
			m_vRankInfo[slot] = rank ;
		}
		catch(...){
		}		
		rank_unlock() ;
	}


#else
*/
	inline void InsertRank( _Rank & rank )
	{
		if( rank.sRanking > static_cast<short>(m_uiMaxRanking) || rank.sRanking < 0 )
			return ;

		rank_lock() ;
		try{
			m_vRankInfo[rank.sRanking] = rank ;
		}
		catch(...){
		}		
		rank_unlock() ;
	}
//#endif

	inline u_int GetMaxCount() {		
		return m_uiMaxRanking ;
	}
	int	GetMyRank( const char * name ) ;				// -1 이면 순위권 밖	
	void GetRankInfo( _Rank * pRank, size_t size ) ;	
};

class CRankingManager
{
private :
	CRankInfo *			m_pRankList ;	// 랭킹 정보들의 리스트


public:
	CRankingManager()
	{
		u_int uiMaxCount = 0 ;
		m_pRankList = new CRankInfo[_ranking::_RANK_TYPE_MAX] ;
		for( int i = 0 ; i < _ranking::_RANK_TYPE_MAX ; i++ )
		{
			switch( i ) {
				case _ranking::_RANK_TYPE_VIMU :		
					break;
				case _ranking::_RANK_TYPE_LEVEL :
					uiMaxCount = 9 ;
					break;
				case _ranking::_RANK_TYPE_MARTIAL :
					break; 
				case _ranking::_RANK_TYPE_PART :
					break; 
				default:
					break;
			}
			(&m_pRankList[i])->SetTypeNum( i, uiMaxCount ) ;
		}
	}

	~CRankingManager()
	{
		delete [] m_pRankList ;
	}

	inline CRankInfo * GetRankInfoPtr( short sType )
	{
		return (&m_pRankList[sType]) ;
	}
};


