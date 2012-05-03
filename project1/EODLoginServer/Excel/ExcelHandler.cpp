// ExcelHandler.cpp: implementation of the CExcelHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExcelHandler.h"
#include <comdef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib, "comsupp.lib")


void DumpComError(_com_error &e)
{
#ifdef _DEBUG
	CString strError;	
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	strError.Format(_T("Error\n\tCode = %08lx\n\tCode meaning = %s\n\t Source = %s\n\tDescription = %s\n") , 
			e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);

    AfxMessageBox(strError, MB_OK | MB_ICONERROR);
#else
    UNUSED_ALWAYS(e);
#endif	
}

void DumpComError(COleException *e)
{
#ifdef _DEBUG
    LPVOID lpMsg;
	
    ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
                    FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e->m_sc,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsg,
                    0, NULL);
    AfxMessageBox((LPCTSTR)lpMsg, MB_SETFOREGROUND);
    ::LocalFree  (lpMsg);
#else
    UNUSED_ALWAYS(e);
#endif	
}

void DumpComError(COleDispatchException *e)
{
#ifdef _DEBUG
    TCHAR szMsg[512];
    swprintf(szMsg, _T("Run-time error '%d':\n\n%s"), 
            e->m_scError & 0x0000FFFF, e->m_strDescription);
    AfxMessageBox(szMsg, MB_SETFOREGROUND);      
#else
    UNUSED_ALWAYS(e);
#endif	
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExcelHandler::CExcelHandler()
{
    m_bRunning = FALSE;
}

CExcelHandler::~CExcelHandler()
{
    if(m_bRunning)
        m_appExcel.ReleaseDispatch();
}

BOOL CExcelHandler::RunExcel(  )
{
    if(m_bRunning)
    {
        m_appExcel.ReleaseDispatch();
    }

	if(!m_appExcel.CreateDispatch(_T("Excel.application")))
	{
		AfxMessageBox(_T("Could not start Micorosoft Excel!"));
		return FALSE;
	}
#ifdef _DEBUG	
	m_appExcel.SetVisible(!TRUE);
#else
	m_appExcel.SetVisible(FALSE);
#endif

    m_bRunning = TRUE;

    if(!GetWorkbook())
        return FALSE;

    return GetWorksheet();
}

void CExcelHandler::CloseExcel()
{
    if(m_bRunning)
    {
        COleVariant varOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);	
        COleVariant varSave((short)FALSE);

        m_bookExcel.Close(varSave, varOption, varSave);
        m_appExcel .Quit ();
        m_appExcel .ReleaseDispatch();
    }

    m_bRunning = FALSE;
}

BOOL CExcelHandler::GetWorkbook()
{
    if(!m_bRunning)
        return FALSE;

    LPDISPATCH  lpDisp;
    COleVariant varOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);	
    COleVariant varSave((short)FALSE);
    
    try
    {
        lpDisp = m_appExcel.GetWorkbooks();
        m_booksExcel.AttachDispatch(lpDisp);
        
        lpDisp = m_booksExcel.Add(varOption);
        m_bookExcel.AttachDispatch(lpDisp);

        m_stylesExcel = m_bookExcel.GetStyles();
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
        CloseExcel();
        return FALSE;
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
        CloseExcel();
        return FALSE;
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
        CloseExcel();
        return FALSE;
    }
    
    return TRUE;
}

BOOL CExcelHandler::SetSheetName(const short &nIndex, LPCTSTR strTitle)
{
	if(!m_bRunning)
		return FALSE;

	try
	{
		m_sheetExcel.SetName(strTitle);
	}
	catch(COleException *e)
	{
		::DumpComError(e);
		CloseExcel();
		return FALSE;
	}
	catch(COleDispatchException *e)
	{
		::DumpComError(e);
		CloseExcel();
		return FALSE;
	}
	catch(_com_error &e)
	{
		::DumpComError(e);
		CloseExcel();
		return FALSE;
	}

	return TRUE;
}

BOOL CExcelHandler::GetWorksheet(const short &nIndex)
{
    if(!m_bRunning)
        return FALSE;

    LPDISPATCH  lpDisp;
    COleVariant varSheet(nIndex);

    try
    {
        lpDisp = m_bookExcel.GetSheets();
        m_sheetsExcel.AttachDispatch(lpDisp);

        lpDisp = m_sheetsExcel.GetItem(varSheet);
        m_sheetExcel.AttachDispatch(lpDisp);
        //m_sheetExcel.SetName(_T("Sheet1"));
/*
        // 처음 실행시에는 시트를 하나만 둔다.
        // 이후에 그룹별로 시트를 추가시킨다.
        long lCount = m_sheetsExcel.GetCount();
        for(long i = lCount ; i > 1 ; i--)
        {
            COleVariant varIndex(i);
            _Worksheet  sheetExcel;
            lpDisp = m_sheetsExcel.GetItem(varIndex);
            sheetExcel.AttachDispatch(lpDisp);
            sheetExcel.Delete();
        }
*/
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
        CloseExcel();
        return FALSE;
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
        CloseExcel();
        return FALSE;
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
        CloseExcel();
        return FALSE;
    }

    return TRUE;
}

BOOL CExcelHandler::SaveExcel(LPCTSTR strFilePath)
{
    if(!m_bRunning)
        return FALSE;

    COleVariant varFile(strFilePath);
    COleVariant varOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    
    try
    {
//        m_bookExcel.SaveCopyAs(varFile);
        m_bookExcel.SaveAs(varFile, varOption, varOption, varOption, varOption, 
                           varOption, 1, varOption, varOption, varOption, 
                           varOption, varOption);
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
        return FALSE;
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
        return FALSE;
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
        return FALSE;
    }

    return TRUE;
}

void CExcelHandler::AddPicture(LPCTSTR strFilePath, const CRect &rcPosition)
{
    if(!m_bRunning)
        return;

    try
    {
        Shapes shapesExcel = m_sheetExcel.GetShapes(); 

        shapesExcel.AddPicture(strFilePath, 0, 1, (float)rcPosition.left, 
                               (float)rcPosition.top, (float)rcPosition.Width(), 
                               (float)rcPosition.Height());
/*
        Shape shapeExcel = shapesExcel.Item(COleVariant(short(shapesExcel.GetCount()))); 
        double pictWidth = shapeExcel.GetWidth(); 
        m_rangeExcel = m_sheetExcel.GetRange(COleVariant("A1"), COleVariant("H1")); 
        VARIANT varr = m_rangeExcel.GetWidth(); 
        double rangeWidth = varr.dblVal;

        float scale = (float)(rangeWidth/pictWidth); 
        shapeExcel.ScaleWidth (scale, 1, COleVariant(short(0))); 
        shapeExcel.ScaleHeight(scale, 1, COleVariant(short(0)));
*/
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
    }
}

void CExcelHandler::AddString(LPCTSTR strString, LPCTSTR strCellFrom, LPCTSTR strCellTo)
{
    if(!m_bRunning)
        return;

    try
    {
        Range   rangeExcel;

        rangeExcel = m_sheetExcel.GetRange(COleVariant(strCellFrom), COleVariant(strCellTo));
        rangeExcel.SetMergeCells(COleVariant((long)TRUE));
        rangeExcel.SetValue2    (COleVariant(strString));
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
    }
}

void CExcelHandler::SetStyle(LPCTSTR strCellFrom, LPCTSTR strCellTo, 
                             const long &lBold, const COLORREF &clrFont, 
                             const long &lSize, LPCTSTR strFontName, 
                             const long &lAlignment, const BOOL &bMerge)
{
    if(!m_bRunning)
        return;

    try
    {
        Font		fontRange;
        Style       styleRange;
        COleVariant varOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
        Range       rangeExcel;
        CString     strName;
        long        i, lCount;

        lCount = m_stylesExcel.GetCount();
        for(i = 1 ; i <= lCount ; i++)
        {
            styleRange = m_stylesExcel.GetItem(COleVariant(i));
            if(styleRange.GetName() == strCellFrom)
                break;
        }
        // Style 이 존재하지 않는 경우에는 새로 생성시킨다.
        if(i > lCount)
        {
            styleRange = m_stylesExcel.Add(strCellFrom, varOption);
        }

        rangeExcel = m_sheetExcel.GetRange(COleVariant(strCellFrom), COleVariant(strCellTo));
        fontRange  = styleRange.GetFont();
        fontRange.SetBold (COleVariant(lBold));
        fontRange.SetColor(COleVariant((long)clrFont));
        fontRange.SetSize (COleVariant(lSize));
        fontRange.SetName (COleVariant(strFontName));

        styleRange.SetHorizontalAlignment(lAlignment);

        rangeExcel.SetMergeCells(COleVariant((long)bMerge));
        rangeExcel.SetStyle (COleVariant(strCellFrom));
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
    }
}

void CExcelHandler::DrawCellBox(LPCTSTR strCellFrom, LPCTSTR strCellTo)
{
    try
    {
        Range   rangeExcel;
        Borders borders; 

        rangeExcel = m_sheetExcel.GetRange(COleVariant(strCellFrom), COleVariant(strCellTo));
        rangeExcel.Select();

        borders = rangeExcel.GetBorders();		
        borders.SetLineStyle(COleVariant((short)1)); //실선으로 라인 그리기
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
    }
}

BOOL CExcelHandler::AddSheet(LPCTSTR strTitle)
{
    if(!m_bRunning)
        return FALSE;

    try
    {
        COleVariant varOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
        VARIANT     varValue;

        V_VT(&varValue)       = VT_DISPATCH;
        V_DISPATCH(&varValue) = (LPDISPATCH)m_sheetExcel;

        m_sheetExcel = m_sheetsExcel.Add(varOption, varValue, varOption, varOption);
        m_sheetExcel.SetName(strTitle);
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
        return FALSE;
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
        return FALSE;
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
        return FALSE;
    }

    return TRUE;
}

void CExcelHandler::SetColumnWidth(LPCTSTR strCell, const long &lWidth)
{
    if(!m_bRunning)
        return;

    try
    {
        Range   rangeExcel;

        rangeExcel = m_sheetExcel.GetRange(COleVariant(strCell), COleVariant(strCell));
        rangeExcel.SetColumnWidth(COleVariant(lWidth));
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
    }
}

void CExcelHandler::SortColumn(LPCTSTR strCellFrom, LPCTSTR strCellTo)
{
	/*
    if(!m_bRunning)
        return;

    try
    {
        Range   rangeExcel;
        ListColumns listColumns;
        ListColumn  listColumn;

        COleVariant varType((long)1);
        COleVariant varOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
        VARIANT     varColumn1, varColumn2;

        rangeExcel  = m_sheetExcel.GetRange(COleVariant(strCellFrom), COleVariant(strCellTo));
        listColumns = rangeExcel.GetColumns();

        ::VariantInit(&varColumn1);
        ::VariantInit(&varColumn2);
        V_VT(&varColumn1)       = VT_DISPATCH;
        V_DISPATCH(&varColumn1) = (LPDISPATCH)listColumns.GetItem(COleVariant((long)1));
        V_VT(&varColumn2)       = VT_DISPATCH;
        V_DISPATCH(&varColumn2) = (LPDISPATCH)listColumns.GetItem(COleVariant((long)2));

        rangeExcel.Sort(varColumn1, 1, varColumn2, varOption, 1, varOption, 1, 0, 
                        varOption, varOption, 1, 1, 1, 1, 1);

        ::VariantClear(&varColumn1);
        ::VariantClear(&varColumn2);
    }
    catch(COleException *e)
    {
        ::DumpComError(e);
    }
    catch(COleDispatchException *e)
    {
        ::DumpComError(e);
    }
    catch(_com_error &e)
    {
        ::DumpComError(e);
    }
	*/
}
