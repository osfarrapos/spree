
#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#ifdef _ACCLAIM_IGAADSYSTEM

#pragma warning( disable : 4786 )
#include <shlwapi.h>
#include <vector>
using namespace std;
typedef vector<int> VECINT;

class Registry
{
public:
	Registry(HKEY hKeyRoot, const char* szKeyName) :
	  m_hRegKey(0),
	  m_hRootKey(0)
	{
		m_szKeyName[0]=0;
		if (szKeyName)
			setKeyName(hKeyRoot, szKeyName);
	}
	~Registry()
	{
		close();
	}

	// Open/close
	int		setKeyName(HKEY hKeyRoot, const char* szKeyName) // return 1 if key existed, 0 if created
	{
		close();
		m_hRootKey = hKeyRoot;
		strncpy(m_szKeyName, szKeyName, 512);
		m_szKeyName[511] = 0;
		DWORD dwDisp = 0;
		if (RegCreateKeyEx(hKeyRoot,szKeyName,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,&m_hRegKey,&dwDisp) != ERROR_SUCCESS)
			m_hRegKey = NULL;
		if (dwDisp == REG_CREATED_NEW_KEY)
			return 0; // key didn't previously exist, was created
		return 1; // key existed
	}
	BOOL isOpen() {return (m_hRegKey == NULL)?0:1;}

	const char*	getKeyName()
	{
		return m_szKeyName;
	}
	void	close()
	{
		m_szKeyName[0] = 0;
		if (m_hRegKey)
			RegCloseKey(m_hRegKey);
		m_hRegKey = 0;
		m_hRootKey=0;
	}

	// Edit
	void	deleteKey(HKEY hKeyRoot, const char* szKeyName)
	{
		HKEY rk = m_hRootKey;
		char kn[512];
		memcpy(kn, m_szKeyName, 512);
		close();
		SHDeleteKey(rk,kn);
	}
	int		deleteValue(const char* szValueName)
	{
		if (m_hRegKey)
			return (RegDeleteValue(m_hRegKey, szValueName) == ERROR_SUCCESS)?1:0;
		return 0;
	}

	// Free memory handle
	void	free(void* pMem)
	{
		delete[] (unsigned char*)pMem;
	}

	// Get BLOB from registry
	void*	getBinary(const char* szName, int& rLen, void* pDefault=0)
	{
		if (!m_hRegKey) 	return 0;
		DWORD size = 0;
		BYTE buf[1];
		if (RegQueryValueEx(m_hRegKey,szName,0,0,buf,&size) == ERROR_MORE_DATA)
		{
			if (size<1)	return 0;
			unsigned char *p = 0;
			if (size < 4)
			{
				size = 4;
				p = new unsigned char[size];
				p[0] = p[1] = p[2] = p[3] = 0;
			} else
				p = new unsigned char[size];
			if (RegQueryValueEx(m_hRegKey,szName,0,0,(BYTE*)p,&size) != ERROR_SUCCESS)
			{
				delete[] p;
				return 0;
			}
			rLen = size;
			return p;
		} else
		if (pDefault)
		{
			setBinary(szName,pDefault,rLen);
			void* pRet = new unsigned char[rLen];
			memcpy(pRet,pDefault,rLen);
			return pRet;
		}
		return 0;
	}
	// Set BLOB to registry
	void	setBinary(const char* szName, void* pData, int iDataLen)
	{
		if (!m_hRegKey)	return;
		if (!pData || !iDataLen) return;
		RegSetValueEx(m_hRegKey,szName,0,REG_BINARY,(BYTE*)pData,iDataLen);
	}

	// Get/Set int
	int		getInt(const char* szName, int iDefault)
	{
		int len = 0;
		int* retval = (int*)getBinary(szName,len);
		if (!retval)
		{
			setInt(szName,iDefault);
			return iDefault;
		}
		int ret = *retval;
		delete[] (unsigned char*)retval;
		return ret;
	}
	void	setInt(const char* szName, int iData)
	{
		if (!m_hRegKey || !szName)	return;
		RegSetValueEx(m_hRegKey,szName,0,REG_DWORD,(BYTE*)&iData,4);
	}

	// Get/Set string
	int	getStringDef(const char* szName, char* szString, int iBufferLen, const char* szDefault)
	{
		int len = 0;
		char* val = (char*)getBinary(szName,len);
		if (!val)
		{
			setString(szName,szDefault);
			return getString(szName, szString, iBufferLen);
		}
		delete[] val;
		return getString(szName, szString, iBufferLen);
	}
	int	getString(const char* szName, char* szString, int iBufferLen)
	{
		int len = 0;
		char* val = (char*)getBinary(szName,len);
		if (!val)
			return 0;
		if (szString && (iBufferLen>0))
		{
			strncpy(szString, val, iBufferLen);
			szString[iBufferLen-1] = 0;
		}
		delete[] val;
		return len;
	}
	void	setString(const char* szName, const char* szStr)
	{
		if (!m_hRegKey || !szName)	return;
		if (szStr)
			RegSetValueEx(m_hRegKey,szName,0,REG_SZ,(BYTE*)szStr,(DWORD)(strlen(szStr)+1));
		else
			RegSetValueEx(m_hRegKey,szName,0,REG_SZ,(BYTE*)"",1);
	}

	// Get/Set array
	int			getVectorInt(const char* szName, VECINT* pVector)
	{
		int len = 0;
		int* data = (int*)getBinary(szName,len);
		if (!data || !len)
			return 0;

		int num = len / sizeof(int);
		if (pVector)
		{
			pVector->resize(num);
			for (int i=0;i<num;i++)
				(*pVector)[i] = data[i];
		}
		delete[] (unsigned char*)data;
		return num;
	}
	void	setVectorInt(const char* szName, VECINT& vec)
	{
		if (vec.size()<1)
			return;
		setBinary(szName,&vec[0],(int)(vec.size() * sizeof(int)));
	}

	struct VSNode
	{
		VSNode(char* sp) {p=sp;next=0;};
		~VSNode() {if (next) delete next;};
		int add(char* sp) {if (next) return next->add(sp)+1; else {next = new VSNode(sp);return 2;}};
		char* p;
		VSNode* next;
	};

	int		getVectorString(const char* szName, char*** pValue)
	{
		int len = 0;
		char* sp = (char*)getBinary(szName,len);
		if (!sp || !len)
			return 0;

		VSNode* head = 0;
		int cnt;
		char* cur = sp;
		while(*cur)
		{
			if (!head)
			{
				head = new VSNode(cur);
				cnt=1;
			}
			else
				cnt = head->add(cur);
			while(*cur) cur++;
			cur++;
		}
		if (pValue)
		{
			*pValue = new char*[cnt];
			VSNode* node = head;
			for (int i=0;i<cnt;i++)
			{
				(*pValue)[i] = new char[strlen(node->p)+1];
				strcpy((*pValue)[i], node->p);
				node = node->next;
			}
		}
		delete head;
		delete[] (unsigned char*)sp;
		return cnt;
	}

	void	setVectorString(const char* szName, char** pValues, int iNumValues)
	{
		int i, tlen = 0;
		if (!m_hRegKey || !szName || !pValues || (iNumValues<1))	return;
		for (i=0;i<iNumValues;i++)
			tlen += (int)(strlen(pValues[i])+1);
		char *buf = new char[tlen+1];
		char *ptr = buf;
		for (i=0;i<iNumValues;i++)
		{
			int stlen = (int)(strlen(pValues[i])+1);
			memcpy(buf,pValues[i],stlen);
			buf += stlen;
		}
		buf[0] = 0;
		RegSetValueEx(m_hRegKey,szName,0,REG_MULTI_SZ,(const BYTE*)ptr,tlen+1);
	}

protected:
	HKEY	m_hRegKey;
	HKEY	m_hRootKey;
	char	m_szKeyName[512];
};

#endif //_ACCLAIM_IGAADSYSTEM

#endif