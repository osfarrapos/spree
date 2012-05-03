	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBAlphaBlend.cpp
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
PUBLIC	?AlphaBlend@CDIB@GS@@QAEXPAUHDC__@@HHE@Z	; GS::CDIB::AlphaBlend
EXTRN	??0CDIB@GS@@QAE@PAUHDC__@@HHHH@Z:NEAR		; GS::CDIB::CDIB
EXTRN	?Draw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z:NEAR		; GS::CDIB::Draw
EXTRN	??1CDIB@GS@@UAE@XZ:NEAR				; GS::CDIB::~CDIB
_TEXT	SEGMENT
_hdc$ = 8
_nX$ = 12
_nY$ = 16
_nAlpha$ = 20
_dib$ = -116
_uLines$55615 = -24
_u$55616 = -20
_nNegativeAplha$55635 = -1
_uLines$55637 = -24
_u$55638 = -16
?AlphaBlend@CDIB@GS@@QAEXPAUHDC__@@HHE@Z PROC NEAR	; GS::CDIB::AlphaBlend

; 14   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 116				; 00000074H
	push	ebx
	push	esi
	mov	esi, ecx
	push	edi

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
; 24   : 	if( m_bHasAlpha )

	cmp	BYTE PTR [esi+89], 0
	je	$L55614

; 25   : 	{
; 26   : 		const UINT uLines = arrScreen.GetSize();

	mov	eax, DWORD PTR _dib$[ebp+72]

; 27   : 		for( UINT u = 0; u < uLines; u++ )

	xor	ecx, ecx
	test	eax, eax
	mov	DWORD PTR _uLines$55615[ebp], eax
	mov	DWORD PTR _u$55616[ebp], ecx
	jbe	$L55641

; 30   : 			DIB_PIXEL *pSrc = arrSrc[ u ];

	mov	eax, DWORD PTR [esi+60]
	mov	DWORD PTR -12+[ebp], eax

; 31   : 			for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )

	mov	eax, DWORD PTR [esi+76]
	mov	DWORD PTR -8+[ebp], eax
$L55617:
	mov	esi, DWORD PTR -12+[ebp]
	mov	edx, ecx
	mov	ecx, DWORD PTR _dib$[ebp+60]
	shl	edx, 2
	test	eax, eax
	mov	ecx, DWORD PTR [edx+ecx]
	mov	esi, DWORD PTR [esi+edx]
	jle	SHORT $L55618

; 28   : 		{
; 29   : 			DIB_PIXEL *pDest = arrScreen[ u ];

	mov	DWORD PTR -16+[ebp], eax
$L55623:

; 32   : 			{
; 33   : 				if( pSrc->rgbReserved )

	mov	al, BYTE PTR [esi+3]
	test	al, al
	je	SHORT $L55624

; 34   : 				{
; 35   : 					unsigned char nPixelAlpha = static_cast<unsigned char>( pSrc->rgbReserved * 256 / 255 * nAlpha / 256 );

	movzx	eax, al
	shl	eax, 8
	cdq
	mov	edi, 255				; 000000ffH
	idiv	edi
	movzx	edx, BYTE PTR _nAlpha$[ebp]

; 36   : 					unsigned char nNegativeAplha = static_cast<unsigned char>( 255 - nPixelAlpha );
; 37   : 					
; 38   : 					ALPHA( pSrc, pDest, rgbRed, nPixelAlpha, nNegativeAplha );

	movzx	ebx, BYTE PTR [ecx+2]
	imul	eax, edx
	cdq
	inc	edi
	idiv	edi
	or	dl, 255					; 000000ffH
	sub	dl, al
	movzx	edi, al
	movzx	eax, BYTE PTR [esi+2]
	movzx	edx, dl
	imul	eax, edi
	imul	ebx, edx
	add	eax, ebx

; 39   : 					ALPHA( pSrc, pDest, rgbGreen, nPixelAlpha, nNegativeAplha );

	movzx	ebx, BYTE PTR [ecx+1]
	sar	eax, 8
	imul	ebx, edx
	mov	BYTE PTR [ecx+2], al
	movzx	eax, BYTE PTR [esi+1]
	imul	eax, edi
	add	eax, ebx
	sar	eax, 8
	mov	BYTE PTR [ecx+1], al

; 40   : 					ALPHA( pSrc, pDest, rgbBlue, nPixelAlpha, nNegativeAplha );

	movzx	eax, BYTE PTR [esi]
	imul	eax, edi
	movzx	edi, BYTE PTR [ecx]
	imul	edi, edx
	add	eax, edi
	sar	eax, 8
	mov	BYTE PTR [ecx], al
$L55624:
	add	ecx, 4
	add	esi, 4
	dec	DWORD PTR -16+[ebp]
	jne	SHORT $L55623

; 31   : 			for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )

	mov	eax, DWORD PTR -8+[ebp]
$L55618:

; 27   : 		for( UINT u = 0; u < uLines; u++ )

	mov	ecx, DWORD PTR _u$55616[ebp]
	inc	ecx
	cmp	ecx, DWORD PTR _uLines$55615[ebp]
	mov	DWORD PTR _u$55616[ebp], ecx
	jb	$L55617

; 41   : 				}
; 42   : 			}
; 43   : 		}
; 44   : 	}
; 45   : 	else

	jmp	$L55641
$L55614:

; 46   : 	{
; 47   : 		unsigned char nNegativeAplha = static_cast<unsigned char>( 255 - nAlpha );
; 48   : 
; 49   : 		const UINT uLines = arrScreen.GetSize();

	mov	ecx, DWORD PTR _dib$[ebp+72]
	or	dl, 255					; 000000ffH
	sub	dl, BYTE PTR _nAlpha$[ebp]

; 50   : 		for( UINT u = 0; u < uLines; u++ )

	xor	eax, eax
	test	ecx, ecx
	mov	BYTE PTR _nNegativeAplha$55635[ebp], dl
	mov	DWORD PTR _uLines$55637[ebp], ecx
	mov	DWORD PTR _u$55638[ebp], eax
	jbe	$L55641

; 51   : 		{
; 52   : 			DIB_PIXEL *pDest = arrScreen[ u ];
; 53   : 			DIB_PIXEL *pSrc = arrSrc[ u ];

	mov	ecx, DWORD PTR [esi+60]
	mov	DWORD PTR -12+[ebp], ecx

; 54   : 			for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )

	mov	ecx, DWORD PTR [esi+76]
	mov	DWORD PTR -8+[ebp], ecx
$L55639:
	mov	esi, DWORD PTR -12+[ebp]
	mov	ecx, eax
	mov	eax, DWORD PTR _dib$[ebp+60]
	shl	ecx, 2
	cmp	DWORD PTR -8+[ebp], 0
	mov	eax, DWORD PTR [ecx+eax]
	mov	ecx, DWORD PTR [ecx+esi]
	jle	SHORT $L55640

; 55   : 			{
; 56   : 				ALPHA( pSrc, pDest, rgbRed, nAlpha, nNegativeAplha );
; 57   : 				ALPHA( pSrc, pDest, rgbGreen, nAlpha, nNegativeAplha );
; 58   : 				ALPHA( pSrc, pDest, rgbBlue, nAlpha, nNegativeAplha );

	movzx	edi, BYTE PTR _nAlpha$[ebp]
	movzx	esi, dl
	mov	edx, DWORD PTR -8+[ebp]
	mov	DWORD PTR -20+[ebp], edx
$L55645:
	movzx	edx, BYTE PTR [ecx+2]
	movzx	ebx, BYTE PTR [eax+2]
	imul	edx, edi
	imul	ebx, esi
	add	edx, ebx
	movzx	ebx, BYTE PTR [eax+1]
	sar	edx, 8
	imul	ebx, esi
	mov	BYTE PTR [eax+2], dl
	movzx	edx, BYTE PTR [ecx+1]
	imul	edx, edi
	add	edx, ebx
	movzx	ebx, BYTE PTR [eax]
	sar	edx, 8
	imul	ebx, esi
	mov	BYTE PTR [eax+1], dl
	movzx	edx, BYTE PTR [ecx]
	imul	edx, edi
	add	edx, ebx
	add	ecx, 4
	sar	edx, 8
	mov	BYTE PTR [eax], dl
	add	eax, 4
	dec	DWORD PTR -20+[ebp]
	jne	SHORT $L55645

; 54   : 			for( int c = 0; c < GetWidth(); c++, pDest++, pSrc++ )

	mov	dl, BYTE PTR _nNegativeAplha$55635[ebp]
$L55640:

; 50   : 		for( UINT u = 0; u < uLines; u++ )

	mov	eax, DWORD PTR _u$55638[ebp]
	inc	eax
	cmp	eax, DWORD PTR _uLines$55637[ebp]
	mov	DWORD PTR _u$55638[ebp], eax
	jb	SHORT $L55639
$L55641:

; 59   : 			}
; 60   : 		}
; 61   : 	}
; 62   : 	dib.Draw( hdc, nX, nY );

	push	DWORD PTR _nY$[ebp]
	lea	ecx, DWORD PTR _dib$[ebp]
	push	DWORD PTR _nX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	?Draw@CDIB@GS@@QAE_NPAUHDC__@@HH@Z	; GS::CDIB::Draw

; 63   : }

	lea	ecx, DWORD PTR _dib$[ebp]
	call	??1CDIB@GS@@UAE@XZ			; GS::CDIB::~CDIB
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	16					; 00000010H
?AlphaBlend@CDIB@GS@@QAEXPAUHDC__@@HHE@Z ENDP		; GS::CDIB::AlphaBlend
_TEXT	ENDS
END
