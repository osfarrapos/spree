/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Audio.h"
#include "Theme.h"

using namespace std;
using namespace Audio;

//------------------------------------------------------------------------//
Theme::Theme()
{
	Clear();
}

//------------------------------------------------------------------------//
Theme::~Theme()
{
	SegmentTransitionMap::iterator itr;
	for(itr = m_SegmentMap.begin(); itr != m_SegmentMap.end(); ++itr)
	{
		_XASAFE_DELETE(itr->second);
	}
	ThemeTransitionMap::iterator itor;
	for(itor = m_ThemeMap.begin(); itor != m_ThemeMap.end(); ++itor)
	{
		_XASAFE_DELETE(itor->second);
	}
}

//------------------------------------------------------------------------//
void Theme::Clear()
{
	m_pDefaultSegment = 0;
	m_SegmentMap.clear();
	m_ThemeMap.clear();
	m_bInterlude = false;
	m_bFirstTransition = false;
}

//------------------------------------------------------------------------//
bool Theme::CreateNode(ISegment* pSegment)
{
	if(!pSegment)
		return false;

	if(!m_pDefaultSegment)
		m_pDefaultSegment = pSegment;

	ISegmentVector* pList = new ISegmentVector;
	if(!m_SegmentMap.insert(make_pair(pSegment, pList)).second)
		return false;
	return true;
}

//------------------------------------------------------------------------//
bool Theme::CreateNode(Theme* pTheme)
{
	if(!pTheme)
		return false;

	ISegmentVector* pList = new ISegmentVector;
	if(!m_ThemeMap.insert(make_pair(pTheme, pList)).second)
		return false;
	return true;
}

//------------------------------------------------------------------------//
bool Theme::CreateTransition(ISegment* pFrom, ISegment* pTo)
{
	SegmentTransitionMap::iterator itr = m_SegmentMap.find(pFrom);
	if(itr == m_SegmentMap.end() || (!itr->second))
		return false;
	itr->second->push_back(pTo);
	return true;
}

//------------------------------------------------------------------------//
bool Theme::CreateTransition(Theme* pFrom, ISegment* pTo)
{
	ThemeTransitionMap::iterator itr = m_ThemeMap.find(pFrom);
	if(itr == m_ThemeMap.end() || (!itr->second))
		return false;
	itr->second->push_back(pTo);
	return true;
}

//------------------------------------------------------------------------//
bool Theme::GetTransition(Theme* pTheme, ISegment* pFrom, ISegment*& pTo)
{
	bool bFirstTransition = m_bFirstTransition;
	m_bFirstTransition = false;
	pTo = 0;

	// First check to see if we can find a specific transition based on the
	// pFrom Segment
	SegmentTransitionMap::iterator itr = m_SegmentMap.find(pFrom);
	if(itr == m_SegmentMap.end() || !itr->second->size())
	{
		// If we can't find a segment-based transition, look for a
		// theme-based transition instead
		ThemeTransitionMap::iterator itor = m_ThemeMap.find(pTheme);
		if(itor != m_ThemeMap.end() && itor->second->size())
		{
			pTo = itor->second->at(rand() % itor->second->size());
			return true;
		}

		if(!m_pDefaultSegment || !bFirstTransition)
			return false;
		pTo = m_pDefaultSegment;
		DebugOut(1, "Returning default segment as To target");
		return true;
	}
	pTo = itr->second->at(rand() % itr->second->size());
	return true;
}
