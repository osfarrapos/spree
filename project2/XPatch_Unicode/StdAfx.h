// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F82AB9C5_0F49_47C0_B535_CA5EC6318573__INCLUDED_)
#define AFX_STDAFX_H__F82AB9C5_0F49_47C0_B535_CA5EC6318573__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#pragma warning(disable:4786)

#include "XRegistry.h"

extern _XRegistryManager	g_RegManager;

#endif // !defined(AFX_STDAFX_H__F82AB9C5_0F49_47C0_B535_CA5EC6318573__INCLUDED_)
