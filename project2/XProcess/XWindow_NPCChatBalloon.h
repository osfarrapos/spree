// XWindow_NPCChatBalloon.h: interface for the _XWindow_NPCChatBalloon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_NPCCHATBALLOON_H__DD0B014A_03F7_4CA0_8660_159955395586__INCLUDED_)
#define AFX_XWINDOW_NPCCHATBALLOON_H__DD0B014A_03F7_4CA0_8660_159955395586__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#define _XDEF_NPCCHATBALLOONSEPARATECOUNT		2 // 2004.06.20->oneway48 insert
#define _XDEF_MAXNPCCHATBALLOONLINEWIDTH		180


class _XWindow_NPCChatBalloon  
{
public:
	int					m_SplitCount;
	LPTSTR				m_PrevGreetingMessage[_XDEF_NPCCHATBALLOONSEPARATECOUNT];
	
	TCHAR				m_NPCRoll[256];
	TCHAR				m_NPCName[256];
	


public:	
	void SetNPCNameString(LPTSTR pNPCName, LPTSTR pNPCRoll);
	_XWindow_NPCChatBalloon();
	virtual ~_XWindow_NPCChatBalloon();

	
	void SetMessage( LPTSTR message  );
	void Draw(   int X, int Y  );

};

#endif // !defined(AFX_XWINDOW_NPCCHATBALLOON_H__DD0B014A_03F7_4CA0_8660_159955395586__INCLUDED_)
