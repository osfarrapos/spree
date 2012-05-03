#ifndef _XENCRYPTOR_HEADER_
#define _XENCRYPTOR_HEADER_

#define _ORIGINAL_FIELD_LENGTH     21
#define _ENCRYPTION_FIELD_LENGTH   26
#define _REPEATCHARACTER_MAXCOUNT  10

class _XEncryptor
{

private:
	UINT    m_Shiftrate;
	UINT    m_Parityvalue;

	char    m_Encryptionkey[ _ENCRYPTION_FIELD_LENGTH + 1 ];

	void    leftshiftString ( LPSTR encryptSerial, UINT repeat );
	void    rightshiftString( LPSTR encryptSerial, UINT repeat );
	
public:

	_XEncryptor();
	virtual ~_XEncryptor();

	void    Reset( void );
	
	void    setKey( UINT shift, UINT parityvalue,  LPCSTR encryptionkey );

	int     enciperSerial( LPCSTR orgSerial,     LPSTR encryptSerial );
	int     deciperSerial( LPCSTR encryptSerial, LPSTR decryptSerial );

	BOOL    checksumSerial( LPCSTR encryptSerial );

	BOOL    xorEncyper( LPCSTR encryptSerial, LPSTR xorencryptSerial );

	DWORD   getDiskSerial(int drv);

	BOOL    mixDiskSerial( LPCSTR orgserialnumber, LPSTR mixedserialnumber );
	
	BOOL    SplitSerial( LPCSTR orgserialnumber, LPSTR firstserialnumber, LPSTR secondserialnumber );
		
	BOOL    JointSerial( LPCSTR firstserialnumber, LPCSTR secondserialnumber, LPSTR jointserialnumber );

	DWORD   GetDiskserialFromSerialString( LPCSTR serialstring );
};

#endif