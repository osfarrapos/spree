// XMLPaser.cpp: implementation of the XMLPaser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLPaser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XMLPaser::XMLPaser()
{
	m_pNodeList = NULL;
	m_docPtr = NULL;
}

XMLPaser::~XMLPaser()
{
	ND_hrClose();
}

HRESULT XMLPaser::ND_hrOpen( LPCTSTR strFile )
{
	try 
	{
		ND_TESTHR( CoInitialize(NULL) );
		ND_TESTHR( m_docPtr.CreateInstance("msxml2.domdocument") );

		// load a document
		_variant_t varXml(strFile);
		_variant_t varOut(true); // BOOL 이 없다..
		varOut = m_docPtr->load(varXml);

		if ((bool)varOut == false)
			throw(0);

		BSTR bstrItem; 
		LPOLESTR pszDataW;
		AnsiToUnicode( _T("Worksheet"), &pszDataW );
		bstrItem = SysAllocString( pszDataW );
 
		m_pNodeList = m_docPtr->getElementsByTagName( bstrItem );

		if ( FAILED( m_pNodeList->get_length( &m_lTableCount ) ) )
			throw (0);
	} 
	catch (...)
	{
		return ( E_FAIL );
	}

	return ( S_OK );
}

HRESULT XMLPaser::ND_hrOpenMemory( LPCTSTR strFile )
{
	try 
	{
		ND_TESTHR( CoInitialize(NULL) );
		ND_TESTHR( m_docPtr.CreateInstance("msxml2.domdocument") );

		// load a document
		_variant_t varOut(true); // BOOL 이 없다..
		_bstr_t bsXML(strFile);

		varOut = m_docPtr->loadXML( BSTR(bsXML) );

		if ((bool)varOut == false)
			throw(0);

		BSTR bstrItem; 
		LPOLESTR pszDataW;
		AnsiToUnicode( _T("Worksheet"), &pszDataW );
		bstrItem = SysAllocString( pszDataW );
 
		m_pNodeList = m_docPtr->getElementsByTagName( bstrItem );

		if ( FAILED( m_pNodeList->get_length( &m_lTableCount ) ) )
			throw (0);
	} 
	catch (...)
	{
		return ( E_FAIL );
	}

	return ( S_OK );
}

HRESULT XMLPaser::ND_hrClose()
{
	/*if ( m_pNodeList )
		m_pNodeList->Release();
	if ( m_docPtr )
		m_docPtr->Release();*/

	m_pNodeList = NULL;
	m_docPtr = NULL;

	CoUninitialize();
	return ( S_OK );
}

// 이 함수의 리턴 값인 IXMLDOMNode 포인터는 쓰고 난 후에 Release를 해야한다.
MSXML2::IXMLDOMNode* XMLPaser::ND_pGetTableNode( LPCTSTR strTable )
{
	MSXML2::IXMLDOMNode *pWorksheetNode = NULL;
	MSXML2::IXMLDOMNode *pTableNode = NULL;
	long lFoundWorksheet = -1;

	try
	{
		m_pNodeList->reset();

		for ( long i = 0; i < m_lTableCount; i++ )
		{
			m_pNodeList->get_item( i, &pWorksheetNode );

			if ( pWorksheetNode )
			{
				MSXML2::IXMLDOMNamedNodeMap* pNodeMap;

				if ( SUCCEEDED( pWorksheetNode->get_attributes( &pNodeMap ) ) && pNodeMap )
				{
					MSXML2::IXMLDOMNode *pAttributeNode = NULL;
					pNodeMap->get_item( 0, &pAttributeNode );

					if ( pAttributeNode )
					{
						BSTR bstrName;
						_variant_t varTableName;
						pAttributeNode->get_nodeName( &bstrName );

						LPSTR pszName;
						UnicodeToAnsi( bstrName, &pszName );
						if ( strcmp( pszName, _T("ss:Name") ) == 0 )
						{
							pAttributeNode->get_nodeValue( &varTableName );

							if ( varTableName.vt == VT_BSTR )
							{
								LPSTR pszTableName;
								UnicodeToAnsi( varTableName.bstrVal, &pszTableName );

								// Found Worksheet!
								if ( stricmp( strTable, pszTableName ) == 0 )
								{
									lFoundWorksheet = i;
								}

								CoTaskMemFree( pszTableName );
							}
						}

						CoTaskMemFree( pszName );

						pAttributeNode->Release();
						VariantClear( &varTableName );
						::SysFreeString( bstrName );
					}
					pNodeMap->Release();
				}
				pWorksheetNode->Release();
			}
		}

		if ( lFoundWorksheet < 0 )
			throw ( 0 );

		m_pNodeList->get_item( lFoundWorksheet, &pWorksheetNode );

		if ( pWorksheetNode )
		{
			MSXML2::IXMLDOMNodeList* pNodeList;
			pWorksheetNode->get_childNodes( &pNodeList );
			if ( pNodeList )
			{
				for ( long i = 0; i < pNodeList->Getlength(); i++ )
				{
					pNodeList->get_item( i, &pTableNode );
					if ( pTableNode )
					{
						BSTR bstrName;
						pTableNode->get_nodeName( &bstrName );

						LPSTR pszName;
						UnicodeToAnsi( bstrName, &pszName );
						if ( strcmp( pszName, _T("Table") ) == 0 )
						{
							CoTaskMemFree( pszName );
							::SysFreeString( bstrName );
							pNodeList->Release();
							pWorksheetNode->Release();
							return ( pTableNode );
						}
						CoTaskMemFree( pszName );
						::SysFreeString( bstrName );
					}
				}
				pNodeList->Release();
			}
			pWorksheetNode->Release();
		}
		return ( NULL );
	}
	catch(...)
	{
		return ( NULL );
	}
}


BOOL XMLPaser::ND_isThereTable( LPCTSTR strTable )
{
	MSXML2::IXMLDOMNode *pNode = NULL;
	BOOL bResult = FALSE;

	try
	{
		pNode = ND_pGetTableNode( strTable );

		if ( pNode )
			bResult = TRUE;
		else
			bResult = FALSE;

		pNode->Release();

		return ( bResult );
	}
	catch(...)
	{
		return ( FALSE );
	}
}

UINT XMLPaser::ND_nGetTableRowCount( LPCTSTR strTable )
{
	UINT nItemCount = 0;
	MSXML2::IXMLDOMNode *pNode = NULL;

	try
	{
		pNode = ND_pGetTableNode( strTable );

		if ( pNode )
		{
			MSXML2::IXMLDOMNamedNodeMap* pNodeMap;

			if ( SUCCEEDED( pNode->get_attributes( &pNodeMap ) ) && pNodeMap )
			{
				MSXML2::IXMLDOMNode *pAttributeNode = NULL;

				for ( long i = 0; i < pNodeMap->Getlength(); i++ )
				{
					pNodeMap->get_item( i, &pAttributeNode );

					if ( pAttributeNode )
					{
						BSTR bstrName;
						_variant_t varRowName;
						pAttributeNode->get_nodeName( &bstrName );

						LPSTR pszName;
						UnicodeToAnsi( bstrName, &pszName );
						if ( strcmp( pszName, _T("ss:ExpandedRowCount") ) == 0 )
						{
							pAttributeNode->get_nodeValue( &varRowName );

							if ( varRowName.vt == VT_BSTR )
							{
								LPSTR pszTableName;
								UnicodeToAnsi( varRowName.bstrVal, &pszTableName );
								nItemCount = atoi( pszTableName );
								CoTaskMemFree( pszTableName );
							}
						}

						CoTaskMemFree( pszName );

						pAttributeNode->Release();
						VariantClear( &varRowName );
						::SysFreeString( bstrName );
					}
				}
				pNodeMap->Release();
			}
			pNode->Release();
		}

		return ( nItemCount );
	}
	catch(...)
	{
		return ( 0 );
	}
}

/*
*					
BSTR bstrName2;
pRowNode->get_text( &bstrName2 );
::SysFreeString( bstrName2 );
pRowNode->get_baseName( &bstrName2 );
::SysFreeString( bstrName2 );
 */

HRESULT XMLPaser::ND_hrGetTableValueV( LPCTSTR strTable, LPCTSTR strField, 
		UINT idxRow, LONGLONG* pVal )
{
	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNode *pNode = NULL;
	long idxColumn = -1;
	long lTableChilds = 0;
	long lfirstRow = 0;

	try
	{
		pNode = ND_pGetTableNode( strTable );

		if ( pNode )
		{
			MSXML2::IXMLDOMNodeList* pRowNodeList;
			pNode->get_childNodes( &pRowNodeList );

			if ( pRowNodeList )
			{
				pRowNodeList->get_length( &lTableChilds );

				MSXML2::IXMLDOMNode *pRowNode;
				BSTR bstrName;
				LPSTR pszName;

				for ( long ll = 0; ll < lTableChilds; ll++ )
				{
					pRowNodeList->get_item( ll, &pRowNode );
					pRowNode->get_baseName( &bstrName );

					// 처음 Row가 나오는 순간
					UnicodeToAnsi( bstrName, &pszName );
					if ( stricmp( pszName, _T("Row") ) == 0 )
					{
						lfirstRow = ll;
						CoTaskMemFree( pszName );
						::SysFreeString( bstrName );
						break;
					}

					CoTaskMemFree( pszName );
					::SysFreeString( bstrName );
				}

				if ( lfirstRow >= lTableChilds )
				{
					return ( E_FAIL );
				}

				pRowNodeList->get_item( lfirstRow, &pRowNode );

				if ( pRowNode )
				{
					MSXML2::IXMLDOMNodeList* pCellNodeList;
					pRowNode->get_childNodes( &pCellNodeList );

					if ( pCellNodeList )
					{
						long lCell = 0;
						pCellNodeList->get_length( &lCell );

						for ( long ll = 0; ll < lCell; ll++ )
						{
							MSXML2::IXMLDOMNode *pCellNode;
							pCellNodeList->get_item( ll, &pCellNode );

							if ( pCellNode )
							{
								BSTR bstrName;
								MSXML2::IXMLDOMNode *pDataNode;
								pCellNode->get_firstChild( &pDataNode );
								if ( pDataNode )
								{
									LPSTR pszColumnName;
									pDataNode->get_text( &bstrName );
									UnicodeToAnsi( bstrName, &pszColumnName );

									if ( stricmp( pszColumnName, strField ) == 0 )
									{
										idxColumn = ll;
									}

									CoTaskMemFree( pszColumnName );
									::SysFreeString( bstrName );
									pDataNode->Release();
								}
								pCellNode->Release();
							}
						}
						pCellNodeList->Release();
					}
					pRowNode->Release();
				}

				pRowNodeList->get_item( idxRow + lfirstRow + 1, &pRowNode );

				if ( pRowNode )
				{
					MSXML2::IXMLDOMNodeList* pCellNodeList;
					pRowNode->get_childNodes( &pCellNodeList );

					if ( pCellNodeList )
					{
						MSXML2::IXMLDOMNode *pDataNode;
						pCellNodeList->get_item( idxColumn, &pDataNode );

						
						if ( pDataNode )
						{
							BSTR bstrData;
							LPSTR pszData;
							pDataNode->get_text( &bstrData );							
							UnicodeToAnsi( bstrData, &pszData );

							*pVal = _atoi64( pszData );
							hr = S_OK;

							CoTaskMemFree( pszData );
							::SysFreeString( bstrData );
							pDataNode->Release();
						}
						pCellNodeList->Release();
					}
					pRowNode->Release();
				}
				pRowNodeList->Release();
			}
			pNode->Release();
		}

		return ( hr );
	}
	catch(...)
	{
		return ( E_FAIL );
	}
}

HRESULT XMLPaser::ND_hrGetTableValue( LPCTSTR strTable, LPCTSTR strField, 
		UINT idxRow, int* pVal )
{
	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNode *pNode = NULL;
	long idxColumn = -1;
	long lTableChilds = 0;
	long lfirstRow = 0;


	try
	{
		pNode = ND_pGetTableNode( strTable );

		if ( pNode )
		{
			MSXML2::IXMLDOMNodeList* pRowNodeList;
			pNode->get_childNodes( &pRowNodeList );

			if ( pRowNodeList )
			{
				pRowNodeList->get_length( &lTableChilds );

				MSXML2::IXMLDOMNode *pRowNode;
				BSTR bstrName;
				LPSTR pszName;

				for ( long ll = 0; ll < lTableChilds; ll++ )
				{
					pRowNodeList->get_item( ll, &pRowNode );
					pRowNode->get_baseName( &bstrName );

					// 처음 Row가 나오는 순간
					UnicodeToAnsi( bstrName, &pszName );
					if ( stricmp( pszName, _T("Row") ) == 0 )
					{
						lfirstRow = ll;
						CoTaskMemFree( pszName );
						::SysFreeString( bstrName );
						break;
					}

					CoTaskMemFree( pszName );
					::SysFreeString( bstrName );
				}

				if ( lfirstRow >= lTableChilds )
				{
					return ( E_FAIL );
				}

				pRowNodeList->get_item( lfirstRow, &pRowNode );

				if ( pRowNode )
				{
					MSXML2::IXMLDOMNodeList* pCellNodeList;
					pRowNode->get_childNodes( &pCellNodeList );

					if ( pCellNodeList )
					{
						long lCell = 0;
						pCellNodeList->get_length( &lCell );

						for ( long ll = 0; ll < lCell; ll++ )
						{
							MSXML2::IXMLDOMNode *pCellNode;
							pCellNodeList->get_item( ll, &pCellNode );

							if ( pCellNode )
							{
								BSTR bstrName;
								MSXML2::IXMLDOMNode *pDataNode;
								pCellNode->get_firstChild( &pDataNode );
								if ( pDataNode )
								{
									LPSTR pszColumnName;
									pDataNode->get_text( &bstrName );
									UnicodeToAnsi( bstrName, &pszColumnName );

									if ( stricmp( pszColumnName, strField ) == 0 )
									{
										idxColumn = ll;
									}

									CoTaskMemFree( pszColumnName );
									::SysFreeString( bstrName );
									pDataNode->Release();
								}
								pCellNode->Release();
							}
						}
						pCellNodeList->Release();
					}
					pRowNode->Release();
				}

				pRowNodeList->get_item( idxRow + lfirstRow + 1, &pRowNode );

				if ( pRowNode )
				{
					MSXML2::IXMLDOMNodeList* pCellNodeList;
					pRowNode->get_childNodes( &pCellNodeList );

					if ( pCellNodeList )
					{
						MSXML2::IXMLDOMNode *pDataNode;
						pCellNodeList->get_item( idxColumn, &pDataNode );

						if ( pDataNode )
						{
							BSTR bstrData;
							LPSTR pszData;
							pDataNode->get_text( &bstrData );							
							UnicodeToAnsi( bstrData, &pszData );

							*pVal = atoi( pszData );
							hr = S_OK;

							CoTaskMemFree( pszData );
							::SysFreeString( bstrData );
							pDataNode->Release();
						}
						pCellNodeList->Release();
					}
					pRowNode->Release();
				}
				pRowNodeList->Release();
			}
			pNode->Release();
		}

		return ( hr );
	}
	catch(...)
	{
		return ( E_FAIL );
	}
}

HRESULT XMLPaser::ND_hrGetTableValue( LPCTSTR strTable, LPCTSTR strField, 
		UINT idxRow, TCHAR* pVal )
{
	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNode *pNode = NULL;
	long idxColumn = -1;
	long lTableChilds = 0;
	long lfirstRow = 0;

	try
	{
		pNode = ND_pGetTableNode( strTable );

		if ( pNode )
		{
			MSXML2::IXMLDOMNodeList* pRowNodeList;
			pNode->get_childNodes( &pRowNodeList );

			if ( pRowNodeList )
			{
				pRowNodeList->get_length( &lTableChilds );

				MSXML2::IXMLDOMNode *pRowNode;
				BSTR bstrName;
				LPSTR pszName;

				for ( long ll = 0; ll < lTableChilds; ll++ )
				{
					pRowNodeList->get_item( ll, &pRowNode );
					pRowNode->get_baseName( &bstrName );

					// 처음 Row가 나오는 순간
					UnicodeToAnsi( bstrName, &pszName );
					if ( stricmp( pszName, _T("Row") ) == 0 )
					{
						lfirstRow = ll;
						CoTaskMemFree( pszName );
						::SysFreeString( bstrName );
						break;
					}

					CoTaskMemFree( pszName );
					::SysFreeString( bstrName );
				}

				if ( lfirstRow >= lTableChilds )
				{
					return ( E_FAIL );
				}

				pRowNodeList->get_item( lfirstRow, &pRowNode );

				if ( pRowNode )
				{
					MSXML2::IXMLDOMNodeList* pCellNodeList;
					pRowNode->get_childNodes( &pCellNodeList );

					if ( pCellNodeList )
					{
						long lCell = 0;
						pCellNodeList->get_length( &lCell );
						
						for ( long ll = 0; ll < lCell; ll++ )
						{
							MSXML2::IXMLDOMNode *pCellNode;
							pCellNodeList->get_item( ll, &pCellNode );

							if ( pCellNode )
							{
								MSXML2::IXMLDOMNode *pDataNode;
								pCellNode->get_firstChild( &pDataNode );
								if ( pDataNode )
								{
									LPSTR pszColumnName;
									pDataNode->get_text( &bstrName );
									UnicodeToAnsi( bstrName, &pszColumnName );

									if ( stricmp( pszColumnName, strField ) == 0 )
									{
										idxColumn = ll;
									}

									CoTaskMemFree( pszColumnName );
									::SysFreeString( bstrName );
									pDataNode->Release();
								}
								pCellNode->Release();
							}
						}
						pCellNodeList->Release();
					}
					pRowNode->Release();
				}

				pRowNodeList->get_item( idxRow + lfirstRow + 1, &pRowNode );

				if ( pRowNode )
				{
					MSXML2::IXMLDOMNodeList* pCellNodeList;
					pRowNode->get_childNodes( &pCellNodeList );

					if ( pCellNodeList )
					{
						MSXML2::IXMLDOMNode *pDataNode;
						pCellNodeList->get_item( idxColumn, &pDataNode );

						if ( pDataNode )
						{
							BSTR bstrData;
							LPSTR pszData;
							pDataNode->get_text( &bstrData );
							UnicodeToAnsi( bstrData, &pszData );

							strcpy( pVal, pszData );
							hr = S_OK;

							CoTaskMemFree( pszData );
							::SysFreeString( bstrData );
							pDataNode->Release();
						}
						pCellNodeList->Release();
					}
					pRowNode->Release();
				}
				pRowNodeList->Release();
			}
			pNode->Release();
		}

		return ( hr );
	}
	catch(...)
	{
		return ( E_FAIL );
	}
}
