// XStringDataBase.h: interface for the _XStringDataBase class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XSTRINGDATABASE_H_
#define _XSTRINGDATABASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
#include <list>

typedef enum _XSTRINGDB_SECTIONDEFINE
{
	_XSTRINGDB_SN_MOBMODEL_XMS				= 0,
	_XSTRINGDB_SN_DUELFIGHTMONEYREF_TXT,
	_XSTRINGDB_SN_DUELRANKREF_TXT,

	_XSTRINGDB_SN_ZONELIST_INI_SHORT,
	_XSTRINGDB_SN_ZONELIST_INI_LONG,
	_XSTRINGDB_SN_ZONELIST_INI_TITLE,

	_XSTRINGDB_SN_BUFFNPCREF_TXT_NAME,
	_XSTRINGDB_SN_BUFFNPCREF_TXT_INFO,

	_XSTRINGDB_SN_CHARACTERCONDITION_XMS_NAME,
	_XSTRINGDB_SN_CHARACTERCONDITION_XMS_CONCEPT,

	_XSTRINGDB_SN_CHARACTERSTATE_XMS_NAME,
	_XSTRINGDB_SN_CHARACTERSTATE_XMS_INFO,

	_XSTRINGDB_SN_MASTERYSKILLCONCEPT_XMS,

	_XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME,

	_XSTRINGDB_SN_MASTERY_TABLE_BMS_NAME,
	_XSTRINGDB_SN_MASTERY_TABLE_BMS_CONTEXT,

	_XSTRINGDB_SN_SKILLCONCEPT_XMS_CLAN,
	_XSTRINGDB_SN_SKILLCONCEPT_XMS_STYPETITLE,
	_XSTRINGDB_SN_SKILLCONCEPT_XMS_STYPECONCEPT,
	_XSTRINGDB_SN_SKILLCONCEPT_XMS_CONCEPT,

	_XSTRINGDB_SN_SKILL_TABLE_XLS_NAME,
	_XSTRINGDB_SN_SKILL_TABLE_XLS_CONCEPT,

	_XSTRINGDB_SN_NICNAMEREF_TXT_TITLE,
	_XSTRINGDB_SN_NICNAMEREF_TXT_INFO,
	_XSTRINGDB_SN_NICNAMEREF_TXT_DETAILINFO,

	_XSTRINGDB_SN_EODSTRING_XSR
};

#define _XDEF_XSTRINGDBHEADER		0xFFCF
#define _XDEF_XSTRINGDBVERSION		0x0001
#define _XDEF_MAXLANGUAGECOUNT		5

struct _XTEXTSTRING_INFO
{
	int			iResourceIndex;
	LPTSTR		strTextString;
	int			iParameterOrder;
	_XTEXTSTRING_INFO()
	{
		strTextString = NULL;
	}
	~_XTEXTSTRING_INFO()
	{
		SAFE_DELETE_ARRAY( strTextString );
	}
};

typedef std::vector<_XTEXTSTRING_INFO*>							svdef_XTEXTSTRING_INFO;
typedef std::map<int, _XTEXTSTRING_INFO*>						smdef_XTEXTSTRING_INFOMAP;

typedef struct _XSTRINGSECTIONINFO
{
	int								m_stringcnt;
	TCHAR							m_sectioname[64];
	
	svdef_XTEXTSTRING_INFO			m_StringVector;
	smdef_XTEXTSTRING_INFOMAP		m_StringMap;

	~_XSTRINGSECTIONINFO();	
	
} XSTRINGSECTIONINFO;

typedef struct _XSTRINGSECTIONREADINFO
{
	int		stringcnt;
	TCHAR	sectioname[64];
} XSTRINGSECTIONREADINFO;


typedef struct _XLOCALIZEDATAINFO
{
	int		stringuid;
	int		resourceid;	
	TCHAR	KoreanString[1024];
	int		KoreanOrder;
	TCHAR	VietnameseString[1024];
	int		VietnameseOrder;
	TCHAR	TaiwaneseString[1024];
	int		TaiwaneseOrder;
	TCHAR	EnglishString[1024];
	int		EnglishOrder;
	TCHAR	ChineseString[1024];
	int		ChineseOrder;
} XLOCALIZEDATAINFO;


class _XStringDataBase  
{
public:
	int								m_TotalSectionCount;
	_XSTRINGSECTIONINFO*			m_pSectionInfo;		

public:
	_XStringDataBase();
	virtual ~_XStringDataBase();

	BOOL	Load( LPCTSTR filename, int languageindex  );
	void	Release( void );

	LPTSTR	GetTextString( _XSTRINGDB_SECTIONDEFINE sectionindex, int resourceid );
};

#endif // !defined(_XSTRINGDATABASE_H_)
