	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\CreateRoundCornerRegion.cpp
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
;	COMDAT ??0FontDef@GS@@QAE@PBDHH_N11E1@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MulDiv@WinHelper@@YAHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
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
;	COMDAT ?Size@CRect@WinHelper@@QBE?AVCSize@2@XZ
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
;	COMDAT ?GetBitmap@CDIB@GS@@QAEPAUHBITMAP__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CBufferedDCDIB@GS@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCBufferedDCDIB@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CBufferedDC@GS@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCBufferedDC@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSafeHdc@CDrawContext@GS@@QAEPAUHDC__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectPen@CDrawContext@GS@@AAEXK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectBrush@CDrawContext@GS@@AAEXK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetClipRect@CDrawContext@GS@@QBEABVCRect@WinHelper@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetClipRegion@CDrawContext@GS@@QAEXPAUHRGN__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectFont@CDrawContext@GS@@QAEXABUFontDef@2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScaleX@CDrawContext@GS@@QBEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScaleY@CDrawContext@GS@@QBEHH@Z
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
;	COMDAT ??_7CBufferedDC@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CBufferedDCDIB@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?CreateRoundCornerRegion@GS@@YAPAUHRGN__@@ABVCRect@WinHelper@@II@Z ; GS::CreateRoundCornerRegion
EXTRN	__imp__CombineRgn@16:NEAR
EXTRN	__imp__CreateRectRgn@16:NEAR
EXTRN	__imp__CreateRoundRectRgn@24:NEAR
EXTRN	__imp__DeleteObject@4:NEAR
_TEXT	SEGMENT
_rc$ = 8
_uRoundCorners$ = 12
_uRoundness$ = 16
_hrgn2$ = 8
?CreateRoundCornerRegion@GS@@YAPAUHRGN__@@ABVCRect@WinHelper@@II@Z PROC NEAR ; GS::CreateRoundCornerRegion

; 12   : {

	push	ebx
	push	ebp
	push	esi

; 13   : 	HRGN hrgn = CreateRoundRectRgn( rc.left, rc.top, rc.right + 1, rc.bottom + 1, uRoundness, uRoundness );

	mov	esi, DWORD PTR _rc$[esp+8]
	push	edi
	mov	edi, DWORD PTR _uRoundness$[esp+12]
	mov	eax, DWORD PTR [esi+12]
	push	edi
	inc	eax
	push	edi
	push	eax
	mov	eax, DWORD PTR [esi+8]
	inc	eax
	push	eax
	push	DWORD PTR [esi+4]
	push	DWORD PTR [esi]
	call	DWORD PTR __imp__CreateRoundRectRgn@24

; 14   : 	HRGN hrgn2 = NULL;
; 15   : 
; 16   : 	if( !( uRoundCorners & GS::knGFCRoundLeftTop ) )

	test	BYTE PTR _uRoundCorners$[esp+12], 1
	mov	ebx, DWORD PTR __imp__CreateRectRgn@16
	mov	ebp, eax
	jne	SHORT $L56058

; 17   : 	{
; 18   : 		hrgn2 = CreateRectRgn( rc.left, rc.top, rc.left + uRoundness, rc.top + uRoundness );

	mov	eax, DWORD PTR [esi+4]
	mov	ecx, DWORD PTR [esi]
	lea	edx, DWORD PTR [eax+edi]
	push	edx
	lea	edx, DWORD PTR [ecx+edi]
	push	edx
	push	eax
	push	ecx
	call	ebx

; 19   : 		CombineRgn( hrgn, hrgn, hrgn2, RGN_OR );

	push	2
	push	eax
	push	ebp
	push	ebp
	mov	DWORD PTR _hrgn2$[esp+28], eax
	call	DWORD PTR __imp__CombineRgn@16

; 20   : 		DeleteObject( hrgn2 );

	push	DWORD PTR _hrgn2$[esp+12]
	call	DWORD PTR __imp__DeleteObject@4
$L56058:

; 21   : 	}
; 22   : 
; 23   : 	if( !( uRoundCorners & GS::knGFCRoundRightTop ) )

	test	BYTE PTR _uRoundCorners$[esp+12], 2
	jne	SHORT $L56059

; 24   : 	{
; 25   : 		hrgn2 = CreateRectRgn( rc.right - uRoundness, rc.top, rc.right, rc.top + uRoundness );

	mov	ecx, DWORD PTR [esi+4]
	mov	eax, DWORD PTR [esi+8]
	lea	edx, DWORD PTR [ecx+edi]
	push	edx
	push	eax
	sub	eax, edi
	push	ecx
	push	eax
	call	ebx

; 26   : 		CombineRgn( hrgn, hrgn, hrgn2, RGN_OR );

	push	2
	push	eax
	push	ebp
	push	ebp
	mov	DWORD PTR _hrgn2$[esp+28], eax
	call	DWORD PTR __imp__CombineRgn@16

; 27   : 		DeleteObject( hrgn2 );

	push	DWORD PTR _hrgn2$[esp+12]
	call	DWORD PTR __imp__DeleteObject@4
$L56059:

; 28   : 	}
; 29   : 
; 30   : 	if( !( uRoundCorners & GS::knGFCRoundLeftBottom ) )

	test	BYTE PTR _uRoundCorners$[esp+12], 4
	jne	SHORT $L56060

; 31   : 	{
; 32   : 		hrgn2 = CreateRectRgn( rc.left, rc.bottom - uRoundness, rc.left + uRoundness, rc.bottom );

	mov	ecx, DWORD PTR [esi]
	mov	eax, DWORD PTR [esi+12]
	push	eax
	sub	eax, edi
	lea	edx, DWORD PTR [ecx+edi]
	push	edx
	push	eax
	push	ecx
	call	ebx

; 33   : 		CombineRgn( hrgn, hrgn, hrgn2, RGN_OR );

	push	2
	push	eax
	push	ebp
	push	ebp
	mov	DWORD PTR _hrgn2$[esp+28], eax
	call	DWORD PTR __imp__CombineRgn@16

; 34   : 		DeleteObject( hrgn2 );

	push	DWORD PTR _hrgn2$[esp+12]
	call	DWORD PTR __imp__DeleteObject@4
$L56060:

; 35   : 	}
; 36   : 
; 37   : 	if( !( uRoundCorners & GS::knGFCRoundRightBottom ) )

	test	BYTE PTR _uRoundCorners$[esp+12], 8
	jne	SHORT $L56061

; 38   : 	{
; 39   : 		hrgn2 = CreateRectRgn( rc.right - uRoundness, rc.bottom - uRoundness, rc.right, rc.bottom );

	mov	eax, DWORD PTR [esi+12]
	mov	esi, DWORD PTR [esi+8]
	push	eax
	push	esi
	sub	eax, edi
	sub	esi, edi
	push	eax
	push	esi
	call	ebx
	mov	esi, eax

; 40   : 		CombineRgn( hrgn, hrgn, hrgn2, RGN_OR );

	push	2
	push	esi
	push	ebp
	push	ebp
	call	DWORD PTR __imp__CombineRgn@16

; 41   : 		DeleteObject( hrgn2 );

	push	esi
	call	DWORD PTR __imp__DeleteObject@4
$L56061:
	pop	edi

; 42   : 	}
; 43   : 
; 44   : 	return hrgn;

	mov	eax, ebp
	pop	esi
	pop	ebp
	pop	ebx

; 45   : }

	ret	0
?CreateRoundCornerRegion@GS@@YAPAUHRGN__@@ABVCRect@WinHelper@@II@Z ENDP ; GS::CreateRoundCornerRegion
_TEXT	ENDS
END
