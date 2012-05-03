#ifndef __X_INTERN_H__
#define __X_INTERN_H__

#if defined _X_WIN
#else
	#error "Unknown OS !"
#endif

// COMMON
// DEPENDANT SYSTEM FUNCTION

// System
int   sys_XStartup(void);
void  sys_XShutdown(void);
void  sys_XAtExit(void (*exitfunc)());
void  sys_XGetExecutableName(char *name, int size);

// Timer
int sys_XTimerStartup(void);
void sys_XTimerShutdown(void);
U32 sys_XReadTimer(void);

// Files
int   sys_XFileIsOK(char *filename);
int   sys_XFileExists(char *filename, int attrib, int *aret);
long  sys_XFileSize(char *filename);
long  sys_XFileTime(char *filename);
void *sys_XFindFirst(char *name, int attrib, char *nameret, int *aret);
int   sys_XFindNext(void *dta, char *nameret, int *aret);
void  sys_XFindClose(void *dta);
int   sys_XGetDrive(void);
void  sys_XGetDcwd(int drive, char *buf, int size);
int   sys_XDeleteFile(char *filename);
int   sys_XCreateDir(char *dir);
int   sys_XDeleteDir(char *dir);

// Gfx
void l8_put_pixel(_XBitmap *bmp, int x, int y, int color);
int  l8_get_pixel(_XBitmap *bmp, int x, int y);
void l8_vline(_XBitmap *bmp, int x, int y1, int y2, int color);
void l8_hline(_XBitmap *bmp, int x, int y,  int x2, int color);
void l8_clear_to_color(_XBitmap *bmp, int color);

void l8_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);
void l8_masked_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);



void l12_put_pixel(_XBitmap *bmp, int x, int y, int color);
int  l12_get_pixel(_XBitmap *bmp, int x, int y);
void l12_vline(_XBitmap *bmp, int x, int y1, int y2, int color);
void l12_hline(_XBitmap *bmp, int x, int y,  int x2, int color);
void l12_clear_to_color(_XBitmap *bmp, int color);

void l12_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);
void l12_masked_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);



void l15_put_pixel(_XBitmap *bmp, int x, int y, int color);
int  l15_get_pixel(_XBitmap *bmp, int x, int y);
void l15_vline(_XBitmap *bmp, int x, int y1, int y2, int color);
void l15_hline(_XBitmap *bmp, int x, int y,  int x2, int color);
void l15_clear_to_color(_XBitmap *bmp, int color);

void l15_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);
void l15_masked_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);



void l16_put_pixel(_XBitmap *bmp, int x, int y, int color);
int  l16_get_pixel(_XBitmap *bmp, int x, int y);
void l16_vline(_XBitmap *bmp, int x, int y1, int y2, int color);
void l16_hline(_XBitmap *bmp, int x, int y,  int x2, int color);
void l16_clear_to_color(_XBitmap *bmp, int color);

void l16_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);
void l16_masked_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);



void l24_put_pixel(_XBitmap *bmp, int x, int y, int color);
int  l24_get_pixel(_XBitmap *bmp, int x, int y);
void l24_vline(_XBitmap *bmp, int x, int y1, int y2, int color);
void l24_hline(_XBitmap *bmp, int x, int y,  int x2, int color);
void l24_clear_to_color(_XBitmap *bmp, int color);

void l24_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);
void l24_masked_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);



void l32_put_pixel(_XBitmap *bmp, int x, int y, int color);
int  l32_get_pixel(_XBitmap *bmp, int x, int y);
void l32_vline(_XBitmap *bmp, int x, int y1, int y2, int color);
void l32_hline(_XBitmap *bmp, int x, int y,  int x2, int color);
void l32_clear_to_color(_XBitmap *bmp, int color);

void l32_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);
void l32_masked_blit(_XBitmap *src, _XBitmap *dst, int xs, int ys, int xd, int yd, int w, int h);

#endif // __X_INTERN_H__
