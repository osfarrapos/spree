// XProgressBar.h: interface for the _XProgressBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROGRESSBAR_H__CE9DECB6_41BA_4951_9EE1_721EBF0A88A7__INCLUDED_)
#define AFX_XPROGRESSBAR_H__CE9DECB6_41BA_4951_9EE1_721EBF0A88A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XProgressBar  
{
public:
	CRect windowrect;


public:
	_XProgressBar();
	virtual ~_XProgressBar();

	void RedrawWindow( CDC* pDC );
};

#endif // !defined(AFX_XPROGRESSBAR_H__CE9DECB6_41BA_4951_9EE1_721EBF0A88A7__INCLUDED_)
