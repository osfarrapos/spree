/************************************************************************
ToonpaletteSkin.vsh
Copyright (C) 2002-2004, Indy21 카툰스키닝 에니메이션
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
; v1      = Normal
; v2	  = ToonShading Texture 	
; v3	  = Model's Base Texture
; oD1	  = 버텍스 칼라
; oT0	  = ToonShading Texture coordinates
; 0T1	  = Model's Base Texture coordinates	
;------------------------------------------------------------------------------

vs.1.1

dcl_position0		v0             
dcl_normal			v1		
dcl_texcoord0		v2	
dcl_texcoord1		v3

; v3에 웨이트 정보와 인덱스 정보가 있다
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

//second bone results -> r2
mov a0.x, v3.w 

dp4 r2.x, v0, c[a0.x + 0] 
dp4 r2.y, v0, c[a0.x + 1] 
dp4 r2.z, v0, c[a0.x + 2] 

; 2th Normal transform
dp3 r4.x, v1, c[a0.x + 0] 
dp3 r4.y, v1, c[a0.x + 1] 
dp3 r4.z, v1, c[a0.x + 2] 

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

mov oT1.xy, v2 ; 베이스 텍스쳐

;blend between transformed Normal r3 and r4
;r4 Final Transformed Blend Normal
mul r3.xyz, r3.xyz, v3.x 
mad r4, r4.xyz, v3.y, r3.xyz

; normalize Transformed Normal r4 
dp3 r4.w, r4, r4
rsq r4.w, r4.w
mul r4, r4, r4.w

;Compute the Vector from eye to vertex in object space
;and store the vector in r1
add r2, c8, -v0 ;c8 EyePosition

;Normalize the eye vector(From VertexPo to EyePos)
dp3 r2.w, r2, r2
rsq r2.w, r2.w
mul r2, r2, r2.w

;Compute half angle between the eye vector and the light
;vector. The result in stored in r3 HalfVector
add r3, r2, c[CV_LIGHT_DIRECTION]; -c4
dp3 r3.w, r3, r3
rsq r3.w, r3.w
mul r3, r3, r3.w

;Find N dot H and store the result in r0.y
dp3 r0.y, r3,  r4 ;v1

mov oT0.x, r0.y ;
mov oT0.y, c5.y    ;

;Final color
mov oD0, c[11] ;CV_VDIFFUSE_COLOR




