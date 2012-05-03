/************************************************************************
ModelShadow.vsh
Copyright (C) 2002-2004, Indy21
*************************************************************************/
vs.1.1

#include "../MeshControl/XVSAConstants.h"

dcl_position0   v0
dcl_normal0		v1             
dcl_texcoord0   v2		

// transform to clip space
dp4 oPos.x, V_POSITION, c[CV_WORLDVIEWPROJ_0]
dp4 oPos.y, V_POSITION, c[CV_WORLDVIEWPROJ_1]
dp4 oPos.z, V_POSITION, c[CV_WORLDVIEWPROJ_2]
dp4 oPos.w, V_POSITION, c[CV_WORLDVIEWPROJ_3]

