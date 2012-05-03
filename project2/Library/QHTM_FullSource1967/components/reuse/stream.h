/*----------------------------------------------------------------------
Copyright (c) 1998 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	Stream.h
Owner:	leea@gipsysoft.com
Purpose:	A serialisation class
----------------------------------------------------------------------*/
#ifndef STREAM_H
#define STREAM_H

#ifndef STORE_H
	#include "store.h"
#endif //#ifndef STORE_H

#ifndef COMMON_H
	#include "common.h"
#endif

class CStream
{
	public:
	CStream( CStoreABC &store );

	bool IsReading() const { return m_Store.IsReading(); }
	bool IsEndOfStream() const{ return m_Store.IsEndOfStore(); }

	PI_int32 GetVersion( bool bIgnoreCachedVersion = false ) { return m_Store.GetVersion( bIgnoreCachedVersion ); }
	void SetVersion( PI_int32 lVersion ) { m_Store.SetVersion( lVersion ); }

	//
	// Store operations
#ifdef WIN32
	CStream& operator	<< ( const LONG l );
	CStream& operator	<< ( const ULONG l );
	CStream& operator	>> ( LONG &l );
	CStream& operator	>> ( ULONG &l );
#endif	//	WIN32

	CStream& operator	<< ( const PI_int32 i );
	CStream& operator	<< ( const PI_uint32 u );

	CStream& operator	<< ( const float f );
	CStream& operator	<< ( const double d );
	CStream& operator	<< ( const PI_int16 w );
	CStream& operator	<< ( const PI_uint16 w );
	CStream& operator	<< ( const PI_int8 ch );
	CStream& operator	<< ( const PI_uint8 u );
	CStream& operator	<< ( const bool b );
	void		Write( const void *pBytes, PI_uint32 uLength ); 
										
	CStream& operator	>> ( PI_int32 &i );
	CStream& operator	>> ( PI_uint32 &u );

	CStream& operator	>> ( float &f );
	CStream& operator	>> ( double &d );
	CStream& operator	>> ( PI_int16 &w );
	CStream& operator	>> ( PI_uint16 &w );
	CStream& operator	>> ( PI_int8 &ch );
	CStream& operator	>> ( PI_uint8 &u );
	CStream& operator	>> ( bool &b );
	void		Read( void *pBytes, PI_uint32 uLength ); 


	private:
		CStoreABC &m_Store;

		CStream();
		CStream( const CStream & );
		CStream & operator =( const CStream & );
};

#endif //STREAM_H