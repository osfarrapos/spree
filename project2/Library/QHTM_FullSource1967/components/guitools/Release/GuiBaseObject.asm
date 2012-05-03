	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\GuiBaseObject.cpp
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
;	COMDAT ?PtInRect@CRect@WinHelper@@QBE_NABUtagPOINT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Intersect@CRect@WinHelper@@QBE_NABUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lock@CCriticalSection@WinHelper@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Unlock@CCriticalSection@WinHelper@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0FontDef@GS@@QAE@PBDHH_N11E1@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??2@YAPAXIPAX@Z
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
;	COMDAT ?IsVisible@CGuiBaseObject@GS@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Stop@CTimer@CGuiBaseObject@GS@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCGuiBaseObject@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAUtagRGBQUAD@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Add@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXABQAVCGuiBaseObject@GS@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@PAVCGuiBaseObject@GS@@@Container@@QBEABQAVCGuiBaseObject@GS@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEAAPAVCGuiBaseObject@GS@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAE_NI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAVCGuiBaseObject@GS@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsMemOverlap@Container@@YAXPAPAVCGuiBaseObject@GS@@0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsNoMemOverlap@Container@@YAXPAPAVCGuiBaseObject@GS@@0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructItems@Container@@YAXPAPAVCGuiBaseObject@GS@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7?$CArray@PAVCGuiBaseObject@GS@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CGuiBaseObject@GS@@6B@
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
PUBLIC	??0CGuiBaseObject@GS@@QAE@PAV01@@Z		; GS::CGuiBaseObject::CGuiBaseObject
PUBLIC	?OnDraw@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z ; GS::CGuiBaseObject::OnDraw
PUBLIC	?OnLayout@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z ; GS::CGuiBaseObject::OnLayout
PUBLIC	?OnMouseLeftDown@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z ; GS::CGuiBaseObject::OnMouseLeftDown
PUBLIC	?OnMouseLeftUp@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z ; GS::CGuiBaseObject::OnMouseLeftUp
PUBLIC	?OnMouseLeftDoubleclick@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z ; GS::CGuiBaseObject::OnMouseLeftDoubleclick
PUBLIC	?OnMouseCancelLeftMouse@CGuiBaseObject@GS@@UAEXXZ ; GS::CGuiBaseObject::OnMouseCancelLeftMouse
PUBLIC	?OnMouseWheel@CGuiBaseObject@GS@@UAEXH@Z	; GS::CGuiBaseObject::OnMouseWheel
PUBLIC	?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z ; GS::CGuiBaseObject::IsPointInSection
PUBLIC	?SetCursor@CGuiBaseObject@GS@@UAEXXZ		; GS::CGuiBaseObject::SetCursor
PUBLIC	?OnMouseLeave@CGuiBaseObject@GS@@UAEXXZ		; GS::CGuiBaseObject::OnMouseLeave
PUBLIC	?OnMouseEnter@CGuiBaseObject@GS@@UAEXXZ		; GS::CGuiBaseObject::OnMouseEnter
PUBLIC	?OnTimer@CGuiBaseObject@GS@@UAEXI@Z		; GS::CGuiBaseObject::OnTimer
PUBLIC	?GetSafeHwnd@CGuiBaseObject@GS@@UBEPAUHWND__@@XZ ; GS::CGuiBaseObject::GetSafeHwnd
PUBLIC	?ForceRedraw@CGuiBaseObject@GS@@MAEXABVCRect@WinHelper@@@Z ; GS::CGuiBaseObject::ForceRedraw
PUBLIC	?StartTimer@CGuiBaseObject@GS@@MAEIPAV12@I@Z	; GS::CGuiBaseObject::StartTimer
PUBLIC	?StopTimer@CGuiBaseObject@GS@@MAEXI@Z		; GS::CGuiBaseObject::StopTimer
PUBLIC	?AddChild@CGuiBaseObject@GS@@IAEXPAV12@@Z	; GS::CGuiBaseObject::AddChild
PUBLIC	??_7?$CArray@PAVCGuiBaseObject@GS@@@Container@@6B@ ; Container::CArray<GS::CGuiBaseObject *>::`vftable'
PUBLIC	??_G?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z ; Container::CArray<GS::CGuiBaseObject *>::`scalar deleting destructor'
PUBLIC	??_E?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z ; Container::CArray<GS::CGuiBaseObject *>::`vector deleting destructor'
PUBLIC	??_7CGuiBaseObject@GS@@6B@			; GS::CGuiBaseObject::`vftable'
PUBLIC	??_GCGuiBaseObject@GS@@UAEPAXI@Z		; GS::CGuiBaseObject::`scalar deleting destructor'
PUBLIC	??_ECGuiBaseObject@GS@@UAEPAXI@Z		; GS::CGuiBaseObject::`vector deleting destructor'
;	COMDAT ??_7CGuiBaseObject@GS@@6B@
; File ../array.inl
CONST	SEGMENT
??_7CGuiBaseObject@GS@@6B@ DD FLAT:??_ECGuiBaseObject@GS@@UAEPAXI@Z ; GS::CGuiBaseObject::`vftable'
	DD	FLAT:?OnDraw@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z
	DD	FLAT:?OnLayout@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z
	DD	FLAT:?OnMouseLeftDown@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z
	DD	FLAT:?OnMouseLeftUp@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z
	DD	FLAT:?OnMouseLeftDoubleclick@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z
	DD	FLAT:?OnMouseCancelLeftMouse@CGuiBaseObject@GS@@UAEXXZ
	DD	FLAT:?OnMouseWheel@CGuiBaseObject@GS@@UAEXH@Z
	DD	FLAT:?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z
	DD	FLAT:?SetCursor@CGuiBaseObject@GS@@UAEXXZ
	DD	FLAT:?OnMouseLeave@CGuiBaseObject@GS@@UAEXXZ
	DD	FLAT:?OnMouseEnter@CGuiBaseObject@GS@@UAEXXZ
	DD	FLAT:?OnTimer@CGuiBaseObject@GS@@UAEXI@Z
	DD	FLAT:?GetSafeHwnd@CGuiBaseObject@GS@@UBEPAUHWND__@@XZ
	DD	FLAT:?ForceRedraw@CGuiBaseObject@GS@@MAEXABVCRect@WinHelper@@@Z
	DD	FLAT:?StartTimer@CGuiBaseObject@GS@@MAEIPAV12@I@Z
	DD	FLAT:?StopTimer@CGuiBaseObject@GS@@MAEXI@Z
CONST	ENDS
;	COMDAT ??_7?$CArray@PAVCGuiBaseObject@GS@@@Container@@6B@
CONST	SEGMENT
??_7?$CArray@PAVCGuiBaseObject@GS@@@Container@@6B@ DD FLAT:??_E?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z ; Container::CArray<GS::CGuiBaseObject *>::`vftable'
CONST	ENDS
_TEXT	SEGMENT
_pgboParent$ = 8
??0CGuiBaseObject@GS@@QAE@PAV01@@Z PROC NEAR		; GS::CGuiBaseObject::CGuiBaseObject

; 22   : {

	push	esi
	mov	esi, ecx
	mov	ecx, DWORD PTR _pgboParent$[esp]
	xor	eax, eax
	mov	DWORD PTR [esi+24], OFFSET FLAT:??_7?$CArray@PAVCGuiBaseObject@GS@@@Container@@6B@ ; Container::CArray<GS::CGuiBaseObject *>::`vftable'

; 23   : 	if( m_pgboParent )

	cmp	ecx, eax
	mov	BYTE PTR [esi+20], 1
	mov	BYTE PTR [esi+21], al
	mov	BYTE PTR [esi+22], al
	mov	DWORD PTR [esi+28], eax
	mov	DWORD PTR [esi+32], 1
	mov	DWORD PTR [esi+36], eax
	mov	DWORD PTR [esi+40], eax
	mov	DWORD PTR [esi+44], ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CGuiBaseObject@GS@@6B@ ; GS::CGuiBaseObject::`vftable'
	je	SHORT $L56177

; 24   : 	{
; 25   : 		m_pgboParent->AddChild( this );

	push	esi
	call	?AddChild@CGuiBaseObject@GS@@IAEXPAV12@@Z ; GS::CGuiBaseObject::AddChild
$L56177:

; 26   : 	}
; 27   : }

	mov	eax, esi
	pop	esi
	ret	4
??0CGuiBaseObject@GS@@QAE@PAV01@@Z ENDP			; GS::CGuiBaseObject::CGuiBaseObject
_TEXT	ENDS
;	COMDAT ?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z
_TEXT	SEGMENT
_pt$ = 8
?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z PROC NEAR ; GS::CGuiBaseObject::IsPointInSection, COMDAT

; 60   : 			{ return PtInRect( pt ); }

	mov	edx, DWORD PTR _pt$[esp-4]
	mov	eax, DWORD PTR [edx]
	cmp	eax, DWORD PTR [ecx+4]
	jl	SHORT $L56524
	cmp	eax, DWORD PTR [ecx+12]
	jge	SHORT $L56524
	mov	eax, DWORD PTR [edx+4]
	cmp	eax, DWORD PTR [ecx+8]
	jl	SHORT $L56524
	cmp	eax, DWORD PTR [ecx+16]
	jge	SHORT $L56524
	push	1
	pop	eax
	jmp	SHORT $L56525
$L56524:
	xor	eax, eax
$L56525:
	ret	4
?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z ENDP ; GS::CGuiBaseObject::IsPointInSection
_TEXT	ENDS
PUBLIC	??1CGuiBaseObject@GS@@UAE@XZ			; GS::CGuiBaseObject::~CGuiBaseObject
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCGuiBaseObject@GS@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_GCGuiBaseObject@GS@@UAEPAXI@Z PROC NEAR		; GS::CGuiBaseObject::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1CGuiBaseObject@GS@@UAE@XZ		; GS::CGuiBaseObject::~CGuiBaseObject
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L56181
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L56181:
	mov	eax, esi
	pop	esi
	ret	4
??_GCGuiBaseObject@GS@@UAEPAXI@Z ENDP			; GS::CGuiBaseObject::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?RemoveChild@CGuiBaseObject@GS@@IAEXPAV12@@Z	; GS::CGuiBaseObject::RemoveChild
PUBLIC	?RemoveAll@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXXZ ; Container::CArray<GS::CGuiBaseObject *>::RemoveAll
_TEXT	SEGMENT
??1CGuiBaseObject@GS@@UAE@XZ PROC NEAR			; GS::CGuiBaseObject::~CGuiBaseObject

; 31   : {

	push	esi
	mov	esi, ecx

; 32   : 	if( m_pgboParent )

	mov	ecx, DWORD PTR [esi+44]
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CGuiBaseObject@GS@@6B@ ; GS::CGuiBaseObject::`vftable'
	test	ecx, ecx
	je	SHORT $L56185

; 33   : 	{
; 34   : 		m_pgboParent->RemoveChild( this );

	push	esi
	call	?RemoveChild@CGuiBaseObject@GS@@IAEXPAV12@@Z ; GS::CGuiBaseObject::RemoveChild
$L56185:

; 35   : 	}
; 36   : }

	lea	ecx, DWORD PTR [esi+24]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7?$CArray@PAVCGuiBaseObject@GS@@@Container@@6B@ ; Container::CArray<GS::CGuiBaseObject *>::`vftable'
	call	?RemoveAll@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXXZ ; Container::CArray<GS::CGuiBaseObject *>::RemoveAll
	pop	esi
	ret	0
??1CGuiBaseObject@GS@@UAE@XZ ENDP			; GS::CGuiBaseObject::~CGuiBaseObject
_dc$ = 8
?OnDraw@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z PROC NEAR ; GS::CGuiBaseObject::OnDraw

; 40   : {

	push	ebx
	push	ebp
	push	esi
	push	edi
	mov	edi, ecx

; 41   : 	if( IsVisible() )

	xor	ebx, ebx
	cmp	BYTE PTR [edi+20], bl
	je	SHORT $L56196

; 42   : 	{
; 43   : 		const WinHelper::CRect &rcClip = dc.GetClipRect();
; 44   : 		const PI_uint32 uSize = m_arrChild.GetSize();

	mov	ebp, DWORD PTR [edi+40]
	mov	esi, DWORD PTR _dc$[esp+12]

; 45   : 
; 46   : 		for( PI_uint32 n = 0; n < uSize; n++ )

	cmp	ebp, ebx
	jbe	SHORT $L56196
$L56194:

; 47   : 		{
; 48   : 			CGuiBaseObject *psect = m_arrChild[ n ];

	mov	eax, DWORD PTR [edi+28]
	mov	ecx, DWORD PTR [eax+ebx*4]

; 49   : 			if( rcClip.Intersect( *psect ) )

	mov	eax, ecx
	neg	eax
	lea	edx, DWORD PTR [ecx+4]
	sbb	eax, eax
	and	eax, edx
	mov	edx, DWORD PTR [eax]
	cmp	edx, DWORD PTR [esi+20]
	jge	SHORT $L56195
	mov	edx, DWORD PTR [eax+8]
	cmp	edx, DWORD PTR [esi+12]
	jle	SHORT $L56195
	mov	edx, DWORD PTR [eax+4]
	cmp	edx, DWORD PTR [esi+24]
	jge	SHORT $L56195
	mov	eax, DWORD PTR [eax+12]
	cmp	eax, DWORD PTR [esi+16]
	jle	SHORT $L56195

; 50   : 			{
; 51   : 				psect->OnDraw( dc );

	mov	eax, DWORD PTR [ecx]
	push	esi
	call	DWORD PTR [eax+4]
$L56195:
	inc	ebx
	cmp	ebx, ebp
	jb	SHORT $L56194
$L56196:
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 52   : 			}
; 53   : 		}
; 54   : 	}
; 55   : }

	ret	4
?OnDraw@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z ENDP ; GS::CGuiBaseObject::OnDraw
?OnLayout@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z PROC NEAR ; GS::CGuiBaseObject::OnLayout

; 60   : 	UNREF( dc );
; 61   : }

	ret	4
?OnLayout@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z ENDP ; GS::CGuiBaseObject::OnLayout
_TEXT	ENDS
PUBLIC	?SetSize@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAE_NI@Z ; Container::CArray<GS::CGuiBaseObject *>::SetSize
_TEXT	SEGMENT
_pgboChild$ = 8
?AddChild@CGuiBaseObject@GS@@IAEXPAV12@@Z PROC NEAR	; GS::CGuiBaseObject::AddChild

; 66   : 	m_arrChild.Add( pgboChild );

	mov	eax, DWORD PTR [ecx+40]
	push	esi
	lea	esi, DWORD PTR [ecx+24]
	inc	eax
	push	eax
	mov	ecx, esi
	call	?SetSize@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAE_NI@Z ; Container::CArray<GS::CGuiBaseObject *>::SetSize
	mov	eax, DWORD PTR [esi+16]
	mov	ecx, DWORD PTR [esi+4]
	mov	edx, DWORD PTR _pgboChild$[esp]
	pop	esi
	mov	DWORD PTR [ecx+eax*4-4], edx

; 67   : }

	ret	4
?AddChild@CGuiBaseObject@GS@@IAEXPAV12@@Z ENDP		; GS::CGuiBaseObject::AddChild
_TEXT	ENDS
PUBLIC	?RemoveAt@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXII@Z ; Container::CArray<GS::CGuiBaseObject *>::RemoveAt
_TEXT	SEGMENT
_pgboChild$ = 8
?RemoveChild@CGuiBaseObject@GS@@IAEXPAV12@@Z PROC NEAR	; GS::CGuiBaseObject::RemoveChild

; 72   : 	const PI_uint32 uSize = m_arrChild.GetSize();

	mov	eax, DWORD PTR [ecx+40]
	push	esi

; 73   : 	for( PI_uint32 n = 0; n < uSize; n++ )

	xor	esi, esi
	push	edi
	test	eax, eax
	jbe	SHORT $L56215

; 72   : 	const PI_uint32 uSize = m_arrChild.GetSize();

	mov	edx, DWORD PTR [ecx+28]
$L56213:

; 74   : 	{
; 75   : 		if( pgboChild == m_arrChild[ n ] )

	mov	edi, DWORD PTR _pgboChild$[esp+4]
	cmp	edi, DWORD PTR [edx]
	je	SHORT $L56599
	inc	esi
	add	edx, 4
	cmp	esi, eax
	jb	SHORT $L56213
	jmp	SHORT $L56215
$L56599:

; 76   : 		{
; 77   : 			m_arrChild.RemoveAt( n, 1 );

	push	1
	push	esi
	add	ecx, 24					; 00000018H
	call	?RemoveAt@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXII@Z ; Container::CArray<GS::CGuiBaseObject *>::RemoveAt
$L56215:
	pop	edi
	pop	esi

; 78   : 			return;
; 79   : 		}
; 80   : 	}
; 81   : 
; 82   : 	//
; 83   : 	//	Couldn't find a child that wanted to be removed
; 84   : 	ASSERT( false );
; 85   : }

	ret	4
?RemoveChild@CGuiBaseObject@GS@@IAEXPAV12@@Z ENDP	; GS::CGuiBaseObject::RemoveChild
_TEXT	ENDS
PUBLIC	?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z ; GS::CGuiBaseObject::FindObjectFromPoint
_TEXT	SEGMENT
_pt$ = 8
_bIncludeThis$ = 12
_uSize$ = -4
?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z PROC NEAR ; GS::CGuiBaseObject::FindObjectFromPoint

; 92   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	edi, ecx

; 93   : 	const PI_uint32 uSize = m_arrChild.GetSize();
; 94   : 	for( PI_uint32 n = 0; n < uSize; n++ )

	xor	ebx, ebx
	mov	eax, DWORD PTR [edi+40]
	test	eax, eax
	mov	DWORD PTR _uSize$[ebp], eax
	jbe	SHORT $L56227
$L56225:

; 95   : 	{
; 96   : 		CGuiBaseObject *psect = m_arrChild[ n ];

	mov	eax, DWORD PTR [edi+28]

; 97   : 		if( psect->IsPointInSection( pt ) && psect->IsVisible() )

	push	DWORD PTR _pt$[ebp]
	mov	esi, DWORD PTR [eax+ebx*4]
	mov	ecx, esi
	mov	eax, DWORD PTR [esi]
	call	DWORD PTR [eax+32]
	test	al, al
	je	SHORT $L56226
	cmp	BYTE PTR [esi+20], 0
	jne	SHORT $L56615
$L56226:

; 93   : 	const PI_uint32 uSize = m_arrChild.GetSize();
; 94   : 	for( PI_uint32 n = 0; n < uSize; n++ )

	inc	ebx
	cmp	ebx, DWORD PTR _uSize$[ebp]
	jb	SHORT $L56225
$L56227:

; 101  : 		}
; 102  : 	}
; 103  : 
; 104  : 	if( bIncludeThis )

	cmp	BYTE PTR _bIncludeThis$[ebp], 0
	je	SHORT $L56231

; 105  : 	{
; 106  : 		return IsPointInSection( pt ) ? const_cast<CGuiBaseObject *>(this) : NULL;

	mov	eax, DWORD PTR [edi]
	push	DWORD PTR _pt$[ebp]
	mov	ecx, edi
	call	DWORD PTR [eax+32]
	neg	al
	sbb	eax, eax
	and	eax, edi
	jmp	SHORT $L56233
$L56615:

; 98   : 		{
; 99   : 			CGuiBaseObject *pSect2 = psect->FindObjectFromPoint( pt, false );

	push	0
	mov	ecx, esi
	push	DWORD PTR _pt$[ebp]
	call	?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z ; GS::CGuiBaseObject::FindObjectFromPoint

; 100  : 			return pSect2 ? pSect2 : psect;

	test	eax, eax
	jne	SHORT $L56233
	mov	eax, esi
	jmp	SHORT $L56233
$L56231:

; 107  : 	}
; 108  : 	else
; 109  : 	{
; 110  : 		return NULL;

	xor	eax, eax
$L56233:
	pop	edi
	pop	esi
	pop	ebx

; 111  : 	}
; 112  : }

	leave
	ret	8
?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z ENDP ; GS::CGuiBaseObject::FindObjectFromPoint
?SetCursor@CGuiBaseObject@GS@@UAEXXZ PROC NEAR		; GS::CGuiBaseObject::SetCursor

; 117  : }

	ret	0
?SetCursor@CGuiBaseObject@GS@@UAEXXZ ENDP		; GS::CGuiBaseObject::SetCursor
_pgbo$ = 8
_uDuration$ = 12
?StartTimer@CGuiBaseObject@GS@@MAEIPAV12@I@Z PROC NEAR	; GS::CGuiBaseObject::StartTimer

; 122  : 	if( m_pgboParent )

	mov	ecx, DWORD PTR [ecx+44]
	test	ecx, ecx
	je	SHORT $L56242

; 123  : 	{
; 124  : 		return m_pgboParent->StartTimer( pgbo, uDuration );

	push	DWORD PTR _uDuration$[esp-4]
	mov	eax, DWORD PTR [ecx]
	push	DWORD PTR _pgbo$[esp]
	call	DWORD PTR [eax+60]
	jmp	SHORT $L56241
$L56242:

; 125  : 	}
; 126  : 	return 0;

	xor	eax, eax
$L56241:

; 127  : }

	ret	8
?StartTimer@CGuiBaseObject@GS@@MAEIPAV12@I@Z ENDP	; GS::CGuiBaseObject::StartTimer
_uTimerID$ = 8
?StopTimer@CGuiBaseObject@GS@@MAEXI@Z PROC NEAR		; GS::CGuiBaseObject::StopTimer

; 132  : 	if( m_pgboParent )

	mov	ecx, DWORD PTR [ecx+44]
	test	ecx, ecx
	je	SHORT $L56247

; 133  : 	{
; 134  : 		m_pgboParent->StopTimer( uTimerID );

	mov	eax, DWORD PTR [ecx]
	push	DWORD PTR _uTimerID$[esp-4]
	call	DWORD PTR [eax+64]
$L56247:

; 135  : 	}
; 136  : }

	ret	4
?StopTimer@CGuiBaseObject@GS@@MAEXI@Z ENDP		; GS::CGuiBaseObject::StopTimer
?OnTimer@CGuiBaseObject@GS@@UAEXI@Z PROC NEAR		; GS::CGuiBaseObject::OnTimer

; 141  : 	UNREF( uTimerID );
; 142  : }

	ret	4
?OnTimer@CGuiBaseObject@GS@@UAEXI@Z ENDP		; GS::CGuiBaseObject::OnTimer
?OnMouseEnter@CGuiBaseObject@GS@@UAEXXZ PROC NEAR	; GS::CGuiBaseObject::OnMouseEnter

; 147  : 	m_bMouseInSection = true;
; 148  : 	ForceRedraw( *this );

	mov	eax, DWORD PTR [ecx]
	mov	edx, ecx
	neg	edx
	push	esi
	lea	esi, DWORD PTR [ecx+4]
	sbb	edx, edx
	mov	BYTE PTR [ecx+22], 1
	and	edx, esi
	push	edx
	call	DWORD PTR [eax+56]
	pop	esi

; 149  : }

	ret	0
?OnMouseEnter@CGuiBaseObject@GS@@UAEXXZ ENDP		; GS::CGuiBaseObject::OnMouseEnter
?OnMouseLeave@CGuiBaseObject@GS@@UAEXXZ PROC NEAR	; GS::CGuiBaseObject::OnMouseLeave

; 154  : 	m_bMouseInSection = false;

	and	BYTE PTR [ecx+22], 0

; 155  : 	ForceRedraw( *this );

	mov	eax, DWORD PTR [ecx]
	mov	edx, ecx
	push	esi
	neg	edx
	lea	esi, DWORD PTR [ecx+4]
	sbb	edx, edx
	and	edx, esi
	push	edx
	call	DWORD PTR [eax+56]
	pop	esi

; 156  : }

	ret	0
?OnMouseLeave@CGuiBaseObject@GS@@UAEXXZ ENDP		; GS::CGuiBaseObject::OnMouseLeave
?GetSafeHwnd@CGuiBaseObject@GS@@UBEPAUHWND__@@XZ PROC NEAR ; GS::CGuiBaseObject::GetSafeHwnd

; 161  : 	if( m_pgboParent )

	mov	ecx, DWORD PTR [ecx+44]
	test	ecx, ecx
	je	SHORT $L56261

; 162  : 	{
; 163  : 		return m_pgboParent->GetSafeHwnd();

	mov	eax, DWORD PTR [ecx]
	jmp	DWORD PTR [eax+52]
$L56261:

; 164  : 	}
; 165  : 
; 166  : 	//	Either we have a parent or we do it ourselves
; 167  : 	ASSERT( FALSE );
; 168  : 	return NULL;

	xor	eax, eax

; 169  : }

	ret	0
?GetSafeHwnd@CGuiBaseObject@GS@@UBEPAUHWND__@@XZ ENDP	; GS::CGuiBaseObject::GetSafeHwnd
_rc$ = 8
?ForceRedraw@CGuiBaseObject@GS@@MAEXABVCRect@WinHelper@@@Z PROC NEAR ; GS::CGuiBaseObject::ForceRedraw

; 174  : 	if( m_pgboParent )

	mov	ecx, DWORD PTR [ecx+44]
	test	ecx, ecx
	je	SHORT $L56267

; 175  : 	{
; 176  : 		m_pgboParent->ForceRedraw( rc );

	mov	eax, DWORD PTR [ecx]
	push	DWORD PTR _rc$[esp-4]
	call	DWORD PTR [eax+56]
$L56267:

; 177  : 	}
; 178  : 	else
; 179  : 	{
; 180  : 		//	Either we have a parent or we handle the drawing ourselves
; 181  : 		ASSERT( FALSE );
; 182  : 	}
; 183  : }

	ret	4
?ForceRedraw@CGuiBaseObject@GS@@MAEXABVCRect@WinHelper@@@Z ENDP ; GS::CGuiBaseObject::ForceRedraw
?OnMouseLeftDown@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z PROC NEAR ; GS::CGuiBaseObject::OnMouseLeftDown

; 188  : 	UNREF( pt );
; 189  : 	m_bMouseDown = true;

	mov	BYTE PTR [ecx+21], 1

; 190  : }

	ret	4
?OnMouseLeftDown@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z ENDP ; GS::CGuiBaseObject::OnMouseLeftDown
?OnMouseLeftUp@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z PROC NEAR ; GS::CGuiBaseObject::OnMouseLeftUp

; 194  : 	m_bMouseDown = false;

	and	BYTE PTR [ecx+21], 0

; 195  : }

	ret	4
?OnMouseLeftUp@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z ENDP ; GS::CGuiBaseObject::OnMouseLeftUp
?OnMouseLeftDoubleclick@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z PROC NEAR ; GS::CGuiBaseObject::OnMouseLeftDoubleclick

; 200  : }

	ret	4
?OnMouseLeftDoubleclick@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z ENDP ; GS::CGuiBaseObject::OnMouseLeftDoubleclick
?OnMouseCancelLeftMouse@CGuiBaseObject@GS@@UAEXXZ PROC NEAR ; GS::CGuiBaseObject::OnMouseCancelLeftMouse

; 204  : 	m_bMouseDown = false;

	and	BYTE PTR [ecx+21], 0

; 205  : }

	ret	0
?OnMouseCancelLeftMouse@CGuiBaseObject@GS@@UAEXXZ ENDP	; GS::CGuiBaseObject::OnMouseCancelLeftMouse
_nDelta$ = 8
?OnMouseWheel@CGuiBaseObject@GS@@UAEXH@Z PROC NEAR	; GS::CGuiBaseObject::OnMouseWheel

; 210  : 	//
; 211  : 	//	Our default behaviour is to call out parent. That way the message
; 212  : 	//	is handled at the highest level but starts at the lowest level.
; 213  : 	if( m_pgboParent )

	mov	ecx, DWORD PTR [ecx+44]
	test	ecx, ecx
	je	SHORT $L56289

; 214  : 	{
; 215  : 		m_pgboParent->OnMouseWheel( nDelta );

	mov	eax, DWORD PTR [ecx]
	push	DWORD PTR _nDelta$[esp-4]
	call	DWORD PTR [eax+28]
$L56289:

; 216  : 	}
; 217  : }

	ret	4
?OnMouseWheel@CGuiBaseObject@GS@@UAEXH@Z ENDP		; GS::CGuiBaseObject::OnMouseWheel
_TEXT	ENDS
EXTRN	__imp__memmove:NEAR
;	COMDAT ?RemoveAt@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXII@Z
_TEXT	SEGMENT
_iPos$ = 8
_uItems$ = 12
?RemoveAt@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXII@Z PROC NEAR ; Container::CArray<GS::CGuiBaseObject *>::RemoveAt, COMDAT

; 191  : 	ASSERT( iPos + uItems <= m_uItemCount );
; 192  : 	ASSERT( uItems > 0 );
; 193  : 
; 194  : 	DestructItems( &m_pItems[iPos], uItems );

	mov	edx, DWORD PTR _iPos$[esp-4]
	push	ebx
	push	esi
	mov	esi, ecx
	push	edi

; 195  : 
; 196  : 	//
; 197  : 	// Move Any items above removeal area down
; 198  : 	const PI_uint32 uItemsToMove = m_uItemCount - iPos - uItems;

	mov	edi, DWORD PTR _uItems$[esp+8]
	mov	eax, DWORD PTR [esi+16]
	mov	ecx, DWORD PTR [esi+4]
	sub	eax, edx
	sub	eax, edi
	lea	ebx, DWORD PTR [ecx+edx*4]

; 199  : 	if( uItemsToMove )

	je	SHORT $L56640

; 200  : 		MoveItemsMemOverlap( &m_pItems[iPos + uItems], &m_pItems[iPos], uItemsToMove);

	shl	eax, 2
	add	edx, edi
	push	eax
	lea	eax, DWORD PTR [ecx+edx*4]
	push	eax
	push	ebx
	call	DWORD PTR __imp__memmove
	add	esp, 12					; 0000000cH
$L56640:

; 201  : 	m_uItemCount -= uItems;

	sub	DWORD PTR [esi+16], edi
	pop	edi
	pop	esi
	pop	ebx

; 202  : }

	ret	8
?RemoveAt@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXII@Z ENDP ; Container::CArray<GS::CGuiBaseObject *>::RemoveAt
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z PROC NEAR ; Container::CArray<GS::CGuiBaseObject *>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@PAVCGuiBaseObject@GS@@@Container@@6B@ ; Container::CArray<GS::CGuiBaseObject *>::`vftable'
	call	?RemoveAll@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXXZ ; Container::CArray<GS::CGuiBaseObject *>::RemoveAll
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L56343
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L56343:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z ENDP ; Container::CArray<GS::CGuiBaseObject *>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXXZ PROC NEAR ; Container::CArray<GS::CGuiBaseObject *>::RemoveAll, COMDAT

; 179  : {

	push	esi
	mov	esi, ecx

; 180  : 	DestructItems( m_pItems, m_uItemCount );
; 181  : 	delete [] reinterpret_cast<PI_byte *>( m_pItems );

	push	DWORD PTR [esi+4]
	call	??3@YAXPAX@Z				; operator delete

; 182  : 	m_pItems = NULL;

	xor	eax, eax
	pop	ecx
	mov	DWORD PTR [esi+4], eax

; 183  : 	m_uGrowBy = 1;

	mov	DWORD PTR [esi+8], 1

; 184  : 	m_uItemsAllocated = 0;

	mov	DWORD PTR [esi+12], eax

; 185  : 	m_uItemCount = 0;

	mov	DWORD PTR [esi+16], eax
	pop	esi

; 186  : }

	ret	0
?RemoveAll@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXXZ ENDP ; Container::CArray<GS::CGuiBaseObject *>::RemoveAll
_TEXT	ENDS
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
;	COMDAT ?SetSize@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAE_NI@Z
_TEXT	SEGMENT
_uSize$ = 8
_pItems$56373 = -4
?SetSize@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAE_NI@Z PROC NEAR ; Container::CArray<GS::CGuiBaseObject *>::SetSize, COMDAT

; 46   : {

	push	ecx
	push	ebx
	push	ebp
	mov	ebp, ecx
	push	esi

; 47   : 	bool bReallocationNeeded = false;
; 48   : 	ASSERT( m_uItemCount <= m_uItemsAllocated );
; 49   : 
; 50   : 
; 51   : 	if( m_uItemCount < uSize )

	mov	esi, DWORD PTR _uSize$[esp+12]
	xor	bl, bl
	mov	eax, DWORD PTR [ebp+16]
	push	edi
	cmp	eax, esi
	jae	SHORT $L56716

; 52   : 	{
; 53   : 		if( m_uItemsAllocated < uSize )

	mov	eax, DWORD PTR [ebp+12]
	cmp	eax, esi
	jae	SHORT $L56370

; 54   : 		{
; 55   : 			PI_uint32 uAllocateExtra = m_uGrowBy = m_uGrowBy < 262144 ? m_uGrowBy << 2 : 262144;

	mov	ecx, DWORD PTR [ebp+8]
	mov	ebx, 262144				; 00040000H
	cmp	ecx, ebx
	jae	SHORT $L56677
	mov	ebx, ecx
	shl	ebx, 2
$L56677:

; 56   : 
; 57   : 			//
; 58   : 			// Check to see if our grow by is enough?
; 59   : 			if( m_uItemsAllocated + uAllocateExtra < uSize )

	lea	ecx, DWORD PTR [eax+ebx]
	mov	DWORD PTR [ebp+8], ebx
	cmp	ecx, esi
	jae	SHORT $L56372

; 60   : 			{
; 61   : 				//
; 62   : 				//	Nope, so we allocate more
; 63   : 				uAllocateExtra = uSize - m_uItemsAllocated;

	mov	ebx, esi
	sub	ebx, eax
$L56372:

; 64   : 			}
; 65   : 
; 66   : 			T *pItems = reinterpret_cast<T *>( new PI_byte[sizeof( T ) * ( m_uItemsAllocated + uAllocateExtra )] );

	add	eax, ebx
	shl	eax, 2
	push	eax
	call	??2@YAPAXI@Z				; operator new
	pop	ecx
	mov	edi, eax

; 67   : 
; 68   : 			if( m_uItemCount )

	mov	ecx, DWORD PTR [ebp+16]
	mov	DWORD PTR _pItems$56373[esp+20], edi
	test	ecx, ecx
	je	SHORT $L56681

; 69   : 				MoveItemsNoMemOverlap( m_pItems, pItems, m_uItemCount );

	mov	esi, DWORD PTR [ebp+4]
	rep movsd
	mov	esi, DWORD PTR _uSize$[esp+16]
$L56681:

; 70   : 			delete[] (PI_byte*)m_pItems;

	push	DWORD PTR [ebp+4]
	call	??3@YAXPAX@Z				; operator delete

; 71   : 			m_pItems = pItems;

	mov	eax, DWORD PTR _pItems$56373[esp+24]

; 72   : 			m_uItemsAllocated += uAllocateExtra;

	add	DWORD PTR [ebp+12], ebx
	pop	ecx
	mov	DWORD PTR [ebp+4], eax

; 73   : 			bReallocationNeeded = true;

	mov	bl, 1
$L56370:

; 74   : 		}
; 75   : 
; 76   : 		//
; 77   : 		// Constuct the new items
; 78   : 		if( uSize > m_uItemCount )

	mov	eax, DWORD PTR [ebp+16]
	cmp	esi, eax
	jbe	SHORT $L56395

; 79   : 		{
; 80   : 			PI_uint32 uItemsToConstruct = uSize - m_uItemCount;
; 81   : 			ConstructItems( &m_pItems[m_uItemCount], uItemsToConstruct );

	mov	edx, DWORD PTR [ebp+4]
	mov	ecx, esi
	sub	ecx, eax

; 82   : 			m_uItemCount = uSize;

	mov	DWORD PTR [ebp+16], esi
	lea	edi, DWORD PTR [edx+eax*4]
	xor	eax, eax
	rep stosd

; 83   : 		}
; 84   : 
; 85   : 	}
; 86   : 	else

	jmp	SHORT $L56395
$L56716:

; 87   : 	{
; 88   : 		//
; 89   : 		//	Check to see if we need to reduce the size
; 90   : 		if( m_uItemCount > uSize )

	jbe	SHORT $L56395

; 91   : 		{
; 92   : 			//
; 93   : 			//	Get rid of excess elements
; 94   : 			RemoveAt( uSize, m_uItemCount - uSize );

	sub	eax, esi
	mov	ecx, ebp
	push	eax
	push	esi
	call	?RemoveAt@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXII@Z ; Container::CArray<GS::CGuiBaseObject *>::RemoveAt
$L56395:
	pop	edi
	pop	esi

; 95   : 		}
; 96   : 	}
; 97   : 
; 98   : 	return bReallocationNeeded;

	mov	al, bl
	pop	ebp
	pop	ebx

; 99   : }

	pop	ecx
	ret	4
?SetSize@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAE_NI@Z ENDP ; Container::CArray<GS::CGuiBaseObject *>::SetSize
_TEXT	ENDS
END
