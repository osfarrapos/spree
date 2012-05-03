/*----------------------------------------------------------------------
Copyright (c) 1998,1999 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	HTMLTextBlock.h
Owner:	russf@gipsysoft.com
Purpose:	<Description of module>.
----------------------------------------------------------------------*/
#ifndef HTMLTEXTBLOCK_H
#define HTMLTEXTBLOCK_H

class CHTMLTextBlock : public CHTMLParagraphObject			//txt
//
//	Text block.
//	Has some text.
{
public:
	CHTMLTextBlock( const CTextABC &strText, const HTMLFontDef * pFont, COLORREF crFore, bool bBackground, COLORREF crBack, bool bPreformatted );

	virtual bool IsEmpty() const;
#ifdef _DEBUG
	virtual void Dump() const;
#endif	//	_DEBUG
	virtual void AddDisplayElements( class CHTMLSectionCreator *psc );

	StringClass m_strText;
	const HTMLFontDef * m_pFont;
	COLORREF m_crFore;
	bool m_bPreformatted;
	bool m_bBackground;
	COLORREF m_crBack;

private:
	CHTMLTextBlock();
	CHTMLTextBlock( const CHTMLTextBlock &);
	CHTMLTextBlock& operator =( const CHTMLTextBlock &);
};

#endif //HTMLTEXTBLOCK_H