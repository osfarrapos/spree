#ifndef _XQUESTSCRIPTDEFINE_H_
#define _XQUESTSCRIPTDEFINE_H_

//////////////////////////////////////////////////////////////////////////////////////
/////  Trigger condition 관련 ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_CONDITION_COUNT								66

#define ID_CONDITION_CHECKVALUE							0		// 플레이어_스탯비교
#define ID_CONDITION_HAVESKILL							1		// 플레이어_무공비교
#define ID_CONDITION_CHECKSTATUS						2		// 플레이어_상태검사
#define ID_CONDITION_CHECKGENDER						3		// 플레이어_성별검사
#define ID_CONDITION_CHECKJOINGROUP						4		// 플레이어_문파검사
#define ID_CONDITION_CHECKAREA							5		// 플레이어_영역검사
#define ID_CONDITION_CHECKMESSAGE						6		// 플레이어_메세지비교
#define ID_CONDITION_HAVEITEM							7       // 플레이어_아이템비교
#define ID_CONDITION_CHECKATTACK						8		// 플레이어_전투결과검사
#define ID_CONDITION_CHECKMARTIAL						9		// 플레이어_무공개수검사
#define ID_CONDITION_CHECKPARTYCOUNT					10		// 파티원_수검사
#define ID_CONDITION_CHECKPARTYSTATUS					11		// 파티원_상태검사 <-수정예정(레벨업, 죽음)
#define ID_CONDITION_CHECKAREAOTHER						12		// 타플레이어_영역검사
#define ID_CONDITION_CHECKDISTANCE						13		// 타플레이어_거리체크
#define ID_CONDITION_CHECKTARGETTING					14		// 타플레이어_타게팅검사
#define ID_CONDITION_CHECKTIMER							15		// 타이머체크
#define ID_CONDITION_CHECKPASSTIME						16		// 시간경과체크
#define ID_CONDITION_CHECKPROCEDINGTRIGGER				17		// 선행트리거체크
#define ID_CONDITION_CHECKACTIONID						18		// 액션실행체크
#define ID_CONDITION_CHECKPROCEDINGQUEST				19		// 선행퀘스트결과체크
#define ID_CONDITION_ISPROGRESSQUEST					20		// 특정퀘스트시작체크
#define ID_CONDITION_PROGRESSQUEST						21		// 진행중인 퀘스트 개수 체크
#define ID_CONDITION_CHECKSELECTION						22		// 선택지에서 선택한 번호 체크 
#define ID_CONDITION_CHECKNPCDIALOGMODE					23		// NPC 대화 모드 종료 체크
#define ID_CONDITION_CHECKWEARITEM						24		// 아이템 장착 체크  
#define ID_CONDITION_SETQUESTMARKAREA					25		// QUEST마크(!) 표시하기 영역  
#define ID_CONDITION_SETQUESTMARKNPC					26		// QUEST마크(!) 표시하기 NPC   
#define ID_CONDITION_SETHINTMARKAREA					27		// HINT마크(?) 표시하기 영역
#define ID_CONDITION_SETHINTMARKNPC						28		// HINT마크(?) 표시하기 NPC 
#define ID_CONDITION_CHECKSELECTIONEND					29      // 선택지 체크 종료가 되었는지.....
#define ID_CONDITION_CHECKCOUNTPOINT					30		// 카운터 포인트 체크하기 
#define ID_CONDITION_CHECKMOTION						31		// 특정 행동 체크 
#define ID_CONDITION_CHECKABNORMALSTATE					32		// 플레이어_상태이상체크 
#define ID_CONDITION_CHECKBATTLERECORD					33      // 플레이어_비무전적체크
#define ID_CONDITION_CHECKPARTYJOINGROUP				34      // 파티원_문파체크 
#define ID_CONDITION_CHECKPARTYVALUE					35		// 파티원_스탯비교(명성,성취단계 검사, 명성,성취단계 총합검사...)
#define ID_CONDITION_DISPLAYREWARD						36		// 보상물 표시하기 서버는 패스
#define ID_CONDITION_CHECKSKILLSTATE					37		// 플레이어_무공상태비교
#define ID_CONDITION_CHECKCLANSERVICEPOINT				38		// 플레이어_문파기여도체크
#define ID_CONDITION_OPTION_ITEMSTATE					39		// 퀘스트 취소시 아이템 삭제 or 유지 정보(옵션조건식에서만 사용)
#define ID_CONDITION_CHECKQUESTEMAIL					40		// 풍운방 수락 or 거부 체크 (서버는 패스, 하지만 분기의 조건으로 쓰일 수 있음) 
#define ID_CONDITION_CHECKBOSSBATTLESTART				41		// 생사결 시작 체크
#define ID_CONDITION_CHECKBOSSBATTELSTAGENUMBER			42		// 생사결 단계 체크
#define ID_CONDITION_CHECKPARTYMASTER					43		// 파티원_행수검사
#define ID_CONDITION_CHECKPARTYMEMBERCLASS				44		// 파티원_직책검사
#define ID_CONDITION_CHECKAREAPARTYMEMBER				45		// 파티원_영역검사
#define ID_CONDITION_CHECKRECORDNUMBER					46		// 플레이어_기록표검사
#define ID_CONDITION_CHECKLEVEL							47		// 플레이어_성취도검사
#define ID_CONDITION_CHECKPLAYERCLASS					48		// 플레이어_직책검사
#define ID_CONDITION_CHECKINVENSPACE					49		// 행낭여유공간검사
#define ID_CONDITION_CHECKPARTYMASTERDEAD				50		// 행수죽음검사(옵션조건식에서만 사용)
#define ID_CONDITION_CHECKDESTORYITEM					51		// 아이템파괴검사(옵션조건식OR시작조건타입)
#define ID_CONDITION_CHECKSTARTLIMITQUEST				52		// 부여선착순갯수검사
#define ID_CONDITION_CHECKCOMPLETELIMITQUEST			53		// 완료선착순갯수검사
#define ID_CONDITION_CHECKPARTYGENDER					54		// 파티원_성별체크
#define ID_CONDITION_CHECKSERVICETYPE					55		// 플렝이어_요금제검사
#define ID_CONDITION_CHECKPARTYORGANIZATION				56		// 파티원_영웅단검사
#define ID_CONDITION_PARTYHAVEITEM						57		// 파티원_아이템검사
#define ID_CONDITION_CHECKNICKNAME						58		// 플레이어_별호검사
#define ID_CONDITION_CHECKGROUPLEADER					59		// 영웅단주 및 단장 검사
#define ID_CONDITION_CHECKPARTYCLAN						60		// 파티원_문파검사
#define ID_CONDITION_CHECKPARTYPREQUEST					61		// 파티원_선행퀘스트검사
#define ID_CONDITION_CHECKPARTYCLASSOR					62		// 파티원_전체직책검사
#define ID_CONDITION_CHECK_MND_MASTERPOINT				63		// 사제시스템_스승기여도검사
#define ID_CONDITION_CHECK_MND_MASTERRANK				64		// 사제시스템_스승등급검사
#define ID_CONDITION_CHECK_MND_STATUS					65		// 사제시스템_상태검사



//////////////////////////////////////////////////////////////////////////////////////
/////  Trigger action 관련 ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_ACTION_COUNT									74

#define ID_ACTION_ADDSKILL								0		// 플레이어_무공설정
#define ID_ACTION_DELETESKILL							1		// 플레이어_무공삭제
#define ID_ACTION_GIVEITEM								2		// 플레이어_아이템주기
#define ID_ACTION_TAKEITEM								3		// 플레이어_아이템삭제
#define ID_ACTION_GIVEEXPERIENCE						4		// 플레이어_경험치 주기
#define ID_ACTION_MOVE									5		// 플레이어_이동
#define ID_ACTION_SETSTATUS								6		// 플레이어_스탯설정
#define ID_ACTION_SOUNDSTART							7		// 효과음 설정
#define ID_ACTION_MUSICSTART							8		// 배경음 설정
#define ID_ACTION_SETENV								9		// 날씨 설정
#define ID_ACTION_EFFECTSTART							10		// 특수효과 설정
#define ID_ACTION_TIMERSTART							11		// 타이머 설정
#define ID_ACTION_SENDMESSAGE							12		// 대화 출력
#define ID_ACTION_AUTOSTART								13		// 자동 퀘스트 설정
#define ID_ACTION_RESULTQUEST							14		// 퀘스트 결과 설정
#define ID_ACTION_MOVECAMERA							15		// 카메라 이동설정
#define ID_ACTION_ROTATECAMERA							16		// 카메라 회전설정
#define ID_ACTION_CAMERASETTING							17		// 카메라 줌설정
#define ID_ACTION_SHAKECAMERA							18		// 카메라 흔들기
#define ID_ACTION_SCREENEFFECT							19		// 화면 연출
#define ID_ACTION_SETWEATHER							20		// 기상 설정
#define ID_ACTION_BLOCKTRIGGER							21		// 트리거 블럭
#define ID_ACTION_JOINGROUP								22		// 문파가입
#define ID_ACTION_GOTOTRIGGER							23		// 해당 트리거로 이동
#define ID_ACTION_QUESTPROGRESSINFO						24		// 퀘스트 진행 정보 : 퀘스트 창에 표시될 트리진행 상항 텍스트 
#define ID_ACTION_SELECTION								25		// 선택지 실행
#define ID_ACTION_STARTMINIGAME							26		// 미니게임 실행( 운기조식 )
#define ID_ACTION_DEFAULT								27		// 아무행동도 안하는 트리거
#define ID_ACTION_TALKINGTOONESELF						28		// 혼자말하기 
#define ID_ACTION_SETMOVEVELOCITY						29		// 이동속도 강제 세팅
#define ID_ACTION_SETWEARITEM							30		// 아이템 착용 세팅
#define ID_ACTION_STARTOPTIONCONDITION					31		// 옵션 조건 리스트 시작하기
#define ID_ACTION_ENDOPTIONCONDITION					32		// 옵션 조건 리스트 끝내기 
#define ID_ACTION_GIVECOUNTPOINT						33		// 카운터 포인트 주기 
#define ID_ACTION_SETMOBSPAWN							34		// 몬스터 불러오기
#define ID_ACTION_SETOBJECTSPAWN						35		// 오브젝트 불러오기
#define ID_ACTION_SETMOTION								36		// 강제로 행동 시키기
#define ID_ACTION_SETMOVIE								37		// 영상 보여주기 
#define ID_ACTION_GIVENICKNAME							38		// 별호주기		
#define ID_ACTION_SETMAP								39		// 지도 ON/OFF	
#define ID_ACTION_MONSTERTALKINGTOONESELF				40		// 몬스터 혼자말하기 	

#define ID_ACTION_SETSKILLEXP							41		// 플레이어_무공숙련도(주기,빼앗기)
#define ID_ACTION_SETSKILLSTATE							42		// 플레이어_무공상태(사용중지,사용가능)
#define ID_ACTION_SETABNORMALSTATE						43		// 플레이어_상태이상설정(주기, 해제)
#define ID_ACTION_GIVESETTINGITEM						44		// 설정된 아이템 주기
#define ID_ACTION_RESETVALUE							45		// 스텟 초기화

#define ID_ACTION_SETPLAYERCLASS						46		// 플레이어_클래스지정
#define ID_ACTION_SETCLANSERVICEPOINT					47		// 플레이어_문파기여도(주기,빼앗기)
#define ID_ACTION_SETITMECOUNTERSTART					48		// 아이템 카운터 시작하기
#define ID_ACTION_STOPCOUNTER							49		// 카운터 멈추기
#define ID_ACTION_SETCONDITIONEFFECT					50		// 플레이어_상태효과부여
#define ID_ACTION_SETSYSTEMMESSAGE						51		// 시스템메세지출력
#define ID_ACTION_SETNPCONOFF							52		// NPC 생성 & 삭제
#define ID_ACTION_SENDQUESTEMAIL						53		// 풍운방 보내기 
#define ID_ACTION_PARTYGIVEITEM							54		// 파티원_아이템주기or뺏기(행수,행원,MVP)
#define ID_ACTION_PARTYGIVEEXPERIENCE					55		// 파티원_스텟주기or뺏기(행수,행원,MVP)  
#define ID_ACTION_PARTYSETABNORMALSTATE					56		// 파티원_상태이상주기
#define ID_ACTION_PARTYGIVEEQUALLYREWARD				57		// 파티원_균등한보상물주기(보상/파티원수) 
#define ID_ACTION_HOLDITEM								58		// 아이템장착해제불가설정
#define ID_ACTION_PARTYMESSAGE							59		// 파티원_메세지출력(혼잣말,시스템)
#define ID_ACTION_INSERTRANK							60		// 랭크에 등록하기
#define ID_ACTION_CHANGESERVERGROUP						61		// 서버군 상태 변경하기 
#define ID_ACTION_GIVEEXPERIENCEBYPERCENTAGE			62		// 플레이어_n%경험치주기
#define ID_ACTION_DISPLAYQUESTRANKING					63		// 특정퀘스트랭킹보여주기
#define ID_ACTION_STARTPARTYITMECOUNTER					64      // 파티아이템카운터시작하기
#define ID_ACTION_GIVECLASSITEM							65		// 역할별 아이템주기(개인,동행)
#define ID_ACTION_PARTYGIVENICKNAME						66		// 파티원_별호주기
#define ID_ACTION_PARTYTAKEITEM							67		// 파티원_아이템뺏기(파티전체)
#define ID_ACTION_SETVIRTUALMODE						68		// 가상공간설정하기
#define ID_ACTION_GIVEEXPERIENCEBYLEVEL					69		// 플레이어_레벨%경험치주기
#define ID_ACTION_SETWIDECONDITIONEFFECT				70		// 플레이어_광역상태효과부여
#define ID_ACTION_SETCLASSGRADE							71		// 직책 등급 설정

#define ID_ACTION_MND_GIVEMASTER_RANK					72		// 사제시스템 스승등급설정
#define ID_ACTION_MND_SETMASTERPOINT					73		// 사제시스템 스승기여도(주기,뺏기)





//////////////////////////////////////////////////////////////////////////////////////////
// 퀘스트에서 사용하는 값 정의
/////////////////////////////////////////////////////////////////////////////////////////

// 트리거 활성 / 비활성 ---------------------------------------------------------------
#define ID_TRIGGER_DISABLE						        0
#define ID_TRIGGER_ENABLE							    1

// 트리거 데이터 초기화 ---------------------------------------------------------------
#define ID_TRIGGER_SETDATA_NONE						   -1

// 트리거 조건문간 계산 공식 ----------------------------------------------------------
#define ID_TRIGGER_CONDITIONFUNCTION_OR			        0
#define ID_TRIGGER_CONDITIONFUNCTION_AND			    1

// 트리거 퀘스트 마크 종류 ----------------------------------------------------------
#define ID_TRIGGER_QUESTMARK_AREA				        0
#define ID_TRIGGER_QUESTMARK_NPC					    1

// 트리거 힌트 마크 종류 ----------------------------------------------------------
#define ID_TRIGGER_HINTMARK_AREA				        0
#define ID_TRIGGER_HINTMARK_NPC						    1

// 트리거 힌트 마크 타입 ----------------------------------------------------------
#define ID_TRIGGER_MARKTYPE_WORLD				        0
#define ID_TRIGGER_MARKTYPE_MAP						    1
#define ID_TRIGGER_MARKTYPE_WORLDANDMAP				    2

// 비교 연산자 관련 -------------------------------------------------------------------
#define ID_TRIGGER_COMPARE_COUNT						3

#define ID_TRIGGER_COMPARE_THELEAST                     0		// <
#define ID_TRIGGER_COMPARE_THEMOST                      1		// >
#define ID_TRIGGER_COMPARE_EXACTLY                      2		// =


// 데이터 셋 관련 ---------------------------------------------------------------------
#define ID_TRIGGER_DATASET_COUNT                        3

#define ID_TRIGGER_DATASET_ADD                          0		// +
#define ID_TRIGGER_DATASET_SUBTRACT                     1		// -
#define ID_TRIGGER_DATASET_SET                          2		// =

// Trigger 종류 ----------------------------------------------------------------------
#define ID_TRIGGER_CLASS_NORMAL							0
#define ID_TRIGGER_CLASS_FAIL							1
#define ID_TRIGGER_CLASS_JUMP							2
#define ID_TRIGGER_CLASS_END							3
#define ID_TRIGGER_CLASS_STARTCONDITION					127

// Trigger 속성 ----------------------------------------------------------------------
#define ID_TRIGGER_PROPERTY_CONTINUE					0
#define ID_TRIGGER_PROPERTY_COMPLETE					1

// Check value ------------------------------------------------------------------------
// Condition_CheckValue 
//-------------------------------------------------------------------------------------
#define ID_QUEST_CHECKVALUETYPE_TOTALLEVEL					0	// 내공 수위
#define ID_QUEST_CHECKVALUETYPE_VITALITY					1	// 생기
#define ID_QUEST_CHECKVALUETYPE_ZEN							2	// 진기
#define ID_QUEST_CHECKVALUETYPE_SOUL						3	// 영기
#define ID_QUEST_CHECKVALUETYPE_HEART						4	// 심기
#define ID_QUEST_CHECKVALUETYPE_STAMINA						5	// 정기
#define ID_QUEST_CHECKVALUETYPE_VITALPOWER					6	// 체력
#define ID_QUEST_CHECKVALUETYPE_INNERPOWER					7	// 내력
#define ID_QUEST_CHECKVALUETYPE_SOULPOWER					8	// 영력
#define ID_QUEST_CHECKVALUETYPE_HEARTPOWER					9	// 심력
#define ID_QUEST_CHECKVALUETYPE_STAMINAPOWER				10	// 정력
#define ID_QUEST_CHECKVALUETYPE_HONOR						11	// 명성
#define ID_QUEST_CHECKVALUETYPE_RETRIBUTION					12	// 업
#define ID_QUEST_CHECKVALUETYPE_EXPERIENCE					13	// 경험치

// Target class -----------------------------------------------------------------------
// Action_EffectStart
//-------------------------------------------------------------------------------------
#define ID_QUEST_TARGETCLASS_PLAYER						0	// 플레이어
#define ID_QUEST_TARGETCLASS_STATICNPC					1	// 정적 NPC
#define ID_QUEST_TARGETCLASS_DYNAMICNPC					2	// 동적 NPC
#define ID_QUEST_TARGETCLASS_AREA						3	// 영역

// Target class ------------------------------------------------------------------------
// Condition_CheckAreaOther
// -------------------------------------------------------------------------------------
#define ID_QUEST_TARGETOTHER_STATICNPC					0	// 정적 NPC
#define ID_QUEST_TARGETOTHER_DYNAMICNPC					1	// 동적 NPC
#define ID_QUEST_TARGETOTHER_MONSTER					2	// 몹
#define ID_QUEST_TARGETOTHER_PARTY						3	// 파티


// Targetting --------------------------------------------------------------------------
// Condition_CheckTargetting
//--------------------------------------------------------------------------------------
#define ID_QUEST_TARGETTING_STATICNPC					0	// 정적 NPC
#define ID_QUEST_TARGETTING_DYNAMICNPC					1	// 동적 NPC
#define ID_QUEST_TARGETTING_AREA						2	// 영역
#define ID_QUEST_TARGETTING_OBJECT						3	// 오브젝트

// Target object ------------------------------------------------------------------------
// Condition_CheckDistance
//---------------------------------------------------------------------------------------
#define ID_QUEST_TARGETOBJECT_STATICNPC					0	// 정적 NPC
#define ID_QUEST_TARGETOBJECT_DYNAMICNPC				1	// 동적 NPC
#define ID_QUEST_TARGETOBJECT_AREA						2	// 영역

// TargetObject2(monster 포함) ----------------------------------------------------------
// Condition_CheckStatusOther
//---------------------------------------------------------------------------------------
#define ID_QUEST_TARGETOTHER_STATICNPC					0	// 정적 NPC
#define ID_QUEST_TARGETOTHER_DYNAMICNPC					1	// 동적 NPC
#define ID_QUEST_TARGETOTHER_MONSTER					2	// 몬스터

// Item type ----------------------------------------------------------------------------
// Condition_HaveItem/Action_GiveItem/Action_TakeItem
//---------------------------------------------------------------------------------------
#define ID_QUEST_ITEMTYPE_WEAPON						0	// 무기류
#define ID_QUEST_ITEMTYPE_CLOTHES						1	// 의상류
#define ID_QUEST_ITEMTYPE_BOOK							2	// 무공비급
#define ID_QUEST_ITEMTYPE_POTION						3	// 회복류
#define ID_QUEST_ITEMTYPE_THROW							4	// 투척
#define ID_QUEST_ITEMTYPE_POISON						5	// 중독 주문서
#define ID_QUEST_ITEMTYPE_COUNTERACT					6	// 해독 주문서
#define ID_QUEST_ITEMTYPE_RESOURCE						7	// 자원
#define ID_QUEST_ITEMTYPE_QUEST							8	// 퀘스트
#define ID_QUEST_ITEMTYPE_ACCESSORY						9	// 악세사리
#define ID_QUEST_ITEMTYPE_LIFE							10	// 생활 도구
#define ID_QUEST_ITEMTYPE_MUSIC							11	// 악기
#define ID_QUEST_ITEMTYPE_PASS							12	// 통행증
#define ID_QUEST_ITEMTYPE_MONEY							13	// 돈


// Status type(player) -------------------------------------------------------------------
// Action_SetStatus
//---------------------------------------------------------------------------------------
#define ID_QUEST_STATUSTYPE_VITALITY					0	// 생기
#define ID_QUEST_STATUSTYPE_ZEN							1	// 진기
#define ID_QUEST_STATUSTYPE_SOUL						2	// 영기
#define ID_QUEST_STATUSTYPE_HEART						3	// 심기
#define ID_QUEST_STATUSTYPE_VIGOR						4	// 정기
#define ID_QUEST_STATUSTYPE_VITALPOWER					5	// 체력
#define ID_QUEST_STATUSTYPE_INNERPOWER					6	// 내력
#define ID_QUEST_STATUSTYPE_HEARTPOWER					7	// 심력
#define ID_QUEST_STATUSTYPE_SOULPOWER					8	// 영력
#define ID_QUEST_STATUSTYPE_STAMINAPOWER				9	// 정력

// Experience type -----------------------------------------------------------------------
// Action_GiveExperience
// ---------------------------------------------------------------------------------------
#define ID_QUEST_SETEXP_EXPERICENCE						0	// 경험치
#define ID_QUEST_SETEXP_HONOR							1	// 명성
#define ID_QUEST_SETEXP_RETRIBUTION						2	// 업보

// Status type(other) ---------------------------------------------------------------------
// Action_SetStatusOther
//-----------------------------------------------------------------------------------------
#define ID_QUEST_STATUSTYPE_DEATH						0	// 죽음
#define ID_QUEST_STATUSTYPE_REVIVAL						1	// 부활


// 메세지 비교 -----------------------------------------------------------------------------
// Condition_CheckMessage
//-----------------------------------------------------------------------------------------
#define ID_QUEST_MATCHMESSAGE_WHOLEWORD					0	// 전체 포함
#define ID_QUEST_MATCHMESSAGE_PARTIALWORD				1	// 부분 포함

// 타이머 ----------------------------------------------------------------------------------
// Condition_CheckTimer
//------------------------------------------------------------------------------------------
#define ID_QUEST_CHECKTIMER_ANYTIME						0	// 관계 없음
#define ID_QUEST_CHECKTIMER_DAYLIGHT					1	// 낮
#define ID_QUEST_CHECKTIMER_NIGHT						2	// 밤

// 날씨 설정 -------------------------------------------------------------------------------
// Action_SetEnv
//------------------------------------------------------------------------------------------
#define ID_QUEST_WEATHER_CLEAR							0	// 맑음
#define ID_QUEST_WEATHER_RAIN							1	// 비
#define ID_QUEST_WEATHER_LIGHTNING						2	// 비 번개
#define ID_QUEST_WEATHER_SNOW							3	// 눈
#define ID_QUEST_WEATHER_SANDY							4	// 황사

// Status type2(player) -------------------------------------------------------------------
// Condition_CheckStatus
//-----------------------------------------------------------------------------------------
#define ID_QUEST_STATUSMYSELF_BATTLE					0	// 전투
#define ID_QUEST_STATUSMYSELF_RIDEHORSE					1	// 승마
#define ID_QUEST_STATUSMYSELF_INPARTY					2	// 파티 가입
#define ID_QUEST_STATUSMYSELF_NOTPARTY					3	// 파티 미가입
#define ID_QUEST_STATUSMYSELF_JOINGROUP					4	// 문파 가입
#define ID_QUEST_STATUSMYSELF_NOTJOINGROUP				5	// 문파 미가입
#define ID_QUEST_STATUSMYSELF_LEVELUP					6	// 레벨업
#define ID_QUEST_STATUSMYSELF_TRADENPC					7	// NPC와 거래
#define ID_QUEST_STATUSMYSELF_TALKNPC					8	// NPC와 대화
#define ID_QUEST_STATUSMYSELF_DEATH						9	// 죽음
#define ID_QUEST_STATUSMYSELF_REVIVE					10	// 부활


// Status type2(other) --------------------------------------------------------------------
// Condition_CheckPartyStatus
//-----------------------------------------------------------------------------------------
#define ID_QUEST_STATUSPARTY_LEVELUP					0	// 레벨 업
#define ID_QUEST_STATUSPARTY_DEATH						1	// 죽음


// 퀘스트 결과 -----------------------------------------------------------------------------
// Action_ResultQuest
//-----------------------------------------------------------------------------------------
#define ID_QUEST_RESULT_SUCCESS							0		// 성공
#define ID_QUEST_RESULT_FAIL							1		// 실패
#define ID_QUEST_RESULT_CANCEL							2		// 취소


// 메세지 체크 - 한 EditBox에 들어가는 단어 수 ----------------------------------------------
// Condition_MessageCheck
//-----------------------------------------------------------------------------------------
#define ID_MAX_CHECKMESSAGE								10		// 10개 단어까지 처리 가능


// 시간 경과 체크 - 새벽/낮/저녁/밤 시작 시간 -----------------------------------------------
// Condition_CheckPassTime
//-----------------------------------------------------------------------------------------
#define ID_QUEST_CHECKTIME_DAWNTIME							0		// 새벽
#define ID_QUEST_CHECKTIME_DAYTIME							3600	// 낮
#define ID_QUEST_CHECKTIME_NIGHTFALLTIME					14400	// 저녁
#define ID_QUEST_CHECKTIME_NIGHTTIME						18000	// 밤
#define ID_QUEST_CHECKTIME_ONEDAYTIME						28800	// 하루 = 28800초 = 8시간

#define ID_QUEST_CHECKTIME_DAWN								0
#define ID_QUEST_CHECKTIME_DAY								1
#define ID_QUEST_CHECKTIME_NIGHTFALL						2
#define ID_QUEST_CHECKTIME_NIGHT							3


// 카메라 흔들기 ---------------------------------------------------------------------------
// Action_ShakeCamera
//-----------------------------------------------------------------------------------------
#define ID_QUEST_SHAKECAMERA_MAXHEIGHT						0.5
#define ID_QUEST_SHAKECAMERA_UNITHEIGHT						0.1
// 방향
#define ID_QUEST_SHAKECAMERA_UP								0
#define ID_QUEST_SHAKECAMERA_DOWN							1
#define ID_QUEST_SHAKECAMERA_LEFT							0
#define ID_QUEST_SHAKECAMERA_RIGHT							1


// 카메라 회전 -------------------------------------------------------------------------------
// Action_RotateCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_ROTATECAMERA_ANGLE							15.0


// 문파 -------------------------------------------------------------------------------------
// Condition_HaveSkill/Action_AddSkill/Action_DeleteSkill
//------------------------------------------------------------------------------------------

#define ID_QUEST_JOINGROUP_NANGIN							0
#define ID_QUEST_JOINGROUP_GAEBANG							1
#define ID_QUEST_JOINGROUP_BEEGOONG							2
#define ID_QUEST_JOINGROUP_SORIM							3
#define ID_QUEST_JOINGROUP_NOCKRIM							4
#define ID_QUEST_JOINGROUP_MOODANG							5
#define ID_QUEST_JOINGROUP_MAKYO							6
#define ID_QUEST_JOINGROUP_SEGA								7
#define ID_QUEST_JOINGROUP_NOFIXED							8

// 플레이어 상태이상 -------------------------------------------------------------------------------------
// Condition_AbnormalState
//------------------------------------------------------------------------------------------

#define ID_QUEST_STATE_STATE1								0 //기절
#define ID_QUEST_STATE_STATE2								1 //마비 (순간적인 기절)
#define ID_QUEST_STATE_STATE3								2 //침묵
#define ID_QUEST_STATE_STATE4								3 //재우기
#define ID_QUEST_STATE_STATE5								4 //점혈
#define ID_QUEST_STATE_STATE6								5 //출혈독
#define ID_QUEST_STATE_STATE7								6 //신경독
#define ID_QUEST_STATE_STATE8								7 //치사독
#define ID_QUEST_STATE_STATE9								8 //혼란
#define ID_QUEST_STATE_STATE10								9 //환각
#define ID_QUEST_STATE_STATE11								10 //환청
#define ID_QUEST_STATE_STATE12								11 //공포
#define ID_QUEST_STATE_STATE13								12 //실명
#define ID_QUEST_STATE_STATE14								13 //불
#define ID_QUEST_STATE_STATE15								14 //번개
#define ID_QUEST_STATE_STATE16								15 //투명
#define ID_QUEST_STATE_STATE17								16 //얼음
#define ID_QUEST_STATE_STATE18								17 //출혈
#define ID_QUEST_STATE_STATE19								18 //주화입마
#define ID_QUEST_STATE_STATE20								19 //취중

// 플레이어 전투 상태 조건 ---------------------------------------------------------------------
// Condition_CheckAttack
// ----------------------------------------------------------------------------------------
#define ID_QUEST_CHECKATTACK_TARGETTING						0
#define ID_QUEST_CHECKATTACK_DEATH							1
#define ID_QUEST_CHECKATTACK_HIT							2
#define ID_QUEST_CHECKATTACK_CHANGE							3
#define ID_QUEST_CHECKATTACK_CLIENTMOBDEATH					4


// 날씨, 기상에 대한 정의 -------------------------------------------------------------------
// Action_CancelWeather
//------------------------------------------------------------------------------------------
#define ID_QUEST_CANCELWEATHER_ENV							0
#define ID_QUEST_CANCELWEATHER_WEATHER						1

// 사운드 기능 선택 ------------------------------------------------------------------------
// Action_SoundStart/Action_MusicStart
// -----------------------------------------------------------------------------------------
#define ID_QUEST_SOUNDTARGET_START							0
#define ID_QUEST_SOUNDTARGET_STOP							1


// 재생 방법 -------------------------------------------------------------------------------
// Action_SoundStart/Action_MusicStart/Action_EffectStart
//------------------------------------------------------------------------------------------
#define ID_QUEST_PLAYTYPE_ONCE								0
#define ID_QUEST_PLAYTYPE_LOOP								1

// 카메라 높이 -----------------------------------------------------------------------------
// Action_RotateCamera
//-----------------------------------------------------------------------------------------
#define ID_QUEST_CAMERAHEIGHT_HIGH							0
#define ID_QUEST_CAMERAHEIGHT_MIDDLE						1
#define ID_QUEST_CAMERAHEIGHT_LOW							2

// 카메라 회전 방향 --------------------------------------------------------------------------
// Action_RotateCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERADIRECTION_LEFT						0
#define ID_QUEST_CAMERADIRECTION_RIGHT						1

// 카메라 거리 --------------------------------------------------------------------------
// Action_RotateCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERADISTANCE_HIGH						0
#define ID_QUEST_CAMERADISTANCE_MIDDLE						1
#define ID_QUEST_CAMERADISTANCE_LOW							2

// 카메라 흔들기 방향 ------------------------------------------------------------------------
// Action_ShakeCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERASHAKE_TOPDOWN						0
#define ID_QUEST_CAMERASHAKE_LEFTRIGHT						1

// 카메라 흔들기 강도 ------------------------------------------------------------------------
// Action_ShakeCamera/Action_RotateCamera 
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERAINTENSITY_FIRST						0
#define ID_QUEST_CAMERAINTENSITY_SECOND						1
#define ID_QUEST_CAMERAINTENSITY_THIRD						2
#define ID_QUEST_CAMERAINTENSITY_FORTH						3
#define ID_QUEST_CAMERAINTENSITY_FIFTH						4


// 카메라 설정 -----------------------------------------------------------------------------
// Action_CameraSetting
//------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERASETTING_ZOOMIN						0
#define ID_QUEST_CAMERASETTING_ZOOMOUT						1

// 줌인/아웃 대상 설정 ----------------------------------------------------------------------
// Action_CameraSetting
// ------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERASETTING_PLAYER						0
#define ID_QUEST_CAMERASETTING_AREA							1


// 카메라 이동 ------------------------------------------------------------------------------
// Action_MoveCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERAMOVE_PASS							0
#define ID_QUEST_CAMERAMOVE_AREA							1

// 성별 체크 --------------------------------------------------------------------------------
// Condition_CheckGender
//-------------------------------------------------------------------------------------------
#define ID_QUEST_GENDER_MAN									0
#define ID_QUEST_GENDER_WOMAN								1

// 화면 설정 (Fadein/out) -------------------------------------------------------------------
// Action_ScreenEffect
// ------------------------------------------------------------------------------------------
#define ID_QUEST_SCREENEFFECT_FADEIN						0
#define ID_QUEST_SCREENEFFECT_FADEOUT						1

// 카메라 워크 관련 -------------------------------------------------------------------------
#define ID_QUEST_CAMERA_KEEP								0	// 실행 후 카메라 그대로 유지
#define ID_QUEST_CAMERA_RESTORE								1	// 실행 후 카메라 되돌리기

// 플레이어 이동속도 --------------------------------------------------------------------
// Action_SetMoveVelocity
// ------------------------------------------------------------------------------------------
#define ID_QUEST_MOVEVELOCITY_LOW							0
#define ID_QUEST_MOVEVELOCITY_MIDDLE						1
#define ID_QUEST_MOVEVELOCITY_USUAL							2

// 행동식 옵션 조건식 결과 종류  --------------------------------------------------------------------
// Action_OptionConditionStart
// ------------------------------------------------------------------------------------------
#define ID_QUEST_OPTIONRESULT_CANCLESUCCESS					0
#define ID_QUEST_OPTIONRESULT_CANCLEFAIL					1

// 퀘스트 취소 결과 선택 --------------------------------------------------------------------
#define ID_QUEST_CANCELCASE_RESET							0
#define ID_QUEST_CANCELCASE_FAIL							1
#define ID_QUEST_CANCELCASE_NO								2

// 퀘스트 시작 마크 종류 --------------------------------------------------------------------
#define ID_QUEST_STARTMARK_AREA								0
#define ID_QUEST_STARTMARK_NPC								1

// 퀘스트 시간 체크 선택 --------------------------------------------------------------------
#define ID_QUEST_CHECKTIME_NEEDLESS							0
#define ID_QUEST_CHECKTIME_NEED								1

// 풍운록 관련 정보  --------------------------------------------------------------------
#define ID_QUEST_QUESTEMAIL_NONE							0
#define ID_QUEST_QUESTEMAIL_OK								1
#define ID_QUEST_QUESTEMAIL_YESNO							2
#define ID_QUEST_QUESTEMAIL_OK_NOSTART						3
#define ID_QUEST_QUESTEMAIL_YESNO_NOSTART					4

// 동시에 진행이 가능한 퀘스트 수 -----------------------------------------------------------
#define ID_QUEST_SYNC									6
#define ID_MAX_QUESTCOUNT								10   // 퀘스트의 최대 수
#define ID_MAX_CONDITIONCOUNT							16   // 조건문의 최대 수
#define ID_MAX_TRIGGERCOUNT								32	 // 트리거의 최대 수

// 전체 퀘스트 수 ---------------------------------------------------------------------------
#define ID_TOTAL_QUESTCOUNT								240

// 트리거 인증 패킷의 cResult ---------------------------------------------------------------
#define _QUEST_RESULT_SUCCESS_							0		// 트리거 인증 성공( 일반, 실패조건 트리거 )
#define _QUEST_FAILRESULT_SUCCESS_						1		// 트리거 인증 성공( 실패 액션 트리거 ) 조건은 실패했지만 그래도 인증은 성공이다.

#define _QUEST_CONDITION_RESULT_FAILED_					2		// 트리거 조건 실패 
#define _QUEST_ACTION_RESULT_FAILED_					3		// 트리서 True 액션 실패
#define _QUEST_FAILACTION_RESULT_FAILED_				4		// 트리서 fail 액션 실패

// 트리거 인증 성공일 경우의 cInfo 값 -------------------------------------------------------
#define _QUEST_INFO_ACTION_SUCCESS						0		// 서버내부에서 쓸것.

// 트리거 인증 실패일경우 cinfo값 -----------------------------------------------------------
#define _QUEST_INFO_IMPROPER_ID_						1		// 잘못된 트리거아이디 1
#define _QUEST_INFO_PACKET_ERROR_						2		// 잘못된 인증패킷	   1 	
#define _QUEST_INFO_SCRIPT_ERROR_						3		// Quest Script 에러   1,2	
#define _QUEST_INFO_QUEST_SETERROR_						4		// Quest 설정 에러	   2
#define _QUEST_INFO_INVEN_FULL_							5		// 인벤이 꽉찾음	   2	
#define _QUEST_INFO_UNKNOWN_ERROR_						6		// 알수없는 에러 try-catch 1,2

// version ver 1.1
#define _QUEST_VERSION_MAJOR							1		
#define _QUEST_VERSION_MINOR							1

// 국가 코드
#define _QUEST_COUNTRYCODE_KR							0x00000001
#define _QUEST_COUNTRYCODE_VN							0x00000002
#define _QUEST_COUNTRYCODE_US							0x00000004
#define _QUEST_COUNTRYCODE_TW							0x00000008
#define _QUEST_COUNTRYCODE_CN							0x00000010
#define _QUEST_COUNTRYCODE_JP							0x00000020
#define _QUEST_COUNTRYCODE_RS							0x00000040

#endif