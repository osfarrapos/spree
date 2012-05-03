#ifndef __X_SYSTEM_H__
#define __X_SYSTEM_H__

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "xconfig.h"
#include "xunicode.h"

#define BIT(x)            (1<<x)
#define BITON(var,x)      (var) |= BIT(x)
#define BITOFF(var,x)     (var) = ((var)&BIT(x)) ? ((var)&(~BIT(x))) : (var)
#define BITSWITCH(var,x)  (var) = ((var)&BIT(x)) ? ((var)&(~BIT(x))) : ((var)|BIT(x))
#define FLAGON(var,x)     (var) |= (x)
#define FLAGOFF(var,x)    (var) = ((var)&(x)) ? ((var)&(~(x))) : (var)
#define FLAGSWITCH(var,x) (var) = ((var)&(x)) ? ((var)&(~(x))) : ((var)|(x))

#define EPSILON1	1e-1f
#define EPSILON2	1e-2f
#define EPSILON3	1e-3f
#define EPSILON4	1e-4f
#define EPSILON5	1e-5f
#define EPSILON6	1e-6f
#define EPSILON7	1e-7f
#define EPSILON8	1e-8f
#define EPSILON9	1e-9f

#define _X_PI			3.1415926535897932384626433832795028841971693993751f
#define _X_1BYPI		0.31830988618379067153776752674503f		//(1.0f/_X_PI)

#define _X_PI_DEV180	0.017453292519943295769236907684883f	//(_X_PI/180.0f)
#define _X_180_DEVPI	57.295779513082320876798154814114f		//(180/_X_PI)

#define _X_RAD(x)		((x)*_X_PI_DEV180) //((x)*(_X_PI/180.0f))

#define _X_DEG(x)		((x)*_X_180_DEVPI) // ((x)*(180/_X_PI))

#undef  MIN
#undef  MAX
#undef  MID
#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define MID(x,y,z)   MAX((x), MIN((y), (z)))

#ifndef ABS
#define ABS(x)       (((x) >= 0) ? (x) : (-(x)))
#endif

#ifndef SGN
#define SGN(x)       (((x) >= 0) ? 1 : -1)
#endif

struct _XRGB;
struct _XBitmap;
struct _XBitmapVTable;

#define HYPOT(x, y)  sqrt((x)*(x) + (y)*(y))

#define MAX_CHAR_STRING 2048

#define _XMalloc(type,num) (type*)malloc(sizeof(type)*(num))
#define _XRealloc(ptr,type,num) (type*)realloc(ptr,sizeof(type)*(num))

#define _XFree(x)    if(x) { free(x); x = NULL; }
#define _XDestroy(type,x) if(x) { _XDestroy##type(x); x = NULL; }

#define failed_NULL(cond,msg) if(cond) { _XLogError(msg); return NULL; }
#define failed_int(cond,msg) if(cond) { _XLogError(msg); return 0; }
#define failed_BOOL(cond,msg) if(cond) { _XLogError(msg); return false; }
#define failed_bool(cond,msg) if(cond) { _XLogError(msg); return false; }
#define xfailed(cond,msg) if(cond) { _XLogError(msg); goto error; }

#ifdef _DEBUG
 #define _XTrace(msg) _XLog(msg);
#else
 #define _XTrace(msg)
#endif

// Helper for the printf-like function 
#define DOVSPRINTFEX(outbuf, str)	\
	va_list ap;					\
	va_start(ap, str);			\
	vsprintf(outbuf, str, ap);	\
	va_end(ap);

#define DOVSPRINTF(str)		\
	TCHAR strVBuffer[1024];	\
	DOVSPRINTFEX(strVBuffer, str)

#define DOUVSPRINTFEX(outbuf, str)	\
	va_list ap;					\
	va_start(ap, str);			\
	uvsprintf(outbuf, str, ap);	\
	va_end(ap);

#define DOUVSPRINTF(str)		\
	unchar ustrVBuffer[1024];	\
	DOUVSPRINTFEX(ustrVBuffer, str)

// AGL INITIALISATION
#define OSTYPE_UNKNOWN     0
#define OSTYPE_DOS         _XID('D','O','S',' ')
#define OSTYPE_WIN3        _XID('W','I','N','3')
#define OSTYPE_WIN95       _XID('W','9','5',' ')
#define OSTYPE_WIN98       _XID('W','9','8',' ')
#define OSTYPE_WINNT       _XID('N','T',' ',' ')
#define OSTYPE_WIN2K       _XID('W','2','K',' ')
#define OSTYPE_WINXP       _XID('W','X','P',' ')
#define OSTYPE_WINVISTA    _XID('W','V','S','T')
#define OSTYPE_WIN2003     _XID('W','2','K','3')
#define OSTYPE_OS2         _XID('O','S','2',' ')
#define OSTYPE_WARP        _XID('W','A','R','P')
#define OSTYPE_DOSEMU      _XID('D','E','M','U')
#define OSTYPE_OPENDOS     _XID('O','D','O','S')
#define OSTYPE_LINUX       _XID('L','I','N','U')
#define OSTYPE_UNIX        _XID('U','N','I','X')
#define OSTYPE_BEOS        _XID('B','E','O','S')
#define OSTYPE_MACOS       _XID('M','A','O','S')

extern int		gnOSType;

extern TCHAR gstrGameBaseVersion[MAX_CHAR_STRING];
extern int  _XUseLFN;

extern bool _XGameBaseStarted;

int _XStartup(int argc, TCHAR **argv);
int _XStartup(TCHAR *cmdline);
void _XShutdown(void);

void _XAddExitFunc(void (*func)(void));
void _XRemoveExitFunc(void (*func)(void));
void _XCallExitFuncs(void);

int   _XArgc(void);
TCHAR *_XArgv(int i);

// LOG FILES
void _XSetLogFile(TCHAR *logfile);
void _XLog(TCHAR *msg, ...);
void _XPushLogFile(void);
void _XPopLogFile(void);

// XKernel Debug window
#ifdef  _XDWDEBUG
#include <conio.h>
#endif

#define _XCOMMENT / ## / 

#ifdef _XDWDEBUG
void _XDWINPRINT( TCHAR *msg, ... );
void _XDWINPRINTONLY( TCHAR *msg, ... );
#else
#define _XDWINPRINT		_XCOMMENT
#define _XDWINPRINTONLY _XCOMMENT
#endif

#define _XDEF_PACKETLOGWRITEON
#ifdef  _XDEF_PACKETLOGWRITEON	
	#define _XDWPACKETLOG _XDWINPRINT
#else

#ifdef _XDWDEBUG
	#define _XDWPACKETLOG _XDWINPRINTONLY
#else 
	#define _XDWPACKETLOG _XCOMMENT
#endif

#endif

void _XMessage( TCHAR* argv, ... );

// ERRORS
extern TCHAR  gstrErrorMsg[];

void _XLogError(TCHAR *msg, ...);
extern void (*gpLogErrorCB)(TCHAR *str);

// OS FUNCTIONS
TCHAR *_XOSTypeString(U32 id);
int _XCmdLineOption(TCHAR *str, int req_params);
int _XCmdLineOption(TCHAR *str, int req_params, int comparesize );
void _XExecutableName(TCHAR *name, int size);

// FICHIERS
char *_XFixFilenameCase(TCHAR *path);
char *_XFixFilenameSlashes(TCHAR *path);
char *_XFixFilenamePath(TCHAR *dest, TCHAR *path, int size);
char *_XReplaceFilename(TCHAR *dest, TCHAR *path, TCHAR *filename, int size);
char *_XReplaceExtension(TCHAR *dest, TCHAR *filename, TCHAR *ext, int size);
char *_XAppendFilename(TCHAR *dest, TCHAR *path, TCHAR *filename, int size);
char *_XGetFilename(TCHAR *path);
char *_XPutExtension(TCHAR *filename);
void _XPutBackslash(TCHAR *filename);
int  _XFileExists(TCHAR *filename, int attrib, int *aret);
int  _XExists(TCHAR *filename);
long _XFileSize(TCHAR *filename);
long _XFileTime(TCHAR *filename);
int  _XCopyFile(TCHAR *srcfile, char *dstfile);
int  _XDeleteFile(TCHAR *filename);
int  _XCreateDir(TCHAR *dir);
int  _XDeleteDir(TCHAR *dir);
int  _XForEachFile(TCHAR *name, int attrib, int (*callback)(TCHAR *file, int attriv, int param), int param);

// READ/WRITE OPERATIONS

// Structure for the read/write operations
typedef struct _XVFile
{
 int (*seek)(struct _XVFile *rwops, int offset);
 int (*seek_set)(struct _XVFile *rwops, int offset);
 int (*read)(struct _XVFile *rwops, void *ptr, int size);
 int (*write)(struct _XVFile *rwops, void *ptr, int size);
 int (*tell)(struct _XVFile *rwops);
 int (*end)(struct _XVFile *rwops);
 int (*close)(struct _XVFile *rwops);

 bool memory;

 union
 {
  struct 
  {
	   int close;
	   FILE *fp;
  } stdio;
  struct 
  {
	   int close;
	   void *fp;
  } gzip;
  struct 
  {
	   bool free;
	   U8 *base;
	   U8 *here;
	   U8 *stop;
  } mem;
  struct 
  {
	   void *data;
  } user;

 } hidden;

} _XVFile;


// _XVFile structure creation from misc data types
_XVFile *_XVFOpenFile(TCHAR *filename, TCHAR *mode);
_XVFile *_XVFOpenFP(FILE *fp, int close);
_XVFile *_XVFOpenGzFile(TCHAR *filename, TCHAR *mode);
_XVFile *_XVFOpenGzFP(void *fp, int close);
_XVFile *_XVFOpenMem(void *mem, int size);

int _XVFSeek(_XVFile *rwops, int offset);
int _XVFSeekSet(_XVFile *rwops, int offset);
int _XVFRead(_XVFile *rwops, void *ptr, int size);
int _XVFWrite(_XVFile *rwops, void *ptr, int size);
int _XVFEnd(_XVFile *rwops);
int _XVFTell(_XVFile *rwop);

float _XVFGetF32(_XVFile *rwops);
double _XVFGetF64(_XVFile *rwops);
U8 _XVFGet8(_XVFile *rwops);
/* Little Endian */
U16 _XVFGetLE16(_XVFile *rwops);
U32 _XVFGetLE32(_XVFile *rwops);
U64 _XVFGetLE64(_XVFile *rwops);
/* Big Endian */
U16 _XVFGetBE16(_XVFile *rwops);
U32 _XVFGetBE32(_XVFile *rwops);
U64 _XVFGetBE64(_XVFile *rwops);

int _XVFPutF32(_XVFile *rwops, float val);
int _XVFPutF64(_XVFile *rwops, double val);
int _XVFPut8(_XVFile *rwops, U8 val);
/* Little Endian */
int _XVFPutLE16(_XVFile *rwops, U16 val);
int _XVFPutLE32(_XVFile *rwops, U32 val);
int _XVFPutLE64(_XVFile *rwops, U64 val);
/* Big Endian */
int _XVFPutBE16(_XVFile *rwops, U16 val);
int _XVFPutBE32(_XVFile *rwops, U32 val);
int _XVFPutBE64(_XVFile *rwops, U64 val);

int _XVFGetStr(_XVFile *rwops, U8 *buf, int maxsize);
int _XVFPutStr(_XVFile *rwops, U8 *buf);

int _XDestroyVFile(_XVFile *rwops);

/* Create an empty _XVFile structure */
_XVFile *_XCreateVFile(void);

// CONFIGURATION FILES
typedef struct _XConfigItem
{
 TCHAR name[MAX_CHAR_STRING];
 TCHAR val[MAX_CHAR_STRING];
} _XConfigItem;

typedef struct _XConfig
{
 int num_entry;
 _XConfigItem *entry;
} _XConfig;

_XConfig *_XCreateConfig(int num_entry);
void _XDestroyConfig(_XConfig *cfg);

_XConfig *_XLoadConfig(TCHAR *filename);
_XConfig *_XVFLoadConfig(_XVFile *rwops);

TCHAR *_XGetConfigStr(_XConfig *config, TCHAR *name, TCHAR *def);
int _XGetConfigInt(_XConfig *config, TCHAR *name, int def);
int _XGetConfigHex(_XConfig *config, TCHAR *name, int def);
float _XGetConfigFloat(_XConfig *config, TCHAR *name, float def);
int _XGetConfigID(_XConfig *config, TCHAR *name, int def);
bool _XGetConfigBool(_XConfig *config, TCHAR *name, bool def);

void _XWriteConfigStr(_XVFile *rwops, TCHAR *name, TCHAR *val);
void _XWriteConfigInt(_XVFile *rwops, TCHAR *name, int val);
void _XWriteConfigHex(_XVFile *rwops, TCHAR *name, int val);
void _XWriteConfigFloat(_XVFile *rwops, TCHAR *name, float val);
void _XWriteConfigID(_XVFile *rwops, TCHAR *name, int val);
void _XWriteConfigBool(_XVFile *rwops, TCHAR *name, bool val);

// _XArch FILES

// An entry in the _XArch file database.
typedef struct _XArchDB
{
 int namelen; // Filename string size
 TCHAR *name;  // Filename string
 int size;    // File size
 int offset;  // File offset of the file in the _XArch file
} _XArchDB;

// Flags
#define _XARCH_CLOSE   1
#define _XARCH_CLONE   2

// Informations on a full _XArch file.
typedef struct _XArch
{
 _XVFile *rwops;		// The a25 file
 int flags;         
 int num_file;			// Number of files
 _XArchDB *db;			// The file database
 TCHAR *filename;		// The filename
} _XArch;

// Read the files database
_XArch *_XLoadArch(TCHAR *filename);
// Clone a a25 structure to allow multiple files access from a _XArch file.
_XArch *_XCloneArch(_XArch *a25);

void _XDestroyArch(_XArch *a25); 

// Open a file which is in a _XArch file
_XVFile *_XVFOpenArch(_XArch *a25, TCHAR *filename);

// TIMER
#define MAX_TIMER_FUNC 32

extern bool _XTimerStarted;

int  _XTimerStartup(void);
void _XTimerShutdown(void);

// Return the number of msec since the computer switch on
U32 _XReadTimer(void);
// Reset a timer
void _XResetTimer(U32 *timer); 
// Return the numver of msec since the last _XResetTimer()
U32 _XElapsedMSec(U32 *timer);

// Run the function at the call of _XPollTimer() if the time is elapsed.
#define TIMER_NORMAL    0 
// Run the function many time as necessary if the function _XPollTimer()
// is called to late.
#define TIMER_LUNCHALL  1 

int  _XAddTimer(void (*func)(void), int msec, int mode);
void _XRemoveTimer(void (*func)(void));
void _XPollTimer(void);

void _XRest(U32 msec);
void _XRest(U32 msecx, U32 msecy, void (*callback)(void));

float _XGetFPSf(void);
U32 _XGetFPS(void);

// MISC
extern U8 gstrTextfBuff[]; 
U8 *_XGetTextf(U8 *buf, U8 *str, ...);
U8 *_XGetTextf(U8 *str, ...);

void _XSRandom(U32 seed);
U32 _XRandom(void);
long _XRand(long max);
double _XRand(double max);
long _XRand2(long min, long max);
double _XRand2(double min, double max);

#endif // __X_SYSTEM_H__
