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

#define BYTES_PER_PIX  2
#define BPP 12

#define CALLBACK callback12

#define FBPP(func) l12_##func

#define PIXEL_PTR       U16*
#define PIXEL_PTR_ADDR(ptr, x) ((PIXEL_PTR)(ptr) + (x))
#define INC_PIXEL_PTR(ptr) ((ptr)++)
#define DEC_PIXEL_PTR(ptr) ((ptr)--)

#define WRITE(ptr, val)      *(ptr) = CAST(U16,val)
#define READ(ptr)            *((PIXEL_PTR)(ptr))

#define MASK_COLOR     MASK_COLOR_12

#define MAKECOL(r,g,b)    _XRGB12(r,g,b)
#define MAKEACOL(r,g,b,a) _XRGBA12(r,g,b,a)

#define GETR(c) _XGetR12(c)
#define GETG(c) _XGetG12(c)
#define GETB(c) _XGetB12(c)
#define GETA(c) _XGetA12(c)

#define ISMASK(v)      (((U32)(v)) == MASK_COLOR_12)
