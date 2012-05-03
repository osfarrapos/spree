// Request.h: interface for the Request class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REQUEST_H__9F2C9BB6_CBA7_40AF_80A4_09A1CE1CE220__INCLUDED_)
#define AFX_REQUEST_H__9F2C9BB6_CBA7_40AF_80A4_09A1CE1CE220__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>


#define MEM_BUFFER_SIZE 10

/* 
	HTTPRequest: Structure that returns the HTTP headers and message
					from the request
*/
typedef struct
{ 
    LPSTR headerSend;								// Pointer to HTTP header Send 
	LPSTR headerReceive;							// Pointer to HTTP headers Receive
    LPSTR message;									// Pointer to the HTTP message 
    long messageLength;								// Length of the message 
} HTTPRequest;

/* 
	MemBuffer:	Structure used to implement a memory buffer, which is a
				buffer of memory that will grow to hold variable sized
				parts of the HTTP message. 
*/
typedef struct
{
    unsigned	char *buffer;
    unsigned	char *position;
    size_t		size;
} MemBuffer;


class Request  
{
public:
	Request();
	virtual ~Request();

private:
	void		MemBufferCreate(MemBuffer *b);
	void		MemBufferGrow(MemBuffer *b);
	void		MemBufferAddByte(MemBuffer *b, unsigned char byt);
	void		MemBufferAddBuffer(MemBuffer *b, unsigned char *buffer, size_t size);
	DWORD		GetHostAddress(LPCSTR host);
	void		SendString(SOCKET sock,LPCSTR str);
	BOOL		ValidHostChar(char ch);
	void		ParseURL(LPCSTR url,LPSTR protocol,int lprotocol, LPSTR host,int lhost,LPSTR request,int lrequest,int *port);

	int			SendHTTP(LPCSTR url,LPCSTR headers,BYTE *post, DWORD postLength,HTTPRequest *req);

public:
	int			SendRequest(bool IsPost, LPCSTR url, int buffersize, char *pszHeaderSend, char *pszHeaderReceive, char *pszMessage);	
};

#endif // !defined(AFX_REQUEST_H__9F2C9BB6_CBA7_40AF_80A4_09A1CE1CE220__INCLUDED_)
