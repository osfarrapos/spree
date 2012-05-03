#include "ADBalloon.h"

#include <atlbase.h>
#include <ExDisp.h>
#include <ExDispId.h>
#include <mshtmhst.h>
#include <ZMouse.h>

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

	void Log(LPCTSTR str) {
		OutputDebugString(str);
	}

	void Log(LPCTSTR str, LONG) {
		OutputDebugString(str);
	}

	void Log(LPCTSTR str, void *) {
		OutputDebugString(str);
	}


#define	ASSERT(a)	(void)(0)
#define _NEW		new
#define _DELETE		delete

	namespace
	{
		enum
		{
			// timer event codes
			kDispose = 'BCdp',
			kFail = 'BCfl',
			kTimeout = 'BCto',

			// initial timeout value
			kInitialTimeout = 30 * 1000,	// 30 seconds
		};
	};

	/***
	BrowserControlPane
	***/
	BrowserControlPane* BrowserControlPane::s_pInCreation = NULL;

	BrowserControlPane::BrowserControlPane( HWND hpwnd, HINSTANCE hInstance, RECT* ipBoundRect )
		: hparentwnd(hpwnd)
		// 콘트롤 폐인 없음... : ControlPane( ipBoundRect )
	{
		HRESULT					err;
		RECT					bounds;
		TCHAR*					sClassName = TEXT("NexonADBrowser");
		WNDCLASS				windowClass;
		CComPtr< IOleObject >	pOleObject;
		RECT					clientRECT;

		ASSERT( ipBoundRect != NULL );

		this->isActivated = TRUE;
		this->allowNewWindow = TRUE;
		this->showScrollBar = FALSE;
		this->sLimitToInside[ 0 ] = 0;
		this->timeout = kInitialTimeout;
		this->keyHandling = kHandleAndDontPass;
		this->mouseHandling = kHandleAndDontPass;
		// this->initialLoading = TRUE;
		this->initialLoading = FALSE;

		bounds = *ipBoundRect;

		// create window

		windowClass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW | CS_DBLCLKS | CS_OWNDC;
		windowClass.lpfnWndProc = ( WNDPROC ) BrowserControlPane::WindowProcEntry;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = hInstance;
		windowClass.hIcon = NULL;
		windowClass.hCursor = LoadCursor( NULL, IDC_ARROW );
		windowClass.hbrBackground = ( HBRUSH ) ::GetStockObject( BLACK_BRUSH );
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = sClassName;
		RegisterClass( &windowClass );

		ASSERT( s_pInCreation == NULL );
		s_pInCreation = this;
		this->hwnd = CreateWindowEx( 0, sClassName, _T( "" ), WS_CHILD | WS_CLIPSIBLINGS,
			bounds.left, bounds.top, bounds.right - bounds.left, bounds.bottom - bounds.top,
			hparentwnd, ( HMENU ) NULL, hInstance, ( LPVOID ) NULL );
		::SetWindowLongA( this->hwnd, GWL_USERDATA, ( DWORD ) this );
		s_pInCreation = NULL;

		// create WebBrowser control
		err = ::CoCreateInstance( CLSID_WebBrowser, NULL, CLSCTX_INPROC, IID_IOleObject, ( void** ) &pOleObject );
		ASSERT( SUCCEEDED( err ) );
		if ( SUCCEEDED( err ) )
		{
			err = pOleObject->SetClientSite( this );
			ASSERT( SUCCEEDED( err ) );
		}

		// in-place actvivate the control
		if ( SUCCEEDED( err ) )
		{
			::SetRect( &clientRECT, 0, 0, bounds.right - bounds.left, bounds.bottom - bounds.top );
			err = pOleObject->DoVerb( OLEIVERB_INPLACEACTIVATE, NULL, this, 0, this->hwnd, &clientRECT );
			ASSERT( SUCCEEDED( err ) );
		}

		// get the browser control pointer
		if ( SUCCEEDED( err ) )
		{
			this->pBrowser = pOleObject;
			ASSERT( this->pBrowser != NULL );
		}

		// setup connection for events
		if ( SUCCEEDED( err ) )
		{
			err = AtlAdvise( this->pBrowser, ( IOleClientSite* ) this, DIID_DWebBrowserEvents2, &this->connCookie );
			ASSERT( SUCCEEDED( err ) );
		}

		// set size
		if ( SUCCEEDED( err ) )
		{
			CComQIPtr<IOleInPlaceObject, &IID_IOleInPlaceObject>	pInPlaceObject( this->pBrowser );

			ASSERT( pInPlaceObject != NULL );
			err = pInPlaceObject->SetObjectRects( &clientRECT, &clientRECT );
			ASSERT( SUCCEEDED( err ) );
		}

		if ( FAILED( err ) )
		{
			if ( this->pBrowser != NULL )
			{
				err = AtlUnadvise( this->pBrowser, DIID_DWebBrowserEvents2, this->connCookie );
				ASSERT( SUCCEEDED( err ) );
			}

			this->pBrowser.Release();
			::OleUninitialize();
			::DestroyWindow( this->hwnd );
			this->hwnd = NULL;
		}
	}


	BrowserControlPane::~BrowserControlPane( void )
	{
		this->Dispose();
	}


	void BrowserControlPane::Dispose( void )
	{
		this->keyHandling = kIgnore;
		this->mouseHandling = kIgnore;

		if ( this->pBrowser != NULL )
		{
			HRESULT	err;

			err = AtlUnadvise( this->pBrowser, DIID_DWebBrowserEvents2, this->connCookie );
			ASSERT( SUCCEEDED( err ) );
		}

		if ( this->pBrowser != NULL )
		{
			this->pBrowser->Stop();
			this->pBrowser->Quit();
			this->pBrowser.Release();
		}

		if ( this->hwnd != NULL )
		{
			::DestroyWindow( this->hwnd );
			this->hwnd = NULL;
			::SetFocus(hparentwnd);
		}
	}


	void BrowserControlPane::NavigateTo( LPCTSTR isURL )
	{
		if ( this->pBrowser != NULL )
		{
			BSTR		sURL;
			CComVariant	postData;

			sURL = T2BSTR( isURL );
			postData.vt = VT_ARRAY;
			postData.parray = ::SafeArrayCreate( VT_UI1, 0, NULL );

			try
			{
				VARIANT t;
				t.vt = VT_I4;
				t.iVal = 0;
				t.iVal |= navNoWriteToCache ;

				this->pBrowser->Navigate( sURL, &CComVariant( navNoHistory, VT_I4 ), &CComVariant( ( BSTR ) NULL ), &postData, &CComVariant( ( BSTR ) NULL ) );
				::SafeArrayDestroy( postData.parray );
				::SysFreeString( sURL );
			}
			catch( ... )
			{
				this->Dispose();
			}
		}
	}


	void BrowserControlPane::SetShowScrollBar( BOOL iValue )
	{
		this->showScrollBar = iValue;
		if ( this->pBrowser != NULL )
		{
			this->pBrowser->Refresh();
		}
	}


	void BrowserControlPane::SetLimitToInside( LPCTSTR isValue )
	{
		if ( isValue == NULL )
		{
			this->sLimitToInside[ 0 ] = 0;
		}
		else
		{
			::_tcsncpy( this->sLimitToInside, isValue, 255 );
			this->sLimitToInside[ 255 ] = 0;
		}
	}


	void BrowserControlPane::SetTimeout( DWORD iValue )
	{
		this->timeout = iValue;
	}


	void BrowserControlPane::SetKeyHandling( BYTE iValue )
	{
		this->keyHandling = iValue;
	}


	void BrowserControlPane::SetMouseHandling( BYTE iValue )
	{
		this->mouseHandling = iValue;
	}

	// ACTIVATE WINDOW: INITINSTANCE [2002/3/29]
	void BrowserControlPane::ActivateWindow( BOOL iValue )
	{
		this->isActivated = iValue;
		if ( iValue == TRUE && !this->initialLoading)
		{
			::ShowWindow( this->hwnd, SW_SHOW );
			::UpdateWindow( this->hwnd );
		}
		else
		{
			::ShowWindow( this->hwnd, SW_HIDE);
		}

	}

	BOOL BrowserControlPane::HandleTimerEvent( LONG iCode, LONG iParam1, LONG iParam2 )
	{
		switch( iCode )
		{
		case kDispose:
			this->Dispose();
			return TRUE;

		case kFail:
			return TRUE;

		case kTimeout:
			return TRUE;

		default:
			return FALSE;
		}
	}


	void BrowserControlPane::Draw( POINT iOrigin )
	{
		if ( !::IsWindowVisible( this->hwnd ) )
		{
		}
	}


	void BrowserControlPane::HandleMSG( MSG* ipMsg )
		// it must be a handler that handles messages from its parent-pane.
	{
		ASSERT( ipMsg != NULL );

		if ( !isActivated )
		{
			return;
		}

		if ( this->keyHandling != kHandleAndDontPass )
		{
			if ( ipMsg->message == WM_KEYDOWN || ipMsg->message == WM_KEYUP || ipMsg->message == WM_SYSKEYDOWN || ipMsg->message == WM_SYSKEYUP )
			{
				SendMessage( hparentwnd, ipMsg->message, ipMsg->wParam, ipMsg->lParam );
				if ( this->keyHandling == kIgnore )
				{
					return ;
				}
			}
		}

		if ( this->mouseHandling != kHandleAndDontPass )
		{
			if ( ipMsg->message == WM_MOUSEMOVE || ipMsg->message == WM_LBUTTONDOWN || ipMsg->message == WM_LBUTTONDBLCLK || ipMsg->message == WM_LBUTTONUP ||
				ipMsg->message == WM_RBUTTONDOWN || ipMsg->message == WM_RBUTTONDBLCLK || ipMsg->message == WM_RBUTTONUP || ipMsg->message == WM_MOUSEWHEEL )
			{
				SendMessage( hparentwnd, ipMsg->message, ipMsg->wParam, ipMsg->lParam );
				if ( this->mouseHandling == kIgnore )
				{
					return;
				}
			}
		}

		if ( ( ipMsg->wParam == VK_TAB || ipMsg->wParam == VK_ESCAPE || ipMsg->wParam == VK_DELETE ) && ( ipMsg->message == WM_KEYDOWN || ipMsg->message == WM_KEYUP || ipMsg->message == WM_SYSKEYDOWN || ipMsg->message == WM_SYSKEYUP ) )
		{
			SendMessage( this->hwnd, ipMsg->message, ipMsg->wParam, ipMsg->lParam );
		}
		else if ( ipMsg->message == WM_KEYDOWN && this->pBrowser != NULL && ipMsg->wParam == 'V' && ::GetKeyState( VK_CONTROL ) < 0 )
		{
			this->pBrowser->ExecWB( OLECMDID_PASTE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL );
		}
	}


	void BrowserControlPane::FailHandler( BOOL iOK )
	{
		this->Dispose();
	}


	void BrowserControlPane::TimeoutHandler( BOOL iOK )
	{
		if ( iOK )
		{
			this->Dispose();
		}
		else
		{
			if ( isActivated )
				::ShowWindow( this->hwnd, SW_SHOW );
		}
	}


	STDMETHODIMP BrowserControlPane::QueryInterface( REFIID riid, void** ppv )
	{
		ASSERT( ppv != NULL );

		if ( riid == IID_IOleClientSite )
		{
			*ppv = ( IOleClientSite* ) this;
		}
		else if ( riid == IID_IUnknown )
		{
			*ppv = ( IOleClientSite* ) this;
		}
		else if ( riid == IID_IOleInPlaceSite )
		{
			*ppv = ( IOleInPlaceSite* ) this;
		}
		else if ( riid == IID_IDispatch )
		{
			*ppv = ( IDispatch* ) this;
		}
		else if ( riid == DIID_DWebBrowserEvents2 )
		{
			*ppv = ( DWebBrowserEvents2* ) this;
		}
		else if ( riid == IID_IOleWindow )
		{
			*ppv = ( IOleInPlaceSite* ) this;
		}
		else if ( riid == IID_IDocHostUIHandler )
		{
			*ppv = ( IDocHostUIHandler* ) this;
		}
		else if ( riid == IID_IDocHostShowUI )
		{
			*ppv = ( IDocHostShowUI* ) this;
		}
		else
		{
			*ppv = NULL;
			return E_NOINTERFACE;
		}

		return S_OK;
	}


	STDMETHODIMP_(ULONG) BrowserControlPane::AddRef( void )
	{
		return 2;
	}


	STDMETHODIMP_(ULONG) BrowserControlPane::Release( void )
	{
		return 1;
	}


	STDMETHODIMP BrowserControlPane::SaveObject( void )
	{
		Log( _T( "BrowserControlPane::SaveObject\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetMoniker( DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk )
	{
		Log( _T( "BrowserControlPane::GetMoniker\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetContainer( LPOLECONTAINER FAR* ppContainer )
	{
		Log( _T( "BrowserControlPane::GetContainer\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::ShowObject( void )
	{
		Log( _T( "BrowserControlPane::ShowObject\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::OnShowWindow( BOOL fShow )
	{
		Log( _T( "BrowserControlPane::OnShowWindow\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::RequestNewObjectLayout( void )
	{
		Log( _T( "BrowserControlPane::RequestNewObjectLayout\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetWindow( HWND* phwnd )
	{
		Log( _T( "BrowserControlPane::GetWindow\n" ) );
		*phwnd = this->hwnd;
		return S_OK;
	}


	STDMETHODIMP BrowserControlPane::ContextSensitiveHelp( BOOL fEnterMode )
	{
		Log( _T( "BrowserControlPane::ContextSensitiveHelp\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::CanInPlaceActivate( void )
	{
		Log( _T( "BrowserControlPane::CanInPlaceActivate\n" ) );
		return S_OK;
	}


	STDMETHODIMP BrowserControlPane::OnInPlaceActivate( void )
	{
		Log( _T( "BrowserControlPane::OnInPlaceActivate\n" ) );
		return S_OK;
	}


	STDMETHODIMP BrowserControlPane::OnUIActivate( void )
	{
		Log( _T( "BrowserControlPane::OnUIActivate\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetWindowContext( IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo )
	{
		Log( _T( "BrowserControlPane::GetWindowContext\n" ) );
		::GetClientRect( this->hwnd, lprcPosRect );
		::GetClientRect( this->hwnd, lprcClipRect );
		return S_OK;
	}


	STDMETHODIMP BrowserControlPane::Scroll( SIZE scrollExtent )
	{
		Log( _T( "BrowserControlPane::Scroll\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::OnUIDeactivate( BOOL fUndoable )
	{
		Log( _T( "BrowserControlPane::OnUIDeactivate\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::OnInPlaceDeactivate( void )
	{
		Log( _T( "BrowserControlPane::OnInPlaceDeactivate\n" ) );
		return S_OK;
	}


	STDMETHODIMP BrowserControlPane::DiscardUndoState( void )
	{
		Log( _T( "BrowserControlPane::DiscardUndoState\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::DeactivateAndUndo( void )
	{
		Log( _T( "BrowserControlPane::DeactivateAndUndo\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::OnPosRectChange( LPCRECT lprcPosRect )
	{
		Log( _T( "BrowserControlPane::OnPosRectChange\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetIDsOfNames( REFIID riid, OLECHAR FAR* FAR* rgsaNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId )
	{
		Log( _T( "BrowserControlPane::GetIDsOfNames\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetTypeInfo( unsigned int iTInfo, LCID lcid, ITypeInfo FAR* FAR* ppTInfo )
	{
		Log( _T( "BrowserControlPane::GetTypeInfo\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetTypeInfoCount( unsigned int FAR* pctinfo )
	{
		Log( _T( "BrowserControlPane::GetTypeInfoCount\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR* pDispParams, VARIANT FAR* pVarResult, EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr )
	{
		HRESULT	result;
		USES_CONVERSION;

		Log( _T( "BrowserControlPane::Invoke, dispIdMember " ) );
		switch( dispIdMember )
		{
		case DISPID_BEFORENAVIGATE2:
			{
				BSTR	sURL = pDispParams->rgvarg[ 5 ].pvarVal->bstrVal;

				LPCSTR lpStr = OLE2CT(sURL);
				if( strstr(  lpStr, _T("empty"))  != NULL) SendMessage(hparentwnd, WM_USER, 0, 0);

			}

			result = S_OK;
			break;

		case DISPID_NEWWINDOW2:
			Log( _T( "DISPID_NEWWINDOW2" ) );			
			if ( !this->allowNewWindow )
			{
				*pDispParams->rgvarg[ 0 ].pboolVal = TRUE;
			}
			SendMessage(hparentwnd, WM_USER, 0, 0);
			result = S_OK;
			break;

		case DISPID_NAVIGATECOMPLETE2:
			Log( _T( "DISPID_NAVIGATECOMPLETE2" ) );
			if ( this->pLoadingDisp == NULL )
			{
				this->pLoadingDisp = pDispParams->rgvarg[ 1 ].pdispVal;
			}				
			result = S_OK;
			break;

		case DISPID_DOCUMENTCOMPLETE:
			Log( _T( "DISPID_DOCUMENTCOMPLETE" ) );
			if ( this->initialLoading )
			{
				if ( isActivated )
				{
					::ShowWindow( this->hwnd, SW_SHOW );
					::UpdateWindow( this->hwnd );
				}
				this->initialLoading = FALSE;
			}
			result = S_OK;
			break;

		default:
			Log( _T( "%u" ), dispIdMember );
			result = DISP_E_MEMBERNOTFOUND;
			break;
		}
		Log( _T( "\n" ) );

		return result;
	}


	STDMETHODIMP BrowserControlPane::EnableModeless( BOOL fEnable )
	{
		Log( _T( "BrowserControlPane::EnableModeless\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::FilterDataObject( IDataObject* pDO, IDataObject** ppDORet )
	{
		Log( _T( "BrowserControlPane::FilterDataObject\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetDropTarget( IDropTarget* pDropTarget, IDropTarget** ppDropTarget )
	{
		Log( _T( "BrowserControlPane::GetDropTarget\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetExternal( IDispatch** ppDispatch )
	{
		Log( _T( "BrowserControlPane::GetExternal\n" ) );
		*ppDispatch = NULL;
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::GetHostInfo( DOCHOSTUIINFO FAR* pInfo )
	{
		ASSERT( pInfo != NULL );

		Log( _T( "BrowserControlPane::GetHostInfo\n" ) );
		pInfo->cbSize = sizeof( DOCHOSTUIINFO );
		pInfo->dwFlags = DOCHOSTUIFLAG_DIALOG | DOCHOSTUIFLAG_DISABLE_HELP_MENU | DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE |
			DOCHOSTUIFLAG_FLAT_SCROLLBAR;
		if ( !this->showScrollBar )
		{
			pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO;
		}
		return S_OK;
	}


	STDMETHODIMP BrowserControlPane::GetOptionKeyPath( LPOLESTR* pchKey, DWORD dwReserved )
	{
		Log( _T( "BrowserControlPane::GetOptionKeyPath\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::HideUI( void )
	{
		Log( _T( "BrowserControlPane::HideUI\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::OnDocWindowActivate( BOOL fActive )
	{
		Log( _T( "BrowserControlPane::OnDocWindowActivate\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::OnFrameWindowActivate( BOOL fActive )
	{
		Log( _T( "BrowserControlPane::OnFrameWindowActivate\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::ResizeBorder( LPCRECT prcBorder, IOleInPlaceUIWindow FAR* pUIWindow, BOOL fFrameWindow )
	{
		Log( _T( "BrowserControlPane::ResizeBorder\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::ShowContextMenu( DWORD dwID, POINT FAR* ppt, IUnknown FAR* pcmdTarget, IDispatch FAR* pdispObject )
	{
		Log( _T( "BrowserControlPane::ShowContextMenu\n" ) );
		return S_OK;
	}


	STDMETHODIMP BrowserControlPane::ShowUI( DWORD dwID, IOleInPlaceActiveObject FAR* pActiveObject, IOleCommandTarget FAR* pCommandTarget, IOleInPlaceFrame FAR* pFrame, IOleInPlaceUIWindow FAR* pDoc )
	{
		Log( _T( "BrowserControlPane::ShowUI\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::TranslateAccelerator( LPMSG lpMsg, const GUID FAR* pguidCmdGroup, DWORD nCmdID )
	{
		Log( _T( "BrowserControlPane::TranslateAccelerator\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::TranslateUrl( DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut )
	{
		Log( _T( "BrowserControlPane::TranslateUrl\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::UpdateUI( void )
	{
		Log( _T( "BrowserControlPane::UpdateUI\n" ) );
		return E_NOTIMPL;
	}


	STDMETHODIMP BrowserControlPane::ShowHelp( HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand, DWORD dwData, POINT ptMouse, IDispatch* pDispatchObjectHit )
	{
		Log( _T( "BrowserControlPane::ShowHelp\n" ) );
		return S_OK;
	}


	STDMETHODIMP BrowserControlPane::ShowMessage( HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT* plResult )
	{
		Log( _T( "BrowserControlPane::ShowMessage\n" ) );
		return S_OK;
	}


	LRESULT CALLBACK BrowserControlPane::WindowProcEntry( HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam )
	{
		BrowserControlPane*	pThis;

		pThis = ( BrowserControlPane* ) ::GetWindowLongA( hWindow, GWL_USERDATA );
		if ( pThis == NULL )
		{
			pThis = s_pInCreation;
		}
		ASSERT( pThis != NULL );
		return pThis->WindowProc( hWindow, iMessage, wParam, lParam );
	}


	LRESULT BrowserControlPane::WindowProc( HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam )
	{

		switch( iMessage )
		{
		case WM_PAINT:
			{
				PAINTSTRUCT	paintStruct;

				::BeginPaint( hWindow, &paintStruct );
				::EndPaint( hWindow, &paintStruct );
			}
			return 0;

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			{
				CComQIPtr< IOleInPlaceActiveObject, &IID_IOleInPlaceActiveObject >	pInPlaceActiveObject( this->pBrowser );
				MSG	msg;

				if ( wParam == VK_TAB )
				{
					CComQIPtr< IOleObject, &IID_IOleObject >	pOleObject( this->pBrowser );
					RECT	rect;

					::GetClientRect( this->hwnd, &rect );
					ASSERT( pOleObject != NULL );
					pOleObject->DoVerb( OLEIVERB_UIACTIVATE, NULL, this, 0, this->hwnd, &rect );
				}

				msg.message = iMessage;
				msg.wParam = wParam;
				msg.lParam = lParam;
				ASSERT( pInPlaceActiveObject != NULL );
				pInPlaceActiveObject->TranslateAccelerator( &msg );
			}
			return 0;

		default:
			break;
		}

		return DefWindowProc( hWindow, iMessage, wParam, lParam );
	}

}

ADAD::BrowserControlPane* bcp;
const ADBallonParam* padp;
BOOL ADBalloonAppActive = FALSE;
BOOL ADAppLoopFlag = FALSE;

LRESULT CALLBACK ADBallonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CREATE:
			RECT r;
			r.left = 0;	r.right = padp->width;
			r.top = 0;	r.bottom = padp->height;
			bcp = new ADAD::BrowserControlPane(hwnd, padp->hInst, &r);
			bcp->NavigateTo(padp->URL);
			bcp->ActivateWindow(TRUE);

			SetTimer(hwnd, 0xAA, padp->timeout * 1000, NULL);
			break;

		case WM_NCHITTEST:
			return HTCLIENT;
		
		case WM_TIMER:
			if (wParam == 0xFF) KillTimer(hwnd, 0xFF);
			KillTimer(hwnd, 0xAA);
			DestroyWindow(hwnd);
			break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) DestroyWindow(hwnd);
			break;

		case WM_USER:
			SetTimer(hwnd, 0xFF, 10, NULL);
			break;

		case WM_CLOSE:
			bcp->ActivateWindow(FALSE);
			DestroyWindow(hwnd);
			ADAppLoopFlag = FALSE;
			break;
		
		case WM_DESTROY:
			delete bcp;
			PostQuitMessage(0);
			ADAppLoopFlag = FALSE;
			break;

		case WM_ACTIVATEAPP:			
			if(wParam == FALSE)
			{
				ADBalloonAppActive = FALSE;
			}
			else
			{
				ADBalloonAppActive = TRUE;
			}

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	
	return 0;
}

int ShowADBalloon(const ADBallonParam& adp)
{
	::OleInitialize( NULL );
	padp = &adp;
	
	// Register Class

	WNDCLASSEX wcl =
	{ 
		sizeof(wcl), CS_OWNDC | CS_DBLCLKS, ADBallonProc, NULL, NULL, adp.hInst, NULL, NULL,
			(HBRUSH)COLOR_BACKGROUND, NULL, "NexonADBallon", NULL 
	};
	if (!RegisterClassEx(&wcl)) return 0;

	// Create and Show Window

	int		width  = adp.width + GetSystemMetrics(SM_CXDLGFRAME) * 2;
	int		height = adp.height + GetSystemMetrics(SM_CYDLGFRAME) * 2;
	RECT	sRect; GetWindowRect(GetDesktopWindow(), &sRect);

	HWND hwnd = 
		CreateWindowEx(WS_EX_TOPMOST /*| WS_EX_TOOLWINDOW*/, "NexonADBallon", "9D AD window", WS_POPUP | WS_THICKFRAME, //WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, 
		(sRect.right - width)/2, (sRect.bottom - height)/2, width, height, NULL, NULL, adp.hInst, NULL);

	if (!hwnd) return 0;
	
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// Message Loop
	ADAppLoopFlag = TRUE;

	MSG msg;
	//BOOL bGotMsg;	
	//PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

	while(ADAppLoopFlag)
	{
		/*
		if( ADBalloonAppActive )
			bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
		else
			bGotMsg = GetMessage(&msg, NULL, 0, 0);
			*/

		GetMessage(&msg, NULL, 0, 0);
		
		//if(bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	::OleUninitialize();
	return 1;
}
