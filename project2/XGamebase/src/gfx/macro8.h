#undef BPP
#undef CALLBACK
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
#undef BYTEST_PER_PIX

#define BYTES_PER_PIX  1
#define BPP 8

#define CALLBACK callback8

#define FBPP(func) l8_##func

#define PIXEL_PTR       U8*
#define PIXEL_PTR_ADDR(ptr, x) ((PIXEL_PTR)(ptr) + (x))
#define INC_PIXEL_PTR(ptr) ((ptr)++)
#define DEC_PIXEL_PTR(ptr) ((ptr)--)

#define WRITE(ptr, val)      *(ptr) = CAST(U8, val)
#define READ(ptr)            *((PIXEL_PTR)(ptr))

#define MASK_COLOR     MASK_COLOR_8

#define MAKECOL(r,g,b)    _XRGB8(r,g,b)
#define MAKEACOL(r,g,b,a) _XRGBA8(r,g,b,a)

#define GETR(c)    _XGetR8(c)
#define GETG(c)    _XGetG8(c)
#define GETB(c)    _XGetB8(c)
#define GETA(c)    _XGetA8(c)

#define ISMASK(v)      (((U8)(v)) == MASK_COLOR_8)

