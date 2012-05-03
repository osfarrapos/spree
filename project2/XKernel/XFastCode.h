#ifndef _XFAST_CODE_H_
#define _XFAST_CODE_H_
#pragma warning (disable : 4035)

////////////////////////////////////////////////////////////////////////
// Prototypes

extern __inline int						_XFC_FixMul(int a, int b);
extern __inline void					_XFC_MemZero(void *Dest, int Count);
extern __inline void					_XFC_MemCpy(void *Dest, const void *Src, int Count);
extern __inline float __fastcall		_XFC_InverseSqrt(float a);
extern __inline int						_XFC_isqrt0 (unsigned long r);
extern __inline float					_XFC_RSqrt(float number);
extern __forceinline float				_XFC_sqrt(float n);
extern __inline double __fastcall		_XFC_Inv_Sqrt(double x);
extern __forceinline float __cdecl		_XFC_DotProduct(const float v1[3], const float v2[3]);
extern __inline void					_XFC_FastNormVect2(float *v);
extern __inline void					_XFC_FastNormVect3(float *v);
extern __inline DWORD					_XFC_Log2(DWORD val);
extern __inline DWORD					_XFC_NextPowerOfTwo(DWORD N);
extern __forceinline float __fastcall	_XFC_Cos(float a);
extern __forceinline float __fastcall	_XFC_Sin(float a);
extern __forceinline float __fastcall	_XFC_Abs(float a);
extern __forceinline int				_XFC_asmifloor(float f);
extern __forceinline int __stdcall		_XFC_ifloor(float  x);
extern __forceinline BYTE __stdcall		_XFC_FloatToByte (float  x);
extern __forceinline int __stdcall		_XFC_FloatToIntRet(float  x);
extern __forceinline int				_XFC_RoundFloatToInt (float f);
extern __forceinline void				_XFC_FloatToInt(int *int_pointer, float f);
extern void								_XFC_BuildSqrtTable();

#endif // _XFAST_CODE_H_

