/************************************************************************
PaletteSkin.vsh
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
dcl_texcoord2		v4		; S
dcl_texcoord3		v5      ; T
dcl_texcoord4		v6      ; S*T

; v3에 웨이트 정보과 인덱스 정보가 있다
; FLOAT weight0 = v3.x, weight1 = v3.y;	
; FLOAT index0 = v3.z,   index1 = v3.w;  //버텍스당 2개의 본 웨이트와 그에 상당한 본의 인덱스

#include "../MeshControl/XVSAConstants.h"

; first bone results -> r1
mov a0.x, v3.z 

dp4 r1.x, v0, c[a0.x + 0] 
dp4 r1.y, v0, c[a0.x + 1] 
dp4 r1.z, v0, c[a0.x + 2] 

; 1th Normal transform
dp3 r3.x, v1, c[a0.x + 0] 
dp3 r3.y, v1, c[a0.x + 1] 
dp3 r3.z, v1, c[a0.x + 2] 

//transform S
dp3 r7.x, v4, c[a0.x + 0]
dp3 r7.y, v4, c[a0.x + 1]
dp3 r7.z, v4, c[a0.x + 2]

//transform T
dp3 r9.x, v5, c[a0.x + 0]
dp3 r9.y, v5, c[a0.x + 1]
dp3 r9.z, v5, c[a0.x + 2]

//transform SxT
dp3 r11.x, v6, c[a0.x + 0]
dp3 r11.y, v6, c[a0.x + 1]
dp3 r11.z, v6, c[a0.x + 2]

//second bone results -> r2
mov a0.x, v3.w 

dp4 r2.x, v0, c[a0.x + 0] 
dp4 r2.y, v0, c[a0.x + 1] 
dp4 r2.z, v0, c[a0.x + 2] 

; 2th Normal transform
dp3 r4.x, v1, c[a0.x + 0] 
dp3 r4.y, v1, c[a0.x + 1] 
dp3 r4.z, v1, c[a0.x + 2] 

//2th transform S
dp3 r8.x, v4, c[a0.x + 0]
dp3 r8.y, v4, c[a0.x + 1]
dp3 r8.z, v4, c[a0.x + 2]

//2th transform T
dp3 r10.x, v5, c[a0.x + 0]
dp3 r10.y, v5, c[a0.x + 1]
dp3 r10.z, v5, c[a0.x + 2]

//2th transform SxT
dp3 r0.x, v6, c[a0.x + 0]
dp3 r0.y, v6, c[a0.x + 1]
dp3 r0.z, v6, c[a0.x + 2]

//blend between transformed Position r1 and r2
mul r1.xyz, r1.xyz, v3.x 
mad r2, r2.xyz, v3.y, r1.xyz
mov r2.w, c[CV_CONSTANTS].z //set w to 1.0f

;ftp://221.147.34.134
//r2 now contains final position
// transform to clip space
dp4 oPos.x, r2, c[CV_WORLDVIEWPROJ_0]
dp4 oPos.y, r2, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, r2, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, r2, c[CV_WORLDVIEWPROJ_3]

;blend between transformed Normal r3 and r4
;r4 Final Transformed Normal
mul r3.xyz, r3.xyz, v3.x 
mad r4, r4.xyz, v3.y, r3.xyz

; normalize Transformed Normal r4 
dp3 r4.w, r4, r4
rsq r4.w, r4.w
mul r4, r4, r4.w

//blend between r7 and r8 -- S
; r8 Final Transformed --- S
mul r7, r7.xyz, v3.x
mad r8, r8.xyz, v3.y, r7

;r10 now contains final T
//blend between r11 and r12 -- T
mul r9, r9.xyz, v3.x
mad r10, r10.xyz, v3.y, r9

;r0 now Contains final S CrosProduct S*T
//blend between r11 and r12 -- SxT
mul r11, r11.xyz, v3.x
mad r0, r0.xyz, v3.y, r11

; Transformed normalize S = r8
dp3 r8.w, r8, r8
rsq r8.w, r8.w
mul r8, r8, r8.w

; Transformed normalize T = r10
dp3 r10.w, r10, r10
rsq r10.w, r10.w
mul r10, r10, r10.w

; Transformed normalize S*T = r0
dp3 r0.w, r0, r0
rsq r0.w, r0.w
mul r0, r0, r0.w

//transform light by skinned basis vector r8, r10, and r0
dp3 r1.x, r8.xyz, -c[CV_LIGHT_DIRECTION];-c4
dp3 r1.y, r10.xyz, -c[CV_LIGHT_DIRECTION]; -c4
dp3 r1.z, r0.xyz, -c[CV_LIGHT_DIRECTION]; -c4

//normalize light vector
dp3 r1.w, r1, r1
rsq r1.w, r1.w
mul r1, r1, r1.w

//pass through texcoords
mov oT0.xy, v2
mov oT1.xy, v2

mad oD0, r1, c5.y, c5.z
;mad oD0, r1,
//scale to 0-1

//[-1, 1] --> [0, 2]
;add r1, r1, c5.z ;[CV_CONSTANTS].z = 1.0

//[0, 2] --> [0, 1] and add in a bit of ambient
;mad oD0, r1, c5.y, c5.w ;c[CV_CONSTANTS].y = 0.5, c[CV_CONSTANTS].w = 0.2



