

#pragma once

#ifndef KIKI20070125_MANOR_TAX_SYSTEM
#define KIKI20070125_MANOR_TAX_SYSTEM
#pragma warning(disable:4786) //6.0에만 발생하는 문제 .net에는 발생하지 않는다.
//#include <afx.h>
/*
syntax error : missing ';' before identifier 'm_hEvent'
#include <afx.h>
Setting->Debug MultyThread DLL
General->Use MFC in a Share DLL

*/
#include <map>
using namespace std;
//#include "../Global/ckSingleton.h"

#ifdef _PD_CASTLE_STEP_2_
//==============================
// TransferTax Info 거래세 정보
//==============================
enum EN_MAP_REGION_OR_GLOBAL_TAX_TYPE 
{
	en_default_tax = 0,
	en_global_tax = 1,	//공통 맵
	en_region_tax= 2,	//장원 맵
};

class CManorTaxInfo 
{
protected:
	short	m_sMapID;				// 맵 번호
	u_char	m_ucRegionOrGlobal;		// 지역인지 공통인지 구분
	u_char	m_ucManorLevelRate;		// 장원 맵 세율
	u_char	m_ucCommonLevelRate;	// 공통 맵 세율
	u_int	m_uiTransferTax;		// 거래세(수입)

public:
	CManorTaxInfo():m_sMapID(0), m_ucRegionOrGlobal(0), m_ucManorLevelRate(0), m_ucCommonLevelRate(0), m_uiTransferTax(0){}
	~CManorTaxInfo(){}

	inline void SetMapID(int sMapID)										{ m_sMapID = sMapID; }
	inline void SetTransferTax(unsigned int uiTransferTax)						{ m_uiTransferTax = uiTransferTax; }
	inline void SetManorLevelRate(u_char ucManorLevelRate)						{ m_ucManorLevelRate = ucManorLevelRate; }
	inline void SetCommonLevelRate(u_char ucCommonLevelRate)					{ m_ucCommonLevelRate = ucCommonLevelRate; }
	inline void SetRegionOrGlobal(unsigned char ucRegionOrGlobal)				{ m_ucRegionOrGlobal = ucRegionOrGlobal; }

	inline void PlusTransferTax(unsigned int uiTransferTax)						
	{
		if( MAX_CASTLE_MONEY - m_uiTransferTax >= uiTransferTax )
			m_uiTransferTax += uiTransferTax;
		else
			m_uiTransferTax = MAX_CASTLE_MONEY;
	}
	inline void MinusTransferTax(unsigned int uiTransferTax)
	{
		if( m_uiTransferTax >= uiTransferTax )
			m_uiTransferTax -= uiTransferTax; 
		else
			m_uiTransferTax = 0;
	}

	inline int GetMapID()										{ return m_sMapID; }
	inline unsigned int GetTransferTax()						{ return m_uiTransferTax; }
	inline u_char	GetManorLevelRate()							{ return m_ucManorLevelRate; }
	inline u_char	GetCommonLevelRate()						{ return m_ucCommonLevelRate; }
	inline unsigned char GetRegionOrGlobal()					{ return m_ucRegionOrGlobal; }

};

//==============================
// 장원전 세금 시스템 관리
//==============================
class CManorTaxManager : public ckSingleton<CManorTaxManager>
{
protected:
	CRITICAL_SECTION			m_cs ;
	map<short, CManorTaxInfo*> m_mapManorTaxInfo;

public:
	CManorTaxManager()
	{
		InitializeCriticalSectionAndSpinCount( &m_cs, (0x80000000)|2000 );
	}
	~CManorTaxManager()
	{
		map<short, CManorTaxInfo*>::iterator  manor_tax_itr = m_mapManorTaxInfo.begin();
		map<short, CManorTaxInfo*>::iterator  manor_tax_end = m_mapManorTaxInfo.end();

		for(; manor_tax_itr != manor_tax_end; ++manor_tax_itr)
		{
			delete manor_tax_itr->second;
		}
		m_mapManorTaxInfo.clear();
	}

	int InsertTaxInfo(CManorTaxInfo* pManorTaxInfo);
	int EraseTaxInfo(short sMapID);
	CManorTaxInfo* SearchTaxInfo(short sMapID);

	//save castle : totalMoney 집계요청이 왔을 때, ds가 꺼질때,
	void CollectManorTax();

	//send ds_gs
	void send_cmsg_manor_tax_info_req_ds_gs();

	//proc gs_ds
	void proc_cmsg_manor_tax_info_gs_ds(CMSG_MANOR_TAX_INFO_GS_DS * pkt);
	void proc_cmsg_manor_tax_req_total_gs_ds(CMSG_MANOR_TAX_REQ_TOTAL_GS_DS * pkt);
	//void proc_cmsg_manor_tax_req_start_compute_gs_ds(CMSG_MANOR_TAX_REQ_START_COMPUTE_GS_DS * pkt);
	void proc_cmsg_tax_req_take( CMSG_MANOR_TAX_REQ_TAKE * pReqTake, _h_DS_Com * pCom );

public:
	void lock()		
	{ 
		EnterCriticalSection( &m_cs ) ;
	}
	void unlock()
	{ 
		LeaveCriticalSection( &m_cs ) ; 
	}

	/*inline void ResetEventTax()
	{
		ResetEvent(m_hEvent);
	}
	inline void SetEventTax()
	{
		SetEvent(m_hEvent);
	}*/
};


#endif  //_PD_CASTLE_STEP_2_

#endif	//#ifndef KIKI20070125_MANOR_TAX_SYSTEM