#ifndef _XUNICODE_CONVERTOR
#define _XUNICODE_CONVERTOR

#define TOTAL_VNCHARS 213
#define TOTAL_ALPHA_VNCHARS 186

enum _XVnConvError 
{
	_XVNCONV_NO_ERROR = 0,
	_XVNCONV_UNKNOWN_ERROR,
	_XVNCONV_INVALID_CHARSET,
	_XVNCONV_OUT_OF_MEMORY,
	_XVNCONV_ERR_WRITING,
	_XVNCONV_LAST_ERROR
};

#define CONV_NONE			0
#define CONV_UPPERCASE		1
#define CONV_LOWERCASE		2
#define CONV_STRIPACCENT	4

class _XStringBIStream
{
protected:
	int m_eos;
	BYTE *m_data, *m_current;
	int m_len, m_left;
	
	public:
		_XStringBIStream(BYTE *data, int len);
		virtual int getNext(BYTE &b);
		virtual int peekNext(BYTE &b);
		virtual int unget(BYTE b);
		
		virtual int getNextW(WORD &w);
		virtual int peekNextW(WORD &w);
		virtual int eos();
		virtual int close();
		
		void reopen();
		int left() 
		{
			return m_left;
		}
};


class _XStringBOStream
{
protected:
	BYTE *m_buf, *m_current;
	int m_out;
	int m_len;
	int m_bad;
public:
	_XStringBOStream(BYTE *buf, int len);
	virtual int putB(BYTE b);
	virtual int putW(WORD w);
	virtual int puts(const char *s, int size = -1);
	virtual int isOK();
	
	virtual int close()	
	{
		return 1;
	};
	
	void reopen();
	int getOutBytes()
	{
		return m_out;
	}
};


class _XWinCP1258Charset
{
protected:
	WORD	m_stdMap[256];
	DWORD	m_vnChars[TOTAL_VNCHARS*2];
	WORD   *m_toDoubleChar;
	int		m_totalChars;
	
public:
	_XWinCP1258Charset();
	virtual int nextInput(_XStringBIStream & is, DWORD & stdChar, int & bytesRead);
	virtual int putChar(_XStringBOStream & os, DWORD stdChar, int & outLen);
};


class _XUnicodeCharset 
{
protected:
	DWORD m_vnChars[TOTAL_VNCHARS];
	WORD * m_toUnicode;
public:
	_XUnicodeCharset();
	virtual int nextInput(_XStringBIStream & is, DWORD & stdChar, int & bytesRead);
	virtual int putChar(_XStringBOStream & os, DWORD stdChar, int & outLen);
};


struct _XUniCompCharInfo
{
	DWORD	compChar;
	int		stdIndex;
};

class _XUnicodeCompCharset 
{
protected:
	_XUniCompCharInfo	m_info[TOTAL_VNCHARS*2];
	DWORD				*m_uniCompChars;
	int					m_totalChars;

public:
	_XUnicodeCompCharset();
	virtual int nextInput(_XStringBIStream & is, DWORD & stdChar, int & bytesRead);
	virtual int putChar(_XStringBOStream & os, DWORD stdChar, int & outLen);
};



int _XCP1258ToUniCode(BYTE *input, BYTE *output, int & inLen, int & maxOutLen, int option = CONV_NONE );
int _XUniCodeToCP1258(BYTE *input, BYTE *output, int & inLen, int & maxOutLen, int option = CONV_NONE );
int _XUniCodeCompToCP1258(BYTE *input, BYTE *output, int & inLen, int & maxOutLen, int option = CONV_NONE );
int _XCP1258Conv(BYTE *input, BYTE *output, int & inLen, int & maxOutLen, int option = CONV_NONE );

#endif