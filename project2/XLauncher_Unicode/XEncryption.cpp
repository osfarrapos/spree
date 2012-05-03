#include "stdafx.h"

#include "XEncryptiondata.h"

#include "XEncryption.h"

#include "XRegistry.h"

template <class A> inline void _SWAP(A &a, A &b)
{   A   c;
    c = a;
    a = b;
    b = c;
}


_XEncryption::_XEncryption()
{
	Reset();	
}

_XEncryption::~_XEncryption()
{

}

void _XEncryption::Reset( void )
{
	this->m_Shiftrate   = 7;
	this->m_Parityvalue = 31;

	strncpy( m_Encryptionkey, "9E2GH4FWDWKC2MBL4EBNKUV6IT", _ENCRYPTION_FIELD_LENGTH );		
}

void _XEncryption::setKey( UINT shift, UINT parityvalue,  LPCSTR encryptionkey )
{

	if( shift == 0 || shift >= _ENCRYPTION_FIELD_LENGTH )
	{
		AfxMessageBox( _T("ERROR : Invalid shift value") );
		return;
	}

	this->m_Shiftrate   = shift;
	this->m_Parityvalue = parityvalue;
	
	memset( m_Encryptionkey, 0, _ENCRYPTION_FIELD_LENGTH + 1 );
	strncpy( m_Encryptionkey, encryptionkey, _ENCRYPTION_FIELD_LENGTH );		

	for( UINT pos = 0; pos < _ENCRYPTION_FIELD_LENGTH; pos++ )
	{
		 m_Encryptionkey[pos] = _SERIALFIELD_ASCIILUT[ encryptionkey[pos] ];
	}
}

void _XEncryption::leftshiftString ( LPSTR encryptSerial, UINT repeat )
{
	BYTE taildata;

	for( UINT shiftrate = 0; shiftrate < repeat; shiftrate++ )
	{
		taildata  =  encryptSerial[0];

		for( UINT pos = 0; pos < _ENCRYPTION_FIELD_LENGTH-2; pos++ )
		{
			encryptSerial[pos] = encryptSerial[pos+1];
		}

		encryptSerial[_ENCRYPTION_FIELD_LENGTH-2] = taildata;
	}
}

void _XEncryption::rightshiftString( LPSTR encryptSerial, UINT repeat )
{
	BYTE taildata;

	for( UINT shiftrate = 0; shiftrate < repeat; shiftrate++ )
	{
		taildata  =  encryptSerial[_ENCRYPTION_FIELD_LENGTH-2];

		for( UINT pos = _ENCRYPTION_FIELD_LENGTH-2; pos > 0; pos-- )
		{
			encryptSerial[pos] = encryptSerial[pos-1];
		}

		encryptSerial[0] = taildata;
	}
}


int _XEncryption::enciperSerial( LPCSTR orgSerial, LPSTR encryptSerial )
{
	if( !orgSerial || !encryptSerial ) return -1;

	if( strlen( orgSerial ) < _ORIGINAL_FIELD_LENGTH ) return -1;

	char serialencodefield[_ENCRYPTION_FIELD_LENGTH+1];

	memset( serialencodefield, 0, _ENCRYPTION_FIELD_LENGTH+1 );
	memset( encryptSerial, 0, _ENCRYPTION_FIELD_LENGTH+1 );

	strncpy( serialencodefield   , orgSerial   , 4 );
	strncpy( serialencodefield+5 , orgSerial+4 , 4 );
	strncpy( serialencodefield+10, orgSerial+8 , 4 );
	strncpy( serialencodefield+15, orgSerial+12, 4 );
	strncpy( serialencodefield+20, orgSerial+16, 5 );

	UINT sum, swapposition=4;

	for( int sumlevel = 0; sumlevel < _ENCRYPTION_FIELD_LENGTH-1; sumlevel+=5, swapposition-- )
	{
		if( sumlevel < 20 )
		{
			sum = _SERIALFIELD_ASCIILUT[ serialencodefield[sumlevel  ] ]+ 
				  _SERIALFIELD_ASCIILUT[ serialencodefield[sumlevel+1] ]+ 
				  _SERIALFIELD_ASCIILUT[ serialencodefield[sumlevel+2] ]+ 
				  _SERIALFIELD_ASCIILUT[ serialencodefield[sumlevel+3] ];		

			sum = (BYTE)((sum*sum) % m_Parityvalue);

			serialencodefield[sumlevel+4] = _SERIALFIELD_CHARACTERDATA[sum];

			_SWAP( serialencodefield[sumlevel+swapposition], serialencodefield[sumlevel+4] );
		}
		else
		{
			sum = _SERIALFIELD_ASCIILUT[ serialencodefield[sumlevel  ] ]+ 
				  _SERIALFIELD_ASCIILUT[ serialencodefield[sumlevel+1] ]+ 
				  _SERIALFIELD_ASCIILUT[ serialencodefield[sumlevel+2] ]+ 
				  _SERIALFIELD_ASCIILUT[ serialencodefield[sumlevel+3] ]+
				  _SERIALFIELD_ASCIILUT[ serialencodefield[sumlevel+4] ];		

			sum = (BYTE)((sum*sum) % m_Parityvalue);

			serialencodefield[sumlevel+5] = _SERIALFIELD_CHARACTERDATA[sum];		
		}		
	}	

	rightshiftString( serialencodefield, m_Shiftrate );

	for( int pos = 0; pos < _ENCRYPTION_FIELD_LENGTH-2; pos++ )
	{
		_SWAP( serialencodefield[ pos ], serialencodefield[ _MIX_FIELD_LUT[ pos ] ] );
	}

	leftshiftString( serialencodefield, _SERIALFIELD_ASCIILUT[ serialencodefield[25] ] );


	
	strncpy( encryptSerial, serialencodefield, _ENCRYPTION_FIELD_LENGTH );

	return 1;
}


int _XEncryption::deciperSerial( LPCSTR encryptSerial, LPSTR decryptSerial )
{
	if( !encryptSerial || !decryptSerial ) return -1;

	if( strlen( encryptSerial ) < _ENCRYPTION_FIELD_LENGTH ) return -1;

	char serialencodefield[_ENCRYPTION_FIELD_LENGTH+1];

	memset( serialencodefield, 0, _ENCRYPTION_FIELD_LENGTH+1 );
	memset( decryptSerial, 0, _ENCRYPTION_FIELD_LENGTH+1 );

	strncpy( serialencodefield, encryptSerial, _ENCRYPTION_FIELD_LENGTH );

	rightshiftString( serialencodefield, _SERIALFIELD_ASCIILUT[ serialencodefield[25] ] );

	for( int pos = _ENCRYPTION_FIELD_LENGTH-3; pos >= 0; pos-- )
	{
		_SWAP( serialencodefield[ pos ], serialencodefield[ _MIX_FIELD_LUT[ pos ] ] );
	}

	leftshiftString( serialencodefield, m_Shiftrate );


	UINT swapposition=4;
	for( int sumlevel = 0; sumlevel < _ENCRYPTION_FIELD_LENGTH-1; sumlevel+=5, swapposition-- )
	{
		if( sumlevel < 20 )
		{			
			_SWAP( serialencodefield[sumlevel+swapposition], serialencodefield[sumlevel+4] );
		}
	}	
	
	strncpy( decryptSerial, serialencodefield, _ENCRYPTION_FIELD_LENGTH );

	return 1;
}


BOOL _XEncryption::checksumSerial( LPCSTR encryptSerial )
{
	if( strlen( encryptSerial ) < _ENCRYPTION_FIELD_LENGTH ) return FALSE;

	for( int i = 0; i < _ENCRYPTION_FIELD_LENGTH; i++ )
	{
		if( _SERIALFIELD_ASCIILUT[ encryptSerial[i] ] == -1 ) return FALSE;			
	}

	char decryptSerial[_ENCRYPTION_FIELD_LENGTH+1];

	memset( decryptSerial, 0, _ENCRYPTION_FIELD_LENGTH+1 );

	if( deciperSerial( encryptSerial, decryptSerial ) != 1 )
	{
		return FALSE;
	}

	UINT sum;

	for( int sumlevel = 0; sumlevel < _ENCRYPTION_FIELD_LENGTH-1; sumlevel+=5 )
	{
		if( sumlevel < 20 )
		{
			sum = _SERIALFIELD_ASCIILUT[ decryptSerial[sumlevel  ] ]+ 
				  _SERIALFIELD_ASCIILUT[ decryptSerial[sumlevel+1] ]+ 
				  _SERIALFIELD_ASCIILUT[ decryptSerial[sumlevel+2] ]+ 
				  _SERIALFIELD_ASCIILUT[ decryptSerial[sumlevel+3] ];

			sum = (BYTE)((sum*sum) % m_Parityvalue);

			if( decryptSerial[sumlevel+4] != _SERIALFIELD_CHARACTERDATA[sum] ) return FALSE;
			
		}
		else
		{
			sum = _SERIALFIELD_ASCIILUT[ decryptSerial[sumlevel  ] ]+ 
				  _SERIALFIELD_ASCIILUT[ decryptSerial[sumlevel+1] ]+ 
				  _SERIALFIELD_ASCIILUT[ decryptSerial[sumlevel+2] ]+ 
				  _SERIALFIELD_ASCIILUT[ decryptSerial[sumlevel+3] ]+
				  _SERIALFIELD_ASCIILUT[ decryptSerial[sumlevel+4] ];		

			sum = (BYTE)((sum*sum) % m_Parityvalue);

			if( decryptSerial[sumlevel+5] != _SERIALFIELD_CHARACTERDATA[sum] ) return FALSE;
		}		
	}	

	int checkcount[36];
	memset( checkcount, 0, sizeof(int)*36 );

	for( i = 0; i < _ENCRYPTION_FIELD_LENGTH; i++ )
	{
		if( _SERIALFIELD_ASCIILUT[ decryptSerial[i] ] != -1 )
			checkcount[ _SERIALFIELD_ASCIILUT[ decryptSerial[i] ] ]++; 
	}

	for( i = 0; i < _ENCRYPTION_FIELD_LENGTH; i++ )	
	{
		if( checkcount[i] > _REPEATCHARACTER_MAXCOUNT )
		{			
			return FALSE;
		}
	}		

	return TRUE;
}


BOOL _XEncryption::xorEncyper( LPCSTR encryptSerial, LPSTR xorencryptSerial )
{
	if( strlen( encryptSerial ) < _ENCRYPTION_FIELD_LENGTH ) return FALSE;

	for( int i = 0; i < _ENCRYPTION_FIELD_LENGTH; i++ )
	{
		*(xorencryptSerial+i) = *(encryptSerial+i) ^ *(m_Encryptionkey+i);
	}

	return TRUE;
}


DWORD _XEncryption::getDiskSerial(int drv)
{
	TCHAR szBuffer[MAX_PATH + 100];
	TCHAR szLabel[129];
	DWORD dwVolumeSerialNumber=0;
	DWORD dwMaxNameLength=0;
	DWORD dwFileSystemFlags=0;
	TCHAR szFileSysName[129];

	memset( szBuffer, 0, MAX_PATH + 100 );
	memset( szFileSysName, 0, 129 );

	wsprintf( szBuffer, _T("%c:\\"), drv+'A');

	GetVolumeInformation( szBuffer, szLabel, sizeof(szLabel)-1,	&dwVolumeSerialNumber,
						  &dwMaxNameLength,	&dwFileSystemFlags,	szFileSysName,	sizeof(szFileSysName)-1 );
	
	if( dwVolumeSerialNumber > 1000000000 ) dwVolumeSerialNumber = dwVolumeSerialNumber % 1000000000;

	return dwVolumeSerialNumber;
}


BOOL _XEncryption::mixDiskSerial( LPCSTR orgserialnumber, LPSTR mixedserialnumber )
{
	if( !orgserialnumber || !mixedserialnumber ) return FALSE;

	DWORD diskserial = getDiskSerial( 2 ); // C드라이브의 디스크 시리얼 

	char diskserialstring[21];
	char finaldiskserialstring[21];

	memset( diskserialstring, 0, 21 );

	_ltoa( diskserial, diskserialstring, 10 );

	int diskseriallength = strlen( diskserialstring );
	int seriallength = strlen( orgserialnumber );

	int dsrepeater = 0;

	memset( finaldiskserialstring, '0', 21 );
	memset( mixedserialnumber, 0, _REGSTRING_MAXLENGTH+1 );

	memcpy(	finaldiskserialstring+21-diskseriallength, diskserialstring, diskseriallength );
	
	memcpy( mixedserialnumber, orgserialnumber, (_ENCRYPTION_FIELD_LENGTH >> 1) );

	memcpy( mixedserialnumber+(_ENCRYPTION_FIELD_LENGTH >> 1), finaldiskserialstring + 21 - 10, 10 );

	return TRUE;
}


BOOL _XEncryption::SplitSerial( LPCSTR orgserialnumber, LPSTR firstserialnumber, LPSTR secondserialnumber )
{
	if( !orgserialnumber || !firstserialnumber || !secondserialnumber ) return FALSE;

	int orglength    = strlen( orgserialnumber );
	int firstlength  = 0;
	int secondlength = 0;

	firstlength  = secondlength = (_ENCRYPTION_FIELD_LENGTH>>1);	

	if( orglength & 0x0001 ) 
	{
		firstlength++;
	}

	for( int i = 0; i < firstlength; i++ )
	{
		firstserialnumber[i] = orgserialnumber[i<<1];
	}

	for( i = 0; i < secondlength; i++ )
	{
		secondserialnumber[i] = orgserialnumber[(i<<1)+1];
	}

	return TRUE;
}


BOOL  _XEncryption::JointSerial( LPCSTR firstserialnumber, LPCSTR secondserialnumber, LPSTR jointserialnumber )
{
	if( !jointserialnumber || !firstserialnumber || !secondserialnumber ) return FALSE;

	int firstlength  = strlen( firstserialnumber );
	int secondlength = strlen( secondserialnumber );
	int orglength    = _ENCRYPTION_FIELD_LENGTH;

	for( int i = 0; i < orglength; i++ )
	{
		if( i & 0x0001 )
			jointserialnumber[i] = secondserialnumber[i>>1];
		else
			jointserialnumber[i] = firstserialnumber[i>>1];
	}

	return TRUE;
}



DWORD _XEncryption::GetDiskserialFromSerialString( LPCSTR serialstring )
{
	if( !serialstring ) return 0;

	char    diskserialstring[21];
	memset( diskserialstring, 0, 21 );
		
	memcpy( diskserialstring, serialstring+(_ENCRYPTION_FIELD_LENGTH>>1), 10 );

	DWORD diskserialnumber = atol( diskserialstring );

	return diskserialnumber;
}

