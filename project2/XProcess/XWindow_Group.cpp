// XWindow_Group.cpp: implementation of the _XWindow_Group class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_MnDGroup.h" // Author : 양희왕 //breif : 사제관련 탭
#include "XWindow_Group.h"
#include "XWindow_RequestParty.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"
#include "XWindow_CastleInfo.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XCallback_MM_Write(  POINT pos, SIZE size  )
{		
	if( g_pGroup_Window )
	{
		if( g_pGroup_Window->m_NoticeMode == _XNOTICEMODE_READ )
			g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2480), _XSC_INFORMATION );	
		else 
			g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_ITEMDROPCONFIRM_OK), _XSC_INFORMATION );	
	}
}

void __stdcall _XCallback_MM_Modify(  POINT pos, SIZE size  )
{
	if( g_pGroup_Window )	
	{
		if( g_pGroup_Window->m_NoticeMode == _XNOTICEMODE_READ )
			g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2481), _XSC_INFORMATION );	
		else 
			g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_ITEMDROPCONFIRM_CANCEL), _XSC_INFORMATION );	
	}	
}

void __stdcall _XIMECallback_NoticeEditEscape( _XIMEContainer* pIMEContainer )
{

}

void __stdcall _XIMECallback_NoticeEditReturn( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;	
	if( !g_pGroup_Window->m_NoticeIMEControl.GetFocus() ) return;
	
	g_pGroup_Window->CalculateNoticeTextength();
	// 글자수 제한
	if( g_pGroup_Window->m_NoticeTextLength + g_pGroup_Window->m_NoticeIMEControl.GetLength_ByMBCSSize() >= _XDEF_MAX_NOTICELENGTH ) return;	

	POINT winpos;				
	winpos = g_pGroup_Window->GetWindowPos();
	
	memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
	strcpy( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] , g_pGroup_Window->m_NoticeIMEControl.GetText());
	
	int cursorx = g_pGroup_Window->m_NoticeIMEControl.GetCursorX_ByMBCSSize();
	int length = strlen( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] );
	
	if( g_pGroup_Window->m_NoticeTextTotalCount-1 == g_pGroup_Window->m_CurrentNoticeLinePos )
	{
		if( cursorx == length )
		{
			g_pGroup_Window->m_NoticeTextTotalCount++;
			g_pGroup_Window->m_CurrentNoticeLinePos++;
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
		}
		else
		{
			TCHAR leftstring[64];
			TCHAR rightstring[64];
			memset( leftstring, 0, sizeof(TCHAR)*64 );
			memset( rightstring, 0, sizeof(TCHAR)*64 );
			strncpy( leftstring, g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], cursorx );
			strncpy( rightstring, g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] + cursorx, length - cursorx );
			
			memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
			strncpy( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] , leftstring, cursorx);
			
			g_pGroup_Window->m_NoticeTextTotalCount++;
			g_pGroup_Window->m_CurrentNoticeLinePos++;
			
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();			
			g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
			g_pGroup_Window->m_NoticeIMEControl.AddString( rightstring, length - cursorx );
			g_pGroup_Window->m_NoticeIMEControl.SetCursorX_ByMBCSSize(0);
		}
		
		if( g_pGroup_Window->m_NoticeScrollBar )
		{
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
			g_pGroup_Window->m_NoticeScrollBar->MoveWindow( winpos.x+300, winpos.y+138 );
#else			
			g_pGroup_Window->m_NoticeScrollBar->MoveWindow( winpos.x + 300, winpos.y + 104 );
#endif
			g_pGroup_Window->m_NoticeScrollBar->SetTotalLineCount( g_pGroup_Window->m_NoticeTextTotalCount );	
			
			if( !g_pGroup_Window->m_NoticeScrollBar->Dragging() )
			{
				g_pGroup_Window->m_NoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_CurrentNoticeLinePos ); 
			}		
			g_pGroup_Window->m_NoticeScrollBar->UpdateData();
			
		}								
		
		int scrollpos = g_pGroup_Window->m_NoticeScrollBar->GetLineScrollPos();
		
		g_pGroup_Window->CalculateNoticeTextength();

		if( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_NoticeTextLength > 46 )
			g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
		else
			g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_NoticeTextLength );


		g_pGroup_Window->m_NoticeIMEControl.MoveWindow(winpos.x + 14,
			winpos.y + 111 +((g_pGroup_Window->m_CurrentNoticeLinePos - scrollpos)*17) );
		g_pGroup_Window->m_NoticeIMEControl.SetFocus();
		
	}
	else
	{
		int scrollpos = g_pGroup_Window->m_NoticeScrollBar->GetLineScrollPos();
		if( cursorx == length )
		{
			for( int i = g_pGroup_Window->m_NoticeTextTotalCount-1; i > g_pGroup_Window->m_CurrentNoticeLinePos; i--)
			{
				TCHAR buffer[128];
				memset( buffer, 0, sizeof(TCHAR)*128 );
				strcpy( buffer, g_pGroup_Window->m_NoticeTempText[ i ] );
				memset( g_pGroup_Window->m_NoticeTempText[ i ], 0, sizeof(TCHAR)*128 );
				memset( g_pGroup_Window->m_NoticeTempText[ i + 1 ], 0, sizeof(TCHAR)*128 );
				strcpy( g_pGroup_Window->m_NoticeTempText[ i + 1 ], buffer);
			}
			g_pGroup_Window->m_NoticeTextTotalCount++;
			g_pGroup_Window->m_CurrentNoticeLinePos++;
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
		}
		else
		{
			
			scrollpos = g_pGroup_Window->m_NoticeScrollBar->GetLineScrollPos();

			for( int i = g_pGroup_Window->m_NoticeTextTotalCount-1; i > g_pGroup_Window->m_CurrentNoticeLinePos; i--)
			{
				TCHAR buffer[64];
				memset( buffer, 0, sizeof(TCHAR)*64 );
				strcpy( buffer, g_pGroup_Window->m_NoticeTempText[ i ] );
				memset( g_pGroup_Window->m_NoticeTempText[ i ], 0, sizeof(TCHAR)*128 );
				memset( g_pGroup_Window->m_NoticeTempText[ i + 1 ], 0, sizeof(TCHAR)*128 );
				strcpy( g_pGroup_Window->m_NoticeTempText[ i + 1 ], buffer);
			}
			TCHAR leftstring[64];
			TCHAR rightstring[64];
			memset( leftstring, 0, sizeof(TCHAR)*64 );
			memset( rightstring, 0, sizeof(TCHAR)*64 );
			strncpy( leftstring, g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], cursorx );
			strncpy( rightstring, g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] + cursorx, length - cursorx );
			
			memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
			strncpy( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] , leftstring, cursorx);
			
			g_pGroup_Window->m_NoticeTextTotalCount++;
			g_pGroup_Window->m_CurrentNoticeLinePos++;
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
			g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
			g_pGroup_Window->m_NoticeIMEControl.AddString( rightstring, length - cursorx );
			g_pGroup_Window->m_NoticeIMEControl.SetCursorX_ByMBCSSize(0);
		}
		
		if( g_pGroup_Window->m_NoticeScrollBar )
		{
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
			g_pGroup_Window->m_NoticeScrollBar->MoveWindow( winpos.x+300, winpos.y+138 );
#else						
			g_pGroup_Window->m_NoticeScrollBar->MoveWindow( winpos.x + 300, winpos.y + 104 );
#endif
			g_pGroup_Window->m_NoticeScrollBar->SetTotalLineCount( g_pGroup_Window->m_NoticeTextTotalCount );	
			g_pGroup_Window->m_NoticeScrollBar->UpdateData();
		}								
		
		
		g_pGroup_Window->CalculateNoticeTextength();
		
		if( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_NoticeTextLength > 46 )
			g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
		else
			g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_NoticeTextLength );
		
		int linecount = 0;
		if( g_pGroup_Window->m_CurrentNoticeLinePos  >= g_pGroup_Window->m_NoticeTextScrollPos + 4)
		{
			if( g_pGroup_Window->m_NoticeScrollBar )
			{			
				g_pGroup_Window->m_NoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_NoticeTextScrollPos + 1 );
			}					
			linecount = 3;			
		}
		else
		{
			if( scrollpos == g_pGroup_Window->m_NoticeTextTotalCount - 4 - 1)
			{
				linecount = g_pGroup_Window->m_CurrentNoticeLinePos - 1 - g_pGroup_Window->m_NoticeTextScrollPos;
			}
			else
			{
				linecount = g_pGroup_Window->m_CurrentNoticeLinePos - g_pGroup_Window->m_NoticeTextScrollPos;
			}
		}
		
		g_pGroup_Window->m_NoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111  +(linecount*17) );
		g_pGroup_Window->m_NoticeIMEControl.SetFocus();
	}
}

void __stdcall _XIMECallback_NoticeEditUp( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;	
	if( !g_pGroup_Window->m_NoticeIMEControl.GetFocus() ) return;
	
	if( g_pGroup_Window->m_CurrentNoticeLinePos == 0 ) return;
	POINT winpos;				
	winpos = g_pGroup_Window->GetWindowPos();
	
	memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
	strcpy( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] , g_pGroup_Window->m_NoticeIMEControl.GetText());
		
	int cursorx = g_pGroup_Window->m_NoticeIMEControl.GetCursorX_ByMBCSSize();
	
	_XIMEKERNEL.ResetCombinationBuffer();
	g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
	g_pGroup_Window->m_CurrentNoticeLinePos--;
	
	g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
	int length = strlen( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] );
	g_pGroup_Window->m_NoticeIMEControl.AddString( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], length );
	

	memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
	g_pGroup_Window->m_NoticeIMEControl.SetCursorX_ByMBCSSize( cursorx );
	
	int linecount = 0;
	if( g_pGroup_Window->m_CurrentNoticeLinePos  < g_pGroup_Window->m_NoticeTextScrollPos)
	{
		if( g_pGroup_Window->m_NoticeScrollBar )
		{			
			g_pGroup_Window->m_NoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_CurrentNoticeLinePos );
		}								
	}
	else
	{
		linecount = g_pGroup_Window->m_CurrentNoticeLinePos - g_pGroup_Window->m_NoticeTextScrollPos;
	}

	g_pGroup_Window->m_NoticeIMEControl.MoveWindow(winpos.x + 15, winpos.y + 111 +(linecount*17) );
	g_pGroup_Window->m_NoticeIMEControl.SetFocus();	
}

void __stdcall _XIMECallback_NoticeEditDown( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;	
	if( !g_pGroup_Window->m_NoticeIMEControl.GetFocus() ) return;
				
	if( g_pGroup_Window->m_CurrentNoticeLinePos == g_pGroup_Window->m_NoticeTextTotalCount-1 ) return;
	POINT winpos;				
	winpos = g_pGroup_Window->GetWindowPos();
	
	memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
	strcpy( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] , g_pGroup_Window->m_NoticeIMEControl.GetText());
					
	int cursorx = g_pGroup_Window->m_NoticeIMEControl.GetCursorX_ByMBCSSize();
	
	_XIMEKERNEL.ResetCombinationBuffer();
	g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
	g_pGroup_Window->m_CurrentNoticeLinePos++;
	
	g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
	int length = strlen( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] );
	g_pGroup_Window->m_NoticeIMEControl.AddString( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], length );
					
	memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
	g_pGroup_Window->m_NoticeIMEControl.SetCursorX_ByMBCSSize( cursorx );
	
	int linecount = 0;
	if( g_pGroup_Window->m_CurrentNoticeLinePos  >= g_pGroup_Window->m_NoticeTextScrollPos + 4)
	{
		if( g_pGroup_Window->m_NoticeScrollBar )
		{			
			g_pGroup_Window->m_NoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_NoticeTextScrollPos + 1 );
		}					
		linecount = 3;			
	}
	else
	{
		linecount = g_pGroup_Window->m_CurrentNoticeLinePos - g_pGroup_Window->m_NoticeTextScrollPos;
	}
	
	g_pGroup_Window->m_NoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
	g_pGroup_Window->m_NoticeIMEControl.SetFocus();
}

void __stdcall _XIMECallback_NoticeEditLeft( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;	
	if( !g_pGroup_Window->m_NoticeIMEControl.GetFocus() ) return;

	int cursorx = g_pGroup_Window->m_NoticeIMEControl.GetCursorX_ByMBCSSize();
	int length = g_pGroup_Window->m_NoticeIMEControl.GetLength_ByMBCSSize();
	
	if( cursorx == 0 )
	{
		if( g_pGroup_Window->m_CurrentNoticeLinePos != 0 )
		{
			POINT winpos;				
			winpos = g_pGroup_Window->GetWindowPos();
			
			memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
			strcpy( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] , g_pGroup_Window->m_NoticeIMEControl.GetText());
						
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
			g_pGroup_Window->m_CurrentNoticeLinePos--;
			
			g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
			int length = strlen( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] );
			g_pGroup_Window->m_NoticeIMEControl.AddString( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], length );
			
			memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
			g_pGroup_Window->m_NoticeIMEControl.SetCursorX_ByMBCSSize( length );
			
			int linecount = 0;
			if( g_pGroup_Window->m_CurrentNoticeLinePos  < g_pGroup_Window->m_NoticeTextScrollPos)
			{
				if( g_pGroup_Window->m_NoticeScrollBar )
				{			
					g_pGroup_Window->m_NoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_CurrentNoticeLinePos );
				}								
			}
			else
			{
				linecount = g_pGroup_Window->m_CurrentNoticeLinePos - g_pGroup_Window->m_NoticeTextScrollPos;
			}
			
			g_pGroup_Window->m_NoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
			g_pGroup_Window->m_NoticeIMEControl.SetFocus();
		}
	}
}

void __stdcall _XIMECallback_NoticeEditRight( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;
	if( !g_pGroup_Window->m_NoticeIMEControl.GetFocus() ) return;
	
	int cursorx = g_pGroup_Window->m_NoticeIMEControl.GetCursorX_ByMBCSSize();
	int length = g_pGroup_Window->m_NoticeIMEControl.GetLength_ByMBCSSize();
	
	if( cursorx == length )
	{
		if( g_pGroup_Window->m_CurrentNoticeLinePos != g_pGroup_Window->m_NoticeTextTotalCount-1 )
		{
			POINT winpos;				
			winpos = g_pGroup_Window->GetWindowPos();
			
			memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
			strcpy( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] , g_pGroup_Window->m_NoticeIMEControl.GetText());
			
			_XIMEKERNEL.ResetCombinationBuffer();
			g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
			g_pGroup_Window->m_CurrentNoticeLinePos++;
			
			g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
			int length = strlen( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] );
			g_pGroup_Window->m_NoticeIMEControl.AddString( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], length );
			
			memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
			g_pGroup_Window->m_NoticeIMEControl.SetCursorX_ByMBCSSize( 0 );
			
			int linecount = 0;
			if( g_pGroup_Window->m_CurrentNoticeLinePos  >= g_pGroup_Window->m_NoticeTextScrollPos + 4)
			{
				if( g_pGroup_Window->m_NoticeScrollBar )
				{			
					g_pGroup_Window->m_NoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_NoticeTextScrollPos + 1 );
				}					
				linecount = 3;
			}
			else
			{
				linecount = g_pGroup_Window->m_CurrentNoticeLinePos - g_pGroup_Window->m_NoticeTextScrollPos;
			}
			
			g_pGroup_Window->m_NoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
			g_pGroup_Window->m_NoticeIMEControl.SetFocus();
						
		}
	}
}

void __stdcall _XIMECallback_NoticeEditBack( _XIMEContainer* pIMEContainer )
{
	if( !g_pGroup_Window ) return;
	if( !g_pGroup_Window->m_NoticeIMEControl.GetFocus() ) return;
	
	int cursorx = g_pGroup_Window->m_NoticeIMEControl.GetCursorX_ByMBCSSize();
	int length = g_pGroup_Window->m_NoticeIMEControl.GetLength_ByMBCSSize();
	
	if( cursorx == 0 )
	{
		if( g_pGroup_Window->m_CurrentNoticeLinePos != 0 )
		{
			POINT winpos;				
			winpos = g_pGroup_Window->GetWindowPos();
						
			if( strlen( g_pGroup_Window->m_NoticeIMEControl.GetText() ) == 0 )
			{
				
				if( g_pGroup_Window->m_NoticeTextTotalCount - 1 == g_pGroup_Window->m_CurrentNoticeLinePos )
				{
					
					g_pGroup_Window->m_NoticeTextTotalCount--;
					
					_XIMEKERNEL.ResetCombinationBuffer();
					g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
					g_pGroup_Window->m_CurrentNoticeLinePos--;
					
					g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
					int length = strlen( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] );
					g_pGroup_Window->m_NoticeIMEControl.AddString( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], length );
					
					memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
					g_pGroup_Window->m_NoticeIMEControl.SetCursorX_ByMBCSSize( length );
					
					g_pGroup_Window->CalculateNoticeTextength();
					
					if( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_NoticeTextLength > 46 )
						g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
					else
						g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_NoticeTextLength );
					
					int linecount;

					if( g_pGroup_Window->m_NoticeTextTotalCount >= 4 )
					{
						linecount = 3;
					}
					else
					{ 
						linecount = g_pGroup_Window->m_NoticeTextTotalCount - 1;
					}


					g_pGroup_Window->m_NoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
					g_pGroup_Window->m_NoticeIMEControl.SetFocus();
					
					if( g_pGroup_Window->m_NoticeScrollBar )
					{
						g_pGroup_Window->m_NoticeScrollBar->SetTotalLineCount( g_pGroup_Window->m_NoticeTextTotalCount );	
						g_pGroup_Window->m_NoticeScrollBar->UpdateData();						
					}
				}
				else
				{
					int scrollpos = g_pGroup_Window->m_NoticeScrollBar->GetLineScrollPos();
										
					for( int i = g_pGroup_Window->m_CurrentNoticeLinePos; i < g_pGroup_Window->m_NoticeTextTotalCount - 1; i++)
					{
						TCHAR buffer[64];
						memset( buffer, 0, sizeof(TCHAR)*64 );
						strcpy( buffer, g_pGroup_Window->m_NoticeTempText[ i + 1 ] );
						memset( g_pGroup_Window->m_NoticeTempText[ i ], 0, sizeof(TCHAR)*128 );
						memset( g_pGroup_Window->m_NoticeTempText[ i + 1 ], 0, sizeof(TCHAR)*128 );
						strcpy( g_pGroup_Window->m_NoticeTempText[ i ], buffer);
					}
					
					g_pGroup_Window->m_NoticeTextTotalCount--;
					
					_XIMEKERNEL.ResetCombinationBuffer();
					g_pGroup_Window->m_NoticeIMEControl.ClearBuffer();
					g_pGroup_Window->m_CurrentNoticeLinePos--;
					
					g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
					int length = strlen( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos] );
					g_pGroup_Window->m_NoticeIMEControl.AddString( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], length );
					
					memset( g_pGroup_Window->m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
					g_pGroup_Window->m_NoticeIMEControl.SetCursorX_ByMBCSSize( length );
					
					
					g_pGroup_Window->CalculateNoticeTextength();
					
					if( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_NoticeTextLength > 46 )
						g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( 46 );
					else
						g_pGroup_Window->m_NoticeIMEControl.SetMaxLength( _XDEF_MAX_NOTICELENGTH - g_pGroup_Window->m_NoticeTextLength );
					
					if( g_pGroup_Window->m_NoticeScrollBar )
					{
						g_pGroup_Window->m_NoticeScrollBar->SetTotalLineCount( g_pGroup_Window->m_NoticeTextTotalCount );	
						g_pGroup_Window->m_NoticeScrollBar->UpdateData();
						
					}
					
					int linecount = 0;
					if( g_pGroup_Window->m_CurrentNoticeLinePos  < g_pGroup_Window->m_NoticeTextScrollPos)
					{
						if( g_pGroup_Window->m_NoticeScrollBar )
						{			
							g_pGroup_Window->m_NoticeScrollBar->SetLineScrollPos( g_pGroup_Window->m_CurrentNoticeLinePos + 1 );
						}								
					}
					else
					{
						g_pGroup_Window->m_NoticeScrollBar->SetLineScrollPos( scrollpos );

						if( scrollpos == 0 )
						{
							linecount = g_pGroup_Window->m_CurrentNoticeLinePos - g_pGroup_Window->m_NoticeTextScrollPos;
						}
						else
						{
							if( scrollpos == g_pGroup_Window->m_NoticeTextTotalCount - 4 + 1)
							{
								linecount = g_pGroup_Window->m_CurrentNoticeLinePos + 1 - g_pGroup_Window->m_NoticeTextScrollPos;
							}
							else
							{
								linecount = g_pGroup_Window->m_CurrentNoticeLinePos - g_pGroup_Window->m_NoticeTextScrollPos;
							}
						}
					}
					
					g_pGroup_Window->m_NoticeIMEControl.MoveWindow(winpos.x + 14, winpos.y + 111 +(linecount*17) );
					g_pGroup_Window->m_NoticeIMEControl.SetFocus();
					
				}				
			}
		}
	}
}


void __stdcall _XCallback_Group_GotoOrganization(  POINT pos, SIZE size  )
{
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3327), _XSC_INFORMATION );//우리단 바로가기
}
void __stdcall _XCallback_Group_GotoAlience(  POINT pos, SIZE size  )
{		
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3328), _XSC_INFORMATION );//우리회 바로가기
}

_XWindow_Group::_XWindow_Group()
{	
	for( int index = 0; index < _XGROUPTAB_TOTALCOUNT; index++ )
	{		
		m_BaseTabButton[index] = NULL;
	}
#ifdef _XTS_ALIANCE_20061018
	memset( m_AlienceMasterName, 0, sizeof(TCHAR) * 13 );
	m_UniqueIndexAlience = 0;
	m_pBtnAlienceDestroy = NULL;
	m_pImageAlienceBorder		= NULL;		// 영웅단 동맹 창 바탕
	m_pImageAlienceName1		= NULL;		// 영웅단 동맹 창 '방'자
	m_pImageAlienceName2		= NULL;		// 영웅단 동맹 창 '회'자
	m_pImageAlienceMemberBorder	= NULL;		// 영웅단 동맹 맴버 버튼 테두리	
	m_pImageAlienceMemberEmptyBorder	= NULL;// 영웅단 동맹 맴버공석 이미지 
	
	// 영웅단 동맹 각각을 표시하는 버튼 
	for( index = 0; index < 5; ++index )
		m_pBtnAlienceName[index] = NULL;	
//#else	
//	m_BaseTabButton			= NULL;
#endif
#ifdef _XDEF_ALIANCE_WEBBOARD_20070130	
	m_pBtnGoToOrganization		= NULL;			// 영웅단 게시판 바로가기 버튼
	m_pBtnGoToAliance			= NULL;			// 영웅회 게시판 바로가기 버튼
#endif

	m_pCotributionBorder		= NULL;
	m_pContributionGaugeLeftBorder	= NULL;
	m_pContributionGaugeRightBorder = NULL;
	
	m_pImageContributionRateLeftBorder	= NULL;
	m_pImageContributionRateRightBorder	= NULL;
	m_pImageLineUpLeftBorder	= NULL;
	m_pImageLineUpRightBorder	= NULL;	
	m_pBtnGroupLeave		= NULL;		// 영웅단 탈퇴 버튼
	m_pBtnLineUpDropDown	= NULL;	// 영웅단 정렬 버튼 


	m_OrganizationListBox		= NULL;
	
	
	m_OrganizationBaseInfo.m_cBaseParty = 0;	
	memset( m_OrganizationBaseInfo.m_cCaptainName, 0, sizeof(TCHAR)*13 );
	memset( m_OrganizationBaseInfo.m_cName, 0, sizeof(TCHAR)*9 );
	m_OrganizationBaseInfo.m_cProperty = 0;
	m_OrganizationBaseInfo.m_iIndex = 0;
	m_OrganizationBaseInfo.m_uiContribution = 0;
	m_OrganizationBaseInfo.m_uiExperience = 0;
	m_OrganizationBaseInfo.m_usCurMemberNum = 0;
	
	for( int i = 0; i < 9; ++i )
	{
		memset( m_GroupInfoText[i], 0, sizeof(TCHAR) * 64 );
	}
	
	memset( m_SendName, 0, sizeof(TCHAR) * 13 );
	

	for( i = 0; i < 8; ++i )
		m_PopUpButton[i] = NULL;	
	
	
	m_DrawPosPopUp.x = m_DrawPosPopUp.y = 0;
	m_PopUpCount = 0;
	m_ManagerCount = 0;
	m_ShowPopUpLine = FALSE;

	m_SortLevelFlag	= FALSE;
	m_SortZoneFlag	= FALSE;
	m_SortOnlineFlag	= FALSE;

	m_SortTypeList = NULL;
	m_NoticeScrollBar = NULL;
	m_NoticeTextScrollPos = 0;
	m_NoticeTextLength = 0;
	m_NoticeTextTotalCount = 1;
	m_NoticeViewTextTotalCount = 1;
	m_CurrentNoticeLinePos = 0;	
	memset(m_CurrentSortName, 0, sizeof(TCHAR)*128);
	memset(m_NoticeTempText, 0, sizeof(TCHAR)*256*128);
	memset(m_NoticeViewText, 0, sizeof(TCHAR)*256*128);	
	m_NoticeText.reset();
	m_SendNoticeTempText.reset();
	m_NoticeMode = _XNOTICEMODE_READ;
	m_pNoticeWrite = NULL;
	m_pNoticemodify = NULL;

	m_SelectedTab = _XGROUPTAB_GROUPCLASS1;

#ifdef _XTS_ALIANCE_20061018	
	m_pImageAlienceMemberBorder	= NULL;		// 영웅단 동맹 맴버 버튼 테두리	
	m_pImageAlienceMemberEmptyBorder	= NULL;// 영웅단 동맹 맴버공석 이미지 
#endif

	
#ifdef _XDEF_CASTLEBATTLE	
	m_pImageCastleBattleBorder1		= NULL;			// 장원전 텍스드 바탕 이미지 1
	m_pImageCastleBattleBorder2		= NULL;			// 장원전 텍스드 바탕 이미지 2
	m_pBtnCastleBattleTimeChange	= NULL;			// 장원전 시간 변경 버튼
	m_pBtnCastleBattleGetMoney		= NULL;			// 장원전 보유금 획득 버튼

	m_pScheduleIcon[0]				= NULL;			// 장원전 일정 아이콘
	m_pScheduleIcon[1]				= NULL;

	memset(m_strDate, 0, sizeof(m_strDate));
	memset(m_strPropertyDate, 0, sizeof(m_strPropertyDate));
	
	m_lstboxChallengers				= NULL;
	m_pMyCastleInfo					= NULL;

	m_bSetCastleTooltip				= FALSE;
#endif

	m_TableBarShort		= NULL;
	m_TableBarLong		= NULL;
	m_lstboxOrgRecord	= NULL;
	
	m_nEntryCount			= 0;

	m_nWinCount			= 0;
	m_nLoseCount			= 0;
	m_nCombatCount		= 0;
	m_nWinRate			= 0;

	m_nManorIndex			= 0;

	m_nYear				= 0;
	m_nMonth				= 0;
	m_nDay				= 0;
	reserved			= 0;

	memset(m_strLastWarDate, 0, sizeof(m_strLastWarDate));

	m_pMnDWindow = NULL; //Author : 양희왕 //breif : 사제관련
}

_XWindow_Group::~_XWindow_Group()
{
	DestroyWindow();
}

BOOL _XWindow_Group::Initialize( void )
{
	int skillwindowindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
	int scrollresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga");

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		_XCHECKBTN_STRUCTURE tabbtnstruct = 
		{ 
			TRUE, 
			{ 57, 1 }, 
			{ 102, 18 }, 
			_XDEF_GROUP_ORGANIZATION,
			skillwindowindex,
			skillwindowindex,
			&g_MainInterfaceTextureArchive 
		};

		for(int i = 0 ; i < 4 ; ++i)
		{
			m_BaseTabButton[i] = new _XCheckButton;

			if(i == 1)
			{
				tabbtnstruct.position.x = 169;
				tabbtnstruct.position.y = 1;
				tabbtnstruct.commandid = _XDEF_GROUP_ALIENCE_ALIENCETAB;
			}
			if(i == 2)
			{
				tabbtnstruct.position.x = 23;
				tabbtnstruct.position.y = 19;
				tabbtnstruct.commandid = _XDEF_GROUP_CASTLEBATTLE_CASTLEBATTLETAB;
			}
			if(i == 3)
			{
				tabbtnstruct.position.x = 203;
				tabbtnstruct.position.y = 19;
				tabbtnstruct.commandid = _XDEF_GROUP_CASTLEBATTLE_WARSITUATIONTAB;
			}

			m_BaseTabButton[i]->Create( tabbtnstruct );
			m_BaseTabButton[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 5, 179, 107, 197);
			m_BaseTabButton[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 5, 204, 107, 222);

			m_BaseTabButton[i]->SetCheck(FALSE);
			m_BaseTabButton[i]->ShowWindow(TRUE);
			m_BaseTabButton[i]->EnableWindow(FALSE);
			m_BaseTabButton[i]->SetButtonSwapMode(TRUE);
			InsertChildObject( m_BaseTabButton[i] );
		}

		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ORGANIZATION );// _T("영웅단")
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS2]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3129 );// _T("방회")
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS3]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3229 );// _T("장원")
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS4]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3230 );// _T("장원전황")

		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->SetCheck(TRUE);
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->EnableWindow(TRUE);

#ifdef _XTS_ALIANCE_20061018
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS2]->EnableWindow(TRUE);
#endif

#ifdef _XDEF_CASTLEBATTLE
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS3]->EnableWindow(TRUE);
#endif

#ifdef _XDEF_CASTLEBATTLE_RECORD
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS4]->EnableWindow(TRUE);
#endif
	}
	else
	{
		int tabindex = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );

		_XCHECKBTN_STRUCTURE tabbtnstruct = 
		{ 
			TRUE, 
			{7, 11 }, 
			{ 64, 24 }, 
			_XDEF_GROUP_ORGANIZATION,

			tabindex,
			tabindex,
			&g_MainInterfaceTextureArchive 
		};

	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
		for( int i = 0; i < _XGROUPTAB_TOTALCOUNT; i++)
		{
			m_BaseTabButton[i] = new _XCheckButton;
			m_BaseTabButton[i]->Create( tabbtnstruct );
			m_BaseTabButton[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 2, 230, 66, 254);
			m_BaseTabButton[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 69, 230, 133, 254);

			m_BaseTabButton[i]->SetCheck(FALSE);
			m_BaseTabButton[i]->ShowWindow(TRUE);
			m_BaseTabButton[i]->EnableWindow(TRUE);
			m_BaseTabButton[i]->SetButtonSwapMode(TRUE);
			InsertChildObject( m_BaseTabButton[i] );
		}

		m_BaseTabButton[_XGROUPTAB_GROUPCLASS5]->MoveWindow( 7, 24 );
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS5]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CINFO_SAJAE );// _T("사제")
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS5]->SetCommandID( _XDEF_GROUP_MASTERNDISCIPLETAB );

		tabbtnstruct.position.x += (tabbtnstruct.windowsize.cx+2);
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->MoveWindow( tabbtnstruct.position.x, 24 );
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ORGANIZATION );// 영웅단
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->SetCommandID( _XDEF_GROUP_ORGANIZATION );

		tabbtnstruct.position.x += (tabbtnstruct.windowsize.cx+2);
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS2]->MoveWindow( tabbtnstruct.position.x, 24 );
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS2]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3129 );// 방회
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS2]->SetCommandID( _XDEF_GROUP_ALIENCE_ALIENCETAB );

		tabbtnstruct.position.x += (tabbtnstruct.windowsize.cx+2);
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS3]->MoveWindow( tabbtnstruct.position.x, 24 );
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS3]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3229 );// 장원
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS3]->SetCommandID( _XDEF_GROUP_CASTLEBATTLE_CASTLEBATTLETAB );

		tabbtnstruct.position.x += (tabbtnstruct.windowsize.cx+2);
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS4]->MoveWindow( tabbtnstruct.position.x, 24 );
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS4]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3230 );// 장원전황
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS4]->SetCommandID( _XDEF_GROUP_CASTLEBATTLE_WARSITUATIONTAB );
		
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->SetCheck(TRUE);
	#else

		for(int i = 0 ; i < 4 ; ++i)
		{
			m_BaseTabButton[i] = new _XCheckButton;

			if(i == 1)
			{
				tabbtnstruct.position.x += (tabbtnstruct.windowsize.cx+2);
				tabbtnstruct.commandid = _XDEF_GROUP_ALIENCE_ALIENCETAB;
			}
			if(i == 2)
			{
				tabbtnstruct.position.x += (tabbtnstruct.windowsize.cx+2);
				tabbtnstruct.commandid = _XDEF_GROUP_CASTLEBATTLE_CASTLEBATTLETAB;
			}
			if(i == 3)
			{
				tabbtnstruct.position.x += (tabbtnstruct.windowsize.cx+2);
				tabbtnstruct.commandid = _XDEF_GROUP_CASTLEBATTLE_WARSITUATIONTAB;
			}

			m_BaseTabButton[i]->Create( tabbtnstruct );
			m_BaseTabButton[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 2, 230, 66, 254);
			m_BaseTabButton[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 69, 230, 133, 254);

			m_BaseTabButton[i]->SetCheck(FALSE);
			m_BaseTabButton[i]->ShowWindow(TRUE);
			m_BaseTabButton[i]->EnableWindow(FALSE);
			m_BaseTabButton[i]->SetButtonSwapMode(TRUE);
			InsertChildObject( m_BaseTabButton[i] );
		}

		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ORGANIZATION );// _T("영웅단")
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS2]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3129 );// _T("방회")
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS3]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3229 );// _T("장원")
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS4]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3230 );// _T("장원전황")

		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->SetCheck(TRUE);
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->EnableWindow(TRUE);

#ifdef _XTS_ALIANCE_20061018
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS2]->EnableWindow(TRUE);
#endif

#ifdef _XDEF_CASTLEBATTLE
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS3]->EnableWindow(TRUE);
#endif

#ifdef _XDEF_CASTLEBATTLE_RECORD
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS4]->EnableWindow(TRUE);
#endif
	#endif //_XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER
	}
	
	// 탈퇴 버튼-----------------------------------------------------------------------------=
#ifdef _XTS_ALIANCE_20061018
	_XBTN_STRUCTURE destroybtnstruct = { TRUE, { 8, 361 }, { 108, 24 }, _XDEF_GROUP_ALIENCE_ALIENCEDESTROY,
		skillwindowindex,skillwindowindex,skillwindowindex,	&g_MainInterfaceTextureArchive };
	
	m_pBtnAlienceDestroy = new _XButton;
	m_pBtnAlienceDestroy->Create( destroybtnstruct );
	m_pBtnAlienceDestroy->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 );
	m_pBtnAlienceDestroy->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 );	
	m_pBtnAlienceDestroy->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 200, 110, 224 );
	m_pBtnAlienceDestroy->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3130 );//동맹탈퇴
	m_pBtnAlienceDestroy->ShowWindow(FALSE);
	InsertChildObject( m_pBtnAlienceDestroy );	
	
#endif
	
#ifdef _XDEF_CASTLEBATTLE	
	// 장원전 시간 변경 버튼
	_XBTN_STRUCTURE timechagebtnstruct = { TRUE, { 8, 361 }, { 108, 24 }, _XDEF_GROUP_CASTLEBATTLE_BTN_TIMECHANGE,
		skillwindowindex,skillwindowindex,skillwindowindex,	&g_MainInterfaceTextureArchive };
	
	m_pBtnCastleBattleTimeChange = new _XButton;
	m_pBtnCastleBattleTimeChange->Create( timechagebtnstruct );
	m_pBtnCastleBattleTimeChange->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 );
	m_pBtnCastleBattleTimeChange->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 );	
	m_pBtnCastleBattleTimeChange->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 200, 110, 224 );
	m_pBtnCastleBattleTimeChange->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3231 );//장원전 시간변경
	m_pBtnCastleBattleTimeChange->ShowWindow(FALSE);
	InsertChildObject( m_pBtnCastleBattleTimeChange );	

	// 장원전 보유금 획득 버튼
	_XBTN_STRUCTURE getmondybtnstruct = { TRUE, { 120, 361 }, { 108, 24 }, _XDEF_GROUP_CASTLEBATTLE_BTN_GETMONEY,
		skillwindowindex,skillwindowindex,skillwindowindex,	&g_MainInterfaceTextureArchive };
	
	m_pBtnCastleBattleGetMoney = new _XButton;
	m_pBtnCastleBattleGetMoney->Create( getmondybtnstruct );
	m_pBtnCastleBattleGetMoney->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 );
	m_pBtnCastleBattleGetMoney->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 );	
	m_pBtnCastleBattleGetMoney->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 200, 110, 224 );
	m_pBtnCastleBattleGetMoney->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3232 );//보유금 획득
	m_pBtnCastleBattleGetMoney->ShowWindow(FALSE);
	InsertChildObject( m_pBtnCastleBattleGetMoney );
	
	for(int i = 0 ; i < 2 ; ++i)
	{
		m_pScheduleIcon[i] = new _XImageStatic;
		m_pScheduleIcon[i]->Create(0, 0, 18, 18, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga"));
		m_pScheduleIcon[i]->SetClipRect(236, 116+(i*18), 254, 134+(i*18));
	}

	// List box
	_XLISTBOX_STRUCTURE tchallengerlistbox = 
	{
		TRUE,
		{10, 247},
		{329, 107},
		_XDEF_GROUP_CHALLENGER_LISTBOX,
		-1,
		&g_MainInterfaceTextureArchive,
		_T(""),
		21,
		5,
		_XLSTYLE_RIGHTSCROLLBAR,

		// scroll bar property
		{ 12, 107 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		255,												// 전체 리스트 갯수
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
	};

	m_lstboxChallengers = new _XListBox;
	m_lstboxChallengers->Create(tchallengerlistbox);

	_XVScrollBar* pScrollBar = m_lstboxChallengers->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	m_lstboxChallengers->SetAlwaysShowScrollBarMode(FALSE);

	POINT columnoffset1 = {10, 6};
	POINT columnoffset2 = {34, 8};
	m_lstboxChallengers->SetColumnOffset(0, columnoffset1);
	m_lstboxChallengers->SetColumnOffset(1, columnoffset2);

	m_lstboxChallengers->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);
	m_lstboxChallengers->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);

	m_lstboxChallengers->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxChallengers->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);

	m_lstboxChallengers->SetColumnWidth(0, 15);
	m_lstboxChallengers->SetColumnWidth(1, 296);

	m_lstboxChallengers->SetTrackWidth(320);
	m_lstboxChallengers->EnableSelectItem(FALSE);
	m_lstboxChallengers->ShowWindow(FALSE);
	InsertChildObject(m_lstboxChallengers);

#endif
	int systemimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );

	_XBTN_STRUCTURE leavebtnstruct = { TRUE, { 8, 361 }, { 60, 24 }, 
		_XDEF_GROUP_ORGANIZATION_LEAVEBTN,
		systemimageindex, systemimageindex, systemimageindex,
		&g_MainInterfaceTextureArchive };	

	m_pBtnGroupLeave = new _XButton;
	m_pBtnGroupLeave->Create( leavebtnstruct );		
	m_pBtnGroupLeave->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	m_pBtnGroupLeave->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
	m_pBtnGroupLeave->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	m_pBtnGroupLeave->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_DROP );	// _T("탈퇴")
	InsertChildObject( m_pBtnGroupLeave );
	
#ifdef _XDEF_ALIANCE_WEBBOARD_20070130	
	int cashresourceindex	= g_MainInterfaceTextureArchive.FindResource("MI_Cash_01.tga");
	// 영웅단 게시판 바로가기 버튼
	_XBTN_STRUCTURE gotoorganizationbtnstruct = { TRUE, 
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
	{ 316, 105 }, 
#else
	{ 320, 12 }, 
#endif
	{ 20, 20 }, 
		_XDEF_GROUP_ALIENCE_BTN_GOTOORGANIZATION,
		cashresourceindex, cashresourceindex, cashresourceindex,
		&g_MainInterfaceTextureArchive };	
	
	m_pBtnGoToOrganization = new _XButton;
	m_pBtnGoToOrganization->Create( gotoorganizationbtnstruct );		
	m_pBtnGoToOrganization->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 209, 123, 229, 143 );
	m_pBtnGoToOrganization->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  209, 146, 229, 166 );	
	m_pBtnGoToOrganization->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  209, 169, 229, 189 );
	m_pBtnGoToOrganization->SetCallBackFunction_ToolTip( _XCallback_Group_GotoOrganization );
	
	InsertChildObject( m_pBtnGoToOrganization );

	// 영웅회 게시판 바로가기 버튼
	_XBTN_STRUCTURE gotoaliencebtnstruct = { TRUE, 
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
	{ 291, 105 }, 
#else
	{ 295, 12 }, 
#endif
	{ 20, 20 }, 
		_XDEF_GROUP_ALIENCE_BTN_GOTOALIENCE,
		cashresourceindex, cashresourceindex, cashresourceindex,
		&g_MainInterfaceTextureArchive };	
	
	m_pBtnGoToAliance = new _XButton;
	m_pBtnGoToAliance->Create( gotoaliencebtnstruct );		
	m_pBtnGoToAliance->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 233, 123, 253, 143 );
	m_pBtnGoToAliance->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  233, 146, 253, 166 );	
	m_pBtnGoToAliance->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  233, 169, 253, 189 );
	m_pBtnGoToAliance->SetCallBackFunction_ToolTip( _XCallback_Group_GotoAlience );
	InsertChildObject( m_pBtnGoToAliance );

#endif
	// ---------------------------------------------------------------------------------------------=

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
	InsertChildObject( m_pCotributionBorder );


	m_pContributionGaugeLeftBorder = new _XImageStatic;
	m_pContributionGaugeLeftBorder->Create( 118, 79, 229, 91, &g_MainInterfaceTextureArchive, charresourceindex );
	m_pContributionGaugeLeftBorder->SetClipRect( 0, 60, 109, 72 );
	InsertChildObject( m_pContributionGaugeLeftBorder );

	m_pContributionGaugeRightBorder = new _XImageStatic;
	m_pContributionGaugeRightBorder->Create( 336, 79, 228, 91, &g_MainInterfaceTextureArchive, charresourceindex );
	m_pContributionGaugeRightBorder->SetClipRect( 0, 60, 109, 72 );
	m_pContributionGaugeRightBorder->SetScale( -1.0f, 1.0f );
	InsertChildObject( m_pContributionGaugeRightBorder );

	int messengerimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );

	// 문파 기여도 가산
	m_pImageContributionRateLeftBorder = new _XImageStatic;
	m_pImageContributionRateLeftBorder->Create( 173, 362, 254, 384, &g_MainInterfaceTextureArchive, messengerimageindex );
	m_pImageContributionRateLeftBorder->SetClipRect( 33, 229, 114, 251 );
	InsertChildObject( m_pImageContributionRateLeftBorder);

	m_pImageContributionRateRightBorder = new _XImageStatic;
	m_pImageContributionRateRightBorder->Create( 335, 362, 416, 384, &g_MainInterfaceTextureArchive, messengerimageindex );
	m_pImageContributionRateRightBorder->SetClipRect( 33, 229, 114, 251 );
	m_pImageContributionRateRightBorder->SetScale( -1.0f, 1.0f );
	InsertChildObject( m_pImageContributionRateRightBorder );

	// 정렬 리스트
	m_pImageLineUpLeftBorder = new _XImageStatic;
	m_pImageLineUpLeftBorder->Create( 70, 362, 120, 384, &g_MainInterfaceTextureArchive, messengerimageindex );
	m_pImageLineUpLeftBorder->SetClipRect( 33, 229, 83, 251 );
	InsertChildObject( m_pImageLineUpLeftBorder );

	m_pImageLineUpRightBorder  = new _XImageStatic;
	m_pImageLineUpRightBorder ->Create( 170, 362, 230, 384, &g_MainInterfaceTextureArchive, messengerimageindex );
	m_pImageLineUpRightBorder ->SetClipRect( 33, 229, 83, 251 ); 
	m_pImageLineUpRightBorder ->SetScale( -1.0f, 1.0f );
	InsertChildObject( m_pImageLineUpRightBorder  );

	_XBTN_STRUCTURE lineupdropdownbtnstruct = { TRUE, { 153, 367 }, { 12, 12 }, _XDEF_GROUP_ORGANIZATION_SORTBTN,
													  g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" ), 
													  g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" ), 
													  g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" ),
													  &g_MainInterfaceTextureArchive };
	
	m_pBtnLineUpDropDown = new _XButton;
	m_pBtnLineUpDropDown->Create( lineupdropdownbtnstruct );	
	m_pBtnLineUpDropDown->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,0,  12, 12, 24 );
	m_pBtnLineUpDropDown->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 12, 12, 24, 24 );
	m_pBtnLineUpDropDown->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 24, 12, 36, 24 );
	InsertChildObject( m_pBtnLineUpDropDown );

	_XLISTBOX_STRUCTURE sortlistboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 70, 384 },		// 윈도우 좌표
		{ 150, 45 },	// 윈도우 사이즈
		_XDEF_GROUP_ORGANIZATION_SORTLIST,				// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// 윈도우 타이틀
		15,												// 리스트 아이템 라인 피치
		10,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_NOSCROLLBAR						// 스크롤바의 위치
	};
	
	m_SortTypeList = new _XListBox;
	m_SortTypeList->Create(sortlistboxstructure);
	_XStatic* tempstatic2 = m_SortTypeList->GetBorderStatic();
	tempstatic2->SetFaceColor(0xFF000000);		
	
	POINT coloffset = { 2, 2 };
	m_SortTypeList->SetColumnOffset(0, coloffset);				// column offset	
	m_SortTypeList->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style 	
	m_SortTypeList->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type	
	m_SortTypeList->SetColumnWidth(0, 148);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_SortTypeList->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_SortTypeList->SetSelectedImagePosition(0, 1);
	m_SortTypeList->SetSelectedImageHeight(15);
	m_SortTypeList->SetTrackWidth(100);
	m_SortTypeList->SetScrollPos(0);

	m_SortTypeList->InsertListItem(_T(""), 0, 1 );		
	m_SortTypeList->SetItemText(0, 0, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_SORTLEVEL));
	m_SortTypeList->SetItemAttrib(0, 0, 0xFFC9CAC6);
	m_SortTypeList->SetItemAttribHighlight(0, 0, 0xFFC9CAC6);				

	m_SortTypeList->InsertListItem(_T(""), 0, 2 );		
	m_SortTypeList->SetItemText(1, 0, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_SORTZONE));
	m_SortTypeList->SetItemAttrib(1, 0, 0xFFC9CAC6);
	m_SortTypeList->SetItemAttribHighlight(1, 0, 0xFFC9CAC6);

	m_SortTypeList->InsertListItem(_T(""), 0, 3 );		
	m_SortTypeList->SetItemText(2, 0, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_SORTONLINE));
	m_SortTypeList->SetItemAttrib(2, 0, 0xFFC9CAC6);
	m_SortTypeList->SetItemAttribHighlight(2, 0, 0xFFC9CAC6);
	
	m_SortTypeList->ShowWindow(FALSE);	
	InsertChildObject(m_SortTypeList);

	// 글쓰기, 수정 버튼
	_XBTN_STRUCTURE noticebtnstruct = { TRUE,  
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
	{ 316, 135 }, 
#else
	{ 316, 101 }, 
#endif
	{ 24, 42 }, 
		_XDEF_GROUP_ORGANIZATION_NOTICEWRITE,
		g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" ), 
		g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" ),
		g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" ),
		&g_MainInterfaceTextureArchive };
	
	m_pNoticeWrite = new _XButton;
	m_pNoticeWrite->Create( noticebtnstruct );		
	m_pNoticeWrite->SetCallBackFunction_ToolTip( _XCallback_MM_Write );
	m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  47, 192, 71, 234 );	
	m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 20, 192, 44, 234 );	
	m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  74, 192, 98, 234 );	
	m_pNoticeWrite->EnableWindow(FALSE);	
	InsertChildObject( m_pNoticeWrite );

	noticebtnstruct.position.y += 41;
	noticebtnstruct.commandid++;

	m_pNoticemodify = new _XButton;
	m_pNoticemodify->Create( noticebtnstruct );		
	m_pNoticemodify->SetCallBackFunction_ToolTip( _XCallback_MM_Modify );
	m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  128, 99, 152, 141 );
	m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 101, 99, 125, 141 );	
	m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  155, 99, 179, 141 );
	m_pNoticemodify->EnableWindow(FALSE);
	InsertChildObject( m_pNoticemodify );	

	// ---------------------------------------------------------------------------------------------=

	// 조직리스트 ----------------------------------------------------------------------------------=
	// scrollresourceindex : 위에서 사용됨

	_XLISTBOX_STRUCTURE organizationtructure = 
	{
		TRUE,											// 윈도우 활성화
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
		{ 4, 223 },		// 윈도우 좌표
		{ 337, 133 },	// 윈도우 사이즈
#else
		{ 4, 189 },		// 윈도우 좌표
		{ 337, 167 },	// 윈도우 사이즈
#endif
		_XDEF_GROUP_ORGANIZATION_LISTBOX,						// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		20,												// 리스트 아이템 라인 피치
		8,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
		{ 12, 133 },										// 스크롤바 사이즈
#else
		{ 12, 167 },										// 스크롤바 사이즈
#endif
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		100,												// 전체 리스트 갯수
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
	};

	m_OrganizationListBox = new _XListBox;
	m_OrganizationListBox->Create( organizationtructure );
	_XStatic* tempstatic = m_OrganizationListBox->GetBorderStatic();
	tempstatic->SetFaceColor(0xB2000000);
	_XVScrollBar* pOrgScrollBar = m_OrganizationListBox->GetScrollBarObject();
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
	m_OrganizationListBox->SetAlwaysShowScrollBarMode(TRUE);

	POINT org_columnoffset1 = { 5, 2 };
	POINT org_columnoffset2 = { 25, 4 };
	m_OrganizationListBox->SetColumnOffset(0, org_columnoffset1);				// column offset
	m_OrganizationListBox->SetColumnOffset(1, org_columnoffset2);
	m_OrganizationListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_OrganizationListBox->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_OrganizationListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_OrganizationListBox->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_OrganizationListBox->SetColumnWidth(0, 320);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_OrganizationListBox->SetColumnWidth(1, 320);
	m_OrganizationListBox->SetTrackBarColor( 0X33CDBEA2 );	
	m_OrganizationListBox->SetTextXOffset(10);
	m_OrganizationListBox->SetTrackWidth(300);

	m_OrganizationListBox->ShowWindow(FALSE);
	InsertChildObject(m_OrganizationListBox);

	// 공지창 IME 
	m_NoticeIMEControl.ResetContainer();
	m_NoticeIMEControl.ResizeWindow(290, 15);
	m_NoticeIMEControl.ShowWindow(FALSE);
	m_NoticeIMEControl.SetMaxLength(46);
	m_NoticeIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );

	m_NoticeIMEControl.SetEscapeKeyCallback( _XIMECallback_NoticeEditEscape );
	m_NoticeIMEControl.SetReturnKeyCallback( _XIMECallback_NoticeEditReturn );
	m_NoticeIMEControl.SetUpArrowKeyCallback( _XIMECallback_NoticeEditUp );
	m_NoticeIMEControl.SetDownArrowKeyCallback( _XIMECallback_NoticeEditDown );
	m_NoticeIMEControl.SetLeftArrowKeyCallback( _XIMECallback_NoticeEditLeft );
	m_NoticeIMEControl.SetRightArrowKeyCallback( _XIMECallback_NoticeEditRight );
	m_NoticeIMEControl.SetBackSpaceKeyCallback( _XIMECallback_NoticeEditBack );

	// 공지창 스크롤바 
	SAFE_DELETE( m_NoticeScrollBar );
	m_NoticeScrollBar = new _XVScrollBar;
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
	
	m_NoticeScrollBar->Create( noticescrbarstructure );
	m_NoticeScrollBar->ShowWindow( TRUE );
	m_NoticeScrollBar->SetViewNullBorder( TRUE );
	m_NoticeScrollBar->UpdateData();
	
	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_NoticeScrollBar->SetClipRect( cliprectstructure );

	// ---------------------------------------------------------------------------------------------=

	// 팝업 버튼들----------------------------------------------------------------------------------=
	int popupimageindex = g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName);

	_XBTN_STRUCTURE popupbtnstruct = 
	{
		TRUE, 
		{4, 4},
		{88, 19},
		_XDEF_GROUP_ORGANIZATION_FRIEND,
		popupimageindex,
		popupimageindex,
		popupimageindex,
		&g_MainInterfaceTextureArchive
	};

	for( int j = 0; j < 8; j++ )
	{
		m_PopUpButton[j] = new _XButton;
		m_PopUpButton[j]->Create(popupbtnstruct);
		m_PopUpButton[j]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_PopUpButton[j]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_PopUpButton[j]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_PopUpButton[j]->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));		
		m_PopUpButton[j]->EnableWindow(TRUE);
		m_PopUpButton[j]->ShowWindow(FALSE);

		popupbtnstruct.position.y += 18;
		popupbtnstruct.commandid++;
	}
	
	m_PopUpButton[0]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_2087);
	m_PopUpButton[1]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_2088);
	m_PopUpButton[2]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_2089);
	m_PopUpButton[3]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REQUESTPARTY_REQUESTPARTY);
	m_PopUpButton[4]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_POWERUP);
	m_PopUpButton[5]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_POWERDOWN);
	m_PopUpButton[6]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ENTRUST);
	m_PopUpButton[7]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_COMPULSIONDROP);
	
	for( j = 0; j < 8; j++ )
		InsertChildObject(m_PopUpButton[j]);
	// ---------------------------------------------------------------------------------------------=

	// 모서리 이미지--------------------------------------------------------------------------------=
	for( j = 0 ; j < 4 ; j++)
		m_CornerImage[j].Create(0, 0, 2, 2, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga"));

	m_CornerImage[0].SetClipRect(185, 216, 187, 218);
	m_CornerImage[1].SetClipRect(188, 216, 190, 218);
	m_CornerImage[2].SetClipRect(191, 216, 193, 218);
	m_CornerImage[3].SetClipRect(194, 216, 196, 218);
	// ---------------------------------------------------------------------------------------------=
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 방회 관련 인터페이스 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
#ifdef _XTS_ALIANCE_20061018	
	SAFE_DELETE(m_pImageAlienceMemberBorder);		// 영웅단 동맹 맴버 버튼 테두리	
	SAFE_DELETE(m_pImageAlienceMemberEmptyBorder);  // 영웅단 동맹 맴버공석 이미지 
	
	int alienceimageindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Alience1.tga" );
	int alienceimageindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Alience2.tga" );

	
	// 영웅단 동맹 창 바탕 ( 336 X 246)
	m_pImageAlienceBorder = new _XImageStatic;
	m_pImageAlienceBorder->Create( 6, 103, 342, 349, &g_MainInterfaceTextureArchive, alienceimageindex1 );
	m_pImageAlienceBorder->SetClipRect( 0, 0, 256, 256 );
	//m_pImageAlienceBorder->SetScale( 1.3125f, 0.9609375f );
	InsertChildObject( m_pImageAlienceBorder );
	
	// 영웅단 동맹 창 '방'자 ( 70 X 80 )
	m_pImageAlienceName1 = new _XImageStatic;
	m_pImageAlienceName1->Create( 6+20, 58+69, 76+20, 138+69, &g_MainInterfaceTextureArchive, alienceimageindex2 );
	m_pImageAlienceName1->SetClipRect( 0, 0, 70, 80 );
	InsertChildObject( m_pImageAlienceName1 );
	
	// 영웅단 동맹 창 '방'자 ( 80 X 80 )
	m_pImageAlienceName2  = new _XImageStatic;
	m_pImageAlienceName2->Create( 6+20, 58+165, 86+20, 138+165, &g_MainInterfaceTextureArchive, alienceimageindex2 );
	m_pImageAlienceName2->SetClipRect( 73, 0, 153, 80 );
	InsertChildObject( m_pImageAlienceName2);
	
	// 영웅단 동맹 맴버 버튼 테두리 ( 212 X 39 )
	m_pImageAlienceMemberBorder  = new _XImageStatic;
	m_pImageAlienceMemberBorder->Create( 111, 76, 111+212, 76+39, &g_MainInterfaceTextureArchive, alienceimageindex2 );
	m_pImageAlienceMemberBorder->SetClipRect( 43, 175, 255, 214 );
	
	// 영웅단 동맹 맴버공석 이미지 ( 212 X 39 )
	m_pImageAlienceMemberEmptyBorder  = new _XImageStatic;
	m_pImageAlienceMemberEmptyBorder->Create( 111, 76, 86+20, 138+180, &g_MainInterfaceTextureArchive, alienceimageindex2 );
	m_pImageAlienceMemberEmptyBorder->SetClipRect( 43, 216, 255, 255 );


	_XBTN_STRUCTURE aliencememberbtnstruct = { TRUE, { 111, 76 }, { 200, 25 },
		_XDEF_GROUP_ALIENCE_BTN_ALIENCEMEMBER1,
		alienceimageindex2,alienceimageindex2,alienceimageindex2,	
		&g_MainInterfaceTextureArchive };

	{
		for( int i = 0; i < 5; ++i )
		{
			++aliencememberbtnstruct.commandid;
			aliencememberbtnstruct.position.y += 46;

			m_pBtnAlienceName[i] = new _XButton;
			m_pBtnAlienceName[i]->Create( aliencememberbtnstruct );		
			m_pBtnAlienceName[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 55, 149, 255, 174 );
			m_pBtnAlienceName[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  55, 122, 255, 147 );	
			m_pBtnAlienceName[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  55, 95, 255, 120 );
			//m_pBtnAlienceName[i]->SetButtonText(_T("공석")); 
			m_pBtnAlienceName[0]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3133);
			m_pBtnAlienceName[i]->ShowWindow(FALSE);
			InsertChildObject( m_pBtnAlienceName[i] );
		}
	}
#endif
	
#ifdef _XDEF_CASTLEBATTLE
	int questimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
	// 장원전 텍스드 바탕 이미지 1
	m_pImageCastleBattleBorder1 = new _XImageStatic;
	m_pImageCastleBattleBorder1->Create( 14, 157, 233, 182, &g_MainInterfaceTextureArchive, questimageindex );
	m_pImageCastleBattleBorder1->SetClipRect(0, 117, 219, 142);
	
	// 장원전 텍스드 바탕 이미지 2
	m_pImageCastleBattleBorder2 = new _XImageStatic;
	m_pImageCastleBattleBorder2->Create( 233, 157, 316, 182, &g_MainInterfaceTextureArchive, questimageindex );
	m_pImageCastleBattleBorder2->SetClipRect(0, 143, 83, 168);
#endif

#ifdef _XDEF_CASTLEBATTLE_RECORD
	int tableresourceindex = g_MainInterfaceTextureArchive.FindResource(g_MI_LifeSkillTextureName);

	m_TableBarLong = new _XImageStatic;
	m_TableBarLong->Create(0, 0, 194, 20, &g_MainInterfaceTextureArchive, tableresourceindex);
	m_TableBarLong->SetClipRect(0, 32, 194, 52);

	m_TableBarShort = new _XImageStatic;
	m_TableBarShort->Create(0, 0, 29, 20, &g_MainInterfaceTextureArchive, tableresourceindex);
	m_TableBarShort->SetClipRect(0, 32, 29, 52);

	// 대전 리스트
	_XLISTBOX_STRUCTURE orgrecordstruct = 
	{
		TRUE,											// 윈도우 활성화
		{ 4, 191 },										// 윈도우 좌표
		{ 337, 183 },									// 윈도우 사이즈
		_XDEF_GROUP_CASTLEBATTLE_ORGRECORD,				// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		20,												// 리스트 아이템 라인 피치
		8,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 183 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		100,												// 전체 리스트 갯수
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
	};

	m_lstboxOrgRecord = new _XListBox;
	m_lstboxOrgRecord->Create(orgrecordstruct);

	pScrollBar = m_lstboxOrgRecord->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	m_lstboxOrgRecord->SetAlwaysShowScrollBarMode(FALSE);

	POINT offset1 = {29, 27};
	POINT offset2 = {226, 27};
	POINT offset3 = {256, 27};
	POINT offset4 = {286, 27};
	POINT offset5 = {316, 27};

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
	{
		offset1.x = 56;
		offset5.x = 320;
	}

	m_lstboxOrgRecord->SetColumnOffset(0, offset1);
	m_lstboxOrgRecord->SetColumnOffset(1, offset2);
	m_lstboxOrgRecord->SetColumnOffset(2, offset3);
	m_lstboxOrgRecord->SetColumnOffset(3, offset4);
	m_lstboxOrgRecord->SetColumnOffset(4, offset5);

	m_lstboxOrgRecord->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lstboxOrgRecord->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_lstboxOrgRecord->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	m_lstboxOrgRecord->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	m_lstboxOrgRecord->SetColumnStyle(4, _XLCOLUMNSTYLE_STRING);

	m_lstboxOrgRecord->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxOrgRecord->SetAlignType(1, _XFONT_ALIGNTYPE_RIGHT);
	m_lstboxOrgRecord->SetAlignType(2, _XFONT_ALIGNTYPE_RIGHT);
	m_lstboxOrgRecord->SetAlignType(3, _XFONT_ALIGNTYPE_RIGHT);
	m_lstboxOrgRecord->SetAlignType(4, _XFONT_ALIGNTYPE_RIGHT);

	m_lstboxOrgRecord->SetColumnWidth(0, 166);
	m_lstboxOrgRecord->SetColumnWidth(1, 30);
	m_lstboxOrgRecord->SetColumnWidth(2, 30);
	m_lstboxOrgRecord->SetColumnWidth(3, 30);
	m_lstboxOrgRecord->SetColumnWidth(4, 30);

	m_lstboxOrgRecord->SetTrackWidth(300);
	m_lstboxOrgRecord->EnableSelectItem(FALSE);
	InsertChildObject(m_lstboxOrgRecord);

#endif

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	//사제 관련 인터페이스
	if( !m_pMnDWindow )
	{
		m_pMnDWindow = new _XWindow_MnDGroup();
		m_pMnDWindow->Initialize();
	}
#endif

	return TRUE;
}

void _XWindow_Group::DestroyWindow( void )
{
	if( !m_OrganizationMemberList.empty() )
	{
		list<_Unit*>::iterator iter = m_OrganizationMemberList.begin();
		while( iter != m_OrganizationMemberList.end() )
		{
			_Unit* tempData = *iter;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter = m_OrganizationMemberList.erase( iter );
			}
			else
			{
				iter++;
			}
		}
	}

	if( !m_OrganizationMemberOutTimeList.empty() )
	{
		list<_OutTime*>::iterator iter2 = m_OrganizationMemberOutTimeList.begin();
		while ( iter2 != m_OrganizationMemberOutTimeList.end() ) 
		{
			_OutTime* tempData = *iter2;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter2 = m_OrganizationMemberOutTimeList.erase( iter2 );
			}
			else
			{
				iter2++;
			}
		}
	}
	
#ifdef _XTS_ALIANCE_20061018
	if( !m_listAlienceMember.empty() )
	{
		list<__ally_client_info*>::iterator iter = m_listAlienceMember.begin();
		while( iter != m_listAlienceMember.end() )
		{
			__ally_client_info* tempData = *iter;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter = m_listAlienceMember.erase( iter );
			}
			else
			{
				++iter;
			}
		}

		//Author : 양희왕 //breif : clear 도 해주자 //Date : 08/03/06
		m_listAlienceMember.clear();
	}
#endif
	_XWindow::DestroyWindow();

	SAFE_DELETE( m_NoticeScrollBar );

#ifdef _XTS_ALIANCE_20061018
	SAFE_DELETE( m_pImageAlienceMemberBorder );
	SAFE_DELETE( m_pImageAlienceMemberEmptyBorder );
#endif

#ifdef _XDEF_CASTLEBATTLE
	SAFE_DELETE(m_pImageCastleBattleBorder1);
	SAFE_DELETE(m_pImageCastleBattleBorder2);

	SAFE_DELETE(m_pScheduleIcon[0]);
	SAFE_DELETE(m_pScheduleIcon[1]);
#endif

	SAFE_DELETE(m_TableBarLong);
	SAFE_DELETE(m_TableBarShort);
}

void _XWindow_Group::ResetData()
{
	m_OrganizationListBox->DeleteAllItem();

	m_OrganizationBaseInfo.m_cBaseParty = 0;	
	memset( m_OrganizationBaseInfo.m_cCaptainName, 0, sizeof(TCHAR)*13 );
	memset( m_OrganizationBaseInfo.m_cName, 0, sizeof(TCHAR)*9 );
	m_OrganizationBaseInfo.m_cProperty = 0;
	m_OrganizationBaseInfo.m_iIndex = 0;
	m_OrganizationBaseInfo.m_uiContribution = 0;
	m_OrganizationBaseInfo.m_uiExperience = 0;
	m_OrganizationBaseInfo.m_usCurMemberNum = 0;
	
	for( int i = 0; i < 9; i++ )
		memset( m_GroupInfoText[i], 0, sizeof(TCHAR)*64);
	
	memset( m_SendName, 0, sizeof(TCHAR)*13);	
	m_UpdateUnit.reset();
	m_DrawPosPopUp.x = m_DrawPosPopUp.y = 0;
	m_PopUpCount = 0;
	m_ManagerCount = 0;
	m_ShowPopUpLine = FALSE;

	memset(m_NoticeTempText, 0, sizeof(TCHAR)*256*128);
	memset(m_NoticeViewText, 0, sizeof(TCHAR)*256*128);	
	m_NoticeText.reset();

	if( !m_OrganizationMemberList.empty() )
	{
		list<_Unit*>::iterator iter = m_OrganizationMemberList.begin();
		while( iter != m_OrganizationMemberList.end() )
		{
			_Unit* tempData = *iter;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter = m_OrganizationMemberList.erase( iter );
			}
			else
			{
				iter++;
			}
		}
	}	
	if( !m_OrganizationMemberOutTimeList.empty() )
	{
		list<_OutTime*>::iterator iter2 = m_OrganizationMemberOutTimeList.begin();
		while ( iter2 != m_OrganizationMemberOutTimeList.end() ) 
		{
			_OutTime* tempData = *iter2;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter2 = m_OrganizationMemberOutTimeList.erase( iter2 );
			}
			else
			{
				iter2++;
			}
		}
	}

	strcpy( m_CurrentSortName, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2099) );
	SelectNoticeMode(_XNOTICEMODE_READ);
/*	if( this->GetShowStatus() )
		this->ShowWindow(TRUE);
	else
		this->ShowWindow(FALSE);*/

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	if( m_pMnDWindow )
	{
		m_pMnDWindow->ResetData();
	}
#endif
}

void _XWindow_Group::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
	m_NoticeScrollBar->MoveWindow( m_WindowPosition.x+300, m_WindowPosition.y+138 );
#else
	m_NoticeScrollBar->MoveWindow( m_WindowPosition.x+300, m_WindowPosition.y+104 );
#endif

	if(m_NoticeMode == _XNOTICEMODE_MODIFY)
	{
		int linecount = 0;
		m_CurrentNoticeLinePos = m_NoticeTextTotalCount-1;
		if( m_NoticeTextTotalCount > 4 )
			linecount = 3;
		else
			linecount = m_NoticeTextTotalCount-1;			
		
		m_NoticeIMEControl.MoveWindow( m_WindowPosition.x+14, m_WindowPosition.y+111 +(linecount*17));
	}
	else
		m_NoticeIMEControl.MoveWindow( m_WindowPosition.x+14, m_WindowPosition.y+111 );

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	m_pMnDWindow->MoveWindow();
#endif
}

void _XWindow_Group::ShowWindow(BOOL show)
{	
	if( show )
	{
		m_ShowPopUpLine = FALSE;
	}
	else
	{
	}
	_XWindow::ShowWindow(show);
}



void _XWindow_Group::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	if( m_SelectedTab == _XGROUPTAB_GROUPCLASS1 )
	{	
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
		//겉에 테투리 큰줄
		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+37, m_WindowPosition.x+341, m_WindowPosition.y+132 , D3DCOLOR_ARGB(100,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+38, m_WindowPosition.x+339, m_WindowPosition.y+130 , 0.0f, 0xFFCCBF9E );

		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+136, m_WindowPosition.x+314, m_WindowPosition.y+217 , D3DCOLOR_ARGB(175,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+137, m_WindowPosition.x+312, m_WindowPosition.y+215 , 0.0f, 0xFFCCBF9E );

		m_NoticeTextScrollPos = m_NoticeScrollBar->GetLineScrollPos();
		m_NoticeScrollBar->Draw( pfocusobject );	

		if( m_NoticeTextTotalCount < 5 || (m_CurrentNoticeLinePos <= m_NoticeTextScrollPos+3) )
		{
			if( m_NoticeIMEControl.GetFocus() )
				m_NoticeIMEControl.Draw();
		}

		int linecount = 0;
		g_XBaseFont->SetColor( 0xFFC0EB7F );

		if( m_NoticeMode == _XNOTICEMODE_READ ) 
		{	
			for( int i = m_NoticeTextScrollPos ; i < m_NoticeTextScrollPos+4; i++)
			{
				g_XBaseFont->Puts( m_WindowPosition.x+14, m_WindowPosition.y+145+(linecount*17), m_NoticeViewText[i] );
				linecount++;
			}	
		}
		else
		{
			for( int i = m_NoticeTextScrollPos ; i < m_NoticeTextScrollPos+4; i++)
			{
				g_XBaseFont->Puts( m_WindowPosition.x+14, m_WindowPosition.y+145+(linecount*17), m_NoticeTempText[i] );
				linecount++;
			}
		}

		POINT listwinpos = m_OrganizationListBox->GetWindowPos();
		SIZE listwinsize = m_OrganizationListBox->GetWindowSize();	
		
		_XDrawRectAngle( listwinpos.x, listwinpos.y-1,
			listwinpos.x+listwinsize.cx- 13, listwinpos.y+listwinsize.cy-1, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( listwinpos.x+1, listwinpos.y,
			listwinpos.x+listwinsize.cx - 14, listwinpos.y+listwinsize.cy-2, 0.0f, D3DCOLOR_ARGB(191,0,0,0) );
		_XDrawRectAngle( listwinpos.x+2, listwinpos.y+1,
			listwinpos.x+listwinsize.cx - 15, listwinpos.y+listwinsize.cy-3, 0.0f, D3DCOLOR_ARGB(63,0,0,0) );
		
		_XWindow::Draw(pfocusobject);	

		/*if( m_ShowPopUpLine )
		{
			DrawPopUp();
			for( int i = 0; i < 8; i++ )
			{
				m_PopUpButton[i]->Draw(pfocusobject);
			}
		}*/
#else
		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+37, m_WindowPosition.x+341, m_WindowPosition.y+98 , D3DCOLOR_ARGB(100,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+38, m_WindowPosition.x+339, m_WindowPosition.y+96 , 0.0f, 0xFFCCBF9E );

		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+102, m_WindowPosition.x+314, m_WindowPosition.y+183 , D3DCOLOR_ARGB(175,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+103, m_WindowPosition.x+312, m_WindowPosition.y+181 , 0.0f, 0xFFCCBF9E );

		m_NoticeTextScrollPos = m_NoticeScrollBar->GetLineScrollPos();
		m_NoticeScrollBar->Draw( pfocusobject );	

		if( m_NoticeTextTotalCount < 5 || (m_CurrentNoticeLinePos <= m_NoticeTextScrollPos+3) )
		{
			if( m_NoticeIMEControl.GetFocus() )
				m_NoticeIMEControl.Draw();
		}

		int linecount = 0;
		g_XBaseFont->SetColor( 0xFFC0EB7F );		

		if( m_NoticeMode == _XNOTICEMODE_READ ) 
		{	
			for( int i = m_NoticeTextScrollPos ; i < m_NoticeTextScrollPos+4; i++)
			{
				g_XBaseFont->Puts( m_WindowPosition.x+14, m_WindowPosition.y+111+(linecount*17), m_NoticeViewText[i] );
				linecount++;
			}	
		}
		else
		{
			for( int i = m_NoticeTextScrollPos ; i < m_NoticeTextScrollPos+4; i++)
			{
				g_XBaseFont->Puts( m_WindowPosition.x+14, m_WindowPosition.y+111+(linecount*17), m_NoticeTempText[i] );
				linecount++;
			}
		}

		if( strlen( m_GroupInfoText[0] ))
		{
			g_XBaseFont->SetColor( 0xFFFFD514 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+45, _XFONT_ALIGNTYPE_LEFT, m_GroupInfoText[0] );
		}
		if( strlen( m_GroupInfoText[1] ))
		{
			g_XBaseFont->SetColor( 0xFFB3B3B3 );
			// 해외 버젼
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+170, m_WindowPosition.y+62, _XFONT_ALIGNTYPE_LEFT, m_GroupInfoText[1] );
			}
			else// 국내 버젼
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x+90, m_WindowPosition.y+45, _XFONT_ALIGNTYPE_LEFT, m_GroupInfoText[1] );
			}
		}
		if( strlen( m_GroupInfoText[2] ))
		{
			g_XBaseFont->SetColor( 0xFFD2D2D2 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+170, m_WindowPosition.y+45, _XFONT_ALIGNTYPE_LEFT, m_GroupInfoText[2] );
		}
		if( strlen( m_GroupInfoText[3] ))
		{
			g_XBaseFont->SetColor( 0xFFD5D49A );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+62, _XFONT_ALIGNTYPE_LEFT, m_GroupInfoText[3] );
		}
		if( strlen( m_GroupInfoText[4] ))
		{
			g_XBaseFont->SetColor( 0xFFFFD514 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+79, _XFONT_ALIGNTYPE_LEFT, m_GroupInfoText[4] );
		}

		if( strlen( m_GroupInfoText[5] ))
		{
			g_XBaseFont->SetColor( 0xFFFFD514 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+170, m_WindowPosition.y+62, _XFONT_ALIGNTYPE_CENTER, m_GroupInfoText[5] );
		}

		POINT listwinpos = m_OrganizationListBox->GetWindowPos();
		SIZE listwinsize = m_OrganizationListBox->GetWindowSize();	
		
		_XDrawRectAngle( listwinpos.x, listwinpos.y-1,
			listwinpos.x+listwinsize.cx- 13, listwinpos.y+listwinsize.cy-1, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		_XDrawRectAngle( listwinpos.x+1, listwinpos.y,
			listwinpos.x+listwinsize.cx - 14, listwinpos.y+listwinsize.cy-2, 0.0f, D3DCOLOR_ARGB(191,0,0,0) );
		_XDrawRectAngle( listwinpos.x+2, listwinpos.y+1,
			listwinpos.x+listwinsize.cx - 15, listwinpos.y+listwinsize.cy-3, 0.0f, D3DCOLOR_ARGB(63,0,0,0) );
		
		_XWindow::Draw(pfocusobject);	

		if( strlen( m_GroupInfoText[6] ))
		{
			g_XBaseFont->SetColor( 0xFF00FF06 );		
			g_XBaseFont->PutsAlign(m_WindowPosition.x+189, m_WindowPosition.y+368, _XFONT_ALIGNTYPE_LEFT, m_GroupInfoText[6] );
		}

		if( strlen(m_CurrentSortName) )
		{
			g_XBaseFont->SetColor( _XSC_DEFAULT );		
			g_XBaseFont->PutsAlign(m_WindowPosition.x+74, m_WindowPosition.y+368, _XFONT_ALIGNTYPE_LEFT, m_CurrentSortName );		
		}

		if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
		{
			if( m_OrganizationListBox->CheckMousePosition() )
			{
				int curindex = m_OrganizationListBox->GetHoverItem()-1;
				int selectindex = m_OrganizationListBox->GetSelectedItem();


				if( curindex != selectindex && curindex >= 0 && curindex < m_OrganizationListBox->GetItemCount() )
				{
					list<_Unit*>::iterator k = m_OrganizationMemberList.begin();
					if( curindex < m_OrganizationMemberList.size() )
						advance(k, curindex);			
					
					_Unit* TempUnit = *k;
					if( TempUnit )
					{
						if( !TempUnit->m_bOnline )
						{
							list<_OutTime*>::iterator kk = m_OrganizationMemberOutTimeList.begin();
							if( curindex < m_OrganizationMemberOutTimeList.size() )
								advance(kk, curindex);
							
							_OutTime* tempunitouttime = *kk;
							if( tempunitouttime )
							{
								TCHAR curUnitOutTime[128];
								memset( curUnitOutTime, 0, sizeof(TCHAR)*128 ) ;
								
								SYSTEMTIME time;
								GetLocalTime( &time );
								
								int servertime = (tempunitouttime->year+2000)*365+tempunitouttime->month*30 ;
								int clienttime = time.wYear*365+time.wMonth*30 ;
								bool flag = false;
								
								if( clienttime > servertime+30 )
								{
									flag = true;
								}

								if( flag )
								{
									strcpy( curUnitOutTime, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2395) ) ;
								}
								else
								{
									TCHAR tempStr[4];
									memset( tempStr, 0, sizeof(TCHAR)*4 );
									sprintf( tempStr, "%d", tempunitouttime->year+100 );
									strcpy( curUnitOutTime, &tempStr[1] );
									
									strcat( curUnitOutTime, "." );
									memset( tempStr, 0, sizeof(TCHAR)*4 );
									sprintf( tempStr, "%d", tempunitouttime->month+100 );
									strcat( curUnitOutTime, &tempStr[1] );
									
									strcat( curUnitOutTime, "." );
									memset( tempStr, 0, sizeof(TCHAR)*4 );
									sprintf( tempStr, "%d", tempunitouttime->day+100 );
									strcat( curUnitOutTime, &tempStr[1] );
									
									strcat( curUnitOutTime, " " );
									memset( tempStr, 0, sizeof(TCHAR)*4 );
									sprintf( tempStr, "%d", tempunitouttime->h+100 );
									strcat( curUnitOutTime, &tempStr[1] );
									
									strcat( curUnitOutTime, ":" );
									memset( tempStr, 0, sizeof(TCHAR)*4 );
									sprintf( tempStr, "%d", tempunitouttime->m+100 );
									strcat( curUnitOutTime, &tempStr[1] );								
								}
								ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
								g_InterfaceTooltip.SetToolTipString( scrnpos->x+20, scrnpos->z, curUnitOutTime, _XSC_INFORMATION );
							}
						}							
					}
				}		
			}
		}
		
		g_XBaseFont->Flush();
		
		if( m_ShowPopUpLine )
		{
			DrawPopUp();
			for( int i = 0; i < 8; i++ )
			{
				m_PopUpButton[i]->Draw(pfocusobject);
			}
		}
#endif
	}

#ifdef _XTS_ALIANCE_20061018
	else if( m_SelectedTab == _XGROUPTAB_GROUPCLASS2 )
	{
		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+37, m_WindowPosition.x+341, m_WindowPosition.y+98 , D3DCOLOR_ARGB(100,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+38, m_WindowPosition.x+339, m_WindowPosition.y+96 , 0.0f, 0xFFCCBF9E );
		
		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+103, m_WindowPosition.x+341, m_WindowPosition.y+360 , D3DCOLOR_ARGB(200,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+104, m_WindowPosition.x+339, m_WindowPosition.y+358 , 0.0f, 0xFFCCBF9E );
		
		_XWindow::Draw(pfocusobject);
		
		if( m_UniqueIndexAlience > 0 )
		{
			g_XBaseFont->SetColor( 0xFFFFD514 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+10, m_WindowPosition.y+45, _XFONT_ALIGNTYPE_LEFT, g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cName );
			
			g_XBaseFont->SetColor( 0xFFD5D49A );
			TCHAR tempStr[256];
			memset( tempStr, 0, sizeof(TCHAR)*256 );
			sprintf( tempStr, _T("%s : %s"), _XGETINTERFACETEXT(ID_STRING_NEW_3126), m_AlienceMasterName );
			g_XBaseFont->Puts(m_WindowPosition.x+10, m_WindowPosition.y+62, tempStr);	//회주
			//g_XBaseFont->Puts(m_WindowPosition.x+10, m_WindowPosition.y+62, _XGETINTERFACETEXT(ID_STRING_NEW_3126));	//회주
			//g_XBaseFont->Puts(m_WindowPosition.x+46, m_WindowPosition.y+62, tempStr );
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFFFD514 );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+170, m_WindowPosition.y+62, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3134) );
		}
		
		POINT btnposition;
		for( int i = 0; i < 5; ++i )
		{			
			if( m_pBtnAlienceName[i]->GetShowStatus() )
			{
				btnposition = m_pBtnAlienceName[i]->GetWindowPos();
				m_pImageAlienceMemberBorder->Draw( btnposition.x-6, btnposition.y-7 );
			}
			else
			{
				btnposition = m_pBtnAlienceName[i]->GetWindowPos();
				m_pImageAlienceMemberEmptyBorder->Draw( btnposition.x-6, btnposition.y-7 );
				g_XBaseFont->SetColor( _XSC_DEFAULT );	
				g_XBaseFont->PutsAlign(btnposition.x-6+106, btnposition.y-7+13, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3133) );
			}
		}
		
		g_XBaseFont->Flush();
		
	}
#endif

#ifdef _XDEF_CASTLEBATTLE
	else if( m_SelectedTab == _XGROUPTAB_GROUPCLASS3 )
	{
		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+45, m_WindowPosition.x+341, m_WindowPosition.y+45+88 , D3DCOLOR_ARGB(100,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+46, m_WindowPosition.x+339, m_WindowPosition.y+45+86 , 0.0f, 0xFFCCBF9E );
		
		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+45+10+88, m_WindowPosition.x+341, m_WindowPosition.y+45+10+88+82 , D3DCOLOR_ARGB(100,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+46+10+88, m_WindowPosition.x+339, m_WindowPosition.y+45+10+88+80 , 0.0f, 0xFFCCBF9E );
		
		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+45+20+88+82, m_WindowPosition.x+341, m_WindowPosition.y+45+20+88+82+125 , D3DCOLOR_ARGB(100,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+46+20+88+82, m_WindowPosition.x+339, m_WindowPosition.y+45+20+88+82+123 , 0.0f, 0xFFCCBF9E );
		
		m_pImageCastleBattleBorder1->Draw(m_WindowPosition.x+23, m_WindowPosition.y+37);
		m_pImageCastleBattleBorder2->Draw(m_WindowPosition.x+23+219, m_WindowPosition.y+37);
		
		m_pImageCastleBattleBorder1->Draw(m_WindowPosition.x+23, m_WindowPosition.y+37+98);
		m_pImageCastleBattleBorder2->Draw(m_WindowPosition.x+23+219, m_WindowPosition.y+37+98);
		
		m_pImageCastleBattleBorder1->Draw(m_WindowPosition.x+23, m_WindowPosition.y+37+98+92);
		m_pImageCastleBattleBorder2->Draw(m_WindowPosition.x+23+219, m_WindowPosition.y+37+98+92);
		
		
		_XWindow::Draw(pfocusobject);

		DrawOwnerInfo();
	}
	else if( m_SelectedTab == _XGROUPTAB_GROUPCLASS4 )
	{
#ifdef _XDEF_CASTLEBATTLE_RECORD
		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+70, m_WindowPosition.x+340, m_WindowPosition.y+118 , 0x77000000 );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+71, m_WindowPosition.x+338, m_WindowPosition.y+116 , 0.0f, 0xFF7E7E74 );
		
		_XDrawSolidBar( m_WindowPosition. x+4, m_WindowPosition.y+147, m_WindowPosition.x+340, m_WindowPosition.y+188 , 0x77000000 );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+148, m_WindowPosition.x+338, m_WindowPosition.y+186 , 0.0f, 0xFF7E7E74 );
		
		_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+191, m_WindowPosition.x+341, m_WindowPosition.y+374 , 0x77000000 );
		
		m_pImageCastleBattleBorder1->Draw(m_WindowPosition.x+23, m_WindowPosition.y+43);
		m_pImageCastleBattleBorder2->Draw(m_WindowPosition.x+23+219, m_WindowPosition.y+43);
		
		m_pImageCastleBattleBorder1->Draw(m_WindowPosition.x+23, m_WindowPosition.y+123);
		m_pImageCastleBattleBorder2->Draw(m_WindowPosition.x+23+219, m_WindowPosition.y+123);
			
		for(int i = 0 ; i < 9 ; ++i)
		{
			m_TableBarLong->Draw(m_WindowPosition.x+8, m_WindowPosition.y+193+i*20);
		}

		for(i = 0 ; i < 4 ; ++i)
		{
			m_TableBarShort->Draw(m_WindowPosition.x+203+i*30, m_WindowPosition.y+193);
		}
		
		_XWindow::Draw(pfocusobject);

		DrawCBOrgTotalRecord();
#endif
	}
#endif

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	if( m_SelectedTab == _XGROUPTAB_GROUPCLASS5)		//사제
	{
		m_pMnDWindow->Draw(m_WindowPosition.x, m_WindowPosition.y);

		_XWindow::Draw(pfocusobject);
	}
#endif
}

void _XWindow_Group::DrawPopUp()
{
	_XDrawRectAngle( m_DrawPosPopUp.x-3, m_DrawPosPopUp.y-3, m_DrawPosPopUp.x+87+3, m_DrawPosPopUp.y+(m_PopUpCount*18)+2, 0.0f,0xFF000000 );
	_XDrawRectAngle( m_DrawPosPopUp.x-2, m_DrawPosPopUp.y-2, m_DrawPosPopUp.x+87+2, m_DrawPosPopUp.y+(m_PopUpCount*18)+1, 0.0f,0xFFD2D2D2 );
	_XDrawRectAngle( m_DrawPosPopUp.x-1, m_DrawPosPopUp.y-1, m_DrawPosPopUp.x+87+1, m_DrawPosPopUp.y+(m_PopUpCount*18), 0.0f,0xFF000000 );
}

BOOL _XWindow_Group::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)		return FALSE;
	if(!this->m_ShowWindow) 
	{		
		return FALSE;
	}

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(mousestate)
	{
		if(mousestate->bButton[0])
		{
			if(CheckMousePosition())
			{
				// Focus
				if( scrnpos->x < m_WindowPosition.x+11 || scrnpos->x > m_WindowPosition.x+331 ||		 // listbox area
					scrnpos->z < m_WindowPosition.y+189 || scrnpos->z > m_WindowPosition.y+357 )
				{
					m_ShowPopUpLine = FALSE;
					for( int i = 0; i < 8; i++ )
					{
						m_PopUpButton[i]->ShowWindow( FALSE );
					}

				#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
					m_pMnDWindow->ShowPopupWindow(FALSE); //리스트영역 이외 영역,, 팝업버튼을 없앤다
				#endif
				}
				else
				{
					if( !m_SortTypeList->CheckMousePosition() )
					{
						m_SortTypeList->ShowWindow(FALSE);
					}
				}
			}
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
			else
			{
				m_pMnDWindow->ShowPopupWindow(FALSE);
			}
		#endif
		}
	}	
	m_NoticeScrollBar->Process(pfocusobject);

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	if( m_SelectedTab == _XGROUPTAB_GROUPCLASS5)		//사제
		m_pMnDWindow->Process();
#endif
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	return TRUE;
}

void _XWindow_Group::CopyBaseInfo(_OrganizationBase* data)
{
	if( m_SelectedTab == _XGROUPTAB_GROUPCLASS1)			//영웅단
	{
		m_pCotributionBorder->ShowWindow(TRUE);
		m_pContributionGaugeLeftBorder->ShowWindow(TRUE);
		m_pContributionGaugeRightBorder->ShowWindow(TRUE);
	}
	memset( m_GroupInfoText[5], 0, sizeof(TCHAR)*32 ); 

	m_OrganizationBaseInfo = *data;	

	g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex = m_OrganizationBaseInfo.m_iIndex;
	strcpy( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName, m_OrganizationBaseInfo.m_cName );
	g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cProperty = m_OrganizationBaseInfo.m_cProperty;
	
	strcpy( m_CurrentSortName, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2099) );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

	if( pChatDefault_Window->m_pChatFilterButton[3] )
	{
		if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 || g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
		{
			pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( TRUE );
		}
		else
		{
			pChatDefault_Window->m_pChatFilterButton[3]->EnableWindow( FALSE );
		}
	}
#endif
}

void _XWindow_Group::CopyMemberList(int count, _Unit* unitlist)
{
	if( !m_OrganizationMemberList.empty() )
	{
		list<_Unit*>::iterator iter = m_OrganizationMemberList.begin();
		while( iter != m_OrganizationMemberList.end() )
		{
			_Unit* tempData = *iter;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter = m_OrganizationMemberList.erase( iter );
			}
			else
			{
				iter++;
			}
		}
	}
	
	m_ManagerCount = 0;
	for( int i = 0; i < count; i++ )
	{
		_Unit* tempunit = new _Unit;
		tempunit->reset();
		*tempunit = *(unitlist+i);
		if( strlen(tempunit->m_cName) )
		{
			m_OrganizationMemberList.push_back(tempunit);
			
			if( strcmp( g_pLocalUser->m_CharacterInfo.charactername, tempunit->m_cName ) == 0 )
			{
				g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass = tempunit->m_cClass;
#ifdef _XTS_ALIANCE_20061018
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
				{					
					_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
					if(pRequestParty_Window)
					{
						pRequestParty_Window->m_RequestAlienceButton->EnableWindow(TRUE);
					}
				}
#endif
			}
			TCHAR memberstr[256];
			memset(memberstr, 0, 256);
			TCHAR tempstr[64];
			memset(tempstr, 0, 64);
			DWORD colorvalue = 0xFFFFFFFF;
			RECT rect = {0, 0, 0, 0};
			
			if( tempunit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_ )
			{
				strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_DEFAULTUNIT) );	// "단원"
				colorvalue = 0xFFD2D2D2;
			}
			else if( tempunit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_ )
			{
				strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_SPECIALUNIT) );	// "단장"
				colorvalue = 0xFFD2D2D2;			
				rect.left = 111;
				rect.top = 74;
				rect.right = 127;
				rect.bottom = 90;
				if( m_ManagerCount < 9 )
					m_ManagerCount++;
			}
			else if( tempunit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
			{
				strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_CAPTAINUNIT) );	// "단주"
				colorvalue = 0xFFD5D49A;
				rect.left = 94;
				rect.top = 74;
				rect.right = 110;
				rect.bottom = 90;
			}
			if( !tempunit->m_bOnline )
				colorvalue = 0xFF555555;
			
			sprintf( tempstr, " : %s", tempunit->m_cName );
			strcat( memberstr, tempstr );
			
			int level = g_UserLevelTable[tempunit->m_sInnerLevel].level;
			int sublevel = g_UserLevelTable[tempunit->m_sInnerLevel].sublevel;
			memset(tempstr, 0, 64);

			// " (%s %d성 / %s)"
			strcpy( tempstr, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ORGANIZATIONINFO1, 
															   g_LevelName[level], &sublevel, g_ZoneInfoTable[tempunit->m_cZone-1].zoneshortname ) );
			strcat( memberstr, tempstr );	
			
			int imageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );		
			
			m_OrganizationListBox->InsertListItem( _T(""), 0, i+1 );
			m_OrganizationListBox->SetIconArchive(i, &g_MainInterfaceTextureArchive);
			m_OrganizationListBox->SetItemAttrib(i, 0, imageindex, rect);
			m_OrganizationListBox->SetItemText(i, 1, memberstr);
			m_OrganizationListBox->SetItemAttrib(i, 1, colorvalue);
			m_OrganizationListBox->SetItemAttribHighlight( i, 1, colorvalue );
		}		
	}	
	_XVScrollBar* pScrollBar = m_OrganizationListBox->GetScrollBarObject();
	if( pScrollBar )
	{
		pScrollBar->SetTotalLineCount(count);
	}
	UpdateTextInfo();
}

void _XWindow_Group::CopyMemberOutTimeList( int count, _OutTime* unitouttimelist )
{
	if( !m_OrganizationMemberOutTimeList.empty() )
	{
		list<_OutTime*>::iterator iter2 = m_OrganizationMemberOutTimeList.begin();
		while ( iter2 != m_OrganizationMemberOutTimeList.end() ) 
		{
			_OutTime* tempData = *iter2;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter2 = m_OrganizationMemberOutTimeList.erase( iter2 );
			}
			else
			{
				iter2++;
			}
		}
	}

	for( int i = 0; i < count; i++ )
	{
		_OutTime* temptime = new _OutTime;
		temptime->reset();
		*temptime = *(unitouttimelist+i);
		m_OrganizationMemberOutTimeList.push_back( temptime );
	}
}

void _XWindow_Group::UpdateTextInfo()
{
	sprintf( m_GroupInfoText[0], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), m_OrganizationBaseInfo.m_cName );	// "%s단"
	sprintf( m_GroupInfoText[1], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO3), m_OrganizationBaseInfo.m_usCurMemberNum );	// "(단원:%d명)"
	
	int currentcount = 0;
	list<_Unit*>::iterator unit = m_OrganizationMemberList.begin();
	for( ; unit != m_OrganizationMemberList.end(); unit++ )
	{
		_Unit* tempunit = *unit;
		if( tempunit )
		{
			if( tempunit->m_bOnline )
				currentcount++;
		}
	}

#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕 //breif : 영웅단 리뉴얼
	sprintf( m_GroupInfoText[2], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO4), currentcount );	// " / 현재 %d명 접속중"
	sprintf( m_GroupInfoText[3], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO5), m_OrganizationBaseInfo.m_cCaptainName );	// "단주 : %s"
	sprintf( m_GroupInfoText[4], "영웅 단 성취단계" );	// "영웅단 단계"
	sprintf( m_GroupInfoText[6], "기여도 성취단계" );	// "문파기여도 가산 : %d%%" //m_OrganizationBaseInfo.m_uiContribution 
	sprintf( m_GroupInfoText[7], "영웅 단 기여도");
	sprintf( m_GroupInfoText[8], "추가 혜택 남은 시간");

#else
	sprintf( m_GroupInfoText[2], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO4), currentcount );	// " / 현재 %d명 접속중"
	sprintf( m_GroupInfoText[3], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO5), m_OrganizationBaseInfo.m_cCaptainName );	// "단주 : %s"
	strcpy( m_GroupInfoText[4], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_GROUPLEVEL) );	// "영웅단 단계"
	sprintf( m_GroupInfoText[6], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO7), m_OrganizationBaseInfo.m_uiContribution );	// "문파기여도 가산 : %d%%"
#endif


	
	if( m_SelectedTab == _XGROUPTAB_GROUPCLASS1)			//영웅단
	{
		m_pCotributionBorder->SetGaugeFactor( (FLOAT)m_OrganizationBaseInfo.m_uiExperience );
	}
	//SelectNoticeMode(_XNOTICEMODE_READ);
}

void _XWindow_Group::UpdateOrganizationListBox()
{
	int i = 0;
	m_ManagerCount = 0;
	m_OrganizationListBox->DeleteAllItem();
	
	list<_Unit*>::iterator findunit = m_OrganizationMemberList.begin();
	for( ; findunit != m_OrganizationMemberList.end(); findunit++ )
	{
		_Unit* tempunit = *findunit;
		if( tempunit )
		{			
			TCHAR memberstr[256];
			memset(memberstr, 0, 256);
			TCHAR tempstr[64];
			memset(tempstr, 0, 64);
			DWORD colorvalue = 0xFFFFFFFF;
			RECT rect = {0, 0, 0, 0};
			
			if( tempunit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_ )
			{
				strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_DEFAULTUNIT) );	// "단원"
				colorvalue = 0xFFD2D2D2;
			}
			else if( tempunit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_ )
			{
				strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_SPECIALUNIT) );	// "단장"
				colorvalue = 0xFFD2D2D2;
				rect.left = 111;
				rect.top = 74;
				rect.right = 127;
				rect.bottom = 90;

				if( m_ManagerCount < 9 )
					m_ManagerCount++;
			}
			else if( tempunit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
			{
				strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_CAPTAINUNIT) );	// "단주"
				colorvalue = 0xFFD5D49A;
				rect.left = 94;
				rect.top = 74;
				rect.right = 110;
				rect.bottom = 90;
			}
			if( !tempunit->m_bOnline )
				colorvalue = 0xFF555555;
			
			sprintf( tempstr, " : %s", tempunit->m_cName );
			strcat( memberstr, tempstr );
			
			int level = g_UserLevelTable[tempunit->m_sInnerLevel].level;
			int sublevel = g_UserLevelTable[tempunit->m_sInnerLevel].sublevel;			
			
			memset(tempstr, 0, 64);
			strcpy( tempstr, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ORGANIZATIONINFO1, 
																g_LevelName[level], &sublevel, g_ZoneInfoTable[tempunit->m_cZone-1].zoneshortname ) );
			strcat( memberstr, tempstr );	

			int imageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
			
			m_OrganizationListBox->InsertListItem( _T(""), 0, i+1 );
			m_OrganizationListBox->SetIconArchive(i, &g_MainInterfaceTextureArchive);
			m_OrganizationListBox->SetItemAttrib(i, 0, imageindex, rect);
			m_OrganizationListBox->SetItemText(i, 1, memberstr);
			m_OrganizationListBox->SetItemAttrib(i, 1, colorvalue);
			m_OrganizationListBox->SetItemAttribHighlight( i, 1, colorvalue );
		}
		i++;
	}

}

void _XWindow_Group::JoinOrganizationMember(_Unit* unit, _OutTime* outtime)
{
	_Unit* tempUnit = new _Unit;
	tempUnit->reset();
	*tempUnit = *unit;
	m_OrganizationMemberList.push_back(tempUnit);

	_OutTime* tempOutTime = new _OutTime;
	tempOutTime->reset();
	*tempOutTime = *outtime;
	m_OrganizationMemberOutTimeList.push_back(tempOutTime);

	m_OrganizationBaseInfo.m_usCurMemberNum++;
	UpdateOrganizationListBox();
	UpdateTextInfo();
}

void _XWindow_Group::DeleteOrganizationMember(_Unit* unit, _OutTime* outtime)
{
	int i = 0;
	list<_Unit*>::iterator findunit = m_OrganizationMemberList.begin();
	for( ; findunit != m_OrganizationMemberList.end(); findunit++ )
	{
		_Unit* tempunit = *findunit;
		if( tempunit )
		{
			if( strcmp( tempunit->m_cName , unit->m_cName ) == 0 )
			{
				if( strcmp(unit->m_cName, g_pLocalUser->m_CharacterInfo.charactername) == 0 )	// 자기 자신
				{
					ResetData();

					TCHAR message[256];
					memset( message, 0, 256 );
					sprintf( message, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2091), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );	
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE);

					g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.Reset();
                    ResetAlienceInfo();
					ResetData();

					m_pCotributionBorder->ShowWindow(FALSE);
					m_pContributionGaugeLeftBorder->ShowWindow(FALSE);
					m_pContributionGaugeRightBorder->ShowWindow(FALSE);
					strcpy( m_GroupInfoText[5], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2090) );
				}
				else		// 분파의 다른 사람
				{
					TCHAR message[256];
					memset( message, 0, 256 );
					
					// "%s님이 %s 영웅단에서 탈퇴하셨습니다."
					strcpy( message, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ORGANIZATIONDROP1, 
																	   unit->m_cName, g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName ) );

					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE);

					findunit = m_OrganizationMemberList.erase(findunit);
					list<_OutTime*>::iterator findouttimeunit = m_OrganizationMemberOutTimeList.begin();

					if( i < m_OrganizationMemberOutTimeList.size() )
						advance(findouttimeunit, i);

					_OutTime* tempOuttimeUnit = *findouttimeunit;
					if( tempOuttimeUnit )
					{
						findouttimeunit = m_OrganizationMemberOutTimeList.erase(findouttimeunit);
					}
					m_OrganizationBaseInfo.m_usCurMemberNum--;
					UpdateOrganizationListBox();
					UpdateTextInfo();
				}				
				break;
			}
		}
		i++;
	}	
}

void _XWindow_Group::DeleteCompulsionOrganizationMember(_Unit* unit, _OutTime* outtime)
{
	int i = 0;
	list<_Unit*>::iterator findunit = m_OrganizationMemberList.begin();
	for( ; findunit != m_OrganizationMemberList.end(); findunit++ )
	{
		_Unit* tempunit = *findunit;
		if( tempunit )
		{
			if( strcmp( tempunit->m_cName , unit->m_cName ) == 0 )
			{
				if( strcmp(unit->m_cName, g_pLocalUser->m_CharacterInfo.charactername) == 0 )	// 자기 자신
				{
					ResetData();

					TCHAR message[256];
					memset( message, 0, 256 );
					sprintf( message, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2092), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );	 
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE);

					g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.Reset();
					ResetData();

					m_pCotributionBorder->ShowWindow(FALSE);
					m_pContributionGaugeLeftBorder->ShowWindow(FALSE);
					m_pContributionGaugeRightBorder->ShowWindow(FALSE);
					
					strcpy( m_GroupInfoText[5], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2090) );
				}
				else		// 분파의 다른 사람
				{
					TCHAR message[256];
					memset( message, 0, 256 );

					LPTSTR param1 = unit->m_cName;
					LPTSTR param2 = g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName;
					strcpy( message, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_ORGANIZATION_2093, param1, param2 ) );
					
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE);

					findunit = m_OrganizationMemberList.erase(findunit);
					list<_OutTime*>::iterator findouttimeunit = m_OrganizationMemberOutTimeList.begin();

					if( i < m_OrganizationMemberOutTimeList.size() )
						advance(findouttimeunit, i);

					_OutTime* tempOuttimeUnit = *findouttimeunit;
					if( tempOuttimeUnit )
					{
						findouttimeunit = m_OrganizationMemberOutTimeList.erase(findouttimeunit);
					}
					m_OrganizationBaseInfo.m_usCurMemberNum--;
					UpdateOrganizationListBox();
					UpdateTextInfo();
				}				
				break;
			}
		}
		i++;
	}
}

void _XWindow_Group::UpdateOrganizationMember(_Unit* unit, _OutTime* outtime)
{
	int i = 0;
	m_ManagerCount = 0;
	list<_Unit*>::iterator findunit = m_OrganizationMemberList.begin();
	for( ; findunit != m_OrganizationMemberList.end(); findunit++ )
	{
		_Unit* tempunit = *findunit;
		if( tempunit )
		{
			if( strcmp( tempunit->m_cName , unit->m_cName ) == 0 )
			{
				list<_OutTime*>::iterator iterTime = m_OrganizationMemberOutTimeList.begin();
				if( i < m_OrganizationMemberOutTimeList.size() )
					advance( iterTime, i );
				_OutTime* tempOuttime = *iterTime;
				if( tempOuttime )
				{
					*tempOuttime = *outtime;
				}
				if( strcmp( g_pLocalUser->m_CharacterInfo.charactername, unit->m_cName ) == 0 )
				{
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_ &&
						unit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_ )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2094), TRUE);
					}
					else if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_ &&
							 unit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_ )
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2095), TRUE);
					}
					
					g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass = unit->m_cClass;
#ifdef _XTS_ALIANCE_20061018
					if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
					{					
						_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
						if(pRequestParty_Window)
						{
							pRequestParty_Window->m_RequestAlienceButton->EnableWindow(TRUE);
						}
					}
#endif
				}

				*tempunit = *unit;
				TCHAR memberstr[256];
				memset(memberstr, 0, 256);
				TCHAR tempstr[64];
				memset(tempstr, 0, 64);
				DWORD colorvalue = 0xFFFFFFFF;
				RECT rect = {0, 0, 0, 0};
				
				if( tempunit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_ )
				{
					strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_DEFAULTUNIT) );	// "단원"
					colorvalue = 0xFFD2D2D2;
				}
				else if( tempunit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_ )
				{
					strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_SPECIALUNIT) );	// "단장"
					colorvalue = 0xFFD2D2D2;
					rect.left = 111;
					rect.top = 74;
					rect.right = 127;
					rect.bottom = 90;
					if( m_ManagerCount < 9 )
						m_ManagerCount++;
				}
				else if( tempunit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{
					strcpy( memberstr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_CAPTAINUNIT) );	// "단주"
					colorvalue = 0xFFD5D49A;
					rect.left = 94;
					rect.top = 74;
					rect.right = 110;
					rect.bottom = 90;
				}
				if( !tempunit->m_bOnline )
					colorvalue = 0xFF555555;
				
				sprintf( tempstr, " : %s", tempunit->m_cName );
				strcat( memberstr, tempstr );
				
				int level = g_UserLevelTable[tempunit->m_sInnerLevel].level;
				int sublevel = g_UserLevelTable[tempunit->m_sInnerLevel].sublevel;
				memset(tempstr, 0, 64);

				strcpy( tempstr, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_ORGANIZATIONINFO1, 
																   g_LevelName[level], &sublevel, g_ZoneInfoTable[tempunit->m_cZone-1].zoneshortname ) );
				strcat( memberstr, tempstr );

				int imageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );		
				
				m_OrganizationListBox->SetIconArchive(i, &g_MainInterfaceTextureArchive);
				m_OrganizationListBox->SetItemAttrib(i, 0, imageindex, rect);
				m_OrganizationListBox->SetItemText(i, 1, memberstr);
				m_OrganizationListBox->SetItemAttrib(i, 1, colorvalue);
				m_OrganizationListBox->SetItemAttribHighlight( i, 1, colorvalue );
				break;
			}
			i++;
		}
	}
	UpdateTextInfo();
}

void _XWindow_Group::UpdateOrganizationEntrust(_Unit* unit)
{
	// 단주를 찾아서 클래스 변경
	list<_Unit*>::iterator findunit = m_OrganizationMemberList.begin();
	for( ; findunit != m_OrganizationMemberList.end(); findunit++ )
	{
		_Unit* tempUnit = *findunit;
		if( tempUnit )
		{
			if( tempUnit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
			{
				tempUnit->m_cClass = _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_;
				strcpy( m_OrganizationBaseInfo.m_cCaptainName, unit->m_cName );
				break;
			}
		}
	}

	// 단주를 찾아서 클래스 변경
	findunit = m_OrganizationMemberList.begin();
	for( ; findunit != m_OrganizationMemberList.end(); findunit++ )
	{
		_Unit* tempUnit = *findunit;
		if( tempUnit )
		{
			if( strcmp( tempUnit->m_cName, m_OrganizationBaseInfo.m_cCaptainName ) == 0 )
			{
				tempUnit->m_cClass = _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_;
				break;
			}
		}
	}

	if( strcmp( m_OrganizationBaseInfo.m_cCaptainName, g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
	{
		g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass = _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_;
	}
	else
	{
		if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
			g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass = _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_;
	}
	
#ifdef _XTS_ALIANCE_20061018
	if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_)
	{					
		_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
		if(pRequestParty_Window)
		{
			pRequestParty_Window->m_RequestAlienceButton->EnableWindow(TRUE);
		}
	}
#endif

	UpdateOrganizationListBox();
	UpdateTextInfo();	
}

void _XWindow_Group::BreakUpOrganization(void)
{
	ResetData();

	TCHAR message[256];
	memset( message, 0, 256 );
	sprintf( message, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2096), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );
	_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
	pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), message, TRUE);

	g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.Reset();
	
	m_pCotributionBorder->ShowWindow(FALSE);
	m_pContributionGaugeLeftBorder->ShowWindow(FALSE);
	m_pContributionGaugeRightBorder->ShowWindow(FALSE);
	strcpy( m_GroupInfoText[5], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_NODATA) );
}

void _XWindow_Group::ProcessPopUp()
{
	// 초기화 ---=
	for( int i = 0; i < 8; i++ )
	{
		m_PopUpButton[i]->ShowWindow( FALSE );
	}
	m_ShowPopUpLine = FALSE;
	// ----------=

	int selectline = m_OrganizationListBox->GetSelectedItem();
					
	if( selectline < 0 || selectline >= m_OrganizationListBox->GetItemCount() )
		return;

	if( selectline >= m_OrganizationBaseInfo.m_usCurMemberNum )
		return;

	list<_Unit*>::iterator findunit = m_OrganizationMemberList.begin();
	if( selectline < m_OrganizationMemberList.size() )
		advance( findunit, selectline );	

	_Unit* tempUnit = *findunit;
	if( tempUnit )
	{
		m_UpdateUnit = *tempUnit;
		if( strlen(tempUnit->m_cName) )
			strcpy( m_SendName, tempUnit->m_cName );

		if( strcmp( tempUnit->m_cName, g_pLocalUser->m_CharacterInfo.charactername ) != 0 )
		{
			m_ShowPopUpLine = TRUE;

			ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
			m_DrawPosPopUp.x = 392; //scrnpos->x+20;
			m_DrawPosPopUp.y = scrnpos->z;

			if( tempUnit->m_bOnline )
			{
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{

					for( int i = 0; i < 4; i++ )
					{
						m_PopUpButton[i]->ShowWindow( TRUE );
						m_PopUpButton[i]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y+(i*18) );
					}

					if( tempUnit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_ )
					{
						m_PopUpButton[5]->ShowWindow( TRUE );
						m_PopUpButton[5]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y+(4*18) );						
					}
					if( tempUnit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_ )
					{
						m_PopUpButton[4]->ShowWindow( TRUE );
						m_PopUpButton[4]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y+(4*18) );
					}

					m_PopUpButton[6]->ShowWindow( TRUE );
					m_PopUpButton[6]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y+(5*18) );
					m_PopUpButton[7]->ShowWindow( TRUE );
					m_PopUpButton[7]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y+(6*18) );
				
					m_PopUpCount = 7;
				}
				else 
				{
					for( int i = 0; i < 4; i++ )
					{
						m_PopUpButton[i]->ShowWindow( TRUE );
						m_PopUpButton[i]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y+(i*18) );
					}
					m_PopUpCount = 4;
				}
			}
			else
			{
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
				{
					m_PopUpButton[1]->ShowWindow( TRUE );
					m_PopUpButton[1]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y );
					if( tempUnit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_ )
					{
						m_PopUpButton[5]->ShowWindow( TRUE );
						m_PopUpButton[5]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y+18 );
					}
					if( tempUnit->m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_ )
					{
						m_PopUpButton[4]->ShowWindow( TRUE );
						m_PopUpButton[4]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y+18 );
					}
					m_PopUpButton[7]->ShowWindow( TRUE );
					m_PopUpButton[7]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y+(2*18) );
					m_PopUpCount = 3;
				}
				else
				{
					m_PopUpButton[1]->ShowWindow( TRUE );
					m_PopUpButton[1]->MoveWindow( m_DrawPosPopUp.x, m_DrawPosPopUp.y );
					m_PopUpCount = 1;
				}
			}
		}
	}
}

void _XWindow_Group::ClosePopUpMenu()
{
	for( int i = 0; i < 8; i++ )
	{
		m_PopUpButton[i]->ShowWindow( FALSE );
	}
	m_ShowPopUpLine = FALSE;
}

void _XWindow_Group::GrantUnit()
{
	if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
	{
		if( m_ManagerCount < 9 )
		{
			m_UpdateUnit.m_cClass = _XUSERORGANIZATIONINFO::_UNIT_CLASS_SPECIAL_UNIT_;
			
			g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_UNIT_UPDATE_, MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_UPDATE_);
		}
	}	
}

void _XWindow_Group::GrantCancleUnit()
{
	if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
	{
		m_UpdateUnit.m_cClass = _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_;
		g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_UNIT_UPDATE_, MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_UPDATE_);
	}	
}

void _XWindow_Group::TrustUnit()
{
	if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
	{
		if( g_CurrentZoneInfoIndex == m_UpdateUnit.m_cZone )
		{
			// 위임할 사람의 성취단계를 검사함.
			bool checklevel = false;
			list<_Unit*>::iterator findunit = m_OrganizationMemberList.begin();
			for( ; findunit != m_OrganizationMemberList.end(); findunit++ )
			{
				_Unit* tempunit = *findunit;
				if( tempunit )
				{
					if( strcmp( tempunit->m_cName , m_UpdateUnit.m_cName ) == 0 )
					{
						if( tempunit->m_sInnerLevel >= 49) checklevel = true;
						break;
					}
				}
			}
			
			if( checklevel )
			{
#ifdef _XTS_ALIANCE_20061018
				if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
				{
					//같은 영웅단 동맹 소속이다.
					TCHAR message[256];
					memset( message, 0, 256 );
					sprintf( message,_XGETINTERFACETEXT(ID_STRING_NEW_3195) );			//_T("방회에 소속되어 있으면 위임을 할 수 없습니다.") 
					g_NetworkKernel.InsertChatString(message, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);
				}
				else
				{
					strcpy( m_SendName, m_UpdateUnit.m_cName );
					g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_);
				}
#else
				strcpy( m_SendName, m_UpdateUnit.m_cName );
				g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_CLIENT, MSG_ORGANIZATION_CLIENT_::_OR_REQ_ENTRUST_);
#endif
			}
			else
			{
				_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2097), TRUE);
			}

		}		
		else
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2098), TRUE);
		}
	}	
}

void _XWindow_Group::PrugeUnit()	// 추방
{
//#ifdef _XDEF_SHUTDOWN_20061027
//	// 셧다운시에 제약 사항
//	// 영웅단 추방 금지
//	if( g_PlayLeftTime == 0 )
//		return;
//#endif

	if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
	{		
		g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_UNIT_UPDATE_, MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_KICK_);
	}
}

void _XWindow_Group::LeaveOrganization()	// 탈퇴
{
	if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass == _XUSERORGANIZATIONINFO::_UNIT_CALSS_CAPTAIN_UNIT_ )
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ERRORDROP), TRUE);	// _T("단주는 영웅단에서 탈퇴가 불가능합니다.")
		return;
	}

	list<_Unit*>::iterator findunit = m_OrganizationMemberList.begin();
	for( ; findunit != m_OrganizationMemberList.end(); findunit++ )
	{
		_Unit* tempunit = *findunit;
		if( tempunit )
		{
			if( strcmp( tempunit->m_cName , g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
			{
				m_UpdateUnit = *tempunit;
				break;
			}
		}
	}
	if( strlen(m_UpdateUnit.m_cName) )
		g_NetworkKernel.SendPacket(MSG_NO_ORGANIZATION_UNIT_UPDATE_, MSG_ORGANIZATION_UNIT_UPDATE_::_OR_UNIT_DROP_);
}

void _XWindow_Group::LineUpLevel()
{
	m_SortLevelFlag = !m_SortLevelFlag;

	for( int i = 1; i < m_OrganizationMemberList.size(); ++i )
		SortLessLevelUp(i, m_SortLevelFlag);
	UpdateOrganizationListBox();
}

void _XWindow_Group::LineUpZone()
{
	m_SortZoneFlag = !m_SortZoneFlag;
	
	for( int i = 1; i < m_OrganizationMemberList.size(); ++i )
		SortLessZone(i, m_SortZoneFlag);
	UpdateOrganizationListBox();
}

void _XWindow_Group::LineUpOnline()
{
	m_SortOnlineFlag = !m_SortOnlineFlag;

	for( int i = 1; i < m_OrganizationMemberList.size(); ++i )
		SortLessOnline(i, m_SortOnlineFlag);
	UpdateOrganizationListBox();
}

void _XWindow_Group::SortLessLevelUp( int unitindex, BOOL sortflag )
{
	if( unitindex == 0 || unitindex > m_OrganizationMemberList.size()-1 ) return;

	list<_Unit*>::iterator k = m_OrganizationMemberList.begin();
	if( unitindex < m_OrganizationMemberList.size() )
		advance(k, unitindex);

	list<_OutTime*>::iterator kt = m_OrganizationMemberOutTimeList.begin();
	if( unitindex < m_OrganizationMemberOutTimeList.size() )
		advance(kt, unitindex);

	int count = unitindex;
	while (count) 
	{
		_Unit* TempUnit = *k;
		_Unit* TempUnit2 = *(--k);

		_OutTime* TempOuttime = *kt;
		_OutTime* TempOuttime2 = *(--kt);

		if( TempUnit && TempUnit2 )
		{
			if( sortflag )			
			{
				if( TempUnit->m_sInnerLevel < TempUnit2->m_sInnerLevel)
				{
					_Unit tempSave = *TempUnit2;
					*TempUnit2 = *TempUnit;
					*TempUnit = tempSave;

					_OutTime tempSaveOuttime = *TempOuttime2 ;
					*TempOuttime2 = *TempOuttime ;
					*TempOuttime = tempSaveOuttime ;
				}
				else
					return;
			}
			else
			{
				if( TempUnit->m_sInnerLevel > TempUnit2->m_sInnerLevel)
				{
					_Unit tempSave = *TempUnit2;
					*TempUnit2 = *TempUnit;
					*TempUnit = tempSave;

					_OutTime tempSaveOuttime = *TempOuttime2 ;
					*TempOuttime2 = *TempOuttime ;
					*TempOuttime = tempSaveOuttime ;
				}
				else
					return;
			}			
		}
		count--;
	}
}

void _XWindow_Group::SortLessZone( int unitindex, BOOL sortflag )
{
	if( unitindex == 0 || unitindex > m_OrganizationMemberList.size()-1 ) return;

	list<_Unit*>::iterator k = m_OrganizationMemberList.begin();
	if( unitindex < m_OrganizationMemberList.size() )
		advance(k, unitindex);

	list<_OutTime*>::iterator kt = m_OrganizationMemberOutTimeList.begin();
	if( unitindex < m_OrganizationMemberOutTimeList.size() )
		advance(kt, unitindex);

	int count = unitindex;
	while (count) 
	{
		_Unit* TempUnit = *k;
		_Unit* TempUnit2 = *(--k);

		_OutTime* TempOuttime = *kt;
		_OutTime* TempOuttime2 = *(--kt);

		if( TempUnit && TempUnit2 )
		{
			if( sortflag )
			{
				if( TempUnit->m_cZone < TempUnit2->m_cZone )
				{
					_Unit tempSave = *TempUnit2;
					*TempUnit2 = *TempUnit;
					*TempUnit = tempSave;

					_OutTime tempSaveOuttime = *TempOuttime2 ;
					*TempOuttime2 = *TempOuttime ;
					*TempOuttime = tempSaveOuttime ;
				}
				else
					return;
			}
			else
			{
				if( TempUnit->m_cZone > TempUnit2->m_cZone )
				{
					_Unit tempSave = *TempUnit2;
					*TempUnit2 = *TempUnit;
					*TempUnit = tempSave;

					_OutTime tempSaveOuttime = *TempOuttime2 ;
					*TempOuttime2 = *TempOuttime ;
					*TempOuttime = tempSaveOuttime ;
				}
				else
					return;
			}			
		}
		count--;
	}
}

void _XWindow_Group::SortLessOnline( int unitindex, BOOL sortflag )
{
	if( unitindex == 0 || unitindex > m_OrganizationMemberList.size()-1 ) return;

	list<_Unit*>::iterator k = m_OrganizationMemberList.begin();
	if( unitindex < m_OrganizationMemberList.size() )
		advance(k, unitindex);

	list<_OutTime*>::iterator kt = m_OrganizationMemberOutTimeList.begin();
	if( unitindex < m_OrganizationMemberOutTimeList.size() )
		advance(kt, unitindex);

	int count = unitindex;
	while (count) 
	{
		_Unit* TempUnit = *k;
		_Unit* TempUnit2 = *(--k);

		_OutTime* TempOuttime = *kt;
		_OutTime* TempOuttime2 = *(--kt);

		if( TempUnit && TempUnit2 )
		{
			if( sortflag )
			{
				if( TempUnit->m_bOnline < TempUnit2->m_bOnline )
				{
					_Unit tempSave = *TempUnit2;
					*TempUnit2 = *TempUnit;
					*TempUnit = tempSave;

					_OutTime tempSaveOuttime = *TempOuttime2 ;
					*TempOuttime2 = *TempOuttime ;
					*TempOuttime = tempSaveOuttime ;
				}
				else
					return;
			}
			else
			{
				if( TempUnit->m_bOnline > TempUnit2->m_bOnline )
				{
					_Unit tempSave = *TempUnit2;
					*TempUnit2 = *TempUnit;
					*TempUnit = tempSave;

					_OutTime tempSaveOuttime = *TempOuttime2 ;
					*TempOuttime2 = *TempOuttime ;
					*TempOuttime = tempSaveOuttime ;
				}
				else
					return;
			}			
		}
		count--;
	}
}

void _XWindow_Group::ClickSortButton( void )
{
	if( this->GetShowStatus() )
	{
		if( m_SortTypeList )						
		{
			if( m_SortTypeList->GetShowStatus() )
			{				
				m_SortTypeList->ShowWindow( FALSE );
			}
			else		
			{
				m_SortTypeList->ShowWindow( TRUE );
			}
		}
	}
}

void _XWindow_Group::SelectSortType( void )
{
	if( this->GetShowStatus() )
	{
		if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
		{
			if( m_SortTypeList )
			{
				if( m_SortTypeList->GetShowStatus() )
				{
					int selectline = m_SortTypeList->GetSelectedItem();
					
					if( selectline < 0 || selectline >= m_SortTypeList->GetItemCount() )
						return;
					
					if( selectline >= m_OrganizationBaseInfo.m_usCurMemberNum )
						return;
					
					switch( selectline ) 
					{
					case 0:
						{					
							LineUpLevel();
							strcpy( m_CurrentSortName, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_SORTLEVEL) );
						}
						break;
					case 1:
						{
							LineUpZone();
							strcpy( m_CurrentSortName, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_SORTZONE) );
						}
						break;
					case 2:
						{
							LineUpOnline();
							strcpy( m_CurrentSortName, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_SORTONLINE) );
						}
						break;
					default :
						{
							strcpy( m_CurrentSortName, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2099) );
						}
						break;
					}
					m_SortTypeList->ShowWindow( FALSE );
				}
			}
		}		
		else
		{
			m_SortTypeList->ShowWindow( FALSE );
		}
	}
}

void _XWindow_Group::CalculateNoticeTextength()
{
	m_NoticeTextLength = 0;
	for( int i = 0; i < m_NoticeTextTotalCount ; i++)
	{
		m_NoticeTextLength += strlen( m_NoticeTempText[i] );
	}

	m_NoticeTextLength += (m_NoticeTextTotalCount - 1);
}

void _XWindow_Group::SelectNoticeMode( _XNoticeMode mode)
{
	m_NoticeMode = mode;

	switch(m_NoticeMode) 
	{
	case _XNOTICEMODE_READ :
		{
			if( m_NoticeIMEControl.GetShowStatus() )
			{
				_XIMEKERNEL.ResetCombinationBuffer();
				m_NoticeIMEControl.ClearBuffer();
				_XIMEKERNEL.SetFocus(NULL);
			}

			SetNoticeText(m_NoticeText.notice);

			m_pNoticeWrite->EnableWindow(FALSE);
			m_pNoticemodify->EnableWindow(FALSE);

			// 가입한 영웅단이 있을 경우 
			if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
			{
				// 단원이상이면 글쓰기 권한 부여
				if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cClass > _XUSERORGANIZATIONINFO::_UNIT_CLASS_DEFAULT_UNIT_ )
				{
					m_pNoticeWrite->EnableWindow(TRUE);
					
					// 자기가 쓴 글만 수정할 수 있음
					if( strcmp( m_NoticeText.writer, g_pLocalUser->m_CharacterInfo.charactername) == 0 )
					{
						m_pNoticemodify->EnableWindow(TRUE);
					}					
				}
			}

			// 버튼이미지 
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  47, 192, 71, 234 );	
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 20, 192, 44, 234 );			
			m_pNoticeWrite->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  74, 192, 98, 234 );	
			
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  128, 99, 152, 141 );
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 101, 99, 125, 141 );			
			m_pNoticemodify->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  155, 99, 179, 141 );
		}
		break;
	case _XNOTICEMODE_WRITE :
		{
			_XIMEKERNEL.ResetCombinationBuffer();
			m_NoticeIMEControl.ClearBuffer();
			memset(m_NoticeTempText, 0, sizeof(TCHAR)*256*128);
			m_NoticeTextScrollPos	= 0;
			m_NoticeTextTotalCount	= 1;
			m_CurrentNoticeLinePos	= 0;
			m_NoticeTextLength		= 0; 

			m_NoticeIMEControl.ShowWindow(TRUE);
			m_NoticeIMEControl.MoveWindow( m_WindowPosition.x+14, m_WindowPosition.y+111 );
			m_NoticeIMEControl.SetTextColor(_XSC_DEFAULT);
			m_NoticeIMEControl.SetFocus();

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
	case _XNOTICEMODE_MODIFY :
		{
			_XIMEKERNEL.ResetCombinationBuffer();
			m_NoticeIMEControl.ClearBuffer();

			SetNoticeText(m_NoticeText.notice, true);
			m_NoticeIMEControl.AddString(m_NoticeTempText[m_NoticeTextTotalCount-1], strlen(m_NoticeTempText[m_NoticeTextTotalCount-1]) );
			memset(m_NoticeTempText[m_NoticeTextTotalCount-1], 0, sizeof(TCHAR)*128);
			
			int linecount = 0;
			m_CurrentNoticeLinePos = m_NoticeTextTotalCount-1;
			if( m_NoticeTextTotalCount > 4 )
				linecount = 3;
			else
				linecount = m_NoticeTextTotalCount-1;			
			
			m_NoticeIMEControl.ShowWindow(TRUE);			
			m_NoticeIMEControl.MoveWindow( m_WindowPosition.x+14, m_WindowPosition.y+111 +(linecount*17));
			m_NoticeIMEControl.SetTextColor(_XSC_DEFAULT);
			m_NoticeIMEControl.SetFocus();
			
			m_NoticeScrollBar->SetLineScrollPos(9999);

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

BOOL _XWindow_Group::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !CheckMousePosition() )	return FALSE;

	if( m_NoticeScrollBar && !m_OrganizationListBox->CheckMousePosition() )
	{
		if( m_NoticeScrollBar->ProcessWheelMessage( zDelta ) )
		{
			return TRUE;
		}
		
		if( CheckMousePosition() )
		{
			int MoveDelta  = (FLOAT)zDelta / 40;			
			
			if( zDelta > 0 )
			{
				m_NoticeScrollBar->LineUp( abs(MoveDelta) );
			}
			else
			{
				m_NoticeScrollBar->LineDown( abs(MoveDelta) );
			}
			return TRUE;				
		}		
	}	

	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_Group::CopySendNoticeText( void )
{
	if( m_NoticeMode == _XNOTICEMODE_READ )	
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2100), TRUE);
		return;
	}

	memset( m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], 0, sizeof(TCHAR) * 128);
	strcpy( m_NoticeTempText[g_pGroup_Window->m_CurrentNoticeLinePos], g_pGroup_Window->m_NoticeIMEControl.GetText());

	m_SendNoticeTempText.reset();
	
	for( int i = 0; i < m_NoticeTextTotalCount ; i++)
	{
		strcat( m_SendNoticeTempText.notice, m_NoticeTempText[i] );
		if( i != m_NoticeTextTotalCount - 1)
			strcat( m_SendNoticeTempText.notice, "\n" );
	}
	CalculateNoticeTextength();
	if( m_NoticeTextLength == 0 )
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2101), TRUE);
		m_NoticeIMEControl.SetFocus();
		return;
	}

	m_SendNoticeTempText.notice[m_NoticeTextLength] = 0;

	SYSTEMTIME time;
	GetLocalTime( &time );

	int param1 = time.wYear;
	int param2 = time.wMonth;
	int param3 = time.wDay;
	strncpy( m_SendNoticeTempText.writedate, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_ORGANIZATION_2102, &param1, &param2, &param3 ), (sizeof(TCHAR)*14) );

	strncpy( m_SendNoticeTempText.writer, g_pLocalUser->m_CharacterInfo.charactername,(sizeof(TCHAR)*12) );

	for( i = 0; i < 3; i++)
		g_NetworkKernel.SendPacket( MSG_NO_ORGANIZATION_NOTICE_REQ, i );
}

void _XWindow_Group::SetNoticeText(TCHAR* text, bool modifymode)
{
	if( !text ) return;
	if( strlen(text) <= 0 ) return;
	
	_XStringSplitInfo splitinfo;	

	if( !modifymode )	// 서버에서 받은 공지
	{
		memset(m_NoticeViewText, 0, sizeof(TCHAR)*256*128);

		// String splitting....		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->SplitString(text, 300, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
		}
		else
		{
			g_XBaseFont->SplitStringByCR(text, splitinfo);
		}
		m_NoticeViewTextTotalCount = splitinfo.splitcount;
		for( int i = 0; i < splitinfo.splitcount; i++ )
		{			
			strncpy( m_NoticeViewText[i], splitinfo.splitpoint[i], splitinfo.splitlength[i] );	
		}

		sprintf( m_NoticeViewText[i], "                   %s %s", m_NoticeText.writedate, m_NoticeText.writer );	
		m_NoticeViewTextTotalCount++;
	}
	else	// 수정 버튼 클릭시 
	{
		memset(m_NoticeTempText, 0, sizeof(TCHAR)*256*128);

		// String splitting....
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->SplitString(text, 300, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
		}
		else
		{
			g_XBaseFont->SplitStringByCR(text, splitinfo);
		}

		m_NoticeTextTotalCount = splitinfo.splitcount;
		for( int i = 0; i < splitinfo.splitcount; i++ )
		{			
			strncpy( m_NoticeTempText[i], splitinfo.splitpoint[i], splitinfo.splitlength[i] );	
		}
	}
	
	if( m_NoticeScrollBar )
	{
#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
		m_NoticeScrollBar->MoveWindow( m_WindowPosition.x+300, m_WindowPosition.y+138 );
#else		
		m_NoticeScrollBar->MoveWindow( m_WindowPosition.x + 300, m_WindowPosition.y + 104 );
#endif
		if( !modifymode )
			m_NoticeScrollBar->SetTotalLineCount( splitinfo.splitcount+1 );
		else
			m_NoticeScrollBar->SetTotalLineCount( splitinfo.splitcount );
	
		m_NoticeScrollBar->SetLineScrollPos(0);
		m_NoticeScrollBar->UpdateData();		
	}
}


void _XWindow_Group::ChangeTab(_XGroupWindowTab selectedtab)
{
	m_SelectedTab = selectedtab;
	
			
	for( int i = 0; i < _XGROUPTAB_TOTALCOUNT; i++ )
	{			
		m_BaseTabButton[i]->SetCheck( FALSE );
		
	}
	
	// 영웅단 탭 이미지
	m_pCotributionBorder->ShowWindow(FALSE);
	m_pContributionGaugeLeftBorder->ShowWindow(FALSE);
	m_pContributionGaugeRightBorder->ShowWindow(FALSE);
	m_pNoticeWrite->ShowWindow(FALSE);
	m_pNoticemodify->ShowWindow(FALSE);
	m_OrganizationListBox->ShowWindow(FALSE);	
	m_pImageContributionRateLeftBorder->ShowWindow(FALSE);
	m_pImageContributionRateRightBorder->ShowWindow(FALSE);
	m_pImageLineUpLeftBorder->ShowWindow(FALSE);
	m_pImageLineUpRightBorder->ShowWindow(FALSE);
	
	for( i = 0; i < 8; i++ )
		m_PopUpButton[i]->ShowWindow(FALSE);

#ifdef _XTS_ALIANCE_20061018	
	// 방회 탭 이미지
	m_pBtnAlienceDestroy->ShowWindow(FALSE);
	m_pImageAlienceBorder->ShowWindow(FALSE);		// 영웅단 동맹 창 바탕
	m_pImageAlienceName1->ShowWindow(FALSE);		// 영웅단 동맹 창 '방'자
	m_pImageAlienceName2->ShowWindow(FALSE);		// 영웅단 동맹 창 '회'자

	m_pBtnGroupLeave->ShowWindow(FALSE);
	m_pBtnLineUpDropDown->ShowWindow(FALSE);
	
	for( i = 0; i < 5; i++ )
		m_pBtnAlienceName[i]->ShowWindow(FALSE);
#endif	
	
#ifdef _XDEF_CASTLEBATTLE
	// 장원전 관련 이미지
	m_pBtnCastleBattleTimeChange->ShowWindow(FALSE);		// 장원전 시간 변경 버튼
	m_pBtnCastleBattleGetMoney->ShowWindow(FALSE);			// 장원전 보유금 획득 버튼
	m_lstboxChallengers->ShowWindow(FALSE);
#endif
	
#ifdef _XDEF_CASTLEBATTLE_RECORD
	m_lstboxOrgRecord->ShowWindow(FALSE);
#endif
	
#ifdef _XDEF_ALIANCE_WEBBOARD_20070130	
	// 영웅회 게시판 바로가기 버튼
	if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_iIndex > 0 )
		m_pBtnGoToAliance->EnableWindow(TRUE);			
	else
		m_pBtnGoToAliance->EnableWindow(FALSE);			

	// 영웅단 게시판 바로가기 버튼
	if( g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex > 0 )
		m_pBtnGoToOrganization->EnableWindow(TRUE);		
	else
		m_pBtnGoToOrganization->EnableWindow(FALSE);		

#endif

#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
		m_pBtnGoToOrganization->ShowWindow(FALSE);
		m_pBtnGoToAliance->ShowWindow(FALSE);
#endif
	
	if( m_SelectedTab == _XGROUPTAB_GROUPCLASS1)			//영웅단
	{
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS1]->SetCheck(TRUE);
	
//		pLeaveButton->ShowWindow(TRUE);
		m_pCotributionBorder->ShowWindow(TRUE);
		m_pContributionGaugeLeftBorder->ShowWindow(TRUE);
		m_pContributionGaugeRightBorder->ShowWindow(TRUE);
		m_pNoticeWrite->ShowWindow(TRUE);
		m_pNoticemodify->ShowWindow(TRUE);
		m_OrganizationListBox->ShowWindow(TRUE);
		m_pImageContributionRateLeftBorder->ShowWindow(TRUE);
		m_pImageContributionRateRightBorder->ShowWindow(TRUE);
		m_pImageLineUpLeftBorder->ShowWindow(TRUE);
		m_pImageLineUpRightBorder->ShowWindow(TRUE);
		m_pBtnGroupLeave->ShowWindow(TRUE);
		m_pBtnLineUpDropDown->ShowWindow(TRUE);

#ifdef _XDEF_GUILD_RENEWAL_070823_MAGVIPER //Author : 양희왕
		m_pBtnGoToOrganization->ShowWindow(TRUE);
		m_pBtnGoToAliance->ShowWindow(TRUE);
#endif

	}
#ifdef _XTS_ALIANCE_20061018
	else if( m_SelectedTab == _XGROUPTAB_GROUPCLASS2)		//방회
	{		
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS2]->SetCheck(TRUE);
		
		m_pImageAlienceBorder->ShowWindow(TRUE);		// 영웅단 동맹 창 바탕
		m_pImageAlienceName1->ShowWindow(TRUE);			// 영웅단 동맹 창 '방'자
		m_pImageAlienceName2->ShowWindow(TRUE);			// 영웅단 동맹 창 '회'자		
		SetAlienceButton();
	}
#endif	
#ifdef _XDEF_CASTLEBATTLE
	else if( m_SelectedTab == _XGROUPTAB_GROUPCLASS3)		//장원
	{
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS3]->SetCheck(TRUE);
		
		m_pBtnCastleBattleTimeChange->ShowWindow(TRUE);		// 장원전 시간 변경 버튼
		m_pBtnCastleBattleGetMoney->ShowWindow(TRUE);		// 장원전 보유금 획득 버튼
		m_lstboxChallengers->ShowWindow(TRUE);

		m_bSetCastleTooltip = FALSE;

		// 정보 요청
		if(g_pLocalUser->m_nCastleMapId > 0)
		{
			m_pMyCastleInfo = g_CBManager.GetCastleInfo(g_pLocalUser->m_nCastleMapId);
			if(m_pMyCastleInfo)
			{
				SetCastleData();
				SetChallengerListItem();
			}
			else
			{
				g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_owner_req_schedule_list);
			}

	#ifdef _XDEF_CASTLEBATTLE_SCHEDULE_RENEWAL_US_KUKURI
			m_pBtnCastleBattleTimeChange->EnableWindow(FALSE);
	#else
			m_pBtnCastleBattleTimeChange->EnableWindow(TRUE);
	#endif

#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
			m_pBtnCastleBattleGetMoney->EnableWindow(TRUE);
#endif
		}
		else
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3371), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE);	//"현재 소유하고 있는 장원이 없습니다."
			m_pBtnCastleBattleTimeChange->EnableWindow(FALSE);
			m_pBtnCastleBattleGetMoney->EnableWindow(FALSE);

			ResetCastleData();
		}
	}
	else if( m_SelectedTab == _XGROUPTAB_GROUPCLASS4)		//장원전황
	{		
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS4]->SetCheck(TRUE);
#ifdef _XDEF_CASTLEBATTLE_RECORD
		m_lstboxOrgRecord->ShowWindow(TRUE);

		// 전황 정보 요청
		ResetRecordData();
		g_NetworkKernel.SendPacket(MSG_NO_COMBAT, en_cb_record_pkt_info_req, g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_iIndex);
#endif
	}
#endif

#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제
	else if( m_SelectedTab == _XGROUPTAB_GROUPCLASS5)		//사제
	{
		m_BaseTabButton[_XGROUPTAB_GROUPCLASS5]->SetCheck(TRUE);
		m_pMnDWindow->ShowMnDWindow(TRUE);
	}

	if( m_SelectedTab != _XGROUPTAB_GROUPCLASS5 )
		m_pMnDWindow->ShowMnDWindow(FALSE);
#endif
}

void _XWindow_Group::RefreshData(void)
{
#ifdef _XDEF_CASTLEBATTLE_SECONDEDITION
	if(m_SelectedTab == _XGROUPTAB_GROUPCLASS3 || m_SelectedTab == _XGROUPTAB_GROUPCLASS4)
		m_SelectedTab = _XGROUPTAB_GROUPCLASS1;
#endif

	ChangeTab(m_SelectedTab);
	if( m_SelectedTab == _XGROUPTAB_GROUPCLASS1 )
	{
		m_OrganizationListBox->ShowWindow(TRUE);
		if( strlen(m_OrganizationBaseInfo.m_cName) )
		{
			m_pCotributionBorder->ShowWindow(TRUE);
			m_pContributionGaugeLeftBorder->ShowWindow(TRUE);
			m_pContributionGaugeRightBorder->ShowWindow(TRUE);
			memset( m_GroupInfoText[5], 0, sizeof(TCHAR)*32 ); 
		}
		else
		{
			m_pCotributionBorder->ShowWindow(FALSE);
			m_pContributionGaugeLeftBorder->ShowWindow(FALSE);
			m_pContributionGaugeRightBorder->ShowWindow(FALSE);
			strcpy( m_GroupInfoText[5], _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_2090) );
		}
	}
	else
	{
		m_OrganizationListBox->ShowWindow(FALSE);
		m_ShowPopUpLine = FALSE;
		for( int i = 0; i < 8; i++ )
		{
			m_PopUpButton[i]->ShowWindow( FALSE );
		}
		
		if( m_SortTypeList->GetShowStatus() )
		m_SortTypeList->ShowWindow( FALSE );
	}	

#ifdef _XDEF_CASTLEBATTLE	
	if( m_SelectedTab == _XGROUPTAB_GROUPCLASS3)		//장원
	{
		m_bSetCastleTooltip = FALSE;
		
		if(g_pLocalUser->m_nCastleMapId > 0)
		{
			m_pMyCastleInfo = g_CBManager.GetCastleInfo(g_pLocalUser->m_nCastleMapId);
			if(m_pMyCastleInfo)
			{
				SetCastleData();
				SetChallengerListItem();
			}
			else
			{
				// 정보 요청
				//#ifdef _XDEF_CASTLEBATTLE_SENDPACKET
				g_NetworkKernel.SendPacket(MSG_NO_CB_SCHEDULE, en_cb_owner_req_schedule_list);
				//#endif
			}
		}
	}
#endif
#ifdef _XDEF_CASTLEBATTLE_RECORD
	if(m_SelectedTab == _XGROUPTAB_GROUPCLASS4)		// 장원 전황
	{
//		g_NetworkKernel.SendPacket(MSG_NO_COMBAT, en_cb_record_pkt_info_req);
	}
#endif
}

#ifdef _XTS_ALIANCE_20061018
void _XWindow_Group::SetAlienceButton(void)
{	
	int count = 0;

	//Author : 양희왕 //breif : 밖으로 뺌 //Date:08/03/06
	for( int i = 0; i < 5; i++ )
		m_pBtnAlienceName[i]->ShowWindow(FALSE);
		

	if( !m_listAlienceMember.empty() )
	{
		__ally_client_info* aliencemember = NULL;
		list <__ally_client_info*>::iterator iter_alience;
		for(iter_alience = m_listAlienceMember.begin() ; iter_alience != m_listAlienceMember.end() ; ++iter_alience)
		{
			aliencemember = *iter_alience;
			if( aliencemember )
			{
				m_pBtnAlienceName[count]->SetButtonText( aliencemember->iOrName );
				m_pBtnAlienceName[count]->ShowWindow(TRUE);
				++count;
			}
		}
	}

	if( g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_CAPTAIN_UNIT_ )
	{
		m_pBtnAlienceDestroy->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3196 );//방회 해산
		m_pBtnAlienceDestroy->ShowWindow(TRUE);
	}
	else if(g_pLocalUser->m_CharacterInfo.m_AlienceInfo.m_cClass == _XUSERALIENCEINFO::_UNIT_CALSS_ALIENCE_SUBCAPTAIN_UNIT_ )
	{
		m_pBtnAlienceDestroy->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3130 );//동맹 탈퇴
		m_pBtnAlienceDestroy->ShowWindow(TRUE);
	}
	else
	{
		m_pBtnAlienceDestroy->ShowWindow(FALSE);
	}
}
void _XWindow_Group::ResetAlienceInfo(void)
{	
	//정보를 받기 전에 영웅단 정보를 지운다.
	//나중에 AlienceReset()이라는 함수를 만든다.
	__ally_client_info* aliencemember = NULL;
	list <__ally_client_info*>::iterator iter_alience;
	for(iter_alience = m_listAlienceMember.begin() ; iter_alience != m_listAlienceMember.end() ; )
	{
		aliencemember = *iter_alience;
		if( aliencemember )
		{
			SAFE_DELETE(aliencemember);
			iter_alience = m_listAlienceMember.erase(iter_alience);				
		}
		else
		{
			++iter_alience;
		}
	}

	//Author : 양희왕 //breif : clear 도 해주자 //Date : 08/03/06
	m_listAlienceMember.clear();

	m_UniqueIndexAlience = 0;
	memset( m_AlienceMasterName, 0, sizeof(TCHAR)*13 );
	g_pLocalUser->m_CharacterInfo.m_AlienceInfo.Reset();
}
#endif	

void _XWindow_Group::SetChallengerListItem(void)
{
#ifdef _XDEF_CASTLEBATTLE
	if(g_pLocalUser->m_nCastleMapId > 0)
	{
		if(!m_pMyCastleInfo)
			m_pMyCastleInfo = g_CBManager.GetCastleInfo(g_pLocalUser->m_nCastleMapId);
	}

	if(m_pMyCastleInfo)
	{
		_XWindow_CastleInfo* pCastleInfo_Window = (_XWindow_CastleInfo*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CASTLEINFO);

		m_lstboxChallengers->DeleteAllItem();
		m_lstboxChallengers->SetScrollPos(0);

		int iconresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_party.tga");

		int count = 0;
		TCHAR messagestr[256];
		TCHAR membercntstr[32];

		vector<_XCastle_OrgInfo*>::iterator iter_org;
		for(iter_org = m_pMyCastleInfo->m_vecChallengers.begin() ; iter_org != m_pMyCastleInfo->m_vecChallengers.end() ; ++iter_org)
		{
			_XCastle_OrgInfo* currentorg = *iter_org;
			if(currentorg)
			{
				RECT rect = {0, 0, 0, 0};
				if(pCastleInfo_Window)
				{
					pCastleInfo_Window->GetIconRect(currentorg->nGroupOfMaster, rect);
				}

				m_lstboxChallengers->InsertListItem(_T(""), 0, count+1);

				// group icon
				m_lstboxChallengers->SetIconArchive(count, &g_MainInterfaceTextureArchive);
				m_lstboxChallengers->SetItemAttrib(count, 0, iconresourceindex, rect);

				// info
				memset(messagestr, 0, sizeof(messagestr));
				memset(membercntstr, 0, sizeof(membercntstr));

				if(currentorg->nType == 0)
				{
					// 단주
					sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), currentorg->szOrgName);	//%s단
					strcat(messagestr, _T("("));
					strcat(messagestr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_CAPTAINUNIT));
				}
				else
				{
					// 회주
					sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3125), currentorg->szOrgName);	//%s회
					strcat(messagestr, _T("("));
					strcat(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3126));
				}

				strcat(messagestr, _T(":"));

				strcat(messagestr, currentorg->szMasterName);

//				strcat(messagestr, _T("/"));

				sprintf(membercntstr, _T(")"));
//				sprintf(membercntstr, "%d명)", currentorg->nMemberCnt);

				strcat(messagestr, membercntstr);

				m_lstboxChallengers->SetItemText(count, 1, messagestr);
				if(currentorg->nType == 0)
				{
					// 단주
					m_lstboxChallengers->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
					m_lstboxChallengers->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				else
				{
					// 회주
					m_lstboxChallengers->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 117, 199, 98));
					m_lstboxChallengers->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 117, 199, 98));
				}
				count++;
			}
		}
	}
	else
	{
		m_lstboxChallengers->DeleteAllItem();
		m_lstboxChallengers->SetScrollPos(0);
	}
#endif	
}

void _XWindow_Group::SetCastleData(void)
{
#ifdef _XDEF_CASTLEBATTLE
	if(g_pLocalUser->m_nCastleMapId > 0)
	{
		if(!m_pMyCastleInfo)
			m_pMyCastleInfo = g_CBManager.GetCastleInfo(g_pLocalUser->m_nCastleMapId);
	}

	if(m_pMyCastleInfo)
	{	
		// 도전 일정
		for(int i = 0 ; i < 3 ; ++i)
		{
			int month = m_pMyCastleInfo->m_tWarSchedule[i].tDate.dwMonth;
			int day = m_pMyCastleInfo->m_tWarSchedule[i].tDate.dwDay;
			int hour = m_pMyCastleInfo->m_tWarSchedule[i].tDate.dwHour;

			sprintf(m_strDate[i], g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NEW_3220, 
				&month, &day, &hour));
		}

		int year = m_pMyCastleInfo->m_tPropertyDate.dwYear+2000;
		int month = m_pMyCastleInfo->m_tPropertyDate.dwMonth;
		int day = m_pMyCastleInfo->m_tPropertyDate.dwDay;

		sprintf(m_strPropertyDate, g_StringDataBase.CompositeString(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ORGANIZATION_2102,
			&year, &month, &day));
	}
	else
	{
		memset(m_strDate, 0, sizeof(m_strDate));
		memset(m_strPropertyDate, 0, sizeof(m_strPropertyDate));
	}
#endif
}

void _XWindow_Group::ShowCastleTooltip()
{
#ifdef _XDEF_CASTLEBATTLE
	if(!m_bSetCastleTooltip)
	{
		int step = 1;

		if(m_pMyCastleInfo)
		{
			step = m_pMyCastleInfo->m_nStep;

			// (mode, 기본 세율, 추가 세율, 단계)
			if(g_pMiniTooltip_Window)
			{
				g_pMiniTooltip_Window->SetTooltipMode(MTMODE_VIEWGROUP_CASTLEINFO, 0, g_pLocalUser->m_nCastleMapId, step);
				g_pMiniTooltip_Window->ShowWindow(TRUE);
			}
			m_bSetCastleTooltip = TRUE;
		}
	}
	else
	{
		int step = 1;

		if(m_pMyCastleInfo)
		{
			step = m_pMyCastleInfo->m_nStep;

			if(g_pMiniTooltip_Window)
			{
				if(step != g_pMiniTooltip_Window->GetItemId())
				{
					g_pMiniTooltip_Window->SetTooltipMode(MTMODE_VIEWGROUP_CASTLEINFO, 0, g_pLocalUser->m_nCastleMapId, step);

					if(!g_pMiniTooltip_Window->GetShowStatus())
						g_pMiniTooltip_Window->ShowWindow(TRUE);
				}
			}
		}
	}

#endif
}

void _XWindow_Group::HideCastleTooltip()
{
#ifdef _XDEF_CASTLEBATTLE

	if(g_pMiniTooltip_Window)
		g_pMiniTooltip_Window->ShowWindow(FALSE);

	m_bSetCastleTooltip = FALSE;

#endif
}

void _XWindow_Group::ResetCastleData(void)
{
#ifdef _XDEF_CASTLEBATTLE
	m_pMyCastleInfo = NULL;
	memset(m_strDate, 0, sizeof(m_strDate));
	memset(m_strPropertyDate, 0, sizeof(m_strPropertyDate));

	m_lstboxChallengers->DeleteAllItem();
	m_lstboxChallengers->SetScrollPos(0);
#endif	
}

void _XWindow_Group::DrawOwnerInfo(void)
{
#ifdef _XDEF_CASTLEBATTLE	
	g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	g_XBaseFont->SetBoldMode(TRUE);
	g_XBaseFont->PutsAlign(m_WindowPosition.x+174, m_WindowPosition.y+37+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3199) );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+174, m_WindowPosition.y+37+98+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3203) );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+174, m_WindowPosition.y+37+98+92+5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3207) );
	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode(FALSE);
	g_XBaseFont->Flush();
	
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 202, 202, 202));
	g_XBaseFont->Puts(m_WindowPosition.x+24, m_WindowPosition.y+64, _XGETINTERFACETEXT(ID_STRING_NEW_3201));	// 소유일수
	g_XBaseFont->Puts(m_WindowPosition.x+24, m_WindowPosition.y+79, _XGETINTERFACETEXT(ID_STRING_NEW_3202));	// 장원단계
	if(m_pMyCastleInfo)
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+307, m_WindowPosition.y+64, 1.0f, _XFONT_ALIGNTYPE_RIGHT, m_strPropertyDate);
		g_XBaseFont->PrintAlign(m_WindowPosition.x+307, m_WindowPosition.y+79, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_3206), m_pMyCastleInfo->m_nStep+1);			// %d 단계
	}
	g_XBaseFont->Flush();

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 136, 141, 226));
	g_XBaseFont->Puts(m_WindowPosition.x+24, m_WindowPosition.y+94, _XGETINTERFACETEXT(ID_STRING_NEW_3264));		//총 세금 누적 금액
	if(m_pMyCastleInfo)
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+307, m_WindowPosition.y+94, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_MONEY), m_pMyCastleInfo->m_nTax);	//%d금
	}
	g_XBaseFont->Flush();

	g_XBaseFont->SetBoldMode(TRUE);
	g_XBaseFont->Puts(m_WindowPosition.x+24, m_WindowPosition.y+110, _XGETINTERFACETEXT(ID_STRING_NEW_3265));	//현 장원 창고 보유 금액
	if(m_pMyCastleInfo)
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+307, m_WindowPosition.y+110, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_MONEY), m_pMyCastleInfo->m_dwMoney);	//%d금
	}
	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode(FALSE);

#if (defined(_XDEF_CASTLEBATTLE_SCHEDULE_RENEWAL_US_KUKURI) || defined (_XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI))

		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 200, 198, 45));
		m_pScheduleIcon[0]->Draw(m_WindowPosition.x+18, m_WindowPosition.y+160);
		TCHAR tempString[512];
		memset( tempString, 0, sizeof(tempString));
		sprintf(tempString, _T("%s  :  %s"), _XGETINTERFACETEXT(ID_STRING_NEW_3775), m_strDate[0]);//장원전
		g_XBaseFont->Puts(m_WindowPosition.x+41, m_WindowPosition.y+164, tempString);
//		g_XBaseFont->Print(m_WindowPosition.x+41, m_WindowPosition.y+164, 1.0f, tempString, 1); //%d 차 장원전

#else

	for(int i = 0 ; i < 3 ; ++i)
	{
		if(m_pMyCastleInfo)
		{
			if(i >= (m_pMyCastleInfo->m_nCurMatchCount))
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 200, 198, 45));
				m_pScheduleIcon[0]->Draw(m_WindowPosition.x+18, m_WindowPosition.y+160+(i*21));
			}
			else
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 86, 86, 86));
				m_pScheduleIcon[1]->Draw(m_WindowPosition.x+18, m_WindowPosition.y+160+(i*21));
			}
		}
		else
		{
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 86, 86, 86));
		}

		g_XBaseFont->Print(m_WindowPosition.x+41, m_WindowPosition.y+164+(i*21), 1.0f, _XGETINTERFACETEXT(ID_STRING_NEW_3219), i+1); //%d 차 장원전
		g_XBaseFont->Puts(m_WindowPosition.x+41+63, m_WindowPosition.y+164+(i*21), _T(":"));
		g_XBaseFont->Puts(m_WindowPosition.x+125, m_WindowPosition.y+164+(i*21), m_strDate[i]);
	}

#endif

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if( scrnpos->x > m_WindowPosition.x+231 && scrnpos->x < m_WindowPosition.x+288 &&
		scrnpos->z > m_WindowPosition.y+78 && scrnpos->z < m_WindowPosition.y+89 )
	{
		ShowCastleTooltip();
	}
	else
	{
		HideCastleTooltip();
	}
#endif
}

void _XWindow_Group::DrawCBOrgTotalRecord(void)
{
#ifdef _XDEF_CASTLEBATTLE_RECORD
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 229, 190, 24));
	g_XBaseFont->SetBoldMode(TRUE);
	g_XBaseFont->Puts(m_WindowPosition.x+145, m_WindowPosition.y+49, _XGETINTERFACETEXT(ID_STRING_NEW_3230) ); //_T("장원전황") 
	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode(FALSE);

	g_XBaseFont->PrintAlign(m_WindowPosition.x+175, m_WindowPosition.y+77, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_3646));	// "누적승률"
	g_XBaseFont->Puts(m_WindowPosition.x+68, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_3666));	// "회"
	g_XBaseFont->Puts(m_WindowPosition.x+168, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2655));	// 전
	g_XBaseFont->Puts(m_WindowPosition.x+239, m_WindowPosition.y+99,  _XGETINTERFACETEXT(ID_STRING_NEW_2656));	// 승
	g_XBaseFont->Puts(m_WindowPosition.x+310, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2657));	// 패

	g_XBaseFont->PrintAlign(m_WindowPosition.x+95, m_WindowPosition.y+198, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3649));	// "대전 영웅단"
	g_XBaseFont->PrintAlign(m_WindowPosition.x+218, m_WindowPosition.y+198, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2655));	// 전
	g_XBaseFont->PrintAlign(m_WindowPosition.x+248, m_WindowPosition.y+198, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2656));	// 승
	g_XBaseFont->Flush();

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 254, 0, 9));

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
		g_XBaseFont->PrintAlign(m_WindowPosition.x+83, m_WindowPosition.y+129, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_3635));	// "對"
	else
		g_XBaseFont->Puts(m_WindowPosition.x+117, m_WindowPosition.y+129, _XGETINTERFACETEXT(ID_STRING_NEW_3635));		// "對"
	
	for(int i = 0 ; i < m_lstboxOrgRecord->GetItemCount() ; ++i)
	{
		g_XBaseFont->Puts(m_WindowPosition.x+15, m_WindowPosition.y+218+i*20, _XGETINTERFACETEXT(ID_STRING_NEW_3635));			// "對"
	}
	g_XBaseFont->Flush();


	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 205, 205, 205));

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
		g_XBaseFont->Puts(m_WindowPosition.x+88, m_WindowPosition.y+129, _XGETINTERFACETEXT(ID_STRING_NEW_3233) );//_T("영웅단 대전 기록")	
	else
		g_XBaseFont->Puts(m_WindowPosition.x+135, m_WindowPosition.y+129, _XGETINTERFACETEXT(ID_STRING_NEW_3233) );//_T("영웅단 대전 기록")	

	g_XBaseFont->Puts(m_WindowPosition.x+17, m_WindowPosition.y+153, _XGETINTERFACETEXT(ID_STRING_NEW_3650));			//"장원명 : "
	g_XBaseFont->Puts(m_WindowPosition.x+17, m_WindowPosition.y+171, _XGETINTERFACETEXT(ID_STRING_NEW_3651));		// "장원전 일자 : "

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
		g_XBaseFont->PrintAlign(m_WindowPosition.x+278, m_WindowPosition.y+198, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2657));	// 패
	else if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) //Author : 양희왕 //breif : 미국 버젼..
		g_XBaseFont->Puts(m_WindowPosition.x+263, m_WindowPosition.y+198, _XGETINTERFACETEXT(ID_STRING_NEW_2657));	// 패
	else
		g_XBaseFont->Puts(m_WindowPosition.x+273, m_WindowPosition.y+198, _XGETINTERFACETEXT(ID_STRING_NEW_2657));	// 패

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
		g_XBaseFont->PrintAlign(m_WindowPosition.x+308, m_WindowPosition.y+198, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2658));	// 승률
	else if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) //Author : 양희왕 //breif : 미국 버젼..
		g_XBaseFont->Puts(m_WindowPosition.x+300, m_WindowPosition.y+198, _XGETINTERFACETEXT(ID_STRING_NEW_2658));	// 승률
	else
		g_XBaseFont->Puts(m_WindowPosition.x+296, m_WindowPosition.y+198, _XGETINTERFACETEXT(ID_STRING_NEW_2658));	// 승률

	g_XBaseFont->PrintAlign(m_WindowPosition.x+209, m_WindowPosition.y+77, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d%%", m_nWinRate);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+67, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_nEntryCount);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+166, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_nCombatCount);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+235, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_nWinCount);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+309, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_nLoseCount);

	if(m_nManorIndex > 0)
	{
		if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
			g_XBaseFont->Puts(m_WindowPosition.x+104, m_WindowPosition.y+153, _XGETINTERFACETEXT(g_CBManager.GetCastleNameTextId(m_nManorIndex)));
		else
			g_XBaseFont->Puts(m_WindowPosition.x+71, m_WindowPosition.y+153, _XGETINTERFACETEXT(g_CBManager.GetCastleNameTextId(m_nManorIndex)));
	}

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
		g_XBaseFont->Puts(m_WindowPosition.x+169, m_WindowPosition.y+171, m_strLastWarDate);
	else
		g_XBaseFont->Puts(m_WindowPosition.x+101, m_WindowPosition.y+171, m_strLastWarDate);

	g_XBaseFont->Flush();

	for(i = 0 ; i < 7 ; ++i)
	{
		_XDrawLine2D(m_WindowPosition.x+205, m_WindowPosition.y+233+i*20, m_WindowPosition.x+324, m_WindowPosition.y+233+i*20, D3DCOLOR_ARGB(255, 116, 116, 116), D3DCOLOR_ARGB(255, 116, 116, 116));
	}

#endif
}

void _XWindow_Group::SetRecordListItem(int count, _PerOrgRecord* record)
{
	m_lstboxOrgRecord->DeleteAllItem();
	m_lstboxOrgRecord->SetScrollPos(0);

	TCHAR messagestr[256];
	TCHAR tempstr[256];

	for(int i = 0 ; i < count ; ++i)
	{
		m_lstboxOrgRecord->InsertListItem(_T(""), 0, i+1);

		memset(messagestr, 0, sizeof(messagestr));
		memset(tempstr, 0, sizeof(tempstr));

		sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), record[i].vsOrgName);

		if(record[i].iAllyIndex > 0)
		{
			strcat(messagestr, "(");
			sprintf(tempstr, _XGETINTERFACETEXT(ID_STRING_NEW_3125), record[i].vsAllyName);
			strcat(messagestr, tempstr);
			strcat(messagestr, ")");
		}

		m_lstboxOrgRecord->SetItemText(i, 0, messagestr);
		m_lstboxOrgRecord->SetItemAttrib(i, 0, D3DCOLOR_ARGB(255, 78, 201, 58));
		m_lstboxOrgRecord->SetItemAttribHighlight(i, 0, D3DCOLOR_ARGB(255, 78, 201, 58));
		
		memset(messagestr, 0, sizeof(messagestr));
		sprintf(messagestr, "%d", record[i].usWinCount+record[i].usLoseCount);
		m_lstboxOrgRecord->SetItemText(i, 1, messagestr);
		m_lstboxOrgRecord->SetItemAttrib(i, 1, D3DCOLOR_ARGB(255, 205, 205, 205));
		m_lstboxOrgRecord->SetItemAttribHighlight(i, 1, D3DCOLOR_ARGB(255, 205, 205, 205));

		memset(messagestr, 0, sizeof(messagestr));
		sprintf(messagestr, "%d", record[i].usWinCount);
		m_lstboxOrgRecord->SetItemText(i, 2, messagestr);
		m_lstboxOrgRecord->SetItemAttrib(i, 2, D3DCOLOR_ARGB(255, 205, 205, 205));
		m_lstboxOrgRecord->SetItemAttribHighlight(i, 2, D3DCOLOR_ARGB(255, 205, 205, 205));

		memset(messagestr, 0, sizeof(messagestr));
		sprintf(messagestr, "%d", record[i].usLoseCount);
		m_lstboxOrgRecord->SetItemText(i, 3, messagestr);
		m_lstboxOrgRecord->SetItemAttrib(i, 3, D3DCOLOR_ARGB(255, 205, 205, 205));
		m_lstboxOrgRecord->SetItemAttribHighlight(i, 3, D3DCOLOR_ARGB(255, 205, 205, 205));

		memset(messagestr, 0, sizeof(messagestr));
		sprintf(messagestr, "%d%%", record[i].usPercentageOfWin);
		m_lstboxOrgRecord->SetItemText(i, 4, messagestr);
		m_lstboxOrgRecord->SetItemAttrib(i, 4, D3DCOLOR_ARGB(255, 205, 205, 205));
		m_lstboxOrgRecord->SetItemAttribHighlight(i, 4, D3DCOLOR_ARGB(255, 205, 205, 205));
	}
}

void _XWindow_Group::ResetRecordData()
{
	m_nEntryCount		= 0;

	m_nWinCount			= 0;
	m_nLoseCount		= 0;
	m_nCombatCount		= 0;
	m_nWinRate			= 0;

	m_nManorIndex		= 0;

	m_nYear				= 0;
	m_nMonth			= 0;
	m_nDay				= 0;
	reserved			= 0;
	
	memset(m_strLastWarDate, 0, sizeof(m_strLastWarDate));

	m_lstboxOrgRecord->DeleteAllItem();
	m_lstboxOrgRecord->SetScrollPos(0);
}

#ifdef ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA

BOOL _XWindow_Group::IsMember( LPTSTR charactername )
{
	if( !charactername ) return FALSE;


	if( !m_OrganizationMemberList.empty() )
	{
		list<_Unit*>::iterator iter = m_OrganizationMemberList.begin();
		while( iter != m_OrganizationMemberList.end() )
		{
			_Unit* tempData = *iter;
			if( tempData )
			{
				if( strcmp( tempData->m_cName, charactername ) == 0 )
				{
					return TRUE;
				}				
			}

			++iter;			
		}
	}

	return FALSE;
}

#endif