#include <windows.h>
#include "myerror.h"
#include "sndgame.h"
#include "sndconv.h"

#define MAXTEMPSTR 300

void SndCnvCheckString(char *str, size_t len, char *strTgt)
{
	ASSERTFILE(strlen(str)<len,str);
	if(str[0]=='0')
		str[0]='\0';
	strcpy(strTgt,str);
}

void SndCnvListName(char *nameSrc, char *nameTgt)
{
	FILE *pfSrc, *pfTgt;
	DWORD nList;
	DWORD nReadItem;
	SNDLISTITEM sli;
	char strTemp[MAXTEMPSTR];
	char strTemp1[MAXTEMPSTR];
	char strTemp2[MAXTEMPSTR];
	
	pfSrc=fopen(nameSrc, "rt");
	ASSERTFILE(pfSrc!=NULL,nameSrc);

	nReadItem=fscanf(pfSrc,"%d\n",&nList);
	ASSERTFILE(nReadItem==1, nameSrc);
	ASSERTFILE(nList>0,nameSrc);		
	pfTgt=SndListOpen(nameTgt,&nList,'w');
	ASSERTFILE(pfTgt!=NULL,nameTgt);
	while(!feof(pfSrc)){		
		nReadItem=fscanf(pfSrc,"%s %s %s\n",strTemp, strTemp1, strTemp2);
		ASSERTFILE(nReadItem==3, nameSrc);
		SndCnvCheckString(strTemp,MAXSNDNAME,sli.name);
		SndCnvCheckString(strTemp1,MAXSNDENVNAME,sli.env);
		SndCnvCheckString(strTemp2,MAXSNDENVNAME,sli.envLink);
		SndListItemWrite(pfTgt,&sli);
	}	
	SndListClose(pfTgt);
	fclose(pfSrc);
}


void SndCnvCheck3D(int tempInCone, int tempOutCone, int tempOutVol, float tempMin,
				   float tempMax, LPSND3DINFO lpP3D)
{
	lpP3D->setFlag=0;
	if(tempInCone>=0){//!=NO3DVALUE
		lpP3D->props.dwInsideConeAngle=tempInCone;
		lpP3D->setFlag|=SND_INCONESET;
	}
	if(tempOutCone>=0){ //!=NO3DVALUE
		lpP3D->props.dwOutsideConeAngle=tempOutCone;
		lpP3D->setFlag|=SND_OUTCONESET;
	}
	if(tempOutVol>=0){ //!=NO3DVALUE
		lpP3D->props.OutsideVolumePerCent=tempOutVol;
		lpP3D->setFlag|=SND_OUTVOLSET;
	}
	if(tempMin>=0){ //!=NO3DVALUE
		lpP3D->props.flMinDistance=tempMin;
		lpP3D->setFlag|=SND_MINDSET;
	}
	if(tempMax>=0){ //!=NO3DVALUE
		lpP3D->props.flMaxDistance=tempMax;
		lpP3D->setFlag|=SND_MAXDSET;
	}
}

void SndCnvScn(char *nameSrc, char *nameTgt)
{
	FILE *pfSrc, *pfTgt;
	DWORD nSnd;
	LOADPATH lPath;
	LOADHANDLE lBlock;
	SNDINFO  si;
	int tempInCone, tempOutCone, tempOutVol;
	float tempMin,tempMax;
	DWORD nReadItem;
	char strTemp[MAXTEMPSTR];
	char strTemp1[MAXTEMPSTR];
	char strTemp2[MAXTEMPSTR];
	
	pfSrc=fopen(nameSrc, "rt");
	ASSERTFILE(pfSrc!=NULL,nameSrc);
	
	nReadItem=fscanf(pfSrc,"%d %s %s",&nSnd, strTemp1, strTemp2);	
	ASSERTFILE(nReadItem==3, nameSrc);
	ASSERTRANGE(nSnd>0);	
	SndCnvCheckString(strTemp1,sizeof(LOADHANDLE),lBlock);
	SndCnvCheckString(strTemp2,sizeof(LOADPATH),lPath);
	
	pfTgt=SndScnInitSave(nameTgt, &nSnd, lBlock, lPath);	
	while (!feof(pfSrc)){
		nReadItem=fscanf(pfSrc,"%s %d %d %d %d %d %d %f %f \n",
			strTemp, &si.sndClass, &si.sndType, &si.nInstRequired,
			&tempInCone,&tempOutCone,&tempOutVol,&tempMin,&tempMax);							
		ASSERTFILE(nReadItem==9, nameSrc);
		// check 		
		SndCnvCheckString(strTemp,sizeof(LOADHANDLE),si.loadID);
		SndCnvCheck3D(tempInCone,tempOutCone,tempOutVol,tempMin,tempMax,&si.param3D);
		SndInfoWrite(&si, pfTgt);
	}	
	SndScnClose(pfTgt);
	fclose(pfSrc);
}

void SndCnvAnimation(char *nameSrc, char *nameTgt)
{
	FILE *pfSrc, *pfTgt;
	SNDANIMHEAD saHead;	
	DWORD i;
	SNDANIMOBJ obj;
	SNDKEYFRAME key;
	DWORD nReadItem;
	char strTemp[MAXTEMPSTR];
	
	pfSrc=fopen(nameSrc, "rt");
	ASSERTFILE(pfSrc!=NULL,nameSrc);
	nReadItem=fscanf(pfSrc,"%d %d %d %d %d\n",
			&saHead.nKey,&saHead.nObj,&saHead.bLoop,&saHead.type,&saHead.hSndScn);
	ASSERTFILE(nReadItem==5, nameSrc);
	ASSERTFILE(saHead.nKey>0,nameSrc);
	ASSERTFILE(saHead.nObj>0,nameSrc);	
	ASSERTFILE(saHead.bLoop==FALSE || saHead.bLoop==TRUE,nameSrc);		
	ASSERTFILE(saHead.type==SATYPE_REALTIME || saHead.type==SATYPE_TIMEFRAME,nameSrc);
	// I can not Know snd scn range
	pfTgt=SndAnimFileOpen(nameTgt,&saHead,'w');
	for(i=0; i<saHead.nObj; i++){
		nReadItem=fscanf(pfSrc, "%d %s %d %d %d %d %d\n",
			&obj.hSnd,strTemp, &obj.vol, &obj.fadeVol,
			&obj.pan,&obj.playFlag, &obj.nextPlay);	
		ASSERTFILE(nReadItem==7, nameSrc);
		SndCnvCheckString(strTemp,MAXLENNAMEOBJ,obj.nameGObj);
		SndAnimWriteObj(&obj, pfTgt);
	}

	for(i=0; i<saHead.nKey; i++){
		nReadItem=fscanf(pfSrc, "%d %d %d %d\n",
			&key.time, &key.cmnd, &key.idObj, &key.arg1);
		ASSERTFILE(nReadItem==4, nameSrc);						
		SndAnimWriteKey(&key, pfTgt);
	}
	SndAnimFileClose(pfTgt);
	fclose(pfSrc);
}