#pragma once

#define _MEGA_					1048576
#define _PAGE_SIZE_				4096

class _h_VMManager
{
protected:
	void *			m_pStartAddr ;
	void *			m_pCurrAddr ;

	UINT			m_uiReserveSize ;
	UINT			m_uiCurrSize ;

public:
	_h_VMManager() : m_pStartAddr(NULL), m_pCurrAddr(NULL), m_uiReserveSize(0), m_uiCurrSize(0) {}
	~_h_VMManager() { Destroy() ; }
	bool Create( unsigned int MegaSize ) ;
	void * MemoryCommit( unsigned int Size ) ;
	void Destroy() ;
} ;