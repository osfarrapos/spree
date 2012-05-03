
#include "../Stdafx.h"

#include "GameEvent.h"
#include "GameEventParser.h"


#ifdef _PD_GAME_EVENT_

#ifdef IS_STR
#undef IS_STR
#endif

#define IS_STR(x)		( 0 == strcmp(cmd,x) )
#define IS_STR2(x,y)		( 0 == strcmp(x,y) )

u_short CGameEventParser::GetSecond( char * str )
{
	char temp[10] = {0, };
	u_short m = 0;
	u_short s = 0;

	int len = strlen( str );

	int i = 0;
	int k = 0;
	while( i < len )
	{
		if( 'm' == str[i] )
		{
			m = atoi(temp);
			k = 0;
		}
		else if( 's' == str[i] )
		{
			s = atoi(temp);
			break;
		}
		else
		{
			temp[k] = str[i];
			++k;
		}

		++i;
	}

	s = s + m*60;

	return s;

}

bool CGameEventParser::GetDateTime( _sND_TimeUnit& t, char * date, char * time )
{
	char temp_y[20] = {0, };
	char temp_m[20] = {0, };
	char temp_d[20] = {0, };

	int value = 0;
	int count = 0;

	int i = 0;
	int k = 0;
	int len = strlen( date );

	while( i < len )
	{		
		if( '-' == date[i] )
		{
			++count;	k = 0;
		}
		else
		{
			switch( count )
			{
			case 0:
				temp_y[k] = date[i];	++k;	break;
			case 1: 
				temp_m[k] = date[i];	++k;	break;
			case 2:
				temp_d[k] = date[i];	++k;	break;
			}
		}

		++i;
	}

	t.uiYear = static_cast<u_int>( atoi(temp_y) - 2000 );
	t.uiMonth = static_cast<u_int>( atoi(temp_m) );
	t.uiDay = static_cast<u_int>( atoi(temp_d) );

	if( 0 == t.uiYear || 0 == t.uiMonth || 0 == t.uiDay )
	{
		return false;
	}

	i = 0;
	k = 0;
	count = 0;
	len = strlen( time );
	ZeroMemory( temp_y, sizeof(temp_y) );
	ZeroMemory( temp_m, sizeof(temp_m) );

	while( i < len )
	{		
		if( ':' == time[i] )
		{
			++count;	k = 0;
		}
		else
		{
			switch( count )
			{
			case 0:
				temp_y[k] = time[i];	++k;	break;
			case 1: 
				temp_m[k] = time[i];	++k;	break;
			}
		}

		++i;
	}

	if( count == 1 )
	{
		t.uiHour = static_cast<u_int>( atoi(temp_y) );
		t.uiMinute = static_cast<u_int>( atoi(temp_m) );
		return true;
	}
	else 
		return false;
}


bool CGameEventParser::LoadGameEventScript( CGameEventManager& mgr, char * fn, bool onlySchedule /* = false */ )
{
	NDStreamFileInput	file( fn );
	if( false == file.file_is_open() )
	{
		printf( "[INIT ERROR] File not found %s\n", fn );
		return false;
	}

	m_bOnlySchedule = onlySchedule;

	char cmd[40];
	int cmd_type;
	bool ret = true;
	bool bEnd = false;

	while( false == bEnd )
	{
		cmd_type = file.file_get_word( cmd, sizeof(cmd) );

		switch( cmd_type )
		{
		case en_cmd_string:
			file.file_make_lower( cmd );

			if( m_bOnlySchedule )
			{
				if( IS_STR("match_schedule") )
				{
					file.file_skip_cur_line();
					if( false == LoadMatchSchedule( mgr, file ) )
					{
						printf( "fail : LoadMatchSchedule()\n" );
						ret = false;
						bEnd = true;
					}
				}
				else if( IS_STR("match_reward") )
				{
					file.file_skip_cur_line();

					if( false == LoadMatchReward( mgr, file ) )
					{
						printf( "fail : LoadMatchRecord()\n" );
						ret = false;
						bEnd = true;
					}
				}
				else if( IS_STR( "_xm_eos" ) )
				{
					ret = true;
					bEnd = true;
				}
				else
                    file.file_skip_cur_line();
			}
			else
			{
				if( IS_STR("match_type") )
				{
					int type = file.file_get_int();
					if( type <= 0 )
					{
						printf( "fail : type muse be positive( at %d )\n", file.file_get_cur_line() );
						ret = false;
						bEnd = true;
					}
					file.file_skip_cur_line();

					if( false == LoadMatchType( type, mgr, file ) )
					{
						printf( "fail : LoadMatchType\n" );
						ret = false;
						bEnd = true;
					}
				}
				else if( IS_STR("match_reward") )
				{
					file.file_skip_cur_line();

					if( false == LoadMatchReward( mgr, file ) )
					{
						printf( "fail : LoadMatchRecord()\n" );
						ret = false;
						bEnd = true;
					}
				}
				else if( IS_STR("match_schedule") )
				{
					file.file_skip_cur_line();
					if( false == LoadMatchSchedule( mgr, file ) )
					{
						printf( "fail : LoadMatchSchedule()\n" );
						ret = false;
						bEnd = true;
					}
				}
				else if( IS_STR( "_xm_eos" ) )
				{
					ret = true;
					bEnd = true;
				}
				else
				{
					printf( "Unknown : %s at %d", cmd, file.file_get_cur_line_count() );
				}
			}
			break;

		case en_cmd_comment:
			file.file_skip_cur_line();
			break;
		case en_cmd_end:
			bEnd = true;
			break;

		case en_cmd_open_bracket:
		case en_cmd_close_bracket:
			break;
		}
	}

	return true;

}


bool CGameEventParser::LoadMatchType( int type, CGameEventManager& mgr, NDStreamFileInput& file )
{
	char cmd[40];
	int cmd_type;
	bool bEnd = false;
	bool ret = true;
	int where = -1;

	CFreeMatchModule * pEvent = new CFreeMatchModule(type, mgr);

	while( false == bEnd )
	{
		cmd_type = file.file_get_word( cmd, sizeof(cmd) );
		switch( cmd_type )
		{
		case en_cmd_string:
			file.file_make_lower( cmd );

			if( IS_STR( "constraint1" ) )
			{
				where = en_fm_step_none;
				file.file_skip_cur_line();
				if( false == LoadMatchConstraints( pEvent, file ) )
				{
					printf( " - Load Constraints Fail\n" );
					ret = false;
					bEnd = true;
				}
			}
			else if( IS_STR( "camera" ) )
			{
				where = en_fm_step_none;
				file.file_skip_cur_line();
				LoadMatchCamera( file );		// 그냥 넘긴다.
			}
			else if( IS_STR( "constraint2" ) )
			{
				where = en_fm_step_none;
				file.file_skip_cur_line();
				if( false == LoadMatchConstraints( pEvent, file ) )
				{
					printf( " - Load Constraints Fail\n" );
					ret = false;
					bEnd = true;
				}
			}
			else if( IS_STR( "constraint3" ) )
			{
				where = en_fm_step_none;
				file.file_skip_cur_line();
				if( false == LoadMatchConstraints( pEvent, file ) )
				{
					printf( " - Load Constraints Fail\n" );
					ret = false;
					bEnd = true;
				}
			}
			else if( IS_STR( "ready" ) )
			{
				where = en_fm_step_ready;
				file.file_skip_cur_line();
				if( false == LoadMatchWork( where, pEvent, file ) )
				{
					printf( " - Load Ready work fail\n" );
					ret = false;
					bEnd = true;
				}
			}
			else if( IS_STR( "start" ) )
			{
				where = en_fm_step_start;
				file.file_skip_cur_line();
				if( false == LoadMatchWork( where, pEvent, file ) )
				{
					printf( " - Load Start work fail\n" );
					ret = false;
					bEnd = true;
				}
			}
			else if( IS_STR( "end" ) )
			{
				where = en_fm_step_end;
				file.file_skip_cur_line();
				if( false == LoadMatchWork( where, pEvent, file ) )
				{
					printf( " - Load End work fail\n" );
					ret = false;
					bEnd = true;
				}
			}
			else
			{
				printf( " = match_type : unknown command at %d\n", file.file_get_cur_line_count() );
				ret = false;
				bEnd = true;
			}
			break;

		case en_cmd_comment:
			file.file_skip_cur_line();
			break;
		case en_cmd_end:
			bEnd = true;
			break;

		case en_cmd_open_bracket:
			break;
		case en_cmd_close_bracket:
			if( where == en_fm_step_end )
				bEnd = true;
			break;
		}
	}

	if( ret )
		mgr.AddMatchModule( pEvent );
	else
		delete pEvent;

	return ret;
}


bool CGameEventParser::LoadMatchConstraints( CFreeMatchModule * pEvent, NDStreamFileInput& file )
{
	char cmd[40];
	int cmd_type;
	bool bEnd = false;
	bool ret = true;

	__ge_reward reward;


#define SET_ON_OFF(_flag)	if( file.file_count_arguement() > 0 )			\
	{												\
	file.file_get_word( cmd, sizeof(cmd) );		\
	file.file_make_lower( cmd );				\
	if( IS_STR( "on" ) )						\
	pEvent->SetConstFlag( en_fm_cf_##_flag );\
	}\
							else\
	{\
	printf( " - match_type : constraint : on/off miss at %d\n", file.file_get_cur_line_count() );\
	ret = false;\
	bEnd = true;\
	}


	// "end" 없이 '}' 이 나오면 안된다.

	while( false == bEnd )
	{
		cmd_type = file.file_get_word( cmd, sizeof(cmd) );

		switch( cmd_type )
		{
		case en_cmd_string:
			file.file_make_lower( cmd );

			if( m_bOnlySchedule )
			{
				file.file_skip_cur_line();
				continue;
			}

			if( IS_STR( "hyperrun" ) )
			{
				SET_ON_OFF(hyperrun)
			}
			else if( IS_STR( "breath" ) )
			{
				SET_ON_OFF(breath)
			}
			else if( IS_STR( "logout" ) )
			{
				SET_ON_OFF(logout)
			}
			else if( IS_STR( "escape" ) )
			{
				SET_ON_OFF(escape)
			}
			else if( IS_STR( "party" ) )
			{
				SET_ON_OFF(party)
			}
			else if( IS_STR( "trade" ) )
			{
				SET_ON_OFF(trade)
			}
			else if( IS_STR( "info" ) )
			{
				SET_ON_OFF(info)
			}
			else if( IS_STR( "recall" ) )
			{
				SET_ON_OFF(recall)
			}
			else if( IS_STR( "shop" ) )
			{
				SET_ON_OFF(shop)
			}
			else if( IS_STR( "hide_me" ) )
			{
				SET_ON_OFF(hide_me)
			}
			else if( IS_STR( "buff" ) )
			{
				SET_ON_OFF(buff_reset)
			}
			else if( IS_STR( "pvp_event" ) )
			{
				SET_ON_OFF(pvp_event)
			}
			else if( IS_STR( "use_skill" ) )
			{
				if( file.file_count_arguement() >= 1 )
				{
					pEvent->AddCannotSkill( (short)file.file_get_int() );
				}
			}
			else if( IS_STR( "use_item" ) )
			{
				if( file.file_count_arguement() >= 2 )
				{
					pEvent->AddUseItem( (char)file.file_get_int(), (short)file.file_get_int() );
				}
				else
				{
					printf( " - match_type : constraint : use_item miss at %d\n", file.file_get_cur_line_count() );
					ret = false;
					bEnd = true;
				}
			}		
			break;

		case en_cmd_comment:
			file.file_skip_cur_line();
			break;
		case en_cmd_end:
			bEnd = true;
			break;

		case en_cmd_open_bracket:
			break;
		case en_cmd_close_bracket:
			bEnd = true;
			break;
		}
	}

	return ret;
}


bool CGameEventParser::LoadMatchCamera( NDStreamFileInput& file )
{
	char cmd[40];
	int cmd_type;
	bool bEnd = false;
	bool ret = true;

	while( false == bEnd )
	{
		cmd_type = file.file_get_word( cmd, sizeof(cmd) );

		switch( cmd_type )
		{
		case en_cmd_string:
			file.file_skip_cur_line();
			break;

		case en_cmd_comment:
			file.file_skip_cur_line();
			break;
		case en_cmd_end:
			bEnd = true;
			break;

		case en_cmd_open_bracket:
			break;
		case en_cmd_close_bracket:
			bEnd = true;
			break;
		}
	}

	return ret;
}

bool CGameEventParser::LoadMatchWork( int where, CFreeMatchModule * pEvent, NDStreamFileInput& file )
{
	char cmd[40];
	int cmd_type;
	bool bEnd = false;
	bool ret = true;

	__ge_reward reward;

	__fm_event_work event_work;


	// "end" 없이 '}' 이 나오면 안된다.

	while( false == bEnd )
	{
		cmd_type = file.file_get_word( cmd, sizeof(cmd) );
		switch( cmd_type )
		{
		case en_cmd_string:
			file.file_make_lower( cmd );

			if( m_bOnlySchedule )
			{
				file.file_skip_cur_line();
				continue;
			}

			if( isdigit( cmd[0] ) )
			{
				char work[40];
				char arg1[40];
				char arg2[20];
				char arg3[20];

				ZeroMemory( &event_work, sizeof(event_work) );

				switch( where )
				{
					//case en_fm_parse_where_constraint:
					//	break;
				case en_fm_step_ready:
				case en_fm_step_start:
				case en_fm_step_end:
					if( file.file_count_arguement() >= 1 )
					{
						file.file_get_word( work, sizeof(work) );
						file.file_make_lower( work );

						if( 0 == strcmp( "notice", work ) )
						{
							if( file.file_count_arguement() >= 2 )
							{
								file.file_get_word( arg1, sizeof(arg1) );
								if( 0 == strcmp( "textid", arg1 ) )
								{
									if( file.file_count_arguement() == 1 )
									{
										file.file_get_word( arg2, sizeof(arg2) );
										arg3[0] = 0;
									}
									else
									{
										file.file_get_word( arg2, sizeof(arg2) );
										file.file_get_word( arg3, sizeof(arg3) );
									}

									event_work.work = en_fm_work_notice;
									event_work.usOrgSecond = GetSecond( cmd );
									event_work.notice.textID = atoi(arg2);
									event_work.notice.arg = atoi(arg3);

									pEvent->AddWork( where, &event_work );

									file.file_skip_cur_line();

									break;
								}
							}

							printf( " - match_type : fail to add work at %d\n", file.file_get_cur_line_count() );
							ret = false;
							bEnd = true;
						}
						else if( 0 == strcmp( "notice_winner", work ) )
						{
							if( file.file_count_arguement() >= 2 )
							{
								file.file_get_word( arg1, sizeof(arg1) );
								if( 0 == strcmp( "textid", arg1 ) )
								{
									if( file.file_count_arguement() == 1 )
									{
										file.file_get_word( arg2, sizeof(arg2) );
										arg3[0] = 0;
									}
									else
									{
										file.file_get_word( arg2, sizeof(arg2) );
										file.file_get_word( arg3, sizeof(arg3) );
									}

									event_work.work = en_fm_work_notice_winner;
									event_work.usOrgSecond = GetSecond( cmd );
									event_work.notice.textID = atoi(arg2);
									event_work.notice.arg = atoi(arg3);

									pEvent->AddWork( where, &event_work );

									file.file_skip_cur_line();

									break;
								}
							}

							printf( " - match_type : fail to add work at %d\n", file.file_get_cur_line_count() );
							ret = false;
							bEnd = true;
						}
						else if( 0 == strcmp( "notice_nowinner", work ) )
						{
							if( file.file_count_arguement() >= 2 )
							{
								file.file_get_word( arg1, sizeof(arg1) );
								if( 0 == strcmp( "textid", arg1 ) )
								{
									if( file.file_count_arguement() == 1 )
									{
										file.file_get_word( arg2, sizeof(arg2) );
										arg3[0] = 0;
									}
									else
									{
										file.file_get_word( arg2, sizeof(arg2) );
										file.file_get_word( arg3, sizeof(arg3) );
									}

									event_work.work = en_fm_work_notice_nowinner;
									event_work.usOrgSecond = GetSecond( cmd );
									event_work.notice.textID = atoi(arg2);
									event_work.notice.arg = atoi(arg3);

									pEvent->AddWork( where, &event_work );

									file.file_skip_cur_line();

									break;
								}
							}

							printf( " - match_type : fail to add work at %d\n", file.file_get_cur_line_count() );
							ret = false;
							bEnd = true;
						}
						else if( 0 == strcmp( "to_winner", work ) )
						{
							if( file.file_count_arguement() >= 2 )
							{
								file.file_get_word( arg1, sizeof(arg1) );
								if( 0 == strcmp( "textid", arg1 ) )
								{
									if( file.file_count_arguement() == 1 )
									{
										file.file_get_word( arg2, sizeof(arg2) );
										arg3[0] = 0;
									}
									else
									{
										file.file_get_word( arg2, sizeof(arg2) );
										file.file_get_word( arg3, sizeof(arg3) );
									}

									event_work.work = en_fm_work_to_winner;
									event_work.usOrgSecond = GetSecond( cmd );
									event_work.notice.textID = atoi(arg2);
									event_work.notice.arg = atoi(arg3);

									pEvent->AddWork( where, &event_work );

									file.file_skip_cur_line();

									break;
								}
							}

							printf( " - match_type : fail to add work at %d\n", file.file_get_cur_line_count() );
							ret = false;
							bEnd = true;
						}
						else if( 0 == strcmp( "send", work ) )
						{
							if( file.file_count_arguement() >= 1 )
							{
								file.file_get_word( arg1, sizeof(arg1) );
								if( 0 == strcmp( "result", arg1 ) )
								{
									event_work.work = en_fm_work_send_result;
									event_work.usOrgSecond = GetSecond( cmd );

									pEvent->AddWork( where, &event_work );
									file.file_skip_cur_line();
									break;
								}
							}

							printf( " - match_type : fail to add work at %d\n", file.file_get_cur_line_count() );
							ret = false;
							bEnd = true;
						}
						else if( 0 == strcmp( "reward", work ) )
						{
							if( file.file_count_arguement() >= 1 )
							{
								file.file_get_word( arg1, sizeof(arg1) );
								if( 0 == strcmp( "nick", arg1 ) )
								{
									event_work.work = en_fm_work_reward_nick;
									event_work.usOrgSecond = GetSecond( cmd );

									pEvent->AddWork( where, &event_work );
									file.file_skip_cur_line();
									break;
								}
								else if( IS_STR2( "item", arg1 ) )
								{
									event_work.work = en_fm_work_reward_item;
									event_work.usOrgSecond = GetSecond( cmd );

									pEvent->AddWork( where, &event_work );
									file.file_skip_cur_line();
									break;
								}
							}

							printf( " - match_type : fail to add work at %d\n", file.file_get_cur_line_count() );
							ret = false;
							bEnd = true;
						}
						else if( 0 == strcmp( "buff", work ) )
						{
							if( file.file_count_arguement() >= 1 )
							{
								file.file_get_word( arg1, sizeof(arg1) );
								if( 0 == strcmp( "all", arg1 ) )
								{
									event_work.work = en_fm_work_buff_all;
									event_work.usOrgSecond = GetSecond( cmd );

									pEvent->AddWork( where, &event_work );
									file.file_skip_cur_line();
									break;
								}
							}

							printf( " - match_type : fail to add work at %d\n", file.file_get_cur_line_count() );
							ret = false;
							bEnd = true;
						}
						else if( 0 == strcmp( "clean", work ) )
						{
							event_work.work = en_fm_work_clean;
							event_work.usOrgSecond = GetSecond( cmd );

							pEvent->AddWork( where, &event_work );
							break;
						}
						else
						{
							printf( " - match_type : unknown(%s) at %d\n", work, file.file_get_cur_line_count() );
							ret = false;
							bEnd = true;
						}
					}
					break;
				default:
					printf( " - match_type : unknown where : at %d\n", file.file_get_cur_line_count() );
					ret = false;
					bEnd = true;
				}
			}
			break;

		case en_cmd_comment:
			file.file_skip_cur_line();
			break;
		case en_cmd_end:
			bEnd = true;
			break;

		case en_cmd_open_bracket:
			break;
		case en_cmd_close_bracket:
			bEnd = true;
			break;
		}
	}

	return ret;
}


bool CGameEventParser::LoadMatchReward( CGameEventManager& mgr, NDStreamFileInput& file )
{
	char cmd[40];
	int cmd_type;
	bool bEnd = false;

	__ge_reward reward;


	// "end" 없이 '}' 이 나오면 안된다.

	while( false == bEnd )
	{
		cmd_type = file.file_get_word( cmd, sizeof(cmd) );
		switch( cmd_type )
		{
		case en_cmd_string:
			file.file_make_lower( cmd );

			if( IS_STR( "reward" ) )
			{
				if( file.file_count_arguement() >= 1 )
				{
					int index = file.file_get_int();
					if( index <= 0 )
					{
						printf( " - match_reward : reward index error(at %d)\n", file.file_get_cur_line_count() );
						return false;
					}

					ZeroMemory( &reward, sizeof(reward) );

					// reward 초기화 하여, 인자로 넣는다.
					reward.reward_index = static_cast<u_short>(index);
					if( false == LoadRewardDetail( reward, file ) )
						return false;

					// reward 는 빈 것이므로, pNext 부터 넣는다.
					mgr.AddRewardInfo( reward.pNext );
				}
				else
				{
					printf( " - match_reward : no reward index(at %d)\n", file.file_get_cur_line_count() );
					return false;
				}
			}
			else
				file.file_skip_cur_line();
			break;

		case en_cmd_comment:
			file.file_skip_cur_line();
			break;
		case en_cmd_end:
			bEnd = true;
			break;

		case en_cmd_open_bracket:
			break;
		case en_cmd_close_bracket:
			bEnd = true;
			break;
		}
	}

	return true;
}

bool CGameEventParser::LoadRewardDetail( __ge_reward& reward, NDStreamFileInput& file )
{
	char cmd[40];
	int cmd_type;
	bool bEnd = false;
	u_short reward_type;
	char	item_type;
	short	item_id;
	u_int	item_count;
	u_short	nick_id;

	char arg[40];

	while( false == bEnd )
	{
		cmd_type = file.file_get_word( cmd, sizeof(cmd) );
		switch( cmd_type )
		{
		case en_cmd_string:
			file.file_make_lower( cmd );

			if( IS_STR( "item" ) )
			{
				item_id = 0;
				int arg_count = file.file_count_arguement();
				if( arg_count >= 3 )
				{
					file.file_get_word( arg, sizeof(arg) );
					file.file_make_lower( arg );
					if( 0 == strcmp( arg, "male" ) )
					{
						reward_type = en_ge_reward_item_to_male;
					}
					else if( 0 == strcmp( arg, "female" ) )
					{
						reward_type = en_ge_reward_item_to_female;
					}
					else if( 0 == strcmp( arg, "all" ) )
					{
						reward_type = en_ge_reward_item;
					}
					else
					{
						printf( " - match_reward : unknown arg at %d\n", file.file_get_cur_line_count() );
						return false;
					}

					item_type = (char)file.file_get_int();
					item_id = (short)file.file_get_int();
					if( arg_count >= 4 )
						item_count = file.file_get_uint();
					else 
						item_count = 1;

					__ge_reward * pNew = new __ge_reward;
					pNew->reward_index = reward.reward_index;
					pNew->reward_type = reward_type;
					pNew->item.cType = item_type;
					pNew->item.sID = item_id;

					if( item_count > 0 && item_count <= 255 )
					{
                        pNew->item.ucCount = static_cast<u_char>(item_count);
						reward.add( pNew );

					}
					file.file_skip_cur_line();
				}
				else
				{
					printf( " - match_reward : insufficient arg at %d\n", file.file_get_cur_line_count() );
					return false;
				}
			}
			else if( IS_STR( "nick" ) )
			{
				if( file.file_count_arguement() >= 1 )
				{
					nick_id = (u_short) file.file_get_uint();

					__ge_reward * pNew = new __ge_reward;
					pNew->reward_index = reward.reward_index;
					pNew->reward_type = en_ge_reward_nick;
					pNew->nick.usNickID = nick_id;

					reward.add( pNew );

					file.file_skip_cur_line();					
				}
				else
				{
					printf( " - match_reward : insufficient arg at %d\n", file.file_get_cur_line_count() );
					return false;
				}
			}
			else if( IS_STR( "end") )
			{
				// AddSchedule...
				bEnd = true;
			}
			break;

		case en_cmd_comment:
			file.file_skip_cur_line();
			break;
		case en_cmd_end:
			bEnd = true;
			break;

		case en_cmd_open_bracket:
			break;
		case en_cmd_close_bracket:
			bEnd = true;
			break;
		}
	}

	return true;
}


bool CGameEventParser::LoadMatchSchedule( CGameEventManager& mgr, NDStreamFileInput& file )
{
	char cmd[40];
	int cmd_type;
	bool bEnd = false;
	bool ret = true;

	char date_str[20];
	char time_str[20];

	_sND_TimeUnit startTime;
	_sND_TimeUnit endTime;
	u_char match_type;
	short min_level, max_level;
	u_short reward_index;
	u_short pos_region;
	u_short respawn_region;
	short buff_index;
	u_short buff_time;
	u_char buff_step;
	u_short fm_rank;

	_sFreeMatchEvent * pFMEvent = NULL;

	while( false == bEnd )
	{
		cmd_type = file.file_get_word( cmd, sizeof(cmd) );
		switch( cmd_type )
		{
		case en_cmd_string:
			file.file_make_lower( cmd );

			if( IS_STR( "start" ) )
			{
				ZeroMemory( &startTime, sizeof(startTime) );
				ZeroMemory( &endTime, sizeof(endTime) );
				match_type = 0;
				min_level = -1;
				max_level = -1;
				reward_index = 0;
				pos_region = 0;
				respawn_region = 0;
				buff_index = 0;
				buff_time = 0U;
				buff_step = 0U;
				fm_rank = 0xffff;
			}
			else if( IS_STR( "end") )
			{
				// AddSchedule...
				if( 0 == startTime.uiYear || 0 == endTime.uiYear )
				{
					printf( " - match schedule : date error at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
				if( 0 == match_type )
				{
					printf( " - match schedule : no type at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
				if( -1 == min_level || -1 == max_level )
				{
					printf( " - match schedule : level error at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
				if( 0xffff == fm_rank )
				{
					printf( " - match schedule : step error at #d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
				if( 0 == reward_index )
				{
					printf( " - match schedule : reward error at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
				if( 0 == pos_region )
				{
					printf( " - match schedule : pos error at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
				if( 0 == respawn_region )
				{
					printf( " - match schedule : respawn error at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}

				// 
				pFMEvent = new _sFreeMatchEvent;
				if( pFMEvent )
				{
					CopyMemory( &pFMEvent->start_time, &startTime, sizeof(_sND_TimeUnit) );
					CopyMemory( &pFMEvent->end_time, &endTime, sizeof(_sND_TimeUnit) );

					pFMEvent->event_type = GEVENT_TYPE_FREE_MATCH;
					pFMEvent->event_state = GEVENT_STATE_NOT_RESERVE;


					pFMEvent->match_type = match_type;
					pFMEvent->match_index = 0;

					pFMEvent->from_level = min_level;
					pFMEvent->to_level = max_level;

					pFMEvent->fm_rank = fm_rank;

					pFMEvent->reward_index = reward_index;

					pFMEvent->pos_region = pos_region;
					pFMEvent->respawn_region = respawn_region;

					pFMEvent->buff_index = buff_index;
					pFMEvent->buff_time = buff_time;
					pFMEvent->buff_step = buff_step;

					CFreeMatchModule * pModule = mgr.GetMatchModule( match_type );
					if( pModule && pModule->GetReadyTime() > 0 )
					{
						CND_TimeUnitFunc::GetTime_BeforeSecond( pFMEvent->ready_time, pFMEvent->start_time, pModule->GetReadyTime() );
					}
					else
					{
						ZeroMemory( &pFMEvent->ready_time, sizeof(_sND_TimeUnit) );
					}

					mgr.AddEvent( pFMEvent );
				}
				else
				{
					printf( " - match schedule : new _sFreeMatchEvent fail\n" );
					ret = false; bEnd = true; break;
				}
			}
			else if( IS_STR( "date" ) )
			{
				if( file.file_count_arguement() >= 4 )
				{
					file.file_get_word( date_str, sizeof(date_str) );
					file.file_get_word( time_str, sizeof(time_str) );

					if( false == GetDateTime( startTime, date_str, time_str ) )
					{
						printf( " - match schedule : start date error at %d\n", file.file_get_cur_line_count() );
						ret = false; bEnd = true; break;
					}

					ZeroMemory( date_str, sizeof(date_str) );
					ZeroMemory( time_str, sizeof(time_str) );

					file.file_get_word( date_str, sizeof(date_str) );
					file.file_get_word( time_str, sizeof(time_str) );

					if( false == GetDateTime( endTime, date_str, time_str ) )
					{
						printf( " - match schedule : end date error at %d\n", file.file_get_cur_line_count() );
						ret = false; bEnd = true; break;
					}
				}
				else
				{
					printf( " - match schedule : date have 4 arg at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
			}
			else if( IS_STR( "type" ) )
			{
				if( file.file_count_arguement() >= 1 )
				{
					match_type = static_cast<u_char>(file.file_get_uint());

					file.file_skip_cur_line();
				}
				else 
				{
					printf( " - match schedule : type have 1 arg at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
			}
			else if( IS_STR( "level" ) )
			{
				if( file.file_count_arguement() >= 1 )
				{
					char level[10];
					char * p = level;
					file.file_get_word( level, sizeof(level) );

					min_level = atoi(level);
					while( *p )
					{
						if( (*p) == '-' )
						{
							p = p+1;
							break;
						}
						p = p+1;
					}

					max_level = atoi(p);

					file.file_skip_cur_line();
				}
				else 
				{
					printf( " - match schedule : level have 1 arg at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
			}
			else if( IS_STR( "reward" ) )
			{
				if( file.file_count_arguement() >= 1 )
				{
					reward_index = file.file_get_int();
					file.file_skip_cur_line();
				}
				else 
				{
					printf( " - match schedule : reward have 1 arg at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
			}
			else if( IS_STR( "step" ) )
			{
				if( file.file_count_arguement() >= 1 )
				{
					fm_rank = static_cast<u_short>( file.file_get_uint() );
					file.file_skip_cur_line();
				}
				else
				{
					printf( " - match schedule : step have 1 arg at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
			}
			else if( IS_STR( "pos" ) )
			{
				if( file.file_count_arguement() >= 1 )
				{
					pos_region = file.file_get_int();
					file.file_skip_cur_line();
				}
				else 
				{
					printf( " - match schedule : pos have 1 arg at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
			}
			else if( IS_STR( "respawn" ) )
			{
				if( file.file_count_arguement() >= 1 )
				{
					respawn_region = file.file_get_int();
					file.file_skip_cur_line();
				}
				else 
				{
					printf( " - match schedule : respawn have 1 arg at %d\n", file.file_get_cur_line_count() );
					ret = false; bEnd = true; break;
				}
			}
			else if( IS_STR( "buff" ) )
			{
				if( file.file_count_arguement() >= 3 )
				{
					buff_index = static_cast<short>( file.file_get_int() );
					buff_time = static_cast<u_short>( file.file_get_uint() );
					buff_step = static_cast<u_char>( file.file_get_uint() );

					if( 0 == buff_index )
					{
						buff_time = 0U;
						buff_step = 1;
					}
					else if( buff_step == 0 )
						buff_step = 1;
					else if( buff_step > 12 )
						buff_step = 12;
				}
			}
			else
			{
				printf( " - match shedule : unknown at %d\n", file.file_get_cur_line_count() );
				ret = false; bEnd = true; break;
			}
			break;

		case en_cmd_comment:
			file.file_skip_cur_line();
			break;
		case en_cmd_end:
			bEnd = true;
			break;

		case en_cmd_open_bracket:
			break;
		case en_cmd_close_bracket:
			bEnd = true;
			break;
		}
	}

	return ret;
}

#endif	// _PD_GAME_EVENT_