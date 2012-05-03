#pragma once

#ifdef _PD_GAME_EVENT_

#include "../global/FileInput.h"
#include "GEventStructure.h"


//////////////////////////////////////////////////////////////////////////
// Game Event Script 파일 로딩.
//////////////////////////////////////////////////////////////////////////
class CGameEventParser
{
protected:
	bool LoadMatchType( int type, CGameEventManager& mgr, NDStreamFileInput& file );
	bool LoadMatchConstraints( CFreeMatchModule * pEvent, NDStreamFileInput& file );
	bool LoadMatchCamera( NDStreamFileInput& file );
	bool LoadMatchWork( int where, CFreeMatchModule * pEvent, NDStreamFileInput& file );
	bool LoadMatchReward( CGameEventManager& mgr, NDStreamFileInput& file );
	bool LoadMatchSchedule( CGameEventManager& mgr, NDStreamFileInput& file );
	bool LoadRewardDetail( __ge_reward& reward, NDStreamFileInput& file );

	bool m_bOnlySchedule;


public:
	CGameEventParser()	{}
	virtual ~CGameEventParser() {}

	bool LoadGameEventScript( CGameEventManager& mgr, char * fn, bool onlySchedule = false );

	static u_short GetSecond( char * str );
	static bool GetDateTime( _sND_TimeUnit& t, char * date, char * time );
};


#endif // _PD_GAME_EVENT_