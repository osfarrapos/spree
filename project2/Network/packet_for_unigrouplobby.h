#pragma once

#define MSG_NO_UNI_GROUP_LOBBY                                               240

struct MSG_UNI_GROUP_LOBBY                // 통합 서버군 패킷 베이스 : 모든 패킷은 ucMode 로 구분.
{
	_sHeader header ;
    u_char   ucMessage ;
	u_char   ucMode ;				// 
} ;

enum
{
	en_ug_simple_grp_info=0,                    // 상위 서버군 정보.
	en_ug_select_simple_grp=1,                  // 상위 서버군 선택.
	en_ug_select_simple_grp_fail=2,             // 상위 서버군 선택 실패.
	en_ug_req_simple_grp_info=3,				// 상위 서버군 정보 요청 

	en_ug_detail_grp_info=10,                   // 하위 서버군의 정보.
	en_ug_move_request,                         // 11 캐릭터와 창고 이동 요청한다.
	en_ug_move_result_complete,                 // 12 이동이 성공.
	en_ug_move_result_change_name,              // 13 이름을 바꿀 필요가 있다.
	en_ug_move_result_fail,                     // 14 이동 요청 실패.

	en_ug_change_name_request=20,               // 이름 바꾸기 요청.
	en_ug_change_name_result,                   // 21 결과.

	en_ug_notyet_move_in_new=30,		// 30 신서버군 : 아직 통합을 완료하지 않았다.
	en_ug_force_use_charac_in_new,		// 31 신서버군 : 아직 통합을 완료하지 않은 서버군에서, 강제로 통합을 완료하도록 설정.
	en_ug_already_move_in_old,			// 32 이미 통합이 완료된 구서버군으로 접속. 메세지 보여주고 튕김.
	en_ug_force_use_result,				// 33 원래는 force_use 패킷 하나로 하려고 했으나, 주민번호 입력이 포함되어 요청/응답으로 나는다.
} ;

struct MSG_UG_SIMPLE_GROUP_INFO : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_simple_grp_info
	u_char	ucGroup_0_NoCharac ;   // 의천의 캐릭터 수.
	
	struct 
	{
		u_char	ucMoveState ;      // 이동 상태 : 0 - 미완, 1 - 완료된 상태(이동시킬수 없음)
		u_char	ucNoCharac[3] ;    // 각 구 서버군의 캐릭터 수.
	} GroupInfo[3] ;               // 그룹마다의 상태와 캐릭터 개수.
} ;

struct MSG_UG_SELECT_SIMPLE_GRP : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_select_simple_grp
	u_char	ucSelectNewGroupNo ;   // 선택한
} ;

struct MSG_UG_SELECT_SIMPLE_GRP_FAIL : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_select_simple_grp_fail
	u_char	ucSelectSimpleGrpFail ;    // 실패 이유 : "이미 통합 완료된 서버군입니다."
} ;

struct MSG_UG_DETAIL_GRP_INFO : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_detail_grp_info=10
    // 캐릭터 이름. 레벨. (창고는.. )

    u_char		ucCharacCount ; // 3개 구서버군의 캐릭터 개수
    u_char      ucDepotCount ;  // 이어서 가게 되는 창고 정보의 개수. 0이라면 창고 정보 안감.

    struct 
	{
		char		cOldGroupIndex ;	// 1,2,3 / 4,5,6 / 7,8,9
		u_char      ucCharacIndex ;		// 0,1,2 서버당 캐릭터 인덱스
		short       sLevel ;            // 성취 단계
		char        cName[13] ;         // 이름 
     } DetailInfo[9] ;   // Max 9
} ;

struct MSG_UG_MOVE_REQUEST : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_move_request,          // 11 캐릭터와 창고 이동 요청한다.
	u_char	ucSelectCharac[3] ;		// 선택 캐릭터 인덱스. 없으면. 0~2, 0~2 ( 0-1, 1-2 ) 없으면 0xff
	u_char	ucSelectDepotGroup ;    // 선택 창고 서버군 인덱스. 0,1,2 없으면 0xff
	char    cPassword[13] ;         // 주민번호 뒷자리 또는 기타.
} ;

struct MSG_UG_MOVE_RESULT_COMPLETE : public MSG_UNI_GROUP_LOBBY
{
   // en_ug_move_result_complete

   // 
	char	ucCompleteName[3][13] ;    // 옮긴 캐릭터 이름.
	u_short	usInnerLevel[3] ;			// 캐릭터 레벨.
	u_char	ucState[3] ;               // 0 - 완료, 1 - 이름을 바꿔야 한다.
	u_char	ucCompleteDepotGroup ;     // 없으면, 0xff
} ;

struct MSG_UG_MOVE_RESULT_FAIL : public MSG_UNI_GROUP_LOBBY
{
	//     en_ug_move_result_fail,  // 14 이동 요청 실패.
	u_char	ucFail ;     // 어따쓰지...
} ;

struct MSG_UG_CHANGE_NAME_REQUEST : public MSG_UNI_GROUP_LOBBY
{
    // en_ug_change_name_request=20, // 이름 바꾸기 요청.
	u_char	ucIndex ;                // 0,1,2 바꾸려는 캐릭터의 인덱스.
	char	ucNewName[13] ;          // 새이름
} ;

struct MSG_UG_CHANGE_NAME_RESULT : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_change_name_result,  // 21 결과.
	u_char	ucSuccess ;           // 0 - Success, 나머진 실패.
	u_char	ucIndex ;             // 
	char	ucNewName[13] ;       // 
} ;

struct MSG_UG_NOTYET_MOVE_IN_NEW : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_notyet_move_in_new : Server -> Client 통보.
	
} ;

struct MSG_UG_FORCE_USE_CHARAC_IN_NEW : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_force_use_charac_in_new : Client -> Server 로 요청 
	// DS 로 보낼 때는, 서버군 번호를 포함할 것.	
	char  cPW[13]; 
} ;

struct MSG_UG_ALREADY_MOVE_IN_OLD : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_already_move_in_old : Server -> Client	
} ;

struct MSG_UG_REQ_SIMPLE_GRP_INFO : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_req_simple_grp_info
} ;

struct MSG_UG_FORCE_USE_RESULT : public MSG_UNI_GROUP_LOBBY
{
	// en_ug_force_use_result
	u_char				ucResult ;	// 0 - Success, 1 - 비번 확인, 2 - 실패.
} ;