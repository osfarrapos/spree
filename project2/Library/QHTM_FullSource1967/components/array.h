/*----------------------------------------------------------------------
Copyright (c) 1998 Lee Alexander
Please see the file "licence.txt" for licencing details.
File:		Array.h
Owner:	leea@pobox.com
Purpose:useful little array class
----------------------------------------------------------------------*/

#ifndef ARRAY_CONTAINER_H
#define ARRAY_CONTAINER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef _INC_NEW
	#if defined (_WIN32_WCE)
		// There is no new.h for WinCE, so we define a placement new here.
		// Borrowed from wcealt.h (Wince MFC SDK)
		inline void * operator new(size_t, void * ptr) 
		{ 
			return ptr; 
		}
	#else	
		#include <new.h>
	#endif
#endif // #ifndef _INC_NEW

#ifndef COMMON_H
	#include <common.h>
#endif	//	COMMON_H

namespace Container
{
	template< class T>
	class CArray
	{
		public:
			CArray();
			explicit CArray( PI_uint32 uSize );
			CArray( const CArray<T> &arrToCopy );
			virtual ~CArray();
			void Add( const T &newItem );
			void Add( const T *p, PI_uint32 uCount );
			T &Add();

			void InsertAt( PI_uint32 iPos, const T &newItem, PI_uint32 uCount = 1 );
			void InsertAt( PI_uint32 iPos, const CArray<T> &arrToCopy );
			void InsertAt( PI_uint32 iPos, const T *p, PI_uint32 uCount );

			T	&GetAt( PI_uint32 iPos) { return operator [] ( iPos ); }
			const T	&GetAt( PI_uint32 iPos) const { return operator [] ( iPos ); }

			T	&operator[](PI_uint32 uValue);
			const T	&operator[](PI_uint32 iPos) const;
			CArray<T>	&operator = ( const CArray<T> &rhs );

			void RemoveAll();
			void RemoveAt( PI_uint32 iPos, PI_uint32 uItems = 1 );

			PI_uint32 GetSize() const;
			bool SetSize( PI_uint32 uSize ); // returns true if a reallocation was needed on the array


			const T *GetData() const;
			T *GetData();

		private:
			T			*m_pItems;
			PI_uint32	m_uGrowBy
						, m_uItemsAllocated
						,	m_uItemCount;

	};

	#include "array.inl"

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// CArray overides

	/*
		**** NOTE if you create your own versions of these functions then you must put
							them in the Container namespace otherwise the compiler won't find them!
	*/

	template<class T> void MoveItemsMemOverlap( T *pSource, T *pDestination, PI_uint32 uItems )
	//
	//	Moves items from pSource to pDestination. Both memory areas might overlap
	{
		memmove( pDestination, pSource, uItems * sizeof( T ) );
	}


	template<class T> void MoveItemsNoMemOverlap( T *pSource, T *pDestination, PI_uint32 uItems )
	//
	//	Moves items from pSource to pDestination. Both memory areas are guaranteed not
	//	to overlap
	{
		memcpy( pDestination, pSource, uItems * sizeof( T ) );
	}

	template<class T> void CopyItems( const T *pSource, T *pDestination, PI_uint32 uItems )
	//
	//	Copies items from pSource to pDestination. Both memory areas might overlap
	{
		while( uItems-- )
		{
			*pDestination++ = *pSource++;
		}
	}


	template<class T> void ConstructItems( T *pItems, PI_uint32 uItems )
	{
		memset( pItems, 0, uItems * sizeof( T ) );

		//
		// Call the constructor(s)
		for( ; uItems--; pItems++ )
		{
			new((void*)pItems) T;
		}
	}

	template<class T> void DestructItems( T *pItems, PI_uint32 uItems )
	{
		for( ;uItems--; pItems++ )
		{
			pItems -> ~T();
		}
	}


	typedef Container::CArray<PI_byte> ArrayOfBytes;
	typedef Container::CArray<PI_uint32> ArrayOfUInts;
	typedef Container::CArray<PI_byte> ByteArray;
	typedef Container::CArray<TCHAR> ArrayOfChar;

}

#endif // ARRAY_CONTAINER_H
