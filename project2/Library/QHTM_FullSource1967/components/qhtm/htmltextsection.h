/*----------------------------------------------------------------------
Copyright (c) 1998 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.

File:	HTMLTextSection.h
Owner:	russf@gipsysoft.com
Purpose:	Simple drawn text object.
----------------------------------------------------------------------*/
#ifndef HTMLTEXTSECTION_H
#define HTMLTEXTSECTION_H

#ifndef HTMLSECTIONABC_H
	#include "HTMLSectionABC.h"
#endif	//	HTMLSECTIONABC_H


class CHTMLTextSection : public CHTMLSectionABC
{
public:
	CHTMLTextSection( CHTMLSection * pParent, LPCTSTR pcszText, int nLength, GS::FontDef &fdef, COLORREF crFore, bool bBackground, COLORREF crBack );
	virtual ~CHTMLTextSection();

	virtual void OnDraw( GS::CDrawContext &dc );
private:
	StringClass m_str;
	GS::FontDef m_fdef;
	COLORREF m_crFore;
	bool m_bBackground;
	COLORREF m_crBack;

private:
	CHTMLTextSection();
	CHTMLTextSection( const CHTMLTextSection & );
	CHTMLTextSection& operator = ( const CHTMLTextSection & );
};

#endif //HTMLTEXTSECTION_H