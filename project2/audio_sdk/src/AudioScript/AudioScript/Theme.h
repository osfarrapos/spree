/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __THEME_H
#define __THEME_H

#include "Audio.h"

namespace Audio
{
class Theme;

typedef std::vector<ISegment*> ISegmentVector;
typedef std::map<ISegment*, ISegmentVector*> SegmentTransitionMap;
typedef std::map<Theme*, ISegmentVector*> ThemeTransitionMap;


class Theme
{
public:
	Theme();
	~Theme();

	bool IsInterlude()				{  return m_bInterlude;  }
	void SetInterlude(bool bVal)	{  m_bInterlude = bVal;  }

	bool CreateNode(ISegment* pSegment);
	bool CreateNode(Theme* pTheme);
	bool CreateTransition(ISegment* pFrom, ISegment* pTo);
	bool CreateTransition(Theme* pFrom, ISegment* pTo);

	void StartTheme()				{  m_bFirstTransition = true;  }
	bool GetTransition(Theme* pTheme, ISegment* pFrom, ISegment*& pTo);

private:
	void Clear();

	ISegment*				m_pDefaultSegment;
	SegmentTransitionMap	m_SegmentMap;
	ThemeTransitionMap		m_ThemeMap;

	bool					m_bInterlude;

	bool					m_bFirstTransition;
};

}; // namespace Audio

#endif // __THEME_H