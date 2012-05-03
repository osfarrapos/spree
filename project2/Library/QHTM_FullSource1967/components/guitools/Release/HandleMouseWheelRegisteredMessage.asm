	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\HandleMouseWheelRegisteredMessage.cpp
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
;	COMDAT ?IsShiftPressed@WinHelper@@YA_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsControlPressed@WinHelper@@YA_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?HandleMouseWheelRegisteredMessage@@YIJPAUHWND__@@IJ@Z ; HandleMouseWheelRegisteredMessage
EXTRN	__imp__SendMessageA@16:NEAR
EXTRN	__imp__GetFocus@0:NEAR
EXTRN	__imp__GetKeyState@4:NEAR
EXTRN	__imp__GetDesktopWindow@0:NEAR
EXTRN	__imp__GetParent@4:NEAR
_TEXT	SEGMENT
_lParam$ = 8
_hwDesktop$ = -4
?HandleMouseWheelRegisteredMessage@@YIJPAUHWND__@@IJ@Z PROC NEAR ; HandleMouseWheelRegisteredMessage

; 43   : {

	push	ecx
	push	ebx
	push	ebp
	push	esi

; 44   : 	//
; 45   : 	//	Fabricate the key state information for our shiny new WM_MOUSEWHEEL message
; 46   : 	//	we are about to send
; 47   : 	WPARAM wpKeyState = ( wParam << 16 );
; 48   : 	if( WinHelper::IsControlPressed() )

	mov	esi, DWORD PTR __imp__GetKeyState@4
	push	edi
	mov	edi, ecx
	shl	edx, 16					; 00000010H
	push	17					; 00000011H
	mov	ebx, edx
	call	esi
	shr	eax, 15					; 0000000fH
	test	al, 1
	je	SHORT $L55209

; 49   : 		wpKeyState |= MK_CONTROL;

	or	ebx, 8
$L55209:

; 50   : 
; 51   : 	if( WinHelper::IsShiftPressed() )

	push	16					; 00000010H
	call	esi
	shr	eax, 15					; 0000000fH
	test	al, 1
	je	SHORT $L55210

; 52   : 		wpKeyState |= MK_SHIFT;

	or	ebx, 4
$L55210:

; 53   : 
; 54   : 
; 55   : 	//
; 56   : 	//	Get the focus window, if there is no focus window then simply send our fabricated
; 57   : 	//	WM_MOUSEWHEEL message to the window passed.
; 58   : 	//	If there is a focus window then according to the docs we shoudl propagate the
; 59   : 	//	message of through the windows until we get to the desktop(owner of all windows)
; 60   : 	//	or until some window handles the message
; 61   : 	HWND hwFocus = ::GetFocus();

	call	DWORD PTR __imp__GetFocus@0
	mov	esi, eax

; 62   : 	const HWND hwDesktop = ::GetDesktopWindow();

	call	DWORD PTR __imp__GetDesktopWindow@0

; 63   : 	LRESULT lResult = 0;

	xor	ebp, ebp
	mov	DWORD PTR _hwDesktop$[esp+20], eax

; 64   : 	if( hwFocus == NULL )

	test	esi, esi
	jne	SHORT $L55266

; 65   : 	{
; 66   : 		lResult = SendMessage( hwnd, WM_MOUSEWHEEL, wpKeyState, lParam );

	push	DWORD PTR _lParam$[esp+16]
	push	ebx
	push	522					; 0000020aH
	push	edi
	call	DWORD PTR __imp__SendMessageA@16
	mov	ebp, eax

; 67   : 	}
; 68   : 	else

	jmp	SHORT $L55218
$L55266:
	mov	edi, DWORD PTR _lParam$[esp+16]
$L55216:

; 69   : 	{
; 70   : 		do
; 71   : 		{
; 72   : 			lParam = ::SendMessage( hwFocus, WM_MOUSEWHEEL, wpKeyState, lParam );

	push	edi
	push	ebx
	push	522					; 0000020aH
	push	esi
	call	DWORD PTR __imp__SendMessageA@16
	mov	edi, eax

; 73   : 			hwFocus = ::GetParent( hwFocus );

	push	esi
	call	DWORD PTR __imp__GetParent@4

; 74   : 		}
; 75   : 		while( lParam == 0 && hwFocus != NULL && hwFocus != hwDesktop );

	test	edi, edi
	mov	esi, eax
	jne	SHORT $L55218
	test	esi, esi
	je	SHORT $L55218
	cmp	esi, DWORD PTR _hwDesktop$[esp+20]
	jne	SHORT $L55216
$L55218:
	pop	edi

; 76   : 	}
; 77   : 	return lResult;

	mov	eax, ebp
	pop	esi
	pop	ebp
	pop	ebx

; 78   : }

	pop	ecx
	ret	4
?HandleMouseWheelRegisteredMessage@@YIJPAUHWND__@@IJ@Z ENDP ; HandleMouseWheelRegisteredMessage
_TEXT	ENDS
END
