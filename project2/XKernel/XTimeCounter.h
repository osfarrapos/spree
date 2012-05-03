// XTimeCounter.h: interface for the _XTimeCounter class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XTIMECOUNTER_H_
#define _XTIMECOUNTER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTimeCounter  
{
private:
	__int64			m_Frequency;
	FLOAT			m_Resolution;
	__int64			m_Performance_timer_start;
	FLOAT			m_Start;
	FLOAT			m_Elapsed;
	FLOAT			m_ElapsedMiliSecond;
	FLOAT			GetTime();
public:
	
	
public:
	_XTimeCounter();
	virtual ~_XTimeCounter();
	
	void			Init();
	void			Update();
	inline FLOAT	GetElapsedTime( void ){ return m_Elapsed; }
	inline FLOAT	GetElapsedMiliSecondTime( void ){ return m_ElapsedMiliSecond; }
};

#endif // !defined(_XTIMECOUNTER_H_)
