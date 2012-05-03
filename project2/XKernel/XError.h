#ifndef __ERROR_H__
#define __ERROR_H__
#ifdef __cplusplus

#include "xbasedefs.h"

#define LOG_DXERROR()   _XLogError("[D3D9] : %s", GetD3DErrorString(hr))

// Return a string from a D3D error code.
TCHAR *GetD3DErrorString(HRESULT hr, TCHAR *buf = NULL);

// Show an error message and leave the application.
void _XFatalError(TCHAR *str, ...);

#endif // __cplusplus
#endif // __ERROR_H__
