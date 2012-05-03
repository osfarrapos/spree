/************************************************************************
PaletteSkinBumpMap.vsh
Copyright (C) 2002-2004, Indy21
*************************************************************************/
vs.1.1

#include "../MeshControl/XVSAConstants.h"

dcl_position0   v0
dcl_normal0		v1             
dcl_texcoord0   v2		

// transform to clip space
dp4 oPos.x, v0, c[CV_WORLDVIEWPROJ_0]
dp4 oPos.y, v0, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, v0, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, v0, c[CV_WORLDVIEWPROJ_3]

;Dot product of L dot N
;and store it in r0.x for use with the lit instruction
dp3 r0.x, v1, -c[CV_LIGHT_DIRECTION]; 

;Compute the Vector from eye to vertex in object space
;and store the vector in r1
add r1, c8, -v0 ;c8 EyePosition

;Normalize the eye vector(From VertexPo to EyePos)
dp3 r1.w, r1, r1
rsq r1.w, r1.w
mul r1, r1, r1.w

;Compute half angle between the eye vector and the light
;vector. The result in stored in r2 HalfVector
add r2, r1, -c[CV_LIGHT_DIRECTION]; -c4
dp3 r2.w, r2, r2
rsq r2.w, r2.w
mul r2, r2, r2.w

;Find N dot H and store the result in r0.y
dp3 r0.y, r2, v1;r10

;Place the power in r0.w
mov r0.w, c9.y

;Find the final diffuse and specular intensities
lit r4, r0

;Compute the specular light value
mul r5, r4.z, c10	;CV_SPECULAR_COLOR

;Compute the diffuse light value and add the ambient light
mul r6, r4.y, c7	;CV_LIGHT_DIFFUSE
mov r8, c11			;CV_VDIFFUSE_COLOR
mad r6, r6, r8, c6	;c6 = CV_AMBIENT_COLOR

;Add the specular component and output the final color
add oD0, r5, r6
mov oT0.xy,  v2