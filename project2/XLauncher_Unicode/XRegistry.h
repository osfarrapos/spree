//XRegistry.h
#ifndef _XREGISTRY_H_
#define _XREGISTRY_H_

#include <windows.h>
#include "XRegistryDef.h"

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

#pragma warning(disable:4786)
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

struct DELETEREGOBJECT
{
	template <typename T>
		void operator()(T* ptr) const 
	{
		delete ptr;
	}
};

typedef struct _XREGISTRY_ITEM
{
	std::string	registrykeyname;
	
	DWORD		integeritem;
	std::string	stringitem;
	LPBYTE		pbinaryitem;
	UINT		binarysize;
	
	_XREGISTRY_ITEM()
	{
		registrykeyname.resize(0);
		
		integeritem		= 0;
		stringitem		= "";
		pbinaryitem		= NULL;
		binarysize		= 0;
	}
	~_XREGISTRY_ITEM()
	{
		stringitem.resize(0);
		if( pbinaryitem )
		{
			delete[] pbinaryitem;
		}
	}
	
	BOOL Load( FILE* pFile );
	BOOL Save( FILE* pFile );
	
}XREGISTRY_ITEM;

struct REGISTRYKEYNAME_COMPARE : public std::binary_function<std::string, std::string, bool>
{
	static bool nocase_compare ( char c1, char c2)
	{
		return toupper(c1) < toupper(c2);
	}
	bool operator () (const std::string& lhs, const std::string& rhs) const
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), nocase_compare);
	}
};

class _XRegistryManager
{
	
public:
	_XRegistryManager()
	{
		memset( m_RegistryPath, 0, sizeof(char) * _MAX_PATH );
	};
	~_XRegistryManager();
	
	UINT _XRegReadInt(    HKEY hKey, const char* lpKey, const char* lpValue, INT nDefault, BOOL bDefaultSet = FALSE );
	BOOL _XRegWriteInt(   HKEY hKey, const char* lpKey, const char* lpValue, UINT nData);

	BOOL _XRegReadString( HKEY hKey, const char* lpKey, const char* lpValue, const char* lpDefault,char* lpRet, DWORD nSize, BOOL bDefaultSet = FALSE );
	BOOL _XRegReadString( HKEY hKey, const char* lpKey, const char* lpValue, const TCHAR* lpDefault,TCHAR* lpRet, DWORD nSize, BOOL bDefaultSet = FALSE );	
	
	BOOL _XRegWriteString(HKEY hKey, const char* lpKey, const char* lpValue, const char* lpData);
	BOOL _XRegWriteString(HKEY hKey, const char* lpKey, const char* lpValue, const TCHAR* lpData);

	BOOL _XRegReadBinary( HKEY hKey, const char* lpKey, const char* lpValue, LPBYTE lpDefault,LPBYTE lpRet, DWORD nSize, BOOL bDefaultSet = FALSE );
	BOOL _XRegWriteBinary(HKEY hKey, const char* lpKey, const char* lpValue, LPBYTE lpData, DWORD nSize );

	BOOL _XRegDeleteNode( HKEY hKey, const char* lpKey, const char* lpValue );
	
	void Reset( void );
	
	void SetRegistryFolder( const char* szPath );
	void SetRegistryFolder( const TCHAR* szPath );
	BOOL LoadRegistry( void );
	BOOL SaveRegistry( void );
	BOOL ImportWindowsRegistry( void );
protected:
	typedef std::vector<_XREGISTRY_ITEM*>								svdef_XREGISTRY;
	typedef std::map<std::string, int, REGISTRYKEYNAME_COMPARE>			smdef_XREGISTRY;
	
	svdef_XREGISTRY			m_svRegistryvector;
	smdef_XREGISTRY			m_smRegistryTable;
	
	char					m_RegistryPath[_MAX_PATH];
	
private:
	int FindRegistryKey( const char* lpValue );
	
};

#endif