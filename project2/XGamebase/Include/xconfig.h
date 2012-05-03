#ifndef __X_CONFIG_H__
#define __X_CONFIG_H__

#if defined _MSC_VER

/* WARNINGS DISABLING ******************************************************/
#pragma warning(disable:4142)
#pragma warning(disable:4146)
#pragma warning(disable:4244)
#pragma warning(disable:4305)
#pragma warning(disable:4273) 
#pragma warning(disable:4018) 
#pragma warning(disable:4761) 

/* HEADERS *****************************************************************/
#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <fcntl.h>
#include <direct.h>
#include <malloc.h>
#include <windows.h>
#include <time.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>
#include <ctype.h>

/* IDENTIFICATION DEFINES **************************************************/
#define _X_PLATFORM_STR  "Win32"
#define _X_I386
#define _X_MSVC
#define _X_WIN
#define _X_LITTLE_ENDIAN
#define _X_HAVE_I64
#define _X_ASM_MSVC
#define HAVE_EXTERN_ASM

/* OTHERS DEFINES **********************************************************/
#define _X_USE_LFN   1

#define I64 signed __int64
#define U64 unsigned __int64

#define inline __forceinline

#ifndef S_IRUSR
#define S_IRUSR   S_IREAD
#define S_IWUSR   S_IWRITE
#endif

/*===========================================================================

   UNKNOWN (ERROR)

===========================================================================*/
#else
 #error "No supported platform detected !"
#endif

/*===========================================================================

   COMMON DECLARATIONS

===========================================================================*/
#ifndef CAST
 #define CAST(t,f) ((t)(f))
#endif

#ifndef U8
 #define U8  unsigned char
#endif

#ifndef I8
 #define I8  signed char
#endif

#ifndef U16
 #define U16  unsigned short
#endif

#ifndef I16
 #define I16  signed short
#endif

#ifndef U32
 #define U32  unsigned long
#endif

#ifndef I32
 #define I32  signed long
#endif

#ifndef U64
 #define U64  unsigned long
#endif

#ifndef I64
 #define I64  I32
#endif

/*#ifndef fixed
 #define fixed I32
#endif*/

#ifndef OTHER_PATH_SEPARATOR
 #if (defined _X_DOS) || (defined _X_WIN)
  #define OTHER_PATH_SEPARATOR  '\\'
 #else
  #define OTHER_PATH_SEPARATOR  '/'
 #endif
#endif

#ifndef DEVICE_SEPARATOR
 #if (defined _X_DOS) || (defined _X_WIN)
  #define DEVICE_SEPARATOR      ':'
 #else
  #define DEVICE_SEPARATOR      '\0'
 #endif
#endif

// EMULATION OF MISC FUNCS
/* Emulation of the FA_* flags */
#ifndef FA_RDONLY
 #define FA_RDONLY       1
 #define FA_HIDDEN       2
 #define FA_SYSTEM       4
 #define FA_LABEL        8
 #define FA_DIREC        16
 #define FA_ARCH         32
#endif

/* Emulation of some libc functions (for Unix systems) */
#ifdef _X_NO_STRICMP
 int stricmp(const char *s1, const char *s2);
#endif

#ifdef _X_NO_STRLWR
 char *strlwr(char *string);
#endif

#ifdef _X_NO_STRUPR
 char *strupr(char *string);
#endif

#ifdef _X_NO_MEMCMP
 int memcmp(const void *s1, const void *s2, size_t num);
#endif

#ifdef _X_NO_FINDFIRST
 struct ffblk
 {
  unsigned char ff_attrib;
  unsigned short ff_ftime;
  unsigned short ff_fdate;
  unsigned long ff_fsize;
  char ff_name[1024];
  void *ff_info;
 };

 extern int findfirst(char *pattern, struct ffblk *ffblk, int attrib);
 extern int findnext(struct ffblk *ffblk);
 extern void findclose(struct ffblk *ffblk);
#endif

#ifndef gbWrite8

#define _XWrite8(addr,val)  *((U8*)((U32)(addr))) = (U8)(val)
#define _XWrite12(addr,val) *((U16*)((U32)(addr))) = (U16)(val)
#define _XWrite15(addr,val) *((U16*)((U32)(addr))) = (U16)(val)
#define _XWrite16(addr,val) *((U16*)((U32)(addr))) = (U16)(val)
#define _XWrite32(addr,val) *((U32*)((U32)(addr))) = (U32)(val)

#define _XRead8(addr)  *((U8*)((U32)(addr)))
#define _XRead12(addr) *((U16*)((U32)(addr)))
#define _XRead15(addr) *((U16*)((U32)(addr)))
#define _XRead16(addr) *((U16*)((U32)(addr)))
#define _XRead32(addr) *((U32*)((U32)(addr)))

#ifdef _X_LITTLE_ENDIAN

inline void __XWrite24(U32 addr, int c)
{
 unsigned char *p = (unsigned char *)addr;
 p[0] = c & 0xFF;
 p[1] = (c>>8) & 0xFF;
 p[2] = (c>>16) & 0xFF;
}

inline int __XRead24(U32 addr)
{
 unsigned char *p = (unsigned char*)addr;
 int c;

 c = p[0];
 c |= (int)p[1] << 8;
 c |= (int)p[2] << 16;

 return c;
}

#else

inline void __XWrite24(U32 addr, int c)
{
 unsigned char *p = (unsigned char *)addr;
 p[0] = (c>>16) & 0xFF;
 p[1] = (c>>8) & 0xFF;
 p[2] = c & 0xFF;
}

inline int _agl__XRead24(U32 addr)
{
 unsigned char *p = (unsigned char *)addr;
 int c;

 c = (int)p[0] << 16;
 c |= (int)p[1] << 8;
 c |= p[2];

 return c;
}

#endif

#define _XWrite24(addr, val) __XWrite24((U32)(addr), (int)(val))
#define _XRead24(addr)       __XRead24((U32)(addr))

#endif

#define SWAP16(val) ((CAST(U16,val)<<8)|(CAST(U16,val)>>8))
#define SWAP32(val) ((CAST(U32,val)<<24)|((CAST(U32,val)<<8)&0x00FF0000)|((CAST(U32,val)>>8)&0x0000FF00)|(CAST(U32,val)>>24))
#define SWAP64(val) _SWAP64(CAST(U64,val))

inline U64 _SWAP64(U64 val)
{
 U32 hi;
 U32 lo;

 lo = (U32)(val&0xFFFFFFFF);
 val >>= 32;
 hi = (U32)(val&0xFFFFFFFF);
 val = SWAP32(lo);
 val <<= 32;
 val |= SWAP32(hi);

 return val;
}

#ifdef _X_LITTLE_ENDIAN
 #define SWAP_LE16(x) (x)
 #define SWAP_LE32(x) (x)
 #define SWAP_LE64(x) (x)
 #define SWAP_BE16(x) SWAP16(x)
 #define SWAP_BE32(x) SWAP32(x)
 #define SWAP_BE64(x) SWAP64(x)
#elif define _X_BIG_ENDIAN
 #define SWAP_BE16(x) (x)
 #define SWAP_BE32(x) (x)
 #define SWAP_BE64(x) (x)
 #define SWAP_LE16(x) SWAP16(x)
 #define SWAP_LE32(x) SWAP32(x)
 #define SWAP_LE64(x) SWAP64(x)
#else
 #error "Endianess not defined !"
#endif

#endif // __X_CONFIG_H__
