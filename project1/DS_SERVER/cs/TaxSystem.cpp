
#include "../stdafx.h"
#include "TaxSystem.h"
#include "Castle.h"
#include "..\Global\Global_ServerInfo.h"
extern _j_ServerInfo * g_pServerInfo ;

#ifdef _PD_CASTLE_STEP_2_

int CManorTaxManager::InsertTaxInfo(CManorTaxInfo* pManorTaxInfo)
{
	lock();
	try
	{
		//insert map
		m_mapManorTaxInfo.insert( map<short, CManorTaxInfo*>::value_type( pManorTaxInfo->GetMapID(), pManorTaxInfo ) );
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] TAX SYSTEM InsertTaxInfo \n");
	}
	unlock();
	return true;
}

int CManorTaxManager::EraseTaxInfo(short sMapID)
{
	lock();
	try
	{
		//erase map
		m_mapManorTaxInfo.erase(sMapID);
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] TAX SYSTEM EraseTaxInfo \n");
	}
	unlock();
	return true;
}

CManorTaxInfo* CManorTaxManager::SearchTaxInfo(short sMapID)
{
	CManorTaxInfo * pManor = NULL;
	lock();	
	try
	{
		map<short, CManorTaxInfo*>::iterator itr = m_mapManorTaxInfo.find(sMapID);
		if(itr != m_mapManorTaxInfo.end())
			pManor = itr->second;
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] Search TAX SYSTEM \n");
	}
	unlock();
	return pManor;
}

void CManorTaxManager::send_cmsg_manor_tax_info_req_ds_gs()
{
	if( CDSCastleManager::GetObj().is_combat_ing() )
	{
		//::PrintConsole("[TAX_SYSEM] send_cmsg_manor_tax_info_req_ds_gs : combat is in progress \n");
		return;
	}

	//모든 맵에 세금 정보를 요청한다.
	CMSG_MANOR_TAX_INFO_REQ_DS_GS sendPkt;

	g_pServerInfo->send_to_all_gs( &sendPkt, sendPkt.usLength );

	//::PrintConsole("[TAX_SYSEM] send_cmsg_manor_tax_info_req_ds_gs \n");
}

void CManorTaxManager::proc_cmsg_manor_tax_info_gs_ds(CMSG_MANOR_TAX_INFO_GS_DS * pPkt)
{
	try
	{
		CManorTaxInfo* pManorTaxInfo = SearchTaxInfo(static_cast<short>(pPkt->ucMapID));
		if(pManorTaxInfo == NULL)
		{
			CManorTaxInfo * pManorTaxInfo = new CManorTaxInfo;
			pManorTaxInfo->SetMapID(static_cast<short>(pPkt->ucMapID));
			pManorTaxInfo->SetRegionOrGlobal(pPkt->ucRegionOrGlobal);
			pManorTaxInfo->SetManorLevelRate(pPkt->ucManorLevelRate);
			pManorTaxInfo->SetCommonLevelRate(pPkt->ucCommonLevelRate);
			pManorTaxInfo->SetTransferTax(pPkt->uiTotalMoney);
			InsertTaxInfo(pManorTaxInfo);

			CManorTaxInfo* pManor = SearchTaxInfo(static_cast<short>(pPkt->ucMapID));

			/*
			::PrintConsole("[TAX_SYSEM] Insert CMSG_MANOR_TAX_INFO_GS_DS Map<%d>, CommonOrManor<%d>, Rate<Manor=%d,Common=%d>, Money<%d>, TotalMoney<%d> \n" \
				, pManor->GetMapID() \
				, pManor->GetRegionOrGlobal() \
				, pManor->GetManorLevelRate() \
				, pManor->GetCommonLevelRate() \
				, pPkt->uiTotalMoney \
				, pManor->GetTransferTax()) ;
			*/
		}
		else
		{
			lock();
			pManorTaxInfo->SetRegionOrGlobal(pPkt->ucRegionOrGlobal);
			pManorTaxInfo->SetRegionOrGlobal(pPkt->ucRegionOrGlobal);
			pManorTaxInfo->SetManorLevelRate(pPkt->ucManorLevelRate);
			pManorTaxInfo->SetCommonLevelRate(pPkt->ucCommonLevelRate);
			pManorTaxInfo->PlusTransferTax(pPkt->uiTotalMoney);

			/*
			::PrintConsole("[TAX_SYSEM] Edit Map<%d>, CommonOrManor<%d>, Rate<Manor=%d,Common=%d>, Money<%d>, TotalMoney<%d> \n" \
				, pManorTaxInfo->GetMapID() \
				, pManorTaxInfo->GetRegionOrGlobal() \
				, pManorTaxInfo->GetManorLevelRate() \
				, pManorTaxInfo->GetCommonLevelRate() \
				, pPkt->uiTotalMoney \
				, pManorTaxInfo->GetTransferTax()) ;
			*/
			unlock();
		}
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] TAX SYSTEM proc_cmsg_manor_tax_info_gs_ds \n");
	}
}

void CManorTaxManager::proc_cmsg_manor_tax_req_total_gs_ds(CMSG_MANOR_TAX_REQ_TOTAL_GS_DS * pkt)
{
	if( CDSCastleManager::GetObj().is_combat_ing() )
	{
		::PrintConsole("[TAX_SYSEM] proc_cmsg_manor_tax_req_total_gs_ds : combat is in progress \n");
		return;
	}

	//Castle에 모든 장원 세금 저장
	CollectManorTax();

	//send notice totalmoney ds gs
}

void CManorTaxManager::proc_cmsg_tax_req_take( CMSG_MANOR_TAX_REQ_TAKE * pReqTake, _h_DS_Com * pCom )
{
	try
	{
		CMSG_MANOR_TAX_ANSWER_TAKE pkAnswer;
		CopyMemory( pkAnswer.cCharacName, pReqTake->cCharacName, en_charac_name_length );
		pkAnswer.cCharacName[en_charac_name_length] = 0;
		pkAnswer.ucMapID = pReqTake->ucMapID;
		pkAnswer.uiTakeMoeny = pReqTake->uiRequestMoney;
		pkAnswer.uiLeftPartMoney = 0;

		CDSCastleInfo * pCastleInfo = CDSCastleManager::GetObj().get_castle_info( static_cast<short>(pReqTake->ucMapID) );
		if( pCastleInfo )
		{
			pCastleInfo->tax_request_take( pReqTake->uiRequestMoney, &pkAnswer, CDSCastleManager::GetObj().get_game_db_con() );
		}
		else
		{
			pkAnswer.ucResult = 3;
		}

		PrintConsole( "[TAX_SYSTEM] Take : Request %u, Result %d, Money %u\n", pReqTake->uiRequestMoney, pkAnswer.ucResult, pkAnswer.uiLeftPartMoney );

		pCom->Send( pkAnswer.usLength, (char*)&pkAnswer );

		if( 0 == pkAnswer.ucResult )
		{
			// 다른 존들에 모두 알린다.
			CMSG_MANOR_TAX_SET_TOTAL_MONEY pkSetTotalMoney;
			pkSetTotalMoney.ucMapID = pkAnswer.ucMapID;
			pkSetTotalMoney.uiTotalMoney = 0;
			pkSetTotalMoney.uiPartMoney = pkAnswer.uiLeftPartMoney;
			
			pkSetTotalMoney.ucType = 2;

			g_pServerInfo->send_to_all_gs( &pkSetTotalMoney, pkSetTotalMoney.usLength );
		}
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] TAX SYSTEM proc_cmsg_tax_req_take \n");
	}
}

//save castle 10 초후
void CManorTaxManager::CollectManorTax()
{
	lock();
	try
	{
		map<short, CManorTaxInfo*>::iterator  common_tax_itr = m_mapManorTaxInfo.begin();
		map<short, CManorTaxInfo*>::iterator  common_tax_end = m_mapManorTaxInfo.end();

		u_int uiCommonTax = 0, uiManorTax = 0, uiCommonTotalMoney = 0;

		//각 공통 세율
		for(; common_tax_itr != common_tax_end; ++common_tax_itr)
		{
			if(en_global_tax == common_tax_itr->second->GetRegionOrGlobal())
			{	
				
				uiCommonTotalMoney += common_tax_itr->second->GetTransferTax();

				/*
				::PrintConsole("[TAX_SYSEM] Collect Common Map<%d>, CommonOrManor<%d>, Rate<Manor=%d,Common=%d>, Tax<%d>, en_global_tax<%d> \n" \
					, common_tax_itr->second->GetMapID()	\
					, common_tax_itr->second->GetRegionOrGlobal()	\
					, common_tax_itr->second->GetManorLevelRate()	\
					, common_tax_itr->second->GetCommonLevelRate()	\
					, uiCommonTotalMoney	\
					, en_global_tax) ;
				*/

				//초기화
				common_tax_itr->second->SetTransferTax(0);
			}
		}

		map<short, CManorTaxInfo*>::iterator  manor_tax_itr = m_mapManorTaxInfo.begin();
		map<short, CManorTaxInfo*>::iterator  manor_tax_end = m_mapManorTaxInfo.end();
		for(; manor_tax_itr != manor_tax_end; ++manor_tax_itr)
		{
			//총 공통 세율 계산 + 장원 세율 계산
			if(en_region_tax == manor_tax_itr->second->GetRegionOrGlobal())
			{
				//세율 계산
				uiManorTax = static_cast<u_int>( ((float)( manor_tax_itr->second->GetTransferTax() ) * (float)(manor_tax_itr->second->GetManorLevelRate())/100.0f) );	//<== 세금으로 걷는 돈.
				uiCommonTax = static_cast<u_int>( (float)uiCommonTotalMoney * (float)(manor_tax_itr->second->GetCommonLevelRate())/100.0f );

				//초기화
				manor_tax_itr->second->SetTransferTax(0);

				//save Catle
				CDSCastleManager::GetObj().tax_add_from_tax_system( manor_tax_itr->second->GetMapID() , uiManorTax + uiCommonTax, uiCommonTotalMoney + manor_tax_itr->second->GetTransferTax());

				/*
				::PrintConsole("[TAX_SYSEM] Collect Total Map<%d>, CommonOrManor<%d>, Rate<Manor=%d,Common=%d>, totalTax<%d>, en_region_tax<%d> \n" \
					, manor_tax_itr->second->GetMapID() \
					, manor_tax_itr->second->GetRegionOrGlobal() \
					, manor_tax_itr->second->GetManorLevelRate() \
					, manor_tax_itr->second->GetCommonLevelRate() \
					, uiManorTax + uiCommonTax \
					, en_region_tax) ;
				*/
			}
		}	
	}
	catch(...)
	{
		::PrintConsole("[EXCEPTION] TAX SYSTEM proc_cmsg_tax_req_take \n");
	}
	unlock();
}

#endif

