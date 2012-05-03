BOOL CXScriptStringManagerView::ImportExcelFormat( LPCTSTR filename )
{
	COleVariant covTrue((short)TRUE), covFalse((short)FALSE), 
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	_Application excel_app;
	Workbooks books;
	_Workbook str_book;

	Worksheets sheets;
	_Worksheet temp_sheet;
	Range range;
	int nSheet = 0;

	CString winTitle;

	if(!excel_app.CreateDispatch(_T("Excel.Application")))
	{
		_ISM(_T("ERROR : Can't start Excel application and get application object"));
		return FALSE;
	}

	excel_app.SetVisible(FALSE);

	books = excel_app.GetWorkbooks();

	str_book = books.Open(filename, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);

	sheets = str_book.GetSheets();
	nSheet = sheets.GetCount();

	CString szValue;

	if(nSheet > 0)
	{
		for( int i = 0; i < g_DBTableNameListCount; i++ )
		{
			g_ScriptStringList[i].Release();		
		}

		for( int tableidx = 0; tableidx < g_DBTableNameListCount; tableidx++ )
		{
			BOOL findsheet = FALSE;
			for(int i = 1 ; i <= nSheet ; i++)
			{
				temp_sheet = sheets.GetItem(COleVariant((short)i));
				if((temp_sheet.GetName()).CompareNoCase(g_DBTableNameList[tableidx][0]) == 0) // Find sheet
				{
					findsheet = TRUE;					
				}

				if(findsheet)
					break;
			}

			if( findsheet )
			{
				_ISM( _T("Import : %s"), g_DBTableNameList[tableidx][0] );

				int rowcounter = 2;
				
				while(1)
				{
					szValue = GetV_string(temp_sheet, 0, rowcounter);

					if( szValue.GetLength() == 0 )
					{
						_ISM( _T("%s : End of section"), g_DBTableNameList[tableidx][0] );
						break;
					}

                    CLocalizeDataInfo* pObject = new CLocalizeDataInfo;
					
					pObject->resourceid			= GetV_int(temp_sheet, 0, rowcounter);
					pObject->Version			= GetV_int(temp_sheet, 1, rowcounter);

					CString szTempString;

					szTempString = GetV_string(temp_sheet, 2, rowcounter);
					wcscpy( pObject->KoreanString, szTempString );
					pObject->KoreanOrder		= GetV_int(temp_sheet, 3, rowcounter);
					
					szTempString = GetV_string(temp_sheet, 4, rowcounter);
					wcscpy( pObject->VietnameseString, szTempString  );
					pObject->VietnameseOrder	= GetV_int(temp_sheet, 5, rowcounter);
					
					szTempString = GetV_string(temp_sheet, 6, rowcounter);
					wcscpy( pObject->TaiwaneseString, szTempString );
					pObject->TaiwaneseOrder		= GetV_int(temp_sheet, 7, rowcounter);

					szTempString = GetV_string(temp_sheet, 8, rowcounter);
					wcscpy( pObject->EnglishString, szTempString );
					pObject->EnglishOrder		= GetV_int(temp_sheet, 9, rowcounter);

					szTempString = GetV_string(temp_sheet, 10, rowcounter);
					wcscpy( pObject->ChineseString, szTempString );
					pObject->ChineseOrder		= GetV_int(temp_sheet, 11, rowcounter);
					
					pObject->Modified = TRUE;

					g_ScriptStringList[tableidx].Insert( pObject );

					_ISM_NOMESSAGEBOX( _T("Import text : %s ID : %d"), g_DBTableNameList[i][1], pObject->resourceid );

					rowcounter++;
				}
			}
		}
	}
	else
	{
		_ISM(_T("ERROR : Invalid sheets count [0]"));
		goto _IMPORTEXIT;
	}



_IMPORTEXIT :
	str_book.Close(covFalse, covOptional, covOptional);
	excel_app.Quit();
	excel_app.ReleaseDispatch();


	QueryAllList();

	return TRUE;
}

BOOL CXScriptStringManagerView::ExportExcelFormat( LPCTSTR filename )
{
	CUploadDialog uploaddialog;

	uploaddialog.m_WindowText = _T("Select export language");	
	uploaddialog.m_CheckPermission = FALSE;

	if( uploaddialog.DoModal() != IDOK )
	{
		return FALSE;
	}

	BOOL exportallcategory = FALSE;
	if( AfxMessageBox( _T("Export all category?"), MB_YESNO ) == IDYES )
	{
		exportallcategory = TRUE;
	}

	_ISM( _T("Start exporting... [%s]"), filename );

	CExcelHandler	excelhandler;

	if( !excelhandler.RunExcel() )
	{
		return FALSE;
	}

	excelhandler.SetSheetName( 0, g_DBTableNameList[0][0] );

	for( int i = 1; i < g_DBTableNameListCount; i++ )
	{
		excelhandler.AddSheet( g_DBTableNameList[i][0] );
	}

	CString excelcoord;
	CString exceldata;
	int rowcounter = 0;
	COLORREF color = RGB(0, 0, 0);
	TCHAR	szBuffer[2048];

	for( i = 0; i < g_DBTableNameListCount; i++ )
	{
		if( !exportallcategory )
		{
			if( m_SelectedCategoryTable != i ) 
				continue;
		}

		_ISM( _T("Export [%s] table..."), g_DBTableNameList[i][0] );
		excelhandler.GetWorksheet(i+1);
		excelhandler.SetColumnWidth(_T("A1"), 5 );	 // resource id
		excelhandler.SetColumnWidth(_T("B1"), 4 );  // version

		if( uploaddialog.uploadlanguage[0] )
		{
			excelhandler.SetColumnWidth(_T("C1"), 20 ); // korean
			excelhandler.SetColumnWidth(_T("D1"), 4 ); 
		}
		else
		{		
			excelhandler.SetColumnWidth(_T("C1"), 1 ); // korean
			excelhandler.SetColumnWidth(_T("D1"), 1 ); 
		}

		if( uploaddialog.uploadlanguage[1] )
		{
			excelhandler.SetColumnWidth(_T("E1"), 20 );
			excelhandler.SetColumnWidth(_T("F1"), 4 );
		}
		else
		{
			excelhandler.SetColumnWidth(_T("E1"), 1 );
			excelhandler.SetColumnWidth(_T("F1"), 1 );
		}

		if( uploaddialog.uploadlanguage[2] )
		{
			excelhandler.SetColumnWidth(_T("G1"), 20 );
			excelhandler.SetColumnWidth(_T("H1"), 4 );
		}
		else
		{
			excelhandler.SetColumnWidth(_T("G1"), 1 );
			excelhandler.SetColumnWidth(_T("H1"), 1 );
		}

		if( uploaddialog.uploadlanguage[3] )
		{
			excelhandler.SetColumnWidth(_T("I1"), 20 );
			excelhandler.SetColumnWidth(_T("J1"), 4 );
		}
		else
		{
			excelhandler.SetColumnWidth(_T("I1"), 1 );
			excelhandler.SetColumnWidth(_T("J1"), 1 );
		}

		if( uploaddialog.uploadlanguage[4] )
		{
			excelhandler.SetColumnWidth(_T("K1"), 20 );
			excelhandler.SetColumnWidth(_T("L1"), 4 );
		}
		else
		{
			excelhandler.SetColumnWidth(_T("K1"), 1 );
			excelhandler.SetColumnWidth(_T("L1"), 1 );
		}
		
		//excelhandler.SetStyle( _T("A1"), _T("C30000"), FALSE, color, 9, _T("Arial Unicode MS"), 3, TRUE );

		rowcounter = 1;
		int columncounter = 0;

		excelcoord.Format( _T("A%d"), rowcounter );		
		excelhandler.AddString( _T("Res ID"), excelcoord, excelcoord );

		excelcoord.Format( _T("B%d"), rowcounter );		
		excelhandler.AddString( _T("Version"), excelcoord, excelcoord );

		excelcoord.Format( _T("C%d"), rowcounter );		
		excelhandler.AddString( _T("Korean"), excelcoord, excelcoord );
		excelcoord.Format( _T("D%d"), rowcounter );		
		excelhandler.AddString( _T("Order"), excelcoord, excelcoord );

		excelcoord.Format( _T("E%d"), rowcounter );		
		excelhandler.AddString( _T("Vietnamese"), excelcoord, excelcoord );
		excelcoord.Format( _T("F%d"), rowcounter );		
		excelhandler.AddString( _T("Order"), excelcoord, excelcoord );

		excelcoord.Format( _T("G%d"), rowcounter );		
		excelhandler.AddString( _T("Taiwanese"), excelcoord, excelcoord );
		excelcoord.Format( _T("H%d"), rowcounter );		
		excelhandler.AddString( _T("Order"), excelcoord, excelcoord );

		excelcoord.Format( _T("I%d"), rowcounter );		
		excelhandler.AddString( _T("English"), excelcoord, excelcoord );
		excelcoord.Format( _T("J%d"), rowcounter );		
		excelhandler.AddString( _T("Order"), excelcoord, excelcoord );

		excelcoord.Format( _T("K%d"), rowcounter );		
		excelhandler.AddString( _T("Chinese"), excelcoord, excelcoord );
		excelcoord.Format( _T("L%d"), rowcounter );		
		excelhandler.AddString( _T("Order"), excelcoord, excelcoord );

		rowcounter++;

		svdef_XTEXTSTRING_INFO::iterator iter;
		for(iter = g_ScriptStringList[i].m_StringVector.begin() ; 
			iter != g_ScriptStringList[i].m_StringVector.end() ; iter++)
		{
			CLocalizeDataInfo* plocalizeinfo = (*iter);
			if(plocalizeinfo)
			{
				excelcoord.Format( _T("A%d"), rowcounter );
				exceldata.Format( _T("%d"), plocalizeinfo->resourceid );
				excelhandler.AddString( exceldata, excelcoord, excelcoord );

				excelcoord.Format( _T("B%d"), rowcounter );
				exceldata.Format( _T("%d"), plocalizeinfo->Version );
				excelhandler.AddString( exceldata, excelcoord, excelcoord );

				columncounter = 2;

				for( int j = 0; j < _XLANGUAGE_MAXCOUNT; j++ )
				{
					if( uploaddialog.uploadlanguage[j] )
					{
						excelcoord.Format( _T("%s%d"), excelField[columncounter], rowcounter );

						LPTSTR pString = NULL;
						int    order = 0;

						switch( j )
						{
						default:
						case _XLANGUAGE_INDEX_KOREAN		:	pString = plocalizeinfo->KoreanString;
							order   = plocalizeinfo->KoreanOrder;
							break;
						case _XLANGUAGE_INDEX_VIETNAMESE	:	pString = plocalizeinfo->VietnameseString;
							order   = plocalizeinfo->VietnameseOrder;
							break;
						case _XLANGUAGE_INDEX_TAIWANESE		:	pString = plocalizeinfo->TaiwaneseString;
							order	= plocalizeinfo->TaiwaneseOrder;
							break;
						case _XLANGUAGE_INDEX_ENGLISH		:	pString = plocalizeinfo->EnglishString;
							order	= plocalizeinfo->EnglishOrder;
							break;
						case _XLANGUAGE_INDEX_CHINESE		:	pString = plocalizeinfo->ChineseString;
							order	= plocalizeinfo->ChineseOrder;
							break;
						}

						CString completestring = pString;
						completestring.Replace( _T('\r'), _T('$') );
						completestring.Replace( _T('\n'), _T(';') );

						memset( szBuffer, 0, sizeof(TCHAR) * 2048 );
						wcscpy( szBuffer, completestring );
						int strsize = wcslen( szBuffer );
						if( szBuffer[strsize-2] == _T('$') && szBuffer[strsize-1] == _T(';') )
						{
							szBuffer[strsize-2] = NULL;
						}

						excelhandler.AddString( szBuffer, excelcoord, excelcoord );

						columncounter++;

						excelcoord.Format( _T("%s%d"), excelField[columncounter], rowcounter );
						exceldata.Format( _T("%d"), order );
						excelhandler.AddString( exceldata, excelcoord, excelcoord );

						columncounter++;
					}
					else
					{
						columncounter+=2;
					}
				}

				rowcounter++;

				_ISM( _T("Export : %d"), plocalizeinfo->resourceid );
			}
		}
	}




	excelhandler.SaveExcel( filename );

	excelhandler.CloseExcel();

	_ISM( _T("Export complete!") );	

	return TRUE;
}