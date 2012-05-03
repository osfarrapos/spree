	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBDrawBitmapOntoUs.cpp
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
PUBLIC	?DrawBitmapOntoUs@CDIB@GS@@QAEXPAUHBITMAP__@@HHHHHH@Z ; GS::CDIB::DrawBitmapOntoUs
EXTRN	__imp__SelectObject@8:NEAR
EXTRN	__imp__GetDC@4:NEAR
EXTRN	__imp__ReleaseDC@8:NEAR
EXTRN	__imp__BitBlt@36:NEAR
EXTRN	__imp__CreateCompatibleDC@4:NEAR
EXTRN	__imp__DeleteDC@4:NEAR
EXTRN	__imp__GetObjectA@12:NEAR
_TEXT	SEGMENT
_hBitmap$ = 8
_x$ = 12
_y$ = 16
_nSourceX$ = 20
_nSourceY$ = 24
_cx$ = 28
_cy$ = 32
_this$ = -8
_hdc$ = -4
_bm$55640 = -32
_hOld$55643 = -8
_hOld2$55644 = 8
?DrawBitmapOntoUs@CDIB@GS@@QAEXPAUHBITMAP__@@HHHHHH@Z PROC NEAR ; GS::CDIB::DrawBitmapOntoUs

; 16   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR _this$[ebp], ecx

; 17   : 	//
; 18   : 	//	Copy the DC into the DIB.
; 19   : 	HDC hdc = GetDC( NULL );

	push	0
	call	DWORD PTR __imp__GetDC@4
	mov	DWORD PTR _hdc$[ebp], eax

; 20   : 	{
; 21   : 		BITMAP bm;
; 22   : 		GetObject( hBitmap, sizeof( bm ), &bm );

	lea	eax, DWORD PTR _bm$55640[ebp]
	push	eax
	push	24					; 00000018H
	push	DWORD PTR _hBitmap$[ebp]
	call	DWORD PTR __imp__GetObjectA@12

; 23   : 
; 24   : 		CCompatibleDC hdcMemDest( hdc );

	push	DWORD PTR _hdc$[ebp]
	mov	esi, DWORD PTR __imp__CreateCompatibleDC@4
	call	esi

; 25   : 		CCompatibleDC hdcMemSource( hdc );

	push	DWORD PTR _hdc$[ebp]
	mov	edi, eax
	call	esi

; 26   : 
; 27   : 		HGDIOBJ hOld = SelectObject( hdcMemDest, m_hBitmap );

	mov	esi, DWORD PTR __imp__SelectObject@8
	mov	ebx, eax
	mov	eax, DWORD PTR _this$[ebp]
	push	DWORD PTR [eax+4]
	push	edi
	call	esi

; 28   : 		HGDIOBJ hOld2 = SelectObject( hdcMemSource, hBitmap );

	push	DWORD PTR _hBitmap$[ebp]
	mov	DWORD PTR _hOld$55643[ebp], eax
	push	ebx
	call	esi

; 29   : 		BitBlt( hdcMemDest, x, y, cx, cy, hdcMemSource, nSourceX, nSourceY, SRCCOPY );

	push	13369376				; 00cc0020H
	mov	DWORD PTR _hOld2$55644[ebp], eax
	push	DWORD PTR _nSourceY$[ebp]
	push	DWORD PTR _nSourceX$[ebp]
	push	ebx
	push	DWORD PTR _cy$[ebp]
	push	DWORD PTR _cx$[ebp]
	push	DWORD PTR _y$[ebp]
	push	DWORD PTR _x$[ebp]
	push	edi
	call	DWORD PTR __imp__BitBlt@36

; 30   : 		SelectObject( hdcMemDest, hOld );

	push	DWORD PTR _hOld$55643[ebp]
	push	edi
	call	esi

; 31   : 		SelectObject( hdcMemSource, hOld2 );

	push	DWORD PTR _hOld2$55644[ebp]
	push	ebx
	call	esi

; 32   : 	}

	mov	esi, DWORD PTR __imp__DeleteDC@4
	push	ebx
	call	esi
	push	edi
	call	esi

; 33   : 	ReleaseDC( NULL, hdc );

	push	DWORD PTR _hdc$[ebp]
	push	0
	call	DWORD PTR __imp__ReleaseDC@8
	pop	edi
	pop	esi
	pop	ebx

; 34   : }

	leave
	ret	28					; 0000001cH
?DrawBitmapOntoUs@CDIB@GS@@QAEXPAUHBITMAP__@@HHHHHH@Z ENDP ; GS::CDIB::DrawBitmapOntoUs
_TEXT	ENDS
END
