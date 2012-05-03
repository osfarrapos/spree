#ifndef __XKERNEL_H__
#define __XKERNEL_H__
#ifdef __cplusplus

/** @file XKernel.h */

#include "xgamebase.h"	

#define XKERNEL_TITLE				"XKernel"
#define XKERNEL_AUTHOR				"9Dragons team"
#define XKERNEL_WEBSITE				"www.indy21.co.kom"
#define XKERNEL_EMAIL				"kerygma@indy21.com"
#define XKERNEL_VERSION				0
#define XKERNEL_SUB_VERSION			3
#define XKERNEL_PATCH_VERSION		0
#define XKERNEL_DATE				20030130

#define _XDEF_LAUNCHER_SIGNAL3   	_T("KC2MB")
#define _XDEF_LAUNCHER_SIGNAL1   	_T("9E2GH")
#define _XDEF_LAUNCHER_SIGNAL5   	_T("KUV63T")
#define _XDEF_LAUNCHER_SIGNAL4   	_T("L4EBN")
#define _XDEF_LAUNCHER_SIGNAL2   	_T("4FWDW")

#define _XMEMORYCHECKDATA_ENCCHARCODE	0x1F
#define _XMEMORYCHECKDATA_ENCBYTECODE	0xC5
#define _XMEMORYCHECKDATA_ENCSHORTCODE	0xCE43
#define _XMEMORYCHECKDATA_ENCWORDCODE	0xBEF1
#define _XMEMORYCHECKDATA_ENCINTCODE	0xAE1CDB23
#define _XMEMORYCHECKDATA_ENCDWORDCODE	0xE137FDB1

#define _XMEMORYCHECKDATA_ENCCHAR(x)	((char)x^(char)_XMEMORYCHECKDATA_ENCCHARCODE)
#define _XMEMORYCHECKDATA_ENCchar(x)	((char)x^(char)_XMEMORYCHECKDATA_ENCCHARCODE)
#define _XMEMORYCHECKDATA_ENCBYTE(x)	((BYTE)x^(BYTE)_XMEMORYCHECKDATA_ENCBYTECODE)
#define _XMEMORYCHECKDATA_ENCSHORT(x)	((short)x^(short)_XMEMORYCHECKDATA_ENCSHORTCODE)
#define _XMEMORYCHECKDATA_ENCshort(x)	((short)x^(short)_XMEMORYCHECKDATA_ENCSHORTCODE)
#define _XMEMORYCHECKDATA_ENCWORD(x)	((WORD)x^(WORD)_XMEMORYCHECKDATA_ENCWORDCODE)
#define _XMEMORYCHECKDATA_ENCINT(x)		((int)x^(int)_XMEMORYCHECKDATA_ENCINTCODE)
#define _XMEMORYCHECKDATA_ENCint(x)		((int)x^(int)_XMEMORYCHECKDATA_ENCINTCODE)
#define _XMEMORYCHECKDATA_ENCDWORD(x)	((DWORD)x^(DWORD)_XMEMORYCHECKDATA_ENCDWORDCODE)
#define _XMEMORYCHECKDATA_ENCBOOL(x)	((BOOL)x^(BOOL)_XMEMORYCHECKDATA_ENCDWORDCODE)

#define _XDEF_ENCVALUE(_type, _val) private: \
									_type _val; \
									public: \
									void Set_## _val( _type _data ){ _val = _XMEMORYCHECKDATA_ENC##_type (_data); } \
									_type Get_##_val( void ){ return _XMEMORYCHECKDATA_ENC##_type (_val); }

// The main function that you must use
void XKernelMain(void);


// Base define
#include "XBaseDefs.h"             
#include "XExternDefs.h"

// Utilities
#include "XUtils.h"

// Application frameWork classes
#include "XDeviceEnum.h"
#include "X3DSettings.h"
#include "XDialogs.h"
#include "XProcess.h"
#include "X3DApp.h"

// Camera control
#include "XCamera.h"

// Math function
#include "XModelContainerDef.h"
#include "XMeshMath.h"
#include "XFastCode.h"

// Collide detection
#include "XCollideMath.h"
#include "XCollide.h"


// D3D Utilities
#include "d3dfont.h"
#include "d3dutil.h"
#include "dsutil.h"
#include "dxutil.h"


// Data manager
#include "Xitem.h"
#include "Xlist.h"                 


// Error detection
#include "XError.h"                
#include "XException.h"            
#include "XExceptionHandler.h"

// Texture manager
#include "XRenderStyleManager.h"
#include "XTexture.h"
#include "XTextureManager.h"

// Font system
#include "X3DUFont.h"              
#include "XFont.h"
#include "XUnicodeFont.h"

// Timer function
#include "XTimeCounter.h"          
#include "XTimer.h"

// XGUI Control
#include "XGUIObject.h"
#include "XGUIControlCenter.h"
#include "XBallGauge.h"
#include "XButton.h"
#include "XCheckButton.h"
#include "XImageStatic.h"
#include "XStatic.h"
#include "XStickGauge.h"
#include "XScrollBar.h"
#include "XToolTip.h"
#include "XHSlider.h"
#include "XVSlider.h"
#include "XWindow.h"
#include "XListBox.h"
#include "XWindowManager.h"
#include "XTreeBox.h"

// Input system
#include "XInput.h"
#include "XIME.h"

// Mesh control
#include "XBillboard.h"
#include "XObb.h"                  

// Security
#include "XEncryptor.h"
//#include "XCrypto.h"
#include "XCompress.h"

// Application data control
#include "XRegistry.h"
#include "XPackage.h"
#include "XMString.h"
#include "XStringItem.h"


// System window
#include "XWindow_IME.h"

#include "XStringDataBase.h"

// Shader

// 2006. 01. 07 ---------------------------------------┓
#define _XDEF_NEWANIMATIONLIST
// ----------------------------------------------------┛

// 2006. 01. 16 ---------------------------------------┓
#define _XTS_NEWSTRING				// eod string 변경
// ----------------------------------------------------┛

#define _XTS_MONSTERREF_12			// monster reference ver1.2
#define _XTS_PARTYMOREINFO			// 동행 추가정보 관련 ON/OFF
#define _XTS_NEWPARTYCOUNTER		// 강호 풍운록에서 파티 카운터 패킷 수정

#define _CHARAC_BLOCKING		// character blocking

// 2006. 03. 20 ---------------------------------------┓
#define _XTS_PK						// PK system
#define _XTS_MASKSYSTEM				// 복면 
// ----------------------------------------------------┛

// 2006. 03. 21 ---------------------------------------┓
#define _XTS_MASTERY			// 오의
// ----------------------------------------------------┛

// 2006. 04. 19 ---------------------------------------┓
#define _XTS_MONSTERREF_13			// monster reference ver 1.3
#define _XTS_FAME					// 명성/악명
#define _XTS_BLOODPOINT				// 혈정
#define _XTS_PKRECORD				// PK 대전 기록
// ----------------------------------------------------┛

// 2006. 05. 03 ---------------------------------------┓
#define _XDEF_NEWSTRINGMANAGER
// ----------------------------------------------------┛

// 2006. 05. 29 ---------------------------------------┓
#define _XTS_VIEWPKTARGETLEVEL		// pk target의 레벨 보기
// ----------------------------------------------------┛

// 2006. 07. 24 ---------------------------------------┓	// 대만/베트남하고 동일
#define _XTS_MONSTERHEAL			// mob heal
#define _XTS_ITEM_OPTIMIZATION		// 아이템 관련 코드 최적화
#define _XTS_QUESTAUTONODE			// 퀘스트 자동노드 : 퀘스트 구조가 바뀜
//#define _XTS_MEDI_TUTORIAL			// 운기요상 강제팝업
#define _XTS_WEAPONSTRADD			// 근력에 의한 무기데미지 증가율 변경
#define _XTS_HAVEQUESTLISTNPC		// 퀘스트를 여러개 가지고 있는 NPC
#define _XTS_VIEWPKTARGETLEVEL		// pk target의 레벨 보기
#define _XTS_CHANGEWEAPONDAMAGE		// 무기 데미지 계산 순서 변경
#define _XTS_AUTOMEDITATION			// 자동 운기요상
#define _XTS_LEVELTOOLTIP			// 레벨 tooltip
#define _XTS_NEWINSENSIBLE			// 기절 animation 변경
// ----------------------------------------------------┛

#define _XTS_NEWCHARACLIST			// character list 변경

// 2006. 08. 05 ---------------------------------------┓
#define _XTS_BASEMINMAXWEAPONDAMAGE	// 물리 무기 기본 공격력
// ----------------------------------------------------┛

// 2006. 08. 24 ---------------------------------------┓
#define _XTS_SKILLCOMATYPE			// 스킬 타겟 타입 추가 - 코마 상태
// ----------------------------------------------------┛

#define _XTS_ITEM_V20				// 아이템 버전 2.0
#define _XTS_ITEM_V21				// item table v2.1

#define _XTS_SHOWTUTORIAL

#define _XDEF_USENEWNPCTEXTSYSTEM		// TextDB에서 NPC 관련 스트링을 가져옴.
#define _XDEF_USENEWQUESTTEXTSYSTEM		// TextDB에서 QUEST 관련 스트링을 가져옴.
#define _XDEF_USENEWAREATEXTSYSTEM		// TextDB에서 AREA 관련 스트링을 가져옴.

// 2006. 11. 14 ---------------------------------------┓
/*       국내버젼과 해외버젼 동일하게 적용시킴.       */ 
#define _XDEF_EXPAND_ORGANIZATIONNAME	/* 영웅단 이름 길이 변경 */ 
#define _XDEF_EVENTWINDOW				/* 이벤트 창을 추가합니다.	2006년 9월 28일 업데이트 예정 입니다. 09월 27일: 엔타즈 구룡쟁패 모바일 런칭	*/										
// ----------------------------------------------------┛


#ifdef _XKOREAN
	// ----------------------------------------------------------------------------------------------------------┓
	#define _XTS_SERVERUNIFICATION					// 서버통합	한국만 적용

	#define _XTS_USECASHITEM						// cash item 사용 - 캐쉬 아이템 border와 message 창 border 처리
	#define _XTS_ITEMMALLBROWSER					// 아이템몰 - 아이템 몰 browser와 cash item package

	#define _XDEF_NEXONLOGIN						// 넥슨 로그인 - 로그인 패킷 변경
	#define _XDEF_NEXONLOGINFINAL					// 넥슨 메신저와 connect 하는 부분은 NEXONLOGINFINAL로 변경했음 
	#define _XDEF_NEXONUNIFICATIONGUIDE				// 정식서버 넥슨 통합 안내문 표시

	#define _XDEF_NEWDELETECHARACTER				// 캐릭터 삭제 - 주민번호 뒤4자리로 변경
	#define _XDEF_NEWVERSONFILETERING				// 필터링 리스트 업그레이드

	#define _XDEF_MONSTERREF_14						// 몬스터 테이블 1.4

	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help system server side save.

	// TEST용 (XTrap)
	//#define _XTS_XTRAP_TEST
	
	#define _XTS_CHARACTERFILTERING					// 캐릭터 이름 필터링 - 임시로 국내 버전만 적용

	#define _XTS_NEWSKILLWINDOW						// 스킬 창 변경
	#define _XTS_ITEM_V22							// item table v2.2

	// 캐쉬 아이템 사용 관련  - 해외버전에서 캐쉬 아이템을 사용만 할 경우 아래의 define들만 적용해주면 됨. --┓
	#define _XDEF_CASHITEM_SHOUT					// 외치기 (사자후)
	#define _XTS_MOVEVILLAGE						// 표국 이동
	#define _XDEF_CLOTHMASK_DIVIDE					// 치장의상을 캐쉬 아이템과 일반 아이템으로 나눈다.- icon 처리
	#define _XDEF_PERIODITEM_20061027				// 기간제 아이템 : 기간제 관련 TimeValue 추가
	// --------------------------------┛

	//#define _XTS_NEWCREATECHARACTER				// 캐릭터 생성창 리뉴얼 
	//#define _XTS_DELETESKILL						// 스킬 삭제

	// 2006.12.21 --┓
	#define _XTS_ALIANCE_20061018					// 영웅단 동맹 관련 인터페이스	12월 21일 업데이트?
	#define _XDEF_ITEMMALL_AUTOLOGIN_20061120		// 아이템몰 자동 로그인 기능 추가 12월 21일 업데이트?
	#define _XDEF_WEBBOARD_20061211					// 웹 게시판 기f능 추가 12월 21일 업데이트?

	#define _XDEF_SEARCHPARTYMEMBER					// 동행 찾기 기능

	#define	_XDEF_CHRISTMASEVENT					// 크리스마스 이벤트
	#define _XTS_REFUSE_ORGANIZATION				// 영웅단 초대 거부
	// --------------┛
	
	#define _XTS_SAFETYZONE							// 안전지역 구현
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone으로 이동할 때 선택 하도록 변경

	// 2007.01.04 ---┓
	#define _XDEF_MONEYDROP_NO_20070102				// 금전 버리기 불가능
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// 몬스터 경고 문구 -> 시스템 메시지로 이동

	#define _XDEF_NOTICE_STRING_PROCESS				// 게임 내 공지사항 클래스 추가 Author : 양희왕 Last updated: 07/01/04
    // --------------┛


	// 2007.02.08 ---┓
	#define _XDEF_ALIANCE_WEBBOARD_20070130	// 장원전 - 영웅단,영웅회 게시판 바로가기 
    // --------------┛

	// 2007.02.15 ---┓
	#define _XDEF_ESCAPE_20070205					// 탈출 기능 
	// --------------┛

	// 2007.02.22 ---┓
	#define _XDEF_SHOWEVENTNOTICE					// 이벤트 창 팝업 ON/OFF
	#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// 이벤트 창 웹 페이지로 만들기
	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB format 변경
	// --------------┛

	// 2007.03.15 ---┓
	#define _XDEF_CASTLEBATTLE						// 장원전
	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet 변경
	// --------------┛

	// 2007.03.22 ---┓
	#define _XTS_MASTERY_04							// masterytable.bms ver 0.4
	#define _XTS_MASTERYOPTIONAL					// 오의 변경 - 확장
	#define	_XDEF_RESIST_DISPLAY					// 음양혼신 저항력 표기
	// --------------┛

	// 2007.04.05 ---┓
	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author 양희왕 @breif 스킬 딜레이 단축되는 부분 수정*/
	#define _XDEF_WASTEBASKET_20070226				// 휴지통 기능 추가
	#define _XDEF_PARTY_NOTAVAILABLE				// 자리비움 & 동행 강화
	
	#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	#define	_XDEF_INPUTPRESS	/*!< @author 양희왕 @breif 초식기 버그 */
	// --------------┛

	// 2007.04.19 ---┓
	#define _XDEF_CASTLEBATTLE_SECONDEDITION		// 장원전 - 2차 업데이트
	#define _XDEF_CASTLEBATTLE_MAPTAXRATE			// 지역 세율 적용
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author 양희왕 @breif 초식기 버그 쪼아 매크로 : _XDEF_INPUTPRESS와 같이 적용되어야 함*/
	// --------------┛

	// 2007.04.26 ---┓
	#define _XDEF_WAREHOSE_RESTRAINT_OF_TRADE	/*!< @author 양희왕 @breif 특정 아이템 창고 거래 제한*/
	// --------------┛

	// 2007.05.03 ---┓
	#define _XDEF_NPC_ATTACK_CHECK	/*!< @author 양희왕 @breif 무림명숙 공격시 쉬프트 클릭*/
	// --------------┛

	// 2007.05.17 ---┓
	#define _XDEF_SMELT_ITEM /*!< @author 양희왕 @breif 용루/용린 제련 아이템 */
	// --------------┛

	// 2007.05.31 ---┓
	#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// 장원전 - 장주 이름표
	#define _XDEF_CASTLEBATTLE_RECORD					// 장원전 - 전적 
	#define _XDEF_CASTLEBATTLE_MINIMAP					// 장원전 - 미니맵
	#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author 양희왕 @breif PK 위치추적 리스트 */
	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// 채팅창 기능 확장.
	#define _XDEF_ZONECHATTING_20070108					// 맵 채팅 (지역 대화)
	#define KR_RB_DELAYSHOWNEXONNOTICEWINDOW_070530_KERYGMA	// 15세 이용가 관련 Delay.
	// --------------┛

	// 2007.06.07 ---┓
	#define _XDEF_AGEGRADE_070607_MAGVIPER	/*!< @author 양희왕 @breif 이용등급*/
	// --------------┛

	// 2007.06.14 ---┓
	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER	/*!< @author 양희왕 @breif 신구룡전도*/
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// 채팅 리뉴얼 중 (동맹)관련 부분 버그 수정.
	// --------------┛

	// 2007.06.28 ---┓
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load 처리:엔진 최적화
	#define VN_RB_GAMBLEBOX_070607_KUKURI			// 패왕궤 관련 아이템 복사 오류 수정 (MSG_SM_EQUIP_RESULT.. 패킷 변경)
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author 양희왕 @breif 친구소환*/

	// 나중에 적당히 define 정리할 것.
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// 인던 입장권
//	#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// 인던 입장권 UI 뺀 것(국내 적용x) : 인던입장권 없이 item 관련 패킷(MSG_EX_BATTLE_ROOM_REQ..)만 변경
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// 생사결 관련 아이템 없이 입장하는 오류 수정
	// --------------┛

	// 2007.07.26 ---┓
	#define _XDEF_4EFF_SYSTEM_070625_KUKURI				// 음양혼신 시스템 : _XDEF_NEWSTRINGMANAGER/_XTS_PK 필요
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// 포션 사용 확장
	#define _XDEF_MODIFY_UI_070710_KUKURI				// 인터페이스 쉽게 수정 : 직책 이름, 성취단계
	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// 몬스터 레벨 표시 
	#define _XDEF_ITEM_V23								// item table v2.3
	#define ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		// 채팅 입력 내용 유지.
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : 양희왕 //breif : 소켓 분리
	// --------------┛

	// 2007.08.30 ---┓
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// 기간제 아이템 창고보관 가능 처리 : 장착물품보기 패킷 변경됨(MSG_CHARAC_MORE_INFO)
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// 캐시 아이템 테두리 그리기 리뉴얼 : cashcheck==1, 2일때
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// 템프 인벤 관련 패킷 변경 
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// 표국이동권 리뉴얼
	#define _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : 양희왕 //breif 무기 사라지는 버그
	// --------------┛

	// 2007.10.04 ---┓
	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window
	// --------------┛

	// 2007.11.29 ---┓
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// 인터페이스 리뉴얼 : Jinstatus창 수정,경험치창 추가
	#define _XDEF_MONSTERREF_15							// 몬스터 테이블 v 1.5
	#define _XDEF_MIXWEAPON_071009_KUKURI				// 무기 조합 시스템	:_XDEF_SMELT_ITEM가 있을때 유효
	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter 변수 확장. 2007.10.10
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// 일반 수집 아이콘 위치 변경	// _XDEF_EVENTWINDOW 필요
	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// 명품 수집 - package 개념 추가
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// 아이템 - 방어력, 공성, 회피성공 값을 기공쪽에 반영
	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : 양희왕 //breif : 미니맵 투명도
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_NEW_SOCKET_071031_KUKURI				// 소켓 장착 제한 추가	: item table v2.4 필요
	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// 음양혼신 공격력 몬스터 이름 표시
	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// 화타신단 오류 수정

	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// 부활창 리뉴얼
	#define _XDEF_MATCH_EVENT_071025_KUKURI				// 영웅 대회
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : 양희왕 //breif : 죽었을때 F12 를 누르지 못하게함 Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : 양희왕 //breif : 기간제 아이템 만료기간 알려줌

	#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// 장원전 스케줄 리뉴얼
	// --------------┛

	// 2008.01.17 ---┓
	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc 사용 제거.
	// --------------┛

	// 2008.02.14 ---┓
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘	
	// --------------┛

	// 2008.02.26 ---┓
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	// --------------┛

	#define _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER //Author : 양희왕 //breif : 수집 조합 가능 메시지 //date : 08/01/17

	// 2008.03.27 ---┓
	#define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI	// 수집 변경
	#define _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI		// 2차 전직 : 오의 페이지 변경
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_MASTERY_TOTALPOINT_080313_KUKURI		// 오의 토탈 포인트 계산 변경 : _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI와 합칠 것
	#define _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI	// 강무기 착용제한 변경
	#define _XDEF_LEVELEVENT_080319_MAGVIPER //Author : 양희왕 //breif : 레벨 달성 이벤트
	// --------------┛

	// 2008.04.03 ---┓
	#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// 수집 인덱스 확장
	// --------------┛

	// 2008.04.24 ---┓
	#define _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI		// 흑백대전 출입 권한 서버에서 주기
	#define ALL_RB_SELECT_CLOTHES_080404_KUKURI			// 의상 교체 시스템 : 패킷 변경 됨(MSG_CHARAC_INFO/MSG_ZONE_CHARAC_INFO/MSG_STATE_CHANGE)
	#define _XDEF_NPCBUFF_CASH_080410_KUKURI			// 무명신침 : 점혈술 침통 아이템
	// --------------┛

	// 2008.05.02 ---┓
	#define _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI		// 이벤트로 경공 주기 (경공 이벤트) -_XDEF_EVENT_RENEWAL_080201_MAGVIPER필요
	#define _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI	// 인챈트 테이블 확장 (id 2개로 늘림)
	// --------------┛

	// 2008.05.29 ---┓
    #define _XDEF_MIXWEAPON_PERIODDELETE_080509_MAGVIPER //Author : 양희왕  //Author : 양희왕 //breif : 국내는 기간을 없애야 함
    #define _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	#define _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장
	#define _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI		// 결과 퀘스트 분리 : MSG_QUEST_AUTH_REQUEST 패킷 변경
	#define _XDEF_ADD_MAX_LEARNSKILL_080522_KUKURI		// 배울 수 있는 무공 수 60->80으로 확장
	// --------------┛


	//#define _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA	// Special BGM looping 처리, Special BGM play 후 원래 BGM 복구.
	
#ifndef _XTESTSERVER // 수련서버는 _XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA로 정의됨.
	#define _XNEXON_WEBLOGIN 
	#define _XDEF_NEXONENDINGADVERTISE_071213_KERYGMA   // Nexon Ending banner, 정식서버만 적용됨.
#endif
#ifndef _XNEXON_WEBLOGIN
	#define _XDEF_NEWNEXONLOGIN						// Nexon login 방식 수정, 빠른 로그인.
#endif

	#define _XDEF_NPROTECT_GUARD					// GameGuard CS 인증
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// 메시지 보낸 시간을 찍어주기위해서 LocalTime 으로 계산

	// 2008.07.31 ---┓
	// 2008.7.16, 보안관련 추가
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA		// 실행파일의 사이즈가 다를 경우 비정상 패킷 정보를 보내 서버가 인지하도록 함.
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA			// 서버에 시간을 측정할 수 있도록 일정간격으로 시간 패킷 보냄.
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA				// 리스트에 등록된 핵툴을 감지하여 서버쪽에 로그로 남김.
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA			// 패킷 암호화시 패턴의 시퀀스를 변경하였음.
	#define _XUSEENCRYPTPATTERN								// Packet 암호화 pattern 처리

	#define _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI		// 다른 캐릭터 선택 기능 추가 - 서버 같이 바뀌어야 함
	#define _XDEF_AVOID_OVERLAPPING_CASHITEM_080721_KUKURI	// 증가 아이템 중복 사용 방지

	#define _XDEF_ALLOW_INCHANTITEM_MIXWEAPON_MAGVIPER_080731 //Author : 양희왕 //breif : 제련 아이템 무기조합 허용

//	#define _XDEF_MATCH_EVENT_PREVENT_ABUSE_080718_KUKURI	// 영웅대회 어뷰징 방지
	// --------------┛

	//#define _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER		//Author : 양희왕 //breif : 사제 시스템

	// -------------------------------------------------------------------------------------------------------------┛
#endif

////////////////////////////////////////////////////////////////
// 공통 - 버그 수정 Rollback용
//
//#define ALL_RB_NPCSCRIPT_DEATH_070611_KUKURI			// NPC 대화 중 죽었을 때 처리
//
////////////////////////////////////////////////////////////////

#ifdef _XRUSSIAN
	/////////
	#define _XTS_NEWLIFEWINDOW						// 생활창 : 명품,자료 수집 인터페이스 변경
	#define _XDEF_NEWVERSONFILETERING				// 필터링 리스트 업그레이드

/* 2006. 11. 14 ---------------------------------------┓ */ 
	#define _XDEF_EXPAND_ZONENAME					/* 해외의 경우 지역이름이 길어서 지역 이름 이미지를 늘여야 한다.*/ 
/* ----------------------------------------------------┛ */ 

	// 2006.12.15 ---------------------------------------┓

	// 캐쉬 아이템 사용 관련  - 해외버전에서 캐쉬 아이템을 사용만 할 경우 아래의 define들만 적용해주면 됨.
	#define _XDEF_CASHITEM_SHOUT					// 외치기 (사자후)
	#define _XTS_MOVEVILLAGE						// 표국 이동
	#define _XDEF_CLOTHMASK_DIVIDE					// 치장의상을 캐쉬 아이템과 일반 아이템으로 나눈다.- icon 처리
	#define _XDEF_PERIODITEM_20061027				// 기간제 아이템 : 기간제 관련 TimeValue 추가
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define _XTS_ITEM_V22							// item table v2.2
	#define _XDEF_MONSTERREF_14						// 몬스터 테이블 1.4

	#define	_XDEF_CHRISTMASEVENT					// 크리스마스 이벤트
	// --------------------------------------------------┛

	
	#define _XTS_SAFETYZONE							// 안전지역 구현
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
//	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone으로 이동할 때 선택 하도록 변경 - pk서버 따로 있는 곳은 활성화하면 안됨
	
	#define _XTS_ALIANCE_20061018					// 영웅단 동맹 
	#define _XDEF_SEARCHPARTYMEMBER					// 동행 찾기 기능
	#define _XDEF_MONEYDROP_NO_20070102				// 금전 버리기 불가능
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// 몬스터 경고 문구 -> 시스템 메시지로 이동
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help log server side save.


	//Author : 양희왕 
	#define _XDEF_NPROTECT_GUARD					// GameGuard CS 인증
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// 메시지 보낸 시간을 찍어주기위해서 LocalTime 으로 계산

	#define _XUSEENCRYPTPATTERN						// Packet 암호화 pattern 처리
	//Last updated: 07/02/2	

	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format 변경

	#define _XDEF_SHOWEVENTNOTICE					// 이벤트 창 팝업 ON/OFF
	#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// 이벤트 창 웹 페이지로 만들기

//	#define _XDEF_CASTLEBATTLE						// 장원전
//	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet 변경


//범용아이템몰 
	#define _XTS_USECASHITEM						// cash item 사용 - 캐쉬 아이템 border와 message 창 border 처리
	#define _XTS_ITEMMALLBROWSER					// 아이템몰 - 아이템 몰 browser와 cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// 범용 아이템몰
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall 관련 category 추가. 

	#define _ACCLAIM_IGAADSYSTEM_DISABLEDUPLICATE_AD_VIEW	// 죽었을때 중복 광고 제거.
	#define _XDEF_VEQUALITYCONTROL							// Visual effect quality control

	#define	US_RB_NEWSPLITTINGNOTICETEXT_070529_KERYGMA	// 흑백대전 텍스트 분리 방식 변경 ( \n 분리 ) : 미국만 적용.

	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// 채팅창 기능 확장.
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// 채팅 리뉴얼 중 (동맹)관련 부분 버그 수정.

	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER			/*!< @author 양희왕 @breif 신구룡전도*/

	#define _XDEF_ESCAPE_20070205						// 탈출 기능 07/09/04
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// 템프 인벤 관련 패킷 변경 

	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// 몬스터 레벨 표시
	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window
	
	// 2007.10.02 -------------------------------┓
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// 포션 사용 확장
	#define _XDEF_ITEM_V23								// item table v2.3
	// ------------------------------------------┛

	// 2007.10.17 -------------------------------┓

	#define _XDEF_CASTLEBATTLE							// 장원전
	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO			// Charac info packet 변경
	#define _XDEF_CASTLEBATTLE_SECONDEDITION			// 장원전 - 2차 업데이트
	#define _XDEF_CASTLEBATTLE_MAPTAXRATE				// 지역 세율 적용
	#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// 장원전 - 장주 이름표
	#define _XDEF_CASTLEBATTLE_RECORD					// 장원전 - 전적 
	#define _XDEF_CASTLEBATTLE_MINIMAP					// 장원전 - 미니맵
	#define _XDEF_CASTLEBATTLE_SCHEDULE_RENEWAL_US_KUKURI	// 장원전 - 스케줄 리뉴얼 (US)

	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author 양희왕 @breif 스킬 딜레이 단축되는 부분 수정*/
	// ------------------------------------------┛

	// 2007.11.06 -------------------------------┓
	#define VN_RB_GAMBLEBOX_070607_KUKURI				// 패왕궤 관련 아이템 복사 오류 수정(MSG_SM_EQUIP_RESULT.. 패킷 변경)
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// 생사결 관련 아이템 없이 입장하는 오류 수정
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// 인던 입장권
	//#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// 인던 입장권 UI 뺀 것 : 인던입장권 없이 item 관련 패킷(MSG_EX_BATTLE_ROOM_REQ..)만 변경, 인던입장권 들어갈 때 삭제해야 함
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// 기간제 아이템 창고보관 가능 처리 : 장착물품보기 패킷 변경됨(MSG_CHARAC_MORE_INFO)
	// ------------------------------------------┛

	// 2007.11.19 -------------------------------┓
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// 표국이동권 리뉴얼
	//#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	//#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author 양희왕 @breif PK 위치추적 리스트 */
	// ------------------------------------------┛

	// 2007.11.26 ---┓ No.38
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA
	// --------------┛

	#define	_XDEF_INPUTPRESS	/*!< @author 양희왕 @breif 초식기 버그 */
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author 양희왕 @breif 초식기 버그 쪼아 매크로 : _XDEF_INPUTPRESS와 같이 적용되어야 함*/
										//Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : 양희왕 //breif : 죽었을때 F12 를 누르지 못하게함 Date : 07/12/03

	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// 화타신단 오류 수정
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author 양희왕 @breif 친구소환*/

	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc 사용 제거.

	// 2008.03.12 ---┓ 
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load 처리:엔진 최적화
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// 캐시 아이템 테두리 그리기 리뉴얼 : cashcheck==1, 2일때
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : 양희왕 //breif : 소켓 분리 // item table 2.3 필요
	// --------------┛

    // 2008.04.28  --┓
    #define _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
    // --------------┛

    #define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// 명품 수집 - package 개념 추가
#endif

// 영어 버젼일 때만 적용하는 곳 
#ifdef _XENGLISH 
/////////
	#define _XTS_NEWLIFEWINDOW						// 생활창 : 명품,자료 수집 인터페이스 변경
	#define _XDEF_NEWVERSONFILETERING				// 필터링 리스트 업그레이드

/* 2006. 11. 14 ---------------------------------------┓ */ 
	#define _XDEF_EXPAND_ZONENAME					/* 해외의 경우 지역이름이 길어서 지역 이름 이미지를 늘여야 한다.*/ 
/* ----------------------------------------------------┛ */ 

	// 2006.12.15 ---------------------------------------┓

	// 캐쉬 아이템 사용 관련  - 해외버전에서 캐쉬 아이템을 사용만 할 경우 아래의 define들만 적용해주면 됨.
	#define _XDEF_CASHITEM_SHOUT					// 외치기 (사자후)
	#define _XTS_MOVEVILLAGE						// 표국 이동
	#define _XDEF_CLOTHMASK_DIVIDE					// 치장의상을 캐쉬 아이템과 일반 아이템으로 나눈다.- icon 처리
	#define _XDEF_PERIODITEM_20061027				// 기간제 아이템 : 기간제 관련 TimeValue 추가
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define _XTS_ITEM_V22							// item table v2.2
	#define _XDEF_MONSTERREF_14						// 몬스터 테이블 1.4

	#define	_XDEF_CHRISTMASEVENT					// 크리스마스 이벤트
	// --------------------------------------------------┛



	// 1월 15일 미국 버젼 No.2 Patch 예정
	#define _XTS_SAFETYZONE							// 안전지역 구현
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
//	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone으로 이동할 때 선택 하도록 변경 - pk서버 따로 있는 곳은 활성화하면 안됨
	
	#define _XTS_ALIANCE_20061018					// 영웅단 동맹 
	#define _XDEF_SEARCHPARTYMEMBER					// 동행 찾기 기능
	#define _XDEF_MONEYDROP_NO_20070102				// 금전 버리기 불가능
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// 몬스터 경고 문구 -> 시스템 메시지로 이동
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help log server side save.


	//Author : 양희왕 
	#define _XDEF_NPROTECT_GUARD					// GameGuard CS 인증
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// 메시지 보낸 시간을 찍어주기위해서 LocalTime 으로 계산

	#define _XUSEENCRYPTPATTERN						// Packet 암호화 pattern 처리
	//Last updated: 07/02/2	

	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format 변경

	#define _XDEF_SHOWEVENTNOTICE					// 이벤트 창 팝업 ON/OFF
	#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// 이벤트 창 웹 페이지로 만들기

//	#define _XDEF_CASTLEBATTLE						// 장원전
//	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet 변경


//범용아이템몰 
	#define _XTS_USECASHITEM						// cash item 사용 - 캐쉬 아이템 border와 message 창 border 처리
	#define _XTS_ITEMMALLBROWSER					// 아이템몰 - 아이템 몰 browser와 cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// 범용 아이템몰
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall 관련 category 추가. 

	#define _ACCLAIM_IGAADSYSTEM_DISABLEDUPLICATE_AD_VIEW	// 죽었을때 중복 광고 제거.
	#define _XDEF_VEQUALITYCONTROL							// Visual effect quality control

	#define	US_RB_NEWSPLITTINGNOTICETEXT_070529_KERYGMA	// 흑백대전 텍스트 분리 방식 변경 ( \n 분리 ) : 미국만 적용.

	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// 채팅창 기능 확장.
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// 채팅 리뉴얼 중 (동맹)관련 부분 버그 수정.

	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER			/*!< @author 양희왕 @breif 신구룡전도*/

	#define _XDEF_ESCAPE_20070205						// 탈출 기능 07/09/04
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// 템프 인벤 관련 패킷 변경 

	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// 몬스터 레벨 표시
	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window
	
	// 2007.10.02 -------------------------------┓
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// 포션 사용 확장
	#define _XDEF_ITEM_V23								// item table v2.3
	// ------------------------------------------┛

	// 2007.10.17 -------------------------------┓

	#define _XDEF_CASTLEBATTLE							// 장원전
	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO			// Charac info packet 변경
	#define _XDEF_CASTLEBATTLE_SECONDEDITION			// 장원전 - 2차 업데이트
	#define _XDEF_CASTLEBATTLE_MAPTAXRATE				// 지역 세율 적용
	#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// 장원전 - 장주 이름표
	#define _XDEF_CASTLEBATTLE_RECORD					// 장원전 - 전적 
	#define _XDEF_CASTLEBATTLE_MINIMAP					// 장원전 - 미니맵
	#define _XDEF_CASTLEBATTLE_SCHEDULE_RENEWAL_US_KUKURI	// 장원전 - 스케줄 리뉴얼 (US)

	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author 양희왕 @breif 스킬 딜레이 단축되는 부분 수정*/
	// ------------------------------------------┛

	// 2007.11.06 -------------------------------┓
	#define VN_RB_GAMBLEBOX_070607_KUKURI				// 패왕궤 관련 아이템 복사 오류 수정(MSG_SM_EQUIP_RESULT.. 패킷 변경)
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// 생사결 관련 아이템 없이 입장하는 오류 수정
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// 인던 입장권
	//#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// 인던 입장권 UI 뺀 것 : 인던입장권 없이 item 관련 패킷(MSG_EX_BATTLE_ROOM_REQ..)만 변경, 인던입장권 들어갈 때 삭제해야 함
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// 기간제 아이템 창고보관 가능 처리 : 장착물품보기 패킷 변경됨(MSG_CHARAC_MORE_INFO)
	// ------------------------------------------┛

	// 2007.11.19 -------------------------------┓
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// 표국이동권 리뉴얼
	//#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	//#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author 양희왕 @breif PK 위치추적 리스트 */
	// ------------------------------------------┛

	// 2007.11.26 ---┓ No.38
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA
	// --------------┛

	#define	_XDEF_INPUTPRESS	/*!< @author 양희왕 @breif 초식기 버그 */
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author 양희왕 @breif 초식기 버그 쪼아 매크로 : _XDEF_INPUTPRESS와 같이 적용되어야 함*/
										//Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : 양희왕 //breif : 죽었을때 F12 를 누르지 못하게함 Date : 07/12/03

	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// 화타신단 오류 수정
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author 양희왕 @breif 친구소환*/

	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc 사용 제거.

	// 2008.03.12 ---┓ 
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load 처리:엔진 최적화
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// 캐시 아이템 테두리 그리기 리뉴얼 : cashcheck==1, 2일때
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : 양희왕 //breif : 소켓 분리 // item table 2.3 필요
	#define _XDEF_ITEM_V24								// item table v2.4
	// --------------┛

	// 2008.04.17 ---┓
	#define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI		// 수집 변경 
	#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// 수집 인덱스 확장
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘(MSG_EVENT_RESULT 패킷 변경)
	#define _XDEF_LEVELEVENT_080319_MAGVIPER //Author : 양희왕 //breif : 레벨업 이벤트	
	#define _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI		// 이벤트로 경공 주기 (친구 추천 이벤트) -_XDEF_EVENT_RENEWAL_080201_MAGVIPER필요
	// --------------┛

    // 2008.04.28  --┓
    #define _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
    // --------------┛

	// 2008.05.16  --┓
	#define _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI	// 인챈트 테이블 확장 (id 2개로 늘림) - 릴레이 이벤트
	// --------------┛

    #define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// 명품 수집 - package 개념 추가

	// 2008.07.02  --┓
	#define _XDEF_WASTEBASKET_20070226				// 휴지통 기능 추가
	// --------------┛
	
	// 2008.07.29  --┓
	#define _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI		// 결과 퀘스트 분리 : MSG_QUEST_AUTH_REQUEST 패킷 변경
	// --------------┛

	// 2008.08.08  --┓
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	#define _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	#define _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장
	#define _XDEF_SMELT_ITEM //< @author 양희왕 @breif 용루/용린 제련 아이템 
	#define _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI		// 2차 전직 : 오의 페이지 변경
	#define _XDEF_MASTERY_TOTALPOINT_080313_KUKURI		// 오의 토탈 포인트 계산 변경 : _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI와 합칠 것

	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// 부활창 리뉴얼
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// 아이템 - 방어력, 공성, 회피성공 값을 기공쪽에 반영
	#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// 장원전 스케줄 리뉴얼
	// --------------┛

	// 2008.08.22  --┓
	#define _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI		// 다른 캐릭터 선택 기능 추가 - 서버 같이 바뀌어야 함
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// 일반 수집 아이콘 위치 변경	// _XDEF_EVENTWINDOW 필요
	// --------------┛

	// 2008.09.23  --┓
	#define _XDEF_SAME_CHAT_MESSAGE_080923_KUKURI		// 같은 채팅 메시지 보내기 금지
//	#define _ACCLAIM_IGAADSYSTEM		// test
	// --------------┛

#endif


// 베트남 버젼일 때만 적용하는 곳 
#ifdef _XVIETNAMESE
	//-----베트남 전용-----//
	#define _XDEF_SHUTDOWN_20061027					// 베트남 셧다운제도 관련 
	#define _XDEF_VINA_PCROOM_INFO						// 베트남 pc방 : Charac_List 패킷 변경
	#define _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI		// SDS on/off system
	#define _XDEF_TREADE_MESSAGE_ADDITION_070809_MAGVIPER /*!< @author 양희왕 @breif 교환 동의 후 아이템거래시에 메시지 출력*/

	//-----범용 아이템몰-------//
	#define _XTS_USECASHITEM						// cash item 사용 - 캐쉬 아이템 border와 message 창 border 처리
	#define _XTS_ITEMMALLBROWSER					// 아이템몰 - 아이템 몰 browser와 cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// 범용 아이템몰
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall 관련 category 추가. 

	//-----보안 및 내부기능 관련-------//
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load 처리:엔진 최적화
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// 템프 인벤 관련 패킷 변경 	
	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format 변경
	#define _XUSEENCRYPTPATTERN						// Packet 암호화 pattern 처리
	#define _XDEF_NPROTECT_GUARD					// GameGuard CS 인증
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help log server side save.
	#define _XDEF_VEQUALITYCONTROL					// Visual effect quality control
	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc 사용 제거.
	
	//-----아이템 테이블-------//
	#define _XTS_ITEM_V22							// item table v2.2
	#define _XDEF_ITEM_V23								// item table v2.3
	#define _XDEF_MONSTERREF_14						// 몬스터 테이블 1.4

	//-----버그 관련-------//
	#define _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : 양희왕 //breif 무기 사라지는 버그
	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author 양희왕 @breif 스킬 딜레이 단축되는 부분 수정*/
	#define	_XDEF_INPUTPRESS	/*!< @author 양희왕 @breif 초식기 버그 */
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author 양희왕 @breif 초식기 버그 쪼아 매크로 : _XDEF_INPUTPRESS와 같이 적용되어야 함*/
										//Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// 생사결 관련 아이템 없이 입장하는 오류 수정
	#define VN_RB_GAMBLEBOX_070607_KUKURI			// 패왕궤 관련 아이템 복사 오류 수정 (MSG_SM_EQUIP_RESULT.. 패킷 변경)

	//-----게임 기능------//
	#define	_XDEF_CHRISTMASEVENT					// 크리스마스 이벤트
	#define _XDEF_EXPAND_ZONENAME					/* 해외의 경우 지역이름이 길어서 지역 이름 이미지를 늘여야 한다.*/ 
	#define _XTS_NEWLIFEWINDOW						// 생활창 : 명품,자료 수집 인터페이스 변경
	#define _XDEF_NEWVERSONFILETERING				// 필터링 리스트 업그레이드
	#define _XTS_SAFETYZONE							// 안전지역 구현
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone으로 이동할 때 선택 하도록 변경 - pk서버 따로 있는 곳은 활성화하면 안됨
	#define _XTS_ALIANCE_20061018					// 영웅단 동맹 
	#define _XDEF_SEARCHPARTYMEMBER					// 동행 찾기 기능
	#define _XDEF_MONEYDROP_NO_20070102				// 금전 버리기 불가능
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// 몬스터 경고 문구 -> 시스템 메시지로 이동
	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window
//	#define _XDEF_NOTICE_STRING_PROCESS				// 게임 내 공지사항 클래스 추가 Author : 양희왕 Last updated: 07/01/04
	#define _XDEF_PARTY_NOTAVAILABLE				// 자리비움 & 동행 강화
	#define _XDEF_WASTEBASKET_20070226				// 휴지통 기능 추가
	#define _XTS_MASTERY_04							// masterytable.bms ver 0.4
	#define _XTS_MASTERYOPTIONAL					// 오의 변경 - 확장
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// 포션 사용 확장
	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// 몬스터 레벨 표시 
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// 기간제 아이템 창고보관 가능 처리 : 장착물품보기 패킷 변경됨(MSG_CHARAC_MORE_INFO)
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// 캐시 아이템 테두리 그리기 리뉴얼 : cashcheck==1, 2일때
	#define _XDEF_NPC_ATTACK_CHECK					// 경비/명숙 Shift key 사용하도록 처리
	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER	/*!< @author 양희왕 @breif 신구룡전도 07_08_28*/
	#define ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA // 인사시 아이템 삭제.
	#define _XDEF_SHOWEVENTNOTICE					// 이벤트 창 팝업 ON/OFF
	#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// 이벤트 창 웹 페이지로 만들기

	//-----음양혼신-----//
	#define	_XDEF_RESIST_DISPLAY					// 음양혼신 저항력 표기
	#define _XDEF_4EFF_SYSTEM_070625_KUKURI				// 음양혼신 시스템 : _XDEF_NEWSTRINGMANAGER/_XTS_PK 필요
	
	//-----유료 아이템 관련------//
	#define _XDEF_CASHITEM_SHOUT					// 외치기 (사자후)
	#define _XTS_MOVEVILLAGE						// 표국 이동
	#define _XDEF_CLOTHMASK_DIVIDE					// 치장의상을 캐쉬 아이템과 일반 아이템으로 나눈다.- icon 처리
	#define _XDEF_PERIODITEM_20061027				// 기간제 아이템 : 기간제 관련 TimeValue 추가
	#define _XDEF_SMELT_ITEM /*!< @author 양희왕 @breif 용루/용린 제련 아이템 */
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author 양희왕 @breif 친구소환*/
	#define _XDEF_ESCAPE_20070205					// 탈출 기능 
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : 양희왕 //breif : 소켓 분리 // item table 2.3 필요
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// 표국이동권 리뉴얼
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// 인던 입장권
//	#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// 인던 입장권 UI 뺀 것(국내 적용x) : 인던입장권 없이 item 관련 패킷(MSG_EX_BATTLE_ROOM_REQ..)만 변경
	
	//-----PK 관련-----//
	#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author 양희왕 @breif PK 위치추적 리스트 */

	//-----채팅 관련------//
	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// 채팅창 기능 확장.
	#define _XDEF_ZONECHATTING_20070108					// 맵 채팅 (지역 대화)
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// 채팅 리뉴얼 중 (동맹)관련 부분 버그 수정.
	#define ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		// 채팅 입력 내용 유지

	//-----장원전 관련------//
	#define _XDEF_CASTLEBATTLE						// 장원전
	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet 변경
	#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// 장원전 - 장주 이름표
	#define _XDEF_CASTLEBATTLE_RECORD					// 장원전 - 전적 
	#define _XDEF_CASTLEBATTLE_MINIMAP					// 장원전 - 미니맵
	#define _XDEF_CASTLEBATTLE_SECONDEDITION		// 장원전 - 2차 업데이트
	#define _XDEF_CASTLEBATTLE_MAPTAXRATE			// 지역 세율 적용
	//#define _XDEF_ALIANCE_WEBBOARD_20070130	// 장원전 - 영웅단,영웅회 게시판 바로가기 

	//#define _XDEF_WAREHOSE_RESTRAINT_OF_TRADE	/*!< @author 양희왕 @breif 특정 아이템 창고 거래 제한*/
	//#define KR_RB_DELAYSHOWNEXONNOTICEWINDOW_070530_KERYGMA	// 15세 이용가 관련 Delay.

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓	
	//┃ 리뉴얼 관련 , 리뉴얼 관련 신규 추가																			┃
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	
	//-----리뉴얼 중 정식에 들어감------//
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	#define _XDEF_MATCH_EVENT_071025_KUKURI				// 영웅 대회

	// 2008.08.12  --┓
	#define _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI		// 2차 전직 : 오의 페이지 변경
	#define _XDEF_MASTERY_TOTALPOINT_080313_KUKURI		// 오의 토탈 포인트 계산 변경 : _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI와 합칠 것
//	#define _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI	// 강무기 착용제한 변경

	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// 화타신단 오류 수정
	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// 부활창 리뉴얼
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// 아이템 - 방어력, 공성, 회피성공 값을 기공쪽에 반영
//	#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// 장원전 스케줄 리뉴얼
	// --------------┛

	//#define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI	// 수집 변경
	//#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// 수집 인덱스 확장

	// 2008.09.18  --┓
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// 인터페이스 리뉴얼 : Jinstatus창 수정,경험치창 추가
	// --------------┛

	//-----리뉴얼 관련 ------//
//	#define _XDEF_MONSTERREF_15							// 몬스터 테이블 v 1.5
//	#define _XDEF_MIXWEAPON_071009_KUKURI				// 무기 조합 시스템	:_XDEF_SMELT_ITEM가 있을때 유효
//	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter 변수 확장. 2007.10.10
//	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// 일반 수집 아이콘 위치 변경	// _XDEF_EVENTWINDOW 필요
//	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// 명품 수집 - package 개념 추가
//	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : 양희왕 //breif : 미니맵 투명도
//	#define _XDEF_NEW_SOCKET_071031_KUKURI				// 소켓 장착 제한 추가	: item table v2.4 필요
//	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// 음양혼신 공격력 몬스터 이름 표시
	
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : 양희왕 //breif : 죽었을때 F12 를 누르지 못하게함 Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : 양희왕 //breif : 기간제 아이템 만료기간 알려줌

	//-----게임 기능-----//
//	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘
//	#define _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER //Author : 양희왕 //breif : 수집 조합 가능 메시지 //date : 08/01/17

	//-----리뉴얼 관련 끝------//

	#define _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI			// 흑백대전 출입 권한 서버에서 주기
    #define _XDEF_WEBBOARD_20061211							// 웹 게시판 기능	

    // 2008.04.28  --┓
    #define _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
    // --------------┛

	// 2008.08.26  --┓
	#define _XDEF_MASKSYSTEM_SHOW_NAME						// 복면을 써도 이름은 보이도록 - 베트남 요청 사항
	#define _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI		// 다른 캐릭터 선택 기능 추가 - 서버 같이 바뀌어야 함
	// --------------┛

#endif



// 대만 버젼일 때만 적용하는 곳 
#ifdef _XTAIWANESE
	#define _XTS_ITEM_V22				// item table v2.2 - 11월 15일 적용(상용화 팩)
	#define _XDEF_MONSTERREF_14			// 몬스터 테이블 1.4 - 11월 15일 적용(상용화 팩)

	// 캐쉬 아이템 사용 관련  - 해외버전에서 캐쉬 아이템을 사용만 할 경우 아래의 define들만 적용해주면 됨.
	#define _XDEF_CASHITEM_SHOUT					// 외치기 (사자후)
	#define _XTS_MOVEVILLAGE						// 표국 이동
	#define _XDEF_CLOTHMASK_DIVIDE					// 치장의상을 캐쉬 아이템과 일반 아이템으로 나눈다.- icon 처리
	#define _XDEF_PERIODITEM_20061027				// 기간제 아이템 : 기간제 관련 TimeValue 추가
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 2006.12.15 ---------------------------------------┓
	//#define	_XDEF_CHRISTMASEVENT					// 크리스마스 이벤트
	// --------------------------------------------------┛
	
	// 대만 No.4 Patch 포함사항 2007년 1월 22일 업데이트 사항에 포함////////////////////////////////////////////////////////////////////
	#define _XDEF_KEYCHANGE_20070115				// 대만만 적용(테스트는 국내에서 하고 적용은 대만임) : 키타입 변경 
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// 몬스터 경고 문구 -> 시스템 메시지로 이동

	//Author : 양희왕 Last updated: 07/01/18
	//#define _XDEF_NPROTECT_GUARD					// GameGuard CS 인증 : 대만 사용기간 만료.
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// 메시지 보낸 시간을 찍어주기위해서 LocalTime 으로 계산
	#define _XUSEENCRYPTPATTERN						// Packet 암호화 pattern 처리

	#define _XDEF_ZONECHATTING_20070108				// 대만만 적용(테스트는 국내에서 하고 적용은 대만임) : 지역 대화 가능 

	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format 변경

	// 2월 28일 추가 사항
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help system server side save.

	#define _XDEF_SEARCHPARTYMEMBER					// 동행 찾기 기능

	#define _XTS_REFUSE_ORGANIZATION				// 영웅단 초대 거부

	#define _XTS_SAFETYZONE							// 안전지역 구현
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone으로 이동할 때 선택 하도록 변경
	//Last updated: 07/02/28

	#define _XDEF_MONEYDROP_NO_20070102				// // 금전 버리기 불가능 -> 이주임님 요청사항.

	// 2007/03/20 방회 추가
	#define _XTS_ALIANCE_20061018					// 영웅단 동맹 

	// 2007/04/04 초식기 버그 수정 - 사신창류 버그
//	#define	_XDEF_INPUTPRESS /*!< @author 양희왕 @breif 초식기 버그 */
	#define	_XDEF_SKILLDEALY_MODIFY /*!< @author 양희왕 @breif 스킬 딜레이 단축되는 부분 수정*/
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load 처리. 

	// 2007/04/23
	#define _XDEF_PARTY_NOTAVAILABLE				// 자리비움 & 동행 강화

	// 2007.07.27 ---┓
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// 인던 입장권
	#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// 인던 입장권 UI 뺀 것(국내 적용x) : 인던입장권 없이 item 관련 패킷(MSG_EX_BATTLE_ROOM_REQ..)만 변경
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// 생사결 관련 아이템 없이 입장하는 오류 수정
	// --------------┛
	
	// 2007.12.05 ---┓ No.38
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA

	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// 템프 인벤 관련 패킷 변경 
	// --------------┛

	// 2008.01.21  ---┓  //패치 예정
	//범용아이템몰 
	#define _XTS_USECASHITEM						// cash item 사용 - 캐쉬 아이템 border와 message 창 border 처리
	#define _XTS_ITEMMALLBROWSER					// 아이템몰 - 아이템 몰 browser와 cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// 범용 아이템몰
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall 관련 category 추가. 

	#define _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : 양희왕 //breif 무기 사라지는 버그
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author 양희왕 @breif 초식기 버그 쪼아 매크로 : _XDEF_INPUTPRESS와 같이 적용되어야 함*/
										//Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03

	#define _XDEF_ESCAPE_20070205					// 탈출 기능 

	#define _XTS_MASTERY_04							// masterytable.bms ver 0.4
	#define _XTS_MASTERYOPTIONAL					// 오의 변경 - 확장
	#define	_XDEF_RESIST_DISPLAY					// 음양혼신 저항력 표기
	
	#define _XDEF_4EFF_SYSTEM_070625_KUKURI				// 음양혼신 시스템 : _XDEF_NEWSTRINGMANAGER/_XTS_PK 필요
	
	#define _XDEF_WASTEBASKET_20070226				// 휴지통 기능 추가

	#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author 양희왕 @breif PK 위치추적 리스트 */

	#define _XDEF_NPC_ATTACK_CHECK	/*!< @author 양희왕 @breif 무림명숙 공격시 쉬프트 클릭*/

	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// 채팅창 기능 확장.
	#define _XDEF_ZONECHATTING_20070108					// 맵 채팅 (지역 대화)
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// 채팅 리뉴얼 중 (동맹)관련 부분 버그 수정.
	#define ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		// 채팅 입력 내용 유지.

	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER	/*!< @author 양희왕 @breif 신구룡전도*/
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author 양희왕 @breif 친구소환*/

	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// 포션 사용 확장

	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// 몬스터 레벨 표시 

	#define _XDEF_ITEM_V23								// item table v2.3
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : 양희왕 //breif : 소켓 분리 // item table 2.3 필요

	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// 표국이동권 리뉴얼

	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// 기간제 아이템 창고보관 가능 처리 : 장착물품보기 패킷 변경됨(MSG_CHARAC_MORE_INFO)
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// 캐시 아이템 테두리 그리기 리뉴얼 : cashcheck==1, 2일때
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// 템프 인벤 관련 패킷 변경 

	//유료 아이템 관련
	//#define _XDEF_SMELT_ITEM /*!< @author 양희왕 @breif 용루/용린 제련 아이템 */

	// --------------┛

	// 2008.01.21  ---┓ // 최신 리뉴얼
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// 인터페이스 리뉴얼 : Jinstatus창 수정,경험치창 추가
	#define _XDEF_MONSTERREF_15							// 몬스터 테이블 v 1.5
	#define _XDEF_MIXWEAPON_071009_KUKURI				// 무기 조합 시스템	:_XDEF_SMELT_ITEM가 있을때 유효
	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter 변수 확장. 2007.10.10

	// 대만 버전에서는 북해빙궁 아이콘 안보이게 소스 추가해놓았음. 북해 추가될 때 소스 수정해야 함 (XWindow_LifeSkill.cpp)
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// 일반 수집 아이콘 위치 변경	// _XDEF_EVENTWINDOW 필요

	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// 명품 수집 - package 개념 추가
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// 아이템 - 방어력, 공성, 회피성공 값을 기공쪽에 반영
	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : 양희왕 //breif : 미니맵 투명도
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_NEW_SOCKET_071031_KUKURI				// 소켓 장착 제한 추가	: item table v2.4 필요
	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// 음양혼신 공격력 몬스터 이름 표시
	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// 화타신단 오류 수정

	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// 부활창 리뉴얼
	#define _XDEF_MATCH_EVENT_071025_KUKURI				// 영웅 대회
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : 양희왕 //breif : 죽었을때 F12 를 누르지 못하게함 Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : 양희왕 //breif : 기간제 아이템 만료기간 알려줌

	//#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI	// 장원전 스케줄 리뉴얼
	// --------------┛

	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc 사용 제거.

	// 2008.08.12  --┓
	#define _XTS_NEWSKILLWINDOW						// 스킬 창 변경
	// --------------┛


	// 2008.08.20  --┓
	#define _XDEF_ALLOW_INCHANTITEM_MIXWEAPON_MAGVIPER_080731 //Author : 양희왕 //breif : 제련 아이템 무기조합 허용
	#define _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER //Author : 양희왕 //breif : 수집 조합 가능 메시지 //date : 08/01/17
	#define _XDEF_MIXWEAPON_PERIODDELETE_080509_MAGVIPER //Author : 양희왕  //Author : 양희왕 //breif : 무기조합 기간을 없애야 함
	// --------------┛

	// 2008.08.22  --┓
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘(MSG_EVENT_RESULT 패킷 변경)
	#define _XDEF_LEVELEVENT_080319_MAGVIPER //Author : 양희왕 //breif : 레벨업 이벤트	
	// --------------┛

	// 2008.09.10  --┓
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI	// 인챈트 테이블 확장 (id 2개로 늘림) - 릴레이 이벤트
	// --------------┛

#endif

// 일본 버젼일 때만 적용하는 곳 
#ifdef _XJAPANESE
	
//	#define _XTS_NEWLIFEWINDOW						// 생활창 : 명품,자료 수집 인터페이스 변경
	#define _XDEF_NEWVERSONFILETERING				// 필터링 리스트 업그레이드

/* 2006. 11. 14 ---------------------------------------┓ */ 
	#define _XDEF_EXPAND_ZONENAME					/* 해외의 경우 지역이름이 길어서 지역 이름 이미지를 늘여야 한다.*/ 
/* ----------------------------------------------------┛ */ 

	// 2006.12.15 ---------------------------------------┓

	// 캐쉬 아이템 사용 관련  - 해외버전에서 캐쉬 아이템을 사용만 할 경우 아래의 define들만 적용해주면 됨.
	#define _XDEF_CASHITEM_SHOUT					// 외치기 (사자후)
	#define _XTS_MOVEVILLAGE						// 표국 이동
	#define _XDEF_CLOTHMASK_DIVIDE					// 치장의상을 캐쉬 아이템과 일반 아이템으로 나눈다.- icon 처리
	#define _XDEF_PERIODITEM_20061027				// 기간제 아이템 : 기간제 관련 TimeValue 추가
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define _XTS_ITEM_V22							// item table v2.2
	#define _XDEF_MONSTERREF_14						// 몬스터 테이블 1.4

	#define	_XDEF_CHRISTMASEVENT					// 크리스마스 이벤트
	// --------------------------------------------------┛

	// 1월 15일 미국 버젼 No.2 Patch 예정
	#define _XTS_SAFETYZONE							// 안전지역 구현
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
//	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone으로 이동할 때 선택 하도록 변경 - pk서버 따로 있는 곳은 활성화하면 안됨
	
	#define _XTS_ALIANCE_20061018					// 영웅단 동맹 
	#define _XDEF_SEARCHPARTYMEMBER					// 동행 찾기 기능
	#define _XDEF_MONEYDROP_NO_20070102				// 금전 버리기 불가능
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// 몬스터 경고 문구 -> 시스템 메시지로 이동
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help log server side save.

	#define _XUSEENCRYPTPATTERN						// Packet 암호화 pattern 처리
	//Last updated: 07/02/2	
	
	#define _XDEF_NPROTECT_GUARD					// GameGuard CS 인증
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// 메시지 보낸 시간을 찍어주기위해서 LocalTime 으로 계산

	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format 변경

//범용아이템몰 
	#define _XTS_USECASHITEM						// cash item 사용 - 캐쉬 아이템 border와 message 창 border 처리
	#define _XTS_ITEMMALLBROWSER					// 아이템몰 - 아이템 몰 browser와 cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// 범용 아이템몰
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall 관련 category 추가. 

	#define ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA // 인사시 아이템 삭제.

	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER	/*!< @author 양희왕 @breif 신구룡전도 07_08_28*/
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// 템프 인벤 관련 패킷 변경 	

	// 2008.01.15    ---┓  //패치 예정
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA

	#define _XDEF_ITEM_V23								// item table v2.3

	#define _XDEF_MODELTHREADLOADING				// Character model multithread load 처리:엔진 최적화
	#define VN_RB_GAMBLEBOX_070607_KUKURI			// 패왕궤 관련 아이템 복사 오류 수정 (MSG_SM_EQUIP_RESULT.. 패킷 변경)

	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window

	//-----버그 관련-------//
	#define _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : 양희왕 //breif 무기 사라지는 버그
	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author 양희왕 @breif 스킬 딜레이 단축되는 부분 수정*/
	#define	_XDEF_INPUTPRESS	/*!< @author 양희왕 @breif 초식기 버그 */
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author 양희왕 @breif 초식기 버그 쪼아 매크로 : _XDEF_INPUTPRESS와 같이 적용되어야 함*/
										//Author : 양희왕 //breif : 한대치고 에니메이션이 끊기는 경우 //date 07/12/03
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// 생사결 관련 아이템 없이 입장하는 오류 수정

	//-----기타 기능------//
	#define _XDEF_NOTICE_STRING_PROCESS				// 게임 내 공지사항 클래스 추가 Author : 양희왕 Last updated: 07/01/04
	#define _XDEF_PARTY_NOTAVAILABLE				// 자리비움 & 동행 강화
	#define _XDEF_WASTEBASKET_20070226				// 휴지통 기능 추가
	#define _XTS_MASTERY_04							// masterytable.bms ver 0.4
	#define _XTS_MASTERYOPTIONAL					// 오의 변경 - 확장
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// 포션 사용 확장
	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// 몬스터 레벨 표시 
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// 기간제 아이템 창고보관 가능 처리 : 장착물품보기 패킷 변경됨(MSG_CHARAC_MORE_INFO)
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// 캐시 아이템 테두리 그리기 리뉴얼 : cashcheck==1, 2일때

	//-----음양혼신-----//
	#define	_XDEF_RESIST_DISPLAY					// 음양혼신 저항력 표기
	#define _XDEF_4EFF_SYSTEM_070625_KUKURI				// 음양혼신 시스템 : _XDEF_NEWSTRINGMANAGER/_XTS_PK 필요
	
	//-----유료 아이템 관련------//
	#define _XDEF_SMELT_ITEM /*!< @author 양희왕 @breif 용루/용린 제련 아이템 */
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author 양희왕 @breif 친구소환*/
	#define _XDEF_ESCAPE_20070205					// 탈출 기능 
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : 양희왕 //breif : 소켓 분리 // item table 2.3 필요
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// 표국이동권 리뉴얼
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// 인던 입장권
//	#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// 인던 입장권 UI 뺀 것(국내 적용x) : 인던입장권 없이 item 관련 패킷(MSG_EX_BATTLE_ROOM_REQ..)만 변경
	
	//-----PK 관련-----//
	#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author 양희왕 @breif PK 위치추적 리스트 */

	//-----채팅 관련------//
	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// 채팅창 기능 확장.
	#define _XDEF_ZONECHATTING_20070108					// 맵 채팅 (지역 대화)
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// 채팅 리뉴얼 중 (동맹)관련 부분 버그 수정.
	#define ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		// 채팅 입력 내용 유지
	// --------------┛

	// 2008.01.15  ---┓ // 최신 리뉴얼
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// 인터페이스 리뉴얼 : Jinstatus창 수정,경험치창 추가
	#define _XDEF_MONSTERREF_15							// 몬스터 테이블 v 1.5
	#define _XDEF_MIXWEAPON_071009_KUKURI				// 무기 조합 시스템	:_XDEF_SMELT_ITEM가 있을때 유효
	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter 변수 확장. 2007.10.10
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// 일반 수집 아이콘 위치 변경	// _XDEF_EVENTWINDOW 필요
	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// 명품 수집 - package 개념 추가
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// 아이템 - 방어력, 공성, 회피성공 값을 기공쪽에 반영
	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : 양희왕 //breif : 미니맵 투명도
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_NEW_SOCKET_071031_KUKURI				// 소켓 장착 제한 추가	: item table v2.4 필요
	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// 음양혼신 공격력 몬스터 이름 표시
	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// 화타신단 오류 수정

	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// 부활창 리뉴얼
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : 양희왕 //breif : 죽었을때 F12 를 누르지 못하게함 Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : 양희왕 //breif : 기간제 아이템 만료기간 알려줌

	//#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// 장원전 스케줄 리뉴얼
	// --------------┛

	// 2008.02.05 ---┓
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘	
	// --------------┛


	//패치 예정?
	//#define _XDEF_MATCH_EVENT_071025_KUKURI			// 영웅 대회

	//----장원전----
	//#define _XDEF_CASTLEBATTLE						// 장원전
	//#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet 변경
	//#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// 장원전 - 장주 이름표
	//#define _XDEF_CASTLEBATTLE_RECORD					// 장원전 - 전적 
	//#define _XDEF_CASTLEBATTLE_MINIMAP					// 장원전 - 미니맵
	//#define _XDEF_ALIANCE_WEBBOARD_20070130	// 장원전 - 영웅단,영웅회 게시판 바로가기 
	//#define _XDEF_CASTLEBATTLE_SECONDEDITION		// 장원전 - 2차 업데이트
	//#define _XDEF_CASTLEBATTLE_MAPTAXRATE			// 지역 세율 적용
	
	//----기능----
	//#define _XDEF_WAREHOSE_RESTRAINT_OF_TRADE	/*!< @author 양희왕 @breif 특정 아이템 창고 거래 제한*/
	//#define _XDEF_NPC_ATTACK_CHECK	/*!< @author 양희왕 @breif 무림명숙 공격시 쉬프트 클릭*/
	//#define _XDEF_SHOWEVENTNOTICE					// 이벤트 창 팝업 ON/OFF
	//#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// 이벤트 창 웹 페이지로 만들기

	//#define KR_RB_DELAYSHOWNEXONNOTICEWINDOW_070530_KERYGMA	// 15세 이용가 관련 Delay.

	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA

	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc 사용 제거.

	// 2008.03.20 ---┓
	#define _XDEF_NPC_ATTACK_CHECK					// 경비/명숙 Shift key 사용하도록 처리
	// --------------┛

    // 2008.04.28  --┓
    #define _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : 양희왕 //breif : 데미지 체팅 메시지 찍기 (대만은 안한다고 함 )
    // --------------┛

    // 2008.05.08  --┓
    #define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI		// 수집 변경 
	#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// 수집 인덱스 확장
    // --------------┛

    // 2008.06.10  --┓
    #define _XDEF_MIXWEAPON_PERIODDELETE_080509_MAGVIPER //Author : 양희왕  //Author : 양희왕 //breif : 무기조합 기간을 없애야 함
    // --------------┛

	// 2008.08.12  --┓
	#define _XTS_NEWSKILLWINDOW						// 스킬 창 변경
	#define _XDEF_ALLOW_INCHANTITEM_MIXWEAPON_MAGVIPER_080731 //Author : 양희왕 //breif : 제련 아이템 무기조합 허용
	// --------------┛

	// 2008.08.14  --┓
	#define _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI		// 다른 캐릭터 선택 기능 추가 - 서버 같이 바뀌어야 함
	#define ALL_RB_SELECT_CLOTHES_080404_KUKURI			// 의상 교체 시스템 : 패킷 변경 됨(MSG_CHARAC_INFO/MSG_ZONE_CHARAC_INFO/MSG_STATE_CHANGE)
	// --------------┛

	// 2008.08.28  --┓
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	// --------------┛

	// 2008.09.26  --┓
	#define _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	#define _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장
	#define _XDEF_WEBBOARD_20061211							// 웹 게시판 기능	
	// --------------┛

#endif


////////////////////////////////////////////////////////////////////////////////////
//수련서버 시작 ------------------------------------------------------------------┓

#ifdef _XTESTSERVER 

	// 2007.10.08 ---┓
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// 인터페이스 리뉴얼 : Jinstatus창 수정,경험치창 추가
	#define _XDEF_MONSTERREF_15							// 몬스터 테이블 v 1.5
	#define _XDEF_MIXWEAPON_071009_KUKURI				// 무기 조합 시스템	:_XDEF_SMELT_ITEM가 있을때 유효
	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter 변수 확장. 2007.10.10

	#undef _XDEF_WEBBOARD_20061211						// 웹 게시판 기능 삭제
	#undef _XDEF_ALIANCE_WEBBOARD_20070130					//영웅단,영웅회 게시판 바로가기  기능 삭제
	// --------------┛

	// 2007.10.18 ---┓
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// 일반 수집 아이콘 위치 변경	// _XDEF_EVENTWINDOW 필요
	// --------------┛

	// 2007.10.29 ---┓
	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// 명품 수집 - package 개념 추가
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// 아이템 - 방어력, 공성, 회피성공 값을 기공쪽에 반영
	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : 양희왕 //breif : 미니맵 투명도
	// --------------┛

	// 2007.11.05 ---┓
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_NEW_SOCKET_071031_KUKURI				// 소켓 장착 제한 추가	: item table v2.4 필요
	// --------------┛

	// 2007.11.05 ---┓
	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// 음양혼신 공격력 몬스터 이름 표시
	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// 화타신단 오류 수정
	// --------------┛

	// 2007.12.07 ---┓
	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// 부활창 리뉴얼
	#define _XDEF_MATCH_EVENT_071025_KUKURI				// 영웅 대회
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : 양희왕 //breif : 죽었을때 F12 를 누르지 못하게함 Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : 양희왕 //breif : 기간제 아이템 만료기간 알려줌
	// --------------┛

	// 2007.12.17 ---┓
	#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// 장원전 스케줄 리뉴얼
	// --------------┛

	// 2008.01.15 ---┓
	#define _XDEF_NEWENCRYPTION_PACKDATA				// ndcore.dsc 사용 제거.
	// --------------┛
	
	// 2008.01.28 ---┓
	#define _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER //Author : 양희왕 //breif : 수집 조합 가능 메시지 //date : 08/01/17
	// --------------┛

	// 2008.02.18 ---┓
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘	
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : 양희왕 //breif : 셋트 아이템
	// --------------┛

	// 2008.03.10 ---┓
	#define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI	// 수집 변경
	#define _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI		// 2차 전직 : 오의 페이지 변경
	#define _XDEF_ITEM_V25								// item table v2.5
	// --------------┛

	// 2008.03.21 ---┓
	#define _XDEF_MASTERY_TOTALPOINT_080313_KUKURI		// 오의 토탈 포인트 계산 변경 : _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI와 합칠 것
	#define _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI	// 강무기 착용제한 변경
	// --------------┛

	// 2008.03.28 ---┓
	#define _XDEF_NPROTECT_GUARD						// GameGuard CS 인증
	// --------------┛

	// 2008.04.03 ---┓
	#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// 수집 인덱스 확장
	#define _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI			// 흑백대전 출입 권한 서버에서 주기
	// --------------┛

	// 2008.04.18 ---┓
	#define ALL_RB_SELECT_CLOTHES_080404_KUKURI			// 의상 교체 시스템 : 패킷 변경 됨(MSG_CHARAC_INFO/MSG_ZONE_CHARAC_INFO/MSG_STATE_CHANGE)
	#define _XDEF_NPCBUFF_CASH_080410_KUKURI			// 무명신침 : 점혈술 침통 아이템
	// --------------┛

	////////////
	// 작업중...박소현
	//#define _XTS_ITEMRENEWAL				// item 코드 정리
	//#define _XTS_MOTIONBLURTEST			// motion blur test
	//
	////////////

	////////////
	// 작업완료...박소현
	//#define _XTS_ACCESSORY_NEWTYPE		// 악세사리 new type 추가
	//
	///////////

	//#define _XTS_XTGVER_89			// 지형버젼 0.0089f Load함수 변경 ( Warp Zone, Functional Object )

	#undef _XTS_SERVERUNIFICATION		// 수련서버에서 서버통합 관련 제외
	//#undef _XDEF_NEXONLOGIN				// 넥슨 로그인 - 로그인 패킷 변경 제외
	//#undef _XDEF_NEXONLOGINFINAL		// 넥슨 메신저와 connect 하는 부분 제외
	#undef _XDEF_NEXONUNIFICATIONGUIDE	//정식서버 넥슨 통합 안내문 표시 제외

	#define _XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA	// 넥슨 로그인 : 새 모듈(블로킹 방식) 변경, 수련서버에서만 적용됨. 정식은 _XNEXON_WEBLOGIN 에 포함됨.

	//#define _XDEF_LEVELEVENT_080319_MAGVIPER //Author : 양희왕 //breif : 레벨업 이벤트	
	//#define _XDEF_NOTORIOUS_SYSTEM_MAGVIPER //Author : 양희왕 //breif : 평판 시스템 

	#define _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 캐시 행낭 추가
	#define _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장

#endif

//수련서버 끝 ---------------------------------------------------------------------┛
///////////////////////////////////////////////////////////////////////////////////////


// Define for Inner Test server -----------------------------------┓
#ifdef _XINNERTESTSERVER

#ifndef _XTESTSERVER
#define _XTESTSERVER
#endif

#define _XTS_ITEMPENERTY		// 적용시 아이템착용 제한 계산시 영약으로 증가된 수치도 적용 시킴
#define _XTS_FURY				// 분노/폭주

//#define _XTS_NEW_HONOR2		// 명성
//#define _XTS_TARGET_STATE		// 인물 상태효과 표시
//#define _XTS_PARTYQUEST		// 파티퀘스트 관련 ON/OFF
//#define _XTS_MINISTOPEVENT	// MINISTOP 관련 ON/OFF
//#define _XTS_CASHITEM			// 유료아이템

#endif	// --------------------------------------------------------┛

#endif // __cplusplus
#endif // __XKERNEL_H__
