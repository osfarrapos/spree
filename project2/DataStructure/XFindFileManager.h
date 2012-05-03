// XFindFileManager.h: interface for the _XFindFileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFINDFILEMANAGER_H__70E08BA7_87D8_48C3_8F82_D8449F02C115__INCLUDED_)
#define AFX_XFINDFILEMANAGER_H__70E08BA7_87D8_48C3_8F82_D8449F02C115__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#pragma warning(disable:4786)

#include <map>
#include <string>
#include <vector>
#include <functional>

struct _XFINDFILE_INFO
{
	std::string strFileName;
	std::string strPathfileName;

	int			referenceindex;
};

struct _XFINDFILENAME_COMPARE : public std::binary_function<std::string, std::string, bool>
{
	static bool nocase_compare ( char c1, char c2)
	{
		return toupper(c1) < toupper(c2);
	}
	
	bool operator () (const std::string& lhs, const std::string& rhs) const
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), nocase_compare);
	}
};

class _XFindFileManager  
{
private:
	typedef std::vector<_XFINDFILE_INFO*>							svdef_XFINDFILENAME;
	typedef std::map<std::string, int, _XFINDFILENAME_COMPARE>		smdef_FileNameTable;

	int						m_FoundFileCount; 
	svdef_XFINDFILENAME		m_svFindFileNameList;
	smdef_FileNameTable		m_smFindFileTable;	

public:
	_XFindFileManager();
	virtual ~_XFindFileManager();

	void			 FindAllFile( LPCTSTR rootpath );
	void			 FindFile( LPCTSTR rootpath );

	int				 FindFileInList( LPCTSTR filename );


	_XFINDFILE_INFO* GetInfo( int index );	
	int				 GetReferenceData( LPCTSTR filename );
	void			 SetInfo( int index, int data );

	LPCTSTR			 GetPathName( int index );
	LPCTSTR			 GetPathName( LPCTSTR filename );

public:	
	
};

#endif // !defined(AFX_XFINDFILEMANAGER_H__70E08BA7_87D8_48C3_8F82_D8449F02C115__INCLUDED_)

