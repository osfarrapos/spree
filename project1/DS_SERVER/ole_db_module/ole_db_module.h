#pragma once

#include <MY_OLE_DB.H>
#include "Accessors.h"

#ifdef ITEMSHOP_PATCH_200607
typedef CDB_Command<C_prTestAccessor> 						C_prTest;
typedef CDB_Command<CTestAccessor2> 						C_Test2;
#endif

/*
#ifdef _CIS_20070326_
typedef CDB_Command<C_prGetCashItemListAccessor> 			C_prGetCashItemList;
#endif
*/

// Account DB side
typedef CDB_Command<C_prDS_LoginAccessor > 					C_prDS_Login ;
typedef CDB_Command<C_prDS_NexonLoginAccessor > 			C_prDS_NexonLogin ;
typedef CDB_Command<C_prDS_GetAccountUIDAccessor>			C_prDS_GetAccountUID ;
typedef CDB_Command<C_prDS_LogoutAccessor > 				C_prDS_Logout ;
typedef CDB_Command<C_prDS_EnterCheckAccessor > 			C_prDS_EnterCheck ;
typedef CDB_Command<C_prDS_NexonEnterCheckAccessor > 		C_prDS_NexonEnterCheck ;

typedef CDB_Command<C_prDS_PW_CHECKAccessor > 				C_prDS_PW_CHECK ;
typedef CDB_Command<C_prDS_All_LogoutAccessor > 			C_prDS_All_Logout;	

// Character DB side
typedef CDB_Command<C_prDS_CreateCharacAccessor >			C_prDS_CreateCharac ;
typedef CDB_Command<C_prDS_DeleteAccessor >					C_prDS_Delete ;

typedef CDB_Command<C_prDs_UpdateLevelAccessor >			C_prDS_UpdateLevel ;
typedef CDB_Command<C_pr_prDS_GetCharacLevelAccessor >		C_prDs_GetCharacLevel ;
#ifdef _PD_LEVEL_RATE_SAVE_
typedef CDB_Command<C_prDs_UpdateCharacState_v2Accessor >		C_prDs_UpdateCharacState ;
typedef CDB_Command<C_prDh_GetCharacState_v2Accessor >			C_prDh_GetCharacState ;
#else
typedef CDB_Command<C_prDs_UpdateCharacStateAccessor >		C_prDs_UpdateCharacState ;
typedef CDB_Command<C_prDh_GetCharacStateAccessor >			C_prDh_GetCharacState ;
#endif

typedef CDB_Command<C_prDS_UpdateCurBasicInfoAccessor >		C_prDS_UpdateCurBasicInfo ;

typedef CDB_Command<C_prDS_GetCurBasicInfoAccessor >		C_prDS_GetCurBasicInfo ;
typedef CDB_Command<C_prDS_CountCharacAccessor >			C_prDS_CountCharac ;
typedef CDB_Command<C_prDS_GetCharacInfoAccessor >			C_prDS_GetCharacInfo ;
typedef CDB_Command<C_prDs_GetInvenAccessor >				C_prDs_GetInven ;
typedef CDB_Command<C_prDs_UpdateInvenAccessor >			C_prDs_UpdateInven ;

//typedef CDB_Command<C_prDs_UpdateInvenAccessor_v3 >			C_prDs_UpdateInven_v3 ;

typedef CDB_Command<C_prDS_GetQuestAccessor >				C_prDS_GetQuest	;
typedef CDB_Command<C_prDS_UpdateQuestAccessor >			C_prDS_UpdateQuest ;
typedef CDB_Command<C_prDs_GetDepotAccessor >				C_prDs_GetDepot ;
typedef CDB_Command<C_prDs_UpdateDepotAccessor >			C_prDs_UpdateDepot ;

typedef CDB_Command<C_prDS_UpdateCharacAccessor >			C_prDS_UpdateCharac ;
typedef CDB_Command<C_prDS_GetMartialAccessor >				C_prDS_GetMartial ;
typedef CDB_Command<C_prDs_UpdateMartialAccessor >			C_prDs_UpdateMartial ;
typedef CDB_Command<C_prDS_SelectCharacAccessor >			C_prDS_SelectCharac	;

typedef CDB_Command<C_prDS_CharacRenameAccessor >			C_prCharacRename ;
typedef CDB_Command<C_prDs_GetAccAccessor >					C_prDs_GetAcc ;

typedef CDB_Command<C_prDS_GetLastServerAccessor >			C_prDS_GetLastServer ;
typedef CDB_Command<C_prDS_SetLastServerAccessor >			C_prDS_SetLastServer ;

typedef CDB_Command<C_prDS_GetNicAccessor >					C_prDS_GetNic ;
typedef CDB_Command<C_prDS_SetNicAccessor >					C_prDS_SetNic ;

typedef CDB_Command<C_prDS_GetMatchAccessor >				C_prDS_GetMatch ;
typedef CDB_Command<C_prDS_SetMatchAccessor >				C_prDS_SetMatch ;

typedef CDB_Command<C_prDS_GetEffectAccessor >				C_prDS_GetEffect ;
typedef CDB_Command<C_prDS_SetEffectAccessor >				C_prDS_SetEffect ;

#ifdef _PD_BUFF_COUNT_MODIFY_
typedef CDB_Command<C_prDS_GetEffect_V2Accessor >			C_prDS_GetEffect_V2 ;
typedef CDB_Command<C_prDS_SetEffect_V2Accessor >			C_prDS_SetEffect_V2 ;
#endif

typedef CDB_Command<C_prDS_CheckAndChangeNameAccessor>		C_prDS_CheckAndChangeName ;

typedef CDB_Command<C_prDS_GetPKRecordAccessor>				C_prDS_GetPkRecord ;
typedef CDB_Command<C_prDS_UpdatePKRecordAccessor>			C_prDS_UpdatePkRecord ;

typedef CDB_Command<C_prDs_GetBackupDataAccessor >			C_prDs_GetBackupData ;

typedef CDB_Command<C_prDSInsertOrganizationAccessor>		C_prDSInsertOrganization ;
typedef CDB_Command<C_prDSUpdateOrganizationAccessor>		C_prDSUpdateOrganization ;
typedef CDB_Command<C_prDSDeleteOrganizationAccessor>		C_prDSDeleteOrganization ;

typedef CDB_Command<C_prDS_NewJoiner_CHECKAccessor>			C_prDS_NewJoiner_CHECK ;
typedef CDB_Command<C_pr_prCharacLogoutAccessor>			C_pr_prCharacLogout ;
typedef CDB_Command<C_prLS_GetIsPcbangAccessor>				C_prLS_GetIsPcbang ;
typedef CDB_Command<_prEvent_CheckAndGiveAccessor>			C_prEvent_CheckAndGive;
typedef CDB_Command<_prEvent_PreCheck>						C_prEvent_PreCheck;			// 이벤트 진행 여부만 확인한다.
typedef CDB_Command<_prEvent_CancelAccessor>				C_prEvent_Cancel;
typedef CDB_Command<_prEvent_SuccessAccessor>				C_prEvent_Success;



typedef CDB_Command<C_prSetForestStateFlagAccessor>			C_prSetForestStateFlag ;
typedef CDB_Command<C_prGetForestStateFlagAccessor>			C_prGetForestStateFlag ;

typedef CDB_Command<C_prDSGetCountLimitAccessor>			C_prDSGetCountLimit ;
typedef CDB_Command<C_prDSSetCountLimitAccessor>			C_prDSSetCountLimit ;
typedef CDB_Command<_db_pr_DSU_GetUnionState>				C_prDSUGetUnionState ;
typedef CDB_Command<_db_pr_DSU_UpdateUnionState_One>		C_prDSUUpdateUnionStateOne ;
typedef CDB_Command<_db_pr_DSU_UpdateUnionState_All>		C_prDSUUpdateUnionStateAll ;
typedef CDB_Command<_db_pr_DSU_InsertUnionLog>				C_prDSUInsertUnionLog ;


// 쿼리타입 엑세서..
typedef CDB_Command<CRankSelectAccessor >					C_RankSelect ;
typedef CDB_Command<CRank_LevelAccessor >					C_Rank_Level ;
typedef CDB_Command<COrSelectAccessor >						C_ORSelect ;
#ifdef _PD_ALLY_ORG_STEP_1_
typedef CDB_Command<CAllyReadAccessor>						C_AllyRead;
typedef CDB_Command<CAllyCreateAccessor>					C_AllyCreate;
typedef CDB_Command<CAllyUpdateAccessor>					C_AllyUpdate;
typedef CDB_Command<CAllyBreakupAccessor>					C_AllyBreakup;
typedef CDB_Command<CAllyUpdateOfOrgAccessor>				C_AllyUpdateOfOrg;
#ifdef _PD_CASTLE_STEP_1_
typedef CDB_Command<CAllyGetFirstOrgAccessor>				C_AllyGetFirstOrg;
#endif
#endif
typedef CDB_Command<CInvenAccessor >						C_InvenSelect ;
typedef CDB_Command<CDepotAccessor>							C_DepotSelect ;
typedef CDB_Command<C_prReflushedCheckAccessor>				C_prReflushedCheck ;

typedef CDB_Command<_db_pr_DS_GetCharacItemUseStatus>		C_prDSGetCharacItemUseStatus ;
typedef CDB_Command<_db_pr_DS_UpdateCharacItemUseStatus>	C_prDSUpdateCharacItemUseStatus ;
typedef CDB_Command<_db_pr_DS_GetAccountItemUseStatus>		C_prDSGetAccountItemUseStatus ;
typedef CDB_Command<_db_pr_DS_UpdateAccountItemUseStatus>	C_prDSUpdateAccountItemUseStatus ;

typedef CDB_Command<_db_pr_DS_GetQuickSlotAccessor>			C_prDS_GetQuickSlot ;
typedef CDB_Command<_db_pr_DS_UpdateQuickSlotAccessor>		C_prDS_UpdateQuickSlot ;

#ifdef _PD_CHARAC_LTS_
typedef CDB_Command<_db_pr_DS_Get_LTSAccessor>				C_prDS_GetLts;
typedef CDB_Command<_db_pr_DS_Set_LTSAccessor>				C_prDS_SetLts;
typedef CDB_Command<_db_pr_DS_GetCUIDAccessor>				C_prDS_GetCUID;


#endif

#ifdef _VINA_SHUTDOWN_20061023_
typedef CDB_Command<_db_pr_DS_GetFatigueInfoAccessor>			C_prDS_GetFatigueInfo ;
typedef CDB_Command<_db_pr_DS_UpdateFatigueInfoAccessor>		C_prDS_UpdateFatigueInfo ;
#endif

#ifdef _PD_CASTLE_STEP_1_
	typedef CDB_Command<_cSelectDefendCastleAccessor>					C_prDSGetDefendCastle;
	typedef CDB_Command<_cSelectAttackCastleAccessor>					C_prDSGetAttackCastle;
	typedef CDB_Command<_cSelectManorCombatAccessor>					C_prDSGetManorCombat;
	typedef CDB_Command<_cSelectDefendDespotAccessor>					C_prDSGetDefendDespot;
	typedef CDB_Command<_cUpdateDefendCastleAccessor>					C_prDSSetDefendCastle;
	typedef CDB_Command<_cUpdateAttackCastleAccessor>					C_prDSUpdateAttackCastle;
	typedef CDB_Command<_cUpdateManorCombatAccessor>					C_prDSSetManorCombat;
	typedef CDB_Command<_cCastleInsertAccessor>							C_prDSCastleInsert;
#endif

#ifdef _PD_CASTLE_STEP_2_
	typedef CDB_Command<_cSelectAllCombatRecord>						C_prDSGetCombatRecord;
	typedef CDB_Command<_cSelectCombatRecordInManor>					C_prDSGetRecordInManor;
	typedef CDB_Command<_cORgStandingsInsertAccessor>					C_prDSInsertOrgStandings;	//타단과의 전적로그 인서트
	typedef CDB_Command<_cUpdateOrgStandingsAccessor>					C_prDSUpdateOrgStandings_bi;//타단과의 전적 업데이트 또는 인서트
	typedef CDB_Command<_cUpdateOrgRecordInManorAccessor>				C_prDSUpdateOrgRecordInManor;//장원에서의 전적
#endif //_PD_CASTLE_STEP_2_
typedef CDB_Command<_db_pr_DS_AddPlayTime>					C_prDS_AddPlayTime;

#ifdef _PD_GAME_EVENT_
typedef CDB_Command<_db_pr_DS_FMSelectWinnerInfo>			C_prDS_FM_SelectWinnerInfo;
typedef CDB_Command<_db_pr_DS_FMUpdateWinner>				C_prDS_FM_UpdateWinner;
#endif


#ifdef _PD_LEVEL_PRESENT_
typedef CDB_Command<_db_pr_DS_LevelPresent_GetNotRecv>			C_prDS_LevelPresent_GetNotRecv;
typedef CDB_Command<_db_pr_DS_LevelPresent_Check>				C_prDS_LevelPresent_Check;
typedef CDB_Command<_db_pr_DS_LevelPresent_Recv>				C_prDS_LevelPresent_Recv;
typedef CDB_Command<_db_pr_DS_LevelPresent_Cancel>				C_prDS_LevelPresent_Cancel;
#endif	// _PD_LEVEL_PRESENT_

#ifdef _PD_PKEVENT_BOX_SPAWN_
typedef CDB_Command<_db_prDS_GetBWWarWinTime>					C_prDS_GetBWWarWinTime;
typedef CDB_Command<_db_prDS_UpdateBWWarWinTime>				C_prDS_UpdateBWWarWinTime;
#endif

#ifdef _PD_EVENT_INVITE_FRIEND_
typedef CDB_Command<_db_prDS_GetInviteFriendState>				C_prDS_GetInviteFriendState;
typedef CDB_Command<_db_prDS_UpdateInviteFriendState>			C_prDS_UpdateInviteFriendState;
#endif _PD_EVENT_INVITE_FRIEND_