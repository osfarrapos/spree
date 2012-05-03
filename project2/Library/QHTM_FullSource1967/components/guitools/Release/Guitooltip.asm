	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\Guitooltip.cpp
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
;	COMDAT ??_GCGuiTooltip@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7CGuiTooltip@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	??0CGuiTooltip@GS@@QAE@XZ			; GS::CGuiTooltip::CGuiTooltip
PUBLIC	??_7CGuiTooltip@GS@@6B@				; GS::CGuiTooltip::`vftable'
PUBLIC	??_GCGuiTooltip@GS@@UAEPAXI@Z			; GS::CGuiTooltip::`scalar deleting destructor'
PUBLIC	??_ECGuiTooltip@GS@@UAEPAXI@Z			; GS::CGuiTooltip::`vector deleting destructor'
;	COMDAT ??_7CGuiTooltip@GS@@6B@
; File D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\Guitooltip.cpp
CONST	SEGMENT
??_7CGuiTooltip@GS@@6B@ DD FLAT:??_ECGuiTooltip@GS@@UAEPAXI@Z ; GS::CGuiTooltip::`vftable'
CONST	ENDS
_TEXT	SEGMENT
??0CGuiTooltip@GS@@QAE@XZ PROC NEAR			; GS::CGuiTooltip::CGuiTooltip

; 19   : {

	mov	eax, ecx
	and	DWORD PTR [eax+4], 0
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CGuiTooltip@GS@@6B@ ; GS::CGuiTooltip::`vftable'

; 20   : 
; 21   : }

	ret	0
??0CGuiTooltip@GS@@QAE@XZ ENDP				; GS::CGuiTooltip::CGuiTooltip
_TEXT	ENDS
PUBLIC	??1CGuiTooltip@GS@@UAE@XZ			; GS::CGuiTooltip::~CGuiTooltip
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCGuiTooltip@GS@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_GCGuiTooltip@GS@@UAEPAXI@Z PROC NEAR			; GS::CGuiTooltip::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1CGuiTooltip@GS@@UAE@XZ		; GS::CGuiTooltip::~CGuiTooltip
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L58403
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L58403:
	mov	eax, esi
	pop	esi
	ret	4
??_GCGuiTooltip@GS@@UAEPAXI@Z ENDP			; GS::CGuiTooltip::`scalar deleting destructor'
_TEXT	ENDS
_TEXT	SEGMENT
??1CGuiTooltip@GS@@UAE@XZ PROC NEAR			; GS::CGuiTooltip::~CGuiTooltip

; 25   : 	m_hwnd = NULL;

	and	DWORD PTR [ecx+4], 0
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CGuiTooltip@GS@@6B@ ; GS::CGuiTooltip::`vftable'

; 26   : }

	ret	0
??1CGuiTooltip@GS@@UAE@XZ ENDP				; GS::CGuiTooltip::~CGuiTooltip
_TEXT	ENDS
PUBLIC	?Create@CGuiTooltip@GS@@QAE_NPAUHWND__@@@Z	; GS::CGuiTooltip::Create
EXTRN	__imp__CreateWindowExA@48:NEAR
EXTRN	__imp__GetWindowLongA@8:NEAR
_DATA	SEGMENT
$SG58419 DB	'tooltips_class32', 00H
_DATA	ENDS
_TEXT	SEGMENT
_hwndParent$ = 8
?Create@CGuiTooltip@GS@@QAE_NPAUHWND__@@@Z PROC NEAR	; GS::CGuiTooltip::Create

; 30   : {

	push	esi

; 31   : 	HINSTANCE hInst = (HINSTANCE)GetWindowLong( hwndParent, GWL_HINSTANCE );

	push	-6					; fffffffaH
	push	DWORD PTR _hwndParent$[esp+4]
	mov	esi, ecx
	call	DWORD PTR __imp__GetWindowLongA@8

; 32   : 	m_hwnd = CreateWindow(TOOLTIPS_CLASS, (LPTSTR) NULL, TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwndParent, (HMENU) NULL, hInst, NULL);

	push	0
	push	eax
	push	0
	mov	eax, -2147483648			; 80000000H
	push	DWORD PTR _hwndParent$[esp+12]
	push	eax
	push	eax
	push	eax
	push	eax
	push	1
	push	0
	push	OFFSET FLAT:$SG58419
	push	0
	call	DWORD PTR __imp__CreateWindowExA@48

; 33   : 	if( m_hwnd )

	test	eax, eax
	mov	DWORD PTR [esi+4], eax
	pop	esi
	setne	al

; 34   : 	{
; 35   : 		return true;
; 36   : 	}
; 37   : 	return false;
; 38   : }

	ret	4
?Create@CGuiTooltip@GS@@QAE_NPAUHWND__@@@Z ENDP		; GS::CGuiTooltip::Create
_TEXT	ENDS
PUBLIC	?RemoveAll@CGuiTooltip@GS@@QAEXXZ		; GS::CGuiTooltip::RemoveAll
EXTRN	__imp__SendMessageA@16:NEAR
_TEXT	SEGMENT
_ti$ = -44
?RemoveAll@CGuiTooltip@GS@@QAEXXZ PROC NEAR		; GS::CGuiTooltip::RemoveAll

; 42   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH
	push	ebx
	push	esi
	push	edi
	mov	ebx, ecx

; 43   : 	TOOLINFO ti;
; 44   : 	ZeroMemory( &ti, sizeof( ti ) );

	push	11					; 0000000bH

; 45   : 	ti.cbSize = sizeof( ti );
; 46   : 	int nCount = ::SendMessage( m_hwnd, TTM_GETTOOLCOUNT, 0, 0 );

	mov	esi, DWORD PTR __imp__SendMessageA@16
	pop	ecx
	xor	eax, eax
	push	eax
	push	eax
	push	1037					; 0000040dH
	lea	edi, DWORD PTR _ti$[ebp]
	push	DWORD PTR [ebx+4]
	rep stosd
	mov	DWORD PTR _ti$[ebp], 44			; 0000002cH
	call	esi
	mov	edi, eax
$L58504:

; 47   : 	for( int n = nCount - 1; n >= 0; n-- )

	dec	edi
	js	SHORT $L58429

; 48   : 	{
; 49   : 		if( ::SendMessage( m_hwnd, TTM_ENUMTOOLS, n, (LPARAM)&ti ) )

	lea	eax, DWORD PTR _ti$[ebp]
	push	eax
	push	edi
	push	1038					; 0000040eH
	push	DWORD PTR [ebx+4]
	call	esi
	test	eax, eax
	je	SHORT $L58504

; 50   : 		{
; 51   : 			::SendMessage( m_hwnd, TTM_DELTOOL, n, (LPARAM)&ti );

	lea	eax, DWORD PTR _ti$[ebp]
	push	eax
	push	edi
	push	1029					; 00000405H
	push	DWORD PTR [ebx+4]
	call	esi
	jmp	SHORT $L58504
$L58429:
	pop	edi
	pop	esi
	pop	ebx

; 52   : 		}
; 53   : 	}
; 54   : }

	leave
	ret	0
?RemoveAll@CGuiTooltip@GS@@QAEXXZ ENDP			; GS::CGuiTooltip::RemoveAll
_TEXT	ENDS
PUBLIC	?AddTool@CGuiTooltip@GS@@QAE_NPAUHWND__@@PBD@Z	; GS::CGuiTooltip::AddTool
EXTRN	__imp__GetClientRect@8:NEAR
_TEXT	SEGMENT
_hwnd$ = 8
_pcszText$ = 12
_ti$ = -44
?AddTool@CGuiTooltip@GS@@QAE_NPAUHWND__@@PBD@Z PROC NEAR ; GS::CGuiTooltip::AddTool

; 58   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH
	push	esi
	push	edi
	mov	esi, ecx

; 59   : 	TOOLINFO ti = {0};

	push	10					; 0000000aH
	pop	ecx
	xor	eax, eax
	lea	edi, DWORD PTR _ti$[ebp+4]
	rep stosd

; 60   : 	ti.cbSize = sizeof(TOOLINFO);
; 61   : 	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
; 62   : 	ti.hwnd = m_hwnd;

	mov	eax, DWORD PTR [esi+4]

; 63   : 	ti.uId = (UINT) hwnd;
; 64   : 	if( GetClientRect( hwnd, &ti.rect ) )

	lea	ecx, DWORD PTR _ti$[ebp+16]
	mov	DWORD PTR _ti$[ebp+8], eax
	mov	eax, DWORD PTR _hwnd$[ebp]
	push	ecx
	push	eax
	mov	DWORD PTR _ti$[ebp], 44			; 0000002cH
	mov	DWORD PTR _ti$[ebp+4], 17		; 00000011H
	mov	DWORD PTR _ti$[ebp+12], eax
	call	DWORD PTR __imp__GetClientRect@8
	test	eax, eax
	je	SHORT $L58445

; 65   : 	{
; 66   : 		ti.lpszText = const_cast<LPTSTR>( pcszText );

	mov	eax, DWORD PTR _pcszText$[ebp]
	mov	DWORD PTR _ti$[ebp+36], eax

; 67   : 		if( ::SendMessage( m_hwnd, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti)) 

	lea	eax, DWORD PTR _ti$[ebp]
	push	eax
	push	0
	push	1028					; 00000404H
	push	DWORD PTR [esi+4]
	call	DWORD PTR __imp__SendMessageA@16
	test	eax, eax
	je	SHORT $L58445

; 68   : 		{
; 69   : 			return true;

	mov	al, 1
	jmp	SHORT $L58437
$L58445:

; 70   : 		}
; 71   : 	}
; 72   : 	return false;

	xor	al, al
$L58437:
	pop	edi
	pop	esi

; 73   : }

	leave
	ret	8
?AddTool@CGuiTooltip@GS@@QAE_NPAUHWND__@@PBD@Z ENDP	; GS::CGuiTooltip::AddTool
_TEXT	ENDS
PUBLIC	?Activate@CGuiTooltip@GS@@QAEXH@Z		; GS::CGuiTooltip::Activate
_TEXT	SEGMENT
_bActive$ = 8
?Activate@CGuiTooltip@GS@@QAEXH@Z PROC NEAR		; GS::CGuiTooltip::Activate

; 78   : 	(void)::SendMessage( m_hwnd, TTM_ACTIVATE, static_cast<WPARAM>(bActive), 0);

	push	0
	push	DWORD PTR _bActive$[esp]
	push	1025					; 00000401H
	push	DWORD PTR [ecx+4]
	call	DWORD PTR __imp__SendMessageA@16

; 79   : }

	ret	4
?Activate@CGuiTooltip@GS@@QAEXH@Z ENDP			; GS::CGuiTooltip::Activate
_TEXT	ENDS
PUBLIC	?UpdateTipText@CGuiTooltip@GS@@QAEXPAUHWND__@@PBD@Z ; GS::CGuiTooltip::UpdateTipText
_TEXT	SEGMENT
_hwnd$ = 8
_pcszText$ = 12
_ti$ = -44
?UpdateTipText@CGuiTooltip@GS@@QAEXPAUHWND__@@PBD@Z PROC NEAR ; GS::CGuiTooltip::UpdateTipText

; 83   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH
	push	edi
	mov	edx, ecx

; 84   : 	TOOLINFO ti = {0};

	push	10					; 0000000aH
	xor	eax, eax
	pop	ecx
	lea	edi, DWORD PTR _ti$[ebp+4]
	rep stosd

; 85   : 	ti.cbSize = sizeof(TOOLINFO);
; 86   : 	ti.hwnd = m_hwnd;
; 87   : 	ti.uId = (UINT) hwnd;

	mov	ecx, DWORD PTR _hwnd$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _ti$[ebp+12], ecx

; 88   : 	ti.lpszText = const_cast<LPTSTR>( pcszText );

	mov	ecx, DWORD PTR _pcszText$[ebp]
	mov	DWORD PTR _ti$[ebp+36], ecx

; 89   : 	(void)::SendMessage( m_hwnd, TTM_UPDATETIPTEXT, 0, (LPARAM) (LPTOOLINFO) &ti);

	lea	ecx, DWORD PTR _ti$[ebp]
	push	ecx
	push	0
	push	1036					; 0000040cH
	push	eax
	mov	DWORD PTR _ti$[ebp], 44			; 0000002cH
	mov	DWORD PTR _ti$[ebp+8], eax
	call	DWORD PTR __imp__SendMessageA@16
	pop	edi

; 90   : }

	leave
	ret	8
?UpdateTipText@CGuiTooltip@GS@@QAEXPAUHWND__@@PBD@Z ENDP ; GS::CGuiTooltip::UpdateTipText
_TEXT	ENDS
PUBLIC	?AddTool@CGuiTooltip@GS@@QAE_NPAUHWND__@@AAUtagRECT@@PBDI@Z ; GS::CGuiTooltip::AddTool
_TEXT	SEGMENT
_hwnd$ = 8
_rc$ = 12
_pcszText$ = 16
_uID$ = 20
_ti$ = -44
?AddTool@CGuiTooltip@GS@@QAE_NPAUHWND__@@AAUtagRECT@@PBDI@Z PROC NEAR ; GS::CGuiTooltip::AddTool

; 94   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH
	push	esi
	push	edi
	mov	edx, ecx

; 95   : 	TOOLINFO ti = {0};

	push	10					; 0000000aH

; 96   : 	ti.cbSize = sizeof(TOOLINFO);
; 97   : 	ti.uFlags = TTF_SUBCLASS;
; 98   : 	ti.hwnd = hwnd;
; 99   : 	ti.uId = uID;
; 100  : 	ti.rect = rc;

	mov	esi, DWORD PTR _rc$[ebp]
	pop	ecx
	xor	eax, eax
	lea	edi, DWORD PTR _ti$[ebp+4]
	rep stosd
	mov	eax, DWORD PTR _hwnd$[ebp]
	lea	edi, DWORD PTR _ti$[ebp+16]
	mov	DWORD PTR _ti$[ebp+8], eax
	mov	eax, DWORD PTR _uID$[ebp]
	mov	DWORD PTR _ti$[ebp], 44			; 0000002cH
	mov	DWORD PTR _ti$[ebp+4], 16		; 00000010H
	mov	DWORD PTR _ti$[ebp+12], eax

; 101  : 	ti.lpszText = const_cast<LPTSTR>( pcszText );

	mov	eax, DWORD PTR _pcszText$[ebp]
	movsd
	movsd
	movsd
	movsd
	mov	DWORD PTR _ti$[ebp+36], eax

; 102  : 	if( ::SendMessage( m_hwnd, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti) )

	lea	eax, DWORD PTR _ti$[ebp]
	push	eax
	push	0
	push	1028					; 00000404H
	push	DWORD PTR [edx+4]
	call	DWORD PTR __imp__SendMessageA@16
	test	eax, eax
	pop	edi
	pop	esi
	setne	al

; 103  : 	{
; 104  : 		return true;
; 105  : 	}
; 106  : 	return false;
; 107  : }

	leave
	ret	16					; 00000010H
?AddTool@CGuiTooltip@GS@@QAE_NPAUHWND__@@AAUtagRECT@@PBDI@Z ENDP ; GS::CGuiTooltip::AddTool
_TEXT	ENDS
PUBLIC	?UpdateToolRect@CGuiTooltip@GS@@QAEXPAUHWND__@@IABUtagRECT@@@Z ; GS::CGuiTooltip::UpdateToolRect
_TEXT	SEGMENT
_hwnd$ = 8
_uID$ = 12
_rc$ = 16
_ti$ = -44
?UpdateToolRect@CGuiTooltip@GS@@QAEXPAUHWND__@@IABUtagRECT@@@Z PROC NEAR ; GS::CGuiTooltip::UpdateToolRect

; 111  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH
	push	esi
	push	edi
	mov	edx, ecx

; 112  : 	TOOLINFO ti = {0};

	push	10					; 0000000aH
	pop	ecx
	xor	eax, eax
	lea	edi, DWORD PTR _ti$[ebp+4]

; 113  : 	ti.cbSize = sizeof(TOOLINFO);
; 114  : 	ti.uFlags = 0;
; 115  : 	ti.hwnd = hwnd;
; 116  : 	ti.uId = uID;
; 117  : 	ti.rect = rc;

	mov	esi, DWORD PTR _rc$[ebp]
	rep stosd
	mov	ecx, DWORD PTR _hwnd$[ebp]
	lea	edi, DWORD PTR _ti$[ebp+16]
	mov	DWORD PTR _ti$[ebp+8], ecx
	mov	ecx, DWORD PTR _uID$[ebp]
	mov	DWORD PTR _ti$[ebp], 44			; 0000002cH
	mov	DWORD PTR _ti$[ebp+4], eax
	mov	DWORD PTR _ti$[ebp+12], ecx

; 118  : 	ti.lpszText = NULL;
; 119  : 	(void)::SendMessage( m_hwnd, TTM_NEWTOOLRECT, 0, (LPARAM) (LPTOOLINFO) &ti );

	lea	ecx, DWORD PTR _ti$[ebp]
	movsd
	movsd
	movsd
	push	ecx
	push	eax
	push	1030					; 00000406H
	push	DWORD PTR [edx+4]
	movsd
	mov	DWORD PTR _ti$[ebp+36], eax
	call	DWORD PTR __imp__SendMessageA@16
	pop	edi
	pop	esi

; 120  : }

	leave
	ret	12					; 0000000cH
?UpdateToolRect@CGuiTooltip@GS@@QAEXPAUHWND__@@IABUtagRECT@@@Z ENDP ; GS::CGuiTooltip::UpdateToolRect
_TEXT	ENDS
END
