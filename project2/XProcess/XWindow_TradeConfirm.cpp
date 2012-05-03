// XWindow_TradeConfirm.cpp: implementation of the _XWindow_TradeConfirm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "XWindow_TradeConfirm.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Inventory.h"
#include "xwindow_warehouse.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

extern unsigned long g_MoneyColor[6];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_TradeConfirmEditReturn( _XIMEContainer* pIMEContainer )
{
//	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
//	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();	
	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRADECONFIRM_OK, 0), (LPARAM)gHWnd); //2004.06.24->oneway48 insert
}

void __stdcall _XIMECallback_TradeConfirmEditEscape( _XIMEContainer* pIMEContainervoid )
{			
//	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
//	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();			
	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRADECONFIRM_CLOSE, 0), (LPARAM)gHWnd); //2004.06.24->oneway48 insert
}

// IME에 입력한 값이 변했을 때 호출
BOOL __stdcall _XIMECallback_TradeConfirmDataChange( _XIMEContainer* pIMEContainer, DWORD lastkey )
{
	TCHAR tempstring[128];
	DWORD tempcount = 0;
	memset( tempstring, 0, sizeof(TCHAR) * 128 );
	strcpy(tempstring, pIMEContainer->GetText());

	//tempcount = atol(tempstring);
	sscanf( tempstring, _T("%u"), &tempcount );
	
	if( tempcount > 0 )
	{
		_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_TRADECONFIRM );
		if( pTradeConfirm_Window )
		{
			if( pTradeConfirm_Window->GetShowStatus() )
			{
				if( pTradeConfirm_Window->m_TradeMode == _XWAREHOUSE_TOWAREHOUSE_MONEY || pTradeConfirm_Window->m_TradeMode == _XDROP_MONEY || 
					pTradeConfirm_Window->m_TradeMode == _XPCTRADE_TOTRADE_MONEY || pTradeConfirm_Window->m_TradeMode == _XCONTRIBUTION_TOCONTRIBUTION_MONEY )
				{
					if( tempcount > g_pLocalUser->m_Money )
					{
						tempcount = g_pLocalUser->m_Money;						
						//itoa(tempcount, tempstring, 10);	
						sprintf( tempstring, _T("%u"), tempcount );
						int length = strlen(tempstring);
						pIMEContainer->ClearBuffer();
						pIMEContainer->AddString(tempstring, length );
					}
				}
				else if( pTradeConfirm_Window->m_TradeMode == _XWAREHOUSE_FROMWAREHOUSE_MONEY )
				{
					if( tempcount > g_pLocalUser->m_WarehouseMoney )
					{
						tempcount = g_pLocalUser->m_WarehouseMoney;
						//itoa(tempcount, tempstring, 10);
						sprintf( tempstring, _T("%u"), tempcount );
						int length = strlen(tempstring);
						pIMEContainer->ClearBuffer();
						pIMEContainer->AddString(tempstring, length );					
					}
				}
				else if( pTradeConfirm_Window->m_TradeMode == _XPCTRADE_FROMTRADE_MONEY )
				{
					_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PCTRADE );
					if( pPCTrade_Window )
					{
						if( pPCTrade_Window->GetShowStatus() )
						{
							if( tempcount > pPCTrade_Window->m_SelfMoney )
							{
								tempcount = pPCTrade_Window->m_SelfMoney;
								//itoa(tempcount, tempstring, 10);
								sprintf( tempstring, _T("%u"), tempcount );
								int length = strlen(tempstring);
								pIMEContainer->ClearBuffer();
								pIMEContainer->AddString(tempstring, length );
							}
						}
					}					
				}
				else if( pTradeConfirm_Window->m_TradeMode == _XWAREHOUSE_FROMWAREHOUSE_ITEM )
				{
					_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_WAREHOUSE );
					if( tempcount > g_pLocalUser->m_WarehouseItemList[pWarehouse_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_ucCount() )
					{
						tempcount = g_pLocalUser->m_WarehouseItemList[pWarehouse_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_ucCount();
						//itoa(tempcount, tempstring, 10);
						sprintf( tempstring, _T("%u"), tempcount );
						int length = strlen(tempstring);
						pIMEContainer->ClearBuffer();
						pIMEContainer->AddString(tempstring, length );
					}
				}
				else if( pTradeConfirm_Window->m_TradeMode == _XPCTRADE_FROMTRADE_ITEM )
				{
					_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PCTRADE );

					if( pPCTrade_Window )
					{
						if( pPCTrade_Window->m_MouseSlotItem.m_InventoryNumber >= 0 && pPCTrade_Window->m_MouseSlotItem.m_InventoryNumber < 10 )
						{
							if( tempcount > pPCTrade_Window->m_MouseSlotItem.Get_m_ucCount() )
							{
								tempcount = pPCTrade_Window->m_MouseSlotItem.Get_m_ucCount();
								//itoa(tempcount, tempstring, 10);
								sprintf( tempstring, _T("%u"), tempcount );
								int length = strlen(tempstring);
								pIMEContainer->ClearBuffer();
								pIMEContainer->AddString(tempstring, length );
							}
						}
					}
				}
				else if( pTradeConfirm_Window->m_TradeMode > 10 )
				{
					if( g_pInventory_Window )
					{
						if( g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber > 0 )
						{
							if( tempcount > g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_ucCount() )				
							{
								tempcount = g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_ucCount();
								//itoa(tempcount, tempstring, 10);
								sprintf( tempstring, _T("%u"), tempcount );
								int length = strlen(tempstring);
								pIMEContainer->ClearBuffer();
								pIMEContainer->AddString(tempstring, length );
							}
						}						
					}					
				}
				else
				{
					if( tempcount > 255 )
					{
						tempcount = 255;
						//itoa(tempcount, tempstring, 10);
						sprintf( tempstring, _T("%u"), tempcount );
						int length = strlen(tempstring);
						pIMEContainer->ClearBuffer();
						pIMEContainer->AddString(tempstring, length );										
					}
				}
			}
		}
	}
	return TRUE;
}

_XWindow_TradeConfirm::_XWindow_TradeConfirm()
{
	m_AllButton = NULL;// 2004.05.17->oneway48 insert	
	m_OKButton = NULL;
	m_CancelButton = NULL;
	
	Reset();
}

_XWindow_TradeConfirm::~_XWindow_TradeConfirm()
{
	DestroyWindow();
}

void _XWindow_TradeConfirm::Reset(void)
{
	m_Price = 0;
	m_TradeCount = 0;
	
	m_SelectedCategory1 = 0;
	m_SelectedItemID = 0;
	m_InventoryNumber = 0;
	
	memset(m_CountString, 0, sizeof(TCHAR)*64);
	
	m_PCTradeSlotIndex = 0;
	m_PersonalStoreSlotIndex = 0;
	
	m_TradeMode = _XTRADE_NONE;
}

BOOL _XWindow_TradeConfirm::Initialize(void)
{
	int confirmimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Trade.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 208, 88, &g_MainInterfaceTextureArchive, confirmimageindex );	
	pTitleBar->SetClipRect( 0, 168, 207, 255 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pEditImage = new _XImageStatic;
	pEditImage->Create(29, 35, 127, 53,
		&g_MainInterfaceTextureArchive,
		g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga"));
	pEditImage->SetClipRect(7, 201, 104, 219);
	InsertChildObject(pEditImage);

	int buttonindex = g_MainInterfaceTextureArchive.FindResource("MI_Trade.tga");

	_XBTN_STRUCTURE allbtnstruct = 
	{
		TRUE, {128, 36}, {55, 17},
		_XDEF_TRADECONFIRM_ALL,
		buttonindex, buttonindex, buttonindex,
		&g_MainInterfaceTextureArchive
	};
	
	m_AllButton = new _XButton;
	m_AllButton->Create(allbtnstruct);
	m_AllButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 78, 244, 95);
	m_AllButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 98, 240, 115);
	m_AllButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 118, 240, 135);
	m_AllButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_AllButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_TRADECONFIRM_ALL);	// "전부입력"
	InsertChildObject(m_AllButton);
	
	_XBTN_STRUCTURE okbtnstruct = 
	{
		TRUE, {52, 59}, {52, 22},
		_XDEF_TRADECONFIRM_OK,
		buttonindex, buttonindex, buttonindex,
		&g_MainInterfaceTextureArchive
	};
	m_OKButton = new _XButton;
	m_OKButton->Create(okbtnstruct);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 28, 240, 49);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 3, 240, 24);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 53, 240, 74);
	m_OKButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_OKButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);	// "확인"
	InsertChildObject(m_OKButton);

	_XBTN_STRUCTURE cancelbtnstruct = 
	{
		TRUE, {107, 59}, {52, 22},
			_XDEF_TRADECONFIRM_CLOSE,
			buttonindex, buttonindex, buttonindex,
			&g_MainInterfaceTextureArchive
	};
	m_CancelButton = new _XButton;
	m_CancelButton->Create(cancelbtnstruct);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 28, 240, 49);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 3, 240, 24);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 53, 240, 74);
	m_CancelButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 214, 15, 15));
	m_CancelButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);	// "취소"
	InsertChildObject(m_CancelButton);

	m_IMEControl.ResetContainer();
	m_IMEControl.MoveWindow(m_WindowPosition.x + 40, m_WindowPosition.y + 39);
	m_IMEControl.ResizeWindow(76, 12);
	m_IMEControl.ShowWindow(FALSE);
	m_IMEControl.SetMaxLength(10);	// 10억단위까지 표시
	m_IMEControl.SetFocus();
	m_IMEControl.SetAlignType( _XFONT_ALIGNTYPE_RIGHT );
	m_IMEControl.SetNumericEditMode( TRUE, TRUE );

	m_IMEControl.SetEscapeKeyCallback( _XIMECallback_TradeConfirmEditEscape );
	m_IMEControl.SetReturnKeyCallback( _XIMECallback_TradeConfirmEditReturn );
	m_IMEControl.SetDataChangeCallBack( &_XIMECallback_TradeConfirmDataChange );

	return TRUE;
}

void _XWindow_TradeConfirm::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_TradeConfirm::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	
	switch(m_TradeMode)
	{
	case _XTRADE_NONE :
		{
		}
		break;

	case _XNPCTRADE_BUY :
	case _XNPCTRADE_SELL :
		{
			if( m_Price == 0 )
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_COUNT) );	// _T("수량을 입력해 주십시오.")
			}
			else
			{
				//g_XBaseFont->Print(m_WindowPosition.x + 30, m_WindowPosition.y + 17, 1.0f, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_PRICE));
				TCHAR moneyStr[128];
				memset( moneyStr, 0, sizeof(moneyStr));
				sprintf(moneyStr, "%u", m_Price);
//				itoa(m_Price, moneyStr, 10 );
				int colorindex = 0;
				
				g_XBaseFont->SetColor(_XSC_INFORMATION);
				if( _XGameItem::GetMoneyColor(moneyStr, colorindex) )
					g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+17, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s %s", _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_PRICE), moneyStr );
				else
					g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_MONEY), m_Price);	// "가격 : %d 금"
			}
		}
		break;	
	case _XPCTRADE_TOTRADE_ITEM :		
	case _XPCTRADE_FROMTRADE_ITEM :
	case _XWAREHOUSE_TOWAREHOUSE_ITEM :
	case _XWAREHOUSE_FROMWAREHOUSE_ITEM :	
	case _XPERSONALSTORE_TOTRADE_ITEM :	
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_COUNT) );
		}
		break;
	case _XWAREHOUSE_TOWAREHOUSE_MONEY :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+8, 
					_XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_STOREMONEY), 198, _XFONT_ALIGNTYPE_CENTER, 1.0f);
			}
			else
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_STOREMONEY) );	// _T("입금할 금액을 적어주십시오.")
			}
		}
		break;
	case _XWAREHOUSE_FROMWAREHOUSE_MONEY :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->Puts_SeparateAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+8, 
					_XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_FINDMONEY), 198, _XFONT_ALIGNTYPE_CENTER, 1.0f);
			}
			else
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_FINDMONEY) ); 	// _T("출금할 금액을 적어주십시오.") 
			}
		}
		break;
	case _XDROP_ITEM :
	case _XDROP_WASTEBASKET_ITEM :		
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_DROPCOUNT) );	// _T("버릴 수량을 입력해주십시오.")
		}
		break;
	case _XPCTRADE_FROMTRADE_MONEY :
	case _XPCTRADE_TOTRADE_MONEY :
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_INPUTMONEY) );	// _T("금액을 입력해주십시오.")
		}
		break;	
	case _XDROP_MONEY :
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_DROPMONEY) );	// _T("버릴 금액을 입력해주십시오.")
		}
		break;
	case _XCONTRIBUTION_TOCONTRIBUTION_ITEM :
		{
			if( m_TradeCount == 0 )
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_COUNT) );	// _T("수량을 입력해 주십시오.")
			}
			else
			{
				g_XBaseFont->SetColor(_XSC_INFORMATION);
				g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_CONTRIBUTIONCOUNT), m_TradeCount);	// "%d개 기부"
			}			
		}
		break;
	case _XCONTRIBUTION_TOCONTRIBUTION_MONEY :
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y + 17, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TRADECONFIRM_CONTRIBUTIONMONEY) );	// _T("기부할 금액을 적어주십시오.")
		}
		break;
	}	

	g_XBaseFont->Flush();
	m_IMEControl.Draw();
}

BOOL _XWindow_TradeConfirm::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckIMEBoxArea())
				{
					m_IMEControl.SetFocus();
				}
			}
		}
	}

	strcpy(m_CountString, m_IMEControl.GetText());	
	
	//m_TradeCount = atoi(m_CountString);
	sscanf( m_CountString, _T("%u"), &m_TradeCount );

	//Author : 양희왕 //breif : 다른것도 가격을 표시 해줘야 한다. 세율도 적용 해준다 08/02/29
	//GenerateItemPrice();
	char itemtype = GetEqualItemType(m_SelectedCategory1);

	if(m_TradeMode == _XNPCTRADE_BUY)
	{
		m_Price = _XGameItem::GetItemPrice(itemtype, m_SelectedItemID);
		#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
		//세금
		u_int orgmoney = m_Price;

		FLOAT correction = 0.4f;
		int tvalue = orgmoney % 10;
		if(tvalue == 6)
		{
			correction = 0.5f;
		}
		u_int tax_money = static_cast<u_int>( ((float)m_Price*(float)(g_nMapTaxRate)/100.0f) + correction );	//<== 세금으로 걷는 돈.

		m_Price += tax_money;//하나의 가격이 나옴 
		#endif
		
	}
	else if(m_TradeMode == _XNPCTRADE_SELL)
	{
		m_Price = PriceCalc(); //안에서 /5 를 한다
		#ifdef _XDEF_CASTLEBATTLE_MAPTAXRATE
		//세금 ( 파는 가격에 세금을 책정..   일반 사는가격의 세금을 책정하지 않음.. 툴팁 계산이 그렇게 해서 따라함 )
		u_int orgmoney = m_Price;

		FLOAT correction = 0.4f;
		int tvalue = orgmoney % 10;
		if(tvalue == 6)
		{
			correction = 0.5f;
		}
		u_int tax_money = static_cast<u_int>( ((float)m_Price*(float)(g_nMapTaxRate)/100.0f) + correction );	//<== 세금으로 걷는 돈.

		m_Price -= tax_money;//하나의 가격이 나옴 
		#endif
	}
	
	m_Price = m_Price * m_TradeCount;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

BOOL _XWindow_TradeConfirm::CheckIMEBoxArea(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(scrnpos->x > (m_WindowPosition.x + 40) &&
		scrnpos->x < (m_WindowPosition.x + 40 + 76) &&
		scrnpos->z > (m_WindowPosition.y + 39) &&
		scrnpos->z < (m_WindowPosition.y + 39 + 12))
		return TRUE;

	return FALSE;
}

void _XWindow_TradeConfirm::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);

	m_IMEControl.MoveWindow(m_WindowPosition.x + 40, m_WindowPosition.y + 39);
}

void _XWindow_TradeConfirm::ShowWindow(BOOL show)
{
	if(!show)
	{
		Reset();
		m_IMEControl.ClearBuffer();
		_XIMEKERNEL.SetFocus( NULL ); // 2004.05.17->oneway48 insert		
	}
	else
	{
		m_pMyManager->SetTopWindow(this);
		m_IMEControl.SetFocus();
		if( g_pInventory_Window )
			g_pInventory_Window->SetTradeConfirmWindowPtr( this );
	}

	_XWindow::ShowWindow(show);
}

int	_XWindow_TradeConfirm::PriceCalc()
{
	int reslutPrice = 0;			
	float tempmoney = 0;
	char itemtype = GetEqualItemType(m_SelectedCategory1);
	
	if( m_SelectedItemID <= 0 )		return 0;	
	
	if( g_pLocalUser->m_UserItemList[m_InventoryNumber].Get_m_usMaxDur() > 0 )
	{
		float TempRet = (float)g_pLocalUser->m_UserItemList[m_InventoryNumber].Get_m_usCurDur() / 
						(float)g_pLocalUser->m_UserItemList[m_InventoryNumber].Get_m_usMaxDur();
		
		if(_XGameItem::GetItemPrice(itemtype, m_SelectedItemID) == 0)
		{
			tempmoney = 0;
		}
		else
		{
			tempmoney = TempRet * static_cast<float>((_XGameItem::GetItemPrice( itemtype, m_SelectedItemID)/5.0f ));
		}
	}
	else
	{
		if(_XGameItem::GetItemPrice(itemtype, m_SelectedItemID) == 0)
		{
			tempmoney = 0;
		}
		else
		{
			tempmoney = static_cast<float>(( _XGameItem::GetItemPrice( itemtype, m_SelectedItemID)/5.0f ));
		}
	}
	
	if( tempmoney > 0.0f && tempmoney < 1.0f )
	{
		reslutPrice = 1;
	}
	else
	{
		reslutPrice = static_cast<int>( tempmoney );
	}	

	return reslutPrice;
}

void _XWindow_TradeConfirm::GenerateItemPrice(void)
{
#ifdef _XTS_ITEM_OPTIMIZATION
	char itemtype = GetEqualItemType(m_SelectedCategory1);
	if( itemtype != -1 )
	{
		switch(itemtype)
		{
		case _XGI_FC_WEAPON :
			{
				_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_SelectedCategory1);				
				if( WeaponItem )
				{
					if(m_TradeMode == _XNPCTRADE_BUY)
						m_Price = WeaponItem[m_SelectedItemID].uiPrice * m_TradeCount;
					else if(m_TradeMode == _XNPCTRADE_SELL)
						m_Price = (WeaponItem[m_SelectedItemID].uiPrice/5) * m_TradeCount;
				}
			}
			break;
		case _XGI_FC_CLOTHES :
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_SelectedCategory1);
				if( ClothesItem )
				{
					if(m_TradeMode == _XNPCTRADE_BUY)
						m_Price = ClothesItem[m_SelectedItemID].uiPrice * m_TradeCount;
					else if(m_TradeMode == _XNPCTRADE_SELL)
						m_Price = (ClothesItem[m_SelectedItemID].uiPrice/5) * m_TradeCount;
				}
			}
			break;			
		case _XGI_FC_BOOK :
			{
				_XGI_BookItem_Property* BookItem = (_XGI_BookItem_Property*)GetFirstTypeItemPorperty(m_SelectedCategory1);
				if( BookItem )
				{
					if(m_TradeMode == _XNPCTRADE_BUY)
						m_Price = BookItem[m_SelectedItemID].uiPrice * m_TradeCount;
					else if(m_TradeMode == _XNPCTRADE_SELL)
						m_Price = (BookItem[m_SelectedItemID].uiPrice/5) * m_TradeCount;
				}
			}
			break;			
		case _XGI_FC_POTION :
			{
				_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(m_SelectedCategory1);
				if( PotionItem )
				{
					if(m_TradeMode == _XNPCTRADE_BUY)
						m_Price = PotionItem[m_SelectedItemID].uiPrice * m_TradeCount;
					else if(m_TradeMode == _XNPCTRADE_SELL)
						m_Price = (PotionItem[m_SelectedItemID].uiPrice/5) * m_TradeCount;
				}
			}
			break;	
		case _XGI_FC_RESOURCE :
			{
				_XGI_ResourceItem_Property* ResourceItem = (_XGI_ResourceItem_Property*)GetFirstTypeItemPorperty(m_SelectedCategory1);				
				if( ResourceItem )
				{
					if(m_TradeMode == _XNPCTRADE_BUY)
						m_Price = ResourceItem[m_SelectedItemID].uiPrice * m_TradeCount;
					else if(m_TradeMode == _XNPCTRADE_SELL)
						m_Price = (ResourceItem[m_SelectedItemID].uiPrice/5) * m_TradeCount;
				}
			}
			break;	
		}
	}
#else
	switch(m_SelectedCategory1)
	{
	case _XGI_FC_WEAPON :
		{
			if(m_TradeMode == _XNPCTRADE_BUY)
				m_Price = g_WeaponItemProperty[m_SelectedItemID].uiPrice * m_TradeCount;
			else if(m_TradeMode == _XNPCTRADE_SELL)
				m_Price = (g_WeaponItemProperty[m_SelectedItemID].uiPrice/5) * m_TradeCount;
		}
		break;
	case _XGI_FC_CLOTHES :
		{
			if(m_TradeMode == _XNPCTRADE_BUY)
				m_Price = g_ClothesItemProperty[m_SelectedItemID].uiPrice * m_TradeCount;
			else if(m_TradeMode == _XNPCTRADE_SELL)
				m_Price = (g_ClothesItemProperty[m_SelectedItemID].uiPrice/5) * m_TradeCount;
		}
		break;
	case _XGI_FC_WEAPON2 :
		{
			if(m_TradeMode == _XNPCTRADE_BUY)
				m_Price = g_WeaponItemProperty2[m_SelectedItemID].uiPrice * m_TradeCount;
			else if(m_TradeMode == _XNPCTRADE_SELL)
				m_Price = (g_WeaponItemProperty2[m_SelectedItemID].uiPrice/5) * m_TradeCount;
		}
		break;
	case _XGI_FC_CLOTHES2 :
		{
			if(m_TradeMode == _XNPCTRADE_BUY)
				m_Price = g_ClothesItemProperty2[m_SelectedItemID].uiPrice * m_TradeCount;
			else if(m_TradeMode == _XNPCTRADE_SELL)
				m_Price = (g_ClothesItemProperty2[m_SelectedItemID].uiPrice/5) * m_TradeCount;
		}
		break;
	case _XGI_FC_WEAPON3 :
		{
			if(m_TradeMode == _XNPCTRADE_BUY)
				m_Price = g_WeaponItemProperty3[m_SelectedItemID].uiPrice * m_TradeCount;
			else if(m_TradeMode == _XNPCTRADE_SELL)
				m_Price = (g_WeaponItemProperty3[m_SelectedItemID].uiPrice/5) * m_TradeCount;
		}
		break;
	case _XGI_FC_CLOTHES3 :
		{
			if(m_TradeMode == _XNPCTRADE_BUY)
				m_Price = g_ClothesItemProperty3[m_SelectedItemID].uiPrice * m_TradeCount;
			else if(m_TradeMode == _XNPCTRADE_SELL)
				m_Price = (g_ClothesItemProperty3[m_SelectedItemID].uiPrice/5) * m_TradeCount;
		}
		break;
	case _XGI_FC_POTION :
		{
			if(m_TradeMode == _XNPCTRADE_BUY)
				m_Price = g_PotionItemProperty[m_SelectedItemID].uiPrice * m_TradeCount;
			else if(m_TradeMode == _XNPCTRADE_SELL)
				m_Price = (g_PotionItemProperty[m_SelectedItemID].uiPrice/5) * m_TradeCount;
		}
		break;

	case _XGI_FC_RESOURCE :
		{
			if(m_TradeMode == _XNPCTRADE_BUY)
				m_Price = g_ResourceItemProperty[m_SelectedItemID].uiPrice * m_TradeCount;
			else if(m_TradeMode == _XNPCTRADE_SELL)
				m_Price = (g_ResourceItemProperty[m_SelectedItemID].uiPrice/5) * m_TradeCount;
		}
		break;
	case _XGI_FC_BOOK :
		{
			if(m_TradeMode == _XNPCTRADE_BUY)
				m_Price = g_BookItemProperty[m_SelectedItemID].uiPrice * m_TradeCount;
			else if(m_TradeMode == _XNPCTRADE_SELL)
				m_Price = (g_BookItemProperty[m_SelectedItemID].uiPrice/5) * m_TradeCount;
		}
		break;
	default :
		break;
	}
#endif
}