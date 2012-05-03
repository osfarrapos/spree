// XWindow_NoticeProcess.cpp: implementation of the XWindow_NoticeProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XSR_STRINGHEADER.H"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XWindow_EventIcon.h"

#include "XNoticeProcess.h"

//Author : 양희왕 
#if defined(_ACCLAIM_VERSION) /*|| defined(_XVIETNAMESE)*/ || defined(_XJAPANESE)
	#define _XENABLE_GAMEGUARD
#endif

#ifdef _XUSEAPEXMODULE
	#undef _XENABLE_GAMEGUARD
#endif

#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT) 
	#ifdef _XENABLE_GAMEGUARD
		#define _XUSE_GAMEGUARD
	#endif
#endif
//Last updated: 07/01/22

_XNoticeProcess* _XNoticeProcess::m_pInstance = 0;

const int _XNoticeProcess::nSearchInterval = 10;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XNoticeProcess::_XNoticeProcess() : m_dCurrnetTime(0), m_bFirst(true), m_st()
{
	GetLocalTime(&m_st);
}

_XNoticeProcess::~_XNoticeProcess()
{
}

_XNoticeProcess* _XNoticeProcess::GetInstance()
{
	if( m_pInstance == 0 )
	{
		m_pInstance = new _XNoticeProcess;
	}

	return m_pInstance;
}

void _XNoticeProcess::Destroy()
{
	vector<sMsgInfo*>::iterator vtiter;

	m_mpData.clear();

	for( vtiter = m_vtInfo.begin(); vtiter != m_vtInfo.end(); vtiter++ )
	{
		delete (*vtiter);
	}
	m_vtInfo.clear();

	m_XNoticeParser.Destroy();

	m_dCurrnetTime = 0;
	m_bFirst = true;

	if( _XNoticeProcess::m_pInstance )
	{
		delete _XNoticeProcess::m_pInstance;
		_XNoticeProcess::m_pInstance = 0;
	}
}

bool _XNoticeProcess::SetNoticeProfile( int nMsgType, LPTSTR pMsg, int nMinStartTime, int nMinInterval, int nPrintCount, BOOL bLargeFont )
{
	vector<sMsgInfo*>::iterator iter = m_vtInfo.begin();

	for(; iter != m_vtInfo.end(); ++iter )
	{
		// ID 가 등록되어있으면 리턴
		if( (*iter)->nMsgID == nMsgType )
			return false;
	}

	// 메세지 정보 생성
	sMsgInfo* pMsgInfo = new sMsgInfo;

	// 아이디와 시작시간, 반복시간을 저장한다
	// nPrintCount 가 -1 이 아닌 양수가 지정되어있으면 그 숫자 만큼만 출력, 0 이면 절대 출력 되지 않음
	pMsgInfo->SetInfo( nMsgType, nMinStartTime, nMinInterval, nPrintCount, bLargeFont );
	m_vtInfo.push_back( pMsgInfo );

	if(	!AddString( nMsgType, pMsg ))
		return false;

	return true;
}

bool _XNoticeProcess::SetNoticeProfile( LPTSTR szFileName )
{
	if( !m_XNoticeParser.Load( szFileName ))
		return false;

	//메시지 총 갯수 만큼 저장
	for( int i = 0; i < m_XNoticeParser.GetMaxInfo(); i++)
	{
		SetNoticeProfile( m_XNoticeParser.GetInfo(i).GetMsgID(),
					  _XGETINTERFACETEXT( m_XNoticeParser.GetInfo(i).GetMsgID() ),
					  m_XNoticeParser.GetInfo(i).GetStartTime(),
					  m_XNoticeParser.GetInfo(i).GetInterval(),
					  m_XNoticeParser.GetInfo(i).GetPrintCount(),
					  m_XNoticeParser.GetInfo(i).GetLargeFont() );
	}

	return true;
}

// 현재 정보들을 수정할려고 할때... 시작 시간이나 반복시간 메세지들을 변경할수있다 
// 시작시간 반복시간이 -1 일경우는 현재 가지고 있던 정보를 유지
// 필요없음
void _XNoticeProcess::ModifyNoticeProfile( int nMsgType, LPTSTR pMsg, int nMinStartTime, int nMinInterval )
{
	vector<sMsgInfo*>::iterator iter = m_vtInfo.begin();

	for(; iter != m_vtInfo.end(); ++iter )
	{
		if( (*iter)->nMsgID == nMsgType )
		{
			if( nMinStartTime != -1 )
				(*iter)->SetStartTime( nMinStartTime );

			if( nMinInterval != -1 )
				(*iter)->SetIntervalTime( nMinInterval );

			if( pMsg )
                AddString( (*iter)->nMsgID, pMsg , true);
		}
	}
}

void _XNoticeProcess::Update(float fAccumTime)
{
	//중첩되는시간 m_dCurrnetTime은 프로그램이 실행한뒤로부터 경과한 시간
	if( m_bFirst )
	{
		m_bFirst = false;
		m_dCurrnetTime = 0;
	}
	else
		m_dCurrnetTime += fAccumTime;

	static const DWORD iCurrentCheckTime = g_LocalSystemTime;

//Author : 양희왕 
//게임가드 유/무 검사 : m_dCurrnetTime을 이용했을때  너무 많이 실행되는것 같아서 타이머로 교체 <박24>
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)
	
	if( (g_LocalSystemTime - iCurrentCheckTime) > 20000 )
	{
		if( g_GameGuardInstance.Check() != NPGAMEMON_SUCCESS ) 
		{
			extern LPTSTR g_nProtectModuleCheckMessage;
			_XFatalError(g_nProtectModuleCheckMessage);
		}
	}

#endif

#ifdef _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : 양희왕 //breif : 기간제 아이템 메시지
	if( (int)m_dCurrnetTime - nSearchInterval == 0 )
	{
		if(SearchSlotFlagItem())
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3757), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 
	}
#endif

	if( (int)m_dCurrnetTime == 0 ) 
	{
		//breif : PC 방 추가 혜택
		if( g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREMIUM ) || g_pLocalUser->GetPCroomState( _XDEF_PCROOM_PREPAID ) )
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3353), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //PC방 추가 혜택이 적용 중 입니다.

		#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 아이콘
		_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
		if( pEventIcon && pEventIcon->GetShowStatus())
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3784), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); //선물 상자 버튼을 눌러, 선물을 받으시길 바랍니다.
		}
		#endif
	}
	//g_NetworkKernel.InsertChatString(_T("사용기간이 만료 되었습니다"), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE); 

	//출력가능한 메시지가 있는지 체크
    CheckingForMessage();
}

void _XNoticeProcess::CheckingForMessage()
{
	vector<sMsgInfo*>::iterator iter = m_vtInfo.begin();

	for( ; iter != m_vtInfo.end(); ++iter )
	{
		// 0 으로 나누는 걸 허용하지 않는다 
		//현재 출력이 되야하는 시간인지 계산한다
		if( (*iter)->nIntervalTime == 0 && (int)m_dCurrnetTime - (*iter)->nStartTime == 0 )
		{
			PrintMessgae( iter );
		}
		
		else if( (*iter)->nIntervalTime != 0 && (int)m_dCurrnetTime - (*iter)->nStartTime >= 0 && 
				 ((int)m_dCurrnetTime - (*iter)->nStartTime) % (*iter)->nIntervalTime == 0 )
		{
            PrintMessgae( iter );
		}
		else
			(*iter)->SetPrint( true ); // 출력이 안되는 시간에는 출력할수있는 준비상태를 만들어준다
	}
}

void _XNoticeProcess::PrintMessgae( vector<sMsgInfo*>::iterator iter )
{
    LPCTSTR pString = GetDataByIndex( (*iter)->nMsgID );

	TCHAR msgbuffer[512];
	int nHour = ((int)m_dCurrnetTime)/3600;
	_snprintf(msgbuffer, sizeof(msgbuffer), pString, nHour);
	
	// 현재 출력이 안되어있는지 검사
	if( (*iter)->GetPrintResult() && 
		(*iter)->GetCount() != 0 ) 
	{
		//출력이 되었다면 여러번 출력을 못하게 바꿔준다 ( 1초에 단 한번만 출력을 위해 )
		(*iter)->SetPrint( false );
		(*iter)->CountDown();		
		
		//여기에 출력 방식을 정하자~~~~~~~~~~~~~~~~


		//현재 시간을 얻어온다
		SYSTEMTIME TimeInfo;
		GetLocalTime(&TimeInfo);

        g_NetworkKernel.InsertChatString( msgbuffer, _XDEF_CHATMESSAGECOLOR_ITEM, (*iter)->GetLargeFont() );
	}
}

void _XNoticeProcess::CompulsionMessage( int nID )
{
	// ID 만으로 바로 출력 할 수 있게 한다
}

bool _XNoticeProcess::AddString( int Key, LPTSTR pString, bool bModify)
{
	map<int, string>::iterator iter;

	iter = m_mpData.find( Key );
	
	//같은 키값이 있는경우
	if( iter != m_mpData.end() && !bModify)
		return false;
	
	//아이디와 스트링 저장
	m_mpData[Key] = pString;
	
	return true;
}

LPCTSTR  _XNoticeProcess::GetDataByIndex( int nIndex )
{
	map<int, string>::iterator iter;
	
	// 인덱스번호로 찾는다
	iter = m_mpData.find( nIndex );
	
	// 없으면 NULL 을 리턴
	if( iter == m_mpData.end() )
		return NULL;

	return iter->second.c_str();
}

bool _XNoticeProcess::SearchSlotFlagItem()
{
	//인벤의 가장 처음 시작위치 (맨위칸 이 25 ) //케릭터 아이템 위치ㅣ 5
	for( int count = 5; count < _XINVENTORY_SLOTTYPE_TOTALCOUNT; count++ )
	{
		if( count >= _XINVENTORY_SLOTTYPE_TOTALCOUNT ) break;

		if( g_pLocalUser->m_UserItemList[count].Get_m_sID() > 0 &&
				g_pLocalUser->m_UserItemList[count].Get_m_ucYear() > 0 )
		{
			//만료 3일후를 계산 해야 함..
			int DueDay = GetTotalDays( (int)(g_pLocalUser->m_UserItemList[count].Get_m_ucYear()+1900), (int)g_pLocalUser->m_UserItemList[count].Get_m_ucMonth(), (int)g_pLocalUser->m_UserItemList[count].Get_m_ucDay()) 
				- GetTotalDays( m_st.wYear, m_st.wMonth, m_st.wDay );

			if( DueDay <= 3 )
			{
				//3일 차이이면..
				return true;
			}
		}
	}

	return false;
}

int _XNoticeProcess::GetTotalDays(int nYear, int nMonth, int nDay )
{
    static int nTempMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    int ntempDay = ( nYear - 1 ) * 365;
    // 윤년 횟수를 더해줌.
    // 4년마다 한번씩 윤년, 100년에 한번씩 윤년 아님, 400년에 한번씩 윤년임
    // 올해는 제외 해야 하니 year-1
    ntempDay = ntempDay + ( nYear - 1 ) / 4 - ( nYear - 1 ) / 100 + ( nYear - 1 ) / 400 ;


    //올해 윤년 체크
    if ( nYear % 4 == 0 )
    {
        nTempMonth[2] = 29;

        if ( nYear % 100 == 0 && nYear % 400 != 0 )
            nTempMonth[2] = 28;
    }

    // 저번 달까지 모두 더해줌.
    for (int i = 1 ; i < nMonth; i ++)
        ntempDay += nTempMonth[i];

    // 이번달에서 지난 날들을 더해줌
    ntempDay += nDay;

    return ntempDay;
}