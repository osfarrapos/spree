	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\GuiBaseWindowObject.cpp
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
;	COMDAT ??_C@_0BB@NAAD@Magellan?5MSWHEEL?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_06FPAF@MouseZ?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BA@CLBL@MSWHEEL_ROLLMSG?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BF@DLPD@MSH_WHEELSUPPORT_MSG?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BF@ODOJ@MSH_SCROLL_LINES_MSG?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CRT$XCA	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCA	ENDS
CRT$XCU	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCU	ENDS
CRT$XCL	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCL	ENDS
CRT$XCC	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCC	ENDS
CRT$XCZ	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCZ	ENDS
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
;	COMDAT ??0CRect@WinHelper@@QAE@ABUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Width@CRect@WinHelper@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Height@CRect@WinHelper@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??BCRect@WinHelper@@QBEPBUtagRECT@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??BCRect@WinHelper@@QAEPAUtagRECT@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Size@CRect@WinHelper@@QBE?AVCSize@2@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Set@CRect@WinHelper@@QAEXHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Set@CRect@WinHelper@@QAEXABV12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PtInRect@CRect@WinHelper@@QBE_NABUtagPOINT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CPoint@WinHelper@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CPoint@WinHelper@@QAE@J@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CPoint@WinHelper@@QAE@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??BCPoint@WinHelper@@QAEPAUtagPOINT@@XZ
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
;	COMDAT ?GetBitmap@CDIB@GS@@QAEPAUHBITMAP__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CBufferedDCDIB@GS@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCBufferedDCDIB@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CBufferedDC@GS@@QAE@AAVCDrawContext@1@@Z
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
;	COMDAT ?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CTimer@CGuiBaseObject@GS@@QAE@PAV12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CTimer@CGuiBaseObject@GS@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsSet@CTimer@CGuiBaseObject@GS@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??8CTimer@CGuiBaseObject@GS@@QAE_NI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Stop@CTimer@CGuiBaseObject@GS@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Start@CTimer@CGuiBaseObject@GS@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashIt@Container@@YAIPBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSafeHwnd@CGuiBaseWindowObject@GS@@UBEPAUHWND__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCGuiBaseWindowObject@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAUtagRGBQUAD@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAE@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEPAPAVCGuiBaseObject@GS@@ABH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABHPAVCGuiBaseObject@GS@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$MapBucketEntry@HPAVCGuiBaseObject@GS@@@Container@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAVCGuiBaseObject@GS@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetBucketSize@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Find@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@ABEPAV?$MapBucketEntry@HPAVCGuiBaseObject@GS@@@2@IABH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashIt@Container@@YAIABH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ElementsTheSame@Container@@YA_NABH0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAVCGuiBaseObject@GS@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7?$CMap@HPAVCGuiBaseObject@GS@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CArray@PAVCGuiBaseObject@GS@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CGuiBaseWindowObject@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __tcsicmp
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7CBufferedDC@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CBufferedDCDIB@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
CRT$XCU	SEGMENT
_$S3	DD	FLAT:_$E2
CRT$XCU	ENDS
_DATA	SEGMENT
$SG57010 DB	'MSWHEEL_ROLLMSG', 00H
_DATA	ENDS
_TEXT	SEGMENT
_$E2	PROC NEAR
	jmp	_$E1
_$E2	ENDP
_TEXT	ENDS
EXTRN	__imp__RegisterWindowMessageA@4:NEAR
EXTRN	__imp__GetVersion@0:NEAR
_BSS	SEGMENT
_nMsgMouseWheel DD 01H DUP (?)
_BSS	ENDS
_TEXT	SEGMENT
_$E1	PROC NEAR

; 23   : 	 ? ::RegisterWindowMessage(MSH_MOUSEWHEEL) : 0;

	push	esi
	mov	esi, DWORD PTR __imp__GetVersion@0
	push	edi
	call	esi
	mov	edi, -2147483648			; 80000000H
	test	eax, edi
	je	SHORT $L57486
	call	esi
	cmp	ax, 4
	je	SHORT $L57488
$L57486:
	call	esi
	test	eax, edi
	jne	SHORT $L57487
	call	esi
	cmp	ax, 3
	jne	SHORT $L57487
$L57488:
	push	OFFSET FLAT:$SG57010
	call	DWORD PTR __imp__RegisterWindowMessageA@4
	mov	DWORD PTR _nMsgMouseWheel, eax
	jmp	SHORT $L57489
$L57487:
	and	DWORD PTR _nMsgMouseWheel, 0
$L57489:
	pop	edi
	pop	esi
	ret	0
_$E1	ENDP
_TEXT	ENDS
PUBLIC	?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z ; GS::CGuiBaseObject::IsPointInSection
PUBLIC	??0CGuiBaseWindowObject@GS@@QAE@PAVCGuiBaseObject@1@PAUHWND__@@@Z ; GS::CGuiBaseWindowObject::CGuiBaseWindowObject
PUBLIC	?OnCloseWindow@CGuiBaseWindowObject@GS@@UAEXXZ	; GS::CGuiBaseWindowObject::OnCloseWindow
PUBLIC	?OnCreateWindow@CGuiBaseWindowObject@GS@@UAE_NXZ ; GS::CGuiBaseWindowObject::OnCreateWindow
PUBLIC	?StartTimer@CGuiBaseWindowObject@GS@@UAEIPAVCGuiBaseObject@2@I@Z ; GS::CGuiBaseWindowObject::StartTimer
PUBLIC	?StopTimer@CGuiBaseWindowObject@GS@@UAEXI@Z	; GS::CGuiBaseWindowObject::StopTimer
PUBLIC	?ForceRedraw@CGuiBaseWindowObject@GS@@UAEXABVCRect@WinHelper@@@Z ; GS::CGuiBaseWindowObject::ForceRedraw
PUBLIC	?GetSafeHwnd@CGuiBaseWindowObject@GS@@UBEPAUHWND__@@XZ ; GS::CGuiBaseWindowObject::GetSafeHwnd
PUBLIC	?OnWindowsMessage@CGuiBaseWindowObject@GS@@UAEJIIJ@Z ; GS::CGuiBaseWindowObject::OnWindowsMessage
PUBLIC	?OnTimer@CGuiBaseWindowObject@GS@@MAEXI@Z	; GS::CGuiBaseWindowObject::OnTimer
PUBLIC	?SetBucketSize@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXI@Z ; Container::CMap<int,GS::CGuiBaseObject *>::SetBucketSize
PUBLIC	??_7?$CMap@HPAVCGuiBaseObject@GS@@@Container@@6B@ ; Container::CMap<int,GS::CGuiBaseObject *>::`vftable'
PUBLIC	??_G?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z ; Container::CMap<int,GS::CGuiBaseObject *>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z ; Container::CMap<int,GS::CGuiBaseObject *>::`vector deleting destructor'
PUBLIC	??_7CGuiBaseWindowObject@GS@@6B@		; GS::CGuiBaseWindowObject::`vftable'
PUBLIC	??_GCGuiBaseWindowObject@GS@@UAEPAXI@Z		; GS::CGuiBaseWindowObject::`scalar deleting destructor'
PUBLIC	??_ECGuiBaseWindowObject@GS@@UAEPAXI@Z		; GS::CGuiBaseWindowObject::`vector deleting destructor'
EXTRN	??0CGuiBaseObject@GS@@QAE@PAV01@@Z:NEAR		; GS::CGuiBaseObject::CGuiBaseObject
EXTRN	?OnDraw@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z:NEAR ; GS::CGuiBaseObject::OnDraw
EXTRN	?OnLayout@CGuiBaseObject@GS@@UAEXAAVCDrawContext@2@@Z:NEAR ; GS::CGuiBaseObject::OnLayout
EXTRN	?OnMouseLeftDown@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z:NEAR ; GS::CGuiBaseObject::OnMouseLeftDown
EXTRN	?OnMouseLeftUp@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z:NEAR ; GS::CGuiBaseObject::OnMouseLeftUp
EXTRN	?OnMouseLeftDoubleclick@CGuiBaseObject@GS@@UAEXABVCPoint@WinHelper@@@Z:NEAR ; GS::CGuiBaseObject::OnMouseLeftDoubleclick
EXTRN	?OnMouseCancelLeftMouse@CGuiBaseObject@GS@@UAEXXZ:NEAR ; GS::CGuiBaseObject::OnMouseCancelLeftMouse
EXTRN	?OnMouseWheel@CGuiBaseObject@GS@@UAEXH@Z:NEAR	; GS::CGuiBaseObject::OnMouseWheel
EXTRN	?SetCursor@CGuiBaseObject@GS@@UAEXXZ:NEAR	; GS::CGuiBaseObject::SetCursor
EXTRN	?OnMouseLeave@CGuiBaseObject@GS@@UAEXXZ:NEAR	; GS::CGuiBaseObject::OnMouseLeave
EXTRN	?OnMouseEnter@CGuiBaseObject@GS@@UAEXXZ:NEAR	; GS::CGuiBaseObject::OnMouseEnter
;	COMDAT ??_7CGuiBaseWindowObject@GS@@6B@
; File ../guitools/GuiBaseObject.h
CONST	SEGMENT
??_7CGuiBaseWindowObject@GS@@6B@ DD FLAT:??_ECGuiBaseWindowObject@GS@@UAEPAXI@Z ; GS::CGuiBaseWindowObject::`vftable'
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
	DD	FLAT:?OnTimer@CGuiBaseWindowObject@GS@@MAEXI@Z
	DD	FLAT:?GetSafeHwnd@CGuiBaseWindowObject@GS@@UBEPAUHWND__@@XZ
	DD	FLAT:?ForceRedraw@CGuiBaseWindowObject@GS@@UAEXABVCRect@WinHelper@@@Z
	DD	FLAT:?StartTimer@CGuiBaseWindowObject@GS@@UAEIPAVCGuiBaseObject@2@I@Z
	DD	FLAT:?StopTimer@CGuiBaseWindowObject@GS@@UAEXI@Z
	DD	FLAT:?OnCloseWindow@CGuiBaseWindowObject@GS@@UAEXXZ
	DD	FLAT:?OnCreateWindow@CGuiBaseWindowObject@GS@@UAE_NXZ
	DD	FLAT:?OnWindowsMessage@CGuiBaseWindowObject@GS@@UAEJIIJ@Z
CONST	ENDS
;	COMDAT ??_7?$CMap@HPAVCGuiBaseObject@GS@@@Container@@6B@
CONST	SEGMENT
??_7?$CMap@HPAVCGuiBaseObject@GS@@@Container@@6B@ DD FLAT:??_E?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z ; Container::CMap<int,GS::CGuiBaseObject *>::`vftable'
CONST	ENDS
_TEXT	SEGMENT
_pgboParent$ = 8
_hwnd$ = 12
??0CGuiBaseWindowObject@GS@@QAE@PAVCGuiBaseObject@1@PAUHWND__@@@Z PROC NEAR ; GS::CGuiBaseWindowObject::CGuiBaseWindowObject

; 37   : {

	push	esi
	push	edi
	push	DWORD PTR _pgboParent$[esp+4]
	mov	esi, ecx
	call	??0CGuiBaseObject@GS@@QAE@PAV01@@Z	; GS::CGuiBaseObject::CGuiBaseObject
	mov	eax, DWORD PTR _hwnd$[esp+4]
	lea	ecx, DWORD PTR [esi+52]
	xor	edi, edi
	push	97					; 00000061H
	mov	DWORD PTR [esi+48], eax
	mov	DWORD PTR [ecx+8], edi
	mov	DWORD PTR [ecx+12], edi
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7?$CMap@HPAVCGuiBaseObject@GS@@@Container@@6B@ ; Container::CMap<int,GS::CGuiBaseObject *>::`vftable'
	call	?SetBucketSize@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXI@Z ; Container::CMap<int,GS::CGuiBaseObject *>::SetBucketSize
	mov	DWORD PTR [esi+72], edi
	mov	DWORD PTR [esi+80], edi
	mov	DWORD PTR [esi+84], edi
	mov	DWORD PTR [esi+68], 1
	mov	DWORD PTR [esi+76], esi
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CGuiBaseWindowObject@GS@@6B@ ; GS::CGuiBaseWindowObject::`vftable'

; 38   : }

	mov	eax, esi
	pop	edi
	pop	esi
	ret	8
??0CGuiBaseWindowObject@GS@@QAE@PAVCGuiBaseObject@1@PAUHWND__@@@Z ENDP ; GS::CGuiBaseWindowObject::CGuiBaseWindowObject
_TEXT	ENDS
;	COMDAT ?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z
_TEXT	SEGMENT
_pt$ = 8
?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z PROC NEAR ; GS::CGuiBaseObject::IsPointInSection, COMDAT

; 60   : 			{ return PtInRect( pt ); }

	mov	edx, DWORD PTR _pt$[esp-4]
	mov	eax, DWORD PTR [edx]
	cmp	eax, DWORD PTR [ecx+4]
	jl	SHORT $L57504
	cmp	eax, DWORD PTR [ecx+12]
	jge	SHORT $L57504
	mov	eax, DWORD PTR [edx+4]
	cmp	eax, DWORD PTR [ecx+8]
	jl	SHORT $L57504
	cmp	eax, DWORD PTR [ecx+16]
	jge	SHORT $L57504
	push	1
	pop	eax
	jmp	SHORT $L57505
$L57504:
	xor	eax, eax
$L57505:
	ret	4
?IsPointInSection@CGuiBaseObject@GS@@UBE_NABVCPoint@WinHelper@@@Z ENDP ; GS::CGuiBaseObject::IsPointInSection
_TEXT	ENDS
;	COMDAT ?GetSafeHwnd@CGuiBaseWindowObject@GS@@UBEPAUHWND__@@XZ
_TEXT	SEGMENT
?GetSafeHwnd@CGuiBaseWindowObject@GS@@UBEPAUHWND__@@XZ PROC NEAR ; GS::CGuiBaseWindowObject::GetSafeHwnd, COMDAT

; 117  : 		HWND GetSafeHwnd() const { return m_hwnd; }

	mov	eax, DWORD PTR [ecx+48]
	ret	0
?GetSafeHwnd@CGuiBaseWindowObject@GS@@UBEPAUHWND__@@XZ ENDP ; GS::CGuiBaseWindowObject::GetSafeHwnd
_TEXT	ENDS
PUBLIC	??1CGuiBaseWindowObject@GS@@UAE@XZ		; GS::CGuiBaseWindowObject::~CGuiBaseWindowObject
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCGuiBaseWindowObject@GS@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_GCGuiBaseWindowObject@GS@@UAEPAXI@Z PROC NEAR	; GS::CGuiBaseWindowObject::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1CGuiBaseWindowObject@GS@@UAE@XZ	; GS::CGuiBaseWindowObject::~CGuiBaseWindowObject
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L57031
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L57031:
	mov	eax, esi
	pop	esi
	ret	4
??_GCGuiBaseWindowObject@GS@@UAEPAXI@Z ENDP		; GS::CGuiBaseWindowObject::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?Stop@CTimer@CGuiBaseObject@GS@@QAEXXZ		; GS::CGuiBaseObject::CTimer::Stop
PUBLIC	??1?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAE@XZ ; Container::CMap<int,GS::CGuiBaseObject *>::~CMap<int,GS::CGuiBaseObject *>
EXTRN	??1CGuiBaseObject@GS@@UAE@XZ:NEAR		; GS::CGuiBaseObject::~CGuiBaseObject
_TEXT	SEGMENT
??1CGuiBaseWindowObject@GS@@UAE@XZ PROC NEAR		; GS::CGuiBaseWindowObject::~CGuiBaseWindowObject

; 42   : {

	push	esi
	mov	esi, ecx

; 43   : }

	lea	ecx, DWORD PTR [esi+72]
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CGuiBaseWindowObject@GS@@6B@ ; GS::CGuiBaseWindowObject::`vftable'
	call	?Stop@CTimer@CGuiBaseObject@GS@@QAEXXZ	; GS::CGuiBaseObject::CTimer::Stop
	lea	ecx, DWORD PTR [esi+52]
	call	??1?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAE@XZ ; Container::CMap<int,GS::CGuiBaseObject *>::~CMap<int,GS::CGuiBaseObject *>
	mov	ecx, esi
	call	??1CGuiBaseObject@GS@@UAE@XZ		; GS::CGuiBaseObject::~CGuiBaseObject
	pop	esi
	ret	0
??1CGuiBaseWindowObject@GS@@UAE@XZ ENDP			; GS::CGuiBaseWindowObject::~CGuiBaseWindowObject
_TEXT	ENDS
;	COMDAT ?Stop@CTimer@CGuiBaseObject@GS@@QAEXXZ
_TEXT	SEGMENT
?Stop@CTimer@CGuiBaseObject@GS@@QAEXXZ PROC NEAR	; GS::CGuiBaseObject::CTimer::Stop, COMDAT

; 94   : 			{

	push	esi
	mov	esi, ecx

; 95   : 				if( m_uTimer )

	mov	eax, DWORD PTR [esi]
	test	eax, eax
	je	SHORT $L56716

; 96   : 				{
; 97   : 					m_pOwner->StopTimer( m_uTimer );

	mov	ecx, DWORD PTR [esi+4]
	push	eax
	mov	edx, DWORD PTR [ecx]
	call	DWORD PTR [edx+64]

; 98   : 					m_uTimer = 0;

	and	DWORD PTR [esi], 0
$L56716:
	pop	esi

; 99   : 				}
; 100  : 				ASSERT( !m_uTimer );
; 101  : 			}

	ret	0
?Stop@CTimer@CGuiBaseObject@GS@@QAEXXZ ENDP		; GS::CGuiBaseObject::CTimer::Stop
_TEXT	ENDS
PUBLIC	??0CRect@WinHelper@@QAE@ABUtagRECT@@@Z		; WinHelper::CRect::CRect
PUBLIC	??0CBufferedDC@GS@@QAE@AAVCDrawContext@1@@Z	; GS::CBufferedDC::CBufferedDC
PUBLIC	??1CBufferedDC@GS@@UAE@XZ			; GS::CBufferedDC::~CBufferedDC
PUBLIC	?Start@CTimer@CGuiBaseObject@GS@@QAEXI@Z	; GS::CGuiBaseObject::CTimer::Start
PUBLIC	?CancelMouseDown@CGuiBaseWindowObject@GS@@IAEXXZ ; GS::CGuiBaseWindowObject::CancelMouseDown
PUBLIC	?CancelHighlight@CGuiBaseWindowObject@GS@@IAEXXZ ; GS::CGuiBaseWindowObject::CancelHighlight
PUBLIC	?Lookup@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEPAPAVCGuiBaseObject@GS@@ABH@Z ; Container::CMap<int,GS::CGuiBaseObject *>::Lookup
EXTRN	__imp__GetWindow@8:NEAR
EXTRN	__imp__SendMessageA@16:NEAR
EXTRN	__imp__DefWindowProcA@16:NEAR
EXTRN	?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z:NEAR ; GS::CGuiBaseObject::FindObjectFromPoint
EXTRN	__imp__RealizePalette@4:NEAR
EXTRN	__imp__SelectPalette@12:NEAR
EXTRN	__imp__IsWindowVisible@4:NEAR
EXTRN	__imp__BeginPaint@8:NEAR
EXTRN	__imp__EndPaint@8:NEAR
EXTRN	?GetCurrentWindowsPalette@GS@@YAPAUHPALETTE__@@XZ:NEAR ; GS::GetCurrentWindowsPalette
EXTRN	__imp__InvalidateRect@12:NEAR
EXTRN	__imp__GetClientRect@8:NEAR
EXTRN	__imp__GetWindowRect@8:NEAR
EXTRN	?HandleMouseWheelRegisteredMessage@@YIJPAUHWND__@@IJ@Z:NEAR ; HandleMouseWheelRegisteredMessage
EXTRN	__imp__GetCursorPos@4:NEAR
EXTRN	__imp__ScreenToClient@8:NEAR
EXTRN	__imp__KillTimer@8:NEAR
EXTRN	??0CDrawContext@GS@@QAE@PBVCRect@WinHelper@@PAUHDC__@@_N@Z:NEAR ; GS::CDrawContext::CDrawContext
EXTRN	??1CDrawContext@GS@@UAE@XZ:NEAR			; GS::CDrawContext::~CDrawContext
EXTRN	__imp__SetWindowOrgEx@16:NEAR
_TEXT	SEGMENT
$T57523 = 8
_message$ = 8
_wParam$ = 12
_lParam$ = 16
_ps$57047 = -204
_rc$57048 = -32
_dc$57049 = -140
_dcBuffer$57050 = -320
_pt$57052 = -8
_pt$57072 = -8
_rc$57080 = -16
_dc$57081 = -140
_pt$57090 = -8
_pt$57099 = -8
_pt$57103 = -8
_pt$57107 = -8
_rcPaint$57118 = -64
_dc$57119 = -140
_rcWindow$57121 = -32
_hwndChild$57122 = 16
_ptOldOrg$57123 = -8
_rc$57127 = -48
?OnWindowsMessage@CGuiBaseWindowObject@GS@@UAEJIIJ@Z PROC NEAR ; GS::CGuiBaseWindowObject::OnWindowsMessage

; 47   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 320				; 00000140H
	push	ebx
	push	esi
	mov	esi, ecx

; 48   : 	switch( message )
; 49   : 	{

	mov	eax, 275				; 00000113H
	mov	ecx, DWORD PTR _message$[ebp]
	push	edi
	cmp	ecx, eax
	ja	$L57524
	je	$L57055
	mov	eax, ecx
	dec	eax
	je	$L57110
	sub	eax, 4
	je	$L57079
	sub	eax, 10					; 0000000aH
	je	SHORT $L57046
	dec	eax
	je	SHORT $L57078
	sub	eax, 4
	je	SHORT $L57045
	sub	eax, 12					; 0000000cH
	jne	$L57130

; 73   : 
; 74   : 
; 75   : 	case WM_SETCURSOR:
; 76   : 		{		
; 77   : 			WinHelper::CPoint pt;
; 78   : 			GetCursorPos( &pt );

	lea	eax, DWORD PTR _pt$57052[ebp]
	push	eax
	call	DWORD PTR __imp__GetCursorPos@4

; 79   : 
; 80   : 			ScreenToClient( GetSafeHwnd(), &pt );

	lea	eax, DWORD PTR _pt$57052[ebp]
	mov	ecx, esi
	push	eax
	mov	eax, DWORD PTR [esi]
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__ScreenToClient@8

; 81   : 			CGuiBaseObject *pgbo = FindObjectFromPoint( pt, true );

	lea	eax, DWORD PTR _pt$57052[ebp]
	push	1
	push	eax
	mov	ecx, esi
	call	?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z ; GS::CGuiBaseObject::FindObjectFromPoint

; 82   : 			if( pgbo )

	test	eax, eax
	je	$L57077

; 83   : 			{
; 84   : 				pgbo->SetCursor();

	mov	edx, DWORD PTR [eax]
	mov	ecx, eax
	call	DWORD PTR [edx+36]

; 85   : 			}
; 86   : 		}
; 87   : 		break;

	jmp	$L57077
$L57045:

; 50   : 	case WM_ERASEBKGND:
; 51   : 		return TRUE;

	push	1
	pop	eax
	jmp	$L57040
$L57078:

; 126  : 				}
; 127  : 			}
; 128  : 		}
; 129  : 		break;
; 130  : 
; 131  : 	case WM_CLOSE:
; 132  : 		OnCloseWindow();

	mov	eax, DWORD PTR [esi]
	mov	ecx, esi
	call	DWORD PTR [eax+68]

; 133  : 		break;

	jmp	$L57077
$L57046:

; 52   : 
; 53   : 	case WM_PAINT:
; 54   : 		{
; 55   : 			PAINTSTRUCT ps;
; 56   : 			BeginPaint( GetSafeHwnd(), &ps );

	lea	eax, DWORD PTR _ps$57047[ebp]
	mov	ecx, esi
	push	eax
	mov	eax, DWORD PTR [esi]
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__BeginPaint@8

; 57   : 			{
; 58   : 				WinHelper::CRect rc( ps.rcPaint );

	lea	eax, DWORD PTR _ps$57047[ebp+8]
	lea	ecx, DWORD PTR _rc$57048[ebp]
	push	eax
	call	??0CRect@WinHelper@@QAE@ABUtagRECT@@@Z	; WinHelper::CRect::CRect

; 59   : 				CDrawContext dc( &rc, ps.hdc );

	push	0
	lea	eax, DWORD PTR _rc$57048[ebp]
	push	DWORD PTR _ps$57047[ebp]
	lea	ecx, DWORD PTR _dc$57049[ebp]
	push	eax
	call	??0CDrawContext@GS@@QAE@PBVCRect@WinHelper@@PAUHDC__@@_N@Z ; GS::CDrawContext::CDrawContext

; 60   : 				//TRACE( _T("WM_PAINT RECT %d, %d, %d, %d\n"), rc.left, rc.top, rc.right, rc.bottom );
; 61   : 				SelectPalette( ps.hdc, GetCurrentWindowsPalette(), TRUE );

	push	1
	call	?GetCurrentWindowsPalette@GS@@YAPAUHPALETTE__@@XZ ; GS::GetCurrentWindowsPalette
	push	eax
	push	DWORD PTR _ps$57047[ebp]
	call	DWORD PTR __imp__SelectPalette@12

; 62   : 				RealizePalette( ps.hdc );

	push	DWORD PTR _ps$57047[ebp]
	call	DWORD PTR __imp__RealizePalette@4

; 63   : 				{
; 64   : 					CBufferedDC dcBuffer( dc );

	lea	eax, DWORD PTR _dc$57049[ebp]
	lea	ecx, DWORD PTR _dcBuffer$57050[ebp]
	push	eax
	call	??0CBufferedDC@GS@@QAE@AAVCDrawContext@1@@Z ; GS::CBufferedDC::CBufferedDC

; 65   : 					OnDraw( dcBuffer );

	mov	eax, DWORD PTR [esi]
	lea	ecx, DWORD PTR _dcBuffer$57050[ebp]
	push	ecx
	mov	ecx, esi
	call	DWORD PTR [eax+4]

; 66   : 					//	Simple test to draw a rect around what needs to be drawn.
; 67   : 					//dcBuffer.Rectangle( rc, RGB( 255, 0, 0 ) );
; 68   : 				}

	lea	ecx, DWORD PTR _dcBuffer$57050[ebp]
	call	??1CBufferedDC@GS@@UAE@XZ		; GS::CBufferedDC::~CBufferedDC

; 69   : 			}

	lea	ecx, DWORD PTR _dc$57049[ebp]
	call	??1CDrawContext@GS@@UAE@XZ		; GS::CDrawContext::~CDrawContext

; 70   : 			EndPaint( GetSafeHwnd(), &ps );

	lea	eax, DWORD PTR _ps$57047[ebp]
	mov	ecx, esi
	push	eax
	mov	eax, DWORD PTR [esi]
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__EndPaint@8

; 71   : 		}
; 72   : 		break;

	jmp	$L57077
$L57079:

; 134  : 
; 135  : 
; 136  : 	case WM_SIZE:
; 137  : 		{
; 138  : 			WinHelper::CRect rc;
; 139  : 			GetClientRect( GetSafeHwnd(), rc );

	lea	eax, DWORD PTR _rc$57080[ebp]
	mov	ecx, esi
	push	eax
	mov	eax, DWORD PTR [esi]
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__GetClientRect@8

; 140  : 			CDrawContext dc;

	xor	edi, edi
	lea	ecx, DWORD PTR _dc$57081[ebp]
	push	edi
	push	edi
	push	edi
	call	??0CDrawContext@GS@@QAE@PBVCRect@WinHelper@@PAUHDC__@@_N@Z ; GS::CDrawContext::CDrawContext

; 141  : 			Set( rc );

	mov	eax, DWORD PTR _rc$57080[ebp]

; 142  : 			OnLayout( dc );

	lea	ecx, DWORD PTR _dc$57081[ebp]
	mov	DWORD PTR [esi+4], eax
	mov	eax, DWORD PTR _rc$57080[ebp+4]
	mov	DWORD PTR [esi+8], eax
	mov	eax, DWORD PTR _rc$57080[ebp+8]
	mov	DWORD PTR [esi+12], eax
	mov	eax, DWORD PTR _rc$57080[ebp+12]
	mov	DWORD PTR [esi+16], eax
	mov	eax, DWORD PTR [esi]
	push	ecx
	mov	ecx, esi
	call	DWORD PTR [eax+8]

; 143  : 			InvalidateRect( GetSafeHwnd(), NULL, TRUE );

	mov	eax, DWORD PTR [esi]
	push	1
	push	edi
	mov	ecx, esi
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__InvalidateRect@12

; 144  : 		}

	lea	ecx, DWORD PTR _dc$57081[ebp]
	call	??1CDrawContext@GS@@UAE@XZ		; GS::CDrawContext::~CDrawContext

; 145  : 		break;

	jmp	$L57077
$L57110:

; 206  : 
; 207  : 	case WM_CREATE:
; 208  : 		if( OnCreateWindow() )

	mov	eax, DWORD PTR [esi]
	mov	ecx, esi
	call	DWORD PTR [eax+72]
	neg	al
	sbb	eax, eax
	inc	eax

; 209  : 		{
; 210  : 			return 0;
; 211  : 		}
; 212  : 		return 1;

	jmp	$L57040
$L57055:

; 88   : 
; 89   : 
; 90   : 	case WM_TIMER:
; 91   : 		{
; 92   : 			CGuiBaseObject **pgbo = m_mapTimerEvents.Lookup( (PI_uint32)wParam );

	mov	edi, DWORD PTR _wParam$[ebp]
	lea	eax, DWORD PTR $T57523[ebp]
	push	eax
	lea	ecx, DWORD PTR [esi+52]
	mov	DWORD PTR $T57523[ebp], edi
	call	?Lookup@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEPAPAVCGuiBaseObject@GS@@ABH@Z ; Container::CMap<int,GS::CGuiBaseObject *>::Lookup

; 93   : 			if( pgbo )

	test	eax, eax
	je	SHORT $L57059

; 94   : 			{
; 95   : 				ASSERT_VALID_WRITEOBJPTR( pgbo );
; 96   : 				(*pgbo)->OnTimer( (PI_uint32)wParam );

	mov	ecx, DWORD PTR [eax]
	push	edi
	mov	eax, DWORD PTR [ecx]
	call	DWORD PTR [eax+48]

; 97   : 			}
; 98   : 			else

	jmp	$L57077
$L57059:

; 99   : 			{
; 100  : 				::KillTimer( GetSafeHwnd(), (PI_uint32)wParam );

	mov	eax, DWORD PTR [esi]
	push	edi
	mov	ecx, esi
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__KillTimer@8

; 101  : 			}
; 102  : 		}		
; 103  : 		break;

	jmp	$L57077
$L57524:

; 48   : 	switch( message )
; 49   : 	{

	mov	eax, ecx
	sub	eax, 512				; 00000200H
	je	$L57064
	dec	eax
	je	$L57098
	dec	eax
	je	$L57101
	dec	eax
	je	$L57106
	sub	eax, 7
	je	$L57082
	sub	eax, 270				; 0000010eH
	je	SHORT $L57112
$L57130:

; 265  : #endif	// !defined (_WIN32_WCE)
; 266  : 
; 267  : 	default:
; 268  : 		#if !defined (_WIN32_WCE)
; 269  : 			if( message == nMsgMouseWheel && nMsgMouseWheel )

	mov	eax, DWORD PTR _nMsgMouseWheel
	cmp	ecx, eax
	jne	SHORT $L57131
	test	eax, eax
	je	SHORT $L57131

; 270  : 			{
; 271  : 				return HandleMouseWheelRegisteredMessage( GetSafeHwnd(), wParam, lParam );

	push	DWORD PTR _lParam$[ebp]
	mov	eax, DWORD PTR [esi]
	mov	ecx, esi
	call	DWORD PTR [eax+52]
	mov	edx, DWORD PTR _wParam$[ebp]
	mov	ecx, eax
	call	?HandleMouseWheelRegisteredMessage@@YIJPAUHWND__@@IJ@Z ; HandleMouseWheelRegisteredMessage
	jmp	$L57040
$L57131:

; 272  : 			}
; 273  : 		#endif	//	!defined (_WIN32_WCE)
; 274  : 
; 275  : 		return DefWindowProc( GetSafeHwnd(), message, wParam, lParam );

	push	DWORD PTR _lParam$[ebp]
	mov	eax, DWORD PTR [esi]
	push	DWORD PTR _wParam$[ebp]
	push	ecx
	mov	ecx, esi
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__DefWindowProcA@16
	jmp	$L57040
$L57112:

; 213  : 
; 214  : 	#if !defined (_WIN32_WCE)
; 215  : 	case WM_PRINTCLIENT:
; 216  : 		{
; 217  : 			if( !(lParam & PRF_CHECKVISIBLE) ||  IsWindowVisible( m_hwnd ) )

	test	BYTE PTR _lParam$[ebp], 1
	je	SHORT $L57114
	push	DWORD PTR [esi+48]
	call	DWORD PTR __imp__IsWindowVisible@4
	test	eax, eax
	je	$L57077
$L57114:

; 218  : 			{
; 219  : 				HDC hdc = (HDC)wParam;
; 220  : 				if( lParam & PRF_ERASEBKGND )
; 221  : 				{
; 222  : //					if( pWnd->IsTransparent() )
; 223  : //					{
; 224  : //						SendMessage( ::GetParent( hwnd ), message, wParam, lParam );
; 225  : //					}
; 226  : 				}
; 227  : 
; 228  : 				WinHelper::CRect rcPaint;
; 229  : 				GetClientRect( m_hwnd, &rcPaint );

	lea	eax, DWORD PTR _rcPaint$57118[ebp]
	push	eax
	push	DWORD PTR [esi+48]
	call	DWORD PTR __imp__GetClientRect@8

; 230  : 				{
; 231  : 					CDrawContext dc( &rcPaint, hdc );

	push	0
	lea	eax, DWORD PTR _rcPaint$57118[ebp]
	push	DWORD PTR _wParam$[ebp]
	lea	ecx, DWORD PTR _dc$57119[ebp]
	push	eax
	call	??0CDrawContext@GS@@QAE@PBVCRect@WinHelper@@PAUHDC__@@_N@Z ; GS::CDrawContext::CDrawContext

; 232  : 					SelectPalette( hdc, GetCurrentWindowsPalette(), TRUE );

	push	1
	call	?GetCurrentWindowsPalette@GS@@YAPAUHPALETTE__@@XZ ; GS::GetCurrentWindowsPalette
	push	eax
	push	DWORD PTR _wParam$[ebp]
	call	DWORD PTR __imp__SelectPalette@12

; 233  : 					RealizePalette( hdc );

	push	DWORD PTR _wParam$[ebp]
	call	DWORD PTR __imp__RealizePalette@4

; 234  : 					OnDraw( dc );

	mov	eax, DWORD PTR [esi]
	lea	ecx, DWORD PTR _dc$57119[ebp]
	push	ecx
	mov	ecx, esi
	call	DWORD PTR [eax+4]

; 235  : 				}

	lea	ecx, DWORD PTR _dc$57119[ebp]
	call	??1CDrawContext@GS@@UAE@XZ		; GS::CDrawContext::~CDrawContext

; 236  : 
; 237  : 				if( lParam & PRF_CHILDREN )

	test	BYTE PTR _lParam$[ebp], 16		; 00000010H
	je	$L57077

; 238  : 				{
; 239  : 					//
; 240  : 					//	Print our children.
; 241  : 					WinHelper::CRect rcWindow;
; 242  : 					GetWindowRect( m_hwnd, rcWindow );

	mov	edi, DWORD PTR __imp__GetWindowRect@8
	lea	eax, DWORD PTR _rcWindow$57121[ebp]
	push	eax
	push	DWORD PTR [esi+48]
	call	edi

; 243  : 					HWND hwndChild = GetWindow( m_hwnd, GW_CHILD );

	mov	ebx, DWORD PTR __imp__GetWindow@8
	push	5
	push	DWORD PTR [esi+48]
	call	ebx

; 244  : 					
; 245  : 					//
; 246  : 					//	It seems like we need to offset the origin of each child so it draws
; 247  : 					//	in just the right place
; 248  : 					POINT ptOldOrg;
; 249  : 					while( hwndChild )

	test	eax, eax
	mov	DWORD PTR _hwndChild$57122[ebp], eax
	je	$L57077
	mov	esi, DWORD PTR __imp__SetWindowOrgEx@16
$L57125:

; 250  : 					{
; 251  : 						WinHelper::CRect rc;
; 252  : 						GetWindowRect( hwndChild, rc );

	lea	eax, DWORD PTR _rc$57127[ebp]
	push	eax
	push	DWORD PTR _hwndChild$57122[ebp]
	call	edi

; 253  : 						VERIFY( SetWindowOrgEx( hdc, rc.left - rcWindow.left, rc.top - rcWindow.top, &ptOldOrg ) );

	lea	eax, DWORD PTR _ptOldOrg$57123[ebp]
	push	eax
	mov	eax, DWORD PTR _rc$57127[ebp+4]
	sub	eax, DWORD PTR _rcWindow$57121[ebp+4]
	push	eax
	mov	eax, DWORD PTR _rc$57127[ebp]
	sub	eax, DWORD PTR _rcWindow$57121[ebp]
	push	eax
	push	DWORD PTR _wParam$[ebp]
	call	esi

; 254  : 						//TRACE("%d %d %d %d\n", rc.left, rc.top, rc.right, rc.bottom );
; 255  : 
; 256  : 						SendMessage( hwndChild, WM_PRINTCLIENT, wParam, PRF_NONCLIENT | PRF_ERASEBKGND | PRF_CLIENT | PRF_CHILDREN );

	push	30					; 0000001eH
	push	DWORD PTR _wParam$[ebp]
	push	792					; 00000318H
	push	DWORD PTR _hwndChild$57122[ebp]
	call	DWORD PTR __imp__SendMessageA@16

; 257  : 
; 258  : 						VERIFY( SetWindowOrgEx( hdc, ptOldOrg.x, ptOldOrg.y, NULL ) );

	push	0
	push	DWORD PTR _ptOldOrg$57123[ebp+4]
	push	DWORD PTR _ptOldOrg$57123[ebp]
	push	DWORD PTR _wParam$[ebp]
	call	esi

; 259  : 						hwndChild = GetWindow( hwndChild, GW_HWNDNEXT );

	push	2
	push	DWORD PTR _hwndChild$57122[ebp]
	call	ebx
	test	eax, eax
	mov	DWORD PTR _hwndChild$57122[ebp], eax
	jne	SHORT $L57125

; 260  : 					}
; 261  : 				}
; 262  : 			}
; 263  : 		}
; 264  : 		break;

	jmp	$L57077
$L57082:

; 146  : 
; 147  : 	case WM_MOUSEWHEEL:
; 148  : 		{
; 149  : 			CancelHighlight();

	mov	ecx, esi
	call	?CancelHighlight@CGuiBaseWindowObject@GS@@IAEXXZ ; GS::CGuiBaseWindowObject::CancelHighlight

; 150  : 			CancelMouseDown();

	mov	ecx, esi
	call	?CancelMouseDown@CGuiBaseWindowObject@GS@@IAEXXZ ; GS::CGuiBaseWindowObject::CancelMouseDown

; 151  : 
; 152  : 			WinHelper::CPoint pt( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );

	mov	eax, DWORD PTR _lParam$[ebp]
	movsx	ecx, ax
	shr	eax, 16					; 00000010H
	movsx	eax, ax
	mov	DWORD PTR _pt$57090[ebp+4], eax

; 153  : 			PI_int32 nDelta = GET_WHEEL_DELTA_WPARAM( wParam );
; 154  : 			ScreenToClient( GetSafeHwnd(), &pt );

	lea	eax, DWORD PTR _pt$57090[ebp]
	push	eax
	mov	eax, DWORD PTR [esi]
	mov	DWORD PTR _pt$57090[ebp], ecx
	mov	ecx, esi
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__ScreenToClient@8

; 155  : 			CGuiBaseObject *pgbo = FindObjectFromPoint( pt, true );

	lea	eax, DWORD PTR _pt$57090[ebp]
	push	1
	push	eax
	mov	ecx, esi
	call	?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z ; GS::CGuiBaseObject::FindObjectFromPoint

; 156  : 			if( pgbo )

	test	eax, eax
	je	$L57077
	mov	ecx, DWORD PTR _wParam$[ebp]

; 157  : 			{
; 158  : 				pgbo->OnMouseWheel( nDelta );

	mov	edx, DWORD PTR [eax]
	shr	ecx, 16					; 00000010H
	movsx	ecx, cx
	push	ecx
	mov	ecx, eax
	call	DWORD PTR [edx+28]

; 159  : 			}
; 160  : 			else
; 161  : 			{
; 162  : 				
; 163  : 			}
; 164  : 		}
; 165  : 		break;

	jmp	$L57077
$L57106:

; 194  : 
; 195  : 
; 196  : 	case WM_LBUTTONDBLCLK:
; 197  : 		{
; 198  : 			WinHelper::CPoint pt( lParam );

	mov	eax, DWORD PTR _lParam$[ebp]

; 199  : 			CGuiBaseObject *pgbo = FindObjectFromPoint( pt, true );

	push	1
	movsx	ecx, ax
	shr	eax, 16					; 00000010H
	movsx	eax, ax
	mov	DWORD PTR _pt$57107[ebp+4], eax
	lea	eax, DWORD PTR _pt$57107[ebp]
	mov	DWORD PTR _pt$57107[ebp], ecx
	push	eax
	mov	ecx, esi
	call	?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z ; GS::CGuiBaseObject::FindObjectFromPoint

; 200  : 			if( pgbo )

	test	eax, eax
	je	$L57077

; 201  : 			{
; 202  : 				pgbo->OnMouseLeftDoubleclick( pt );

	mov	edx, DWORD PTR [eax]
	lea	ecx, DWORD PTR _pt$57107[ebp]
	push	ecx
	mov	ecx, eax
	call	DWORD PTR [edx+20]

; 203  : 			}
; 204  : 		}
; 205  : 		break;

	jmp	$L57077
$L57101:

; 179  : 
; 180  : 
; 181  : 	case WM_LBUTTONUP:
; 182  : 		if( m_pgboMouseDowned )

	cmp	DWORD PTR [esi+80], 0
	je	$L57077

; 183  : 		{
; 184  : 			WinHelper::CPoint pt( lParam );

	mov	eax, DWORD PTR _lParam$[ebp]

; 185  : 			CGuiBaseObject *pgbo = FindObjectFromPoint( pt, true );

	push	1
	movsx	ecx, ax
	shr	eax, 16					; 00000010H
	movsx	eax, ax
	mov	DWORD PTR _pt$57103[ebp+4], eax
	lea	eax, DWORD PTR _pt$57103[ebp]
	mov	DWORD PTR _pt$57103[ebp], ecx
	push	eax
	mov	ecx, esi
	call	?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z ; GS::CGuiBaseObject::FindObjectFromPoint

; 186  : 			if( pgbo && pgbo == m_pgboMouseDowned )

	test	eax, eax
	je	SHORT $L57105
	cmp	eax, DWORD PTR [esi+80]
	jne	SHORT $L57105

; 187  : 			{
; 188  : 				pgbo->OnMouseLeftUp( pt );

	mov	edx, DWORD PTR [eax]
	lea	ecx, DWORD PTR _pt$57103[ebp]
	push	ecx
	mov	ecx, eax
	call	DWORD PTR [edx+16]
$L57105:

; 189  : 			}
; 190  : 
; 191  : 			m_pgboMouseDowned = NULL;

	and	DWORD PTR [esi+80], 0

; 192  : 		}
; 193  : 		break;

	jmp	$L57077
$L57098:

; 166  : 
; 167  : 	case WM_LBUTTONDOWN:
; 168  : 		{
; 169  : 			CancelMouseDown();

	mov	ecx, esi
	call	?CancelMouseDown@CGuiBaseWindowObject@GS@@IAEXXZ ; GS::CGuiBaseWindowObject::CancelMouseDown

; 170  : 			WinHelper::CPoint pt( lParam );

	mov	eax, DWORD PTR _lParam$[ebp]

; 171  : 
; 172  : 			m_pgboMouseDowned = FindObjectFromPoint( pt, true );

	push	1
	movsx	ecx, ax
	shr	eax, 16					; 00000010H
	movsx	eax, ax
	mov	DWORD PTR _pt$57099[ebp+4], eax
	lea	eax, DWORD PTR _pt$57099[ebp]
	mov	DWORD PTR _pt$57099[ebp], ecx
	push	eax
	mov	ecx, esi
	call	?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z ; GS::CGuiBaseObject::FindObjectFromPoint

; 173  : 			if( m_pgboMouseDowned )

	test	eax, eax
	mov	DWORD PTR [esi+80], eax
	je	SHORT $L57077

; 174  : 			{
; 175  : 				m_pgboMouseDowned->OnMouseLeftDown( pt );

	mov	edx, DWORD PTR [eax]
	lea	ecx, DWORD PTR _pt$57099[ebp]
	push	ecx
	mov	ecx, eax
	call	DWORD PTR [edx+12]

; 176  : 			}
; 177  : 		}
; 178  : 		break;

	jmp	SHORT $L57077
$L57064:

; 104  : 
; 105  : 
; 106  : 	case WM_MOUSEMOVE:
; 107  : 		{
; 108  : 			WinHelper::CPoint pt( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );

	mov	eax, DWORD PTR _lParam$[ebp]
	movsx	ecx, ax
	shr	eax, 16					; 00000010H

; 109  : 			if( !m_timerEnterLeave.IsSet() )

	cmp	DWORD PTR [esi+72], 0
	mov	DWORD PTR _pt$57072[ebp], ecx
	movsx	eax, ax
	lea	ecx, DWORD PTR [esi+72]
	mov	DWORD PTR _pt$57072[ebp+4], eax
	jne	SHORT $L57073

; 110  : 			{
; 111  : 				m_timerEnterLeave.Start( 50 );

	push	50					; 00000032H
	call	?Start@CTimer@CGuiBaseObject@GS@@QAEXI@Z ; GS::CGuiBaseObject::CTimer::Start
$L57073:

; 112  : 			}
; 113  : 
; 114  : 			CGuiBaseObject *pgbo = FindObjectFromPoint( pt, true );

	lea	eax, DWORD PTR _pt$57072[ebp]
	push	1
	push	eax
	mov	ecx, esi
	call	?FindObjectFromPoint@CGuiBaseObject@GS@@QBEPAV12@ABVCPoint@WinHelper@@_N@Z ; GS::CGuiBaseObject::FindObjectFromPoint

; 115  : 			if( pgbo != m_pgboHighlighted )

	mov	ecx, DWORD PTR [esi+84]
	mov	edi, eax
	cmp	edi, ecx
	je	SHORT $L57077

; 116  : 			{
; 117  : 				if( m_pgboHighlighted )

	test	ecx, ecx
	je	SHORT $L57076

; 118  : 				{
; 119  : 					m_pgboHighlighted->OnMouseLeave();

	mov	eax, DWORD PTR [ecx]
	call	DWORD PTR [eax+40]
$L57076:

; 120  : 				}
; 121  : 				m_pgboHighlighted = pgbo;
; 122  : 
; 123  : 				if( m_pgboHighlighted )

	test	edi, edi
	mov	DWORD PTR [esi+84], edi
	je	SHORT $L57077

; 124  : 				{
; 125  : 					m_pgboHighlighted->OnMouseEnter();

	mov	eax, DWORD PTR [edi]
	mov	ecx, edi
	call	DWORD PTR [eax+44]
$L57077:

; 276  : 	}
; 277  : 	return 0;

	xor	eax, eax
$L57040:
	pop	edi
	pop	esi
	pop	ebx

; 278  : }

	leave
	ret	12					; 0000000cH
?OnWindowsMessage@CGuiBaseWindowObject@GS@@UAEJIIJ@Z ENDP ; GS::CGuiBaseWindowObject::OnWindowsMessage
_TEXT	ENDS
;	COMDAT ??0CRect@WinHelper@@QAE@ABUtagRECT@@@Z
_TEXT	SEGMENT
_rhs$ = 8
??0CRect@WinHelper@@QAE@ABUtagRECT@@@Z PROC NEAR	; WinHelper::CRect::CRect, COMDAT

; 74   : 		inline explicit CRect( const RECT& rhs ) { Set( rhs.left, rhs.top, rhs.right, rhs.bottom );}

	mov	eax, ecx
	mov	ecx, DWORD PTR _rhs$[esp-4]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR [eax+4], edx
	mov	edx, DWORD PTR [ecx+12]
	mov	DWORD PTR [eax+12], edx
	mov	edx, DWORD PTR [ecx+8]
	mov	ecx, DWORD PTR [ecx]
	mov	DWORD PTR [eax+8], edx
	mov	DWORD PTR [eax], ecx
	ret	4
??0CRect@WinHelper@@QAE@ABUtagRECT@@@Z ENDP		; WinHelper::CRect::CRect
_TEXT	ENDS
PUBLIC	?Size@CRect@WinHelper@@QBE?AVCSize@2@XZ		; WinHelper::CRect::Size
PUBLIC	??_7CBufferedDC@GS@@6B@				; GS::CBufferedDC::`vftable'
PUBLIC	??_GCBufferedDC@GS@@UAEPAXI@Z			; GS::CBufferedDC::`scalar deleting destructor'
PUBLIC	??_ECBufferedDC@GS@@UAEPAXI@Z			; GS::CBufferedDC::`vector deleting destructor'
EXTRN	__imp__SelectObject@8:NEAR
EXTRN	__imp__CreateCompatibleBitmap@12:NEAR
EXTRN	__imp__CreateCompatibleDC@4:NEAR
;	COMDAT ??_7CBufferedDC@GS@@6B@
; File ../guitools/DrawContext.inl
CONST	SEGMENT
??_7CBufferedDC@GS@@6B@ DD FLAT:??_ECBufferedDC@GS@@UAEPAXI@Z ; GS::CBufferedDC::`vftable'
CONST	ENDS
;	COMDAT ??0CBufferedDC@GS@@QAE@AAVCDrawContext@1@@Z
_TEXT	SEGMENT
_dc$ = 8
??0CBufferedDC@GS@@QAE@AAVCDrawContext@1@@Z PROC NEAR	; GS::CBufferedDC::CBufferedDC, COMDAT

; 221  : 		{

	push	ebx
	push	ebp
	mov	ebp, DWORD PTR _dc$[esp+4]
	push	esi
	push	edi
	push	0
	mov	eax, DWORD PTR [ebp+8]
	mov	ebx, ecx
	push	eax
	lea	esi, DWORD PTR [ebp+12]
	call	DWORD PTR __imp__CreateCompatibleDC@4
	push	eax
	push	esi
	mov	ecx, ebx
	call	??0CDrawContext@GS@@QAE@PBVCRect@WinHelper@@PAUHDC__@@_N@Z ; GS::CDrawContext::CDrawContext
	lea	edi, DWORD PTR [ebx+88]
	lea	ecx, DWORD PTR [ebp+12]
	movsd
	movsd
	movsd
	movsd
	lea	esi, DWORD PTR [ebx+104]
	mov	DWORD PTR [ebx+84], ebp
	push	esi
	call	?Size@CRect@WinHelper@@QBE?AVCSize@2@XZ	; WinHelper::CRect::Size
	push	DWORD PTR [ebx+108]
	mov	ebp, DWORD PTR [ebp+8]
	push	DWORD PTR [esi]
	push	ebp
	call	DWORD PTR __imp__CreateCompatibleBitmap@12

; 222  : 			m_hbm = ::SelectObject( GetSafeHdc(), m_hbm );

	mov	ecx, DWORD PTR [ebx+8]
	push	eax
	push	ecx
	mov	DWORD PTR [ebx+112], eax
	mov	DWORD PTR [ebx], OFFSET FLAT:??_7CBufferedDC@GS@@6B@ ; GS::CBufferedDC::`vftable'
	call	DWORD PTR __imp__SelectObject@8
	mov	DWORD PTR [ebx+112], eax

; 223  : 			VERIFY( SetWindowOrgEx( GetSafeHdc(), rcClip.left, rcClip.top, &m_ptOldOrg ) );

	lea	eax, DWORD PTR [ebx+76]
	push	eax
	push	DWORD PTR [ebx+92]
	push	DWORD PTR [ebx+88]
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__SetWindowOrgEx@16

; 224  : 
; 225  : 			(void)SelectPalette( GetSafeHdc(), GetCurrentWindowsPalette(), TRUE );

	mov	esi, DWORD PTR [ebx+8]
	push	1
	call	?GetCurrentWindowsPalette@GS@@YAPAUHPALETTE__@@XZ ; GS::GetCurrentWindowsPalette
	push	eax
	push	esi
	call	DWORD PTR __imp__SelectPalette@12

; 226  : 			(void)RealizePalette( GetSafeHdc() );

	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__RealizePalette@4
	pop	edi
	pop	esi

; 227  : 		}

	mov	eax, ebx
	pop	ebp
	pop	ebx
	ret	4
??0CBufferedDC@GS@@QAE@AAVCDrawContext@1@@Z ENDP	; GS::CBufferedDC::CBufferedDC
_TEXT	ENDS
;	COMDAT ?Size@CRect@WinHelper@@QBE?AVCSize@2@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
?Size@CRect@WinHelper@@QBE?AVCSize@2@XZ PROC NEAR	; WinHelper::CRect::Size, COMDAT

; 89   : 			{ CSize s( Width(), Height() ); return s; }

	mov	edx, DWORD PTR [ecx+8]
	mov	eax, DWORD PTR ___$ReturnUdt$[esp-4]
	sub	edx, DWORD PTR [ecx]
	push	esi
	mov	esi, DWORD PTR [ecx+12]
	sub	esi, DWORD PTR [ecx+4]
	mov	DWORD PTR [eax], edx
	mov	DWORD PTR [eax+4], esi
	pop	esi
	ret	4
?Size@CRect@WinHelper@@QBE?AVCSize@2@XZ ENDP		; WinHelper::CRect::Size
_TEXT	ENDS
EXTRN	__imp__RestoreDC@8:NEAR
EXTRN	__imp__BitBlt@36:NEAR
EXTRN	__imp__DeleteDC@4:NEAR
EXTRN	__imp__DeleteObject@4:NEAR
;	COMDAT ??1CBufferedDC@GS@@UAE@XZ
_TEXT	SEGMENT
??1CBufferedDC@GS@@UAE@XZ PROC NEAR			; GS::CBufferedDC::~CBufferedDC, COMDAT

; 230  : 		{

	push	esi
	push	edi

; 231  : 			VERIFY( SetWindowOrgEx( GetSafeHdc(), m_ptOldOrg.x, m_ptOldOrg.y, NULL ) );

	xor	edi, edi
	mov	esi, ecx
	push	edi
	push	DWORD PTR [esi+80]
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CBufferedDC@GS@@6B@ ; GS::CBufferedDC::`vftable'
	push	DWORD PTR [esi+76]
	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__SetWindowOrgEx@16

; 232  : 
; 233  : 			::BitBlt( m_dc.GetSafeHdc(), rcClip.left,  rcClip.top, m_sizeClip.cx, m_sizeClip.cy, GetSafeHdc(), 0, 0, SRCCOPY );

	push	13369376				; 00cc0020H
	push	edi
	push	edi
	mov	eax, DWORD PTR [esi+84]
	push	DWORD PTR [esi+8]
	push	DWORD PTR [esi+108]
	push	DWORD PTR [esi+104]
	push	DWORD PTR [esi+92]
	push	DWORD PTR [esi+88]
	push	DWORD PTR [eax+8]
	call	DWORD PTR __imp__BitBlt@36

; 234  : 			VAPI( ::DeleteObject( ::SelectObject( GetSafeHdc(), m_hbm ) ) );

	push	DWORD PTR [esi+112]
	mov	eax, DWORD PTR [esi+8]
	push	eax
	call	DWORD PTR __imp__SelectObject@8
	push	eax
	call	DWORD PTR __imp__DeleteObject@4

; 235  : 
; 236  : 
; 237  : 			VAPI( ::RestoreDC( m_hdc, -1 ) );

	push	-1
	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__RestoreDC@8

; 238  : 			::DeleteDC( m_hdc );

	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__DeleteDC@4

; 239  : 			m_hdc = NULL;
; 240  : 			m_hbm = NULL;
; 241  : 
; 242  : 
; 243  : 
; 244  : 		}

	mov	ecx, esi
	mov	DWORD PTR [esi+8], edi
	mov	DWORD PTR [esi+112], edi
	call	??1CDrawContext@GS@@UAE@XZ		; GS::CDrawContext::~CDrawContext
	pop	edi
	pop	esi
	ret	0
??1CBufferedDC@GS@@UAE@XZ ENDP				; GS::CBufferedDC::~CBufferedDC
_TEXT	ENDS
;	COMDAT ??_GCBufferedDC@GS@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_GCBufferedDC@GS@@UAEPAXI@Z PROC NEAR			; GS::CBufferedDC::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1CBufferedDC@GS@@UAE@XZ		; GS::CBufferedDC::~CBufferedDC
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L56434
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L56434:
	mov	eax, esi
	pop	esi
	ret	4
??_GCBufferedDC@GS@@UAEPAXI@Z ENDP			; GS::CBufferedDC::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?Start@CTimer@CGuiBaseObject@GS@@QAEXI@Z
_TEXT	SEGMENT
_uDuration$ = 8
?Start@CTimer@CGuiBaseObject@GS@@QAEXI@Z PROC NEAR	; GS::CGuiBaseObject::CTimer::Start, COMDAT

; 104  : 			{

	push	esi
	mov	esi, ecx

; 105  : 				Stop();

	call	?Stop@CTimer@CGuiBaseObject@GS@@QAEXXZ	; GS::CGuiBaseObject::CTimer::Stop

; 106  : 				m_uTimer = m_pOwner->StartTimer( m_pOwner, uDuration );

	mov	ecx, DWORD PTR [esi+4]
	push	DWORD PTR _uDuration$[esp]
	mov	eax, DWORD PTR [ecx]
	push	ecx
	call	DWORD PTR [eax+60]
	mov	DWORD PTR [esi], eax
	pop	esi

; 107  : 				ASSERT( m_uTimer );
; 108  : 			}

	ret	4
?Start@CTimer@CGuiBaseObject@GS@@QAEXI@Z ENDP		; GS::CGuiBaseObject::CTimer::Start
_TEXT	ENDS
_TEXT	SEGMENT
?CancelMouseDown@CGuiBaseWindowObject@GS@@IAEXXZ PROC NEAR ; GS::CGuiBaseWindowObject::CancelMouseDown

; 283  : 	if( m_pgboMouseDowned )

	mov	ecx, DWORD PTR [ecx+80]
	test	ecx, ecx
	je	SHORT $L57135

; 284  : 	{
; 285  : 		m_pgboMouseDowned->OnMouseCancelLeftMouse();

	mov	eax, DWORD PTR [ecx]
	jmp	DWORD PTR [eax+24]
$L57135:

; 286  : 	}
; 287  : }

	ret	0
?CancelMouseDown@CGuiBaseWindowObject@GS@@IAEXXZ ENDP	; GS::CGuiBaseWindowObject::CancelMouseDown
_TEXT	ENDS
EXTRN	__imp__WindowFromPoint@8:NEAR
_TEXT	SEGMENT
_uTimerID$ = 8
_pt$57141 = -8
?OnTimer@CGuiBaseWindowObject@GS@@MAEXI@Z PROC NEAR	; GS::CGuiBaseWindowObject::OnTimer

; 291  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ecx
	push	esi
	mov	esi, ecx
	push	edi

; 292  : 	if( m_timerEnterLeave == uTimerID )

	mov	eax, DWORD PTR [esi+72]
	lea	edi, DWORD PTR [esi+72]
	cmp	eax, DWORD PTR _uTimerID$[ebp]
	jne	SHORT $L57145

; 293  : 	{
; 294  : 		WinHelper::CPoint pt;
; 295  : 		GetCursorPos( pt );

	lea	eax, DWORD PTR _pt$57141[ebp]
	push	ebx
	push	eax
	call	DWORD PTR __imp__GetCursorPos@4

; 296  : 		HWND hwndCursor = WindowFromPoint( pt );

	push	DWORD PTR _pt$57141[ebp+4]
	push	DWORD PTR _pt$57141[ebp]
	call	DWORD PTR __imp__WindowFromPoint@8
	mov	ebx, eax

; 297  : 		if( hwndCursor != GetSafeHwnd() )

	mov	eax, DWORD PTR [esi]
	mov	ecx, esi
	call	DWORD PTR [eax+52]
	cmp	ebx, eax
	pop	ebx
	je	SHORT $L57145

; 298  : 		{
; 299  : 			m_timerEnterLeave.Stop();

	mov	ecx, edi
	call	?Stop@CTimer@CGuiBaseObject@GS@@QAEXXZ	; GS::CGuiBaseObject::CTimer::Stop

; 300  : 
; 301  : 			if( m_pgboHighlighted )

	mov	ecx, DWORD PTR [esi+84]
	test	ecx, ecx
	je	SHORT $L57145

; 302  : 			{
; 303  : 				m_pgboHighlighted->OnMouseLeave();

	mov	eax, DWORD PTR [ecx]
	call	DWORD PTR [eax+40]

; 304  : 				m_pgboHighlighted = NULL;

	and	DWORD PTR [esi+84], 0
$L57145:
	pop	edi
	pop	esi

; 305  : 			}
; 306  : 		}	
; 307  : 	}
; 308  : }

	leave
	ret	4
?OnTimer@CGuiBaseWindowObject@GS@@MAEXI@Z ENDP		; GS::CGuiBaseWindowObject::OnTimer
_TEXT	ENDS
PUBLIC	?SetAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABHPAVCGuiBaseObject@GS@@@Z ; Container::CMap<int,GS::CGuiBaseObject *>::SetAt
EXTRN	__imp__SetTimer@16:NEAR
_TEXT	SEGMENT
$T57713 = 12
_pgbo$ = 8
_nInterval$ = 12
?StartTimer@CGuiBaseWindowObject@GS@@UAEIPAVCGuiBaseObject@2@I@Z PROC NEAR ; GS::CGuiBaseWindowObject::StartTimer

; 312  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 313  : 	ASSERT_VALID_HWND( GetSafeHwnd() );
; 314  : 	ASSERT_VALID_WRITEOBJPTR( pgbo );
; 315  : 	ASSERT( nInterval > 0 );
; 316  : 
; 317  : 	VAPI( ::SetTimer( GetSafeHwnd(), m_uNextTimerID, nInterval, 0 ) );

	push	0
	push	DWORD PTR _nInterval$[ebp]
	mov	esi, ecx
	push	DWORD PTR [esi+68]
	mov	eax, DWORD PTR [esi]
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__SetTimer@16

; 318  : 	m_mapTimerEvents.SetAt( m_uNextTimerID, pgbo );

	mov	eax, DWORD PTR [esi+68]
	push	DWORD PTR _pgbo$[ebp]
	mov	DWORD PTR $T57713[ebp], eax
	lea	eax, DWORD PTR $T57713[ebp]
	push	eax
	lea	ecx, DWORD PTR [esi+52]
	call	?SetAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABHPAVCGuiBaseObject@GS@@@Z ; Container::CMap<int,GS::CGuiBaseObject *>::SetAt

; 319  : 
; 320  : 	//TRACE( _T("Started timer %d\n"), m_uNextTimerID );
; 321  : 	return m_uNextTimerID++;

	mov	eax, DWORD PTR [esi+68]
	lea	ecx, DWORD PTR [eax+1]
	mov	DWORD PTR [esi+68], ecx
	pop	esi

; 322  : }

	pop	ebp
	ret	8
?StartTimer@CGuiBaseWindowObject@GS@@UAEIPAVCGuiBaseObject@2@I@Z ENDP ; GS::CGuiBaseWindowObject::StartTimer
_TEXT	ENDS
PUBLIC	?RemoveAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABH@Z ; Container::CMap<int,GS::CGuiBaseObject *>::RemoveAt
_TEXT	SEGMENT
$T57715 = 8
$T57716 = 8
_uTimerID$ = 8
?StopTimer@CGuiBaseWindowObject@GS@@UAEXI@Z PROC NEAR	; GS::CGuiBaseWindowObject::StopTimer

; 326  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	mov	esi, ecx
	push	edi

; 327  : 	//TRACE( _T("Removed timer id %d\n"), nTimerEventID );
; 328  : 	ASSERT( uTimerID  );
; 329  : 	if( m_mapTimerEvents.Lookup( uTimerID ) )

	mov	edi, DWORD PTR _uTimerID$[ebp]
	lea	eax, DWORD PTR $T57715[ebp]
	lea	ebx, DWORD PTR [esi+52]
	push	eax
	mov	ecx, ebx
	mov	DWORD PTR $T57715[ebp], edi
	call	?Lookup@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEPAPAVCGuiBaseObject@GS@@ABH@Z ; Container::CMap<int,GS::CGuiBaseObject *>::Lookup
	test	eax, eax
	je	SHORT $L57162

; 330  : 	{
; 331  : 		(void)::KillTimer( GetSafeHwnd(), uTimerID );

	mov	eax, DWORD PTR [esi]
	push	edi
	mov	ecx, esi
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__KillTimer@8

; 332  : 		m_mapTimerEvents.RemoveAt( uTimerID );

	lea	eax, DWORD PTR $T57716[ebp]
	mov	ecx, ebx
	push	eax
	mov	DWORD PTR $T57716[ebp], edi
	call	?RemoveAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABH@Z ; Container::CMap<int,GS::CGuiBaseObject *>::RemoveAt
$L57162:
	pop	edi
	pop	esi
	pop	ebx

; 333  : 		//TRACE( _T("Stopped timer %d\n"), uTimerID );
; 334  : 	}
; 335  : 	else
; 336  : 	{
; 337  : 		//	Trying to unregister an event twice
; 338  : 		//ASSERT( FALSE );
; 339  : 	}
; 340  : }

	pop	ebp
	ret	4
?StopTimer@CGuiBaseWindowObject@GS@@UAEXI@Z ENDP	; GS::CGuiBaseWindowObject::StopTimer
_rc$ = 8
?ForceRedraw@CGuiBaseWindowObject@GS@@UAEXABVCRect@WinHelper@@@Z PROC NEAR ; GS::CGuiBaseWindowObject::ForceRedraw

; 344  : 	InvalidateRect( GetSafeHwnd(), rc, TRUE );

	mov	eax, DWORD PTR [ecx]
	push	1
	push	DWORD PTR _rc$[esp]
	call	DWORD PTR [eax+52]
	push	eax
	call	DWORD PTR __imp__InvalidateRect@12

; 345  : }

	ret	4
?ForceRedraw@CGuiBaseWindowObject@GS@@UAEXABVCRect@WinHelper@@@Z ENDP ; GS::CGuiBaseWindowObject::ForceRedraw
?OnCreateWindow@CGuiBaseWindowObject@GS@@UAE_NXZ PROC NEAR ; GS::CGuiBaseWindowObject::OnCreateWindow

; 350  : 	return true;

	mov	al, 1

; 351  : }

	ret	0
?OnCreateWindow@CGuiBaseWindowObject@GS@@UAE_NXZ ENDP	; GS::CGuiBaseWindowObject::OnCreateWindow
?CancelHighlight@CGuiBaseWindowObject@GS@@IAEXXZ PROC NEAR ; GS::CGuiBaseWindowObject::CancelHighlight

; 355  : {

	push	esi
	mov	esi, ecx

; 356  : 	if( m_pgboHighlighted )

	mov	ecx, DWORD PTR [esi+84]
	test	ecx, ecx
	je	SHORT $L57176

; 357  : 	{
; 358  : 		m_pgboHighlighted->OnMouseLeave();

	mov	eax, DWORD PTR [ecx]
	call	DWORD PTR [eax+40]
$L57176:

; 359  : 	}
; 360  : 	m_pgboHighlighted = NULL;

	and	DWORD PTR [esi+84], 0
	pop	esi

; 361  : }

	ret	0
?CancelHighlight@CGuiBaseWindowObject@GS@@IAEXXZ ENDP	; GS::CGuiBaseWindowObject::CancelHighlight
?OnCloseWindow@CGuiBaseWindowObject@GS@@UAEXXZ PROC NEAR ; GS::CGuiBaseWindowObject::OnCloseWindow

; 366  : 	
; 367  : }

	ret	0
?OnCloseWindow@CGuiBaseWindowObject@GS@@UAEXXZ ENDP	; GS::CGuiBaseWindowObject::OnCloseWindow
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXXZ ; Container::CMap<int,GS::CGuiBaseObject *>::RemoveAll
;	COMDAT ??1?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAE@XZ
_TEXT	SEGMENT
??1?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAE@XZ PROC NEAR ; Container::CMap<int,GS::CGuiBaseObject *>::~CMap<int,GS::CGuiBaseObject *>, COMDAT

; 21   : {

	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CMap@HPAVCGuiBaseObject@GS@@@Container@@6B@ ; Container::CMap<int,GS::CGuiBaseObject *>::`vftable'

; 22   : 	RemoveAll();

	call	?RemoveAll@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXXZ ; Container::CMap<int,GS::CGuiBaseObject *>::RemoveAll

; 23   : 	delete [] m_pEntrys;

	push	DWORD PTR [esi+8]
	call	??3@YAXPAX@Z				; operator delete

; 24   : 	m_pEntrys = NULL;

	and	DWORD PTR [esi+8], 0
	pop	ecx
	pop	esi

; 25   : }

	ret	0
??1?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAE@XZ ENDP ; Container::CMap<int,GS::CGuiBaseObject *>::~CMap<int,GS::CGuiBaseObject *>
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEPAPAVCGuiBaseObject@GS@@ABH@Z
_TEXT	SEGMENT
_key$ = 8
?Lookup@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEPAPAVCGuiBaseObject@GS@@ABH@Z PROC NEAR ; Container::CMap<int,GS::CGuiBaseObject *>::Lookup, COMDAT

; 111  : {

	push	esi

; 112  : 	PI_uint32 uHash = HashIt( key ) % m_uBucketArrayLength;
; 113  : 	MapBucketEntry<Key, Value>	*pEntry = Find( uHash, key );

	mov	esi, DWORD PTR _key$[esp]
	xor	edx, edx
	movzx	eax, WORD PTR [esi]
	shr	eax, 4
	div	DWORD PTR [ecx+4]
	mov	eax, DWORD PTR [ecx+8]
	mov	edx, DWORD PTR [eax+edx*4]
	test	edx, edx
	je	SHORT $L57759
	mov	esi, DWORD PTR [esi]
$L57757:
	cmp	DWORD PTR [edx+4], esi
	je	SHORT $L57762
	mov	edx, DWORD PTR [edx+8]
	test	edx, edx
	jne	SHORT $L57757
$L57759:
	xor	eax, eax
$L57755:
	pop	esi

; 114  : 	return pEntry != NULL ? &pEntry -> m_Value : NULL;
; 115  : }

	ret	4
$L57762:

; 112  : 	PI_uint32 uHash = HashIt( key ) % m_uBucketArrayLength;
; 113  : 	MapBucketEntry<Key, Value>	*pEntry = Find( uHash, key );

	mov	eax, edx
	jmp	SHORT $L57755
?Lookup@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEPAPAVCGuiBaseObject@GS@@ABH@Z ENDP ; Container::CMap<int,GS::CGuiBaseObject *>::Lookup
_TEXT	ENDS
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
;	COMDAT ?SetAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABHPAVCGuiBaseObject@GS@@@Z
_TEXT	SEGMENT
_key$ = 8
_value$ = 12
?SetAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABHPAVCGuiBaseObject@GS@@@Z PROC NEAR ; Container::CMap<int,GS::CGuiBaseObject *>::SetAt, COMDAT

; 188  : {

	push	ebx

; 189  : 	PI_uint32 uHash = HashIt( key ) % m_uBucketArrayLength;

	mov	ebx, DWORD PTR _key$[esp]
	push	esi
	mov	esi, ecx
	movzx	eax, WORD PTR [ebx]
	shr	eax, 4
	xor	edx, edx
	push	edi
	div	DWORD PTR [esi+4]

; 190  : 	MapBucketEntry<Key, Value>	*pEntry = Find( uHash , key);

	mov	eax, DWORD PTR [esi+8]
	mov	edi, edx
	shl	edi, 2
	mov	eax, DWORD PTR [eax+edi]
	test	eax, eax
	je	SHORT $L57783
	mov	ecx, DWORD PTR [ebx]
$L57781:
	cmp	DWORD PTR [eax+4], ecx
	je	SHORT $L57779
	mov	eax, DWORD PTR [eax+8]
	test	eax, eax
	jne	SHORT $L57781
$L57783:
	xor	eax, eax
$L57779:

; 191  : 
; 192  : 	if( pEntry )

	test	eax, eax
	je	SHORT $L57226

; 193  : 	{
; 194  : 		pEntry -> m_Value = value;

	mov	ecx, DWORD PTR _value$[esp+8]
	mov	DWORD PTR [eax], ecx

; 195  : 	}
; 196  : 	else

	jmp	SHORT $L57233
$L57226:

; 197  : 	{
; 198  : 		MapBucketEntry<Key, Value>	*pEntry = new MapBucketEntry<Key, Value>;

	push	12					; 0000000cH
	call	??2@YAPAXI@Z				; operator new
	test	eax, eax
	pop	ecx
	je	SHORT $L57767
	and	DWORD PTR [eax+8], 0
	jmp	SHORT $L57768
$L57767:
	xor	eax, eax
$L57768:

; 199  : 		m_uCount++;
; 200  : 		pEntry -> m_Value = value;

	mov	ecx, DWORD PTR _value$[esp+8]
	inc	DWORD PTR [esi+12]
	mov	DWORD PTR [eax], ecx

; 201  : 		pEntry -> m_Key = key;

	mov	ecx, DWORD PTR [ebx]
	mov	DWORD PTR [eax+4], ecx

; 202  : 
; 203  : 		MapBucketEntry<Key, Value>	*pRootEntry = m_pEntrys[uHash];

	mov	ecx, DWORD PTR [esi+8]
	add	ecx, edi
	mov	edx, DWORD PTR [ecx]

; 204  : 		m_pEntrys[uHash] = pEntry;

	mov	DWORD PTR [ecx], eax

; 205  : 		if( pRootEntry )

	test	edx, edx
	je	SHORT $L57232

; 206  : 		{
; 207  : 			pEntry -> m_pNext = pRootEntry;

	mov	DWORD PTR [eax+8], edx

; 208  : 		}
; 209  : 		else

	jmp	SHORT $L57233
$L57232:

; 210  : 		{
; 211  : 			pEntry -> m_pNext = NULL;

	and	DWORD PTR [eax+8], 0
$L57233:
	pop	edi
	pop	esi
	pop	ebx

; 212  : 		}
; 213  : 	}
; 214  : }

	ret	8
?SetAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABHPAVCGuiBaseObject@GS@@@Z ENDP ; Container::CMap<int,GS::CGuiBaseObject *>::SetAt
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABH@Z
_TEXT	SEGMENT
_key$ = 8
?RemoveAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABH@Z PROC NEAR ; Container::CMap<int,GS::CGuiBaseObject *>::RemoveAt, COMDAT

; 218  : {

	push	ebx
	push	esi
	push	edi

; 219  : 	PI_uint32 uHash = HashIt( key ) % m_uBucketArrayLength;

	mov	edi, DWORD PTR _key$[esp+8]
	mov	esi, ecx
	xor	edx, edx
	movzx	eax, WORD PTR [edi]
	shr	eax, 4
	div	DWORD PTR [esi+4]

; 220  : 	MapBucketEntry<Key, Value> *pLastItem = NULL;
; 221  : 	for( MapBucketEntry<Key, Value> *pEntry = m_pEntrys[uHash]; pEntry != NULL; pLastItem = pEntry, pEntry = pEntry -> m_pNext )

	mov	eax, DWORD PTR [esi+8]
	xor	ebx, ebx
	lea	ecx, DWORD PTR [eax+edx*4]
	mov	eax, DWORD PTR [eax+edx*4]
	test	eax, eax
	je	SHORT $L57246

; 222  : 	{
; 223  : 		if( ElementsTheSame( pEntry -> m_Key, key ) )

	mov	edi, DWORD PTR [edi]
$L57244:
	cmp	DWORD PTR [eax+4], edi
	je	SHORT $L57801
	mov	ebx, eax
	mov	eax, DWORD PTR [eax+8]
	test	eax, eax
	jne	SHORT $L57244
	jmp	SHORT $L57246
$L57801:

; 224  : 		{
; 225  : 			if( pLastItem )

	test	ebx, ebx
	je	SHORT $L57253

; 226  : 			{
; 227  : 				pLastItem -> m_pNext = pEntry -> m_pNext;

	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR [ebx+8], ecx

; 228  : 			}
; 229  : 			else

	jmp	SHORT $L57254
$L57253:

; 230  : 			{
; 231  : 				m_pEntrys[uHash] = pEntry -> m_pNext;

	mov	edx, DWORD PTR [eax+8]
	mov	DWORD PTR [ecx], edx
$L57254:

; 232  : 			}
; 233  : 			delete pEntry;

	push	eax
	call	??3@YAXPAX@Z				; operator delete

; 234  : 			m_uCount--;

	dec	DWORD PTR [esi+12]
	pop	ecx
$L57246:
	pop	edi
	pop	esi
	pop	ebx

; 235  : 			break;
; 236  : 		}
; 237  : 	}
; 238  : }

	ret	4
?RemoveAt@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXABH@Z ENDP ; Container::CMap<int,GS::CGuiBaseObject *>::RemoveAt
_TEXT	ENDS
;	COMDAT ??_G?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z PROC NEAR ; Container::CMap<int,GS::CGuiBaseObject *>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAE@XZ ; Container::CMap<int,GS::CGuiBaseObject *>::~CMap<int,GS::CGuiBaseObject *>
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L57267
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L57267:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CMap@HPAVCGuiBaseObject@GS@@@Container@@UAEPAXI@Z ENDP ; Container::CMap<int,GS::CGuiBaseObject *>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?SetBucketSize@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXI@Z
_TEXT	SEGMENT
_uSize$ = 8
?SetBucketSize@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXI@Z PROC NEAR ; Container::CMap<int,GS::CGuiBaseObject *>::SetBucketSize, COMDAT

; 30   : {

	push	esi
	mov	esi, ecx
	push	edi

; 31   : 	ASSERT( !GetSize() ); //Cannot be called unless map is empty (possible improvement would be to allow this).
; 32   : 	if( !GetSize() )

	cmp	DWORD PTR [esi+12], 0
	jne	SHORT $L57295

; 33   : 	{
; 34   : 		delete [] m_pEntrys;

	push	DWORD PTR [esi+8]
	call	??3@YAXPAX@Z				; operator delete

; 35   : 		m_pEntrys = NULL;

	and	DWORD PTR [esi+8], 0
	pop	ecx
$L57295:

; 36   : 	}
; 37   : 
; 38   : 	m_uBucketArrayLength = uSize;

	mov	eax, DWORD PTR _uSize$[esp+4]
	mov	DWORD PTR [esi+4], eax

; 39   : 	m_pEntrys = new MapBucketEntry<Key, Value>*[m_uBucketArrayLength];

	shl	eax, 2
	push	eax
	call	??2@YAPAXI@Z				; operator new
	pop	ecx
	mov	edi, eax

; 40   : 
; 41   : 	for( PI_uint32 i = 0; i < m_uBucketArrayLength; i++ )

	mov	ecx, DWORD PTR [esi+4]
	mov	DWORD PTR [esi+8], edi
	test	ecx, ecx
	jbe	SHORT $L57300
	xor	eax, eax
	rep stosd
$L57300:
	pop	edi
	pop	esi

; 42   : 	{
; 43   : 		m_pEntrys[i] = NULL;
; 44   : 	} 
; 45   : }

	ret	4
?SetBucketSize@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXI@Z ENDP ; Container::CMap<int,GS::CGuiBaseObject *>::SetBucketSize
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXXZ PROC NEAR ; Container::CMap<int,GS::CGuiBaseObject *>::RemoveAll, COMDAT

; 57   : {

	push	esi
	mov	esi, ecx

; 58   : 	if( m_uCount )

	cmp	DWORD PTR [esi+12], 0
	je	SHORT $L57306
	push	edi

; 59   : 	{
; 60   : 		for( PI_uint32 i = 0; i < m_uBucketArrayLength; i++ )

	xor	edi, edi
	cmp	DWORD PTR [esi+4], edi
	jbe	SHORT $L57310
	push	ebx
$L57308:

; 61   : 		{
; 62   : 			MapBucketEntry<Key, Value> *pEntry = m_pEntrys[i];

	mov	eax, DWORD PTR [esi+8]
	mov	eax, DWORD PTR [eax+edi*4]

; 63   : 			while( pEntry )

	test	eax, eax
	je	SHORT $L57314
$L57313:

; 64   : 			{
; 65   : 				MapBucketEntry<Key, Value> *pEntryNext = pEntry -> m_pNext;

	mov	ebx, DWORD PTR [eax+8]

; 66   : 				delete pEntry;

	push	eax
	call	??3@YAXPAX@Z				; operator delete
	test	ebx, ebx
	pop	ecx

; 67   : 				pEntry = pEntryNext;

	mov	eax, ebx
	jne	SHORT $L57313
$L57314:

; 68   : 			}
; 69   : 
; 70   : 			m_pEntrys[i] = NULL;

	mov	eax, DWORD PTR [esi+8]
	and	DWORD PTR [eax+edi*4], 0
	inc	edi
	cmp	edi, DWORD PTR [esi+4]
	jb	SHORT $L57308
	pop	ebx
$L57310:

; 71   : 		}
; 72   : 
; 73   : 		m_uCount = 0;

	and	DWORD PTR [esi+12], 0
	pop	edi
$L57306:
	pop	esi

; 74   : 	}
; 75   : }

	ret	0
?RemoveAll@?$CMap@HPAVCGuiBaseObject@GS@@@Container@@QAEXXZ ENDP ; Container::CMap<int,GS::CGuiBaseObject *>::RemoveAll
_TEXT	ENDS
END
