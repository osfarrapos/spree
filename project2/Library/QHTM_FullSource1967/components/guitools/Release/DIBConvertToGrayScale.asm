	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBConvertToGrayScale.cpp
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
PUBLIC	?ConvertToGrayScale@GS@@YAXAAVCDIB@1@@Z		; GS::ConvertToGrayScale
_TEXT	SEGMENT
_dib$ = 8
?ConvertToGrayScale@GS@@YAXAAVCDIB@1@@Z PROC NEAR	; GS::ConvertToGrayScale

; 14   : {

	push	ebp
	mov	ebp, esp
	push	ecx

; 15   : 	const GS::CDIB::CLineArray & arrDest = dib.GetLineArray();

	mov	eax, DWORD PTR _dib$[ebp]

; 16   : 	const UINT uSize = arrDest.GetSize();

	mov	ecx, DWORD PTR [eax+72]

; 17   : 
; 18   : 	for( register UINT u = 0; u < uSize; u++ )

	test	ecx, ecx
	jbe	SHORT $L55612
	push	ebx
	push	esi
	mov	esi, DWORD PTR [eax+60]
	push	edi

; 21   : 		for( int c = 0; c < dib.GetWidth(); c++, pDest++ )

	mov	edi, DWORD PTR [eax+76]
	mov	DWORD PTR -4+[ebp], ecx
$L55610:
	mov	ecx, DWORD PTR [esi]
	test	edi, edi
	jle	SHORT $L55611

; 19   : 	{
; 20   : 		DIB_PIXEL *pDest = arrDest[ u ];

	mov	DWORD PTR 8+[ebp], edi
$L55615:

; 22   : 		{
; 23   : 			//const BYTE bNew = pDest->rgbRed * 0.299 + pDest->rgbGreen * 0.258 + pDest->rgbBlue * 0.114;
; 24   : 			//const BYTE bNew = (pDest->rgbRed * 3 + pDest->rgbGreen * 6 + pDest->rgbBlue) / 10;
; 25   : 			const BYTE bNew = (BYTE)( (pDest->rgbRed + pDest->rgbGreen + pDest->rgbBlue) / 3 );

	movzx	eax, BYTE PTR [ecx]
	movzx	edx, BYTE PTR [ecx+2]
	add	eax, edx
	push	3
	movzx	edx, BYTE PTR [ecx+1]
	add	eax, edx
	pop	ebx
	cdq
	idiv	ebx

; 26   : 			pDest->rgbRed = bNew;

	mov	BYTE PTR [ecx+2], al

; 27   : 			pDest->rgbGreen = bNew;

	mov	BYTE PTR [ecx+1], al

; 28   : 			pDest->rgbBlue = bNew;

	mov	BYTE PTR [ecx], al
	add	ecx, 4
	dec	DWORD PTR 8+[ebp]
	jne	SHORT $L55615
$L55611:

; 17   : 
; 18   : 	for( register UINT u = 0; u < uSize; u++ )

	add	esi, 4
	dec	DWORD PTR -4+[ebp]
	jne	SHORT $L55610
	pop	edi
	pop	esi
	pop	ebx
$L55612:

; 29   : 		}
; 30   : 	}
; 31   : 	
; 32   : }

	leave
	ret	0
?ConvertToGrayScale@GS@@YAXAAVCDIB@1@@Z ENDP		; GS::ConvertToGrayScale
_TEXT	ENDS
END
