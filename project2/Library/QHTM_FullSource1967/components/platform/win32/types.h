/*----------------------------------------------------------------------
Copyright (c) 1998,1999 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	types.h
Owner:	russf@gipsysoft.com
Purpose:	Win32 system types
----------------------------------------------------------------------*/
#ifndef TYPES_H
#define TYPES_H

	typedef long						PI_int64;
	typedef unsigned long		PI_uint64;

	typedef int							PI_int32;
	typedef unsigned int		PI_uint32;

	typedef short						PI_int16;
	typedef unsigned short	PI_uint16;

	typedef char						PI_int8;
	typedef unsigned char		PI_uint8;

	typedef unsigned char		PI_byte;

	typedef time_t					PI_time;

	typedef HICON						PI_icon;
	typedef HCURSOR					PI_cursor;
	typedef HBITMAP					PI_bitmap;
	typedef HWND						PI_window;

	#define FASTCALL __fastcall
	#define vsnprintf _vsnprintf
	#define tcstoul _tcstoul
	#define ultot _ultoa
	#define ttol _ttol
	#define istspace _istspace

#endif //TYPES_H