/*----------------------------------------------------------------------
Copyright (c) 1998 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	Store.h
Owner:	leea@gipsysoft.com
Purpose:	Base class which implements the base functionality which CStream needs
----------------------------------------------------------------------*/
#ifndef STORE_H
#define STORE_H

#ifndef COMMON_H
	#include <common.h>
#endif	//	COMMON_H

class CStoreABC
{
	public:
		enum {knNoVersion = 0};
		CStoreABC(): m_bIsReading( false ), m_lVersion( knNoVersion ){}
		CStoreABC( bool bIsReading ): m_bIsReading( bIsReading ), m_lVersion( knNoVersion ){}

		void	SetReading( bool bIsReading ){ m_bIsReading = bIsReading; }
		virtual PI_uint32 Read( void *pBytes, PI_uint32 uLength ) = 0;
		virtual void Write( const void *pBytes, PI_uint32 uLength ) = 0;
		virtual bool IsEndOfStore() const = 0;
		virtual PI_int32 GetVersion(bool bIgnoreCaching = false )
		{
			if( bIgnoreCaching || m_lVersion == knNoVersion )
			{
				ASSERT( IsReading() );
				(void)Read( &m_lVersion, sizeof( m_lVersion ) );
			}
			return m_lVersion;
		}
		
		virtual void SetVersion( PI_int32 lVersion )
		{
			//
			//	Must be in write mode
			ASSERT( !IsReading() );

			m_lVersion = lVersion;
			Write( &lVersion, sizeof( lVersion ) );
		}

		bool IsReading() const { return m_bIsReading; }

	private:
		bool m_bIsReading;
		PI_int32 m_lVersion;
};


#endif //STORE_H