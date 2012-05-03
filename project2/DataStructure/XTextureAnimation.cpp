// XTextureAnimation.cpp: implementation of the XTextureAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"

#include "XTextureAnimation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XTextureAnimation::_XTextureAnimation(int left, int top, int right, int bottom, _XWindow* pWindow,_XTextureManager* ptexturemanager, int textureindex, 
									  unsigned int nNumOfFrameX, unsigned int nNumOfFrameY, unsigned int nAllOfFrame, 
									  unsigned int nCurrentXpos, unsigned int nCurrentYpos) :	m_pImage(NULL), m_dwCurrentCheckTime(0), 
																								m_bIsEndAnimate(false), m_bDrawFirstAnimate(false),
																								m_bDrawEndAnimate(false), m_bReverse(false), 
																								m_nAniLoopCount(0), m_nAniCurrentLoopCount(0)
{
	Create(left, top, right, bottom, ptexturemanager, textureindex );
	InitLocationA( 0, nNumOfFrameX, nNumOfFrameY, nAllOfFrame, nCurrentXpos, nCurrentYpos );
	SetWindow( pWindow );
}


_XTextureAnimation::~_XTextureAnimation()
{
//	SAFE_DELETE( m_pImage );
	Release();
	
}

void _XTextureAnimation::Release()
{
	m_dwCurrentCheckTime = 0;

	m_nID = 0;

	//Original Texture Image Size
	m_nNumOfFrameX = 0;
	m_nNumOfFrameY = 0;

	m_nNumberOfAnimateFrame = 0;
	m_nAniLoopCount = 0;
	m_nAniCurrentLoopCount = 0;

	m_nCurrentFrame = 0;

	m_nFrameImageSIzeWidth = 0;
	m_nFrameImageSizeHeight = 0;

	m_nImageTotalWidth = 0;
	m_nImageTotalHeight = 0;

	//current pixel postion
	m_nCurrentXpos = 0;
	m_nCurrentYpos = 0;

	m_nCurrentEndXpos = 0;
	m_nCurrentEndYpos = 0;
	
	//image pixel start point
	m_nStartXPos = 0;
	m_nStartYPos = 0;

	m_nEndXPos = 0;
	m_nEndYPos = 0;

	m_nCenterXpos = 0;
	m_nCenterYpos = 0;

	m_bIsEndAnimate = false;
	m_bReverse = false;
}

bool _XTextureAnimation::Create( int left, int top, int right, int bottom, _XTextureManager* ptexturemanager, int textureindex )
{
	if( m_pImage )
		return false;

	m_pImage = new _XImageStatic;
	
	if( !m_pImage )
		return false;

	m_pImage->Create( left, top, right, bottom, ptexturemanager, textureindex );

	//한 프레임당 이미지 사이즈 저장
	m_nFrameImageSIzeWidth = right - left;
	m_nFrameImageSizeHeight = bottom - top;

	return true;	
}

void _XTextureAnimation::Update( float fdealyTime, BOOL bReverse )
{
	if(!m_pImage->GetShowStatus()) return;
	
	if(m_bIsEndAnimate)
	{
		if( m_bDrawEndAnimate )
			m_pImage->SetClipRect( m_nCurrentXpos, m_nCurrentYpos, m_nCurrentEndXpos, m_nCurrentEndYpos );

		return;
	}
	
	if( (g_LocalSystemTime - m_dwCurrentCheckTime) < fdealyTime * 1000 )
	{
		m_pImage->SetClipRect( m_nCurrentXpos, m_nCurrentYpos, m_nCurrentEndXpos, m_nCurrentEndYpos );
		return;
	}

	m_dwCurrentCheckTime = g_LocalSystemTime;

	if( bReverse )
		ReverseFrame();
	else
		NextFrame();

	m_pImage->SetClipRect( m_nCurrentXpos, m_nCurrentYpos, m_nCurrentEndXpos, m_nCurrentEndYpos );
}

void _XTextureAnimation::InitLocationA( unsigned int nID, //이미지초기화
	 								  unsigned int nNumOfFrameX, unsigned int nNumOfFrameY,
									  unsigned int nAllOfFrame,
									  unsigned int nCurrentXpos,unsigned int nCurrentYpos )
{
	m_nID = nID;

	m_nNumOfFrameX = nNumOfFrameX;
	m_nNumOfFrameY = nNumOfFrameY;

	//for counting easy..
	m_nNumberOfAnimateFrame = nAllOfFrame-1;

	//default init..
	m_nImageTotalWidth = m_nFrameImageSIzeWidth * nNumOfFrameX;
	m_nImageTotalHeight = m_nFrameImageSizeHeight * nNumOfFrameY;

	m_nCurrentXpos = nCurrentXpos;
	m_nCurrentYpos = nCurrentYpos;

	m_nCurrentEndXpos = nCurrentXpos + m_nFrameImageSIzeWidth;
	m_nCurrentEndYpos = nCurrentYpos + m_nFrameImageSizeHeight;

	m_nStartXPos = nCurrentXpos;
	m_nStartYPos = nCurrentYpos;

	m_nEndXPos = nCurrentXpos + m_nFrameImageSIzeWidth;
	m_nEndYPos = nCurrentYpos + m_nFrameImageSizeHeight;

	m_nCenterXpos = m_nCurrentXpos + (m_nFrameImageSIzeWidth/2);
	m_nCenterYpos = m_nCurrentYpos + (m_nFrameImageSizeHeight/2);

	m_nCurrentFrame = 0;
}

void _XTextureAnimation::InitFrame()
{
	m_nCurrentFrame = 0;
	m_dwCurrentCheckTime = 0;
	
	m_nCurrentXpos = m_nStartXPos;
	m_nCurrentYpos = m_nStartYPos;

	m_nCurrentEndXpos = m_nEndXPos;
	m_nCurrentEndYpos = m_nEndYPos;

	m_nAniCurrentLoopCount = 0;
	m_bIsEndAnimate = false;

	m_bReverse = false;
}

void _XTextureAnimation::NextFrame()
{
	//if Animation looping end
	if( m_nAniLoopCount == m_nAniCurrentLoopCount )
	{
		//Zero value is infinite
		if( m_nAniLoopCount != 0)
		{
			m_bIsEndAnimate = true;

			if( m_bDrawFirstAnimate )
			{
				m_nCurrentXpos = m_nStartXPos;
				m_nCurrentYpos = m_nStartYPos;
				
				m_nCurrentEndXpos = m_nEndXPos;
				m_nCurrentEndYpos = m_nEndYPos;
			}

			return;// return error code
		}
	}
	
	//animation infinity..
	if(m_nCurrentFrame > m_nNumberOfAnimateFrame)
	{
		//return first		
		m_nCurrentFrame = 0;

		m_nCurrentXpos = m_nStartXPos;
		m_nCurrentYpos = m_nStartYPos;
		
		m_nCurrentEndXpos = m_nEndXPos;
		m_nCurrentEndYpos = m_nEndYPos;
		//loop count add
		m_nAniCurrentLoopCount++;
		m_nCurrentFrame++;

		//return m_nCurrentFrame;
	}
	else
	{
		int colum;
		int row;

		colum = m_nCurrentFrame % m_nNumOfFrameX;
		row   = m_nCurrentFrame / m_nNumOfFrameX;

		m_nCurrentXpos = m_nStartXPos + (colum * m_nFrameImageSIzeWidth);
		m_nCurrentYpos = m_nStartYPos + (row * m_nFrameImageSizeHeight);

		m_nCurrentEndXpos = m_nEndXPos + (colum * m_nFrameImageSIzeWidth);
		m_nCurrentEndYpos = m_nEndYPos + (row * m_nFrameImageSizeHeight);
		
		m_nCurrentFrame++;
	}
}

void _XTextureAnimation::ReverseFrame()
{
	//if Animation looping end
	if( m_nAniLoopCount == m_nAniCurrentLoopCount )
	{
		//Zero value is infinite
		if( m_nAniLoopCount != 0)
		{
			m_bIsEndAnimate = true;
			return;// return error code
		}
	}

	int nTempframe = m_nNumberOfAnimateFrame;
	if(m_nCurrentFrame > nTempframe)
	{
		m_nAniCurrentLoopCount++;
		m_nCurrentFrame = m_nNumberOfAnimateFrame - 1;
		m_bReverse = true;
	}
	else if( m_nCurrentFrame < 0 )
	{
		m_nCurrentFrame = 1;
		m_bReverse = false;
	}

	if(m_bReverse)
	{
		int colum;
		int row;

		colum = m_nCurrentFrame % m_nNumOfFrameX;
		row   = m_nCurrentFrame / m_nNumOfFrameX;

		m_nCurrentXpos = m_nStartXPos + (colum * m_nFrameImageSIzeWidth);
		m_nCurrentYpos = m_nStartYPos + (row * m_nFrameImageSizeHeight);

		m_nCurrentEndXpos = m_nEndXPos + (colum * m_nFrameImageSIzeWidth);
		m_nCurrentEndYpos = m_nEndYPos + (row * m_nFrameImageSizeHeight);
		
		m_nCurrentFrame--;
	}
	else
	{
		int colum;
		int row;

		colum = m_nCurrentFrame % m_nNumOfFrameX;
		row   = m_nCurrentFrame / m_nNumOfFrameX;

		m_nCurrentXpos = m_nStartXPos + (colum * m_nFrameImageSIzeWidth);
		m_nCurrentYpos = m_nStartYPos + (row * m_nFrameImageSizeHeight);

		m_nCurrentEndXpos = m_nEndXPos + (colum * m_nFrameImageSIzeWidth);
		m_nCurrentEndYpos = m_nEndYPos + (row * m_nFrameImageSizeHeight);
		
		m_nCurrentFrame++;
	}
}

//윈도우 보여줌
void _XTextureAnimation::ShowWindow( BOOL show )
{
	InitFrame();
	m_pImage->ShowWindow( show );
}

//지정된 윈도우에 추가
void _XTextureAnimation::SetWindow( _XWindow* pWindow )
{
	if( pWindow )
		pWindow->InsertChildObject( m_pImage );
}

void _XTextureAnimation::SetDrawPostion( int X, int Y )
{
	m_pImage->MoveWindow( X, Y );
}