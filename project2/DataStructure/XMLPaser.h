// XMLPaser.h: interface for the XMLPaser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLPASER_H__439DD14D_0293_4E50_B6BE_CB33EC52779E__INCLUDED_)
#define AFX_XMLPASER_H__439DD14D_0293_4E50_B6BE_CB33EC52779E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

#import "msxml3.dll"
using namespace MSXML2;

class XMLPaser  
{
public:
	XMLPaser();
	~XMLPaser();

	HRESULT			ND_hrOpen( LPCTSTR strFile );
	HRESULT			ND_hrOpenMemory( LPCTSTR strFile );
	HRESULT			ND_hrClose();
	UINT			ND_nGetTableRowCount( LPCTSTR strTable );
	BOOL			ND_isThereTable( LPCTSTR strTable );
	UINT			ND_nGetTableCount()		{ return ( m_lTableCount ); }

	HRESULT			ND_hrGetTableValue( LPCTSTR strTable, LPCTSTR strField, 
		UINT idxRow, int* pVal );
	HRESULT			ND_hrGetTableValueV( LPCTSTR strTable, LPCTSTR strField, 
		UINT idxRow, LONGLONG* pVal );
	HRESULT			ND_hrGetTableValue( LPCTSTR strTable, LPCTSTR strField, 
		UINT idxRow, TCHAR* pVal );

private:
	MSXML2::IXMLDOMDocument2Ptr	m_docPtr;
	MSXML2::IXMLDOMNodeListPtr	m_pNodeList;
	long						m_lTableCount;

	MSXML2::IXMLDOMNode*		ND_pGetTableNode( LPCTSTR strTable );
};

inline void ND_TESTHR( HRESULT _hr ) { if FAILED(_hr) throw(_hr); }


/*
 * AnsiToUnicode converts the ANSI string pszA to a Unicode string
 * and returns the Unicode string through ppszW. Space for the
 * the converted string is allocated by AnsiToUnicode.
*/ 
inline HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW)
{
   ULONG cCharacters;
   DWORD dwError;

   // If input is null then just return the same.
   if (NULL == pszA)
   {
      *ppszW = NULL;
      return NOERROR;
   }

   // Determine number of wide characters to be allocated for the
   // Unicode string.
   cCharacters =  strlen(pszA)+1;

   // Use of the OLE allocator is required if the resultant Unicode
   // string will be passed to another COM component and if that
   // component will free it. Otherwise you can use your own allocator.
   *ppszW = (LPOLESTR) CoTaskMemAlloc(cCharacters*2);
   if (NULL == *ppszW)
      return E_OUTOFMEMORY;

   // Covert to Unicode.
   if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
                                *ppszW, cCharacters))
   {
      dwError = GetLastError();
      CoTaskMemFree(*ppszW);
      *ppszW = NULL;
      return HRESULT_FROM_WIN32(dwError);
   }

   return NOERROR;
}


/*
 * UnicodeToAnsi converts the Unicode string pszW to an ANSI string
 * and returns the ANSI string through ppszA. Space for the
 * the converted string is allocated by UnicodeToAnsi.
*/
inline HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA)
{
   ULONG cbAnsi, cCharacters;
   DWORD dwError;

   // If input is null then just return the same.
   if (pszW == NULL)
   {
       *ppszA = NULL;
       return NOERROR;
   }

   cCharacters = wcslen(pszW)+1;
   // Determine number of bytes to be allocated for ANSI string. An
   // ANSI string can have at most 2 bytes per character (for Double
   // Byte Character Strings.)
   cbAnsi = cCharacters*2;

   // Use of the OLE allocator is not required because the resultant
   // ANSI string will never be passed to another COM component. You
   // can use your own allocator.
   *ppszA = (LPSTR) CoTaskMemAlloc(cbAnsi);
   if (NULL == *ppszA)
      return E_OUTOFMEMORY;

   // Convert to ANSI.
   if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
                                cbAnsi, NULL, NULL))
   {
      dwError = GetLastError();
      CoTaskMemFree(*ppszA);
      *ppszA = NULL;
      return HRESULT_FROM_WIN32(dwError);
   }

   return NOERROR;
}

#endif // !defined(AFX_XMLPASER_H__439DD14D_0293_4E50_B6BE_CB33EC52779E__INCLUDED_)
