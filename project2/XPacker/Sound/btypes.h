#ifndef __BTYPES_H
#define __BTYPES_H

#pragma warning(disable:4100)  // unreferenced formal parameter
#pragma warning(disable:4115)  // named type definition in parentheses
#pragma warning(disable:4201)  // nonstandard extension used : nameless struct/union

#include "fastmat.h"

////////////////////BASIC PREAL/////////////////////
typedef float PREAL;

#define PVALUE(x) ((float)(x))
#define INTTOP(x) ((float)(x))
#define PTOINT(x) (PtoInt(x))

#define PZERO PVALUE(0)
#define PUNO  PVALUE(1)
#define PDUE  PVALUE(2)
#define PTRE  PVALUE(3)
#define P4    PVALUE(4)
#define PNOVE PVALUE(9)

#define PREAL_MAX FLT_MAX


#define PABS(x) ((float) fabs(x))

#define PLESS(x, y) ((x)<(y))
#define PLESSEQ(x, y) ((x)<=(y))
#define PNOTEQ(x,y) ((x)!=(y))
#define PEQ(x,y) ((x)==(y))
#define PGREAT(t1,t2) (PLESS((t2),(t1)))
#define PGREATEQ(t1,t2) (PLESSEQ((t2),(t1)))

#define PISZERO(x) ((x)==PZERO)
#define NEAR_TO(x, y, eps) PLESS(PABS(PSUB(x, y)), eps)

#define PIS_BETWEEN(x,m,M) ((x)>(m) && (x)<(M))
#define PIS_BETWEEN_EQ(x,m,M) ((x)>=(m) && (x)<=(M))

#define PSIGN(x) (x==0 ? 0 : x/PABS(x) )

#define PADD(x, y) ((x)+(y))
#define PINC(x, y) ((x)+=(y))
#define PDEC(x, y) ((x)-=(y))
#define PSUB(x, y) ((x)-(y))
#define INV(x) (-(x))

#define PMULT(x,y)     ((x)*(y))
#define PREPMULT(x, y) ((x)*(y))
#define PDIV(x,y)      ((x)/(y))
#define PREPDIV(x,y)   ((x)/(y))
#define PINV(x)        ((1/(x)))
#define PREPINV(x)     ((1)/(x))

#define PSQUARE(x)    (PMULT(x,x))
#define PREPSQUARE(x) (PREPMULT(x,x))

#define HALF(x)    ((x)/2)
#define PDOUBLE(x) (2*(x))

typedef PREAL ANGLE;

#define ANGLE_MIN_VALUE (1)

/////////////// CONVERSION RAD <-> ANGLE /////////////////
// aegis : Radian과 Angle을 1:1 비율로 사용하고 있음 -> 변환이 동일한 결과로 나옴
#define RAD_PI PIGRECO
#define FACTOR_RAD2ANGLE PREPDIV(PIGRECO, RAD_PI)
#define FACTOR_ANGLE2RAD PREPDIV(RAD_PI, PIGRECO)

#define ANGLE2RAD(x) PMULT((x),FACTOR_ANGLE2RAD)
#define RAD2ANGLE(x) PMULT((x),FACTOR_RAD2ANGLE)

#define PREANGLE2RAD(x) PREPMULT((x),FACTOR_ANGLE2RAD)
#define PRERAD2ANGLE(x) PREPMULT((x),FACTOR_RAD2ANGLE)

///////////// CONVERSION DEGREE <-> ANGLE ///////////////
// dirty calculation to gain precision
#define DEG180 180
#define FACTOR_DEG2ANGLE PREPDIV(PIGRECO, DEG180)
#define FACTOR_ANGLE2DEG PREPDIV(DEG180, PIGRECO)

#define ANGLE2DEG(x) INTTOP(PMULT((x),FACTOR_ANGLE2DEG))
#define DEG2ANGLE(x) PTOINT(PMULT((x),FACTOR_DEG2ANGLE))

#define PREANGLE2DEG(x) PVALUE(PREPMULT((x),FACTOR_ANGLE2DEG))
#define PREDEG2ANGLE(x) PTOINT(PREPMULT((x),FACTOR_DEG2ANGLE))

//reduction of angle to minimize its absolute value
extern float a123temp;
// aegis : something wrong! if NANGLE normalizes angle value from -Pi to Pi then
//         when PABS(X) must be compared with PIGRECO instead of PIGRECO2.
//         On the other hand, if NANGLE normalizes angle value from 0 to 2*Pi then
//         we can get it just by fmod after adding 2*Pi
#if 0 // original code
#define NANGLE(x) ( (PABS(x)<=PIGRECO2 ? a123temp=(float) (x) : (a123temp=(float) fmod(x, PIGRECO2))), \
				  (a123temp>PIGRECO ? (a123temp-PIGRECO2): \
					(a123temp<-PIGRECO ? (a123temp+PIGRECO2):a123temp)))
#else
#define NANGLE(x) ( (PABS(x)<=PIGRECO ? a123temp=(float) (x) : \
                      (a123temp=(float) fmod(x, PIGRECO2))), \
					   (a123temp>PIGRECO ? (a123temp-PIGRECO2): \
					   (a123temp<-PIGRECO ? (a123temp+PIGRECO2):a123temp)))
#endif
#define PNANGLE(x) PVALUE(NANGLE((x)))

////////////////////PREAL TRIG  /////////////////////
//I add it because often  x is not normalized and fast fn are not robust
#define PSIN_SECURE(x) (fastSin_asm(PNANGLE(x)))
#define PCOS_SECURE(x) (fastCos_asm(PNANGLE(x)))
#define PSIN(x)        PSIN_SECURE(x)
#define PCOS(x)        PCOS_SECURE(x)
#define PATAN(x)       (fastatan(x))

#define PTAN(x)        (PDIV(PSIN(x),PCOS(x)))
#define PCOTAN(x)      (PDIV(PCOS(x),PSIN(x)))
#define PPATAN(x)      fastatan(x,PVALUE(1.0))

#define PSINCOS(theta, lpSin, lpCos) {*(lpSin)=PSIN(theta); *(lpCos)=PCOS(theta);}

////////////////////PREAL OTHER OPS /////////////////
#define PMAX max
#define PMIN min

#define PEXP(x) ((float)exp(x))
#define FASTROOT(x) fastroot(x)
#define PSQRT(x) ((float) sqrt((float)x))//(fastSqrt_asm(x))

////////////////////VECTOR MATH /////////////////////
typedef  PREAL MYMATRIX[16];
typedef  PREAL MYVECTOR[4];

#define DotProduct(x, y) PADD(PMULT(x[0], y[0]), PADD(PMULT(x[1], y[1]), PMULT(x[2], y[2])))
#define DotProduct2(x, y, a) PADD(PMULT(x[0], y[0]), PADD(PMULT(PMULT(x[1], y[1]), a), PMULT(x[2], y[2])))

#define VSUB(x, y, z) ((z[0]=PSUB(x[0], y[0])), \
					   (z[1]=PSUB(x[1], y[1])), \
					   (z[2]=PSUB(x[2], y[2])))

#define VADD(x, y, z) ((z[0]=PADD(x[0], y[0])), \
					   (z[1]=PADD(x[1], y[1])), \
					   (z[2]=PADD(x[2], y[2])))

#define VMULT(v, a, vf)   ((vf[0]=PMULT(v[0], a)), \
						   (vf[1]=PMULT(v[1], a)), \
					       (vf[2]=PMULT(v[2], a)))

#define CROSSPRODUCT(VI1, VI2, VO)\
	VO[0]=PSUB(PMULT(VI1[2], VI2[1]), PMULT(VI1[1], VI2[2]));\
	VO[1]=PSUB(PMULT(VI1[0], VI2[2]), PMULT(VI1[2], VI2[0]));\
	VO[2]=PSUB(PMULT(VI1[1], VI2[0]), PMULT(VI1[0], VI2[1]))

//MTVECTOR to D3DVECTOR conversion
#define MYV_TO_D3DV(d3dv,myv) (d3dv.x=myv[0],d3dv.y=myv[1],d3dv.z=myv[2])

//conversion to PREAL pointer, i.e. I convert a pointer to myvector pointer
#define PV(v) ((PREAL *)(v))
///VSUB coordinate for D3DVETOR e MYVECTOR
#define V_SUB_CRD(v1,v2,i) ( PV(v1)[i] - PV(v2)[i] )

//MODULO
#define MODULO2D(x, y) Modulo2DT((x),(y))
#define MODULO3D(v) Modulo3DT(v)
#define TED_MODULO3D(P) ( PSQRT( PADD(PSQUARE(P[0]), \
									PADD(PSQUARE(P[1]),PSQUARE(P[2]))) ) )

#define MODXZ_2(v) PADD(PSQUARE(PV(v)[0]),PSQUARE(PV(v)[2]))
#define MOD3D_2(v) DotProduct(v,v)

//norma del max
#define max3(x,y,z) max(max(x,y),z)
#define NORM_MAX_V(v) max3(PABS(PV(v)[0]),PABS(PV(v)[1]),PABS(PV(v)[2]))
#define NORM_MAX_XZ(v) (max(PABS(PV(v)[0]),PABS(PV(v)[2]))) 

//normalize fns
#define NORMALIZE2D(x, y) MyNormalize2D((&(x)), (&(y))) // MyNormalize2D((x), (y))
#define NORMALIZE3D(v) MyNormalize(v)

////////////////////GETANGLE FNS/////////////////////
//if x=K*cos(theta), and y=K*sin(theta) the function below returns theta
#define GETANGLE(x, y) ((PREAL)GetAngleFromCoords((float)(x), (float)(y)))

// angle calculation
// GETANGLE_ZX() : Z 축을 기준으로하는 angle (goal line과의 각도)
// GETANGLE_XZ() : X 축을 기준으로 하는 angle (side line과의 각도)
#define GETANGLE_ZX(v1,v2) ( GETANGLE(V_SUB_CRD(v2,v1,2),V_SUB_CRD(v2,v1,0)) )
#define GETANGLE_XZ(v1,v2) ( GETANGLE(V_SUB_CRD(v2,v1,0),V_SUB_CRD(v2,v1,2)) )

////////////////////MATRIX OPS  /////////////////////
void MatrixMultiply(MYMATRIX x, MYMATRIX y, MYMATRIX z);
void VByMatrixMultiply(MYVECTOR x, MYMATRIX y, MYVECTOR z);
void InvVByMatrixMultiply(MYVECTOR x, MYMATRIX y, MYVECTOR z);
void VectorProduct(D3DVECTOR *v1, D3DVECTOR *v2, D3DVECTOR *res);

void BBoxTransform(MYVECTOR VI1, MYVECTOR VI2, MYMATRIX y, MYVECTOR VO1, MYVECTOR VO2);
void Transpose(MYMATRIX x, MYMATRIX y);
void InvertOMatrix(MYMATRIX x, MYMATRIX y);

//ROTATIONS AND EULER ANGLES
void SetZRotation(MYMATRIX x, PREAL theta);
void UpdateZRotation(MYMATRIX x, PREAL theta);
void SetYRotation(MYMATRIX x, PREAL theta);
void UpdateYRotation(MYMATRIX x, PREAL theta);
void SetXRotation(MYMATRIX x, PREAL theta);
void UpdateXRotation(MYMATRIX x, PREAL theta);

void InitializeEuler(void);
void GetEulerAngles(PREAL *MM, MYVECTOR Euler);
void SetEulerAngles(PREAL *MM, MYVECTOR Euler);

////////////////////TIME AND CLOCK///////////////////
typedef int BIGTIMEINTERVAL ;   //seconds
typedef int SMALLTIMEINTERVAL ; //milliseconds

typedef struct _ABSTIME {
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
    unsigned int milliseconds;
	unsigned int timeInMSecs;
} ABSTIME;

void UpdateClock(ABSTIME *lpClock, SMALLTIMEINTERVAL DeltaT);
SMALLTIMEINTERVAL MyTime(void);
SMALLTIMEINTERVAL DeltaTime(void);

extern PREAL IncTime;
extern PREAL IncTimeSec;

#define DEFAULT_INCTIME PVALUE(100.0) // PVALUE(33.33) : aegis

////////////////////BOUNDING BOX/////////////////////
#define BOUNDINGBOX D3DRMBOX
#define LPBOUNDINGBOX LPD3DRMBOX

////////////////////RAND FNS///////////////////////
#define SET_FIRST_RAND(s) srand(s)
#define P_INIT_RAND() srand(timeGetTime())
extern int myrand();
extern int rand_log(char *, int);
#define PRAND(X)    (X>0 ? myrand()%(X) + rand_log(__FILE__, __LINE__) : 0)
#define BALL_RAND() (ball_rand()%(X) + rand_log(__FILE__, __LINE__))
static int TempP;
#define DOUBLE_CHOICE(P1) ((PRAND(100)<P1) ? 0 : 1)
#define TRIPLE_CHOICE(P1, P2) (((TempP=PRAND(100))<P1) ? 0: ((TempP<(P1+P2)) ? 1 : 2))

#define RANDNORM() ((float)PRAND(RAND_MAX)/(float)RAND_MAX)
#define RANDFLOAT(X) (RANDNORM()*(float)X)

////////////////////VARIE FNS///////////////////////
#include <stdlib.h> 
#define SORT(v,n_elem,size_elem,cmp) qsort(v, n_elem, size_elem, cmp);

#define COMPUTE_EXTREME(NEWPNT,PNT,MOD, theta) {\
		PREAL sin_t, cos_t;\
		PSINCOS(theta, &sin_t, &cos_t);\
		(NEWPNT)[0]=PADD((PNT)[0],PMULT(MOD,cos_t)); \
		(NEWPNT)[2]=PADD((PNT)[2],PMULT(MOD,sin_t));\
}

#define POINT_XZ_DISTANCE_2(P1,P2) \
		PADD(PSQUARE(PSUB((P2)[0],(P1)[0])),PSQUARE(PSUB((P2)[2],(P1)[2])))
#define POINT_XZ_DISTANCE(P1,P2) \
		MODULO2D((PSUB((P2)[0],(P1)[0])),(PSUB((P2)[2],(P1)[2])))

#define GET_DISTANCE_2D_2(x1, y1, x2, y2)	\
		PADD(PSQUARE(PSUB(x2,x1)),PSQUARE(PSUB(y2,y1))

#define PLESSTRUNK(x, min) \
		{ if (x<min) x=min; }
#define PGREATTRUNK(x, max) \
		{ if (x>max) x=max; }
#define PBETWEENTRUNK(x, min, max)	\
		{ if (x<min) x=min; if (x>max) x=max; }

//STRUCTURE GIVEN FOR FUNCTION OF PAINT OF RIGID BODY
typedef struct _SOLID_OBJ{
	MYVECTOR Position;
	MYVECTOR Euler;
	unsigned int FrameHandle;
} SOLID_OBJ;

//PATH STRINGS/////////
typedef char NEWNAME[MAX_PATH];

#endif
