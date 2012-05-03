#ifndef _XTIMER_HEADER
#define _XTIMER_HEADER


#define	MAX_TIMER_COUNT		20						// 최대 timer 수


class _XTimer
{
private:	// 멤버 
	float	m_thistime[MAX_TIMER_COUNT];			// 현재의 시간을 저장한다.
	float	m_oldtime[MAX_TIMER_COUNT];				// 마지막 프레임이 출력된 시간을 저장한다.

	int		m_thisframe[MAX_TIMER_COUNT];			// 현재 프레임 

	int		m_fps[MAX_TIMER_COUNT];					// 현재 Frame Per Second





private:	// 함수 



public:		// 함수
	_XTimer();
	virtual ~_XTimer();



	// timer count
	void	Init(int tm);												// id값을 초기화 한다.
	void	Init_All();													// id값을 초기화 한다.
	void    UnInit(int tm);												// id값을 최대값으로 초기화 한다.
	void    UnInit_All();												// id값을 최대값으로 초기화 한다.

	BOOL	FrameSkip(int tm, float frame);								// 프레임 스킵함수

	int		GetFPS(int tm)			{	return 	this->m_fps[tm];	}	// fps를 얻어온다.
	float	GetThisTime( int tm )	{	return  this->m_thistime[tm];}  

	void	Clear();													// info clear
	
};



#endif


