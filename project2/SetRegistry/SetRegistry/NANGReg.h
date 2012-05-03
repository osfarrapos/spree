//NANGReg.h
#ifndef _NANGREG_H_
#define _NANGREG_H_

#include <windows.h>

#define SHLM					HKEY_LOCAL_MACHINE
#define SHCU					HKEY_CURRENT_USER
#define SHCR					HKEY_CLASSES_ROOT

#define _REGSTRING_MAXLENGTH	256
							
UINT _XRegReadInt(HKEY hKey,LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault, BOOL bDefaultSet = FALSE );
BOOL _XRegReadString(HKEY hKey, LPCTSTR lpKey,LPCTSTR lpValue, LPCTSTR lpDefault,LPTSTR lpRet, DWORD nSize, BOOL bDefaultSet = FALSE );
BOOL _XRegWriteInt(HKEY hKey,LPCTSTR lpKey, LPCTSTR lpValue, UINT nData);
BOOL _XRegWriteString(HKEY hKey,LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
BOOL _XRegDeleteNode(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue );
BOOL _XRegReadBinary(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPBYTE lpDefault,LPBYTE lpRet, DWORD nSize, BOOL bDefaultSet = FALSE );
BOOL _XRegWriteBinary(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPBYTE lpData, DWORD nSize );

#endif