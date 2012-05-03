// MywndTab.h: interface for the CMywndTab class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYWNDTAB_H__EEED1C8A_DF86_46C9_84A6_262ECA4DEEC4__INCLUDED_)
#define AFX_MYWNDTAB_H__EEED1C8A_DF86_46C9_84A6_262ECA4DEEC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wndTab.h"

class CMywndTab : public CwndTab  
{
public:
	CMywndTab();
	virtual ~CMywndTab();

	virtual void     SelectView( UINT nIndex );
};

#endif // !defined(AFX_MYWNDTAB_H__EEED1C8A_DF86_46C9_84A6_262ECA4DEEC4__INCLUDED_)
