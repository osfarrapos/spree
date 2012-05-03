/************************************************************************
SillouetteEdge.vsh
Copyright (C) 2002-2004, Indy21
*************************************************************************/

;모델의 외곽선을 그리기 위해서 사용한다.
vs.1.1

#include "../MeshControl/XVSAConstants.h"

dcl_position0   v0
dcl_normal0		v1             

mul     r0   , v1   , c[9].zzzw ; r0  = Normal * LineSize                  
add     r1   , v0   , r0					; r1  = Vertex + ( Normal * LineSize )     

;Do the final transformation and emit it in oPos. This
;is completely independent of the lighting operations

dp4 oPos.x, r1, c[CV_WORLDVIEWPROJ_0]		; Pos = r1 * World * View * Projection
dp4 oPos.y, r1, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, r1, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, r1, c[CV_WORLDVIEWPROJ_3]

mov     oD0  , c[17]						;c17 외곽선 칼라

