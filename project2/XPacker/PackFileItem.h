// PackFileItem.h: interface for the CPackFileItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKFILEITEM_H__E6947B24_6C7D_11D5_A852_00022A009AE8__INCLUDED_)
#define AFX_PACKFILEITEM_H__E6947B24_6C7D_11D5_A852_00022A009AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Xitem.h"

class CPackFileItem : public item  
{
public:

	SHFILEINFO  rFileInfo;

	long        rFileSize;

	TCHAR		m_OrgFileName[ 512 ];

	CPackFileItem();
	CPackFileItem( LPCTSTR filename );
	virtual ~CPackFileItem();

	void setData( SHFILEINFO fileinfo ){ rFileInfo = fileinfo; }
	void setFileInformations( LPCTSTR filename );

};

#endif // !defined(AFX_PACKFILEITEM_H__E6947B24_6C7D_11D5_A852_00022A009AE8__INCLUDED_)
