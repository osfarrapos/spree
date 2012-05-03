// XWindow_MnDGroup.h: interface for the XWindow_MnDGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MNDGROUP_H__4B800B93_0192_4875_A69D_CD7A2076B592__INCLUDED_)
#define AFX_XWINDOW_MNDGROUP_H__4B800B93_0192_4875_A69D_CD7A2076B592__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMnDStructInfo.h"
#include <vector>
#include <map>
using namespace std;

/// 영웅단 공지 정보
struct MND_NOTICEINFO
{
	TCHAR		writer[13];
	TCHAR		writedate[15];
	TCHAR		notice[272];

	MND_NOTICEINFO::MND_NOTICEINFO()
	{
		memset( writer, 0, sizeof(writer) );
		memset( writedate, 0, sizeof(writedate) );
		memset( notice, 0, sizeof(notice) );
	}

	void reset()
	{
		memset( writer, 0, sizeof(writer) );
		memset( writedate, 0, sizeof(writedate) );
		memset( notice, 0, sizeof(notice) );
	} ;

} ;

typedef enum
{
	_XMNDNOTICEMODE_READ		= 0,
	_XMNDNOTICEMODE_WRITE,
	_XMNDNOTICEMODE_MODIFY
}_XMnDNoticeMode;

class _XWindow_MnDGroup
{
public:
	_XWindow_MnDGroup();
	~_XWindow_MnDGroup();

	BOOL Initialize();
	void ShowMnDWindow(BOOL bShow);
	void ShowPopupWindow(BOOL bShow);

	void Draw(int nX, int nY);
	void Process();
	void MoveWindow();
	BOOL ProcessWheelMessage( short zDelta );

	void CalculateNoticeTextength(); //현재 길이
	void SelectNoticeMode( _XMnDNoticeMode mode);	///< 영웅단 공지모드 선택 - 읽기, 쓰기, 수정
	void CopySendNoticeText( void );	///< 작성된 공지사항 서버로 Send	
	void SetNoticeText(TCHAR* text, bool modifymode = false );	///< 공지사항 셋팅 (서버에서 받았을 시 , 수정 했을 시 )

	void SetListItem(); //사제 리스트
	void ResetData();

private:
	_XStickGauge*			m_pCotributionBorder;
	_XImageStatic*			m_pContributionGaugeLeftBorder;
	_XImageStatic*			m_pContributionGaugeRightBorder;	
	
	_XImageStatic*			m_pImageContributionRateLeftBorder;		// 하단 기여도 창
	_XImageStatic*			m_pImageContributionRateRightBorder;	// 하단 기여도 창

	_XListBox*				m_pMnDInfoListBox;

	_XButton*				m_pNoticeWrite;			//공지 작성 버튼
	_XButton*				m_pNoticemodify;		//공지 수정 버튼
	_XButton*				m_pBtnGroupLeave;		//하산 버튼

	TCHAR					m_szString[8][128];//일반적인 글씨

	map<int, int> m_mpLastCotribution; //스승 

public :
	_XButton*				m_pPopUpButton[4];		//팝업 버튼

	_XVScrollBar*			m_pNoticeScrollBar; //공지창 스크롤바
	_XIMEContainer			m_MnDNoticeIMEControl; //공지 사항 

	int m_nPopupCount; //리스트 클릭 할 때 몇개의 버튼이 뜨는지
	int m_nNoticeScrollPos; //공지사항 스크롤 위치
	int m_nNoticeLength; //공지사항 길이
	int m_nNoticeTotalCount; //공지사항 라인카운트 몇줄인지
	int m_nCurrentNoticeLinePos; //현재 몇번째 줄인지
	int m_nNoticeViewTextTotalCount; //전체 보여지는 공지 줄수

	_XMnDNoticeMode m_nNoticeMode;
	MND_NOTICEINFO	m_stNoticeText;			///< 현재 적용되어있는 영웅단 공지	
	MND_NOTICEINFO	m_stSendNoticeTempText;	///< 변경할 영웅단 공지 
	TCHAR					m_szNoticeTempText[256][128]; //임시 저장 공지 사항
	TCHAR					m_szNoticeViewText[256][128];	//확정된 공지사항

	vector<MnDGroupList*>	m_vtMnDGroup;
};

#endif // !defined(AFX_XWINDOW_MNDGROUP_H__4B800B93_0192_4875_A69D_CD7A2076B592__INCLUDED_)
