/************************************************************************
Toon.vsh
Copyright (C) 2002-2004, Indy21
*************************************************************************/
vs.1.1

#include "../MeshControl/XVSAConstants.h"

dcl_position0   v0
dcl_normal0		v1             
dcl_texcoord0   v2  

;Do the final transformation and emit it in oPos. This
;is completely independent of the lighting operations
dp4 oPos.x, v0, c[CV_WORLDVIEWPROJ_0]
dp4 oPos.y, v0, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, v0, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, v0, c[CV_WORLDVIEWPROJ_3]

mov oT1.xy, v2 ; 베이스 텍스쳐

;Compute the Vector from eye to vertex in object space
;and store the vector in r1
add r1, c8, -v0 ;c8 EyePosition

;Normalize the eye vector(From VertexPo to EyePos)
dp3 r1.w, r1, r1
rsq r1.w, r1.w
mul r1, r1, r1.w

;Compute half angle between the eye vector and the light
;vector. The result in stored in r4 HalfVector
add r4, r1, c[CV_LIGHT_DIRECTION] 
dp3 r4.w, r4, r4
rsq r4.w, r4.w
mul r4, r4, r4.w

;Find N dot H and store the result in r0.y
dp3 oT0.x, r4, v1
mov oT0.y, c5.y ;

;Final color
mov oD0, c[11] ;CV_VDIFFUSE_COLOR

