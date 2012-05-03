
#ifndef _XREFERENCE_
#define _XREFERENCE_

#include <assert.h>

class _XReference
{
	int m_nRefCount;

public:
	_XReference()			{ m_nRefCount = 0; }

	void	AddRef()		{ ++ m_nRefCount; }
	void	ReleaseRef()	{ /*assert(m_nRefCount > 0);*/ -- m_nRefCount; if( m_nRefCount < 0 ) m_nRefCount = 0; }
	int		GetRef()		{ return m_nRefCount; }
	void	ResetRef()		{ m_nRefCount = 0; }
};

#endif