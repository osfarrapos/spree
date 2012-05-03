
//NANGReg.cpp
#include "stdafx.h"

#include "NANGReg.h"

#include "RegStringDefine.h"

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
	memset( data, 0, sizeof(TCHAR) * _REGSTRING_MAXLENGTH );
	Size = sizeof(TCHAR) * _REGSTRING_MAXLENGTH;
    
	if(RegQueryValueEx(key,lpValue, NULL, &dwType,(LPBYTE)data,&Size) != ERROR_SUCCESS)
	{
		if( lpDefault )
		{
			strncpy( lpRet,lpDefault,_REGSTRING_MAXLENGTH );
			if(bDefaultSet) RegSetValueEx( key, lpValue, NULL, REG_SZ, (CONST BYTE *)lpDefault, strlen(lpDefault) );
		}		
		return FALSE;
	}
	else
	{		
		memset( lpRet, 0, _REGSTRING_MAXLENGTH );
		if( Size > 0 )
		{
			strncpy( lpRet, data, _REGSTRING_MAXLENGTH );
		}
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
	if(RegSetValueEx(key,lpValue,0,REG_SZ,(LPBYTE)lpData,strlen(lpData)+1) != ERROR_SUCCESS)
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
		return TRUE;
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