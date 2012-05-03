/* ------------------------------------------------------------------
	 Sound Object Interface

	Pixelstorm Copyright - by Teodoro Di Bello
 
	Version: 0.5		Date: 19/01/98

	Description: 
	Gestione delle scene suono (=lista di suoni con relative proprieta')
	Load, Destroy e altro!
		   
	Note:	
  ------------------------------------------------------------------ */

#include <string.h>
#include "myutil.h"
#include "myassert.h"
#include "myerror.h"
#include "funcalloc.h"
#include "sndsuppl.h"
#include "sndscene.h"

#define SNDSCNEXT "ssl"
#define SNDSCNHEADER "SNDSCN1"
#define SNDSCNPATH SNDDFLTPATH
#define SNDSCNTBLNAME "sndscntb"

typedef struct{
	SNDLISTITEM li;
	LPSNDOBJ *table;
	DWORD nSnd;
	LOADHANDLE loadBlock; //Block of belongings for the loading	
	LOADPATH loadPath;    //Path in which the relative rows are found	
} SNDSCENE, *LPSNDSCENE;

// Sound Scenes Table: it contains the list of all the scenes
static LPSNDSCENE sndScnTable=NULL;
// Total Sound Scenes number, not loaded scenes
static DWORD nSndScn=0;

#define INDEX2HANDLE(ind) ((ind)+1)
#define HANDLE2INDEX(hnd) ((hnd)-1)
// I Know That SNDNOHANDLE=0
#define ISVALIDHANDLE(hnd) ((hnd)>SNDNOHANDLE && (hnd)<=nSndScn) 

//name of snd scn table
static char  nameScnTbl[MAX_PATH];

char *SndScnTblGetName(void)
{
	BuildFullPath(nameScnTbl,NULL, SNDSCNTBLNAME, 0,SNDLISTEXT);
	return nameScnTbl;
}

// loads the list with scenes
BOOL SndScnTblLoad(void)
{
	FILE *pf;
	char nameFull[MAX_PATH];
	char *nameTbl=SNDSCNTBLNAME;
	DWORD i;
	if(sndScnTable!=NULL) //already load
		return TRUE;
	BuildFullPath(nameFull,SNDSCNPATH, nameTbl, sndDriveContext,SNDLISTEXT);
	pf=SndListOpen(nameFull,&nSndScn,'r');	
	ASSERTFILE(pf!=NULL, nameFull);
	sndScnTable=MyMalloc(SNDSCENE,nSndScn); 
	ASSERTOUTMEMORY(sndScnTable!=NULL);
	for(i=0; i<nSndScn; i++){
		SndListItemRead(pf,&sndScnTable[i].li);	
		sndScnTable[i].nSnd=0;
		sndScnTable[i].table=NULL;
	}
	SndListClose(pf);
	return TRUE;
}

// Destroy
void SndScnTblDestroy(void)
{
	SndScnDestroyAll();
	if(sndScnTable!=NULL)
		MyFree(sndScnTable);
	sndScnTable=NULL;
	nSndScn=0;	
}

// Returns the handle of the scene from the name
DWORD SndScnGetHandle(SNDNAME nameFile)
{
	DWORD i;	
	for(i=0; i<nSndScn; i++)
		if(!strcmp(nameFile,sndScnTable[i].li.name))
			break;
	if(i==nSndScn)
		return SNDNOHANDLE;
	return INDEX2HANDLE(i);
}

// Returns the name of the scene from the handle
char *SndScnGetName(DWORD hndl)
{
	if(!ISVALIDHANDLE(hndl)) 	
		return NULL;
	return sndScnTable[HANDLE2INDEX(hndl)].li.name;
}

DWORD SndScnGetNSnd(DWORD hndl)
{
	if(!ISVALIDHANDLE(hndl)) 	
		return 0;
	return sndScnTable[HANDLE2INDEX(hndl)].nSnd;
}

//-----------------------------------------------------------------
// special functions for the editor

// Returns the number of scenes
DWORD SndScnTblGetN(void)
{
	return nSndScn;
}

// copy the table of scenes play in tblCopy
void SndScnTblCopy(LPSNDLISTITEM tblCopy)
{
	DWORD i;
	for(i=0; i<nSndScn; i++)
		COPYMEMORY(tblCopy+i,&sndScnTable[i].li,sizeof(SNDLISTITEM));		
}

// The list tblCopy of nScn scenes saves on the rows
void SndScnTblSave(DWORD nScn, LPSNDLISTITEM tblCopy)
{
	FILE *pf;
	DWORD nSndScn=nScn;
	char nameFull[MAX_PATH];
	char *nameTbl=SNDSCNTBLNAME;
	DWORD i;
	BuildFullPath(nameFull,SNDSCNPATH, nameTbl, sndDriveContext,SNDLISTEXT);
	pf=SndListOpen(nameFull,&nSndScn,'w');
	ASSERTFILE(pf!=NULL,nameFull);
	for(i=0; i<nSndScn; i++)
		SndListItemWrite(pf,tblCopy+i);
	SndListClose(pf);	
}


// write and reads singlo an item that it describes a sound 
BOOL SndInfoWrite(SNDINFO *pSndInfo, FILE *pf)
{
	return (fwrite(pSndInfo, sizeof(SNDINFO), 1, pf)>0);
}

BOOL SndInfoRead(SNDINFO *pSndInfo, FILE *pf)
{
	return (fread(pSndInfo, sizeof(SNDINFO), 1, pf)>0);
}


// Initialize and load scene
FILE *SndScnInitLoad(SNDNAME name, DWORD *pnSnd,LOADHANDLE lpBlock, LOADPATH lpPath)
{
	FILE *pf=NULL;
	char scene[MAX_PATH];
	char scnHeader[sizeof(SNDSCNHEADER)+1];
		
	BuildFullPath(scene,SNDSCNPATH,name,sndDriveContext, SNDSCNEXT);	
	pf=fopen(scene,FMTREAD);	
	ASSERTFILE(pf!=NULL,scene);
	fread(&scnHeader,sizeof(char),strlen(SNDSCNHEADER)+1,pf);	
	ASSERTFILE(strcmp(scnHeader,SNDSCNHEADER)==0,scene);	
	fread(pnSnd,sizeof(DWORD),1,pf);
	fread(lpBlock,sizeof(LOADHANDLE),1,pf);
	fread(lpPath,sizeof(LOADPATH),1,pf);	
	return pf;
}

// Initialize the save of the scene
FILE *SndScnInitSave(SNDNAME name, DWORD *pnSnd, LOADHANDLE lpBlock, LOADPATH lpPath)
{
	FILE *pf=NULL;
	char scene[MAX_PATH];
		
	BuildFullPath(scene,SNDSCNPATH,name,sndDriveContext, SNDSCNEXT);	
	pf=fopen(scene,FMTWRITE);	
	ASSERTFILE(pf!=NULL,scene);
	fwrite(SNDSCNHEADER,sizeof(char),strlen(SNDSCNHEADER)+1,pf);		
	fwrite(pnSnd,sizeof(DWORD),1,pf);
	fwrite(lpBlock,sizeof(LOADHANDLE),1,pf);
	fwrite(lpPath,sizeof(LOADPATH),1,pf);	
	return pf;	
}

// Chiude la scena
void SndScnClose(FILE *pf)
{
	fclose(pf);
}

//-----------------------------------------------------------------
// Environment load functions

void SndScnEnvLoadGeneral(SND_GAME_ENV env, BOOL linkFlag)
{
	DWORD i;	
	if(SndEnvCmpEq(env,NOSNDENV))
		return;
	for(i=0; i<nSndScn; i++) {
		if(SndEnvCmpEq(sndScnTable[i].li.env,env)){	
			if(linkFlag && !SndEnvCmpEq(sndScnTable[i].li.envLink,NOSNDENV) )
				SndScnEnvLoadGeneral(sndScnTable[i].li.envLink,FALSE);
			HSndScnLoad(INDEX2HANDLE(i));			
		}
	}
}

void SndScnEnvLoad(SND_GAME_ENV env)
{
	SndScnEnvLoadGeneral(env,TRUE);
}

void SndScnEnvDestroyGeneral(SND_GAME_ENV env, BOOL linkFlag)
{
	DWORD i;
	if(SndEnvCmpEq(env,NOSNDENV))
		return;	
	for(i=0; i<nSndScn; i++) {
		if(SndEnvCmpEq(sndScnTable[i].li.env,env)){	
			HSndScnDestroy(INDEX2HANDLE(i));
			if(linkFlag && !SndEnvCmpEq(sndScnTable[i].li.envLink,NOSNDENV) )
				SndScnEnvDestroyGeneral(sndScnTable[i].li.envLink,FALSE);
		}
	}
}

void SndScnEnvDestroy(SND_GAME_ENV env)
{
	SndScnEnvDestroyGeneral(env,TRUE);
}
//---------------------------------------------------------------------

//loads a scene in memory and it creates the sounds
BOOL HSndScnLoad(DWORD hFile)
{
	FILE *pf;
	
	SNDSCENE *pcrScn;
	SNDINFO sndInfo;
	DWORD i;	
	
	if(nSndScn==0 || !ISVALIDHANDLE(hFile)) //no init system
		return FALSE;
	
	pcrScn=&sndScnTable[HANDLE2INDEX(hFile)];
	if(pcrScn->nSnd!=0){ // already loaded
		return TRUE;
	}
	pf=SndScnInitLoad(pcrScn->li.name, &pcrScn->nSnd, pcrScn->loadBlock,pcrScn->loadPath);
	ASSERTFILE(pf!=NULL,pcrScn->li.name);
	pcrScn->table=MyMalloc(LPSNDOBJ,pcrScn->nSnd);
	ASSERTOUTMEMORY(pcrScn->table!=NULL);
	//Create sounds they are NULL if no sound card
	for(i=0; i<pcrScn->nSnd; i++){		
		SndInfoRead(&sndInfo, pf);
		// I substitute the correct drive letter
		//sndInfo.loadPath[0]=sndDriveContext;		
		pcrScn->table[i]=SndObjCreate(&sndInfo,pcrScn->loadBlock,pcrScn->loadPath);
		//myassert(sndScnTable[hFile]!=NULL,NULL,sndInfo.loadID);		
	}		
	fclose(pf);
	return TRUE;
}

// the object Returns sound correspondent to the hFile scene and the handle hSnd
LPSNDOBJ HSndScnGetObj(DWORD hFile,DWORD hSnd)
{
	//it checks if sound card is detected if not it returns NULL
	if(!HwSndGetCardDetect() || nSndScn==0  || !ISVALIDHANDLE(hFile) || hSnd==SNDNOHANDLE)
		return NULL;	
 	ASSERTRANGE(HANDLE2INDEX(hSnd)<sndScnTable[HANDLE2INDEX(hFile)].nSnd);
	return sndScnTable[HANDLE2INDEX(hFile)].table[HANDLE2INDEX(hSnd)];
}

// destroys the hFile scene
void HSndScnDestroy(DWORD hFile)
{
	DWORD iFile;
	if(nSndScn==0 || !ISVALIDHANDLE(hFile)) //no init system
		return;
	// Libera i suoni creati
	iFile=HANDLE2INDEX(hFile);
	if(sndScnTable!=NULL && sndScnTable[iFile].table!=NULL){
		DWORD iSnd;
		for(iSnd=0; iSnd<sndScnTable[iFile].nSnd; iSnd++){
			SndObjDestroy(sndScnTable[iFile].table[iSnd]);
			sndScnTable[iFile].table[iSnd]=NULL;
		}
		MyFree(sndScnTable[iFile].table);		
		sndScnTable[iFile].table=NULL;
	}
	sndScnTable[iFile].nSnd=0;	
}

void SndScnDestroyAll(void)
{
	DWORD i;
	for(i=0; i<nSndScn; i++)
		HSndScnDestroy(INDEX2HANDLE(i));
}
