// XWindow_MnDGroup.cpp: implementation of the XWindow_MnDGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "XProc_ProcessDefine.h"

#include "XWindow_MnDGroup.h"

void __stdcall _XIMECallback_MnDNoticeEditEscape( _XIMEContainer* pIMEContainer )
{

}

void __stdcall _XIMECallback_MnDNoticeEditReturn( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;	
	if( !g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetFocus() ) return;
	
	g_pGroup_Window->m_pMnDWindow->CalculateNoticeTextength();
	// 글자수 제한
	if( g_pGroup_Window->m_pMnDWindow->m_nNoticeLength + g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetLength_ByMBCSSize() >= _XDEF_MAX_NOTICELENGTH ) return;	

	POINT winpos;				
	winpos = g_pGroup_Window->GetWindowPos();
	
	memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, 
			sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
	strcpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetText());
	
	int cursorx = g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetCursorX_ByMBCSSize();
	int length = strlen( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] );
	
	if( g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount-1 == g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos )
	{
		if( cursorx == length )
		{
			g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount++;
			g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos++;
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();
		}
		else
		{
			TCHAR leftstring[64];
			TCHAR rightstring[64];
			memset( leftstring, 0, sizeof(leftstring) );
			memset( rightstring, 0, sizeof(rightstring) );
			strncpy( leftstring, g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], cursorx );
			strncpy( rightstring, g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] + cursorx, length - cursorx );
			
			memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, 
				sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
			strncpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], leftstring, cursorx);
			
			g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount++;
			g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos++;
			
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();			
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.AddString( rightstring, length - cursorx );
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetCursorX_ByMBCSSize(0);
		}
		
		if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
		{			
			g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->MoveWindow( winpos.x + 300, winpos.y + 104 );
			g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetTotalLineCount( g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount );	
			
			if( !g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->Dragging() )
			{
				g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos ); 
			}		
			g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->UpdateData();
			
		}								
		
		int scrollpos = g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->GetLineScrollPos();
		
		g_pGroup_Window->m_pMnDWindow->CalculateNoticeTextength();

		if( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_pMnDWindow->m_nNoticeLength > 46 )
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
		else
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_pMnDWindow->m_nNoticeLength );


		g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.MoveWindow(winpos.x + 14,
			winpos.y + 111 +((g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos - scrollpos)*17) );
		g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetFocus();
		
	}
	else
	{
		int scrollpos = g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->GetLineScrollPos();
		if( cursorx == length )
		{
			for( int i = g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount-1; i > g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos; i--)
			{
				TCHAR buffer[128];
				memset( buffer, 0, sizeof(buffer) );
				strcpy( buffer, g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i ] );
				memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i ], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[i]) );
				memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i + 1 ], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[i+1]) );
				strcpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i + 1 ], buffer);
			}
			g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount++;
			g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos++;
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();
		}
		else
		{
			
			scrollpos = g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->GetLineScrollPos();

			for( int i = g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount-1; i > g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos; i--)
			{
				TCHAR buffer[64];
				memset( buffer, 0, sizeof(buffer));
				strcpy( buffer, g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i ] );
				memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i ], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[i]) );
				memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i + 1 ], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[i+1]) );
				strcpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i + 1 ], buffer);
			}
			TCHAR leftstring[64];
			TCHAR rightstring[64];
			memset( leftstring, 0, sizeof(leftstring) );
			memset( rightstring, 0, sizeof(rightstring) );
			strncpy( leftstring, g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], cursorx );
			strncpy( rightstring, g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] + cursorx, length - cursorx );
			
			memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, 
				sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
			strncpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] , leftstring, cursorx);
			
			g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount++;
			g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos++;
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.AddString( rightstring, length - cursorx );
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetCursorX_ByMBCSSize(0);
		}
		
		if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
		{					
			g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->MoveWindow( winpos.x + 300, winpos.y + 104 );
			g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetTotalLineCount( g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount );	
			g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->UpdateData();
		}								
		
		
		g_pGroup_Window->CalculateNoticeTextength();
		
		if( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_pMnDWindow->m_nNoticeLength > 46 )
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
		else
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_pMnDWindow->m_nNoticeLength );
		
		int linecount = 0;
		if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos  >= g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos + 4)
		{
			if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
			{			
				g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos + 1 );
			}					
			linecount = 3;			
		}
		else
		{
			if( scrollpos == g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount - 4 - 1)
			{
				linecount = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos - 1 - g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos;
			}
			else
			{
				linecount = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos - g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos;
			}
		}
		
		g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111  +(linecount*17) );
		g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetFocus();
	}
}

void __stdcall _XIMECallback_MnDNoticeEditUp( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;	
	if( !g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetFocus() ) return;
	
	if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos == 0 ) return;
	POINT winpos;				
	winpos = g_pGroup_Window->GetWindowPos();
	
	memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, 
		sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
	strcpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] , g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetText());
		
	int cursorx = g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetCursorX_ByMBCSSize();
	
	_XIMEKERNEL.ResetCombinationBuffer();
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();
	g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos--;
	
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
	int length = strlen( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] );
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.AddString( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], length );
	

	memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetCursorX_ByMBCSSize( cursorx );
	
	int linecount = 0;
	if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos  < g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos)
	{
		if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
		{			
			g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos );
		}								
	}
	else
	{
		linecount = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos - g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos;
	}

	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.MoveWindow(winpos.x + 15, winpos.y + 111 +(linecount*17) );
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetFocus();	
}

void __stdcall _XIMECallback_MnDNoticeEditDown( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;	
	if( !g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetFocus() ) return;
	
	if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos == 0 ) return;
	if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos == g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount-1 ) return;
	POINT winpos;				
	winpos = g_pGroup_Window->GetWindowPos();
	
	memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, 
		sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
	strcpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] , g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetText());
		
	int cursorx = g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetCursorX_ByMBCSSize();
	
	_XIMEKERNEL.ResetCombinationBuffer();
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();
	g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos++;
	
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
	int length = strlen( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] );
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.AddString( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], length );
	

	memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetCursorX_ByMBCSSize( cursorx );
	
	int linecount = 0;
	if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos  >= g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos + 4)
	{
		if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
		{			
			g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos + 1 );
		}
		linecount = 3;
	}
	else
	{
		linecount = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos - g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos;
	}

	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
	g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetFocus();
}

void __stdcall _XIMECallback_MnDNoticeEditLeft( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;	
	if( !g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetFocus() ) return;

	int cursorx = g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetCursorX_ByMBCSSize();
	int length = g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetLength_ByMBCSSize();
	
	if( cursorx == 0 )
	{
		if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos != 0 )
		{
			POINT winpos;				
			winpos = g_pGroup_Window->GetWindowPos();
			
			memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, 
					sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
			strcpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] , g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetText());
						
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();
			g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos--;
			
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
			int length = strlen( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] );
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.AddString( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], length );
			
			memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetCursorX_ByMBCSSize( length );
			
			int linecount = 0;
			if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos  < g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos)
			{
				if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
				{			
					g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos );
				}								
			}
			else
			{
				linecount = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos - g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos;
			}
			
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetFocus();
		}
	}
}

void __stdcall _XIMECallback_MnDNoticeEditRight( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;
	if( !g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetFocus() ) return;
	
	int cursorx = g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetCursorX_ByMBCSSize();
	int length = g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetLength_ByMBCSSize();
	
	if( cursorx == length )
	{
		if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos != g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount-1 )
		{
			POINT winpos;				
			winpos = g_pGroup_Window->GetWindowPos();
			
			memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, 
					sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
			strcpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] , g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetText());
			
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();
			g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos++;
			
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
			int length = strlen( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] );
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.AddString( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], length );
			
			memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetCursorX_ByMBCSSize( 0 );
			
			int linecount = 0;
			if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos  >= g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos + 4)
			{
				if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
				{			
					g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos + 1 );
				}					
				linecount = 3;
			}
			else
			{
				linecount = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos - g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos;
			}
			
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
			g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetFocus();
						
		}
	}
}

void __stdcall _XIMECallback_MnDNoticeEditBack( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;
	if( !g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetFocus() ) return;
	
	int cursorx = g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetCursorX_ByMBCSSize();
	int length = g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetLength_ByMBCSSize();
	
	if( cursorx == 0 )
	{
		if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos != 0 )
		{
			POINT winpos;				
			winpos = g_pGroup_Window->GetWindowPos();
						
			if( strlen( g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.GetText() ) == 0 )
			{
				
				if( g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount - 1 == g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos )
				{
					
					g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount--;
					
					_XIMEKERNEL.ResetCombinationBuffer();
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();
					g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos--;
					
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
					int length = strlen( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] );
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.AddString( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], length );
					
					memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos]));
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetCursorX_ByMBCSSize( length );
					
					g_pGroup_Window->CalculateNoticeTextength();
					
					if( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_pMnDWindow->m_nNoticeLength > 46 )
						g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
					else
						g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_pMnDWindow->m_nNoticeLength );
					
					int linecount;

					if( g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount >= 4 )
					{
						linecount = 3;
					}
					else
					{ 
						linecount = g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount - 1;
					}


					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetFocus();
					
					if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
					{
						g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetTotalLineCount( g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount );	
						g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->UpdateData();						
					}
				}
				else
				{
					int scrollpos = g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->GetLineScrollPos();
										
					for( int i = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos; i < g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount - 1; i++)
					{
						TCHAR buffer[64];
						memset( buffer, 0, sizeof(TCHAR)*64 );
						strcpy( buffer, g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i + 1 ] );
						memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i ], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i ]) );
						memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i + 1 ], 0, sizeof(g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i + 1 ]));
						strcpy( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[ i ], buffer);
					}
					
					g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount--;
					
					_XIMEKERNEL.ResetCombinationBuffer();
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.ClearBuffer();
					g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos--;
					
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
					int length = strlen( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos] );
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.AddString( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], length );
					
					memset( g_pGroup_Window->m_pMnDWindow->m_szNoticeTempText[g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetCursorX_ByMBCSSize( length );
					
					
					g_pGroup_Window->CalculateNoticeTextength();
					
					if( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_pMnDWindow->m_nNoticeLength > 46 )
						g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( 46 );
					else
						g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetMaxLength( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_pMnDWindow->m_nNoticeLength );
					
					if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
					{
						g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetTotalLineCount( g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount );	
						g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->UpdateData();
						
					}
					
					int linecount = 0;
					if( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos  < g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos)
					{
						if( g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar )
						{			
							g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos + 1 );
						}								
					}
					else
					{
						g_pGroup_Window->m_pMnDWindow->m_pNoticeScrollBar->SetLineScrollPos( scrollpos );

						if( scrollpos == 0 )
						{
							linecount = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos - g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos;
						}
						else
						{
							if( scrollpos == g_pGroup_Window->m_pMnDWindow->m_nNoticeTotalCount - 4 + 1)
							{
								linecount = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos + 1 - g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos;
							}
							else
							{
								linecount = g_pGroup_Window->m_pMnDWindow->m_nCurrentNoticeLinePos - g_pGroup_Window->m_pMnDWindow->m_nNoticeScrollPos;
							}
						}
					}
					
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
					g_pGroup_Window->m_pMnDWindow->m_MnDNoticeIMEControl.SetFocus();
					
				}				
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MnDGroup::_XWindow_MnDGroup() : m_pCotributionBorder(NULL),
										 m_pContributionGaugeLeftBorder(NULL),
										 m_pContributionGaugeRightBorder(NULL),
										 m_pImageContributionRateLeftBorder(NULL),
										 m_pImageContributionRateRightBorder(NULL),
										 m_pMnDInfoListBox(NULL),
										 m_pNoticeWrite(NULL),
										 m_pNoticemodify(NULL),
										 m_pBtnGroupLeave(NULL),
										 m_pNoticeScrollBar(NULL),
										 m_nPopupCount(0),
										 m_nNoticeScrollPos(0),
										 m_nNoticeLength(0),
										 m_nNoticeTotalCount(1),
										 m_nCurrentNoticeLinePos(0),
										 m_nNoticeViewTextTotalCount(0),
										 m_nNoticeMode(_XMNDNOTICEMODE_READ),
										 m_stNoticeText(),
										 m_stSendNoticeTempText(),
										 m_vtMnDGroup()
{
}

_XWindow_MnDGroup::~_XWindow_MnDGroup()
{

}

BOOL _XWindow_MnDGroup::Initialize()
{
	//초기화
	memset(m_szNoticeTempText, 0, sizeof(m_szNoticeTempText));
	memset(m_szNoticeViewText, 0, sizeof(m_szNoticeViewText));

	// 기여도 게이지, 가산률 -----------------------------------------------------------------------=
	int charresourceindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 120, 81 }, { 216, 8 },	
			charresourceindex, -1, &g_MainInterfaceTextureArchive
	};

	m_pCotributionBorder = new _XStickGauge;
	m_pCotributionBorder->Create( gaugestruct );	
	m_pCotributionBorder->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 51, 142, 59 );
	m_pCotributionBorder->SetFactorDraw(TRUE);
	m_pCotributionBorder->SetGaugeImageScaleFactor( 1.5f );
	m_pCotributionBorder->SetGaugeFactor( 0.0f );
	g_pGroup_Window->InsertChildObject( m_pCotributionBorder ); //게이지

	m_pContributionGaugeLeftBorder = new _XImageStatic;
	m_pContributionGaugeLeftBorder->Create( 118, 79, 229, 91, &g_MainInterfaceTextureArchive, charresourceindex );
	m_pContributionGaugeLeftBorder->SetClipRect( 0, 60, 109, 72 );
	g_pGroup_Window->InsertChildObject( m_pContributionGaugeLeftBorder ); //게이지 왼쪽

	m_pContributionGaugeRightBorder = new _XImageStatic;
	m_pContributionGaugeRightBorder->Create( 336, 79, 228, 91, &g_MainInterfaceTextureArchive, charresourceindex );
	m_pContributionGaugeRightBorder->SetClipRect( 0, 60, 109, 72 );
	m_pContributionGaugeRightBorder->SetScale( -1.0f, 1.0f );
	g_pGroup_Window->InsertChildObject( m_pContributionGaugeRightBorder ); //게이지 오른쪽 (왼쪽 그림을 뒤집었다)

	int systemimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE leavebtnstruct = 
	{ TRUE, { 8, 361 }, { 60, 24 }, 
		_XDEF_GROUP_MASTERNDISCIPLE_LEAVEBTN,
		systemimageindex, systemimageindex, systemimageindex,
		&g_MainInterfaceTextureArchive 
	};	

	m_pBtnGroupLeave = new _XButton;
	m_pBtnGroupLeave->Create( leavebtnstruct );		
	m_pBtnGroupLeave->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	m_pBtnGroupLeave->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
	m_pBtnGroupLeave->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	m_pBtnGroupLeave->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3858 );
	g_pGroup_Window->InsertChildObject( m_pBtnGroupLeave ); // 하산 버튼

	// 글쓰기, 수정 버튼
	_XBTN_STRUCTURE noticebtnstruct = { TRUE,  
	{ 316, 101 }, 
	{ 24, 42 }, 
		_XDEF_GROUP_MASTERNDISCIPLE_NOTICEWRITE,
		g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" ), 
		g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" ),
		g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" ),
		&g_MainInterfaceTextureArchive };
	
	m_pNoticeWrite = new _XButton;
	m_pNoticeWrite->Create( noticebtnstruct );		
	//m_pNoticeWrite->SetCallBackFunction_ToolTip( _XCallback_MM_Write );
	m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  47, 192, 71, 234 );	
	m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 20, 192, 44, 234 );	
	m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  74, 192, 98, 234 );	
	m_pNoticeWrite->EnableWindow(FALSE);	
	g_pGroup_Window->InsertChildObject( m_pNoticeWrite );

	noticebtnstruct.position.y += 41;
	noticebtnstruct.commandid = 5590;

	m_pNoticemodify = new _XButton;
	m_pNoticemodify->Create( noticebtnstruct );		
	//m_pNoticemodify->SetCallBackFunction_ToolTip( _XCallback_MM_Modify );
	m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  128, 99, 152, 141 );
	m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 101, 99, 125, 141 );	
	m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  155, 99, 179, 141 );
	m_pNoticemodify->EnableWindow(FALSE);
	g_pGroup_Window->InsertChildObject( m_pNoticemodify );

	//하단 기여도
	int messengerimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	m_pImageContributionRateLeftBorder = new _XImageStatic;
	m_pImageContributionRateLeftBorder->Create( 173, 362, 254, 384, &g_MainInterfaceTextureArchive, messengerimageindex );
	m_pImageContributionRateLeftBorder->SetClipRect( 33, 229, 114, 251 );
	g_pGroup_Window->InsertChildObject( m_pImageContributionRateLeftBorder);

	m_pImageContributionRateRightBorder = new _XImageStatic;
	m_pImageContributionRateRightBorder->Create( 335, 362, 416, 384, &g_MainInterfaceTextureArchive, messengerimageindex );
	m_pImageContributionRateRightBorder->SetClipRect( 33, 229, 114, 251 );
	m_pImageContributionRateRightBorder->SetScale( -1.0f, 1.0f );
	g_pGroup_Window->InsertChildObject( m_pImageContributionRateRightBorder );

	int scrollresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga");
	_XLISTBOX_STRUCTURE mndstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 4, 189 },		// 윈도우 좌표
		{ 337, 167 },	// 윈도우 사이즈
		_XDEF_GROUP_MASTERNDISCIPLE_LISTBOX,			// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		20,												// 리스트 아이템 라인 피치
		8,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치	
		// scroll bar property
		{ 12, 167 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		100,												// 전체 리스트 갯수
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
	};

	m_pMnDInfoListBox = new _XListBox;
	m_pMnDInfoListBox->Create( mndstructure );
	_XStatic* tempstatic = m_pMnDInfoListBox->GetBorderStatic();
	tempstatic->SetFaceColor(0xB2000000);
	_XVScrollBar* pOrgScrollBar = m_pMnDInfoListBox->GetScrollBarObject();
	if( pOrgScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pOrgScrollBar->SetClipRect( cliprectstructure );	
	}
	m_pMnDInfoListBox->SetAlwaysShowScrollBarMode(TRUE);

	//사제는 아이콘이 필요없으니까,,
	POINT org_columnoffset1 = { 25, 4 };
	m_pMnDInfoListBox->SetColumnOffset(0, org_columnoffset1);				// column offset
	m_pMnDInfoListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pMnDInfoListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_pMnDInfoListBox->SetColumnWidth(0, 320);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_pMnDInfoListBox->SetTrackBarColor( 0X33CDBEA2 );	
	m_pMnDInfoListBox->SetTextXOffset(10);
	m_pMnDInfoListBox->SetTrackWidth(320);
	m_pMnDInfoListBox->ShowWindow(FALSE);
	g_pGroup_Window->InsertChildObject(m_pMnDInfoListBox);


	// 공지창 IME 
	m_MnDNoticeIMEControl.ResetContainer();
	m_MnDNoticeIMEControl.ResizeWindow(290, 15);
	m_MnDNoticeIMEControl.ShowWindow(FALSE);
	m_MnDNoticeIMEControl.SetMaxLength(46);
	m_MnDNoticeIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );

	m_MnDNoticeIMEControl.SetEscapeKeyCallback( _XIMECallback_MnDNoticeEditEscape );
	m_MnDNoticeIMEControl.SetReturnKeyCallback( _XIMECallback_MnDNoticeEditReturn );
	m_MnDNoticeIMEControl.SetUpArrowKeyCallback( _XIMECallback_MnDNoticeEditUp );
	m_MnDNoticeIMEControl.SetDownArrowKeyCallback( _XIMECallback_MnDNoticeEditDown );
	m_MnDNoticeIMEControl.SetLeftArrowKeyCallback( _XIMECallback_MnDNoticeEditLeft );
	m_MnDNoticeIMEControl.SetRightArrowKeyCallback( _XIMECallback_MnDNoticeEditRight );
	m_MnDNoticeIMEControl.SetBackSpaceKeyCallback( _XIMECallback_MnDNoticeEditBack );


	// 공지창 스크롤바 
	m_pNoticeScrollBar = new _XVScrollBar;
	_XSCRBAR_STRUCTURE noticescrbarstructure = 
	{
		TRUE, { 300, 104 }, 
			12, 77, // width, height
			12,		 // btnysize
			22,		 // trackbar size
			7, 4, 0,
			scrollresourceindex,
			scrollresourceindex,
			scrollresourceindex,
			scrollresourceindex,
			scrollresourceindex,
			&g_MainInterfaceTextureArchive
	};
	
	m_pNoticeScrollBar->Create( noticescrbarstructure );
	m_pNoticeScrollBar->ShowWindow( TRUE );
	m_pNoticeScrollBar->SetViewNullBorder( TRUE );
	m_pNoticeScrollBar->UpdateData();
	
	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_pNoticeScrollBar->SetClipRect( cliprectstructure );
	g_pGroup_Window->InsertChildObject(m_pNoticeScrollBar);


	// 팝업 버튼들----------------------------------------------------------------------------------=
	int popupimageindex = g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName);

	_XBTN_STRUCTURE popupbtnstruct = 
	{
		TRUE, 
		{4, 4},
		{88, 19},
		_XDEF_GROUP_MASTERNDISCIPLE_EMAIL,
		popupimageindex,
		popupimageindex,
		popupimageindex,
		&g_MainInterfaceTextureArchive
	};

	for( int i = 0; i < 4; i++ )
	{
		m_pPopUpButton[i] = NULL;
		m_pPopUpButton[i] = new _XButton;
		m_pPopUpButton[i]->Create(popupbtnstruct);
		m_pPopUpButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_pPopUpButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_pPopUpButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_pPopUpButton[i]->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));		
		m_pPopUpButton[i]->EnableWindow(TRUE);
		m_pPopUpButton[i]->ShowWindow(FALSE);

		g_pGroup_Window->InsertChildObject(m_pPopUpButton[i]);

		popupbtnstruct.position.y += 18;
		popupbtnstruct.commandid++;
	}
	
	m_pPopUpButton[0]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_2088); //전서구보내기
	m_pPopUpButton[1]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_2089); //귓속말 보내기
	m_pPopUpButton[2]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_REQUESTPARTY); //동행 신청
	m_pPopUpButton[3]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_COMPULSIONDROP); //이거 하산으로 바꾸자

	return TRUE;
}

void _XWindow_MnDGroup::ResetData()
{
	vector<MnDGroupList*>::iterator MnDiter = m_vtMnDGroup.begin();
	for(; MnDiter != m_vtMnDGroup.end(); ++MnDiter)
	{
		if( (*MnDiter) )
		{
			delete (*MnDiter);
			(*MnDiter) = NULL;
		}
	}
	m_vtMnDGroup.clear();

	m_pMnDInfoListBox->DeleteAllItem();
}

void _XWindow_MnDGroup::ShowMnDWindow(BOOL bShow)
{
	memset(m_szString, 0, sizeof(m_szString));

	//게이지 창 보이기
	m_pCotributionBorder->ShowWindow(bShow);
	m_pContributionGaugeLeftBorder->ShowWindow(bShow);
	m_pContributionGaugeRightBorder->ShowWindow(bShow);
	m_pImageContributionRateLeftBorder->ShowWindow(bShow);
	m_pImageContributionRateRightBorder->ShowWindow(bShow);
	m_pMnDInfoListBox->ShowWindow(bShow);
	m_pBtnGroupLeave->ShowWindow(bShow);
	m_pNoticeWrite->ShowWindow(bShow);
	m_pNoticemodify->ShowWindow(bShow);
	m_pNoticeScrollBar->ShowWindow(bShow);
	
	m_pNoticeWrite->EnableWindow(FALSE);
	m_pNoticemodify->EnableWindow(FALSE);

	for( int i = 0; i < 4; i++ )
	{
		m_pPopUpButton[i]->ShowWindow(FALSE);
	}

	if(bShow)
	{
		SelectNoticeMode(_XMNDNOTICEMODE_READ);

		if( g_pLocalUser->m_pLocalMnDInfomation.nMnDClass == _XDEF_NONE )
		{
			//게이지 창 닫고,,
			m_pCotributionBorder->ShowWindow(FALSE);
			m_pContributionGaugeLeftBorder->ShowWindow(FALSE);
			m_pContributionGaugeRightBorder->ShowWindow(FALSE);

			//하산버튼 닫기
			m_pBtnGroupLeave->ShowWindow(FALSE);

			//공지 작성 수정 닫기
			m_pNoticeWrite->ShowWindow(FALSE);
			m_pNoticemodify->ShowWindow(FALSE);

			_snprintf( m_szString[0], sizeof(m_szString[0]), _XGETINTERFACETEXT(ID_STRING_NEW_3859));//"맺어진 사제 관계가 없습니다.");
		}
		else
		{
			if( g_pLocalUser->m_pLocalMnDInfomation.nMnDClass == _XDEF_MASTER ) //스승이면
			{
				m_pNoticeWrite->EnableWindow(TRUE);
				m_pNoticemodify->EnableWindow(TRUE);

				m_pBtnGroupLeave->ShowWindow(FALSE);
				m_pImageContributionRateLeftBorder->ShowWindow(FALSE);
				m_pImageContributionRateRightBorder->ShowWindow(FALSE);
			}
			_snprintf( m_szString[0], sizeof(m_szString[0]),  _XGETINTERFACETEXT(ID_STRING_NEW_3860), g_pLocalUser->m_pLocalMnDInfomation.szMasterNickName); //"사호 : %s"
			_snprintf( m_szString[1], sizeof(m_szString[1]), _XGETINTERFACETEXT(ID_STRING_NEW_3861), 0); //"사제원 : %d명"
			_snprintf( m_szString[2], sizeof(m_szString[2]), _XGETINTERFACETEXT(ID_STRING_NEW_3862), 0); //"(현재 %d명 접속 중)"
			_snprintf( m_szString[3], sizeof(m_szString[3]), _XGETINTERFACETEXT(ID_STRING_NEW_3863), g_pLocalUser->m_pLocalMnDInfomation.szMasterName); //"스승 : %s"
			_snprintf( m_szString[4], sizeof(m_szString[4]), _XGETINTERFACETEXT(ID_STRING_CONTRIBUTION_CONTRIBUTIONRATE));//"기여도 :"
			_snprintf( m_szString[5], sizeof(m_szString[5]), "%d", g_pLocalUser->m_pLocalMnDInfomation.nMasterContribution);
			_snprintf( m_szString[6], sizeof(m_szString[6]), _XGETINTERFACETEXT(ID_STRING_NEW_3864), g_pLocalUser->m_pLocalMnDInfomation.nMasterRank); //"스승등급 : %d 級"
		}
	}
}

void _XWindow_MnDGroup::ShowPopupWindow(BOOL bShow)
{
	for( int i = 0; i < 4; i++ )
	{
		m_pPopUpButton[i]->ShowWindow(FALSE);
	}

	if(bShow)
	{
		int selectline = m_pMnDInfoListBox->GetSelectedItem();
		if( selectline < 0 || selectline >= m_pMnDInfoListBox->GetItemCount() )
				return;

		POINT pos = m_pMnDInfoListBox->GetColumnOffsetWindowPos( selectline, 0 ); //선택한 것에 위치를 리턴함
		//선택한 곳에서 팝업버튼 위치를 바꿔주자
		for( i = 0; i < 4; i++ )
		{
			//선택한 유저에 따라 보이고 안보이고 설정을 해줘야함
			m_pPopUpButton[i]->MoveWindow( g_pGroup_Window->GetWindowPos().x + g_pGroup_Window->GetWindowSize().cx + 5, pos.y+(i*18) );
			m_pPopUpButton[i]->ShowWindow(TRUE);
		}
		m_nPopupCount = 4;
	}	
}

void _XWindow_MnDGroup::MoveWindow()
{
	if(m_nNoticeMode == _XNOTICEMODE_MODIFY)
	{
		int linecount = 0;
		m_nCurrentNoticeLinePos = m_nNoticeTotalCount-1;
		if( m_nNoticeTotalCount > 4 )
			linecount = 3;
		else
			linecount = m_nNoticeTotalCount-1;			
		
		m_MnDNoticeIMEControl.MoveWindow( g_pGroup_Window->GetWindowPos().x+14, g_pGroup_Window->GetWindowPos().y+111 +(linecount*17));
	}
	else
		m_MnDNoticeIMEControl.MoveWindow( g_pGroup_Window->GetWindowPos().x+14, g_pGroup_Window->GetWindowPos().y+111 );
}

void _XWindow_MnDGroup::Process()
{
	
}

void _XWindow_MnDGroup::Draw(int nX, int nY)
{
	if(m_pPopUpButton[0]->GetShowStatus()) //가장 첫번째,, 팝업인데 팝업 열 때 항상 나오는 것이므로,,
	{
		_XDrawRectAngle( m_pPopUpButton[0]->GetWindowPos().x-3, m_pPopUpButton[0]->GetWindowPos().y-3, 
						 m_pPopUpButton[0]->GetWindowPos().x+87+3, m_pPopUpButton[0]->GetWindowPos().y+(m_nPopupCount*18)+2, 
						 0.0f,0xFF000000 );
		_XDrawRectAngle( m_pPopUpButton[0]->GetWindowPos().x-2, m_pPopUpButton[0]->GetWindowPos().y-2,
						 m_pPopUpButton[0]->GetWindowPos().x+87+2, m_pPopUpButton[0]->GetWindowPos().y+(m_nPopupCount*18)+1, 
						 0.0f,0xFFD2D2D2 );
		_XDrawRectAngle( m_pPopUpButton[0]->GetWindowPos().x-1, m_pPopUpButton[0]->GetWindowPos().y-1,
						 m_pPopUpButton[0]->GetWindowPos().x+87+1, m_pPopUpButton[0]->GetWindowPos().y+(m_nPopupCount*18), 
						 0.0f,0xFF000000 );
	}

	//테두리 커다란 위에것
	_XDrawSolidBar( nX+4, nY+37, nX+341, nY+98 , D3DCOLOR_ARGB(100,0,0,0) );
	_XDrawRectAngle( nX+5, nY+38, nX+339, nY+96 , 0.0f, 0xFFCCBF9E );

	//아래 공지사항 테두리
	_XDrawSolidBar( nX+4, nY+102, nX+314, nY+183 , D3DCOLOR_ARGB(175,0,0,0) );
	_XDrawRectAngle( nX+5, nY+103, nX+312, nY+181 , 0.0f, 0xFFCCBF9E );

	if( g_pLocalUser->m_pLocalMnDInfomation.nMnDClass == _XDEF_NONE )
	{
		g_XBaseFont->SetColor( 0xFFFFD514 );	
		g_XBaseFont->PutsAlign(nX+170, nY+62, _XFONT_ALIGNTYPE_CENTER, m_szString[0] );
	}
	else
	{
		/*POINT listwinpos = m_pMnDInfoListBox->GetWindowPos();
		SIZE listwinsize = m_pMnDInfoListBox->GetWindowSize();	
		
		_XDrawRectAngle( listwinpos.x, listwinpos.y-1,
			listwinpos.x+listwinsize.cx- 13, listwinpos.y+listwinsize.cy-1, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( listwinpos.x+1, listwinpos.y,
			listwinpos.x+listwinsize.cx - 14, listwinpos.y+listwinsize.cy-2, 0.0f, D3DCOLOR_ARGB(191,0,0,0) );
		_XDrawRectAngle( listwinpos.x+2, listwinpos.y+1,
			listwinpos.x+listwinsize.cx - 15, listwinpos.y+listwinsize.cy-3, 0.0f, D3DCOLOR_ARGB(63,0,0,0) );*/
		
		if( m_pMnDInfoListBox->CheckMousePosition() )
		{
			int curindex = m_pMnDInfoListBox->GetMouseOverItem(); //마우스 올리면 기여도 찍어줄려고,,
			if( curindex > 0 )
				return;

			POINT pos = m_pMnDInfoListBox->GetColumnOffsetWindowPos( curindex, 0 );

			vector<MnDGroupList*>::iterator iter = m_vtMnDGroup.begin();
			if(curindex < m_vtMnDGroup.size())
			{
				std::advance( iter, curindex );			

				MnDGroupList* pGroup = (*iter);

				TCHAR tempStr[64];
				memset( tempStr, 0, sizeof(tempStr));
				sprintf( tempStr, _XGETINTERFACETEXT(ID_STRING_CONTRIBUTION_CURRENTCONTRIBUTION), pGroup->nDiscipleContribution ); //보유 기여도 :

				g_InterfaceTooltip.SetToolTipString( pos.x+260, pos.y, tempStr, 0xFF01FE06 ); //녹색으로 찍어주자
			}
		}

		m_nNoticeScrollPos = m_pNoticeScrollBar->GetLineScrollPos();

		if( m_nNoticeTotalCount < 5 || (m_nCurrentNoticeLinePos <= m_nNoticeScrollPos+3) )
		{
			if( m_MnDNoticeIMEControl.GetFocus() )
				m_MnDNoticeIMEControl.Draw();
		}

		int linecount = 0;
		g_XBaseFont->SetColor( 0xFFC0EB7F );		

		if( m_nNoticeMode == _XNOTICEMODE_READ ) 
		{	
			for( int i = m_nNoticeScrollPos ; i < m_nNoticeScrollPos+4; i++)
			{
				g_XBaseFont->Puts( nX+14, nY+111+(linecount*17), m_szNoticeViewText[i] );
				linecount++;
			}	
		}
		else
		{
			for( int i = m_nNoticeScrollPos ; i < m_nNoticeScrollPos+4; i++)
			{
				g_XBaseFont->Puts( nX+14, nY+111+(linecount*17), m_szNoticeTempText[i] );
				linecount++;
			}
		}

		g_XBaseFont->SetColor( 0xFFFFD514 );	
		g_XBaseFont->PutsAlign(nX+10, nY+45, _XFONT_ALIGNTYPE_LEFT, m_szString[0] );

		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->PutsAlign(nX+130, nY+45, _XFONT_ALIGNTYPE_LEFT, m_szString[1] );

		g_XBaseFont->PutsAlign(nX+215, nY+45, _XFONT_ALIGNTYPE_LEFT, m_szString[2] );

		g_XBaseFont->PutsAlign(nX+10, nY+62, _XFONT_ALIGNTYPE_LEFT, m_szString[3] );

		g_XBaseFont->PutsAlign(nX+130, nY+62, _XFONT_ALIGNTYPE_LEFT, m_szString[4] );

		g_XBaseFont->SetColor( 0xFF01FE06 );//녹색
		g_XBaseFont->PutsAlign(nX+185, nY+62, _XFONT_ALIGNTYPE_LEFT, m_szString[5] );

		g_XBaseFont->SetColor( 0xFFFFD514 );
		g_XBaseFont->PutsAlign(nX+10, nY+79, _XFONT_ALIGNTYPE_LEFT, m_szString[6] );
	}

	g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Flush();
}

void _XWindow_MnDGroup::SetListItem()
{
	int imageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int i = 0;
	TCHAR memberstr[256];
	TCHAR tempstr[64];
	
	m_pMnDInfoListBox->DeleteAllItem();
	
	vector<MnDGroupList*>::iterator findunit = m_vtMnDGroup.begin();
	for( ; findunit != m_vtMnDGroup.end(); findunit++ )
	{
		MnDGroupList* tempunit = *findunit;
		if( tempunit )
		{			
			DWORD colorvalue = 0xFFFFFFFF;
			RECT rect = {0, 0, 0, 0};

			memset(memberstr, 0, sizeof(memberstr));
			memset(tempstr, 0, sizeof(tempstr));
			
			if( tempunit->nMnDClass >= _XDEF_DESCIPLE )
			{
				switch( tempunit->nMnDClass )
				{
				case 2 : //일등 제자
					strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_NEW_3872) );	// 일등제자
					break;
				case 3 : //이등 제자
					strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_NEW_3873) );	// 이등제자
					break;
				default : //삼등 제자 
					strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_NEW_3874) );	// 삼등제자
					break;
				}
				colorvalue = 0xFFD2D2D2;
			}
			else if( tempunit->nMnDClass == _XDEF_MASTER )
			{
				strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_NEW_3852) );	// 스승
				colorvalue = 0xFFD5D49A;
			}
			if( !tempunit->nPlayerState )
				colorvalue = 0xFF555555;
			
			sprintf( tempstr, " : %s", tempunit->szName );
			strcat( memberstr, tempstr );
			
			int level = g_UserLevelTable[tempunit->nLevel].level;
			int sublevel = g_UserLevelTable[tempunit->nLevel].sublevel;
			
			memset(tempstr, 0, sizeof(tempstr));

			if( tempunit->nPlayerState == 2 ) //서버 이동중
			{
				strcpy( tempstr, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ORGANIZATIONINFO1, 
																   g_LevelName[level], &sublevel, "서버 이동중" ) );
			}
			else
			{
				strcpy( tempstr, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ORGANIZATIONINFO1, 
																   g_LevelName[level], &sublevel, g_ZoneInfoTable[tempunit->nCurrentZone-1].zoneshortname ) );
			}
			strcat( memberstr, tempstr );	
			
			m_pMnDInfoListBox->InsertListItem( _T(""), 0, i+1 );
			m_pMnDInfoListBox->SetItemText(i, 0, memberstr);
			m_pMnDInfoListBox->SetItemAttrib(i, 0, colorvalue);
			m_pMnDInfoListBox->SetItemAttribHighlight( i, 0, colorvalue );
			//m_pMnDInfoListBox->SetIconArchive(i, &g_MainInterfaceTextureArchive);
			//m_pMnDInfoListBox->SetItemAttrib(i, 0, imageindex, rect);
			//m_pMnDInfoListBox->SetItemText(i, 0, memberstr);
			//m_pMnDInfoListBox->SetItemAttrib(i, 0, colorvalue);
			//m_pMnDInfoListBox->SetItemAttribHighlight( i, 0, colorvalue );
		}
		i++;
	}
}

void _XWindow_MnDGroup::CalculateNoticeTextength()
{
	m_nNoticeLength = 0;
	for( int i = 0; i < m_nNoticeTotalCount ; i++)
	{
		m_nNoticeLength += strlen( m_szNoticeTempText[i] );
	}

	m_nNoticeLength += (m_nNoticeTotalCount - 1); //줄수 만큼은 왜 더하는 것일까,,
}

void _XWindow_MnDGroup::SelectNoticeMode(_XMnDNoticeMode mode)
{
	m_nNoticeMode = mode;

	switch( m_nNoticeMode )
	{
	case _XMNDNOTICEMODE_READ :
		{
			if( m_MnDNoticeIMEControl.GetShowStatus() )
			{
				_XIMEKERNEL.ResetCombinationBuffer();
				m_MnDNoticeIMEControl.ClearBuffer();
				_XIMEKERNEL.SetFocus(NULL);
			}

			SetNoticeText(m_stNoticeText.notice);

			m_pNoticeWrite->EnableWindow(FALSE);
			m_pNoticemodify->EnableWindow(FALSE);

			//스승만 글을 쓸 수 있도록 추가 해야함
			m_pNoticeWrite->EnableWindow(TRUE);
			m_pNoticemodify->EnableWindow(TRUE);

			// 버튼이미지 
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  47, 192, 71, 234 );	
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 20, 192, 44, 234 );			
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  74, 192, 98, 234 );	
			
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  128, 99, 152, 141 );
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 101, 99, 125, 141 );			
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  155, 99, 179, 141 );
		}
		break;
	case _XMNDNOTICEMODE_WRITE:
		{
			_XIMEKERNEL.ResetCombinationBuffer();
			m_MnDNoticeIMEControl.ClearBuffer();
			memset(m_szNoticeTempText, 0, sizeof(m_szNoticeTempText));
			m_nNoticeScrollPos	= 0;
			m_nNoticeTotalCount	= 1;
			m_nCurrentNoticeLinePos	= 0;
			m_nNoticeLength = 0; 

			m_MnDNoticeIMEControl.ShowWindow(TRUE);
			m_MnDNoticeIMEControl.MoveWindow( g_pGroup_Window->GetWindowPos().x+14, g_pGroup_Window->GetWindowPos().y+111 );
			m_MnDNoticeIMEControl.SetTextColor(_XSC_DEFAULT);
			m_MnDNoticeIMEControl.SetFocus();

			// 버튼이미지 
			m_pNoticeWrite->EnableWindow(TRUE);
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  128, 145, 152, 187 );	
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 101, 145, 125, 187 );			
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  155, 145, 179, 187 );	
			
			m_pNoticemodify->EnableWindow(TRUE);
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  128, 192, 152, 234 );
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 101, 192, 125, 234 );			
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  155, 192, 179, 234 );
		}
		break;
	case _XMNDNOTICEMODE_MODIFY :
		{
			_XIMEKERNEL.ResetCombinationBuffer();
			m_MnDNoticeIMEControl.ClearBuffer();

			SetNoticeText(m_stNoticeText.notice, true);
			m_MnDNoticeIMEControl.AddString(m_szNoticeTempText[m_nNoticeTotalCount-1], strlen(m_szNoticeTempText[m_nNoticeTotalCount-1]) );
			memset(m_szNoticeTempText[m_nNoticeTotalCount-1], 0, sizeof(m_szNoticeTempText[m_nNoticeTotalCount-1]));
			
			int linecount = 0;
			m_nCurrentNoticeLinePos = m_nNoticeTotalCount-1;
			if( m_nNoticeTotalCount > 4 )
				linecount = 3;
			else
				linecount = m_nNoticeTotalCount-1;			
			
			m_MnDNoticeIMEControl.ShowWindow(TRUE);			
			m_MnDNoticeIMEControl.MoveWindow( g_pGroup_Window->GetWindowPos().x+14, g_pGroup_Window->GetWindowPos().y+111 +(linecount*17));
			m_MnDNoticeIMEControl.SetTextColor(_XSC_DEFAULT);
			m_MnDNoticeIMEControl.SetFocus();
			
			m_pNoticeScrollBar->SetLineScrollPos(9999);

			// 버튼이미지 
			m_pNoticeWrite->EnableWindow(TRUE);
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  128, 145, 152, 187 );	
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 101, 145, 125, 187 );			
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  155, 145, 179, 187 );	
			
			m_pNoticemodify->EnableWindow(TRUE);
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  128, 192, 152, 234 );
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 101, 192, 125, 234 );			
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  155, 192, 179, 234 );
		}
		break;
	}
}

void _XWindow_MnDGroup::SetNoticeText(TCHAR* text, bool modifymode)
{
	if( !text ) return;
	if( strlen(text) <= 0 ) return;
	
	_XStringSplitInfo splitinfo;	

	if( !modifymode )	// 서버에서 받은 공지
	{
		memset(m_szNoticeViewText, 0, sizeof(m_szNoticeViewText));

		// String splitting....		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->SplitString(text, 300, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
		}
		else
		{
			g_XBaseFont->SplitStringByCR(text, splitinfo);
		}
		m_nNoticeViewTextTotalCount = splitinfo.splitcount;
		for( int i = 0; i < splitinfo.splitcount; i++ )
		{			
			strncpy( m_szNoticeViewText[i], splitinfo.splitpoint[i], splitinfo.splitlength[i] );	
		}

		sprintf( m_szNoticeViewText[i], "                   %s %s", m_stNoticeText.writedate, m_stNoticeText.writer );	
		m_nNoticeViewTextTotalCount++;
	}
	else	// 수정 버튼 클릭시 
	{
		memset(m_szNoticeTempText, 0, sizeof(m_szNoticeTempText));

		// String splitting....
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->SplitString(text, 300, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
		}
		else
		{
			g_XBaseFont->SplitStringByCR(text, splitinfo);
		}

		m_nNoticeTotalCount = splitinfo.splitcount;
		for( int i = 0; i < splitinfo.splitcount; i++ )
		{			
			strncpy( m_szNoticeTempText[i], splitinfo.splitpoint[i], splitinfo.splitlength[i] );	
		}
	}
	
	if( m_pNoticeScrollBar )
	{
		m_pNoticeScrollBar->MoveWindow( g_pGroup_Window->GetWindowPos().x + 300, g_pGroup_Window->GetWindowPos().y + 104 );

		if( !modifymode )
			m_pNoticeScrollBar->SetTotalLineCount( splitinfo.splitcount+1 );
		else
			m_pNoticeScrollBar->SetTotalLineCount( splitinfo.splitcount );
	
		m_pNoticeScrollBar->SetLineScrollPos(0);
		m_pNoticeScrollBar->UpdateData();		
	}
}
