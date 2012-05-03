#ifndef FASTMAT
#define FASTMAT

#include <d3dtypes.h> 
#include "Mat4&Mat4.h"

#ifndef PIGRECO
#define PIGRECO   (3.141592654f)
#define PIGRECOD2 (PIGRECO/2.f)
#define PIGRECO2  (PIGRECO*2.f)
#endif

#define S_TABLEDIM (1024*16)
#define R_TABLEDIM (1024*16)
#define A_TABLEDIM (1024*16)

#define PRECISION 10 // (1/10) degree
#define RAD 57.29577951f
#define RIPETITION 5
#define H_FS_TABLEDIM (360*PRECISION*RIPETITION)
#define FS_TABLEDIM (H_FS_TABLEDIM*2+1)

extern float MULTI_RAD;
extern float tseno[FS_TABLEDIM];
extern float tcoseno[FS_TABLEDIM];
extern short tSqrt[256];
extern int __x__;

void SinTable (void);
void RootTable (void);
void SqrtTable(void);
void InitFastMath(void);
int PtoInt(float y);
float fastSin_asm(float x);
float fastCos_asm(float y);
float fastSqrt_asm(float y);
float fastatan(float y, float z);
float fastroot(float x);
float Modulo2D(D3DVALUE x, D3DVALUE y);
float Modulo2DT(D3DVALUE x, D3DVALUE y);
float Modulo3D(D3DVALUE x, D3DVALUE y, D3DVALUE z);
float Modulo3DT(D3DVECTOR *v);
float GetAngleFromCoords(float x, float y);
void MyNormalize2D(float *x, float *y);
void MyNormalize(D3DVECTOR *v);
extern float AuxiliaryResult;

__inline int PtoInt(float y)
{
	__asm{
		fld dword ptr y
		fistp dword ptr __x__
	}
	return __x__;
}

__inline float fastSin_asm(float y)
{
	__asm{
		fld dword ptr [y]
		fmul dword ptr [MULTI_RAD]
		fistp dword ptr [__x__]
		mov eax,[__x__]
		fld dword ptr tseno[eax*4+H_FS_TABLEDIM*4]
		fstp dword ptr [y]
	}
	return y;
}

__inline float fastCos_asm(float y)
{
	__asm{
		fld dword ptr [y]
		fmul dword ptr [MULTI_RAD]
		fistp dword ptr [__x__]
		mov eax,[__x__]
		fld dword ptr tcoseno[eax*4+H_FS_TABLEDIM*4]
		fstp dword ptr [y]
	}
	return y;
}

__inline float fastSqrt_asm(float y)
{
	__asm{
		mov eax, dword ptr [y]
		mov ebx,eax
		shr eax,16 //may be pipelined "write-after-read"
		shr ebx,24			
		jnc EXP_ODD
		and eax,0xFF
		add ebx,64		
		mov ecx,dword ptr [tSqrt+2*eax-0x100]
		shl ebx,23
		or ecx,ebx
		mov [y],ecx
		jmp END			
	EXP_ODD:
		and eax,0xFF
		add ebx,63
		or eax,0x80
		mov ecx,dword ptr [tSqrt+2*eax]		
		shl ebx,23
		or ecx,ebx
		mov [y],ecx
	END:
	}
	return y;
}
#endif
