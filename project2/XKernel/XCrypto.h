#define _XNEWCRYPTO

#ifdef _XNEWCRYPTO

#pragma once

#ifndef IN
#define IN
#define OUT
#endif


#define KEYBOX_FILE			"lump.dat"	// 키박스 파일 입니다.
#define KEYVALLENTH			16			// 랜덤생성키의 최대 크기
#define KEY_RANGE			0xff		// 키 갯수..
#define KEY_X_RANGE			100			// x 키값 범위 (0~99) : 범위를 넘지 말아야함..
#define KEY_Y_RANGE			100			// y 키값 범위 (0~99)
#define BUF_SIZE_			2048 * 10000 		// 버퍼 길이..


/*================================================================================
 CLASS NAME : _j_Crypto 
 EXP		: 데이타 암호화 / 복호화
 AUTHOR		: ozzywow
 DATE		: 2003.11. ,   SE : 2005,4,07
=================================================================================*/

struct _sCryptoHeader
{
	short	sLength : 12;		// -2048 - 2047
	short	sCrypto : 2;		// 0 : false, 나머지는 true
	short	sCompressed : 2;	// 0 : flase, 나머지는 true
};

class _j_Crypto
{

private:	
	
	DWORD	m_dwKeyLength;									// Key 의 길이	
	char m_cKeyBox[KEY_RANGE][KEYVALLENTH];					// Key 들이 들어있는 BOX	
	
	int		m_iBufMaxSize ;
	int		m_iBufCurSet ;
	

public:		
	_j_Crypto::_j_Crypto( size_t bufSize ) : m_iBufMaxSize( bufSize ), m_iBufCurSet(0)
	{
		memset(m_cKeyBox,0,sizeof(m_cKeyBox));		
	}
	_j_Crypto::~_j_Crypto()
	{
	}

	
	// 초기화 (키값 생성)
	// pathName : 키 맵으로 사용할 파일 이름..
	// 클래스를 생성할깨 초기 좌표값 (x,y) 를 넣어서 초기화해 주세요.
	bool _j_Crypto::Init(FILE* pKeyMapFile);
	
	//암호화//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 이 함수는 데이타를 암호화 한다.   									//
	// 함수 : Encryption(char * pData, USHORT usDataLen)					//
	// 설명 : 키와 비트 연산을 하고 대각선형 전치 조합으로 데이타를 혼돈시켰음//
	//////////////////////////////////////////////////////////////////////////
	inline bool _j_Crypto::Encryption( IN OUT char * pData, const USHORT usDataLen, const unsigned char key )
	{
	
		// Get key string
		char * cValKey ;
		UINT * uiValKey ;
		cValKey = GetKey( key ) ;		
		uiValKey = (UINT*)cValKey ;	
		

		try {

			int x, i,j,k,l ;
			
			UINT * puiData = (UINT *) pData ;						// 'int *' type data 로 변환		

			j = 0 ;							// line step
			k = usDataLen / KEYVALLENTH ;	// line count
			l = usDataLen % KEYVALLENTH ;	// char count (잔텡이 데이타)			
		
			
			// line data block Encrypto
			for( i = 0, x = 0, j = 0 ; i < k ; i++, x += 4, j += 16 ) {	// Key 와 pData 를 전치한다.				
				puiData[x+0] = uiValKey[0] ^ puiData[x+0] ;
				puiData[x+1] = ~(uiValKey[2] ^ puiData[x+1]) ;
				puiData[x+2] = uiValKey[1] ^ puiData[x+2] ;
				puiData[x+3] = ~(uiValKey[3] ^ puiData[x+3]) ;
			}
			
			// char data Encrypto (잔텡이 데이타)
			//j += KEYVALLENTH ;
			for(int h = 0 ; h < l ; h++ ) {
				pData[j+h] = ~(cValKey[h] ^ pData[j+h]) ;
			}
			
		} // end try
		catch (...) {
			return false ;
		}	
		return true ;
		
	}

#ifdef _SERVER_VERSION_
	//복호화//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 이 함수는 데이타를 복호화 한다.   									//
	// 함수 : Decryption(char * pData, USHORT usDataLen)						//
	// 설명 : 키와 비트 연산을 하고 대각선형 전치 조합으로 데이타를 혼돈시켰음//
	//////////////////////////////////////////////////////////////////////////
	inline bool _j_Crypto::Decryption(char * pData, const USHORT usDataLen, const unsigned char key )
	{	

		// Get key string
		char * cValKey ;
		UINT * uiValKey ;
		cValKey = GetKey( key ) ;		
		uiValKey = (UINT*)cValKey ;	

		try {	
			
			int x, i,j,k,l ;
			
			UINT * puiData = (UINT *) pData ;						// 'int *' type data 로 변환		
			
			j = 0 ;							// line step
			k = usDataLen / KEYVALLENTH ;	// line count
			l = usDataLen % KEYVALLENTH ;	// char count (잔텡이 데이타)			
			
			
			// line data block Encrypto
			for( i = 0, x = 0, j = 0 ; i < k ; i++, x += 4, j += 16 ) {	// Key 와 pData 를 전치한다.				
				puiData[x+0] = uiValKey[0] ^ puiData[x+0] ;
				puiData[x+1] = ~(uiValKey[2] ^ puiData[x+1]) ;
				puiData[x+2] = uiValKey[1] ^ puiData[x+2] ;
				puiData[x+3] = ~(uiValKey[3] ^ puiData[x+3]) ;
			}
			
			// char data Encrypto (잔텡이 데이타)
			//j += KEYVALLENTH ;
			for(int h = 0 ; h < l ; h++ ) {
				pData[j+h] = ~(cValKey[h] ^ pData[j+h]) ;
			}

		}// try
		catch (...) {
			return false ;
		}
		return true ;		
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// 이 함수는 데이타를 암/복호화 한다.									//
	// 함수 : Xor(char * pData, USHORT usDataLen)	인수 : 문자열 포인터(plantext)	//
	// 설명 : 문자열을 입력된 키와 단순한 XOR 연산을 수행한다.              //
	//////////////////////////////////////////////////////////////////////////
	inline char * _j_Crypto::Xor(char *pData, USHORT usDataLen, const unsigned char key )
	{	
		char * cValKey ;
		cValKey = GetKey( key ) ;		
		
		for(USHORT i = 0; i < usDataLen; i++){
			pData[i]=~ pData[i]^cValKey[i % KEYVALLENTH];
		}

		return pData;		
	}	

	
	inline unsigned char _j_Crypto::checksum(unsigned char *buffer, int size) {
		
		unsigned long cksum=0;
		
		while(size >1) {
			cksum+=*buffer++;
			size -=sizeof(unsigned char);
		}
		
		if(size ) {
			cksum += *(unsigned char*)buffer;
		}
		
		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >>16);		
		
		return (unsigned char)(~cksum);
	}


private:
	inline char * _j_Crypto::GetKey(unsigned char key , const char * pathName = NULL )	
	{				
#ifdef _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA
		key += 97; 
#endif
		return m_cKeyBox[key % KEY_RANGE];
	}	
};



struct _Tail {
	unsigned char		crc ;
	unsigned char		seq ;
};

struct _Encapsule_info {	
	char *				buf ;
	unsigned short 		length ;
};

struct _Decapsule_info {	
	char *				buf ;
	unsigned short		length ;
	unsigned char		seq ;
};


#ifdef _SERVER_VERSION_
#define _CAPSULE_BUF_SIZE_		1048576 * 5
#else
#define _CAPSULE_BUF_SIZE_		4096
#endif

#define _TAIL_SIZE_				2
class CCapsulateCrypto : public _j_Crypto
{

private:
	char *	m_pCapsule_buf ;
	int		m_iBufMaxSize ;
	int		m_iBufCurSet ;
	unsigned char	m_ucSeq ;

#ifdef _SERVER_VERSION_
	CRITICAL_SECTION	m_cs ;
	inline void lock() { EnterCriticalSection( &m_cs ) ; }
	inline void unlock() { LeaveCriticalSection( &m_cs ) ; }	
#endif

public:

	CCapsulateCrypto(size_t bufSize = _CAPSULE_BUF_SIZE_ ) : _j_Crypto(bufSize), m_iBufMaxSize(bufSize), m_iBufCurSet(0), m_ucSeq(0)
	{
#ifdef _SERVER_VERSION_
#if (_WIN32_WINNT > 0x0403)
		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_cs, (0xf0000000)|2000 ) )
		{
			throw ;
		}
#else
		InitializeCriticalSection( &m_cs ) ;		
#endif
#endif
		m_pCapsule_buf = new char[bufSize] ;
	}
	~CCapsulateCrypto()
	{
		delete [] m_pCapsule_buf ;
#ifdef _SERVER_VERSION_
		DeleteCriticalSection( &m_cs ) ;
#endif
	}

	inline void InitSeqNum( unsigned char seq ) { m_ucSeq = seq ; }
	inline unsigned char GetSeqNum( void ){ return m_ucSeq; }
	
	inline _Encapsule_info Encapsulate( char * packet, unsigned char key = 0  ) 
	{
		if( key == 0 ) 	{ key = m_ucSeq ; }
		_sCryptoHeader * pHeader = (_sCryptoHeader*)packet ;
		_Encapsule_info capsuleInfo ;

#ifdef _SERVER_VERSION_
		lock() ;
		if( pHeader->sLength + _TAIL_SIZE_ >= m_iBufMaxSize - m_iBufCurSet )
		{
			m_iBufCurSet = 0 ;
		}
		memcpy( &m_pCapsule_buf[m_iBufCurSet], packet, pHeader->sLength ) ;
		packet = &m_pCapsule_buf[m_iBufCurSet] ;
		m_iBufCurSet += pHeader->sLength + _TAIL_SIZE_ ;
		unlock() ;
#else		
		if( pHeader->sLength + _TAIL_SIZE_ > m_iBufMaxSize )
		{
			capsuleInfo.buf = NULL ;
			capsuleInfo.length = 0 ;
			return capsuleInfo ;
		}

		memcpy( m_pCapsule_buf, packet, pHeader->sLength ) ;
		packet = m_pCapsule_buf ;		
#endif
		
		//  Add tail
		pHeader = (_sCryptoHeader*)packet ;
		_Tail * pTail = (_Tail*)&packet[pHeader->sLength] ;
		pTail->crc = checksum( (unsigned char*)packet, pHeader->sLength ) ;
		pTail->seq = m_ucSeq;
		pHeader->sLength +=  _TAIL_SIZE_ ;

		if( m_ucSeq >= 255 )
		{
			m_ucSeq = 1;
		}
		else
		{
			m_ucSeq++;
		}
		

		// encryption
		if( pHeader->sCrypto == 1 )
		{
#ifdef _XUSEENCRYPTPATTERN
			if( Encryption( (char*)&packet[2], pHeader->sLength-_TAIL_SIZE_, key ) == false )		
#else
			if( Encryption( (char*)&packet[2], pHeader->sLength, key ) == false )
#endif
			{
				capsuleInfo.buf = NULL ;
				capsuleInfo.length = 0 ;
				return capsuleInfo ;
			}
		}

		capsuleInfo.buf = packet ;
		capsuleInfo.length = pHeader->sLength ;

		return capsuleInfo ;
	}

#ifdef _SERVER_VERSION_
	inline _Decapsule_info Decapsulate( char * packet, unsigned char key = 0  ) 
	{
		if( key == 0 ) 	{ key = m_ucSeq ; }
		_sCryptoHeader * pHeader = (_sCryptoHeader*)packet ;
		_Decapsule_info	decapsule_info ;
		if( pHeader->sCrypto == 1 )
		{
			if( Decryption( (char*)&packet[2], pHeader->sLength, key ) == false )
			{
				decapsule_info.buf = NULL ;
				decapsule_info.length = 0 ;
				return decapsule_info ;
			}
		}

		pHeader->sLength -= _TAIL_SIZE_ ;
		_Tail * pTail = (_Tail*)&packet[pHeader->sLength] ;
		if( pTail->crc == checksum( (unsigned char*)packet, pHeader->sLength ) )
		{
			decapsule_info.buf = packet ;
			decapsule_info.length = pHeader->sLength ;			
		}
		else
		{
			decapsule_info.buf = NULL ;
			decapsule_info.length = 0 ;
		}
		decapsule_info.seq = pTail->seq ;

		return decapsule_info ;
	}
#endif
};




#else // old crypto

#ifndef _XCRIPTO_
#define _XCRIPTO_

/*
암호화 모듈이 두가지 있습니다.
API 를 사용한 암호화 모듈과.. OS 독립적인 모듈이 있습니다.
아래 '//#define API_CRYPTO_FUNC' 주석을 제거하면..
API를 이용한 암호화 모듈로 컴파일 됩니다.
*/

//#define API_CRYPTO_FUNC

#ifndef API_CRYPTO_FUNC 
// API 암호화 함수 사용하지 않을경우


#define KEYBOX_FILE			"lump.dat"	// 키박스 파일 입니다.
#define PACKET_HEADER_LEN	1			// 패킷의 헤더 길이(설명 : 패킷에서 헤더 부분(길이정보)은 암호화 하지 않습니다.)

#define KEYVALLENTH			16			// 랜덤생성키의 최대 크기
#define KEY_X_RANGE			100			// x 키값 범위 (0~99) : 범위를 넘지 말아야함..
#define KEY_Y_RANGE			100			// y 키값 범위 (0~99)


/*================================================================================
CLASS NAME : _j_Crypto 
EXP		: 데이타 암호화 / 복호화
AUTHOR		: ozzywow
DATE		: 2003.11.
=================================================================================*/


class _j_Crypto
{

protected:
		
	DWORD	m_dwKeyLength;									// Key 의 길이	
	char m_cKeyBox[KEY_X_RANGE][KEY_Y_RANGE][KEYVALLENTH];	// Key 들이 들어있는 BOX
	char * m_cValKey;										// 지금 키
	UINT * m_uiValKey ;
	
	UINT m_buf[2048] ;
	
	int x,y,z, h,i,j,k,l,m ;

public:	
	 _j_Crypto();	
	~_j_Crypto();	
		
	bool _j_Crypto::Init(const char x, const char y, FILE* pKeyMapFile);
	bool _j_Crypto::Encryption(char * pData, USHORT usDataLen);	
	bool _j_Crypto::Decryption(char * pData, USHORT usDataLen);
	char * _j_Crypto::Xor(char *pData, USHORT usDataLen);


protected:
	bool _j_Crypto::GetKey(const char x, const char y, FILE* pKeyMapFile);

private:
};


#else

#include <Wincrypt.h>


/* If a 128-bit RC4 session key is to be generated, 
the value 0x00800000 is combined with any other dwFlags 
predefined value with a bitwise OR operation. */
//#define _WIN32_WINNT 0x0400 
#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
#define KEYLENGTH			0x00800000	
#define ENCRYPT_ALGORITHM	CALG_RC4	//암호화 알고리즘 종류
#define KEYBOX_FILE			"lump.dat"	// 키박스 파일 입니다.
#define PACKET_HEADER_LEN	1			// 패킷의 헤더 길이(설명 : 패킷에서 헤더 부분(길이정보)은 암호화 하지 않습니다.)

#define KEYVALLENTH			10			// 랜덤생성키의 최대 크기
#define KEY_X_RANGE			100			// x 키값 범위 (0~99) : 범위를 넘지 말아야함..
#define KEY_Y_RANGE			100			// y 키값 범위 (0~99)


/*================================================================================
CLASS NAME : _j_Crypto 
EXP		: 데이타 암호화 / 복호화
AUTHOR		: ozzywow
DATE		: 2003.11.
=================================================================================*/

class _j_Crypto
{

protected:
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;

	DWORD	m_dwKeyLength;						//Key 의 길이	
	char m_cKeyBox[KEY_X_RANGE][KEY_Y_RANGE][KEYVALLENTH];
	char * m_cValKey;

public:		
	_j_Crypto();	
	~_j_Crypto();
	
		   bool Init(const char x, const char y, FILE* pKeyMapFile);	
	inline char * Xor(char *pData, USHORT usDataLen);
	inline bool Encryption(char * pData, USHORT usDataLen);	
	inline bool Decryption(char * pData, USHORT usDataLen);

protected:
	bool Cps();
	bool GetKey(const char x, const char y, FILE* pKeyMapFile);
	void Free();	

private:
};

#endif  // defined API_CRYPTO_FUNC 
	
#endif  // _XCRIPTO_

#endif  // old crypto