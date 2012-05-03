	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBBlur.cpp
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
;	COMDAT ?GetWidth@CDIB@GS@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLineArray@CDIB@GS@@QAEABV?$CArray@PAUtagRGBQUAD@@@Container@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CArray@PAUtagRGBQUAD@@@Container@@QAE@ABV01@@Z
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
;	COMDAT ?InsertAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXIABV12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InsertAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXIPBQAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsMemOverlap@Container@@YAXPAPAUtagRGBQUAD@@0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyItems@Container@@YAXPBQAUtagRGBQUAD@@PAPAU2@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsNoMemOverlap@Container@@YAXPAPAUtagRGBQUAD@@0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?Blur@CDIB@GS@@QAEXH@Z				; GS::CDIB::Blur
PUBLIC	??0?$CArray@PAUtagRGBQUAD@@@Container@@QAE@ABV01@@Z ; Container::CArray<tagRGBQUAD *>::CArray<tagRGBQUAD *>
PUBLIC	?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ ; Container::CArray<tagRGBQUAD *>::RemoveAll
PUBLIC	??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@	; Container::CArray<tagRGBQUAD *>::`vftable'
PUBLIC	??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z ; Container::CArray<tagRGBQUAD *>::`scalar deleting destructor'
PUBLIC	??_E?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z ; Container::CArray<tagRGBQUAD *>::`vector deleting destructor'
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
; File ../array.h
CONST	SEGMENT
??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ DD FLAT:??_E?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z ; Container::CArray<tagRGBQUAD *>::`vftable'
CONST	ENDS
_TEXT	SEGMENT
_nTimes$ = 8
_this$ = -36
_arr$ = -56
_nRow$55614 = 8
_bPrev$55618 = -8
?Blur@CDIB@GS@@QAEXH@Z PROC NEAR			; GS::CDIB::Blur

; 14   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 56					; 00000038H
	push	esi
	mov	esi, ecx

; 15   : #define BLUR( color, bytes )  \
; 16   : 		    (BYTE)((bPrev[nCol - bytes].color + bPrev[nCol].color + bPrev[nCol + bytes].color \
; 17   :  		   + pbyte[nCol - bytes].color + pbyte[nCol].color + pbyte[nCol + bytes].color	\
; 18   : 		   + bNext[nCol - bytes].color + bNext[nCol].color + bNext[nCol + bytes].color) / 9)\
; 19   : 
; 20   : 	//
; 21   : 	//	REVIEW - russf - There is a problem when blurring a mostly white image in that it gives some
; 22   : 	//										artifact sliding down the display.
; 23   : 	const CLineArray arr = GetLineArray();

	lea	ecx, DWORD PTR _arr$[ebp]
	mov	DWORD PTR _this$[ebp], esi
	lea	eax, DWORD PTR [esi+56]
	push	eax
	call	??0?$CArray@PAUtagRGBQUAD@@@Container@@QAE@ABV01@@Z ; Container::CArray<tagRGBQUAD *>::CArray<tagRGBQUAD *>

; 24   : 
; 25   : 	#define bytes 1
; 26   : 
; 27   : 	while( nTimes )

	mov	eax, DWORD PTR _nTimes$[ebp]
	test	eax, eax
	je	$L55613
	push	ebx
	push	edi
	mov	DWORD PTR -16+[ebp], eax
$L55612:

; 28   : 	{
; 29   : 		for( UINT nRow = bytes; nRow < arr.GetSize() - bytes; nRow++ )

	mov	ecx, DWORD PTR _arr$[ebp+16]
	push	1
	pop	eax
	dec	ecx
	cmp	ecx, eax
	mov	DWORD PTR _nRow$55614[ebp], eax
	jbe	$L55617

; 32   : 			DIB_PIXEL * pbyte = arr[ nRow ];
; 33   : 			const DIB_PIXEL * bNext = arr[ nRow + 1];
; 34   : 			for( UINT nCol = bytes; nCol < (UINT)GetWidth() - bytes ; nCol++ )

	mov	eax, DWORD PTR [esi+76]
	dec	eax
	mov	DWORD PTR -12+[ebp], eax
$L55615:
	mov	eax, DWORD PTR _nRow$55614[ebp]
	mov	ecx, DWORD PTR _arr$[ebp+4]
	mov	edx, eax
	shl	edx, 2
	cmp	DWORD PTR -12+[ebp], 1
	mov	edi, DWORD PTR [edx+ecx-4]
	mov	eax, DWORD PTR [edx+ecx]
	mov	edx, DWORD PTR [edx+ecx+4]
	mov	DWORD PTR _bPrev$55618[ebp], edi
	jbe	$L55616
	mov	DWORD PTR -4+[ebp], eax
	mov	ebx, edi
	sub	DWORD PTR -4+[ebp], edx
	lea	ecx, DWORD PTR [edx+8]
	sub	ebx, edx
	mov	edx, DWORD PTR -12+[ebp]
	lea	esi, DWORD PTR [eax+6]
	sub	eax, edi
	dec	edx
	mov	DWORD PTR -20+[ebp], ebx
	mov	DWORD PTR -24+[ebp], eax
	mov	DWORD PTR -8+[ebp], edx
	jmp	SHORT $L55623
$L55849:

; 30   : 		{
; 31   : 			const DIB_PIXEL * bPrev = arr[ nRow - 1];

	mov	ebx, DWORD PTR -20+[ebp]
	mov	eax, DWORD PTR -24+[ebp]
$L55623:

; 35   : 			{
; 36   : 				pbyte[nCol].rgbBlue = BLUR( rgbBlue, bytes );

	movzx	eax, BYTE PTR [edi+eax]
	movzx	ebx, BYTE PTR [ebx+ecx]
	add	eax, ebx
	mov	ebx, DWORD PTR -4+[ebp]
	lea	edx, DWORD PTR [edi+4]
	push	9
	movzx	ebx, BYTE PTR [ebx+ecx]
	add	eax, ebx
	mov	DWORD PTR -28+[ebp], edx
	movzx	ebx, BYTE PTR [ecx-8]
	add	eax, ebx
	movzx	ebx, BYTE PTR [ecx-4]
	add	eax, ebx
	movzx	ebx, BYTE PTR [edi]
	add	eax, ebx
	movzx	ebx, BYTE PTR [ecx]
	movzx	edx, BYTE PTR [edx]
	add	eax, ebx
	movzx	ebx, BYTE PTR [esi-2]
	add	eax, ebx
	pop	ebx
	add	eax, edx

; 37   : 				pbyte[nCol].rgbRed = BLUR( rgbRed, bytes );

	push	9
	cdq
	idiv	ebx
	lea	edx, DWORD PTR [esi+4]
	mov	DWORD PTR -32+[ebp], edx
	mov	BYTE PTR [esi-2], al
	movzx	eax, BYTE PTR [ecx-6]
	movzx	ebx, BYTE PTR [ecx-2]
	add	eax, ebx
	movzx	ebx, BYTE PTR [ecx+2]
	add	eax, ebx
	movzx	ebx, BYTE PTR [edi+10]
	add	eax, ebx
	movzx	ebx, BYTE PTR [edi+6]
	add	eax, ebx
	movzx	ebx, BYTE PTR [edi+2]
	add	eax, ebx
	movzx	ebx, BYTE PTR [edx-8]
	movzx	edx, BYTE PTR [edx]
	add	eax, ebx
	movzx	ebx, BYTE PTR [esi]
	add	eax, ebx
	pop	ebx
	add	eax, edx
	cdq
	idiv	ebx
	mov	BYTE PTR [esi], al

; 38   : 				pbyte[nCol].rgbGreen = BLUR( rgbGreen, bytes );

	movzx	eax, BYTE PTR [ecx-7]
	movzx	edx, BYTE PTR [ecx-3]
	add	eax, edx
	movzx	edx, BYTE PTR [ecx+1]
	add	eax, edx
	movzx	edx, BYTE PTR [edi+9]
	add	eax, edx
	movzx	edx, BYTE PTR [edi+5]
	add	eax, edx
	movzx	edx, BYTE PTR [edi+1]
	add	eax, edx
	movzx	edx, BYTE PTR [esi-5]
	add	eax, edx
	movzx	edx, BYTE PTR [esi+3]
	add	eax, edx
	movzx	edx, BYTE PTR [esi-1]
	add	eax, edx
	cdq
	idiv	ebx
	mov	BYTE PTR [esi-1], al

; 39   : 
; 40   : 				//
; 41   : 				//	Don't forget to also blur th alpha channel
; 42   : 				pbyte[nCol].rgbReserved = BLUR( rgbReserved, bytes );

	movzx	eax, BYTE PTR [ecx-5]
	movzx	edx, BYTE PTR [ecx+3]
	add	eax, edx
	movzx	edx, BYTE PTR [ecx-1]
	add	eax, edx
	movzx	edx, BYTE PTR [edi+11]
	add	eax, edx
	movzx	edx, BYTE PTR [edi+7]
	add	eax, edx
	movzx	edx, BYTE PTR [edi+3]
	add	eax, edx
	movzx	edx, BYTE PTR [esi+5]
	add	eax, edx
	push	ebx
	movzx	edx, BYTE PTR [esi-3]
	add	eax, edx
	pop	edi
	movzx	edx, BYTE PTR [esi+1]
	add	eax, edx
	add	ecx, 4
	cdq
	idiv	edi
	dec	DWORD PTR -8+[ebp]
	mov	edi, DWORD PTR -28+[ebp]
	mov	BYTE PTR [esi+1], al
	mov	esi, DWORD PTR -32+[ebp]
	jne	$L55849

; 32   : 			DIB_PIXEL * pbyte = arr[ nRow ];
; 33   : 			const DIB_PIXEL * bNext = arr[ nRow + 1];
; 34   : 			for( UINT nCol = bytes; nCol < (UINT)GetWidth() - bytes ; nCol++ )

	mov	esi, DWORD PTR _this$[ebp]
$L55616:

; 28   : 	{
; 29   : 		for( UINT nRow = bytes; nRow < arr.GetSize() - bytes; nRow++ )

	mov	eax, DWORD PTR _arr$[ebp+16]
	inc	DWORD PTR _nRow$55614[ebp]
	dec	eax
	cmp	DWORD PTR _nRow$55614[ebp], eax
	jb	$L55615
$L55617:

; 24   : 
; 25   : 	#define bytes 1
; 26   : 
; 27   : 	while( nTimes )

	dec	DWORD PTR -16+[ebp]
	jne	$L55612
	pop	edi
	pop	ebx
$L55613:

; 43   : 			}
; 44   : 		}
; 45   : 		nTimes--;
; 46   : 	}
; 47   : }

	lea	ecx, DWORD PTR _arr$[ebp]
	mov	DWORD PTR _arr$[ebp], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	call	?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ ; Container::CArray<tagRGBQUAD *>::RemoveAll
	pop	esi
	leave
	ret	4
?Blur@CDIB@GS@@QAEXH@Z ENDP				; GS::CDIB::Blur
_TEXT	ENDS
PUBLIC	?InsertAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXIPBQAUtagRGBQUAD@@I@Z ; Container::CArray<tagRGBQUAD *>::InsertAt
;	COMDAT ??0?$CArray@PAUtagRGBQUAD@@@Container@@QAE@ABV01@@Z
_TEXT	SEGMENT
_arrToCopy$ = 8
??0?$CArray@PAUtagRGBQUAD@@@Container@@QAE@ABV01@@Z PROC NEAR ; Container::CArray<tagRGBQUAD *>::CArray<tagRGBQUAD *>, COMDAT

; 34   : 	InsertAt( 0, arrToCopy );

	mov	eax, DWORD PTR _arrToCopy$[esp-4]
	push	esi
	mov	esi, ecx
	push	DWORD PTR [eax+16]
	xor	ecx, ecx
	mov	DWORD PTR [esi+4], ecx
	mov	DWORD PTR [esi+12], ecx
	push	DWORD PTR [eax+4]
	mov	DWORD PTR [esi+16], ecx
	mov	DWORD PTR [esi+8], 1
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	push	ecx
	mov	ecx, esi
	call	?InsertAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXIPBQAUtagRGBQUAD@@I@Z ; Container::CArray<tagRGBQUAD *>::InsertAt

; 35   : }

	mov	eax, esi
	pop	esi
	ret	4
??0?$CArray@PAUtagRGBQUAD@@@Container@@QAE@ABV01@@Z ENDP ; Container::CArray<tagRGBQUAD *>::CArray<tagRGBQUAD *>
_TEXT	ENDS
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z PROC NEAR ; Container::CArray<tagRGBQUAD *>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@ ; Container::CArray<tagRGBQUAD *>::`vftable'
	call	?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ ; Container::CArray<tagRGBQUAD *>::RemoveAll
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L55648
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L55648:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z ENDP	; Container::CArray<tagRGBQUAD *>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ PROC NEAR ; Container::CArray<tagRGBQUAD *>::RemoveAll, COMDAT

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
?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ ENDP ; Container::CArray<tagRGBQUAD *>::RemoveAll
_TEXT	ENDS
PUBLIC	?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z ; Container::CArray<tagRGBQUAD *>::SetSize
EXTRN	__imp__memmove:NEAR
;	COMDAT ?InsertAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXIPBQAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT
_iPos$ = 8
_p$ = 12
_uCount$ = 16
?InsertAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXIPBQAUtagRGBQUAD@@I@Z PROC NEAR ; Container::CArray<tagRGBQUAD *>::InsertAt, COMDAT

; 150  : {

	push	ebp
	mov	ebp, esp

; 151  : 	if( p )

	cmp	DWORD PTR _p$[ebp], 0
	push	esi
	mov	esi, ecx
	je	SHORT $L55893

; 152  : 	{
; 153  : 		//
; 154  : 		// Cannot handle adding to its self
; 155  : 		ASSERT( m_pItems != p );
; 156  : 		ASSERT_VALID_READOBJPTR( p );
; 157  : 		//ASSERT( !IsBadReadPtr( p, sizeof( T ) * uCount ) );
; 158  : 
; 159  : 		if( uCount == 0 )

	mov	ecx, DWORD PTR _uCount$[ebp]
	test	ecx, ecx
	je	SHORT $L55893

; 160  : 			return;
; 161  : 
; 162  : 		PI_uint32 uItemsToMove = m_uItemCount - iPos;

	mov	eax, DWORD PTR [esi+16]
	push	ebx
	mov	ebx, DWORD PTR _iPos$[ebp]
	push	edi
	mov	edi, eax

; 163  : 
; 164  : 		ASSERT( iPos <= m_uItemCount );
; 165  : 		SetSize( m_uItemCount + uCount );

	add	eax, ecx
	push	eax
	mov	ecx, esi
	sub	edi, ebx
	call	?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z ; Container::CArray<tagRGBQUAD *>::SetSize

; 166  : 
; 167  : 		//
; 168  : 		//	Move Elements above insertion point up one to make
; 169  : 		//	room for our new item
; 170  : 		MoveItemsMemOverlap( &m_pItems[iPos], &m_pItems[iPos + uCount], uItemsToMove );

	mov	eax, DWORD PTR [esi+4]
	mov	ecx, edi
	mov	edi, DWORD PTR _uCount$[ebp]
	shl	ecx, 2
	push	ecx
	lea	ecx, DWORD PTR [eax+ebx*4]
	push	ecx
	lea	ecx, DWORD PTR [ebx+edi]
	lea	eax, DWORD PTR [eax+ecx*4]
	push	eax
	call	DWORD PTR __imp__memmove

; 171  : 
; 172  : 		CopyItems( p, m_pItems + iPos, uCount );

	mov	esi, DWORD PTR [esi+4]
	mov	eax, ebx
	shl	eax, 2
	lea	edx, DWORD PTR [esi+ebx*4]
	mov	ecx, eax
	mov	eax, edx
	add	esp, 12					; 0000000cH
	sub	eax, ecx
	lea	ecx, DWORD PTR [edi]
	sub	eax, esi
	mov	edi, edx
	add	eax, DWORD PTR _p$[ebp]
	mov	esi, eax
	rep movsd
	pop	edi
	pop	ebx
$L55893:
	pop	esi

; 173  : 	}
; 174  : }

	pop	ebp
	ret	12					; 0000000cH
?InsertAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXIPBQAUtagRGBQUAD@@I@Z ENDP ; Container::CArray<tagRGBQUAD *>::InsertAt
_TEXT	ENDS
PUBLIC	?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z ; Container::CArray<tagRGBQUAD *>::RemoveAt
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
;	COMDAT ?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z
_TEXT	SEGMENT
_uSize$ = 8
_pItems$55702 = -4
?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z PROC NEAR ; Container::CArray<tagRGBQUAD *>::SetSize, COMDAT

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
	jae	SHORT $L55942

; 52   : 	{
; 53   : 		if( m_uItemsAllocated < uSize )

	mov	eax, DWORD PTR [ebp+12]
	cmp	eax, esi
	jae	SHORT $L55699

; 54   : 		{
; 55   : 			PI_uint32 uAllocateExtra = m_uGrowBy = m_uGrowBy < 262144 ? m_uGrowBy << 2 : 262144;

	mov	ecx, DWORD PTR [ebp+8]
	mov	ebx, 262144				; 00040000H
	cmp	ecx, ebx
	jae	SHORT $L55903
	mov	ebx, ecx
	shl	ebx, 2
$L55903:

; 56   : 
; 57   : 			//
; 58   : 			// Check to see if our grow by is enough?
; 59   : 			if( m_uItemsAllocated + uAllocateExtra < uSize )

	lea	ecx, DWORD PTR [eax+ebx]
	mov	DWORD PTR [ebp+8], ebx
	cmp	ecx, esi
	jae	SHORT $L55701

; 60   : 			{
; 61   : 				//
; 62   : 				//	Nope, so we allocate more
; 63   : 				uAllocateExtra = uSize - m_uItemsAllocated;

	mov	ebx, esi
	sub	ebx, eax
$L55701:

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
	mov	DWORD PTR _pItems$55702[esp+20], edi
	test	ecx, ecx
	je	SHORT $L55907

; 69   : 				MoveItemsNoMemOverlap( m_pItems, pItems, m_uItemCount );

	mov	esi, DWORD PTR [ebp+4]
	rep movsd
	mov	esi, DWORD PTR _uSize$[esp+16]
$L55907:

; 70   : 			delete[] (PI_byte*)m_pItems;

	push	DWORD PTR [ebp+4]
	call	??3@YAXPAX@Z				; operator delete

; 71   : 			m_pItems = pItems;

	mov	eax, DWORD PTR _pItems$55702[esp+24]

; 72   : 			m_uItemsAllocated += uAllocateExtra;

	add	DWORD PTR [ebp+12], ebx
	pop	ecx
	mov	DWORD PTR [ebp+4], eax

; 73   : 			bReallocationNeeded = true;

	mov	bl, 1
$L55699:

; 74   : 		}
; 75   : 
; 76   : 		//
; 77   : 		// Constuct the new items
; 78   : 		if( uSize > m_uItemCount )

	mov	eax, DWORD PTR [ebp+16]
	cmp	esi, eax
	jbe	SHORT $L55724

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

	jmp	SHORT $L55724
$L55942:

; 87   : 	{
; 88   : 		//
; 89   : 		//	Check to see if we need to reduce the size
; 90   : 		if( m_uItemCount > uSize )

	jbe	SHORT $L55724

; 91   : 		{
; 92   : 			//
; 93   : 			//	Get rid of excess elements
; 94   : 			RemoveAt( uSize, m_uItemCount - uSize );

	sub	eax, esi
	mov	ecx, ebp
	push	eax
	push	esi
	call	?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z ; Container::CArray<tagRGBQUAD *>::RemoveAt
$L55724:
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
?SetSize@?$CArray@PAUtagRGBQUAD@@@Container@@QAE_NI@Z ENDP ; Container::CArray<tagRGBQUAD *>::SetSize
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z
_TEXT	SEGMENT
_iPos$ = 8
_uItems$ = 12
?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z PROC NEAR ; Container::CArray<tagRGBQUAD *>::RemoveAt, COMDAT

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

	je	SHORT $L55954

; 200  : 		MoveItemsMemOverlap( &m_pItems[iPos + uItems], &m_pItems[iPos], uItemsToMove);

	shl	eax, 2
	add	edx, edi
	push	eax
	lea	eax, DWORD PTR [ecx+edx*4]
	push	eax
	push	ebx
	call	DWORD PTR __imp__memmove
	add	esp, 12					; 0000000cH
$L55954:

; 201  : 	m_uItemCount -= uItems;

	sub	DWORD PTR [esi+16], edi
	pop	edi
	pop	esi
	pop	ebx

; 202  : }

	ret	8
?RemoveAt@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXII@Z ENDP ; Container::CArray<tagRGBQUAD *>::RemoveAt
_TEXT	ENDS
END
