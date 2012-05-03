#include "stdafx.h"
#include <direct.h>
#include "XRegistry.h"


BOOL _XREGISTRY_ITEM::Load( FILE* pFile )
{
	if( !pFile ) return FALSE;

	int stringlength = 0;
	if( fread( &stringlength, sizeof(int), 1, pFile ) != 1 ) return FALSE;
	
	char buffer[1024];

	if( stringlength > 0 )
	{
		memset( buffer, 0, sizeof(char) * 1024 );
		if( fread( buffer, sizeof(char) * stringlength, 1, pFile ) != 1 ) return FALSE;
		registrykeyname = buffer;
	}
	
	if( fread( &integeritem, sizeof(DWORD), 1, pFile ) != 1 ) return FALSE;
	
	stringlength = 0;
	if( fread( &stringlength, sizeof(int), 1, pFile ) != 1 ) return FALSE;

	if( stringlength > 0 )
	{
		memset( buffer, 0, sizeof(char) * 1024 );
		if( fread( buffer, sizeof(char) * stringlength, 1, pFile ) != 1 ) return FALSE;
		stringitem = buffer;
	}
	
	if( fread( &binarysize, sizeof(UINT), 1, pFile ) != 1 ) return FALSE;

	if( pbinaryitem )
	{
		delete[] pbinaryitem;
		pbinaryitem = NULL;
	}

	if( binarysize > 0 )
	{
		pbinaryitem = new BYTE[ binarysize ];
		if( !pbinaryitem )
		{
			//_XFatalError( _T("Could not allocate binary memory [%d]"), binarysize );
			return FALSE;
		}

		if( fread( pbinaryitem, sizeof(BYTE) * binarysize, 1, pFile ) != 1 ) return FALSE;
	}	
	
	return TRUE;
}

BOOL _XREGISTRY_ITEM::Save( FILE* pFile )
{
	if( !pFile ) return FALSE;

	int stringlength = registrykeyname.size();	
	fwrite( &stringlength, sizeof(int), 1, pFile );	
	if( stringlength > 0 )
		fwrite( registrykeyname.c_str(), sizeof(char) * stringlength, 1, pFile );
	
	fwrite( &integeritem, sizeof(DWORD), 1, pFile );
	
	stringlength = stringitem.size();
	fwrite( &stringlength, sizeof(int), 1, pFile );	
	if( stringlength > 0 )
		fwrite( stringitem.c_str(), sizeof(char) * stringlength, 1, pFile );
	
	fwrite( &binarysize, sizeof(UINT), 1, pFile );
	if( binarysize > 0 )
		fwrite( pbinaryitem, sizeof(BYTE) * binarysize, 1, pFile );

	return TRUE;
}

void _XRegistryManager::SetRegistryFolder( const char* szPath )
{
	memset( m_RegistryPath, 0, sizeof(char) * _MAX_PATH );
	strcpy( m_RegistryPath, szPath );
}

void _XRegistryManager::SetRegistryFolder( const TCHAR* szPath )
{
	char mbcsBuffer[_MAX_PATH+1];	
	memset( mbcsBuffer, 0, sizeof(char) * _MAX_PATH+1 );
	WideCharToMultiByte(CP_ACP, 0, szPath, -1, mbcsBuffer, _MAX_PATH, NULL, NULL);	
	
	memset( m_RegistryPath, 0, sizeof(char) * _MAX_PATH );
	strcpy( m_RegistryPath, mbcsBuffer );
}

BOOL _XRegistryManager::LoadRegistry( void )
{
	_chdir( m_RegistryPath );

	FILE* pFile = NULL;
	pFile = fopen( "ndreg.xrg", "rb" );
	if( !pFile )
	{
		//_XFatalError( _T("Could not open xkernel ini file [%s]"), m_RegistryPath );
		return FALSE;
	}
	
	int count = 0;
	if( fread( &count, sizeof(int), 1, pFile ) != 1 )
	{
		fclose( pFile )	;
		return FALSE;
	}
		 
	for( int i = 0; i < count; i++ )
	{
		_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;		
		if( !pItem->Load( pFile ) )
		{
			fclose( pFile )	;
			return FALSE;
		}
		
		m_svRegistryvector.push_back(pItem);
		int id = m_svRegistryvector.size();		
		m_smRegistryTable.insert(smdef_XREGISTRY::value_type(pItem->registrykeyname, id-1));
	}

	fclose( pFile )	;

	return TRUE;
}

BOOL _XRegistryManager::SaveRegistry( void )
{
	_chdir( m_RegistryPath );

	FILE* pFile = NULL;
	pFile = fopen( "ndreg.xrg", "wb" );
	if( !pFile )
	{
		//_XFatalError( _T("Could not open xkernel ini file [%s]"), m_RegistryPath );
		return FALSE;
	}
	
	int count = m_svRegistryvector.size();
	fwrite( &count, sizeof(int), 1, pFile );

	for(svdef_XREGISTRY::iterator it = m_svRegistryvector.begin(); it != m_svRegistryvector.end(); ++it)
	{
		if( !((*it)->Save( pFile )) )
		{
			fclose( pFile )	;
			return FALSE;
		}
	}
	
	fclose( pFile )	;

	return TRUE;
}

BOOL _XRegistryManager::ImportWindowsRegistry( void )
{
	Reset();

	HKEY      subKey;
    TCHAR	  szKeyName[MAX_PATH + 1];
	
	LONG Status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Indy21\\NineDragons"), 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &subKey);
    if ( Status != ERROR_SUCCESS )
	{
        return FALSE;
    }

	LONG	retcode = 0;
	BYTE	Result[1024];
	DWORD	Size;
	DWORD	Type;
	int		i = 0;

	memset( szKeyName, 0, sizeof(TCHAR) * MAX_PATH+1 );
	while( 1 )
    {
		DWORD datalen = MAX_PATH;
		Size = sizeof(BYTE) * 1024;
		memset( Result, 0, Size );
		retcode = RegEnumValue(subKey, i , szKeyName, &datalen, NULL , &Type , (LPBYTE)Result, &Size );
		if( retcode != ERROR_SUCCESS )
		{
			break;
		}

		char mbcsBuffer[_MAX_PATH+1];
		memset( mbcsBuffer, 0, sizeof(char) * _MAX_PATH+1 );
		WideCharToMultiByte(CP_ACP, 0, szKeyName, -1, mbcsBuffer, _MAX_PATH, NULL, NULL);	
		
		switch( Type ) 
		{
		case REG_SZ			:
			{
				_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;				
				pItem->registrykeyname = mbcsBuffer;

				char strBuffer[_MAX_PATH+1];
				memset( strBuffer, 0, sizeof(char) * _MAX_PATH+1 );
				WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)Result, -1, strBuffer, _MAX_PATH, NULL, NULL);	
				pItem->stringitem = strBuffer;

				m_svRegistryvector.push_back(pItem);
				int id = m_svRegistryvector.size();			
				m_smRegistryTable.insert(smdef_XREGISTRY::value_type(mbcsBuffer, id-1));
			}
			break;		
		case REG_BINARY		:
			{
				_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;
				
				pItem->pbinaryitem = new BYTE[Size];						
				pItem->binarysize = Size;
									
				memcpy( pItem->pbinaryitem, Result, Size );

				pItem->registrykeyname = mbcsBuffer;			
				m_svRegistryvector.push_back(pItem);
				int id = m_svRegistryvector.size();			
				m_smRegistryTable.insert(smdef_XREGISTRY::value_type(mbcsBuffer, id-1));
			}
			break;
		case REG_DWORD		:
			{
				_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;
				memcpy( &pItem->integeritem, Result, sizeof(DWORD) );
				pItem->registrykeyname = mbcsBuffer;
				m_svRegistryvector.push_back(pItem);
				int id = m_svRegistryvector.size();			
				m_smRegistryTable.insert(smdef_XREGISTRY::value_type(mbcsBuffer, id-1));
			}
			break;
		default				:
			break;
		}

		i++;
    }

	RegCloseKey(subKey);

	SaveRegistry();

	return TRUE;
}

_XRegistryManager::~_XRegistryManager()
{
	Reset();
}

void _XRegistryManager::Reset( void )
{
	if( m_smRegistryTable.size() > 0 )
	{
		m_smRegistryTable.clear();
	}
	
	if( m_svRegistryvector.size() > 0 )
	{
		std::for_each(m_svRegistryvector.begin(), m_svRegistryvector.end(), DELETEREGOBJECT());
		m_svRegistryvector.clear();
	}
}

int _XRegistryManager::FindRegistryKey( const char* lpValue )
{
	if( m_smRegistryTable.size() <= 0 )
	{
		return -1;
	}

	smdef_XREGISTRY::iterator it = m_smRegistryTable.find(lpValue);
	if ( it != m_smRegistryTable.end() )
	{
		return (*it).second;
	}
	return -1;
}


UINT _XRegistryManager::_XRegReadInt(HKEY hKey, const char* lpKey, const char* lpValue, INT nDefault, BOOL bDefaultSet )
{
	if( !lpValue || strlen(lpValue) <= 0 )
		return nDefault;

	int regkeyindex = FindRegistryKey( lpValue );
	if( regkeyindex > -1 )
	{	
		if( regkeyindex < m_svRegistryvector.size() )
		{
			_XREGISTRY_ITEM* pItem = m_svRegistryvector[regkeyindex];
			
			return pItem->integeritem;
		}
		else
		{
			return nDefault;
		}
	}
	else
	{
		if( bDefaultSet )
		{
			_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;
			pItem->integeritem = nDefault;
			pItem->registrykeyname = lpValue;

			m_svRegistryvector.push_back(pItem);
			int id = m_svRegistryvector.size();

			m_smRegistryTable.insert(smdef_XREGISTRY::value_type(lpValue, id-1));
			SaveRegistry();
		}
		
		return nDefault;
	}
}

BOOL _XRegistryManager::_XRegReadString( HKEY hKey, const char* lpKey, const char* lpValue, const TCHAR* lpDefault,TCHAR* lpRet, DWORD nSize, BOOL bDefaultSet )
{
	char mbcsBuffer1[_REGSTRING_MAXLENGTH+1];
	char mbcsBuffer2[_REGSTRING_MAXLENGTH+1];
	memset( mbcsBuffer1, 0, sizeof(char) * (_REGSTRING_MAXLENGTH+1) );
	memset( mbcsBuffer2, 0, sizeof(char) * (_REGSTRING_MAXLENGTH+1) );

	if( wcslen( lpDefault ) > 0 )
	{
		WideCharToMultiByte(CP_ACP, 0, lpDefault, -1, mbcsBuffer1, _REGSTRING_MAXLENGTH, NULL, NULL);	
	}

	BOOL retcode = _XRegReadString( hKey, lpKey, lpValue, mbcsBuffer1, mbcsBuffer2, nSize, bDefaultSet );	
	
	memset( lpRet, 0, nSize );
	MultiByteToWideChar(CP_ACP, 0, mbcsBuffer2, strlen(mbcsBuffer2), lpRet, nSize);

	return retcode;
}



BOOL _XRegistryManager::_XRegReadString( HKEY hKey, const char* lpKey, const char* lpValue, const char* lpDefault,char* lpRet, DWORD nSize, BOOL bDefaultSet )
{
	if( !lpValue || strlen(lpValue) <= 0  )
		return FALSE;

	int regkeyindex = FindRegistryKey( lpValue );
	if( regkeyindex > -1 )
	{	
		if( regkeyindex < m_svRegistryvector.size() )
		{
			_XREGISTRY_ITEM* pItem = m_svRegistryvector[regkeyindex];

			if( pItem->stringitem.size() >= nSize )
			{
				memset( lpRet, 0, sizeof(char) * nSize );
				strncpy( lpRet, pItem->stringitem.c_str(), nSize );
			}
			else
			{
				memset( lpRet, 0, sizeof(char) * nSize );
				strcpy( lpRet, pItem->stringitem.c_str() );
			}
			return TRUE;
		}
		else
		{
			memset( lpRet, 0, sizeof(char) * nSize );
			if( lpDefault && strlen(lpDefault) )
			{
				strncpy( lpRet,lpDefault,_REGSTRING_MAXLENGTH );
			}
			return FALSE;
		}
	}
	else
	{
		if( bDefaultSet )
		{
			_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;
			if( lpDefault && strlen(lpDefault) > 0 )
			{
				pItem->stringitem = lpDefault;
			}
			else
			{
				pItem->stringitem = "";
			}			

			memset( lpRet, 0, sizeof(char) * nSize );
			if( lpDefault && strlen(lpDefault) )
			{
				strncpy( lpRet,lpDefault,_REGSTRING_MAXLENGTH );
			}
			
			pItem->registrykeyname = lpValue;
			
			m_svRegistryvector.push_back(pItem);
			int id = m_svRegistryvector.size();
			m_smRegistryTable.insert(smdef_XREGISTRY::value_type(lpValue, id-1));

			SaveRegistry();
		}
		else
		{
			//memset( lpRet, 0, sizeof(char) * nSize );
		}
		
		return TRUE;
	}

	return TRUE;
}

BOOL _XRegistryManager::_XRegWriteInt(HKEY hKey, const char* lpKey, const char* lpValue, UINT nData)
{
	if( !lpValue || strlen(lpValue) <= 0  )
		return FALSE;
	
	int regkeyindex = FindRegistryKey( lpValue );
	if( regkeyindex > -1 )
	{	
		if( regkeyindex < m_svRegistryvector.size() )
		{
			_XREGISTRY_ITEM* pItem = m_svRegistryvector[regkeyindex];			
			pItem->integeritem = nData;
			SaveRegistry();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;
		pItem->integeritem = nData;
		pItem->registrykeyname = lpValue;
		
		m_svRegistryvector.push_back(pItem);
		int id = m_svRegistryvector.size();
		
		m_smRegistryTable.insert(smdef_XREGISTRY::value_type(lpValue, id-1));
		SaveRegistry();
		
		return TRUE;
	}
}

BOOL _XRegistryManager::_XRegWriteString(HKEY hKey, const char* lpKey, const char* lpValue, const TCHAR* lpData)
{
	char mbcsBuffer1[_REGSTRING_MAXLENGTH+1];	
	memset( mbcsBuffer1, 0, sizeof(char) * _REGSTRING_MAXLENGTH+1 );	
	
	WideCharToMultiByte(CP_ACP, 0, lpData, -1, mbcsBuffer1, _REGSTRING_MAXLENGTH, NULL, NULL);	
	return _XRegWriteString( hKey, lpKey, lpValue, mbcsBuffer1 );
}

BOOL _XRegistryManager::_XRegWriteString(HKEY hKey, const char* lpKey, const char* lpValue, const char* lpData)
{
	if( !lpValue || strlen(lpValue) <= 0  )
		return FALSE;
	
	int regkeyindex = FindRegistryKey( lpValue );
	if( regkeyindex > -1 )
	{	
		if( regkeyindex < m_svRegistryvector.size() )
		{
			_XREGISTRY_ITEM* pItem = m_svRegistryvector[regkeyindex];
			pItem->stringitem = lpData;
			SaveRegistry();

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;
		pItem->stringitem = lpData;
		pItem->registrykeyname = lpValue;
		
		m_svRegistryvector.push_back(pItem);
		int id = m_svRegistryvector.size();
		
		m_smRegistryTable.insert(smdef_XREGISTRY::value_type(lpValue, id-1));
		SaveRegistry();
		
		return TRUE;
	}
}

BOOL _XRegistryManager::_XRegDeleteNode(HKEY hKey, const char* lpKey, const char* lpValue )
{
    if( !lpValue || strlen(lpValue) <= 0  )
		return FALSE;

	int regkeyindex = FindRegistryKey( lpValue );
	if( regkeyindex > -1 )
	{
		smdef_XREGISTRY::iterator it = m_smRegistryTable.find(lpValue);
		if ( it != m_smRegistryTable.end() )
		{
			m_smRegistryTable.erase( it );
		}

		m_svRegistryvector.erase( &m_svRegistryvector[regkeyindex] );

		SaveRegistry();
		return TRUE;
	}
	
	return FALSE;
}

BOOL _XRegistryManager::_XRegReadBinary(HKEY hKey, const char* lpKey, const char* lpValue, LPBYTE lpDefault,LPBYTE lpRet, DWORD nSize, BOOL bDefaultSet )
{
	if( !lpValue || strlen(lpValue) <= 0  || nSize <= 0 )
		return FALSE;
	
	int regkeyindex = FindRegistryKey( lpValue );
	if( regkeyindex > -1 )
	{	
		if( regkeyindex < m_svRegistryvector.size() )
		{
			_XREGISTRY_ITEM* pItem = m_svRegistryvector[regkeyindex];

			if( pItem->binarysize != nSize )
			{
				//_XFatalError( _T("Registry size[%d:%d]"), pItem->binarysize, nSize );
				return FALSE;
			}

			memcpy( lpRet, pItem->pbinaryitem, nSize );
			pItem->binarysize = nSize;

			SaveRegistry();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if( bDefaultSet )
		{
			_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;
			
			pItem->pbinaryitem = new BYTE[nSize];
			pItem->binarysize = nSize;

			memcpy( pItem->pbinaryitem, lpDefault, nSize );
			memcpy( lpRet, pItem->pbinaryitem, nSize );

			pItem->registrykeyname = lpValue;
			
			m_svRegistryvector.push_back(pItem);
			int id = m_svRegistryvector.size();			
			m_smRegistryTable.insert(smdef_XREGISTRY::value_type(lpValue, id-1));

			SaveRegistry();
		}
		
		return TRUE;
	}

	return FALSE;
}

BOOL _XRegistryManager::_XRegWriteBinary(HKEY hKey, const char* lpKey, const char* lpValue, LPBYTE lpData, DWORD nSize )
{
	if( !lpValue || strlen(lpValue) <= 0  || nSize <= 0 )
		return FALSE;
	
	int regkeyindex = FindRegistryKey( lpValue );
	if( regkeyindex > -1 )
	{	
		if( regkeyindex < m_svRegistryvector.size() )
		{
			_XREGISTRY_ITEM* pItem = m_svRegistryvector[regkeyindex];

			if( pItem->binarysize != nSize )
			{
				if( pItem->pbinaryitem )
				{
					delete[] pItem->pbinaryitem;
					pItem->pbinaryitem = new BYTE[nSize];
				}

				pItem->binarysize = nSize;
			}

			memcpy( pItem->pbinaryitem, lpData, nSize );

			SaveRegistry();

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		_XREGISTRY_ITEM* pItem = new _XREGISTRY_ITEM;
		
		pItem->pbinaryitem = new BYTE[nSize];
		pItem->binarysize = nSize;

		memcpy( pItem->pbinaryitem, lpData, nSize );

		pItem->registrykeyname = lpValue;
		
		m_svRegistryvector.push_back(pItem);
		int id = m_svRegistryvector.size();			
		m_smRegistryTable.insert(smdef_XREGISTRY::value_type(lpValue, id-1));
		
		SaveRegistry();

		return TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
// real access methods to registry 

//////////////////////////////////////////////////////////////////////////
// real access methods to registry 

UINT _XRegReadInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault, BOOL bDefaultSet )
{
	HKEY key;
	DWORD dwDisp;
	UINT Result;
	DWORD Size;
	if(RegCreateKeyEx( hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_QUERY_VALUE, NULL, &key, &dwDisp ) != ERROR_SUCCESS)
		return 0;
	Size = sizeof(LONG);
	if(RegQueryValueEx( key,lpValue,0,NULL,(LPBYTE)&Result,&Size) != ERROR_SUCCESS)
	{
		Result = nDefault;
		if(bDefaultSet) RegSetValueEx( key, lpValue, NULL, REG_DWORD, (LPBYTE)&nDefault, sizeof(UINT) );
	}
	RegCloseKey(key);
	return Result;
}

BOOL _XRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault,LPTSTR lpRet, DWORD nSize, BOOL bDefaultSet )
{
	HKEY key;
	DWORD dwDisp;
	DWORD dwType;
	DWORD Size;
	if(RegCreateKeyEx(hKey,lpKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE|KEY_QUERY_VALUE ,NULL,&key,&dwDisp) != ERROR_SUCCESS)
		return FALSE;
	
	TCHAR data[_REGSTRING_MAXLENGTH];
	Size = sizeof(TCHAR) * _REGSTRING_MAXLENGTH;
    
	if(RegQueryValueEx(key,lpValue, NULL, &dwType,(LPBYTE)data,&Size) != ERROR_SUCCESS)
	{
		if( lpDefault )
		{
			memset( lpRet, 0, nSize );
			if( wcslen(lpDefault) ) wcsncpy( lpRet,lpDefault,_REGSTRING_MAXLENGTH );
			if(bDefaultSet) RegSetValueEx( key, lpValue, NULL, REG_SZ, (CONST BYTE *)lpDefault, sizeof(TCHAR) * wcslen(lpDefault) );
		}		
		return FALSE;
	}
	else
	{		
		memset( lpRet, 0, _REGSTRING_MAXLENGTH );
		wcsncpy( lpRet, data, _REGSTRING_MAXLENGTH );
	}
	
	RegCloseKey(key);
	return TRUE;
}

BOOL _XRegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, UINT nData)
{
	HKEY key;
	DWORD dwDisp;
	if(RegCreateKeyEx(hKey,lpKey,0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	if(RegSetValueEx(key,lpValue,0,REG_DWORD,(LPBYTE)&nData,sizeof(UINT)) != ERROR_SUCCESS)
		return FALSE;
	RegCloseKey(key);
	return TRUE;
}

BOOL _XRegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	HKEY key;
	DWORD dwDisp;
	if(RegCreateKeyEx(hKey,lpKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&dwDisp) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	if(RegSetValueEx(key,lpValue,0,REG_SZ,(LPBYTE)lpData,sizeof(TCHAR) * (wcslen(lpData)+1)) != ERROR_SUCCESS)
		return FALSE;
	
	RegCloseKey(key);
	return TRUE;
}

BOOL _XRegDeleteNode(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue )
{
    HKEY key;
    if(RegOpenKeyEx(hKey, lpKey, 0, KEY_ALL_ACCESS, &key) != ERROR_SUCCESS) return FALSE;
	
    LONG lResult = RegDeleteValue(key, lpValue);
    RegCloseKey(key);
	
    if (lResult == ERROR_SUCCESS) return TRUE;
    else return FALSE;
}



BOOL _XRegReadBinary(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPBYTE lpDefault,LPBYTE lpRet, DWORD nSize, BOOL bDefaultSet )
{
	HKEY key;
	DWORD dwDisp;
	DWORD dwType;
	DWORD Size;
	if(RegCreateKeyEx(hKey,lpKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE|KEY_QUERY_VALUE ,NULL,&key,&dwDisp) != ERROR_SUCCESS)
		return FALSE;
	
	BYTE data[1024];
	Size = sizeof(data);	
    
	if(RegQueryValueEx(key,lpValue, NULL, &dwType,(LPBYTE)data,&Size) != ERROR_SUCCESS)
	{
		if( lpDefault )
		{
			if( lpDefault != lpRet )
			{
				memset( lpRet, 0, nSize );
				if( lpDefault ) memcpy( lpRet,lpDefault,nSize);
			}
			
			if( bDefaultSet) RegSetValueEx( key, lpValue, NULL, REG_BINARY, (CONST BYTE *)lpDefault, nSize );
		}		
		if( bDefaultSet) 
			return TRUE;
		else
			return FALSE;
	}
	else
	{		
		memset( lpRet, 0, nSize );
		memcpy( lpRet, data, nSize );
	}
	
	RegCloseKey(key);
	return TRUE;
}

BOOL _XRegWriteBinary(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPBYTE lpData, DWORD nSize )
{
	HKEY key;
	DWORD dwDisp;
	if(RegCreateKeyEx(hKey,lpKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&dwDisp) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	if(RegSetValueEx(key,lpValue,0,REG_BINARY,(LPBYTE)lpData,nSize) != ERROR_SUCCESS)
		return FALSE;
	
	RegCloseKey(key);
	return TRUE;
}