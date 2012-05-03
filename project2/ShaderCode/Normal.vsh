/************************************************************************
Normal.vsh
Copyright (C) 2002-2004, Indy21
*************************************************************************/

;------------------------------------------------------------------------------
; oPos	  = Output position
; oD0	  = Diffuse
;------------------------------------------------------------------------------

vs.1.1

dcl_position0		v0             
dcl_color0			v1		

#include "../MeshControl/XVSAConstants.h"

; transform to clip space
dp4 oPos.x, v0, c[CV_WORLDVIEWPROJ_0]
dp4 oPos.y, v0, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, v0, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, v0, c[CV_WORLDVIEWPROJ_3]

; move vertex diffuse Color
mov oD0, v1



