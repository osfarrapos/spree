	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBCreateCopy.cpp
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
;	COMDAT ?GetBitmap@CDIB@GS@@QAEPAUHBITMAP__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetHasAlphaChannel@CDIB@GS@@QAEX_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CCompatibleDC@GS@@QAE@PAUHDC__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CCompatibleDC@GS@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??BCCompatibleDC@GS@@QAEPAUHDC__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAUtagRGBQUAD@@@Container@@UAE@XZ
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
PUBLIC	?CreateCopy@CDIB@GS@@QBEPAV12@HH@Z		; GS::CDIB::CreateCopy
EXTRN	??0CDIB@GS@@QAE@PAUHDC__@@HH@Z:NEAR		; GS::CDIB::CDIB
EXTRN	__imp__GdiFlush@0:NEAR
EXTRN	?SetColourTransparent@CDIB@GS@@QAEXK@Z:NEAR	; GS::CDIB::SetColourTransparent
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
EXTRN	__imp__SelectObject@8:NEAR
EXTRN	__imp__StretchDIBits@52:NEAR
EXTRN	__imp__GetDC@4:NEAR
EXTRN	__imp__ReleaseDC@8:NEAR
EXTRN	__imp__CreateCompatibleDC@4:NEAR
EXTRN	__imp__DeleteDC@4:NEAR
_TEXT	SEGMENT
_nCX$ = 8
_nCY$ = 12
_hdc$ = -12
_pdib$ = -4
_hdcMem$ = -8
_hOld$ = -16
?CreateCopy@CDIB@GS@@QBEPAV12@HH@Z PROC NEAR		; GS::CDIB::CreateCopy

; 17   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	ebx

; 18   : 	HDC hdc = GetDC( NULL );

	xor	ebx, ebx
	push	esi
	mov	esi, ecx
	push	ebx
	call	DWORD PTR __imp__GetDC@4

; 19   : 
; 20   : 	CDIB *pdib = new CDIB( hdc, nCX, nCY );

	push	92					; 0000005cH
	mov	DWORD PTR _hdc$[ebp], eax
	call	??2@YAPAXI@Z				; operator new
	cmp	eax, ebx
	pop	ecx
	je	SHORT $L55709
	push	DWORD PTR _nCY$[ebp]
	mov	ecx, eax
	push	DWORD PTR _nCX$[ebp]
	push	DWORD PTR _hdc$[ebp]
	call	??0CDIB@GS@@QAE@PAUHDC__@@HH@Z		; GS::CDIB::CDIB
	mov	DWORD PTR _pdib$[ebp], eax
	jmp	SHORT $L55710
$L55709:
	mov	DWORD PTR _pdib$[ebp], ebx
$L55710:
	push	edi

; 21   : 	CCompatibleDC hdcMem( hdc );

	push	DWORD PTR _hdc$[ebp]
	call	DWORD PTR __imp__CreateCompatibleDC@4
	mov	DWORD PTR _hdcMem$[ebp], eax

; 22   : 
; 23   : 	HGDIOBJ hOld = SelectObject( hdcMem, pdib->GetBitmap() );

	call	DWORD PTR __imp__GdiFlush@0
	mov	eax, DWORD PTR _pdib$[ebp]
	mov	edi, DWORD PTR __imp__SelectObject@8
	push	DWORD PTR [eax+4]
	push	DWORD PTR _hdcMem$[ebp]
	call	edi
	mov	DWORD PTR _hOld$[ebp], eax

; 24   : 
; 25   : 	::StretchDIBits( hdcMem,
; 26   :               0,
; 27   :               0,
; 28   :               nCX,
; 29   :               nCY,
; 30   :               0,
; 31   :               0,
; 32   :               GetWidth(),
; 33   :               GetHeight(),
; 34   :               m_pBits,
; 35   :               (BITMAPINFO *) &m_bi,
; 36   :               DIB_RGB_COLORS,
; 37   :               SRCCOPY );

	push	13369376				; 00cc0020H
	lea	eax, DWORD PTR [esi+12]
	push	ebx
	push	eax
	push	DWORD PTR [esi+8]
	push	DWORD PTR [esi+80]
	push	DWORD PTR [esi+76]
	push	ebx
	push	ebx
	push	DWORD PTR _nCY$[ebp]
	push	DWORD PTR _nCX$[ebp]
	push	ebx
	push	ebx
	push	DWORD PTR _hdcMem$[ebp]
	call	DWORD PTR __imp__StretchDIBits@52

; 38   : 
; 39   : 	SelectObject( hdcMem, hOld );

	push	DWORD PTR _hOld$[ebp]
	push	DWORD PTR _hdcMem$[ebp]
	call	edi

; 40   : 
; 41   : 	ReleaseDC( NULL, hdc );

	push	DWORD PTR _hdc$[ebp]
	push	ebx
	call	DWORD PTR __imp__ReleaseDC@8

; 42   : 	if( m_bTransparentColorSet )

	cmp	BYTE PTR [esi+88], bl
	pop	edi
	je	SHORT $L55642

; 43   : 	{
; 44   : 		pdib->SetColourTransparent( m_crTransparent );

	push	DWORD PTR [esi+84]
	mov	ecx, DWORD PTR _pdib$[ebp]
	call	?SetColourTransparent@CDIB@GS@@QAEXK@Z	; GS::CDIB::SetColourTransparent
$L55642:

; 45   : 	}
; 46   : 
; 47   : 	if( m_bHasAlpha )

	mov	al, BYTE PTR [esi+89]
	pop	esi
	cmp	al, bl
	pop	ebx
	je	SHORT $L55737

; 48   : 	{
; 49   : 		pdib->SetHasAlphaChannel( m_bHasAlpha );

	mov	ecx, DWORD PTR _pdib$[ebp]
	mov	BYTE PTR [ecx+89], al
$L55737:

; 50   : 	}
; 51   : 
; 52   : 	return pdib;

	push	DWORD PTR _hdcMem$[ebp]
	call	DWORD PTR __imp__DeleteDC@4
	mov	eax, DWORD PTR _pdib$[ebp]

; 53   : }

	leave
	ret	8
?CreateCopy@CDIB@GS@@QBEPAV12@HH@Z ENDP			; GS::CDIB::CreateCopy
_TEXT	ENDS
END
