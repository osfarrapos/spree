#ifdef	_SMALL_NETWORK_

#ifndef	_PACKET_
#define	_PACKET_

#pragma pack(1)

enum PKT_CELL {
	CELL_0			= 0,
	CELL_256		= 256,
	CELL_1024		= 1024,
};

//////////////////////////////////////////////////////////////////////////
// _NEXON_NETWORK_
#if defined(_NEXON_NETWORK_)
class CPktHeader {
public:
	UCHAR	ucMark;		// 0xAA(고정값)
	WORD	wLen;		// without ucMark, wLen
	UCHAR	ucType;		// packet id

public:
	WORD	GetPktSize() { return wLen; }
	UCHAR	GetPktId() { return ucType; }
	void	SetMark() { ucMark = 0xAA; }
	void	SetPktSize(WORD wPktSize) {
		wPktSize -= (WORD)(sizeof(ucMark) + sizeof(wLen));
		wLen = ConvertByteOrder2(wPktSize);
	}
	void	SetPktId(WORD wPktId) { ucType = (UCHAR)wPktId; }
};
#define TCP_PH_SIZE		(sizeof(CPktHeader))

//////////////////////////////////////////////////////////////////////////
// else
#else
class CPktHeader {
public:
	WORD	GetPktSize() { return 0; }
	WORD	GetPktId() { return 0; }
	void	SetMark() { return; }
	void	SetPktSize(WORD wPktSize) { return; }
	void	SetPktId(WORD wPktId) { return; }
};
#define	TCP_PH_SIZE		0

#endif
#pragma pack()

class CPacket {
private:
	WSABUF		wsaBuf;
	int			iOffset;	// wsaBuf.buf's offset
	PKT_CELL	cell;		// 패킷 반환할 때, 패킷 타입 파악을 위해

public:
	CPacket() {
		wsaBuf.buf = NULL;
	}
	~CPacket() {
		try {
			Destroy();
		}
		catch(...) {
			Logout(_T("#ERR Occur an exception while running ~CPacket()"));
		}
	}
	BOOL	Create(PKT_CELL cell) {
		if(cell == CELL_0) {
			wsaBuf.buf = NULL;
		}
		else {
			wsaBuf.buf = new char[cell]; assert(wsaBuf.buf);
		}
		wsaBuf.len = 0;
		iOffset = TCP_PH_SIZE;
		this->cell = cell;
		return TRUE;
	}
	void		Destroy() { SAFE_DEL_ARRAY(wsaBuf.buf); }
	void		Init() { iOffset = TCP_PH_SIZE; }
	
	PKT_CELL	GetCell() { return cell; }
	char*		GetBuf() { return wsaBuf.buf; }
	char*		GetContents() { return wsaBuf.buf + TCP_PH_SIZE; }
	int			GetBufOffset() { return iOffset; }
	void		AddBufOffset(int iOffset) { this->iOffset += iOffset; }
	CPktHeader*	GetHeader() { return (CPktHeader*)(wsaBuf.buf); }

public:
//////////////////////////////////////////////////////////////////////////
// _NEXON_NETWORK_
#if defined(_NEXON_NETWORK_)
	CPacket& operator << (char* pszStr) {
		char*	p = wsaBuf.buf + iOffset;
		UCHAR	ucStrLen = strlen(pszStr);
		p[0] = ucStrLen;
		memcpy(p+1, pszStr, (size_t)ucStrLen);
		iOffset += ucStrLen+1;
		return *this;
	}

	CPacket& operator >> (char* pszStr) {
		char*	p = wsaBuf.buf + iOffset;
		UCHAR	ucStrLen = p[0];
		memcpy(pszStr, p+1, (size_t)ucStrLen);
		pszStr[ucStrLen] = NULL;
		iOffset += ucStrLen+1;
		return *this;
	}

	template <class T> CPacket& operator << (T& t) {
		char*	p = wsaBuf.buf + iOffset;
		int		iSize = sizeof(t);
		switch(iSize) {
		case 1:
			p[0] = (char)t;
			break;
		case 2:
			p[0] = (char)((t & 0xFF00) >> 8);
			p[1] = (char)(t & 0x00FF);
			break;
		case 4:
			p[0] = (char)((t & 0xFF000000) >> 24);
			p[1] = (char)((t & 0x00FF0000) >> 16);
			p[2] = (char)((t & 0x0000FF00) >> 8);
			p[3] = (char)(t & 0x000000FF);
			break;
		}
		iOffset += iSize;
		return *this;
	}

	template <class T> CPacket& operator >> (T& t) {
		char*	buf = wsaBuf.buf + iOffset;
		char*	p = (char*)&t;
		int		iSize = sizeof(t);
		switch(iSize) {
		case 1:
			p[0] = buf[0];
			break;
		case 2:
			p[0] = buf[1];
			p[1] = buf[0];
			break;
		case 4:
			p[0] = buf[3];
			p[1] = buf[2];
			p[2] = buf[1];
			p[3] = buf[0];
			break;
		}
		iOffset += iSize;
		return *this;
	}

//////////////////////////////////////////////////////////////////////////
// else
#else
	CPacket& operator << (LPTSTR pszStr) {
		lstrcpy((TCHAR*)(wsaBuf.buf + iOffset), pszStr);
#ifdef _UNICODE
		iOffset += (lstrlen(pszStr) + 1) << 1; // NULL 문자 포함
#else
		iOffset += lstrlen(pszStr) + 1; // NULL 문자 포함
#endif
		return *this;
	}

	CPacket& operator >> (LPTSTR pszStr) {
		lstrcpy(pszStr, (TCHAR*)(wsaBuf.buf + iOffset));
#ifdef _UNICODE
		iOffset += (lstrlen(pszStr) + 1) << 1; // NULL 문자 포함
#else
		iOffset += lstrlen(pszStr) + 1; // NULL 문자 포함
#endif
		return *this;
	}

	template <class T> CPacket& operator << (T& t) {
		*(T*)(wsaBuf.buf + iOffset) = t;
		iOffset += sizeof(T);
		return *this;
	}

	template <class T> CPacket& operator >> (T& t) {
		t = *(T*)(wsaBuf.buf + iOffset);
		iOffset += sizeof(T);
		return *this;
	}

#endif

	CPacket& operator = (CPacket& packet) {
		if(iOffset <= (int)cell) {
			memcpy(wsaBuf.buf, packet.GetBuf(), packet.GetBufOffset());
			iOffset = packet.GetBufOffset();
		}
		return packet;
	}
};

#endif //_PACKET_
#endif //_SMALL_NETWORK_