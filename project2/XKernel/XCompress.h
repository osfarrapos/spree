// XCompress.h: interface for the XCompress class.
// Code by Park il jun 
//////////////////////////////////////////////////////////////////////

#ifndef _XCOMPRESS_H_
#define _XCOMPRESS_H_

#define   FLAG_Copied         0x80
#define   FLAG_Compress       0x40

class _XCompress  
{
public:
	_XCompress();
	virtual ~_XCompress();

	BOOL  GetMatch( LPBYTE Source, DWORD X, DWORD SourceSize, int *Hash, DWORD *Size, int *Pos );

	DWORD Compression(   LPBYTE Source, LPBYTE  Dest, DWORD SourceSize );

	DWORD Decompression( LPBYTE Source, LPBYTE  Dest, DWORD SourceSize );

};

#endif // !defined(_XCOMPRESS_H_)
