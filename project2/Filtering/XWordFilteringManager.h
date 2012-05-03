#ifndef _XWORDFILTERINGMANAGER_
#define _XWORDFILTERINGMANAGER_


#ifdef WIN32
	#ifndef	_INC_STDIO 
		#include <stdio.h>
	#endif

	#ifndef _INC_MALLOC
		#include <malloc.h>
	#endif

	#ifndef _INC_STRING
		#include <string.h>
	#endif
	
	#ifndef	_XWORDFILTER_LIST_
		#include "XWordFilteringList.h"
	#endif
#else
	#ifndef	_STDIO_H
		#include <stdio.h>
	#endif

	#ifndef _MALLOC_H
		#include <malloc.h>
	#endif

	#ifndef _STRING_H
		#include <string.h>
	#endif

	#ifndef	_XWORDFILTER_LIST_
		#include "XWordFilteringList.h"
	#endif
#endif

#define CHAT_LENGTH_51		51
#define CHAT_LENGTH_256		256
#define CHAT_LENGHT_128      128

class _XWordFilteringManager
{
public:
	_XWordFilteringManager();
	virtual ~_XWordFilteringManager();

public:
	BOOL	DataInit();
	BOOL	DataInit(XMLPaser& m_xXML); //Author : 양희왕 추가
	void	RemoveAll();	
	BOOL	Filtering( LPTSTR strCmpData, BYTE filteringtype, BOOL checkwordonly = FALSE );
    void	SafeDelete();
	LPTSTR  GetLastFilterdWord( void ){ return m_LastFilteredWord; }

private:
	_XWordFilteringList		m_WordFilteringList;	//+ 1은 영문필터링을 위한 리스트 
	 TCHAR					m_LastFilteredWord[256];
};


#endif
