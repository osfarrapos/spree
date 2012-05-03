/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __TYPES_H__
#define __TYPES_H__

namespace Audio
{

// basic user type definitions
typedef unsigned __int64	uint64;
typedef __int64				int64;
typedef unsigned long		uint32;
typedef long				int32;
typedef unsigned short		uint16;
typedef short				int16;
typedef unsigned char		uint8;
typedef char				int8;

}; // namespace Audio

//! The audio system uses d3d vectors by default		
#define AUDIOVECTOR D3DVECTOR

#endif // __TYPES_H__