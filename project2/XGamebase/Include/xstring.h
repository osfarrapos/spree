#ifndef __X_XSTRING_H__
#define __X_XSTRING_H__
#ifdef  __cplusplus
 
class XString
{

public:
	XString()
	{
		mp_str = new TCHAR[1];
		mp_str[0] = '\0';
		mp_nLen = 0;
	}
	
	XString(const TCHAR* str)
	{
		if(!str)
		{
			mp_str = new TCHAR[1];
			mp_str[0] = '\0';
			mp_nLen = 0;
		}
		else
		{
			mp_nLen = strlen(str);
			mp_str = new TCHAR[mp_nLen+1];
			strcpy(mp_str, str);
			mp_str[mp_nLen] = '\0';
		}
	}
	
	XString(const XString &str)
	{
		mp_nLen = str.Len();
		mp_str = new TCHAR[mp_nLen+1];
		strcpy(mp_str, (const TCHAR*)str);
		mp_str[mp_nLen] = '\0';
	}
	
	XString(_XVFile *rwops)
	{
		mp_str = NULL;
		Read(rwops);
	}

	XString(U16 p_nLen)
	{
		mp_str = new TCHAR[p_nLen+1];
		memset(mp_str, 0, sizeof(TCHAR)*(p_nLen+1));
		mp_nLen = p_nLen;
	}	
	
	~XString()
	{
		delete [] mp_str;
		mp_nLen = 0;
	}
	
	
	// overloaded operators
	TCHAR & operator [] (U16 offset)
	{
		if(offset >= mp_nLen)
			return mp_str[mp_nLen-1];
		else
			return mp_str[offset];
	}
	
	TCHAR operator [] (U16 offset) const
	{
		if(offset >= mp_nLen)
			return mp_str[mp_nLen-1];
		else
			return mp_str[offset];
	}
	
	XString operator + (const XString& str)
	{
		U16 nFullLen = mp_nLen + str.Len();
		XString temp(nFullLen);
		U16 i,j;
		
		for(i = 0; i < mp_nLen; i++)
			temp[i] = mp_str[i];
		
		for(j = 0; j < str.Len(); j++,i++)
		{
			TCHAR c = str[j];
			temp[i] = str[j];
		}
		
		return temp;
	}
	
	void operator += (const XString& str)
	{
		*this = *this + str;
	}
	
	XString& operator = (const XString& strIn)
	{
		if(this == &strIn)
			return *this;
		
		if(mp_str) delete [] mp_str;
		
		mp_nLen = strIn.Len();
		mp_str = new TCHAR[mp_nLen+1];
		strcpy(mp_str, strIn);
		
		return *this;
	}

	// casting operators
	inline operator const TCHAR* () const { return (const TCHAR*)mp_str; }
	inline operator TCHAR* () { return (TCHAR*)mp_str; }
	
	// misc acces method
	inline void CheckLen() { mp_nLen = strlen(mp_str); }
	inline U16 Len() const { return mp_nLen; }

	int	   Compare( LPSTR string ){ return strcmpi( mp_str, string ); }
	
	// files
	void Write(_XVFile *fp)
	{
		_XVFPutLE16(fp, mp_nLen);
		_XVFWrite(fp, mp_str, mp_nLen);
	}
	
	void Read(_XVFile *fp)
	{
		if(mp_str) delete [] mp_str;
		mp_nLen = _XVFGetLE16(fp);
		mp_str = new TCHAR[mp_nLen+1];
		_XVFRead(fp, mp_str, mp_nLen);
		mp_str[mp_nLen] = '\0';
	}

	// return the filename of a path
	XString GetPathFile()
	{
		return XString(_XGetFilename(mp_str));
	}

	// return the filename extension of a path
	XString GetPathExtension()
	{
		return XString(_XPutExtension(mp_str));
	}

	// return the directory of a path
	XString GetPathDir()
	{
		XString tmp(mp_nLen);
		strncpy(tmp,mp_str,mp_nLen-strlen(_XGetFilename(mp_str)));
		tmp.CheckLen();
		return XString(tmp);
	}

private:
	U16  mp_nLen;
	TCHAR *mp_str;
	
};

#endif // __cplusplus
#endif // __X_STRING_H__
