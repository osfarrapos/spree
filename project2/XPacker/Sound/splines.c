#include "myenv.h"
#include "splines.h"
 
static PREAL ActualTimeInterval;
static PREAL ActualTime;
LPSPLINE LocateTime(PREAL T, LPSPLINE Sp, int N){
	int i;
	for (i=0; i<N; i++){
		if (Sp[i].T>=T) break;
	}
	if (i>=N) {
		i=N-1; 
		T=Sp[i].T;
	}
	if (i>0) {
		ActualTimeInterval=PSUB(Sp[i].T, Sp[i-1].T);
		ActualTime=PSUB(T, Sp[i-1].T);
	}
	else{ 
		ActualTimeInterval=Sp[i].T;
		ActualTime=T;
	}
	return Sp+i;
}

static PREAL pCubo, pQuadro, p, ipCubo, ipQuadro, ip, p2i, pi2;
void GetSplineValue(LPSPLINE Sp, MYVECTOR Result, BOOL doit){	
	p=PDIV(ActualTime, ActualTimeInterval);
	ip=PSUB(PVALUE(1.), p);
	pQuadro=PMULT(p, p);
	ipQuadro=PMULT(ip, ip);
	pCubo=PMULT(p, pQuadro);
	ipCubo=PMULT(ip, ipQuadro);
	p2i=PMULT(PMULT(pQuadro, ip), PVALUE(3.));
	pi2=PMULT(PMULT(ipQuadro, p), PVALUE(3.));
	if(doit){
		Result[3]=PVALUE(1.);
		Result[0]=PADD(PADD(PMULT(Sp->P.x, ipCubo), PMULT((Sp+1)->P.x, pCubo)), 
					   PADD(PMULT(p2i, Sp->R.x), PMULT(pi2, Sp->Q.x)));
		Result[1]=PADD(PADD(PMULT(Sp->P.y, ipCubo), PMULT((Sp+1)->P.y, pCubo)), 
					   PADD(PMULT(p2i, Sp->R.y), PMULT(pi2, Sp->Q.y)));
		Result[2]=PADD(PADD(PMULT(Sp->P.z, ipCubo), PMULT((Sp+1)->P.z, pCubo)), 
					   PADD(PMULT(p2i, Sp->R.z), PMULT(pi2, Sp->Q.z)));
	}
}

void PrepareSpline (PREAL Newp){
	p=Newp;
	ip=PSUB(PVALUE(1.), p);
	pQuadro=PMULT(p, p);
	ipQuadro=PMULT(ip, ip);
	pCubo=PMULT(p, pQuadro);
	ipCubo=PMULT(ip, ipQuadro);
	p2i=PMULT(PMULT(pQuadro, ip), PVALUE(3.));
	pi2=PMULT(PMULT(ipQuadro, p), PVALUE(3.));
}

static PREAL cQ, cR, cP, cP1;
void GetSplineSpeed(LPSPLINE Sp, MYVECTOR tan){
	cP=INV(ipQuadro);
	cP1=pQuadro;
	cQ=PADD(PMULT(PMULT(PVALUE(-2.), ip), p), ipQuadro);
	cR=PADD(PMULT(PMULT(PVALUE(2.), ip), p), INV(pQuadro));
	tan[3]=PVALUE(0.);
	tan[0]=PADD(PADD(PMULT(cP, Sp->P.x), PMULT(cP1, (Sp+1)->P.x)), 
				PADD(PMULT(cQ, Sp->Q.x), PMULT(cR, Sp->R.x)));
	tan[1]=PADD(PADD(PMULT(cP, Sp->P.y), PMULT(cP1, (Sp+1)->P.y)), 
				PADD(PMULT(cQ, Sp->Q.y), PMULT(cR, Sp->R.y)));
	tan[2]=PADD(PADD(PMULT(cP, Sp->P.z), PMULT(cP1, (Sp+1)->P.z)), 
				PADD(PMULT(cQ, Sp->Q.z), PMULT(cR, Sp->R.z)));
}
static MYMATRIX Reg;
#define MIN_VECT PVALUE(.02)
#define MIN_VECT_RESULT PVALUE(.01)
#define INV_PREC PVALUE(.01)
#define DIST_PREC PVALUE(.2)
/*
#define PSQRT(x) ((float) sqrt((float)(x)) )
#define PSQUARE(x) (PMULT(x,x))
#define PREPSQUARE(x) (PREPMULT(x,x))
*/
void InversionPoint(LPSPLINE Sp){
	D3DVECTOR Den, Num;
	PREAL Mod;
	BOOL InvPValid;
	Den.x=PADD(PSUB((Sp+1)->P.x, Sp->P.x), PMULT(PVALUE(3), PSUB(Sp->Q.x, Sp->R.x)));
	Num.x=PADD(Sp->P.x, PSUB(Sp->R.x, PMULT(PVALUE(2), Sp->Q.x)));

	Den.y=PADD(PSUB((Sp+1)->P.y, Sp->P.y), PMULT(PVALUE(3), PSUB(Sp->Q.y, Sp->R.y)));
	Num.y=PADD(Sp->P.y, PSUB(Sp->R.y, PMULT(PVALUE(2), Sp->Q.y)));

	Den.z=PADD(PSUB((Sp+1)->P.z, Sp->P.z), PMULT(PVALUE(3), PSUB(Sp->Q.z, Sp->R.z)));
	Num.z=PADD(Sp->P.z, PSUB(Sp->R.z, PMULT(PVALUE(2), Sp->Q.z)));
	
	Mod=PMULT(PSQRT(PADD(PSQUARE(Den.x), PADD(PSQUARE(Den.y), PSQUARE(Den.z)))), INV_PREC);
	InvPValid=FALSE;
	if (PLESS(PABS(Den.x), Mod)){
		if (PLESS(PABS(Num.x), Mod)) ;
		else {Sp->InvP=PVALUE(10000); return;}
	}
	else{
		InvPValid=TRUE;
		Sp->InvP=PDIV(INV(Num.x), Den.x);
	}

	if (PLESS(PABS(Den.y), Mod)){
		if (PLESS(PABS(Num.y), Mod)) ;
		else {Sp->InvP=PVALUE(10000); return;}
	}
	else{
		PREAL Temp;
		Temp=PDIV(INV(Num.y), Den.y);
		if (InvPValid&&PLESS(INV_PREC, PABS(PSUB(Temp, Sp->InvP)))){
			Sp->InvP=PVALUE(10000); return;
		}
		else if (!InvPValid) 
			Sp->InvP=Temp;
		InvPValid=TRUE;
	}

	if (PLESS(PABS(Den.z), Mod)){
		if (PLESS(PABS(Num.z), Mod)) ;
		else {Sp->InvP=PVALUE(10000); return;}
	}
	else{
		PREAL Temp;
		Temp=PDIV(INV(Num.z), Den.z);
		if (InvPValid&&PLESS(INV_PREC, PABS(PSUB(Temp, Sp->InvP)))){
			Sp->InvP=PVALUE(10000); return;
		}
		else if (!InvPValid) 
			Sp->InvP=Temp;
		InvPValid=TRUE;
	}
	if (PLESS(Sp->InvP, PVALUE(0))) Sp->InvP=PVALUE(10000);
	if (!InvPValid) Sp->InvP=PVALUE(-10000);
}

void GetSplineFrame(LPSPLINE Sp, PREAL *Result, BOOL RecomputeInv){

	PREAL *tan=Result+8;
	PREAL *binor=Result+4;
	PREAL Sc, Dec;
	PREAL *Reg;
	PREAL tanMax;
	BOOL bRetryTan;
	PREAL incTan;
	BOOL PInv;
	PREAL OldP;
	
	Reg=Sp->Matrix;
	
	incTan=INV(MIN_VECT);
	/*if(PLESS(p,PVALUE(0.5)))
		incTan=MIN_VECT;
	else 
		incTan=INV(MIN_VECT);
		*/
	OldP=p;
	if(PLESS(p,Sp->tTan)){
		memcpy(Result, Sp->Matrix, 3*sizeof(PREAL));
		VectorProduct((D3DVECTOR *)(Sp->Matrix+8), (D3DVECTOR *)Result, (D3DVECTOR *)binor);
		binor[3]=PVALUE(0.);
		return;
	}
	do{
		bRetryTan=FALSE;
		GetSplineSpeed(Sp,tan);
		tanMax=PMAX(PABS(tan[0]),PMAX(PABS(tan[1]),PABS(tan[2])));
		if(PLESS(tanMax,MIN_VECT)){
			bRetryTan=TRUE;
			PINC(p,incTan);			
			PrepareSpline(p);
		}
		if(PLESS(p,PVALUE(0))){
			memcpy(Result, Sp->Matrix, 3*sizeof(PREAL));
			VectorProduct((D3DVECTOR *)(Sp->Matrix+8), (D3DVECTOR *)Result, (D3DVECTOR *)binor);
			binor[3]=PVALUE(0.);
			if(PLESS(p,Sp->tTan))
				Sp->tTan=OldP;
			return;
		}
	}while(bRetryTan);
	//p=OldP;
	//PrepareSpline(p);
	//Dec=DotProduct(tan, (((PREAL *)Reg)+8));
	/*if (PLESS(PABS(Dec), MIN_VECT)){
		memcpy(tan, ((PREAL *)Reg)+8, sizeof(MYVECTOR));
	}
	else if (PLESS(Dec, PVALUE(0))){*/
	
	if(Sp->Inversion<0){
		tan[0]=INV(tan[0]);
		tan[1]=INV(tan[1]);
		tan[2]=INV(tan[2]);
	}
	/////choose right sign////////////
	NORMALIZE3D((D3DVECTOR *)tan);
///////////////////////////////
	if (PLESS(Sp->InvP, PVALUE(0))) {
		memcpy(Result, Sp->Matrix, 3*sizeof(PREAL));
		VectorProduct((D3DVECTOR *)tan, (D3DVECTOR *)Result, (D3DVECTOR *)binor);
		binor[3]=PVALUE(0.);
		return;
	}
	OldP=p;
	if(PLESS(p, Sp->tRes)){
		memcpy(Result, Sp->Matrix, 3*sizeof(PREAL));
		VectorProduct((D3DVECTOR *)tan, (D3DVECTOR *)Result, (D3DVECTOR *)binor);
		binor[3]=PVALUE(0.);
		return;
	}
	while (PLESS(PABS(PSUB(p, Sp->InvP)), MIN_VECT))
		PINC(p, MIN_VECT);
	PrepareSpline(p);
	do {
		bRetryTan=FALSE;
		cQ=PSUB(PMULT(p, PVALUE(3.)), PVALUE(2.));
		cR=PSUB(PVALUE(-1.), cQ);
		Result[3]=PVALUE(0.);
		Result[0]=INV(PADD(PADD(PMULT(ip, Sp->P.x), PMULT(p, (Sp+1)->P.x)), 
					PADD(PMULT(cQ, Sp->Q.x), PMULT(cR, Sp->R.x))));
		Result[1]=INV(PADD(PADD(PMULT(ip, Sp->P.y), PMULT(p, (Sp+1)->P.y)), 
					PADD(PMULT(cQ, Sp->Q.y), PMULT(cR, Sp->R.y))));
		Result[2]=INV(PADD(PADD(PMULT(ip, Sp->P.z), PMULT(p, (Sp+1)->P.z)), 
					PADD(PMULT(cQ, Sp->Q.z), PMULT(cR, Sp->R.z))));
		
		tanMax=PMAX(PABS(Result[0]),PMAX(PABS(Result[1]),PABS(Result[2])));
		if(PLESS(tanMax,MIN_VECT)){
			bRetryTan=TRUE;
			PINC(p,incTan);			
			PrepareSpline(p);
		}else{
			PREAL tanMax1;
			Sc=PADD(PMULT(tan[0], Result[0]), 
			PADD(PMULT(tan[1], Result[1]), PMULT(tan[2], Result[2])));
			PDEC(Result[0], PMULT(Sc, tan[0]));
			PDEC(Result[1], PMULT(Sc, tan[1]));
			PDEC(Result[2], PMULT(Sc, tan[2]));
			tanMax1=PMAX(PABS(Result[0]),PMAX(PABS(Result[1]),PABS(Result[2])));
			if(PLESS(tanMax1,PMULT(MIN_VECT_RESULT,tanMax))){
				bRetryTan=TRUE;
				PINC(p,incTan);			
				PrepareSpline(p);
			}
		}
		if(PLESS(p,PVALUE(0))){
			memcpy(Result, Sp->Matrix, 3*sizeof(PREAL));
			VectorProduct((D3DVECTOR *)tan, (D3DVECTOR *)Result, (D3DVECTOR *)binor);
			binor[3]=PVALUE(0.);
			if(PLESS(p,Sp->tRes))
				Sp->tRes=OldP;
			return;
		}
	}while (bRetryTan);	
	NORMALIZE3D((D3DVECTOR *)Result);
	if (RecomputeInv){
		Dec=DotProduct(Result, Reg);
		if (PLESS(Dec, PVALUE(0))){
			Sp->PInversion=TRUE;
		}
		else Sp->PInversion=FALSE;
		
	}
	PInv=Sp->PInversion;
	if (PLESS(Sp->InvP, p)) 
			PInv=!PInv; 
	if (PInv){
		Result[0]=INV(Result[0]);
		Result[1]=INV(Result[1]);
		Result[2]=INV(Result[2]);
	}
	/*if (Sp->Inversion<0){
		//perform a 180° turn to allow model move in reverse 
		tan[0]=INV(tan[0]);
		tan[1]=INV(tan[1]);
		tan[2]=INV(tan[2]);
		///////verify this inversion i dont know if it is correct!!
		Result[0]=INV(Result[0]);
		Result[1]=INV(Result[1]);
		Result[2]=INV(Result[2]);
	}*/
	NORMALIZE3D((D3DVECTOR *)Result);
	VectorProduct((D3DVECTOR *)tan, (D3DVECTOR *)Result, (D3DVECTOR *)binor);
	binor[3]=PVALUE(0.);
}