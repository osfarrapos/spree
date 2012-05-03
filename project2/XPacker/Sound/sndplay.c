#include "sndlow.h"
#include "sndplay.h"

#define NSNDPLAYSLOT 50
#define ISVALIDINDEX(ind) ((ind)<NSNDPLAYSLOT)
#define ISVALIDHANDLE(hnd) ((hnd)!=SNDNOHANDLE)
#define ISSLOTFREE(slot) ((slot).lpso==NULL)

#define INDBITS ((DWORD)8)
#define HNDBITS ((DWORD) (32-INDBITS)) //24
#define GETINDEX(extHnd) ((extHnd)>>HNDBITS)
#define GETCURRHND(extHnd) ((extHnd)&0x00ffffff)
#define GETEXTERNALHANDLE(ind, hnd) ( (ind)<<HNDBITS | (hnd))
#define SNDMAXHANDLE GETCURRHND(0xffffffff)
#define ISFREELPSLOT(lpSlot) (lpSlot->lpso==NULL)
#define ISFREEINDEX(ind) (playSystem.vPlayObj[ind].lpso==NULL)
	
typedef struct SNDPLAYLISTtag {
	SNDPLAYSLOT *head;
	SNDPLAYSLOT *tail;
} SNDPLAYLIST, *LPSNDPLAYLIST;

typedef struct {
	SNDPLAYLIST playList;
	SNDPLAYLIST freeList;	
	SNDPLAYSLOT vPlayObj[NSNDPLAYSLOT];
	DWORD nPlaying;
} SNDPLAYSYSTEM;

static SNDPLAYSYSTEM playSystem;

//----------------------------------------------------------------------------

// Initialize system
void SndPlaySlotInitSystem(void)
{
	DWORD i;
	playSystem.playList.head=NULL;
	playSystem.playList.tail=NULL;	
	playSystem.nPlaying=0;
	for(i=0;i<NSNDPLAYSLOT; i++){
		playSystem.vPlayObj[i].ind=i;
		playSystem.vPlayObj[i].lpso=NULL;
		playSystem.vPlayObj[i].pauseOn=FALSE;
		playSystem.vPlayObj[i].currHnd=SNDNOHANDLE;
		if(i==NSNDPLAYSLOT-1)
			playSystem.vPlayObj[i].next=NULL;			
		else
			playSystem.vPlayObj[i].next=playSystem.vPlayObj+(i+1);
		if(i==0)
			playSystem.vPlayObj[i].prev=NULL;
		else
			playSystem.vPlayObj[i].prev=playSystem.vPlayObj+(i-1);
	}
	playSystem.freeList.head=playSystem.vPlayObj;
	playSystem.freeList.tail=playSystem.vPlayObj+(NSNDPLAYSLOT-1);	
}

// Delete an element using index and vector from list (free or play)
void SndPlayListDelete(LPSNDPLAYLIST lpList, DWORD i)
{
	if(lpList->head==NULL)
		return; // no element to delete
	DEBUGASSERT(ISVALIDINDEX(i));
	if(!ISVALIDINDEX(i))
		return;
	if(playSystem.vPlayObj[i].prev==NULL) //It's the head of the list
		lpList->head=playSystem.vPlayObj[i].next;
	else 
		playSystem.vPlayObj[i].prev->next=playSystem.vPlayObj[i].next;
	if(playSystem.vPlayObj[i].next==NULL) //It's the tail of the list
		lpList->tail=playSystem.vPlayObj[i].prev;
	else
		playSystem.vPlayObj[i].next->prev=playSystem.vPlayObj[i].prev;
}

// Append an element using index and vector from list (free or play)
void SndPlayListAppend(LPSNDPLAYLIST lpList, DWORD i)
{
	DEBUGASSERT(ISVALIDINDEX(i));
	if(!ISVALIDINDEX(i))
		return;
	if(lpList->head==NULL){ //list is empty
		lpList->head=playSystem.vPlayObj+i;		
		playSystem.vPlayObj[i].prev=NULL;		
	}
	else{
		lpList->tail->next=playSystem.vPlayObj+i;
		playSystem.vPlayObj[i].prev=lpList->tail;				
	}
	playSystem.vPlayObj[i].next=NULL;
	lpList->tail=playSystem.vPlayObj+i;
}

	
DWORD SndPlayGetNextHandle(DWORD hnd)
{
	if(hnd==SNDNOHANDLE || hnd==SNDMAXHANDLE) //I don't use SNDNOHANDLE(=0)
		hnd=1;
	else
		hnd++;
	return hnd;
}

BOOL SndPlayGetIndexFromHandle(SNDHANDLE hnd, DWORD *lpInd)
{
	*lpInd=GETINDEX(hnd);
	DEBUGASSERT(ISVALIDINDEX(*lpInd));
	if(!ISVALIDINDEX(*lpInd))
		return FALSE;
	return playSystem.vPlayObj[*lpInd].currHnd==GETCURRHND(hnd);
}

SNDHANDLE SndPlayAllocateHnd(LPHWSNDOBJ lpso)
{
	SNDHANDLE hnd=SNDNOHANDLE;

	if (lpso!=NULL){		
		LPSNDPLAYSLOT lpSlot=playSystem.freeList.head;
		if(lpSlot!=NULL){
			lpSlot->lpso=lpso;
			SndPlayListDelete(&playSystem.freeList,lpSlot->ind);
			SndPlayListAppend(&playSystem.playList,lpSlot->ind);
			lpSlot->currHnd=SndPlayGetNextHandle(lpSlot->currHnd);							
			hnd=GETEXTERNALHANDLE(lpSlot->ind,lpSlot->currHnd);
			playSystem.nPlaying++;
		}
	}
	return hnd; 
}

BOOL SndPlayCheckIndex(DWORD ind)
{
	DEBUGASSERT(ISVALIDINDEX(ind));
	if(!ISVALIDINDEX(ind))
		return FALSE;
	//DEBUGASSERT(playSystem.vPlayObj[ind].lpso);
	if(playSystem.vPlayObj[ind].lpso && !HwSndObjIsPlaying(playSystem.vPlayObj[ind].lpso)
													&& !playSystem.vPlayObj[ind].pauseOn){
		HwSndObjSetHnd(playSystem.vPlayObj[ind].lpso,SNDNOHANDLE);
		playSystem.vPlayObj[ind].lpso=NULL;		
		SndPlayListDelete(&playSystem.playList,ind);
		SndPlayListAppend(&playSystem.freeList,ind);
		playSystem.nPlaying--;
		return FALSE;
	}
	return TRUE;
}

void SndPlayStopHnd(DWORD hnd)
{
	DWORD ind;
	if (SndPlayGetIndexFromHandle(hnd,&ind)) {
		if(playSystem.vPlayObj[ind].lpso){
			HwSndObjStop(playSystem.vPlayObj[ind].lpso);			
			playSystem.vPlayObj[ind].pauseOn=FALSE;
		}
	}
}

void SndPlayFreeHnd(SNDHANDLE hnd)
{
	DWORD ind;	
	if(SndPlayGetIndexFromHandle(hnd, &ind)){
		SndPlayStopHnd(hnd);
		while(SndPlayIsPlayingHnd(hnd))
			;
		SndPlayCheckIndex(ind);
	}
}

void SndPlayPauseHnd(DWORD hnd)
{
	DWORD ind;
	if(SndPlayGetIndexFromHandle(hnd,&ind)) {
		if(playSystem.vPlayObj[ind].lpso){
			HwSndObjPause(playSystem.vPlayObj[ind].lpso);			
			playSystem.vPlayObj[ind].pauseOn=TRUE;
		}
	}
}

void SndPlayResumeHnd(DWORD hnd)
{
	DWORD ind;
	if(SndPlayGetIndexFromHandle(hnd,&ind)) {
		if(playSystem.vPlayObj[ind].lpso){
			HwSndObjResume(playSystem.vPlayObj[ind].lpso);			
			playSystem.vPlayObj[ind].pauseOn=FALSE;
		}
	}
}

void SndPlaySetVolumeHnd(DWORD hnd, DWORD vol)
{
	DWORD ind;
	if(SndPlayGetIndexFromHandle(hnd,&ind))
		if(playSystem.vPlayObj[ind].lpso)
			HwSndObjSetVolume(playSystem.vPlayObj[ind].lpso,vol);		
}

void SndPlayGetVolumeHnd(DWORD hnd, DWORD *lpVol)
{
	DWORD ind;
	if(SndPlayGetIndexFromHandle(hnd,&ind))
		if(playSystem.vPlayObj[ind].lpso)
			HwSndObjGetVolume(playSystem.vPlayObj[ind].lpso,lpVol);				
}

void SndPlaySetPanHnd(DWORD hnd, int pan)
{
	DWORD ind;
	if(SndPlayGetIndexFromHandle(hnd,&ind))
		if(playSystem.vPlayObj[ind].lpso)
			HwSndObjSetPan(playSystem.vPlayObj[ind].lpso,pan);
}

void SndPlayGetPanHnd(DWORD hnd, int *lpPan)
{
	DWORD ind;
	if(SndPlayGetIndexFromHandle(hnd,&ind))
		if(playSystem.vPlayObj[ind].lpso)
			HwSndObjGetPan(playSystem.vPlayObj[ind].lpso,lpPan);		
}

BOOL SndPlayIsPlayingHnd(DWORD hnd)
{
	DWORD ind;
	if(SndPlayGetIndexFromHandle(hnd,&ind))
		if(playSystem.vPlayObj[ind].lpso)			
			return HwSndObjIsPlaying(playSystem.vPlayObj[ind].lpso);		
	return FALSE;
}

void SndPlayStopAll(void)
{
	LPSNDPLAYSLOT scan;
	DWORD ind;
	scan=playSystem.playList.head;
	while(scan!=NULL){			
		if(!ISFREELPSLOT(scan))
			HwSndObjStop(scan->lpso);
		ind=scan->ind;
		scan=scan->next;
		SndPlayCheckIndex(ind);						
	}						
}

void SndPlayClsStop(DWORD cls)
{
	LPSNDPLAYSLOT scan;
	DWORD ind;
	if(cls==SNDCLS_NONE)
		return;
	scan=playSystem.playList.head;
	while(scan!=NULL){			
		if(!ISFREELPSLOT(scan))
			if(HwSndObjGetCls(scan->lpso)==cls)
				HwSndObjStop(scan->lpso);
		ind=scan->ind;
		scan=scan->next;
		SndPlayCheckIndex(ind);				
	}						
}

void SndPlayUpdateClsVolume(DWORD cls)
{
	LPSNDPLAYSLOT scan;
	DWORD ind;
	if(cls==SNDCLS_NONE)
		return;
	scan=playSystem.playList.head;
	while(scan!=NULL){			
		if(!ISFREELPSLOT(scan))
			if(HwSndObjGetCls(scan->lpso)==cls)
				HwSndObjRefreshVolume(scan->lpso);			
		ind=scan->ind;
		scan=scan->next;
		SndPlayCheckIndex(ind);			
	}		
}

void SndPlayRefresh(void)
{
	LPSNDPLAYSLOT scan;
	DWORD ind;
	scan=playSystem.playList.head;
	while(scan!=NULL){	
		ind=scan->ind;
		scan=scan->next;
		SndPlayCheckIndex(ind);					
	}			
}

//------------------------------------------------------------------------------------

#define TIME3DCHANGE 100 // in millesimi di secondo

void SndPlayRTUpdate(void)
{
	LPSNDPLAYSLOT scan;
	DWORD ind;
	static DWORD timeChange3D=0;
	DWORD newTimeChange3D;
	BOOL bChange=FALSE,bChangeLs=FALSE;
	PREAL deltaT;

	newTimeChange3D=SndGetTime();
	SndStreamRTUpdate();
	deltaT=INTTOP(newTimeChange3D-timeChange3D);
	if(deltaT>TIME3DCHANGE)
		bChangeLs=HwSndUpdateListener(deltaT);
	scan=playSystem.playList.head;
	while(scan!=NULL){
		ind=scan->ind;	
		scan=scan->next;
		if(SndPlayCheckIndex(ind)){
			if(deltaT>TIME3DCHANGE){
				if(playSystem.vPlayObj[ind].lpso){	
					if(HwSndObjIs3DSound(playSystem.vPlayObj[ind].lpso))
						bChange=HwSndObjUpdate3D(playSystem.vPlayObj[ind].lpso,deltaT, bChangeLs);			
				}
			}
		}		
	}
	if(deltaT>TIME3DCHANGE){		
		if(bChange||bChangeLs)
			HwSnd3DExecuteChanges();	
		timeChange3D=newTimeChange3D;
	}
}
