#include "btypes.h"
#include "myerror.h"
#include <math.h>

#define SNDTOL PVALUE(0.1)
#define SND_PNOTEQ(t1,t2) (PGREAT(PABS((t2)-(t1)),SNDTOL))

void HwSndNormalize(MYVECTOR pos)
{
	int i;
	ASSERTRANGE(pos[3]!=PVALUE(0.0));	
	for(i=0; i<=2; i++)
		PDIV(pos[i],pos[3]);
	pos[3]=PVALUE(1.0);
}

//Returns TRUE if vector are almost equal (see tollerance)
BOOL HwSndCompareMYVECTOR(PREAL *v1, PREAL *v2)
{
	int i;	
	FATALASSERT(v1!=NULL && v2!=NULL);
	for(i=0; i<4; i++)
		if(SND_PNOTEQ(v1[i],v2[i]))
			break;
		return i==4;
}

PREAL HwSndSquareDistance(MYVECTOR pos1, MYVECTOR pos2, PREAL dFact)
{
	static PREAL dFact2=PVALUE(0.0);
	PREAL d=PVALUE(0);
	PREAL temp;
	int i;
	dFact2=PSQUARE(dFact);
	if(pos1[3]!=PVALUE(1.0))
		HwSndNormalize(pos1);
	if(pos2[3]!=PVALUE(1.0))
		HwSndNormalize(pos2);
	for(i=0;i<=2;i++){
		temp=PSUB(pos1[i],pos2[i]);
		d=PADD(d,PSQUARE(temp));
	}
	d=PMULT(d,dFact2);	
	return d;
}

void SndObjInitVelocity(MYVECTOR vel)
{
	int i;
	for(i=0; i<3; i++)
		vel[0]=PVALUE(0);
	vel[3]=PVALUE(1);
}

void SndObjComputeVelocity(PREAL *lpVel, PREAL *lpPos1, PREAL *lpPos2, PREAL deltaT)
{
	DWORD i;
	FATALASSERT(lpVel!=NULL && lpPos1!=NULL && lpPos2!=NULL && 
		SND_PNOTEQ(deltaT,PVALUE(0)));
	for(i=0; i<3; i++){ //I suppose MYVECTOR[3]=1.0!!!!!!!!!!
		lpVel[i]=PSUB(lpPos2[i],lpPos1[i]);
		//deltaT is in milliseconds, i convert it in seconds		
		lpVel[i]=PDIV(PMULT(lpVel[i], PVALUE(1000)), deltaT);
	}
	lpVel[3]=PVALUE(1);
}

DWORD SndObjComputePercent(DWORD factor, DWORD value,DWORD cento)
{
	return (DWORD) ( PMULT(PDIV(INTTOP(factor),INTTOP(cento)),value) );
}