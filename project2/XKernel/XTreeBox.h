// XTreeBox.h: interface for the _XTreeBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTREEBOX_H__DEE578C8_E8EA_4313_AE80_AAB9AED7F21E__INCLUDED_)
#define AFX_XTREEBOX_H__DEE578C8_E8EA_4313_AE80_AAB9AED7F21E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XStringItem.h"
#include "XScrollBar.h"
#include "XMString.h"
#include "XFont.h"
#include "XWindow.h"

#include <list>
using namespace std;

class _XNodeData 
{
public:
	int						NodeId;
	_XNodeData*				ParentNode;
	int						Index;
	bool					Opened;		
	_XMString				NodeString;
	_XMString				NodeIcon;
	int						Pos;
	list<_XNodeData*>		ChildIdList;
	
	_XNodeData();
	virtual ~_XNodeData();
	
	_XNodeData* FindNodeByIndex(_XNodeData*findNode, int index);
	_XNodeData* FindNodeByCommandId(_XNodeData*findNode, int commandid);
	int GrantIndex(int count);
	int ParentNodeCount(_XNodeData* parentnode, int count);
};

typedef struct
{
	// window property
	BOOL				enable;										// 윈도우 활성화
	POINT				position;									// 윈도우 좌표 
	SIZE				windowsize;									// 윈도우 사이즈
	DWORD				commandid;									// 윈도우 아이디 <사용안함>
	int					windowtextureindex;							// 윈도우 볼더 이미지 인덱스
	_XTextureManager*	texturearchive;								// 텍스쳐리스트
	char				WindowTitle[_XDEF_WINDOWTITLESTRINGSIZE];	// 윈도우 타이틀 
	int					ypitch;										// 리스트 아이템 라인 피치
	int					maxviewlinecount;							// 리스트 박스에 보이는 최대 라인수 	

	// Scroll bar property
	SIZE				scrollbarsize;								// 스크롤바 사이즈
	int					btnysize;									// 스크롤바 상하버튼 사이즈
	int					trackbarysize;								// 스크롤바 트랙바 사이즈
	int					totallinecount;								// 전체 리스트 갯수 <설정 필요없슴>	

	int					imageindex_uparrow;							// 버튼 이미지 인덱스
	int					imageindex_uparrowclick;
	int					imageindex_downarrow;
	int					imageindex_downarrowclick;
	int					imageindex_trackbar;

} _XTREEBOX_STRUCTURE;

typedef	BOOL (__stdcall *_XGUITreeBoxItemSelectCallback)( int param1, int param2 );
typedef	BOOL (__stdcall *_XGUITreeBoxItemMouseHoverCallback)( int param1, int param2, int XPos, int YPos );

class _XTreeBox : public _XWindow  
{
protected:
	int						m_YPitch;
	int						m_TrackWidth;
	int						m_TrackHeight;
	int						m_SelectedItem;
	int						m_PrevSelectedItem;
	int						m_MaxViewLineCount;
	int						m_CurrentScrollPos;	
	int						m_IndexListCount;
	int						m_TextXOffset;							// 스크롤바의 위치에 따른 Item offset
	BOOL					m_ItemSelectable;	

	_XNodeData*				m_RootItem;	
	_XVScrollBar			m_ScrollBar;
	_XGUIObject*			m_pScrbarFocusedObject;	
	_XImageStatic			m_IconImageStatic;
	_XStatic				m_SelectStatic;
	_XStatic				m_SelectedStatic;

	BOOL					m_bAlwaysShowScrollBarMode;
	BOOL					m_bMoveScrollBarMode;
	POINT					m_ScrollBarPositon;
	
	_XImageStatic			m_SelectedImageStatic;
	int						m_SelectedImageHeight;
	POINT					m_SelectedImagePos;

	_XFONTID				m_FontID;
		
	int						m_ColumnWidth;
	_XFONT_ALIGNTYPE		m_ColumnAlignType;
	POINT					m_ColumnOffset;	
	_XFRECT					m_ColumnClipRect;

	int						m_IconImage;
	int						m_IconImageEx;
	RECT					m_IconRect;
	RECT					m_IconRectEx;
	int						m_SelectedCommandId;

	_XGUITreeBoxItemSelectCallback			m_ItemSelectCallBack;
	_XGUITreeBoxItemMouseHoverCallback		m_ItemMouseHoverCallBack;
public:
	_XTreeBox();
	virtual ~_XTreeBox();	

	BOOL			Create( _XTREEBOX_STRUCTURE treeboxstructure );
	void			DestroyWindow( void );
				
	void			MoveWindow( int X, int Y );
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			ProcessWheelMessage( short zDelta );

	void			UpdateTreeBox();
	int				GetTextXOffset( void ){ return m_TextXOffset; }
	void			SetTextXOffset( int offset ){ m_TextXOffset = offset; }

	void			SetMaxViewLineCount( int viewlinecount );
	
	int				GetItemCount( void ){ return m_IndexListCount; };

	BOOL			InsertListItem( LPTSTR itemstring, int commandid = 0, int parentid = 0, _XTextureManager* pIconArchive = NULL );
	
	void			SetIconArchive( int index, _XTextureManager* pIconArchive );
	_XTextureManager* GetIconArchive( int index );

	BOOL			SetItemText( int index, int column, LPTSTR itemstring, BOOL boldstring = FALSE );
	LPTSTR			GetItemText( int index, int column );
	
	BOOL			SetItemAttrib( int index, DWORD attrib );
	DWORD			GetItemAttrib( int index );
	
	BOOL			SetItemAttrib(int index, DWORD attrib, RECT rect);
	BOOL			SetItemAttribHighlight(int index, DWORD attrib);
	
	BOOL			SetItemCommandID( int index, DWORD commandid = 0 );
	DWORD			GetItemCommandID( int index );	

	void			SettingIcon(int image1, int image2, RECT rt1, RECT rt2 );

	void			SetColumnWidth( int width )
	{
					
					m_ColumnWidth = width;

					m_ColumnClipRect.left	=	m_WindowPosition.x;
					m_ColumnClipRect.right	=	m_WindowPosition.x + m_ColumnClipRect.left + width;
	}

	void			SetAlignType( _XFONT_ALIGNTYPE type )
	{
					m_ColumnAlignType = type;
	}

	void			SetColumnOffset( POINT offset )
	{
					m_ColumnOffset = offset;

					m_ColumnClipRect.left	=	m_WindowPosition.x + offset.x;
					m_ColumnClipRect.right	=	m_WindowPosition.x + offset.x + m_ColumnWidth;
	}
	void			SetColumnOffset( int x, int y )
	{					
					m_ColumnOffset.x = x;
					m_ColumnOffset.y = y;
	}

	int				CheckSelectedItem( void );

	void			SetTrackBarColor( D3DCOLOR color )
	{
					m_SelectStatic.SetFaceColor( color );
	}
	void			SetSelectedTrackBarColor( D3DCOLOR color )
	{
					m_SelectedStatic.SetFaceColor( color );
	}

	void			SetTrackWidth ( int width  )
	{				
					m_TrackWidth  = width;
					m_SelectStatic.MoveWindow( 0, 0, m_TrackWidth, m_TrackHeight );
					m_SelectedStatic.MoveWindow( 0, 0, m_TrackWidth, m_TrackHeight );
	}
	void			SetTrackHeight( int height )
	{				
					m_TrackHeight = height;
					m_SelectStatic.MoveWindow( 0, 0, m_TrackWidth, m_TrackHeight );
					m_SelectedStatic.MoveWindow( 0, 0, m_TrackWidth, m_TrackHeight );
	}		

	void			SetWindowCommandID(DWORD commandid)	
	{
					m_CommandID = commandid;
	}
	DWORD			GetWindowCommandID(void)
	{
					return m_CommandID;
	}

	void			SetSelectItemCallBack( _XGUITreeBoxItemSelectCallback _callbackfunction )
	{
					m_ItemSelectCallBack = _callbackfunction;
	}
	void			SetSelectItemMouseHoverCallBack( _XGUITreeBoxItemMouseHoverCallback _callbackfunction )
	{
					m_ItemMouseHoverCallBack = _callbackfunction;
	}
	int				GetSelectedCommandId() { return m_SelectedCommandId; };

	void            EnableSelectItem( BOOL enable ){ m_ItemSelectable = enable; }
	void			SelectItem( int index );
	int 			GetSelectedItem( void ){ return m_SelectedItem; }
	void			SetSelectedItem( int selectitem ) { m_SelectedItem = selectitem; 	};
	void			SetSelectItemChange( int flag );
	int 			GetPrevSelectedItem( void ){ return m_PrevSelectedItem; }
	void			DeleteItem(  int index  );

	_XVScrollBar*	GetScrollBarObject( void ){ return &m_ScrollBar; }
	int				GetScrollPos( void ){ return m_CurrentScrollPos; }
	void			SetScrollPos(int scrollpos)					{	m_ScrollBar.SetScrollPos(scrollpos);	}
	int				GetLineScrollPos( void ){ return m_ScrollBar.GetLineScrollPos(); }
	void			SetLineScrollPos(int scrollpos)					{	m_ScrollBar.SetLineScrollPos(scrollpos);	}
	
	_XImageStatic*	GetSelectBarObject( void ){ return &m_SelectedImageStatic; }	

	// 스크롤바의 위치와 크기를 임의로 조절하기 위한 함수 
	void			SetMoveScrollBarMode( BOOL bmovescrollbarmode ) { m_bMoveScrollBarMode = bmovescrollbarmode; }
	void			SetAlwaysShowScrollBarMode( BOOL balwaysshowscrollbarmode ); 
	void			MoveScrollBarPos(int X, int Y);
	void			SetScrollBarHeight(int height){	m_ScrollBar.SetWindowHeight( height );	}

	int				GetSelectedItemCommandID(int selected);
	void			SetSelectedImageArchive(int index, _XTextureManager* pImageArchive);
	void			SetSelectedImageHeight(int height);
	void			SetSelectedImagePosition(int X, int Y);	
	
	void			SetFont( _XFONTID fontid );
};

#endif // !defined(AFX_XTREEBOX_H__DEE578C8_E8EA_4313_AE80_AAB9AED7F21E__INCLUDED_)
