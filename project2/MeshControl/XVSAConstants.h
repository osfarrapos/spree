#ifndef _XVSACONSTANTS_H_
#define _XVSACONSTANTS_H_

//Constants for PaletteSkin

#define CV_WORLDVIEWPROJ_0	0
#define CV_WORLDVIEWPROJ_1	1
#define CV_WORLDVIEWPROJ_2	2
#define CV_WORLDVIEWPROJ_3	3

#define CV_LIGHT_DIRECTION	4

#define CV_CONSTANTS		5
#define CV_AMBIENT_COLOR	6
#define CV_LIGHT_DIFFUSE	7
#define CV_EYE_VECTOR		8
#define CV_CONSTANTS1		9		//x = 범프맵에서 값을 0~1사이로 편향시키기위해 사용되는 값 ,y = 스페큘러 파워 ,z=실루엣 사이즈 ;
#define CV_SPECULAR_COLOR	10		// 스페큘러 칼라
#define CV_VDIFFUSE_COLOR	11		// 버텍스 칼라

#define CV_WORLD_0			12
#define CV_WORLD_1			13
#define CV_WORLD_2			14
#define CV_WORLD_3			15

#define CV_SHDOWCOLOR		16
#define CV_SILLHOUETTECOLOR	17	//외곽선 칼라
#define CV_TOONCONST		18	// x = ToonTexture에서 의V 값 

#define CV_WORLDVIEW_0		12
#define CV_WORLDVIEW_1		13
#define CV_WORLDVIEW_2		14
#define CV_WORLDVIEW_3		15
//#define CV_WORLD_3			15

//each bone is three constants long, so bone 0 starts at 20, bone 1 starts at 21, etc.
#define CV_BONESTART 20

//v1.x = 0번째 웨이트  v1.y = 1첫번째 웨이트
//v2.x = 0번째 본 인덱스 v2.y = 1번째 본 인데쓰
//Vertex components
#define V_POSITION  v0
#define V_NORMAL	v1
#define V_INDICES   v2		
#define V_TEX       v4
#define V_S         v5
#define V_T         v6
#define V_SxT       v7
#define V_WEIIND	v3		//x = 0th웨이트 y = 1th웨이트  v2.z = 0th본IDX  v2.y = 1th본IDX

#endif