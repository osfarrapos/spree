	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBDrawWithAlphaChannel.cpp
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
PUBLIC	?DrawWithAlphaChannel@CDIB@GS@@QAE_NPAUHDC__@@HH@Z ; GS::CDIB::DrawWithAlphaChannel
EXTRN	??0CDIB@GS@@QAE@PAUHDC__@@HHHH@Z:NEAR		; GS::CDIB::CDIB
EXTRN	?BasicDraw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z:NEAR	; GS::CDIB::BasicDraw
EXTRN	??1CDIB@GS@@UAE@XZ:NEAR				; GS::CDIB::~CDIB
_TEXT	SEGMENT
_hdc$ = 8
_nX$ = 12
_nY$ = 16
_dib$ = -108
_u$ = -8
?DrawWithAlphaChannel@CDIB@GS@@QAE_NPAUHDC__@@HH@Z PROC NEAR ; GS::CDIB::DrawWithAlphaChannel

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
; 21   : 	const CLineArray & arrScreen = dib.GetLineArray();
; 22   : 	const CLineArray & arrSrc = GetLineArray();
; 23   : 
; 24   : 	unsigned char nNegativeAplha;
; 25   : 	for( UINT u = 0; u < arrScreen.GetSize(); u++ )

	xor	eax, eax
	cmp	DWORD PTR _dib$[ebp+72], eax
	mov	DWORD PTR _u$[ebp], eax
	jbe	$L55617

; 28   : 		DIB_PIXEL *pSrc = arrSrc[ u ];

	mov	ecx, DWORD PTR [esi+60]
	push	edi
	mov	DWORD PTR -12+[ebp], ecx

; 29   : 		for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )

	mov	ecx, DWORD PTR [esi+76]
	mov	DWORD PTR -16+[ebp], ecx
$L55615:
	mov	esi, DWORD PTR -12+[ebp]
	mov	edx, eax
	mov	eax, DWORD PTR _dib$[ebp+60]
	shl	edx, 2
	test	ecx, ecx
	mov	eax, DWORD PTR [edx+eax]
	mov	esi, DWORD PTR [esi+edx]
	jle	SHORT $L55616

; 26   : 	{
; 27   : 		DIB_PIXEL *pDest = arrScreen[ u ];

	mov	DWORD PTR -4+[ebp], ecx
$L55621:

; 30   : 		{
; 31   : 			const int nAlpha = pSrc->rgbReserved;

	movzx	edx, BYTE PTR [esi+3]

; 32   :       nNegativeAplha = static_cast<unsigned char>( 255 - nAlpha );
; 33   : 
; 34   :       ALPHA( pSrc, pDest, rgbRed, nAlpha, nNegativeAplha );

	movzx	ebx, BYTE PTR [eax+2]
	or	cl, 255					; 000000ffH
	sub	cl, dl
	movzx	edi, cl
	movzx	ecx, BYTE PTR [esi+2]
	imul	ebx, edi
	imul	ecx, edx
	add	ecx, ebx

; 35   :       ALPHA( pSrc, pDest, rgbGreen, nAlpha, nNegativeAplha );

	movzx	ebx, BYTE PTR [eax+1]
	sar	ecx, 8
	imul	ebx, edi
	mov	BYTE PTR [eax+2], cl
	movzx	ecx, BYTE PTR [esi+1]
	imul	ecx, edx
	add	ecx, ebx
	sar	ecx, 8
	mov	BYTE PTR [eax+1], cl

; 36   :       ALPHA( pSrc, pDest, rgbBlue, nAlpha, nNegativeAplha );

	movzx	ecx, BYTE PTR [eax]
	imul	ecx, edi
	movzx	edi, BYTE PTR [esi]
	imul	edi, edx
	add	ecx, edi
	add	esi, 4
	sar	ecx, 8
	mov	BYTE PTR [eax], cl
	add	eax, 4
	dec	DWORD PTR -4+[ebp]
	jne	SHORT $L55621

; 29   : 		for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )

	mov	ecx, DWORD PTR -16+[ebp]
$L55616:

; 18   : 
; 19   : 	//
; 20   : 	//	Now copy our stuff into the copy of the current screen...
; 21   : 	const CLineArray & arrScreen = dib.GetLineArray();
; 22   : 	const CLineArray & arrSrc = GetLineArray();
; 23   : 
; 24   : 	unsigned char nNegativeAplha;
; 25   : 	for( UINT u = 0; u < arrScreen.GetSize(); u++ )

	mov	eax, DWORD PTR _u$[ebp]
	inc	eax
	cmp	eax, DWORD PTR _dib$[ebp+72]
	mov	DWORD PTR _u$[ebp], eax
	jb	SHORT $L55615
	pop	edi
$L55617:

; 37   : 		}
; 38   : 	}
; 39   : 	return dib.BasicDraw( hdc, nX, nY );

	push	DWORD PTR _nY$[ebp]
	lea	ecx, DWORD PTR _dib$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?BasicDraw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z	; GS::CDIB::BasicDraw
	lea	ecx, DWORD PTR _dib$[ebp]
	mov	bl, al
	call	??1CDIB@GS@@UAE@XZ			; GS::CDIB::~CDIB
	mov	al, bl
	pop	esi
	pop	ebx

; 40   : }

	leave
	ret	12					; 0000000cH
?DrawWithAlphaChannel@CDIB@GS@@QAE_NPAUHDC__@@HH@Z ENDP	; GS::CDIB::DrawWithAlphaChannel
_TEXT	ENDS
END
