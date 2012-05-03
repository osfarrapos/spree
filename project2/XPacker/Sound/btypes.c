#include "myenv.h"
#include <stdlib.h>
#include <math.h>
#include <mmsystem.h>
#include "fastmat.h"

static MYMATRIX RotX, RotY, RotZ, Temp, Temp1;
float a123temp;
#define MY_GETANGLE(x,y) GETANGLE(x,y)

void InitializeEuler(void)
{
	SetXRotation(RotX, 0);
	SetYRotation(RotY, 0);
	SetZRotation(RotZ, 0);
}

#define EPS_SIN PVALUE(.005)
#define EPS_ANGLE PVALUE(0.1)
#define DELTA_ZERO PVALUE(0.001) //PVALUE(.000000001)
void GetEulerAngles_2(PREAL *MM, MYVECTOR Euler);

void GetEulerAngles_2(PREAL *MM, MYVECTOR Euler)
{
	float cos_x;
	
	if(MM[6]>1.0) MM[6]=1.0;
	if(MM[6]<-1.0) MM[6]=-1.0;
	
	Euler[0]=asin(MM[6]);
	cos_x=cos(Euler[0]);
	if (NEAR_TO(cos_x, PVALUE(0.0), DELTA_ZERO)){		
		Euler[1]=PVALUE(0);
		Euler[2]=MY_GETANGLE(MM[0], MM[1]);
	} else{
		Euler[1]=MY_GETANGLE(MM[10]/cos_x, INV(MM[2]/cos_x));
		Euler[2]=MY_GETANGLE(MM[5]/cos_x, INV(MM[4]/cos_x));
	}
}

//#if 0
#define EPS_SIN_2 PVALUE(.000005)
void GetEulerAngles(PREAL *MM, MYVECTOR Euler)
{
	if (NEAR_TO(MM[6], PVALUE(1.0), EPS_SIN_2)){
		Euler[0]=PIGRECOD2;
		Euler[1]=PVALUE(0);
		Euler[2]=MY_GETANGLE(MM[0], MM[1]);
	}
	else if (NEAR_TO(MM[6], PVALUE(-1.0), EPS_SIN_2)){
		Euler[0]=INV(PIGRECOD2);
		Euler[1]=PVALUE(0);
		Euler[2]=MY_GETANGLE(MM[0], MM[1]);
	}
	else{
		Euler[1]=MY_GETANGLE(MM[10], INV(MM[2]));
		Euler[2]=MY_GETANGLE(MM[5], INV(MM[4]));
		if (NEAR_TO(Euler[2], PIGRECOD2, EPS_ANGLE)
			|| NEAR_TO(Euler[2], INV(PIGRECOD2), EPS_ANGLE)){
			Euler[0]=MY_GETANGLE(PDIV(-MM[4], PSIN(Euler[2])), MM[6]);
		}
		else
			Euler[0]=MY_GETANGLE(PDIV(MM[5], PCOS(Euler[2])), MM[6]);
	}
}


#if 0
void GetEulerAngles(PREAL *MM, MYVECTOR Euler)
{
	int i;
	MYVECTOR Euler_2;
	if (NEAR_TO(MM[6], PVALUE(1.0), EPS_SIN)){
		Euler[0]=PIGRECOD2;
		Euler[1]=PVALUE(0);
		Euler[2]=MY_GETANGLE(MM[0], MM[1]);
	}
	else if (NEAR_TO(MM[6], PVALUE(-1.0), EPS_SIN)){
		Euler[0]=INV(PIGRECOD2);
		Euler[1]=PVALUE(0);
		Euler[2]=MY_GETANGLE(MM[0], MM[1]);
	}
	else{
		Euler[1]=MY_GETANGLE(MM[10], INV(MM[2]));
		Euler[2]=MY_GETANGLE(MM[5], INV(MM[4]));
		if (NEAR_TO(Euler[2], PIGRECOD2, EPS_ANGLE)
			|| NEAR_TO(Euler[2], INV(PIGRECOD2), EPS_ANGLE)){
			Euler[0]=MY_GETANGLE(PDIV(-MM[4], sin(Euler[2])), MM[6]);
		}else
			Euler[0]=MY_GETANGLE(PDIV(MM[5], cos(Euler[2])), MM[6]);
	}
	GetEulerAngles_2(MM, Euler_2);
}
#endif


//RyRxRz
void SetEulerAngles(PREAL *MM, MYVECTOR Euler)
{
	UpdateXRotation(RotX, Euler[0]);
	UpdateYRotation(RotY, Euler[1]);
	UpdateZRotation(RotZ, Euler[2]);
	MatrixMultiply(RotY, RotX, Temp);
	MatrixMultiply(Temp, RotZ, Temp1);
	memcpy(MM, Temp1, 12*sizeof(PREAL));
}

void UpdateClock(ABSTIME *lpClock, SMALLTIMEINTERVAL DeltaT)
{
	int partial;
	lpClock->milliseconds+=DeltaT;
	partial=lpClock->milliseconds/1000;
	lpClock->milliseconds-=partial*1000;
	lpClock->seconds+=partial;
	partial=lpClock->seconds/60;
	lpClock->seconds-=partial*60;
	lpClock->minutes+=partial;
	partial=lpClock->minutes/60;
	lpClock->minutes-=partial*60;
	lpClock->hours+=partial;
	partial=lpClock->hours/24;
	lpClock->hours-=partial*24; 
	lpClock->timeInMSecs+=DeltaT;
}

extern float gf_default_inctime; // aegis
SMALLTIMEINTERVAL DeltaTime(void)
{
	static ULONG PrevTime=0;
#ifdef TNDEMO
	extern int IsNetEnabled;
#endif
	ULONG ActTime, Result;
	ActTime=timeGetTime();
	Result=ActTime-PrevTime;
	PrevTime=ActTime;
#ifdef TNDEMO
	if (IsNetEnabled)
		return (SMALLTIMEINTERVAL)gf_default_inctime;
	else
		return (SMALLTIMEINTERVAL)gf_default_inctime;//33.3;//Result;
#endif
	//	Result = Result >> 2;
	if (Result>100) Result=100;
		return (SMALLTIMEINTERVAL)Result;   
}

PREAL IncTime;
PREAL IncTimeSec;
PREAL CurrTime;

SMALLTIMEINTERVAL MyTime(void)
{
	extern DWORD myTimeGetTime();
	return (SMALLTIMEINTERVAL)myTimeGetTime();//timeGetTime();
}


void MatrixMultiply(MYMATRIX x, MYMATRIX y, MYMATRIX z)
{
	__asm mov edi, z;
	__asm mov ecx, x;
	__asm mov edx, y;
    MatPerMat_Asm();
}

/*
 * |x vector| * [Y matrix]  = |z vector|
 */
void VByMatrixMultiply(MYVECTOR x, MYMATRIX y, MYVECTOR z)
{
	static unsigned char  l, j, j1, s;
	memset(z, 0 , sizeof(MYVECTOR));
	for(l=0; l<4; l++)
		for(j=0, j1=0; j<4; j++, j1+=4){
			s=j1+l;
			if (x[j]==PVALUE(0)||y[s]==PVALUE(0))
				;
			else if (x[j]==PVALUE(1))
				z[l]=PADD(z[l], y[s]);
			else if (y[s]==PVALUE(1))
				z[l]=PADD(z[l], x[j]);
			else
				z[l]=PADD(z[l], PMULT(x[j],y[s]));
		}
}				  

void InvVByMatrixMultiply(MYVECTOR x, MYMATRIX y, MYVECTOR z)
{
	static unsigned char  l, j, j1, s;
	memset(z, 0 , sizeof(MYVECTOR));
	for(l=0; l<4; l++)
		for(j=0, j1=0; j<4; j++, j1+=4){
			s=j1+l;
			if (x[j]==PVALUE(0)||y[s]==PVALUE(0))
				;
			else if (x[j]==PVALUE(1))
				z[l]=PSUB(z[l], y[s]);
			else if (y[s]==PVALUE(1))
				z[l]=PSUB(z[l], x[j]);
			else
				z[l]=PSUB(z[l], PMULT(x[j],y[s]));
		}
}

void VectorProduct(D3DVECTOR *v1, D3DVECTOR *v2, D3DVECTOR *res)
{
	res->x=PSUB(PMULT(v1->y, v2->z), PMULT(v2->y, v1->z));
	res->y=PSUB(PMULT(v1->z, v2->x), PMULT(v2->z, v1->x));
	res->z=PSUB(PMULT(v1->x, v2->y), PMULT(v2->x, v1->y));
}

void BBoxTransform(MYVECTOR VI1, MYVECTOR VI2, MYMATRIX y,
	                    MYVECTOR z1, MYVECTOR z2)
{
	static unsigned char  l, j, j1, s;
	static PREAL *x1, *x2;
	memset(z1, 0 , sizeof(MYVECTOR));
	memset(z2, 0 , sizeof(MYVECTOR));
	for(l=0; l<4; l++)
		for(j=0, j1=0; j<4; j++, j1+=4){
			s=j1+l;
			if (y[s]>0) {
				x1=VI1;
				x2=VI2;
			}	else{
				x2=VI1;
				x1=VI2;
			}
			if (x1[j]==PVALUE(0)||y[s]==PVALUE(0))
				;
			else if (x1[j]==PVALUE(1))
				z1[l]+=y[s];
			else if (y[s]==PVALUE(1))
				z1[l]+=x1[j];
			else
				z1[l]+=PMULT(x1[j],y[s]);

			if (x2[j]==PVALUE(0)||y[s]==PVALUE(0))
				;
			else if (x2[j]==PVALUE(1))
				z2[l]+=y[s];
			else if (y[s]==PVALUE(1))
				z2[l]+=x2[j];
			else
				z2[l]+=PMULT(x2[j], y[s]);
		}
}

void Transpose(MYMATRIX x, MYMATRIX y)
{
	int i, j, i1, j1;
	for(i=0, i1=0;i1<4; i+=4, i1++)
		for(j=0, j1=0; j<4;j++, j1+=4){
			y[i1+j1]=x[i+j];
		}
}

void InvertOMatrix(MYMATRIX x, MYMATRIX y)
{
	static MYVECTOR T;
	static int i, j, i1, j1;
	for(i=0, i1=0;i1<3; i+=4, i1++)
		for(j=0, j1=0; j<3;j++, j1+=4){
			y[i1+j1]=x[i+j];
		}
	y[3]=PVALUE(0); y[7]=PVALUE(0); y[11]=PVALUE(0); y[15]=PVALUE(1);
	memset(y+12, 0, 3*sizeof(PREAL));
	InvVByMatrixMultiply(x+12, y, T);
	memcpy(y+12, T, 3*sizeof(PREAL));
}

void SetZRotation(MYMATRIX x, PREAL theta)
{
	PREAL coseno, seno;
	memset(x, 0, sizeof(MYMATRIX));
	seno=PSIN(theta); coseno=PCOS(theta);
	x[0]=coseno;
	x[1]=seno;
	x[4]=INV(seno);
	x[5]=coseno;
	x[10]=PVALUE(1);
	x[15]=PVALUE(1);    
}

void UpdateZRotation(MYMATRIX x, PREAL theta)
{
	PREAL coseno, seno;
	seno=PSIN(theta); coseno=PCOS(theta);
	x[0]=coseno;
	x[1]=seno;
	x[4]=INV(seno);
	x[5]=coseno;
}

/*
 * [ cos(t)   0   -sin(t)   0  ]
 * [   0      1      0      0  ]
 * [ sin(t)   0    cos(t)   0  ]
 * [   0      0      0      1  ]
 */
void SetYRotation(MYMATRIX x, PREAL theta)
{
	PREAL coseno, seno;
	memset(x, 0, sizeof(MYMATRIX));
	seno=PSIN(theta); coseno=PCOS(theta);
	x[0]=coseno;
	x[8]=seno;
	x[2]=INV(seno);
	x[10]=coseno;
	x[5]=PVALUE(1);
	x[15]=PVALUE(1);    
}

void UpdateYRotation(MYMATRIX x, PREAL theta)
{
	PREAL coseno, seno;
	seno=PSIN(theta); coseno=PCOS(theta);
	x[0]=coseno;
	x[8]=seno;
	x[2]=INV(seno);
	x[10]=coseno;
}

void SetXRotation(MYMATRIX x, PREAL theta)
{
	PREAL coseno, seno;
	memset(x, 0, sizeof(MYMATRIX));
	seno=PSIN(theta); coseno=PCOS(theta);
	x[5]=coseno;
	x[6]=seno;
	x[9]=INV(seno);
	x[10]=coseno;
	x[0]=PVALUE(1);
	x[15]=PVALUE(1);    
}

void UpdateXRotation(MYMATRIX x, PREAL theta)
{
	PREAL coseno, seno;
	seno=PSIN(theta); coseno=PCOS(theta);
	x[5]=coseno;
	x[6]=seno;
	x[9]=INV(seno);
	x[10]=coseno;  
}

