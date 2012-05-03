/* ------------------------------------------------------------------
	 Sound NAME Interface

	Pixelstorm Copyright - by Teodoro Di Bello
 
	Version: 0.5		Date: 19/01/98

	Description: 
	It simply handles the loading and the writing of list of snd name,
	i.e. a name (max 8 char  = MAXSNDNAME (8+1)) + 
		environment name (max 30 char=MAXSNDENVNAME (30+1)) +
		a linked environment name (max 30 char=MAXSNDENVNAME (30+1)) 
	Note:	
  ------------------------------------------------------------------ */
#include <stddef.h>
#include "sndname.h"
#include "myerror.h"

#define SNDLISTHEADER "SNDLIST1"

typedef size_t (* FILEOPER)( void *buffer, size_t size, size_t count, FILE *stream);
size_t SndNameOpItem(FILE *pf,SNDNAME name, FILEOPER opFunc);

size_t SndNameOpItem(FILE *pf,SNDNAME name, FILEOPER opFunc)
{
	size_t n;
	n=opFunc(name,MAXSNDNAME,1,pf);
	myassert(strlen(name)!=MAXSNDNAME,"Fatal Error!", "Scene Name not correct");
	return n;
}

size_t SndListOpItem(FILE *pf,LPSNDLISTITEM lpLI, FILEOPER opFunc)
{	
	size_t n;
	n=opFunc(lpLI,sizeof(SNDLISTITEM),1,pf);
	return n;
}

BOOL SndNameItemWrite(FILE *pf,SNDNAME name)
{
	return (SndNameOpItem(pf,name,fwrite)>0);
}

BOOL SndNameItemRead(FILE *pf,SNDNAME name)
{
	return (SndNameOpItem(pf,name,fread)>0);
}


BOOL SndListItemWrite(FILE *pf,LPSNDLISTITEM lpLI)
{
	return (SndListOpItem(pf,lpLI,fwrite)>0);
}

BOOL SndListItemRead(FILE *pf,LPSNDLISTITEM lpLI)
{
	return (SndListOpItem(pf,lpLI,fread)>0);
}

FILE *SndListOpen(char *nameFile,DWORD *pnItem,char mode)
{
	FILE *pf=NULL;
	size_t nf1,nf2;
	char *opMode;
	char head[sizeof(SNDLISTHEADER)+1];

	if(mode=='w'){
		opMode=FMTWRITE;		
	}
	else if(mode=='r')
		opMode=FMTREAD;
	else
		return NULL;
	pf=fopen(nameFile,opMode);	
	ASSERTFILE(pf!=NULL,nameFile);
	if(mode=='w'){
		nf1=fwrite(SNDLISTHEADER, sizeof(char), strlen(SNDLISTHEADER)+1, pf);
		nf2=fwrite(pnItem, sizeof(DWORD), 1, pf);
	}
	else{
		nf1=fread(head, sizeof(char), strlen(SNDLISTHEADER)+1, pf);
		if(!strcmp(head,SNDLISTHEADER))
			nf2=fread(pnItem, sizeof(DWORD), 1, pf);
		else{
			fclose(pf);
			pf=NULL;
		}
	}
	return pf;
}

void SndListClose(FILE *pf)
{
	fclose(pf);
}

BOOL SndEnvCmpEq(SND_GAME_ENV env1,SND_GAME_ENV env2)
{
	return (strcmp(env1,env2)==0);
}

BOOL SndEnvCopy(SND_GAME_ENV envtgt,SND_GAME_ENV envsrc)
{
	return (strcpy(envtgt,envsrc)!=NULL);
}
