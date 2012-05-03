#ifndef __X_INLINE_H__
#define __X_INLINE_H__
//---------------------------------------------------------------------------
// Name: _XSwap()
// Desc: Swap two value.
//---------------------------------------------------------------------------
template<typename T> inline void _XSwap(T & a, T & b)
{
	T c = a; a = b; b = c;
}

//---------------------------------------------------------------------------
// Name: _XID()
// Desc: Create a 32 bit ID code from chars.
//---------------------------------------------------------------------------
#define _XID(a,b,c,d) ((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))

inline U32 _XIDStr(U8 *str)
{
	return _XID(str[0], str[1], str[2], str[3]);
}

//---------------------------------------------------------------------------
// Name: _XID*()
// Desc: Return the asked component
//---------------------------------------------------------------------------
inline U8 _XIDA(U32 id)
{
	return (id)&0xff;
}

inline U8 _XIDB(U32 id)
{
	return (id>>8)&0xff;
}

inline U8 _XIDC(U32 id)
{
	return (id>>16)&0xff;
}

inline U8 _XIDD(U32 id)
{
	return (id>>24)&0xff;
}

//---------------------------------------------------------------------------
// Name: _XIDString()
// Desc: Return the ID string.
//---------------------------------------------------------------------------
inline void _XIDString(U32 id, U8 *out)
{
	out[0] = _XIDA(id);
	out[1] = _XIDB(id);
	out[2] = _XIDC(id);
	out[3] = _XIDD(id);
}

// _XBitmap
inline void put_pixel(_XBitmap *bmp, int x, int y, U32 cl)
{
	bmp->vtbl->put_pixel(bmp, x, y, cl);
}

inline U32 get_pixel(_XBitmap *bmp, int x, int y)
{
	return bmp->vtbl->get_pixel(bmp, x, y);
}

inline void vline(_XBitmap *bmp, int x, int y1, int y2, U32 cl)
{
	bmp->vtbl->vline(bmp, x, y1, y2, cl);
}

inline void hline(_XBitmap *bmp, int x, int y, int x2, U32 cl)
{
	bmp->vtbl->hline(bmp, x, y, x2, cl);
}

inline void clear(_XBitmap *bmp)
{
	bmp->vtbl->clear(bmp, 0);
}

inline void clear_to_color(_XBitmap *bmp, U32 cl)
{
	bmp->vtbl->clear(bmp, cl);
}

//   COLORS
inline int _XGetR8(int c)
{
	return sys_palette[c].r;
}

inline int _XGetG8(int c)
{
	return sys_palette[c].g;
}

inline int _XGetB8(int c)
{
	return sys_palette[c].b;
}

inline int _XGetA8(int c)
{
	return 0;
}

inline int _XGetR12(int c)
{
	return ((c>>_rgb_r_shift_12)<<4) & 0xFF;
}

inline int _XGetG12(int c)
{
	return ((c>>_rgb_g_shift_12)<<4) & 0xFF;
}

inline int _XGetB12(int c)
{
	return ((c>>_rgb_b_shift_12)<<4) & 0xFF;
}

inline int _XGetA12(int c)
{
	return ((c>>_rgb_a_shift_12)<<4) & 0xFF;
}

inline int _XGetR15(int c)
{
	return _rgb_scale_5[(c >> _rgb_r_shift_15) & 0x1F];
}

inline int _XGetG15(int c)
{
	return _rgb_scale_5[(c >> _rgb_g_shift_15) & 0x1F];
}

inline int _XGetB15(int c)
{
	return _rgb_scale_5[(c >> _rgb_b_shift_15) & 0x1F];
}

inline int _XGetA15(int c)
{
	return 0;
}

inline int _XGetR16(int c)
{
	return _rgb_scale_5[(c >> _rgb_r_shift_16) & 0x1F];
}

inline int _XGetG16(int c)
{
	return _rgb_scale_6[(c >> _rgb_g_shift_16) & 0x3F];
}

inline int _XGetB16(int c)
{
	return _rgb_scale_5[(c >> _rgb_b_shift_16) & 0x1F];
}

inline int _XGetA16(int c)
{
	return 0;
}

inline int _XGetR24(int c)
{
	return (c >> _rgb_r_shift_24) & 0xFF;
}

inline int _XGetG24(int c)
{
	return (c >> _rgb_g_shift_24) & 0xFF;
}

inline int _XGetB24(int c)
{
	return (c >> _rgb_b_shift_24) & 0xFF;
}

inline int _XGetA24(int c)
{
	return 255;
}

inline int _XGetR32(int c)
{
	return (c >> _rgb_r_shift_32) & 0xFF;
}

inline int _XGetG32(int c)
{	
	return (c >> _rgb_g_shift_32) & 0xFF;
}

inline int _XGetB32(int c)
{
	return (c >> _rgb_b_shift_32) & 0xFF;
}

inline int _XGetA32(int c)
{
	return (c >> _rgb_a_shift_32) & 0xFF;
}

#define _XRGB8BF(r,g,b)  (_XBestfitColor(sys_palette, int(r), int(g), int(b)))
#define _XRGB8Map(r,g,b) (rgb_map->data[int(r)>>3][int(g)>>3][int(b)>>3])
#define _XRGB8332(r,g,b) ((((r) >> 5) << 5) | (((g) >> 5) << 2) | ((b) >> 5))

inline int _XRGB8(int r, int g, int b)
{
	return (rgb_map) ? _XRGB8Map(r,g,b) : _XRGB8BF(r, g, b);
}

inline int _XRGBA8(int r, int g, int b, int a)
{
	return (rgb_map) ? _XRGB8Map(r, g, b) : _XRGB8BF(r, g, b);
}

inline int _XRGB12(int r, int g, int b)
{
	 return (((CAST(int,r) >> 4) << _rgb_r_shift_12) |
			 ((CAST(int,g) >> 4) << _rgb_g_shift_12) |
			 ((CAST(int,b) >> 4) << _rgb_b_shift_12));
}

inline int _XRGBA12(int r, int g, int b, int a)
{
	 return (((CAST(int,a) >> 4) << _rgb_a_shift_12) |
			 ((CAST(int,r) >> 4) << _rgb_r_shift_12) |
			 ((CAST(int,g) >> 4) << _rgb_g_shift_12) |
			 ((CAST(int,b) >> 4) << _rgb_b_shift_12));
}

inline int _XRGB15(int r, int g, int b)
{
	 return (((CAST(int,r) >> 3) << _rgb_r_shift_15) |
			 ((CAST(int,g) >> 3) << _rgb_g_shift_15) |
			 ((CAST(int,b) >> 3) << _rgb_b_shift_15));
}

inline int _XRGBA15(int r, int g, int b, int a)
{
	 return (((CAST(int,r) >> 3) << _rgb_r_shift_15) |
			 ((CAST(int,g) >> 3) << _rgb_g_shift_15) |
			 ((CAST(int,b) >> 3) << _rgb_b_shift_15));
}

inline int _XRGB16(int r, int g, int b)
{
	 return (((CAST(int,r) >> 3) << _rgb_r_shift_16) |
			 ((CAST(int,g) >> 2) << _rgb_g_shift_16) |
			 ((CAST(int,b) >> 3) << _rgb_b_shift_16));
}

inline int _XRGBA16(int r, int g, int b, int a)
{
	 return (((CAST(int,r) >> 3) << _rgb_r_shift_16) |
			 ((CAST(int,g) >> 2) << _rgb_g_shift_16) |
			 ((CAST(int,b) >> 3) << _rgb_b_shift_16));
}

inline int _XRGB24(int r, int g, int b)
{
	 return ((CAST(int,r) << _rgb_r_shift_24) |
			 (CAST(int,g) << _rgb_g_shift_24) |
			 (CAST(int,b) << _rgb_b_shift_24));
}

inline int _XRGBA24(int r, int g, int b, int a)
{
	 return ((CAST(int,r) << _rgb_r_shift_24) |
			 (CAST(int,g) << _rgb_g_shift_24) |
			 (CAST(int,b) << _rgb_b_shift_24));
}

inline int _XRGB32(int r, int g, int b)
{
	 return ((CAST(int,r) << _rgb_r_shift_32) |
			 (CAST(int,g) << _rgb_g_shift_32) |
			 (CAST(int,b) << _rgb_b_shift_32));
}

inline int _XRGBA32(int r, int g, int b, int a)
{
	 return ((CAST(int,r) << _rgb_r_shift_32) |
			 (CAST(int,g) << _rgb_g_shift_32) |
			 (CAST(int,b) << _rgb_b_shift_32) |
			 (CAST(int,a) << _rgb_a_shift_32));
}

// rwops
inline int _XVFGetStr(_XVFile *rwops, char *buf, int maxsize)
{
	 return _XVFGetStr(rwops, (U8*)(buf), maxsize);
}

inline int _XVFPutStr(_XVFile *rwops, char *buf)
{
	 return _XVFPutStr(rwops, (U8*)(buf));
}

// _XGetTextf
inline char *_XGetTextfEx(char *buf, char *s, ...)
{
	 va_list ap;

	 if(!s)
	  return NULL;

	 if(!buf)
	  buf = _XMalloc(char, 8000);

	 va_start(ap, s);
	 vsprintf((char*)buf, (char*)s, ap);
	 va_end(ap);

	 return buf;
}

inline char *_XGetTextf(char *s, ...)
{
	 va_list ap;

	 if(!s)
	  return NULL;

	 va_start(ap, s);
	 vsprintf((char*)gstrTextfBuff, (char*)s, ap);
	 va_end(ap);

	 return CAST(char*,gstrTextfBuff);
}

#endif // __X_INLINE_H__
