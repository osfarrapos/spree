#include "../stdafx.h"

#include "..\MAPManager\MAPManager.h"

#include "RecallFriend.h"



extern _j_AccMAPManager *	g_pAccMapManager;		// Account ID KEY MAP
//CUser* pUser = g_pAccMapManager->Find_User(pMsgSvEnterGame->cAccount);

void RF_RequestRecall( CMSG_RF_REQUEST_RECALL * pRecall, _h_DS_Com * pFromCom )
{
	PrintConsole( "[RF] Request F(%s)->T(%s) : ", pRecall->cFromName, pRecall->cTargetName );
	// 요청이므로, 타겟을 찾아 타겟에게 보낸다.
	PCHARAC pTarget = g_pAccMapManager->Find_Charac( pRecall->cTargetName );
	if( pTarget )
	{
		sendGS( pTarget->CHARAC_CUR_BASIC.sZone, (char*)pRecall, pRecall->usLength );
		PrintConsole( "Ok\n" );
	}
	else
	{
		CMSG_RF_FAIL pkFail(pRecall->ucRFMode);
		pkFail.set_from_name( pRecall->cFromName );
		pkFail.set_target_name( pRecall->cTargetName );

		pkFail.ucFailReason = en_rf_fail_no_target;

		pFromCom->Send( pkFail.usLength, (char*)&pkFail );

		PrintConsole( "Yes\n" );
	}
}


void RF_Answer( CMSG_RF_ANSWER * pAnswer )
{
	PrintConsole( "[RF] Answer F(%s)->T(%s) : ", pAnswer->cFromName, pAnswer->cTargetName );
	// 응답이므로, from 을 찾아 보낸다.
	PCHARAC pFrom = g_pAccMapManager->Find_Charac( pAnswer->cFromName );
	if( pFrom )
	{
		sendGS( pFrom->CHARAC_CUR_BASIC.sZone, (char*)pAnswer, pAnswer->usLength );
		PrintConsole( "Ok\n" );
	}
	else
	{
		PrintConsole( "No\n" );
	}
}



void RF_ForceMove( CMSG_RF_FORCE_MOVE * pMove )
{
	PrintConsole( "[RF] Force Mode F(%s)->T(%s) : ", pMove->cFromName, pMove->cTargetName );
	if( 0 == pMove->ucRFMode )	// 친구소환
	{
		PCHARAC pTarget = g_pAccMapManager->Find_Charac( pMove->cTargetName );
		if( pTarget )
		{
			sendGS( pTarget->CHARAC_CUR_BASIC.sZone, (char*)pMove, pMove->usLength );
			PrintConsole( "Ok\n" );
		}
		else
		{
			PrintConsole( "No\n" );
		}
	}
	else
	{
		PCHARAC pFrom = g_pAccMapManager->Find_Charac( pMove->cFromName );
		if( pFrom )
		{
			sendGS( pFrom->CHARAC_CUR_BASIC.sZone, (char*)pMove, pMove->usLength );
			PrintConsole( "Ok\n" );
		}
		else
		{
			PrintConsole( "No\n" );
		}
	}
}


void RF_Fail( CMSG_RF_FAIL * pFail )
{
	PrintConsole( "[RF] Fail F(%s)->T(%s) : ", pFail->cFromName, pFail->cTargetName );
	PCHARAC pFrom = g_pAccMapManager->Find_Charac( pFail->cFromName );
	if( pFrom )
	{
		sendGS( pFrom->CHARAC_CUR_BASIC.sZone, (char*)pFail, pFail->usLength );
		PrintConsole( "Ok\n" );
	}
	else
	{
		PrintConsole( "No\n" );
	}
}
