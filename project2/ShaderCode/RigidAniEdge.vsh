/************************************************************************
RigidAniEdge.vsh
Copyright (C) 2002-2004, Indy21
*************************************************************************/

;모델의 외곽선을 그리기 위해서 사용한다.
;Rigid 에니메이션하고 엣지를 그린다.
vs.1.1

dcl_position0		v0             
dcl_normal			v1		
dcl_texcoord0		v2	
dcl_texcoord1		v3

; v3에 웨이트 정보와 인덱스 정보가 있다
; FLOAT weight0 = v3.x, weight1 = v3.y;	
; FLOAT index0 = v3.z,   index1 = v3.w;  //버텍스당 2개의 본 웨이트와 그에 상당한 본의 인덱스

#include "../MeshControl/XVSAConstants.h"

dp4 r1.x, v0, c[20] 
dp4 r1.y, v0, c[21] 
dp4 r1.z, v0, c[22] 
mov r1.w, c[CV_CONSTANTS].z //set w to 1.0f 

; 1th Normal transform
dp3 r2.x, v1, c[20] 
dp3 r2.y, v1, c[21] 
dp3 r2.z, v1, c[22] 

; normalize Transformed Normal r2 
dp3 r2.w, r2, r2
rsq r2.w, r2.w
mul r2, r2, r2.w

mul     r0   , r2   , c[9].zzzw ; r0  = Normal * LineSize                  
add     r1   , r1   , r0					; r2  = Vertex + ( Normal * LineSize )  

dp4 oPos.x, r1, c[CV_WORLDVIEWPROJ_0]		; Pos = r1 * World * View * Projection
dp4 oPos.y, r1, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, r1, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, r1, c[CV_WORLDVIEWPROJ_3]

mov     oD0  , c[17]						;c17 외곽선 칼라


