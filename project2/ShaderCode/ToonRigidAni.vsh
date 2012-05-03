/************************************************************************
ToonRigidAni.vsh
목걸이나 모자 같이 메쉬에 피지크가 안걸려 있는
메쉬를 에니 메이션 시킬때. 특정 본의 에니메이션 값을
가지고 메쉬를 에니메이션 시킨다.(일종의 Rigid 에니메이션)
카툰 에니메이션
Copyright (C) 2002-2004, Indy21
*************************************************************************/

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

#include "../MeshControl/XVSAConstants.h"

; 셰이더 상수 20번 부터 본 메트릭스가 들어 가므로 
dp4 r1.x, v0, c[20] 
dp4 r1.y, v0, c[21] 
dp4 r1.z, v0, c[22]
mov r1.w, c[CV_CONSTANTS].z //set w to 1.0f 

; Normal transform
dp3 r2.x, v1, c[20] 
dp3 r2.y, v1, c[21] 
dp3 r2.z, v1, c[22] 

//r1 now contains final position
// transform to 투영 공간
dp4 oPos.x, r1, c[CV_WORLDVIEWPROJ_0]
dp4 oPos.y, r1, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, r1, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, r1, c[CV_WORLDVIEWPROJ_3]

mov oT1.xy, v2 ; 베이스 텍스쳐

; normalize Transformed Normal r4 
dp3 r2.w, r2, r2
rsq r2.w, r2.w
mul r2, r2, r2.w

;Compute the Vector from eye to vertex in object space
;and store the vector in r3
add r3, c8, -v0 ;c8 EyePosition

;Normalize the eye vector(From VertexPo to EyePos)
dp3 r3.w, r3, r3
rsq r3.w, r3.w
mul r3, r3, r3.w

;Compute half angle between the eye vector and the light
;vector. The result in stored in r4 HalfVector
add r4, r3, c[CV_LIGHT_DIRECTION]; -c4
dp3 r4.w, r4, r4
rsq r4.w, r4.w
mul r4, r4, r4.w

;Find N dot H and store the result in r0.y
dp3 r0.y, r4,  r2 ;v1

mov oT0.x, r0.y ;
mov oT0.y, c5.y    ;

;Final color
mov oD0, c[11] ;CV_VDIFFUSE_COLOR




