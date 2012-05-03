#include "myenv.h"
#include <math.h>
#include <ddraw.h>
#include "fastmat.h"

float AuxiliaryResult;
//float tseno[S_TABLEDIM];

//#define MULTI_RAD (RAD*PRECISION)
float MULTI_RAD = RAD*PRECISION;
float tseno[FS_TABLEDIM];
float tcoseno[FS_TABLEDIM];
float troot[R_TABLEDIM];
float tatan[A_TABLEDIM];
short tSqrt[256];
int __x__;
/*
void SinTable (void) {
        int i;
        PIGRECO=atan(1)*4;
        PIGRECO2=2*PIGRECO;
        PIGRECOD2=PIGRECO/2;
		invPIGRECOD2=1/PIGRECOD2;
        for (i=0; i<S_TABLEDIM; i++) {
                tseno[i]=sin((PIGRECOD2*((float)i))/(S_TABLEDIM-1));
                
        }
}
*/

void SinCosTable(void) {
       int i;
/*        PIGRECO=atan(1)*4;
        PIGRECO2=2*PIGRECO;
        PIGRECOD2=PIGRECO/2;
		invPIGRECOD2=1/PIGRECOD2;
*/
		  for (i=0; i<FS_TABLEDIM; i++) {
                tseno[i]=sin(i/MULTI_RAD);
				tcoseno[i]=cos(i/MULTI_RAD);
        }
}



void AtanTable (void)
{
	int i;       
	for (i=0; i<A_TABLEDIM; i++) {
		tatan[i]=atan(((float)i)/(A_TABLEDIM-1));
	}
}

/*float fastsin(float x){
    static char negative;
    static long y;
    x=fmod((double)x, (double)PIGRECO2);
    if (x>PIGRECO) x=x-PIGRECO2;
    if (x<0) {negative=TRUE; x=-x;}
    else negative=FALSE;
    if (x>PIGRECOD2) x=PIGRECO-x;
    y=floor((x*(S_TABLEDIM-1))/PIGRECOD2+0.5);
    if (negative) return -tseno[y];
    else return tseno[y];
}*/

/*////////////////////
float fastsin(float y){
    static char negative;
    static long x;
    x=floor((y*(S_TABLEDIM-1))*invPIGRECOD2+0.5);
    x=x%((S_TABLEDIM-1)*4);
    if (x>((S_TABLEDIM-1)*2)) x=x-((S_TABLEDIM-1)*4);
    if (x<-((S_TABLEDIM-1)*2)) x=x+((S_TABLEDIM-1)*4);
    if (x<0) {negative=TRUE; x=-x;}
    else negative=FALSE;
    if (x>(S_TABLEDIM-1)) x=((S_TABLEDIM-1)*2)-x;
    if (negative) return -tseno[x];
    else return tseno[x];
}

float fastcos(float y){
    static char negative;
    static long x;
    x=floor((y*(S_TABLEDIM-1))*invPIGRECOD2+0.5);
    x=(S_TABLEDIM-1)-x;
    x=x%((S_TABLEDIM-1)*4);
    if (x>((S_TABLEDIM-1)*2)) x=x-((S_TABLEDIM-1)*4);
    if (x<-((S_TABLEDIM-1)*2)) x=x+((S_TABLEDIM-1)*4);
    if (x<0) {negative=TRUE; x=-x;}
    else negative=FALSE;
    if (x>(S_TABLEDIM-1)) x=((S_TABLEDIM-1)*2)-x;
    if (negative) return -tseno[x];
    else return tseno[x];
}
////////////////*/
/*******
__inline float fastsin(float y){
	return tseno[(int)(y*MULTI_RAD)];
}
__inline float fastcos(float y){
	return tcoseno[(int)(y*MULTI_RAD)];
}
********/

/*
 * aegis : 
 * y와 z의 sign이 같을 때를 고려하지 않고 넘겨주는 값. 
 * [-pi/2, pi/2] 사이의 값을 넘겨준다. 방향을 고려한다면 
 * 이 함수 이후에 별도의 작업이 필요하다. 왜냐면, (-1,-1)과 (1,1)의 fastatan()의
 * 값이 동일하기 때문이다.
 */
float fastatan(float y, float z)
{
	int negative;
	long x;

	negative=FALSE;
	if (y<0) {negative=!negative; y=-y;}
	if (z<0) {negative=!negative; z=-z;}
	if (y<=z) {
		x=floor((y*(A_TABLEDIM-1)/z)+.5);
		if (negative) 
			return -tatan[x];
		else
			return tatan[x];
	}
	else {
		x=floor((z*(A_TABLEDIM-1))/y+.5);
		if (negative) 
			return tatan[x]-PIGRECOD2;
		else
			return PIGRECOD2-tatan[x];
	}
}

void SqrtTable(void){
	int i;
	float f;
	int *f_=(int *)(&f);
	for (i=0; i<0x7f;i++){
		//*f_=0;
		*f_=(i<<16)|(127<<23);
		f=sqrt(f);
		tSqrt[i] = (*f_&0x7FFFFF)>>16;

		//*f_=0;
		*f_=(i<<16)|(128<<23);
		f=sqrt(f);
		tSqrt[i+0x80] = (*f_&0x7FFFFF)>>16;
	}
}


void RootTable (void) {
        int i;
        for (i=0; i<R_TABLEDIM; i++) {
                troot[i]=sqrt((2*((float)i)/(R_TABLEDIM-1)));
                
        }
}

float fastroot(float x){
    int i;
    if (x<0) return 0;
    if(x>2.) x=2.;
    i=floor(((R_TABLEDIM-1)*x)*0.5+.5);
    return troot[i];
}

float Modulo2D(D3DVALUE x, D3DVALUE y){
    static float x1, y1;
    x1=x*x; y1=y*y;
    if (x1>y1) return x*fastroot(y1/x1);
    else return y*fastroot(x1/y1);
}

float Modulo3D(D3DVALUE x, D3DVALUE y, D3DVALUE z){
    static float x1, y1, z1, max, max1;
    x1=x*x; y1=y*y; z1=z*z;
    if (x1>y1) {max1=x1; max=x;}
    else {max1=y1; max=y;}
    if (z1>max1) {max1=z1; max=z;}
    return max*fastroot(x1/max1+y1/max1+z1/max1-1.);
}
/*
void MyNormalize(D3DVECTOR *v){
    D3DVALUE Mod;
    Mod=Modulo3D(v->x, v->y, v->z);
    v->x=v->x/Mod;
    v->y=v->y/Mod;
    v->z=v->z/Mod;
}*/
/*
void InitFastMath(void){
    SinTable ();
    RootTable ();
}*/

void MyNormalize2D(float *x, float *y){
    static float theta;
    theta=fastatan(*y, *x);
    if (*x>=0){
        *x=fastCos_asm(theta);
        *y=fastSin_asm(theta);
    }
    else{
        *x=-fastCos_asm(theta);
        *y=-fastSin_asm(theta);        
    }
}

/*
 * fastatan()은 [-pi/2, pi/2] 사이의 값을 넘겨주는데, 이 함수는
 * 이것을 [0, pi] 사이의 값으로 만들어서 넘겨준다.
 */
float GetAngleFromCoords(float x, float y)
{
    static float theta;

	theta = fastatan(y, x);
    if (x >= 0) {
		if (y >= 0) // 1
			return theta;
		else        // 4
			return theta + PIGRECO*2;
	}
    else {
		// 2, 3
		return theta + PIGRECO;
    }
}

// aegis : think x and y are length and height of a triangle. then this function just returns
// the length of hypotenuse (빗변)
float Modulo2DT(D3DVALUE x, D3DVALUE y)
{
    static float temp1, temp2;
    temp1=x;
    temp2=y;
    MyNormalize2D(&temp1, &temp2);
    if (fabs(temp1)>fabs(temp2)) return x/temp1;
    else return y/temp2;
}



void MyNormalize(D3DVECTOR *v){
    static float temp, temp1, temp2, temp0;
	
    temp1=(v->z);
    temp2=(v->x);
    MyNormalize2D(&temp2, &temp1);
    if (fabs(temp1)>fabs(temp2)){
		temp=(v->z);
		temp0=temp1;
	}
	else{
		temp=(v->x);
		temp0=temp2;
	}
	if (fabs(temp0)>0)
		temp=(AuxiliaryResult=temp/temp0);
    else AuxiliaryResult=temp=0;
    MyNormalize2D(&temp, &(v->y));
    (v->z)=temp1*temp;
    (v->x)=temp2*temp;
}

float Modulo3DT(D3DVECTOR *v){
    static float temp, temp1, temp2, temp0, temp3;
	temp3=(v->y);
    temp1=(v->z);
    temp2=(v->x);
    MyNormalize2D(&temp2, &temp1);
    if (fabs(temp1)>fabs(temp2)){
		temp=(v->z);
		temp0=temp1;
	}
	else{
		temp=(v->x);
		temp0=temp2;
	}
	if (fabs(temp0)>0)
		temp=temp/temp0;
	else temp=0;  
    MyNormalize2D(&temp, &temp3);
	if (fabs(v->y)>temp) {temp=(v->y);temp0=temp3;}
	if (fabs(temp0)>0) return temp/temp0;
	else return 0.;
}


void InitFastMath(void){
//  SinTable ();
	SinCosTable ();
	RootTable ();
	AtanTable (); 
	SqrtTable();
}
