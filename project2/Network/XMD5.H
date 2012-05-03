/* MD5.H - header file for MD5C.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

/* MD5 context. */
typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

void _XMD5Init(MD5_CTX* context);
void _XMD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputLen);
void _XMD5Final( unsigned char* digest, MD5_CTX *context );

struct MSG_ACCLAIM_LOGIN
{
	char Mark[2];
	char GameID[4];
	char UserID[20];
	char Password[40];
};

struct MSG_ACCLAIM_LOGINRESULT
{
	char ReturnCode[2];
	char ReturnMessage[80];
};


int AcclaimAuthentication( LPTSTR IDString, LPTSTR PWString );