	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBClearToColour.cpp
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
PUBLIC	?ClearToColour@CDIB@GS@@QAEXK@Z			; GS::CDIB::ClearToColour
_TEXT	SEGMENT
_cr$ = 8
_dp$ = 8
?ClearToColour@CDIB@GS@@QAEXK@Z PROC NEAR		; GS::CDIB::ClearToColour

; 15   : {

	push	ebp
	mov	ebp, esp

; 16   : 	const CLineArray & arrDest = GetLineArray();
; 17   : 	const UINT uSize = arrDest.GetSize();
; 18   : 
; 19   : 	register DIB_PIXEL dp;
; 20   :   dp.rgbBlue = GetBValue( cr );

	mov	eax, DWORD PTR _cr$[ebp]
	push	esi
	mov	esi, DWORD PTR [ecx+72]
	mov	edx, eax
	shr	edx, 16					; 00000010H
	mov	BYTE PTR _dp$[ebp], dl

; 21   :   dp.rgbGreen = GetGValue( cr );

	xor	edx, edx
	mov	dl, ah

; 22   :   dp.rgbRed = GetRValue( cr );

	mov	BYTE PTR _dp$[ebp+2], al

; 23   :   dp.rgbReserved = GetAValue( cr );

	shr	eax, 24					; 00000018H

; 24   : 
; 25   : 	for( register UINT u = 0; u < uSize; u++ )

	test	esi, esi
	mov	BYTE PTR _dp$[ebp+1], dl
	mov	BYTE PTR _dp$[ebp+3], al
	jbe	SHORT $L55619
	mov	edx, DWORD PTR [ecx+60]
	push	ebx

; 26   : 	{
; 27   : 		DIB_PIXEL *pDest = arrDest[ u ];
; 28   : 		for( int c = 0; c < GetWidth(); c++, pDest++ )

	mov	ebx, DWORD PTR [ecx+76]
	push	edi
$L55617:
	mov	edi, DWORD PTR [edx]
	test	ebx, ebx
	jle	SHORT $L55618
	mov	eax, DWORD PTR _dp$[ebp]
	mov	ecx, ebx
	rep stosd
$L55618:
	add	edx, 4
	dec	esi
	jne	SHORT $L55617
	pop	edi
	pop	ebx
$L55619:
	pop	esi

; 29   : 		{
; 30   :       *pDest = dp;
; 31   : 		}
; 32   : 	}
; 33   : }

	pop	ebp
	ret	4
?ClearToColour@CDIB@GS@@QAEXK@Z ENDP			; GS::CDIB::ClearToColour
_TEXT	ENDS
END
