/************************************************************************
SkinEdge.vsh
Copyright (C) 2002-2004, Indy21
*************************************************************************/

;모델의 외곽선을 그리기 위해서 사용한다.
;toon skinnig 에니메이션하고 엣지를 그린다.
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


;blend between transformed Normal r3 and r4
;r4 Final Transformed Normal
mul r3.xyz, r3.xyz, v3.x 
mad r4, r4.xyz, v3.y, r3.xyz

; normalize Transformed Normal r4 
dp3 r4.w, r4, r4
rsq r4.w, r4.w
mul r4, r4, r4.w

mul     r0   , r4   , c[9].zzzw ; r0  = Normal * LineSize                  
add     r1   , r2   , r0					; r1  = Vertex + ( Normal * LineSize )  

dp4 oPos.x, r1, c[CV_WORLDVIEWPROJ_0]		; Pos = r1 * World * View * Projection
dp4 oPos.y, r1, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, r1, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, r1, c[CV_WORLDVIEWPROJ_3]

mov     oD0  , c[17]						;c17 외곽선 칼라


