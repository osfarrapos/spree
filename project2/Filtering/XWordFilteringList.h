#ifndef	_XWORDFILTER_LIST_
#define	_XWORDFILTER_LIST_

#define	MAX_KEY_LENGTH		3

#define	_XDEF_WORDFILETERINGTYPE_ALWAYS	1
#define	_XDEF_WORDFILETERINGTYPE_NAME	2
#define	_XDEF_WORDFILETERINGTYPE_GUILD	4
#define	_XDEF_WORDFILETERINGTYPE_CHANGE	8

class _XWORDFILTERNODE
{
public:
	_XWORDFILTERNODE();
	virtual ~_XWORDFILTERNODE();

public:
	_XWORDFILTERNODE*	pNext;
	char*				pData;	
#ifdef _XDEF_NEWVERSONFILETERING
	char*				pChangeData;
#endif
	BYTE				FilteringType;
};

class _XWordFilteringList
{
public:
	_XWordFilteringList();
	virtual ~_XWordFilteringList();

protected:
	_XWORDFILTERNODE*	m_pTop;
	

public:
	
	void				AddData(char*, int, BYTE);
#ifdef _XDEF_NEWVERSONFILETERING
	void				AddChangeData(char*, int, char*, int, BYTE);
	char*				SearchChangeData(char*,int&,BYTE&);
#endif
	void				RemoveAll();
	char*				SearchData(char*,BYTE&);
};


#endif