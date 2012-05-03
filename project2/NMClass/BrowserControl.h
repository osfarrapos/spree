#pragma once
#include <atlbase.h>
#include <ExDisp.h>
#include <ExDispId.h>
#include <mshtmhst.h>

namespace ADAD
{

class BrowserControlPane : 
	public IOleClientSite,
	public IOleInPlaceSite,
	public DWebBrowserEvents2,
	public IDocHostUIHandler,
	public IDocHostShowUI
{
	public:
		enum
			{
			// notification messages
			kRequestClose = 'BCrc',
			kBeforeNavigate = 'SCbn',

			// event handling mode values
			kIgnore = 0,
			kHandleAndPass,
			kHandleAndDontPass,
			};

	private:
		static BrowserControlPane*	s_pInCreation;

		HWND											hwnd;
		HWND											hparentwnd;
		CComQIPtr< IWebBrowser2, &IID_IWebBrowser2 >	pBrowser;
		DWORD											connCookie;

		// settings
		BOOL			allowNewWindow;
		BOOL			showScrollBar;
		BOOL			isActivated;
		TCHAR			sLimitToInside[ 256 ];
		DWORD			timeout;
		BYTE			keyHandling;
		BYTE			mouseHandling;

		// states
		BOOL			initialLoading;
		IDispatch*		pLoadingDisp;	// used for determining When a page is done loading

	public:
		BrowserControlPane( HWND hpwnd, HINSTANCE hInstance, RECT* ipBoundRect );	// iLoadingVideo == -1 for no video
		virtual ~BrowserControlPane( void );

		void			Dispose( void );

		void			NavigateTo( LPCTSTR isURL );

		void			ActivateWindow( BOOL iValue );
		void			SetShowScrollBar( BOOL iValue );
		void			SetLimitToInside( LPCTSTR isValue = NULL );
		void			SetTimeout( DWORD iValue );	// 0 for no timeout
		void			SetKeyHandling( BYTE iValue );
		void			SetMouseHandling( BYTE iValue );

		virtual BOOL	HandleTimerEvent( LONG iCode, LONG iParam1, LONG iParam2 );
		virtual void	Draw( POINT iOrigin );

		virtual void	HandleMSG( MSG* ipMsg );

	private:
		void					FailHandler( BOOL iOK );
		void					TimeoutHandler( BOOL iOK );

		// IUnknown
		STDMETHODIMP			QueryInterface( REFIID riid, void** ppv );
		STDMETHODIMP_(ULONG)	AddRef( void );
		STDMETHODIMP_(ULONG)	Release( void );

		// IOleClientSite
		STDMETHODIMP			SaveObject( void );
		STDMETHODIMP			GetMoniker( DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk );
		STDMETHODIMP			GetContainer( LPOLECONTAINER FAR* ppContainer );
		STDMETHODIMP			ShowObject( void );
		STDMETHODIMP			OnShowWindow( BOOL fShow );
		STDMETHODIMP			RequestNewObjectLayout( void );

		// IOleWindow
		STDMETHODIMP			GetWindow( HWND* phwnd );
		STDMETHODIMP			ContextSensitiveHelp( BOOL fEnterMode );

		// IOleInPlaceSite
		STDMETHODIMP			CanInPlaceActivate( void );
		STDMETHODIMP			OnInPlaceActivate( void );
		STDMETHODIMP			OnUIActivate( void );
		STDMETHODIMP			GetWindowContext( IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo );
		STDMETHODIMP			Scroll( SIZE scrollExtent );
		STDMETHODIMP			OnUIDeactivate( BOOL fUndoable );
		STDMETHODIMP			OnInPlaceDeactivate( void );
		STDMETHODIMP			DiscardUndoState( void );
		STDMETHODIMP			DeactivateAndUndo( void );
		STDMETHODIMP			OnPosRectChange( LPCRECT lprcPosRect );

		// IDispatch
		STDMETHODIMP			GetIDsOfNames( REFIID riid, OLECHAR FAR* FAR* rgsaNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId );
		STDMETHODIMP			GetTypeInfo( unsigned int iTInfo, LCID lcid, ITypeInfo FAR* FAR* ppTInfo );
		STDMETHODIMP			GetTypeInfoCount( unsigned int FAR* pctinfo );
		STDMETHODIMP			Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR* pDispParams, VARIANT FAR* pVarResult, EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr );

		// IDocHostUIHandler
		STDMETHODIMP			EnableModeless( BOOL fEnable );
		STDMETHODIMP			FilterDataObject( IDataObject* pDO, IDataObject** ppDORet );
		STDMETHODIMP			GetDropTarget( IDropTarget* pDropTarget, IDropTarget** ppDropTarget );
		STDMETHODIMP			GetExternal( IDispatch** ppDispatch );
		STDMETHODIMP			GetHostInfo( DOCHOSTUIINFO FAR* pInfo );
		STDMETHODIMP			GetOptionKeyPath( LPOLESTR* pchKey, DWORD dwReserved );
		STDMETHODIMP			HideUI( void );
		STDMETHODIMP			OnDocWindowActivate( BOOL fActive );
		STDMETHODIMP			OnFrameWindowActivate( BOOL fActive );
		STDMETHODIMP			ResizeBorder( LPCRECT prcBorder, IOleInPlaceUIWindow FAR* pUIWindow, BOOL fFrameWindow );
		STDMETHODIMP			ShowContextMenu( DWORD dwID, POINT FAR* ppt, IUnknown FAR* pcmdTarget, IDispatch FAR* pdispObject );
		STDMETHODIMP			ShowUI( DWORD dwID, IOleInPlaceActiveObject FAR* pActiveObject, IOleCommandTarget FAR* pCommandTarget, IOleInPlaceFrame FAR* pFrame, IOleInPlaceUIWindow FAR* pDoc );
		STDMETHODIMP			TranslateAccelerator( LPMSG lpMsg, const GUID FAR* pguidCmdGroup, DWORD nCmdID );
		STDMETHODIMP			TranslateUrl( DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut );
		STDMETHODIMP			UpdateUI( void );

		// IDocHostShowUI
		STDMETHODIMP			ShowHelp( HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand, DWORD dwData, POINT ptMouse, IDispatch* pDispatchObjectHit );
		STDMETHODIMP			ShowMessage( HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT* plResult );

		static LRESULT CALLBACK	WindowProcEntry( HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam );
		LRESULT					WindowProc( HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam );
};

}