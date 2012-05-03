#ifndef __PLATFORMTYPE
#define __PLATFORMTYPE

#include <windows.h>
#include <windowsx.h>	
#include <ddraw.h>
#include <d3d.h>
#include <d3drm.h>
#include <dsound.h>
#include <d3dtypes.h>
#include <dinput.h>
#include "math.h"
// TIME Function Platform dependent
#include <mmsystem.h>

#include "myassert.h"
#include "funcalloc.h"
#include "btypes.h"

// info needed to initialize low input system
typedef struct {//main application hinstance and window handle
	HINSTANCE hInst;
	HWND hWnd;
} IPLATF_INFO;

typedef HWND PLATFHWND;

#endif