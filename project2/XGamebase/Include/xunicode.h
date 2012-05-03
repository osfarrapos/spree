#ifndef __X_UNICODE_H__
#define __X_UNICODE_H__

#include <stdio.h>
#include <string.h>
#include <wchar.h>

typedef unsigned short unchar;

#define USTR(str) L##str

#define utolower	towlower
#define utoupper	towupper
#define uisspace	iswspace
#define ustrcpy		wcscpy
#define ustrcat		wcscat
#define ustrlen		wcslen
#define ustrcmp		wcscmp
#define ustricmp	wcsicmp
#define ustrncpy	wcsncpy
#define ustrncat	wcsncat
#define ustrncmp	wcsncmp
#define ustrlwr		wcslwr
#define ustrupr		wcsupr
#define ustrchr		wcschr
#define ustrstr		wcsstr
#define ustrpbrk	wcspbrk
#define ustrtok		wcstok
#define ustrtol		wcstol
#define uvsprintf	vswprintf
#define usprintf	swprintf

#define ufopen		_wfopen
#define ufputs		fputws
#define ufgets		fgetws
#define ufputc		fputwc
#define ufgetc		fgetwc
#define ufprintf	fwprintf

// Convert a unicode string to an ASCII string.
// This functions used a global temp buffer... so you must copy the returned
// string if you want to keep the content.
char *uascii(unchar *s, ...);

// some useful extra functions
int uinsert(unchar *s, int index, int c);
int uremove(unchar *s, int index);
int ustrsize(unchar *s);
int ustrsizez(unchar *s);
double uatof(unchar *s);

#endif // __X_UNICODE_H__
