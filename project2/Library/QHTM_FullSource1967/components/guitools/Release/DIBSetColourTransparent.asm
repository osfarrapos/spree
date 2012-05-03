	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBSetColourTransparent.cpp
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
;	COMDAT ?SetHasAlphaChannel@CDIB@GS@@QAEX_N@Z
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
PUBLIC	?SetColourTransparent@CDIB@GS@@QAEXK@Z		; GS::CDIB::SetColourTransparent
_TEXT	SEGMENT
_crTransparent$ = 8
_bGreen$ = 11
?SetColourTransparent@CDIB@GS@@QAEXK@Z PROC NEAR	; GS::CDIB::SetColourTransparent

; 15   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ecx
	push	ebx

; 16   : 	m_bTransparentColorSet = true;
; 17   : 	m_crTransparent = crTransparent;

	mov	ebx, DWORD PTR _crTransparent$[ebp]

; 18   : 	SetHasAlphaChannel( true );
; 19   : 
; 20   : 	BYTE bRed, bGreen, bBlue;
; 21   : 	bRed = GetRValue( crTransparent );
; 22   : 	bGreen = GetGValue( crTransparent );

	xor	eax, eax

; 23   : 	bBlue = GetBValue( crTransparent );

	mov	edx, ebx
	mov	al, bh
	mov	BYTE PTR [ecx+88], 1
	mov	BYTE PTR _bGreen$[ebp], al

; 24   : 
; 25   : 	const CLineArray & arrSrc = GetLineArray();
; 26   : 	for( UINT u = 0; u < arrSrc.GetSize(); u++ )

	mov	eax, DWORD PTR [ecx+72]
	shr	edx, 16					; 00000010H
	test	eax, eax
	mov	DWORD PTR [ecx+84], ebx
	mov	BYTE PTR [ecx+89], 1
	jbe	SHORT $L55619
	push	esi
	mov	esi, DWORD PTR [ecx+60]
	push	edi

; 29   : 		for( int c = 0; c < GetWidth(); c++, pSrc++ )

	mov	edi, DWORD PTR [ecx+76]
	mov	DWORD PTR -8+[ebp], eax
$L55617:
	mov	eax, DWORD PTR [esi]
	test	edi, edi
	jle	SHORT $L55618

; 27   : 	{
; 28   : 		DIB_PIXEL *pSrc = arrSrc[ u ];

	mov	DWORD PTR -4+[ebp], edi
$L55622:

; 30   : 		{
; 31   : 			if( pSrc->rgbRed == bRed && pSrc->rgbGreen == bGreen && pSrc->rgbBlue == bBlue )

	cmp	BYTE PTR [eax+2], bl
	jne	SHORT $L55625
	mov	cl, BYTE PTR _bGreen$[ebp]
	cmp	BYTE PTR [eax+1], cl
	jne	SHORT $L55625
	cmp	BYTE PTR [eax], dl
	jne	SHORT $L55625

; 32   : 			{
; 33   : 				pSrc->rgbReserved = 0;

	and	BYTE PTR [eax+3], 0

; 34   : 			}
; 35   : 			else

	jmp	SHORT $L55623
$L55625:

; 36   : 			{
; 37   : 				pSrc->rgbReserved = 255;

	or	BYTE PTR [eax+3], 255			; 000000ffH
$L55623:

; 29   : 		for( int c = 0; c < GetWidth(); c++, pSrc++ )

	add	eax, 4
	dec	DWORD PTR -4+[ebp]
	jne	SHORT $L55622
$L55618:

; 24   : 
; 25   : 	const CLineArray & arrSrc = GetLineArray();
; 26   : 	for( UINT u = 0; u < arrSrc.GetSize(); u++ )

	add	esi, 4
	dec	DWORD PTR -8+[ebp]
	jne	SHORT $L55617
	pop	edi
	pop	esi
$L55619:
	pop	ebx

; 38   : 			}
; 39   : 		}
; 40   : 	}
; 41   : }

	leave
	ret	4
?SetColourTransparent@CDIB@GS@@QAEXK@Z ENDP		; GS::CDIB::SetColourTransparent
_TEXT	ENDS
END
