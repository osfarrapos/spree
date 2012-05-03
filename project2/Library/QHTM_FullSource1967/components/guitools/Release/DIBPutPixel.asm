	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBPutPixel.cpp
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
;	COMDAT ??0CSize@WinHelper@@QAE@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CSize@WinHelper@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Width@CRect@WinHelper@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Height@CRect@WinHelper@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Set@CRect@WinHelper@@QAEXHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lock@CCriticalSection@WinHelper@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Unlock@CCriticalSection@WinHelper@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetWidth@CDIB@GS@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLineArray@CDIB@GS@@QAEABV?$CArray@PAUtagRGBQUAD@@@Container@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAUtagRGBQUAD@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@PAUtagRGBQUAD@@@Container@@QBEABQAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?PutPixel@CDIB@GS@@QAEXIIEEEE@Z			; GS::CDIB::PutPixel
_TEXT	SEGMENT
_x$ = 8
_y$ = 12
_r$ = 16
_g$ = 20
_b$ = 24
_a$ = 28
?PutPixel@CDIB@GS@@QAEXIIEEEE@Z PROC NEAR		; GS::CDIB::PutPixel

; 15   : {

	push	ebp
	mov	ebp, esp

; 16   : 	const CLineArray & arrDest = GetLineArray();
; 17   : 	if( y < arrDest.GetSize() && y >= 0 )

	mov	edx, DWORD PTR _y$[ebp]
	cmp	edx, DWORD PTR [ecx+72]
	jae	SHORT $L55617

; 18   : 	{
; 19   : 		DIB_PIXEL *pDest = arrDest[ y ];
; 20   : 		if( x >= 0 && x < (UINT)GetWidth() )

	mov	eax, DWORD PTR _x$[ebp]
	cmp	eax, DWORD PTR [ecx+76]
	jae	SHORT $L55617

; 21   : 		{
; 22   : 			DIB_PIXEL *pPixel = pDest + x;

	mov	ecx, DWORD PTR [ecx+60]
	mov	ecx, DWORD PTR [ecx+edx*4]
	lea	eax, DWORD PTR [ecx+eax*4]

; 23   : 			pPixel->rgbRed = r;

	mov	cl, BYTE PTR _r$[ebp]
	mov	BYTE PTR [eax+2], cl

; 24   :       pPixel->rgbGreen = g;

	mov	cl, BYTE PTR _g$[ebp]
	mov	BYTE PTR [eax+1], cl

; 25   :       pPixel->rgbBlue = b;

	mov	cl, BYTE PTR _b$[ebp]
	mov	BYTE PTR [eax], cl

; 26   : 			pPixel->rgbReserved = a;

	mov	cl, BYTE PTR _a$[ebp]
	mov	BYTE PTR [eax+3], cl
$L55617:

; 27   : 		}
; 28   : 	}
; 29   : }

	pop	ebp
	ret	24					; 00000018H
?PutPixel@CDIB@GS@@QAEXIIEEEE@Z ENDP			; GS::CDIB::PutPixel
_TEXT	ENDS
END
