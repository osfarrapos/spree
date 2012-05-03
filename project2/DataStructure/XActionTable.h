#ifndef		_XACTIONTABLE_DEFINE
#define		_XACTIONTABLE_DEFINE

///////////////////////////////////////////////////////////////////////
// Character action number...

typedef enum _XCHARACTER_MOTION
{
	////////////////////////////////////////////////////////////////////////////////
	// 평화모드 기본 동작
	_XAN_IDLE_NONE		=	0,				//	0 맨손 대기	
	_XAN_IDLE_NONE_ACTION,					//	1 맨손 대기 중간 액션 
	
	_XAN_IDLE_SHORTWEAPON,					//	2 단병기 대기	
	_XAN_IDLE_SHORTWEAPON_ACTION,			//	3 단병기 대기 중간 액션 

	_XAN_IDLE_LONGWEAPON,					//	4 장병기 대기	
	_XAN_IDLE_LONGWEAPON_ACTION,			//	5 장병기 대기 중간 액션 	

	////////////////////////////////////////////////////////////////////////////////
	// 맨손	
	_XAN_MOVE_NONE_START,				    //	6 맨손 이동	시작	
	_XAN_MOVE_NONE_REPEAT,				    //	7 맨손 이동	반복
	_XAN_MOVE_NONE_END,						//	8 맨손 이동	끝
	
	_XAN_FASTMOVE_NONE_START,				//	9 맨손 질주	시작	
	_XAN_FASTMOVE_NONE_REPEAT,				// 10 맨손 질주	반복
	_XAN_FASTMOVE_NONE_END,					// 11 맨손 질주	끝

	////////////////////////////////////////////////////////////////////////////////
	// 단병기	
	_XAN_MOVE_SHORTWEAPON_START,			// 12 단병기 이동	시작	
	_XAN_MOVE_SHORTWEAPON_REPEAT,			// 13 단병기 이동	반복
	_XAN_MOVE_SHORTWEAPON_END,				// 14 단병기 이동	끝
		
	_XAN_FASTMOVE_SHORTWEAPON_START,		// 15 단병기 질주	시작	
	_XAN_FASTMOVE_SHORTWEAPON_REPEAT,		// 16 단병기 질주	반복
	_XAN_FASTMOVE_SHORTWEAPON_END,			// 17 단병기 질주	끝

	////////////////////////////////////////////////////////////////////////////////
	// 장병기	
	_XAN_MOVE_LONGWEAPON_START,				// 18 장병기 이동	시작	
	_XAN_MOVE_LONGWEAPON_REPEAT,			// 19 장병기 이동	반복
	_XAN_MOVE_LONGWEAPON_END,				// 20 장병기 이동	끝
		
	_XAN_FASTMOVE_LONGWEAPON_START,			// 21 장병기 질주	시작	
	_XAN_FASTMOVE_LONGWEAPON_REPEAT,		// 22 장병기 질주	반복
	_XAN_FASTMOVE_LONGWEAPON_END,			// 23 장병기 질주	끝

	////////////////////////////////////////////////////////////////////////////////
	// 연병기	
	_XAN_MOVE_SOFTWEAPON_START,				// 24 연병기 이동	시작	
	_XAN_MOVE_SOFTWEAPON_REPEAT,			// 25 연병기 이동	반복
	_XAN_MOVE_SOFTWEAPON_END,				// 26 연병기 이동	끝
		
	_XAN_FASTMOVE_SOFTWEAPON_START,			// 27 연병기 질주	시작	
	_XAN_FASTMOVE_SOFTWEAPON_REPEAT,		// 28 연병기 질주	반복
	_XAN_FASTMOVE_SOFTWEAPON_END,			// 29 연병기 질주	끝
	
	////////////////////////////////////////////////////////////////////////////////
	// 전투 모드 기본 동작
	_XAN_BATTLEIDLE_NONE,					// 30 맨손 대기
	_XAN_BATTLEIDLE_SHORTWEAPON,			// 31 단병기 대기
	_XAN_BATTLEIDLE_LONGWEAPON,				// 32 장병기 대기
	
	////////////////////////////////////////////////////////////////////////////////
	// 낭인 일반 공격
	_XAN_NI_ATTACK_KK,						// 33 권각술	
	_XAN_NI_ATTACK_SHORTWEAPON,				// 34 단병기	
	_XAN_NI_ATTACK_LONGWEAPON,				// 35 장병기
	_XAN_NI_ATTACK_SOFTWEAPON,				// 36 연병기
	_XAN_NI_ATTACK_HIDDENWEAPON,			// 37 암기
	_XAN_NI_ATTACK_THROWWEAPON,				// 38 투석기

	////////////////////////////////////////////////////////////////////////////////
	// 방어
	_XAN_DEFENCE_NONE,						//  39 맨손 막기
	_XAN_DEFENCE_SHORTWEAPON,				//  40 단병기 막기
	_XAN_DEFENCE_LONGWEAPON,				//  41 장병기 막기
	_XAN_DEFENCE_SOFTWEAPON,				//  42 연병기 막기

	////////////////////////////////////////////////////////////////////////////////
	// 얻어맞기
	_XAN_BLOW_NONE,							//  43 맨손   얻어맞기
	_XAN_BLOW_SHORTWEAPON,					//  44 단병기 얻어맞기
	_XAN_BLOW_LONGWEAPON,					//  45 장병기 얻어맞기	
	_XAN_BLOW_SOFTWEAPON,					//  46 연병기 얻어맞기	

	////////////////////////////////////////////////////////////////////////////////
	// 사후 처리
	_XAN_DYINGREADY_NONE,						//  47 맨손 밀리기
	_XAN_DYINGREADY_SHORTWEAPON,				//  48 단병기 밀리기
	_XAN_DYINGREADY_LONGWEAPON,					//  49 장병기 밀리기
	
	_XAN_DYINGREPEAT_NONE,						//  50 맨손 심호흡
	_XAN_DYINGREPEAT_SHORTWEAPON,				//  51 단병기 심호흡
	_XAN_DYINGREPEAT_LONGWEAPON,				//  52 장병기 심호흡

	////////////////////////////////////////////////////////////////////////////////
	// 등치기
	_XAN_HITBACK_NONE,						//  53 맨손 등을 치기
	_XAN_HITBACK_SHORTWEAPON,				//  54 단병기 잡고 등을 치기
	_XAN_HITBACK_LONGWEAPON,				//  55 장병기 잡고 등을 치기

	////////////////////////////////////////////////////////////////////////////////
	// 깨어나기
	_XAN_WAKEUP_NONE,						//  56 맨손   깨어나기
	_XAN_WAKEUP_SHORTWEAPON,				//  57 단병기 깨어나기
	_XAN_WAKEUP_LONGWEAPON,					//  58 장병기 깨어나기
	
	////////////////////////////////////////////////////////////////////////////////
	// 쓰러지기
	_XAN_DIE,								//  59 맨손 죽기

	////////////////////////////////////////////////////////////////////////////////
	// 감정 표현
	_XAN_EMOTION_BOW,						// 60 가벼운 인사
	_XAN_EMOTION_BOWPOLITELY,				// 61 정중한 인사	
	_XAN_EMOTION_EXCITEMENT,				// 62 도발
	_XAN_EMOTION_RUSH,						// 63 돌격
	
	_XAN_EMOTION_CRINGESTART,				// 64 무릅꿇기 시작
	_XAN_EMOTION_CRINGEREPEAT,				// 65 무릅꿇기 반복
	_XAN_EMOTION_CRINGEEND,					// 66 무릅꿇기 끝

	_XAN_EMOTION_CHEER,						// 67 환호
	_XAN_EMOTION_BREAKDOWN,					// 68 좌절
	_XAN_EMOTION_CRY,						// 69 울기
	_XAN_EMOTION_LAUGH,						// 70 웃음	
	_XAN_EMOTION_ANGRY,						// 71 화냄
	_XAN_EMOTION_DOZ,						// 72 졸기
	
	_XAN_EMOTION_SITDOWNSTART,				// 73 앉기 시작
	_XAN_EMOTION_SITDOWNREPEAT,				// 74 앉기 반복
	_XAN_EMOTION_SITDOWNEND,				// 75 앉기 끝 

	_XAN_EMOTION_MARRYCLAPPINGHAND,			// 76 박수	
	_XAN_EMOTION_MARRYBOX,					// 77 인사
	_XAN_EMOTION_MARRYBOWPOLITELY,			// 78 큰절

	_XAN_EMOTION_CLAPPINGHAND,				// 79 박수
	
	//////////////////////////////////////////////////////////////////////
	// 낭인 공격 추가
	_XAN_NI_ATTACK_AXE,						// 80 낭인 공격 부법

	//////////////////////////////////////////////////////////////////////
	// 쓰러지기 추가
	_XAN_DIE_SHORTWEAPON,					//  81 단병기 죽기

	//////////////////////////////////////////////////////////////////////
	// 감정 표현 추가
	_XAN_EMOTION_ANIMATION,					//	82 방향지시

	//////////////////////////////////////////////////////////////////////
	// 쓰러지기 추가
	_XAN_DIE_LONGWEAPON,					// 83  장병기 죽기
	
	//////////////////////////////////////////////////////////////////////
	// Damaged action

	_XAN_MEDITATIONDAMAGE,					// 84  운기조식 데미지

	_XAN_COMASTATEWALK,						// 85  주화입마 걷기

	_XAN_COMASTATEIDLE,						// 86  주화입마 대기
	
	//////////////////////////////////////////////////////////////////////
	// LPS Action

	_XAN_LPS_HANDTOUCH,						// 87  맨손 건드리기

	_XAN_LPS_SHORTWEAPON_HANDTOUCH,			// 88  단병기 건드리기

	_XAN_LPS_LONGWEAPON_HANDTOUCH,			// 89  장병기 건드리기

	_XAN_LPS_FOOTTOUCH,						// 90  맨손 발로 건드리기
	
	_XAN_LPS_SHORTWEAPON_FOOTTOUCH,			// 91  단병기 발로 건드리기
	
	_XAN_LPS_LONGWEAPON_FOOTTOUCH,			// 92  장병기 발로 건드리기

	_XAN_LPS_USEKEY_HAND,					// 93  맨손   열쇠로 열기
	_XAN_LPS_USEKEY_SHORTWEAPON,			// 94  단병기 열쇠로 열기
	_XAN_LPS_USEKEY_LONGWEAPON,				// 95  장병기 열쇠로 열기

	_XAN_ITEMPICKING,						// 96  아이템 줍기
	
	_XAN_MATCH_NONE,						// 97  비무 맨손
	_XAN_MATCH_SHORTWEAPON,					// 98  비무 단병기
	_XAN_MATCH_LONGWEAPON,					// 99  비무 장병기
	_XAN_MATCH_CANCEL,						// 100 비무 기권

	_XAN_IDLE_AXE,							// 101 부법 대기

	_XAN_ELIXIR_FAIL,						// 102 영약 복용 실패
	
	//////////////////////////////////////////////////////////////////////
	// 검 관련
	_XAN_IDLE_SWORD_PEACE,					// 103 검 대기(평화)
	_XAN_IDLE_SWORD_ACTION,					// 104 검 액션(평화)
	
	_XAN_MOVE_SWORD_REPEAT_PEACE,			// 105 검 이동(평화)
	_XAN_MOVE_SWORD_REPEAT_BATTLE,			// 106 검 이동(전투)

	_XAN_FASTMOVE_SWORD_START_PEACE,		// 107 검 질주 시작(평화)
	_XAN_FASTMOVE_SWORD_REPEAT_PEACE,		// 108 검 질주 반복(평화)
	_XAN_FASTMOVE_SWORD_END_PEACE,			// 109 검 질주 정지(평화)

	_XAN_FASTMOVE_SWORD_START_BATTLE,		// 110 검 질주 시작(전투)
	_XAN_FASTMOVE_SWORD_REPEAT_BATTLE,		// 111 검 질주 반복(전투)
	_XAN_FASTMOVE_SWORD_END_BATTLE,			// 112 검 질주 정지(전투)

	_XAN_BATTLEIDLE_SWORD_START,			// 113 검 대기 전환 시작
	_XAN_BATTLEIDLE_SWORD_REPEAT,			// 114 검 대기 전환 반복
	_XAN_BATTLEIDLE_SWORD_END,				// 115 검 대기 전환 정지

	_XAN_NI_ATTACK_SWORD,					// 116 검 공격

	_XAN_DYINGREADY_SWORD_PEACE,			// 117 검 밀리기(평화)
	_XAN_DYINGREPEAT_SWORD_PEACE,			// 118 검 심호흡(평화)
	_XAN_HITBACK_SWORD_PEACE,				// 119 검 등치기(평화)
	_XAN_WAKEUP_SWORD_PEACE,				// 120 검 깨어나기(평화)
	_XAN_DIE_SWORD_PEACE,					// 121 검 쓰러지기(평화)

	_XAN_DYINGREADY_SWORD_BATTLE,			// 122 검 밀리기(전투)
	_XAN_DYINGREPEAT_SWORD_BATTLE,			// 123 검 심호흡(전투)
	_XAN_HITBACK_SWORD_BATTLE,				// 124 검 등치기(전투)
	_XAN_WAKEUP_SWORD_BATTLE,				// 125 검 깨어나기(전투)
	_XAN_DIE_SWORD_BATTLE,					// 126 검 쓰러지기(전투)

	_XAN_KNEE_SWORD_START,					// 127 검 무릎꿇기 시작
	_XAN_KNEE_SWORD_REPEAT,					// 128 검 무릎꿇기 반복
	_XAN_KNEE_SWORD_END,					// 129 검 무릎꿇기 정지

	_XAN_LPS_USEKEY_SWORD_PEACE,			// 130 검 열쇠로 열기(평화)
	_XAN_LPS_USEKEY_SWORD_BATTLE,			// 131 검 열쇠로 열기(전투)
	
	_XAN_MATCH_WINNER_START,				// 132 비무 - 승자
	_XAN_MATCH_WINNER_REPEAT,				// 133 비무 - 승자

	_XAN_MATCH_LOSER_START,					// 134 비무 - 패자
	_XAN_MATCH_LOSER_REPEAT,				// 135 비무 - 패자

	_XAN_MATCH_DIE_START,					// 136 비무 - 혼수
	_XAN_MATCH_DIE_REPEAT,					// 137 비무 - 혼수

	_XAN_MATCH_VIEW_START,					// 138 비무 - 혼수중 관람
	_XAN_MATCH_VIEW_REPEAT,					// 139 비무 - 혼수중 관람

	_XAN_MATCH_PR_START,					// 140 비무 - 홍보
	_XAN_MATCH_PR_REPEAT,					// 141 비무 - 홍보
	_XAN_MATCH_PR_END,						// 142 비무 - 홍보
	
#ifdef _XDEF_CASTLEBATTLE
	_XAN_MANOR_WIN = 219,					// 219 장원전 - 승리의 기쁨
	_XAN_MANOR_SHOUT,						// 220 장원전 - 사자후
#endif
	
}_XCHARACTER_MOTION;





#endif