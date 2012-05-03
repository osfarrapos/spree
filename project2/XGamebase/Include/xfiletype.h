#ifndef __X_FILETYPE_H__
#define __X_FILETYPE_H__

// Reading and Writing of palette files
int _XVFReadPalette(_XVFile *f, _XPalette pal);
void _XVFWritePalette(_XVFile *f, _XPalette pal);
int _XLoadPalette(TCHAR *filename, _XPalette pal);
int _XSavePalette(TCHAR *filename, _XPalette pal);

// Reading and Writing of bitmap files
// Conversion mode on loading
#define BMPCONV_NONE            0
#define BMPCONV_TO8BPP          1
#define BMPCONV_TO15BPP         2
#define BMPCONV_TO16BPP         4
#define BMPCONV_TO24BPP         8
#define BMPCONV_TO32BPP         16
#define BMPCONV_MASK            32

void _XSetBitmapConvMode(int mode);
int _XGetBitmapConvMode(void);

void _XPushBitmapConvMode(void);
void _XPopBitmapConvMode(void);

bool _XVFIsBitmap(_XVFile *fp);
bool _XIsBitmapFile(TCHAR *filename);

_XBitmap *XVFLoadBitmap( FILE *fileptr, _XPalette pal);

_XBitmap *_XVFLoadBitmap(_XVFile *rwops, _XPalette pal);
_XBitmap *_XLoadBitmap(TCHAR *filename, _XPalette pal);
int _XSaveBitmap(TCHAR *filename, _XBitmap *bmp, _XPalette pal);

void _XRegisterBitmapFileType(char *ext,
                              bool (*is)(_XVFile *rwops),
                              _XBitmap *(*read)(_XVFile *rwops, _XRGB *pal),
                              int (*write)(_XVFile *rwops, _XBitmap *bmp, _XRGB *pal));

bool _XVFIsBMP(_XVFile *rwops);
_XBitmap *_XVFReadBMP(_XVFile *rwops, _XRGB *pal);
int _XVFWriteBMP(_XVFile *rwops, _XBitmap *bmp, _XRGB *pal);

bool _XVFIsPCX(_XVFile *rwops);
_XBitmap *_XVFReadPCX(_XVFile *rwops, _XRGB *pal);
int _XVFWritePCX(_XVFile *rwops, _XBitmap *bmp, _XRGB *pal);

bool _XVFIsGIF(_XVFile *rwops);
_XBitmap *_XVFReadGIF(_XVFile *rwops, _XRGB *pal);
int _XVFWriteGIF(_XVFile *rwops, _XBitmap *bmp, _XRGB *pal);

bool _XVFIsTGA(_XVFile *rwops);
_XBitmap *_XVFReadTGA(_XVFile *rwops, _XRGB *pal);
int _XVFWriteTGA(_XVFile *rwops, _XBitmap *bmp, _XRGB *pal);

#include "jpeglib.h"
#include "setjmp.h"

#define INPUT_BUFFER_SIZE	4096

typedef struct
{
	struct jpeg_source_mgr pub;
	_XVFile *ctx;
	U8     buffer[INPUT_BUFFER_SIZE];
} my_source_mgr;

struct my_error_mgr
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
	_XBitmap **bmp;
	U8         **buf;
};

extern void my_error_exit(j_common_ptr cinfo);
extern void my_output_message(j_common_ptr cinfo);

bool _XVFIsJPG(_XVFile *rwops);
_XBitmap *_XVFReadJPG(_XVFile *rwops, _XRGB *pal);

bool _XVFIsPNG(_XVFile *rwops);
_XBitmap *_XVFReadPNG(_XVFile *rwops, _XRGB *pal);

#endif // __X_FILETYPE_H__
