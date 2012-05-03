#define WORS_SUCCESS	0
#define WORS_TIMEOUT	-1
#define WORS_DESTROYED	-2

static const SAFEARRAYBOUND _XArrayBound = {1, 0};

BSTR WINAPI _XTStr2BStr(HWND hwnd, const char *string)
{
	BSTR	bstr;

	if (!IsWindowUnicode(hwnd))
	{
		WCHAR		*buffer;
		DWORD		size;

		size = MultiByteToWideChar(CP_ACP, 0, (char *)string, -1, 0, 0);
		if (!(buffer = (WCHAR *)GlobalAlloc(GMEM_FIXED, sizeof(WCHAR) * size))) return(0);
		MultiByteToWideChar(CP_ACP, 0, (char *)string, -1, buffer, size);
		bstr = SysAllocString(buffer);
		GlobalFree(buffer);
	}
	else
		bstr = SysAllocString((WCHAR *)string);

	return(bstr);
}


static void _XDoEvents(HWND hwnd)
{
	MSG		msg;

	while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}
}

HRESULT WINAPI _XGetWebPtrs(HWND hwnd, IWebBrowser2 **webBrowser2Result, IHTMLDocument2 **htmlDoc2Result)
{
	IOleObject		*browserObject;
	IWebBrowser2	*webBrowser2;

	// Make sure he supplied at least one of the return handles. If not, we
	// have nothing to do here
	if (webBrowser2Result || htmlDoc2Result)
	{
		// Make sure he supplied a window
		if (!IsWindow(hwnd) ||

		// Get the browser object stored in the window's USERDATA member
		!(browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA))) ||

		// Get the IWebBrowser2 object embedded within the browser object
		browserObject->QueryInterface(IID_IWebBrowser2, (void **)&webBrowser2))
		{
			goto fail;
		}

		// Now the pointer to our IWebBrowser2 object is in 'webBrowser2', and so its VTable is
		// webBrowser2->lpVtbl.

		// Does the caller want the IHTMLDocument2 object for the browser?
		if (htmlDoc2Result)
		{
			LPDISPATCH		lpDispatch;

			// Set his handle to 0 initially
			*htmlDoc2Result = (struct IHTMLDocument2 *)0;
			lpDispatch = 0;

			// First, we need the IDispatch object
			webBrowser2->get_Document(&lpDispatch);
			if (lpDispatch)
			{
				// Get the IHTMLDocument2 object embedded within the IDispatch object
				lpDispatch->QueryInterface(IID_IHTMLDocument2, (void **)htmlDoc2Result);

				// Release the IDispatch object now that we have the IHTMLDocument2
				lpDispatch->Release();
			}

			// If we failed to get IHTMLDocument2, then free the IWebBrowser2 and
			// return an error to the caller
			if (!(*htmlDoc2Result))
			{
				webBrowser2->Release();
fail:			return(E_FAIL);
			}
		}

		// If the caller wants the IWebBrowser2 returned, store it in his supplied handle.
		// Note: We do not Release it here. The caller must do that when he is done with it
		if (webBrowser2Result)
			*webBrowser2Result = webBrowser2;

		// If he doesn't want it returned, we need to release it here
		else
			webBrowser2->Release();
	}

	return(S_OK);
}


HRESULT WINAPI _XWaitOnReadyState(HWND hwnd, READYSTATE rs, DWORD timeout, IWebBrowser2 *webBrowser2)
{
	READYSTATE		rsi;
	DWORD			dwStart;
	unsigned char	releaseOnComplete;

	// If the caller didn't supply the IWebBrowser2, then we need to get it (and
	// release it when we're done)
	releaseOnComplete = 0;
	if (!webBrowser2)
	{
		if (_XGetWebPtrs(hwnd, &webBrowser2, 0)) goto destroyed;
		releaseOnComplete = 1;
	}

	// Get the current ready state of the loading page
	webBrowser2->get_ReadyState(&rsi);

	// Is the current ready state at least as high as the caller needs?
	if (rsi >= rs)
	{
		// Yes, it is. Tell the caller that the page is in a state where
		// he can proceed with whatever he wants to do.
yes:	rs = (READYSTATE)WORS_SUCCESS;

		// If we got the IWebBrowser2 ourselves above, release it now.
out:	if (releaseOnComplete) webBrowser2->Release();

		return((HRESULT)rs);
	}

	// Ok, the loading page is not yet in the state that the caller
	// requires. We need to timeout. We can't just Sleep() for the
	// specified amount of time. Why? Because a page will not load
	// unless we are emptying out certain messages in our thread's
	// message queue. So we need to at least call doEvents() periodically
	// while we are waiting for the ready state to be achieved.
	dwStart = GetTickCount();
	do
	{
		// Empty out messages in the message queue.
		_XDoEvents(hwnd);

		// Make sure our window with the browser object wasn't closed down in while processing messages.
		if (!IsWindow(hwnd))
		{
			// Oops! It was. Get out of here with WORS_DESTROYED.
destroyed:	rs = (READYSTATE)WORS_DESTROYED;
			goto out;
		}

		// Is the current ready state at least as high as the caller needs?
		webBrowser2->get_ReadyState(&rsi);
		if (rsi >= rs) goto yes;

		// We may need a sleep here on Win9x/Me systems to avoid a system hang.
		Sleep(10);

		// Did we timeout yet?
	} while (!timeout || (GetTickCount() - dwStart) <= timeout);

	// We timed out before the page achieved the desired ready state.
	rs = (READYSTATE)WORS_TIMEOUT;
	goto out;
}


long WINAPI _XDisplayHTMLStr(HWND hwnd, LPCTSTR webPageName, const char *string)
{	
	IHTMLDocument2	*htmlDoc2;
	IWebBrowser2	*webBrowser2;
	SAFEARRAY		*sfArray;
	VARIANT			myURL;
	VARIANT			*pVar;

	VariantInit(&myURL);
	myURL.vt = VT_BSTR;

	wchar_t			*buffer;
	DWORD			size;
	size = MultiByteToWideChar(CP_ACP, 0, webPageName, -1, 0, 0);
	if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size))) 
	{
		_XLog( "_XDisplayHTMLStr::Can't allocation memory" );
	}
	MultiByteToWideChar(CP_ACP, 0, webPageName, -1, buffer, size);
	myURL.bstrVal = SysAllocString(buffer);
	GlobalFree(buffer);	

	if (!_XGetWebPtrs(hwnd, &webBrowser2, 0))
	{
		webBrowser2->Navigate2(&myURL, 0, 0, 0, 0);

		if (_XWaitOnReadyState(hwnd, READYSTATE_COMPLETE, 5000, webBrowser2) != WORS_DESTROYED)
		{
			SysFreeString(myURL.bstrVal);

			if (!_XGetWebPtrs(hwnd, 0, &htmlDoc2))
			{
				if ((sfArray = SafeArrayCreate(VT_VARIANT, 1, (SAFEARRAYBOUND *)&_XArrayBound)))
				{
					if (!SafeArrayAccessData(sfArray, (void **)&pVar))
					{
						pVar->vt = VT_BSTR;

						// Store our BSTR pointer in the VARIENT.
						if ((pVar->bstrVal = _XTStr2BStr(hwnd, string)))
						{
							// Pass the VARIENT with its BSTR to write() in order to shove our desired HTML string
							// into the body of that empty page we created above.
							htmlDoc2->write(sfArray);

							// Close the document. If we don't do this, subsequent calls to DisplayHTMLStr
							// would append to the current contents of the page
							htmlDoc2->close();

							// Success. Just set this to something other than VT_BSTR to flag success
							++myURL.vt;

							// Normally, we'd need to free our BSTR, but SafeArrayDestroy() does it for us
	//						SysFreeString(pVar->bstrVal);
						}

						// Free the array. This also frees the VARIENT that SafeArrayAccessData created for us,
						// and even frees the BSTR we allocated with SysAllocString
						SafeArrayDestroy(sfArray);
					}
				}

				// Release the IHTMLDocument2 object.
				htmlDoc2->Release();
			}
		}
		else
			SysFreeString(myURL.bstrVal);

		// Release the IWebBrowser2 object.
		webBrowser2->Release();
	}

	// No error?
	if (myURL.vt != VT_BSTR) return(0);

	// An error
	return(-1);
}

IHTMLElement * WINAPI _XGetWebElement(HWND hwnd, IHTMLDocument2 *htmlDoc2, const char *name, INT nIndex)
{
	IHTMLElementCollection *	htmlCollection;
	IHTMLElement *				htmlElem;
	LPDISPATCH					lpDispatch;

	lpDispatch = 0;
	htmlElem = 0;

	// Get the browser's IHTMLDocument2 object if it wasn't passed
	if (htmlDoc2 || !_XGetWebPtrs(hwnd, 0, &htmlDoc2))
	{
		// Get the IHTMLElementCollection object. We need this to get the IDispatch
		// object for the element the caller wants on the web page. And from that
		// IDispatch, we get the IHTMLElement object. Really roundabout, ain't it?
		// That's COM
		if (!htmlDoc2->get_all(&htmlCollection) && htmlCollection)
		{
			VARIANT			varName;
			VARIANT			varIndex;

			// Get the IDispatch for that element. We need to call the IHTMLElementCollection
			// object's item() function, passing it the name of the element. Note that we
			// have to pass the element name as a BSTR stuffed into a VARIENT struct. And
			// we also need to stuff the index into a VARIENT struct too.
			VariantInit(&varName);
			varName.vt = VT_BSTR;
			if ((varName.bstrVal = _XTStr2BStr(hwnd, name)))
			{
				VariantInit(&varIndex);
				varIndex.vt = VT_I4;
				varIndex.lVal = nIndex;

				htmlCollection->item(varName, varIndex, &lpDispatch);

				// We don't need the VARIENTs anymore. This frees the string that SysAllocString() gave us
				VariantClear(&varName);
				VariantClear(&varIndex);
			}

			// Release the IHTMLElementCollection now that we're done with it.
			htmlCollection->Release();
			
			// Did we get the IDispatch for that element?
			if (lpDispatch)
			{
				// We can finally get the IHTMLElement object for the desired object.
				lpDispatch->QueryInterface(IID_IHTMLElement, (void **)&htmlElem);

				// Release the IDispatch now that we got the IHTMLElement.
				lpDispatch->Release();
			}
		}
	}

	// Return the IHTMLElement ptr.
	return(htmlElem);
}

long WINAPI _XCheckNullHTMLPage(HWND hwnd, LPCTSTR webPageName)
{
	TCHAR htmlstring[1024];
	memset( htmlstring, 0, sizeof(TCHAR) * 1024 );
	long retcode = _XDisplayHTMLStr( hwnd, webPageName, htmlstring );

	FILE* pFile = fopen("aaa.txt","wb");

	fwrite( htmlstring, strlen(htmlstring), 1, pFile );

	fclose(pFile);

	return retcode;
}