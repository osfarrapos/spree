// XTextureAnimation.h: interface for the XTextureAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTEXTUREANIMATION_H__635C32CA_5FCC_46D4_A317_4D37DD429511__INCLUDED_)
#define AFX_XTEXTUREANIMATION_H__635C32CA_5FCC_46D4_A317_4D37DD429511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//breif : 텍스쳐 에니메이션
// 밋밋한 구조... 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//1. → 방향으로 진행 ( 맨 끝에서 다음 줄이 있으면 ↓ 줄로 내려간 후 → 방향으로 진행 )
//2. → 방향으로 진행 ( 맨 끝에서 다음 줄이 있으면 ↓ 줄로 내려간 후 → 방향으로 진행 ) 가장 끝을 만나면 ← 방향으로 진행
//넓이, 높이의 크기는 일정해야 함
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class _XTextureAnimation  
{
public:
	_XTextureAnimation( int left, int top, int right, int bottom, _XWindow* pWindow,_XTextureManager* ptexturemanager, int textureindex,
						unsigned int nNumOfFrameX, unsigned int nNumOfFrameY, unsigned int nAllOfFrame, 
						unsigned int nCurrentXpos = 0,unsigned int nCurrentYpos = 0	);
	~_XTextureAnimation();

	//에니메이션 실행
	void Update( float fdealyTime, BOOL bReverse = FALSE );

	void SetAniLoopCount( int nLoop ); // 에니메이션 반복 횟수 지정
	void SetDrawFirstAni( bool bRes ); // 에니가 끝나면 첫번째 애니를 그리게 할껀지
	void SetDrawEndAni(bool bDraw ); //애니가 끝나면 안그릴지 그릴지

	void InitFrame(); // 처음 셋팅한 위치로
	bool IsEndAnimate(); //애니 중이냐

	void ShowWindow( BOOL show );
	void SetDrawPostion( int X, int Y );
	_XImageStatic* GetImageStatic();

private:
	//!!!그려지는 이미지 크기!!! 사이즈 파일 이미지 크기가 아님, 화면에 그려지는 이미지 크기, g_MainInterfaceTextureArchive, 텍스쳐 번호
	bool Create( int left, int top, int right, int bottom, _XTextureManager* ptexturemanager, int textureindex );

	//Sprite, 아이디는 현재 사용안함..  ( 아이디, x 축 프레임 개수, y축 프레임 개수, 총 프레임 개수, 이미지 파일에서 시작 x, 이미지 파일에서 시작 y )
	void InitLocationA(	unsigned int nID, 
						unsigned int nNumOfFrameX, unsigned int nNumOfFrameY, 
						unsigned int nAllOfFrame, unsigned int nCurrentXpos,unsigned int nCurrentYpos );

	void SetWindow( _XWindow* pWindow );

	void Release();

	void NextFrame(); // 에니메이션 연산 에니메이션이 끝나면 처음으로 되돌린다
	void ReverseFrame(); // 끝으로 가면 다시 돌아간다..

private:
	//텍스쳐 포인터
	_XImageStatic*	m_pImage;

	DWORD m_dwCurrentCheckTime; //진행시간

	bool m_bIsEndAnimate; // 에니메이션이 끝났는지
	bool m_bDrawFirstAnimate; //에니메이션이 끝나면(m_bIsEndAnimate) 첫번째 애니로 돌릴껀지
	bool m_bDrawEndAnimate; //끝난 에니메이션 계속 그릴지 말지

	bool m_bReverse; //역으로 진행

	//object ID
	unsigned int m_nID;

	//에니메이션 x,y 개수
	unsigned int m_nNumOfFrameX;
	unsigned int m_nNumOfFrameY;

	unsigned int m_nNumberOfAnimateFrame; //전체 에니메이션 개수

	unsigned int m_nAniLoopCount;	//애니메이션 총 반복 횟수   ( 0 이면 무제한 )
	unsigned int m_nAniCurrentLoopCount; //현재 진행된 반복 횟수

	int m_nCurrentFrame; // 현재 그려지고 있는 프레임 번호

	//한 프레임 당 이미지 크기
	int m_nFrameImageSIzeWidth;
	int m_nFrameImageSizeHeight;

	//이미지 전체 크기 ( 그림파일 이미지 사이즈가 아님 )
	//적용할 에니메이션 전체에 이미지 사이즈
	int	m_nImageTotalWidth;
	int	m_nImageTotalHeight;

	//출력될 이미지, 그림 파일 좌표 RECT
	int	m_nCurrentXpos;
	int	m_nCurrentYpos;
	int	m_nCurrentEndXpos;
	int	m_nCurrentEndYpos;

	//첫 이미지 좌표 RECT
	int m_nStartXPos;
	int m_nStartYPos;
	int m_nEndXPos;
	int m_nEndYPos;

	//그려지는 이미지 중점 RECT
	int	m_nCenterXpos;
	int	m_nCenterYpos;
};


//inline----------------------------------------------------------------------------------------------------------------
inline void _XTextureAnimation::SetAniLoopCount( int nLoop )
{
	m_nAniLoopCount = nLoop;
}

inline void _XTextureAnimation::SetDrawFirstAni( bool bRes )
{
	m_bDrawFirstAnimate = bRes;
}

inline void _XTextureAnimation::SetDrawEndAni( bool bDraw )
{
	m_bDrawEndAnimate = bDraw;
}

inline bool _XTextureAnimation::IsEndAnimate()
{ 
	return m_bIsEndAnimate; 
}

inline _XImageStatic* _XTextureAnimation::GetImageStatic()
{ 
	return m_pImage; 
}
//inline----------------------------------------------------------------------------------------------------------------

#endif // !defined(AFX_XTEXTUREANIMATION_H__635C32CA_5FCC_46D4_A317_4D37DD429511__INCLUDED_)
