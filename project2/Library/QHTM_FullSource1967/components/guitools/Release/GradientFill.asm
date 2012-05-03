	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\GradientFill.cpp
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
;	COMDAT ??0CRect@WinHelper@@QAE@XZ
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
;	COMDAT ?FillRect@CDrawContext@GS@@QAEXABVCRect@WinHelper@@K@Z
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
;	COMDAT ?RemoveClip@CDrawContext@GS@@QAEXXZ
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
PUBLIC	?GradientFillTopToBottom@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KK@Z ; GS::GradientFillTopToBottom
EXTRN	?GetBrush@CDrawContext@GS@@AAEPAUHBRUSH__@@K@Z:NEAR ; GS::CDrawContext::GetBrush
EXTRN	__imp__FillRect@12:NEAR
_TEXT	SEGMENT
_dc$ = 8
_rc$ = 12
_crTop$ = 16
_crBottom$ = 20
_r1$ = -12
_g1$ = -8
_b1$ = -4
_rcTemp$ = -36
?GradientFillTopToBottom@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KK@Z PROC NEAR ; GS::GradientFillTopToBottom

; 12   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 36					; 00000024H

; 13   :   const int r1 = GetRValue( crTop ), g1 = GetGValue( crTop ), b1 = GetBValue( crTop );

	mov	eax, DWORD PTR _crTop$[ebp]

; 14   :   const int r2 = GetRValue( crBottom ), g2 = GetGValue( crBottom ), b2 = GetBValue( crBottom );

	xor	edx, edx
	movzx	ecx, al
	mov	DWORD PTR _r1$[ebp], ecx
	xor	ecx, ecx
	mov	cl, ah
	push	ebx
	shr	eax, 16					; 00000010H
	movzx	eax, al
	movzx	ecx, cl
	mov	DWORD PTR _b1$[ebp], eax
	mov	eax, DWORD PTR _crBottom$[ebp]
	mov	DWORD PTR _g1$[ebp], ecx
	mov	dl, ah
	movzx	ecx, al
	push	esi
	push	edi
	shr	eax, 16					; 00000010H
	movzx	edi, al

; 15   :   
; 16   : 	WinHelper::CRect rcTemp;
; 17   :   rcTemp.left = rc.left;

	mov	eax, DWORD PTR _rc$[ebp]
	movzx	edx, dl
	mov	esi, DWORD PTR [eax]

; 18   :   rcTemp.right = rc.right; 
; 19   : 	rcTemp.top = rc.top - 1;

	mov	ebx, DWORD PTR [eax+4]
	mov	DWORD PTR _rcTemp$[ebp], esi
	mov	esi, DWORD PTR [eax+8]
	mov	DWORD PTR _rcTemp$[ebp+8], esi
	lea	esi, DWORD PTR [ebx-1]
	mov	DWORD PTR _rcTemp$[ebp+4], esi

; 20   : 	rcTemp.bottom = rc.top;
; 21   : 
; 22   : 	const int nEnd = rc.Height();

	mov	esi, DWORD PTR [eax+12]
	sub	esi, ebx
	mov	DWORD PTR _rcTemp$[ebp+12], ebx

; 23   :   for( int i=0; i < nEnd; i++ )

	test	esi, esi
	jle	SHORT $L56076
	and	DWORD PTR 12+[ebp], 0
	and	DWORD PTR 20+[ebp], 0
	and	DWORD PTR 16+[ebp], 0
	sub	ecx, DWORD PTR _r1$[ebp]
	sub	edx, DWORD PTR _g1$[ebp]
	sub	edi, DWORD PTR _b1$[ebp]

; 24   :   { 
; 25   :     int r,g,b; 
; 26   : 
; 27   :     r = r1 + (i * (r2-r1) / nEnd); 
; 28   :     g = g1 + (i * (g2-g1) / nEnd);
; 29   :     b = b1 + (i * (b2-b1) / nEnd);
; 30   :     
; 31   :     
; 32   :     rcTemp.top++;
; 33   :     rcTemp.bottom++; 
; 34   :     
; 35   :     dc.FillRect( rcTemp, RGB( r, g, b) );

	mov	ebx, DWORD PTR _dc$[ebp]
	mov	DWORD PTR -20+[ebp], ecx
	mov	DWORD PTR 8+[ebp], edi
	mov	DWORD PTR -16+[ebp], edx
	mov	edi, esi
$L56074:
	mov	eax, DWORD PTR 16+[ebp]
	xor	ecx, ecx
	cdq
	idiv	esi
	inc	DWORD PTR _rcTemp$[ebp+4]
	inc	DWORD PTR _rcTemp$[ebp+12]
	add	al, BYTE PTR _b1$[ebp]
	mov	ch, al
	mov	eax, DWORD PTR 20+[ebp]
	cdq
	idiv	esi
	add	al, BYTE PTR _g1$[ebp]
	mov	cl, al
	mov	eax, DWORD PTR 12+[ebp]
	cdq
	idiv	esi
	shl	ecx, 8
	add	al, BYTE PTR _r1$[ebp]
	movzx	eax, al
	or	ecx, eax
	push	ecx
	mov	ecx, ebx
	call	?GetBrush@CDrawContext@GS@@AAEPAUHBRUSH__@@K@Z ; GS::CDrawContext::GetBrush
	push	eax
	lea	eax, DWORD PTR _rcTemp$[ebp]
	push	eax
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__FillRect@12
	mov	eax, DWORD PTR 8+[ebp]
	add	DWORD PTR 16+[ebp], eax
	mov	eax, DWORD PTR -16+[ebp]
	add	DWORD PTR 20+[ebp], eax
	mov	eax, DWORD PTR -20+[ebp]
	add	DWORD PTR 12+[ebp], eax
	dec	edi
	jne	SHORT $L56074
$L56076:
	pop	edi
	pop	esi
	pop	ebx

; 36   :   }
; 37   : }

	leave
	ret	0
?GradientFillTopToBottom@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KK@Z ENDP ; GS::GradientFillTopToBottom
_TEXT	ENDS
PUBLIC	?GradientFillLeftToRight@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KK@Z ; GS::GradientFillLeftToRight
_TEXT	SEGMENT
_dc$ = 8
_rc$ = 12
_crTop$ = 16
_crBottom$ = 20
_r1$ = -12
_g1$ = -8
_b1$ = -4
_rcTemp$ = -36
?GradientFillLeftToRight@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KK@Z PROC NEAR ; GS::GradientFillLeftToRight

; 41   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 36					; 00000024H

; 42   :   const int r1 = GetRValue( crTop ), g1 = GetGValue( crTop ), b1 = GetBValue( crTop );

	mov	eax, DWORD PTR _crTop$[ebp]

; 43   :   const int r2 = GetRValue( crBottom ), g2 = GetGValue( crBottom ), b2 = GetBValue( crBottom );

	xor	edx, edx
	movzx	ecx, al
	mov	DWORD PTR _r1$[ebp], ecx
	xor	ecx, ecx
	mov	cl, ah
	push	ebx
	shr	eax, 16					; 00000010H
	movzx	eax, al
	movzx	ecx, cl
	mov	DWORD PTR _b1$[ebp], eax
	mov	eax, DWORD PTR _crBottom$[ebp]
	mov	DWORD PTR _g1$[ebp], ecx
	mov	dl, ah
	movzx	ecx, al
	push	esi
	push	edi
	shr	eax, 16					; 00000010H
	movzx	edi, al

; 44   :   
; 45   : 	WinHelper::CRect rcTemp;
; 46   :   rcTemp.left = rc.left - 1;

	mov	eax, DWORD PTR _rc$[ebp]
	movzx	edx, dl
	mov	ebx, DWORD PTR [eax]
	lea	esi, DWORD PTR [ebx-1]
	mov	DWORD PTR _rcTemp$[ebp], esi

; 47   :   rcTemp.right = rc.left + 1; 

	lea	esi, DWORD PTR [ebx+1]
	mov	DWORD PTR _rcTemp$[ebp+8], esi

; 48   : 	rcTemp.top = rc.top;

	mov	esi, DWORD PTR [eax+4]
	mov	DWORD PTR _rcTemp$[ebp+4], esi

; 49   : 	rcTemp.bottom = rc.bottom;

	mov	esi, DWORD PTR [eax+12]
	mov	DWORD PTR _rcTemp$[ebp+12], esi

; 50   : 
; 51   : 	const int nEnd = rc.Width() - 1;

	mov	esi, DWORD PTR [eax+8]
	sub	esi, ebx
	dec	esi

; 52   :   for( int i=0; i < nEnd; i++ )

	test	esi, esi
	jle	SHORT $L56111
	and	DWORD PTR 12+[ebp], 0
	and	DWORD PTR 20+[ebp], 0
	and	DWORD PTR 16+[ebp], 0
	sub	ecx, DWORD PTR _r1$[ebp]
	sub	edx, DWORD PTR _g1$[ebp]
	sub	edi, DWORD PTR _b1$[ebp]

; 53   :   { 
; 54   :     int r,g,b; 
; 55   : 
; 56   :     r = r1 + (i * (r2-r1) / nEnd); 
; 57   :     g = g1 + (i * (g2-g1) / nEnd);
; 58   :     b = b1 + (i * (b2-b1) / nEnd);
; 59   :     
; 60   :     
; 61   :     rcTemp.left++;
; 62   :     rcTemp.right++;
; 63   :     
; 64   :     dc.FillRect( rcTemp, RGB( r, g, b) );

	mov	ebx, DWORD PTR _dc$[ebp]
	mov	DWORD PTR -20+[ebp], ecx
	mov	DWORD PTR 8+[ebp], edi
	mov	DWORD PTR -16+[ebp], edx
	mov	edi, esi
$L56109:
	mov	eax, DWORD PTR 16+[ebp]
	xor	ecx, ecx
	cdq
	idiv	esi
	inc	DWORD PTR _rcTemp$[ebp]
	inc	DWORD PTR _rcTemp$[ebp+8]
	add	al, BYTE PTR _b1$[ebp]
	mov	ch, al
	mov	eax, DWORD PTR 20+[ebp]
	cdq
	idiv	esi
	add	al, BYTE PTR _g1$[ebp]
	mov	cl, al
	mov	eax, DWORD PTR 12+[ebp]
	cdq
	idiv	esi
	shl	ecx, 8
	add	al, BYTE PTR _r1$[ebp]
	movzx	eax, al
	or	ecx, eax
	push	ecx
	mov	ecx, ebx
	call	?GetBrush@CDrawContext@GS@@AAEPAUHBRUSH__@@K@Z ; GS::CDrawContext::GetBrush
	push	eax
	lea	eax, DWORD PTR _rcTemp$[ebp]
	push	eax
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__FillRect@12
	mov	eax, DWORD PTR 8+[ebp]
	add	DWORD PTR 16+[ebp], eax
	mov	eax, DWORD PTR -16+[ebp]
	add	DWORD PTR 20+[ebp], eax
	mov	eax, DWORD PTR -20+[ebp]
	add	DWORD PTR 12+[ebp], eax
	dec	edi
	jne	SHORT $L56109
$L56111:
	pop	edi
	pop	esi
	pop	ebx

; 65   :   }
; 66   : }

	leave
	ret	0
?GradientFillLeftToRight@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KK@Z ENDP ; GS::GradientFillLeftToRight
_TEXT	ENDS
PUBLIC	?SetClipRegion@CDrawContext@GS@@QAEXPAUHRGN__@@@Z ; GS::CDrawContext::SetClipRegion
PUBLIC	?GradientFillTopToBottom@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KKII@Z ; GS::GradientFillTopToBottom
EXTRN	__imp__SelectClipRgn@8:NEAR
EXTRN	?CreateRoundCornerRegion@GS@@YAPAUHRGN__@@ABVCRect@WinHelper@@II@Z:NEAR ; GS::CreateRoundCornerRegion
EXTRN	__imp__DeleteObject@4:NEAR
_TEXT	SEGMENT
_dc$ = 8
_rc$ = 12
_crTop$ = 16
_crBottom$ = 20
_uRoundCorners$ = 24
_uRoundness$ = 28
?GradientFillTopToBottom@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KKII@Z PROC NEAR ; GS::GradientFillTopToBottom

; 70   : {

	push	ebp
	mov	ebp, esp
	push	esi
	push	edi

; 71   : 	HRGN hrgn = CreateRoundCornerRegion( rc, uRoundCorners, uRoundness );

	push	DWORD PTR _uRoundness$[ebp]
	push	DWORD PTR _uRoundCorners$[ebp]
	push	DWORD PTR _rc$[ebp]
	call	?CreateRoundCornerRegion@GS@@YAPAUHRGN__@@ABVCRect@WinHelper@@II@Z ; GS::CreateRoundCornerRegion

; 72   : 	dc.SetClipRegion( hrgn );

	mov	esi, DWORD PTR _dc$[ebp]
	add	esp, 12					; 0000000cH
	mov	edi, eax
	mov	ecx, esi
	push	edi
	call	?SetClipRegion@CDrawContext@GS@@QAEXPAUHRGN__@@@Z ; GS::CDrawContext::SetClipRegion

; 73   : 	GradientFillTopToBottom( dc, rc, crTop, crBottom );

	push	DWORD PTR _crBottom$[ebp]
	push	DWORD PTR _crTop$[ebp]
	push	DWORD PTR _rc$[ebp]
	push	esi
	call	?GradientFillTopToBottom@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KK@Z ; GS::GradientFillTopToBottom
	add	esp, 16					; 00000010H

; 74   : 	VERIFY( DeleteObject( hrgn ) );

	push	edi
	call	DWORD PTR __imp__DeleteObject@4

; 75   : 	dc.RemoveClip();

	push	0
	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__SelectClipRgn@8
	pop	edi
	pop	esi

; 76   : }

	pop	ebp
	ret	0
?GradientFillTopToBottom@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KKII@Z ENDP ; GS::GradientFillTopToBottom
_TEXT	ENDS
EXTRN	__imp__GetWindowOrgEx@8:NEAR
EXTRN	__imp__OffsetClipRgn@12:NEAR
;	COMDAT ?SetClipRegion@CDrawContext@GS@@QAEXPAUHRGN__@@@Z
_TEXT	SEGMENT
_hrgn$ = 8
_pt$ = -8
?SetClipRegion@CDrawContext@GS@@QAEXPAUHRGN__@@@Z PROC NEAR ; GS::CDrawContext::SetClipRegion, COMDAT

; 83   : 	{

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ecx
	push	esi
	mov	esi, ecx

; 84   : 		::SelectClipRgn( m_hdc, hrgn );

	push	DWORD PTR _hrgn$[ebp]
	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__SelectClipRgn@8

; 85   : 		POINT pt;
; 86   : 		VAPI( ::GetWindowOrgEx( m_hdc, &pt ) );

	lea	eax, DWORD PTR _pt$[ebp]
	push	eax
	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__GetWindowOrgEx@8

; 87   : 		(void)::OffsetClipRgn( m_hdc, -pt.x, -pt.y );

	mov	eax, DWORD PTR _pt$[ebp+4]
	neg	eax
	push	eax
	mov	eax, DWORD PTR _pt$[ebp]
	neg	eax
	push	eax
	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__OffsetClipRgn@12
	pop	esi

; 88   : 	}

	leave
	ret	4
?SetClipRegion@CDrawContext@GS@@QAEXPAUHRGN__@@@Z ENDP	; GS::CDrawContext::SetClipRegion
_TEXT	ENDS
PUBLIC	?GradientFillLeftToRight@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KKII@Z ; GS::GradientFillLeftToRight
_TEXT	SEGMENT
_dc$ = 8
_rc$ = 12
_crTop$ = 16
_crBottom$ = 20
_uRoundCorners$ = 24
_uRoundness$ = 28
?GradientFillLeftToRight@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KKII@Z PROC NEAR ; GS::GradientFillLeftToRight

; 80   : {

	push	ebp
	mov	ebp, esp
	push	esi
	push	edi

; 81   : 	HRGN hrgn = CreateRoundCornerRegion( rc, uRoundCorners, uRoundness );

	push	DWORD PTR _uRoundness$[ebp]
	push	DWORD PTR _uRoundCorners$[ebp]
	push	DWORD PTR _rc$[ebp]
	call	?CreateRoundCornerRegion@GS@@YAPAUHRGN__@@ABVCRect@WinHelper@@II@Z ; GS::CreateRoundCornerRegion

; 82   : 	dc.SetClipRegion( hrgn );

	mov	esi, DWORD PTR _dc$[ebp]
	add	esp, 12					; 0000000cH
	mov	edi, eax
	mov	ecx, esi
	push	edi
	call	?SetClipRegion@CDrawContext@GS@@QAEXPAUHRGN__@@@Z ; GS::CDrawContext::SetClipRegion

; 83   : 	GradientFillLeftToRight( dc, rc, crTop, crBottom );

	push	DWORD PTR _crBottom$[ebp]
	push	DWORD PTR _crTop$[ebp]
	push	DWORD PTR _rc$[ebp]
	push	esi
	call	?GradientFillLeftToRight@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KK@Z ; GS::GradientFillLeftToRight
	add	esp, 16					; 00000010H

; 84   : 	VERIFY( DeleteObject( hrgn ) );

	push	edi
	call	DWORD PTR __imp__DeleteObject@4

; 85   : 	dc.RemoveClip();

	push	0
	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__SelectClipRgn@8
	pop	edi
	pop	esi

; 86   : }

	pop	ebp
	ret	0
?GradientFillLeftToRight@GS@@YAXAAVCDrawContext@1@ABVCRect@WinHelper@@KKII@Z ENDP ; GS::GradientFillLeftToRight
_TEXT	ENDS
END
