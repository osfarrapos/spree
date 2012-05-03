// XRenderStateControl.h: interface for the _XRenderStateControl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XRENDERSTATECONTROL_H_
#define _XRENDERSTATECONTROL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XRenderStateControl  
{
public:
	DWORD fillmode;
	BOOL  fogenable;
	
public:
	_XRenderStateControl();
	virtual ~_XRenderStateControl();

	void SetRenderState( D3DRENDERSTATETYPE statetype, DWORD value1 = 0, DWORD value2 = 0, DWORD value3 = 0 );

	void ProcCommand( LPSTR commandstring );
};

#endif // !defined(_XRENDERSTATECONTROL_H_)
