#undef CALLBACK
#undef BPP
#undef FBPP
#undef PIXEL_PTR
#undef PIXEL_PTR_ADDR
#undef INC_PIXEL_PTR
#undef DEC_PIXEL_PTR
#undef WRITE
#undef READ
#undef MASK_COLOR
#undef MAKECOL
#undef MAKEACOL
#undef GETR
#undef GETG
#undef GETB
#undef GETA
#undef ISMASK
#undef BYTES_PER_PIX

#define BYTES_PER_PIX  3
#define BPP 24

#define CALLBACK callback24
#define FBPP(func) l24_##func

#define PIXEL_PTR       U8*
#define PIXEL_PTR_ADDR(ptr, x) ((PIXEL_PTR)(ptr) + (3 * (x)))
#define INC_PIXEL_PTR(ptr) ((ptr) += 3)
#define DEC_PIXEL_PTR(ptr) ((ptr) -= 3)

#define WRITE(ptr, val)    _XWrite24((U32)(ptr), (val))
#define READ(ptr)          _XRead24((U32)(ptr))

#define MASK_COLOR     MASK_COLOR_24

#define MAKECOL(r,g,b)    _XRGB24(r,g,b)
#define MAKEACOL(r,g,b,a) _XRGBA24(r,g,b,a)

#define GETR(c) _XGetR24(c)
#define GETG(c) _XGetG24(c)
#define GETB(c) _XGetB24(c)
#define GETA(c) _XGetA24(c)

#define ISMASK(v)      ((U32)(v) == MASK_COLOR_24)

