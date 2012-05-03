/*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       dinstall.c
 *  Content:    Game SDK sample setup program
 ***************************************************************************/
#include <stdio.h>      // for sprintf
#include <windows.h>
#include <string.h>
#include "funcalloc.h"
#include "myassert.h"

/*
 * global constants and macros
 */
#define BUFFER_SIZE 8192

#define LoadStrFromRes(uID,lpBuffer) \
LoadString (GetModuleHandle(NULL),\
            (uID),\
            (lpBuffer),\
            sizeof((lpBuffer)));

void _myassert(char *msg,void *file,unsigned linea,...)
{
	char ogg[BUFFER_SIZE];
	int total = 0;
	va_list ap;
	unsigned linear;
	char *interno1;
	char *interno2;
	char *filer;
	FILE *files;

	strcpy(ogg,"");
	va_start(ap, msg);
	filer = va_arg(ap,char*);
	linear=va_arg(ap,unsigned);
	interno1=va_arg(ap,char*);
	interno2=va_arg(ap,char*);
	if ( interno1==NULL)
	{
		sprintf(ogg,"\n%s\n%s\nat line %d",msg,filer,linear);
	}
	else
	{
		sprintf(ogg,"\n%s\n%s\nat line %d\n%s %s",msg,filer,linear,interno1,interno2);
	}

#ifdef _MYDEBUG
	OutputDebugString(ogg);
#endif

	files=fopen("error.log","w");
	if (filer!=NULL)
	{
		fwrite( ogg, sizeof( char ), strlen(ogg), files );
		fclose(files);
	}
	strcpy(ogg,"");
	va_end(ap);

	*(int *)0x0 = 0;
}
