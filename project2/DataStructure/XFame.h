// XFame.h: interface for the _XFame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFAME_H__9AFBC6F9_836E_4F1B_B222_D3087DC76CC5__INCLUDED_)
#define AFX_XFAME_H__9AFBC6F9_836E_4F1B_B222_D3087DC76CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XFame  
{
public :
	int			m_Index;
	int			m_FameRange;
	int			m_InfamyRange;
	TCHAR		m_FameName[256];
	TCHAR		m_InfamyName[256];

public:
	_XFame();
	virtual ~_XFame();
	
	static int	GetFameIndex(int value);
	static int	GetInfamyIndex(int value);
	static BOOL CheckItemFame(int index);

};

#endif // !defined(AFX_XFAME_H__9AFBC6F9_836E_4F1B_B222_D3087DC76CC5__INCLUDED_)
