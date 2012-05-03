/************************************************************************
PaletteSkinBumpMap.vsh
Copyright (C) 2002-2004, Indy21
*************************************************************************/

// CV_BONESTART + index is the start of the bones
//
// CV_LIGHT_DIRECTION is the light vector
// CV_WORLDVIEWPROJ_0-3 is the world-view-proj matrix
//
// CV_CONSTANTS = (0.0f, 0.5f, 1.0f, 0.2f)
// CV_CONSTANTS1 = (0.0f, 0.0f, 스페큘러 파워, 0.0f)
//

;------------------------------------------------------------------------------
; oPos	  = Output position
; oD0	  = Diffuse
; oD1	  = Specular
; oT0	  = texture coordinates
;------------------------------------------------------------------------------

vs.1.1

dcl_position0		v0             
dcl_normal			v1		
dcl_texcoord0		v2	
dcl_texcoord1		v3
;dcl_texcoord2		v4		; S
;dcl_texcoord3		v5      ; T
;dcl_texcoord4		v6      ; S*T

; v3에 웨이트 정보과 인덱스 정보가 있다
; FLOAT weight0 = v3.x, weight1 = v3.y;	
; FLOAT index0 = v3.z,   index1 = v3.w;  //버텍스당 2개의 본 웨이트와 그에 상당한 본의 인덱스

#include "../MeshControl/XVSAConstants.h"

; first bone results -> r1
mov a0.x, v3.z 

dp4 r1.x, v0, c[a0.x + 0] 
dp4 r1.y, v0, c[a0.x + 1] 
dp4 r1.z, v0, c[a0.x + 2] 

//second bone results -> r2
mov a0.x, v3.w 

dp4 r2.x, v0, c[a0.x + 0] 
dp4 r2.y, v0, c[a0.x + 1] 
dp4 r2.z, v0, c[a0.x + 2] 

//blend between transformed Position r1 and r2
mul r1.xyz, r1.xyz, v3.x 
mad r2, r2.xyz, v3.y, r1.xyz
mov r2.w, c[CV_CONSTANTS].z //set w to 1.0f

//r2 now contains final position
// transform to clip space
dp4 oPos.x, r2, c[CV_WORLDVIEWPROJ_0]
dp4 oPos.y, r2, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, r2, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, r2, c[CV_WORLDVIEWPROJ_3]



