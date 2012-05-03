#ifndef SPLINEFILE 
#define SPLINEFILE

typedef struct _SPLINE{
	char Stop;
	signed char Inversion;
	signed char PInversion;
	D3DVECTOR P;
	D3DVECTOR Q;
	D3DVECTOR R;
	PREAL T;
	PREAL InvP;
	PREAL tTan;
	PREAL tRes;
	MYMATRIX Matrix;
} SPLINE, *LPSPLINE;

LPSPLINE LocateTime(PREAL T, LPSPLINE Sp, int N);

void GetSplineValue(LPSPLINE Sp, MYVECTOR Result, BOOL doit);

void GetSplineFrame(LPSPLINE Sp, PREAL *Res, BOOL RecomputeInv);

void GetSplineSpeed(LPSPLINE Sp, MYVECTOR tan);

void PrepareSpline (PREAL Newp);

void InversionPoint(LPSPLINE Sp);

#endif
