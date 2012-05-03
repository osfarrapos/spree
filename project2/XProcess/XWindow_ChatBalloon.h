#ifndef _XWINDDOW_CHATBALLOON_H_
#define _XWINDDOW_CHATBALLOON_H_

#include "XKernel.h"

#define _XDEF_CHATBALLOONSEPARATECOUNT	5

class _XWindow_ChatBalloon 
{
public:
	_XStringSplitInfo	m_StringSpritInfo;	
	int					m_SplitCount;
	LPTSTR				m_SplitMessage[_XDEF_CHATBALLOONSEPARATECOUNT];
	int					m_MaxWidth;
	LONG				m_Counter;
	BOOL				m_Show;
	D3DCOLOR			m_MessageColor;	
public:
	_XWindow_ChatBalloon();
	virtual ~_XWindow_ChatBalloon();
	
	void SetMessage( LPTSTR message );
	void Draw(   int X, int Y   );
};

#endif //_XWINDDOW_CHATBALLOON_H_