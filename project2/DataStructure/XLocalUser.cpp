//////////////////////////////////////////////////////////////////////
// XLocalUser.h
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMob.h"
#include "XUser.h"
#include "SoundEffectList.h"
#include "XProc_MainGameDef.h"
#include "XWindow_Skill.h"
#include "XPathFinder.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_ChargeGauge.h"
#include "XWindow_Warehouse.h"
#include "XWindow_JinStatus.h"
#include "XSR_STRINGHEADER.H"
#include "XParticleIDDefine.h"
#include "XWindow_Rebirth.h"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"
#include "XWindow_MonsterStatus.h"
#include "XWindow_TrainingMed.h"
#include "XWindow_TrainingDummy.h"
#include "XWindow_TrainingSpell.h"
#include "Xwindow_TrainingComplete.h"
#include "xwindow_repairitem.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_MainMenu.h"
#include "Xwindow_MatchNoticeLarge.h"
#include "Xwindow_NickName.h"
#include "XWindow_Escape.h"

#include "XWindow_PersonalStore.h"
#include "XWindow_MnDGroup.h" // Author : 양희왕 //breif : 사제관련 탭

#include "XParticleAction.h"

// by mahwang
#include "XLifeEventManager.h"

//#define _XDEF_RENDERNUMBEREFFECT
FLOAT				g_fWaterDepth = 0.28f; // 수심 몇미터까지 들어갈수 있다.
_XMOTION_MOVETYPE	g_PrevMoveType = _XMOVETYPE_RUN;
_XCHARACTER_MOTION	g_EmotionActionTable[16] = 
{
	_XAN_EMOTION_BOW,						// 60 가벼운 인사
	_XAN_EMOTION_BOWPOLITELY,				// 61 정중한 인사	
	_XAN_EMOTION_EXCITEMENT,				// 62 도발
	_XAN_EMOTION_RUSH,						// 63 돌격
		
	_XAN_EMOTION_CRINGESTART,				// 64 무릅꿇기 시작
		
	_XAN_EMOTION_CHEER,						// 67 환호
	_XAN_EMOTION_BREAKDOWN,					// 68 좌절
	_XAN_EMOTION_CRY,						// 69 울기
	_XAN_EMOTION_LAUGH,						// 70 웃음	
	_XAN_EMOTION_ANGRY,						// 71 화냄
	_XAN_EMOTION_DOZ,						// 72 졸기

	// Emotion에서 제외 - 운기조식 수련에서 사용
//	_XAN_EMOTION_SITDOWNSTART,				// 73 앉기 시작
	_XAN_EMOTION_MARRYCLAPPINGHAND,			// 76 박수	
	_XAN_EMOTION_MARRYBOX,					// 77 인사
	_XAN_EMOTION_MARRYBOWPOLITELY,			// 78 큰절	
	
	_XAN_EMOTION_CLAPPINGHAND,				// 79 박수
};

extern void ChangeModel(DWORD lparam, char firstType, char facemaskmode = 0 );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
_XLocalUser::_XLocalUser()
{
#if defined(_XDEF_ITEMMALL_AUTOLOGIN_20061120) || defined(_XDEF_ITEMMALL_AUTOLOGIN_SE_20070514)
	memset( m_ItemMallWebLoginID, 0, sizeof(TCHAR) * 33 );
#endif
	memset( m_UserName, 0, sizeof(TCHAR) * 33 );
	memset( m_UserPW, 0, sizeof(TCHAR) * 33 );

	m_PickItem = NULL;
	m_LastInsertedMoveTargetEffectPosition = D3DXVECTOR3( 9999.0f, 9999.0f, 9999.0f );
	m_NextRespawnPosition.x =   0;
	m_NextRespawnPosition.y =   0;
	m_NextRespawnPosition.z =   0;
	ResetLocalUserData();
}

_XLocalUser::~_XLocalUser()
{	
	//2004.06.18->oneway48 insert
	m_MiniHpBorder.DestroyWindow();
	m_MonsterMiniHpBorder.DestroyWindow();
	m_imageMiniHpBackBorder.DestroyWindow();
}

//#define _XDEF_CHECKGROUNDNORMALVECTOR

void _XLocalUser::UpdatePosition( void )
{	
	D3DXMATRIX	rotmat;
	//D3DXMatrixIdentity( &rotmat );
	D3DXMatrixRotationY( &rotmat, m_RotateAngle );
	
	/*if( m_pGoundCollideOBB )
	{
		m_Position.y = m_pGoundCollideOBB->m_kCenter.y + m_pGoundCollideOBB->m_afExtent[1];
		FLOAT terrainheight = g_LodTerrain.GetTerrainHeight( m_Position ); 
		if( terrainheight > m_Position.y ) m_Position.y = terrainheight;
		m_PrevPosition.y = m_Position.y;
	}
	else 
	{
		m_Position.y = g_LodTerrain.GetTerrainHeight( m_Position );
		m_PrevPosition.y = m_Position.y;
	}	*/

	D3DXMatrixTranslation( &m_ModelDescriptor.m_Position, m_Position.x, m_Position.y, m_Position.z );
	
#ifdef _XDEF_CHECKGROUNDNORMALVECTOR
	
	if( m_CharacterInfo.animationdescript.motionclass == _XACTION_MOVE )
	{
		D3DXMATRIX  rotmatrix;
		D3DXQUATERNION normalrotquat;
		D3DXVECTOR3	upvector = D3DXVECTOR3(0.0f,1.0f,0.0f);
		
		if( m_CurrentPositionTerrainNormal.x != 0.0f || m_CurrentPositionTerrainNormal.y != 1.0f || m_CurrentPositionTerrainNormal.z != 0.0f ) // check exactly upvector...
		{			
#ifdef _XDWDEBUG			

			if( g_SpiderMode )
			{
				normalrotquat = _XRotationArcNormalized( &upvector, &m_CurrentPositionTerrainNormal  );
			}
			else
			{
#endif	
				// rotate from terrain normal vector...					
				FLOAT theta = _XGetAngleFromVectors( &upvector, &m_CurrentPositionTerrainNormal );
				
				if( theta > 0.122173022f/* deg 7' */ )
				{
					FLOAT dev = theta / 0.122173022f; //0.0872664444f(5');
					
					D3DXVECTOR3 modvector = m_CurrentPositionTerrainNormal;
					
					modvector.x /= dev;
					modvector.z /= dev;
					
					normalrotquat = _XRotationArcNormalized( &upvector, &modvector );
				}
				else
				{
					normalrotquat = _XRotationArcNormalized( &upvector, &m_CurrentPositionTerrainNormal  );
				}			

#ifdef _XDWDEBUG	
			}
#endif
			D3DXMatrixRotationQuaternion( &rotmatrix, &normalrotquat );
			D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmatrix, &m_ModelDescriptor.m_Position );
		}				
	}
	
#endif
	
	D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmat, &m_ModelDescriptor.m_Position );

	g_UserWorldMatrix = m_ModelDescriptor.m_Position;
	
}

void _XLocalUser::UpdateRotateAngle(void)
{
	if((m_LastRotateAngle - m_RotateAngle) <= EPSILON1 && (m_LastRotateAngle - m_RotateAngle) >= -EPSILON1)
	{
		m_RotateAngle = m_LastRotateAngle;
		return;
	}

	FLOAT deg_LastRotateAngle = _X_DEG(m_LastRotateAngle);
	FLOAT deg_RotateAngle = _X_DEG(m_RotateAngle);

	FLOAT addunit = _X_RAD(30);
	if(fabs(deg_LastRotateAngle - deg_RotateAngle) <= 30.0f)
		addunit = _X_RAD(5);
	/*
	FLOAT addunit = _X_RAD(10);
	if(fabs(deg_LastRotateAngle - deg_RotateAngle) <= 10.0f)
		addunit = _X_RAD(2);
		*/

	if(m_RotateDirection == _XROTATE_CLOCKWISE)
	{
		m_RotateAngle += addunit;
		if(m_RotateAngle >= _X_PI)
			m_RotateAngle = -_X_PI;
	}
	else
	{
		m_RotateAngle -= addunit;
		if(m_RotateAngle <= -_X_PI)
			m_RotateAngle = _X_PI;
	}
}

// 죽었을때 원점으로 이동 
void _XLocalUser::MoveStartPosition( void )
{	
	m_PrevProcessTime			=	-1.0f;
	ResetAttackTarget();
	ResetViewTarget();

	m_TargetPosition			=   m_Position;

	if(g_CurrentZoneInfoIndex == 18 || g_CurrentZoneInfoIndex == 19)
		m_RotateAngle			= _X_RAD(-90);
	else
		m_RotateAngle			=	0.0f;
			
	m_StopAnimation				=	FALSE;
}

void _XLocalUser::ResetLocalUserData( void )
{
	// 처음에 들어갈 때 reset되는 data
	m_iNetworkKey				=	-1;	// hotblood 20040824
	m_sMiniGameId				=	0;	// 미니게임 종류 hotblood 20040824
	m_CharacterCount			=	0;
	m_PrevPacketSendedTime		=	0;
	m_PickItem					=	NULL;

	m_SkillCombo				=	0;
	m_SkillStarLevel			=	11;
	m_MartialStep				=	0;
	m_SelectedSkillID			=	0;
	m_ActiveSlotID				=	0;

	//Author : 양희왕
#ifdef _XDEF_SKILLDEALY_MODIFY
	m_FinallySelectedSkillID	=	0;
	m_FinallySelectedSkillItem	=	NULL;
#endif
//Last Updated : 07/03/22

	m_SelectedSkillItem			=	NULL;	
	m_SelectedWeaponItem		=   NULL;

	m_NextSkillID				=	0;
	m_bUsingSkill				=	FALSE;
	m_bCheckDelayTime			=	FALSE;

	m_bRestartActive			=	FALSE;
	m_bStartCharge				=	FALSE;
	m_bUseMouseChargeSkill		=	FALSE;
	
	m_ExperiencePercentage		=	0;
	LifePowerPercentage			=	0;	
	ForcePowerPercentage		=	0;
	ConcentrationPowerPercentage=	0;
				
	m_SpecialActionTimer		=   0;
	
	m_PrevProcessTime			=	-1.0f;
	m_PrevMoveTime				=    0;
		
	m_TargetPosition			=   m_Position;
	GetItemTileIndex(m_Position.x, m_Position.z, m_CurrentTileIndex);
	m_LastItemRequestSendTime	= 0;
	
	m_RotateAngle				=	0.0f;	
	m_FinalTargetPosition		=	D3DXVECTOR3( 0.0f,0.0f,0.0f );
	m_LeftFinalTargetLength		=   0.0f;
		
	m_StopAnimation				=	FALSE;
		
	for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i) // 2004.06.01->oneway48 modify : 15->9
	{
		memset(m_PartyInfo[i].playername, 0, sizeof(TCHAR)*13);
		m_PartyInfo[i].serverno = m_PartyInfo[i].posX = m_PartyInfo[i].posZ = 0;
		m_PartyInfo[i].innerlevel = m_PartyInfo[i].vitalpercent = m_PartyInfo[i].zenpercent = 0;
		m_PartyInfo[i].isLeader = FALSE;
	}
	
	m_TotalCountOfPartyQuestCounter = 0	;			// 파티퀘스트에서 파티 카운터의 총합을 나타냄 
	m_PartySelfIndex				= -1;			// 파티 리스트 배열에서 자기 자신의 위치를 나타내는 인덱스
	m_PartyMemberCount				= 0	;
	m_PartyMemberIndex				= -1;
	m_InParty						= FALSE;
	m_bPartyQuestProgress			= FALSE;
	m_bPartyProgress				= FALSE;
	m_IsServerMoveInParty			= FALSE;

	ResetInventoryInfo();
	ResetSkillList();
	ResetWarehouseInfo();

	m_ChargeStartTime				= 0;
	m_ChargeIterateValue			= 0;
	m_ChargeSendStartPacket			= FALSE;

	m_RoundingDelayStartTime		= 0;
	m_RoundingSendPacket			= FALSE;

	m_bChannelingSendStartPacket	= FALSE;
	m_ChannelingSendLastPacketTime	= 0;
	m_ChannelingSpellRepeatTime		= 0;
	m_RegionTargetPosX				= 0.0f;
	m_RegionTargetPosZ				= 0.0f;

	m_ChangeDelayStartTime			= 0;

	m_CurrentSelectedSkillType		= 0;
	m_CurrentBattleState			= _XUSER_STATE_NONE;
	m_StanceMotionFirst				= FALSE;
	m_LastModeChangeTime			= 0;

	m_FinishSpeed					= 0;
	m_FinishSendPacket				= FALSE;
	m_bSendFinishFirePacket			= FALSE;

	m_CastingSpellRepeatTime		= 0;
	m_CastingSendPacket				= FALSE;

	m_bLeftClickProcess				= FALSE;
	m_bRightClickProcess			= FALSE;
	
	ResetAttackTarget();
	ResetViewTarget();
	ResetSpellAttackTarget();

	m_LastRotateAngle				= 0.0f;
	m_RotateDirection				= _XROTATE_NONE;
	
	m_UseSpeedSkill					= FALSE;
	m_AttackPointNumber				= 0;
	m_CurrentAttackPoint			= 0;

	m_DyingStartTime				= 0;
	m_LastCheckedDyingStartTime		= -1;
	m_DyingTimer					= 0;

	m_bCheckFirstFrame				= FALSE;
	m_bWaitPassiveReq				= FALSE;
	
	m_LastPassiveUsefulType			= 0;
	m_LastPassiveSType				= 0;
	m_LastPassiveOnOffTime			= 0;

	memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
	memset(&m_RegionDamageResult, 0, sizeof(m_RegionDamageResult));

	m_PathNodeCount					= 0;
	m_CurrentNavigatePathNodeIndex	= 0;
	
	m_DisarmamentItem = NULL;
	m_CurrentWeaponID				= 0;

	m_SelectedTrainingMode			= _XTRAINING_NONE;
	m_CurrentTrainingState			= _XUSER_STATE_NONE;
	m_CurrentInternalUseState		= _XUSER_STATE_NONE;
	m_CurrentReadBookState			= _XUSER_STATE_NONE;
	m_bSendTrainingStartPacket		= FALSE;
	m_ServerKey						= 0;
	memset(&m_TrainingDummyInfo, 0, sizeof(_XDUMMY_INFO));
	m_TrainingSpellInfo.Reset();

	
	if(g_pJinStatus_Window)
		g_pJinStatus_Window->SetButtonStatus(FALSE);

	m_bSaveRespawnPos				= FALSE;
	memset(m_RespawnName, 0, sizeof(TCHAR)*13);
	
	m_BossRoomID	= -1;
	m_BossRoomStageNumber = 0;
	m_bIsBossBattle = FALSE;
	m_bBossBattleEndProcess = FALSE;
	m_bSuccessBossBattle	= FALSE;
	m_bExitBossBattleRoom = FALSE;
	m_bWaitBossBattle = FALSE;
	m_BossStartTime	= 0;
	m_BossTimeLimit	= 0;
	m_bVirtualMode = FALSE;

	// OLD
	m_MatchType = 0;
	m_MatchID = 0;
	m_MatchTargetID = 0;
	m_bWaitFlagInfo = FALSE;

	m_ChatMuteTime = 0;
	m_LastChatLockNoticeMinute = 0;
	
	// NEW
	ResetMatchState();
	m_MatchPRTimer		= 0; // 비무 홍보 패킷을 5초마다 보낸다.
	
	m_bGrabItem = FALSE;
	m_StateList.Release();
	memset(&m_Lock, 0, sizeof(m_Lock));
	Set_m_MoveSpeedFromCondition(0);
	m_PvpNPCLock = FALSE;

	memset(&m_P_AttackDamage, 0, sizeof(_XSU_Parameter));
	memset(&m_P_AttackRate, 0, sizeof(_XSU_Parameter));
	memset(&m_P_Defence, 0, sizeof(_XSU_Parameter));
	memset(&m_P_AvoidRate, 0, sizeof(_XSU_Parameter));
	memset(&m_M_AttackDamage, 0, sizeof(_XSU_Parameter));
	memset(&m_M_AttackRate, 0, sizeof(_XSU_Parameter));
	memset(&m_M_Defence, 0, sizeof(_XSU_Parameter));	
	memset(&m_M_AvoidRate, 0, sizeof(_XSU_Parameter));

	memset(&m_Strength, 0, sizeof(_XSU_Parameter));
	memset(&m_Zen, 0, sizeof(_XSU_Parameter));
	memset(&m_Intelligence, 0, sizeof(_XSU_Parameter));
	memset(&m_Dex, 0, sizeof(_XSU_Parameter));
	memset(&m_Constitution, 0, sizeof(_XSU_Parameter));
	
	memset(&m_MaxLifePower, 0, sizeof(_XSU_Parameter));
	memset(&m_MaxForcePower, 0, sizeof(_XSU_Parameter));
	memset(&m_MaxConcentrationPower, 0, sizeof(_XSU_Parameter));

	m_P_MinAttackDamage = 0;
	m_P_MaxAttackDamage = 0;
	m_M_MinAttackDamage = 0;
	m_M_MaxAttackDamage = 0;

	Set_m_MoveSpeedFromEffect(0);
	Set_m_AttackSpeedFromEffect(0);
	
	m_ResistMinus		= 0;
	m_ResistPlus		= 0;
	m_ResistHon			= 0;
	m_ResistSin			= 0;
	
	m_AttackDamageMinus	= 0;
	m_AttackDamagePlus	= 0;
	m_AttackDamageHon	= 0;
	m_AttackDamageSin	= 0;
	
	m_bKeyLPSProcess	= FALSE;
	m_HiperRunFailedFlag = FALSE;
	
	m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
	
	m_bWaitDyingSkip		= FALSE;
	m_bHaveStance			= FALSE;
	reserved				= FALSE;

	m_PrevRequestPacketSendedTime = 0;
	
	m_PKModeLeftTime		= 0;
	m_PKModeStartTime		= 0;

	m_PEModeLeftTime		= 0;
	m_PEModeStartTime		= 0;
	
#ifdef _XTS_MASKSYSTEM
	m_bFaceMaskMode			= FALSE;
#endif
	
#ifdef _XTS_BLOODPOINT
	m_BloodPoint			= 0;
#endif
	m_FameAttrib			= -1;	
	m_bPEMode				= FALSE;

	m_UseItemInvenNumber	= 0;
	
#ifdef _XDEF_CASHITEM_SHOUT
	ResetShout();
#endif

	m_AccountItemcType		= 0;
	m_AccountItemsID		= 0;

	ResetExpItemEndTime();
	
	m_InsideSafetyZoneIndex	= 0;
	m_SafetyZoneCheckTimer	= 0;
	
	m_bCastleBattle			= false;
	m_bSendThreatenPacket	= false;
	m_bThreatening			= false;

	m_nCastleMapId			= 0;
	m_dwRoleInCastleBattle	= 0;
	m_dwAttackType			= 0;
	m_dwResultCastleBattle	= 0;
	m_dwDeadCount			= 0;
	m_dwDieStartTime		= 0;
	m_dwDeadPenaltyTime		= 0;

	m_bOut					= FALSE;

	m_bGuardMob = false; //Author : 양희왕 //breif : 베트남 shift 클릭

	m_nNotoriousValue = 0; //Author : 양희왕 //breif : 평판 시스템 

	m_bChangingWeapon = FALSE; //Author : 양희왕 //breif : 무기를 교체 중인지 판단
	m_bChangingWeaponPacket = FALSE;
	m_nChangeWeaponModel = 0;

	memset( &m_dwPCroomInfo, 0, sizeof(PCROOM_App) );	//Author : 양희왕 //breif : PC 방

	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = m_listPKTracing.begin() ; iter_pktracing != m_listPKTracing.end() ; ++iter_pktracing)
	{
		pktracing = *iter_pktracing;
		SAFE_DELETE( pktracing );
	}
	m_listPKTracing.clear();
#endif
	
	m_bEnterMatchEvent		= FALSE;
	m_nWinnerNickName		= 0;
	m_nKillCount			= 0;
	m_nDeadCount			= 0;
	
	m_dwBWLeftTimeFromServer	= 0;
	m_dwBWReceiveTimeFromServer = 0;
	
	m_bSendFunctionalMovePacket = FALSE;
	m_nFuctionalNormalAction = 0;

	m_nLocalMnDClass = _XDEF_DESCIPLE;//_XDEF_NONE; //Author : 양희왕 //breif : 자신이 스승인지 제자인지

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	memset(&m_pLocalMnDInfomation, 0, sizeof(m_pLocalMnDInfomation)); //Author : 양희왕 //breif : 사제 초기화
#endif

}

void _XLocalUser::ResetInventoryInfo( void )
{
	m_Money = 0;			
	m_cAmorPocketNum = 0;
	m_cPantsPocketNum = 0;
	m_cPocketNum = 0;		// 추후 삭제
	m_cBackPackExtend = 0;	
	m_nCashBackPackMaxValue = 0;				//Author : 양희왕 //breif : 캐시 행낭 최대값

	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		m_UserItemList[i].Reset();
	}
}

void _XLocalUser::ResetWarehouseInfo( void )
{
	for( int i = 0; i < _XDEF_WAREHOUSE_TOTALCOUNT; ++i )
	{
		m_WarehouseItemList[i].Reset();
	}
}

void _XLocalUser::ResetSkillList(void)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		m_SkillList[i].m_sSkillID = 0;
		m_SkillList[i].m_cSkillLevel = -1;

		m_SkillList[i].m_cCurMission = 0;
		m_SkillList[i].m_iSkillExp = 0;

		m_SkillList[i].m_dwCooldownStartTime = 0;
		m_SkillList[i].m_ucSkillState = 0;
	}
}

void _XLocalUser::ResetLocalUserTempData(void)
{	
	// 맵을 넘나들 때 reset 되는 data

//	m_CharacterCount			=	0;
	m_PrevPacketSendedTime		=	0;
	m_PickItem					=	NULL;
	
	m_ExperiencePercentage		=	0;
	LifePowerPercentage			=	0;	
	ForcePowerPercentage		=	0;
	ConcentrationPowerPercentage=	0;
			
	m_SpecialActionTimer		=   0;
	
	m_PrevProcessTime			=	-1.0f;
	m_PrevMoveTime				=    0;
		
	m_TargetPosition			=	m_Position;
	GetItemTileIndex(m_Position.x,	m_Position.z, m_CurrentTileIndex);
	m_LastItemRequestSendTime	=	0;
	
	m_RotateAngle				=	0.0f;	
	m_FinalTargetPosition		=	D3DXVECTOR3( 0.0f,0.0f,0.0f );
	m_LeftFinalTargetLength		=   m_LeftFinalTargetLength;
		
	m_StopAnimation				=	FALSE;
	
	if( ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer == FALSE )
	{	
		for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i) 
		{
			memset(m_PartyInfo[i].playername, 0, sizeof(TCHAR)*13);
			m_PartyInfo[i].serverno = m_PartyInfo[i].posX = m_PartyInfo[i].posZ = 0;
			m_PartyInfo[i].innerlevel = m_PartyInfo[i].vitalpercent = m_PartyInfo[i].zenpercent = 0;
			m_PartyInfo[i].isLeader = FALSE;
		}
		
		m_PartyMemberCount		= 0;
		m_PartyMemberIndex		= -1;
		m_PartySelfIndex		= -1;
		m_InParty				= FALSE;
		m_bPartyQuestProgress	= FALSE;
	}

	memset( &m_CharacterLog, 0, sizeof(_XUSERACTIONLOG) );
	_XDWINPRINT("[LOCALUSER/ResetLocalUserTempData] All log clear");

	m_bReadingBook					= FALSE;		// 2005.1.17
	m_bInternalUse					= FALSE;	
	m_bSocketPlugIn					= FALSE;
	m_bGambleBoxOpen				= FALSE;
	m_bSocketPlugInLock				= FALSE;
	m_bGambleBoxOpenLock			= FALSE;
	m_bTargetInfoLock				= FALSE;
	m_bSocketPlugInEXLock			= FALSE;

	m_ChargeStartTime				= 0;
	m_ChargeIterateValue			= 0;
	m_ChargeSendStartPacket			= FALSE;

	m_RoundingDelayStartTime		= 0;
	m_RoundingSendPacket			= FALSE;

	m_bChannelingSendStartPacket	= FALSE;
	m_ChannelingSendLastPacketTime	= 0;
	m_ChannelingSpellRepeatTime		= 0;
	m_RegionTargetPosX				= 0.0f;
	m_RegionTargetPosZ				= 0.0f;

	m_ChangeDelayStartTime			= 0;
	m_CurrentSelectedSkillType		= 0;
	m_CurrentBattleState			= _XUSER_STATE_NONE;
	m_StanceMotionFirst				= FALSE;
	m_LastModeChangeTime			= 0;

	m_FinishSpeed					= 0;
	m_FinishSendPacket				= FALSE;
	m_bSendFinishFirePacket			= FALSE;

	m_CastingSpellRepeatTime		= 0;
	m_CastingSendPacket				= FALSE;

	m_bLeftClickProcess				= FALSE;
	m_bRightClickProcess			= FALSE;

	m_bUsingSkill					= FALSE;
	m_bCheckDelayTime				= FALSE;

	m_bRestartActive				= FALSE;
	m_bStartCharge					= FALSE;
	m_bUseMouseChargeSkill			= FALSE;

	m_LastRotateAngle				= 0.0f;
	m_RotateDirection				= _XROTATE_NONE;
	
	m_UseSpeedSkill					= FALSE;
	m_AttackPointNumber				= 0;
	m_CurrentAttackPoint			= 0;

	m_DyingStartTime				= 0;
	m_LastCheckedDyingStartTime		= -1;
	m_DyingTimer					= 0;

	m_bWaitPassiveReq				= FALSE;

	m_LastPassiveUsefulType			= 0;
	m_LastPassiveSType				= 0;
	m_LastPassiveOnOffTime			= 0;

	m_bCheckFirstFrame				= FALSE;
	m_CurrentTrainingState			= _XUSER_STATE_NONE;
	m_CurrentInternalUseState		= _XUSER_STATE_NONE;
	m_CurrentReadBookState			= _XUSER_STATE_NONE;
	m_bSendTrainingStartPacket		= FALSE;
	memset(&m_TrainingDummyInfo, 0, sizeof(_XDUMMY_INFO));
	m_TrainingSpellInfo.Reset();
	
	ResetAttackTarget();
	ResetViewTarget();
	ResetSpellAttackTarget();
	
	if(g_pQuickSlot_Window)
	{
		if( g_MainWindowManager.GetWindowCount() )
		{
			g_pQuickSlot_Window->ChangePeaceBattleMode();
		}
		
	}

	memset(&m_AttackResult, 0, sizeof(_XUSER_ATTACKRESULT));
	memset(&m_RegionDamageResult, 0, sizeof(m_RegionDamageResult));

	m_PathNodeCount					=	0;
	m_CurrentNavigatePathNodeIndex	=	0;

	m_DisarmamentItem				= NULL;
	m_CurrentWeaponID				= 0;

	// OLD
	m_MatchType = 0;
	m_MatchID = 0;
	m_MatchTargetID = 0;
	m_bWaitFlagInfo = FALSE;

	// NEW
	ResetMatchState();
	m_MatchPRTimer		= 0; // 비무 홍보 패킷을 5초마다 보낸다.

	m_bGrabItem = FALSE;

	if( !m_bIsBossBattle && !m_bBossBattleEndProcess)//보스전이면 상태효과를 다시 받지 않음.->유지
	{
		// 상태효과 모두 삭제 - 다시 받음
		m_StateList.DeleteAll();
		memset(&m_Lock, 0, sizeof(m_Lock));
	}
	Set_m_MoveSpeedFromCondition(0);
	m_PvpNPCLock = FALSE;

	Set_m_MoveSpeedFromEffect(0);
	Set_m_AttackSpeedFromEffect(0);

	m_HiperRunFailedFlag = FALSE;
	m_bWaitDyingSkip		= FALSE;
	
	// 퀘스트 데이타도 다시 받으므로 reset
	m_bViewMasterySkill		= FALSE;

	m_bHaveStance			= FALSE;
	reserved				= FALSE;

	m_bPEMode				= FALSE;
	m_PKModeLeftTime		= 0;
	m_PKModeStartTime		= 0;

#ifdef _XDEF_CASHITEM_SHOUT
	ResetShout();
#endif

	// 맵 넘어갈때 안전 지역 체크 타이머 초기화
	m_InsideSafetyZoneIndex	= 0;
	m_SafetyZoneCheckTimer	= 0;

	m_bOut					= FALSE;
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	_XPKTRACING_INFO* pktracing = NULL;
	list <_XPKTRACING_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = m_listPKTracing.begin() ; iter_pktracing != m_listPKTracing.end() ; ++iter_pktracing)
	{
		pktracing = *iter_pktracing;
		SAFE_DELETE( pktracing );
	}
	m_listPKTracing.clear();
#endif

	m_bEnterMatchEvent		= FALSE;
	m_nWinnerNickName		= 0;
	m_nKillCount			= 0;
	m_nDeadCount			= 0;

	m_dwBWLeftTimeFromServer	= 0;
	m_dwBWReceiveTimeFromServer = 0;

	m_bSendFunctionalMovePacket = FALSE;
	m_nFuctionalNormalAction = 0;
}

void _XLocalUser::ResetCastleBattleInfo()
{
	m_nCastleMapId			= 0;

	m_bCastleBattle			= false;
	m_bSendThreatenPacket	= false;
	m_bThreatening			= false;

	m_dwRoleInCastleBattle	= 0;
	m_dwAttackType			= 0;
	m_dwResultCastleBattle	= 0;
	m_dwDeadCount			= 0;
	m_dwDieStartTime		= 0;
	m_dwDeadPenaltyTime		= 0;
	m_nCBMapIdInWar			= 0;
	m_nDefenderType			= 0;
	m_nDefenderIndex		= 0;
	memset(m_nAttackerType, 0, sizeof(m_nAttackerType));
	memset(m_nAttackerIndex, 0, sizeof(m_nAttackerIndex));
}


_XUserItem*	_XLocalUser::GetUserItem( int firstcategory, int id )
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return &m_UserItemList[i];
		}															
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return &m_UserItemList[i];
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return &m_UserItemList[i];
		}		
	}
#endif
	
	return NULL;
}

_XUserItem* _XLocalUser::GetInventoryEmptySlot( void )
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if( m_UserItemList[i].Get_m_cType() == -1 )
		{
			return &m_UserItemList[i];
		}					
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if( m_UserItemList[i].Get_m_cType() == -1 )
		{
			return &m_UserItemList[i];
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if( m_UserItemList[i].Get_m_cType() == -1 )
		{
			return &m_UserItemList[i];
		}		
	}
#endif
	
	return NULL;
}

int _XLocalUser::GetUserItemInvenSlot(int firstcategory, int id)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return i;
		}		
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return i;
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if( m_UserItemList[i].Get_m_cType() == firstcategory && 
			m_UserItemList[i].Get_m_sID() == id )
		{
			return i;
		}		
	}
#endif
	
	return -1;
}

_XUserItem* _XLocalUser::GetUserItemFromSecondType(int cType, int sType)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			default :
				break;
			}
		}		
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			default :
				break;
			}
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return &m_UserItemList[i];
				}
				break;
			default :
				break;
			}
		}
	}
#endif

	return NULL;
}

int _XLocalUser::GetUserItemInvenSlotFromSecondType(int cType, int sType)
{
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
    for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			default :
				break;
			}
		}		
	}
				
    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			default :
				break;
			}
		}		
	}
#else
	for( int i = 0; i < _XINVENTORY_SLOTTYPE_TOTALCOUNT; ++i )
	{
		if(m_UserItemList[i].Get_m_cType() == cType)
		{
			switch(cType)
			{
			case _XGI_FC_POTION :
				{
					if(g_PotionItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			case _XGI_FC_RESOURCE :
				{
					if(g_ResourceItemProperty[m_UserItemList[i].Get_m_sID()].cSecondType == sType)
						return i;
				}
				break;
			default :
				break;
			}
		}
	}
#endif

	return -1;
}

_XUserSkill* _XLocalUser::GetSkillItem( short uniqueid )
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		if(m_SkillList[i].m_sSkillID == uniqueid)
		{
			return &m_SkillList[i];
		}
	}

	return NULL;
}

int _XLocalUser::GetLearnedSkillIndex(short uniqueid, BOOL bIncludeEventSkill)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
	{
		if(m_SkillList[i].m_sSkillID == uniqueid)
		{
			return i;
		}
	}

#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
	if(bIncludeEventSkill)
	{
		if(m_EventSkillList.m_sSkillID == uniqueid)
		{
			return _XDEF_MAX_LEARNSKILL;
		}
	}
#endif

	return -1;
}

int _XLocalUser::GetLearnedMasterySkillIndex(short uniqueid)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
	{
		if(m_MasterySkillList[i].skillid == uniqueid)
		{
			return i;
		}
	}
	
	return -1;
}

void _XLocalUser::Process(void)
{
#ifdef _XDWDEBUG 
	if( g_UserTracing )
	{
		if( GetMotionClass() == _XACTION_IDLE )
		{
			_XUser* pUser = g_Userlist.FindUser( g_UserTraceTargetName );
			if( pUser )
			{
				D3DXVECTOR3 nexttargetdir = m_Position - pUser->m_Position;
				D3DXVec3Normalize( &nexttargetdir, &nexttargetdir );
				SetTargetPositionAngle( pUser->m_Position + (nexttargetdir * 1.5f) );
			}			
		}
	}	
#endif

#ifdef _XTS_SAFETYZONE
#ifdef _XTS_PKZONE_SERVERSETTING
	if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo == 1)
#else
	if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkclientinfo == 1)
#endif
	{
		if(m_SafetyZoneCheckTimer == 0)
		{
			m_SafetyZoneCheckTimer = g_LocalSystemTime;
		}
		else
		{
			DWORD safetyzoneelapsedtime = g_LocalSystemTime - m_SafetyZoneCheckTimer;
			if(safetyzoneelapsedtime >= 5000)		// 5초마다 체크
			{
				int areanumber = 0;
				BOOL bCheckSafetyZone = TRUE;
				int	extraskillindex = 0;
#ifdef _XDWDEBUG
//				D3DXVECTOR3 position;
//				g_AreaManager.GetMyAreaCenterPosition(g_pLocalUser->m_Position, position);
#endif
				if(m_BattleMode == _XBATTLE_PK)
				{
					if(g_pLocalUser->m_PKModeStartTime > 0)
					{
						if((g_pLocalUser->m_PKModeStartTime + g_pLocalUser->m_PKModeLeftTime) >= g_LocalSystemTime)
						{
							DWORD lefttime = (g_pLocalUser->m_PKModeStartTime + g_pLocalUser->m_PKModeLeftTime) - g_LocalSystemTime;	
							lefttime /= 1000;
							if(lefttime >= 6)
							{
								// PK카운트가 6초 이상 남아있을 경우 안전지역에 진입하더라도 안전 버프를 받을 수 없다.
//								bCheckSafetyZone = FALSE;
								// 안전 버프 대신 경범 요청한다.
								extraskillindex = 2;
							}
						}
					}
				}
				
				if(bCheckSafetyZone)
				{
					if(g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_SAFE, areanumber))
					{
						// 안전 지역 안쪽
						if(m_InsideSafetyZoneIndex == 0)
						{
							// 진입 한 것
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3031), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);

							// 서버에 알림
							if(extraskillindex > 0)
								g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_saferegion_req, extraskillindex);
							else
								g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_saferegion_req, 1);
						}
					}
					else
					{
						// 안전 지역 바깥쪽
						if(m_InsideSafetyZoneIndex == 1)
						{
							// 나갔음
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3033), _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL);

							// 서버에 알림
							g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_saferegion_req, 0);
						}
					}
				}
				
#ifdef _XDEF_CASTLEBATTLE_MINIMAP
				if(m_bCastleBattle)
				{
					if(g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_CASTLE, areanumber))
					{
						// 장원전에 참가 중이며 장원 안에 있음
						if(g_pMinimap_Window)
						{
							g_pMinimap_Window->SetMinimapRateInCastleBattle();
						}
					}
					else
					{
						if(g_pMinimap_Window)
						{
							if(g_pMinimap_Window->m_bDisableZoomButton)
								g_pMinimap_Window->SetMinimapRate();
						}
					}
				}
				else
				{
					if(g_pMinimap_Window)
					{
						if(g_pMinimap_Window->m_bDisableZoomButton)
							g_pMinimap_Window->SetMinimapRate();
					}
				}
#endif

				m_SafetyZoneCheckTimer = g_LocalSystemTime;
			}
		}
	}
#endif

	if(m_PrevProcessTime == -1.0f)
	{
		m_PrevProcessTime = g_fLocalSystemTime;
	}

	FLOAT progresstime = (g_fLocalSystemTime - m_PrevProcessTime) / 1000.0f;
	if(progresstime <= 0.0f)
		return;

	m_PrevProcessTime = g_fLocalSystemTime;

	///////////////////////////////////////////////////////////////////////
	// Apply gravity
	
	if(!g_FreeMoveMode )
	{
		_XOBBData*	pCollideOBB = NULL;
		FLOAT terrainheight = g_LodTerrain.GetTerrainAndOBBHeight( m_Position, pCollideOBB );
		
		if( fabs(m_Position.y - terrainheight) > EPSILON3 )
		{
			if( m_Position.y < terrainheight && (terrainheight - m_Position.y) < _XDEF_RIDEOBBLIMITHEIGHT )
			{
				m_Position.y = terrainheight;
				
				if( m_Velocity.y > 0.0f )
				{
					m_Velocity.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;

					FLOAT Length = _XFC_sqrt(m_Velocity.y * m_Velocity.y);
					if ( Length > _XDEF_MAXJUMPVELOCITY )
					{
						m_Velocity.y *= ( _XDEF_MAXJUMPVELOCITY / Length );
					}

					m_Position.y += m_Velocity.y * g_fElapsedFrameMilisecondTime;
					
					if( terrainheight+_XDEF_MAXJUMPHEIGHT < m_Position.y )  // 특정 높이 이상 점프 불가 
					{
						m_Position.y = terrainheight+_XDEF_MAXJUMPHEIGHT;
						m_Velocity.y = 0.0f;
					}

					D3DXVECTOR3 vecDec = -m_Velocity;
					D3DXVec3Normalize( &vecDec, &vecDec );

					Length = D3DXVec3Length( &m_Velocity );
					FLOAT Dec = (_XDEF_CHARACTERFRICTION * g_fElapsedFrameMilisecondTime);
					if ( Dec > Length ) Dec = Length;					
					m_Velocity += vecDec * Dec;					
					if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;
				}

				UpdatePosition();
				g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, m_Position.z) );
				g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain, FALSE );

			}
			else if( m_Position.y > terrainheight )
			{
				if( m_UseSpeedSkill )
				{
					m_Position.y += (_XDEF_CHARACTERGRAVITY * 4.0f) * g_fElapsedFrameMilisecondTime;
				}
				else
				{
					m_Position.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;
				}

				if( m_Velocity.y > 0.0f )
				{
					m_Velocity.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;

					FLOAT Length = _XFC_sqrt(m_Velocity.y * m_Velocity.y);
					if ( Length > _XDEF_MAXJUMPVELOCITY )
					{
						m_Velocity.y *= ( _XDEF_MAXJUMPVELOCITY / Length );
					}

					m_Position.y += m_Velocity.y * g_fElapsedFrameMilisecondTime;

					if( terrainheight+_XDEF_MAXJUMPHEIGHT < m_Position.y ) // 특정 높이 이상 점프 불가 
					{
						m_Position.y = terrainheight+_XDEF_MAXJUMPHEIGHT;
						m_Velocity.y = 0.0f;
					}

					D3DXVECTOR3 vecDec = -m_Velocity;
					D3DXVec3Normalize( &vecDec, &vecDec );

					Length = D3DXVec3Length( &m_Velocity );
					FLOAT Dec = (_XDEF_CHARACTERFRICTION * g_fElapsedFrameMilisecondTime);
					if ( Dec > Length ) Dec = Length;
					m_Velocity += vecDec * Dec;					
					if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;
				}
				
				if( m_Position.y < terrainheight ) m_Position.y = terrainheight;
				
				UpdatePosition();
				g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, m_Position.z) );
				g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain, FALSE );
			}
		}
		else
		{
			m_Velocity.y = 0.0f;
		}
	}
	
	///////////////////////////////////////////////////////////////////////
	// Animation 전처리
	int eventlevel = 0; // Effect의 종류를 결정함.
		
	switch(m_CurrentSelectedSkillType)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{				
			if( m_CurrentBattleState == _XUSER_STATE_8 )
			{
				if( m_ChargeIterateValue >= 0 && m_ChargeIterateValue < 2 ) eventlevel = 0;
				else if( m_ChargeIterateValue >= 2 && m_ChargeIterateValue < 4 ) eventlevel = 1;
				else if( m_ChargeIterateValue >= 4 && m_ChargeIterateValue <= 11 ) eventlevel = 2;
				if( g_ZeroLevelMode )		// hotblood zerolevel
					eventlevel = 0;
			}
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			if(m_CurrentBattleState == _XUSER_STATE_6)
				eventlevel = 0;
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{				
			if(  m_CurrentBattleState == _XUSER_STATE_4 )
				eventlevel = 0;
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			if(  m_CurrentBattleState == _XUSER_STATE_4 )
				eventlevel = 0;
		}
		break;
	}
	
	// animation 전처리
	bool soundplay = true;

	if( m_SelectedSkillID )
	{
		// 스킬의 적용 반경이 정해진 경우 이펙트의 적용 최대 반경을 제한한다.
		if( g_SkillProperty[m_SelectedSkillID]->radius > 0.0f )
		{
			m_ModelDescriptor.Advance( soundplay, eventlevel, g_SkillProperty[m_SelectedSkillID]->radius * 2.0f );
		}
		else
		{
			m_ModelDescriptor.Advance(soundplay, eventlevel);
		}
	}
	else
	{
		m_ModelDescriptor.Advance(soundplay, eventlevel);
	}
	
	// by mahwang
	if (m_UserState == _XDEF_USERSTATE_TRAINING && m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
		g_LifeEventManager.Process(g_fElapsedFrameTime / 1000.f);

	GetTargetPosition();

	if(m_bGrabItem)
		GrabWorldItem();

	// 다음에 이동할 위치 처리 - 상태에 따라서 움직이지 않음
	BOOL	bCanNotMove = CanNotMove();	
	if(!bCanNotMove)
		ProcessNextMovePosition();
	else
	{
		m_TargetPosition = m_Position;
		if(GetMotionClass() == _XACTION_MOVE) //Author : 양희왕 //breif : ProcessNextMovePosition안에 있던 예외 처리..
		{
			// 예외처리 - 제자리 뛰기 중
			SetAnimation(_XACTION_IDLE);
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
	}

	// 전투 처리
	ProcessBattle();

	// Delay tiem check
	CheckDelayTime();
	
	// 다음 스킬 적용 
	ApplyNextSkill();
	
	// 수련 처리
	ProcessTraining();

	// 비무 처리
	ProcessMatch();

	// PK 처리
	ProcessPKMode();

#ifdef _XDEF_CASHITEM_SHOUT
	// 외치기 처리
	ProcessShout();
#endif

	// Animaiton 처리
	ProcessAnimation();

	// 상태 효과 처리
	ProcessStateList();

	// parameter처리
	ProcessParameter(progresstime);
	
	// 영약 복용 준비 자세 처리
	ProcessInternalUse();

	// 비급 읽기 처리
	ProcessReadBook();

	// PE mode 처리
	if(g_LocalSystemTime - m_PEModeStartTime > m_PEModeLeftTime && m_PEModeStartTime > 0)
	{
		g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_req_pe_end);
		m_PEModeStartTime = 0;		// 1번만 보내도록
	}

	// 영약 중독 감소 처리
	if( g_LocalSystemTime - m_IntoxicationTimer > 60000 )	// 20초마다 감소
	{
		if( m_ElixirInfo._sIntoxication -1 >= 0 )
		{
			m_IntoxicationTimer = g_LocalSystemTime;
			int tempinto = m_ElixirInfo._sIntoxication;
			m_ElixirInfo._sIntoxication--;
			if( tempinto == 100 && m_ElixirInfo._sIntoxication < 100 )
			{
				g_pState_Window->DeleteIntoxication(_XST_INTOXICATION, 2003);
			}			
		}
		else 
			m_ElixirInfo._sIntoxication = 0;
	}
	
	// 서있는 상태에서도 일정 시간 간격으로 시그널을 보냄
	if(GetMotionClass() == _XACTION_IDLE || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_DYING)
	{
		if(m_UserState >= _XDEF_USERSTATE_MATCH_MATCHING && m_UserState <= _XDEF_USERSTATE_MATCH_END)
		{
			// 비무장 안에 있을 때는 3초에 한번씩 보낸다.
			if((g_LocalSystemTime - m_PrevPacketSendedTime) > 3000)
			{
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				m_PrevPacketSendedTime = g_LocalSystemTime;
			}
		}
		else
		{
			if((g_LocalSystemTime - m_PrevPacketSendedTime) > _XDEF_SENDTIME_IDLEMOTION)
			{
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				m_PrevPacketSendedTime = g_LocalSystemTime;
			}
		}

		CheckCollideStatusInIdle();
	}

	if( !m_bIsBossBattle ) // 보스전일때는 아이템 갱신을 하지 않는다.
	{
		// world item broadcasting request
		_XTILE_ITEM prevtileindex;
		prevtileindex = m_CurrentTileIndex;
		GetItemTileIndex(m_Position.x, m_Position.z, m_CurrentTileIndex);
		if((m_CurrentTileIndex.sX != prevtileindex.sX) || (m_CurrentTileIndex.sZ != prevtileindex.sZ))
		{
			if((g_LocalSystemTime - m_LastItemRequestSendTime) > 3000)
			{
				// 8초 이상일 때 보낸다.
				g_NetworkKernel.SendPacket(MSG_NO_WORLD_ITEM_REQUEST);
				m_LastItemRequestSendTime = g_LocalSystemTime;
			}
		}
		else // 2004.05.28->oneway48 insert : 같은 타일일때도 일정 시간 간격으로 시그널을 보냄
		{
			if((g_LocalSystemTime - m_LastItemRequestSendTime) > 8000)
			{
				g_NetworkKernel.SendPacket(MSG_NO_WORLD_ITEM_REQUEST);
				m_LastItemRequestSendTime = g_LocalSystemTime;
			}
		} // insert end	
	}
	

	if( m_bMatchPRMode )// 비무 홍보 패킷을 5초마다 보낸다.
	{
		if((g_LocalSystemTime - m_MatchPRTimer) > 5000)
		{
			g_NetworkKernel.SendPacket(MSG_NO_MATCH_NOTICE, en_match_ntc_inform);
			m_MatchPRTimer = g_LocalSystemTime;
		}
	}

	//  수련 완료 표시
	_XWindow_TrainingComplete* pComplete_Window = (_XWindow_TrainingComplete*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGCOMPLETE);
	if(pComplete_Window)
	{
		if((g_LocalSystemTime - pComplete_Window->m_CompleteTime > 3000) && pComplete_Window->m_CompleteTime > 0)
		{
			switch(m_SelectedTrainingMode)
			{
			case _XTRAINING_MEDITATION :
				{
				}
				break;
			case _XTRAINING_DUMMY :
				{
				}
				break;
			}
			pComplete_Window->ShowWindow(FALSE);
		}
	}	
}

void _XLocalUser::SetAttackTarget( _XItem* target, _XPICKINGTARGETTYPE targettype )
{
	if( m_AttackTarget )
	{
		switch(m_AttackTargetObjectType)
		{
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XDummy*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		}
	}
	
	
	m_AttackTarget			 = target;
	m_AttackTargetObjectType = targettype;
	
	// 이동누적시간 초기화
	m_PrevMoveTime			 = 0;

	if(m_ViewTarget)
	{
		if(m_ViewTarget == m_AttackTarget)
		{
			switch(m_ViewTargetType)
			{
			case _XPICKINGTARGET_PC :
				{
					if(((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
				break;
			case _XPICKINGTARGET_MOB :
				{
					if(((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
				break;
			case _XPICKINGTARGET_NPC :
				{
					if(((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
				break;
			case _XPICKINGTARGET_DUMMY :
				{
					if(((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					{
						((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
					}
				}
				break;
			case _XPICKINGTARGET_SELF :
				{
					if(m_ModelDescriptor.m_EffectManager)
						m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
				break;
			}
		}
	}

	if(m_AttackTarget)
	{
		switch(targettype)
		{
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)target)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_LOCKON,
																				&((_XUser*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
			}
			break;
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)target)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_LOCKON,
					&((_XMob*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);

				m_CharacterLog.changeTargetUniqueID = ((_XMob*)target)->m_MobID;
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)target)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
																				&((_XNPCObject*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)target)->m_ModelDescriptor.m_EffectManager)
				{
					((_XDummy*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_LOCKON,
						&((_XDummy*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
				}
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_LOCKON,
					&m_ModelDescriptor.m_Position, NULL, TRUE);
			}
			break;
		}
	}

	//if( targettype != _XPICKINGTARGET_FUNCTIONALOBJECT )
	{
		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
		if(pMonsterStatus_Window)
		{
			pMonsterStatus_Window->ShowWindow( ( target ) ? TRUE : FALSE );
		}
	}
}

void _XLocalUser::ResetAttackTarget(void)
{
	if(m_AttackTarget)
	{
		switch(m_AttackTargetObjectType)
		{
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager)
				{
					((_XDummy*)m_AttackTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
				}
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_LOCKON);
			}
			break;
		case _XPICKINGTARGET_FUNCTIONALOBJECT :
			{
//				_XWindow_MatchNoticeLarge* pMatchNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
//				if(pMatchNoticeLarge_Window)
//				{
//					if(pMatchNoticeLarge_Window->GetShowStatus())
//						pMatchNoticeLarge_Window->ShowWindow(FALSE);
//				}
			}
			break;
		}

		if(m_AttackTarget == m_ViewTarget)
		{
			// view target effect 넣어주기
			switch(m_ViewTargetType)
			{
			case _XPICKINGTARGET_PC :
				{
					if(((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_Position, NULL, TRUE );
				}
				break;
			case _XPICKINGTARGET_MOB :
				{
					if(((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_Position, NULL, TRUE);
					
					_XPlayInterfaceSound(ID_SR_INTERFACE_TARGETING00_WAV);
					g_NetworkKernel.SendPacket(MSG_NO_REQ_SPELLEFFECT, ((_XMob*)m_ViewTarget)->m_MobID);
				}
				break;
			case _XPICKINGTARGET_NPC :
				{
					if(((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
						((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_Position, NULL, TRUE);
				}
				break;
			case _XPICKINGTARGET_DUMMY :
				{
					if(((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					{
						((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
							&((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_Position, NULL, TRUE);
					}
				}
				break;
			case _XPICKINGTARGET_SELF :
				{
					if(m_ModelDescriptor.m_EffectManager)
						m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&m_ModelDescriptor.m_Position, NULL, TRUE);
				}
				break;
			}
		}

		m_AttackTarget = NULL;
		m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
		
		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
		if(pMonsterStatus_Window)
		{
			if(!m_ViewTarget)
			{
				if(pMonsterStatus_Window->GetShowStatus())
					pMonsterStatus_Window->ShowWindow(FALSE);
			}
		}
	}
}

void _XLocalUser::InsertTargetPositionEffect( void )
{
	if( m_TargetPosition != m_LastInsertedMoveTargetEffectPosition )
	{
		m_LastInsertedMoveTargetEffectPosition = m_FinalTargetPosition;
		D3DXMATRIX	MoveTargetMatrix;
		D3DXMatrixTranslation( &MoveTargetMatrix, m_FinalTargetPosition.x,	m_FinalTargetPosition.y, m_FinalTargetPosition.z );
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );		
		g_WorldEffectInstanceManager.InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_INTERFACEID_PICKING, &MoveTargetMatrix );
	}
}

/** GetTargetPosition(void)
 *	마우스 클릭 처리, Game screen focus 처리
 *	Local User의 Target position 처리
 */
void _XLocalUser::GetTargetPosition(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
		return;

	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	
	MouseState* mousestate = gpInput->GetMouseState();
	
	if(mousestate)
	{
		if(!g_CurrentFocusedObject)
		{
			// 경공 사용 모드가 아니고 마우스 L 버튼이 눌린경우 이동 타겟 수정
			if( !gpInput->CheckKeyPress(DIK_LMENU) && mousestate->bButton[0])
			{
				if( g_CurrentFocusedObject != _XDEF_DUMMYID_FUNCTIONALOBJECT )
				{
					g_CurrentFocusedObject = _XDEF_DUMMYID_GAMEBACKGROUND;
				}
			}
		}
		
		if(!mousestate->bButton[0])
		{
			m_bLeftClickProcess = FALSE;
			
			if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND || g_CurrentFocusedObject == _XDEF_DUMMYID_FUNCTIONALOBJECT )
			{
				g_CurrentFocusedObject = NULL;
			}
			
		}
	}
	
	if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
	{
		GetTargetPositionIdleMode();
		GetTargetPositionBattleMode();
	}
}

void _XLocalUser::GetTargetPositionIdleMode(void)
{
	if(m_BattleMode)
		return;

	MouseState* mousestate = gpInput->GetMouseState();
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

	if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND && mousestate->bButton[0])
	{
		if(g_LastPickedObject)
		{
			if(g_LastPickedObjectType == _XPICKINGTARGET_ITEM)
			{
				// 아이템 클릭 - 아이템 쪽으로 이동
				_XGameItem* pickeditem = (_XGameItem*)g_LastPickedObject;
				m_PickItem = pickeditem;
				m_bGrabItem = TRUE;
				// 이동누적시간 초기화
				m_PrevMoveTime			 = 0;
			}
			else
			{
				// 다른 곳을 클릭했을 경우 아이템 집기 취소
				m_bGrabItem = FALSE;

				if(!m_ViewTarget)
				{
					// 타켓이 없음 - 타겟 지정
					SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
					m_bLeftClickProcess = TRUE;

					// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
					if( g_TutorialInfo )
					{
						if( !g_TutorialInfo->_XTutorial_003 && g_LastPickedObjectType == _XPICKINGTARGET_MOB )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_029);
							g_TutorialInfo->_XTutorial_003 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					
						if( !g_TutorialInfo->_XTutorial_016 && g_LastPickedObjectType == _XPICKINGTARGET_MOB && 
							(g_MobAttribTable[((_XMob*)m_ViewTarget)->m_MobType].party > 16) && 
							(g_MobAttribTable[((_XMob*)m_ViewTarget)->m_MobType].party < 25) )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_016);
							g_TutorialInfo->_XTutorial_016 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					}					
					// ----------------------------------------------------------------------------------------------------------------------------=
				}
				else if(!m_bLeftClickProcess)
				{
					// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=
					if( g_TutorialInfo )
					{
						if( !g_TutorialInfo->_XTutorial_016 && g_LastPickedObjectType == _XPICKINGTARGET_MOB && 
							(g_MobAttribTable[((_XMob*)m_ViewTarget)->m_MobType].party > 16) && 
							(g_MobAttribTable[((_XMob*)m_ViewTarget)->m_MobType].party < 25) )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_016);
							g_TutorialInfo->_XTutorial_016 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					}
					// ----------------------------------------------------------------------------------------------------------------------------=
					
					// 타겟이 있음 - 두번째 클릭이거나 혹은 다른 타겟
					if(m_ViewTargetType == _XPICKINGTARGET_NPC)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							if(!pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
							{
								// NPC 대화모드가 아닐때만
								_XNPCObject* pNPC = (_XNPCObject*)m_ViewTarget;
								D3DXVECTOR3 position = D3DXVECTOR3(pNPC->m_matWorldPosition._41, pNPC->m_matWorldPosition._42, pNPC->m_matWorldPosition._43);
								int ret = pProc_Maingame->StartNPCScript();
								SetAttackTarget((_XItem*)pNPC, _XPICKINGTARGET_NPC);

								if(ret == 0)
								{
									// NPC쪽으로 이동
									D3DXVECTOR3 directionvector = m_Position - position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = position + (directionvector*(_XDEF_DISTANCEOFSTARTNPCSCRIPT*0.95f));
									targetposition.y = position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									SetTargetPositionAngle(m_Position);
									
									// 대화 로그 기록
									if(g_ScriptStarted)
									{
										m_CharacterLog.isTalk = TRUE;
										m_CharacterLog.talkTime = g_ServerTimeCode;

										_XDWINPRINT("[LOCAL USER/GetTargetPositionIdleMode] NPC[%d] talk", pNPC->m_UniqueID);
									}
								}
							} // 대화모드
						}
						else
						{
							// 다른 타겟
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // if(m_ViewTargetType == _XPICKINGTARGET_NPC)
					else if( m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_ViewTarget;
							if( pFunctionalObject->m_HealthPercentage > 0 )
							{
								int ret = pProc_Maingame->StartFunctionalObjectScript();
									
								//SetAttackTarget((_XItem*)pFunctionalObject, _XPICKINGTARGET_FUNCTIONALOBJECT);
								
								if( ret == 0 )
								{
									// OBJECT 쪽으로 이동
									D3DXVECTOR3 position = pFunctionalObject->m_Position;
									D3DXVECTOR3 directionvector = m_Position - position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = position + (directionvector*(g_LodTerrain.m_MeshObjectManager.m_ModelList[ pFunctionalObject->m_ObjectIndex ]->OBB_Extent[2]*2 + 2.5f));
									targetposition.y = position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{									
									g_CurrentFocusedObject = _XDEF_DUMMYID_FUNCTIONALOBJECT;
									SetTargetPositionAngle(m_Position);								
								}
							}
						
						}
						else
						{
							// 다른 타겟
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					}//else if( m_ViewTargetType = _XPICKINGTARGET_FUNCTIONALOBJECT )
					else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							_XMob* pMob = (_XMob*)m_ViewTarget;
							FLOAT range = m_AttackRange + g_MobAttribTable[pMob->m_MobType].collideradius;
							FLOAT dx = m_Position.x - pMob->m_Position.x;
							FLOAT dz = m_Position.z - pMob->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

//							SetAttackTarget((_XItem*)pMob, _XPICKINGTARGET_MOB);
							
							if(distance > range)
							{
								
								D3DXVECTOR3 directionvector = m_Position - pMob->m_Position;
								D3DXVec3Normalize(&directionvector, &directionvector);

								D3DXVECTOR3 targetposition = pMob->m_Position + (directionvector*(range*0.7f));
								targetposition.y = pMob->m_Position.y;
								SetTargetPositionAngle(targetposition);
								InsertTargetPositionEffect();
							}
							else
							{
								ProcessLPSSystem(pMob);
								UpdatePosition();								
							}

							// LPS는 메세지를 출력하지 않는다 
							if( g_MobAttribTable[pMob->m_MobType].scripttype < 81 || g_MobAttribTable[pMob->m_MobType].scripttype > 85)
							{
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2621), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// 평화모드에서는 무공을 사용할 수 없습니다.
							}
						}
						else
						{
							// 다른 타겟
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}	
					} // else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
					else if(m_ViewTargetType == _XPICKINGTARGET_PC)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							_XUser* pUser = (_XUser*)m_ViewTarget;
							FLOAT range = m_AttackRange + 1.0f;
							FLOAT dx = m_Position.x - pUser->m_Position.x;
							FLOAT dz = m_Position.z - pUser->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
							
//							SetAttackTarget(pUser, _XPICKINGTARGET_PC);
							bool jump = false;
							if( this->m_PersonalStoreState == _XUSER_PERSONALSTORE_NONE )
							{
								// PC가 개인 상점을 열었을 경우 
								if( pUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
								{
									jump = true;
									if(distance > 1.0f ) // 1.0f 안에 들어와야 거래를 할 수 있다.
									{
										// PC쪽으로 이동
										D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
										D3DXVec3Normalize(&directionvector, &directionvector);
										
										D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector);
										targetposition.y = pUser->m_Position.y;
										SetTargetPositionAngle(targetposition);
										InsertTargetPositionEffect();
									}
									else
									{
										if((g_LocalSystemTime - m_PrevRequestPacketSendedTime) > 1000)
										{		
											g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_shopitem_list_req,pUser->m_Uniqueid);
											m_PrevRequestPacketSendedTime  = g_LocalSystemTime;
										}
										
										_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
										if( pPersonalStore_Window )
										{
											pPersonalStore_Window->m_OwnerUniqueID = pUser->m_Uniqueid;
											memset(pPersonalStore_Window->m_OwnerName, 0, sizeof(TCHAR)*_XDEF_MAX_USERNAMESTRINGLENGTH);
											memset(pPersonalStore_Window->m_PersonalStoreName, 0, sizeof(TCHAR)*37);
											strcpy( pPersonalStore_Window->m_OwnerName, pUser->m_CharacterInfo.charactername);
											strcpy( pPersonalStore_Window->m_PersonalStoreName, pUser->m_PersonalStoreName );
										}
									}
								}
							}
							
							if( !jump )
							{
								if(distance > range)
								{
									// PC쪽으로 이동
									D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector*(range*0.7f));
									targetposition.y = pUser->m_Position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
									UpdatePosition();
									
									if(pUser->GetMotionClass() == _XACTION_DYING)
										ProcessRescueUser();
								}
							}
							
						}
						else
						{
							// 다른 타겟
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_PC)
					else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							_XDummy* pDummy = (_XDummy*)m_ViewTarget;
							FLOAT range = m_AttackRange + 1.0f;
							FLOAT dx = m_Position.x - pDummy->m_Position.x;
							FLOAT dz = m_Position.z - pDummy->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

							SetAttackTarget(pDummy, _XPICKINGTARGET_DUMMY);

							if(distance > range)
							{
								D3DXVECTOR3 directionvector = m_Position - pDummy->m_Position;
								D3DXVec3Normalize(&directionvector, &directionvector);

								D3DXVECTOR3 targetposition = pDummy->m_Position + (directionvector*range);
								targetposition.y = pDummy->m_Position.y;
								SetTargetPositionAngle(targetposition);
								InsertTargetPositionEffect();
							}
							else
							{
								if(m_UserState != _XDEF_USERSTATE_TRAINING)
								{
									_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
									if(pDummy_Window)
									{
										if(!pDummy_Window->m_bComplete)
											g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2896), TRUE); //_T("평화상태에서는 수련을 시작할 수 없습니다.")
									}
								}
							}
						}
						else
						{
							// 다른 타겟
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
					else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							_XSeat* pSeat = (_XSeat*)m_ViewTarget;
							FLOAT range = 1.0f;
							FLOAT dx = m_Position.x - pSeat->m_Position.x;
							FLOAT dz = m_Position.z - pSeat->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
							
							SetAttackTarget(pSeat, _XPICKINGTARGET_SEAT);
							
							if(distance > range)
							{
								D3DXVECTOR3 directionvector = m_Position - pSeat->m_Position;
								D3DXVec3Normalize(&directionvector, &directionvector);
								
								D3DXVECTOR3 targetposition = pSeat->m_Position + (directionvector*range);
								targetposition.y = pSeat->m_Position.y;
								SetTargetPositionAngle(targetposition);
								InsertTargetPositionEffect();
							}
							else
							{
								if(m_UserState != _XDEF_USERSTATE_TRAINING)
								{
									g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2897), TRUE); //_T("평화상태에서는 기공 수련을 시작할 수 없습니다.")
								}
							}
						}
						else
						{
							// 다른 타겟
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
					else if(m_ViewTargetType == _XPICKINGTARGET_SELF)
					{
						if(m_ViewTarget != g_LastPickedObject)
						{
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					}
					
					// by mahwang
					else if(m_ViewTargetType == _XPICKINGTARGET_COLLECT)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							_XOctreeObject* pObject = (_XOctreeObject*)m_ViewTarget;

							D3DXVECTOR3 vObjPosition(pObject->m_matWorldPosition._41, pObject->m_matWorldPosition._42, pObject->m_matWorldPosition._43);

							FLOAT range = 1.0f;
							FLOAT dx = m_Position.x - vObjPosition.x;
							FLOAT dz = m_Position.z - vObjPosition.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

							SetAttackTarget(pObject, _XPICKINGTARGET_COLLECT);

							if(distance > range)
							{
								D3DXVECTOR3 directionvector = m_Position - vObjPosition;
								D3DXVec3Normalize(&directionvector, &directionvector);

								D3DXVECTOR3 targetposition = vObjPosition + (directionvector*range);
								targetposition.y = vObjPosition.y;
								SetTargetPositionAngle(targetposition);
								InsertTargetPositionEffect();
							}
							else
							{
								if(GetMotionClass() != _XACTION_COLLECT)
								{
									switch(pObject->m_nEventType)
									{
									case OBJECT_EVENT_COLLECT_COTTON:
									case OBJECT_EVENT_COLLECT_TEA:
									case OBJECT_EVENT_COLLECT_MUSHROOM:
									case OBJECT_EVENT_COLLECT_TREE:
										{
											// 들고 있는 아이템 삭제
											m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
											m_CharacterInfo.modeldescript.item1 = 0;
											m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
											m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;

											_XAnimMeshAbstractClass* pEventItem = NULL;
											if (pObject->m_nEventType == OBJECT_EVENT_COLLECT_TREE)
												pEventItem = g_LifeEventManager.GetEventItem(m_CharacterInfo.gender, LIFE_EVENT_ITEM_TYPE_AXE);
											else if (pObject->m_nEventType == OBJECT_EVENT_COLLECT_MUSHROOM)
												pEventItem = g_LifeEventManager.GetEventItem(m_CharacterInfo.gender, LIFE_EVENT_ITEM_TYPE_HOMI);

											m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, pEventItem );


											int nIndex = g_LifeEventManager.GetaniIndex(m_CharacterInfo.gender, pObject->m_nEventType - 1);
											if (nIndex < 0) break;
											SetAnimationIndex(nIndex);
										}
										break;
									}
								}
								if(m_UserState != _XDEF_USERSTATE_TRAINING)
								{
									switch(pObject->m_nEventType)
									{
									case OBJECT_EVENT_FIREWOOD_POINT:
										{
											m_nTrainingCount = 0;
											m_UserState = _XDEF_USERSTATE_TRAINING;
											m_SelectedTrainingMode = _XTRAINING_FIREWOODCUT;
											m_CurrentTrainingState = _XUSER_STATE_1;

											D3DXVECTOR3 vD;
											vD.x = pObject->m_matWorldPosition._41;
											vD.y = pObject->m_matWorldPosition._42;
											vD.z = pObject->m_matWorldPosition._43;

											m_Position.x = vD.x + pObject->m_matWorldPosition._31 * 1.f;
											m_Position.y = vD.y + pObject->m_matWorldPosition._32 * 1.f;
											m_Position.z = vD.z + pObject->m_matWorldPosition._33 * 1.f;

											m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, vD);
											UpdatePosition();
										}
										break;
									case OBJECT_EVENT_WATER_POINT:
										m_nTrainingCount = 0;
										m_UserState = _XDEF_USERSTATE_TRAINING;
										m_SelectedTrainingMode = _XTRAINING_WATERDRAW;
										m_CurrentTrainingState = _XUSER_STATE_1;
										break;
									}
								}
								else 
								{
									switch(pObject->m_nEventType)
									{
										case OBJECT_EVENT_WATER_POINT:
											if (m_SelectedTrainingMode == _XTRAINING_WATERDRAW && m_CurrentTrainingState == _XUSER_STATE_2)
											{
												m_CurrentTrainingState = _XUSER_STATE_3;
											}
											break;
										case OBJECT_EVENT_WATER_TUBE:
											if (m_SelectedTrainingMode == _XTRAINING_WATERDRAW && m_CurrentTrainingState == _XUSER_STATE_5)
											{
												m_CurrentTrainingState = _XUSER_STATE_6;
											}
											break;
									}
								}
							}
						}
						else
						{
							// 다른 타겟
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
							m_bLeftClickProcess = TRUE;
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_COLLECT)
				}
			}
		} // if(g_LastPickedObject)
		else if(!m_bLeftClickProcess)
		{
			if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
			{
				// NPC 대화모드가 아닐때만
				pProc_Maingame->m_bAutoNPCScriptPopUp = FALSE;

				// 다른 곳을 클릭했을 경우 아이템 집기 취소
				m_bGrabItem = FALSE;

				// 상태효과로 이동 불능일 경우 메세지 출력
				if(m_Lock.movelock == 1)
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2898), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //"상태 이상으로 인하여 이동이 불가능 합니다."
				}

				if(CheckValidNextPosition())
				{
					SetTargetPositionAngle(g_vMousePosition);
					InsertTargetPositionEffect();
				}
				else
				{
					if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE && GetMotionClass() != _XACTION_REBIRTH )
					{
						m_TargetPosition = m_Position;
					}
				}
			}
		}
	} // if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
}


void _XLocalUser::SetViewTargetToAttackTarget(void)
{
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	
	if( !m_ViewTarget ) return;
	if( !m_BattleMode ) return;

	// 상태효과로 맨손 전투 불가능
	if(m_Lock.battlelock == 1 && m_SelectedSkillID == 0)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2736), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //"상태 이상으로 인하여 전투가 불가능 합니다."
		return;
	}

	if(m_ViewTargetType == _XPICKINGTARGET_MOB)
	{
		_XMob* pMob = (_XMob*)m_ViewTarget;

		if(g_MobAttribTable[pMob->m_MobType].scripttype != 82)	// Key LPS일 때는 처리하지 않음
		{
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
			if(g_MobAttribTable[pMob->m_MobType].scripttype >= 91 &&
			   g_MobAttribTable[pMob->m_MobType].scripttype <= 95 )
			{
				if(pMob->m_Action.action != _XMOBACTION_DIE)
				{
					if( gpInput->CheckKeyPress( DIK_LSHIFT ) )
						m_bGuardMob = true;

					if( pMob->m_Action.action == _XMOBACTION_ATTACK || m_bGuardMob)
					{
						FLOAT range = 0.0f;

						if(m_AttackTarget != m_ViewTarget)
						{
							SetAttackTarget(pMob, _XPICKINGTARGET_MOB);

							if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
							{
								// 몹 따라가기
								if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
								{
									SetTargetPosition(pMob->m_Position, range);
								}
							}
							else
							{
								// 몹 옆에서 더블 클릭
								if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
								{
									// active skill 시작
								}
							}
						}
						else
						{
							if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
							{
								// 몹 따라가기
								if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
								{
									SetTargetPosition(pMob->m_Position, range);
								}
							}
							else
							{
								if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
								{
									// 몹 옆에서 더블 클릭
									if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
									{
										// active skill 시작
									}
								}
							}
						}
					}
				}
			}
			else
			{
#endif
				if(pMob->m_Action.action != _XMOBACTION_DIE)
				{
					FLOAT range = 0.0f;

					if(m_AttackTarget != m_ViewTarget)
					{
						SetAttackTarget(pMob, _XPICKINGTARGET_MOB);

						if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
						{
							// 몹 따라가기
							if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
							{
								SetTargetPosition(pMob->m_Position, range);
							}
						}
						else
						{
							// 몹 옆에서 더블 클릭
							if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
							{
								// active skill 시작
							}
						}
					}
					else
					{
						if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
						{
							// 몹 따라가기
							if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
							{
								SetTargetPosition(pMob->m_Position, range);
							}
						}
						else
						{
							if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
							{
								// 몹 옆에서 더블 클릭
								if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
								{
									// active skill 시작
								}
							}
						}
					}
				} // if(pMob->m_Action.action != _XMOBACTION_DIE)
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
			}
#endif
		}
	}
	else if(m_ViewTargetType == _XPICKINGTARGET_PC)
	{
		_XUser* pUser = (_XUser*)m_ViewTarget;
		if(!pUser)
			return;

		if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
			(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
			(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
		{
			// 전투 가능
			if(pUser->GetMotionClass() != _XACTION_DYING && pUser->GetMotionClass() != _XACTION_DIE)
			{
				FLOAT range = 0.0f;
				
				if(m_AttackTarget != m_ViewTarget)
				{
					SetAttackTarget(pUser, _XPICKINGTARGET_PC);
					
					if(!CheckDistanceToTarget(pUser->m_Position, m_ActiveSlotID, range, 0, true))
					{
						// 몹 따라가기
						if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
						{
							SetTargetPosition(pUser->m_Position, range);
						}
					}
					else
					{
						// 몹 옆에서 더블 클릭
						if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
						{
							// active skill 시작
						}
					}
				}
				else
				{
					if(!CheckDistanceToTarget(pUser->m_Position, m_ActiveSlotID, range, 0, true))
					{
						// 몹 따라가기
						if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
						{
							SetTargetPosition(pUser->m_Position, range);
						}
					}
					else
					{
						if(m_CurrentBattleState == _XUSER_STATE_NONE)
						{
							// 몹 옆에서 더블 클릭
							if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
							{
								// active skill 시작
							}
						}
					}
				}
			} // if(pUser->GetMotionClass() != _XACTION_DYING || pUser->GetMotionClass() != _XACTION_DIE)
		} // 전투 가능
	} // _XPICKINGTARGET_PC
}


void _XLocalUser::GetTargetPositionBattleMode(void)
{
	if(!m_BattleMode)
		return;

	MouseState* mousestate = gpInput->GetMouseState();
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	
	if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND && mousestate->bButton[0])
	{
		if(g_LastPickedObject)
		{
			if(g_LastPickedObjectType == _XPICKINGTARGET_ITEM)
			{
				// 아이템 클릭 - 아이템쪽으로 이동
				_XGameItem* pickeditem = (_XGameItem*)g_LastPickedObject;
				m_PickItem = pickeditem;
				m_bGrabItem = TRUE;

				// 이동누적시간 초기화
				m_PrevMoveTime			 = 0;
			}
			else
			{
				// 다른 곳을 클릭했을 경우 아이템 집기 취소
				m_bGrabItem = FALSE;

				BOOL bChangeTarget = FALSE;
				if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
				{
					bChangeTarget = TRUE;
				}
				else
				{
					if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
					{
						bChangeTarget = TRUE;
					}
				}

				if(!m_ViewTarget)
				{					
					// 타겟이 없음 - 타겟 지정
#ifdef _XTS_PK
					SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
					SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
					m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
					m_bGuardMob =false;
#endif

					// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
					if( g_TutorialInfo )
					{
						if( !g_TutorialInfo->_XTutorial_003 && g_LastPickedObjectType == _XPICKINGTARGET_MOB )
						{
							_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
							pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_029);
							g_TutorialInfo->_XTutorial_003 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
							g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
						}
					}					
					// ----------------------------------------------------------------------------------------------------------------------------=
				}
				else if(!m_bLeftClickProcess && bChangeTarget)
				{
					// 타겟이 있음 - 두번째 클릭이거나 다른 타겟 지정
					if(m_ViewTargetType == _XPICKINGTARGET_NPC)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							if(!pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
							{
								// NPC 대화모드가 아닐때
								_XNPCObject* pNPC = (_XNPCObject*)m_ViewTarget;
								D3DXVECTOR3 position = D3DXVECTOR3(pNPC->m_matWorldPosition._41, pNPC->m_matWorldPosition._42, pNPC->m_matWorldPosition._43);
								int ret = pProc_Maingame->StartNPCScript();

								SetAttackTarget((_XItem*)pNPC, _XPICKINGTARGET_NPC);

								if(ret == 0)
								{
									// NPC 쪽으로 이동
									D3DXVECTOR3 directionvector = m_Position - position;
									D3DXVec3Normalize(&directionvector, &directionvector);

									D3DXVECTOR3 targetposition = position + (directionvector*(_XDEF_DISTANCEOFSTARTNPCSCRIPT*0.95f));
									targetposition.y = position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									SetTargetPositionAngle(m_Position);

									// 대화 로그 기록
									if(g_ScriptStarted)
									{
										m_CharacterLog.isTalk = TRUE;
										m_CharacterLog.talkTime = g_ServerTimeCode;

										_XDWINPRINT("[LOCAL USER/GetTargetPositionBattleMode] NPC[%d] talk", pNPC->m_UniqueID);
									}
								}
							}
						} // if(m_ViewTarget == g_LastPickedObject)
						else
						{
							// 다른 타겟
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
							m_bGuardMob =false;
#endif
						}
					} // if(m_ViewTargetType == _XPICKINGTARGET_NPC)
					else if(m_ViewTargetType == _XPICKINGTARGET_PC)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							_XUser* pUser = (_XUser*)m_ViewTarget;
							FLOAT range = m_AttackRange + 0.5f;
							FLOAT dx = m_Position.x - pUser->m_Position.x;
							FLOAT dz = m_Position.z - pUser->m_Position.z;
							FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

							if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
								(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
								(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
							{
								// 전투가 가능한 경우
								
								// debug message 이므로 string table에 넣을 필요 없습니다.
								if(m_UserState == _XDEF_USERSTATE_MATCH_FIGHT)
								{
									_XDWINPRINT("공격가능 타겟(더블클릭) : localuserstate[비무중]  matchtargetid[%d] userid[%d]", m_UserState, m_MatchTargetID, pUser->m_Uniqueid);
								}
								else if(m_UserState == _XDEF_USERSTATE_PK_READY || m_UserState == _XDEF_USERSTATE_PK_FIGHT)
								{
									_XDWINPRINT("공격가능 타겟(더블클릭) : localuserstate[PK중] userid[%d]", m_UserState, pUser->m_Uniqueid);
								}

								if(m_Lock.battlelock == 1 && m_SelectedSkillID == 0)
								{
									// 상태효과에서 맨손 전투 불가 - GetTargetPositionIdleMode()와 동일하게 동작
									if(distance > range)
									{
										D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
										D3DXVec3Normalize(&directionvector, &directionvector);

										D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector*(range*0.7f));
										targetposition.y = pUser->m_Position.y;
										SetTargetPositionAngle(targetposition);
										InsertTargetPositionEffect();
									}
									else
									{
										UpdatePosition();
									}
									g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2736 ), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); // "상태 이상으로 인하여 전투가 불가능 합니다."
								}
								else
								{
									if(pUser->GetMotionClass() != _XACTION_DYING && pUser->GetMotionClass() != _XACTION_DIE)
									{
										FLOAT range = 0.0f;

										if(m_AttackTarget != g_LastPickedObject)
										{
											SetAttackTarget(pUser, _XPICKINGTARGET_PC);

											if(!CheckDistanceToTarget(pUser->m_Position, m_ActiveSlotID, range, 0, true))
											{
												// 따라가기
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													SetTargetPosition(pUser->m_Position, range);
												}
											}
											else
											{
												// 옆에서 더블클릭
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													// active skill 시작
												}
											}
										}
										else
										{
											if(!CheckDistanceToTarget(pUser->m_Position, m_ActiveSlotID, range, 0, true))
											{
												// 따라가기
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													SetTargetPosition(pUser->m_Position, range);
												}
											}
											else
											{
												// 옆에서 더블클릭
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													// active skill 시작
												}
											}
										}
									} // if(pUser->GetMotionClass() != _XACTION_DYING || pUser->GetMotionClass() != _XACTION_DIE)
								}
							}	// 전투 가능
							else
							{
								// 전투 불가능

								// debug message 이므로 string table에 넣을 필요 없습니다.
								if(m_UserState == _XDEF_USERSTATE_MATCH_FIGHT)
								{
									_XDWINPRINT("공격불가능 타겟(더블클릭) : localuserstate[비무중]  matchtargetid[%d] userid[%d]", m_UserState, m_MatchTargetID, pUser->m_Uniqueid);
								}
								else if(m_UserState == _XDEF_USERSTATE_PK_READY || m_UserState == _XDEF_USERSTATE_PK_FIGHT)
								{
									_XDWINPRINT("공격불가능 타겟(더블클릭) : localuserstate[PK중] userid[%d]", m_UserState, pUser->m_Uniqueid);
								}

								// pk check
								SetPKReadyMode();

								bool jump = false;
								if( m_PersonalStoreState == _XUSER_PERSONALSTORE_NONE )
								{
									// PC가 개인 상점을 열었을 경우 
									if( pUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
									{
										jump = true;
										if(distance > 1.0f ) // 1.0f 안에 들어와야 거래를 할 수 있다.
										{
											// PC쪽으로 이동
											D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
											D3DXVec3Normalize(&directionvector, &directionvector);
											
											D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector);
											targetposition.y = pUser->m_Position.y;
											SetTargetPositionAngle(targetposition);
											InsertTargetPositionEffect();
										}
										else
										{
											if((g_LocalSystemTime - m_PrevRequestPacketSendedTime) > 1000)
											{		
												g_NetworkKernel.SendPacket(MSG_NO_PS_CTRL,ps_ctrl_shopitem_list_req,pUser->m_Uniqueid);
												m_PrevRequestPacketSendedTime  = g_LocalSystemTime;
											}

											_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
											if( pPersonalStore_Window )
											{
												pPersonalStore_Window->m_OwnerUniqueID = pUser->m_Uniqueid;
												memset(pPersonalStore_Window->m_OwnerName, 0, sizeof(TCHAR)*_XDEF_MAX_USERNAMESTRINGLENGTH);
												memset(pPersonalStore_Window->m_PersonalStoreName, 0, sizeof(TCHAR)*37);
												strcpy( pPersonalStore_Window->m_OwnerName, pUser->m_CharacterInfo.charactername);
												strcpy( pPersonalStore_Window->m_PersonalStoreName, pUser->m_PersonalStoreName );
											}
										}
									} // 개인상점
								}
								if( !jump )
								{
									if(distance > range)
									{
										
										D3DXVECTOR3 directionvector = m_Position - pUser->m_Position;
										D3DXVec3Normalize(&directionvector, &directionvector);
										
										D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector*(range*0.7f));
										targetposition.y = pUser->m_Position.y;
										SetTargetPositionAngle(targetposition);
										InsertTargetPositionEffect();
									}
									else
									{
										m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
										UpdatePosition();
										
										if(pUser->GetMotionClass() == _XACTION_DYING)
											ProcessRescueUser();
									}
								}
							} // 전투 불가능							
						}
						else
						{
							// 다른 타겟
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
							m_bGuardMob =false;
#endif
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_PC)
					else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
					{
						XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	// hotblood zerolevel
						if( pZeroLevelProcess->GetCurrentTutorial() == 3 )
						{
							pZeroLevelProcess->SetTutorialModeOn(FALSE);
						}

						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							_XMob* pMob = (_XMob*)g_LastPickedObject;


							// 상태효과에서 맨손 전투 불가
							if(m_Lock.battlelock == 1 && m_SelectedSkillID == 0)
							{
								// GetTargetPositionIdleMode()와 동일하게 동작
								FLOAT range = m_AttackRange + g_MobAttribTable[pMob->m_MobType].collideradius;
								FLOAT dx = m_Position.x - pMob->m_Position.x;
								FLOAT dz = m_Position.z - pMob->m_Position.z;
								FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
								
								if(distance > range)
								{
									
									D3DXVECTOR3 directionvector = m_Position - pMob->m_Position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = pMob->m_Position + (directionvector*(range*0.7f));
									targetposition.y = pMob->m_Position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									UpdatePosition();								
								}

								g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_2736), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //상태 이상으로 인하여 전투가 불가능 합니다.
							}
							else
							{
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
								if(g_MobAttribTable[pMob->m_MobType].scripttype >= 91 &&
								   g_MobAttribTable[pMob->m_MobType].scripttype <= 95 )
								{
									if(pMob->m_Action.action != _XMOBACTION_DIE)
									{
										if( gpInput->CheckKeyPress( DIK_LSHIFT ) )
											m_bGuardMob = true;

										if( pMob->m_Action.action == _XMOBACTION_ATTACK || m_bGuardMob)
										{
											FLOAT range = 0.0f;

											if(m_AttackTarget != g_LastPickedObject)
											{
												SetAttackTarget(pMob, _XPICKINGTARGET_MOB);
												
												if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
												{
													// 몹 따라가기
													if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
													{
														SetTargetPosition(pMob->m_Position, range);
													}
												}
												else
												{
													// 몹 옆에서 더블 클릭
													if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
													{
														// active skill 시작
													}
												}
											}
											else
											{
												{
													if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
													{
														// 몹 따라가기
														if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
														{
															SetTargetPosition(pMob->m_Position, range);
														}
													}
													else
													{
														// 몹 옆에서 더블 클릭
														if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
														{
															// active skill 시작
														}
													}
												}
											}
										}								
									}
								}
								else
								{
#endif
									if(pMob->m_Action.action != _XMOBACTION_DIE)
									{
										FLOAT range = 0.0f;

										if(m_AttackTarget != g_LastPickedObject)
										{
											SetAttackTarget(pMob, _XPICKINGTARGET_MOB);
											
											if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
											{
												// 몹 따라가기
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													SetTargetPosition(pMob->m_Position, range);
												}
											}
											else
											{
												// 몹 옆에서 더블 클릭
												if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
												{
													// active skill 시작
												}
											}
										}
										else
										{
											{
												if(!CheckDistanceToTarget(pMob->m_Position, m_ActiveSlotID, range, pMob->m_MobType, true))
												{
													// 몹 따라가기
													if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
													{
														SetTargetPosition(pMob->m_Position, range);
													}
												}
												else
												{
													// 몹 옆에서 더블 클릭
													if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
													{
														// active skill 시작
													}
												}
											}
											
										}
									} // if(pMob->m_Action.action != _XMOBACTION_DIE)
#ifdef _XDEF_NPC_ATTACK_CHECK
								}
#endif
							}
						} // if(m_ViewTarget == g_LastPickedObject)
						else
						{
							// 다른 타겟
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
							m_bGuardMob =false;
#endif
						}
					} // else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
					else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭
							_XDummy* pDummy = (_XDummy*)g_LastPickedObject;
							if(pDummy)
							{
								FLOAT dx = m_Position.x - pDummy->m_Position.x;
								FLOAT dz = m_Position.z - pDummy->m_Position.z;
								FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
								FLOAT range = m_AttackRange + 1.0f;

								SetAttackTarget(pDummy, _XPICKINGTARGET_DUMMY);

								if(distance > range)
								{
									D3DXVECTOR3 directionvector = m_Position - pDummy->m_Position;
									D3DXVec3Normalize(&directionvector, &directionvector);

									D3DXVECTOR3 targetposition = pDummy->m_Position + (directionvector*range);
									targetposition.y = pDummy->m_Position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pDummy->m_Position);
									UpdatePosition();

									// 수련을 시작하겠습니까?
									if(m_UserState != _XDEF_USERSTATE_TRAINING)
									{
										if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
										{
											_XWindow_TrainingDummy* pDummy_Window = (_XWindow_TrainingDummy*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRAININGDUMMY);
											if(pDummy_Window)
											{
												if(!pDummy_Window->m_bComplete)
													g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2899), TRUE, _XDEF_TRAINING_DUMMY_START); //_T("무공 수련을 시작하겠습니까?")
											}
										}
									}
								}
							}
						}
						else
						{
							// 다른 타겟
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
							m_bGuardMob =false;
#endif
						}
					} //  else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
					else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
					{
						// 두번째 클릭
						if(m_ViewTarget == g_LastPickedObject)
						{
							_XSeat* pSeat = (_XSeat*)g_LastPickedObject;
							if(pSeat)
							{
								FLOAT dx = m_Position.x - pSeat->m_Position.x;
								FLOAT dz = m_Position.z - pSeat->m_Position.z;
								FLOAT distance= _XFC_sqrt(dx*dx + dz*dz);
								FLOAT range = 0.0f;

								SetAttackTarget(pSeat, _XPICKINGTARGET_SEAT);

								if(distance > range)
								{
									D3DXVECTOR3 directionvector = m_Position - pSeat->m_Position;
									D3DXVec3Normalize(&directionvector, &directionvector);

									D3DXVECTOR3 targetposition = pSeat->m_Position + (directionvector*range);
									targetposition.y = pSeat->m_Position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pSeat->m_Position);
									UpdatePosition();

									if(m_UserState != _XDEF_USERSTATE_TRAINING)
									{
										if(m_CurrentBattleState == _XUSER_STATE_NONE || m_CurrentBattleState == _XUSER_STATE_LAST)
										{
											g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2900), TRUE, _XDEF_TRAINING_SPELL_START); //_T("기공 수련을 시작하겠습니까?")
										}
									}
								}
							}
						}
						else
						{
							// 다른 타겟
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
							m_bGuardMob =false;
#endif
						}
					} // // else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
					else if(m_ViewTargetType == _XPICKINGTARGET_SELF)
					{
						if(m_ViewTarget != g_LastPickedObject)
						{
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
							m_bGuardMob =false;
#endif
						}
					}
					else if( m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
					{
						if(m_ViewTarget == g_LastPickedObject)
						{
							// 두번째 클릭							
							_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_ViewTarget;
							if( pFunctionalObject->m_HealthPercentage > 0 )
							{
								int ret = pProc_Maingame->StartFunctionalObjectScript();
								
								SetAttackTarget((_XItem*)pFunctionalObject, _XPICKINGTARGET_FUNCTIONALOBJECT);
								
								if( ret == 0 )
								{
									// OBJECT 쪽으로 이동
									D3DXVECTOR3 position = pFunctionalObject->m_Position;
									D3DXVECTOR3 directionvector = m_Position - position;
									D3DXVec3Normalize(&directionvector, &directionvector);
									
									D3DXVECTOR3 targetposition = position + (directionvector*(g_LodTerrain.m_MeshObjectManager.m_ModelList[ pFunctionalObject->m_ObjectIndex ]->OBB_Extent[2]*2 + 2.5f));
									targetposition.y = position.y;
									SetTargetPositionAngle(targetposition);
									InsertTargetPositionEffect();
								}
								else
								{
									if( !pFunctionalObject->m_bAttackable )
									{
										g_CurrentFocusedObject = _XDEF_DUMMYID_FUNCTIONALOBJECT;
										SetTargetPositionAngle(m_Position);								
									}
									else
									{										
										// 몹 옆에서 더블 클릭
										if(pProc_Maingame->SelectSkill(m_ActiveSlotID, FALSE))
										{
											// active skill 시작
										}
									}									
								}
							}
						}
						else
						{
							// 다른 타겟
#ifdef _XTS_PK
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType, SetPKReadyMode());
#else
							SetViewTarget(g_LastPickedObject, g_LastPickedObjectType);
#endif
							m_bLeftClickProcess = TRUE;
#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
							m_bGuardMob =false;
#endif
						}
					}//else if( m_ViewTargetType = _XPICKINGTARGET_FUNCTIONALOBJECT )
				}
			}
		} // if(g_LastPickedObject)
		else if(!m_bLeftClickProcess)
		{
			if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView) // 2004.06.03->oneway48 insert
			{
				if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL && 
					(m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7))
				{
					// 마우스 클릭으로 움직일 수 없음
				}
				else
				{
					// 다른 곳을 클릭했을 경우 아이템 집기 취소
					m_bGrabItem = FALSE;

#ifdef _XDEF_NPC_ATTACK_CHECK //Author : 양희왕 //breif : 베트남 shift 클릭
					m_bGuardMob = false;
#endif

					ResetAttackTarget();
					pProc_Maingame->m_bAutoNPCScriptPopUp = FALSE;
					if(CheckValidNextPosition())
					{
						SetTargetPositionAngle(g_vMousePosition);
						InsertTargetPositionEffect();
					}
					else
					{
						if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE && GetMotionClass() != _XACTION_REBIRTH)
						{
							m_TargetPosition = m_Position;
						}
					}

					// 상태효과로 이동 불능일 경우 메세지 출력
					if(m_Lock.movelock == 1)
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2898), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS); //"상태 이상으로 인하여 이동이 불가능 합니다."
					}
				}
			}
		}

		ProcessBattleChannelingNextStep();

	} // if(g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND && mousestate->bButton[0] && !m_bLeftClickProcess)
}

BOOL _XLocalUser::CheckValidNextPosition(void)
{
	if(((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.GetMousePosition())
	{
		// 피킹포인트를 계산한다. 마우스를 떼기 전까지는 재계산 안함.
		g_LodTerrain.GetPickedPoint();

		if(g_vMousePosition.x > 0.0f)
		{
			if(g_vMousePosition.x > (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingWidth))
				g_vMousePosition.x = g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingWidth;
		}
		else
		{
			if(g_vMousePosition.x < (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingWidth))
				g_vMousePosition.x = -g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingWidth;
		}

		if(g_vMousePosition.z > 0.0f)
		{
			if(g_vMousePosition.z > (g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingHeight))
				g_vMousePosition.z = g_LodTerrain.m_HalfWorldSize - g_LodTerrain.m_EdgeBoundingHeight;
		}
		else
		{
			if(g_vMousePosition.z < (-g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingHeight))
				g_vMousePosition.z = -g_LodTerrain.m_HalfWorldSize + g_LodTerrain.m_EdgeBoundingHeight;
		}

		FLOAT minimumdistance;
		if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN)
			minimumdistance = _XDEF_MINIMUMRUNDISTANCE;

		FLOAT dx = g_vMousePosition.x - m_Position.x;
		FLOAT dy = g_vMousePosition.y - m_Position.y;
		FLOAT dz = g_vMousePosition.z - m_Position.z;
		FLOAT distance = _XFC_sqrt(dx*dx + dy*dy + dz*dz);

		if(distance < minimumdistance)
		{
			FLOAT ddx = g_vMousePosition.x - m_TargetPosition.x;
			FLOAT ddy = g_vMousePosition.y - m_TargetPosition.y;
			FLOAT ddz = g_vMousePosition.z - m_TargetPosition.z;
			FLOAT ddis = _XFC_sqrt(ddx*ddx + ddz*ddz + ddy*ddy);

			if(ddis < _XDEF_MINIMUMRUNDISTANCE && GetMotionClass() == _XACTION_MOVE)
			{
			}
			else
			{
				m_PrevMoveTime = 0;
				return TRUE; // Change 2005/11/15 kerygma FALSE -> TRUE ///////????
			}
		}

		return TRUE;
	}

	return FALSE;
}

void _XLocalUser::ProcessNextMovePosition(void)
{
	if( GetMotionClass () == _XACTION_DIE || GetMotionClass () == _XACTION_DYING )
		return;
	
	// position과 targetposition이 같음
	
	if( GetMotionClass() == _XACTION_MOVE &&
		(m_Position.x == m_TargetPosition.x && m_Position.z == m_TargetPosition.z) )   // 타겟 포지션과 좌표가 같으면 정지 또는 남은 패스 이동 
	{
		if(!gpInput->GetMouseState()->bButton[0])
		{
			if(m_LastRotateAngle == m_RotateAngle && !m_UseSpeedSkill)
			{
				if( m_PathNodeCount > 0 ) // 패스가 계산되었다면 
				{
					if( m_CurrentNavigatePathNodeIndex < m_PathNodeCount-1 ) // 남은 패스 포인트가 있다면 다음 포인트 설정하고 계속 이동
					{
						m_CurrentNavigatePathNodeIndex++;
						
						if( m_CurrentNavigatePathNodeIndex == m_PathNodeCount-1 )
						{
							m_TargetPosition.x = m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.x;
							m_TargetPosition.z = m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.y;
						}
						else
						{
							m_TargetPosition.x = ( m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
							m_TargetPosition.z = ( m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
						}
						
						m_LastRotateAngle = _XMeshMath_GetRotateRadian(m_Position, m_TargetPosition);
						
						FLOAT elapsedangle_1 = fabs(m_LastRotateAngle - m_RotateAngle);
						FLOAT elapsedangle_2 = (_X_PI - fabs(m_LastRotateAngle)) + (_X_PI - fabs(m_RotateAngle));
						
						if(elapsedangle_1 <= elapsedangle_2)
						{
							if(m_LastRotateAngle >= m_RotateAngle)
								m_RotateDirection = _XROTATE_CLOCKWISE;
							else
								m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
						}
						else
						{
							if(m_LastRotateAngle >= m_RotateAngle)
								m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
							else
								m_RotateDirection = _XROTATE_CLOCKWISE;
						}
					}
					else
					{
						UpdatePosition();
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("[LocalUser/ProcessNextMovePosition1] Send Move Pakcet");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
				else
				{
					UpdatePosition();

					if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
					{
						if(m_CurrentBattleState != _XUSER_STATE_NONE && m_CurrentBattleState != _XUSER_STATE_LAST)
						{
							// motion change 하지 않음
						}
						else
						{
							SetAnimation(_XACTION_IDLE);
//							_XDWINPRINT("[LocalUser/ProcessNextMovePosition2] Send Move Pakcet");
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						}
					}
					else
					{
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("[LocalUser/ProcessNextMovePosition2] Send Move Pakcet");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
				
				if(g_pMinimap_Window)
				{
					g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
					g_pMinimap_Window->SetLocalUserPosition();
				}
			}			
			else 
			{			
				UpdateRotateAngle();
				UpdatePosition();
			}
		} // if(GetMotionClass() == _XACTION_MOVE)
		else
		{
			UpdatePosition();
			
			if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
			{
				if(m_CurrentBattleState != _XUSER_STATE_NONE && m_CurrentBattleState != _XUSER_STATE_LAST)
				{
					// motion change 하지 않음
				}
				else
				{
					SetAnimation(_XACTION_IDLE);
//					_XDWINPRINT("[LocalUser/ProcessNextMovePosition2] Send Move Pakcet");
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}
			}
			else
			{
				SetAnimation(_XACTION_IDLE);
//				_XDWINPRINT("[LocalUser/ProcessNextMovePosition2] Send Move Pakcet");
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
	}
	
	if( m_Position.x != m_TargetPosition.x || m_Position.z != m_TargetPosition.z ) // 타겟 포지션과 좌표가 다르면 이동 처리
	{
		D3DXVECTOR3 nextposition = m_Position;
		if(!GetNextMove(nextposition))
		{
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL && (m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7))
			{
				// finish skill중 - 애니매이션 변경하지 않음
			}
			else
			{
				if(GetMotionClass() != _XACTION_IDLE)
				{
					// 더 이상 갈 수 없음
					SetAnimation(_XACTION_IDLE);
//					_XDWINPRINT("============= GetNextMove Fail : Send Move packet");
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}
			}
		}
		// 이동중일 경우 다음 position을 적용
		if(GetMotionClass() == _XACTION_MOVE)
		{
			// Target과 멀어질 경우 Target 삭제
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_NPC)
				{
					_XNPCObject* pNPC = (_XNPCObject*)m_AttackTarget;
					if(pNPC->m_Distance > _XDEF_NPC_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if(pUser->m_ModelDescriptor.m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					if(pMob->m_ModelDescriptor.m_CameraDistance > g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_AttackTarget;
					if(pFunctionalObject->m_ModelDescriptor.m_CameraDistance > g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_DUMMY)
				{
					_XDummy* pDummy = (_XDummy*)m_AttackTarget;
					if(pDummy->m_ModelDescriptor.m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_SEAT)
				{
					_XSeat* pSeat = (_XSeat*)m_AttackTarget;
					if(pSeat->m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetAttackTarget();
					}
				}
			}
			if(m_ViewTarget)
			{
				if(m_ViewTargetType == _XPICKINGTARGET_NPC)
				{
					_XNPCObject* pNPC = (_XNPCObject*)m_ViewTarget;
					if(pNPC->m_Distance > _XDEF_NPC_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_ViewTarget;
					if(pUser->m_ModelDescriptor.m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_MOB)
				{
					_XMob* pMob = (_XMob*)m_ViewTarget;
					if(pMob->m_ModelDescriptor.m_CameraDistance > g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_ViewTarget;
					if(pFunctionalObject->m_ModelDescriptor.m_CameraDistance > g_LodTerrain.m_3PCamera.GetDistance() + _XDEF_MOB_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_DUMMY)
				{
					_XDummy* pDummy = (_XDummy*)m_ViewTarget;
					if(pDummy->m_ModelDescriptor.m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
				else if(m_ViewTargetType == _XPICKINGTARGET_SEAT)
				{
					_XSeat* pSeat = (_XSeat*)m_ViewTarget;
					if(pSeat->m_CameraDistance > _XDEF_USER_CULLINGRADIUS)
					{
						ResetViewTarget();
					}
				}
			}

			if( g_FreeMoveMode )
			{
				m_Position.x = nextposition.x;
				if(m_Position.y < nextposition.y )
					m_Position.y = nextposition.y;
				m_Position.z = nextposition.z;
			}
			else
			{
				m_Position = nextposition;
			}

			UpdateRotateAngle();
			UpdatePosition();

			if(g_pMinimap_Window)
			{
				g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
				g_pMinimap_Window->SetLocalUserPosition();
			}

			// 경공을 사용하면 0.3초에 한번씩 패킷 전송하도록 변경
			if(m_UseSpeedSkill)
			{
				if((g_LocalSystemTime - m_PrevPacketSendedTime) >= 300)
				{
					if(GetMotionClass() == _XACTION_MOVE && m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG)
					{
//						_XDWINPRINT(" SEND MOVE PACKET [LocalUSer->ProcessBattle] -use speed skill 0.3 Delay");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
			}
			else
			{
				// 0.5초마다 한번씩 패킷 전송
//				if(g_GameTimer.FrameSkip(XTIMER_SENDMOVEMESSAGE, XTIMER_SENDMOVEMESSAGETIME))
				if((g_LocalSystemTime - m_PrevPacketSendedTime) >= 500)
				{
					if(GetMotionClass() == _XACTION_MOVE)
					{
//						_XDWINPRINT(" SEND MOVE PACKET [LocalUSer->ProcessBattle] 0.5 Delay");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
			}

			if(!g_QuestScriptManager.GetQuestCameraMode() )
			{
				g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, 
																	m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, 
																	m_Position.z));
				g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
				g_LodTerrain.m_3PCamera.UpdateProjMatrix();
				g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				g_LodTerrain.RebuildLevel();
			}
		}
		else if(GetMotionClass() == _XACTION_DYING)
		{
			m_Position = nextposition;
			UpdatePosition();
			
			if(g_pMinimap_Window)
			{
				g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
				g_pMinimap_Window->SetLocalUserPosition();
			}

			g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, 
																m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, 
																m_Position.z));
			g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
			g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
			g_LodTerrain.RebuildLevel();
		}
		else if(GetMotionClass() == _XACTION_ATTACK)
		{
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL)
			{
				if(m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7)
				{
					m_Position = nextposition;
					UpdatePosition();
					
					if(g_pMinimap_Window)
					{
						g_pMinimap_Window->SetRotateArrow(m_RotateAngle + _X_PI);
						g_pMinimap_Window->SetLocalUserPosition();
					}
					
					g_LodTerrain.m_3PCamera.SetTargetPosition(D3DXVECTOR3(m_Position.x, 
						m_Position.y + _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, 
						m_Position.z));
					g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
					g_LodTerrain.RebuildLevel();
				}
			}
		}
	}
	else
	{
		if(GetMotionClass() == _XACTION_MOVE)
		{
			// 예외처리 - 제자리 뛰기 중
			SetAnimation(_XACTION_IDLE);
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
	}
}

// by mahwang
void _XLocalUser::ProcessCollectEvent(void)
{
	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	POINT mousept = { gpInput->GetMouseVirScreenPos()->x, gpInput->GetMouseVirScreenPos()->z };
	pMainGameProcess->GetVectorFromMousePos( mousept, g_vPickRayDir, g_vPickRayOrig );

	g_LodTerrain.m_MeshObjectManager.IntersectionEventObject(g_vPickRayOrig, g_vPickRayDir);

}
void _XLocalUser::ProcessAnimation(void)
{
	if(GetMotionClass() == _XACTION_IDLE)
	{
		// Emotion
		if( m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_CRINGESTART )
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_EMOTION_CRINGEREPEAT,FALSE);// Repeate
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if( 
#ifdef ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA
				 m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_BOW ||
				 m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_BOWPOLITELY ||
#endif
				 (m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
				  m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY ) )
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				ReloadWeaponModel();
				SetAnimation( _XACTION_IDLE );
			}			
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_KNEE_SWORD_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_KNEE_SWORD_REPEAT, TRUE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART ||
			m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNEND ||
			m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_CRINGEREPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_KNEE_SWORD_REPEAT ||
			m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_REPEAT)
		{
			// 처리 하면 안됨 - 수련 용
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_ModelDescriptor.SetLockEndFrame(FALSE);
				SetAnimationIndex(_XAN_MATCH_PR_REPEAT, TRUE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_END)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM5, NULL);
				ReloadWeaponModel();
				SetAnimation( _XACTION_IDLE );
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_WINNER_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_MATCH_WINNER_REPEAT);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else if(m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_LOSER_START)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				SetAnimationIndex(_XAN_MATCH_LOSER_REPEAT);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else 
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
#ifdef _XDEF_NEWANIMATIONLIST
				/*
				if( m_SelectedSkillID )
				{
					if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
						g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
					{
						int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
											  g_SkillProperty[m_SelectedSkillID]->motion_male : 
											  g_SkillProperty[m_SelectedSkillID]->motion_female;
						
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{
						}
						else
						{
							_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
							
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
								pWeaponItemProperty = g_WeaponItemProperty;
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
								pWeaponItemProperty = g_WeaponItemProperty2;
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
								pWeaponItemProperty = g_WeaponItemProperty3;
							
							switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// 단병기
								if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
								}
								else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
									else
										baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
								}
								else
								{
									baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// 장병기
								baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
								break;				
							case _XGI_SC_WEAPON_SOFT				:				// 연병기
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
								break;
							case _XGI_SC_WEAPON_HIDDEN				:				// 암기
								break;
							case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
								break;
							case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
								break;
							}
						}
						
						if( m_ModelDescriptor.m_CurMotion != baseanimindex+2 ) // 경공 마지막 동작 
						{
							SetAnimation( _XACTION_IDLE );
						}
					}
					{
						SetAnimation( _XACTION_IDLE );
					}
				}
				else
				*/
#endif
				{
					SetAnimation( _XACTION_IDLE );
				}
			}
		}		

		if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
		{
			if(m_SpecialActionTimer == 0)
				m_SpecialActionTimer = g_LocalSystemTime;

			if((g_LocalSystemTime - m_SpecialActionTimer) > _XDEF_CHANGEIDLEMOTIONTIME)
			{
				m_SpecialActionTimer = g_LocalSystemTime;

				switch(m_ModelDescriptor.GetCurMotion())
				{
				case _XAN_IDLE_NONE :
					{
						SetAnimationIndex(_XAN_IDLE_NONE_ACTION, TRUE);
					}
					break;				
				case _XAN_IDLE_SHORTWEAPON :
					{
						SetAnimationIndex(_XAN_IDLE_SHORTWEAPON_ACTION, TRUE);
					}
					break;				
				case _XAN_IDLE_LONGWEAPON :
					{
						SetAnimationIndex(_XAN_IDLE_LONGWEAPON_ACTION, TRUE);
					}
					break;
				case _XAN_IDLE_SWORD_PEACE :
					{
						SetAnimationIndex(_XAN_IDLE_SWORD_ACTION, TRUE);
					}
					break;
				}
			}

			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.GetCurMotion())
				{
				case _XAN_IDLE_NONE_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;
				case _XAN_IDLE_SHORTWEAPON_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;				
				case _XAN_IDLE_LONGWEAPON_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;
				case _XAN_IDLE_SWORD_ACTION :
					{
						SetAnimation(_XACTION_IDLE);
					}
					break;
				}
			}
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_MOVE)
	{
		if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG)
		{
#ifdef _XDEF_NEWANIMATIONLIST

			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{

				int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
									  g_SkillProperty[m_SelectedSkillID]->motion_male : 
									  g_SkillProperty[m_SelectedSkillID]->motion_female;


				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// 단병기
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
									else
										baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
								}
								else
								{
									baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// 장병기
								baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
								break;				
							case _XGI_SC_WEAPON_SOFT				:				// 연병기
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
								break;
							case _XGI_SC_WEAPON_HIDDEN				:				// 암기
								break;
							case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
								break;
							case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
								break;
							}
						}
					}
				}
#else
					  _XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
					  
					  if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
						  pWeaponItemProperty = g_WeaponItemProperty;
					  else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
						  pWeaponItemProperty = g_WeaponItemProperty2;
					  else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
						  pWeaponItemProperty = g_WeaponItemProperty3;
					  
					  switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					  {
					  case _XGI_SC_WEAPON_SHORT				:				// 단병기
						  if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						  {
						  }
						  else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						  {
							  if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								  baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
							  else
								  baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
						  }
						  else
						  {
							  baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
						  }
						  break;
					  case _XGI_SC_WEAPON_LONG				:				// 장병기
						  baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
						  break;				
					  case _XGI_SC_WEAPON_SOFT				:				// 연병기
						  baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
						  break;
					  case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						  break;
					  case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						  break;
					  case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						  break;
					  }
				}
#endif
				if( m_ModelDescriptor.m_CurMotion == baseanimindex )
				{
					SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1);// Repeate
				}
				//else if( m_ModelDescriptor.m_CurMotion == baseanimindex+1 )
				//			{
				//
				//			}			
				else if( m_ModelDescriptor.m_CurMotion == baseanimindex+2 )
				{
					SetAnimation(_XACTION_IDLE);
					m_UseSpeedSkill = FALSE;
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
					m_TargetPosition = m_Position;
					
					if( m_SelectedSkillID )
					{
						if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
							_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
							g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
						{
							m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
							m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);
						}
					}
				}
			
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}

#else //_XDEF_NEWANIMATIONLIST
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.m_CurMotion)
				{
				case _XAN_FASTMOVE_NONE_START		 :
				case _XAN_FASTMOVE_SHORTWEAPON_START :
				case _XAN_FASTMOVE_LONGWEAPON_START  :
				case _XAN_FASTMOVE_SOFTWEAPON_START  :
				case _XAN_FASTMOVE_SWORD_START_PEACE :
				case _XAN_FASTMOVE_SWORD_START_BATTLE :
					{
						SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1);// Repeate
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				break;
				/*
				case _XAN_FASTMOVE_NONE_REPEAT		 :
				case _XAN_FASTMOVE_SHORTWEAPON_REPEAT:
				case _XAN_FASTMOVE_LONGWEAPON_REPEAT :
				case _XAN_FASTMOVE_SOFTWEAPON_REPEAT :
					{
					}
				break;*/

				case _XAN_FASTMOVE_NONE_END			 :
				case _XAN_FASTMOVE_SHORTWEAPON_END	 :
				case _XAN_FASTMOVE_LONGWEAPON_END	 :
				case _XAN_FASTMOVE_SOFTWEAPON_END	 :
				case _XAN_FASTMOVE_SWORD_END_PEACE :
				case _XAN_FASTMOVE_SWORD_END_BATTLE :
					{
						SetAnimation(_XACTION_IDLE);
						m_UseSpeedSkill = FALSE;
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						m_TargetPosition = m_Position;
						
						if( m_SelectedSkillID )
						{
							if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
								_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
								g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
							{
								m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
								m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
							}
						}
					}
					break;
				}
			}
#endif //_XDEF_NEWANIMATIONLIST
		}
		else if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN)
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				switch(m_ModelDescriptor.m_CurMotion)
				{
				case _XAN_MOVE_NONE_START		 :
				case _XAN_MOVE_SHORTWEAPON_START :
				case _XAN_MOVE_LONGWEAPON_START  :
				case _XAN_MOVE_SOFTWEAPON_START  :
					{
						SetAnimationIndex(m_ModelDescriptor.m_CurMotion+1);// Repeate						
					}
					break;
					
				/*case _XAN_MOVE_NONE_REPEAT		 :
				case _XAN_MOVE_SHORTWEAPON_REPEAT:
				case _XAN_MOVE_LONGWEAPON_REPEAT :
				case _XAN_MOVE_SOFTWEAPON_REPEAT :
					{						
					}
					break;*/
					
				case _XAN_MOVE_NONE_END			 :
				case _XAN_MOVE_SHORTWEAPON_END	 :
				case _XAN_MOVE_LONGWEAPON_END	 :
				case _XAN_MOVE_SOFTWEAPON_END	 :
					{
						SetAnimation(_XACTION_IDLE);						
					}
					break;
				}
			}
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_DYING)
	{
		m_ModelDescriptor.SetLockEndFrame(TRUE);

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.SetLockEndFrame(FALSE);
			switch(m_ModelDescriptor.m_CurMotion)
			{
			case _XAN_DYINGREADY_NONE :
				{
					SetAnimationIndex(_XAN_DYINGREPEAT_NONE);
				}
				break;
			case _XAN_DYINGREADY_SHORTWEAPON :
				{
					SetAnimationIndex(_XAN_DYINGREPEAT_SHORTWEAPON);
				}
				break;
			case _XAN_DYINGREADY_LONGWEAPON :
				{
					SetAnimationIndex(_XAN_DYINGREPEAT_LONGWEAPON);
				}
				break;
			case _XAN_DYINGREADY_SWORD_PEACE :
				SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_PEACE);
				break;
			case _XAN_DYINGREADY_SWORD_BATTLE :
				SetAnimationIndex(_XAN_DYINGREPEAT_SWORD_BATTLE);
				break;
			}
			
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
				m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, _XDEF_PLAYERID_DYINGWAIT, &m_ModelDescriptor.m_Position );
			}
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_DIE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
		{
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			}

			int rand = _XRandom() % 2;
			
			if( m_CharacterInfo.gender == _XGENDER_MAN )
				_XPlaySoundEffect( g_DieEffectSoundIndex[0][rand] , m_Position );
			else 
				_XPlaySoundEffect( g_DieEffectSoundIndex[1][rand] , m_Position );
		}

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_ModelDescriptor.SetLockEndFrame(TRUE);
		}
	}
	else if(GetMotionClass() == _XACTION_RESCUE)
	{		
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			}
			SetAnimation(_XACTION_IDLE);
		}
		
		// display 되지 않은 전투 결과 처리
		ProcessMobDie();

	}
	else if(GetMotionClass() == _XACTION_LPS)
	{

		if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE )
		{
			// 나중에 LPS동작 처리가 들어가면 따로 분리한다.
			if(m_ViewTargetType == _XPICKINGTARGET_MOB)
			{
				// 두번째 클릭
				_XMob* pMob = (_XMob*)m_ViewTarget;
				ProcessLPSSystem(pMob);
			}
		}

		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
		}
	}		
	else if(GetMotionClass() == _XACTION_REBIRTH)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if( m_ModelDescriptor.m_EffectManager )
			{
				m_ModelDescriptor.m_EffectManager->DeleteInstance( _XDEF_PLAYERID_DYINGWAIT );
			}

#ifdef _XDEF_USESCREEENMOTIONBLUR 
			if(g_ScrrenMotionBlurMode)
				g_ScrrenMotionBlurMode = FALSE;
#endif
			
			SetAnimation(_XACTION_IDLE);
		}

		// display 되지 않은 전투 결과 처리
		ProcessMobDie();
	}
	else if(GetMotionClass() == _XACTION_MATCH_START)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
		}
	}
	else if(GetMotionClass() == _XACTION_MATCH_CANCEL)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			SetAnimation(_XACTION_IDLE);
	}
#ifdef _XTS_NEWINSENSIBLE
	else if(GetMotionClass() == _XAN_EMOTION_CLAPPINGHAND)
	{
		// 기절 - 처리 하지 않음
	}
#endif

	// by mahwang
	else if(GetMotionClass() == _XACTION_COLLECT)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if(m_CurrentWeaponID > 0)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);

#ifdef _XDEF_MODELTHREADLOADING
					if( WeaponItem && WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
					}
#else
					if( WeaponItem )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
#endif
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
#ifdef _XDEF_MODELTHREADLOADING
					if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );			
					}

#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
#ifdef _XDEF_MODELTHREADLOADING
					if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
#ifdef _XDEF_MODELTHREADLOADING
					if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex > 0  )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				}
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
			else
			{
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			}

			SetAnimation(_XACTION_IDLE);
		}
	}
}

void _XLocalUser::ProcessParameter(FLOAT progresstime)
{
	// 전투쪽 parameter 처리 할 것
	/*	_XWindow_FSWindow* pFSWindow = (_XWindow_FSWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_FSWINDOW);
	if(!pFSWindow)
	return;
	
	  if(GetMotionClass() == _XACTION_MOVE)
	  {
	  if(m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN)
	  {
	  if((int)m_CharacterInfo.junggipower <= 0)
	  m_CharacterInfo.junggipower = 0.0f;
	  else
	  {
	  m_CharacterInfo.junggipower -= (_XDEF_HEALTHDECREASERATE_RUN * progresstime);
	  }
	  }
	  else
	  {
	  if((int)m_CharacterInfo.junggipower >= m_CharacterInfo.max_junggipower)
	  m_CharacterInfo.junggipower = m_CharacterInfo.max_junggipower;
	  else
	  m_CharacterInfo.junggipower += (_XDEF_HEALTHINCREASERATE_WALK * progresstime);
	  }
	  
		if((int)m_CharacterInfo.junggipower <= 0)
		{
		if(m_CharacterInfo.animationdescript.movetype != _XMOVETYPE_WALK)
		{
		m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_WALK;
		SetAnimation(_XACTION_MOVE);
		SetMoveSpeed(_XDEF_CHARACTERMOVESPEED_WALK);
		g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
		}
		pFSWindow->SetParameter();
		}
		else if(GetMotionClass() == _XACTION_IDLE)
		{
		if(m_CharacterInfo.junggipower >= (FLOAT)m_CharacterInfo.max_junggipower)
		{
		m_CharacterInfo.junggipower = m_CharacterInfo.max_junggipower;
		pFSWindow->SetParameter();
		}
		else
		{
		m_CharacterInfo.junggipower += (_XDEF_HEALTHINCREASERATE_IDLE * progresstime);
		pFSWindow->SetParameter();
		}
	}*/
}

void _XLocalUser::ProcessInternalUse(void)
{	
	switch(m_CurrentInternalUseState)
	{
	case _XUSER_STATE_1 :
		{
			SetAnimationIndex(_XAN_EMOTION_SITDOWNSTART);
			// 들고 있는 아이템을 삭제
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					if(WeaponItem[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
					{
						m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
						m_CharacterInfo.modeldescript.item1 = 0;
						m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
						m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				if(g_WeaponItemProperty[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}	
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				if(g_WeaponItemProperty2[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				if(g_WeaponItemProperty3[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel( _XDEF_MODELCLUSTER_ITEM1, NULL );
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}
#endif
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			m_CurrentInternalUseState = _XUSER_STATE_2;			
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNREPEAT, FALSE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					if( m_bInternalUse )
						m_CurrentInternalUseState = _XUSER_STATE_3;					
					else 
						m_CurrentInternalUseState = _XUSER_STATE_5;

					g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
									_XDEF_MONSTERID_YAK, &g_pLocalUser->m_ModelDescriptor.m_Position );
				}
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			if( g_pInventory_Window )
			{
				g_pLocalUser->UseItem( g_pInventory_Window->m_ElixirInvenNumber );
				g_pInventory_Window->m_ElixirInvenNumber = -1;				
				m_ElixirSoundRepeatTime = g_LocalSystemTime;
				_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_RUN_WAV);
			}
		}
		break;
	case _XUSER_STATE_4 :
		{
			if( g_LocalSystemTime - m_ElixirSoundRepeatTime > 2000 )
				_XPlayInterfaceSound(ID_SR_INTERFACE_ELIXIR_RUN_WAV);
		}
		break;
	case _XUSER_STATE_5 :
		{
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_MONSTERID_YAK);

			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT || m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNEND);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentInternalUseState = _XUSER_STATE_6;
				}
			}
			else
				m_CurrentInternalUseState = _XUSER_STATE_6;
		}
		break;
	case _XUSER_STATE_6 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNEND)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// 들고 있던 무기 복구
					ReloadWeaponModel();
					SetAnimation(_XACTION_IDLE);
					m_CurrentInternalUseState = _XUSER_STATE_NONE;
					m_bInternalUse = FALSE;
				}
			}
			else
			{
				m_bInternalUse = FALSE;
				m_CurrentInternalUseState = _XUSER_STATE_NONE;
			}
		}
		break;
	case _XUSER_STATE_7 :
		{	
			g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_MONSTERID_YAK);
			
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT || m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_ELIXIR_FAIL);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentInternalUseState = _XUSER_STATE_8;
				}
			}
			else
				m_CurrentInternalUseState = _XUSER_STATE_8;
		}
		break;
	case _XUSER_STATE_8 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_ELIXIR_FAIL )
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNEND);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentInternalUseState = _XUSER_STATE_6;
				}
			}
			else
				m_CurrentInternalUseState = _XUSER_STATE_6;
		}
		break;
	}
}

void _XLocalUser::ProcessReadBook(void)
{
	switch(m_CurrentReadBookState)
	{
	case _XUSER_STATE_1 :
		{
			SetAnimationIndex(_XAN_EMOTION_SITDOWNSTART);
			DeleteWeaponModel();
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			m_CurrentReadBookState = _XUSER_STATE_2;			
		}
		break;
	case _XUSER_STATE_2 :
		{
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
			{
				if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNREPEAT, FALSE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					if( m_bReadingBook )
						m_CurrentReadBookState = _XUSER_STATE_3;					
					else 
						m_CurrentReadBookState = _XUSER_STATE_5;
				}
			}
		}
		break;
	case _XUSER_STATE_3 :
		{
			if( g_pInventory_Window )
			{
				g_pLocalUser->UseItem( g_pInventory_Window->m_BookInvenNumber );
				g_pInventory_Window->m_BookInvenNumber = -1;
			}
		}
		break;
	case _XUSER_STATE_4 :
		{
		}
		break;
	case _XUSER_STATE_5 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNREPEAT || m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNSTART)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					SetAnimationIndex(_XAN_EMOTION_SITDOWNEND);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_CurrentReadBookState = _XUSER_STATE_6;
				}
			}
			else
				m_CurrentReadBookState = _XUSER_STATE_6;
		}
		break;
	case _XUSER_STATE_6 :
		{
			if(m_ModelDescriptor.GetCurMotion() == _XAN_EMOTION_SITDOWNEND)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// 들고 있던 무기 복구
					ReloadWeaponModel();
					SetAnimation(_XACTION_IDLE);
					m_CurrentReadBookState = _XUSER_STATE_NONE;
					m_bReadingBook = FALSE;
				}
			}
			else
			{
				m_bReadingBook = FALSE;
				m_CurrentReadBookState = _XUSER_STATE_NONE;
			}
		}
		break;
	}
}

void _XLocalUser::ProcessSpeedSkill(void)
{
	if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
	{
		m_CurrentBattleState = _XUSER_STATE_NONE;
		m_HiperRunFailedFlag = TRUE;
		return;
	}

	if(!g_QuestScriptManager.GetQuestCameraMode())
	{
		XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;		
		if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
		{
			if( gpInput->CheckKeyPress(DIK_LMENU) && 
				!m_HiperRunFailedFlag && 
				pProc_Maingame->SelectSkill( m_SelectedSkillID ) )
			{
				

				if( m_CharacterInfo.Get_current_forcepower() <= 0 )
				{
					if( m_UseSpeedSkill )
					{
						ResetSkillAnimation();
						m_UseSpeedSkill = FALSE;
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("============= 경공 끝 ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						
						if( m_SelectedSkillID )
						{
							if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
								_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
								g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
							{
								m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
								m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
							}
						}
					}
				}
				// Check hiper run cooldown time
				else if( m_SelectedSkillItem->m_dwCooldownStartTime > 0 )
				{
					if( m_UseSpeedSkill )
					{
						ResetSkillAnimation();
						m_UseSpeedSkill = FALSE;
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("============= 경공 끝 ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
				}
				else if( m_bReadingBook )	// 비급 읽는 중이면 리턴
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2901), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //"비급을 읽는 중에는 경공(신법)을 사용할 수 없습니다."
				}
				else if( m_bSocketPlugIn )	// 소켓 장착 중이면 리턴
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2902), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //"장식물을 다는 중에는 경공(신법)을 사용할 수 없습니다."
				}
				else if( m_bGambleBoxOpen )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2903), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //"패왕궤를 여는 중에는 경공(신법)을 사용할 수 없습니다."
				}
				else if( g_pLocalUser->m_bInternalUse )	// 영약 복용 중이면 리턴
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2904), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 ); //"영약복용 중에는 경공(신법)을 사용할 수 없습니다."
				}
				else
				{
					if(g_LodTerrain.m_3PCamera.mp_fFov == g_DefaultCameraFov)
					{
						if( !m_UseSpeedSkill )//m_CharacterInfo.animationdescript.movetype != _XMOVETYPE_KYUNGGONG)
						{
							m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_KYUNGGONG;
							m_UseSpeedSkill = TRUE;

							if( m_SelectedSkillID && m_SelectedSkillItem )
							{
								if( m_SelectedSkillItem->m_cSkillLevel >= 11 ) // 11성 이상일때만 Radial blur effect 보임
								{
									g_ObjectBlurProcessManager.SetBlurStep( 1 );
								}
							}
						}

						g_PrevMoveType = m_CharacterInfo.animationdescript.movetype;
						SetAttackTarget(NULL, _XPICKINGTARGET_NONE);
						g_LastPickedObject = NULL;
						g_LastPickedObjectType = _XPICKINGTARGET_NONE;
						SetAnimation(_XACTION_MOVE);
//						_XDWINPRINT("============= 경공 시작 ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					}
					if(g_LodTerrain.m_3PCamera.mp_fFov < _XDEF_MAXFOV)
					{
						g_LodTerrain.m_3PCamera.mp_fFov += _XDEF_FOVINCREASERATE;

						if( m_SelectedSkillID && m_SelectedSkillItem )
						{
							if( m_SelectedSkillItem->m_cSkillLevel >= 11 ) // 11성 이상일때만 Radial blur effect 보임
							{
								if( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps < 30 )
								{
									if( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps + 2 <= 30 )
									{
										g_ObjectBlurProcessManager.SetBlurStep( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps + 2 );
									}
									else
									{
										g_ObjectBlurProcessManager.SetBlurStep( 30 );
									}
								}
								else
								{
									g_ObjectBlurProcessManager.SetBlurStep( 30 );
								}
							}
						}
					}

					g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);

					/*
					if(m_UseSpeedSkill)
					{
						if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
						{
							switch(m_ModelDescriptor.m_CurMotion)
							{
							case _XAN_MOVE_NONE_START :
								{
									SetAnimationIndex(_XAN_MOVE_NONE_REPEAT);
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
								}
								break;
							case _XAN_MOVE_NONE_REPEAT :
								{
								}
								break;
							case _XAN_MOVE_SHORTWEAPON_START :
								{
									SetAnimationIndex(_XAN_MOVE_SHORTWEAPON_REPEAT);
				
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
								}
								break;
							case _XAN_MOVE_SHORTWEAPON_REPEAT :
								{
								}
								break;
							case _XAN_MOVE_LONGWEAPON_START :
								{
									SetAnimationIndex(_XAN_MOVE_LONGWEAPON_REPEAT);
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
								}
								break;
							case _XAN_MOVE_LONGWEAPON_REPEAT :
								{
								}
								break;
							}
						}
					}
					*/
					
					D3DXVECTOR3 targetposition = m_Position + (g_LodTerrain.m_3PCamera.m_LookVector * 3.0f);
					SetTargetPositionAngle(targetposition);
				}
			}
			else
			{
				m_CurrentBattleState = _XUSER_STATE_NONE;
				
				if(g_LodTerrain.m_3PCamera.mp_fFov > g_DefaultCameraFov)
				{

#ifdef _XDEF_NEWANIMATIONLIST			
					int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
										  g_SkillProperty[m_SelectedSkillID]->motion_male : 
										  g_SkillProperty[m_SelectedSkillID]->motion_female;
					
					if( m_CharacterInfo.modeldescript.item1 == 0 )
					{
					}
					else
					{
#ifdef _XTS_ITEM_OPTIMIZATION
						if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
						{
							_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
							if( WeaponItem )
							{
								switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
									}
									else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
										else
											baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
									}
									else
									{
										baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									break;
								}
							}
						}
#else
						_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
						
						if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							pWeaponItemProperty = g_WeaponItemProperty;
						else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							pWeaponItemProperty = g_WeaponItemProperty2;
						else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							pWeaponItemProperty = g_WeaponItemProperty3;
						
						switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
							}
							else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
								else
									baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
							}
							else
							{
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							break;
						}
#endif
					}
					
					// Alt를 살짝 눌렀다 떼었을때
					if( m_ModelDescriptor.m_CurMotion == baseanimindex ) 
					{
						D3DXVECTOR3 targetposition;
						targetposition = m_Position + (g_LodTerrain.m_3PCamera.m_LookVector * 3.0f);
						SetTargetPositionAngle(targetposition);
						m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
						m_UseSpeedSkill = FALSE;
						SetAnimation(_XACTION_IDLE);
//						_XDWINPRINT("============= 경공 끝 ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
						
						if( m_SelectedSkillID )
						{
							if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
								_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
								g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
							{
								m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
								m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
							}
						}
					}
					// Repeat mode 종료
					else if( m_ModelDescriptor.m_CurMotion == baseanimindex+1 )
					{
						SetAnimationIndex(m_ModelDescriptor.m_CurMotion+2);
//						_XDWINPRINT("============= 경공 끝 ");
						g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);

						if( m_SelectedSkillID )
						{
							if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
								_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
								g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
							{
								m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
								m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
							}
						}
					}
#else
				
					switch( m_ModelDescriptor.m_CurMotion ) 
					{
					case  _XAN_FASTMOVE_NONE_REPEAT				:
						SetAnimationIndex(_XAN_FASTMOVE_NONE_END);
						//m_UseSpeedSkill = FALSE;
						break;
					case  _XAN_FASTMOVE_SHORTWEAPON_REPEAT		:	
						SetAnimationIndex(_XAN_FASTMOVE_SHORTWEAPON_END);
						//m_UseSpeedSkill = FALSE;
						break;
					case  _XAN_FASTMOVE_LONGWEAPON_REPEAT		:
						SetAnimationIndex(_XAN_FASTMOVE_LONGWEAPON_END);
						//m_UseSpeedSkill = FALSE;
						break;
					case  _XAN_FASTMOVE_SOFTWEAPON_REPEAT		:
						SetAnimationIndex(_XAN_FASTMOVE_SOFTWEAPON_END);
						//m_UseSpeedSkill = FALSE;
						break;
					case _XAN_FASTMOVE_SWORD_REPEAT_PEACE		:
						SetAnimationIndex(_XAN_FASTMOVE_SWORD_END_PEACE);
						break;
					case _XAN_FASTMOVE_SWORD_REPEAT_BATTLE		:
						SetAnimationIndex(_XAN_FASTMOVE_SWORD_END_BATTLE);
						break;
					// Alt를 살짝 눌렀다 떼었을때
					case _XAN_FASTMOVE_NONE_START :
					case _XAN_FASTMOVE_SHORTWEAPON_START :
					case _XAN_FASTMOVE_LONGWEAPON_START  :
					case _XAN_FASTMOVE_SOFTWEAPON_START  :
					case _XAN_FASTMOVE_SWORD_START_PEACE :
					case _XAN_FASTMOVE_SWORD_START_BATTLE :
						{
							D3DXVECTOR3 targetposition;
							targetposition = m_Position + (g_LodTerrain.m_3PCamera.m_LookVector * 3.0f);
							SetTargetPositionAngle(targetposition);
							m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
							m_UseSpeedSkill = FALSE;
							SetAnimation(_XACTION_IDLE);
							
							if( m_SelectedSkillID )
							{
								if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
									_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
									g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
								{
									m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
									m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
								}
							}
						}
						break;
					}
#endif					
					g_LodTerrain.m_3PCamera.mp_fFov -= _XDEF_FOVDECREASERATE;
					if(g_LodTerrain.m_3PCamera.mp_fFov <= g_DefaultCameraFov)
					{
						g_LodTerrain.m_3PCamera.mp_fFov = g_DefaultCameraFov;
						if(m_UseSpeedSkill)
						{
							D3DXVECTOR3 targetposition;
							targetposition = m_Position + (g_LodTerrain.m_3PCamera.m_LookVector * 3.0f);
							SetTargetPositionAngle(targetposition);
//							_XDWINPRINT("============= 경공 끝 ");
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							
							if( m_SelectedSkillID && m_SelectedSkillItem )
							{
								if( m_SelectedSkillItem->m_cSkillLevel >= 11 ) // 11성 이상일때만 Radial blur effect 보임
								{
									if( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps > 3 )
									{
										g_ObjectBlurProcessManager.SetBlurStep( g_ObjectBlurProcessManager.m_iNumRadialBlurSteps - 2 );
									}
									else
									{
										g_ObjectBlurProcessManager.SetBlurStep( 1 );
									}
								}
							}
						}
					}
					
					g_LodTerrain.m_3PCamera.UpdateViewMatrix(&g_LodTerrain);
					g_LodTerrain.m_3PCamera.UpdateProjMatrix();
					g_LodTerrain.m_ObjectQuadTree.SetCameraInfo(g_LodTerrain.m_3PCamera);
					g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
				} // if(g_LodTerrain.m_3PCamera.mp_fFov > g_DefaultCameraFov)			
			}
		} // if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView ) 
	} // if(!g_QuestScriptManager.GetQuestCameraMode())
	else
	{
		m_SelectedSkillID = 0;
		m_UseSpeedSkill = FALSE;
		m_CurrentBattleState = _XUSER_STATE_NONE;
	}
}

void _XLocalUser::Render( void )
{
	if(m_StopAnimation)
	{
		m_ModelDescriptor.SetLastFrame();
	}
	else
	{
		m_ModelDescriptor.Animation();
	}

	_XParticleActionBase::bLocalUser = TRUE;
	if( g_LodTerrain.m_3PCamera.m_CurrentTargetDistance >= 1.2f )
	{
		if( m_CharacterInfo.animationdescript.motionclass != _XACTION_DIE /*&& !m_pGoundCollideOBB*/ )
		{
			if( !g_FreeMoveMode )
				m_ModelDescriptor.RenderShadow();
		}

		// Camera collide alpha blending
		if( g_LodTerrain.m_3PCamera.m_CurrentTargetDistance < 1.5f )
		{
			g_RenderAlphaBlendFlag = TRUE;
			g_RenderAlphaBlendUseAlphaChannelFlag = TRUE;
			g_LastUsedRenderStyle = NULL;
			
			FLOAT distfactor = 255.0f * ( (g_LodTerrain.m_3PCamera.m_CurrentTargetDistance - 1.2f) / 0.3f );

#ifdef _XDWDEBUG
			extern BOOL g_MouseLockFlag;
			if( !g_MouseLockFlag )
			{
				g_RenderAlphaBlendTextureFactor = ( ( ((BYTE)distfactor) <<24) | 0xFFFFFF );
			}
			else
			{
				g_RenderAlphaBlendTextureFactor = 0xFFFFFFFF;
			}
#else
			g_RenderAlphaBlendTextureFactor = ( ( ((BYTE)distfactor) <<24) | 0xFFFFFF );
#endif
			
		}
		else
		{
			if( m_UseSpeedSkill || g_ObjectBlurProcessManager.m_iNumRadialBlurSteps > 1 )
			{
				if( m_UserTroubleState == _XDEF_CONVSTATE_FURY )
				{
					g_RenderAlphaBlendFlag = TRUE;
					g_RenderAlphaBlendUseAlphaChannelFlag = TRUE;
					g_LastUsedRenderStyle = NULL;
					g_RenderAlphaBlendTextureFactor = 0xA0FFFFFF;
				}
				else
				{
					if( m_SelectedSkillID )
					{
						if( g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
							_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
							g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// 경공
						{
							if( m_SelectedSkillItem->m_cSkillLevel >= 11 ) // 11성 이상일때만 Radial blur effect 보임 
							{
								g_RenderAlphaBlendFlag = TRUE;
								g_RenderAlphaBlendUseAlphaChannelFlag = TRUE;
								g_LastUsedRenderStyle = NULL;
								g_RenderAlphaBlendTextureFactor = ( ((30-g_ObjectBlurProcessManager.m_iNumRadialBlurSteps)<<27) | 0xFFFFFF );					
							}
						}
					}
				}
			}
		}

		if( !(g_FreeMoveMode && g_InnerUser) )
		{
			if( m_CharacterInfo.animationdescript.motionclass == _XACTION_BLOW )
			{	
				if( m_ModelDescriptor.GetCurFrame() >= 2.0f && m_ModelDescriptor.GetCurFrame() <= 4.0f )
				{	
					g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
					m_ModelDescriptor.RenderCharacter(this);
					g_pCurrentSelectMaterial = NULL;
				}
				else
				{			
					g_pCurrentSelectMaterial = NULL;
					m_ModelDescriptor.RenderCharacter(this);	
				}	
			}
			else if( m_CharacterInfo.animationdescript.motionclass == _XACTION_DEFENCE )
			{	
				if( m_ModelDescriptor.GetCurFrame() >= 2.0f && m_ModelDescriptor.GetCurFrame() <= 4.0f )
				{
					g_pCurrentSelectMaterial = &g_DamagedObjectMaterial;
					g_pCurrentSelectMaterial->Emissive.g = 0.8f;
					g_pCurrentSelectMaterial->Emissive.b = 0.8f;
					m_ModelDescriptor.RenderCharacter(this);
					g_pCurrentSelectMaterial->Emissive.g = 0.0f;
					g_pCurrentSelectMaterial->Emissive.b = 0.0f;
					g_pCurrentSelectMaterial = NULL;
				}
				else
				{
					g_pCurrentSelectMaterial = NULL;
					m_ModelDescriptor.RenderCharacter(this);
				}
			}
			else
			{			
				g_pCurrentSelectMaterial = NULL;
				m_ModelDescriptor.RenderCharacter(this);	
			}
		}
	}

	_XParticleActionBase::bLocalUser = FALSE;
	
	if( g_RenderAlphaBlendFlag )
	{
		g_RenderAlphaBlendFlag = FALSE;
		g_RenderAlphaBlendUseAlphaChannelFlag = FALSE;
		g_LastUsedRenderStyle = NULL;
	}

	// 전투 관련 Rendering
	DrawBattle();
	
	// 캐릭터 상태 효과 Rendering
	DrawStateList();

#ifdef _XDWDEBUG	
	if( g_LodTerrain.m_ViewOBBObject )
	{
		gpDev->SetRenderState( D3DRS_ZENABLE, g_RS_UseZBuffer  );
		gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
		m_CharacterOBB.m_OBBModel.Render();
	}
#endif		

	// by mahwang
	if (m_UserState == _XDEF_USERSTATE_TRAINING && m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
	{
		_XAnimMeshAbstractClass* pEventItem = NULL;
		pEventItem = g_LifeEventManager.GetEventItem(m_CharacterInfo.gender, LIFE_EVENT_ITEM_TYPE_HOMI);
		switch(m_CurrentTrainingState)
		{
		case _XUSER_STATE_2:
			{
				D3DXMATRIX matWorld = g_LifeEventManager.m_matPointWorld;
				float fSrcHeight = matWorld._42;
				matWorld._42 += g_LifeEventManager.m_fAxisTime;
				pEventItem->Render(matWorld);
				matWorld._42 = fSrcHeight;
				break;
			}
		case _XUSER_STATE_5:
			{
				D3DXMATRIX matWorld = g_LifeEventManager.m_matTubeWorld;
				float fSrcHeight = matWorld._42;
				matWorld._42 += g_LifeEventManager.m_fAxisTime;
				pEventItem->Render(matWorld);
				matWorld._42 = fSrcHeight;
				break;
			}
		}
	}
}

void _XLocalUser::RequestBattleMode(_XBATTLEMODETYPE mode)
{
	// 상태효과에서 mode를 변경할 수 없음
	if(m_Lock.modelock == 1)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2325), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "상태 이상으로 인하여 자세 변환이 불가능 합니다."
		return;
	}
	else if( m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN 
		|| m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_PREPARE 
		|| m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_MODIFY)
	{
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
		TCHAR messagestring[512];
		memset( messagestring, 0, sizeof(messagestring) );
		_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1571));// "노점이 열린 채로는 상태변경이 불가능 합니다."
		g_NetworkKernel.InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
		return;
	}
	else if( m_bSocketPlugIn )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1572), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	else if( m_bReadingBook )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1573), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	
	else if( m_bInternalUse )	
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1574), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}	
	else if( m_bGambleBoxOpen )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1575), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	else if(m_bMatchPRMode)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1576), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}
	else if(m_UserState == _XDEF_USERSTATE_TRAINING)
	{
		// 기공, 운기조식 수련중에는 자세 변환 불가
		if(m_SelectedTrainingMode == _XTRAINING_MEDITATION || m_SelectedTrainingMode == _XTRAINING_SPELL)
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2602), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
			return;
		}
	}
	// 경공 모드 사용 중일 때 변경 불가 //Author : 양희왕 추가
	else if( gpInput->CheckKeyPress(DIK_LMENU) && 
			g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_INVENTORY_2129), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
		return;
	}

	// 요청한 모드로 변경하도록 한다
	if(mode == _XBATTLE_PEACE)
		g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_peace_mode);	// 평화모드로 변경
	else
		g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_battle_mode);	// 전투모드로 변경
}

void _XLocalUser::SetBattleMode(_XBATTLEMODETYPE mode)
{
	g_pLocalUser->m_LastModeChangeTime = g_LocalSystemTime;

	m_BattleMode = mode;
	
	if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE)
	{
		if(m_BattleMode == _XBATTLE_PEACE)
			m_CharacterInfo.animationdescript.idletype = _XBATTLE_PEACE;
		else
			m_CharacterInfo.animationdescript.idletype = _XBATTLE_BATTLE;
	}

	if(m_BattleMode == _XBATTLE_BATTLE)
	{
		if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE)
		{
			m_bHaveStance = TRUE;
			m_StanceMotionFirst = TRUE;
		}
		
		if(g_ViewMyBattleGauge)
		{
			m_MiniHpBorder.SetGaugeFactor(LifePowerPercentage);
		}
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_BATTLE );

		if(GetMotionClass() != _XACTION_MOVE)
			SetAnimation( m_CharacterInfo.animationdescript.motionclass );
	}
	else if(m_BattleMode == _XBATTLE_PK)
	{
		// 1st stance motion은 생략

		if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE)
		{
			m_bHaveStance = TRUE;
		}

		if(g_ViewMyBattleGauge)
		{
			m_MiniHpBorder.SetGaugeFactor(LifePowerPercentage);
		}
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_BATTLE );
		
		if(GetMotionClass() != _XACTION_MOVE)
			SetAnimation( m_CharacterInfo.animationdescript.motionclass );
	}
	else
	{
		m_bHaveStance = TRUE;
		m_StanceMotionFirst = TRUE;
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PEACE );

		ResetSkillAnimation();
	}
	
	g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);

	if(g_pQuickSlot_Window)
		g_pQuickSlot_Window->ChangePeaceBattleMode();
}

void _XLocalUser::SetAnimation( _XMOTIONCLASS_TYPE motionclass, bool changemotion )
{
	if(motionclass < 0)
		return;

	/*
	if( m_SelectedSkillID )
	{
		if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
			g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
		{
			
			int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
								  g_SkillProperty[m_SelectedSkillID]->motion_male : 
							      g_SkillProperty[m_SelectedSkillID]->motion_female;
			
			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
			}
			else
			{
				_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
				
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					pWeaponItemProperty = g_WeaponItemProperty;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					pWeaponItemProperty = g_WeaponItemProperty2;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					pWeaponItemProperty = g_WeaponItemProperty3;
				
				switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
					}
					else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
						else
							baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
					}
					else
					{
						baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
					break;				
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					break;
				}
			}
			
			if( m_ModelDescriptor.GetCurMotion() == baseanimindex+2 ) // 경공 마지막 동작 유지 후 동작 교체.
			{
				if( !(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME) )
					return;
			}
		}
	}
	*/


	// 이동 시작 또는 종료 동작교체시는 이동시작시간 초기화 
	m_PrevMoveTime = 0;

	switch( motionclass )
	{
	case _XACTION_IDLE :
		// 질주 이외의 동작이 설정되면 질주모드 해제
		if( m_UseSpeedSkill )
		{
			m_CurrentBattleState = _XUSER_STATE_NONE;
			m_UseSpeedSkill = FALSE;
			m_HiperRunFailedFlag = TRUE;
		}

		if( m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE )
		{
			if(m_ActiveSlotID > 0 || m_bHaveStance)
			{
				short stancemotion = GetStanceMotion(m_ActiveSlotID);
				
				if(m_StanceMotionFirst)
				{
					if(stancemotion >= 0)
					{
						m_ModelDescriptor.SetCurMotion(stancemotion, TRUE);
						m_StanceMotionFirst = FALSE;
						m_bHaveStance = FALSE;
					}
					else
					{	
						ReloadWeaponModel();
						
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{					
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{
									case _XGI_SC_WEAPON_SHORT				:				// 단병기
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// 장병기						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
										break;
									case _XGI_SC_WEAPON_SOFT				:				// 연병기						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
										break;
									case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
#endif
						}
						
						m_StanceMotionFirst = FALSE;
						m_bHaveStance = FALSE;
					}
				}
				else
				{
					if(stancemotion >= 0)
					{
						// 평화상태에서 stance가 있는 경우는 검 밖에 없음 - 무조건 검 평화자세로 돌아감
						m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, changemotion);
					}
					else
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{					
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{
									case _XGI_SC_WEAPON_SHORT				:				// 단병기
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// 장병기						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
										break;
									case _XGI_SC_WEAPON_SOFT				:				// 연병기						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
										break;
									case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
									break;
								}
							}
#endif
						}
					}
				}
			}
			else
			{
				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{					
					m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// 단병기
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
								}
								else
								{
									m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// 장병기						
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
								break;
							case _XGI_SC_WEAPON_SOFT				:				// 연병기						
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
								break;
							case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
								break;
							}							
						}
					}
#else					
					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					{
						switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					{
						switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					{
						switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_SWORD_PEACE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_IDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_IDLE_NONE, TRUE );
							break;
						}
					}
#endif
				}
			}
		}
		else
		{
			if(m_ActiveSlotID > 0 || m_bHaveStance)
			{
				short stancemotion = GetStanceMotion(m_ActiveSlotID);
				
				if(m_StanceMotionFirst)
				{
					if(stancemotion >= 0)
					{
						m_ModelDescriptor.SetCurMotion(stancemotion, TRUE);
						m_StanceMotionFirst = FALSE;
						m_bHaveStance = FALSE;
					}
					else
					{
						ReloadWeaponModel();
						
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{					
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{
									case _XGI_SC_WEAPON_SHORT				:				// 단병기
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// 장병기						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
										break;
									case _XGI_SC_WEAPON_SOFT				:				// 연병기						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
										break;
									case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
#endif
						}
						
						m_StanceMotionFirst = FALSE;
						m_bHaveStance = FALSE;
					}
				}
				else
				{
					if(stancemotion >= 0)
						m_ModelDescriptor.SetCurMotion(stancemotion + _XANI_ORDER_REPEAT, changemotion);
					else
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )
						{					
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{
									case _XGI_SC_WEAPON_SHORT				:				// 단병기
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// 장병기						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
										break;
									case _XGI_SC_WEAPON_SOFT				:				// 연병기						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
										break;
									case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
									break;
								case _XGI_SC_WEAPON_SOFT				:				// 연병기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
									break;
								case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
									break;
								}
							}
#endif
						}
					}
				}
			}
			else
			{
				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{					
					m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// 단병기
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
								{
									m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
								}
								else
								{
									m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// 장병기						
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
								break;
							case _XGI_SC_WEAPON_SOFT				:				// 연병기						
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
								break;
							case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
								break;
							}
						}
					}
#else
					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					{
						switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							{
									m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					{
						switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					{
						switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_IDLE_AXE, TRUE);
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_BATTLEIDLE_SWORD_REPEAT, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_SHORTWEAPON, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_LONGWEAPON, TRUE );						
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );						
							break;
						case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_BATTLEIDLE_NONE, TRUE );
							break;
						}
					}
#endif
				}
			}
		}


		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );

		break;
	
	case _XACTION_MOVE :
		if( m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_RUN )
		{			
			// 질주 이외의 동작이 설정되면 질주모드 해제
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;

				if( m_SelectedSkillID )
				{
					if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
						_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF			 &&
						g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
					{
						m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
						m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
					}
				}
			}

			SetMoveSpeed( _XDEF_CHARACTERMOVESPEED_RUN );
			
			// 앞에 보간 동작 없음
			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
				m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_BATTLE, TRUE);
							}
							else
							{						
								m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_REPEAT, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기					
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_REPEAT, TRUE );					
							break;
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_REPEAT, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_BATTLE, TRUE);
						}
						else
						{						
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_REPEAT, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기					
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_REPEAT, TRUE );					
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_BATTLE, TRUE);
						}
						else
						{						
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_REPEAT, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기					
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_REPEAT, TRUE );					
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_MOVE_SWORD_REPEAT_BATTLE, TRUE);
						}
						else
						{						
							m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SHORTWEAPON_REPEAT, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기					
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_LONGWEAPON_REPEAT, TRUE );					
						break;
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_SOFTWEAPON_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_MOVE_NONE_REPEAT, TRUE );
						break;
					}
				}
#endif
			}
		}
		else if( m_CharacterInfo.animationdescript.movetype == _XMOVETYPE_KYUNGGONG )
		{
#ifdef _XDEF_NEWANIMATIONLIST
			SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_RUN  );
			{
				int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
									  g_SkillProperty[m_SelectedSkillID]->motion_male : 
									  g_SkillProperty[m_SelectedSkillID]->motion_female;

				if( m_CharacterInfo.modeldescript.item1 == 0 )
				{
					m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
							{
							case _XGI_SC_WEAPON_SHORT				:				// 단병기
								if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
								{
									m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SWORD, TRUE);
									else
										m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_BATTLESWORD, TRUE);
								}
								else
								{
									m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
								}
								break;
							case _XGI_SC_WEAPON_LONG				:				// 장병기
								m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_LONGWEAPON, TRUE);
								break;				
							case _XGI_SC_WEAPON_SOFT				:				// 연병기
								m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
								break;
							case _XGI_SC_WEAPON_HIDDEN				:				// 암기
								m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
								break;
							case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
								m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
								break;
							case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
								m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
								break;
							}
						}
					}
#else
					_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;

					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
						pWeaponItemProperty = g_WeaponItemProperty;
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
						pWeaponItemProperty = g_WeaponItemProperty2;
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
						pWeaponItemProperty = g_WeaponItemProperty3;
					
					switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
						}
						else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SWORD, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_BATTLESWORD, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_LONGWEAPON, TRUE);
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion(baseanimindex + _XDEF_FASTMOVEINDEX_SHORTWEAPON, TRUE);
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( baseanimindex, TRUE );
						break;
					}
#endif
				}
			}
#else
			SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_RUN  );

			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
				m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					}					
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					}					
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_FASTMOVE_SWORD_START_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SHORTWEAPON_START, TRUE );
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_LONGWEAPON_START, TRUE );
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_SOFTWEAPON_START, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_FASTMOVE_NONE_START, TRUE );
						break;
					}					
				}
#endif
			}
#endif
		}

		break;
	
	case _XACTION_BLOW :
		// 질주 이외의 동작이 설정되면 질주모드 해제
		if( m_UseSpeedSkill )
		{
			ResetSkillAnimation();
			m_UseSpeedSkill = FALSE;
			m_HiperRunFailedFlag = TRUE;			
		}

		if( m_CharacterInfo.modeldescript.item1 == 0 ) // 아무것도 안들은 경우 
		{
			m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );							
						break;			
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );			
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
						break;
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					}
					else
					{
						m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SHORTWEAPON, TRUE );								
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_SOFTWEAPON, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_BLOW_NONE, TRUE );			
					break;
				}
			}
#endif
		}		
		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		break;

	case _XACTION_DIE :
		// 질주 이외의 동작이 설정되면 질주모드 해제
		if( m_UseSpeedSkill )
		{
			ResetSkillAnimation();
			m_UseSpeedSkill = FALSE;
			m_HiperRunFailedFlag = TRUE;			
		}

		if( m_CharacterInfo.modeldescript.item1 == 0 ) // 아무것도 안들은 경우 
		{
			m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
					{
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// 권각
							m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
						else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
						}
						else
							m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
						break;			
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
						break;
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// 권각
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
					else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
						else
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
					}
					else
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// 권각
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
					else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
						else
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
					}
					else
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )	// 권각
						m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );
					else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_PEACE, TRUE);
						else
							m_ModelDescriptor.SetCurMotion(_XAN_DIE_SWORD_BATTLE, TRUE);
					}
					else
						m_ModelDescriptor.SetCurMotion( _XAN_DIE_SHORTWEAPON, TRUE );								
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE_LONGWEAPON, TRUE );							
					break;			
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					m_ModelDescriptor.SetCurMotion( _XAN_DIE, TRUE );			
					break;
				}
			}
#endif
		}

		g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		break;

	case _XACTION_DYING :
		{
			// 질주 이외의 동작이 설정되면 질주모드 해제
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if(m_CharacterInfo.modeldescript.item1 == 0)
			{
				m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
										m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
									else
										m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
								}
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_LONGWEAPON, TRUE);
							}
							break;
						case _XGI_SC_WEAPON_HIDDEN :
							{
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							}
							break;
						case _XGI_SC_WEAPON_MUSICAL :
							{
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							}
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
							}
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_LONGWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
							}
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_LONGWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SWORD_BATTLE, TRUE);
							}
							else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_SHORTWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_LONGWEAPON, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREADY_NONE, TRUE);
						}
						break;
					}
				}
#endif
			}

			{
				int rand = _XRandom() % 3;
				if( m_CharacterInfo.gender == _XGENDER_MAN )
					_XPlaySoundEffect( g_BlowEffectSoundIndex[0][rand] , m_Position );
				else 
					_XPlaySoundEffect( g_BlowEffectSoundIndex[1][rand] , m_Position );
			}

			g_WorldEffectInstanceManager.DeleteInstance(_XDEF_INTERFACEID_PICKING);
		}
		break;

	case _XACTION_ATTACK :		
		{
			// 질주 이외의 동작이 설정되면 질주모드 해제
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;
			}

			if(m_SelectedSkillID == 0)
			{
				if(m_CharacterInfo.modeldescript.item1 == 0)
				{
					m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;
				}
				else
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
							{
							case _XGI_SC_WEAPON_SHORT :
								{
									if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
										m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// 권각
									else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
										m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;
									else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SWORD;
									else
										m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// 단병기
								}
								break;
							case _XGI_SC_WEAPON_LONG :
								{
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 장병기
								}
								break;
							case _XGI_SC_WEAPON_SOFT :
							case _XGI_SC_WEAPON_HIDDEN :
							case _XGI_SC_WEAPON_MUSICAL :						
								{
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 임시
								}
								break;
							case _XGI_SC_WEAPON_SPECIAL :
								{
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// 강무기
								}
								break;
							}
						}
					}
#else
					if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					{
						switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// 권각
								else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;
								else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SWORD;
								else
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// 단병기
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 장병기
							}
							break;
						case _XGI_SC_WEAPON_SOFT :
						case _XGI_SC_WEAPON_HIDDEN :
						case _XGI_SC_WEAPON_MUSICAL :						
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 임시
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// 강무기
							}
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					{
						switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// 권각
								else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;
								else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SWORD;
								else
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// 단병기
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 장병기
							}
							break;
						case _XGI_SC_WEAPON_SOFT :
						case _XGI_SC_WEAPON_HIDDEN :
						case _XGI_SC_WEAPON_MUSICAL :						
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 임시
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// 강병기 == 권각 
							}
							break;
						}
					}
					else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					{
						switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// 권각
								else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;
								else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SWORD;
								else
									m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// 단병기
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 장병기
							}
							break;
						case _XGI_SC_WEAPON_SOFT :
						case _XGI_SC_WEAPON_HIDDEN :
						case _XGI_SC_WEAPON_MUSICAL :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 임시
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;		// 강병기
							}
							break;
						}
					}
#endif
				}
			}

			m_ModelDescriptor.SetCurMotion( m_CharacterInfo.animationdescript.attacktype, changemotion );			
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		}				
		break;
	case _XACTION_DEFENCE :
		{			
			// 질주 이외의 동작이 설정되면 질주모드 해제
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );						
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_SOFTWEAPON, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_DEFENCE_NONE, TRUE );
						break;
					}
				}
#endif
			}
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
		}
		break;
	case _XACTION_RESCUE :
		{
			// 질주 이외의 동작이 설정되면 질주모드 해제
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_HITBACK_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_HITBACK_NONE, TRUE );
						break;
					}
				}
#endif
			}
		}
		break;
	case _XACTION_REBIRTH :
		{
			// 질주 이외의 동작이 설정되면 질주모드 해제
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
			{				
				m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
								m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
								else
									m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );						
						}
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_PEACE, TRUE);
							else
								m_ModelDescriptor.SetCurMotion(_XAN_WAKEUP_SWORD_BATTLE, TRUE);
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_WAKEUP_NONE, TRUE );
						break;
					}
				}
#endif
			}
		}
		break;
	case _XACTION_MATCH_START :
		{
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if(m_CharacterInfo.modeldescript.item1 == 0)// 아무것도 안들은 경우 
			{				
				m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
						{				
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							{
								m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);						
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( _XAN_MATCH_SHORTWEAPON, TRUE );						
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_LONGWEAPON, TRUE );	
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
						{
							m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
						{
							m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
					{				
					case _XGI_SC_WEAPON_SHORT				:				// 단병기
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
						{
							m_ModelDescriptor.SetCurMotion(_XAN_MATCH_NONE, TRUE);						
						}
						else
						{
							m_ModelDescriptor.SetCurMotion( _XAN_MATCH_SHORTWEAPON, TRUE );						
						}
						break;
					case _XGI_SC_WEAPON_LONG				:				// 장병기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_LONGWEAPON, TRUE );	
						break;				
					case _XGI_SC_WEAPON_SOFT				:				// 연병기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_HIDDEN				:				// 암기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
						m_ModelDescriptor.SetCurMotion( _XAN_MATCH_NONE, TRUE );
						break;
					}
				}
#endif
			}
		}
		break;
	case _XACTION_MATCH_CANCEL :
		{
			// 질주 이외의 동작이 설정되면 질주모드 해제
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}
			m_ModelDescriptor.SetCurMotion(_XAN_MATCH_CANCEL);
		}
		break;
	
	case _XACTION_LPS :
		{
			m_ModelDescriptor.m_ViewLeftSwordEffect	= FALSE;
			
			_XMob* pMob = NULL;
			if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE )
			{
				// 나중에 LPS동작 처리가 들어가면 따로 분리한다.
				if(m_ViewTargetType == _XPICKINGTARGET_MOB)
				{
					// 두번째 클릭
					pMob = (_XMob*)m_ViewTarget;
				}
			}
			else
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					// 두번째 클릭
					pMob = (_XMob*)m_AttackTarget;
				}
			}
			if( pMob )
			{
				switch( g_MobAttribTable[pMob->m_MobType].scripttype ) {
				case 81:// Normal LPS
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
						{				
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{				
									case _XGI_SC_WEAPON_SHORT				:				// 단병기
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
											else
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// 장병기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
										break;				
									case _XGI_SC_WEAPON_SOFT				:				// 연병기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN				:				// 암기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
#endif
						}
					}
					break;
				case 82:// Key LPS
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
						{				
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{				
									case _XGI_SC_WEAPON_SHORT				:				// 단병기
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
											else
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// 장병기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
										break;				
									case _XGI_SC_WEAPON_SOFT				:				// 연병기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN				:				// 암기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
#endif
						}
					}
					break;
				case 83:// Transform LPS
					{
					}
					break;
				case 84:// HP LPS
					{
					}
					break;
				case 85:// Optional LPS
					{
						if( m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
						{				
							m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
						}
						else
						{
#ifdef _XTS_ITEM_OPTIMIZATION
							if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
							{
								_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
								if( WeaponItem )
								{
									switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
									{				
									case _XGI_SC_WEAPON_SHORT				:				// 단병기
										if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
										}
										else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
										{
											if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
											else
												m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
										}
										else
										{
											m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
										}
										break;
									case _XGI_SC_WEAPON_LONG				:				// 장병기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
										break;				
									case _XGI_SC_WEAPON_SOFT				:				// 연병기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_HIDDEN				:				// 암기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
										break;
									}
								}
							}
#else
							if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							{
								switch( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							{
								switch( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
							else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							{
								switch( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType )
								{				
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );						
									}
									else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_PEACE, TRUE);
										else
											m_ModelDescriptor.SetCurMotion(_XAN_LPS_USEKEY_SWORD_BATTLE, TRUE);
									}
									else
									{
										m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_SHORTWEAPON, TRUE );						
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_LONGWEAPON, TRUE );	
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									m_ModelDescriptor.SetCurMotion( _XAN_LPS_USEKEY_HAND, TRUE );
									break;
								}
							}
#endif
						}
					}
					break;
				}
			}
		}
		break;

	case _XACTION_INSENSIBLE :		// 기절 - 상태효과
		{
			// 질주 이외의 동작이 설정되면 질주모드 해제
			if( m_UseSpeedSkill )
			{
				ResetSkillAnimation();
				m_UseSpeedSkill = FALSE;
				m_HiperRunFailedFlag = TRUE;			
			}

			if(m_CharacterInfo.modeldescript.item1 == 0)
			{
#ifdef _XTS_NEWINSENSIBLE
				m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
				m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
						{
						case _XGI_SC_WEAPON_SHORT :
							{
								if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
								}
								else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
								{
									if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									{
#ifdef _XTS_NEWINSENSIBLE
										m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
										m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_PEACE, TRUE);
#endif
									}
									else
									{
#ifdef _XTS_NEWINSENSIBLE
										m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
										m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_BATTLE, TRUE);
#endif
									}
								}
								else
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SHORTWEAPON, TRUE);
#endif
								}
							}
							break;
						case _XGI_SC_WEAPON_LONG :
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_LONGWEAPON, TRUE);
#endif
							}
							break;
						case _XGI_SC_WEAPON_HIDDEN :
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							break;
						case _XGI_SC_WEAPON_MUSICAL :
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							break;
						case _XGI_SC_WEAPON_SPECIAL :
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							break;
						}
					}
				}
#else
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
				{
					switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_PEACE, TRUE);
#endif
								}
								else
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_BATTLE, TRUE);
#endif
								}
							}
							else
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SHORTWEAPON, TRUE);
#endif
							}
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_LONGWEAPON, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
				{
					switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_PEACE, TRUE);
#endif
								}
								else
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_BATTLE, TRUE);
#endif
								}
							}
							else
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SHORTWEAPON, TRUE);
#endif
							}
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_LONGWEAPON, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					}
				}
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
				{
					switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
							}
							else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_PEACE, TRUE);
#endif
								}
								else
								{
#ifdef _XTS_NEWINSENSIBLE
									m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
									m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SWORD_BATTLE, TRUE);
#endif
								}
							}
							else
							{
#ifdef _XTS_NEWINSENSIBLE
								m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
								m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_SHORTWEAPON, TRUE);
#endif
							}
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_LONGWEAPON, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_HIDDEN :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_MUSICAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
#ifdef _XTS_NEWINSENSIBLE
							m_ModelDescriptor.SetCurMotion(_XAN_EMOTION_CLAPPINGHAND, TRUE);
#else
							m_ModelDescriptor.SetCurMotion(_XAN_DYINGREPEAT_NONE, TRUE);
#endif
						}
						break;
					}
				}
#endif
			}
			
			{
				int rand = _XRandom() % 3;
				if( m_CharacterInfo.gender == _XGENDER_MAN )
					_XPlaySoundEffect( g_BlowEffectSoundIndex[0][rand] , m_Position );
				else 
					_XPlaySoundEffect( g_BlowEffectSoundIndex[1][rand] , m_Position );
			}
			
			g_WorldEffectInstanceManager.DeleteInstance(_XDEF_INTERFACEID_PICKING);
		}
		break;

	default:
		// 질주 이외의 동작이 설정되면 질주모드 해제
		if( m_UseSpeedSkill )
		{
			ResetSkillAnimation();
			m_UseSpeedSkill = FALSE;
			m_HiperRunFailedFlag = TRUE;			
		}
		break;
	}	
		
	m_SpecialActionTimer = g_LocalSystemTime;

	m_CharacterInfo.animationdescript.motionclass = motionclass;

	m_ModelDescriptor.m_FrameResult = _XDEF_ACI_NONE;
	m_ModelDescriptor.m_ViewLeftSwordEffect = (motionclass == _XACTION_ATTACK);
}

void _XLocalUser::SetAnimationIndex( int animationindex, bool changemotion )
{	
	// 이동 시작 또는 종료 동작교체시는 이동시작시간 초기화 
	m_PrevMoveTime = 0;
	
	if(animationindex < 0)
		return;

	if( g_LocalSystemTime == 0.0f )
		 m_PrevProcessTime = -1.0f;
	else m_PrevProcessTime = g_fLocalSystemTime;

#ifdef _XDEF_NEWANIMATIONLIST

	BOOL checkfastmove = FALSE;

	if( m_SelectedSkillID )
	{
		if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
			_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF	&&
			g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
		{
	
			int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
								  g_SkillProperty[m_SelectedSkillID]->motion_male : 
								  g_SkillProperty[m_SelectedSkillID]->motion_female;
		
			if( m_CharacterInfo.modeldescript.item1 == 0 )
			{
			}
			else
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
							}
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
								else
									baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
							}
							else
							{
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							break;
						}
					}
				}
#else
				_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
				
				if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
					pWeaponItemProperty = g_WeaponItemProperty;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
					pWeaponItemProperty = g_WeaponItemProperty2;
				else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
					pWeaponItemProperty = g_WeaponItemProperty3;
				
				switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
					{
					}
					else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
							baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
						else
							baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
					}
					else
					{
						baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기
					baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
					break;				
				case _XGI_SC_WEAPON_SOFT				:				// 연병기
					baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
					break;
				case _XGI_SC_WEAPON_HIDDEN				:				// 암기
					break;
				case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
					break;
				case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
					break;
				}
#endif
			}
			
			if( animationindex == baseanimindex )
			{
				checkfastmove = TRUE;					

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
				m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
				SetMoveSpeed( 	_XDEF_CHARACTERMOVESPEED_RUN  );
			}
			else if( animationindex == baseanimindex+1 )
			{
				checkfastmove = TRUE;

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
				m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
				SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_RUN );
			}			
			else if( animationindex == baseanimindex+2 )
			{
				checkfastmove = TRUE;

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
				m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
				SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			}
		}
	}

	if( !checkfastmove )	

#endif

	{	
		switch( animationindex )
		{	
		case _XAN_IDLE_NONE					:	
		case _XAN_IDLE_NONE_ACTION			:		
		case _XAN_IDLE_SHORTWEAPON			:	
		case _XAN_IDLE_SHORTWEAPON_ACTION	:		
		case _XAN_IDLE_LONGWEAPON			:	
		case _XAN_IDLE_LONGWEAPON_ACTION	:
		case _XAN_IDLE_SWORD_PEACE			:
		case _XAN_IDLE_SWORD_ACTION			:
			m_CharacterInfo.animationdescript.idletype			= _XBATTLE_PEACE;
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;	

		case _XAN_BATTLEIDLE_NONE			: 
		case _XAN_BATTLEIDLE_SHORTWEAPON	:
		case _XAN_BATTLEIDLE_LONGWEAPON		:
		case _XAN_BATTLEIDLE_SWORD_REPEAT :
			m_CharacterInfo.animationdescript.idletype			= _XBATTLE_BATTLE;
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;
			
		case _XAN_EMOTION_CRINGESTART :
			{
				if(m_CharacterInfo.modeldescript.item1 != 0)
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
								WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									animationindex = _XAN_KNEE_SWORD_START;
							}
						}
					}
#else
					if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
					{
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_START;
						}
						
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
					{
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_START;
						}
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
					{
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_START;
						}
					}
#endif
				}

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
				g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			}
			break;
		case _XAN_EMOTION_CRINGEREPEAT :
			{
				if(m_CharacterInfo.modeldescript.item1 != 0)
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
								WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									animationindex = _XAN_KNEE_SWORD_REPEAT;
							}
						}
					}
#else
					if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
					{
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_REPEAT;
						}
						
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
					{
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_REPEAT;
						}
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
					{
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_REPEAT;
						}
					}
#endif
				}
				
				m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
				g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			}
			break;
		case _XAN_EMOTION_CRINGEEND :
			{
				if(m_CharacterInfo.modeldescript.item1 != 0)
				{
#ifdef _XTS_ITEM_OPTIMIZATION		
					if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
					{
						_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
						if( WeaponItem )
						{
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
								WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									animationindex = _XAN_KNEE_SWORD_END;
							}
						}
					}
#else
					if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
					{
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_END;
						}
						
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
					{
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_END;
						}
					}
					else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
					{
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
								animationindex = _XAN_KNEE_SWORD_END;
						}
					}
#endif
				}

				m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
				g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			}
			break;

#ifdef ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA
		case _XAN_EMOTION_BOW				:
		case _XAN_EMOTION_BOWPOLITELY		:
#endif

		case _XAN_EMOTION_MARRYCLAPPINGHAND	:		
		case _XAN_EMOTION_MARRYBOX			:		
		case _XAN_EMOTION_MARRYBOWPOLITELY	:
			{
				DeleteWeaponModel();
				m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
				g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			}			
			break;
		case _XAN_KNEE_SWORD_START			:
		case _XAN_KNEE_SWORD_REPEAT			:
		case _XAN_KNEE_SWORD_END			:
	
#ifndef ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA
		case _XAN_EMOTION_BOW				:
		case _XAN_EMOTION_BOWPOLITELY		:
#endif			

		case _XAN_EMOTION_EXCITEMENT		:
		case _XAN_EMOTION_RUSH				:	
		case _XAN_EMOTION_CHEER				:
		case _XAN_EMOTION_BREAKDOWN			:
		case _XAN_EMOTION_CRY				:
		case _XAN_EMOTION_LAUGH				:				
		case _XAN_EMOTION_ANGRY				:
		case _XAN_EMOTION_DOZ				:
		case _XAN_EMOTION_SITDOWNSTART		:
		case _XAN_EMOTION_SITDOWNREPEAT		:
		case _XAN_EMOTION_SITDOWNEND		:							
		case _XAN_EMOTION_CLAPPINGHAND		:

		case _XAN_MATCH_PR_START			:
		case _XAN_MATCH_PR_REPEAT			:
		case _XAN_MATCH_PR_END				:
		case _XAN_MATCH_WINNER_START		:
		case _XAN_MATCH_WINNER_REPEAT		:
		case _XAN_MATCH_LOSER_START			:
		case _XAN_MATCH_LOSER_REPEAT		:

#ifdef _XDEF_CASTLEBATTLE
		case _XAN_MANOR_WIN					:
		case _XAN_MANOR_SHOUT				:
#endif
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_IDLE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;

		case  _XAN_MOVE_NONE_START				:
		case  _XAN_MOVE_SHORTWEAPON_START		:
		case  _XAN_MOVE_LONGWEAPON_START		:
		case  _XAN_MOVE_SOFTWEAPON_START		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;

		case  _XAN_MOVE_NONE_REPEAT				:
		case  _XAN_MOVE_SHORTWEAPON_REPEAT		:	
		case  _XAN_MOVE_LONGWEAPON_REPEAT		:
		case  _XAN_MOVE_SOFTWEAPON_REPEAT		:
		case  _XAN_MOVE_SWORD_REPEAT_PEACE		:
		case  _XAN_MOVE_SWORD_REPEAT_BATTLE		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_RUN );
			break;

		case  _XAN_MOVE_NONE_END				:
		case  _XAN_MOVE_SHORTWEAPON_END			:
		case  _XAN_MOVE_LONGWEAPON_END			:
		case  _XAN_MOVE_SOFTWEAPON_END			:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_RUN;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;
		
		case  _XAN_FASTMOVE_NONE_START			:	
		case  _XAN_FASTMOVE_SHORTWEAPON_START	:
		case  _XAN_FASTMOVE_LONGWEAPON_START	:
		case  _XAN_FASTMOVE_SOFTWEAPON_START	:
		case  _XAN_FASTMOVE_SWORD_START_PEACE	:
		case  _XAN_FASTMOVE_SWORD_START_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;

		case  _XAN_FASTMOVE_NONE_REPEAT			:	
		case  _XAN_FASTMOVE_SHORTWEAPON_REPEAT	:	
		case  _XAN_FASTMOVE_LONGWEAPON_REPEAT	:	
		case  _XAN_FASTMOVE_SOFTWEAPON_REPEAT	:
		case  _XAN_FASTMOVE_SWORD_REPEAT_PEACE	:
		case  _XAN_FASTMOVE_SWORD_REPEAT_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_RUN );
			break;

		case  _XAN_FASTMOVE_NONE_END			:	
		case  _XAN_FASTMOVE_SHORTWEAPON_END		:
		case  _XAN_FASTMOVE_LONGWEAPON_END		:
		case  _XAN_FASTMOVE_SOFTWEAPON_END		:
		case  _XAN_FASTMOVE_SWORD_END_PEACE		:
		case  _XAN_FASTMOVE_SWORD_END_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_MOVE;
			m_CharacterInfo.animationdescript.movetype			= _XMOVETYPE_KYUNGGONG;
			SetMoveSpeed(  _XDEF_CHARACTERMOVESPEED_WALK );
			break;

		case _XAN_DYINGREADY_NONE :
		case _XAN_DYINGREADY_SHORTWEAPON :
		case _XAN_DYINGREADY_LONGWEAPON :
		case _XAN_DYINGREADY_SWORD_PEACE :
		case _XAN_DYINGREADY_SWORD_BATTLE :
			{
				int rand = _XRandom() % 3;			
				if( m_CharacterInfo.gender == _XGENDER_MAN )
					_XPlaySoundEffect( g_BlowEffectSoundIndex[0][rand] , m_Position );
				else 
					_XPlaySoundEffect( g_BlowEffectSoundIndex[1][rand] , m_Position );
			}

			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DYING;
			g_WorldEffectInstanceManager.DeleteInstance(_XDEF_INTERFACEID_PICKING);
			break;
			
		case _XAN_DYINGREPEAT_NONE :
		case _XAN_DYINGREPEAT_SHORTWEAPON :
		case _XAN_DYINGREPEAT_LONGWEAPON :
		case _XAN_DYINGREPEAT_SWORD_PEACE :
		case _XAN_DYINGREPEAT_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DYING;
			g_WorldEffectInstanceManager.DeleteInstance(_XDEF_INTERFACEID_PICKING);
			break;

		case _XAN_DIE :
		case _XAN_DIE_SHORTWEAPON :
		case _XAN_DIE_LONGWEAPON :
		case _XAN_DIE_SWORD_PEACE :
		case _XAN_DIE_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DIE;		
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;

		case _XAN_HITBACK_NONE :
		case _XAN_HITBACK_SHORTWEAPON :
		case _XAN_HITBACK_LONGWEAPON :
		case _XAN_HITBACK_SWORD_PEACE :
		case _XAN_HITBACK_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_RESCUE;
			break;
			
		case _XAN_WAKEUP_NONE :
		case _XAN_WAKEUP_SHORTWEAPON :
		case _XAN_WAKEUP_LONGWEAPON :
		case _XAN_WAKEUP_SWORD_PEACE :
		case _XAN_WAKEUP_SWORD_BATTLE :
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_REBIRTH;
			break;

		case _XAN_DEFENCE_NONE			:
		case _XAN_DEFENCE_SHORTWEAPON	:
		case _XAN_DEFENCE_LONGWEAPON	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_DEFENCE;	
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;
			
		case _XAN_BLOW_NONE				:
		case _XAN_BLOW_SHORTWEAPON		:
		case _XAN_BLOW_LONGWEAPON		:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_BLOW;		
			break;	

		case _XAN_LPS_HANDTOUCH				:			
		case _XAN_LPS_SHORTWEAPON_HANDTOUCH :			
		case _XAN_LPS_LONGWEAPON_HANDTOUCH	:			
		case _XAN_LPS_FOOTTOUCH				:			
		case _XAN_LPS_SHORTWEAPON_FOOTTOUCH :			
		case _XAN_LPS_LONGWEAPON_FOOTTOUCH	:			
		case _XAN_LPS_USEKEY_HAND			:
		case _XAN_LPS_USEKEY_SHORTWEAPON	:
		case _XAN_LPS_USEKEY_LONGWEAPON		:
		case _XAN_LPS_USEKEY_SWORD_PEACE	:
		case _XAN_LPS_USEKEY_SWORD_BATTLE	:
			m_CharacterInfo.animationdescript.motionclass		= _XACTION_LPS;
			break;	

		case _XAN_MATCH_NONE :
		case _XAN_MATCH_SHORTWEAPON :
		case _XAN_MATCH_LONGWEAPON :
			m_CharacterInfo.animationdescript.motionclass = _XACTION_MATCH_START;
			break;

		case _XAN_MATCH_CANCEL :
			m_CharacterInfo.animationdescript.motionclass = _XACTION_MATCH_CANCEL;
			break;
			
		default :
			m_CharacterInfo.animationdescript.motionclass = _XACTION_ATTACK;
			m_CharacterInfo.animationdescript.attacktype = animationindex;
			g_WorldEffectInstanceManager.DeleteInstance( _XDEF_INTERFACEID_PICKING );
			break;
		}
	}

	// 질주 이외의 동작이 설정되면 질주모드 해제
	if( m_CharacterInfo.animationdescript.motionclass != _XACTION_MOVE || 
	    m_CharacterInfo.animationdescript.movetype	  != _XMOVETYPE_KYUNGGONG )
	{
		if( m_UseSpeedSkill )
		{
			ResetSkillAnimation();
			m_UseSpeedSkill = FALSE;
		}		
	}

	m_ModelDescriptor.SetCurMotion( animationindex, changemotion );

	m_ModelDescriptor.m_FrameResult = _XDEF_ACI_NONE;
	m_SpecialActionTimer = g_LocalSystemTime;
	m_ModelDescriptor.m_ViewLeftSwordEffect = (m_CharacterInfo.animationdescript.motionclass == _XACTION_ATTACK);
}

void _XLocalUser::InitializeUserDataTable( void )
{

	int	jinlevellut[241];
	int	gonglevellut[241];

	if( !g_ScriptArchive.ReOpenPackage() )
		return;

	FILE* pScriptFile = NULL;

	pScriptFile = g_ScriptArchive.GetPackedFile( _T("Level_Table.XMS") );
		
	if( !pScriptFile )
	{
		_XFatalError( "Load Level LUT( %s ) File open", _T("Level_Table.XMS") );
		g_ScriptArchive.ClosePackage();
		return; 
	}
	
	TCHAR parsingString[512];
	int index = 0;
	while( 1 )
	{			
		if( feof(pScriptFile) ) break;
		
		memset( parsingString, 0, 512 );
		if( !fgets( parsingString, 511, pScriptFile ) ) break;
		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
		
		if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}
		
		sscanf( parsingString, "%d", &index );

		sscanf( parsingString, "%d %d %d", &index, &jinlevellut[index], &gonglevellut[index] );
	}
		
	memset( g_UserLevelTable, 0, sizeof(_XUSERLEVEL_LUTTYPE) * 241 );

	//_XLog( "Notice : Level table loaded" );

	int		pos = 1;
	DWORD	sum = 0;

	for (int i=1; i<=20 ; i++ ) 
	{
		for (int  j=1; j<=12 ; j++) 
		{
			g_UserLevelTable[pos].level				= i;								// 내공단계
			g_UserLevelTable[pos].sublevel			= j;								// 수위
			g_UserLevelTable[pos].innerlevel		= (i-1) * 12 + j;					// 총수위

			g_UserLevelTable[pos].needjinexperience	= jinlevellut[pos-1];
			g_UserLevelTable[pos].needgongexperience = gonglevellut[pos-1];
			
			sum += g_UserLevelTable[pos].needjinexperience;								// 누적경험치			
			g_UserLevelTable[pos].accumulatejinexperience = sum;

//			_XLog( "%d %d %d %d %d", g_UserLevelTable[pos].level, g_UserLevelTable[pos].sublevel, g_UserLevelTable[pos].totalsublevel, g_UserLevelTable[pos].needexperience, g_UserLevelTable[pos].accumulateexperience );

			pos++;
		}
	}

	//_XLog( "Notice : Level table calculated" );

	pScriptFile = g_ScriptArchive.GetPackedFile( _T("PointTable.xms") );
	
	if( !pScriptFile )
	{
		_XFatalError( "Load Point table( %s ) File open", _T("PointTable.xms") );
		g_ScriptArchive.ClosePackage();
		return; 
	}
		
	index = 0;
	while( 1 )
	{			
		if( feof(pScriptFile) ) break;
		
		memset( parsingString, 0, 512 );
		if( !fgets( parsingString, 511, pScriptFile ) ) break;
		if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
		
		if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}
		
		DWORD dummy;
		sscanf( parsingString, "%d %d %d %d %d %d %d %d %d", &dummy, 
							   &g_UsePointTable[index].vitality,
							   &g_UsePointTable[index].inner,
							   &g_UsePointTable[index].concenturate,
							   
			                   &g_UsePointTable[index].need_constitution, 
							   &g_UsePointTable[index].need_zen,
							   &g_UsePointTable[index].need_intelligence,
							   &g_UsePointTable[index].need_strength,
							   &g_UsePointTable[index].need_dexterity) ;
		index++;
	}

	//_XLog( "Notice : Point table loaded" );

	pScriptFile = NULL;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		pScriptFile = g_ScriptArchive.GetPackedFile( _T("vn_RollName_Table.xms") );
		
		if( !pScriptFile )
		{
			_XFatalError( "Load Roll name table( %s ) File open", _T("vn_RollName_Table.xms") );
			g_ScriptArchive.ClosePackage();
			return; 
		}

		int groupindex = 0;
		int rollclass = 0;
		int rollclassgrade = 0;
		TCHAR tempparsingbuffer[256];
		TCHAR tempparsingbuffer2[256];
		
		memset( g_CharacterRollNameTable, 0, sizeof(TCHAR) * _XGROUP_MAXGROUPCOUNT * _XROLLCLASS_MAXCOUNT * 5 * 64 );
		
		while( 1 )
		{			
			if( feof(pScriptFile) ) break;
			
			memset( parsingString, 0, 512 );
			if( !fgets( parsingString, 511, pScriptFile ) ) break;
			if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
			
			if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
			{
				break;			
			}
			
			if( strncmp( parsingString, _T("_XC_GROUP"), strlen(_T("_XC_GROUP")) ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &groupindex );
			}
			else if( strncmp( parsingString, _T("_XC_ROLLNAME"), strlen(_T("_XC_ROLLNAME"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclassgrade );
				int dummy = rollclassgrade;
				sscanf( parsingString, "%s %d %s", tempparsingbuffer, &dummy, tempparsingbuffer2 );
				
				LPTSTR pszPtr = strstr( parsingString, tempparsingbuffer2 );

				strcpy( g_CharacterRollNameTable[groupindex][rollclass][rollclassgrade], pszPtr );
			}
			else if( strncmp( parsingString, _T("_XC_ROLL"), strlen(_T("_XC_ROLL"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclass );
			}		
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("us_RollName_Table.xms") );
		else if(g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN)
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("rs_RollName_Table.xms") );			
				
		if( !pScriptFile )
		{
			_XFatalError( "Load Roll name table( %s ) File open", _T("us_RollName_Table.xms") );
			g_ScriptArchive.ClosePackage();
			return; 
		}
		
		int groupindex = 0;
		int rollclass = 0;
		int rollclassgrade = 0;
		TCHAR tempparsingbuffer[256];
		TCHAR tempparsingbuffer2[256];
		
		memset( g_CharacterRollNameTable, 0, sizeof(TCHAR) * _XGROUP_MAXGROUPCOUNT * _XROLLCLASS_MAXCOUNT * 5 * 64 );
		
		while( 1 )
		{			
			if( feof(pScriptFile) ) break;
			
			memset( parsingString, 0, 512 );
			if( !fgets( parsingString, 511, pScriptFile ) ) break;
			if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
			
			if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
			{
				break;			
			}
			
			if( strncmp( parsingString, _T("_XC_GROUP"), strlen(_T("_XC_GROUP")) ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &groupindex );
			}
			else if( strncmp( parsingString, _T("_XC_ROLLNAME"), strlen(_T("_XC_ROLLNAME"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclassgrade );
				int dummy = rollclassgrade;
				sscanf( parsingString, "%s %d %s", tempparsingbuffer, &dummy, tempparsingbuffer2 );
				
				LPTSTR pszPtr = strstr( parsingString, tempparsingbuffer2 );
				
				strcpy( g_CharacterRollNameTable[groupindex][rollclass][rollclassgrade], pszPtr );
			}
			else if( strncmp( parsingString, _T("_XC_ROLL"), strlen(_T("_XC_ROLL"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclass );
			}		
		}
	}
	else
	{
		if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE)
			pScriptFile = g_ScriptArchive.GetPackedFile(_T("TW_Rollname_Table.xms"));
		else if(g_LanguageType == _XLANGUAGE_TYPE_JAPANESE)
			pScriptFile = g_ScriptArchive.GetPackedFile(_T("JP_Rollname_Table.xms"));
		else
			pScriptFile = g_ScriptArchive.GetPackedFile( _T("RollName_Table.xms") );
		
		if( !pScriptFile )
		{
			_XFatalError( "Load Roll name table( %s ) File open", _T("RollName_Table.xms") );
			g_ScriptArchive.ClosePackage();
			return; 
		}

		int groupindex = 0;
		int rollclass = 0;
		int rollclassgrade = 0;
		TCHAR tempparsingbuffer[256];

		memset( g_CharacterRollNameTable, 0, sizeof(TCHAR) * _XGROUP_MAXGROUPCOUNT * _XROLLCLASS_MAXCOUNT * 5 * 64 );
		
		while( 1 )
		{			
			if( feof(pScriptFile) ) break;
			
			memset( parsingString, 0, 512 );
			if( !fgets( parsingString, 511, pScriptFile ) ) break;
			if( parsingString[0] == ';' || parsingString[0] == '\n' || !strlen(parsingString) ) continue;
			
			if( strncmp( parsingString, "_XM_EOS", 7 ) == 0 )
			{
				break;			
			}
		
			if( strncmp( parsingString, _T("_XC_GROUP"), strlen(_T("_XC_GROUP")) ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &groupindex );
			}
			else if( strncmp( parsingString, _T("_XC_ROLLNAME"), strlen(_T("_XC_ROLLNAME"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclassgrade );
				int dummy = rollclassgrade;
				sscanf( parsingString, "%s %d %s", tempparsingbuffer, &dummy, g_CharacterRollNameTable[groupindex][rollclass][rollclassgrade] );
			}
			else if( strncmp( parsingString, _T("_XC_ROLL"), strlen(_T("_XC_ROLL"))  ) == 0 )
			{
				sscanf( parsingString, "%s %d", tempparsingbuffer, &rollclass );
			}		
		}
	}

	g_ScriptArchive.ClosePackage();

	//_XLog( "Notice : roll name table loaded" );
}

void _XLocalUser::RecalcLocalUserParameter( void )
{	
	UpdateLocalUserLevel();

	if( g_pJinStatus_Window )
	{
		g_pJinStatus_Window ->SetParameter();
	}
}

int _XLocalUser::CheckPartyMember( int partymemberuniqueid )
{	
	if( m_InParty )
	{
		for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)// 2004.06.01->oneway48 modify : 15->9
		{
			if( m_PartyInfo[i].playeruniqueid == partymemberuniqueid )
			{
				return i;
			}
		}
	}
	else
	{
		return -1;
	}
	return -1;
}

// 로컬 유저의 레벨값 업데이트
void _XLocalUser::UpdateLocalUserLevel( void )
{		
	m_CharacterInfo.Set_level( g_UserLevelTable[m_CharacterInfo.Get_innerlevel()].level );
	m_CharacterInfo.Set_sublevel( g_UserLevelTable[m_CharacterInfo.Get_innerlevel()].sublevel );
	
	UpdateLocalUserAttackLevel();	
}

void _XLocalUser::UpdateLocalUserAttackLevel(void)
{
	// 이전 Data clear
	m_P_AttackDamage.Set_applyCount(0);				// 공격력
	m_P_AttackRate.Set_applyCount(0);					// 공격 성공치
	m_P_Defence.Set_applyCount(0);						// 방어력
	m_P_AvoidRate.Set_applyCount(0);					// 회피력
	m_M_AttackDamage.Set_applyCount(0);				// 기공 공격력
	m_M_AttackRate.Set_applyCount(0);					// 기공 성공치
	m_M_Defence.Set_applyCount(0);						// 기공 방어력
	m_M_AvoidRate.Set_applyCount(0);					// 기공 회피력

	m_P_AttackDamage.Set_applyCountAdditional(0);				// 공격력
	m_P_AttackRate.Set_applyCountAdditional(0);				// 공격 성공치
	m_P_Defence.Set_applyCountAdditional(0);					// 방어력
	m_P_AvoidRate.Set_applyCountAdditional(0);					// 회피력
	m_M_AttackDamage.Set_applyCountAdditional(0);				// 기공 공격력
	m_M_AttackRate.Set_applyCountAdditional(0);				// 기공 성공치
	m_M_Defence.Set_applyCountAdditional(0);					// 기공 방어력
	m_M_AvoidRate.Set_applyCountAdditional(0);					// 기공 회피력
	
	m_Strength.Set_applyCount(0);						// 근력
	m_Strength.Set_bBuff(0);
	m_Zen.Set_applyCount(0);							// 진기
	m_Zen.Set_bBuff(0);
	m_Intelligence.Set_applyCount(0);					// 지혜
	m_Intelligence.Set_bBuff(0);
	m_Dex.Set_applyCount(0);							// 민첩
	m_Dex.Set_bBuff(0);
	m_Constitution.Set_applyCount(0);					// 건강
	m_Constitution.Set_bBuff(0);
	
	m_MaxLifePower.Set_applyCount(0);					// 최대 생명
	m_MaxForcePower.Set_applyCount(0);					// 최대 내력
	m_MaxConcentrationPower.Set_applyCount(0);			// 최대 의지
	
	m_MaxLifePower.Set_applyCountAdditional(0);			// 최대 생명
	m_MaxForcePower.Set_applyCountAdditional(0);			// 최대 내력
	m_MaxConcentrationPower.Set_applyCountAdditional(0);	// 최대 의지
	
#ifdef _XTS_BASEMINMAXWEAPONDAMAGE
	m_P_AttackDamage.bonus_min = 0.f;				// 최소 공격력
	m_P_AttackDamage.bonus_max = 0.f;				// 최대 공격력
#else
	m_P_AttackDamage.bonus = 0.f;					// 공격력
#endif
	m_P_AttackRate.bonus = 0.f;						// 공격 성공치
	m_P_Defence.bonus = 0.f;						// 방어력
	m_P_AvoidRate.bonus = 0.f;						// 회피력
	m_M_AttackDamage.bonus = 0.f;					// 기공 공격력
	m_M_AttackRate.bonus = 0.f;						// 기공 성공치
	m_M_Defence.bonus = 0.f;						// 기공 방어력
	m_M_AvoidRate.bonus = 0.f;						// 기공 회피력
	
	m_Strength.bonus = 0.f;							// 근력
	m_Zen.bonus = 0.f;								// 진기
	m_Intelligence.bonus = 0.f;						// 지혜
	m_Dex.bonus = 0.f;								// 민첩
	m_Constitution.bonus = 0.f;						// 건강
	
	m_MaxLifePower.bonus = 0.f;						// 최대 생명
	m_MaxForcePower.bonus = 0.f;					// 최대 내력
	m_MaxConcentrationPower.bonus = 0.f;			// 최대 의지
	
	Set_m_MoveSpeedFromEffect(0);						// 이동 속도

	m_ResistMinus		= 0;						// 음의 저항력
	m_ResistPlus		= 0;						// 양의 저항력
	m_ResistHon			= 0;						// 혼의 저항력
	m_ResistSin			= 0;						// 신의 저항력

	m_AttackDamageMinus	= 0;
	m_AttackDamagePlus	= 0;
	m_AttackDamageHon	= 0;
	m_AttackDamageSin	= 0;

	memset(m_PenaltyValueFromItem, 0, sizeof(m_PenaltyValueFromItem));	// penalty from item
	memset(m_WeaponMinDamageBonus, 0, sizeof(m_WeaponMinDamageBonus));	// weapon min damage bonus
	memset(m_WeaponMaxDamageBonus, 0, sizeof(m_WeaponMaxDamageBonus));	// weapon max damage bonus
	
	// disable skill / disable stype
	m_Lock.disableskill = 0;
	m_Lock.disableskillindex = 0;
	m_Lock.disablestype = 0;
	m_Lock.disablestypeindex = 0;
	
	// Reset Skill mod Table
#ifdef _XTS_MASTERY
	_XSkillItem::ResetSkillModTable();
#endif

	// Bonus 계산
	ApplyCharacterStateBonus();

	// 아이템으로 인한 Penalty 계산
	ApplyCharacterStatePenalty();

	// 최종 값 계산
	ApplyCharacterStateFinal();

	if( g_pJinStatus_Window )
	{
		g_pJinStatus_Window->SetParameter();
	}
	
	// 외상/내상 비율 다시 계산하기 - 08.08.19 Author:박소현

	// 외상을 입은 경우
	if(g_pLocalUser->m_CharacterInfo.Get_externalwoundrate() > 0)
	{
		if(g_pState_Window)
		{
			FLOAT percentage = (((FLOAT)g_pLocalUser->m_CharacterInfo.Get_externalwoundrate())/(FLOAT)g_pLocalUser->m_MaxLifePower.Get_result())*100.0f;
			if(percentage >= 40.0f)
				g_pState_Window->InsertStateListFromExIn(_XST_EXTERNAL, 0, g_pLocalUser->m_CharacterInfo.Get_externalwoundrate());
			else
				g_pState_Window->DeleteStateList(_XST_EXTERNAL, 0);
		}
	}

	// 내상을 입은 경우
	if(g_pLocalUser->m_CharacterInfo.Get_internalwoundrate() > 0)
	{
		if(g_pState_Window)
		{
			FLOAT percentage = (((FLOAT)g_pLocalUser->m_CharacterInfo.Get_internalwoundrate())/(FLOAT)g_pLocalUser->m_MaxForcePower.Get_result())*100.0f;
			if(percentage >= 40.0f)
				g_pState_Window->InsertStateListFromExIn(_XST_INTERNAL, 0, g_pLocalUser->m_CharacterInfo.Get_internalwoundrate());
			else
				g_pState_Window->DeleteStateList(_XST_INTERNAL, 0);
		}
	}
}

void _XLocalUser::ApplyCharacterStateBonus(void)
{
#ifdef _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	g_pSetIteminfo->GroupItemSkillUpdate();
#endif

	// 오기 계산
	// Skill, Effect, Condition, Item, 오의
	m_StateList.ApplyBonusPoint();

	// 나머지 계산
	// Skill, Effect, Condition, Item, 오의
	m_StateList.ApplyBonusAdditional();
}

void _XLocalUser::ApplyCharacterStatePenalty(void)
{
	// 무기 - 요구 내공 수위에 맞지 않는 경우 
	short reqLevel = 0;

#ifdef _XTS_ITEM_OPTIMIZATION
	if( _XGI_FC_WEAPON == GetEqualItemType(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType()) )
	{
		_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType());
		if( WeaponItem )
		{
			reqLevel = WeaponItem[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].sReqLevel;

			int nPercent = 0;
			int diff = reqLevel - m_CharacterInfo.Get_innerlevel();
			if(diff > 0)
			{
				if( reqLevel != 0 )
				{
					nPercent = diff * 100 / reqLevel;
					if(nPercent <= 20)
					{
						// 20%까지 공격력 10% 하락
						m_PenaltyValueFromItem[0] -= 10;
					}
					else if(nPercent <= 50)
					{
						// 50%까지 공격력 30% 하락
						m_PenaltyValueFromItem[0] -= 30;
					}
					else
					{
						// 공격력 50% 하락
						m_PenaltyValueFromItem[0] -= 50;
					}
				}
			}
			
			// 무기 - 요구 민첩에 맞지 않는 경우
			short reqDex = 0;
			reqDex = WeaponItem[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_sID()].sReqSim;
			
			short dex = 0;
			nPercent = 0;
			diff = 0;
			
			if(g_pLocalUser->m_Dex.Get_applyCount() > 0)
			{
				dex = g_pLocalUser->m_Dex.Get_org() + g_pLocalUser->m_Dex.bonus;
			}
			else
			{
				dex = g_pLocalUser->m_Dex.Get_org();
			}
			
			// 영약 값 더해주기
			dex += m_ElixirInfo.GetTotalBonus(4);
			
			diff = reqDex - dex;
			if(diff > 0)
			{
				if( reqDex != 0 )
				{
					nPercent = diff * 100 / reqDex;
					if(nPercent <= 20)
					{
						// 20%까지 공격성공치 5% 하락 (원래 음수)
						m_PenaltyValueFromItem[1] += 5;
					}
					else if(nPercent <= 50)
					{
						// 50%까지 공격성공치 10% 하락 (원래 음수)
						m_PenaltyValueFromItem[1] += 10;
					}
					else
					{
						// 공격성공치 20% 하락 (원래 음수)
						m_PenaltyValueFromItem[1] += 20;
					}
				}
			}
		}
	}	
#else
	if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON)
		reqLevel = g_WeaponItemProperty[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqLevel;
	else if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON2)
		reqLevel = g_WeaponItemProperty2[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqLevel;
	else if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON3)
		reqLevel = g_WeaponItemProperty3[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqLevel;

	int nPercent = 0;
	int diff = reqLevel - m_CharacterInfo.innerlevel;
	if(diff > 0)
	{
		if( reqLevel != 0 )
		{
			nPercent = diff * 100 / reqLevel;
			if(nPercent <= 20)
			{
				// 20%까지 공격력 10% 하락
				m_PenaltyValueFromItem[0] -= 10;
			}
			else if(nPercent <= 50)
			{
				// 50%까지 공격력 30% 하락
				m_PenaltyValueFromItem[0] -= 30;
			}
			else
			{
				// 공격력 50% 하락
				m_PenaltyValueFromItem[0] -= 50;
			}
		}
	}

	// 무기 - 요구 민첩에 맞지 않는 경우
	short reqDex = 0;
	if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON)
		reqDex = g_WeaponItemProperty[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqSim;
	else if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON2)
		reqDex = g_WeaponItemProperty2[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqSim;
	else if(m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_cType() == _XGI_FC_WEAPON3)
		reqDex = g_WeaponItemProperty3[m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].m_sID].sReqSim;

	short dex = 0;
	nPercent = 0;
	diff = 0;

	if(g_pLocalUser->m_Dex.applyCount > 0)
	{
		dex = g_pLocalUser->m_Dex.org + g_pLocalUser->m_Dex.bonus;
	}
	else
	{
		dex = g_pLocalUser->m_Dex.org;
	}

	// 영약 값 더해주기
	dex += m_ElixirInfo.GetTotalBonus(4);

	diff = reqDex - dex;
	if(diff > 0)
	{
		if( reqDex != 0 )
		{
			nPercent = diff * 100 / reqDex;
			if(nPercent <= 20)
			{
				// 20%까지 공격성공치 5% 하락 (원래 음수)
				m_PenaltyValueFromItem[1] += 5;
			}
			else if(nPercent <= 50)
			{
				// 50%까지 공격성공치 10% 하락 (원래 음수)
				m_PenaltyValueFromItem[1] += 10;
			}
			else
			{
				// 공격성공치 20% 하락 (원래 음수)
				m_PenaltyValueFromItem[1] += 20;
			}
		}
	}
#endif

	// 무기 - 요구 근력에 맞지 않는 경우 : 치명타율 하락 (디스플레이할 필요 없음)

	// 무기 - 내구도 하락했을 경우
	FLOAT fPercent = 0;
	if( g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_usMaxDur() != 0 )
	{
		fPercent = (FLOAT)g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_usCurDur() / 
						(FLOAT)g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_WEAPON_1].Get_m_usMaxDur() * 100.0f;
	}
	if(fPercent > 0.0f)
	{
		if(fPercent <= 10)
		{
			// 내구도 10% 이하 : 공격력 40% 감소, 공격성공치 4배(300%하락)
			m_PenaltyValueFromItem[0] -= 40;
			m_PenaltyValueFromItem[1] += 300;
		}
		else if(fPercent <= 20)
		{
			// 내구도 20% 이하 : 공격력 30% 감소, 공격성공치 3배(200%하락)
			m_PenaltyValueFromItem[0] -= 30;
			m_PenaltyValueFromItem[1] += 200;
		}
		else if(fPercent <= 40)
		{
			// 내구도 40% 이하 : 공격력 10% 감소, 공격성공치 2배(100%하락)
			m_PenaltyValueFromItem[0] -= 10;
			m_PenaltyValueFromItem[1] += 100;
		}
	}
}

void _XLocalUser::ApplyCharacterStateFinal(void)
{
	// 최대, 최소값 보정
	if(Get_m_MoveSpeedFromEffect() < -90)
		Set_m_MoveSpeedFromEffect(-90);
	if(Get_m_MoveSpeedFromEffect() > 100)
		Set_m_MoveSpeedFromEffect(100);
	
	// 최종 값 계산
	m_StateList.ApplyFinalValue();

	// 최소값 보정
	if(m_MaxLifePower.Get_applyCount() > 0 || m_MaxLifePower.Get_applyCountAdditional() > 0)
	{
		if(m_MaxLifePower.Get_result() < g_UsePointTable[m_CharacterInfo.Get_cClass()].vitality)
		{
			m_MaxLifePower.Set_result( g_UsePointTable[m_CharacterInfo.Get_cClass()].vitality );
		}
	}
	if(m_MaxForcePower.Get_applyCount() > 0 || m_MaxForcePower.Get_applyCountAdditional() > 0)
	{
		if(m_MaxForcePower.Get_result() < g_UsePointTable[m_CharacterInfo.Get_cClass()].inner)
		{
			m_MaxForcePower.Set_result( g_UsePointTable[m_CharacterInfo.Get_cClass()].inner );
		}
	}
	if(m_MaxConcentrationPower.Get_applyCount() > 0 || m_MaxConcentrationPower.Get_applyCountAdditional() > 0)
	{
		if(m_MaxConcentrationPower.Get_result() < g_UsePointTable[m_CharacterInfo.Get_cClass()].concenturate)
		{
			m_MaxConcentrationPower.Set_result( g_UsePointTable[m_CharacterInfo.Get_cClass()].concenturate );
		}
	}

	// 음양 혼신 데미지 적용
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
	m_P_MinAttackDamage += m_AttackDamageMinus;
	m_P_MinAttackDamage += m_AttackDamagePlus;
	m_P_MinAttackDamage += m_AttackDamageHon;
	m_P_MinAttackDamage += m_AttackDamageSin;

	m_P_MaxAttackDamage += m_AttackDamageMinus;
	m_P_MaxAttackDamage += m_AttackDamagePlus;
	m_P_MaxAttackDamage += m_AttackDamageHon;
	m_P_MaxAttackDamage += m_AttackDamageSin;

	m_M_MinAttackDamage += m_AttackDamageMinus;
	m_M_MinAttackDamage += m_AttackDamagePlus;
	m_M_MinAttackDamage += m_AttackDamageHon;
	m_M_MinAttackDamage += m_AttackDamageSin;

	m_M_MaxAttackDamage += m_AttackDamageMinus;
	m_M_MaxAttackDamage += m_AttackDamagePlus;
	m_M_MaxAttackDamage += m_AttackDamageHon;
	m_M_MaxAttackDamage += m_AttackDamageSin;
	
	// 음양혼신 데미지 중 최대값 설정
	m_CharacterInfo.m_4Eff_MaxDam = FindMaxDamage();

	// 음양 혼신 지역 패널티 적용
	m_ResistMinus += g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonepenalty;
	m_ResistPlus += g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonepenalty;
	m_ResistHon += g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonepenalty;
	m_ResistSin += g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].zonepenalty;
	
#endif

}

_XEN_4EFF_SYS _XLocalUser::FindMaxDamage()
{
	int damage[4];
	int maxindex = 0;

	damage[0] = m_AttackDamageMinus;
	damage[1] = m_AttackDamagePlus;
	damage[2] = m_AttackDamageHon;
	damage[3] = m_AttackDamageSin;

	int max = damage[0];

	for(int i = 1 ; i < 4 ; ++i)
	{
		if(max < damage[i])
		{
			max = damage[i];
			maxindex = i;
		}
		else if(max == damage[i])
		{
			int rad = _XRandom() % 2;
			if(rad == 1)
				maxindex = i;
		}
	}
	
	if(max <= 0)
	{
		return _XEN_4EFF_NONE;
	}
	else
	{
		switch(maxindex)
		{
		case 0 :
			return _XEN_4EFF_MINUS;
		case 1 :
			return _XEN_4EFF_PLUS;
		case 2 :
			return _XEN_4EFF_HON;
		case 3 :
			return _XEN_4EFF_SIN;
		}
	}

	return _XEN_4EFF_NONE;
}

void _XLocalUser::GetUserLevel( DWORD innerlevel, DWORD& level, DWORD& sublevel )
{
	level	   = g_UserLevelTable[innerlevel].level;
	sublevel   = g_UserLevelTable[innerlevel].sublevel;
}

void _XLocalUser::SelectCharacter( int characterindex )
{
	if( characterindex >= m_CharacterCount )
	{
		_XDWINPRINT( "WARNING : _XLocalUser::SelectCharacter : Invalid character index");
		return;
	}

	m_SelectedCharacter =	characterindex;

	memcpy( &m_CharacterInfo, &m_AllCharacterInfo[m_SelectedCharacter], sizeof( _XCHARACTER_INFO ) );

	RecalcLocalUserParameter();	

	if( m_CharacterInfo.gender == _XGENDER_MAN )	
		m_ModelDescriptor.LinkBipedObject( &g_MaleBipedObject );
	else
		m_ModelDescriptor.LinkBipedObject( &g_FemaleBipedObject );

	SetCharacterModel();	
}

void _XLocalUser::SetDieMotion(void)
{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	if(!m_bIsBossBattle && !GetEnterMatchEvent()) 
#else
	if( !m_bIsBossBattle )
#endif
	{
		// 부활 윈도우 띄우기
		_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
		if( pRebirth_Window && !g_ZeroLevelMode && m_UserState != _XDEF_USERSTATE_MATCH_FIGHT)
		{
			pRebirth_Window->SetRebirthMode(_XREBIRTH_HONSU);
//			pRebirth_Window->SetCastleBattleState(bCBMode);

#ifdef _ACCLAIM_IGAADSYSTEM			
			if( g_IGAADWrapper.CheckExistContent( _XIGAADZONE_DEATHVIDEOAD ) )
			{
				pRebirth_Window->m_ShowIGAADButton = TRUE;
			}
			else
			{
				pRebirth_Window->m_ShowIGAADButton = FALSE;
			}
#endif

			pRebirth_Window->ShowWindow(TRUE);
		}
	}

	SetAnimation( _XACTION_DIE );
	m_ModelDescriptor.SetLockEndFrame( TRUE );

	// display 되지 않은 전투 결과 처리
	ProcessMobDie();
	
	ResetViewTarget();
	ResetAttackTarget();
	ResetSkillAnimation();

#ifdef _XDEF_PARTY_NOTAVAILABLE
	// 자리비움 이면 해제
	if(m_bOut)
	{
		SetOut(FALSE);
	}
#endif

}

void _XLocalUser::SetBlowMotion(void)
{
	if(GetMotionClass() == _XACTION_DIE)
		return;

	ProcessMobDie();

	if(GetMotionClass() == _XACTION_RESCUE)
		return;

	char usefultype = 0;
	if(m_SelectedSkillID > 0)
		usefultype = g_SkillProperty[m_SelectedSkillID]->usefulType;
	else 
		usefultype = _XSI_USEFULTYPE_ACTIVESKILL;

	switch(usefultype)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
	case _XSI_USEFULTYPE_CASTINGSPELL :
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
	case _XSI_USEFULTYPE_FINISHSKILL :
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
	case _XSI_USEFULTYPE_HIDDENMA :
		return;
	default :
		break;
	}

	SetAnimation(_XACTION_BLOW);
}

void _XLocalUser::SetDefenceMotion(void)
{
	if(GetMotionClass() == _XACTION_DIE)
		return;

	ProcessMobDie();

	if(GetMotionClass() == _XACTION_RESCUE)
		return;

	char usefultype = 0;
	if(m_SelectedSkillID > 0)
		usefultype = g_SkillProperty[m_SelectedSkillID]->usefulType;
	else 
		usefultype = _XSI_USEFULTYPE_ACTIVESKILL;

	switch(usefultype)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
	case _XSI_USEFULTYPE_CASTINGSPELL :
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
	case _XSI_USEFULTYPE_FINISHSKILL :
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
	case _XSI_USEFULTYPE_HIDDENMA :
		return;
	default :
		break;
	}

	SetAnimation(_XACTION_DEFENCE);

	// Defence 하는 sound	
	if( m_CharacterInfo.modeldescript.item1 == 0 )
	{
		_XPlaySoundEffect( g_WeaponSoundIndexList[ 4 ], m_Position );
	}
	else
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
			if( WeaponItem )
			{
				int weaponmaterial = WeaponItem[m_CharacterInfo.modeldescript.item1].ucQuality2;		
				if( weaponmaterial < g_WeaponSoundCount )
				{
					_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			int weaponmaterial = g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].ucQuality2;		
			if( weaponmaterial < g_WeaponSoundCount )
			{
				_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )		
		{
			int weaponmaterial = g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].ucQuality2;
			if( weaponmaterial < g_WeaponSoundCount )
			{
				_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )		
		{
			int weaponmaterial = g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].ucQuality2;
			if( weaponmaterial < g_WeaponSoundCount )
			{
				_XPlaySoundEffect( g_WeaponSoundIndexList[ weaponmaterial ], m_Position );
			}
		}
#endif
	}
}

void _XLocalUser::SetRebirthBinsaMode(void)
{
	m_DyingStartTime = 0;
	m_DyingTimer = 0;
	m_PrevMoveTime = 0;
	m_LastCheckedDyingStartTime = -1;

	ResetSkillAnimation();
	ResetTrainingMode();
	_XUser::SetRebirthBinsaMode();

	_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
	if(pRebirth_Window)
	{
		if(pRebirth_Window->GetShowStatus())
		{
			pRebirth_Window->ShowWindow(FALSE);
		}
	}
}


BOOL _XLocalUser::GetNextMove(D3DXVECTOR3& next)
{
	if( !gApp->m_bHasFocus				   ||
		GetMotionClass() == _XACTION_DYING || 
		GetMotionClass() == _XACTION_DIE   || 
		GetMotionClass() == _XACTION_REBIRTH )
	{
		m_HiperRunFailedFlag = TRUE;
		m_TargetPosition = m_Position;
		next = m_Position;

        //Author : 양희왕 //breif : 경공 사용 중 포커스를 잃었을 때 딜레이 처리.. 이곳이 가장 좋겠다.
        if( m_SelectedSkillID && m_UseSpeedSkill )
		{
			if(	g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
				_XSkillItem::GetTargetType(m_SelectedSkillID) == _XSI_TARGETTYPE_SELF &&
				g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )
			{
                m_UseSpeedSkill = FALSE;
				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);
			}
		}
		return FALSE;
	}

	FLOAT dx = fabs(m_TargetPosition.x - m_Position.x);
	FLOAT dz = fabs(m_TargetPosition.z - m_Position.z);
	FLOAT leftlength = _XFC_sqrt(dx*dx + dz*dz);	

	if( leftlength < EPSILON3 )
	{
		if( m_PathNodeCount > 0 ) // 패스가 계산되었다면 
		{
			if( m_CurrentNavigatePathNodeIndex < m_PathNodeCount-1 ) // 남은 패스 포인트가 있다면 다음 포인트 설정하고 계속 이동
			{
				m_CurrentNavigatePathNodeIndex++;
				
				if( m_CurrentNavigatePathNodeIndex == m_PathNodeCount-1 )
				{
					m_TargetPosition.x = m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.x;
					m_TargetPosition.z = m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.y;
				}
				else
				{
					m_TargetPosition.x = ( m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
					m_TargetPosition.z = ( m_PathPointArray[m_CurrentNavigatePathNodeIndex].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
				}
				
				m_LastRotateAngle = _XMeshMath_GetRotateRadian(m_Position, m_TargetPosition);
				
				FLOAT elapsedangle_1 = fabs(m_LastRotateAngle - m_RotateAngle);
				FLOAT elapsedangle_2 = (_X_PI - fabs(m_LastRotateAngle)) + (_X_PI - fabs(m_RotateAngle));
				
				if(elapsedangle_1 <= elapsedangle_2)
				{
					if(m_LastRotateAngle >= m_RotateAngle)
						m_RotateDirection = _XROTATE_CLOCKWISE;
					else
						m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
				}
				else
				{
					if(m_LastRotateAngle >= m_RotateAngle)
						m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
					else
						m_RotateDirection = _XROTATE_CLOCKWISE;
				}

				//m_PrevMoveTime = 0;
			}
			else
			{
				m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함
				m_TargetPosition = m_Position;
				next = m_Position;
				m_PrevMoveTime = 0;
				return FALSE;
			}
		}
		else
		{
			m_TargetPosition = m_Position;
			next = m_Position;
			m_PrevMoveTime = 0;
			return FALSE;
		}
	}	

	FLOAT movespeed = 0;	


	// 경공 모드 	
	if( g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL &&
		g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03 )	// 경공
	{
		if(gpInput->CheckKeyPress(DIK_LMENU) && !m_HiperRunFailedFlag )
		{
			if(m_SelectedSkillItem)
			{
				if( m_SelectedSkillItem->m_dwCooldownStartTime > 0 )
				{
					m_HiperRunFailedFlag = TRUE;
					return FALSE;
				}
			}

			if( m_CharacterInfo.Get_current_forcepower() <= 0 )
			{
				if( m_UseSpeedSkill )
				{
					m_UseSpeedSkill = FALSE;
					ResetSkillAnimation();
					m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
					SetAnimation( _XACTION_IDLE );
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
					m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함
					m_TargetPosition = m_Position;
					next = m_Position;			
					m_HiperRunFailedFlag = TRUE;
					//m_HiperRunStopSignalSended = TRUE;
				}
				return FALSE;
			}
			else
			{
				//m_HiperRunStopSignalSended = FALSE;

				XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;		
				if( !pProc_Maingame->m_bNPCDialogMode && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView ) 
				{


#ifdef _XDEF_NEWANIMATIONLIST
					
					int baseanimindex = ( this->m_CharacterInfo.gender == _XGENDER_MAN ) ? 
										  g_SkillProperty[m_SelectedSkillID]->motion_male : 
							              g_SkillProperty[m_SelectedSkillID]->motion_female;
							
					if( m_CharacterInfo.modeldescript.item1 == 0 )
					{
					}
					else
					{
#ifdef _XTS_ITEM_OPTIMIZATION
						if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
						{
							_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
							if( WeaponItem )
							{
								switch( WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType )
								{
								case _XGI_SC_WEAPON_SHORT				:				// 단병기
									if( WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
									{
									}
									else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
									{
										if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
											baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
										else
											baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
									}
									else
									{
										baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
									}
									break;
								case _XGI_SC_WEAPON_LONG				:				// 장병기
									baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
									break;				
								case _XGI_SC_WEAPON_SOFT				:				// 연병기
									baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
									break;
								case _XGI_SC_WEAPON_HIDDEN				:				// 암기
									break;
								case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
									break;
								case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
									break;
								}
							}
						}
#else
						_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
						
						if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
							pWeaponItemProperty = g_WeaponItemProperty;
						else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
							pWeaponItemProperty = g_WeaponItemProperty2;
						else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
							pWeaponItemProperty = g_WeaponItemProperty3;
						
						switch( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType )
						{
						case _XGI_SC_WEAPON_SHORT				:				// 단병기
							if( pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
							{
							}
							else if(pWeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
							{
								if(m_CharacterInfo.animationdescript.idletype == _XBATTLE_PEACE)
									baseanimindex += _XDEF_FASTMOVEINDEX_SWORD;
								else
									baseanimindex += _XDEF_FASTMOVEINDEX_BATTLESWORD;
							}
							else
							{
								baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							}
							break;
						case _XGI_SC_WEAPON_LONG				:				// 장병기
							baseanimindex += _XDEF_FASTMOVEINDEX_LONGWEAPON;
							break;				
						case _XGI_SC_WEAPON_SOFT				:				// 연병기
							baseanimindex += _XDEF_FASTMOVEINDEX_SHORTWEAPON;
							break;
						case _XGI_SC_WEAPON_HIDDEN				:				// 암기
							break;
						case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
							break;
						case _XGI_SC_WEAPON_SPECIAL				:				// 특수무기
							break;
						}
#endif
					}
					
					if( m_ModelDescriptor.GetCurMotion() == baseanimindex ) // start
					{
						movespeed = 0.0f; // 경공 준비중에는 정지...	
					}
					else if( m_ModelDescriptor.GetCurMotion() == baseanimindex+1 ) // repeat
					{
						if( m_CharacterInfo.Get_current_forcepower() > 0 )
						{
							if( m_SelectedSkillID )
							{
								movespeed = _XSkillItem::GetFinishSpeed(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillItem->m_sSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].finishSpeed; //_XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1;
							}
							else
							{
								movespeed = _XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1;
							}
						}
						else
						{
							//movespeed = m_fMoveSpeed;
							ResetSkillAnimation();
							m_UseSpeedSkill = FALSE;
							
							SetAnimation( _XACTION_IDLE );
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함
							m_TargetPosition = m_Position;
							next = m_Position;
							return FALSE;							
						}
						
						if( m_ChannelingSendLastPacketTime == 0 )
						{
							m_ChannelingSendLastPacketTime = g_LocalSystemTime;
						}
						else
						{
							if( (g_LocalSystemTime - m_ChannelingSendLastPacketTime) > 1000 ) // 1초에 한번씩 전송 
							{
								m_ChannelingSendLastPacketTime = g_LocalSystemTime;// + (g_LocalSystemTime - m_ChannelingSendLastPacketTime - 1000);
								if( m_CharacterInfo.Get_current_forcepower() >= _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel)/*g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce*/ )
								{
									m_CharacterInfo.Set_current_forcepower( m_CharacterInfo.Get_current_forcepower() - _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel) );//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce;
								}
								else
								{
									m_CharacterInfo.Set_current_forcepower(0);
									
									ResetSkillAnimation();
									m_UseSpeedSkill = FALSE;
									
									m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
									SetAnimation( _XACTION_IDLE );
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
									m_TargetPosition = m_Position;
									m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함
									next = m_Position;
									return FALSE;
								}
								
								if( g_pJinStatus_Window )
								{
									g_pJinStatus_Window->SetParameter();	
								}		
								
								g_NetworkKernel.SendPacket(MSG_NO_SELF_CHANNEL_START_REQUEST);
							}
						}	
					}			
					else if( m_ModelDescriptor.GetCurMotion() == baseanimindex+2 )  // finish
					{
						movespeed = 0.0f; // 경공 준비중에는 정지...
					}
								
#else //#ifdef _XDEF_NEWANIMATIONLIST
					
					if( g_LodTerrain.m_3PCamera.mp_fFov >= _XDEF_MAXFOV )
					{
						if( m_CharacterInfo.current_forcepower > 0 )
						{
							if( m_SelectedSkillID )
							{
								movespeed = _XSkillItem::GetFinishSpeed(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillItem->m_sSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].finishSpeed; //_XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1;
							}
							else
							{
								movespeed = _XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1;
							}
						}
						else
						{
							//movespeed = m_fMoveSpeed;
							ResetSkillAnimation();
							m_UseSpeedSkill = FALSE;
							
							SetAnimation( _XACTION_IDLE );
							g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
							m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함
							m_TargetPosition = m_Position;
							next = m_Position;
							return FALSE;
						}

						if( m_ChannelingSendLastPacketTime == 0 )
						{
							m_ChannelingSendLastPacketTime = g_LocalSystemTime;
						}
						else
						{
							if( (g_LocalSystemTime - m_ChannelingSendLastPacketTime) > 1000 ) // 1초에 한번씩 전송 
							{
								m_ChannelingSendLastPacketTime = g_LocalSystemTime;// + (g_LocalSystemTime - m_ChannelingSendLastPacketTime - 1000);
								if( m_CharacterInfo.current_forcepower >= _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel)/*g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce*/ )
									m_CharacterInfo.current_forcepower -= _XSkillItem::GetCostForce(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].costForce;
								else
								{
									m_CharacterInfo.current_forcepower = 0;
									
									ResetSkillAnimation();
									m_UseSpeedSkill = FALSE;
									
									m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
									SetAnimation( _XACTION_IDLE );
									g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
									m_TargetPosition = m_Position;
									m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함
									next = m_Position;
									return FALSE;
								}

								if( g_pJinStatus_Window )
								{
									g_pJinStatus_Window->SetParameter();	
								}		

								g_NetworkKernel.SendPacket(MSG_NO_SELF_CHANNEL_START_REQUEST);
							}
						}

					}
					else if(g_LodTerrain.m_3PCamera.mp_fFov > g_DefaultCameraFov)
					{
						movespeed = 0.0f; // 경공 준비중에는 정지...
					}
					else
					{
						//movespeed = m_fMoveSpeed;
						movespeed = 0.0f; // 경공 준비중에는 정지...
					}
#endif
				}
				else
				{
					//movespeed = m_fMoveSpeed;
					movespeed = 0.0f; // 경공 준비중에는 정지...
				}
			}
		}
		else
		{
			//m_HiperRunStopSignalSended = FALSE;

			// by mahwang
/*			if (m_UserState == _XDEF_USERSTATE_TRAINING && m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
				movespeed = m_fMoveSpeed * 0.2f;
			else*/

			if(Get_m_MoveSpeedFromEffect() != 0)
			{
				movespeed = Get_m_fMoveSpeed() + (Get_m_fMoveSpeed() * ((FLOAT)Get_m_MoveSpeedFromEffect()/100.0f));
			}
			else
				movespeed = Get_m_fMoveSpeed();
		}
	}
	else
	{
		// by mahwang
/*		if (m_UserState == _XDEF_USERSTATE_TRAINING && m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
			movespeed = m_fMoveSpeed * 0.2f;
		else*/

		if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL && 
			(m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7))
		{
			movespeed = m_FinishSpeed;
		}
		else if(Get_m_MoveSpeedFromEffect() != 0)
		{
			movespeed = Get_m_fMoveSpeed() + (Get_m_fMoveSpeed() * ((FLOAT)Get_m_MoveSpeedFromEffect()/100.0f));
		}
		else
		{
			movespeed = Get_m_fMoveSpeed();
		}
	}
			
	FLOAT		elapsedtime;
	D3DXVECTOR3 directionvector;
	FLOAT		curtimemovelength;

	if(m_PrevMoveTime == 0.0f)
	{
		m_PrevMoveTime = g_fLocalSystemTime;
		elapsedtime = 0;
		return TRUE;
	}
	else
	{
		XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	// hotblood zerolevel
		if( pZeroLevelProcess  )
		{
			if( pZeroLevelProcess->GetTutorialModeOn() )
				m_PrevMoveTime = g_fLocalSystemTime;
		}
		elapsedtime= g_fLocalSystemTime - m_PrevMoveTime;
		curtimemovelength  = elapsedtime * movespeed;	// 이번 프레임에 갈 거리

		if( curtimemovelength > leftlength )
			curtimemovelength = leftlength;
	}
	
	if( leftlength < 0.00001f ) // 타켓거리가 갈거리 보다 짧으면 남은거리만큼 간다.
	{	// 이번타임에 타겟에 이동된다. (도착상태)
		
		m_TargetPosition = next = m_Position;
		//m_PrevMoveTime = 0.0f;
		
		if(m_AttackTarget)
		{
			if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			{
				if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
				{
					if(GetMotionClass() != _XACTION_DYING)
					{
//						_XDWINPRINT("[GetNextMotion] Set Attack");
						SetAnimation(_XACTION_ATTACK);
						m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함
					}
				}
			}
			else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)m_AttackTarget;
				
				m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pUser->m_Position);
				UpdatePosition();
				
				if(pUser->GetMotionClass() == _XACTION_DYING)
				{
					ProcessRescueUser();
					m_PathNodeCount = 0;
				}
			}
		}
		else
		{
			if(m_LastRotateAngle == m_RotateAngle && !m_UseSpeedSkill)
			{
				if( GetMotionClass() == _XACTION_MOVE )
				{
//					SetAnimation(_XACTION_IDLE);
				}
			} // if(m_LastRotateAngle == m_RotateAngle)
		}

		return FALSE;
	}
	
	// Time slicing...
//	int repeatcollidechecktime = (int) (curtimemovelength / (_XDEF_CHARACTERMOVESPEED_RUN * 30.0f) );
//	FLOAT lastdistance = fmod( curtimemovelength, (_XDEF_CHARACTERMOVESPEED_RUN * 30.0f) );

	int repeatcollidechecktime = (int) (curtimemovelength / (movespeed * 30.0f) );
	FLOAT lastdistance = fmod( curtimemovelength, (movespeed * 30.0f) );
	if( lastdistance > EPSILON4 )
	{
		repeatcollidechecktime++;
	}

	POINT		pt;
	FLOAT		terrainheight = m_Position.y;
	D3DXVECTOR3 nextposition, nextjumpposition, contactpoint, contactfunctionalobjectpoint;
	_XOBBData*  pUnderObb = NULL;
	_XOBBData*  pPreviousGroundCollideOBB = m_pGoundCollideOBB;		

	m_PrevMoveTime = g_fLocalSystemTime;
	
	/*
	if( m_Velocity.y > 0.0f )
	{
		m_Velocity.y += _XDEF_CHARACTERGRAVITY * g_fElapsedFrameMilisecondTime;
		if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;

		FLOAT Length = sqrtf(m_Velocity.y * m_Velocity.y);
		if ( Length > _XDEF_MAXJUMPVELOCITY )
		{
			m_Velocity.y *= ( _XDEF_MAXJUMPVELOCITY / Length );
		}

		nextjumpposition.y += m_Velocity.y * g_fElapsedFrameMilisecondTime;
		
		D3DXVECTOR3 vecDec = -m_Velocity;
		D3DXVec3Normalize( &vecDec, &vecDec );

		Length = D3DXVec3Length( &m_Velocity );
		FLOAT Dec = (_XDEF_CHARACTERFRICTION * g_fElapsedFrameMilisecondTime);
		if ( Dec > Length ) Dec = Length;

		m_Velocity += vecDec * Dec;
		if( m_Velocity.y < 0.0f ) m_Velocity.y = 0.0f;
	}
	
	nextposition = nextjumpposition;

	*/

	// 진행위치
	D3DXVECTOR3 currentposition = m_Position;	
	
	// 갈 방향의 벡터 생성	
	directionvector = D3DXVECTOR3(m_TargetPosition.x - currentposition.x, 0.0f, m_TargetPosition.z - currentposition.z) ;
	D3DXVec3Normalize(&directionvector, &directionvector);
	

	// 최소 시간단위로 쪼갠다...
//	FLOAT separatedmovelength = (_XDEF_CHARACTERMOVESPEED_RUN * 30.0f);

	FLOAT separatedmovelength = (movespeed * 30.0f);
	for( int repeat = 0; repeat < repeatcollidechecktime; ++repeat )
	{
		if( (lastdistance > EPSILON4) && repeat == repeatcollidechecktime-1 ) separatedmovelength = lastdistance;

		// 계속 타겟까지 가야하면 다음 위치 계산
		nextposition = currentposition + (directionvector * separatedmovelength);
				 
		_XLODTILE*	pTerrainTile = g_LodTerrain.GetTerrainTile( nextposition , &pt );

		if( !pTerrainTile )
		{
			next = m_Position;
			m_PrevMoveTime = 0.0f;
			return FALSE;
		}

		terrainheight = g_LodTerrain.GetTerrainAndOBBHeightWithCurrentHeight( nextposition, pUnderObb, m_CurrentPositionTerrainNormal );
		
		// 다음 중력값 적용 높이가 지형보다 낮은경우나 무릅보다 작으면 위치 보정
		if( terrainheight > currentposition.y && terrainheight < currentposition.y + _XDEF_RIDEOBBLIMITHEIGHT )
		{
			directionvector = D3DXVECTOR3(nextposition.x - currentposition.x, terrainheight - currentposition.y, nextposition.z - currentposition.z) ;
			D3DXVec3Normalize(&directionvector, &directionvector);
			nextposition.y = terrainheight;
		}
		else
		{
			directionvector = D3DXVECTOR3(nextposition.x - currentposition.x, 0.0f, nextposition.z - currentposition.z) ;
			D3DXVec3Normalize(&directionvector, &directionvector);
			nextposition.y = currentposition.y;
		}
		
		m_pGoundCollideOBB = NULL;
		m_pCollideOBB = NULL;
			
		// Rotate obb axis
		D3DXMATRIX	rotmat, newobbmat;
		D3DXMatrixRotationY( &rotmat, 6.28318530717958647692528676624f - m_LastRotateAngle );
		D3DXMatrixMultiply( &newobbmat, &rotmat, &g_BaseOBBAxisMatrix );	
		m_CharacterOBB.m_akAxis[0].x = newobbmat._11;
		m_CharacterOBB.m_akAxis[0].y = newobbmat._21;
		m_CharacterOBB.m_akAxis[0].z = newobbmat._31;
		m_CharacterOBB.m_akAxis[1].x = newobbmat._12;
		m_CharacterOBB.m_akAxis[1].y = newobbmat._22;
		m_CharacterOBB.m_akAxis[1].z = newobbmat._32;
		m_CharacterOBB.m_akAxis[2].x = newobbmat._13;
		m_CharacterOBB.m_akAxis[2].y = newobbmat._23;
		m_CharacterOBB.m_akAxis[2].z = newobbmat._33;
		
		// Expand obb
		FLOAT halfmovelength = separatedmovelength / 2.0f;
		m_CharacterOBB.m_afExtent[2] = 0.32f + halfmovelength;
		m_CharacterOBB.m_kCenter = currentposition + ((nextposition - currentposition) / 2.0f);
		m_CharacterOBB.m_kCenter.y = nextposition.y + 0.89f;
		m_CharacterOBB.RecalcRadius();
		
		// Check collision
		//int collidedetectedcount = 0; // 경공시 충돌하면 멈춘다

		if( !pTerrainTile->svObbList.empty() && !g_FreeMoveMode )
		{
			_XOBBData* pObbData;
			svdef_OBBData::iterator it;
			for (it = pTerrainTile->svObbList.begin(); it != pTerrainTile->svObbList.end(); ++ it)
			{
				pObbData = (*it);
				if( m_CharacterOBB.IntersectOBBvsOBB( *pObbData, contactpoint ) )
				{
					if( pObbData->m_OBBLevel == 1 )
						m_pCollideOBB = pObbData;

					ProcessCollision(contactpoint, directionvector, nextposition );
					m_CharacterOBB.m_kCenter.x = nextposition.x;
					m_CharacterOBB.m_kCenter.y = nextposition.y + 0.89f;
					m_CharacterOBB.m_kCenter.z = nextposition.z;

					/*
					// Regenerate axis
					FLOAT rotateangle = _XMeshMath_GetRotateRadian(m_Position, nextposition);
					D3DXMatrixRotationY( &rotmat, 6.28318530717958647692528676624f - rotateangle );
					D3DXMatrixMultiply( &newobbmat, &rotmat, &g_BaseOBBAxisMatrix );
					m_CharacterOBB.m_akAxis[0].x = newobbmat._11;
					m_CharacterOBB.m_akAxis[0].y = newobbmat._21;
					m_CharacterOBB.m_akAxis[0].z = newobbmat._31;
					m_CharacterOBB.m_akAxis[1].x = newobbmat._12;
					m_CharacterOBB.m_akAxis[1].y = newobbmat._22;
					m_CharacterOBB.m_akAxis[1].z = newobbmat._32;
					m_CharacterOBB.m_akAxis[2].x = newobbmat._13;
					m_CharacterOBB.m_akAxis[2].y = newobbmat._23;
					m_CharacterOBB.m_akAxis[2].z = newobbmat._33;

					FLOAT dx = fabs(nextposition.x - m_Position.x);
					FLOAT dz = fabs(nextposition.z - m_Position.z);
					FLOAT movelength = _XFC_sqrt(dx*dx + dz*dz);
					
					m_CharacterOBB.m_afExtent[2] = 0.16f + (movelength / 2.0f);
					*/
										
#ifdef _XDWDEBUG
					pObbData->m_Collide	= TRUE;
#endif				
					//collidedetectedcount++;
				}
#ifdef _XDWDEBUG
				else pObbData->m_Collide = FALSE;
#endif					
			}
		}

		if( g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject != 0 && !g_FreeMoveMode )
		{
			for( int i = 0; i < g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject; ++i )
			{
				if( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_bCollidable )// 충돌 가능할 때만 충돌 처리를 한다. ( 문이 부서지면 충돌처리를 하지 않는다.)
				{
					if( m_CharacterOBB.IntersectOBBvsOBB( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i], contactfunctionalobjectpoint ) )
					{
						m_pCollideOBB = &g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i];
						
						ProcessCollision(contactfunctionalobjectpoint, directionvector, nextposition );
						m_CharacterOBB.m_kCenter.x = nextposition.x;
						m_CharacterOBB.m_kCenter.y = nextposition.y + 0.89f;
						m_CharacterOBB.m_kCenter.z = nextposition.z;
					}
#ifdef _XDWDEBUG
					g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_Collide = TRUE;
#endif				
				}
#ifdef _XDWDEBUG
				else g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_Collide = FALSE;
#endif					
			}
		}
		
		currentposition = nextposition;

		terrainheight = g_LodTerrain.GetTerrainAndOBBHeightWithCurrentHeight( currentposition, m_pGoundCollideOBB, m_CurrentPositionTerrainNormal );
		
		if( m_pGoundCollideOBB )
		{
			if( m_Velocity.y < EPSILON4 )
			{
				if( currentposition.y < terrainheight || currentposition.y + _XDEF_RIDEOBBLIMITHEIGHT > terrainheight )
				{
					currentposition.y = terrainheight;
					m_CharacterOBB.m_kCenter.y = currentposition.y + 0.89f;
				}
			}
		}
	}

	if( !m_pGoundCollideOBB )
	{
		if( currentposition.y < terrainheight || currentposition.y + _XDEF_RIDEOBBLIMITHEIGHT > terrainheight )
		{
			currentposition.y = terrainheight;
			m_CharacterOBB.m_kCenter.y = currentposition.y + 0.89f;
		}
	}

	nextposition = currentposition;

#ifdef _XDWDEBUG	
	m_CharacterOBB.CreateOBBModel();
#endif	
	
	if( m_pCollideOBB )
	{
		next = m_Position;
		m_TargetPosition = m_Position;
		m_PrevMoveTime = 0.0f;
		//m_PrevMoveTime = 0.0f;
		m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함

		if( m_UseSpeedSkill )
		{			
			if(m_SelectedSkillItem)
			{
				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			}

			m_HiperRunFailedFlag = TRUE;			
		}

		return FALSE;
	}

	// Check water object
	S_WATER* curwater = g_LodTerrain.m_pWaterObjectManager->GetWaterObject(pt.y + (pt.x * g_LodTerrain.m_TileCount));
	
	if(curwater && curwater->enabled && (curwater->height > (nextposition.y + g_fWaterDepth)))
	{
		next = m_Position;
		m_TargetPosition = m_Position;
		//m_PrevMoveTime = 0.0f;
		m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함

		if( m_UseSpeedSkill )
		{
			if(m_SelectedSkillItem)
			{
				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			}

			m_HiperRunFailedFlag = TRUE;
		}

		return FALSE;
	}
	
	int	xcoord = _XFC_FloatToIntRet( ( nextposition.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	int zcoord = _XFC_FloatToIntRet( ( nextposition.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;		

	if( g_LodTerrain.GetPathFindAreaNeighborInfo(zcoord, xcoord ) > 7 )
	{
		next = m_Position;
		m_TargetPosition = m_Position;
		m_PrevMoveTime = 0.0f;
		m_PathNodeCount = 0;	// 패스 파인딩 포인트 개수를 초기화 하여 멈추게 함

		if( m_UseSpeedSkill )
		{
			if(m_SelectedSkillItem)
			{
				m_SelectedSkillItem->m_dwCooldownStartTime = g_LocalSystemTime;
				m_SelectedSkillItem->m_dwCooldownTime = _XSkillItem::GetCoolDown(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].coolDown;
			}

			m_HiperRunFailedFlag = TRUE;
		}

		return FALSE;
	}

	/*
	if( !m_pGoundCollideOBB )
	{
		D3DXQUATERNION normalrotquat;
		D3DXVECTOR3	upvector = D3DXVECTOR3(0.0f,1.0f,0.0f);
		
		// Check terrain slant rate
		if( m_CurrentPositionTerrainNormal.x != 0.0f || m_CurrentPositionTerrainNormal.y != 1.0f || m_CurrentPositionTerrainNormal.z != 0.0f ) // check exactly upvector...
		{			
			// get angle from terrain normal vector...
			FLOAT theta = _XGetAngleFromVectors( &upvector, &m_CurrentPositionTerrainNormal );
			
			if( theta > 0.523598f )
			{
				if( nextposition.y > m_Position.y ) // 다음 포인트의 높이가 높을때만 가지 못한다.
				{
					FLOAT slowfactor = ( theta - 0.523598f ) / _X_PI_DEV180 * 4.0f;
					nextposition   = m_Position + ( directionvector * (curtimemovelength/slowfactor) );
					nextposition.y = g_LodTerrain.GetTerrainHeight( nextposition , &m_CurrentPositionTerrainNormal );
				}
			}
		}
	}
	*/
	
	m_PrevPosition = m_Position;
	next = nextposition;
	
	if(GetMotionClass() == _XACTION_DYING)
	{
		if(nextposition.x != m_Position.x && nextposition.z != m_Position.z)
		{
			m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
		}
		else return FALSE;
	}
	else if(GetMotionClass() != _XACTION_MOVE)
	{
		if(nextposition.x != m_Position.x || nextposition.z != m_Position.z)
		{
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL && 
				(m_CurrentBattleState == _XUSER_STATE_6 || m_CurrentBattleState == _XUSER_STATE_7))
			{
			}
/*#ifdef _XDEF_BLOCK_MOVINGATTACK
			else if(GetMotionClass() == _XACTION_ATTACK) //Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03
			{
				//공격 중엔 이동을 막아 보자..
			}
#endif		*/
			else if( m_ModelDescriptor.GetCurMotion() >= _XAN_EMOTION_MARRYCLAPPINGHAND && 
				m_ModelDescriptor.GetCurMotion() <= _XAN_EMOTION_MARRYBOWPOLITELY )
			{
				//인사나 박수 중일때는 이동 불가 
				// 경고 메세지?
				// 무기 삭제된 상태에서는 다시 장착하기 전까지 다른 행동을 할 수 없다.
			}
			else
			{
				m_CharacterInfo.animationdescript.movetype = _XMOVETYPE_RUN;
				SetAnimation(_XACTION_MOVE);

				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
		else return FALSE;
	}	

	return TRUE;
}

// SetSkillAnimation - Active skill 용
void _XLocalUser::SetSkillAnimation(void)
{	
	if( m_SelectedSkillID == 0 )
	{
		m_MartialStep = 0;
		m_SkillStarLevel = 0;
		
		if(m_CharacterInfo.modeldescript.item1 == 0)
		{
			m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;
		}
		else
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					switch(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType)
					{
					case _XGI_SC_WEAPON_SHORT :
						{
							if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// 권각
							else if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;			// 부법
							else
								m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// 단병기
						}
						break;
					case _XGI_SC_WEAPON_LONG :
						{
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 장병기
						}
						break;
					case _XGI_SC_WEAPON_SOFT :
					case _XGI_SC_WEAPON_HIDDEN :
					case _XGI_SC_WEAPON_MUSICAL :
						{
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 임시
						}
						break;
					case _XGI_SC_WEAPON_SPECIAL :
						{
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// 깅병기
						}
						break;
					}
				}
			}
#else
			if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				switch(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType)
				{
				case _XGI_SC_WEAPON_SHORT :
					{
						if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// 권각
						else if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;			// 부법
						else
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// 단병기
					}
					break;
				case _XGI_SC_WEAPON_LONG :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 장병기
					}
					break;
				case _XGI_SC_WEAPON_SOFT :
				case _XGI_SC_WEAPON_HIDDEN :
				case _XGI_SC_WEAPON_MUSICAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 임시
					}
					break;
				case _XGI_SC_WEAPON_SPECIAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// 깅병기
					}
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				switch(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType)
				{
				case _XGI_SC_WEAPON_SHORT :
					{
						if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// 권각
						else if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;			// 부법
						else
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// 단병기
					}
					break;
				case _XGI_SC_WEAPON_LONG :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 장병기
					}
					break;
				case _XGI_SC_WEAPON_SOFT :
				case _XGI_SC_WEAPON_HIDDEN :
				case _XGI_SC_WEAPON_MUSICAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 임시
					}
					break;
				case _XGI_SC_WEAPON_SPECIAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// 깅병기
					}
					break;
				}
			}
			else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				switch(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType)
				{
				case _XGI_SC_WEAPON_SHORT :
					{
						if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;			// 권각
						else if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_AXE)
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_AXE;			// 부법
						else
							m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;	// 단병기
					}
					break;
				case _XGI_SC_WEAPON_LONG :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 장병기
					}
					break;
				case _XGI_SC_WEAPON_SOFT :
				case _XGI_SC_WEAPON_HIDDEN :
				case _XGI_SC_WEAPON_MUSICAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;		// 임시
					}
					break;
				case _XGI_SC_WEAPON_SPECIAL :
					{
						m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;				// 깅병기
					}
					break;
				}
			}
#endif
		}
	}
	else
	{
		short motionindex = GetMotionIndex(m_SelectedSkillID);
		
		int maxactioncount = g_SkillProperty[m_SelectedSkillID]->martialNumber;		
		if( maxactioncount > 0 )
		{
			if(m_SelectedSkillItem->m_cSelectedSkillLevel < 2)		// 0, 1 : 1초식
				m_MartialStep = 0;
			else if(m_SelectedSkillItem->m_cSelectedSkillLevel < 4)	// 2, 3 : 2초식
			{
				if(maxactioncount >=2)
					m_MartialStep = 1;
				else
					m_MartialStep = 0;
			}
			else if(m_SelectedSkillItem->m_cSelectedSkillLevel < 12) // 4-11 : 3초식
			{
				if(maxactioncount >= 3)
					m_MartialStep = 2;
				else if(maxactioncount >=2)
					m_MartialStep = 1;
				else
					m_MartialStep = 0;
			}
		}
		else 
		{
			m_MartialStep = 0;
		}
		
		// 애니매이션 자른 후 
		m_CharacterInfo.animationdescript.attacktype = motionindex;
	}

	m_ModelDescriptor.SetCurMotion( m_CharacterInfo.animationdescript.attacktype);
}

// SetSkillAnimation - 다른 skill 용
void _XLocalUser::SetSkillAnimation(int motionindex)
{
	if(motionindex == 0)
	{
		m_MartialStep = 0;
		m_SkillStarLevel = 0;
		m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;		
		m_ModelDescriptor.SetCurMotion(_XAN_NI_ATTACK_KK, TRUE);

		// 이동 시작 또는 종료 동작교체시는 이동시작시간 초기화 
		m_PrevMoveTime = 0;
	}
	else
	{
		m_CharacterInfo.animationdescript.attacktype = motionindex;
	}
}

void _XLocalUser::ResetSkillAnimation(bool changeskill)
{
	m_CurrentBattleState = _XUSER_STATE_NONE;
	m_bUsingSkill = FALSE;

	ResetSpellAttackTarget();

	g_WorldEffectInstanceManager.DeleteInstance( _XDEF_SKILLID_1076 );
	//g_WorldEffectInstanceManager.DeleteInstance( m_SelectedSkillID + 40000 );
	if( m_ModelDescriptor.m_EffectManager )
	{
		m_ModelDescriptor.m_EffectManager->DeleteInstance(  m_SelectedSkillID + 40000  );
	}
	
	switch(m_CurrentSelectedSkillType)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			m_ChargeStartTime = 0;
			m_ChargeIterateValue = 0;
			m_ChargeSendStartPacket = FALSE;
			m_bStartCharge = FALSE;
			m_bUseMouseChargeSkill = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			m_CastingSpellRepeatTime = 0;
			m_CastingSendPacket = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			m_bChannelingSendStartPacket = FALSE;
			m_ChannelingSendLastPacketTime = 0;
			m_ChannelingSpellRepeatTime = 0;
			m_RegionTargetPosX = 0.0f;
			m_RegionTargetPosZ = 0.0f;
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		{
			m_FinishSpeed = 0;
			m_FinishSendPacket = FALSE;
			m_bSendFinishFirePacket = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			m_RoundingDelayStartTime = 0;
			m_RoundingSendPacket = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_HIDDENMA :
		{
			m_HiddenSendLastPacketTime = 0;
			m_HiddenReceivePacketTime = 0;
			m_HiddenSendStartPacket = FALSE;

			_XWindow_MainMenu* pMainMenu_Window = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);
			if(pMainMenu_Window)
				pMainMenu_Window->m_bDrawMedEffectInfinite = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_SMASHINGSKILL :
		{
			m_SmashingSendPacket = FALSE;
		}
		break;
	case _XSI_USEFULTYPE_ACTIONSKILL :
		{
			m_ActionSendPacket = FALSE;
		}
		break;
	}
	
	if(GetMotionClass() == _XACTION_ATTACK || GetMotionClass() == _XACTION_IDLE)
	{
		if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_FINISHSKILL)
			SetAnimation( _XACTION_IDLE, FALSE );
		else 
		{
			SetAnimation(_XACTION_IDLE);
		}

		if(m_CurrentSelectedSkillType != _XSI_USEFULTYPE_ACTIVESKILL)
		{
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
	}
	m_ModelDescriptor.SetLockEndFrame(FALSE);

	// 들고 있던 무기가 있을 경우 복구해주기
	ReloadWeaponModel();
}

void _XLocalUser::DrawBattle(void)
{
	//2004.06.18->oneway48 insert
	if(m_BattleMode == _XBATTLE_BATTLE)
	{
		
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
		
		gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE );	
		gpDev->SetRenderState( D3DRS_DEPTHBIAS , 0 );
		
		D3DXVECTOR3 rvec;
		D3DXVECTOR3	position = m_Position;
		position.y = m_Position.y + 0.25f;
		D3DXVec3Project(&rvec, &position, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(),
			&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
		
		int X = (int)(rvec.x);
		int Y = (int)(rvec.y);
		
		if(X < 0)
			X = 0;
		if(Y < 0)
			Y = 0;
		
		if( !g_FreeMoveMode )
		{
			if(g_ViewMyBattleGauge)
			{
				m_imageMiniHpBackBorder.Draw( X-54, Y-1 );
				m_MiniHpBorder.MoveWindow( X-45, Y );
				m_MiniHpBorder.Process();
				
				if( !(g_FreeMoveMode && g_InnerUser) )
				{
					m_MiniHpBorder.Draw();
				}
			}
		}
	}

	if(m_SelectedSkillID == 0)
		return;
	
	switch(g_SkillProperty[m_SelectedSkillID]->usefulType)
	{
	case _XSI_USEFULTYPE_PASSIVESKILL :
		break;
	case _XSI_USEFULTYPE_ACTIVESKILL :
		break;
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			// draw charge skill
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
				{
					// draw gauge
					_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
					if(pChargeGauge_Window)
					{
						if(m_ChargeStartTime > 0)
						{
							if(!pChargeGauge_Window->GetShowStatus())
							{
								pChargeGauge_Window->SetGaugeMode(_XGAUGE_CHARGE);
								pChargeGauge_Window->ShowWindow(TRUE);
							}

							FLOAT chargepercentage = 0;
							int casting = 0;
							if(m_SelectedSkillItem->m_cSkillLevel == 0)
								casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
							else
								casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel)
											- _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel-1);
								//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting 
										//	- g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel-1].casting;

							if(casting > 0)
								chargepercentage = ((g_LocalSystemTime - m_ChargeStartTime) / (FLOAT)casting) * 100;
							else
								chargepercentage = 100;

							pChargeGauge_Window->SetParameter(chargepercentage);

							if(m_ChargeIterateValue < m_SelectedSkillItem->m_cSelectedSkillLevel)
							{
								if(chargepercentage >= 100.0f)
								{
									m_ChargeIterateValue++;
									m_ChargeStartTime = g_LocalSystemTime;
									pChargeGauge_Window->ShowWindow(FALSE);
								}
							}
						}
					}
				}
				break;
			default :
				{
					if(GetMotionClass() != _XACTION_DYING )
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if(pChargeGauge_Window)
						{
							if(pChargeGauge_Window->GetShowStatus())
							{
								if( pChargeGauge_Window->GetCurrentGaugeMode() == _XGAUGE_CHARGE )
									pChargeGauge_Window->ShowWindow(FALSE);
							}
						}
					}
				}
				break;
			}
		}
		break;
	case _XSI_USEFULTYPE_PASSIVESPELL :
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_5 :
				DrawBattleCastingSpell();
				break;
			default :
				{
					if(GetMotionClass() != _XACTION_DYING )
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if(pChargeGauge_Window)
						{
							if(pChargeGauge_Window->GetShowStatus())
							{
								if( pChargeGauge_Window->GetCurrentGaugeMode() == _XGAUGE_CHARGE )
									pChargeGauge_Window->ShowWindow(FALSE);
							}
						}
					}
				}
				break;
			}
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			// draw channeling spell - other or all 
/*			if(g_SkillProperty[m_SelectedSkillID]->targetType != _XSI_TARGETTYPE_SELF)
			{
				switch(m_CurrentBattleState)
				{
					case _XUSER_STATE_4 :
					case _XUSER_STATE_5 :
					case _XUSER_STATE_6 :
					{
						// draw gauge
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if(pChargeGauge_Window)
						{
							if(!pChargeGauge_Window->GetShowStatus())
							{
								pChargeGauge_Window->ShowWindow(TRUE);
								pChargeGauge_Window->SetGaugeMode(_XGAUGE_CHARGE);
							}
	
							FLOAT channelpercent = ((g_LocalSystemTime - m_ChannelingReceivePacketTime) / 4000.) * 100.0f;
							pChargeGauge_Window->SetParameter(channelpercent);
	
							if(channelpercent >= 100.0f)
							{
								m_ChannelingReceivePacketTime = g_LocalSystemTime;
								pChargeGauge_Window->ShowWindow(FALSE);
							}
						}
					}
					break;
				default :
					{
						if(GetMotionClass() != _XACTION_DYING)
						{
							_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
							if(pChargeGauge_Window)
							{
								if(pChargeGauge_Window->GetShowStatus())
									pChargeGauge_Window->ShowWindow(FALSE);
							}
						}
					}
					break;
				}
			}*/

		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		break;
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
		{
			if(_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF)
			{
				switch(m_CurrentBattleState)
				{
				case _XUSER_STATE_4 :
					DrawBattleRoundingSpell();
					break;
				default :
					{
						if(GetMotionClass() != _XACTION_DYING)
						{
							_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
							if(pChargeGauge_Window)
							{
								if(pChargeGauge_Window->GetShowStatus())
								{
									if( pChargeGauge_Window->GetCurrentGaugeMode() == _XGAUGE_CHARGE )
										pChargeGauge_Window->ShowWindow(FALSE);
								}
							}
						}
					}
					break;
				}
			}// target type - self
			else
			{
				switch(m_CurrentBattleState)
				{
				case _XUSER_STATE_3 :
				case _XUSER_STATE_4 :
					DrawBattleRoundingSpell();
					break;
				default :
					{
						if(GetMotionClass() != _XACTION_DYING)
						{
							_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
							if(pChargeGauge_Window)
							{
								if(pChargeGauge_Window->GetShowStatus())
								{
									if( pChargeGauge_Window->GetCurrentGaugeMode() == _XGAUGE_CHARGE )
										pChargeGauge_Window->ShowWindow(FALSE);
								}
							}
						}
					}
					break;
				}
			} // target type - other/all
		} // rounding spell
		break;
	}
}

void _XLocalUser::DrawBattleRoundingSpell(void)
{
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	if(pChargeGauge_Window)
	{
		if(m_RoundingDelayStartTime > 0)
		{
			if(!pChargeGauge_Window->GetShowStatus())
			{
				pChargeGauge_Window->SetGaugeMode(_XGAUGE_CHARGE);
				pChargeGauge_Window->ShowWindow(TRUE);
			}
			
			FLOAT delaypercentage = 0.0f;
			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
			if(casting > 0)
				delaypercentage = ((g_LocalSystemTime - m_RoundingDelayStartTime) / (FLOAT)casting) * 100;
			else
				delaypercentage = 100;
			
			pChargeGauge_Window->SetParameter(delaypercentage);
		}
	}
}

void _XLocalUser::DrawBattleCastingSpell(void)
{
	_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
	if(pChargeGauge_Window)
	{
		if(m_CastingSpellRepeatTime > 0)
		{
			if(!pChargeGauge_Window->GetShowStatus())
			{
				pChargeGauge_Window->SetGaugeMode(_XGAUGE_CHARGE);
				pChargeGauge_Window->ShowWindow(TRUE);
			}

			FLOAT delaypercentage = 0.0f;
			int casting = _XSkillItem::GetCasting(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSelectedSkillLevel].casting;
			if(casting > 0)
				delaypercentage = ((g_LocalSystemTime - m_CastingSpellRepeatTime) / (FLOAT)casting) * 100;
			else 
				delaypercentage = 100;

			pChargeGauge_Window->SetParameter(delaypercentage);
		}
	}
}

void _XLocalUser::ProcessCommonAction(void)
{
	short stancemotion = GetStanceMotion(m_ActiveSlotID);	

	if(GetMotionClass() == _XACTION_IDLE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if((m_ModelDescriptor.GetCurMotion() == stancemotion) && (stancemotion != 0))
			{
				SetAnimation(_XACTION_IDLE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}

		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_DEFENCE)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
		{
			if(g_LodTerrain.m_3PCamera.m_ImpactValue == 0.0f)
				g_LodTerrain.m_3PCamera.m_ImpactValue = _XDEF_IMPACTPOWER;
			
			int rand = _XRandom() % 3;
			_XPlaySoundEffect(g_DefenceEffectSoundIndex[m_CharacterInfo.gender][rand], m_Position);
		}
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			SetAnimation(_XACTION_IDLE);
//			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}

		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_BLOW)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
		{
			if(g_LodTerrain.m_3PCamera.m_ImpactValue == 0.0f)
				g_LodTerrain.m_3PCamera.m_ImpactValue = _XDEF_IMPACTPOWER * 1.4f;
			
			int rand = _XRandom() % 3;
			_XPlaySoundEffect(g_BlowEffectSoundIndex[m_CharacterInfo.gender][rand], m_Position);
		}
		else if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			m_SpecialActionTimer = g_LocalSystemTime;

			if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
			{
				if(m_AttackTarget && m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
//					_XDWINPRINT("[ProcessCommonAction] Set Attack");
					m_CurrentBattleState = _XUSER_STATE_1;
//					SetAnimation(_XACTION_ATTACK);
				}
				else
				{
					m_CurrentBattleState = _XUSER_STATE_LAST;
//					SetAnimation(_XACTION_IDLE);
//					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}
			}
			else
			{
				SetAnimation(_XACTION_IDLE);
//				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}

		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_DYING)
	{
		/*
		if(m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_NONE			||
			m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_SHORTWEAPON	||
			m_ModelDescriptor.GetCurMotion() == _XAN_DYINGREADY_LONGWEAPON		)
		{
			
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_FIRSTFRAME)
			{
				// 뒤로 밀어주기 시작
				if( m_AttackTarget )
				{
					_XMob* pMob = (_XMob*)m_AttackTarget;
					D3DXVECTOR3 viewdirection;
					viewdirection.x = pMob->m_Position.x - m_Position.x;
					viewdirection.y = 1.0f;
					viewdirection.z = pMob->m_Position.z - m_Position.z;
					
					m_TargetPosition.x = m_Position.x + viewdirection.x*(-0.5f);
					m_TargetPosition.z = m_Position.z + viewdirection.z*(-0.5f);
					
					m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position);
					m_LastRotateAngle = m_RotateAngle;
					UpdatePosition();
					
					ResetViewTarget();
					ResetAttackTarget();
				}
			}
			if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME)
			{
				// 멈춘다
				m_TargetPosition = m_Position;
				UpdatePosition();
			}			
		}
		*/

		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_DIE)
	{
		ResetActiveSkillAnimationControl();
	}
	else if(GetMotionClass() == _XACTION_REBIRTH)
	{
#ifdef _XDEF_USESCREEENMOTIONBLUR 
		if(g_ScrrenMotionBlurMode)
		{
			g_ScreenMotionBlurEffectManager.AddBlurAlpha(-0.002f);
			
			if(g_ScreenMotionBlurEffectManager.GetBlurAlpha() <= g_ScreenMotionBlurEffectManager.GetMinBlurAlpha())
				g_ScrrenMotionBlurMode = FALSE;
		}
#endif

		ResetActiveSkillAnimationControl();
	}
}

void _XLocalUser::ProcessNormalAction(void)
{
	if(m_CharacterInfo.animationdescript.motionclass == _XACTION_ATTACK)
	{
		if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
		{
			if(!m_AttackTarget)
			{
				SetAnimation(_XACTION_IDLE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				m_AttackTargetObjectType = _XPICKINGTARGET_NONE;
			}
		}
	}
}

// CheckDistanceToTarget() : target이 공격거리에 들어와있으면 TRUE, 아니면 FALSE
BOOL _XLocalUser::CheckDistanceToTarget(D3DXVECTOR3 targetposition, short skillid, FLOAT& range, unsigned short mobType, bool bAttackAfterMove)
{
	FLOAT dx = m_Position.x - targetposition.x;
	FLOAT dz = m_Position.z - targetposition.z;
	FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

	FLOAT targetdistance;
	FLOAT smallrange = 0.0f;

	if(mobType == 0)
	{
		// PC
		targetdistance = 0.7f;
		
		if(skillid == 0)
			range = m_AttackRange + targetdistance;
		else
			range = m_AttackRange + _XSkillItem::GetDistance(skillid); //g_SkillProperty[m_SelectedSkillID]->distance;
		
		if(bAttackAfterMove)
		{
			smallrange = range;
		}
		else
		{
			smallrange = range * 1.2f;
		}
	} // pc
	else
	{
		if( g_MobAttribTable[mobType].scripttype > 80 && g_MobAttribTable[mobType].scripttype < 85)// Key LPS 및 LPS
		{
			if( g_MobAttribTable[mobType].scripttype != 84 )
			{
				range = smallrange = 0.7f;
			}
			else // HP LPS일때는 몬스터 공격거리를 적용한다.
			{
				targetdistance = g_MobAttribTable[mobType].collideradius;

				if(skillid == 0)
					range = m_AttackRange + targetdistance;
				else
					range = m_AttackRange + _XSkillItem::GetDistance(skillid);//g_SkillProperty[m_SelectedSkillID]->distance;
				
				if(bAttackAfterMove)
				{
					smallrange = range;
				}
				else
				{
					smallrange = range * 1.2f;
				}
			}
		}
		else if( g_MobAttribTable[mobType].scripttype == 85 || // optional lps (trap)
				 g_MobAttribTable[mobType].scripttype == 90 )// 공격가능한 오브젝트 
		{
			range = smallrange = 2.5f;
		}
		else //일반몬스터
		{
			if(mobType > 0)
				targetdistance = g_MobAttribTable[mobType].collideradius;
			else
				targetdistance = 0.7f;		// PC

			if(skillid == 0)
				range = m_AttackRange + targetdistance;
			else
				range = m_AttackRange + _XSkillItem::GetDistance(skillid); //g_SkillProperty[m_SelectedSkillID]->distance;

			if(bAttackAfterMove)
			{
				smallrange = range;
			}
			else
			{
				smallrange = range * 1.2f;
			}
		}
	} // monster

	if(distance <= smallrange)
	{
//		_XDWINPRINT("[CheckDistanceToTarget] return TRUE - distance : %f  range :%f  smallrange:%f", distance , range, smallrange);
		m_TargetPosition = m_Position;
		return TRUE;
	}
	else
	{
//		_XDWINPRINT("[CheckDistanceToTarget] return FALSE - distance : %f  range :%f  smallrange:%f", distance , range, smallrange);
		return FALSE;
	}
}

// SetTargetPosition() : m_TargetPosition 지정, 이동할 지점을 지정해주는 함수
void _XLocalUser::SetTargetPosition(D3DXVECTOR3 targetposition, FLOAT range)
{
	D3DXVECTOR3 directionvector = m_Position - targetposition;
	D3DXVec3Normalize(&directionvector, &directionvector);
	
	D3DXVECTOR3 position = targetposition + (directionvector*(range*0.7f));
	position.y = targetposition.y;
	SetTargetPositionAngle(position);
}

void _XLocalUser::CheckDelayTime(void)
{
	if(m_bCheckDelayTime)
	{
		if(m_ChangeDelayStartTime == 0)
			m_bCheckDelayTime = FALSE;

		int delaytime = 0;
//Author : 양희왕
#ifdef _XDEF_SKILLDEALY_MODIFY
		if(m_NextSkillID > 0)
		{
			// 예약된 skill 있음
			if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
			{
				if(g_SkillProperty[m_NextSkillID]->usefulType != _XSI_USEFULTYPE_ACTIVESKILL)
				{
#ifdef _XDEF_BLOCK_MOVINGATTACK
					// 적용 (Other - All)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// 상태효과 - dealy time 증가
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Other[%d] - All[없음] delay[%d]", m_FinallySelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
#else
					// delay time 적용하지 않음 (Active - Other)
					m_ChangeDelayStartTime = 0;
					m_bCheckDelayTime = FALSE;
					m_bAddDelayTime = FALSE;

					_XDWINPRINT("[CheckDelayTime] Active[%d] - Other[%d] ", m_SelectedSkillID, m_NextSkillID);
#endif
				}
				else
				{
					// 적용 (Active - Active)
					if(m_SelectedSkillID > 0)
						delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;
					else
						delaytime = 500;

					if(m_Lock.adddelaytime == 1)
					{
						// 상태효과 - dealy time 증가
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[%d] delay[%d]", m_SelectedSkillID, m_NextSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
			}
			else
			{
				// 적용 (Other - All)
				if(m_SelectedSkillID > 0)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;
				else
					delaytime = 500;

				if(m_Lock.adddelaytime == 1)
				{
					// 상태효과 - dealy time 증가
					delaytime *= 2;
				}
				else if(m_bAddDelayTime)
				{
					delaytime += 1000;
				}
				
				if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
				{
					if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
					{
						_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[%d] delay[%d]", m_SelectedSkillID, m_NextSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
						m_ChangeDelayStartTime = 0;
						m_bCheckDelayTime = FALSE;
						m_bAddDelayTime = FALSE;
					}
				}
			}
		} // if(m_NextSkillID > 0)
		else
		{
			// 예약된 스킬 없음 (Active - Active 에만 적용)
			if(m_FinallySelectedSkillID > 0)
			{
				if(g_SkillProperty[m_FinallySelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
				{
					// 적용 (Active - Active)
					delaytime = _XSkillItem::GetDelay(m_FinallySelectedSkillID, m_FinallySelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// 상태효과 - dealy time 증가
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[없음] delay[%d]", m_FinallySelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_FinallySelectedSkillID = 0;
							m_FinallySelectedSkillItem = NULL;
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
				else
				{
					// 적용 (Other - All)
					delaytime = _XSkillItem::GetDelay(m_FinallySelectedSkillID, m_FinallySelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// 상태효과 - dealy time 증가
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Other[%d] - All[없음] delay[%d]", m_FinallySelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_FinallySelectedSkillID = 0;
							m_FinallySelectedSkillItem = NULL;
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
			}
			else
			{
				// 적용 (Active - Active)
				delaytime = 500;

				if(m_bAddDelayTime)
				{
					delaytime += 1000;
				}
				
				if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
				{
					if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
					{
						_XDWINPRINT("[CheckDelayTime] Axtive[%d] - Active[없음] delay[%d]", m_SelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
						m_ChangeDelayStartTime = 0;
						m_bCheckDelayTime = FALSE;
						m_bAddDelayTime = FALSE;
					}
				}
			}
		}
#else
//Last Updated : 07/03/23

		if(m_NextSkillID > 0)
		{
			// 예약된 skill 있음
			if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
			{
				if(g_SkillProperty[m_NextSkillID]->usefulType != _XSI_USEFULTYPE_ACTIVESKILL)
				{
#ifdef _XDEF_BLOCK_MOVINGATTACK //Author : 양희왕
					// 적용 (Other - All)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// 상태효과 - dealy time 증가
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Other[%d] - All[없음] delay[%d]", m_FinallySelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
#else //Last Updated : 07/04/19
					// delay time 적용하지 않음 (Active - Other)
					m_ChangeDelayStartTime = 0;
					m_bCheckDelayTime = FALSE;
					m_bAddDelayTime = FALSE;

					_XDWINPRINT("[CheckDelayTime] Active[%d] - Other[%d] ", m_SelectedSkillID, m_NextSkillID);
#endif
				}
				else
				{
					// 적용 (Active - Active)
					if(m_SelectedSkillID > 0)
						delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;
					else
						delaytime = 500;

					if(m_Lock.adddelaytime == 1)
					{
						// 상태효과 - dealy time 증가
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[%d] delay[%d]", m_SelectedSkillID, m_NextSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
			}
			else
			{
				// 적용 (Other - All)
				if(m_SelectedSkillID > 0)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;
				else
					delaytime = 500;

				if(m_Lock.adddelaytime == 1)
				{
					// 상태효과 - dealy time 증가
					delaytime *= 2;
				}
				else if(m_bAddDelayTime)
				{
					delaytime += 1000;
				}
				
				if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
				{
					if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
					{
						_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[%d] delay[%d]", m_SelectedSkillID, m_NextSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
						m_ChangeDelayStartTime = 0;
						m_bCheckDelayTime = FALSE;
						m_bAddDelayTime = FALSE;
					}
				}
			}
		} // if(m_NextSkillID > 0)
		else
		{
			// 예약된 스킬 없음 (Active - Active 에만 적용)
			if(m_SelectedSkillID > 0)
			{
				if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
				{
					// 적용 (Active - Active)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// 상태효과 - dealy time 증가
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Active[%d] - Active[없음] delay[%d]", m_SelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
				else
				{
					// 적용 (Other - All)
					delaytime = _XSkillItem::GetDelay(m_SelectedSkillID, m_SelectedSkillItem->m_cSelectedSkillLevel);//g_SkillProperty[m_SelectedSkillID]->detailInfo[m_SelectedSkillItem->m_cSkillLevel].delay;

					if(m_Lock.adddelaytime == 1)
					{
						// 상태효과 - dealy time 증가
						delaytime *= 2;
					}
					else if(m_bAddDelayTime)
					{
						delaytime += 1000;
					}
					
					if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
					{
						if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
						{
							_XDWINPRINT("[CheckDelayTime] Other[%d] - All[없음] delay[%d]", m_SelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
							m_ChangeDelayStartTime = 0;
							m_bCheckDelayTime = FALSE;
							m_bAddDelayTime = FALSE;
						}
					}
				}
			}
			else
			{
				// 적용 (Active - Active)
				delaytime = 500;

				if(m_bAddDelayTime)
				{
					delaytime += 1000;
				}
				
				if((m_ChangeDelayStartTime > 0 && g_LocalSystemTime >= m_ChangeDelayStartTime))
				{
					if(g_LocalSystemTime - m_ChangeDelayStartTime > delaytime)
					{
						_XDWINPRINT("[CheckDelayTime] Axtive[%d] - Active[없음] delay[%d]", m_SelectedSkillID, g_LocalSystemTime - m_ChangeDelayStartTime);
						m_ChangeDelayStartTime = 0;
						m_bCheckDelayTime = FALSE;
						m_bAddDelayTime = FALSE;
					}
				}
			}
		}
#endif
		
	} // if(m_bCheckDelayTime)
}

void _XLocalUser::ApplyNextSkill(void)
{
	if(!m_bUsingSkill && !m_bCheckDelayTime)
	{
		if(m_NextSkillID > 0)
		{
			XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

			switch(g_SkillProperty[m_NextSkillID]->usefulType)
			{
			case _XSI_USEFULTYPE_ACTIVESKILL :
				{
					pProc_Maingame->ChangeToActive(m_NextSkillID);
				}
				break;
			case _XSI_USEFULTYPE_CHANNELINGSPELL :
				{
					pProc_Maingame->ChangeToChanneling(m_NextSkillID, TRUE);
				}
				break;
			case _XSI_USEFULTYPE_CHARGESKILL :
				{
					pProc_Maingame->ChangeToOther(m_NextSkillID, TRUE);
					m_bStartCharge = TRUE;
				}
				break;
			case _XSI_USEFULTYPE_CASTINGSPELL :
			case _XSI_USEFULTYPE_FINISHSKILL :
			case _XSI_USEFULTYPE_ROUNDINGSPELL :
				{	
					pProc_Maingame->ChangeToOther(m_NextSkillID, TRUE);
				}
				break;
			case _XSI_USEFULTYPE_SMASHINGSKILL :
				{
					pProc_Maingame->ChangeToOther(m_NextSkillID, TRUE);
					m_bRestartActive = FALSE;
				}
				break;
			case _XSI_USEFULTYPE_ACTIONSKILL :
				{
					pProc_Maingame->ChangeToOther(m_NextSkillID, TRUE);
				}
				break;
			case _XSI_USEFULTYPE_HIDDENMA :
				{
					pProc_Maingame->ChangeToHiddenMA(m_NextSkillID, TRUE);
				}
				break;
			}

			m_NextSkillID = 0;
		}
		else
		{
			if(m_bRestartActive)
			{
				if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
				{
					m_CurrentBattleState = _XUSER_STATE_1;
					m_bRestartActive = FALSE;
				}
				else if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_SMASHINGSKILL)
				{
					XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
					pProc_Maingame->ChangeToActive(m_ActiveSlotID);
					m_bRestartActive = FALSE;
				}
			}
		}
	}
}

/*void _XLocalUser::SetActiveSlotSkill(void)
{
	int skillid = m_ActiveSlotID;
	if(((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.CheckDelayTime(skillid))
	{
		m_CurrentSelectedSkillType = g_SkillProperty[skillid]->usefulType;
		m_SelectedSkillID = skillid;
		m_SelectedSkillItem = GetSkillItem(skillid);
		
//		SetSkillAnimation();
		UpdateLocalUserAttackLevel();
		g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
	}
}*/

void _XLocalUser::SetViewTarget(_XItem* target, _XPICKINGTARGETTYPE targettype, BOOL bsetpkready)
{
	if( m_ViewTarget )
	{
		switch(m_ViewTargetType)
		{
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		}
	}
	
	m_ViewTarget = target;
	m_ViewTargetType = targettype;
	// 이동누적시간 초기화
	m_PrevMoveTime			 = 0;

	// pk ready 상태 초기화
	if(!bsetpkready)
	{
		if(m_UserState == _XDEF_USERSTATE_PK_READY)
		{
			ResetPKReadyMode();
//			m_UserState = _XDEF_USERSTATE_NORMAL;

			if(targettype == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)target;
				if(pUser)
				{
					pUser->m_bEnemy = FALSE;
				}
			}
		}
	}

	// Functional Object를 클릭했으면 공격가능하지를 판별한다.
	if( m_ViewTargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
	{
		if( m_ViewTarget )
		{							
			_XFunctionalObject* pFunctionalObject = (_XFunctionalObject*)m_ViewTarget;
			if( pFunctionalObject )
			{
				pFunctionalObject->SetAttackableFunctionalObject();
			}
		}
	}
	else	// Functional Object는 이펙트를 추가하지 않는다.
	{
		BOOL bAddEffect = FALSE;
		if(m_ViewTarget)
		{
			if(m_AttackTarget)
			{
				if(m_ViewTarget != m_AttackTarget)
					bAddEffect = TRUE;
			}
			else
			{
				bAddEffect = TRUE;
			}
		}
		
		if(bAddEffect)
		{
			switch(targettype)
			{
			case _XPICKINGTARGET_PC :
				{
					if(((_XUser*)target)->m_ModelDescriptor.m_EffectManager)
						((_XUser*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XUser*)target)->m_ModelDescriptor.m_Position, NULL, TRUE );
				}
				break;
			case _XPICKINGTARGET_MOB :
				{
					if(((_XMob*)target)->m_ModelDescriptor.m_EffectManager)
						((_XMob*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XMob*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
					
					_XPlayInterfaceSound(ID_SR_INTERFACE_TARGETING00_WAV);
					g_NetworkKernel.SendPacket(MSG_NO_REQ_SPELLEFFECT, ((_XMob*)target)->m_MobID);
					
					SetMonsterWarningMessage( ((_XMob*)target)->m_MobType );
				}
				break;
			case _XPICKINGTARGET_NPC :
				{
					if(((_XNPCObject*)target)->m_ModelDescriptor.m_EffectManager)
						((_XNPCObject*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&((_XNPCObject*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
				}
				break;
			case _XPICKINGTARGET_DUMMY :
				{
					if(((_XDummy*)target)->m_ModelDescriptor.m_EffectManager)
					{
						((_XDummy*)target)->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
							&((_XDummy*)target)->m_ModelDescriptor.m_Position, NULL, TRUE);
					}
				}
				break;
			case _XPICKINGTARGET_SELF :
				{
					if(m_ModelDescriptor.m_EffectManager)
						m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_INTERFACEID_SELECT,
						&m_ModelDescriptor.m_Position, NULL, TRUE);
				}
				break;
			}
		}
	}

	
	_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
	if(pMonsterStatus_Window)
	{
		pMonsterStatus_Window->ShowWindow( ( target ) ? TRUE : FALSE );
	}	
}

void _XLocalUser::ResetViewTarget(void)
{
	if(m_ViewTarget)
	{
		switch(m_ViewTargetType)
		{
		case _XPICKINGTARGET_PC :
			{
				if(((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XUser*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_MOB :
			{
				if(((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XMob*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_NPC :
			{
				if(((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
					((_XNPCObject*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_DUMMY :
			{
				if(((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager)
				{
					((_XDummy*)m_ViewTarget)->m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
				}
			}
			break;
		case _XPICKINGTARGET_SELF :
			{
				if(m_ModelDescriptor.m_EffectManager)
					m_ModelDescriptor.m_EffectManager->DeleteInstance(_XDEF_INTERFACEID_SELECT);
			}
			break;
		case _XPICKINGTARGET_FUNCTIONALOBJECT :
			{
				_XWindow_MatchNoticeLarge* pMatchNoticeLarge_Window = (_XWindow_MatchNoticeLarge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHNOTICELARGE);
				if(pMatchNoticeLarge_Window)
				{
					if(pMatchNoticeLarge_Window->GetShowStatus())
						pMatchNoticeLarge_Window->ShowWindow(FALSE);
				}
			}
			break;
		}

		// pk ready 상태 초기화
		if(m_UserState == _XDEF_USERSTATE_PK_READY)
		{
			ResetPKReadyMode();
//			m_UserState = _XDEF_USERSTATE_NORMAL;
			
			if(m_ViewTargetType == _XPICKINGTARGET_PC)
			{
				_XUser* pUser = (_XUser*)m_ViewTarget;
				if(pUser)
				{
					pUser->m_bEnemy = FALSE;
				}
			}
		}

		m_ViewTarget = NULL;
		m_ViewTargetType = _XPICKINGTARGET_NONE;
		
		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
		if(pMonsterStatus_Window)
		{
			if(!m_AttackTarget)
			{
				if(pMonsterStatus_Window->GetShowStatus())
					pMonsterStatus_Window->ShowWindow( FALSE );
			}
		}
	}
}

void _XLocalUser::SetSpellAttackTarget(_XItem* target, _XPICKINGTARGETTYPE targettype)
{
	m_SpellAttackTarget = target;
	m_SpellAttackTargetType = targettype;
}

void _XLocalUser::ResetSpellAttackTarget(void)
{
	m_SpellAttackTarget = NULL;
	m_SpellAttackTargetType = _XPICKINGTARGET_NONE;
}

void _XLocalUser::GetItemTileIndex(FLOAT x, FLOAT z, _XTILE_ITEM& itemtile)
{
	int ix = (int)x + (gnWidth / 2);
	int iz = (int)z + (gnWidth / 2);
	
	if(ix > gnWidth)
		ix = gnWidth - 1;
	else if(ix < 0)
		ix = 1;
	
	if(iz > gnWidth)
		iz = gnWidth - 1;
	else if(iz < 0)
		iz = 1;

	int NumWidthTile = gnWidth / 16;
	int NumHeightTile = gnHeight / 16;

	if(ix < 0 || iz < 0 || (ix/16) >= (NumWidthTile*2) || (iz/16) >= (NumHeightTile*2))
		return;
	
	itemtile.sX = (short)(ix / 16);
	itemtile.sZ = (short)(iz / 16);
}

void _XLocalUser::SetTargetPositionAngle(D3DXVECTOR3 targetposition)
{
	if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE && GetMotionClass() != _XACTION_REBIRTH )
	{
		m_TargetPosition = m_FinalTargetPosition = targetposition;

		FLOAT dx = m_Position.x - m_FinalTargetPosition.x;
		FLOAT dz = m_Position.z - m_FinalTargetPosition.z;
		m_LeftFinalTargetLength = _XFC_sqrt(dx*dx + dz*dz);

		//m_PrevMoveTime = 0;

		//FLOAT dx = m_Position.x - m_TargetPosition.x;
		//FLOAT dz = m_Position.z - m_TargetPosition.z;		
		//if( _XFC_sqrt(dx*dx + dz*dz) > 4.0f ) // 8.0 이상 피킹된경우만 길찾기
		{
			// 경공이 아닐때만 패스 파인딩
			if( !( g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL)||
				!( _XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_SELF		   )||
				!( g_SkillProperty[m_SelectedSkillID]->skillSType == _XSI_STYPE_03			   )||
				!gpInput->CheckKeyPress(DIK_LMENU) )
			{
				CreateMovePath();
				
				if(m_PathNodeCount > 0)
				{	
					if( m_PathNodeCount == 1 )
					{
						m_TargetPosition.x = m_PathPointArray[0].pos.x;
						m_TargetPosition.z = m_PathPointArray[0].pos.y;
					}
					else
					{
						m_TargetPosition.x = ( m_PathPointArray[0].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
						m_TargetPosition.z = ( m_PathPointArray[0].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;				
						
						FLOAT dx = m_Position.x - m_TargetPosition.x;
						FLOAT dz = m_Position.z - m_TargetPosition.z;
						if( _XFC_sqrt(dx*dx + dz*dz) < 3.0f ) // 다음 포인트가 근처 타일의 위치라면 스킵
						{
							m_TargetPosition.x = ( m_PathPointArray[1].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
							m_TargetPosition.z = ( m_PathPointArray[1].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
							m_CurrentNavigatePathNodeIndex = 1;
						}
					}
				}
				else
				{
					if(m_PathNodeCount <= 0) // 패스 노드가 0이면 직통 이하면 패스파인딩 실패
					{
						m_PathNodeCount = 0; // Path find 사용 않함.
						m_CurrentNavigatePathNodeIndex = 0;
					}
				}
			}
		}		
		/*else
		{
			m_PathNodeCount = 0; // Path find 사용 않함.
			m_CurrentNavigatePathNodeIndex = 0;
		}*/

		m_LastRotateAngle = _XMeshMath_GetRotateRadian(m_Position, m_TargetPosition);
		
		FLOAT elapsedangle_1 = fabs(m_LastRotateAngle - m_RotateAngle);
		FLOAT elapsedangle_2 = (_X_PI - fabs(m_LastRotateAngle)) + (_X_PI - fabs(m_RotateAngle));
		
		if(elapsedangle_1 <= elapsedangle_2)
		{
			if(m_LastRotateAngle >= m_RotateAngle)
				m_RotateDirection = _XROTATE_CLOCKWISE;
			else
				m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
		}
		else
		{
			if(m_LastRotateAngle >= m_RotateAngle)
				m_RotateDirection = _XROTATE_COUNTCLOCKWISE;
			else
				m_RotateDirection = _XROTATE_CLOCKWISE;
		}
	}
}

void _XLocalUser::ProcessRescueUser(void)
{
	g_NetworkKernel.SendPacket(MSG_NO_CHARAC_REBIRTH_TARGET, 0);
	SetAnimation(_XACTION_RESCUE);
	ResetAttackTarget();
}

void _XLocalUser::CreateMovePath( void )
{
	int	xcoord = _XFC_FloatToIntRet( ( m_Position.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	int zcoord = _XFC_FloatToIntRet( ( m_Position.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );		
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;	
	
	int startxpoint = xcoord;
	int startzpoint = zcoord;
		
	xcoord = _XFC_FloatToIntRet( ( m_FinalTargetPosition.x + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );
	zcoord = _XFC_FloatToIntRet( ( m_FinalTargetPosition.z + g_LodTerrain.m_HalfWorldSize ) / (g_LodTerrain.m_GridRealSize/2.0f) );	
	if( xcoord < 0 ) xcoord = 0;
	if( zcoord < 0 ) zcoord = 0;
	if( xcoord >= (g_LodTerrain.m_TileCount<<4) ) xcoord = (g_LodTerrain.m_TileCount<<4) - 1;
	if( zcoord >= (g_LodTerrain.m_TileCount<<4) ) zcoord = (g_LodTerrain.m_TileCount<<4) - 1;	
	
	int endxpoint = xcoord;
	int endzpoint = zcoord;
	
	if( !g_LodTerrain.GetPathFindAreaInfo( startzpoint, startxpoint ) && 
		!g_LodTerrain.GetPathFindAreaInfo( endzpoint, endxpoint ) ) // 타게팅된 영역이 갈수 있는 영역일때만 검사.
	{	
		if( !g_LodTerrain.CheckDirectPath( startxpoint, startzpoint, xcoord, zcoord ) ) // 일직선 패스가 가능하면 길찾기 않함.
		{
			m_PathNodeCount = g_PathFinder.FindPath( D3DXVECTOR2(startxpoint,startzpoint), D3DXVECTOR2(endxpoint,endzpoint), -1.0f, m_PathPointArray );
			
			if( m_PathNodeCount > 0 )
			{
				m_PathPointArray[m_PathNodeCount-1].pos.x = m_FinalTargetPosition.x;
				m_PathPointArray[m_PathNodeCount-1].pos.y = m_FinalTargetPosition.z;
				g_LodTerrain.m_3PCamera.m_AutoBackTrace = TRUE;
			}		
			else
			{
				m_PathNodeCount = 0;
			}
		}
		else
		{
			m_PathNodeCount = 0;
		}
	}
	else
	{
		m_PathNodeCount = 0;
	}

	m_CurrentNavigatePathNodeIndex = 0;
}


#ifdef _XDWDEBUG
void _XLocalUser::DrawMovePath( void )
{
	if( m_PathNodeCount <= 0 ) return;
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetTexture(0, NULL); 
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	gpDev->SetFVF( _XDEF_FVF_AXIS_VERTEX );
	gpDev->SetTransform( D3DTS_WORLD, &g_LodTerrain.m_TerrainMatrix );
	
	AXIS_VERTEX	line[2];
	line[0].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	line[1].color = D3DCOLOR_ARGB( 255, 255, 255, 0 );
	D3DXVECTOR3 curpos = m_Position;
	curpos.y += 0.3f;
	
	for( int i = 0; i < m_PathNodeCount -1; ++i )
	{						
		line[0].v = curpos;
		
		line[1].v.x = ( m_PathPointArray[i].pos.x + g_PathFinder.m_CollideArrayStartPosition.x) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
		line[1].v.z = ( m_PathPointArray[i].pos.y + g_PathFinder.m_CollideArrayStartPosition.y) * (g_LodTerrain.m_GridRealSize/2.0f) - g_LodTerrain.m_HalfWorldSize;
		line[1].v.y = g_LodTerrain.GetTerrainHeight( line[1].v ) + 0.3f;
		gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));
		
		curpos = line[1].v;
	}
	
	
	line[0].v = curpos;
	line[1].v = m_FinalTargetPosition;
	line[1].v.y = g_LodTerrain.GetTerrainHeight( line[1].v ) + 0.3f;
	
	gpDev->DrawPrimitiveUP( D3DPT_LINELIST, 1, line, sizeof(AXIS_VERTEX));	
}
#endif

void _XLocalUser::ProcessMobDie(void)
{
	_XMob* pAttackTarget = NULL;
	_XMob* pMob = NULL;

	for(int i = 0 ; i < 3 ; ++i)
	{
		if(m_AttackResult.display[i])
		{
			if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
			{
				if(m_AttackTarget && m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					pAttackTarget = reinterpret_cast<_XMob*>(m_AttackTarget);
				}
				if(pAttackTarget)
				{
					if(pAttackTarget->m_MobID == m_AttackResult.usDefenderID)
						pMob = pAttackTarget;
					else
						pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);
				}
				else
				{
					pMob = (_XMob*)g_Moblist.FindMob(m_AttackResult.usDefenderID);
				}

				if(pMob)
				{
					if( g_ZeroLevelMode )
						pMob->SetCharacterMobAnimation(_XMOBACTION_DIE);
					else
						pMob->SetAnimation(_XMOBACTION_DIE);
					pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);
					
					if(m_AttackResult.sDamage[i] == -1)
					{
//						m_CharacterLog.isKillClient = TRUE;
//						m_CharacterLog.killTimeClient = g_ServerTimeCode;
//						m_CharacterLog.killTargetTypeClient = pMob->m_MobType;


						_XDWINPRINT("[LOCAL USER/ProcessMobDie] Mob[%d] kill", pMob->m_MobType);
					}
					m_AttackResult.cBattleResult = 0;
					m_AttackResult.display[i] = FALSE;
					break;
				}

				if(i == 2)
				{
					// 마지막 타에 죽어야할 몬스터를 못찾으면
					if(m_AttackResult.cBattleResult == en_battle_result_binsa_or_die || m_AttackResult.cBattleResult == en_battle_result_honsu_or_dead)
						m_AttackResult.cBattleResult = 0;
				}
			}
			m_AttackResult.display[i] = FALSE;
		}
	}
}

/*void _XLocalUser::SetActiveSkill(void)
{
	BOOL	bIdle = FALSE;

	m_CurrentBattleState = _XUSER_STATE_NONE;
	m_CurrentSelectedSkillType = _XSI_USEFULTYPE_ACTIVESKILL;

	short prevskillid = m_SelectedSkillID;
	m_SelectedSkillID = m_NextSkillID;
	short prevactiveslot = m_ActiveSlotID;
	m_ActiveSlotID = m_NextSkillID;

	if(m_SelectedSkillID == 0)
		m_SelectedSkillItem = NULL;
	else
		m_SelectedSkillItem = GetSkillItem(m_SelectedSkillID);

	if(m_ActiveSlotID != prevactiveslot)
	{
		if(m_CharacterInfo.animationdescript.motionclass == _XACTION_IDLE)
		{
			SetBattleMode(_XBATTLE_BATTLE);
			if(g_pQuickSlot_Window)
				g_pQuickSlot_Window->ChangePeaceBattleMode();

			bIdle = TRUE;
		}
	}

	if(m_CharacterInfo.animationdescript.motionclass == _XACTION_ATTACK)
		SetSkillAnimation();

	UpdateLocalUserAttackLevel();

	if(prevskillid != m_SelectedSkillID)
		g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);

	if((m_NextSkillID!= prevskillid && g_SkillProperty[prevskillid]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL) || prevskillid == 0)
	{
		ResetAttackTarget();
		if(!bIdle)
			SetAnimation(_XACTION_IDLE);
	}

	VIEWBATTLELOG
		_XDWINPRINT("Select Active skill [%d]", m_SelectedSkillID);
}*/

void _XLocalUser::ResetActiveSkillAnimationControl(void)
{
	if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_ACTIVESKILL || m_SelectedSkillID == 0)
	{
		m_CurrentAttackSequence = _XDEF_ATK_SEQ_NONE;
		
		// 애니매이션 자른 후
		if(m_CharacterInfo.gender == _XGENDER_MAN)
			m_CharacterInfo.animationdescript.attacktype = g_SkillProperty[m_SelectedSkillID]->motion_male;
		else
			m_CharacterInfo.animationdescript.attacktype = g_SkillProperty[m_SelectedSkillID]->motion_female;
	}
	m_bCheckFirstFrame = FALSE;
}

BOOL _XLocalUser::IsTroubleStateNormal(void)
{
	DWORD result;
	result = m_UserTroubleState ^ _XDEF_USER_TROUBLESTATE_NORMAL;

	if(result == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL _XLocalUser::IsTroubleStateFury(void)
{
	DWORD result;
	result = m_UserTroubleState || _XDEF_USER_TROUBLESTATE_FURY;

	if(result == 0)
		return TRUE;
	else 
		return FALSE;
}

BOOL _XLocalUser::IsTroubleStateConvergence(void)
{
	return FALSE;
}

BOOL _XLocalUser::CanNotMove(void)
{
	switch(m_CurrentSelectedSkillType)
	{
	case _XSI_USEFULTYPE_CHARGESKILL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
			case _XUSER_STATE_7 :
			case _XUSER_STATE_8 :
					return TRUE;
			}
		}
		break;
	case _XSI_USEFULTYPE_CHANNELINGSPELL :
		{
			if(m_SelectedSkillID > 0)
			{
				if(_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/ == _XSI_TARGETTYPE_POINT)
				{
					switch(m_CurrentBattleState)
					{
					case _XUSER_STATE_2 :
					case _XUSER_STATE_3 :
					case _XUSER_STATE_4 :
					case _XUSER_STATE_5 :
					case _XUSER_STATE_6 :
					case _XUSER_STATE_7 :
					case _XUSER_STATE_8 :
					case _XUSER_STATE_9 :
							return TRUE;
					}
				}
				else
				{
					switch(m_CurrentBattleState)
					{
					case _XUSER_STATE_2 :
					case _XUSER_STATE_3 :
					case _XUSER_STATE_4 :
					case _XUSER_STATE_5 :
					case _XUSER_STATE_6 :
					case _XUSER_STATE_7 :
						return TRUE;
					}
				}
			}
		}
		break;
	case _XSI_USEFULTYPE_SMASHINGSKILL :
	case _XSI_USEFULTYPE_ROUNDINGSPELL :
	case _XSI_USEFULTYPE_ACTIONSKILL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
					return TRUE;
			}
		}
		break;
	case _XSI_USEFULTYPE_CASTINGSPELL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
			case _XUSER_STATE_7 :
				return TRUE;
			}
		}
		break;
	case _XSI_USEFULTYPE_FINISHSKILL :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
					return TRUE;
			}
		}
		break;
	case _XSI_USEFULTYPE_HIDDENMA :
		{
			switch(m_CurrentBattleState)
			{
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
			case _XUSER_STATE_7 :
			case _XUSER_STATE_8 :
			case _XUSER_STATE_9 :
				return TRUE;
			}
		}
		break;
	/*default : //Author : 양희왕 //breif :6 /20 밖으로 뺌
		{
			_XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
			if(pInventory_Window)
			{
				if(pInventory_Window->m_bHaveMouseSlot)
					return TRUE;
			}
			_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
			if(pNPCTrade_Window)
			{
				if(pNPCTrade_Window->m_bHaveMouseSlot)
					return TRUE;
			}
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if(pPCTrade_Window)
			{
				if(pPCTrade_Window->m_bHaveMouseSlot)
					return TRUE;
			}
			_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
			if(pWarehouse_Window)
			{
				if(pWarehouse_Window->m_bHaveMouseSlot)
					return TRUE;
			}
			_XWindow_RepairItem* pRepairitem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REPAIRITEM);
			if( pRepairitem_Window )
			{
				if( pRepairitem_Window->m_bHaveMouseSlot )
					return TRUE;
			}
			if(g_pSkill_Window)
			{
				if(g_pSkill_Window->GetHaveMouseSlot())
					return TRUE;
			}
			if( g_ZeroLevelMode )	// hotblood zerolevel
			{
				XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	
				if( pZeroLevelProcess  )
				{
					if( pZeroLevelProcess->GetEndEventSceneStart() )
						return TRUE;
					if( !pZeroLevelProcess->GetFirstEventSceneEnd() )
						return TRUE;
					if( pZeroLevelProcess->GetAllEventEnd() )
						return FALSE;
				}
			}
		}
		break;*/
	}

	if(m_UserState == _XDEF_USERSTATE_TRAINING)
	{
		if(m_SelectedTrainingMode == _XTRAINING_MEDITATION)
		{
			switch(m_CurrentTrainingState)
			{
			case _XUSER_STATE_1 :
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
			case _XUSER_STATE_6 :
			case _XUSER_STATE_7 :
				return TRUE;
			}
		}
		if(m_SelectedTrainingMode == _XTRAINING_DUMMY)
		{
			switch(m_CurrentTrainingState)
			{
			case _XUSER_STATE_1 :
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
				return TRUE;
			}
		}
		if(m_SelectedTrainingMode == _XTRAINING_SPELL)
		{
			switch(m_CurrentTrainingState)
			{
			case _XUSER_STATE_1 :
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
				return TRUE;
			}
		}

		// by mahwang
		if(m_SelectedTrainingMode == _XTRAINING_FIREWOODCUT)
		{
			switch(m_CurrentTrainingState)
			{
			case _XUSER_STATE_1 :
			case _XUSER_STATE_2 :
			case _XUSER_STATE_3 :
			case _XUSER_STATE_4 :
			case _XUSER_STATE_5 :
				return TRUE;
			}
		}
		
		// by mahwang
		// 물긷기는 이동이 가능해야 한다.
		if(m_SelectedTrainingMode == _XTRAINING_WATERDRAW)
		{
			switch(m_CurrentTrainingState)
			{
				// 물을 긷는 동작과 물을 붓는 동작은 움직이지 못하도록 한다.
			case _XUSER_STATE_4 :
			case _XUSER_STATE_7 :
				return TRUE;
			}
		}
	}
	
    _XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
	if(pInventory_Window)
	{
		if(pInventory_Window->m_bHaveMouseSlot)
			return TRUE;
	}
	_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
	if(pNPCTrade_Window)
	{
		if(pNPCTrade_Window->m_bHaveMouseSlot)
			return TRUE;
	}
	_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
	if(pPCTrade_Window)
	{
		if(pPCTrade_Window->m_bHaveMouseSlot)
			return TRUE;
	}
	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	if(pWarehouse_Window)
	{
		if(pWarehouse_Window->m_bHaveMouseSlot)
			return TRUE;
	}
	_XWindow_RepairItem* pRepairitem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REPAIRITEM);
	if( pRepairitem_Window )
	{
		if( pRepairitem_Window->m_bHaveMouseSlot )
			return TRUE;
	}
	if(g_pSkill_Window)
	{
		if(g_pSkill_Window->GetHaveMouseSlot())
			return TRUE;
	}
	if( g_ZeroLevelMode )	// hotblood zerolevel
	{
		XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;	
		if( pZeroLevelProcess  )
		{
			if( pZeroLevelProcess->GetEndEventSceneStart() )
				return TRUE;
			if( !pZeroLevelProcess->GetFirstEventSceneEnd() )
				return TRUE;
			if( pZeroLevelProcess->GetAllEventEnd() )
				return FALSE;
		}
	}

	// 탈출 중이면 이동이 불가능하다.
#ifdef _XDEF_ESCAPE_20070205
	_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
	if(pEscape_Window)
	{					
		if( pEscape_Window->GetShowStatus() )
		{
			// 탈출 중이면 이동이 불가능하다.
			// 메시지 삭제 - 2007.07.13 KUKURI
//			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3313), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);// 탈출 중이면 이동이 불가능하다
			return TRUE;
		}
	}
#endif

	// 개인 상점을 열었을 때는 이동이 불가능하다
	if( m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{
		return TRUE;
	}

	// by mahwang
	// 채집할때는 이동이 불가능하다
	if (GetMotionClass() == _XACTION_COLLECT)
	{
		return TRUE;
	}

	// 상태효과에서 이동 할 수 없음
	if(m_Lock.movelock == 1)
	{
		return TRUE;
	}	

	// 책을 읽는 도중이면 이동 불가
	if( m_bReadingBook )
		return TRUE;
	// 영약 복용 중이면 이동 불가
	if( m_bInternalUse )
		return TRUE;

	// 소켓 장착 중이면 이동 불가
	if( m_bSocketPlugIn )
		return TRUE; 

	// 겜블 박스 여는 중이면 이동 불가
	if( m_bGambleBoxOpen )
		return TRUE;

	// 비무의 특정 상태중이면 이동 불가
	if(m_UserState == _XDEF_USERSTATE_MATCH_MATCHING || m_UserState == _XDEF_USERSTATE_MATCH_BOW)
		return TRUE;

	// 비무 홍보/홍보동작 마무리 중이면 이동 불가
	if( m_bMatchPRMode || m_ModelDescriptor.GetCurMotion() == _XAN_MATCH_PR_END)
		return TRUE;
	
	// 장원전 진행중에 장주 협박 중이면 이동 불가
	if(m_bThreatening)
	{
		return TRUE;
	}
	
	return FALSE;
}

#define _XDEF_ITEMPICKINGENABLETIME 500

void _XLocalUser::GrabWorldItem(void)
{
	if(m_PickItem)
	{
		if( m_PvpNPCLock )	// PVP 시 Lock 
		{
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1578), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			return;
		}

		if( m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
		{
			g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LOCALUSER_1579), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
			return;
		}

        //베트남은 복면쓰면 아이템 못 줍게..
    #if defined(_XVIETNAMESE)
        if( m_bFaceMaskMode )
        {
            g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3829), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS ); //복면 착용 중에는 물품을 습득할 수 없습니다.
			return;
        }
    #endif

		FLOAT dx = m_Position.x - m_PickItem->m_Position.x;
		FLOAT dz = m_Position.z - m_PickItem->m_Position.z;
		FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

		if(distance < 1.3f)
		{
			DWORD lastpickeditemtime = ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LastPickedItemTime;
			if((g_LocalSystemTime - lastpickeditemtime) > _XDEF_ITEMPICKINGENABLETIME)
			{
				_XWindow_Inventory* pInventory_Window = (_XWindow_Inventory*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INVENTORY);
				if(pInventory_Window)
				{
					if(!pInventory_Window->m_bHaveMouseSlot)
					{
						((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_LastPickedItemTime = g_LocalSystemTime;

						int slotnumber = GetEmptyInventorySlot(m_PickItem);

						if(slotnumber != -1)
							g_NetworkKernel.SendPacket(MSG_NO_ITEM_GRAB_REQ, m_PickItem->m_ItemWorldID, slotnumber);
						else
							g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAIGAME_NOTEMPTY), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);

						m_PickItem = NULL;
						m_bGrabItem = FALSE;
					} // if(!pInventory_Window->m_bHaveMouseSlot)
				}
			}
		} // if(distance < 1.3f)
		else
		{
			D3DXVECTOR3 directionvector = m_Position - m_PickItem->m_Position;
			D3DXVec3Normalize(&directionvector, &directionvector);
			SetTargetPositionAngle(m_PickItem->m_Position);
		}
	}
}

#ifdef _XTS_ITEM_OPTIMIZATION
int _XLocalUser::GetEmptyInventorySlot(_XGameItem* pItem)
{
	if(pItem->m_cType == _XGI_FC_MONEY)
	{
		return _XINVENTORY_SLOTTYPE_MONEYINDEX;
	}

	char itemType = GetEqualItemType(pItem->m_cType);
	if( itemType == -1 )
		return -1;

	switch(itemType)
	{
	case _XGI_FC_CLOTHES :
	case _XGI_FC_BOOK :
	case _XGI_FC_ACCESSORY :
	case _XGI_FC_MUSIC :	
	case _XGI_FC_SOCKET :
	case _XGI_FC_ELIXIR :
	case _XGI_FC_BOX :
		{
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if(m_UserItemList[i].Get_m_sID() == 0)
					return i;
			}
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if(m_UserItemList[i].Get_m_sID() == 0)
					return i;
			}
		#endif
		}
		break;
	case _XGI_FC_WEAPON :
		{
			if(pItem->m_cSecond == _XGI_SC_WEAPON_HIDDEN)
			{
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			    {
				    if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				    {
					    if(m_UserItemList[i].Get_m_ucCount() < 255)
						    return i;
				    }
			    }
                for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			    {
				    if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                    {
                        if(m_UserItemList[i].Get_m_ucCount() < 255)
						    return i;
                    }
                }

                for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			    {
                    if( m_UserItemList[i].Get_m_sID() == 0 )
					    return i;
                }

			    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			    {
				    if( m_UserItemList[i].Get_m_sID() == 0 )
					    return i;
			    }
			#else
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
					{
						if(m_UserItemList[i].Get_m_ucCount() >= 255)
							continue;
						
						return i;
					}
				}
				if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				}
			#endif
			}
			else
			{
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if(m_UserItemList[i].Get_m_sID() == 0)
						return i;
				}
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
				for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
				{
					if(m_UserItemList[i].Get_m_sID() == 0)
						return i;
				}
			#endif
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			switch(pItem->m_cSecond) 
			{
			case _XGI_SC_LIFE_SOCKET :
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				#endif
				}
				break;
/*			case _XGI_SC_LIFE_CONSUMING :
			case _XGI_SC_LIFE_INCHANTSTUFF :
			case _XGI_SC_LIFE_SOCKETSSTUFF :*/
			default :
				{
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				        {
					        if(m_UserItemList[i].Get_m_ucCount() < 255)
						        return i;
				        }
			        }
                    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                        {
                            if(m_UserItemList[i].Get_m_ucCount() < 255)
						        return i;
                        }
                    }

                    for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
                        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
                    }

			        for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
			        }
				#else
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
						{
							if(m_UserItemList[i].Get_m_ucCount() >= 255)
								continue;
							
							return i;
						}
					}
					if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
					{
						for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
						{
							if(m_UserItemList[i].Get_m_sID() == 0)
								return i;
						}
					}
				#endif
				}
				break;
			}
		}
		break;
	case _XGI_FC_POTION :
		{
			switch(pItem->m_cSecond)
			{
			case _XGI_SC_POTION_NOTMERGE :
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(m_UserItemList[i].Get_m_sID() == 0)
							return i;
					}
				#endif
				}
				break;
			default :
				{
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				        {
					        if(m_UserItemList[i].Get_m_ucCount() < 255)
						        return i;
				        }
			        }
                    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                        {
                            if(m_UserItemList[i].Get_m_ucCount() < 255)
						        return i;
                        }
                    }

                    for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
                        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
                    }

			        for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
			        }
				#else
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
						{
							if(m_UserItemList[i].Get_m_ucCount() >= 255)
								continue;
							
							return i;
						}
					}
					if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
					{
						for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
						{
							if(m_UserItemList[i].Get_m_sID() == 0)
								return i;
						}
					}
				#endif
				}
				break;
			}
		}
		break;
	case _XGI_FC_CONSUMING :			
	case _XGI_FC_COUNTERACT :
	case _XGI_FC_RESOURCE :
	case _XGI_FC_QUEST :	
	case _XGI_FC_PASS :
	case _XGI_FC_BOXKEY :
		{
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				{
					if(m_UserItemList[i].Get_m_ucCount() < 255)
						return i;
				}
			}
            for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                {
                    if(m_UserItemList[i].Get_m_ucCount() < 255)
						return i;
                }
            }

            for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
                if( m_UserItemList[i].Get_m_sID() == 0 )
					return i;
            }

			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if( m_UserItemList[i].Get_m_sID() == 0 )
					return i;
			}
		#else
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				{
					if(m_UserItemList[i].Get_m_ucCount() >= 255)
						continue;
					
					return i;
				}
			}
			if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
			{
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if(m_UserItemList[i].Get_m_sID() == 0)
						return i;
				}
			}
		#endif
		}
		break;
	default :
		{
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if(m_UserItemList[i].Get_m_sID() == 0)
					return i;
			}
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if(m_UserItemList[i].Get_m_sID() == 0)
					return i;
			}
		#endif
		}
		break;
	}

	return -1;
}
#else
int _XLocalUser::GetEmptyInventorySlot(_XGameItem* pItem)
{
	if(pItem->m_cType == _XGI_FC_MONEY)
	{
		return _XINVENTORY_SLOTTYPE_MONEYINDEX;
	}

	switch(pItem->m_cType)
	{
	case _XGI_FC_CLOTHES :
	case _XGI_FC_CLOTHES2 : 
	case _XGI_FC_CLOTHES3 : 
	case _XGI_FC_BOOK :
	case _XGI_FC_ACCESSORY :
	case _XGI_FC_MUSIC :	
	case _XGI_FC_SOCKET :
	case _XGI_FC_ELIXIR :
	case _XGI_FC_BOX :
		{
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if(m_UserItemList[i].m_sID == 0)
					return i;
			}
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if(m_UserItemList[i].m_sID == 0)
					return i;
			}
		#endif
		}
		break;
	case _XGI_FC_WEAPON :
	case _XGI_FC_WEAPON2 :
	case _XGI_FC_WEAPON3 :
		{
			if(pItem->m_cSecond == _XGI_SC_WEAPON_HIDDEN)
			{
			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			    {
				    if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				    {
					    if(m_UserItemList[i].m_ucCount < 255)
						    return i;
				    }
			    }
                for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			    {
				    if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                    {
                        if(m_UserItemList[i].m_ucCount < 255)
						    return i;
                    }
                }

                for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			    {
                    if( m_UserItemList[i].Get_m_sID() == 0 )
					    return i;
                }

			    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			    {
				    if( m_UserItemList[i].Get_m_sID() == 0 )
					    return i;
			    }
			#else
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].m_sID == pItem->m_sID))
					{
						if(m_UserItemList[i].m_ucCount >= 255)
							continue;
						
						return i;
					}
				}
				if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].m_sID == 0)
							return i;
					}
				}
			#endif
			}
			else
			{
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if(m_UserItemList[i].m_sID == 0)
						return i;
				}

			#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
				for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
				{
					if(m_UserItemList[i].m_sID == 0)
						return i;
				}
			#endif
			}
		}
		break;
	case _XGI_FC_LIFE :
		{
			switch(pItem->m_cSecond) 
			{
			case _XGI_SC_LIFE_SOCKET :
				{
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(m_UserItemList[i].m_sID == 0)
							return i;
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
					{
						if(m_UserItemList[i].m_sID == 0)
							return i;
					}
				#endif
				}
				break;
			case _XGI_SC_LIFE_CONSUMING :
			case _XGI_SC_LIFE_INCHANTSTUFF :
			case _XGI_SC_LIFE_SOCKETSSTUFF :
				{
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				        {
					        if(m_UserItemList[i].m_ucCount < 255)
						        return i;
				        }
			        }
                    for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                        {
                            if(m_UserItemList[i].m_ucCount < 255)
						        return i;
                        }
                    }

                    for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			        {
                        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
                    }

			        for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			        {
				        if( m_UserItemList[i].Get_m_sID() == 0 )
					        return i;
			        }
				#else
					for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].m_sID == pItem->m_sID))
						{
							if(m_UserItemList[i].m_ucCount >= 255)
								continue;
							
							return i;
						}
					}
					if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
					{
						for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
						{
							if(m_UserItemList[i].m_sID == 0)
								return i;
						}
					}
				#endif
				}
				break;
			}
		}
		break;
	case _XGI_FC_POTION :
	case _XGI_FC_CONSUMING :			
	case _XGI_FC_COUNTERACT :
	case _XGI_FC_RESOURCE :
	case _XGI_FC_QUEST :	
	case _XGI_FC_PASS :
	case _XGI_FC_BOXKEY :
		{
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
				{
					if(m_UserItemList[i].m_ucCount < 255)
						return i;
				}
			}
            for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].Get_m_sID() == pItem->m_sID))
                {
                    if(m_UserItemList[i].m_ucCount < 255)
						return i;
                }
            }

            for(i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
                if( m_UserItemList[i].Get_m_sID() == 0 )
					return i;
            }

			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if( m_UserItemList[i].Get_m_sID() == 0 )
					return i;
			}
		#else
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if((m_UserItemList[i].Get_m_cType() == pItem->m_cType) && (m_UserItemList[i].m_sID == pItem->m_sID))
				{
					if(m_UserItemList[i].m_ucCount >= 255)
						continue;
					
					return i;
				}
			}
			if(i == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend)
			{
				for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
				{
					if(m_UserItemList[i].m_sID == 0)
						return i;
				}
			}
		#endif
		}
		break;
	default :
		{
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
			{
				if(m_UserItemList[i].m_sID == 0)
					return i;
			}

		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
			{
				if(m_UserItemList[i].m_sID == 0)
					return i;
			}
		#endif

		}
		break;
	}

	return -1;
}
#endif

void _XLocalUser::ProcessStateList(void)
{
	m_StateList.ProcessState();
}

void _XLocalUser::DrawStateList(void)
{
#ifdef _XDWDEBUG
#ifdef _XTS_PK
/*
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));

	if(m_UserState == _XDEF_USERSTATE_PK_READY)
		g_XBaseFont->Print(gnWidth/2, gnHeight/2, 1.0f, "userstate : pk ready");
	else if(m_UserState == _XDEF_USERSTATE_PK_FIGHT)
	{
		g_XBaseFont->Print(gnWidth/2, gnHeight/2, 1.0f, "userstate : pk fight");

		DWORD lefttime = (m_PKModeStartTime + m_PKModeLeftTime) - g_LocalSystemTime;
		g_XBaseFont->Print(gnWidth/2, gnHeight/2+15, 1.0f, "lefttime : %d", lefttime);
	}
	else if(m_UserState  == _XDEF_USERSTATE_PK_END)
		g_XBaseFont->Print(gnWidth/2, gnHeight/2, 1.0f, "userstate : pk end");
*/
#endif
#endif

#ifdef _XDWDEBUG

	extern BOOL	g_ShowUserStatus;
	
	if( g_ShowUserStatus )
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		
		int i = 0;
		DWORD lefttime = 0;

		map <int, _XCharacterState*>::iterator iter_state;
		for(iter_state = m_StateList.m_mapCharacterStateList.begin() ; iter_state != m_StateList.m_mapCharacterStateList.end() ; ++iter_state)
		{
			_XCharacterState* pState = iter_state->second;

			if(pState)
			{
				lefttime = (pState->m_dwStateStartTime + pState->m_usLeftTimeSec*1000) - g_LocalSystemTime;
				
				g_XBaseFont->Print(gnWidth/2, gnHeight/2 + i*15, 1.0f, "id[%d] / skillid[%d] / lefttimefromserver [%ds] /leftime[%d ms]", 
					pState->m_sEffectID, pState->m_sSkillID, pState->m_usLeftTimeSec, lefttime);
				i++;
			}
		}
		g_XBaseFont->Flush();
	}		
#endif
}

DWORD _XLocalUser::GetLeftTimeFromStateList(char fromtype, short effectid, short skillid, char slotindex)
{
	int index = m_StateList.FindState(fromtype, effectid, skillid, slotindex);
	if(index >= 0)
	{
		map <int, _XCharacterState*>::iterator iter_state = m_StateList.m_mapCharacterStateList.find(index);
		if(iter_state != m_StateList.m_mapCharacterStateList.end())
		{
			_XCharacterState* currentstate = iter_state->second;
			if(currentstate)
			{
				DWORD lefttime = currentstate->m_usLeftTimeSec * 1000;

				if((currentstate->m_dwStateStartTime + lefttime) <= g_LocalSystemTime)
					return 0;
				else
					return ((currentstate->m_dwStateStartTime + lefttime) - g_LocalSystemTime);
			}
		}
	}
	
	return 0;
}

void _XLocalUser::CharacterStateChatLock(TCHAR* message)
{
	int msglength = strlen(message);
	int randomvalue = 0;
	TCHAR changeStr[] = {"!@#$%^&*()_+=-~`<>?/.,"};

	for( int i = 0; i < msglength; ++i )
	{
		randomvalue = _XRandom() % 22;
		message[i] = changeStr[randomvalue];
	}
}

//	#ifdef _XTS_USECASHITEM -테스트중
void _XLocalUser::UseItem(int invennumber)
{
	if(!g_pState_Window)
		return;
	
	if(invennumber < 0)
		return;

	if(m_UserItemList[invennumber].Get_m_sID() <= 0)
		return;
	
#ifdef _XDEF_ESCAPE_20070205
	// 탈출 중에는 무공, 포션 키를 사용할 수 없다.
	_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);
	if(pEscape_Window)
	{
		if(pEscape_Window->GetShowStatus())	return;
	}
#endif

#ifdef _XDEF_CASTLEBATTLE
	if(m_bThreatening)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3355), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	// "장주 협박 중에는 사용할 수 없습니다."
		return;
	}
#endif
	
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	if(g_pLocalUser->GetEnterMatchEvent())
	{
		if(!g_MatchEventManager.CanUseItem(invennumber))
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3684), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//비무 중에는 사용할 수 없습니다.
			return;
		}
	}
#endif
	
	// errcode
	// 0 : 성공
	// 1 : 아직 사용할 수 없습니다.
	// 2 : 전투 상태에서는 사용할 수 없습니다.
	// 3 : 지금은 사용할 수 없습니다.
	// 4 : 다른 문파의 물건입니다.
	// 5 : 다른 보조영약의 효과가 남아 있습니다(재접속 시 해제)
	// 6 : 아직 사용할 수 없습니다.(재사용시간 종료 되지않음)
	// 7 : 타 세력의 사자후는 사용할 수 없습니다.
	// 8 : 낭인은 문파용 사자후는 사용할 수 없습니다.
	int errcode = 0;

	switch(m_UserItemList[invennumber].Get_m_cType())
	{
	case _XGI_FC_POTION :
		{
			// 예외 사항 처리 -----------------------------------------------┓
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(m_CurrentBattleState != _XUSER_STATE_NONE && m_CurrentBattleState != _XUSER_STATE_LAST))	// 내공심법 시전중
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1580), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return;
			}
			
			if( m_bReadingBook )	// 비급 읽는 중이면 리턴
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( m_bSocketPlugIn )	// 소켓 장착 중이면 리턴
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
			if( m_bGambleBoxOpen )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( g_pLocalUser->m_bInternalUse )	// 영약 복용 중이면 리턴
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
#ifdef _XTS_MASKSYSTEM
			if( g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].cSecondType == _XGI_SC_POTION_MASK )
			{
				if(m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)	// 비무중
				{
					if(g_pState_Window )
						g_pState_Window->PrintSystemMessage(5);
					return;
				}
				
				if( g_pLocalUser->m_bPEMode )
				{
					if(g_pState_Window )
						g_pState_Window->PrintSystemMessage(13);
					return;
				}
			}			
#endif
			// ------------------------------------------------------------------┛
			
#ifdef _XDEF_EXPANSION_POTION_070709_KUKURI
			if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sReqLevel > m_CharacterInfo.Get_innerlevel())
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_2333), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				return;
			}
#endif

			switch(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].cSecondType)
			{
			case _XGI_SC_POTION_HERT_IN :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_HERT_IN))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_HERT_OUT :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_HERT_OUT))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_FATIGUE :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_FATIGUE))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_FOOD :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_FOOD))
						errcode = 1;

					if(m_BattleMode != _XBATTLE_PEACE)
						errcode = 2;
				}
				break;
			case _XGI_SC_POTION_EVENT :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_EVENT))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_HWATA :
				{
					errcode = 3;
				}
				break;
#ifdef _XTS_MASKSYSTEM
			case _XGI_SC_POTION_MASK :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_MASK))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_ANTI :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_ANTI))
						errcode = 1;
				}
				break;
#endif
			case _XGI_SC_POTION_COOLDOWN :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_COOLDOWN))
						errcode = 1;
				}
				break;
			case _XGI_SC_POTION_ADDEXP :
				{
					if(g_PotionItemProperty[m_AccountItemsID].sSimgi == 4)
					{
						// 구룡 기행 사용자
						errcode = 9;
						break;
					}

					if(g_PotionItemProperty[m_AccountItemsID].sSimgi == 5)
					{
						// 새로운 구룡천하 사용자
						errcode = 12;
						break;
					}
					if(g_PotionItemProperty[m_AccountItemsID].sSimgi == 6)
					{
						// 구룡 기행 사용자
						errcode = 9;
						break;
					}

					TCHAR msgbuffer[512];
					memset(msgbuffer, 0, sizeof(msgbuffer));

					if(m_AccountItemsID > 0)
					{
						// 증가 아이템 사용중
#ifdef _XDEF_AVOID_OVERLAPPING_CASHITEM_080721_KUKURI	// 증가 아이템 중복 사용 불가
						errcode = 15;
						break;
#else
						_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2959));	//아이템 사용시, 기존 사용중이던 증가 아이템 서비스는 사라집니다. 사용하시겠습니까?
#endif
					}
					else
					{
						if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 1)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2960));	//경험치 증가 아이템을 사용하시겠습니까? 선택한 아이템은 사용 시 삭제됩니다.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 2)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2961));	//숙련도 증가 아이템을 사용하시겠습니까? 선택한 아이템은 사용 시 삭제됩니다.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 3)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2962));	//경험치/숙련도 증가 아이템을 사용하시겠습니까? 선택한 아이템은 사용 시 삭제됩니다.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 4)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2971));	//구룡천하를 사용하시겠습니까? 선택한 아이템은 사용 시 삭제됩니다.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 5)
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_3415));	//구룡천하(가칭)를 사용하시겠습니까? 선택한 아이템은 사용 시 삭제됩니다.
						}
						else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 6) //Author :양희왕 //breif : 구룡천하 6 추가
						{
							_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2971));	//구룡천하를 사용하시겠습니까? 선택한 아이템은 사용 시 삭제됩니다.
						}
					}

					if(errcode == 0)
					{
						m_UseItemInvenNumber = invennumber;

#ifdef _XTS_USECASHITEM
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
#else
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
#endif
						pMessageWindow->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), msgbuffer,TRUE, _XDEF_CASHITEMUSE_ADDEXPYES, _XDEF_CASHITEMUSE_ADDEXPNO);
						//pMessageWindow->StartTimer(5000, TRUE);
						
						return;
					}
				}
				break;
			case _XGI_SC_POTION_RESETPOINT :
				{
					TCHAR msgbuffer[512];
					memset(msgbuffer, 0, sizeof(msgbuffer));
					
					if( g_pLocalUser->m_CharacterInfo.Get_cClassGrade() > g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sJungGi )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3351), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
						return;
					}
					
					if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 1)
					{
						_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2963));	//오기 초기화 아이템을 사용하시겠습니까? 사용시 모든 오기가 초기화 됩니다.
					}
					else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 2)
					{
						_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2964)); //오의 초기화 아이템을 사용하시겠습니까? 사용시 모든 오의가 초기화 됩니다.
					}
					else
					{
						errcode = 1;
					}

					if(errcode == 0)
					{
						m_UseItemInvenNumber = invennumber;

#ifdef _XTS_USECASHITEM
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
#else
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
#endif

						pMessageWindow->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), msgbuffer,TRUE, _XDEF_CASHITEMUSE_RESETPOINTYES, _XDEF_CASHITEMUSE_RESETPOINTNO);
						//pMessageWindow->StartTimer(5000, TRUE);

						return;
					}
				}
				break;
			case _XGI_SC_POTION_NICKNAME :
				{
					if(g_PotionItemProperty[m_AccountItemsID].sSimgi == 4 || g_PotionItemProperty[m_AccountItemsID].sSimgi == 5 ||
						g_PotionItemProperty[m_AccountItemsID].sSimgi == 6)
					{
						// 구룡 기행 사용자
						errcode = 10;
						break;
					}

					_XWindow_NickName* pNickName_Window = (_XWindow_NickName*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NICKNAME);
					if(pNickName_Window)
					{
						if(pNickName_Window->m_RemainChangeTime == 0)
						{
							// 교체 시간이 이미 0
							errcode = 11;
							break;
						}
					}

					if(errcode == 0)
					{
						m_UseItemInvenNumber = invennumber;

						// system message 추가
#ifdef _XTS_USECASHITEM
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
#else
						_XMessageWindow* pMessageWindow = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
#endif
						pMessageWindow->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2968),TRUE, _XDEF_CASHITEMUSE_CLEARNICKNAMEYES, _XDEF_CASHITEMUSE_CLEARNICKNAMENO);
						//pMessageWindow->StartTimer(5000, TRUE);
						
						return;
					}
				}
				break;
#ifdef _XDEF_CASHITEM_SHOUT
			case _XGI_SC_POTION_SHOUT :
				{
					if(g_pLocalUser->m_bVirtualMode)
					{
						errcode = 14;	// 튜토리얼에서 사용 불가
					}

					if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 1)
					{
						// 백도 외치기
						if(GetGroupType() != _XGROUPTYPE_WHITE)
						{
							errcode = 7;
						}
					}
					else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 2)
					{
						// 흑도 외치기
						if(GetGroupType() != _XGROUPTYPE_BLACK)
						{
							errcode = 7;
						}
					}
					else if(g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sSimgi == 4)
					{
						// 문파 외치기
						if(m_CharacterInfo.groupindex == _XGROUP_NANGIN)
						{
							errcode = 8;
						}
					}

					if(g_pState_Window->FindUsingItemFromSecondType(_XST_SHOUT, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_SHOUT))
					{
						// 아직 사용할 수 없습니다. - 지연시간 체크
						errcode = 1;
					}
					
					if(errcode == 0)
					{
						// 성공이면 inven# 저장 후 메세지 박스 띄우기
						m_UseItemInvenNumber = invennumber;

						TCHAR msgbuffer[512];
						memset(msgbuffer, 0, sizeof(msgbuffer));

						int time = g_PotionItemProperty[m_UserItemList[invennumber].Get_m_sID()].sJungGi / 60;

						_snprintf(msgbuffer, sizeof(msgbuffer), _XGETINTERFACETEXT(ID_STRING_NEW_2958), time); // 사자후를 사용하시겠습니까? 사용시 %d분간 외치기가 가능합니다.

#ifdef _XTS_USECASHITEM
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW, TRUE);
#else
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
#endif
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), msgbuffer, TRUE, _XDEF_CASHITEMUSE_SHOUTYES, _XDEF_CASHITEMUSE_SHOUTNO);
						//pMessageBox->StartTimer(5000, TRUE);

						return;
					}
				}
				break;
#endif
			case _XGI_SC_POTION_RECALL : //Author : 양희왕 //소환령/호출령
				{
					errcode = 13; //아무것도 출력하지 않는다
				}
				break;
			case _XGI_SC_POTION_EVENT2 : //Author : 양희왕 //breif : 신약 아이콘 모잘라서 추가
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_POTION_EVENT2))
						errcode = 1;
				}
				break;
			}//SECONDTYPE

			// second type에 관계없이 체력이 0보다 작을 때는 모든 포션 사용 금지
			if(g_pLocalUser->m_CharacterInfo.Get_current_lifepower() <= 0)
			{
				errcode = 3;
			}

			switch(errcode)
			{
			case 0 :
				{
					// 성공
					g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);
				}
				break;
			case 1 :
				{
					// 아직 사용할 수 없습니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
				break;
			case 2 :
				{
					// 전투상태에서는 사용할 수 없습니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1586), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 3 :
				{
					// 지금은 사용할 수 없습니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				}
				break;
			case 7 :
				{
					// 타 세력의 사자후는 사용할 수 없습니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2956), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 8 :
				{
					// 낭인은 문파용 사자후는 사용할 수 없습니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2957), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 9 :
				{
					// 구룡 천하 사용자는 본 아이템을 사용할 수 없습니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2965), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 10 :
				{
					// 별호 변경이 자유로운 서비스를 이미 이용중입니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2966), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 11 :
				{
					// 아이템을 사용하지 않아도 별호를 교체할 수 있습니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_2967), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				}
				break;
			case 12 :
				{
					// 새로운 베트남 구룡천하
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3416), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//구룡 천하(가칭) 사용자는 본 아이템을 사용할 수 없습니다.
				}
				break;
			case 13 ://Author : 양희왕 추가
				{
					//아무것도 출력하지 않음
				}
				break;
			case 14 :
				{
					// 튜토리얼에서는 사용할 수 없음
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3692), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//"튜토리얼에서는 사용할 수 없습니다."
				}
				break;
			case 15 : 
				{
					// 동일한 효과의 아이템 사용 중에는 이용할 수 없습니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3843), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//동일한 효과의 아이템 사용 중에는 이용할 수 없습니다.
				}
				break;
			default :
				{
					// 아직 사용할 수 없습니다.
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
				break;
			}
		}
		break;//POTION
	case _XGI_FC_CONSUMING :
			{
				// 예외 사항 처리 -----------------------------------------------┓
				if( m_bReadingBook )	// 비급 읽는 중이면 리턴
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
					return;
				}
				if( m_bSocketPlugIn )	// 소켓 장착 중이면 리턴
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
					return ;
				}
				if( m_bGambleBoxOpen )
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
					return;
				}
				if( g_pLocalUser->m_bInternalUse )	// 영약 복용 중이면 리턴
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
					return ;
				}
				// ------------------------------------------------------------------┛

				if( g_ConsumingItemProperty[m_UserItemList[invennumber].Get_m_sID()].cClan != g_pLocalUser->m_CharacterInfo.groupindex )
					errcode = 4;
				
				switch(errcode)
				{
				case 0 :
					{

						// 성공
						g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);
					}
					break;
				case 4 :
					{
						// 다른 문파의 물건입니다.
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORGROUP), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					break;
				default :
					{
						// 아직 사용할 수 없습니다.
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					break;
				}
			}
			break;//CONSUMING
		case _XGI_FC_ELIXIR :
			{
				switch(g_ElixirItemProperty[m_UserItemList[invennumber].Get_m_sID()].cSecondType)
				{
				case _XGI_SC_ELIXIR_SUPPORT :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_SUPPORT))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_STRENGTH :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_STRENGTH))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_INNER :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_INNER))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_HEART :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_HEART))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_HEALTH :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_HEALTH))
							errcode = 5;
					}
					break;
				case _XGI_SC_ELIXIR_DEX :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_ELIXIR_DEX))
							errcode = 5;
					}
					break;
				}

				switch(errcode)
				{
				case 0 :
					{
						if( g_pInventory_Window )
						{
							if(g_ElixirItemProperty[m_UserItemList[invennumber].Get_m_sID()].cCashCheck == 1)
							{
								// 캐쉬 아이템 영약
								if(m_ElixirInfo._sIntoxication >= 100)
								{
									// 중독 지수가 100보다 클 경우 사용할 수 없음
									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
									pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3048), TRUE, 0 );
									pMessageBox->StartTimer(5000, TRUE);

									// 실패 처리
									g_pInventory_Window->m_ElixirInvenNumber = -1;
									m_CurrentInternalUseState = _XUSER_STATE_5;
									break;
								}
							}

							if( g_pInventory_Window->CheckIsInternalUse(m_UserItemList[invennumber].Get_m_sID()) )
							{
								// 성공
								_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
								if( pChargeGauge_Window )
								{
									if(!pChargeGauge_Window->GetShowStatus())	// 게이지모드셋팅후 쇼윈도우 해야함. 순서 철저
									{
										pChargeGauge_Window->SetDelayTime( FLOAT(g_ElixirItemProperty[m_UserItemList[invennumber].Get_m_sID()].usCoolDownDelay*1000.0f) );
										pChargeGauge_Window->SetGaugeMode( _XGAUGE_ELIXIR );
										pChargeGauge_Window->SetSelectedNumber( invennumber);								
										pChargeGauge_Window->ShowWindow(TRUE);
										m_CurrentInternalUseState = _XUSER_STATE_4;
										_XPlayInterfaceSound(ID_SR_INTERFACE_USE_ELIXIR01_WAV);								
									}
								}
							}
							else
							{
								// 실패
								g_pInventory_Window->m_ElixirInvenNumber = -1;						
								m_CurrentInternalUseState = _XUSER_STATE_5;
							}
						}
					}
					break;
				case 5 :
					{
						// 다른 보조영약의 효과가 남아 있습니다(재접속 시 해제)
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1588), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
						g_pInventory_Window->m_ElixirInvenNumber = -1;
						m_CurrentInternalUseState = _XUSER_STATE_5;
						
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
					}
					break;
				default :
					{
						// 아직 사용할 수 없습니다.
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					break;
				}
			}
			break;//ELIXIR
		case _XGI_FC_QUEST :
			{
				if( g_QuestItemProperty[g_pLocalUser->m_UserItemList[invennumber].Get_m_sID()].cSecondType == 1 )
				{
					g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);	
				}
			}
			break;//QUEST
		case _XGI_FC_BOOK :
			{
				switch(g_BookItemProperty[m_UserItemList[invennumber].Get_m_sID()].cSecondType)
				{
				case _XGI_SC_BOOK_SKILL :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_SKILL))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_STUDY :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_STUDY))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_ABILITY :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_ABILITY))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_SCROLL :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_SCROLL))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_STUFF :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_STUFF))
							errcode = 6;
					}
					break;
				case _XGI_SC_BOOK_PAPER :
					{
						if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].Get_m_cType(), _XGI_SC_BOOK_PAPER))
							errcode = 6;
					}
					break;
				}

				switch(errcode)
				{
				case 0 :
					{
						if( g_pInventory_Window )
						{
							if( g_pInventory_Window->CheckIsRead(m_UserItemList[invennumber].Get_m_sID()) )
							{
								// 성공
								_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
								if( pChargeGauge_Window )
								{
									if(!pChargeGauge_Window->GetShowStatus())
									{
										pChargeGauge_Window->SetDelayTime( FLOAT(g_BookItemProperty[m_UserItemList[invennumber].Get_m_sID()].usDelay*1000.0f) );
										pChargeGauge_Window->SetGaugeMode( _XGAUGE_READ );
										pChargeGauge_Window->SetSelectedNumber( invennumber);
										pChargeGauge_Window->ShowWindow(TRUE);
										m_CurrentReadBookState = _XUSER_STATE_4;
									}
								}
							}
							else
							{
								// 실패
								g_pInventory_Window->m_BookInvenNumber = -1;
								m_CurrentReadBookState = _XUSER_STATE_5;
							}
						}
					}
					break;
				case 6 :
					{
						// 아직 사용할 수 없습니다.(재사용시간 종료 되지않음)
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
						g_pInventory_Window->m_BookInvenNumber = -1;
						m_CurrentReadBookState = _XUSER_STATE_5;
					}
					break;
				default :
					{
						// 아직 사용할 수 없습니다.
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
					}
					break;
				}
			}
			break;//BOOK
	}
}
/*
#else
void _XLocalUser::UseItem(int invennumber)
{
	if(!g_pState_Window)
		return;

	bool bUse = true;
	bool bFood = true;

	if(m_UserItemList[invennumber].m_sID > 0)
	{
		if(m_UserItemList[invennumber].m_cType == _XGI_FC_POTION)
		{
			if(m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
				(m_CurrentBattleState != _XUSER_STATE_NONE && m_CurrentBattleState != _XUSER_STATE_LAST))	// 내공심법 시전중
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1580), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
				return;
			}
			
			if( m_bReadingBook )	// 비급 읽는 중이면 리턴
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( m_bSocketPlugIn )	// 소켓 장착 중이면 리턴
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
			if( m_bGambleBoxOpen )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( g_pLocalUser->m_bInternalUse )	// 영약 복용 중이면 리턴
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
#ifdef _XTS_MASKSYSTEM
			if( g_PotionItemProperty[m_UserItemList[invennumber].m_sID].cSecondType == _XGI_SC_POTION_MASK )
			{
				if(m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT)	// 비무중
				{
					if(g_pState_Window )
						g_pState_Window->PrintSystemMessage(5);
					return;
				}

				if( g_pLocalUser->m_bPEMode )
				{
					if(g_pState_Window )
						g_pState_Window->PrintSystemMessage(13);
					return;
				}
			}			
#endif			
			switch(g_PotionItemProperty[m_UserItemList[invennumber].m_sID].cSecondType)
			{
			case _XGI_SC_POTION_HERT_IN :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_HERT_IN))
						bUse = false;
				}
				break;
			case _XGI_SC_POTION_HERT_OUT :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_HERT_OUT))
						bUse = false;
				}
				break;
			case _XGI_SC_POTION_FATIGUE :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_FATIGUE))
						bUse = false;
				}
				break;
			case _XGI_SC_POTION_FOOD :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_FOOD))
						bUse = false;
#ifdef _XTS_PK
					if(m_BattleMode != _XBATTLE_PEACE)
#else
					if(m_BattleMode == _XBATTLE_BATTLE)
#endif
						bFood = false;
				}
				break;
			case _XGI_SC_POTION_HWATA :
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
					return;
				}
				break;
#ifdef _XTS_MASKSYSTEM
			case _XGI_SC_POTION_MASK :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_MASK))
						bUse = false;
				}
				break;
			case _XGI_SC_POTION_ANTI :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_POTION_ANTI))
						bUse = false;
				}
				break;
#endif
			}
			
			if(!bUse || !bFood)
			{
				if(!bFood)
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1586), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
				else
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1587), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
			}
			else
				g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);
		} // if(m_UserItemList[invennumber].m_cType == _XGI_FC_POTION)	
		else if(m_UserItemList[invennumber].m_cType == _XGI_FC_CONSUMING)
		{
			if( m_bReadingBook )	// 비급 읽는 중이면 리턴
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( m_bSocketPlugIn )	// 소켓 장착 중이면 리턴
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}
			if( m_bGambleBoxOpen )
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return;
			}
			if( g_pLocalUser->m_bInternalUse )	// 영약 복용 중이면 리턴
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );		
				return ;
			}

			if( g_ConsumingItemProperty[m_UserItemList[invennumber].m_sID].cClan == g_pLocalUser->m_CharacterInfo.groupindex )
				g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);						
			else 
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_CINFO_ERRORGROUP), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
		}
		else if( m_UserItemList[invennumber].m_cType == _XGI_FC_ELIXIR )
		{
			switch(g_ElixirItemProperty[m_UserItemList[invennumber].m_sID].cSecondType)
			{
			case _XGI_SC_ELIXIR_SUPPORT :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_SUPPORT))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_STRENGTH :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_STRENGTH))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_INNER :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_INNER))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_HEART :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_HEART))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_HEALTH :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_HEALTH))
						bUse = false;
				}
				break;
			case _XGI_SC_ELIXIR_DEX :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_ELIXIR_DEX))
						bUse = false;
				}
				break;
			}

			if(!bUse)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1588), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000);
				g_pInventory_Window->m_ElixirInvenNumber = -1;
				m_CurrentInternalUseState = _XUSER_STATE_5;

				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			}
			else
			{				
				if( g_pInventory_Window )
				{
					if( g_pInventory_Window->CheckIsInternalUse(m_UserItemList[invennumber].m_sID) )
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if( pChargeGauge_Window )
						{
							if(!pChargeGauge_Window->GetShowStatus())	// 게이지모드셋팅후 쇼윈도우 해야함. 순서 철저
							{
								pChargeGauge_Window->SetDelayTime( FLOAT(g_ElixirItemProperty[m_UserItemList[invennumber].m_sID].usCoolDownDelay*1000.0f) );
								pChargeGauge_Window->SetGaugeMode( _XGAUGE_ELIXIR );
								pChargeGauge_Window->SetSelectedNumber( invennumber);								
								pChargeGauge_Window->ShowWindow(TRUE);
								m_CurrentInternalUseState = _XUSER_STATE_4;
								_XPlayInterfaceSound(ID_SR_INTERFACE_USE_ELIXIR01_WAV);								
							}
						}							
					}
					else
					{
						g_pInventory_Window->m_ElixirInvenNumber = -1;						
						m_CurrentInternalUseState = _XUSER_STATE_5;
					}
				}				
			}
		}
		else if( m_UserItemList[invennumber].m_cType == _XGI_FC_QUEST )
		{
			if( g_QuestItemProperty[g_pLocalUser->m_UserItemList[invennumber].m_sID].cSecondType == 1 )
			{
				g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, invennumber);	
			}
		}
		else if(m_UserItemList[invennumber].m_cType == _XGI_FC_BOOK)
		{
			switch(g_BookItemProperty[m_UserItemList[invennumber].m_sID].cSecondType)
			{
			case _XGI_SC_BOOK_SKILL :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_SKILL))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_STUDY :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_STUDY))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_ABILITY :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_ABILITY))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_SCROLL :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_SCROLL))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_STUFF :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_STUFF))
						bUse = false;
				}
				break;
			case _XGI_SC_BOOK_PAPER :
				{
					if(g_pState_Window->FindUsingItemFromSecondType(_XST_ITEM, m_UserItemList[invennumber].m_cType, _XGI_SC_BOOK_PAPER))
						bUse = false;
				}
				break;
			}
			if(!bUse)
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_NOTUSE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	
				g_pInventory_Window->m_BookInvenNumber = -1;
				m_CurrentReadBookState = _XUSER_STATE_5;
			}
			else
			{
				
				if( g_pInventory_Window )
				{
					if( g_pInventory_Window->CheckIsRead(m_UserItemList[invennumber].m_sID) )
					{
						_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
						if( pChargeGauge_Window )
						{
							if(!pChargeGauge_Window->GetShowStatus())
							{
								pChargeGauge_Window->SetDelayTime( FLOAT(g_BookItemProperty[m_UserItemList[invennumber].m_sID].usDelay*1000.0f) );
								pChargeGauge_Window->SetGaugeMode( _XGAUGE_READ );
								pChargeGauge_Window->SetSelectedNumber( invennumber);
								pChargeGauge_Window->ShowWindow(TRUE);
								m_CurrentReadBookState = _XUSER_STATE_4;
							}
						}
					}
					else
					{
						g_pInventory_Window->m_BookInvenNumber = -1;
						m_CurrentReadBookState = _XUSER_STATE_5;
					}
				}				
			}			
		}  // BOOK		
	}
}
#endif
*/
BOOL _XLocalUser::HaveSkillType(char clan, char stype)
{
	if(clan >= 0)
	{
		if(stype == 0)
		{
			// 문파 무공 체크
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
			{
				if(g_SkillProperty[m_SkillList[i].m_sSkillID]->clanType == clan)
					return TRUE;
			}
		}
		else
		{
			// 무공 stype 체크
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
			{
				if(g_SkillProperty[m_SkillList[i].m_sSkillID]->clanType == clan && 
					g_SkillProperty[m_SkillList[i].m_sSkillID]->skillSType == stype)
					return TRUE;
			}
		}
	}
	
	return FALSE;
}

void _XLocalUser::SetDyingMode(void)
{
	if(GetMotionClass() != _XACTION_DYING)
	{
		// 부활 윈도우 띄우기
		_XWindow_Rebirth* pRebirth_Window = (_XWindow_Rebirth*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REBIRTH);
		if( pRebirth_Window && !g_ZeroLevelMode && m_UserState != _XDEF_USERSTATE_MATCH_FIGHT)
		{
			if(!pRebirth_Window->GetShowStatus())
			{
				pRebirth_Window->SetRebirthMode(_XREBIRTH_BINSA);
#ifdef _ACCLAIM_IGAADSYSTEM
				if( g_IGAADWrapper.CheckExistContent( _XIGAADZONE_DEATHVIDEOAD ) )
				{
					pRebirth_Window->m_ShowIGAADButton = TRUE;
				}
				else
				{
					pRebirth_Window->m_ShowIGAADButton = FALSE;
				}
#endif
				pRebirth_Window->ShowWindow(TRUE);
			}
		}

		SetAnimation(_XACTION_DYING);
		_XPlayInterfaceSound(ID_SR_INTERFACE_LEVELFAIL_WAV);

#ifdef _XDEF_PARTY_NOTAVAILABLE
		// 자리비움 상태이면 해제
		if(m_bOut)
		{
			SetOut(FALSE);
		}
#endif
	}
}


void InsertAttackResultFactorEffect( int resultfactor )
{

}

short _XLocalUser::GetStanceMotion(short activeslotid)
{
	if(m_BattleMode == _XBATTLE_BATTLE || m_BattleMode == _XBATTLE_PK)
	{
		short stancemotion = -1;
		
		if(activeslotid > 0)
		{
			if(m_CharacterInfo.gender == _XGENDER_MAN)
				stancemotion = g_SkillProperty[activeslotid]->stance_male;
			else
				stancemotion = g_SkillProperty[activeslotid]->stance_female;
		}

		if(stancemotion < 0)
		{
			if(m_CharacterInfo.modeldescript.item1 != 0)
			{
#ifdef _XTS_ITEM_OPTIMIZATION
				if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
				{
					_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
					if( WeaponItem )
					{
						if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
							WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
						{
							stancemotion = _XAN_BATTLEIDLE_SWORD_START;
						}
					}
				}
#else
				if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
				{
					if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
						g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						stancemotion = _XAN_BATTLEIDLE_SWORD_START;
					}
				}
				else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
				{
					if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
						g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						stancemotion = _XAN_BATTLEIDLE_SWORD_START;
					}
				}
				else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
				{
					if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
						g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						stancemotion = _XAN_BATTLEIDLE_SWORD_START;
					}
				}
#endif
			}
		}

		return stancemotion;
	}
	else
	{
		short stancemotion = -1;
		
		if(m_CharacterInfo.modeldescript.item1 != 0)
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
				if( WeaponItem )
				{
					if(WeaponItem[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
						WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						if(activeslotid > 0)
						{
							if(m_CharacterInfo.gender == _XGENDER_MAN)
								stancemotion = g_SkillProperty[activeslotid]->stance_male+2;
							else
								stancemotion = g_SkillProperty[activeslotid]->stance_female+2;
						}
						else
							stancemotion = _XAN_BATTLEIDLE_SWORD_END;
					}
				}
			}
#else
			if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON)
			{
				if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
					g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
				{
					if(activeslotid > 0)
					{
						if(m_CharacterInfo.gender == _XGENDER_MAN)
							stancemotion = g_SkillProperty[activeslotid]->stance_male+2;
						else
							stancemotion = g_SkillProperty[activeslotid]->stance_female+2;
					}
					else
						stancemotion = _XAN_BATTLEIDLE_SWORD_END;
				}
			}
			else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2)
			{
				if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
					g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
				{
					if(activeslotid > 0)
					{
						if(m_CharacterInfo.gender == _XGENDER_MAN)
							stancemotion = g_SkillProperty[activeslotid]->stance_male+2;
						else
							stancemotion = g_SkillProperty[activeslotid]->stance_female+2;
					}
					else
						stancemotion = _XAN_BATTLEIDLE_SWORD_END;
				}
			}
			else if(m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3)
			{
				if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cSecondType == _XGI_SC_WEAPON_SHORT &&
					g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_SWORD)
				{
					if(activeslotid > 0)
					{
						if(m_CharacterInfo.gender == _XGENDER_MAN)
							stancemotion = g_SkillProperty[activeslotid]->stance_male+2;
						else
							stancemotion = g_SkillProperty[activeslotid]->stance_female+2;
					}
					else
						stancemotion = _XAN_BATTLEIDLE_SWORD_END;
				}
			}
#endif
		}
		
		return stancemotion;
	}
}

short _XLocalUser::GetMotionIndex(short selectedskillid)
{
	short motionindex = -1;
	if(m_CharacterInfo.gender == _XGENDER_MAN)
		motionindex = g_SkillProperty[selectedskillid]->motion_male;
	else 
		motionindex = g_SkillProperty[selectedskillid]->motion_female;

	return motionindex;	
}

char _XLocalUser::GetUsefulType(char selectedskillid)
{
	return g_SkillProperty[m_SelectedSkillID]->usefulType;
}

BOOL _XLocalUser::ProcessLPSSystem(_XMob* pMob)
{
	switch( g_MobAttribTable[pMob->m_MobType].scripttype ) {
	case 81:// Normal LPS
		{
			if(GetMotionClass() == _XACTION_LPS)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
				
					short tempselectedskillid = g_pLocalUser->m_SelectedSkillID;
					g_pLocalUser->m_SelectedSkillID = 0;
					g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
					
					g_pLocalUser->m_SelectedSkillID = tempselectedskillid;
					
					
					if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
					{
						if(pMob == (_XMob*)g_pLocalUser->m_AttackTarget)
							g_pLocalUser->ResetAttackTarget();
					}
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					m_CurrentBattleState = _XUSER_STATE_LAST;
					// delay time 계산 시작
					m_ChangeDelayStartTime = g_LocalSystemTime;
					
					pMob->SetAnimation(_XMOBACTION_DIE);
					SetAnimation(_XACTION_IDLE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);				
				}
			}
			else
			{
				
				SetAnimation(_XACTION_LPS);
				m_RotateAngle = _XMeshMath_GetRotateRadian(m_Position, pMob->m_Position );
				UpdatePosition();
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				
				if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
				{
					SetAttackTarget((_XItem*)pMob, _XPICKINGTARGET_MOB);
				}
				else
				{
					m_CurrentBattleState = _XUSER_STATE_1;
				}
			}
			return TRUE;
		}
		break;
	case 82:// Key LPS
		{
			if(GetMotionClass() == _XACTION_LPS)
			{
				if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
					// 아이템 체크 하기 
					bool bisitem = false;
					int	 inventorynumber = -1;
					for(int i = 0 ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; ++i)
					{
						if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == g_MobAttribTable[pMob->m_MobType].key_type )
						{
							if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == g_MobAttribTable[pMob->m_MobType].key_id )
							{
								inventorynumber = i;
								bisitem = true;
								break;
							}
						}
					}
				#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장  
					if( !bisitem )
					{
						for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++) 
						{
							if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == g_MobAttribTable[pMob->m_MobType].key_type )
							{
								if(g_pLocalUser->m_UserItemList[i].Get_m_sID() == g_MobAttribTable[pMob->m_MobType].key_id )
								{
									inventorynumber = i;
									bisitem = true;
									break;
								}
							}
						}
					}
				#endif

					if( bisitem )
					{
						// 패킷보내기 
						m_bKeyLPSProcess = TRUE;
						g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, inventorynumber, 1, pMob->m_MobID );
						// 몬스터는 사라진다.
						pMob->SetAnimation(_XMOBACTION_DIE);
						pMob->m_ModelDescriptor.SetLockEndFrame(TRUE);	
						
					} 
					else
					{												
						TCHAR messagestring[512];
						memset(messagestring, 0, sizeof(messagestring));

						_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_LOCALUSER_2326), 
							_XGameItem::GetItemName(g_MobAttribTable[pMob->m_MobType].key_type, 
							g_MobAttribTable[pMob->m_MobType].key_id));
						g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
						m_bLeftClickProcess = TRUE;
					}
					
					if( g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
					{
						if(pMob == (_XMob*)g_pLocalUser->m_AttackTarget)
							g_pLocalUser->ResetAttackTarget();
					}
					
					m_bUsingSkill = FALSE;
					m_bCheckDelayTime = TRUE;
					m_CurrentBattleState = _XUSER_STATE_LAST;
					// delay time 계산 시작
					m_ChangeDelayStartTime = g_LocalSystemTime;
					
					SetAnimation(_XACTION_IDLE);
					g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				}
			}
			else
			{
				// 상자 여는 행동
				SetAnimation(_XACTION_LPS);
			}
			return TRUE;		
		}
		break;
	case 83:// Transform LPS
		{
			//short tempselectedskillid = g_pLocalUser->m_SelectedSkillID;
			//g_pLocalUser->m_SelectedSkillID = 0;
			//g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
			//SetAnimation(_XACTION_IDLE);
			//g_pLocalUser->m_SelectedSkillID = tempselectedskillid;
			return FALSE;
		}
		break;
	case 84:// HP LPS
		{	
			// 이벤트 때문에 임시로 처리한 코드 : 끝나면 지워야 함.
			bool biscondition = true;
			if( pMob->m_MobType == 1055 )
			{
				biscondition = false;
				if( g_pState_Window->FindPlayerStateList(_XST_EFFECT, 380))
					biscondition = true;
				
				if( !biscondition )
					if( g_pState_Window->FindBuffStateList(_XST_EFFECT, 380))
						biscondition = true;
					
				if( !biscondition )
					if( g_pState_Window->FindHostileStateList(_XST_EFFECT, 380))
						biscondition = true;
			}
			
			if( biscondition )
			{
				return FALSE;

			}
			else
			{
			
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT( ID_STRING_NEW_2905 ), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);//_T("진 위장군의 기가 부족합니다.")
				SetAnimation(_XACTION_IDLE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
				
				return TRUE;
			}
			//short tempselectedskillid = g_pLocalUser->m_SelectedSkillID;
			//g_pLocalUser->m_SelectedSkillID = 0;
			//g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
			//SetAnimation(_XACTION_IDLE);
			//g_pLocalUser->m_SelectedSkillID = tempselectedskillid;
			return FALSE;
		}
		break;
	case 85:// Optional LPS
		{
			/*
			short tempselectedskillid = g_pLocalUser->m_SelectedSkillID;
			g_pLocalUser->m_SelectedSkillID = 0;
			g_NetworkKernel.SendPacket(MSG_NO_MARTIAL_FIRE);
			SetAnimation(_XACTION_IDLE);
			g_pLocalUser->m_SelectedSkillID = tempselectedskillid;
			
			m_bUsingSkill = FALSE;
			m_bCheckDelayTime = TRUE;
			m_CurrentBattleState = _XUSER_STATE_LAST;
			// delay time 계산 시작
			m_ChangeDelayStartTime = g_LocalSystemTime;*/
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}

BOOL _XLocalUser::CheckTargetTypeFinal(void)
{
	if(m_SelectedSkillID <= 0)		// active skill에서는 사용하지 않으므로 0도 false로 처리
		return FALSE;

	switch(_XSkillItem::GetTargetType(m_SelectedSkillID)/*g_SkillProperty[m_SelectedSkillID]->targetType*/)
	{
	case _XSI_TARGETTYPE_SELF :
		{
			return TRUE;
		}
		break;
	case _XSI_TARGETTYPE_OTHER_ENEMY :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					return TRUE;
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pObj = (_XFunctionalObject*)m_AttackTarget;
					if(pObj)
					{
						if(pObj->m_bAttackable)
							return TRUE;
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
						(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
						(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
					{
						if(pUser->GetMotionClass() == _XACTION_DYING || pUser->GetMotionClass() == _XACTION_DIE)
							return FALSE;

						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_OTHER_FRIEND :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
						(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) || 
						(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
					{
						return FALSE;
					}
					else
					{
						return TRUE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_SELFOTHER_FRIEND :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF)
				{
					return TRUE;
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
						(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) || 
						(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
					{
						return FALSE;
					}
					else
					{
						return TRUE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_SELFOTHER_ENEMY :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF || m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
				{
					return TRUE;
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					_XFunctionalObject* pObj = (_XFunctionalObject*)m_AttackTarget;
					if(pObj)
					{
						if(pObj->m_bAttackable)
							return TRUE;
					}
				}
				else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if((m_UserState == _XDEF_USERSTATE_MATCH_FIGHT && m_MatchTargetID == pUser->m_Uniqueid) ||
						(m_UserState == _XDEF_USERSTATE_PK_READY && pUser->m_bEnemy) ||
						(m_UserState == _XDEF_USERSTATE_PK_FIGHT && CheckPKTarget(pUser)))
					{
						if(pUser->GetMotionClass() == _XACTION_DYING || pUser->GetMotionClass() == _XACTION_DIE)
							return FALSE;

						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_OTHER_ALL :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_PC || m_AttackTargetObjectType == _XPICKINGTARGET_MOB || m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					return TRUE;
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_ALL :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF || m_AttackTargetObjectType == _XPICKINGTARGET_PC ||
					m_AttackTargetObjectType == _XPICKINGTARGET_MOB || m_AttackTargetObjectType == _XPICKINGTARGET_FUNCTIONALOBJECT)
				{
					return TRUE;
				}
				else
					return FALSE;
			}
			return FALSE;
		}
		break;
	case _XSI_TARGETTYPE_POINT :
		{
			return TRUE;
		}
		break;
#ifdef _XTS_SKILLCOMATYPE
	case _XSI_TARGETTYPE_COMA :
		{
			if(m_AttackTarget)
			{
				if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
				{
					_XUser* pUser = (_XUser*)m_AttackTarget;
					if(pUser)
					{
						if(pUser->GetMotionClass() == _XACTION_DIE)
						{
							return TRUE;
						}
						else
							return FALSE;
					}
				}
			}
		}
		break;
#endif
	}
	
	return FALSE;
}

void _XLocalUser::SetMonsterWarningMessage(unsigned short mobtype)
{
	if( g_MobAttribTable[mobtype].explevel != 1 )
	{
		int difflevel = g_MobAttribTable[mobtype].level - g_pLocalUser->m_CharacterInfo.Get_innerlevel();

		if(difflevel < -4)
		{
			// LPS는 메세지를 출력하지 않는다 
			if( g_MobAttribTable[mobtype].scripttype < 81 || g_MobAttribTable[mobtype].scripttype > 85)
#ifdef _XDEF_MONSTERWARNING_MESSAGE_20070102
            {
                //Author : 양희왕 //미국 쥐는 메시지 안나오게..
                if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH && (mobtype == 1931 || mobtype == 1932) )
                {
                }
                else
                    g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2327), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
            }
#else
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2327), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE, D3DCOLOR_ARGB( 255, 250, 230, 170 ));
#endif

		}
	}

	int droplevelcount = 0;
	for( int i = 0; i < 8; ++i )
	{
		droplevelcount += g_MobAttribTable[mobtype].droplevel[i];
	}
	if( droplevelcount <= 4 )
	{
		int difflevel = g_MobAttribTable[mobtype].level - g_pLocalUser->m_CharacterInfo.Get_innerlevel();

		if(difflevel < -4)
		{
			// LPS는 메세지를 출력하지 않는다 
			if( g_MobAttribTable[mobtype].scripttype < 81 || g_MobAttribTable[mobtype].scripttype > 85)
#ifdef _XDEF_MONSTERWARNING_MESSAGE_20070102
			{
                //Author : 양희왕 //미국 쥐는 메시지 안나오게..
                if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH && (mobtype == 1931 || mobtype == 1932) )
                {
                }
                else
                    g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2327), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
            }
#else
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_2328), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE, D3DCOLOR_ARGB( 255, 250, 230, 170 ));
#endif

		}
	}
}

void _XLocalUser::DeleteWeaponModel(short skillid)
{
	// 모든 무기 type이면 들고 있던 무기 삭제
	if(g_SkillProperty[skillid]->weaponCType == -1 && g_SkillProperty[skillid]->weaponSType == 0)
	{
		int effectid = _XGameItem::GetInchantEffectId( m_CharacterInfo.modeldescript.item1Category,
						m_CharacterInfo.modeldescript.item1, m_CharacterInfo.weaponStrength );
		
		if( effectid > 0 )
		{
			m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
		}
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
#ifdef _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : 양희왕 //breif 무기 사라지는 버그
			if( WeaponItem && m_CurrentWeaponID == 0)
#else
			if( WeaponItem )
#endif
			{
				if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
					m_CharacterInfo.modeldescript.item1 = 0;
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
					m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
#endif
	}
}

void _XLocalUser::DeleteWeaponModel(void)
{
	int effectid = _XGameItem::GetInchantEffectId( m_CharacterInfo.modeldescript.item1Category,
					m_CharacterInfo.modeldescript.item1, m_CharacterInfo.weaponStrength );
	
	if( effectid > 0 )
	{
		m_ModelDescriptor.m_EffectManager->DeleteInstance(effectid);
	}
	// 들고 있던 무기 삭제
#ifdef _XTS_ITEM_OPTIMIZATION
	if(_XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
	{
		_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
#ifdef _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : 양희왕 //breif 무기 사라지는 버그
		if( WeaponItem && m_CurrentWeaponID == 0)
#else
		if( WeaponItem )
#endif
		{
			if(WeaponItem[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
				m_CharacterInfo.modeldescript.item1 = 0;
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
				m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
			}
		}
	}
#else
	if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
	{
		if(g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
		{
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		}
	}
	else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
	{
		if(g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
		{
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		}
	}
	else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
	{
		if(g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].cThirdType != _XGI_TC_WEAPON_FIST)
		{
			m_CurrentWeaponID = m_CharacterInfo.modeldescript.item1;
			m_CharacterInfo.modeldescript.item1 = 0;
			m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL);
			m_ModelDescriptor.m_ViewLeftSwordEffect = FALSE;
		}
	}
#endif
}

void _XLocalUser::ReloadWeaponModel(void)
{
	// 들고 있던 무기가 있을 경우 복구해주기
	if(m_CurrentWeaponID > 0)
	{
#ifdef _XTS_ITEM_OPTIMIZATION
		if( _XGI_FC_WEAPON == GetEqualItemType(m_CharacterInfo.modeldescript.item1Category) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_CharacterInfo.modeldescript.item1Category);
			if( WeaponItem )
			{
				if(WeaponItem[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
				{
					m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;

#ifdef _XDEF_MODELTHREADLOADING
					if( WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
							g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
					}
					else
					{
						m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					}
#else
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][WeaponItem[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
					m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
					m_CurrentWeaponID = 0;
				}
			}
		}
#else
		if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
		{
			if(g_WeaponItemProperty[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;

#ifdef _XDEF_MODELTHREADLOADING
				if( g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
												   g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
				}
				else
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
												   
				}
#else
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
					g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
		{
			if(g_WeaponItemProperty2[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;
#ifdef _XDEF_MODELTHREADLOADING
				if( g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
				}
				else
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
					
				}

#else
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
					g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty2[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
		}
		else if( m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
		{
			if(g_WeaponItemProperty3[m_CurrentWeaponID].cThirdType != _XGI_TC_WEAPON_FIST)
			{
				m_CharacterInfo.modeldescript.item1 = m_CurrentWeaponID;

#ifdef _XDEF_MODELTHREADLOADING
				if( g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex > 0 )
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
						g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
				}
				else
				{
					m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1, NULL );
				}
#else
				m_ModelDescriptor.SetMeshModel(_XDEF_MODELCLUSTER_ITEM1,				
					g_ItemModel_Weapon[m_CharacterInfo.gender][g_WeaponItemProperty3[m_CharacterInfo.modeldescript.item1].sModelIndex] );
#endif
				m_ModelDescriptor.m_ViewLeftSwordEffect = TRUE;
				m_CurrentWeaponID = 0;
			}
		}
#endif
		int effectid = _XGameItem::GetInchantEffectId( m_CharacterInfo.modeldescript.item1Category,
						m_CharacterInfo.modeldescript.item1, m_CharacterInfo.weaponStrength );

		if( effectid > 0 )
		{
			if( !m_ModelDescriptor.m_EffectManager->FindInstance(effectid) )
			{
				m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
					effectid, &m_ModelDescriptor.m_Position, NULL, TRUE );
			}			
		}
	}
}

short _XLocalUser::GetInterpolateMotionIndex(short skillid)
{
	short motionindex = -1;
	if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_FIRST)
	{
		if(m_CharacterInfo.gender == _XGENDER_MAN)
		{
			if(skillid > 0)
				motionindex = g_SkillProperty[skillid]->intermotion1_male;
		}
		else
		{
			if(skillid > 0)
				motionindex = g_SkillProperty[skillid]->intermotion1_female;
		}
	}
	else if(m_CurrentAttackSequence == _XDEF_ATK_SEQ_SECOND)
	{
		if(m_CharacterInfo.gender == _XGENDER_MAN)
		{
			if(skillid > 0)
				motionindex = g_SkillProperty[skillid]->intermotion2_male;
		}
		else
		{
			if(skillid > 0)
				motionindex = g_SkillProperty[skillid]->intermotion2_female;
		}
	}

	return motionindex;
}

bool ProcessXZCollision(const D3DXVECTOR3& contactpoint, D3DXVECTOR3& movevelocity, D3DXVECTOR3& nextposition)
{
	// Collision plane
	D3DXVECTOR3 N = contactpoint;
	
	// move the object
	nextposition += N;
	
	float nv = D3DXVec3Dot( &N, &movevelocity );    // impact velocity	
	if (nv > 0.0f)									// don't reflect
		return false;
	
    float n2 = D3DXVec3Dot( &N, &N );				// the normal of collision length squared
	
	if (n2 < 0.00001f)								// to small, can't be of any use
		return false;
	
	static float fElasticity = 2.8f;				// coefficient of elasticity
	static float fFriction   = 0.005f;				// coefficient of friciton
	//static float fElasticity = 1.4f;				// coefficient of elasticity
	//static float fFriction   = 0.010f;				// coefficient of friciton
	
	// collision response. calcualte velocity...
	D3DXVECTOR3 Vn = N * (nv / n2);					// relative velocity alon the normal of collision
	D3DXVECTOR3 Vt = movevelocity - Vn;				// tangencial velocity
	
	// V = -Vn . (1.0f + CoR) + Vt . CoF
	movevelocity -= (1.0f + fElasticity) * Vn + Vt * fFriction;	// reflect
	
	return true;
}

void _XLocalUser::CheckCollideStatusInIdle( void )
{
	if( g_FreeMoveMode ) return;

	if( !m_pCollideOBB ) return;
	if( gpInput->GetMouseState()->bButton[0] ) return;
	if( gpInput->CheckKeyPress(DIK_LMENU) ) return;
	
	POINT pt;
	_XLODTILE*	pTerrainTile = g_LodTerrain.GetTerrainTile( m_Position , &pt );	
	if( !pTerrainTile )
	{
		return;
	}
	
	D3DXVECTOR3 directionvector = m_pCollideOBB->m_kCenter - m_Position;
	D3DXVec3Normalize( &directionvector, &directionvector );

	m_CharacterOBB.m_kCenter.x = m_Position.x;
	m_CharacterOBB.m_kCenter.y = m_Position.y + 0.89f;
	m_CharacterOBB.m_kCenter.z = m_Position.z;
	
	m_pGoundCollideOBB = NULL;
	m_pCollideOBB = NULL;
	D3DXVECTOR3 contactpoint;
	
	// Check collision
	if( !pTerrainTile->svObbList.empty() )
	{
		_XOBBData* pObbData;
		svdef_OBBData::iterator it;
		for (it = pTerrainTile->svObbList.begin(); it != pTerrainTile->svObbList.end(); ++ it)
		{
			pObbData = (*it);
			if( m_CharacterOBB.IntersectOBBvsOBB( *pObbData, contactpoint ) )
			{
				if( pObbData->m_OBBLevel == 1 )
					m_pCollideOBB = pObbData;
				
				D3DXVECTOR3 prevPosition = m_Position;
				ProcessXZCollision(contactpoint, directionvector, m_Position );

				if( prevPosition.x == m_Position.x && prevPosition.y != m_Position.y && prevPosition.z == m_Position.z )
				{
					m_Position = prevPosition + (directionvector * -0.03f);
				}

				m_CharacterOBB.m_kCenter.x = m_Position.x;
				m_CharacterOBB.m_kCenter.y = m_Position.y + 0.89f;
				m_CharacterOBB.m_kCenter.z = m_Position.z;
				
				m_FinalTargetPosition = m_TargetPosition = m_Position;
				
				UpdatePosition();
				
#ifdef _XDWDEBUG
				m_CharacterOBB.CreateOBBModel();
				pObbData->m_Collide	= TRUE;
#endif
			}
#ifdef _XDWDEBUG
			else pObbData->m_Collide = FALSE;
#endif
		}
	}

	if( g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject != 0 )
	{
		for( int i = 0; i < g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject; ++i )
		{
			if( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_bCollidable )// 충돌 가능할 때만 충돌 처리를 한다. ( 문이 부서지면 충돌처리를 하지 않는다.)
			{

				if( m_CharacterOBB.IntersectOBBvsOBB( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i], contactpoint ) )
				{
					D3DXVECTOR3 prevPosition = m_Position;
					ProcessXZCollision(contactpoint, directionvector, m_Position );

					if( prevPosition.x == m_Position.x && prevPosition.y != m_Position.y && prevPosition.z == m_Position.z )
					{
						m_Position = prevPosition + (directionvector * -0.03f);
					}

					m_CharacterOBB.m_kCenter.x = m_Position.x;
					m_CharacterOBB.m_kCenter.y = m_Position.y + 0.89f;
					m_CharacterOBB.m_kCenter.z = m_Position.z;
					
					m_FinalTargetPosition = m_TargetPosition = m_Position;
					
					UpdatePosition();
#ifdef _XDWDEBUG
					g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_Collide = TRUE;
#endif				
				}
			}
#ifdef _XDWDEBUG
			else g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_Collide = FALSE;
#endif					
		}
	}	
}

void _XLocalUser::SetDefaultValueMatchTable()
{
	m_myMatchTableInfo.usWaitTime = 0;
	m_myMatchTableInfo.cRecordType = 0;
	m_myMatchTableInfo.cMatchType = 1;
	m_myMatchTableInfo.cMatchTime = 2;
	m_myMatchTableInfo.cUseHyperrun = 0;
	m_myMatchTableInfo.cUseBuff = 0;
	m_myMatchTableInfo.cMatchBalance = 50;

	m_myMatchTableInfo.myBetMoney = 0;
	m_myMatchTableInfo.targetBetMoney = 0;

	for(int i = 0 ; i < 6 ; ++i)
	{
		m_myMatchTableInfo.myBetItem[i].Reset();
		m_myMatchTableInfo.targetBetItem[i].Reset();
	}
}

void _XLocalUser::ResetLock(BOOL bCondition)
{
	if(bCondition)
	{
		// condition lock만 reset
		m_Lock.movelock			= 0;
		m_Lock.tradelock		= 0;
		m_Lock.battlelock		= 0;
		m_Lock.skilllock		= 0;
		m_Lock.medlock			= 0;
		m_Lock.spelllock		= 0;
		m_Lock.speedlock		= 0;
		m_Lock.chatlock			= 0;
		m_Lock.modelock			= 0;
		m_Lock.partylock		= 0;
		m_Lock.camerarotatelock	= 0;
		m_Lock.camerazoomlock	= 0;
		m_Lock.camerareverse	= 0;
		m_Lock.adddelaytime		= 0;
		m_Lock.notdrawusername	= 0;
	}
	else
	{
		// 전체 reset
		memset(&m_Lock, 0, sizeof(m_Lock));
	}
}

void _XLocalUser::SetOut(BOOL state)
{
	if(state)
	{
		// 자리비움 setting
		if(!m_bOut)
		{
			// 빈사 or 죽은 상태 일때
			if(GetMotionClass() == _XACTION_DYING || GetMotionClass() == _XACTION_DIE || GetMotionClass() == _XACTION_REBIRTH)
			{
				return;
			}
			// 노점 열고 있을 때
			if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
			{
				return;
			}
			// 비무 홍보중
			if(g_pLocalUser->m_bMatchPRMode)
			{
				return;
			}

			m_bOut = state;
			
			// send packet
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_to_absent_state);

			// 앉기로 변경
			g_pLocalUser->SetAnimationIndex(g_EmotionActionTable[4]);
			g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
		}
	}
	else
	{
		// 자리비움 취소
		if(m_bOut)
		{
			m_bOut = state;

			// send packet
			g_NetworkKernel.SendPacket(MSG_NO_STATE_CHANGE_REQUEST, state_change_en_to_normal_state);

			// idle motion으로 변경
			if(GetMotionClass() != _XACTION_DYING && GetMotionClass() != _XACTION_DIE && GetMotionClass() != _XACTION_REBIRTH)
			{
				g_pLocalUser->SetAnimation(_XACTION_IDLE);
				g_NetworkKernel.SendPacket(MSG_NO_ZONE_MOVE);
			}
		}
	}
}

//Author : 양희왕
const DWORD _XLocalUser::GetPCroomState( const _XUSER_PCROOM_STATE PCroomState ) const
{
	switch(PCroomState)
	{
	case _XDEF_PCROOM_ERROR :
		return m_dwPCroomInfo.PCROOM_State.dwError;
		break;
	case _XDEF_PCROOM_INTERNET:
		return m_dwPCroomInfo.PCROOM_State.dwInternet;
		break;
	case _XDEF_PCROOM_RATIOCAFE :
		return m_dwPCroomInfo.PCROOM_State.dwRatio;
		break;
	case  _XDEF_PCROOM_PREPAID:
		return m_dwPCroomInfo.PCROOM_State.dwPrepaid;
		break;
	case _XDEF_PCROOM_PREMIUM:
		return m_dwPCroomInfo.PCROOM_State.dwPremium;
		break;
	case _XDEF_PCROOM_NONE :
		return m_dwPCroomInfo.PCROOM_State.dwNone;
		break;
	default :
		return 0;
		break;
	}
}

void _XLocalUser::SetPCroomState( const _XUSER_PCROOM_STATE PCroomState, const DWORD dwResult )
{
	switch(PCroomState)
	{
	case _XDEF_PCROOM_ERROR :
		m_dwPCroomInfo.PCROOM_State.dwError = dwResult;
		break;
	case _XDEF_PCROOM_INTERNET:
		m_dwPCroomInfo.PCROOM_State.dwInternet = dwResult;
		break;
	case _XDEF_PCROOM_RATIOCAFE :
		m_dwPCroomInfo.PCROOM_State.dwRatio = dwResult;
		break;
	case _XDEF_PCROOM_PREPAID:
		m_dwPCroomInfo.PCROOM_State.dwPrepaid = dwResult;
		break;
	case _XDEF_PCROOM_PREMIUM:
		m_dwPCroomInfo.PCROOM_State.dwPremium = dwResult;
		break;
	case _XDEF_PCROOM_NONE :
		m_dwPCroomInfo.PCROOM_State.dwNone = dwResult;
		break;
	default :
		break;
	}
}

//Last Updated : 07/03/15

BOOL _XLocalUser::GetEnterMatchEvent()
{
	if(g_MatchEventManager.GetCurrentState() == 2	// start 상태
		&& this->m_bEnterMatchEvent == TRUE			// 참가중
		&& g_AreaManager.FindPositionInArea(this->m_Position, g_MatchEventManager.GetCurrentEventArea(), _XDEF_AREACATEGORY_PVPAREA))//비무장 안에 있음
	{
		return TRUE;
	}

	return FALSE;
}

BOOL _XLocalUser::GetEnterCastleBattle()
{
	//Author : 양희왕 //breif : 장원전 
	int areanumber = 0;
	if( m_bCastleBattle && (g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_CASTLE, areanumber) ||
		g_AreaManager.CheckInsideArea(g_pLocalUser->m_Position, _XDEF_AREACATEGORY_PUBLICPVPAREA, areanumber)) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL _XLocalUser::GetEnterMatchReadyArea()
{
	if(g_MatchEventManager.GetCurrentState() == 2	// start 상태
		&& this->m_bEnterMatchEvent == TRUE			// 참가중
		&& g_AreaManager.FindPositionInArea(this->m_Position, 94, _XDEF_AREACATEGORY_PLAYERSPAWN))//비무장 앞 부활 영역 안에 있음
	{
		return TRUE;
	}

	return FALSE;
}

void _XLocalUser::GetBWLeftTime(int& hour, int& min, int& sec)
{
	if(m_dwBWLeftTimeFromServer == 0)
	{
		// 권한 없음
		hour = 0;
		min = 0;
		sec = 0;
	}
	else
	{
		DWORD elapsedtime = g_LocalSystemTime - m_dwBWReceiveTimeFromServer;

		if(m_dwBWLeftTimeFromServer >= elapsedtime)
		{
			DWORD lefttime = m_dwBWLeftTimeFromServer - elapsedtime;

			min = (lefttime / 1000) / 60;
			sec = (lefttime / 1000) % 60;

			hour = min / 60;
			min = min % 60;
		}
		else
		{
			// 시간 초과
			hour = 0;
			min = 0;
			sec = 0;
		}
	}
}

void _XLocalUser::DrawBWLeftTime()
{
	int hour = 0, min = 0, sec = 0;
	GetBWLeftTime(hour, min, sec);
	
	if(hour == 0 && min == 0 && sec == 0)
		return;

	g_XLargeFont->SetColor(0xFFF5E88A);
	g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);

	int y = (gnHeight>>1) - 145 -70;

#ifdef _XTS_PKZONE_SERVERSETTING
	if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkserverinfo)
#else
	if(g_ZoneInfoTable[g_CurrentZoneInfoIndex-1].pkclientinfo)
#endif
	{
		DWORD elapsedtime = g_LocalSystemTime - ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_dwMapEnteringTime;
		if(elapsedtime <= 30000)
		{
			y = (gnHeight>>1) - 145 -30;
		}
	}

	g_XLargeFont->PutsAlign((gnWidth>>1), y, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3805));	// 패왕동 출입 남은 시간
	
	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN)
		g_XLargeFont->PrintAlign((gnWidth>>1), y+20, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%02d;%02d;%02d", hour, min, sec);
	else
		g_XLargeFont->PrintAlign((gnWidth>>1), y+20, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%02d:%02d:%02d", hour, min, sec);

	g_XLargeFont->Flush();
	g_XLargeFont->DisableGrowMode();
}