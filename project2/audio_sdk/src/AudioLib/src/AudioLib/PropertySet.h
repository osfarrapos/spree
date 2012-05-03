/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef PROPERTYSET_H__
#define PROPERTYSET_H__

#include "Audio.h"

namespace Audio
{

struct GenericProperty
{
	GenericProperty()
	{
		m_guid = GUID_NULL;
		m_nID = 0;
		m_pInstanceData = 0;
		m_nInstanceLength = 0;
		m_pPropData = 0;
		m_nPropLength = 0;
	}
	GUID	m_guid;
	uint32	m_nID;
	uint8*	m_pInstanceData;
	uint32	m_nInstanceLength;
	uint8*	m_pPropData;
	uint32	m_nPropLength;
};

typedef std::list<GenericProperty> PropertyList;

class PropertySet
{
public:
	PropertySet();
	~PropertySet();

	void Clear();

	void Term();

	// Call when the associated DS buffer is loaded or unloaded
	bool OnLoad(IUnknown* pUnknown);
	bool OnUnload();

	// Generic property support (for driver-specific extensions)
	bool QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport);
	bool Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, uint32 nPropLength, 
		uint32* pBytesReturned);
	bool Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, uint32 nPropLength,
		bool bStoreProperty);
private:

	IKsPropertySet*		m_pPropertySet;
	PropertyList		m_PropertyList;
};

}; // namespace Audio

#endif // PROPERTYSET_H__