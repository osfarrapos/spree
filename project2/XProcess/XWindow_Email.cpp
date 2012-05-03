// XWindow_Email.cpp: implementation of the _XWindow_Email class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_Email.h"

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_MainMenu.h"
#include "XWindow_SystemMsgDefault.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_ReceiverEditReturn( _XIMEContainer* pIMEContainer )
{
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);

	if( !pEmail_Window ) return;
	if( pEmail_Window->m_SelectedTab != _XEMAILTAB_SEND) return;
	if( !pEmail_Window->m_ReceiverIMEControl.GetFocus() ) return;
	if( pEmail_Window->m_ReceiverIMEControl.GetLength() == 0 ) return;

	pEmail_Window->m_EmailIMEControl.SetTextColor(_XSC_DEFAULT);

	memset( pEmail_Window->m_EmailReceiver, 0, sizeof(TCHAR)*13 );
	strcpy( pEmail_Window->m_EmailReceiver , pEmail_Window->m_ReceiverIMEControl.GetText());

	if( pEmail_Window->m_EmailReceiver[0] == '@')
	{
		pEmail_Window->m_EmailIMEControl.SetTextColor(0xFFBCEB7F);
	}

	pEmail_Window->m_ReceiverIMEControl.ClearBuffer();
	pEmail_Window->m_EmailIMEControl.SetFocus();
}

void __stdcall _XIMECallback_EmailEditReturn( _XIMEContainer* pIMEContainer )
{
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);

	
	if( !pEmail_Window ) return;
	if( pEmail_Window->m_SelectedTab != _XEMAILTAB_SEND) return;
	if( !pEmail_Window->m_EmailIMEControl.GetFocus() ) return;
	
	pEmail_Window->CalculateEmailLength();
	if( pEmail_Window->m_EmailTextLength + pEmail_Window->m_EmailIMEControl.GetLength_ByMBCSSize() >= _XDEF_MAX_EMAILLENGTH - 2 ) return;

	POINT winpos;				
	winpos = pEmail_Window->GetWindowPos();
	
	memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
	strcpy( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] , pEmail_Window->m_EmailIMEControl.GetText());
	
	int cursorx = pEmail_Window->m_EmailIMEControl.GetCursorX_ByMBCSSize();

	int length = strlen( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] );
	
	if( pEmail_Window->m_EmailTextLineTotalCount-1 == pEmail_Window->m_EmailTextCurrentLinePos )
	{
		if( cursorx == length )
		{
			pEmail_Window->m_EmailTextLineTotalCount++;
			pEmail_Window->m_EmailTextCurrentLinePos++;
			_XIMEKERNEL.ResetCombinationBuffer();
			pEmail_Window->m_EmailIMEControl.ClearBuffer();
		}
		else
		{
			TCHAR leftstring[128];
			TCHAR rightstring[128];
			memset( leftstring, 0, sizeof(TCHAR)*128 );
			memset( rightstring, 0, sizeof(TCHAR)*128 );
			strncpy( leftstring, pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], cursorx );
			strncpy( rightstring, pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] + cursorx, length - cursorx );
			
			memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
			strncpy( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] , leftstring, cursorx);
			
			pEmail_Window->m_EmailTextLineTotalCount++;
			pEmail_Window->m_EmailTextCurrentLinePos++;
			
			_XIMEKERNEL.ResetCombinationBuffer();
			pEmail_Window->m_EmailIMEControl.ClearBuffer();			
			pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
			pEmail_Window->m_EmailIMEControl.AddString( rightstring, length - cursorx );
			pEmail_Window->m_EmailIMEControl.SetCursorX_ByMBCSSize(0);
		}
		
		if( pEmail_Window->m_EmailTextScrollBar )
		{
			
			pEmail_Window->m_EmailTextScrollBar->MoveWindow( winpos.x + 165, winpos.y + 56 );
		//	if( pEmail_Window->m_EmailTextLineTotalCount > 8 )
		//	{
				pEmail_Window->m_EmailTextScrollBar->SetTotalLineCount( pEmail_Window->m_EmailTextLineTotalCount );	
				
				if( !pEmail_Window->m_EmailTextScrollBar->Dragging() )
				{
					pEmail_Window->m_EmailTextScrollBar->SetLineScrollPos( pEmail_Window->m_EmailTextCurrentLinePos ); 
				}
		//	}
		//	else
		//	{
			//	pEmail_Window->m_EmailTextScrollBar->SetTotalLineCount( 9 );	
		//	}
			pEmail_Window->m_EmailTextScrollBar->UpdateData();
			
		}								
		
		int scrollpos = pEmail_Window->m_EmailTextScrollBar->GetLineScrollPos();
		
		pEmail_Window->CalculateEmailLength();

		if( _XDEF_MAX_EMAILLENGTH - pEmail_Window->m_EmailTextLength > 25 )
			pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
		else
			pEmail_Window->m_EmailIMEControl.SetMaxLength( _XDEF_MAX_EMAILLENGTH - pEmail_Window->m_EmailTextLength );


		pEmail_Window->m_EmailIMEControl.MoveWindow(winpos.x + 10,
			winpos.y + 60 +((pEmail_Window->m_EmailTextCurrentLinePos - scrollpos)*17) );
		pEmail_Window->m_EmailIMEControl.SetFocus();
		
	}
	else
	{
		int scrollpos = pEmail_Window->m_EmailTextScrollBar->GetLineScrollPos();
		if( cursorx == length )
		{
			for( int i = pEmail_Window->m_EmailTextLineTotalCount-1; i > pEmail_Window->m_EmailTextCurrentLinePos; i--)
			{
				TCHAR buffer[128];
				memset( buffer, 0, sizeof(TCHAR)*128 );
				strcpy( buffer, pEmail_Window->m_EmailDisplayText[ i ] );
				memset( pEmail_Window->m_EmailDisplayText[ i ], 0, sizeof(TCHAR)*128 );
				memset( pEmail_Window->m_EmailDisplayText[ i + 1 ], 0, sizeof(TCHAR)*128 );
				strcpy( pEmail_Window->m_EmailDisplayText[ i + 1 ], buffer);
			}
			pEmail_Window->m_EmailTextLineTotalCount++;
			pEmail_Window->m_EmailTextCurrentLinePos++;
			_XIMEKERNEL.ResetCombinationBuffer();
			pEmail_Window->m_EmailIMEControl.ClearBuffer();
		}
		else
		{
			
			scrollpos = pEmail_Window->m_EmailTextScrollBar->GetLineScrollPos();

			for( int i = pEmail_Window->m_EmailTextLineTotalCount-1; i > pEmail_Window->m_EmailTextCurrentLinePos; i--)
			{
				TCHAR buffer[128];
				memset( buffer, 0, sizeof(TCHAR)*128 );
				strcpy( buffer, pEmail_Window->m_EmailDisplayText[ i ] );
				memset( pEmail_Window->m_EmailDisplayText[ i ], 0, sizeof(TCHAR)*128 );
				memset( pEmail_Window->m_EmailDisplayText[ i + 1 ], 0, sizeof(TCHAR)*128 );
				strcpy( pEmail_Window->m_EmailDisplayText[ i + 1 ], buffer);
			}
			TCHAR leftstring[128];
			TCHAR rightstring[128];
			memset( leftstring, 0, sizeof(TCHAR)*128 );
			memset( rightstring, 0, sizeof(TCHAR)*128 );
			strncpy( leftstring, pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], cursorx );
			strncpy( rightstring, pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] + cursorx, length - cursorx );
			
			memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
			strncpy( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] , leftstring, cursorx);
			
			pEmail_Window->m_EmailTextLineTotalCount++;
			pEmail_Window->m_EmailTextCurrentLinePos++;
			_XIMEKERNEL.ResetCombinationBuffer();
			pEmail_Window->m_EmailIMEControl.ClearBuffer();
			pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
			pEmail_Window->m_EmailIMEControl.AddString( rightstring, length - cursorx );
			pEmail_Window->m_EmailIMEControl.SetCursorX_ByMBCSSize(0);
		}
		
		if( pEmail_Window->m_EmailTextScrollBar )
		{
			
			pEmail_Window->m_EmailTextScrollBar->MoveWindow( winpos.x + 165, winpos.y + 56 );
		//	if( pEmail_Window->m_EmailTextLineTotalCount > 8 )
		//	{
				pEmail_Window->m_EmailTextScrollBar->SetTotalLineCount( pEmail_Window->m_EmailTextLineTotalCount );	
		//	}
		//	else
		//	{
			//	pEmail_Window->m_EmailTextScrollBar->SetTotalLineCount(9);
		//	}
			pEmail_Window->m_EmailTextScrollBar->UpdateData();
		}								
		
		
		pEmail_Window->CalculateEmailLength();
		
		if( _XDEF_MAX_EMAILLENGTH - pEmail_Window->m_EmailTextLength > 25 )
			pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
		else
			pEmail_Window->m_EmailIMEControl.SetMaxLength( _XDEF_MAX_EMAILLENGTH - pEmail_Window->m_EmailTextLength );
		
		int linecount = 0;
		if( pEmail_Window->m_EmailTextCurrentLinePos  >= pEmail_Window->m_EmailTextScrollPos + 8)
		{
			if( pEmail_Window->m_EmailTextScrollBar )
			{			
				pEmail_Window->m_EmailTextScrollBar->SetLineScrollPos( pEmail_Window->m_EmailTextScrollPos + 1 );
			}					
			linecount = 7;			
		}
		else
		{
			if( scrollpos == pEmail_Window->m_EmailTextLineTotalCount - 8 - 1)
			{
				linecount = pEmail_Window->m_EmailTextCurrentLinePos - 1 - pEmail_Window->m_EmailTextScrollPos;
			}
			else
			{
				linecount = pEmail_Window->m_EmailTextCurrentLinePos - pEmail_Window->m_EmailTextScrollPos;
			}
		}
		
		pEmail_Window->m_EmailIMEControl.MoveWindow(winpos.x + 10, winpos.y + 60  +(linecount*17) );
		pEmail_Window->m_EmailIMEControl.SetFocus();
		
	}	

}

void __stdcall _XIMECallback_EmailEditBack( _XIMEContainer* pIMEContainer )
{			
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	
	if( !pEmail_Window ) return;
	if( pEmail_Window->m_SelectedTab != _XEMAILTAB_SEND) return;
	if( !pEmail_Window->m_EmailIMEControl.GetFocus() ) return;
	
	int cursorx = pEmail_Window->m_EmailIMEControl.GetCursorX_ByMBCSSize();
	int length = pEmail_Window->m_EmailIMEControl.GetLength_ByMBCSSize();
	
	if( cursorx == 0 )
	{
		if( pEmail_Window->m_EmailTextCurrentLinePos != 0 )
		{
			POINT winpos;				
			winpos = pEmail_Window->GetWindowPos();
						
			if( strlen( pEmail_Window->m_EmailIMEControl.GetText() ) == 0 )
			{
				
				if( pEmail_Window->m_EmailTextLineTotalCount - 1 == pEmail_Window->m_EmailTextCurrentLinePos )
				{
					
					pEmail_Window->m_EmailTextLineTotalCount--;
					
					_XIMEKERNEL.ResetCombinationBuffer();
					pEmail_Window->m_EmailIMEControl.ClearBuffer();
					pEmail_Window->m_EmailTextCurrentLinePos--;
					
					pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
					int length = strlen( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] );
					pEmail_Window->m_EmailIMEControl.AddString( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], length );
					
					memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
					pEmail_Window->m_EmailIMEControl.SetCursorX_ByMBCSSize( length );
					
					pEmail_Window->CalculateEmailLength();
					
					if( _XDEF_MAX_EMAILLENGTH - pEmail_Window->m_EmailTextLength > 25 )
						pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
					else
						pEmail_Window->m_EmailIMEControl.SetMaxLength( _XDEF_MAX_EMAILLENGTH - pEmail_Window->m_EmailTextLength );
					
					int linecount;

					if( pEmail_Window->m_EmailTextLineTotalCount >= 8 )
					{
						linecount = 7;
					}
					else
					{ 
						linecount = pEmail_Window->m_EmailTextLineTotalCount - 1;
					}


					pEmail_Window->m_EmailIMEControl.MoveWindow(winpos.x + 10, winpos.y + 60 +(linecount*17) );
					pEmail_Window->m_EmailIMEControl.SetFocus();
					
					if( pEmail_Window->m_EmailTextScrollBar )
					{
						pEmail_Window->m_EmailTextScrollBar->SetTotalLineCount( pEmail_Window->m_EmailTextLineTotalCount );	
//						if( pEmail_Window->m_EmailTextLineTotalCount > 8 )
//						{
//						}
//						else
//						{
//						//	pEmail_Window->m_EmailTextScrollBar->SetTotalLineCount( 9 );	
//						}

						pEmail_Window->m_EmailTextScrollBar->UpdateData();
						
					}
				}
				else
				{
					int scrollpos = pEmail_Window->m_EmailTextScrollBar->GetLineScrollPos();
										
					for( int i = pEmail_Window->m_EmailTextCurrentLinePos; i < pEmail_Window->m_EmailTextLineTotalCount - 1; i++)
					{
						TCHAR buffer[128];
						memset( buffer, 0, sizeof(TCHAR)*128 );
						strcpy( buffer, pEmail_Window->m_EmailDisplayText[ i + 1 ] );
						memset( pEmail_Window->m_EmailDisplayText[ i ], 0, sizeof(TCHAR)*128 );
						memset( pEmail_Window->m_EmailDisplayText[ i + 1 ], 0, sizeof(TCHAR)*128 );
						strcpy( pEmail_Window->m_EmailDisplayText[ i ], buffer);
					}
					
					pEmail_Window->m_EmailTextLineTotalCount--;
					
					_XIMEKERNEL.ResetCombinationBuffer();
					pEmail_Window->m_EmailIMEControl.ClearBuffer();
					pEmail_Window->m_EmailTextCurrentLinePos--;
					
					pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
					int length = strlen( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] );
					pEmail_Window->m_EmailIMEControl.AddString( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], length );
					
					memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
					pEmail_Window->m_EmailIMEControl.SetCursorX_ByMBCSSize( length );
					
					
					pEmail_Window->CalculateEmailLength();
					
					if( _XDEF_MAX_EMAILLENGTH - pEmail_Window->m_EmailTextLength > 25 )
						pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
					else
						pEmail_Window->m_EmailIMEControl.SetMaxLength( _XDEF_MAX_EMAILLENGTH - pEmail_Window->m_EmailTextLength );
					
					if( pEmail_Window->m_EmailTextScrollBar )
					{
					//	if( pEmail_Window->m_EmailTextLineTotalCount > 8 )
					//	{
							pEmail_Window->m_EmailTextScrollBar->SetTotalLineCount( pEmail_Window->m_EmailTextLineTotalCount );	
					//	}
					//	else
					//	{
						//	pEmail_Window->m_EmailTextScrollBar->SetTotalLineCount( 9 );	
					//	}	
						pEmail_Window->m_EmailTextScrollBar->UpdateData();
						
					}
					
					int linecount = 0;
					if( pEmail_Window->m_EmailTextCurrentLinePos  < pEmail_Window->m_EmailTextScrollPos)
					{
						if( pEmail_Window->m_EmailTextScrollBar )
						{			
							pEmail_Window->m_EmailTextScrollBar->SetLineScrollPos( pEmail_Window->m_EmailTextCurrentLinePos + 1 );
						}								
					}
					else
					{

						pEmail_Window->m_EmailTextScrollBar->SetLineScrollPos( scrollpos );

						if( scrollpos == 0 )
						{
							linecount = pEmail_Window->m_EmailTextCurrentLinePos - pEmail_Window->m_EmailTextScrollPos;
						}
						else
						{
							if( scrollpos == pEmail_Window->m_EmailTextLineTotalCount - 8 + 1)
							{
								linecount = pEmail_Window->m_EmailTextCurrentLinePos + 1 - pEmail_Window->m_EmailTextScrollPos;
							}
							else
							{
								linecount = pEmail_Window->m_EmailTextCurrentLinePos - pEmail_Window->m_EmailTextScrollPos;
							}
						}
					}
					
					pEmail_Window->m_EmailIMEControl.MoveWindow(winpos.x + 10, winpos.y + 60 +(linecount*17) );
					pEmail_Window->m_EmailIMEControl.SetFocus();
					
				}
				
			}
		}
	}
	
	
}

void __stdcall _XIMECallback_EmailEditUp( _XIMEContainer* pIMEContainer )
{			
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	
	
	if( !pEmail_Window ) return;
	if(pEmail_Window->m_SelectedTab != _XEMAILTAB_SEND) return;
	if( !pEmail_Window->m_EmailIMEControl.GetFocus() ) return;
	
	if( pEmail_Window->m_EmailTextCurrentLinePos == 0 ) return;
	POINT winpos;				
	winpos = pEmail_Window->GetWindowPos();
	
	memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
	strcpy( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] , pEmail_Window->m_EmailIMEControl.GetText());
		
	int cursorx = pEmail_Window->m_EmailIMEControl.GetCursorX_ByMBCSSize();
	
	_XIMEKERNEL.ResetCombinationBuffer();
	pEmail_Window->m_EmailIMEControl.ClearBuffer();
	pEmail_Window->m_EmailTextCurrentLinePos--;
	
	pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
	int length = strlen( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] );
	pEmail_Window->m_EmailIMEControl.AddString( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], length );
	

	memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
	pEmail_Window->m_EmailIMEControl.SetCursorX_ByMBCSSize( cursorx );
	
	int linecount = 0;
	if( pEmail_Window->m_EmailTextCurrentLinePos  < pEmail_Window->m_EmailTextScrollPos)
	{
		if( pEmail_Window->m_EmailTextScrollBar )
		{			
			pEmail_Window->m_EmailTextScrollBar->SetLineScrollPos( pEmail_Window->m_EmailTextCurrentLinePos );
		}								
	}
	else
	{
		linecount = pEmail_Window->m_EmailTextCurrentLinePos - pEmail_Window->m_EmailTextScrollPos;
	}

	pEmail_Window->m_EmailIMEControl.MoveWindow(winpos.x + 10, winpos.y + 60 +(linecount*17) );
	pEmail_Window->m_EmailIMEControl.SetFocus();

}
void __stdcall _XIMECallback_EmailEditDown( _XIMEContainer* pIMEContainer )
{			
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	
	if( !pEmail_Window ) return;
	if(pEmail_Window->m_SelectedTab != _XEMAILTAB_SEND) return;
	if( !pEmail_Window->m_EmailIMEControl.GetFocus() ) return;
				
	if( pEmail_Window->m_EmailTextCurrentLinePos == pEmail_Window->m_EmailTextLineTotalCount-1 ) return;
	POINT winpos;				
	winpos = pEmail_Window->GetWindowPos();
	
	memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
	strcpy( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] , pEmail_Window->m_EmailIMEControl.GetText());
					
	int cursorx = pEmail_Window->m_EmailIMEControl.GetCursorX_ByMBCSSize();
	
	_XIMEKERNEL.ResetCombinationBuffer();
	pEmail_Window->m_EmailIMEControl.ClearBuffer();
	pEmail_Window->m_EmailTextCurrentLinePos++;
	
	pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
	int length = strlen( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] );
	pEmail_Window->m_EmailIMEControl.AddString( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], length );
					
	memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
	pEmail_Window->m_EmailIMEControl.SetCursorX_ByMBCSSize( cursorx );
	
	int linecount = 0;
	if( pEmail_Window->m_EmailTextCurrentLinePos  >= pEmail_Window->m_EmailTextScrollPos + 8)
	{
		if( pEmail_Window->m_EmailTextScrollBar )
		{			
			pEmail_Window->m_EmailTextScrollBar->SetLineScrollPos( pEmail_Window->m_EmailTextScrollPos + 1 );
		}					
		linecount = 7;			
	}
	else
	{
		linecount = pEmail_Window->m_EmailTextCurrentLinePos - pEmail_Window->m_EmailTextScrollPos;
	}
	
	pEmail_Window->m_EmailIMEControl.MoveWindow(winpos.x + 10, winpos.y + 60 +(linecount*17) );
	pEmail_Window->m_EmailIMEControl.SetFocus();
	

}
void __stdcall _XIMECallback_EmailEditLeft( _XIMEContainer* pIMEContainer )
{			
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);

	if( !pEmail_Window ) return;
	if(pEmail_Window->m_SelectedTab != _XEMAILTAB_SEND) return;
	if( !pEmail_Window->m_EmailIMEControl.GetFocus() ) return;

	int cursorx = pEmail_Window->m_EmailIMEControl.GetCursorX_ByMBCSSize();
	int length = pEmail_Window->m_EmailIMEControl.GetLength_ByMBCSSize();
	
	if( cursorx == 0 )
	{
		if( pEmail_Window->m_EmailTextCurrentLinePos != 0 )
		{
			POINT winpos;				
			winpos = pEmail_Window->GetWindowPos();
			
			memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
			strcpy( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] , pEmail_Window->m_EmailIMEControl.GetText());
						
			_XIMEKERNEL.ResetCombinationBuffer();
			pEmail_Window->m_EmailIMEControl.ClearBuffer();
			pEmail_Window->m_EmailTextCurrentLinePos--;
			
			pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
			int length = strlen( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] );
			pEmail_Window->m_EmailIMEControl.AddString( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], length );
			
			memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
			pEmail_Window->m_EmailIMEControl.SetCursorX_ByMBCSSize( length );
			
			int linecount = 0;
			if( pEmail_Window->m_EmailTextCurrentLinePos  < pEmail_Window->m_EmailTextScrollPos)
			{
				if( pEmail_Window->m_EmailTextScrollBar )
				{			
					pEmail_Window->m_EmailTextScrollBar->SetLineScrollPos( pEmail_Window->m_EmailTextCurrentLinePos );
				}								
			}
			else
			{
				linecount = pEmail_Window->m_EmailTextCurrentLinePos - pEmail_Window->m_EmailTextScrollPos;
			}
			
			pEmail_Window->m_EmailIMEControl.MoveWindow(winpos.x + 10, winpos.y + 60 +(linecount*17) );
			pEmail_Window->m_EmailIMEControl.SetFocus();
		}
	}

}
void __stdcall _XIMECallback_EmailEditRight( _XIMEContainer* pIMEContainer )
{			
	_XWindow_Email* pEmail_Window = (_XWindow_Email*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EMAILWINDOW);
	
	if( !pEmail_Window ) return;
	if( pEmail_Window->m_SelectedTab != _XEMAILTAB_SEND) return;
	if( !pEmail_Window->m_EmailIMEControl.GetFocus() ) return;
	
	int cursorx = pEmail_Window->m_EmailIMEControl.GetCursorX_ByMBCSSize();
	int length = pEmail_Window->m_EmailIMEControl.GetLength_ByMBCSSize();
	
	if( cursorx == length )
	{
		if( pEmail_Window->m_EmailTextCurrentLinePos != pEmail_Window->m_EmailTextLineTotalCount-1 )
		{
			POINT winpos;				
			winpos = pEmail_Window->GetWindowPos();
			
			memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
			strcpy( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] , pEmail_Window->m_EmailIMEControl.GetText());
			
			_XIMEKERNEL.ResetCombinationBuffer();
			pEmail_Window->m_EmailIMEControl.ClearBuffer();
			pEmail_Window->m_EmailTextCurrentLinePos++;
			
			pEmail_Window->m_EmailIMEControl.SetMaxLength( 25 );
			int length = strlen( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos] );
			pEmail_Window->m_EmailIMEControl.AddString( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], length );
			
			memset( pEmail_Window->m_EmailDisplayText[pEmail_Window->m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
			pEmail_Window->m_EmailIMEControl.SetCursorX_ByMBCSSize( 0 );
			
			int linecount = 0;
			if( pEmail_Window->m_EmailTextCurrentLinePos  >= pEmail_Window->m_EmailTextScrollPos + 8)
			{
				if( pEmail_Window->m_EmailTextScrollBar )
				{			
					pEmail_Window->m_EmailTextScrollBar->SetLineScrollPos( pEmail_Window->m_EmailTextScrollPos + 1 );
				}					
				linecount = 7;			
			}
			else
			{
				linecount = pEmail_Window->m_EmailTextCurrentLinePos - pEmail_Window->m_EmailTextScrollPos;
			}
			
			pEmail_Window->m_EmailIMEControl.MoveWindow(winpos.x + 10, winpos.y + 60 +(linecount*17) );
			pEmail_Window->m_EmailIMEControl.SetFocus();
						
		}
	}
}
void __stdcall _XIMECallback_EmailEditEscape( _XIMEContainer* pIMEContainer )
{			
	//	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	//	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();			
	//PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_ITEMDROPCONFIRM_CLOSE, 0), (LPARAM)gHWnd); //2004.06.24->oneway48 insert
}

_XWindow_Email::_XWindow_Email()
{
	
	m_imageEmailTitleBar					= NULL;		// 전서구 타이틀 바
	m_imageEmailDisplayBoxUpBorder			= NULL;		// 전서구 내용표시 박스 위쪽 바탕 
	m_imageEmailDisplayBoxDownBorder		= NULL;		// 전서구 내용표시 박스 아래쪽 바탕 
	m_imageEmailDisplayBoxLeftSideBorder	= NULL;		// 전서구 왼쪽 사이드 이미지
	m_imageEmailDisplayBoxRightSideBorder	= NULL;		// 전서구 오른쪽 사이드 이미지
	m_imageEmailDownBorder					= NULL;		// 전서구 창 아래쪽  바탕
	m_imageEmailDockingBorder				= NULL;		// 전서구 메인 메뉴창에 도킹했을때 생성되는 바탕
	m_imageEmailDownEndLine					= NULL;		// 전서구 창 아래쪽 끝 라인 
	m_imageEmailCountBoxBorder				= NULL;		// 전서구 보관된 편지 표시 창 박스

	m_SelectedTab							= _XEMAILTAB_RECEIVE;	
	m_TabButtonKeepingBox					= NULL;		// 전서구 탭버튼 보관함
	m_TabButtonReceiveBox					= NULL;		// 전서구 탭버튼 수신함
	m_TabButtonSendBox						= NULL;		// 전서구 탭버튼 발신함
	m_btnEmailClose							= NULL;		// 전서구 창 닫기 버튼
	m_btnEmailHelp							= NULL;		// 전서구 창 도움 버튼
	m_btnNext								= NULL;		// 저장된 편지 다음 보기 버튼 
	m_btnPrev								= NULL;		// 저장된 편지 이전 보기 버튼
	m_btnKeepingMailDelete					= NULL;		// 저장된 편지 삭제하기 버튼
	m_btnReceiveReply						= NULL;		// 받은 편지 답장하기 버튼
	m_btnReceiveSave						= NULL;		// 받은 편지 보관하기 버튼
	m_btnReceiveDelete						= NULL;		// 받은 편지 삭제하기 버튼
	m_btnSendDelete							= NULL;		// 보낼 편지 내용 지우기 버튼
	m_btnSendCancle							= NULL;		// 보낼 편지 취소 버튼
	m_btnSend								= NULL;		// 보낼 편지 보내기 버튼
	
	memset(m_EmailDisplayText, 0, sizeof(TCHAR) * 128 * 256);
	memset(m_EmailText, 0, sizeof(TCHAR) * 101);
	memset(m_EmailReceiver, 0, sizeof(TCHAR) * 13);
	memset(m_EmailSender, 0, sizeof(TCHAR) * 20);
	m_EmailTextLineTotalCount = 1;
	m_EmailTextCurrentLinePos = 0;
	m_EmailTextLength = 0;
	
	m_EmailTextScrollPos = 0;
	m_EmailTextScrollBar = NULL;	
	m_pScrbarFocusedObject = NULL;	
	m_EmailTextTotalCount = 0;
	m_indexEmailText = 0;
	m_EmailReceiveTextTotalCount = 0;
	m_indexEmailReceiveText = 0;
	ResetTempEmailInfo();
	
	m_bDockingMode = FALSE;
	m_EmailColor = _XSC_DEFAULT;
}

_XWindow_Email::~_XWindow_Email()
{	
}

BOOL _XWindow_Email::Initialize(void)
{
	int mainbackresourceindex1	= g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int messengerresourceindex1	= g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	int messengerresourceindex2	= g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" );
	int charinfoindex			= g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int tradeindex				= g_MainInterfaceTextureArchive.FindResource("MI_Trade.tga");	
	int chattingbuttonindex		= g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	
	//  전서구 타이틀 바	( 183 X 54 )
	m_imageEmailTitleBar = new _XImageStatic;
	m_imageEmailTitleBar->Create( 0, 0, 183, 54, &g_MainInterfaceTextureArchive, mainbackresourceindex1 );
	m_imageEmailTitleBar->SetClipRect(0, 125, 183, 179);
	m_imageEmailTitleBar->ShowWindow(TRUE);
	InsertChildObject( m_imageEmailTitleBar );
	
	//  전서구 내용표시 박스 위쪽 바탕   ( 183 X 3 )
	m_imageEmailDisplayBoxUpBorder = new _XImageStatic;
	m_imageEmailDisplayBoxUpBorder->Create( 0, 54, 183, 57, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageEmailDisplayBoxUpBorder->SetClipRect(0, 0, 183, 3);
	m_imageEmailDisplayBoxUpBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageEmailDisplayBoxUpBorder );
	
	// 전서구 왼쪽 사이드 이미지 ( 9 X 137 )
	m_imageEmailDisplayBoxLeftSideBorder = new _XImageStatic;
	m_imageEmailDisplayBoxLeftSideBorder->Create( 0, 57, 9, 194, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageEmailDisplayBoxLeftSideBorder->SetClipRect(0, 119, 9, 256);
	m_imageEmailDisplayBoxLeftSideBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageEmailDisplayBoxLeftSideBorder );
	
	// 전서구 오른쪽 사이드 이미지 ( 20 X 137 )
	m_imageEmailDisplayBoxRightSideBorder = new _XImageStatic;
	m_imageEmailDisplayBoxRightSideBorder->Create( 163, 57, 183, 194, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageEmailDisplayBoxRightSideBorder->SetClipRect(10, 119, 30, 256);
	m_imageEmailDisplayBoxRightSideBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageEmailDisplayBoxRightSideBorder );
	
	// 전서구 내용표시 박스 아래쪽 바탕	 ( 183 X 21 )
	m_imageEmailDisplayBoxDownBorder = new _XImageStatic;
	m_imageEmailDisplayBoxDownBorder->Create( 0, 194, 183, 215, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageEmailDisplayBoxDownBorder->SetClipRect( 0, 4, 183, 25);
	m_imageEmailDisplayBoxDownBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageEmailDisplayBoxDownBorder );
	
	// 전서구 창 아래쪽  바탕	 ( 183 X 52 )
	m_imageEmailDownBorder = new _XImageStatic;
	m_imageEmailDownBorder->Create( 0, 215, 183, 267, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageEmailDownBorder->SetClipRect( 0, 26, 183, 78);
	m_imageEmailDownBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageEmailDownBorder );
	
	// 전서구 메인 메뉴창에 도킹했을때 생성되는 바탕	 ( 183 X 21 )
	m_imageEmailDockingBorder = new _XImageStatic;
	m_imageEmailDockingBorder->Create( 0, 267, 183, 288, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageEmailDockingBorder->SetClipRect( 0, 79, 183, 100);
	m_imageEmailDockingBorder->ShowWindow(TRUE);
	InsertChildObject( m_imageEmailDockingBorder );
	
	// 전서구 창 아래쪽 끝 라인	 ( 183 X 3 )
	m_imageEmailDownEndLine = new _XImageStatic;
	m_imageEmailDownEndLine->Create( 0, 267, 183, 270, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageEmailDownEndLine->SetClipRect( 0, 106, 183, 109);
	m_imageEmailDownEndLine->ShowWindow(FALSE);
	InsertChildObject( m_imageEmailDownEndLine );
	
	// 전서구 보관된 편지 표시 창 박스	 ( 112 X 16 )
	m_imageEmailCountBoxBorder = new _XImageStatic;
	m_imageEmailCountBoxBorder->Create( 9, 217, 121, 233, &g_MainInterfaceTextureArchive, messengerresourceindex1 );
	m_imageEmailCountBoxBorder->SetClipRect( 33, 119, 145, 135);
	m_imageEmailCountBoxBorder->ShowWindow(FALSE);
	InsertChildObject( m_imageEmailCountBoxBorder );
	
	// 전서구 닫기 버튼	 ( 12 X 12 ) 
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_EMAILWINDOW_EMAILCLOSE,
		charinfoindex,charinfoindex,charinfoindex, 
		&g_MainInterfaceTextureArchive };
	m_btnEmailClose = new _XButton;
	m_btnEmailClose->Create( closebtnstruct );	
	
	m_btnEmailClose->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_btnEmailClose->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_btnEmailClose->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	m_btnEmailClose->ShowWindow(TRUE);
	InsertChildObject( m_btnEmailClose );
	
	//전서구 도움 버튼 	 ( 12 X 12 ) 
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_EMAILWINDOW_EMAILHELP,
		charinfoindex,charinfoindex,charinfoindex, 
		&g_MainInterfaceTextureArchive };
	m_btnEmailHelp = new _XButton;
	m_btnEmailHelp->Create( helpstruct );	
	
	m_btnEmailHelp->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	m_btnEmailHelp->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	m_btnEmailHelp->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	m_btnEmailHelp->ShowWindow(TRUE);
	InsertChildObject( m_btnEmailHelp );

	// 전서구 탭버튼 보관함	 ( 58 X 22 )
	_XCHECKBTN_STRUCTURE emailcheckbtnstruct = 
	{ 
		TRUE, { 5, 217}, {58, 22}, 
			_XDEF_EMAILWINDOW_EMAILKEEPBOX, messengerresourceindex1, messengerresourceindex1,
			&g_MainInterfaceTextureArchive
	};
	m_TabButtonKeepingBox = new _XCheckButton;
	m_TabButtonKeepingBox->Create(emailcheckbtnstruct);
	m_TabButtonKeepingBox->SetButtonSwapMode(TRUE);
	m_TabButtonKeepingBox->SetCheck(FALSE);
	m_TabButtonKeepingBox->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_KEEPINGBOX);
	m_TabButtonKeepingBox->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
	m_TabButtonKeepingBox->SetGrowTextMode( TRUE, 0xFF000000 );
	m_TabButtonKeepingBox->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 190, 222, 248, 244);
	m_TabButtonKeepingBox->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 190, 168, 248, 190);
	m_TabButtonKeepingBox->ShowWindow(TRUE);
	InsertChildObject(m_TabButtonKeepingBox);
	
	// 전서구 탭버튼 수신함	 ( 58 X 22 )
	emailcheckbtnstruct.position.x += 58;
	emailcheckbtnstruct.commandid = _XDEF_EMAILWINDOW_EMAILRECEIVEBOX;
	m_TabButtonReceiveBox = new _XCheckButton;
	m_TabButtonReceiveBox->Create(emailcheckbtnstruct);
	m_TabButtonReceiveBox->SetButtonSwapMode(TRUE);
	m_TabButtonReceiveBox->SetCheck(FALSE);
	m_TabButtonReceiveBox->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_RECEIVEBOX);
	m_TabButtonReceiveBox->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
	m_TabButtonReceiveBox->SetGrowTextMode( TRUE, 0xFF000000 );
	m_TabButtonReceiveBox->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 190, 222, 248, 244);
	m_TabButtonReceiveBox->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 190, 168, 248, 190);
	m_TabButtonReceiveBox->ShowWindow(TRUE);
	InsertChildObject(m_TabButtonReceiveBox);
	
	// 전서구 탭버튼 발신함	 ( 58 X 22 )
	emailcheckbtnstruct.position.x += 58;
	emailcheckbtnstruct.commandid = _XDEF_EMAILWINDOW_EMAILSENDBOX;
	m_TabButtonSendBox = new _XCheckButton;
	m_TabButtonSendBox->Create(emailcheckbtnstruct);
	m_TabButtonSendBox->SetButtonSwapMode(TRUE);
	m_TabButtonSendBox->SetCheck(FALSE);
	m_TabButtonSendBox->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_SENDBOX);
	m_TabButtonSendBox->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
	m_TabButtonSendBox->SetGrowTextMode( TRUE, 0xFF000000 );
	m_TabButtonSendBox->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 190, 222, 248, 244);
	m_TabButtonSendBox->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 190, 168, 248, 190);
	m_TabButtonSendBox->ShowWindow(TRUE);
	InsertChildObject(m_TabButtonSendBox);

	// 저장된 편지 다음 보기 버튼 	 ( 10 X 10 )
	_XBTN_STRUCTURE emailbutton1 = {
		TRUE, { 83, 201  }, { 10, 10 }, 
		_XDEF_EMAILWINDOW_EMAILNEXT, messengerresourceindex1, messengerresourceindex1, messengerresourceindex1, 
		&g_MainInterfaceTextureArchive 
	};
	m_btnNext = new _XButton;
	m_btnNext->Create( emailbutton1 );	
	m_btnNext->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 101, 204, 111, 214 );
	m_btnNext->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 91, 204, 101, 214 );
	m_btnNext->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 111, 204, 121, 214 );
	m_btnNext->ShowWindow(FALSE);
	InsertChildObject( m_btnNext );
	
	// 저장된 편지 이전 보기 버튼 	 ( 10 X 10 )
	emailbutton1.position.x = 35;
	emailbutton1.commandid = _XDEF_EMAILWINDOW_EMAILPREV; 
	m_btnPrev = new _XButton;
	m_btnPrev->Create( emailbutton1 );	
	m_btnPrev->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 70, 204, 80, 214 );
	m_btnPrev->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 60, 204, 70, 214 );
	m_btnPrev->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 80, 204, 90, 214 );
	m_btnPrev->ShowWindow(FALSE);
	InsertChildObject( m_btnPrev );
	
	
	// 저장된 편지 삭제하기 버튼 	 ( 56 X 18 )
	_XBTN_STRUCTURE emailbutton2 = 
	{
		TRUE, {121, 196}, {56, 18},
		_XDEF_EMAILWINDOW_EMAILKEEPINGMAILDELETE, tradeindex, tradeindex, tradeindex,
		&g_MainInterfaceTextureArchive
	};
	
	m_btnKeepingMailDelete = new _XButton;
	m_btnKeepingMailDelete->Create( emailbutton2 );	
	m_btnKeepingMailDelete->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	m_btnKeepingMailDelete->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	m_btnKeepingMailDelete->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245241, 136 );
	m_btnKeepingMailDelete->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnKeepingMailDelete->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_DEL); // 삭제
	m_btnKeepingMailDelete->ShowWindow(FALSE);
	InsertChildObject( m_btnKeepingMailDelete );
	
	// 받은 편지 답장하기 버튼 	 ( 56 X 18 )
	emailbutton2.position.x = 7;
	emailbutton2.commandid = _XDEF_EMAILWINDOW_EMAILRECEIVEREPLY; 
	m_btnReceiveReply = new _XButton;
	m_btnReceiveReply->Create( emailbutton2 );	
	m_btnReceiveReply->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	m_btnReceiveReply->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	m_btnReceiveReply->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245, 136 );
	m_btnReceiveReply->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnReceiveReply->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_MAILREPLY);
	m_btnReceiveReply->ShowWindow(FALSE);
	InsertChildObject( m_btnReceiveReply );

	// 받은 편지 보관하기 버튼 	 ( 56 X 18 )
	emailbutton2.position.x = 64;
	emailbutton2.commandid = _XDEF_EMAILWINDOW_EMAILRECEIVESAVE; 
	m_btnReceiveSave = new _XButton;
	m_btnReceiveSave->Create( emailbutton2 );	
	m_btnReceiveSave->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	m_btnReceiveSave->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	m_btnReceiveSave->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245, 136 );
	m_btnReceiveSave->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnReceiveSave->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_MAILSAVE);
	m_btnReceiveSave->ShowWindow(FALSE);
	InsertChildObject( m_btnReceiveSave );
	
	// 받은 편지 삭제하기 버튼 	 ( 56 X 18 )
	emailbutton2.position.x = 121;
	emailbutton2.commandid = _XDEF_EMAILWINDOW_EMAILRECEIVEDELETE; 
	m_btnReceiveDelete = new _XButton;
	m_btnReceiveDelete->Create( emailbutton2 );	
	m_btnReceiveDelete->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	m_btnReceiveDelete->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	m_btnReceiveDelete->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245, 136 );
	m_btnReceiveDelete->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnReceiveDelete->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_DEL);	// 삭제
	m_btnReceiveDelete->ShowWindow(FALSE);
	InsertChildObject( m_btnReceiveDelete );
	
	// 보낼 편지 내용 지우기 버튼 	 ( 56 X 18 )
	emailbutton2.position.x = 7;
	emailbutton2.commandid = _XDEF_EMAILWINDOW_EMAILSENDDELETE; 
	m_btnSendDelete = new _XButton;
	m_btnSendDelete->Create( emailbutton2 );	
	m_btnSendDelete->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	m_btnSendDelete->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	m_btnSendDelete->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245, 136 );
	m_btnSendDelete->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnSendDelete->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_DEL);	// 삭제
	m_btnSendDelete->ShowWindow(FALSE);
	InsertChildObject( m_btnSendDelete );
	
	// 보낼 편지 취소 버튼 	 ( 56 X 18 )
	emailbutton2.position.x = 64;
	emailbutton2.commandid = _XDEF_EMAILWINDOW_EMAILSENDCANCLE; 
	m_btnSendCancle = new _XButton;
	m_btnSendCancle->Create( emailbutton2 );	
	m_btnSendCancle->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	m_btnSendCancle->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	m_btnSendCancle->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245, 136 );
	m_btnSendCancle->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnSendCancle->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);
	m_btnSendCancle->ShowWindow(FALSE);
	InsertChildObject( m_btnSendCancle );
	
	// 보낼 편지 보내기 버튼 	 ( 56 X 18 )
	emailbutton2.position.x = 121;
	emailbutton2.commandid = _XDEF_EMAILWINDOW_EMAILSEND; 
	m_btnSend = new _XButton;
	m_btnSend->Create( emailbutton2 );	
	m_btnSend->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	m_btnSend->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	m_btnSend->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245, 136 );
	m_btnSend->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnSend->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_MAILSEND);
	m_btnSend->ShowWindow(FALSE);
	InsertChildObject( m_btnSend );
	
	
	m_EmailIMEControl.ResetContainer();
	m_EmailIMEControl.ResizeWindow(152, 15);
	m_EmailIMEControl.ShowWindow(FALSE);
	m_EmailIMEControl.SetMaxLength(25);
	m_EmailIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );

	m_EmailIMEControl.SetEscapeKeyCallback( _XIMECallback_EmailEditEscape );
	m_EmailIMEControl.SetReturnKeyCallback( _XIMECallback_EmailEditReturn );
	m_EmailIMEControl.SetUpArrowKeyCallback( _XIMECallback_EmailEditUp );
	m_EmailIMEControl.SetDownArrowKeyCallback( _XIMECallback_EmailEditDown );
	m_EmailIMEControl.SetLeftArrowKeyCallback( _XIMECallback_EmailEditLeft );
	m_EmailIMEControl.SetRightArrowKeyCallback( _XIMECallback_EmailEditRight );
	m_EmailIMEControl.SetBackSpaceKeyCallback( _XIMECallback_EmailEditBack );
	
	// 입출력창 스크롤바 ( 12 X 137 )
	SAFE_DELETE( m_EmailTextScrollBar );
	m_EmailTextScrollBar = new _XVScrollBar;
	_XSCRBAR_STRUCTURE emailscrbarstructure = 
	{
		TRUE, { 165, 56 }, 
			12, 136, // width, height
			12,		 // btnysize
			22,		 // trackbar size
			_XDEF_CHATLIST_QUESIZE, 8, 0,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			&g_MainInterfaceTextureArchive
	};
	
	m_EmailTextScrollBar->Create( emailscrbarstructure );
	m_EmailTextScrollBar->ShowWindow( TRUE );
	m_EmailTextScrollBar->SetViewNullBorder( TRUE );
//	m_EmailTextScrollBar->SetTotalLineCount(9);	
	m_EmailTextScrollBar->UpdateData();
	
	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_EmailTextScrollBar->SetClipRect( cliprectstructure );
	
	
	// 수신인 입력 창
	m_ReceiverIMEControl.ResetContainer();
	m_ReceiverIMEControl.ResizeWindow(110, 15);
	m_ReceiverIMEControl.ShowWindow(FALSE);
	m_ReceiverIMEControl.SetMaxLength(12);
	m_ReceiverIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );	

	m_ReceiverIMEControl.SetReturnKeyCallback( _XIMECallback_ReceiverEditReturn );
	ChangeTab(_XEMAILTAB_SEND);

	if( m_WindowPosition.x+m_WindowSize.cx > ( gnWidth - 100) )
	{
		if( m_WindowPosition.y+m_WindowSize.cy > ( gnHeight - 90 ) )
		{
			SetDockingMode(TRUE);
		}
		else
		{
			SetDockingMode(FALSE);
		}
	}
	else
	{
		SetDockingMode(FALSE);
	}

	return TRUE;
}

void _XWindow_Email::DestroyWindow()
{
	_XWindow::DestroyWindow();
	SAFE_DELETE( m_EmailTextScrollBar );
}

void _XWindow_Email::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	if(!this->m_ShowWindow)	return;

	// 발신인 수신인 박스 
	_XDrawSolidBar( m_WindowPosition.x + 56, m_WindowPosition.y + 32,
		m_WindowPosition.x + 176, m_WindowPosition.y + 48 , D3DCOLOR_ARGB(200,0,0,0));
	_XDrawRectAngle( m_WindowPosition.x + 56, m_WindowPosition.y + 32,
		m_WindowPosition.x + 176, m_WindowPosition.y + 48 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	int titlepos = 35;
	int namepos = 35;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		titlepos = 18;
	}

	switch(m_SelectedTab) {
	case _XEMAILTAB_RECEIVE:
		{			
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->Puts( m_WindowPosition.x + 12, m_WindowPosition.y + titlepos, _XGETINTERFACETEXT(ID_STRING_MESSENGER_2476) );
			g_XBaseFont->Puts( m_WindowPosition.x + 13, m_WindowPosition.y + titlepos, _XGETINTERFACETEXT(ID_STRING_MESSENGER_2476) );
			g_XBaseFont->SetColor( m_EmailColor );

			g_XBaseFont->Puts( m_WindowPosition.x + 61, m_WindowPosition.y + namepos, m_EmailSender );
			g_XBaseFont->Puts( m_WindowPosition.x + 62, m_WindowPosition.y + namepos, m_EmailSender );

			if( m_indexEmailReceiveText != 0 )
			{
				g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
				g_XBaseFont->Print(m_WindowPosition.x + 9, m_WindowPosition.y + 243, 1.0f, _XGETINTERFACETEXT(ID_STRING_MESSENGER_REMAINDERMAIL), m_indexEmailReceiveText );
				g_XBaseFont->Print(m_WindowPosition.x + 10, m_WindowPosition.y + 243, 1.0f, _XGETINTERFACETEXT(ID_STRING_MESSENGER_REMAINDERMAIL), m_indexEmailReceiveText );				
			}

			g_XBaseFont->SetColor( _XSC_DEFAULT );
			m_EmailTextScrollPos = m_EmailTextScrollBar->GetLineScrollPos();
			m_EmailTextScrollBar->Draw( m_pScrbarFocusedObject );	

			g_XBaseFont->SetColor( m_EmailColor );
			int linecount = 0;
			for( int i = m_EmailTextScrollPos ; i < m_EmailTextScrollPos + 8; i++)
			{
				g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 58 +(linecount*17), m_EmailDisplayText[i] );
				linecount++;
			}
		}
		break;
	case _XEMAILTAB_SEND:
		{
			m_EmailIMEControl.Draw();
			m_ReceiverIMEControl.Draw();
			
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->Puts( m_WindowPosition.x + 12, m_WindowPosition.y + titlepos, _XGETINTERFACETEXT(ID_STRING_MESSENGER_2477) );
			g_XBaseFont->Puts( m_WindowPosition.x + 13, m_WindowPosition.y + titlepos , _XGETINTERFACETEXT(ID_STRING_MESSENGER_2477) );

			if( m_EmailReceiver[0] == '@')
			{
				g_XBaseFont->SetColor( 0xFFBCEB7F );
			}

			g_XBaseFont->Puts( m_WindowPosition.x + 61, m_WindowPosition.y + namepos , m_EmailReceiver );
			
			m_EmailTextScrollPos = m_EmailTextScrollBar->GetLineScrollPos();
			m_EmailTextScrollBar->Draw( m_pScrbarFocusedObject );	
			
			int linecount = 0;
			for( int i = m_EmailTextScrollPos ; i < m_EmailTextScrollPos + 8; i++)
			{
				g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 60 +(linecount*17), m_EmailDisplayText[i] );
				linecount++;
			}
		}
		break;
	case _XEMAILTAB_KEEP:
		{
			
			_XDrawSolidBar( m_WindowPosition.x + 10, m_WindowPosition.y + 197,
				m_WindowPosition.x + 118, m_WindowPosition.y + 211 , D3DCOLOR_ARGB(200,0,0,0));
			_XDrawRectAngle( m_WindowPosition.x + 10, m_WindowPosition.y + 197,
				m_WindowPosition.x + 118, m_WindowPosition.y + 211 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

			m_btnNext->Draw(pfocusobject);
			m_btnPrev->Draw(pfocusobject);
			
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->Puts( m_WindowPosition.x + 12, m_WindowPosition.y + titlepos, _XGETINTERFACETEXT(ID_STRING_MESSENGER_2476) );
			g_XBaseFont->Puts( m_WindowPosition.x + 13, m_WindowPosition.y + titlepos, _XGETINTERFACETEXT(ID_STRING_MESSENGER_2476) );

			g_XBaseFont->SetColor( m_EmailColor );
			g_XBaseFont->Puts( m_WindowPosition.x + 61, m_WindowPosition.y + namepos, m_EmailSender );
			g_XBaseFont->Puts( m_WindowPosition.x + 62, m_WindowPosition.y + namepos, m_EmailSender );
			
			g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
			g_XBaseFont->Print(m_WindowPosition.x + 51, m_WindowPosition.y + 200, 1.0f, "%d/%d", m_indexEmailText, m_EmailTextTotalCount );
			
			m_EmailTextScrollPos = m_EmailTextScrollBar->GetLineScrollPos();
			m_EmailTextScrollBar->Draw( m_pScrbarFocusedObject );	
			
			g_XBaseFont->SetColor( m_EmailColor );

			int linecount = 0;
			for( int i = m_EmailTextScrollPos ; i < m_EmailTextScrollPos + 8; i++)
			{
				g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 60 +(linecount*17), m_EmailDisplayText[i] );
				linecount++;
			}
		}
		break;
	}
	g_XBaseFont->Flush();

}

#define _XDEF_EMAILRESIZETRACKSIZE 4
BOOL _XWindow_Email::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
	

	if( !CheckIMEBoxArea() && !CheckReceiverIMEBoxArea() && !m_EmailTextScrollBar->CheckMousePosition()  )
	{
		if(!_XWindow::Process(pfocusobject))
			return FALSE;
		
		ProcessDrag( pfocusobject );
	}
	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(m_SelectedTab == _XEMAILTAB_RECEIVE || m_SelectedTab == _XEMAILTAB_KEEP)
	{
	}
	else
	{
		if(mousestate)
		{
			if(mousestate->bButton[0] )
			{
				if( CheckIMEBoxArea() || CheckReceiverIMEBoxArea() )
				{
					pfocusobject = (_XGUIObject*)this;
					m_WindowMouseState = _XW_STATE_LEFTCLICK;
				}
				else if( CheckMousePosition() )
				{

					if( strlen( m_ReceiverIMEControl.GetText()) != 0 )
					{
						memset( m_EmailReceiver, 0, sizeof(TCHAR)*13 );
						strcpy( m_EmailReceiver , m_ReceiverIMEControl.GetText());
					}
					
					if( strlen( m_EmailIMEControl.GetText() ) != 0 )
					{
						memset( m_EmailDisplayText[m_EmailTextCurrentLinePos], 0, sizeof(TCHAR)*128 );
						strcpy( m_EmailDisplayText[m_EmailTextCurrentLinePos] , m_EmailIMEControl.GetText());
					}
					
					
					m_EmailIMEControl.ClearBuffer();
					_XIMEKERNEL.SetFocus( NULL ); 
				}
				else
				{
					if( strlen( m_ReceiverIMEControl.GetText()) != 0 )
					{
						memset( m_EmailReceiver, 0, sizeof(TCHAR)*13 );
						strcpy( m_EmailReceiver , m_ReceiverIMEControl.GetText());
					}
					
					if( strlen( m_EmailIMEControl.GetText() ) != 0 )
					{
						memset( m_EmailDisplayText[m_EmailTextCurrentLinePos], 0, sizeof(TCHAR)*128 );
						strcpy( m_EmailDisplayText[m_EmailTextCurrentLinePos] , m_EmailIMEControl.GetText());
					}
					
					
					m_EmailIMEControl.ClearBuffer();
					_XIMEKERNEL.SetFocus( NULL ); 
				}
			}

			if(!mousestate->bButton[0])
			{
				if( m_WindowMouseState == _XW_STATE_LEFTCLICK )
				{
					if(CheckIMEBoxArea())
					{
						if( strlen( m_ReceiverIMEControl.GetText()) != 0 )
						{
							memset( m_EmailReceiver, 0, sizeof(TCHAR)*13 );
							strcpy( m_EmailReceiver , m_ReceiverIMEControl.GetText());
						}
						
						if( m_EmailTextLineTotalCount <= 8 )
						{
							m_EmailIMEControl.SetMaxLength( 25 );
							
							int length = 0;
							if( m_EmailIMEControl.GetLength() == 0 )
							{
								length = strlen( m_EmailDisplayText[m_EmailTextCurrentLinePos] );
								m_EmailIMEControl.AddString( m_EmailDisplayText[m_EmailTextCurrentLinePos], length );									
							}


							memset( m_EmailDisplayText[m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
							m_EmailIMEControl.SetCursorX_ByMBCSSize( m_EmailIMEControl.GetLength_ByMBCSSize() );
							
							m_EmailIMEControl.MoveWindow(m_WindowPosition.x + 10,
								m_WindowPosition.y + 60 +((m_EmailTextCurrentLinePos - m_EmailTextScrollBar->GetLineScrollPos())*17) );
							
							m_EmailIMEControl.SetFocus();
						}
						else
						{
							if( m_EmailTextCurrentLinePos < m_EmailTextScrollBar->GetLineScrollPos() + 8  )
							{
								m_EmailIMEControl.SetMaxLength( 25 );
								int length = 0;
								if( m_EmailIMEControl.GetLength() == 0 )
								{
									length = strlen( m_EmailDisplayText[m_EmailTextCurrentLinePos] );
									m_EmailIMEControl.AddString( m_EmailDisplayText[m_EmailTextCurrentLinePos], length );									
								}
								
								memset( m_EmailDisplayText[m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
								m_EmailIMEControl.SetCursorX_ByMBCSSize( m_EmailIMEControl.GetLength_ByMBCSSize() );
								
								m_EmailIMEControl.MoveWindow(m_WindowPosition.x + 10,
									m_WindowPosition.y + 60 +((m_EmailTextCurrentLinePos - m_EmailTextScrollBar->GetLineScrollPos())*17) );
								
								m_EmailIMEControl.SetFocus();
							}
							else
							{
								m_EmailTextScrollBar->SetLineScrollPos(m_EmailTextCurrentLinePos-2);
								
								m_EmailIMEControl.SetMaxLength( 25 );
								int length = strlen( m_EmailDisplayText[m_EmailTextCurrentLinePos] );
								m_EmailIMEControl.AddString( m_EmailDisplayText[m_EmailTextCurrentLinePos], length );
								
								memset( m_EmailDisplayText[m_EmailTextCurrentLinePos], 0, sizeof(TCHAR) * 128);
								m_EmailIMEControl.SetCursorX_ByMBCSSize( length );
								
								m_EmailIMEControl.MoveWindow(m_WindowPosition.x + 10,
									m_WindowPosition.y + 60 +((m_EmailTextCurrentLinePos - m_EmailTextScrollBar->GetLineScrollPos())*17) );
								
								m_EmailIMEControl.SetFocus();
							}
						}

					}
					else if( CheckReceiverIMEBoxArea())
					{
						memset( m_EmailReceiver, 0, sizeof(TCHAR)*13 );
						m_ReceiverIMEControl.ClearBuffer();
						m_ReceiverIMEControl.SetFocus();
					}
					else
					{
						if( strlen( m_ReceiverIMEControl.GetText()) != 0 )
						{
							memset( m_EmailReceiver, 0, sizeof(TCHAR)*13 );
							strcpy( m_EmailReceiver , m_ReceiverIMEControl.GetText());
						}
						
						if( strlen( m_EmailIMEControl.GetText() ) != 0 )
						{
							memset( m_EmailDisplayText[m_EmailTextCurrentLinePos], 0, sizeof(TCHAR)*128 );
							strcpy( m_EmailDisplayText[m_EmailTextCurrentLinePos] , m_EmailIMEControl.GetText());
						}
						
						
						m_EmailIMEControl.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL ); 
					}
					
					m_WindowMouseState = _XW_STATE_NONE;
				}
			}
							
		}
		
	}
	

	return TRUE;
}

BOOL _XWindow_Email::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !CheckMousePosition() )	return FALSE;

	if( m_EmailTextScrollBar )
	{
		if( m_SelectedTab != _XEMAILTAB_SEND )
		{
			if( m_EmailTextScrollBar->ProcessWheelMessage( zDelta ) )
			{
				return TRUE;
			}
			
			if( CheckMousePosition() )
			{
				int MoveDelta  = (FLOAT)zDelta / 40;			
				
				if( zDelta > 0 )
				{
					m_EmailTextScrollBar->LineUp( abs(MoveDelta) );
				}
				else
				{
					m_EmailTextScrollBar->LineDown( abs(MoveDelta) );
				}
				return TRUE;				
			}
		}
	}	

	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_Email::MoveWindow(int X, int Y)
{
	
	_XWindow::MoveWindow(X, Y);
	if(m_SelectedTab == _XEMAILTAB_RECEIVE || m_SelectedTab == _XEMAILTAB_KEEP)
	{
		m_EmailTextScrollBar->MoveWindow( m_WindowPosition.x + 165, m_WindowPosition.y + 56 );
	}
	else
	{	
		int namepos = 35;				
		m_ReceiverIMEControl.MoveWindow( m_WindowPosition.x + 61, m_WindowPosition.y + namepos  );
		m_EmailIMEControl.MoveWindow( m_WindowPosition.x + 10, 
			m_WindowPosition.y + 60 + ((m_EmailTextCurrentLinePos - m_EmailTextScrollPos)*17) );
		m_EmailTextScrollBar->MoveWindow( m_WindowPosition.x + 165, m_WindowPosition.y + 56 );
	}		
}

void _XWindow_Email::ShowWindow(BOOL show)
{
	_XWindow_SystemMsgDefault* SystemMsgWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);
	if( show )
	{
		if( m_bDockingMode )
		{
			SetEnableTopWindow(FALSE);	
			
			_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);	
			g_MainWindowManager.SetTopWindow(pMainmenuwindow);			

			if( SystemMsgWindow )
				SystemMsgWindow->SetDockingMode(TRUE);
		}
		else
		{
			SetEnableTopWindow(TRUE);				
			if( SystemMsgWindow )
				SystemMsgWindow->SetDockingMode(FALSE);
		}
	}
	else
	{
		if( SystemMsgWindow )
			SystemMsgWindow->SetDockingMode(FALSE);
	}
	_XWindow::ShowWindow(show);
}

void _XWindow_Email::ProcessDrag( _XGUIObject*& pfocusobject )
{
	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	BOOL changedmousecursor = FALSE;
	
	if( mousestate && scrnpos )
	{			
		if( !pfocusobject )
		{
			if( CheckMousePosition() )
			{	
				if( !CheckIMEBoxArea() ) // 이메일 내용 입력 박스가 아닐때 
				{
					if( scrnpos->x < m_WindowPosition.x+_XDEF_EMAILRESIZETRACKSIZE )
					{	
						if( scrnpos->z < m_WindowPosition.y+_XDEF_EMAILRESIZETRACKSIZE )
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;								
								m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
							}											
						}
						else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_EMAILRESIZETRACKSIZE )
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;								
								m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y+m_WindowSize.cy - scrnpos->z;
							}													
						}
						else
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;								
								m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
							}
						}

					}
					else if( scrnpos->x >= m_WindowPosition.x+m_WindowSize.cx-_XDEF_EMAILRESIZETRACKSIZE )
					{	
						if( scrnpos->z < m_WindowPosition.y+_XDEF_EMAILRESIZETRACKSIZE )
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;								
								m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
							}													
						}
						else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_EMAILRESIZETRACKSIZE )
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;								
								m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y+m_WindowSize.cy - scrnpos->z;
							}							
						}
						else
						{
							if( mousestate->bButton[0] )
							{
								pfocusobject = (_XGUIObject*)this;								
								m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;							
							}
						}
					}
					
					if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_EMAILRESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;							
							m_DragStartPos.y = m_WindowPosition.y + m_WindowSize.cy - scrnpos->z;
						}
					}					
					else
					{		
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							if( (m_WindowMoveMode != _XWMOVE_FIXED) && !m_Dragging )
							{					
								m_Dragging = TRUE;
								m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
								m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
							}
						}					
					}
				}
			} // if( CheckMousePosition() ) end
		}


		if( !mousestate->bButton[0] )
		{
			m_Dragging = FALSE;	
			m_DragStartPos.x = m_DragStartPos.y = 0;			
			
			if( m_bDockingMode )
			{
				MoveWindow( gnWidth - (1024 - 841), gnHeight - 48 - m_WindowSize.cy  );
				
			}

			if( CheckMousePosition() )
			{	
				if( pfocusobject == (_XGUIObject*)this )
				{
					if( m_CommandID )
					{						
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), (LPARAM)gHWnd);
						mousestate->bButton[0] = 0;
					}						
					pfocusobject = NULL;
				}		
			}
			if( pfocusobject == (_XGUIObject*)this )
			{
				pfocusobject = NULL;
			}					
		}
		else if( pfocusobject == (_XGUIObject*)this )
		{
			if( m_Dragging )
			{				
				int newxpos = scrnpos->x + m_DragStartPos.x;
				int newypos = scrnpos->z + m_DragStartPos.y;
				
				if( newxpos < 0	) newxpos = 0;
				if( newypos < 0	) newypos = 0;				
				if( newxpos+m_WindowSize.cx >= gnWidth ) newxpos = gnWidth - m_WindowSize.cx;
				if( newypos+m_WindowSize.cy >= gnHeight) newypos = gnHeight- m_WindowSize.cy;
				
				
				if( newxpos+m_WindowSize.cx > ( gnWidth - 100) )
				{
					if( newypos+m_WindowSize.cy > ( gnHeight - 90 ) )
					{
						if( !m_bDockingMode )
						{
							// 도킹모드일때 
							SetDockingMode(TRUE);							
							ResizeWindow( gnWidth - (1024 - 841), gnHeight - 48 - m_WindowSize.cy - 18 ,gnWidth, gnHeight- 48 );	
							MoveWindow( gnWidth - (1024 - 841), gnHeight - 48 - m_WindowSize.cy  );
							
							m_Dragging = FALSE;	
							m_DragStartPos.x = m_DragStartPos.y = 0;							
							
							if( CheckMousePosition() )
							{	
								if( pfocusobject == (_XGUIObject*)this )
								{
									if( m_CommandID )
									{						
										PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), (LPARAM)gHWnd);
										mousestate->bButton[0] = 0;
									}						
									pfocusobject = NULL;
								}		
							}
							if( pfocusobject == (_XGUIObject*)this )
							{
								pfocusobject = NULL;
							}				

						}	
						else
						{
							switch( m_WindowMoveMode )
							{
							case _XWMOVE_FREE :
								WindowPosChanging( newxpos, newypos );
								MoveWindow( newxpos, newypos );
								break;
							case _XWMOVE_HORIZ :
								WindowPosChanging( newxpos, (int&)m_WindowPosition.y );
								MoveWindow( newxpos, this->m_WindowPosition.y );
								break;
							case _XWMOVE_VERT :								
								WindowPosChanging( (int&)this->m_WindowPosition.x, newypos );
								MoveWindow( this->m_WindowPosition.x, newypos );
								break;
							}				
							
						}
						
					}	
					else
					{
						if( m_bDockingMode )
						{
							SetDockingMode(FALSE);
							ResizeWindow( m_WindowPosition.x, m_WindowPosition.y,
								m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy - 18  );	
						}
						else
						{
							SetDockingMode(FALSE);
						}
					}		
				}
				else
				{ 
					if( m_bDockingMode )
					{
						SetDockingMode(FALSE);
						ResizeWindow( m_WindowPosition.x, m_WindowPosition.y 
							,m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy - 18  );	
					}
					else
					{
						SetDockingMode(FALSE);
					}
				}

				
				if( !m_bDockingMode )
				{
					switch( m_WindowMoveMode )
					{
					case _XWMOVE_FREE :
						WindowPosChanging( newxpos, newypos );
						MoveWindow( newxpos, newypos );
						break;
					case _XWMOVE_HORIZ :
						WindowPosChanging( newxpos, (int&)m_WindowPosition.y );
						MoveWindow( newxpos, this->m_WindowPosition.y );
						break;
					case _XWMOVE_VERT :								
						WindowPosChanging( (int&)this->m_WindowPosition.x, newypos );
						MoveWindow( this->m_WindowPosition.x, newypos );
						break;
					}				
				}
				
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PICKUP_CLICK );
					
			}
		}
	}

}

#define _XDEF_EMAILWINDOWMINSIZE_WIDTH		183
#define _XDEF_EMAILWINDOWMINSIZE_HEIGHT		270

#define _XDEF_EMAILWINDOWMAXSIZE_WIDTH		183
#define _XDEF_EMAILWINDOWMAXSIZE_HEIGHT		288 

void _XWindow_Email::ResizeWindow( int left, int top, int right, int bottom )
{
	if( right-left < _XDEF_EMAILWINDOWMINSIZE_WIDTH && bottom-top < _XDEF_EMAILWINDOWMINSIZE_HEIGHT ) return;
	if( right-left > _XDEF_EMAILWINDOWMAXSIZE_WIDTH && bottom-top > _XDEF_EMAILWINDOWMAXSIZE_HEIGHT ) return;
	
	if( right-left > _XDEF_EMAILWINDOWMAXSIZE_WIDTH )
	{
		if( left == m_WindowPosition.x )
		{
			right = m_WindowPosition.x + _XDEF_EMAILWINDOWMAXSIZE_WIDTH;
		}
		else
		{
			left = m_WindowPosition.x + m_WindowSize.cx - _XDEF_EMAILWINDOWMAXSIZE_WIDTH;
		}
	}
	else if( right-left < _XDEF_EMAILWINDOWMINSIZE_WIDTH )
	{
		if( left == m_WindowPosition.x )
		{
			right = m_WindowPosition.x + _XDEF_EMAILWINDOWMINSIZE_WIDTH;
		}
		else
		{
			left = m_WindowPosition.x + m_WindowSize.cx - _XDEF_EMAILWINDOWMINSIZE_WIDTH;
		}
	}
	

	m_WindowPosition.x = left;
	m_WindowPosition.y = top;
	
	m_WindowSize.cx = right-left;
	m_WindowSize.cy = bottom-top;
	
	m_solidborderstatic.MoveWindow( left, top, right, bottom-1 );
	
	if( m_bDockingMode )
	{			
		m_imageEmailTitleBar->MoveWindow(m_WindowPosition.x , m_WindowPosition.y);// 전서구 타이틀 바
		m_imageEmailDisplayBoxUpBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + 54);// 전서구 내용표시 박스 위쪽 바탕 
		m_imageEmailDisplayBoxLeftSideBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + 57);// 전서구 왼쪽 사이드 이미지
		m_imageEmailDisplayBoxRightSideBorder->MoveWindow(m_WindowPosition.x + 163 , m_WindowPosition.y + 57);// 전서구 오른쪽 사이드 이미지
		m_imageEmailDisplayBoxDownBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + 194);// 전서구 내용표시 박스 아래쪽 바탕
		m_imageEmailDownBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + 215 );// 전서구 창 아래쪽  바탕
		m_imageEmailDockingBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + m_WindowSize.cy - 21);// 전서구 메인 메뉴창에 도킹했을때 생성되는 바탕
		m_imageEmailDownEndLine->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + m_WindowSize.cy - 3);// 전서구 창 아래쪽 끝 라인 
		m_imageEmailCountBoxBorder->MoveWindow(m_WindowPosition.x + 9  , m_WindowPosition.y + 217);// 전서구 보관된 편지 표시 창 박스
		
		
		m_btnEmailHelp->MoveWindow( m_WindowPosition.x + 153 , m_WindowPosition.y + 2 );// 전서구 창 도움 버튼
		m_btnEmailClose->MoveWindow( m_WindowPosition.x + 166 , m_WindowPosition.y + 2 );// 전서구 창 닫기 버튼

		m_TabButtonKeepingBox->MoveWindow(m_WindowPosition.x + 5 , m_WindowPosition.y + 217);// 전서구 탭버튼 보관함
		m_TabButtonReceiveBox->MoveWindow(m_WindowPosition.x + 63 , m_WindowPosition.y + 217);// 전서구 탭버튼 수신함
		m_TabButtonSendBox->MoveWindow(m_WindowPosition.x + 121 , m_WindowPosition.y + 217);// 전서구 탭버튼 발신함

		m_btnPrev->MoveWindow(m_WindowPosition.x + 35 , m_WindowPosition.y + 201);// 저장된 편지 이전 보기 버튼
		m_btnNext->MoveWindow(m_WindowPosition.x + 83 , m_WindowPosition.y + 201);// 저장된 편지 다음 보기 버튼 
		m_btnKeepingMailDelete->MoveWindow(m_WindowPosition.x + 121 , m_WindowPosition.y + 196);// 저장된 편지 삭제하기 버튼

		m_btnReceiveReply->MoveWindow(m_WindowPosition.x + 7 , m_WindowPosition.y + 196);// 받은 편지 답장하기 버튼
		m_btnReceiveSave->MoveWindow(m_WindowPosition.x + 64 , m_WindowPosition.y + 196);// 받은 편지 보관하기 버튼
		m_btnReceiveDelete->MoveWindow(m_WindowPosition.x + 121 , m_WindowPosition.y + 196);// 받은 편지 삭제하기 버튼
		m_btnSendDelete->MoveWindow(m_WindowPosition.x + 7 , m_WindowPosition.y + 196);// 보낼 편지 내용 지우기 버튼
		m_btnSendCancle->MoveWindow(m_WindowPosition.x + 64 , m_WindowPosition.y + 196);// 보낼 편지 취소 버튼
		m_btnSend->MoveWindow(m_WindowPosition.x + 121 , m_WindowPosition.y + 196);// 보낼 편지 보내기 버튼
		
		m_imageEmailDockingBorder->ShowWindow(TRUE);
		m_imageEmailDownEndLine->ShowWindow(FALSE);

		ChangeTab( m_SelectedTab );

		SetEnableTopWindow(FALSE);	

		_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);	
		g_MainWindowManager.SetTopWindow(pMainmenuwindow);		

	}
	else
	{	
		m_imageEmailTitleBar->MoveWindow(m_WindowPosition.x , m_WindowPosition.y);// 전서구 타이틀 바
		m_imageEmailDisplayBoxUpBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + 54);// 전서구 내용표시 박스 위쪽 바탕 
		m_imageEmailDisplayBoxLeftSideBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + 57);// 전서구 왼쪽 사이드 이미지
		m_imageEmailDisplayBoxRightSideBorder->MoveWindow(m_WindowPosition.x + 163 , m_WindowPosition.y + 57);// 전서구 오른쪽 사이드 이미지
		m_imageEmailDisplayBoxDownBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + 194);// 전서구 내용표시 박스 아래쪽 바탕
		m_imageEmailDownBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + 215 );// 전서구 창 아래쪽  바탕
		m_imageEmailDockingBorder->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + m_WindowSize.cy - 21);// 전서구 메인 메뉴창에 도킹했을때 생성되는 바탕
		m_imageEmailDownEndLine->MoveWindow(m_WindowPosition.x , m_WindowPosition.y + m_WindowSize.cy - 3);// 전서구 창 아래쪽 끝 라인 
		m_imageEmailCountBoxBorder->MoveWindow(m_WindowPosition.x + 9  , m_WindowPosition.y + 217);// 전서구 보관된 편지 표시 창 박스
		
		m_btnEmailHelp->MoveWindow( m_WindowPosition.x + 153 , m_WindowPosition.y + 2 );// 전서구 창 도움 버튼
		m_btnEmailClose->MoveWindow( m_WindowPosition.x + 166 , m_WindowPosition.y + 2 );// 전서구 창 닫기 버튼
		
		m_TabButtonKeepingBox->MoveWindow(m_WindowPosition.x + 5 , m_WindowPosition.y + 217);// 전서구 탭버튼 보관함
		m_TabButtonReceiveBox->MoveWindow(m_WindowPosition.x + 63 , m_WindowPosition.y + 217);// 전서구 탭버튼 수신함
		m_TabButtonSendBox->MoveWindow(m_WindowPosition.x + 121 , m_WindowPosition.y + 217);// 전서구 탭버튼 발신함
		
		m_btnPrev->MoveWindow(m_WindowPosition.x + 35 , m_WindowPosition.y + 201);// 저장된 편지 이전 보기 버튼
		m_btnNext->MoveWindow(m_WindowPosition.x + 83 , m_WindowPosition.y + 201);// 저장된 편지 다음 보기 버튼 
		m_btnKeepingMailDelete->MoveWindow(m_WindowPosition.x + 121 , m_WindowPosition.y + 196);// 저장된 편지 삭제하기 버튼
		
		m_btnReceiveReply->MoveWindow(m_WindowPosition.x + 7 , m_WindowPosition.y + 196);// 받은 편지 답장하기 버튼
		m_btnReceiveSave->MoveWindow(m_WindowPosition.x + 64 , m_WindowPosition.y + 196);// 받은 편지 보관하기 버튼
		m_btnReceiveDelete->MoveWindow(m_WindowPosition.x + 121 , m_WindowPosition.y + 196);// 받은 편지 삭제하기 버튼
		m_btnSendDelete->MoveWindow(m_WindowPosition.x + 7 , m_WindowPosition.y + 196);// 보낼 편지 내용 지우기 버튼
		m_btnSendCancle->MoveWindow(m_WindowPosition.x + 64 , m_WindowPosition.y + 196);// 보낼 편지 취소 버튼
		m_btnSend->MoveWindow(m_WindowPosition.x + 121 , m_WindowPosition.y + 196);// 보낼 편지 보내기 버튼
		
		m_imageEmailDockingBorder->ShowWindow(FALSE);
		m_imageEmailDownEndLine->ShowWindow(TRUE);

		ChangeTab( m_SelectedTab );		

		SetEnableTopWindow(TRUE);	
		
		_XWindow_MainMenu* pMainmenuwindow = (_XWindow_MainMenu*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINMENUWINDOW);	
		g_MainWindowManager.SetTopWindow(pMainmenuwindow);		
	}
}


void _XWindow_Email::ChangeTab(_XEmailTab selectedtab)
{
	m_SelectedTab = selectedtab;
	ResetEmailText();

	if(m_SelectedTab == _XEMAILTAB_RECEIVE)
	{		
		m_TabButtonReceiveBox->SetCheck(TRUE);
		m_TabButtonSendBox->SetCheck(FALSE);
		m_TabButtonKeepingBox->SetCheck(FALSE);
				
		m_imageEmailCountBoxBorder->ShowWindow(FALSE);
		m_btnNext->ShowWindow(FALSE);
		m_btnPrev->ShowWindow(FALSE);
		m_btnKeepingMailDelete->ShowWindow(FALSE);

		m_btnReceiveReply->ShowWindow(TRUE);
		m_btnReceiveSave->ShowWindow(TRUE);
		m_btnReceiveDelete->ShowWindow(TRUE);
		
		m_btnSendDelete->ShowWindow(FALSE);
		m_btnSend->ShowWindow(FALSE);
		m_btnSendCancle->ShowWindow(FALSE);
		
		if( m_EmailIMEControl.GetFocus() || m_ReceiverIMEControl.GetFocus() ) _XIMEKERNEL.SetFocus( NULL ); 
		m_EmailIMEControl.ShowWindow(FALSE);
		m_ReceiverIMEControl.ShowWindow(FALSE);

		if( m_indexEmailReceiveText > 0 && m_indexEmailReceiveText <= m_EmailReceiveTextTotalCount )
			g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, m_EmailListReceiveBox[m_indexEmailReceiveText-1].uiIndex );
	}
	else if( m_SelectedTab == _XEMAILTAB_KEEP)
	{
		m_TabButtonReceiveBox->SetCheck(FALSE);
		m_TabButtonSendBox->SetCheck(FALSE);
		m_TabButtonKeepingBox->SetCheck(TRUE);
		
		m_imageEmailCountBoxBorder->ShowWindow(TRUE);
		m_btnNext->ShowWindow(TRUE);
		m_btnPrev->ShowWindow(TRUE);
		m_btnKeepingMailDelete->ShowWindow(TRUE);
		
		m_btnReceiveReply->ShowWindow(FALSE);
		m_btnReceiveSave->ShowWindow(FALSE);
		m_btnReceiveDelete->ShowWindow(FALSE);
		
		m_btnSendDelete->ShowWindow(FALSE);
		m_btnSend->ShowWindow(FALSE);
		m_btnSendCancle->ShowWindow(FALSE);
		
		if( m_EmailIMEControl.GetFocus() || m_ReceiverIMEControl.GetFocus() ) _XIMEKERNEL.SetFocus( NULL ); 
		m_EmailIMEControl.ShowWindow(FALSE);
		m_ReceiverIMEControl.ShowWindow(FALSE);
		
		if( m_indexEmailText > 0 && m_indexEmailText <= m_EmailTextTotalCount )
			g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, m_EmailList[m_indexEmailText-1].uiIndex );
	}
	else if( m_SelectedTab == _XEMAILTAB_SEND )
	{		
		m_TabButtonReceiveBox->SetCheck(FALSE);
		m_TabButtonSendBox->SetCheck(TRUE);
		m_TabButtonKeepingBox->SetCheck(FALSE);
		
		m_imageEmailCountBoxBorder->ShowWindow(FALSE);
		m_btnNext->ShowWindow(FALSE);
		m_btnPrev->ShowWindow(FALSE);
		m_btnKeepingMailDelete->ShowWindow(FALSE);
		
		m_btnReceiveReply->ShowWindow(FALSE);
		m_btnReceiveSave->ShowWindow(FALSE);
		m_btnReceiveDelete->ShowWindow(FALSE);
		
		m_btnSendDelete->ShowWindow(TRUE);
		m_btnSend->ShowWindow(TRUE);
		m_btnSendCancle->ShowWindow(TRUE);
		
		int namepos = 35;				
		m_ReceiverIMEControl.ClearBuffer();
		m_ReceiverIMEControl.MoveWindow( m_WindowPosition.x + 61, m_WindowPosition.y + namepos );
		m_ReceiverIMEControl.ShowWindow(TRUE);
		m_ReceiverIMEControl.SetFocus();
		
		m_EmailIMEControl.MoveWindow( m_WindowPosition.x + 10, 
			m_WindowPosition.y + 60 + ((m_EmailTextCurrentLinePos - m_EmailTextScrollPos)*17) );
		m_EmailIMEControl.ShowWindow(TRUE);
	}
}


BOOL _XWindow_Email::CheckIMEBoxArea()
{	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 9) &&
		scrnpos->x < (m_WindowPosition.x + 181 ) &&
		scrnpos->z > (m_WindowPosition.y + 57) &&
		scrnpos->z < (m_WindowPosition.y + 194))
		return TRUE;

	return FALSE;
}

BOOL _XWindow_Email::CheckEmailScrollBarArea()
{	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 165) &&
		scrnpos->x < (m_WindowPosition.x + 177 ) &&
		scrnpos->z > (m_WindowPosition.y + 56) &&
		scrnpos->z < (m_WindowPosition.y + 192))
		return TRUE;
	
	return FALSE;
}

BOOL _XWindow_Email::CheckReceiverIMEBoxArea()
{	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(scrnpos->x > (m_WindowPosition.x + 57) &&
		scrnpos->x < (m_WindowPosition.x + 211 ) &&
		scrnpos->z > (m_WindowPosition.y + 33) &&
		scrnpos->z < (m_WindowPosition.y + 49))
		return TRUE;
	
	return FALSE;
}

void _XWindow_Email::ResetEmailText()
{	
	m_EmailIMEControl.ClearBuffer();
	memset(m_EmailDisplayText, 0, sizeof(TCHAR) * 128 * 256);
	m_EmailTextLineTotalCount = 1;
	m_EmailTextCurrentLinePos = 0;
	m_EmailTextLength = 0;
	//m_EmailTextScrollBar->SetTotalLineCount(9);
	m_EmailTextScrollBar->SetLineScrollPos(0);
	m_EmailTextScrollBar->UpdateData();		
	
}

void _XWindow_Email::CalculateEmailLength()
{
	m_EmailTextLength = 0;
	for( int i = 0; i < m_EmailTextLineTotalCount ; i++)
	{
		m_EmailTextLength += strlen( m_EmailDisplayText[i] );
	}

	m_EmailTextLength += (m_EmailTextLineTotalCount - 1);
}

void _XWindow_Email::ResetEmailList()
{
	for( int i = 0; i < 20; i++ )
	{
		m_EmailList[i].uiIndex = 0;
		m_EmailList[i].check = FALSE;
	}

}

void _XWindow_Email::ResetEmailListReceiveBox()
{
	for( int i = 0; i < 20; i++ )
	{
		m_EmailListReceiveBox[i].uiIndex = 0;
		m_EmailListReceiveBox[i].check = FALSE;
	}
	
}


void _XWindow_Email::SetEmailText(LPSTR emailtext)
{
	ResetEmailText();
	if( !emailtext ) return;
	if( strlen(emailtext) <= 0 ) return;
		
	// String splitting....
	_XStringSplitInfo splitinfo;	

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->SplitString(emailtext, 100, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
	}
	else
	{
		g_XBaseFont->SplitStringByCR(emailtext, splitinfo);	
	}
	
	memset(m_EmailDisplayText, 0, sizeof(TCHAR) * 128 * 256);
	
	TCHAR datemessage[128];
	memset( datemessage,0,sizeof(TCHAR)*128 );
	
	strcpy( datemessage, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MESSENGER_2478, 
														   &m_EmailYear, &m_EmailMonth, &m_EmailDay, &m_EmailHour, &m_EmailMinute ) );
	
	strcpy(m_EmailDisplayText[0], datemessage);	
	strcpy(m_EmailDisplayText[1], _T("========================="));
	
	for( int i = 0; i < splitinfo.splitcount; i++ )
	{			
		strncpy( m_EmailDisplayText[i+2], splitinfo.splitpoint[i], splitinfo.splitlength[i] );	
	}
	
	if( m_EmailTextScrollBar )
	{		
		m_EmailTextScrollBar->MoveWindow( m_WindowPosition.x + 165, m_WindowPosition.y + 56  );
	//	if( splitinfo.splitcount+2 > 8 )
	//	{
			m_EmailTextScrollBar->SetTotalLineCount( splitinfo.splitcount+2 );
	//	}
	//	else
	//	{
		//	m_EmailTextScrollBar->SetTotalLineCount( 9 );
	//	}
		m_EmailTextScrollBar->SetLineScrollPos(0);
		m_EmailTextScrollBar->UpdateData();		
	}								
}

void _XWindow_Email::DeleteEmailListReceiveBox(int emailindex)
{
	for(int i = emailindex; i < m_EmailReceiveTextTotalCount; i++)
	{
		if( i != m_EmailReceiveTextTotalCount-1 )
		{
			m_EmailListReceiveBox[i].uiIndex = m_EmailListReceiveBox[i+1].uiIndex;
			m_EmailListReceiveBox[i].check = m_EmailListReceiveBox[i+1].check;
		}
		else
		{
			m_EmailListReceiveBox[i].uiIndex = 0;
			m_EmailListReceiveBox[i].check = FALSE;
		}
	}
	
	if( m_EmailReceiveTextTotalCount > 0 )
		m_EmailReceiveTextTotalCount--;
	
	if( m_EmailReceiveTextTotalCount == 0 )
	{
		m_indexEmailReceiveText = 0;
		ResetEmailText();
	}
	else if( emailindex ==  m_EmailReceiveTextTotalCount )
	{
		g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, m_EmailListReceiveBox[emailindex-1].uiIndex );
		m_indexEmailReceiveText--;
	}
	else
	{
		g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, m_EmailListReceiveBox[emailindex].uiIndex );
	}
	
	
	
}

void _XWindow_Email::DeleteEmailList(int emailindex)
{
	for(int i = emailindex; i < m_EmailTextTotalCount; i++)
	{
		if( i != m_EmailTextTotalCount-1 )
		{
			m_EmailList[i].uiIndex = m_EmailList[i+1].uiIndex;
			m_EmailList[i].check = m_EmailList[i+1].check;
		}
		else
		{
			m_EmailList[i].uiIndex = 0;
			m_EmailList[i].check = FALSE;
		}
	}
	m_EmailTextTotalCount--;
	
	if( m_EmailTextTotalCount == 0 )
	{
		m_indexEmailText = 0;
		ResetEmailText();
	}
	else if( emailindex ==  m_EmailTextTotalCount )
	{
		g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, m_EmailList[emailindex-1].uiIndex );
		m_indexEmailText--;
	}
	else
	{
		g_NetworkKernel.SendMessengerPacket( MMSG_NO_MAIL_TEXT_REQ_C_S, m_EmailList[emailindex].uiIndex );
	}



}

void _XWindow_Email::ResetTempEmailInfo()
{
	for( int i = 0; i < 20; i++)
	{
		m_TempEmailInfo[i].uiIndex		= 0;
		m_TempEmailInfo[i].cStrLen		= 0;
		m_TempEmailInfo[i].date.year	= 0;
		m_TempEmailInfo[i].date.month	= 0;
		m_TempEmailInfo[i].date.day		= 0;
		m_TempEmailInfo[i].date.hour	= 0;
		m_TempEmailInfo[i].date.minute	= 0;
		
		memset( m_TempEmailInfo[i].cFrom, 0, sizeof(TCHAR)*13 );
		memset( m_TempEmailInfo[i].cText, 0, sizeof(TCHAR)*101 );
	}

}

void _XWindow_Email::SetDockingMode(BOOL flag)
{
	m_bDockingMode = flag;

	_XWindow_SystemMsgDefault* systemWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SYSTEMMSGDEFAULT);	
	if( systemWindow )
		systemWindow->SetDockingMode(flag);
}