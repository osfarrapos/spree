#pragma once
#include <string>
#include <set>

using namespace std;


/*
ip 필터링 
*/
class CIpFilter
{
private:
	// 필터링할 아이피
	set<string>	m_IpFilterList ;	

public:

	CIpFilter( const char * fileName /*ip list file*/)
	{
		if( IpListFileRead(fileName) == false )
		{
			throw ;
		}		
	}
	~CIpFilter()
	{
	}
	
	// 필터링할 아이피 리스트를 읽는다.
	bool IpListFileRead( const char * cFileName ) ;

	// 필터링할 아이피를 추가한다.
	void InsertIp( char * ip )
	{
		//m_IpFilterList[ip] = 0 ;
		m_IpFilterList.insert(ip) ;
	}

	// 필터링할 아이피인지 검사한다.
	bool IsFilterIP( char * ip )
	{
		try{
			set<string>::iterator itr = m_IpFilterList.find(ip) ;
			if( itr == m_IpFilterList.end() ) 
				return false ;
			else 
				return true ;
		}
		catch (...) {
		}
		return false ;
	}
};


