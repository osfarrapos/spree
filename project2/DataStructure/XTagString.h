// XTagString.h: interface for the XTagString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTAGSTRING_H__393A044D_5FB9_4B80_94C8_1CE6D7997080__INCLUDED_)
#define AFX_XTAGSTRING_H__393A044D_5FB9_4B80_94C8_1CE6D7997080__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <string>

using namespace std;

class XTagString  
{
public:
	XTagString();
	~XTagString();

	void StringCompare(string pString, BOOL bBold = FALSE, DWORD dwColor = 0xFF000000);
	void Update();
	void StringClear();

	void UpdateListBox( _XListBox& listbox, int& linecount);

private:
	void CheckBold(const string::size_type& index, const string pString);
	void CheckColor(const string::size_type& index, const string pString);
	void SetStringColor( const char* szString, DWORD& dwColor ); 

	XTagString(const XTagString&);
	XTagString& operator=(const XTagString&);

	void TagString(const string pString, BOOL bBold, DWORD dwColor);

private:
	struct TagInfo
	{
		BOOL bTag; //태그 적용이 되어있는지...
		BOOL bStrong; //진한 글씨체인지...
		DWORD dwColor; //색깔
		string strString;

		TagInfo() : bTag(FALSE), bStrong(FALSE), dwColor(0), strString()
		{}
	};
	list<TagInfo*> m_ltString; //스트링 리스트 저장..
};

inline void XTagString::TagString(const string pString, BOOL bBold, DWORD dwColor)
{
	TagInfo* info = new TagInfo;

	info->bStrong = bBold;
	info->dwColor = dwColor;
	info->strString = pString;
	m_ltString.push_back(info);
}

#endif // !defined(AFX_XTAGSTRING_H__393A044D_5FB9_4B80_94C8_1CE6D7997080__INCLUDED_)
