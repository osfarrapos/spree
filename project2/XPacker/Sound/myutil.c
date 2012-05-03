
#include <string.h>
#include "myutil.h"
#include "myerror.h"
//---------------------------------------------------------------------
//General Utility 

void AddBackslash(char *string)
{
	int len;
	len=strlen(string);
	if(len==0 || string[len-1]!='\\')
		strcpy(string+len, "\\");
}

/* 
	nameFull: buffer that contains full path
	path: path, if NULL no path and then no drive
	name: file name with or without extension 
	drive: if drive=0 no drive change if drive!=0 change drive 
			and force path to contain it
	ext: extension, if null no extension
*/
void BuildFullPath(char *nameFull,char *path, char *name, char drive,char *ext)
{
	extern char Path_Program[];
	FATALASSERT(nameFull!=NULL);

	if(drive!='0'){
		strcpy(nameFull,Path_Program);
		AddBackslash(nameFull);
	}
	else{
		nameFull[0]='\0';
	}
	
	if(path!=NULL && path[0]!='\0'){
		strcat(nameFull,path);	
		AddBackslash(nameFull);
		if(drive!='\0' && path[1]==':') 
			nameFull[0]=drive;	
	}
	strcat(nameFull,name);
	if(ext!=NULL && strlen(ext)>0){
		if(ext[0]!='.') {
			FATALASSERT(strlen(ext)<=3);  //ext max 3 char
			strcat(nameFull,".");	
		}
		else
			FATALASSERT(strlen(ext)<=4);
		strcat(nameFull,ext);
	}
}

