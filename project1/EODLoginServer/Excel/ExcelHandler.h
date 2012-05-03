// ExcelHandler.h: interface for the CExcelHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCELHANDLER_H__1D8C68B3_3B60_4042_B9ED_5C490A4766CA__INCLUDED_)
#define AFX_EXCELHANDLER_H__1D8C68B3_3B60_4042_B9ED_5C490A4766CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Excel.h"

class CExcelHandler  
{
public:
	CExcelHandler();
	virtual ~CExcelHandler();

public:
	void    SortColumn  (LPCTSTR strCellFrom, LPCTSTR strCellTo);
	void    DrawCellBox (LPCTSTR strCellFrom, LPCTSTR strCellTo);
	void    SetStyle    (LPCTSTR strCellFrom, LPCTSTR strCellTo, const long &lBold, 
                         const COLORREF &clrFont, const long &lSize, 
                         LPCTSTR strFontName, const long &lAlignment = 3, 
                         const BOOL &bMerge = FALSE);
	void    AddString   (LPCTSTR strString, LPCTSTR strCellFrom, LPCTSTR strCellTo);
	void    AddPicture  (LPCTSTR strFilePath, const CRect &rcPosition);
	BOOL    AddSheet    (LPCTSTR strTitle);
	BOOL	SetSheetName(const short &nIndex, LPCTSTR strTitle);
	BOOL    GetWorksheet(const short &nIndex = 1);
	BOOL    GetWorkbook ();
	void    CloseExcel  ();
	BOOL    SaveExcel   (LPCTSTR strFilePath);
	BOOL    RunExcel    ();

    void    SetColumnWidth(LPCTSTR strCell, const long &lWidth);

private:
    _Application m_appExcel;
    Workbooks    m_booksExcel;
    _Workbook    m_bookExcel;
    Worksheets   m_sheetsExcel;
    _Worksheet   m_sheetExcel;
    Range        m_rangeExcel;
    Borders      m_bordersExcel;
    PageSetup    m_pageExcel;
    Styles       m_stylesExcel;

    BOOL         m_bRunning;
};

#endif // !defined(AFX_EXCELHANDLER_H__1D8C68B3_3B60_4042_B9ED_5C490A4766CA__INCLUDED_)
