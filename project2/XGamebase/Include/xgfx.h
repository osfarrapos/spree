#ifndef __X_GFX_H__
#define __X_GFX_H__

//   _XBitmap
#define MASK_COLOR_8       0
#define MASK_COLOR_12	   0x0F0F      /* (Pink R:255 G:0 B:255) */
#define MASK_COLOR_15      0x7C1F      /* (Pink R:255 G:0 B:255) */
#define MASK_COLOR_16      0xF81F      /* (Pink R:255 G:0 B:255) */
#define MASK_COLOR_24      0xFF00FF    /* (Pink R:255 G:0 B:255) */
#define MASK_COLOR_32      0x00FF00FF  /* (Pink R:255 G:0 B:255) */

int _XIsSupportedBpp(int bpp);


// Bitmap (Drawing surface)
typedef struct _XBitmap
{
	 int w, h, bpp,					/* Size */
			   bpl;					/* Width size in bytes */
	 int size;						/* Size in bytes */
	 int id;						/* ID */
	 int cl, cr, ct, cb;			/* Clipping Zone */
	 int x_ofs, y_ofs;				/* For sub-bitmaps */ 
	 struct _XBitmap *parent;		/* Idem */
	 struct _XBitmapVTable *vtbl;	/* Function table */
	 void *extra;					/* External data (used by the AGL) */
	 void *user;					/* Datas for the user (you =)) */
	 void *idat;					/* Memory allowed to the bitmap */
	 U8 *line[1];					/* Ptr on the beginning of each lines */

	 _XBitmap()
	 {
		 idat = user = extra = NULL;
	 }
} _XBitmap;

#define BMP_MASK(bmp) ((bmp)->vtbl->mask_color)

#define BMP_ID_OWNDAT             0x08000000
#define BMP_ID_SUBBMP             0x04000000
#define BMP_ID_MASK               0x03FFFFFF

// Internal function to draw in the bitmaps
// NA - The bitmap has not to be acquired before the use of this function.
typedef struct _XBitmapVTable
{
	 int bpp;
	 int mask_color;

	 // Primitives
	 void (*put_pixel)(_XBitmap *bmp, int x, int y, int color);
	 int  (*get_pixel)(_XBitmap *bmp, int x, int y);
	 void (*vline)(_XBitmap *bmp, int x, int y1, int y2, int color); 
	 void (*hline)(_XBitmap *bmp, int x, int y, int x2, int color); 
	 void (*clear)(_XBitmap *bmp, int color); /* NA */ 

	 // Blitting
	 void (*blit)(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);
	 void (*blit_mask)(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);
} _XBitmapVTable;

extern _XBitmapVTable bmpvtbl_empty;
extern _XBitmapVTable bmpvtbl_8bit_linear;
extern _XBitmapVTable bmpvtbl_12bit_linear;
extern _XBitmapVTable bmpvtbl_15bit_linear;
extern _XBitmapVTable bmpvtbl_16bit_linear;
extern _XBitmapVTable bmpvtbl_24bit_linear;
extern _XBitmapVTable bmpvtbl_32bit_linear;

_XBitmapVTable *_XGetBitmapVTable(int bpp);
void _XSetBitmapAddress(_XBitmap *b, void *addr);
void _XSetClipState(_XBitmap *bitmap, int x1, int y1, int x2, int y2);

_XBitmap *_XCreateBitmap(int w, int h, int bpp, int bpl, void *addr, _XBitmapVTable *vtbl, int id);
_XBitmap *_XCreateBitmap(int width, int height, int bpp);
_XBitmap *_XCreateBitmap(_XBitmap *parent, int x, int y, int w, int h);
void _XDestroyBitmap(_XBitmap *bmp);

typedef struct _XClipState
{
	int cl, cr, ct, cb;
} _XClipState;

void _XPushClipState(_XBitmap *bmp, _XClipState *pc);
void _XPopClipState(_XBitmap *bmp, _XClipState *pc);

// Helper
void _XDoLine(_XBitmap *bmp, int x1, int y1, int x2, int y2, int d, void (*proc)(_XBitmap*, int, int, int));
void _XDoCircle(_XBitmap *bmp, int x, int y, int radius, int d, void (*proc)(_XBitmap*, int, int, int));

// Primitives
void _XLine(_XBitmap *bmp, int x1,int y1,int x2,int y2, int color);
void _XRect(_XBitmap *bmp, int x1, int y1, int x2, int y2, int color);
void _XFillRect(_XBitmap *bmp, int x1, int y1, int x2, int y2, int color);
void _XCircle(_XBitmap *bmp, int x, int y, int radius, int color);
void _XFillCircle(_XBitmap *bmp, int x, int y, int radius, int color);

void _XDottedRect(_XBitmap *bmp, int x1, int y1, int x2, int y2, int fg, int bg);

// Blitting
void _XBlit(_XBitmap *src, _XBitmap *dst, int s_x, int s_y, int d_x, int d_y, int w, int h);
void _XBlitCMask(_XBitmap *src, _XBitmap *dst, int s_x, int s_y, int d_x, int d_y, int w, int h);
void _XBlitMask(_XBitmap *src, _XBitmap *dst, int s_x, int s_y, int d_x, int d_y, int w, int h);
void _XBlit(_XBitmap *d, _XBitmap *s, int x_s, int y_s, int x_d, int y_d, int w, int h, int w_d, int h_d);
void _XBlitMask(_XBitmap *d, _XBitmap *s, int x_s, int y_s, int x_d, int y_d, int w, int h, int w_d, int h_d);

// Conversion
_XBitmap *_XConvertBitmapSize(_XBitmap *bmp, int w, int h, int destroy);
_XBitmap *_XConvertTextureBitmapSize(_XBitmap *bmp, int w, int h, int destroy);
_XBitmap *_XConvertBitmapBpp(_XBitmap *bmp, struct _XRGB *pal, int bpp, int destroy);
_XBitmap *_XConvertBitmapMaskBpp(_XBitmap *bmp, struct _XRGB *pal, int bpp, int destroy);
_XBitmap *_XBlurBitmap(_XBitmap *bmp, int destroy);

inline _XBitmap *_XCreateBitmap(_XBitmap *bmp) { return _XConvertBitmapSize(bmp, bmp->w, bmp->h, 0); }

//COLORS AND PALETTES
#define BYTES_PER_PIXEL(bpp)     ((CAST(int,bpp)+7) >> 3)

#define _X_PAL_SIZE        256

//A RGB color
typedef struct _XRGB
{
 unsigned char r,g,b;
} _XRGB;

// RGB Table
typedef struct _XRGBMap
{
 U8 data[32][32][32];
} _XRGBMap;

// A 256 colors palette
typedef _XRGB _XPalette[256];

#define SET_RGB(col, _r, _g, _b) (col).r = _r; (col).g = _g; (col).b = _b
#define SET_RGBA(col, _r, _g, _b, _a) (col).r = _r; (col).g = _g; (col).b = _b; (col).a = _a

// Global palette
extern _XPalette sys_palette;
extern _XPalette default_palette;
extern _XPalette black_palette;

// Colors index
extern int _rgb_a_shift_12;
extern int _rgb_r_shift_12;
extern int _rgb_g_shift_12;
extern int _rgb_b_shift_12;
extern int _rgb_r_shift_15;
extern int _rgb_g_shift_15;
extern int _rgb_b_shift_15;
extern int _rgb_r_shift_16;
extern int _rgb_g_shift_16;
extern int _rgb_b_shift_16;
extern int _rgb_r_shift_24;
extern int _rgb_g_shift_24;
extern int _rgb_b_shift_24;
extern int _rgb_r_shift_32;
extern int _rgb_g_shift_32;
extern int _rgb_b_shift_32;
extern int _rgb_a_shift_32;

extern int _rgb_scale_5[];
extern int _rgb_scale_6[];

void _XPushRGBShift(int bpp);
void _XPopRGBShift(void);
void _XSetRGBShift(int bpp, int rs, int gs, int bs, int as);
void _XGetRGBShift(int bpp, int *rs, int *gs, int *bs, int *as);

extern _XRGBMap *rgb_map;

void _XCreateRGBTable(_XRGBMap *table, _XPalette pal, void (*callback)(int pos));

void _XSetColor(int index, _XRGB *p);
void _XGetColor(int index, _XRGB *p);

void _XSetPalette(_XPalette p, int from, int to, int vbl);
void _XGetPalette(_XPalette p, int from, int to);
inline void _XSetPalette(_XPalette p) { _XSetPalette(p, 0, _X_PAL_SIZE-1, 0); }
inline void _XGetPalette(_XPalette p) { _XGetPalette(p, 0, _X_PAL_SIZE-1); }
void _XSetPaletteMap(_XPalette p, int updmap);
void _XGetPaletteFromBuffer(unsigned char *src, _XPalette dst);
void _XCopyPalette(_XPalette src, _XPalette dst);

void _XSelectPalette(_XPalette p);
void _XUnselectPalette(void);

void _XDrawPalette(_XBitmap *bmp, int xd, int yd, int w, int h);

void _XGenerate332Palette(_XPalette pal);

void _XCreatePaletteShading(_XPalette pal, int ib, int rb, int gb, int bb,
							                 int ie, int re, int ge, int be);

int _XBestfitColor(_XPalette pal, int r, int g, int b);

int _XRGBEx(int r, int g, int b, int color_depth);
int _XRGBAEx(int r, int g, int b, int a, int color_depth);

int _XGetREx(int c, int color_depth);
int _XGetGEx(int c, int color_depth);
int _XGetBEx(int c, int color_depth);
int _XGetAEx(int c, int color_depth);

void _XHSVToRGB(float h, float s, float v, int *r, int *g, int *b);
void _XRGBToHSV(int r, int g, int b, float *h, float *s, float *v);

int _XGenerateOptimizedPalette(_XBitmap *image, _XPalette pal,
								  signed char *rsvdcols, int bitsperrgb,
								  int fraction, int maxswaps, int mindiff);
int _XGenerateOptimizedPalette(_XBitmap *image, _XPalette pal,
							   signed char *rsvdcols);

#endif // __X_GFX_H__
