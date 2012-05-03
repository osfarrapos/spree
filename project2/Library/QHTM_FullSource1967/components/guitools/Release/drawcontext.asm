	TITLE	D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\drawcontext.cpp
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
;	COMDAT ??0FontDef@GS@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
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
;	COMDAT ??4CSize@WinHelper@@QAEAAV01@ABUtagSIZE@@@Z
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
;	COMDAT ??2@YAPAXIPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetBitmap@CDIB@GS@@QAEPAUHBITMAP__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsPrinting@CDrawContext@GS@@QBE_NXZ
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
;	COMDAT ?DrawTextA@CDrawContext@GS@@QAEXHHPBDHK@Z
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
;	COMDAT ?HashIt@Container@@YAIPBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CObjectCache@GS@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Clear@CObjectCache@GS@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFocusBrush@CObjectCache@GS@@QAEPAUHBRUSH__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCObjectCache@GS@@QAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CObjectCache@GS@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCDrawContext@GS@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAUtagRGBQUAD@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CArray@PAX@Container@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAX@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Add@?$CArray@PAX@Container@@QAEXABQAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@PAX@Container@@QAEAAPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAX@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CArray@PAX@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAE@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEPAPAUFontInfo@CDrawContext@GS@@ABUFontDef@5@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXABUFontDef@GS@@PAUFontInfo@CDrawContext@4@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CMap@KPAUHBRUSH__@@@Container@@QAE@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@KPAUHBRUSH__@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@KPAUHBRUSH__@@@Container@@QAEPAPAUHBRUSH__@@ABK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@KPAUHBRUSH__@@@Container@@QAEXABKPAUHBRUSH__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@KPAUHBRUSH__@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CMap@KPAUHPEN__@@@Container@@QAE@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@KPAUHPEN__@@@Container@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@KPAUHPEN__@@@Container@@QAEPAPAUHPEN__@@ABK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@KPAUHPEN__@@@Container@@QAEXABKPAUHPEN__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@KPAUHPEN__@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAE@ABV?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Next@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EOL@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetValue@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QBEAAPAUFontInfo@CDrawContext@GS@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAUtagRGBQUAD@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAX@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@KPAUHBRUSH__@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@KPAUHPEN__@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$MapBucketEntry@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$MapBucketEntry@KPAUHBRUSH__@@@Container@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$MapBucketEntry@KPAUHPEN__@@@Container@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAUtagRGBQUAD@@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@PAX@Container@@QAE_NI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetBucketSize@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Find@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@ABEPAV?$MapBucketEntry@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@2@IABUFontDef@GS@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetBucketSize@?$CMap@KPAUHBRUSH__@@@Container@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Find@?$CMap@KPAUHBRUSH__@@@Container@@ABEPAV?$MapBucketEntry@KPAUHBRUSH__@@@2@IABK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashIt@Container@@YAIABK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetBucketSize@?$CMap@KPAUHPEN__@@@Container@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Find@?$CMap@KPAUHPEN__@@@Container@@ABEPAV?$MapBucketEntry@KPAUHPEN__@@@2@IABK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Reset@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CMap@KPAUHBRUSH__@@@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CMap@KPAUHPEN__@@@Container@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructItems@Container@@YAXPAPAUtagRGBQUAD@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CArray@PAX@Container@@QAEXII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsNoMemOverlap@Container@@YAXPAPAX0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructItems@Container@@YAXPAPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ElementsTheSame@Container@@YA_NABK0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveItemsMemOverlap@Container@@YAXPAPAX0I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7?$CMap@KPAUHPEN__@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CMap@KPAUHBRUSH__@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CArray@PAX@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CArray@PAUtagRGBQUAD@@@Container@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __tcsinc
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __tcsdec
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7CDrawContext@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __tcscmp
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __tcsicmp
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7CBufferedDC@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CBufferedDCDIB@GS@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
CONST	SEGMENT
_g_FocusBitmap DW 0aaH
	DW	055H
CONST	ENDS
_DATA	SEGMENT
_g_pcszEllipsis DD FLAT:$SG56193
_g_nEllipsisLength DD 03H
$SG56193 DB	'...', 00H
_DATA	ENDS
PUBLIC	?Startup@CDrawContext@GS@@SAXXZ			; GS::CDrawContext::Startup
PUBLIC	??0CObjectCache@GS@@QAE@XZ			; GS::CObjectCache::CObjectCache
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
_BSS	SEGMENT
_m_pocPrint DD	01H DUP (?)
_m_pocDisplay DD 01H DUP (?)
_BSS	ENDS
_TEXT	SEGMENT
?Startup@CDrawContext@GS@@SAXXZ PROC NEAR		; GS::CDrawContext::Startup

; 133  : 	m_pocPrint = new GS::CObjectCache;

	push	72					; 00000048H
	call	??2@YAPAXI@Z				; operator new
	test	eax, eax
	pop	ecx
	je	SHORT $L57278
	mov	ecx, eax
	call	??0CObjectCache@GS@@QAE@XZ		; GS::CObjectCache::CObjectCache
	mov	DWORD PTR _m_pocPrint, eax
	jmp	SHORT $L57279
$L57278:
	and	DWORD PTR _m_pocPrint, 0
$L57279:

; 134  : 	m_pocDisplay = new GS::CObjectCache;

	push	72					; 00000048H
	call	??2@YAPAXI@Z				; operator new
	test	eax, eax
	pop	ecx
	je	SHORT $L57281
	mov	ecx, eax
	call	??0CObjectCache@GS@@QAE@XZ		; GS::CObjectCache::CObjectCache
	mov	DWORD PTR _m_pocDisplay, eax

; 135  : }

	ret	0
$L57281:

; 134  : 	m_pocDisplay = new GS::CObjectCache;

	and	DWORD PTR _m_pocDisplay, 0

; 135  : }

	ret	0
?Startup@CDrawContext@GS@@SAXXZ ENDP			; GS::CDrawContext::Startup
_TEXT	ENDS
PUBLIC	?SetBucketSize@?$CMap@KPAUHBRUSH__@@@Container@@QAEXI@Z ; Container::CMap<unsigned long,HBRUSH__ *>::SetBucketSize
PUBLIC	??_7?$CMap@KPAUHBRUSH__@@@Container@@6B@	; Container::CMap<unsigned long,HBRUSH__ *>::`vftable'
PUBLIC	??_G?$CMap@KPAUHBRUSH__@@@Container@@UAEPAXI@Z	; Container::CMap<unsigned long,HBRUSH__ *>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@KPAUHBRUSH__@@@Container@@UAEPAXI@Z	; Container::CMap<unsigned long,HBRUSH__ *>::`vector deleting destructor'
PUBLIC	?SetBucketSize@?$CMap@KPAUHPEN__@@@Container@@QAEXI@Z ; Container::CMap<unsigned long,HPEN__ *>::SetBucketSize
PUBLIC	??_7?$CMap@KPAUHPEN__@@@Container@@6B@		; Container::CMap<unsigned long,HPEN__ *>::`vftable'
PUBLIC	??_G?$CMap@KPAUHPEN__@@@Container@@UAEPAXI@Z	; Container::CMap<unsigned long,HPEN__ *>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@KPAUHPEN__@@@Container@@UAEPAXI@Z	; Container::CMap<unsigned long,HPEN__ *>::`vector deleting destructor'
PUBLIC	??_7?$CArray@PAX@Container@@6B@			; Container::CArray<void *>::`vftable'
PUBLIC	??_G?$CArray@PAX@Container@@UAEPAXI@Z		; Container::CArray<void *>::`scalar deleting destructor'
PUBLIC	??_E?$CArray@PAX@Container@@UAEPAXI@Z		; Container::CArray<void *>::`vector deleting destructor'
PUBLIC	?SetBucketSize@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXI@Z ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::SetBucketSize
PUBLIC	??_7?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@6B@ ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::`vftable'
PUBLIC	??_G?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAEPAXI@Z ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAEPAXI@Z ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::`vector deleting destructor'
;	COMDAT ??_7?$CMap@KPAUHBRUSH__@@@Container@@6B@
; File ../map.inl
CONST	SEGMENT
??_7?$CMap@KPAUHBRUSH__@@@Container@@6B@ DD FLAT:??_E?$CMap@KPAUHBRUSH__@@@Container@@UAEPAXI@Z ; Container::CMap<unsigned long,HBRUSH__ *>::`vftable'
CONST	ENDS
;	COMDAT ??_7?$CArray@PAX@Container@@6B@
CONST	SEGMENT
??_7?$CArray@PAX@Container@@6B@ DD FLAT:??_E?$CArray@PAX@Container@@UAEPAXI@Z ; Container::CArray<void *>::`vftable'
CONST	ENDS
;	COMDAT ??_7?$CMap@KPAUHPEN__@@@Container@@6B@
CONST	SEGMENT
??_7?$CMap@KPAUHPEN__@@@Container@@6B@ DD FLAT:??_E?$CMap@KPAUHPEN__@@@Container@@UAEPAXI@Z ; Container::CMap<unsigned long,HPEN__ *>::`vftable'
CONST	ENDS
;	COMDAT ??_7?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@6B@
CONST	SEGMENT
??_7?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@6B@ DD FLAT:??_E?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAEPAXI@Z ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::`vftable'
CONST	ENDS
;	COMDAT ??0CObjectCache@GS@@QAE@XZ
_TEXT	SEGMENT
??0CObjectCache@GS@@QAE@XZ PROC NEAR			; GS::CObjectCache::CObjectCache, COMDAT

; 72   : 			{};

	push	esi
	mov	esi, ecx
	push	edi
	xor	edi, edi
	lea	ecx, DWORD PTR [esi+20]
	mov	DWORD PTR [esi+8], 1
	mov	DWORD PTR [esi+12], edi
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@PAX@Container@@6B@ ; Container::CArray<void *>::`vftable'
	push	397					; 0000018dH
	mov	DWORD PTR [esi+4], edi
	mov	DWORD PTR [esi+16], edi
	mov	DWORD PTR [ecx+8], edi
	mov	DWORD PTR [ecx+12], edi
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@6B@ ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::`vftable'
	call	?SetBucketSize@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXI@Z ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::SetBucketSize
	lea	ecx, DWORD PTR [esi+36]
	push	97					; 00000061H
	mov	DWORD PTR [ecx+8], edi
	mov	DWORD PTR [ecx+12], edi
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7?$CMap@KPAUHBRUSH__@@@Container@@6B@ ; Container::CMap<unsigned long,HBRUSH__ *>::`vftable'
	call	?SetBucketSize@?$CMap@KPAUHBRUSH__@@@Container@@QAEXI@Z ; Container::CMap<unsigned long,HBRUSH__ *>::SetBucketSize
	lea	ecx, DWORD PTR [esi+52]
	push	97					; 00000061H
	mov	DWORD PTR [ecx+8], edi
	mov	DWORD PTR [ecx+12], edi
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7?$CMap@KPAUHPEN__@@@Container@@6B@ ; Container::CMap<unsigned long,HPEN__ *>::`vftable'
	call	?SetBucketSize@?$CMap@KPAUHPEN__@@@Container@@QAEXI@Z ; Container::CMap<unsigned long,HPEN__ *>::SetBucketSize
	mov	eax, esi
	pop	edi
	pop	esi
	ret	0
??0CObjectCache@GS@@QAE@XZ ENDP				; GS::CObjectCache::CObjectCache
_TEXT	ENDS
PUBLIC	??1CObjectCache@GS@@QAE@XZ			; GS::CObjectCache::~CObjectCache
PUBLIC	?Shutdown@CDrawContext@GS@@SAXXZ		; GS::CDrawContext::Shutdown
PUBLIC	?Clear@CObjectCache@GS@@QAEXXZ			; GS::CObjectCache::Clear
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
_TEXT	SEGMENT
?Shutdown@CDrawContext@GS@@SAXXZ PROC NEAR		; GS::CDrawContext::Shutdown

; 140  : 	m_pocDisplay->Clear();

	mov	ecx, DWORD PTR _m_pocDisplay
	push	esi
	call	?Clear@CObjectCache@GS@@QAEXXZ		; GS::CObjectCache::Clear

; 141  : 	m_pocPrint->Clear();

	mov	ecx, DWORD PTR _m_pocPrint
	call	?Clear@CObjectCache@GS@@QAEXXZ		; GS::CObjectCache::Clear

; 142  : 
; 143  : 	delete m_pocPrint;

	mov	ecx, DWORD PTR _m_pocPrint
	test	ecx, ecx
	mov	esi, ecx
	je	SHORT $L57303
	call	??1CObjectCache@GS@@QAE@XZ		; GS::CObjectCache::~CObjectCache
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L57303:

; 144  : 	delete m_pocDisplay;

	mov	ecx, DWORD PTR _m_pocDisplay
	test	ecx, ecx
	mov	esi, ecx
	je	SHORT $L57307
	call	??1CObjectCache@GS@@QAE@XZ		; GS::CObjectCache::~CObjectCache
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L57307:
	pop	esi

; 145  : }

	ret	0
?Shutdown@CDrawContext@GS@@SAXXZ ENDP			; GS::CDrawContext::Shutdown
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CMap@KPAUHBRUSH__@@@Container@@QAEXXZ ; Container::CMap<unsigned long,HBRUSH__ *>::RemoveAll
PUBLIC	?RemoveAll@?$CMap@KPAUHPEN__@@@Container@@QAEXXZ ; Container::CMap<unsigned long,HPEN__ *>::RemoveAll
PUBLIC	?Reset@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ; Container::CMapIter<GS::FontDef,GS::CDrawContext::FontInfo *>::Reset
PUBLIC	?Next@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ; Container::CMapIter<GS::FontDef,GS::CDrawContext::FontInfo *>::Next
PUBLIC	?RemoveAll@?$CArray@PAX@Container@@QAEXXZ	; Container::CArray<void *>::RemoveAll
PUBLIC	?RemoveAll@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::RemoveAll
EXTRN	__imp__DeleteObject@4:NEAR
;	COMDAT ?Clear@CObjectCache@GS@@QAEXXZ
_TEXT	SEGMENT
_itr$ = -16
?Clear@CObjectCache@GS@@QAEXXZ PROC NEAR		; GS::CObjectCache::Clear, COMDAT

; 83   : 		{

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	ebx
	push	esi
	mov	esi, ecx
	push	edi

; 84   : 			//TRACE( _T("There were %d GDI objects cached.\n"), m_arrObjToDelete.GetSize());
; 85   : 			const UINT uSize = m_arrObjToDelete.GetSize();
; 86   : 			for( UINT n = 0; n < uSize; n++ )

	xor	edi, edi
	mov	ebx, DWORD PTR [esi+16]
	test	ebx, ebx
	jbe	SHORT $L56433
$L56431:

; 87   : 			{
; 88   : 				VAPI( ::DeleteObject( m_arrObjToDelete[ n ] ) );

	mov	eax, DWORD PTR [esi+4]
	push	DWORD PTR [eax+edi*4]
	call	DWORD PTR __imp__DeleteObject@4
	inc	edi
	cmp	edi, ebx
	jb	SHORT $L56431
$L56433:

; 89   : 			}
; 90   : 			m_arrObjToDelete.RemoveAll();

	mov	ecx, esi
	call	?RemoveAll@?$CArray@PAX@Container@@QAEXXZ ; Container::CArray<void *>::RemoveAll

; 91   : 
; 92   : 			//TRACE( _T("There were %d fonts cached.\n"), m_mapFonts.GetSize());
; 93   : 			for( Container::CMapIter<FontDef, CDrawContext::FontInfo*> itr( m_mapFonts ); !itr.EOL(); itr.Next() )

	lea	edi, DWORD PTR [esi+20]
	xor	ebx, ebx
	lea	ecx, DWORD PTR _itr$[ebp]
	mov	DWORD PTR _itr$[ebp], edi
	mov	DWORD PTR _itr$[ebp+4], ebx
	mov	DWORD PTR _itr$[ebp+12], ebx
	call	?Reset@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ; Container::CMapIter<GS::FontDef,GS::CDrawContext::FontInfo *>::Reset
$L57390:
	cmp	DWORD PTR _itr$[ebp+4], ebx
	je	SHORT $L56459

; 94   : 			{
; 95   : 				delete itr.GetValue();

	mov	eax, DWORD PTR _itr$[ebp+4]
	push	DWORD PTR [eax]
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
	lea	ecx, DWORD PTR _itr$[ebp]
	call	?Next@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ; Container::CMapIter<GS::FontDef,GS::CDrawContext::FontInfo *>::Next
	jmp	SHORT $L57390
$L56459:

; 96   : 			}
; 97   : 			m_mapFonts.RemoveAll();

	mov	ecx, edi
	call	?RemoveAll@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::RemoveAll

; 98   : 			m_mapBrushes.RemoveAll();

	lea	ecx, DWORD PTR [esi+36]
	call	?RemoveAll@?$CMap@KPAUHBRUSH__@@@Container@@QAEXXZ ; Container::CMap<unsigned long,HBRUSH__ *>::RemoveAll

; 99   : 			m_mapPens.RemoveAll();

	lea	ecx, DWORD PTR [esi+52]
	call	?RemoveAll@?$CMap@KPAUHPEN__@@@Container@@QAEXXZ ; Container::CMap<unsigned long,HPEN__ *>::RemoveAll
	pop	edi
	pop	esi
	pop	ebx

; 100  : 		}

	leave
	ret	0
?Clear@CObjectCache@GS@@QAEXXZ ENDP			; GS::CObjectCache::Clear
_TEXT	ENDS
PUBLIC	??1?$CMap@KPAUHBRUSH__@@@Container@@UAE@XZ	; Container::CMap<unsigned long,HBRUSH__ *>::~CMap<unsigned long,HBRUSH__ *>
PUBLIC	??1?$CMap@KPAUHPEN__@@@Container@@UAE@XZ	; Container::CMap<unsigned long,HPEN__ *>::~CMap<unsigned long,HPEN__ *>
PUBLIC	??1?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAE@XZ ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::~CMap<GS::FontDef,GS::CDrawContext::FontInfo *>
;	COMDAT ??1CObjectCache@GS@@QAE@XZ
_TEXT	SEGMENT
??1CObjectCache@GS@@QAE@XZ PROC NEAR			; GS::CObjectCache::~CObjectCache, COMDAT
	push	esi
	mov	esi, ecx
	lea	ecx, DWORD PTR [esi+52]
	call	??1?$CMap@KPAUHPEN__@@@Container@@UAE@XZ ; Container::CMap<unsigned long,HPEN__ *>::~CMap<unsigned long,HPEN__ *>
	lea	ecx, DWORD PTR [esi+36]
	call	??1?$CMap@KPAUHBRUSH__@@@Container@@UAE@XZ ; Container::CMap<unsigned long,HBRUSH__ *>::~CMap<unsigned long,HBRUSH__ *>
	lea	ecx, DWORD PTR [esi+20]
	call	??1?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAE@XZ ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::~CMap<GS::FontDef,GS::CDrawContext::FontInfo *>
	mov	ecx, esi
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@PAX@Container@@6B@ ; Container::CArray<void *>::`vftable'
	call	?RemoveAll@?$CArray@PAX@Container@@QAEXXZ ; Container::CArray<void *>::RemoveAll
	pop	esi
	ret	0
??1CObjectCache@GS@@QAE@XZ ENDP				; GS::CObjectCache::~CObjectCache
_TEXT	ENDS
PUBLIC	?ClearAllObjects@CDrawContext@GS@@SAXXZ		; GS::CDrawContext::ClearAllObjects
_TEXT	SEGMENT
?ClearAllObjects@CDrawContext@GS@@SAXXZ PROC NEAR	; GS::CDrawContext::ClearAllObjects

; 152  : 	m_pocDisplay->Clear();

	mov	ecx, DWORD PTR _m_pocDisplay
	call	?Clear@CObjectCache@GS@@QAEXXZ		; GS::CObjectCache::Clear

; 153  : 	m_pocPrint->Clear();

	mov	ecx, DWORD PTR _m_pocPrint
	jmp	?Clear@CObjectCache@GS@@QAEXXZ		; GS::CObjectCache::Clear
?ClearAllObjects@CDrawContext@GS@@SAXXZ ENDP		; GS::CDrawContext::ClearAllObjects
_TEXT	ENDS
PUBLIC	??0CDrawContext@GS@@QAE@PBVCRect@WinHelper@@PAUHDC__@@_N@Z ; GS::CDrawContext::CDrawContext
PUBLIC	??_7CDrawContext@GS@@6B@			; GS::CDrawContext::`vftable'
PUBLIC	??_GCDrawContext@GS@@UAEPAXI@Z			; GS::CDrawContext::`scalar deleting destructor'
PUBLIC	??_ECDrawContext@GS@@UAEPAXI@Z			; GS::CDrawContext::`vector deleting destructor'
EXTRN	__imp__SaveDC@4:NEAR
EXTRN	__imp__SetBkMode@8:NEAR
EXTRN	__imp__GetDC@4:NEAR
EXTRN	__imp__ReleaseDC@8:NEAR
EXTRN	__imp__GetDeviceCaps@8:NEAR
;	COMDAT ??_7CDrawContext@GS@@6B@
; File D:\Work\EmperorOfDragons\Library\QHTM_FullSource1967\components\guitools\DrawContext.h
CONST	SEGMENT
??_7CDrawContext@GS@@6B@ DD FLAT:??_ECDrawContext@GS@@UAEPAXI@Z ; GS::CDrawContext::`vftable'
CONST	ENDS
_TEXT	SEGMENT
_prcClip$ = 8
_hdc$ = 12
_bIsPrinting$ = 16
??0CDrawContext@GS@@QAE@PBVCRect@WinHelper@@PAUHDC__@@_N@Z PROC NEAR ; GS::CDrawContext::CDrawContext

; 191  : {

	mov	eax, DWORD PTR _hdc$[esp-4]
	push	ebx
	push	esi
	mov	ebx, ecx

; 192  : 	if( prcClip )

	mov	esi, DWORD PTR _prcClip$[esp+4]
	push	edi
	and	BYTE PTR [ebx+4], 0
	mov	cl, BYTE PTR _bIsPrinting$[esp+8]
	push	1
	mov	DWORD PTR [ebx+8], eax
	pop	eax
	mov	BYTE PTR [ebx+72], cl
	mov	DWORD PTR [ebx+28], eax
	mov	DWORD PTR [ebx+32], eax
	mov	DWORD PTR [ebx+36], eax
	mov	DWORD PTR [ebx+40], eax
	xor	eax, eax
	mov	DWORD PTR [ebx], OFFSET FLAT:??_7CDrawContext@GS@@6B@ ; GS::CDrawContext::`vftable'
	and	BYTE PTR [ebx+52], al
	and	BYTE PTR [ebx+60], al
	cmp	esi, eax
	mov	DWORD PTR [ebx+44], eax
	mov	DWORD PTR [ebx+48], eax
	mov	DWORD PTR [ebx+68], eax
	je	SHORT $L56504

; 193  : 		m_rcClip = *prcClip;

	lea	edi, DWORD PTR [ebx+12]
	movsd
	movsd
	movsd
	movsd

; 194  : 	else

	jmp	SHORT $L57419
$L56504:

; 195  : 		m_rcClip.Set(0,0,0,0);

	mov	DWORD PTR [ebx+24], eax
	mov	DWORD PTR [ebx+16], eax
	mov	DWORD PTR [ebx+20], eax
	mov	DWORD PTR [ebx+12], eax
$L57419:

; 196  : 
; 197  : 	if( !m_hdc )

	cmp	DWORD PTR [ebx+8], eax
	mov	esi, DWORD PTR __imp__GetDC@4
	jne	SHORT $L56506

; 198  : 	{
; 199  : 		m_hdc = ::GetDC( NULL );

	push	eax
	call	esi
	mov	DWORD PTR [ebx+8], eax

; 200  : 		m_bWeCreatedDC = true;

	mov	BYTE PTR [ebx+4], 1
	xor	eax, eax
$L56506:

; 201  : 	}
; 202  : 
; 203  : 	// Set the device scale if needed
; 204  : 	if( IsPrinting() )

	cmp	BYTE PTR [ebx+72], 0
	je	SHORT $L56507

; 205  : 	{
; 206  : 		m_poc = m_pocPrint;

	mov	ecx, DWORD PTR _m_pocPrint

; 207  : 		HDC screenDC = ::GetDC(NULL);

	push	eax
	mov	DWORD PTR [ebx+48], ecx
	call	esi

; 208  : 		m_cxDeviceScaleNumer = ::GetDeviceCaps(m_hdc, LOGPIXELSX);

	mov	esi, DWORD PTR __imp__GetDeviceCaps@8
	push	88					; 00000058H
	push	DWORD PTR [ebx+8]
	mov	edi, eax
	call	esi

; 209  : 		m_cxDeviceScaleDenom = ::GetDeviceCaps(screenDC, LOGPIXELSX);

	push	88					; 00000058H
	push	edi
	mov	DWORD PTR [ebx+28], eax
	call	esi

; 210  : 		m_cyDeviceScaleNumer = ::GetDeviceCaps(m_hdc, LOGPIXELSY);

	push	90					; 0000005aH
	mov	DWORD PTR [ebx+32], eax
	push	DWORD PTR [ebx+8]
	call	esi

; 211  : 		m_cyDeviceScaleDenom = ::GetDeviceCaps(screenDC, LOGPIXELSY);

	push	90					; 0000005aH
	push	edi
	mov	DWORD PTR [ebx+36], eax
	call	esi

; 212  : 		::ReleaseDC(NULL, screenDC);

	push	edi
	push	0
	mov	DWORD PTR [ebx+40], eax
	call	DWORD PTR __imp__ReleaseDC@8

; 213  : 	}
; 214  : 	else

	xor	eax, eax
	jmp	SHORT $L56509
$L56507:

; 215  : 	{
; 216  : 		m_poc = m_pocDisplay;

	mov	ecx, DWORD PTR _m_pocDisplay
	mov	DWORD PTR [ebx+48], ecx
$L56509:

; 217  : 	}
; 218  : 
; 219  : 	if( !m_poc )

	cmp	DWORD PTR [ebx+48], eax
	jne	SHORT $L56511

; 220  : 	{
; 221  : 		Startup();

	call	?Startup@CDrawContext@GS@@SAXXZ		; GS::CDrawContext::Startup

; 222  : 		if( IsPrinting() )

	cmp	BYTE PTR [ebx+72], 0
	je	SHORT $L56511

; 223  : 		{
; 224  : 			m_poc = m_pocPrint;

	mov	eax, DWORD PTR _m_pocPrint
	mov	DWORD PTR [ebx+48], eax
$L56511:

; 225  : 		}
; 226  : 	}
; 227  : 
; 228  : 	VAPI( ::SaveDC( m_hdc ) );

	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__SaveDC@4

; 229  : 
; 230  : 	::SetBkMode( m_hdc, TRANSPARENT );

	push	1
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__SetBkMode@8
	pop	edi

; 231  : }

	mov	eax, ebx
	pop	esi
	pop	ebx
	ret	12					; 0000000cH
??0CDrawContext@GS@@QAE@PBVCRect@WinHelper@@PAUHDC__@@_N@Z ENDP ; GS::CDrawContext::CDrawContext
_TEXT	ENDS
PUBLIC	??1CDrawContext@GS@@UAE@XZ			; GS::CDrawContext::~CDrawContext
;	COMDAT ??_GCDrawContext@GS@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_GCDrawContext@GS@@UAEPAXI@Z PROC NEAR		; GS::CDrawContext::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1CDrawContext@GS@@UAE@XZ		; GS::CDrawContext::~CDrawContext
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L56516
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L56516:
	mov	eax, esi
	pop	esi
	ret	4
??_GCDrawContext@GS@@UAEPAXI@Z ENDP			; GS::CDrawContext::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	__imp__RestoreDC@8:NEAR
_TEXT	SEGMENT
??1CDrawContext@GS@@UAE@XZ PROC NEAR			; GS::CDrawContext::~CDrawContext

; 235  : {

	push	esi
	mov	esi, ecx

; 236  : 	if( m_hdc )

	mov	eax, DWORD PTR [esi+8]
	mov	DWORD PTR [esi], OFFSET FLAT:??_7CDrawContext@GS@@6B@ ; GS::CDrawContext::`vftable'
	test	eax, eax
	je	SHORT $L56520

; 237  : 		VAPI( ::RestoreDC( m_hdc, -1 ) );

	push	-1
	push	eax
	call	DWORD PTR __imp__RestoreDC@8
$L56520:

; 238  : 
; 239  : 	if( IsPrinting() )

	cmp	BYTE PTR [esi+72], 0
	je	SHORT $L56522

; 240  : 	{
; 241  : 		m_poc->Clear();

	mov	ecx, DWORD PTR [esi+48]
	call	?Clear@CObjectCache@GS@@QAEXXZ		; GS::CObjectCache::Clear
$L56522:

; 242  : 	}
; 243  : 
; 244  : 	if( m_bWeCreatedDC )

	cmp	BYTE PTR [esi+4], 0
	je	SHORT $L56523

; 245  : 	{
; 246  : 		::ReleaseDC( NULL, m_hdc );

	push	DWORD PTR [esi+8]
	push	0
	call	DWORD PTR __imp__ReleaseDC@8

; 247  : 		m_hdc = NULL;

	and	DWORD PTR [esi+8], 0
$L56523:
	pop	esi

; 248  : 	}
; 249  : 
; 250  : }

	ret	0
??1CDrawContext@GS@@UAE@XZ ENDP				; GS::CDrawContext::~CDrawContext
_TEXT	ENDS
PUBLIC	?DrawFocus@CDrawContext@GS@@QAEXABVCRect@WinHelper@@@Z ; GS::CDrawContext::DrawFocus
PUBLIC	?GetFocusBrush@CObjectCache@GS@@QAEPAUHBRUSH__@@XZ ; GS::CObjectCache::GetFocusBrush
EXTRN	__imp__FrameRect@12:NEAR
_TEXT	SEGMENT
_rc$ = 8
?DrawFocus@CDrawContext@GS@@QAEXABVCRect@WinHelper@@@Z PROC NEAR ; GS::CDrawContext::DrawFocus

; 254  : { VAPI( FrameRect( m_hdc, &rc, m_poc->GetFocusBrush() ) ); }

	push	esi
	mov	esi, ecx
	mov	ecx, DWORD PTR [esi+48]
	call	?GetFocusBrush@CObjectCache@GS@@QAEPAUHBRUSH__@@XZ ; GS::CObjectCache::GetFocusBrush
	push	eax
	push	DWORD PTR _rc$[esp+4]
	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__FrameRect@12
	pop	esi
	ret	4
?DrawFocus@CDrawContext@GS@@QAEXABVCRect@WinHelper@@@Z ENDP ; GS::CDrawContext::DrawFocus
_TEXT	ENDS
PUBLIC	?SetSize@?$CArray@PAX@Container@@QAE_NI@Z	; Container::CArray<void *>::SetSize
EXTRN	__imp__CreateBitmap@20:NEAR
EXTRN	__imp__CreatePatternBrush@4:NEAR
;	COMDAT ?GetFocusBrush@CObjectCache@GS@@QAEPAUHBRUSH__@@XZ
_TEXT	SEGMENT
?GetFocusBrush@CObjectCache@GS@@QAEPAUHBRUSH__@@XZ PROC NEAR ; GS::CObjectCache::GetFocusBrush, COMDAT

; 102  : 		{

	push	esi
	mov	esi, ecx

; 103  : 			if( !m_hbrFocus )

	cmp	DWORD PTR [esi+68], 0
	jne	SHORT $L56463
	push	ebx
	push	edi

; 104  : 			{
; 105  : 				HBITMAP hbmpFocus = CreateBitmap( 8, countof( g_FocusBitmap ), 1, 1, g_FocusBitmap );

	push	OFFSET FLAT:_g_FocusBitmap
	push	1
	push	1
	push	2
	push	8
	call	DWORD PTR __imp__CreateBitmap@20
	mov	ebx, eax

; 106  : 				m_hbrFocus = CreatePatternBrush( hbmpFocus );

	push	ebx
	call	DWORD PTR __imp__CreatePatternBrush@4
	mov	edi, eax

; 107  : 				m_arrObjToDelete.Add( m_hbrFocus );

	mov	eax, DWORD PTR [esi+16]
	inc	eax
	mov	ecx, esi
	push	eax
	mov	DWORD PTR [esi+68], edi
	call	?SetSize@?$CArray@PAX@Container@@QAE_NI@Z ; Container::CArray<void *>::SetSize
	mov	eax, DWORD PTR [esi+16]
	mov	ecx, DWORD PTR [esi+4]

; 108  : 				VAPI( ::DeleteObject( hbmpFocus ) );

	push	ebx
	mov	DWORD PTR [ecx+eax*4-4], edi
	call	DWORD PTR __imp__DeleteObject@4
	pop	edi
	pop	ebx
$L56463:

; 109  : 			}
; 110  : 			return m_hbrFocus;

	mov	eax, DWORD PTR [esi+68]
	pop	esi

; 111  : 		}

	ret	0
?GetFocusBrush@CObjectCache@GS@@QAEPAUHBRUSH__@@XZ ENDP	; GS::CObjectCache::GetFocusBrush
_TEXT	ENDS
PUBLIC	?DrawFocus@CDrawContext@GS@@QAEXPAUHRGN__@@@Z	; GS::CDrawContext::DrawFocus
EXTRN	__imp__FrameRgn@20:NEAR
_TEXT	SEGMENT
_rgn$ = 8
?DrawFocus@CDrawContext@GS@@QAEXPAUHRGN__@@@Z PROC NEAR	; GS::CDrawContext::DrawFocus

; 258  : { FrameRgn( m_hdc, rgn, m_poc->GetFocusBrush(), 1, 1 ); }

	push	esi
	mov	esi, ecx
	push	1
	push	1
	mov	ecx, DWORD PTR [esi+48]
	call	?GetFocusBrush@CObjectCache@GS@@QAEPAUHBRUSH__@@XZ ; GS::CObjectCache::GetFocusBrush
	push	eax
	push	DWORD PTR _rgn$[esp+12]
	push	DWORD PTR [esi+8]
	call	DWORD PTR __imp__FrameRgn@20
	pop	esi
	ret	4
?DrawFocus@CDrawContext@GS@@QAEXPAUHRGN__@@@Z ENDP	; GS::CDrawContext::DrawFocus
_TEXT	ENDS
PUBLIC	?Lookup@?$CMap@KPAUHPEN__@@@Container@@QAEPAPAUHPEN__@@ABK@Z ; Container::CMap<unsigned long,HPEN__ *>::Lookup
PUBLIC	?SetAt@?$CMap@KPAUHPEN__@@@Container@@QAEXABKPAUHPEN__@@@Z ; Container::CMap<unsigned long,HPEN__ *>::SetAt
PUBLIC	?GetPen@CDrawContext@GS@@AAEPAUHPEN__@@K@Z	; GS::CDrawContext::GetPen
EXTRN	__imp__CreatePen@12:NEAR
_TEXT	SEGMENT
_cr$ = 8
?GetPen@CDrawContext@GS@@AAEPAUHPEN__@@K@Z PROC NEAR	; GS::CDrawContext::GetPen

; 263  : {

	push	edi
	mov	edi, ecx

; 264  : 	HPEN *pPen = m_poc->m_mapPens.Lookup( cr );

	lea	eax, DWORD PTR _cr$[esp]
	mov	ecx, DWORD PTR [edi+48]
	push	eax
	add	ecx, 52					; 00000034H
	call	?Lookup@?$CMap@KPAUHPEN__@@@Container@@QAEPAPAUHPEN__@@ABK@Z ; Container::CMap<unsigned long,HPEN__ *>::Lookup

; 265  : 	if( pPen )

	test	eax, eax
	je	SHORT $L56538

; 266  : 	{
; 267  : 		return *pPen;

	mov	eax, DWORD PTR [eax]
	jmp	SHORT $L56536
$L56538:

; 268  : 	}
; 269  : 
; 270  : 	HPEN hPen = CreatePen( PS_SOLID, 1, 0x02000000 | cr );

	mov	eax, DWORD PTR _cr$[esp]
	push	ebx
	or	eax, 33554432				; 02000000H
	push	esi
	push	eax
	push	1
	push	0
	call	DWORD PTR __imp__CreatePen@12

; 271  : 	VAPI( hPen );
; 272  : 	m_poc->m_arrObjToDelete.Add( hPen );

	mov	esi, DWORD PTR [edi+48]
	mov	ebx, eax
	mov	ecx, esi
	mov	eax, DWORD PTR [esi+16]
	inc	eax
	push	eax
	call	?SetSize@?$CArray@PAX@Container@@QAE_NI@Z ; Container::CArray<void *>::SetSize
	mov	eax, DWORD PTR [esi+16]
	mov	ecx, DWORD PTR [esi+4]

; 273  : 	m_poc->m_mapPens.SetAt( cr, hPen );

	push	ebx
	mov	DWORD PTR [ecx+eax*4-4], ebx
	mov	ecx, DWORD PTR [edi+48]
	lea	eax, DWORD PTR _cr$[esp+12]
	add	ecx, 52					; 00000034H
	push	eax
	call	?SetAt@?$CMap@KPAUHPEN__@@@Container@@QAEXABKPAUHPEN__@@@Z ; Container::CMap<unsigned long,HPEN__ *>::SetAt

; 274  : 	return hPen;

	mov	eax, ebx
	pop	esi
	pop	ebx
$L56536:
	pop	edi

; 275  : }

	ret	4
?GetPen@CDrawContext@GS@@AAEPAUHPEN__@@K@Z ENDP		; GS::CDrawContext::GetPen
_TEXT	ENDS
PUBLIC	?Lookup@?$CMap@KPAUHBRUSH__@@@Container@@QAEPAPAUHBRUSH__@@ABK@Z ; Container::CMap<unsigned long,HBRUSH__ *>::Lookup
PUBLIC	?SetAt@?$CMap@KPAUHBRUSH__@@@Container@@QAEXABKPAUHBRUSH__@@@Z ; Container::CMap<unsigned long,HBRUSH__ *>::SetAt
PUBLIC	?GetBrush@CDrawContext@GS@@AAEPAUHBRUSH__@@K@Z	; GS::CDrawContext::GetBrush
EXTRN	__imp__CreateSolidBrush@4:NEAR
_TEXT	SEGMENT
_cr$ = 8
?GetBrush@CDrawContext@GS@@AAEPAUHBRUSH__@@K@Z PROC NEAR ; GS::CDrawContext::GetBrush

; 279  : {

	push	edi
	mov	edi, ecx

; 280  : 	HBRUSH *pBrush = m_poc->m_mapBrushes.Lookup( cr );

	lea	eax, DWORD PTR _cr$[esp]
	mov	ecx, DWORD PTR [edi+48]
	push	eax
	add	ecx, 36					; 00000024H
	call	?Lookup@?$CMap@KPAUHBRUSH__@@@Container@@QAEPAPAUHBRUSH__@@ABK@Z ; Container::CMap<unsigned long,HBRUSH__ *>::Lookup

; 281  : 	if( pBrush )

	test	eax, eax
	je	SHORT $L56547

; 282  : 	{
; 283  : 		return *pBrush;

	mov	eax, DWORD PTR [eax]
	jmp	SHORT $L56545
$L56547:

; 284  : 	}
; 285  : 
; 286  : 	HBRUSH hbr = ::CreateSolidBrush( 0x02000000 | cr );

	mov	eax, DWORD PTR _cr$[esp]
	push	ebx
	or	eax, 33554432				; 02000000H
	push	esi
	push	eax
	call	DWORD PTR __imp__CreateSolidBrush@4

; 287  : 	VAPI( hbr );
; 288  : 	m_poc->m_arrObjToDelete.Add( hbr );

	mov	esi, DWORD PTR [edi+48]
	mov	ebx, eax
	mov	ecx, esi
	mov	eax, DWORD PTR [esi+16]
	inc	eax
	push	eax
	call	?SetSize@?$CArray@PAX@Container@@QAE_NI@Z ; Container::CArray<void *>::SetSize
	mov	eax, DWORD PTR [esi+16]
	mov	ecx, DWORD PTR [esi+4]

; 289  : 	m_poc->m_mapBrushes.SetAt( cr, hbr );

	push	ebx
	mov	DWORD PTR [ecx+eax*4-4], ebx
	mov	ecx, DWORD PTR [edi+48]
	lea	eax, DWORD PTR _cr$[esp+12]
	add	ecx, 36					; 00000024H
	push	eax
	call	?SetAt@?$CMap@KPAUHBRUSH__@@@Container@@QAEXABKPAUHBRUSH__@@@Z ; Container::CMap<unsigned long,HBRUSH__ *>::SetAt

; 290  : 	return hbr;

	mov	eax, ebx
	pop	esi
	pop	ebx
$L56545:
	pop	edi

; 291  : }

	ret	4
?GetBrush@CDrawContext@GS@@AAEPAUHBRUSH__@@K@Z ENDP	; GS::CDrawContext::GetBrush
_TEXT	ENDS
PUBLIC	?GetTextExtent@CDrawContext@GS@@QAEHPBDH@Z	; GS::CDrawContext::GetTextExtent
EXTRN	__imp__GetTextExtentExPointA@28:NEAR
_TEXT	SEGMENT
_pcszText$ = 8
_nTextLength$ = 12
_sizeTemp$ = -12
_nMaxChars$ = -4
?GetTextExtent@CDrawContext@GS@@QAEHPBDH@Z PROC NEAR	; GS::CDrawContext::GetTextExtent

; 355  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH

; 356  : 	/*
; 357  : 	ASSERT_VALID_STR_LEN( pcszText, nTextLength );
; 358  : 	ASSERT( m_pFontInfo );
; 359  : 
; 360  : 	int nLength = 0;
; 361  : 	while( nTextLength )
; 362  : 	{
; 363  : 		nLength += m_pFontInfo->m_nWidths[ (TBYTE)*pcszText ];
; 364  : 		pcszText++;
; 365  : 		nTextLength--;
; 366  : 	}
; 367  : 	if( nLength )
; 368  : 	{
; 369  : 		pcszText--;
; 370  : 		nLength += m_pFontInfo->m_nOverhang[ (TBYTE)*pcszText ];
; 371  : 	}
; 372  : 	return nLength;
; 373  : 	*/
; 374  : 	
; 375  : 	ASSERT_VALID_STR_LEN( pcszText, nTextLength );
; 376  : 	ASSERT( m_pFontInfo );
; 377  : 	
; 378  : 	SIZE sizeTemp;
; 379  : 	INT nMaxChars = 0;
; 380  : 	GetTextExtentExPoint( m_hdc, pcszText, nTextLength, 32768, &nMaxChars, NULL, &sizeTemp );

	lea	eax, DWORD PTR _sizeTemp$[ebp]
	and	DWORD PTR _nMaxChars$[ebp], 0
	push	eax
	lea	eax, DWORD PTR _nMaxChars$[ebp]
	push	0
	push	eax
	push	32768					; 00008000H
	push	DWORD PTR _nTextLength$[ebp]
	push	DWORD PTR _pcszText$[ebp]
	push	DWORD PTR [ecx+8]
	call	DWORD PTR __imp__GetTextExtentExPointA@28

; 381  : 	
; 382  : 	return sizeTemp.cx;

	mov	eax, DWORD PTR _sizeTemp$[ebp]

; 383  : }

	leave
	ret	8
?GetTextExtent@CDrawContext@GS@@QAEHPBDH@Z ENDP		; GS::CDrawContext::GetTextExtent
_TEXT	ENDS
PUBLIC	?GetTextFitWithWordWrap@CDrawContext@GS@@QBE_NHAAPBDAAVCSize@WinHelper@@_N@Z ; GS::CDrawContext::GetTextFitWithWordWrap
EXTRN	?g_HTMLBoardWidth@@3HA:DWORD			; g_HTMLBoardWidth
EXTRN	?g_CurrentXPos@@3HA:DWORD			; g_CurrentXPos
EXTRN	__imp___mbsdec:NEAR
EXTRN	__imp___mbsinc:NEAR
EXTRN	__imp___ismbcspace:NEAR
_TEXT	SEGMENT
_nMaxWidth$ = 8
_pcszText$ = 12
_size$ = 16
_bStartOfLine$ = 20
_this$ = -8
_sizeTemp$ = -16
_nMaxChars$ = 12
?GetTextFitWithWordWrap@CDrawContext@GS@@QBE_NHAAPBDAAVCSize@WinHelper@@_N@Z PROC NEAR ; GS::CDrawContext::GetTextFitWithWordWrap

; 395  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H

; 396  : 	 nMaxWidth = max( nMaxWidth, 0 );

	cmp	DWORD PTR _nMaxWidth$[ebp], 0
	mov	edx, ecx
	mov	DWORD PTR _this$[ebp], edx
	jg	SHORT $L57503
	and	DWORD PTR _nMaxWidth$[ebp], 0
$L57503:
	push	ebx

; 397  : 	 LPCTSTR pcszStart = pcszText;

	mov	ebx, DWORD PTR _pcszText$[ebp]
	push	esi
	push	edi
	mov	edi, DWORD PTR [ebx]

; 398  : 	 int nStringlen = _tcslen( pcszText );

	or	ecx, -1
	xor	eax, eax
	mov	DWORD PTR -4+[ebp], edi
	repne scasb
	not	ecx
	dec	ecx
	mov	esi, ecx

; 399  : 	 if( !nStringlen )
; 400  : 		 return true;

	je	SHORT $L57527

; 401  : 	 
; 402  : 	 SIZE sizeTemp;
; 403  : 	 INT nMaxChars = 0;

	and	DWORD PTR _nMaxChars$[ebp], eax

; 404  : 	 GetTextExtentExPoint( m_hdc, pcszText, nStringlen, nMaxWidth, &nMaxChars, NULL, &sizeTemp );

	lea	eax, DWORD PTR _sizeTemp$[ebp]
	push	eax
	lea	eax, DWORD PTR _nMaxChars$[ebp]
	push	0
	push	eax
	push	DWORD PTR _nMaxWidth$[ebp]
	push	esi
	push	DWORD PTR -4+[ebp]
	push	DWORD PTR [edx+8]
	call	DWORD PTR __imp__GetTextExtentExPointA@28

; 405  : 	 size = sizeTemp;

	mov	edi, DWORD PTR _size$[ebp]
	mov	ecx, DWORD PTR _sizeTemp$[ebp+4]
	mov	eax, DWORD PTR _sizeTemp$[ebp]
	mov	DWORD PTR [edi+4], ecx

; 406  : 	 if( nStringlen != nMaxChars || sizeTemp.cx > (g_HTMLBoardWidth - g_CurrentXPos) )

	mov	ecx, DWORD PTR _nMaxChars$[ebp]
	cmp	esi, ecx
	mov	DWORD PTR [edi], eax
	jne	SHORT $L56575
	mov	edx, DWORD PTR ?g_HTMLBoardWidth@@3HA	; g_HTMLBoardWidth
	sub	edx, DWORD PTR ?g_CurrentXPos@@3HA	; g_CurrentXPos
	cmp	eax, edx
	jg	SHORT $L56575

; 447  : 	 }
; 448  : 
; 449  : 	 pcszText += nMaxChars;

	add	DWORD PTR [ebx], ecx
$L57527:

; 450  : 	 
; 451  : 	 return true;

	mov	al, 1
	jmp	$L56568
$L56575:

; 407  : 	 {
; 408  : 		 LPCTSTR pcszPos = pcszText + nMaxChars;

	mov	esi, DWORD PTR [ebx]
	add	esi, ecx

; 409  : 		 while( pcszPos && !_istspace( *pcszPos ) && *pcszPos != '\r' && *pcszPos != '\n' )

	je	SHORT $L57520
$L56578:
	movsx	eax, BYTE PTR [esi]
	push	eax
	call	DWORD PTR __imp___ismbcspace
	test	eax, eax
	pop	ecx
	jne	SHORT $L56579
	mov	al, BYTE PTR [esi]
	cmp	al, 13					; 0000000dH
	je	SHORT $L56579
	cmp	al, 10					; 0000000aH
	je	SHORT $L56579

; 410  : 		 {
; 411  : 			 pcszPos = _tcsdec( pcszStart, pcszPos );

	push	esi
	push	DWORD PTR -4+[ebp]
	call	DWORD PTR __imp___mbsdec

; 412  : 			 nMaxChars--;

	dec	DWORD PTR _nMaxChars$[ebp]
	mov	esi, eax
	pop	ecx
	test	esi, esi
	pop	ecx
	jne	SHORT $L56578
$L56579:

; 413  : 		 }
; 414  : 		 
; 415  : 		 if( !pcszPos )

	test	esi, esi
	jne	SHORT $L56580
$L57520:

; 416  : 		 {
; 417  : 			 pcszPos = pcszText;

	mov	esi, DWORD PTR [ebx]
$L56580:

; 418  : 		 }
; 419  : 		 
; 420  : 		 if( bStartOfLine && pcszPos == pcszText )

	cmp	BYTE PTR _bStartOfLine$[ebp], 0
	je	SHORT $L57525
	cmp	esi, DWORD PTR [ebx]
	jne	SHORT $L57525

; 421  : 		 {
; 422  : 			 nMaxChars ++;

	inc	DWORD PTR _nMaxChars$[ebp]
$L57526:

; 423  : 			 
; 424  : 			 while( *pcszPos && !_istspace( *pcszPos ) && *pcszPos != '\r' && *pcszPos != '\n' )

	mov	al, BYTE PTR [esi]
	test	al, al
	je	SHORT $L56584
	movsx	eax, al
	push	eax
	call	DWORD PTR __imp___ismbcspace
	test	eax, eax
	pop	ecx
	jne	SHORT $L56584
	mov	al, BYTE PTR [esi]
	cmp	al, 13					; 0000000dH
	je	SHORT $L56584
	cmp	al, 10					; 0000000aH
	je	SHORT $L56584

; 425  : 			 {
; 426  : 				 nMaxChars++;

	inc	DWORD PTR _nMaxChars$[ebp]

; 427  : 				 pcszPos = _tcsinc( pcszPos );

	push	esi
	call	DWORD PTR __imp___mbsinc
	pop	ecx
	mov	esi, eax
	jmp	SHORT $L57526
$L56584:

; 428  : 			 }
; 429  : 			 nMaxWidth = g_HTMLBoardWidth - g_CurrentXPos;//32768;

	mov	eax, DWORD PTR ?g_HTMLBoardWidth@@3HA	; g_HTMLBoardWidth
	sub	eax, DWORD PTR ?g_CurrentXPos@@3HA	; g_CurrentXPos
	jmp	SHORT $L56581
$L57525:
	mov	eax, DWORD PTR _nMaxWidth$[ebp]
$L56581:

; 430  : 		 }
; 431  : 
; 432  : 		 if( nMaxWidth > (g_HTMLBoardWidth - g_CurrentXPos) )

	mov	ecx, DWORD PTR ?g_HTMLBoardWidth@@3HA	; g_HTMLBoardWidth
	sub	ecx, DWORD PTR ?g_CurrentXPos@@3HA	; g_CurrentXPos
	cmp	eax, ecx
	jle	SHORT $L56585

; 433  : 		 {
; 434  : 			 nMaxWidth = g_HTMLBoardWidth - g_CurrentXPos;

	mov	DWORD PTR _nMaxWidth$[ebp], ecx
	mov	eax, ecx
$L56585:

; 435  : 		 }
; 436  : 
; 437  : 		 GetTextExtentExPoint( m_hdc, pcszText, nMaxChars, nMaxWidth, &nMaxChars, NULL, &sizeTemp );

	lea	ecx, DWORD PTR _sizeTemp$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _nMaxChars$[ebp]
	push	0
	push	ecx
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	push	DWORD PTR _nMaxChars$[ebp]
	push	DWORD PTR [ebx]
	push	DWORD PTR [eax+8]
	call	DWORD PTR __imp__GetTextExtentExPointA@28

; 438  : 		 
; 439  : 		 //modified for Japanese 
; 440  : 		 if( nMaxChars == 0 )

	mov	eax, DWORD PTR _nMaxChars$[ebp]
	test	eax, eax
	jne	SHORT $L56586

; 441  : 		 	 nMaxChars = 1;

	push	1
	pop	eax
$L56586:

; 442  : 
; 443  : 		 pcszText += nMaxChars;

	add	DWORD PTR [ebx], eax

; 444  : 		 size = sizeTemp;

	mov	eax, DWORD PTR _sizeTemp$[ebp]
	mov	DWORD PTR [edi], eax
	mov	eax, DWORD PTR _sizeTemp$[ebp+4]
	mov	DWORD PTR [edi+4], eax

; 445  : 
; 446  : 		 return false;

	xor	al, al
$L56568:
	pop	edi
	pop	esi
	pop	ebx

; 452  : }

	leave
	ret	16					; 00000010H
?GetTextFitWithWordWrap@CDrawContext@GS@@QBE_NHAAPBDAAVCSize@WinHelper@@_N@Z ENDP ; GS::CDrawContext::GetTextFitWithWordWrap
_TEXT	ENDS
PUBLIC	?GetFont@CDrawContext@GS@@AAEPAUFontInfo@12@ABUFontDef@2@@Z ; GS::CDrawContext::GetFont
PUBLIC	?Lookup@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEPAPAUFontInfo@CDrawContext@GS@@ABUFontDef@5@@Z ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::Lookup
PUBLIC	?SetAt@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXABUFontDef@GS@@PAUFontInfo@CDrawContext@4@@Z ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::SetAt
EXTRN	__imp__GetTextExtentPoint32A@16:NEAR
EXTRN	__imp__SelectObject@8:NEAR
EXTRN	__imp__CreateFontIndirectA@4:NEAR
EXTRN	__imp__GetTextMetricsA@8:NEAR
EXTRN	__imp__GetCharWidth32A@16:NEAR
EXTRN	__imp__GetCharABCWidthsA@16:NEAR
_TEXT	SEGMENT
_fdef$ = 8
_lf$ = -84
_hFontOld$ = -16
_tm$ = -140
_abcWids$ = -3212
_parrOverhang$56607 = -8
_parrWidths$56618 = -4
_size$56620 = -24
_ch$56621 = -9
_n$56622 = -8
?GetFont@CDrawContext@GS@@AAEPAUFontInfo@12@ABUFontDef@2@@Z PROC NEAR ; GS::CDrawContext::GetFont

; 511  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 3212				; 00000c8cH
	push	ebx
	mov	ebx, ecx

; 512  : 	FontInfo **ppInfo = m_poc->m_mapFonts.Lookup( fdef );

	push	DWORD PTR _fdef$[ebp]
	mov	ecx, DWORD PTR [ebx+48]
	add	ecx, 20					; 00000014H
	call	?Lookup@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEPAPAUFontInfo@CDrawContext@GS@@ABUFontDef@5@@Z ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::Lookup

; 513  : 	if( ppInfo )

	test	eax, eax
	je	SHORT $L56592

; 514  : 	{
; 515  : 		return (*ppInfo);

	mov	eax, DWORD PTR [eax]
	jmp	$L56590
$L56592:
	push	esi
	push	edi

; 516  : 	}
; 517  : 
; 518  : 	LOGFONT lf = {0};

	push	14					; 0000000eH
	xor	eax, eax
	pop	ecx
	lea	edi, DWORD PTR _lf$[ebp+4]
	rep stosd

; 519  : 	(void)_tcscpy( lf.lfFaceName, fdef.m_szFontName );

	mov	edi, DWORD PTR _fdef$[ebp]
	or	ecx, -1
	repne scasb
	not	ecx
	sub	edi, ecx
	lea	edx, DWORD PTR _lf$[ebp+28]
	mov	eax, ecx
	mov	esi, edi
	mov	edi, edx
	shr	ecx, 2
	rep movsd
	mov	ecx, eax
	and	ecx, 3
	rep movsb

; 520  : 	lf.lfWeight = fdef.m_nWeight;

	mov	edi, DWORD PTR _fdef$[ebp]

; 521  : 	lf.lfHeight = fdef.m_nSizePixels;
; 522  : 	lf.lfItalic = fdef.m_bItalic;
; 523  : 	lf.lfUnderline = fdef.m_bUnderline;
; 524  : 	lf.lfStrikeOut = fdef.m_bStrike;
; 525  : 	lf.lfQuality = ANTIALIASED_QUALITY;

	mov	BYTE PTR _lf$[ebp+26], 4

; 526  : 	lf.lfCharSet = fdef.m_cCharSet;
; 527  : 	#if !defined (_WIN32_WCE)
; 528  : 		lf.lfOutPrecision = OUT_TT_PRECIS;

	mov	BYTE PTR _lf$[ebp+24], 4
	mov	eax, DWORD PTR [edi+132]

; 529  : 	#else		//	!defined (_WIN32_WCE)
; 530  : 		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
; 531  : 	#endif	//	!defined (_WIN32_WCE)
; 532  : 
; 533  : 	if( fdef.m_bFixedPitchFont )

	cmp	BYTE PTR [edi+140], 0
	mov	DWORD PTR _lf$[ebp+16], eax
	mov	eax, DWORD PTR [edi+128]
	mov	DWORD PTR _lf$[ebp], eax
	mov	al, BYTE PTR [edi+137]
	mov	BYTE PTR _lf$[ebp+20], al
	mov	al, BYTE PTR [edi+136]
	mov	BYTE PTR _lf$[ebp+21], al
	mov	al, BYTE PTR [edi+138]
	mov	BYTE PTR _lf$[ebp+22], al
	mov	al, BYTE PTR [edi+139]
	mov	BYTE PTR _lf$[ebp+23], al
	je	SHORT $L56595

; 534  : 		lf.lfPitchAndFamily = FIXED_PITCH;

	mov	BYTE PTR _lf$[ebp+27], 1
$L56595:

; 535  : 
; 536  : 	FontInfo *pInfo = new FontInfo;

	push	2064					; 00000810H
	call	??2@YAPAXI@Z				; operator new
	mov	esi, eax
	pop	ecx

; 537  : 	pInfo->hFont = CreateFontIndirect( &lf );

	lea	eax, DWORD PTR _lf$[ebp]
	push	eax
	call	DWORD PTR __imp__CreateFontIndirectA@4

; 538  : 
; 539  : 
; 540  : 	//
; 541  : 	//	Get and store the widths of the individual characters, these are used
; 542  : 	//	later when calculating the lengths of text in pixels.
; 543  : 	HGDIOBJ hFontOld = ::SelectObject( m_hdc, pInfo->hFont );

	push	eax
	mov	DWORD PTR [esi], eax
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__SelectObject@8
	mov	DWORD PTR _hFontOld$[ebp], eax

; 544  : 
; 545  : 	//
; 546  : 	//	Store some extra bits and pieces.
; 547  : 	TEXTMETRIC tm;
; 548  : 	GetTextMetrics( m_hdc, &tm );

	lea	eax, DWORD PTR _tm$[ebp]
	push	eax
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__GetTextMetricsA@8

; 549  : 	pInfo->m_nBaseline = tm.tmAscent;

	mov	eax, DWORD PTR _tm$[ebp+4]
	mov	DWORD PTR [esi+2052], eax

; 550  : 	pInfo->m_nLineSpace = tm.tmHeight + tm.tmExternalLeading;

	mov	ecx, DWORD PTR _tm$[ebp]
	mov	edx, DWORD PTR _tm$[ebp+16]
	lea	eax, DWORD PTR [esi+2056]
	add	edx, ecx

; 551  : 	pInfo->m_nAverageWidth = tm.tmAveCharWidth;
; 552  : 	if( fdef.m_bUnderline )

	cmp	BYTE PTR [edi+136], 0
	mov	DWORD PTR [eax], edx
	mov	ecx, DWORD PTR _tm$[ebp+20]
	mov	DWORD PTR [esi+2060], ecx
	je	SHORT $L56601

; 553  : 		pInfo->m_nLineSpace++;

	inc	DWORD PTR [eax]
$L56601:

; 554  : 
; 555  : #ifndef USE_WIN32_MEASURING
; 556  : 
; 557  : 	ABC abcWids[ countof( pInfo->m_nWidths ) ];
; 558  : 	memset( abcWids, 0, sizeof( abcWids ) );

	mov	ecx, 768				; 00000300H
	xor	eax, eax
	lea	edi, DWORD PTR _abcWids$[ebp]
	rep stosd

; 559  : 	if( GetCharABCWidths( m_hdc, 0, countof( pInfo->m_nWidths ) - 1, abcWids ) )

	lea	eax, DWORD PTR _abcWids$[ebp]
	mov	edi, 255				; 000000ffH
	push	eax
	push	edi
	push	0
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__GetCharABCWidthsA@16
	test	eax, eax
	je	SHORT $L56603

; 560  : 	{
; 561  : 		ABC *pABC = &abcWids[0];
; 562  : 		int *parrWidths = pInfo->m_nWidths;
; 563  : 		if( tm.tmOverhang )

	cmp	DWORD PTR _tm$[ebp+32], 0
	lea	edx, DWORD PTR [esi+4]
	je	SHORT $L56606

; 564  : 		{
; 565  : 			int *parrOverhang = pInfo->m_nOverhang;

	lea	eax, DWORD PTR [esi+1028]
	mov	DWORD PTR -4+[ebp], 256			; 00000100H
	mov	DWORD PTR _parrOverhang$56607[ebp], eax
	lea	eax, DWORD PTR _abcWids$[ebp+8]
$L56609:

; 566  : 
; 567  : 			for( UINT n = 0; n < countof( pInfo->m_nWidths ) ; n++, pABC++ )
; 568  : 			{
; 569  : 				*parrWidths++ = ( pABC->abcA + pABC->abcB + pABC->abcC );

	mov	edi, DWORD PTR [eax-8]
	mov	ecx, DWORD PTR [eax]
	add	edi, DWORD PTR [eax-4]
	add	eax, 12					; 0000000cH
	add	edi, ecx
	mov	DWORD PTR [edx], edi

; 570  : 				*parrOverhang++ = pABC->abcC;

	mov	edi, DWORD PTR _parrOverhang$56607[ebp]
	add	DWORD PTR _parrOverhang$56607[ebp], 4
	add	edx, 4
	dec	DWORD PTR -4+[ebp]
	mov	DWORD PTR [edi], ecx
	jne	SHORT $L56609

; 571  : 			}
; 572  : 		}
; 573  : 		else

	jmp	SHORT $L56617
$L56606:

; 574  : 		{
; 575  : 			for( UINT n = 0; n < countof( pInfo->m_nWidths ) ; n++, pABC++ )

	mov	ecx, 256				; 00000100H
	lea	eax, DWORD PTR _abcWids$[ebp+4]
	mov	DWORD PTR -4+[ebp], ecx
$L56614:

; 576  : 			{
; 577  : 				*parrWidths++ = ( pABC->abcA + pABC->abcB + pABC->abcC );

	mov	edi, DWORD PTR [eax-4]
	add	edi, DWORD PTR [eax+4]
	add	edi, DWORD PTR [eax]
	add	eax, 12					; 0000000cH
	mov	DWORD PTR [edx], edi
	add	edx, 4
	dec	DWORD PTR -4+[ebp]
	jne	SHORT $L56614

; 578  : 			}
; 579  : 			memset( pInfo->m_nOverhang, 0, sizeof( pInfo->m_nOverhang ) );
; 580  : 		}
; 581  : 	}
; 582  : 	else

	jmp	SHORT $L57561
$L56603:

; 583  : 	{
; 584  : 		int *parrWidths = pInfo->m_nWidths;

	lea	eax, DWORD PTR [esi+4]

; 585  : 		if( !GetCharWidth32( m_hdc, 0, 255, parrWidths ) )

	push	eax
	push	edi
	push	0
	mov	DWORD PTR _parrWidths$56618[ebp], eax
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__GetCharWidth32A@16
	test	eax, eax
	jne	SHORT $L56625

; 586  : 		{
; 587  : 			SIZE size;
; 588  : 			TCHAR ch = 0;
; 589  : 			for( UINT n = 0; n < 255; n++ )

	and	DWORD PTR _n$56622[ebp], eax
$L56623:

; 590  : 			{
; 591  : 				ch = (TCHAR)n;

	mov	al, BYTE PTR _n$56622[ebp]
	mov	BYTE PTR _ch$56621[ebp], al

; 592  : 				GetTextExtentPoint32( m_hdc, &ch, 1, &size );

	lea	eax, DWORD PTR _size$56620[ebp]
	push	eax
	lea	eax, DWORD PTR _ch$56621[ebp]
	push	1
	push	eax
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__GetTextExtentPoint32A@16

; 593  : 				*parrWidths++ = size.cx;

	mov	ecx, DWORD PTR _parrWidths$56618[ebp]
	add	DWORD PTR _parrWidths$56618[ebp], 4
	inc	DWORD PTR _n$56622[ebp]
	mov	eax, DWORD PTR _size$56620[ebp]
	cmp	DWORD PTR _n$56622[ebp], edi
	mov	DWORD PTR [ecx], eax
	jb	SHORT $L56623
$L56625:

; 594  : 			}
; 595  : 		}
; 596  : 		memset( pInfo->m_nOverhang, 0, sizeof( pInfo->m_nOverhang ) );

	mov	ecx, 256				; 00000100H
$L57561:
	xor	eax, eax
	lea	edi, DWORD PTR [esi+1028]
	rep stosd
$L56617:

; 597  : 	}
; 598  : #endif	//	USE_WIN32_MEASURING
; 599  : 
; 600  : 	::SelectObject( m_hdc, hFontOld );

	push	DWORD PTR _hFontOld$[ebp]
	push	DWORD PTR [ebx+8]
	call	DWORD PTR __imp__SelectObject@8

; 601  : 
; 602  : 	//
; 603  : 	//	And finally add the font to our cache.
; 604  : 	if( pInfo->hFont )

	mov	eax, DWORD PTR [esi]
	test	eax, eax
	mov	DWORD PTR -16+[ebp], eax
	je	SHORT $L57552

; 605  : 		m_poc->m_arrObjToDelete.Add( pInfo->hFont );

	mov	edi, DWORD PTR [ebx+48]
	mov	ecx, edi
	mov	eax, DWORD PTR [edi+16]
	inc	eax
	push	eax
	call	?SetSize@?$CArray@PAX@Container@@QAE_NI@Z ; Container::CArray<void *>::SetSize
	mov	eax, DWORD PTR [edi+16]
	mov	ecx, DWORD PTR [edi+4]
	mov	edx, DWORD PTR -16+[ebp]
	mov	DWORD PTR [ecx+eax*4-4], edx
$L57552:

; 606  : 	m_poc->m_mapFonts.SetAt( fdef, pInfo );

	mov	ecx, DWORD PTR [ebx+48]
	push	esi
	push	DWORD PTR _fdef$[ebp]
	add	ecx, 20					; 00000014H
	call	?SetAt@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXABUFontDef@GS@@PAUFontInfo@CDrawContext@4@@Z ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::SetAt

; 607  : 
; 608  : 	return pInfo;

	mov	eax, esi
	pop	edi
	pop	esi
$L56590:
	pop	ebx

; 609  : }

	leave
	ret	4
?GetFont@CDrawContext@GS@@AAEPAUFontInfo@12@ABUFontDef@2@@Z ENDP ; GS::CDrawContext::GetFont
_TEXT	ENDS
PUBLIC	?DrawEllipsisText@CDrawContext@GS@@QAE_NHHHPBDHKAAH@Z ; GS::CDrawContext::DrawEllipsisText
PUBLIC	?DrawTextCharacterClipped@CDrawContext@GS@@QAE_NHHHPBDKAAH@Z ; GS::CDrawContext::DrawTextCharacterClipped
_TEXT	SEGMENT
_nLeft$ = 8
_nTop$ = 12
_nRight$ = 16
_pcszText$ = 20
_nStringLength$ = 24
_cr$ = 28
_nLength$ = 32
_nLength2$56647 = 20
?DrawEllipsisText@CDrawContext@GS@@QAE_NHHHPBDHKAAH@Z PROC NEAR ; GS::CDrawContext::DrawEllipsisText

; 613  : {

	push	ebp
	mov	ebp, esp

; 614  : 	if( nStringLength )

	cmp	DWORD PTR _nStringLength$[ebp], 0
	push	ebx
	push	edi
	mov	edi, ecx
	je	$L56648

; 615  : 	{
; 616  : 		ASSERT_VALID_STR_LEN( pcszText, nStringLength );
; 617  : 		ASSERT( m_pFontInfo );
; 618  : 		const PI_int32 nLengthPixels = GetTextExtent( pcszText, nStringLength );

	push	DWORD PTR _nStringLength$[ebp]
	push	DWORD PTR _pcszText$[ebp]
	call	?GetTextExtent@CDrawContext@GS@@QAEHPBDH@Z ; GS::CDrawContext::GetTextExtent

; 619  : 		if( nLengthPixels > nRight - nLeft )

	mov	ecx, DWORD PTR _nRight$[ebp]
	mov	ebx, DWORD PTR _nLeft$[ebp]
	sub	ecx, ebx
	cmp	eax, ecx
	jle	SHORT $L56643

; 620  : 		{
; 621  : 			const PI_int32 nElipsisLengthPixels = GetTextExtent( g_pcszEllipsis, g_nEllipsisLength );

	push	DWORD PTR _g_nEllipsisLength
	mov	ecx, edi
	push	DWORD PTR _g_pcszEllipsis
	call	?GetTextExtent@CDrawContext@GS@@QAEHPBDH@Z ; GS::CDrawContext::GetTextExtent

; 622  : 			PI_int32 nNewRight = nRight - nElipsisLengthPixels;

	mov	ecx, DWORD PTR _nRight$[ebp]
	sub	ecx, eax

; 623  : 			if( nNewRight > nLeft )

	cmp	ecx, ebx
	jle	SHORT $L56648
	push	esi

; 624  : 			{
; 625  : 				DrawTextCharacterClipped( nLeft, nTop, nNewRight, pcszText, cr, nLength );

	mov	esi, DWORD PTR _nLength$[ebp]
	push	esi
	push	DWORD PTR _cr$[ebp]
	push	DWORD PTR _pcszText$[ebp]
	push	ecx
	mov	ecx, edi
	push	DWORD PTR _nTop$[ebp]
	push	ebx
	call	?DrawTextCharacterClipped@CDrawContext@GS@@QAE_NHHHPBDKAAH@Z ; GS::CDrawContext::DrawTextCharacterClipped

; 626  : 				PI_int32 nLength2;
; 627  : 				DrawTextCharacterClipped( nLeft + nLength, nTop, nRight, g_pcszEllipsis, cr, nLength2 );

	lea	eax, DWORD PTR _nLength2$56647[ebp]
	mov	ecx, edi
	push	eax
	mov	eax, DWORD PTR [esi]
	push	DWORD PTR _cr$[ebp]
	add	eax, ebx
	push	DWORD PTR _g_pcszEllipsis
	push	DWORD PTR _nRight$[ebp]
	push	DWORD PTR _nTop$[ebp]
	push	eax
	call	?DrawTextCharacterClipped@CDrawContext@GS@@QAE_NHHHPBDKAAH@Z ; GS::CDrawContext::DrawTextCharacterClipped

; 628  : 				nLength += nLength2;

	mov	eax, DWORD PTR _nLength2$56647[ebp]
	add	DWORD PTR [esi], eax

; 629  : 				return true;

	mov	al, 1
	pop	esi
	jmp	SHORT $L56638
$L56643:

; 630  : 			}
; 631  : 		}
; 632  : 		else
; 633  : 		{
; 634  : 			DrawTextCharacterClipped( nLeft, nTop, nRight, pcszText, cr, nLength );

	push	DWORD PTR _nLength$[ebp]
	mov	ecx, edi
	push	DWORD PTR _cr$[ebp]
	push	DWORD PTR _pcszText$[ebp]
	push	DWORD PTR _nRight$[ebp]
	push	DWORD PTR _nTop$[ebp]
	push	ebx
	call	?DrawTextCharacterClipped@CDrawContext@GS@@QAE_NHHHPBDKAAH@Z ; GS::CDrawContext::DrawTextCharacterClipped

; 635  : 			return true;

	mov	al, 1
	jmp	SHORT $L56638
$L56648:

; 636  : 		}
; 637  : 	}
; 638  : 	return false;

	xor	al, al
$L56638:
	pop	edi
	pop	ebx

; 639  : }

	pop	ebp
	ret	28					; 0000001cH
?DrawEllipsisText@CDrawContext@GS@@QAE_NHHHPBDHKAAH@Z ENDP ; GS::CDrawContext::DrawEllipsisText
_TEXT	ENDS
EXTRN	__imp__SetTextColor@8:NEAR
EXTRN	__imp__ExtTextOutA@32:NEAR
_TEXT	SEGMENT
_nLeft$ = 8
_nTop$ = 12
_nRight$ = 16
_pcszText$ = 20
_cr$ = 24
_nLength$ = 28
_nMaxLength$ = 16
?DrawTextCharacterClipped@CDrawContext@GS@@QAE_NHHHPBDKAAH@Z PROC NEAR ; GS::CDrawContext::DrawTextCharacterClipped

; 643  : {

	push	ebp
	mov	ebp, esp

; 644  : 	ASSERT_VALID_STR( pcszText );
; 645  : 	//
; 646  : 	//	There's no font selected
; 647  : 	ASSERT( m_pFontInfo );
; 648  : 	LPCTSTR pcszTextCopy = pcszText;
; 649  : 	const PI_int32 nMaxLength = max( nRight - nLeft, 0 );

	mov	eax, DWORD PTR _nRight$[ebp]
	push	ebx
	sub	eax, DWORD PTR _nLeft$[ebp]
	push	esi
	mov	esi, DWORD PTR _pcszText$[ebp]
	push	edi
	test	eax, eax
	mov	edi, ecx
	jle	SHORT $L57564
	mov	DWORD PTR _nMaxLength$[ebp], eax
	jmp	SHORT $L57565
$L57564:
	and	DWORD PTR _nMaxLength$[ebp], 0
$L57565:

; 650  : #ifdef USE_WIN32_MEASURING
; 651  : 	PI_int32 nStringlen = _tcslen( pcszText );
; 652  : 	SIZE sizeTemp;
; 653  : 	INT nMaxChars = 0;
; 654  : 	GetTextExtentExPoint( m_hdc, pcszText, nStringlen, nMaxLength, &nMaxChars, NULL, &sizeTemp );
; 655  : 	DrawText( nLeft, nTop, pcszText, nMaxChars, cr );
; 656  : 	if( sizeTemp.cx <= nMaxLength )
; 657  : 	{
; 658  : 		return true;
; 659  : 	}
; 660  : 	GetTextExtentExPoint( m_hdc, pcszText, nMaxChars, nMaxLength, &nMaxChars, NULL, &sizeTemp );
; 661  : 	nLength = sizeTemp.cx;
; 662  : 	return false;
; 663  : 
; 664  : #else	USE_WIN32_MEASURING
; 665  : 	nLength = 0;

	mov	eax, DWORD PTR _nLength$[ebp]

; 666  : 	UCHAR ch;
; 667  : 	while( ( ch = *pcszTextCopy ) != '\000' && nLength + m_pFontInfo->m_nWidths[ ch ] <= nMaxLength )

	mov	cl, BYTE PTR [esi]
	and	DWORD PTR [eax], 0
	test	cl, cl
	je	SHORT $L56665
	mov	edx, DWORD PTR [edi+44]
$L56664:
	mov	ebx, DWORD PTR [eax]
	movzx	ecx, cl
	mov	ecx, DWORD PTR [edx+ecx*4+4]
	add	ecx, ebx
	cmp	ecx, DWORD PTR _nMaxLength$[ebp]
	jg	SHORT $L56665

; 668  : 	{
; 669  : 		nLength += m_pFontInfo->m_nWidths[ ch ];
; 670  : 		pcszTextCopy++;

	inc	esi
	mov	DWORD PTR [eax], ecx
	mov	cl, BYTE PTR [esi]
	test	cl, cl
	jne	SHORT $L56664
$L56665:

; 671  : 	}
; 672  : 	DrawText( nLeft, nTop, pcszText, pcszTextCopy - pcszText, cr );

	mov	ebx, DWORD PTR _cr$[ebp]
	cmp	ebx, DWORD PTR [edi+68]
	je	SHORT $L57569
	mov	eax, ebx
	or	eax, 33554432				; 02000000H
	push	eax
	push	DWORD PTR [edi+8]
	call	DWORD PTR __imp__SetTextColor@8
	mov	DWORD PTR [edi+68], ebx
$L57569:
	mov	ecx, esi
	xor	eax, eax
	sub	ecx, DWORD PTR _pcszText$[ebp]
	push	eax
	push	ecx
	push	DWORD PTR _pcszText$[ebp]
	push	eax
	push	eax
	push	DWORD PTR _nTop$[ebp]
	push	DWORD PTR _nLeft$[ebp]
	push	DWORD PTR [edi+8]
	call	DWORD PTR __imp__ExtTextOutA@32

; 673  : 
; 674  : #endif	//	USE_WIN32_MEASURING
; 675  : 
; 676  : 	return *pcszTextCopy == '\000' ? false : true;

	cmp	BYTE PTR [esi], 0
	pop	edi
	pop	esi
	pop	ebx
	setne	al

; 677  : }

	pop	ebp
	ret	24					; 00000018H
?DrawTextCharacterClipped@CDrawContext@GS@@QAE_NHHHPBDKAAH@Z ENDP ; GS::CDrawContext::DrawTextCharacterClipped
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@PAX@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CArray@PAX@Container@@QAEXXZ PROC NEAR	; Container::CArray<void *>::RemoveAll, COMDAT

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
?RemoveAll@?$CArray@PAX@Container@@QAEXXZ ENDP		; Container::CArray<void *>::RemoveAll
_TEXT	ENDS
;	COMDAT ??1?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAE@XZ
_TEXT	SEGMENT
??1?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAE@XZ PROC NEAR ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::~CMap<GS::FontDef,GS::CDrawContext::FontInfo *>, COMDAT

; 21   : {

	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@6B@ ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::`vftable'

; 22   : 	RemoveAll();

	call	?RemoveAll@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::RemoveAll

; 23   : 	delete [] m_pEntrys;

	push	DWORD PTR [esi+8]
	call	??3@YAXPAX@Z				; operator delete

; 24   : 	m_pEntrys = NULL;

	and	DWORD PTR [esi+8], 0
	pop	ecx
	pop	esi

; 25   : }

	ret	0
??1?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAE@XZ ENDP ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::~CMap<GS::FontDef,GS::CDrawContext::FontInfo *>
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEPAPAUFontInfo@CDrawContext@GS@@ABUFontDef@5@@Z
_TEXT	SEGMENT
_key$ = 8
?Lookup@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEPAPAUFontInfo@CDrawContext@GS@@ABUFontDef@5@@Z PROC NEAR ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::Lookup, COMDAT

; 111  : {

	push	ebx
	push	ebp

; 112  : 	PI_uint32 uHash = HashIt( key ) % m_uBucketArrayLength;

	mov	ebp, DWORD PTR _key$[esp+4]
	xor	edx, edx
	push	esi
	push	edi
	mov	eax, DWORD PTR [ebp+144]
	div	DWORD PTR [ecx+4]

; 113  : 	MapBucketEntry<Key, Value>	*pEntry = Find( uHash, key );

	mov	eax, DWORD PTR [ecx+8]
	mov	ebx, DWORD PTR [eax+edx*4]
$L57612:
	test	ebx, ebx
	je	SHORT $L57607
	sub	esp, 148				; 00000094H
	mov	esi, ebp
	push	37					; 00000025H
	pop	ecx
	mov	edi, esp
	sub	esp, 148				; 00000094H
	rep movsd
	push	37					; 00000025H
	lea	esi, DWORD PTR [ebx+4]
	pop	ecx
	mov	edi, esp
	rep movsd
	call	?ElementsTheSame@Container@@YAHUFontDef@GS@@0@Z ; Container::ElementsTheSame
	add	esp, 296				; 00000128H
	test	eax, eax
	jne	SHORT $L57610
	mov	ebx, DWORD PTR [ebx+152]
	jmp	SHORT $L57612
$L57610:
	mov	eax, ebx
	jmp	SHORT $L57603
$L57607:
	xor	eax, eax
$L57603:

; 114  : 	return pEntry != NULL ? &pEntry -> m_Value : NULL;
; 115  : }

	pop	edi
	pop	esi
	pop	ebp
	pop	ebx
	ret	4
?Lookup@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEPAPAUFontInfo@CDrawContext@GS@@ABUFontDef@5@@Z ENDP ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::Lookup
_TEXT	ENDS
EXTRN	__imp___mbscmp:NEAR
_TEXT	SEGMENT
_n1$ = 8
_n2$ = 156
?ElementsTheSame@Container@@YAHUFontDef@GS@@0@Z PROC NEAR ; Container::ElementsTheSame

; 158  : 	{

	push	ebp
	mov	ebp, esp

; 159  : 		//
; 160  : 		//	Don't optimise this unless you have empirical evidence to proove that you can
; 161  : 		//	increase it' speed.
; 162  : 		return n1.m_bItalic == n2.m_bItalic
; 163  : 			&& n1.m_nSizePixels == n2.m_nSizePixels
; 164  : 			&& n1.m_nWeight == n2.m_nWeight
; 165  : 			&& n1.m_bUnderline == n2.m_bUnderline
; 166  : 			&& n1.m_bStrike == n2.m_bStrike
; 167  : 			&& n1.m_cCharSet == n2.m_cCharSet
; 168  : 			&& !_tcscmp( n1.m_szFontName, n2.m_szFontName );

	mov	al, BYTE PTR _n1$[ebp+137]
	cmp	al, BYTE PTR _n2$[ebp+137]
	jne	SHORT $L57615
	mov	eax, DWORD PTR _n1$[ebp+128]
	cmp	eax, DWORD PTR _n2$[ebp+128]
	jne	SHORT $L57615
	mov	eax, DWORD PTR _n1$[ebp+132]
	cmp	eax, DWORD PTR _n2$[ebp+132]
	jne	SHORT $L57615
	mov	al, BYTE PTR _n1$[ebp+136]
	cmp	al, BYTE PTR _n2$[ebp+136]
	jne	SHORT $L57615
	mov	al, BYTE PTR _n1$[ebp+138]
	cmp	al, BYTE PTR _n2$[ebp+138]
	jne	SHORT $L57615
	mov	al, BYTE PTR _n1$[ebp+139]
	cmp	al, BYTE PTR _n2$[ebp+139]
	jne	SHORT $L57615
	lea	eax, DWORD PTR _n2$[ebp]
	push	eax
	lea	eax, DWORD PTR _n1$[ebp]
	push	eax
	call	DWORD PTR __imp___mbscmp
	pop	ecx
	test	eax, eax
	pop	ecx
	jne	SHORT $L57615
	push	1
	pop	eax

; 169  : 	}

	pop	ebp
	ret	0
$L57615:

; 159  : 		//
; 160  : 		//	Don't optimise this unless you have empirical evidence to proove that you can
; 161  : 		//	increase it' speed.
; 162  : 		return n1.m_bItalic == n2.m_bItalic
; 163  : 			&& n1.m_nSizePixels == n2.m_nSizePixels
; 164  : 			&& n1.m_nWeight == n2.m_nWeight
; 165  : 			&& n1.m_bUnderline == n2.m_bUnderline
; 166  : 			&& n1.m_bStrike == n2.m_bStrike
; 167  : 			&& n1.m_cCharSet == n2.m_cCharSet
; 168  : 			&& !_tcscmp( n1.m_szFontName, n2.m_szFontName );

	xor	eax, eax

; 169  : 	}

	pop	ebp
	ret	0
?ElementsTheSame@Container@@YAHUFontDef@GS@@0@Z ENDP	; Container::ElementsTheSame
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXABUFontDef@GS@@PAUFontInfo@CDrawContext@4@@Z
_TEXT	SEGMENT
_pEntry$57637 = -4
_key$ = 8
_value$ = 12
?SetAt@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXABUFontDef@GS@@PAUFontInfo@CDrawContext@4@@Z PROC NEAR ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::SetAt, COMDAT

; 188  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ecx

; 189  : 	PI_uint32 uHash = HashIt( key ) % m_uBucketArrayLength;

	mov	eax, DWORD PTR _key$[ebp]
	push	ebx
	mov	ebx, ecx
	xor	edx, edx
	mov	eax, DWORD PTR [eax+144]
	push	esi
	div	DWORD PTR [ebx+4]

; 190  : 	MapBucketEntry<Key, Value>	*pEntry = Find( uHash , key);

	mov	ecx, DWORD PTR [ebx+8]
	push	edi
	mov	eax, edx
	shl	eax, 2
	mov	DWORD PTR -8+[ebp], eax
	mov	eax, DWORD PTR [eax+ecx]
	test	eax, eax
	mov	DWORD PTR _pEntry$57637[ebp], eax
	je	SHORT $L57640
	jmp	SHORT $L57638
$L57653:
	mov	eax, DWORD PTR _pEntry$57637[ebp]
$L57638:
	sub	esp, 148				; 00000094H
	mov	esi, DWORD PTR _key$[ebp]
	push	37					; 00000025H
	pop	ecx
	mov	edi, esp
	sub	esp, 148				; 00000094H
	rep movsd
	push	37					; 00000025H
	lea	esi, DWORD PTR [eax+4]
	pop	ecx
	mov	edi, esp
	rep movsd
	call	?ElementsTheSame@Container@@YAHUFontDef@GS@@0@Z ; Container::ElementsTheSame
	add	esp, 296				; 00000128H
	test	eax, eax
	mov	eax, DWORD PTR _pEntry$57637[ebp]
	jne	SHORT $L57636
	mov	eax, DWORD PTR [eax+152]
	test	eax, eax
	mov	DWORD PTR _pEntry$57637[ebp], eax
	jne	SHORT $L57653
$L57640:
	xor	eax, eax
$L57636:

; 191  : 
; 192  : 	if( pEntry )

	test	eax, eax
	je	SHORT $L56736

; 193  : 	{
; 194  : 		pEntry -> m_Value = value;

	mov	ecx, DWORD PTR _value$[ebp]
	mov	DWORD PTR [eax], ecx

; 195  : 	}
; 196  : 	else

	jmp	SHORT $L56743
$L56736:

; 197  : 	{
; 198  : 		MapBucketEntry<Key, Value>	*pEntry = new MapBucketEntry<Key, Value>;

	push	156					; 0000009cH
	call	??2@YAPAXI@Z				; operator new
	test	eax, eax
	pop	ecx
	je	SHORT $L57622
	and	DWORD PTR [eax+152], 0
	jmp	SHORT $L57623
$L57622:
	xor	eax, eax
$L57623:

; 199  : 		m_uCount++;
; 200  : 		pEntry -> m_Value = value;

	mov	ecx, DWORD PTR _value$[ebp]

; 201  : 		pEntry -> m_Key = key;

	mov	esi, DWORD PTR _key$[ebp]

; 202  : 
; 203  : 		MapBucketEntry<Key, Value>	*pRootEntry = m_pEntrys[uHash];

	mov	edx, DWORD PTR -8+[ebp]
	push	37					; 00000025H
	mov	DWORD PTR [eax], ecx
	lea	edi, DWORD PTR [eax+4]
	pop	ecx
	inc	DWORD PTR [ebx+12]
	rep movsd
	mov	ecx, DWORD PTR [ebx+8]
	add	ecx, edx
	mov	edx, DWORD PTR [ecx]

; 204  : 		m_pEntrys[uHash] = pEntry;

	mov	DWORD PTR [ecx], eax

; 205  : 		if( pRootEntry )

	test	edx, edx
	je	SHORT $L56742

; 206  : 		{
; 207  : 			pEntry -> m_pNext = pRootEntry;

	mov	DWORD PTR [eax+152], edx

; 208  : 		}
; 209  : 		else

	jmp	SHORT $L56743
$L56742:

; 210  : 		{
; 211  : 			pEntry -> m_pNext = NULL;

	and	DWORD PTR [eax+152], 0
$L56743:

; 212  : 		}
; 213  : 	}
; 214  : }

	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	8
?SetAt@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXABUFontDef@GS@@PAUFontInfo@CDrawContext@4@@Z ENDP ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::SetAt
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ PROC NEAR ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::RemoveAll, COMDAT

; 57   : {

	push	esi
	mov	esi, ecx

; 58   : 	if( m_uCount )

	cmp	DWORD PTR [esi+12], 0
	je	SHORT $L56747
	push	edi

; 59   : 	{
; 60   : 		for( PI_uint32 i = 0; i < m_uBucketArrayLength; i++ )

	xor	edi, edi
	cmp	DWORD PTR [esi+4], edi
	jbe	SHORT $L56751
	push	ebx
$L56749:

; 61   : 		{
; 62   : 			MapBucketEntry<Key, Value> *pEntry = m_pEntrys[i];

	mov	eax, DWORD PTR [esi+8]
	mov	eax, DWORD PTR [eax+edi*4]

; 63   : 			while( pEntry )

	test	eax, eax
	je	SHORT $L56755
$L56754:

; 64   : 			{
; 65   : 				MapBucketEntry<Key, Value> *pEntryNext = pEntry -> m_pNext;

	mov	ebx, DWORD PTR [eax+152]

; 66   : 				delete pEntry;

	push	eax
	call	??3@YAXPAX@Z				; operator delete
	test	ebx, ebx
	pop	ecx

; 67   : 				pEntry = pEntryNext;

	mov	eax, ebx
	jne	SHORT $L56754
$L56755:

; 68   : 			}
; 69   : 
; 70   : 			m_pEntrys[i] = NULL;

	mov	eax, DWORD PTR [esi+8]
	and	DWORD PTR [eax+edi*4], 0
	inc	edi
	cmp	edi, DWORD PTR [esi+4]
	jb	SHORT $L56749
	pop	ebx
$L56751:

; 71   : 		}
; 72   : 
; 73   : 		m_uCount = 0;

	and	DWORD PTR [esi+12], 0
	pop	edi
$L56747:
	pop	esi

; 74   : 	}
; 75   : }

	ret	0
?RemoveAll@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ENDP ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::RemoveAll
_TEXT	ENDS
;	COMDAT ??1?$CMap@KPAUHBRUSH__@@@Container@@UAE@XZ
_TEXT	SEGMENT
??1?$CMap@KPAUHBRUSH__@@@Container@@UAE@XZ PROC NEAR	; Container::CMap<unsigned long,HBRUSH__ *>::~CMap<unsigned long,HBRUSH__ *>, COMDAT

; 21   : {

	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CMap@KPAUHBRUSH__@@@Container@@6B@ ; Container::CMap<unsigned long,HBRUSH__ *>::`vftable'

; 22   : 	RemoveAll();

	call	?RemoveAll@?$CMap@KPAUHBRUSH__@@@Container@@QAEXXZ ; Container::CMap<unsigned long,HBRUSH__ *>::RemoveAll

; 23   : 	delete [] m_pEntrys;

	push	DWORD PTR [esi+8]
	call	??3@YAXPAX@Z				; operator delete

; 24   : 	m_pEntrys = NULL;

	and	DWORD PTR [esi+8], 0
	pop	ecx
	pop	esi

; 25   : }

	ret	0
??1?$CMap@KPAUHBRUSH__@@@Container@@UAE@XZ ENDP		; Container::CMap<unsigned long,HBRUSH__ *>::~CMap<unsigned long,HBRUSH__ *>
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@KPAUHBRUSH__@@@Container@@QAEPAPAUHBRUSH__@@ABK@Z
_TEXT	SEGMENT
_key$ = 8
?Lookup@?$CMap@KPAUHBRUSH__@@@Container@@QAEPAPAUHBRUSH__@@ABK@Z PROC NEAR ; Container::CMap<unsigned long,HBRUSH__ *>::Lookup, COMDAT

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
	je	SHORT $L57679
	mov	esi, DWORD PTR [esi]
$L57677:
	cmp	DWORD PTR [edx+4], esi
	je	SHORT $L57682
	mov	edx, DWORD PTR [edx+8]
	test	edx, edx
	jne	SHORT $L57677
$L57679:
	xor	eax, eax
$L57675:
	pop	esi

; 114  : 	return pEntry != NULL ? &pEntry -> m_Value : NULL;
; 115  : }

	ret	4
$L57682:

; 112  : 	PI_uint32 uHash = HashIt( key ) % m_uBucketArrayLength;
; 113  : 	MapBucketEntry<Key, Value>	*pEntry = Find( uHash, key );

	mov	eax, edx
	jmp	SHORT $L57675
?Lookup@?$CMap@KPAUHBRUSH__@@@Container@@QAEPAPAUHBRUSH__@@ABK@Z ENDP ; Container::CMap<unsigned long,HBRUSH__ *>::Lookup
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@KPAUHBRUSH__@@@Container@@QAEXABKPAUHBRUSH__@@@Z
_TEXT	SEGMENT
_key$ = 8
_value$ = 12
?SetAt@?$CMap@KPAUHBRUSH__@@@Container@@QAEXABKPAUHBRUSH__@@@Z PROC NEAR ; Container::CMap<unsigned long,HBRUSH__ *>::SetAt, COMDAT

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
	je	SHORT $L57703
	mov	ecx, DWORD PTR [ebx]
$L57701:
	cmp	DWORD PTR [eax+4], ecx
	je	SHORT $L57699
	mov	eax, DWORD PTR [eax+8]
	test	eax, eax
	jne	SHORT $L57701
$L57703:
	xor	eax, eax
$L57699:

; 191  : 
; 192  : 	if( pEntry )

	test	eax, eax
	je	SHORT $L56798

; 193  : 	{
; 194  : 		pEntry -> m_Value = value;

	mov	ecx, DWORD PTR _value$[esp+8]
	mov	DWORD PTR [eax], ecx

; 195  : 	}
; 196  : 	else

	jmp	SHORT $L56805
$L56798:

; 197  : 	{
; 198  : 		MapBucketEntry<Key, Value>	*pEntry = new MapBucketEntry<Key, Value>;

	push	12					; 0000000cH
	call	??2@YAPAXI@Z				; operator new
	test	eax, eax
	pop	ecx
	je	SHORT $L57687
	and	DWORD PTR [eax+8], 0
	jmp	SHORT $L57688
$L57687:
	xor	eax, eax
$L57688:

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
	je	SHORT $L56804

; 206  : 		{
; 207  : 			pEntry -> m_pNext = pRootEntry;

	mov	DWORD PTR [eax+8], edx

; 208  : 		}
; 209  : 		else

	jmp	SHORT $L56805
$L56804:

; 210  : 		{
; 211  : 			pEntry -> m_pNext = NULL;

	and	DWORD PTR [eax+8], 0
$L56805:
	pop	edi
	pop	esi
	pop	ebx

; 212  : 		}
; 213  : 	}
; 214  : }

	ret	8
?SetAt@?$CMap@KPAUHBRUSH__@@@Container@@QAEXABKPAUHBRUSH__@@@Z ENDP ; Container::CMap<unsigned long,HBRUSH__ *>::SetAt
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@KPAUHBRUSH__@@@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CMap@KPAUHBRUSH__@@@Container@@QAEXXZ PROC NEAR ; Container::CMap<unsigned long,HBRUSH__ *>::RemoveAll, COMDAT

; 57   : {

	push	esi
	mov	esi, ecx

; 58   : 	if( m_uCount )

	cmp	DWORD PTR [esi+12], 0
	je	SHORT $L56809
	push	edi

; 59   : 	{
; 60   : 		for( PI_uint32 i = 0; i < m_uBucketArrayLength; i++ )

	xor	edi, edi
	cmp	DWORD PTR [esi+4], edi
	jbe	SHORT $L56813
	push	ebx
$L56811:

; 61   : 		{
; 62   : 			MapBucketEntry<Key, Value> *pEntry = m_pEntrys[i];

	mov	eax, DWORD PTR [esi+8]
	mov	eax, DWORD PTR [eax+edi*4]

; 63   : 			while( pEntry )

	test	eax, eax
	je	SHORT $L56817
$L56816:

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
	jne	SHORT $L56816
$L56817:

; 68   : 			}
; 69   : 
; 70   : 			m_pEntrys[i] = NULL;

	mov	eax, DWORD PTR [esi+8]
	and	DWORD PTR [eax+edi*4], 0
	inc	edi
	cmp	edi, DWORD PTR [esi+4]
	jb	SHORT $L56811
	pop	ebx
$L56813:

; 71   : 		}
; 72   : 
; 73   : 		m_uCount = 0;

	and	DWORD PTR [esi+12], 0
	pop	edi
$L56809:
	pop	esi

; 74   : 	}
; 75   : }

	ret	0
?RemoveAll@?$CMap@KPAUHBRUSH__@@@Container@@QAEXXZ ENDP	; Container::CMap<unsigned long,HBRUSH__ *>::RemoveAll
_TEXT	ENDS
;	COMDAT ??1?$CMap@KPAUHPEN__@@@Container@@UAE@XZ
_TEXT	SEGMENT
??1?$CMap@KPAUHPEN__@@@Container@@UAE@XZ PROC NEAR	; Container::CMap<unsigned long,HPEN__ *>::~CMap<unsigned long,HPEN__ *>, COMDAT

; 21   : {

	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CMap@KPAUHPEN__@@@Container@@6B@ ; Container::CMap<unsigned long,HPEN__ *>::`vftable'

; 22   : 	RemoveAll();

	call	?RemoveAll@?$CMap@KPAUHPEN__@@@Container@@QAEXXZ ; Container::CMap<unsigned long,HPEN__ *>::RemoveAll

; 23   : 	delete [] m_pEntrys;

	push	DWORD PTR [esi+8]
	call	??3@YAXPAX@Z				; operator delete

; 24   : 	m_pEntrys = NULL;

	and	DWORD PTR [esi+8], 0
	pop	ecx
	pop	esi

; 25   : }

	ret	0
??1?$CMap@KPAUHPEN__@@@Container@@UAE@XZ ENDP		; Container::CMap<unsigned long,HPEN__ *>::~CMap<unsigned long,HPEN__ *>
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@KPAUHPEN__@@@Container@@QAEPAPAUHPEN__@@ABK@Z
_TEXT	SEGMENT
_key$ = 8
?Lookup@?$CMap@KPAUHPEN__@@@Container@@QAEPAPAUHPEN__@@ABK@Z PROC NEAR ; Container::CMap<unsigned long,HPEN__ *>::Lookup, COMDAT

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
	je	SHORT $L57737
	mov	esi, DWORD PTR [esi]
$L57735:
	cmp	DWORD PTR [edx+4], esi
	je	SHORT $L57740
	mov	edx, DWORD PTR [edx+8]
	test	edx, edx
	jne	SHORT $L57735
$L57737:
	xor	eax, eax
$L57733:
	pop	esi

; 114  : 	return pEntry != NULL ? &pEntry -> m_Value : NULL;
; 115  : }

	ret	4
$L57740:

; 112  : 	PI_uint32 uHash = HashIt( key ) % m_uBucketArrayLength;
; 113  : 	MapBucketEntry<Key, Value>	*pEntry = Find( uHash, key );

	mov	eax, edx
	jmp	SHORT $L57733
?Lookup@?$CMap@KPAUHPEN__@@@Container@@QAEPAPAUHPEN__@@ABK@Z ENDP ; Container::CMap<unsigned long,HPEN__ *>::Lookup
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@KPAUHPEN__@@@Container@@QAEXABKPAUHPEN__@@@Z
_TEXT	SEGMENT
_key$ = 8
_value$ = 12
?SetAt@?$CMap@KPAUHPEN__@@@Container@@QAEXABKPAUHPEN__@@@Z PROC NEAR ; Container::CMap<unsigned long,HPEN__ *>::SetAt, COMDAT

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
	je	SHORT $L57761
	mov	ecx, DWORD PTR [ebx]
$L57759:
	cmp	DWORD PTR [eax+4], ecx
	je	SHORT $L57757
	mov	eax, DWORD PTR [eax+8]
	test	eax, eax
	jne	SHORT $L57759
$L57761:
	xor	eax, eax
$L57757:

; 191  : 
; 192  : 	if( pEntry )

	test	eax, eax
	je	SHORT $L56859

; 193  : 	{
; 194  : 		pEntry -> m_Value = value;

	mov	ecx, DWORD PTR _value$[esp+8]
	mov	DWORD PTR [eax], ecx

; 195  : 	}
; 196  : 	else

	jmp	SHORT $L56866
$L56859:

; 197  : 	{
; 198  : 		MapBucketEntry<Key, Value>	*pEntry = new MapBucketEntry<Key, Value>;

	push	12					; 0000000cH
	call	??2@YAPAXI@Z				; operator new
	test	eax, eax
	pop	ecx
	je	SHORT $L57745
	and	DWORD PTR [eax+8], 0
	jmp	SHORT $L57746
$L57745:
	xor	eax, eax
$L57746:

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
	je	SHORT $L56865

; 206  : 		{
; 207  : 			pEntry -> m_pNext = pRootEntry;

	mov	DWORD PTR [eax+8], edx

; 208  : 		}
; 209  : 		else

	jmp	SHORT $L56866
$L56865:

; 210  : 		{
; 211  : 			pEntry -> m_pNext = NULL;

	and	DWORD PTR [eax+8], 0
$L56866:
	pop	edi
	pop	esi
	pop	ebx

; 212  : 		}
; 213  : 	}
; 214  : }

	ret	8
?SetAt@?$CMap@KPAUHPEN__@@@Container@@QAEXABKPAUHPEN__@@@Z ENDP ; Container::CMap<unsigned long,HPEN__ *>::SetAt
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@KPAUHPEN__@@@Container@@QAEXXZ
_TEXT	SEGMENT
?RemoveAll@?$CMap@KPAUHPEN__@@@Container@@QAEXXZ PROC NEAR ; Container::CMap<unsigned long,HPEN__ *>::RemoveAll, COMDAT

; 57   : {

	push	esi
	mov	esi, ecx

; 58   : 	if( m_uCount )

	cmp	DWORD PTR [esi+12], 0
	je	SHORT $L56870
	push	edi

; 59   : 	{
; 60   : 		for( PI_uint32 i = 0; i < m_uBucketArrayLength; i++ )

	xor	edi, edi
	cmp	DWORD PTR [esi+4], edi
	jbe	SHORT $L56874
	push	ebx
$L56872:

; 61   : 		{
; 62   : 			MapBucketEntry<Key, Value> *pEntry = m_pEntrys[i];

	mov	eax, DWORD PTR [esi+8]
	mov	eax, DWORD PTR [eax+edi*4]

; 63   : 			while( pEntry )

	test	eax, eax
	je	SHORT $L56878
$L56877:

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
	jne	SHORT $L56877
$L56878:

; 68   : 			}
; 69   : 
; 70   : 			m_pEntrys[i] = NULL;

	mov	eax, DWORD PTR [esi+8]
	and	DWORD PTR [eax+edi*4], 0
	inc	edi
	cmp	edi, DWORD PTR [esi+4]
	jb	SHORT $L56872
	pop	ebx
$L56874:

; 71   : 		}
; 72   : 
; 73   : 		m_uCount = 0;

	and	DWORD PTR [esi+12], 0
	pop	edi
$L56870:
	pop	esi

; 74   : 	}
; 75   : }

	ret	0
?RemoveAll@?$CMap@KPAUHPEN__@@@Container@@QAEXXZ ENDP	; Container::CMap<unsigned long,HPEN__ *>::RemoveAll
_TEXT	ENDS
;	COMDAT ?Next@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ
_TEXT	SEGMENT
?Next@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ PROC NEAR ; Container::CMapIter<GS::FontDef,GS::CDrawContext::FontInfo *>::Next, COMDAT

; 44   : 	if( m_pNext )

	mov	eax, DWORD PTR [ecx+8]
	push	esi
	test	eax, eax
	push	edi
	je	SHORT $L57778

; 45   : 	{
; 46   : 		m_pBucket = m_pNext;

	mov	DWORD PTR [ecx+4], eax

; 47   : 		m_pNext = m_pNext -> m_pNext;

	mov	eax, DWORD PTR [eax+152]

; 48   : 		return;

	jmp	SHORT $L57780
$L57778:

; 49   : 	}
; 50   : 
; 51   : 	//
; 52   : 	// Get next array bucket
; 53   : 	while( ++m_iEntry < m_Map.m_uBucketArrayLength )

	mov	edx, DWORD PTR [ecx]
	inc	DWORD PTR [ecx+12]
	mov	eax, DWORD PTR [ecx+12]
	mov	esi, DWORD PTR [edx+4]
	cmp	eax, esi
	jae	SHORT $L56891

; 54   : 	{
; 55   : 		MapBucketEntry<Key, Value> *pEntry = m_Map.m_pEntrys[m_iEntry];

	mov	edi, DWORD PTR [edx+8]
$L56890:
	mov	edx, DWORD PTR [edi+eax*4]

; 56   : 		if( pEntry )

	test	edx, edx
	jne	SHORT $L57779
	inc	eax
	cmp	eax, esi
	mov	DWORD PTR [ecx+12], eax
	jb	SHORT $L56890
$L56891:

; 61   : 		}
; 62   : 	}
; 63   : 
; 64   : 	m_pBucket = NULL;

	and	DWORD PTR [ecx+4], 0
$L56887:
	pop	edi
	pop	esi

; 65   : }

	ret	0
$L57779:
	mov	eax, DWORD PTR [edx+152]
	mov	DWORD PTR [ecx+4], edx
$L57780:

; 57   : 		{
; 58   : 			m_pBucket = pEntry;
; 59   : 			m_pNext = m_pBucket -> m_pNext;

	mov	DWORD PTR [ecx+8], eax

; 60   : 			return;

	jmp	SHORT $L56887
?Next@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ENDP ; Container::CMapIter<GS::FontDef,GS::CDrawContext::FontInfo *>::Next
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAX@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CArray@PAX@Container@@UAEPAXI@Z PROC NEAR		; Container::CArray<void *>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7?$CArray@PAX@Container@@6B@ ; Container::CArray<void *>::`vftable'
	call	?RemoveAll@?$CArray@PAX@Container@@QAEXXZ ; Container::CArray<void *>::RemoveAll
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L56911
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L56911:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CArray@PAX@Container@@UAEPAXI@Z ENDP		; Container::CArray<void *>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAEPAXI@Z PROC NEAR ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAE@XZ ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::~CMap<GS::FontDef,GS::CDrawContext::FontInfo *>
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L56914
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L56914:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@UAEPAXI@Z ENDP ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@KPAUHBRUSH__@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CMap@KPAUHBRUSH__@@@Container@@UAEPAXI@Z PROC NEAR ; Container::CMap<unsigned long,HBRUSH__ *>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1?$CMap@KPAUHBRUSH__@@@Container@@UAE@XZ ; Container::CMap<unsigned long,HBRUSH__ *>::~CMap<unsigned long,HBRUSH__ *>
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L56917
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L56917:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CMap@KPAUHBRUSH__@@@Container@@UAEPAXI@Z ENDP	; Container::CMap<unsigned long,HBRUSH__ *>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@KPAUHPEN__@@@Container@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_G?$CMap@KPAUHPEN__@@@Container@@UAEPAXI@Z PROC NEAR	; Container::CMap<unsigned long,HPEN__ *>::`scalar deleting destructor', COMDAT
	push	esi
	mov	esi, ecx
	call	??1?$CMap@KPAUHPEN__@@@Container@@UAE@XZ ; Container::CMap<unsigned long,HPEN__ *>::~CMap<unsigned long,HPEN__ *>
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $L56920
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
$L56920:
	mov	eax, esi
	pop	esi
	ret	4
??_G?$CMap@KPAUHPEN__@@@Container@@UAEPAXI@Z ENDP	; Container::CMap<unsigned long,HPEN__ *>::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?RemoveAt@?$CArray@PAX@Container@@QAEXII@Z	; Container::CArray<void *>::RemoveAt
;	COMDAT ?SetSize@?$CArray@PAX@Container@@QAE_NI@Z
_TEXT	SEGMENT
_uSize$ = 8
_pItems$56947 = -4
?SetSize@?$CArray@PAX@Container@@QAE_NI@Z PROC NEAR	; Container::CArray<void *>::SetSize, COMDAT

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
	jae	SHORT $L57850

; 52   : 	{
; 53   : 		if( m_uItemsAllocated < uSize )

	mov	eax, DWORD PTR [ebp+12]
	cmp	eax, esi
	jae	SHORT $L56944

; 54   : 		{
; 55   : 			PI_uint32 uAllocateExtra = m_uGrowBy = m_uGrowBy < 262144 ? m_uGrowBy << 2 : 262144;

	mov	ecx, DWORD PTR [ebp+8]
	mov	ebx, 262144				; 00040000H
	cmp	ecx, ebx
	jae	SHORT $L57811
	mov	ebx, ecx
	shl	ebx, 2
$L57811:

; 56   : 
; 57   : 			//
; 58   : 			// Check to see if our grow by is enough?
; 59   : 			if( m_uItemsAllocated + uAllocateExtra < uSize )

	lea	ecx, DWORD PTR [eax+ebx]
	mov	DWORD PTR [ebp+8], ebx
	cmp	ecx, esi
	jae	SHORT $L56946

; 60   : 			{
; 61   : 				//
; 62   : 				//	Nope, so we allocate more
; 63   : 				uAllocateExtra = uSize - m_uItemsAllocated;

	mov	ebx, esi
	sub	ebx, eax
$L56946:

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
	mov	DWORD PTR _pItems$56947[esp+20], edi
	test	ecx, ecx
	je	SHORT $L57816

; 69   : 				MoveItemsNoMemOverlap( m_pItems, pItems, m_uItemCount );

	mov	esi, DWORD PTR [ebp+4]
	rep movsd
	mov	esi, DWORD PTR _uSize$[esp+16]
$L57816:

; 70   : 			delete[] (PI_byte*)m_pItems;

	push	DWORD PTR [ebp+4]
	call	??3@YAXPAX@Z				; operator delete

; 71   : 			m_pItems = pItems;

	mov	eax, DWORD PTR _pItems$56947[esp+24]

; 72   : 			m_uItemsAllocated += uAllocateExtra;

	add	DWORD PTR [ebp+12], ebx
	pop	ecx
	mov	DWORD PTR [ebp+4], eax

; 73   : 			bReallocationNeeded = true;

	mov	bl, 1
$L56944:

; 74   : 		}
; 75   : 
; 76   : 		//
; 77   : 		// Constuct the new items
; 78   : 		if( uSize > m_uItemCount )

	mov	eax, DWORD PTR [ebp+16]
	cmp	esi, eax
	jbe	SHORT $L56969

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

	jmp	SHORT $L56969
$L57850:

; 87   : 	{
; 88   : 		//
; 89   : 		//	Check to see if we need to reduce the size
; 90   : 		if( m_uItemCount > uSize )

	jbe	SHORT $L56969

; 91   : 		{
; 92   : 			//
; 93   : 			//	Get rid of excess elements
; 94   : 			RemoveAt( uSize, m_uItemCount - uSize );

	sub	eax, esi
	mov	ecx, ebp
	push	eax
	push	esi
	call	?RemoveAt@?$CArray@PAX@Container@@QAEXII@Z ; Container::CArray<void *>::RemoveAt
$L56969:
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
?SetSize@?$CArray@PAX@Container@@QAE_NI@Z ENDP		; Container::CArray<void *>::SetSize
_TEXT	ENDS
;	COMDAT ?SetBucketSize@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXI@Z
_TEXT	SEGMENT
_uSize$ = 8
?SetBucketSize@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXI@Z PROC NEAR ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::SetBucketSize, COMDAT

; 30   : {

	push	esi
	mov	esi, ecx
	push	edi

; 31   : 	ASSERT( !GetSize() ); //Cannot be called unless map is empty (possible improvement would be to allow this).
; 32   : 	if( !GetSize() )

	cmp	DWORD PTR [esi+12], 0
	jne	SHORT $L56979

; 33   : 	{
; 34   : 		delete [] m_pEntrys;

	push	DWORD PTR [esi+8]
	call	??3@YAXPAX@Z				; operator delete

; 35   : 		m_pEntrys = NULL;

	and	DWORD PTR [esi+8], 0
	pop	ecx
$L56979:

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
	jbe	SHORT $L56984
	xor	eax, eax
	rep stosd
$L56984:
	pop	edi
	pop	esi

; 42   : 	{
; 43   : 		m_pEntrys[i] = NULL;
; 44   : 	} 
; 45   : }

	ret	4
?SetBucketSize@?$CMap@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXI@Z ENDP ; Container::CMap<GS::FontDef,GS::CDrawContext::FontInfo *>::SetBucketSize
_TEXT	ENDS
;	COMDAT ?SetBucketSize@?$CMap@KPAUHBRUSH__@@@Container@@QAEXI@Z
_TEXT	SEGMENT
_uSize$ = 8
?SetBucketSize@?$CMap@KPAUHBRUSH__@@@Container@@QAEXI@Z PROC NEAR ; Container::CMap<unsigned long,HBRUSH__ *>::SetBucketSize, COMDAT

; 30   : {

	push	esi
	mov	esi, ecx
	push	edi

; 31   : 	ASSERT( !GetSize() ); //Cannot be called unless map is empty (possible improvement would be to allow this).
; 32   : 	if( !GetSize() )

	cmp	DWORD PTR [esi+12], 0
	jne	SHORT $L57008

; 33   : 	{
; 34   : 		delete [] m_pEntrys;

	push	DWORD PTR [esi+8]
	call	??3@YAXPAX@Z				; operator delete

; 35   : 		m_pEntrys = NULL;

	and	DWORD PTR [esi+8], 0
	pop	ecx
$L57008:

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
	jbe	SHORT $L57013
	xor	eax, eax
	rep stosd
$L57013:
	pop	edi
	pop	esi

; 42   : 	{
; 43   : 		m_pEntrys[i] = NULL;
; 44   : 	} 
; 45   : }

	ret	4
?SetBucketSize@?$CMap@KPAUHBRUSH__@@@Container@@QAEXI@Z ENDP ; Container::CMap<unsigned long,HBRUSH__ *>::SetBucketSize
_TEXT	ENDS
;	COMDAT ?SetBucketSize@?$CMap@KPAUHPEN__@@@Container@@QAEXI@Z
_TEXT	SEGMENT
_uSize$ = 8
?SetBucketSize@?$CMap@KPAUHPEN__@@@Container@@QAEXI@Z PROC NEAR ; Container::CMap<unsigned long,HPEN__ *>::SetBucketSize, COMDAT

; 30   : {

	push	esi
	mov	esi, ecx
	push	edi

; 31   : 	ASSERT( !GetSize() ); //Cannot be called unless map is empty (possible improvement would be to allow this).
; 32   : 	if( !GetSize() )

	cmp	DWORD PTR [esi+12], 0
	jne	SHORT $L57041

; 33   : 	{
; 34   : 		delete [] m_pEntrys;

	push	DWORD PTR [esi+8]
	call	??3@YAXPAX@Z				; operator delete

; 35   : 		m_pEntrys = NULL;

	and	DWORD PTR [esi+8], 0
	pop	ecx
$L57041:

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
	jbe	SHORT $L57046
	xor	eax, eax
	rep stosd
$L57046:
	pop	edi
	pop	esi

; 42   : 	{
; 43   : 		m_pEntrys[i] = NULL;
; 44   : 	} 
; 45   : }

	ret	4
?SetBucketSize@?$CMap@KPAUHPEN__@@@Container@@QAEXI@Z ENDP ; Container::CMap<unsigned long,HPEN__ *>::SetBucketSize
_TEXT	ENDS
;	COMDAT ?Reset@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ
_TEXT	SEGMENT
?Reset@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ PROC NEAR ; Container::CMapIter<GS::FontDef,GS::CDrawContext::FontInfo *>::Reset, COMDAT

; 23   : 	m_pBucket = NULL;
; 24   : 
; 25   : 	//
; 26   : 	// Set pointers to first entry that contains some data
; 27   : 	for( m_iEntry = 0; m_iEntry < m_Map.m_uBucketArrayLength; m_iEntry++ )

	mov	eax, DWORD PTR [ecx]
	and	DWORD PTR [ecx+4], 0
	and	DWORD PTR [ecx+12], 0
	push	esi
	mov	esi, DWORD PTR [eax+4]
	push	edi
	test	esi, esi
	jbe	SHORT $L57069

; 28   : 	{
; 29   : 		MapBucketEntry<Key, Value> *pEntry = m_Map.m_pEntrys[m_iEntry];

	mov	edi, DWORD PTR [eax+8]
$L57067:
	mov	eax, DWORD PTR [ecx+12]
	mov	edx, DWORD PTR [edi+eax*4]

; 30   : 		if( pEntry )

	test	edx, edx
	jne	SHORT $L57884
	inc	eax
	cmp	eax, esi
	mov	DWORD PTR [ecx+12], eax
	jb	SHORT $L57067
	jmp	SHORT $L57069
$L57884:

; 31   : 		{
; 32   : 			m_pBucket = pEntry;
; 33   : 			m_pNext = m_pBucket -> m_pNext;

	mov	eax, DWORD PTR [edx+152]
	mov	DWORD PTR [ecx+4], edx
	mov	DWORD PTR [ecx+8], eax
$L57069:
	pop	edi
	pop	esi

; 34   : 			return;
; 35   : 		}
; 36   : 	}
; 37   : }

	ret	0
?Reset@?$CMapIter@UFontDef@GS@@PAUFontInfo@CDrawContext@2@@Container@@QAEXXZ ENDP ; Container::CMapIter<GS::FontDef,GS::CDrawContext::FontInfo *>::Reset
_TEXT	ENDS
EXTRN	__imp__memmove:NEAR
;	COMDAT ?RemoveAt@?$CArray@PAX@Container@@QAEXII@Z
_TEXT	SEGMENT
_iPos$ = 8
_uItems$ = 12
?RemoveAt@?$CArray@PAX@Container@@QAEXII@Z PROC NEAR	; Container::CArray<void *>::RemoveAt, COMDAT

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

	je	SHORT $L57898

; 200  : 		MoveItemsMemOverlap( &m_pItems[iPos + uItems], &m_pItems[iPos], uItemsToMove);

	shl	eax, 2
	add	edx, edi
	push	eax
	lea	eax, DWORD PTR [ecx+edx*4]
	push	eax
	push	ebx
	call	DWORD PTR __imp__memmove
	add	esp, 12					; 0000000cH
$L57898:

; 201  : 	m_uItemCount -= uItems;

	sub	DWORD PTR [esi+16], edi
	pop	edi
	pop	esi
	pop	ebx

; 202  : }

	ret	8
?RemoveAt@?$CArray@PAX@Container@@QAEXII@Z ENDP		; Container::CArray<void *>::RemoveAt
_TEXT	ENDS
END
