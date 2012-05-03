	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DIBCreateRegionFromTransparency.cpp
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
;	COMDAT ??0CRect@WinHelper@@QAE@XZ
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
;	COMDAT ?GetLineArray@CDIB@GS@@QAEABV?$CArray@PAUtagRGBQUAD@@@Container@@XZ
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
;	COMDAT ??0?$CArray@E@Container@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@E@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Add@?$CArray@E@Container@@QAEXPBEI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InsertAt@?$CArray@E@Container@@QAEXIPBEI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CArray@E@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetData@?$CArray@E@Container@@QAEPAEXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@E@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@E@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@E@Container@@QAE_NI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsMemOverlap@Container@@YAXPAE0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyItems@Container@@YAXPBEPAEI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CArray@E@Container@@QAEXII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAEI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsNoMemOverlap@Container@@YAXPAE0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructItems@Container@@YAXPAEI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7?$CArray@E@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?DIBCreateRegionFromTransparency@GS@@YAPAUHRGN__@@AAVCDIB@1@@Z ; GS::DIBCreateRegionFromTransparency
PUBLIC	?InsertAt@?$CArray@E@Container@@QAEXIPBEI@Z	; Container::CArray<unsigned char>::InsertAt
PUBLIC	?RemoveAll@?$CArray@E@Container@@QAEXXZ		; Container::CArray<unsigned char>::RemoveAll
PUBLIC	??_7?$CArray@E@Container@@6B@			; Container::CArray<unsigned char>::`vftable'
PUBLIC	??_G?$CArray@E@Container@@UAEPAXI@Z		; Container::CArray<unsigned char>::`scalar deleting destructor'
PUBLIC	??_E?$CArray@E@Container@@UAEPAXI@Z		; Container::CArray<unsigned char>::`vector deleting destructor'
EXTRN	__imp__ExtCreateRegion@12:NEAR
;	COMDAT ??_7?$CArray@E@Container@@6B@
; File ../array.h
CONST	SEGMENT
??_7?$CArray@E@Container@@6B@ DD FLAT:??_E?$CArray@E@Container@@UAEPAXI@Z ; Container::CArray<unsigned char>::`vftable'
CONST	ENDS
_TEXT	SEGMENT
_dib$ = 8
_uSize$ = -12
_arrData$ = -48
_uCount$ = 8
_rc$ = -28
_x$55681 = -4
_rdh$ = -80
?DIBCreateRegionFromTransparency@GS@@YAPAUHRGN__@@AAVCDIB@1@@Z PROC NEAR ; GS::DIBCreateRegionFromTransparency

; 14   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 80					; 00000050H
	push	ebx
	push	esi

; 15   : 	const GS::CDIB::CLineArray & arrSrc = dib.GetLineArray();

	mov	esi, DWORD PTR _dib$[ebp]
	push	edi

; 16   : 	const UINT uSize = arrSrc.GetSize();
; 17   : 
; 18   : 	Container::CArray< BYTE > arrData;

	xor	ebx, ebx

; 19   : 	UINT uCount = 0;
; 20   : 
; 21   : 	WinHelper::CRect rc;
; 22   : 	for( register UINT y = 0; y < uSize; y++ )

	xor	edi, edi
	mov	eax, DWORD PTR [esi+72]
	mov	DWORD PTR _arrData$[ebp+4], ebx
	cmp	eax, ebx
	mov	DWORD PTR _uSize$[ebp], eax
	mov	DWORD PTR _arrData$[ebp+8], 1
	mov	DWORD PTR _arrData$[ebp+12], ebx
	mov	DWORD PTR _arrData$[ebp+16], ebx
	mov	DWORD PTR _arrData$[ebp], OFFSET FLAT:??_7?$CArray@E@Container@@6B@ ; Container::CArray<unsigned char>::`vftable'
	mov	DWORD PTR _uCount$[ebp], ebx
	jbe	SHORT $L55679
$L55677:

; 23   : 	{
; 24   : 		DIB_PIXEL *pSrc = arrSrc[ y ];

	mov	eax, DWORD PTR [esi+60]
	mov	ecx, DWORD PTR [eax+edi*4]

; 25   : 		for( int x = 0; x < dib.GetWidth(); x++, pSrc++ )

	xor	eax, eax
	cmp	DWORD PTR [esi+76], ebx
	mov	DWORD PTR _x$55681[ebp], eax
	jle	SHORT $L55678
	add	ecx, 3
	mov	DWORD PTR -8+[ebp], ecx
$L55682:

; 26   : 		{
; 27   : 			if( pSrc->rgbReserved != 0 )

	cmp	BYTE PTR [ecx], bl
	je	SHORT $L55683

; 28   : 			{
; 29   : 				rc.Set( x, y, x + 1, y + 1 );

	lea	ecx, DWORD PTR [edi+1]
	mov	DWORD PTR _rc$[ebp], eax
	mov	DWORD PTR _rc$[ebp+12], ecx
	lea	ecx, DWORD PTR [eax+1]

; 30   : 				arrData.Add( (BYTE*)&rc, sizeof( rc ) );

	lea	eax, DWORD PTR _rc$[ebp]
	push	16					; 00000010H
	push	eax
	mov	DWORD PTR _rc$[ebp+8], ecx
	push	DWORD PTR _arrData$[ebp+16]
	lea	ecx, DWORD PTR _arrData$[ebp]
	mov	DWORD PTR _rc$[ebp+4], edi
	call	?InsertAt@?$CArray@E@Container@@QAEXIPBEI@Z ; Container::CArray<unsigned char>::InsertAt

; 31   : 				uCount++;

	inc	DWORD PTR _uCount$[ebp]
	mov	eax, DWORD PTR _x$55681[ebp]
$L55683:
	mov	ecx, DWORD PTR -8+[ebp]
	inc	eax
	add	ecx, 4
	cmp	eax, DWORD PTR [esi+76]
	mov	DWORD PTR _x$55681[ebp], eax
	mov	DWORD PTR -8+[ebp], ecx
	jl	SHORT $L55682
$L55678:

; 19   : 	UINT uCount = 0;
; 20   : 
; 21   : 	WinHelper::CRect rc;
; 22   : 	for( register UINT y = 0; y < uSize; y++ )

	inc	edi
	cmp	edi, DWORD PTR _uSize$[ebp]
	jb	SHORT $L55677
$L55679:

; 32   : 			}
; 33   : 		}
; 34   : 	}
; 35   : 
; 36   : 	RGNDATAHEADER rdh;
; 37   : 	rdh.dwSize = sizeof( rdh );
; 38   : 	rdh.iType = RDH_RECTANGLES;
; 39   : 	rdh.nCount = uCount;

	mov	ecx, DWORD PTR _uCount$[ebp]
	push	32					; 00000020H
	mov	DWORD PTR _rdh$[ebp+8], ecx

; 40   : 	rdh.nRgnSize = 0;
; 41   : 	rdh.rcBound.left = 0;
; 42   : 	rdh.rcBound.top = 0;
; 43   : 	rdh.rcBound.right = dib.GetWidth();

	mov	ecx, DWORD PTR [esi+76]
	pop	eax
	mov	DWORD PTR _rdh$[ebp+24], ecx

; 44   : 	rdh.rcBound.bottom = dib.GetHeight();

	mov	ecx, DWORD PTR [esi+80]
	mov	DWORD PTR _rdh$[ebp], eax

; 45   : 	arrData.InsertAt( 0, (BYTE*)&rdh, sizeof( rdh ) );

	push	eax
	lea	eax, DWORD PTR _rdh$[ebp]
	mov	DWORD PTR _rdh$[ebp+28], ecx
	push	eax
	push	ebx
	lea	ecx, DWORD PTR _arrData$[ebp]
	mov	DWORD PTR _rdh$[ebp+4], 1
	mov	DWORD PTR _rdh$[ebp+12], ebx
	mov	DWORD PTR _rdh$[ebp+16], ebx
	mov	DWORD PTR _rdh$[ebp+20], ebx
	call	?InsertAt@?$CArray@E@Container@@QAEXIPBEI@Z ; Container::CArray<unsigned char>::InsertAt

; 46   : 
; 47   : 	HRGN hrgn = ExtCreateRegion( NULL, arrData.GetSize(), (RGNDATA *)arrData.GetData() );

	push	DWORD PTR _arrData$[ebp+4]
	push	DWORD PTR _arrData$[ebp+16]
	push	ebx
	call	DWORD PTR __imp__ExtCreateRegion@12

; 48   : 	return hrgn;

	lea	ecx, DWORD PTR _arrData$[ebp]
	mov	esi, eax
	mov	DWORD PTR _arrData$[ebp], OFFSET FLAT:??_7?$CArray@E@Container@@6B@ ; Container::CArray<unsigned char>::`vftable'
	call	?RemoveAll@?$CArray@E@Container@@QAEXXZ	; Container::CArray<unsigned char>::RemoveAll
	mov	eax, esi
	pop	edi
	pop	esi
	pop	ebx

; 49   : }

	leave
	ret	0
?DIBCreateRegionFromTransparency@GS@@YAPAUHRGN__@@AAVCDIB@1@@Z ENDP ; GS::DIBCreateRegionFromTransparency
_TEXT	ENDS
PUBLIC	?SetSize@?$CArray@E@Container@@QAE_NI@Z		; Container::CArray<unsigned char>::SetSize
EXTRN	__imp__memmove:NEAR
;	COMDAT ?InsertAt@?$CArray@E@Container@@QAEXIPBEI@Z
_TEXT	SEGMENT
_iPos$ = 8
_p$ = 12
_uCount$ = 16
_uItemsToMove$55724 = 16
?InsertAt@?$CArray@E@Container@@QAEXIPBEI@Z PROC NEAR	; Container::CArray<unsigned char>::InsertAt, COMDAT

; 150  : {

	push	ebp
	mov	ebp, esp

; 151  : 	if( p )

	cmp	DWORD PTR _p$[ebp], 0
	push	ebx
	push	esi
	mov	esi, ecx
	je	SHORT $L55978

; 152  : 	{
; 153  : 		//
; 154  : 		// Cannot handle adding to its self
; 155  : 		ASSERT( m_pItems != p );
; 156  : 		ASSERT_VALID_READOBJPTR( p );
; 157  : 		//ASSERT( !IsBadReadPtr( p, sizeof( T ) * uCount ) );
; 158  : 
; 159  : 		if( uCount == 0 )

	mov	ebx, DWORD PTR _uCount$[ebp]
	test	ebx, ebx
	je	SHORT $L55978

; 160  : 			return;
; 161  : 
; 162  : 		PI_uint32 uItemsToMove = m_uItemCount - iPos;

	mov	eax, DWORD PTR [esi+16]
	push	edi
	mov	edi, DWORD PTR _iPos$[ebp]
	mov	ecx, eax
	sub	ecx, edi

; 163  : 
; 164  : 		ASSERT( iPos <= m_uItemCount );
; 165  : 		SetSize( m_uItemCount + uCount );

	add	eax, ebx
	mov	DWORD PTR _uItemsToMove$55724[ebp], ecx
	push	eax
	mov	ecx, esi
	call	?SetSize@?$CArray@E@Container@@QAE_NI@Z	; Container::CArray<unsigned char>::SetSize

; 166  : 
; 167  : 		//
; 168  : 		//	Move Elements above insertion point up one to make
; 169  : 		//	room for our new item
; 170  : 		MoveItemsMemOverlap( &m_pItems[iPos], &m_pItems[iPos + uCount], uItemsToMove );

	mov	eax, DWORD PTR [esi+4]
	push	DWORD PTR _uItemsToMove$55724[ebp]
	add	eax, edi
	push	eax

; 171  : 
; 172  : 		CopyItems( p, m_pItems + iPos, uCount );

	add	eax, ebx
	push	eax
	call	DWORD PTR __imp__memmove
	mov	esi, DWORD PTR [esi+4]
	lea	ecx, DWORD PTR [ebx]
	add	esp, 12					; 0000000cH
	lea	edx, DWORD PTR [esi+edi]
	mov	eax, edx
	sub	eax, esi
	sub	eax, edi
	mov	edi, edx
	add	eax, DWORD PTR _p$[ebp]
	mov	esi, eax
	mov	eax, ecx
	shr	ecx, 2
	rep movsd
	mov	ecx, eax
	and	ecx, 3
	rep movsb
	pop	edi
$L55978:
	pop	esi
	pop	ebx

; 173  : 	}
; 174  : }

	pop	ebp
	ret	12					; 0000000cH
?InsertAt@?$CArray@E@Container@@QAEXIPBEI@Z ENDP	; Container::CArray<unsigned char>::InsertAt
_TEXT	ENDS
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_G?$CArray@E@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CArray@E@Container@@UAEPAXI@Z PROC NEAR		; Container::CArray<unsigned char>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@E@Container@@6B@ ; Container::CArray<unsigned char>::`vftable'
	call	?RemoveAll@?$CArray@E@Container@@QAEXXZ	; Container::CArray<unsigned char>::RemoveAll
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L55751
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L55751:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CArray@E@Container@@UAEPAXI@Z ENDP		; Container::CArray<unsigned char>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@E@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CArray@E@Container@@QAEXXZ PROC NEAR	; Container::CArray<unsigned char>::RemoveAll, COMDAT

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
?RemoveAll@?$CArray@E@Container@@QAEXXZ ENDP		; Container::CArray<unsigned char>::RemoveAll
_TEXT	ENDS
PUBLIC	?RemoveAt@?$CArray@E@Container@@QAEXII@Z	; Container::CArray<unsigned char>::RemoveAt
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
;	COMDAT ?SetSize@?$CArray@E@Container@@QAE_NI@Z
_TEXT	SEGMENT
_uSize$ = 8
_bReallocationNeeded$ = -1
_uAllocateExtra$55780 = -8
_pItems$55782 = -12
?SetSize@?$CArray@E@Container@@QAE_NI@Z PROC NEAR	; Container::CArray<unsigned char>::SetSize, COMDAT

; 46   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH

; 47   : 	bool bReallocationNeeded = false;

	and	BYTE PTR _bReallocationNeeded$[ebp], 0
	push	ebx
	mov	ebx, ecx
	push	esi

; 48   : 	ASSERT( m_uItemCount <= m_uItemsAllocated );
; 49   : 
; 50   : 
; 51   : 	if( m_uItemCount < uSize )

	mov	esi, DWORD PTR _uSize$[ebp]
	push	edi
	mov	eax, DWORD PTR [ebx+16]
	cmp	eax, esi
	jae	$L56055

; 52   : 	{
; 53   : 		if( m_uItemsAllocated < uSize )

	mov	eax, DWORD PTR [ebx+12]
	cmp	eax, esi
	jae	SHORT $L55779

; 54   : 		{
; 55   : 			PI_uint32 uAllocateExtra = m_uGrowBy = m_uGrowBy < 262144 ? m_uGrowBy << 2 : 262144;

	mov	ecx, DWORD PTR [ebx+8]
	mov	edi, 262144				; 00040000H
	cmp	ecx, edi
	jae	SHORT $L56016
	mov	edi, ecx
	shl	edi, 2
$L56016:

; 56   : 
; 57   : 			//
; 58   : 			// Check to see if our grow by is enough?
; 59   : 			if( m_uItemsAllocated + uAllocateExtra < uSize )

	lea	ecx, DWORD PTR [eax+edi]
	mov	DWORD PTR [ebx+8], edi
	cmp	ecx, esi
	mov	DWORD PTR _uAllocateExtra$55780[ebp], edi
	jae	SHORT $L55781

; 60   : 			{
; 61   : 				//
; 62   : 				//	Nope, so we allocate more
; 63   : 				uAllocateExtra = uSize - m_uItemsAllocated;

	mov	ecx, esi
	sub	ecx, eax
	mov	DWORD PTR _uAllocateExtra$55780[ebp], ecx
	mov	edi, ecx
$L55781:

; 64   : 			}
; 65   : 
; 66   : 			T *pItems = reinterpret_cast<T *>( new PI_byte[sizeof( T ) * ( m_uItemsAllocated + uAllocateExtra )] );

	add	eax, edi
	push	eax
	call	??2@YAPAXI@Z				; operator new
	pop	ecx
	mov	DWORD PTR _pItems$55782[ebp], eax

; 67   : 
; 68   : 			if( m_uItemCount )

	mov	ecx, DWORD PTR [ebx+16]
	test	ecx, ecx
	je	SHORT $L56020

; 69   : 				MoveItemsNoMemOverlap( m_pItems, pItems, m_uItemCount );

	mov	esi, DWORD PTR [ebx+4]
	mov	edi, eax
	mov	eax, ecx
	shr	ecx, 2
	rep movsd
	mov	ecx, eax
	and	ecx, 3
	rep movsb
	mov	edi, DWORD PTR _uAllocateExtra$55780[ebp]
	mov	esi, DWORD PTR _uSize$[ebp]
$L56020:

; 70   : 			delete[] (PI_byte*)m_pItems;

	push	DWORD PTR [ebx+4]
	call	??3@YAXPAX@Z				; operator delete

; 71   : 			m_pItems = pItems;

	mov	eax, DWORD PTR _pItems$55782[ebp]

; 72   : 			m_uItemsAllocated += uAllocateExtra;

	add	DWORD PTR [ebx+12], edi
	pop	ecx
	mov	DWORD PTR [ebx+4], eax

; 73   : 			bReallocationNeeded = true;

	mov	BYTE PTR _bReallocationNeeded$[ebp], 1
$L55779:

; 74   : 		}
; 75   : 
; 76   : 		//
; 77   : 		// Constuct the new items
; 78   : 		if( uSize > m_uItemCount )

	mov	eax, DWORD PTR [ebx+16]
	cmp	esi, eax
	jbe	SHORT $L55804

; 79   : 		{
; 80   : 			PI_uint32 uItemsToConstruct = uSize - m_uItemCount;
; 81   : 			ConstructItems( &m_pItems[m_uItemCount], uItemsToConstruct );

	mov	edi, DWORD PTR [ebx+4]
	mov	ecx, esi
	sub	ecx, eax
	add	edi, eax
	mov	edx, ecx
	xor	eax, eax
	shr	ecx, 2
	rep stosd
	mov	ecx, edx

; 82   : 			m_uItemCount = uSize;

	mov	DWORD PTR [ebx+16], esi
	and	ecx, 3
	rep stosb

; 83   : 		}
; 84   : 
; 85   : 	}
; 86   : 	else

	jmp	SHORT $L55804
$L56055:

; 87   : 	{
; 88   : 		//
; 89   : 		//	Check to see if we need to reduce the size
; 90   : 		if( m_uItemCount > uSize )

	jbe	SHORT $L55804

; 91   : 		{
; 92   : 			//
; 93   : 			//	Get rid of excess elements
; 94   : 			RemoveAt( uSize, m_uItemCount - uSize );

	sub	eax, esi
	mov	ecx, ebx
	push	eax
	push	esi
	call	?RemoveAt@?$CArray@E@Container@@QAEXII@Z ; Container::CArray<unsigned char>::RemoveAt
$L55804:

; 95   : 		}
; 96   : 	}
; 97   : 
; 98   : 	return bReallocationNeeded;

	mov	al, BYTE PTR _bReallocationNeeded$[ebp]
	pop	edi
	pop	esi
	pop	ebx

; 99   : }

	leave
	ret	4
?SetSize@?$CArray@E@Container@@QAE_NI@Z ENDP		; Container::CArray<unsigned char>::SetSize
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CArray@E@Container@@QAEXII@Z
_TEXT	SEGMENT
_iPos$ = 8
_uItems$ = 12
?RemoveAt@?$CArray@E@Container@@QAEXII@Z PROC NEAR	; Container::CArray<unsigned char>::RemoveAt, COMDAT

; 191  : 	ASSERT( iPos + uItems <= m_uItemCount );
; 192  : 	ASSERT( uItems > 0 );
; 193  : 
; 194  : 	DestructItems( &m_pItems[iPos], uItems );

	mov	edx, DWORD PTR _iPos$[esp-4]
	push	esi
	mov	esi, ecx
	push	edi

; 195  : 
; 196  : 	//
; 197  : 	// Move Any items above removeal area down
; 198  : 	const PI_uint32 uItemsToMove = m_uItemCount - iPos - uItems;

	mov	edi, DWORD PTR _uItems$[esp+4]
	mov	eax, DWORD PTR [esi+4]
	lea	ecx, DWORD PTR [eax+edx]
	mov	eax, DWORD PTR [esi+16]
	sub	eax, edx
	sub	eax, edi

; 199  : 	if( uItemsToMove )

	je	SHORT $L56066

; 200  : 		MoveItemsMemOverlap( &m_pItems[iPos + uItems], &m_pItems[iPos], uItemsToMove);

	push	eax

; 201  : 	m_uItemCount -= uItems;

	lea	eax, DWORD PTR [ecx+edi]
	push	eax
	push	ecx
	call	DWORD PTR __imp__memmove
	add	esp, 12					; 0000000cH
$L56066:
	sub	DWORD PTR [esi+16], edi
	pop	edi
	pop	esi

; 202  : }

	ret	8
?RemoveAt@?$CArray@E@Container@@QAEXII@Z ENDP		; Container::CArray<unsigned char>::RemoveAt
_TEXT	ENDS
END
