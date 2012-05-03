	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBDrawTransparent.cpp
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
;	COMDAT ?GetHeight@CDIB@GS@@QBEHXZ
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
PUBLIC	?DrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHK@Z	; GS::CDIB::DrawTransparent
EXTRN	??0CDIB@GS@@QAE@PAUHDC__@@HHHH@Z:NEAR		; GS::CDIB::CDIB
EXTRN	?Draw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z:NEAR		; GS::CDIB::Draw
EXTRN	??1CDIB@GS@@UAE@XZ:NEAR				; GS::CDIB::~CDIB
_TEXT	SEGMENT
_hdc$ = 8
_nX$ = 12
_nY$ = 16
_crTransparent$ = 20
_dib$ = -108
_bGreen$ = -1
_u$ = -8
?DrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHK@Z PROC NEAR ; GS::CDIB::DrawTransparent

; 14   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 108				; 0000006cH
	push	ebx
	push	esi
	mov	esi, ecx

; 15   : 	//
; 16   : 	//	Grab a copy of what's there already...
; 17   : 	CDIB dib( hdc, nX, nY, GetWidth(), GetHeight() );

	lea	ecx, DWORD PTR _dib$[ebp]
	push	DWORD PTR [esi+80]
	push	DWORD PTR [esi+76]
	push	DWORD PTR _nY$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	??0CDIB@GS@@QAE@PAUHDC__@@HHHH@Z	; GS::CDIB::CDIB

; 18   : 
; 19   : 	//
; 20   : 	//	Now copy our stuff into the copy of the current screen...
; 21   : 
; 22   : 	const CLineArray & arrScreen = dib.GetLineArray();
; 23   : 	const CLineArray & arrSrc = GetLineArray();
; 24   : 
; 25   : 	BYTE bRed, bGreen, bBlue;
; 26   : 	bRed = GetRValue( crTransparent );
; 27   : 	bGreen = GetGValue( crTransparent );

	mov	ecx, DWORD PTR _crTransparent$[ebp]

; 28   : 	bBlue = GetBValue( crTransparent );
; 29   : 
; 30   : 	for( UINT u = 0; u < arrScreen.GetSize(); u++ )

	and	DWORD PTR _u$[ebp], 0
	xor	eax, eax
	mov	al, ch
	shr	ecx, 16					; 00000010H
	cmp	DWORD PTR _dib$[ebp+72], 0
	mov	BYTE PTR _bGreen$[ebp], al
	jbe	SHORT $L55624

; 33   : 		DIB_PIXEL *pSrc = arrSrc[ u ];

	mov	eax, DWORD PTR [esi+60]

; 34   : 		for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )

	mov	esi, DWORD PTR [esi+76]
	push	edi
	mov	DWORD PTR -16+[ebp], eax
$L55622:
	mov	eax, DWORD PTR _u$[ebp]
	mov	edx, DWORD PTR _dib$[ebp+60]
	shl	eax, 2
	test	esi, esi
	mov	edi, DWORD PTR [eax+edx]
	mov	edx, DWORD PTR -16+[ebp]
	mov	eax, DWORD PTR [edx+eax]
	jle	SHORT $L55623

; 31   : 	{
; 32   : 		DIB_PIXEL *pDest = arrScreen[ u ];

	mov	DWORD PTR -12+[ebp], esi
$L55628:

; 35   : 		{
; 36   : 			if( pSrc->rgbRed != bRed || pSrc->rgbGreen != bGreen || pSrc->rgbBlue != bBlue )

	mov	dl, BYTE PTR [eax+2]
	cmp	dl, BYTE PTR _crTransparent$[ebp]
	jne	SHORT $L55632
	mov	bl, BYTE PTR _bGreen$[ebp]
	cmp	BYTE PTR [eax+1], bl
	jne	SHORT $L55632
	cmp	BYTE PTR [eax], cl
	je	SHORT $L55629
$L55632:

; 37   : 			{
; 38   : 				pDest->rgbRed = pSrc->rgbRed;

	mov	BYTE PTR [edi+2], dl

; 39   : 				pDest->rgbGreen = pSrc->rgbGreen;

	mov	dl, BYTE PTR [eax+1]
	mov	BYTE PTR [edi+1], dl

; 40   : 				pDest->rgbBlue = pSrc->rgbBlue;

	mov	dl, BYTE PTR [eax]
	mov	BYTE PTR [edi], dl
$L55629:
	add	edi, 4
	add	eax, 4
	dec	DWORD PTR -12+[ebp]
	jne	SHORT $L55628
$L55623:

; 28   : 	bBlue = GetBValue( crTransparent );
; 29   : 
; 30   : 	for( UINT u = 0; u < arrScreen.GetSize(); u++ )

	inc	DWORD PTR _u$[ebp]
	mov	eax, DWORD PTR _u$[ebp]
	cmp	eax, DWORD PTR _dib$[ebp+72]
	jb	SHORT $L55622
	pop	edi
$L55624:

; 41   : 			}
; 42   : 		}
; 43   : 	}
; 44   : 	return dib.Draw( hdc, nX, nY );

	push	DWORD PTR _nY$[ebp]
	lea	ecx, DWORD PTR _dib$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?Draw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z	; GS::CDIB::Draw
	lea	ecx, DWORD PTR _dib$[ebp]
	mov	bl, al
	call	??1CDIB@GS@@UAE@XZ			; GS::CDIB::~CDIB
	mov	al, bl
	pop	esi
	pop	ebx

; 45   : }

	leave
	ret	16					; 00000010H
?DrawTransparent@CDIB@GS@@QAE_NPAUHDC__@@HHK@Z ENDP	; GS::CDIB::DrawTransparent
_TEXT	ENDS
END
