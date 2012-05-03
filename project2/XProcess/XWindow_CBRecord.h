// XWindow_CBRecord.h: interface for the _XWindow_CBRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_CBRECORD_H__E643B271_CBEB_4ACA_A135_6A0841A01516__INCLUDED_)
#define AFX_XWINDOW_CBRECORD_H__E643B271_CBEB_4ACA_A135_6A0841A01516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_CBRecord : public _XWindow
{
private :
	_XListBox*		m_lstboxOrgRecord;
	int				m_nRecordCount;
	
public:
	_XWindow_CBRecord();
	virtual ~_XWindow_CBRecord();

	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void			DeleteAll(void);
	void			InsertRecord(_PerOrgRecord orgrecord);
	
};

#endif // !defined(AFX_XWINDOW_CBRECORD_H__E643B271_CBEB_4ACA_A135_6A0841A01516__INCLUDED_)
