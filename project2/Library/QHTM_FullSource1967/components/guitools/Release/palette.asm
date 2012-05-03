	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\palette.cpp
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
;	COMDAT ??2@YAPAXIPAX@Z
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
;	COMDAT ?Set@CRect@WinHelper@@QAEXHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lock@CCriticalSection@WinHelper@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Unlock@CCriticalSection@WinHelper@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CHTMLPalette@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CHTMLPalette@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateFromColours@CHTMLPalette@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CArray@UtagPALETTEENTRY@@@Container@@QAE@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@UtagPALETTEENTRY@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@UtagPALETTEENTRY@@@Container@@QAEAAUtagPALETTEENTRY@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CArray@UtagPALETTEENTRY@@@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetData@?$CArray@UtagPALETTEENTRY@@@Container@@QAEPAUtagPALETTEENTRY@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@UtagPALETTEENTRY@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@UtagPALETTEENTRY@@@Container@@QAE_NI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAUtagPALETTEENTRY@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsNoMemOverlap@Container@@YAXPAUtagPALETTEENTRY@@0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructItems@Container@@YAXPAUtagPALETTEENTRY@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsMemOverlap@Container@@YAXPAUtagPALETTEENTRY@@0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7?$CArray@UtagPALETTEENTRY@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
CRT$XCU	SEGMENT
_$S5	DD	FLAT:_$E4
CRT$XCU	ENDS
_TEXT	SEGMENT
_$E4	PROC NEAR
	call	_$E1
	jmp	_$E3
_$E4	ENDP
_TEXT	ENDS
PUBLIC	?CreateFromColours@CHTMLPalette@@QAEXXZ		; CHTMLPalette::CreateFromColours
_BSS	SEGMENT
_g_Pal	DD	01H DUP (?)
_BSS	ENDS
_TEXT	SEGMENT
_$E1	PROC NEAR

; 82   : static CHTMLPalette g_Pal;

	and	DWORD PTR _g_Pal, 0
	mov	ecx, OFFSET FLAT:_g_Pal
	jmp	?CreateFromColours@CHTMLPalette@@QAEXXZ	; CHTMLPalette::CreateFromColours
_$E1	ENDP
_TEXT	ENDS
PUBLIC	??0?$CArray@UtagPALETTEENTRY@@@Container@@QAE@I@Z ; Container::CArray<tagPALETTEENTRY>::CArray<tagPALETTEENTRY>
PUBLIC	?RemoveAll@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXXZ ; Container::CArray<tagPALETTEENTRY>::RemoveAll
PUBLIC	??_7?$CArray@UtagPALETTEENTRY@@@Container@@6B@	; Container::CArray<tagPALETTEENTRY>::`vftable'
PUBLIC	??_G?$CArray@UtagPALETTEENTRY@@@Container@@UAEPAXI@Z ; Container::CArray<tagPALETTEENTRY>::`scalar deleting destructor'
PUBLIC	??_E?$CArray@UtagPALETTEENTRY@@@Container@@UAEPAXI@Z ; Container::CArray<tagPALETTEENTRY>::`vector deleting destructor'
;	COMDAT ??_7?$CArray@UtagPALETTEENTRY@@@Container@@6B@
; File ../array.h
CONST	SEGMENT
??_7?$CArray@UtagPALETTEENTRY@@@Container@@6B@ DD FLAT:??_E?$CArray@UtagPALETTEENTRY@@@Container@@UAEPAXI@Z ; Container::CArray<tagPALETTEENTRY>::`vftable'
CONST	ENDS
;	COMDAT ?CreateFromColours@CHTMLPalette@@QAEXXZ
_TEXT	SEGMENT
_arrPals$ = -28
_nGreen$ = -8
_nRed$55462 = -4
?CreateFromColours@CHTMLPalette@@QAEXXZ PROC NEAR	; CHTMLPalette::CreateFromColours, COMDAT

; 52   : 	{

	push	ebp
	mov	ebp, esp
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	edi, ecx

; 53   : 
; 54   : 		//	add a standard palette for the HTML.
; 55   : 		//	This code was taken from a palette GIF from which I found at lynda.com
; 56   : 		//	In her article she suggested that it was a mathematical palette rather than one chosen
; 57   : 		//	for aethetics.
; 58   : 		//
; 59   : 		//	Create an array and create a HPALETTE from that array
; 60   : 		Container::CArray< PALETTEENTRY > arrPals( 216 );

	push	216					; 000000d8H
	lea	ecx, DWORD PTR _arrPals$[ebp]
	call	??0?$CArray@UtagPALETTEENTRY@@@Container@@QAE@I@Z ; Container::CArray<tagPALETTEENTRY>::CArray<tagPALETTEENTRY>

; 61   : 		int n = 0;

	xor	esi, esi

; 62   : 		for( int nGreen = 0; nGreen <= 255; nGreen+=51 )

	mov	ecx, 255				; 000000ffH
	and	DWORD PTR _nGreen$[ebp], esi
$L55459:

; 63   : 		{
; 64   : 			for( int nRed = 0; nRed <= 255; nRed+=51 )

	and	DWORD PTR _nRed$55462[ebp], 0
$L55463:

; 65   : 			{
; 66   : 				for( int nBlue = 0; nBlue <= 255; nBlue+=51 )

	xor	edx, edx
$L55467:
	mov	eax, DWORD PTR _arrPals$[ebp+4]

; 67   : 				{
; 68   : 					PALETTEENTRY &pal = arrPals[ n ];
; 69   : 					pal.peRed = (BYTE)nRed;

	mov	bl, BYTE PTR _nRed$55462[ebp]
	mov	BYTE PTR [eax+esi*4], bl

; 70   : 					pal.peGreen = (BYTE)nGreen;

	mov	bl, BYTE PTR _nGreen$[ebp]
	lea	eax, DWORD PTR [eax+esi*4]
	mov	BYTE PTR [eax+1], bl

; 71   : 					pal.peBlue = (BYTE)nBlue;

	mov	BYTE PTR [eax+2], dl

; 72   : 					pal.peFlags = 0;

	and	BYTE PTR [eax+3], 0
	add	edx, 51					; 00000033H

; 73   : 					n++;

	inc	esi
	cmp	edx, ecx
	jle	SHORT $L55467
	add	DWORD PTR _nRed$55462[ebp], 51		; 00000033H
	cmp	DWORD PTR _nRed$55462[ebp], ecx
	jle	SHORT $L55463
	add	DWORD PTR _nGreen$[ebp], 51		; 00000033H
	cmp	DWORD PTR _nGreen$[ebp], ecx
	jle	SHORT $L55459

; 74   : 				}
; 75   : 			}
; 76   : 		}
; 77   : 		m_hPalette = CreatePaletteFrom( arrPals.GetData(), arrPals.GetSize() );

	push	DWORD PTR _arrPals$[ebp+16]
	push	DWORD PTR _arrPals$[ebp+4]
	call	?CreatePaletteFrom@@YAPAUHPALETTE__@@QBUtagPALETTEENTRY@@H@Z ; CreatePaletteFrom
	pop	ecx
	mov	DWORD PTR [edi], eax
	pop	ecx

; 78   : 	}

	mov	DWORD PTR _arrPals$[ebp], OFFSET FLAT:??_7?$CArray@UtagPALETTEENTRY@@@Container@@6B@ ; Container::CArray<tagPALETTEENTRY>::`vftable'
	lea	ecx, DWORD PTR _arrPals$[ebp]
	call	?RemoveAll@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXXZ ; Container::CArray<tagPALETTEENTRY>::RemoveAll
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	0
?CreateFromColours@CHTMLPalette@@QAEXXZ ENDP		; CHTMLPalette::CreateFromColours
_TEXT	ENDS
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	__imp__CreatePalette@4:NEAR
_TEXT	SEGMENT
_pPalEntry$ = 8
_nSize$ = 12
?CreatePaletteFrom@@YAPAUHPALETTE__@@QBUtagPALETTEENTRY@@H@Z PROC NEAR ; CreatePaletteFrom

; 15   : {

	push	ebx
	push	esi
	push	edi

; 16   : 	ASSERT( pPalEntry );
; 17   : 	ASSERT( nSize > 0 );
; 18   : 
; 19   : 	HPALETTE     hPalette = NULL;
; 20   : 	LPLOGPALETTE lpLogPal = reinterpret_cast<LPLOGPALETTE>( new char [ sizeof(LOGPALETTE)+(sizeof(PALETTEENTRY)*nSize) ] );

	mov	edi, DWORD PTR _nSize$[esp+8]
	xor	ebx, ebx
	lea	eax, DWORD PTR [edi*4+8]
	push	eax
	call	??2@YAPAXI@Z				; operator new
	mov	esi, eax
	pop	ecx

; 21   : 	if( lpLogPal )

	test	esi, esi
	je	SHORT $L55361

; 22   : 	{
; 23   : 		lpLogPal->palNumEntries = (WORD)nSize;
; 24   : 		lpLogPal->palVersion    = (WORD)0x0300;
; 25   : 		for(int i = 0; i < nSize; i++)

	test	edi, edi
	mov	WORD PTR [esi+2], di
	mov	WORD PTR [esi], 768			; 00000300H
	jle	SHORT $L55367
	mov	eax, DWORD PTR _pPalEntry$[esp+8]
	lea	ecx, DWORD PTR [esi+5]
	add	eax, 2
$L55365:

; 26   : 		{
; 27   : 			lpLogPal->palPalEntry[i].peRed   = pPalEntry[i].peRed;

	mov	dl, BYTE PTR [eax-2]
	mov	BYTE PTR [ecx-1], dl

; 28   : 			lpLogPal->palPalEntry[i].peGreen = pPalEntry[i].peGreen;

	mov	dl, BYTE PTR [eax-1]
	mov	BYTE PTR [ecx], dl

; 29   : 			lpLogPal->palPalEntry[i].peBlue  = pPalEntry[i].peBlue;

	mov	dl, BYTE PTR [eax]
	mov	BYTE PTR [ecx+1], dl

; 30   : 			lpLogPal->palPalEntry[i].peFlags = pPalEntry[i].peFlags;

	mov	dl, BYTE PTR [eax+1]
	mov	BYTE PTR [ecx+2], dl
	add	eax, 4
	add	ecx, 4
	dec	edi
	jne	SHORT $L55365
$L55367:

; 31   : 		}
; 32   : 		hPalette = CreatePalette( lpLogPal );

	push	esi
	call	DWORD PTR __imp__CreatePalette@4

; 33   : 		delete[] (char*)lpLogPal;

	push	esi
	mov	ebx, eax
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L55361:
	pop	edi

; 34   : 	}
; 35   : 
; 36   : 	return hPalette;

	mov	eax, ebx
	pop	esi
	pop	ebx

; 37   : }

	ret	0
?CreatePaletteFrom@@YAPAUHPALETTE__@@QBUtagPALETTEENTRY@@H@Z ENDP ; CreatePaletteFrom
_TEXT	ENDS
EXTRN	_atexit:NEAR
_TEXT	SEGMENT
_$E3	PROC NEAR
	push	OFFSET FLAT:_$E2
	call	_atexit
	pop	ecx
	ret	0
_$E3	ENDP
_TEXT	ENDS
EXTRN	__imp__DeleteObject@4:NEAR
_TEXT	SEGMENT
_$E2	PROC NEAR
	push	DWORD PTR _g_Pal
	call	DWORD PTR __imp__DeleteObject@4
	and	DWORD PTR _g_Pal, 0
	ret	0
_$E2	ENDP
_TEXT	ENDS
PUBLIC	?GetCurrentWindowsPalette@GS@@YAPAUHPALETTE__@@XZ ; GS::GetCurrentWindowsPalette
_TEXT	SEGMENT
?GetCurrentWindowsPalette@GS@@YAPAUHPALETTE__@@XZ PROC NEAR ; GS::GetCurrentWindowsPalette

; 87   : 	return g_Pal.m_hPalette;

	mov	eax, DWORD PTR _g_Pal

; 88   : }

	ret	0
?GetCurrentWindowsPalette@GS@@YAPAUHPALETTE__@@XZ ENDP	; GS::GetCurrentWindowsPalette
_TEXT	ENDS
PUBLIC	?SetSize@?$CArray@UtagPALETTEENTRY@@@Container@@QAE_NI@Z ; Container::CArray<tagPALETTEENTRY>::SetSize
;	COMDAT ??0?$CArray@UtagPALETTEENTRY@@@Container@@QAE@I@Z
_TEXT	SEGMENT
_uSize$ = 8
??0?$CArray@UtagPALETTEENTRY@@@Container@@QAE@I@Z PROC NEAR ; Container::CArray<tagPALETTEENTRY>::CArray<tagPALETTEENTRY>, COMDAT

; 23   : {

	push	esi
	mov	esi, ecx

; 24   : 	SetSize( uSize );

	push	DWORD PTR _uSize$[esp]
	xor	eax, eax
	mov	DWORD PTR [esi+4], eax
	mov	DWORD PTR [esi+8], 1
	mov	DWORD PTR [esi+12], eax
	mov	DWORD PTR [esi+16], eax
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@UtagPALETTEENTRY@@@Container@@6B@ ; Container::CArray<tagPALETTEENTRY>::`vftable'
	call	?SetSize@?$CArray@UtagPALETTEENTRY@@@Container@@QAE_NI@Z ; Container::CArray<tagPALETTEENTRY>::SetSize

; 25   : }

	mov	eax, esi
	pop	esi
	ret	4
??0?$CArray@UtagPALETTEENTRY@@@Container@@QAE@I@Z ENDP	; Container::CArray<tagPALETTEENTRY>::CArray<tagPALETTEENTRY>
_TEXT	ENDS
;	COMDAT ??_G?$CArray@UtagPALETTEENTRY@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CArray@UtagPALETTEENTRY@@@Container@@UAEPAXI@Z PROC NEAR ; Container::CArray<tagPALETTEENTRY>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@UtagPALETTEENTRY@@@Container@@6B@ ; Container::CArray<tagPALETTEENTRY>::`vftable'
	call	?RemoveAll@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXXZ ; Container::CArray<tagPALETTEENTRY>::RemoveAll
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L55512
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L55512:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CArray@UtagPALETTEENTRY@@@Container@@UAEPAXI@Z ENDP ; Container::CArray<tagPALETTEENTRY>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXXZ PROC NEAR ; Container::CArray<tagPALETTEENTRY>::RemoveAll, COMDAT

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
?RemoveAll@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXXZ ENDP ; Container::CArray<tagPALETTEENTRY>::RemoveAll
_TEXT	ENDS
PUBLIC	?RemoveAt@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXII@Z ; Container::CArray<tagPALETTEENTRY>::RemoveAt
;	COMDAT ?SetSize@?$CArray@UtagPALETTEENTRY@@@Container@@QAE_NI@Z
_TEXT	SEGMENT
_uSize$ = 8
_pItems$55533 = -4
?SetSize@?$CArray@UtagPALETTEENTRY@@@Container@@QAE_NI@Z PROC NEAR ; Container::CArray<tagPALETTEENTRY>::SetSize, COMDAT

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
	jae	SHORT $L55756

; 52   : 	{
; 53   : 		if( m_uItemsAllocated < uSize )

	mov	eax, DWORD PTR [ebp+12]
	cmp	eax, esi
	jae	SHORT $L55530

; 54   : 		{
; 55   : 			PI_uint32 uAllocateExtra = m_uGrowBy = m_uGrowBy < 262144 ? m_uGrowBy << 2 : 262144;

	mov	ecx, DWORD PTR [ebp+8]
	mov	ebx, 262144				; 00040000H
	cmp	ecx, ebx
	jae	SHORT $L55717
	mov	ebx, ecx
	shl	ebx, 2
$L55717:

; 56   : 
; 57   : 			//
; 58   : 			// Check to see if our grow by is enough?
; 59   : 			if( m_uItemsAllocated + uAllocateExtra < uSize )

	lea	ecx, DWORD PTR [eax+ebx]
	mov	DWORD PTR [ebp+8], ebx
	cmp	ecx, esi
	jae	SHORT $L55532

; 60   : 			{
; 61   : 				//
; 62   : 				//	Nope, so we allocate more
; 63   : 				uAllocateExtra = uSize - m_uItemsAllocated;

	mov	ebx, esi
	sub	ebx, eax
$L55532:

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
	mov	DWORD PTR _pItems$55533[esp+20], edi
	test	ecx, ecx
	je	SHORT $L55721

; 69   : 				MoveItemsNoMemOverlap( m_pItems, pItems, m_uItemCount );

	mov	esi, DWORD PTR [ebp+4]
	rep movsd
	mov	esi, DWORD PTR _uSize$[esp+16]
$L55721:

; 70   : 			delete[] (PI_byte*)m_pItems;

	push	DWORD PTR [ebp+4]
	call	??3@YAXPAX@Z				; operator delete

; 71   : 			m_pItems = pItems;

	mov	eax, DWORD PTR _pItems$55533[esp+24]

; 72   : 			m_uItemsAllocated += uAllocateExtra;

	add	DWORD PTR [ebp+12], ebx
	pop	ecx
	mov	DWORD PTR [ebp+4], eax

; 73   : 			bReallocationNeeded = true;

	mov	bl, 1
$L55530:

; 74   : 		}
; 75   : 
; 76   : 		//
; 77   : 		// Constuct the new items
; 78   : 		if( uSize > m_uItemCount )

	mov	eax, DWORD PTR [ebp+16]
	cmp	esi, eax
	jbe	SHORT $L55555

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

	jmp	SHORT $L55555
$L55756:

; 87   : 	{
; 88   : 		//
; 89   : 		//	Check to see if we need to reduce the size
; 90   : 		if( m_uItemCount > uSize )

	jbe	SHORT $L55555

; 91   : 		{
; 92   : 			//
; 93   : 			//	Get rid of excess elements
; 94   : 			RemoveAt( uSize, m_uItemCount - uSize );

	sub	eax, esi
	mov	ecx, ebp
	push	eax
	push	esi
	call	?RemoveAt@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXII@Z ; Container::CArray<tagPALETTEENTRY>::RemoveAt
$L55555:
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
?SetSize@?$CArray@UtagPALETTEENTRY@@@Container@@QAE_NI@Z ENDP ; Container::CArray<tagPALETTEENTRY>::SetSize
_TEXT	ENDS
EXTRN	__imp__memmove:NEAR
;	COMDAT ?RemoveAt@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXII@Z
_TEXT	SEGMENT
_iPos$ = 8
_uItems$ = 12
?RemoveAt@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXII@Z PROC NEAR ; Container::CArray<tagPALETTEENTRY>::RemoveAt, COMDAT

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

	je	SHORT $L55768

; 200  : 		MoveItemsMemOverlap( &m_pItems[iPos + uItems], &m_pItems[iPos], uItemsToMove);

	shl	eax, 2
	add	edx, edi
	push	eax
	lea	eax, DWORD PTR [ecx+edx*4]
	push	eax
	push	ebx
	call	DWORD PTR __imp__memmove
	add	esp, 12					; 0000000cH
$L55768:

; 201  : 	m_uItemCount -= uItems;

	sub	DWORD PTR [esi+16], edi
	pop	edi
	pop	esi
	pop	ebx

; 202  : }

	ret	8
?RemoveAt@?$CArray@UtagPALETTEENTRY@@@Container@@QAEXII@Z ENDP ; Container::CArray<tagPALETTEENTRY>::RemoveAt
_TEXT	ENDS
END
