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
#include "AudioCommon.h"
#include "AudioMgr.h"
#include "PropertySet.h"

using namespace Audio;

//------------------------------------------------------------------------//
PropertySet::PropertySet()
{
	Clear();
}

//------------------------------------------------------------------------//
PropertySet::~PropertySet()
{
	Term();
}


//------------------------------------------------------------------------//
void PropertySet::Clear()
{
	m_pPropertySet = 0;
	m_PropertyList.clear();
}


//------------------------------------------------------------------------//
void PropertySet::Term()
{
	PropertyList::iterator itr;
	for(itr = m_PropertyList.begin(); itr != m_PropertyList.end(); ++itr)
	{
		_XASAFE_DELETE_ARRAY((*itr).m_pInstanceData);
		_XASAFE_DELETE_ARRAY((*itr).m_pPropData);
	}
	Clear();
}


//------------------------------------------------------------------------//
bool PropertySet::OnLoad(IUnknown* pUnknown)
{
	if(!pUnknown)
		return false;
	IKsPropertySet* pPropertySet;
	HRESULT hr = pUnknown->QueryInterface(IID_IKsPropertySet, (void**)&pPropertySet);
	if(FAILED(hr))
		return false;
	m_pPropertySet = pPropertySet;
	// Iterate through all the properties and set them on the buffer
	PropertyList::iterator itr;
	for(itr = m_PropertyList.begin(); itr != m_PropertyList.end(); ++itr)
		m_pPropertySet->Set((*itr).m_guid, (*itr).m_nID, (*itr).m_pInstanceData, 
		(*itr).m_nInstanceLength, (*itr).m_pPropData, (*itr).m_nPropLength);
	return true;
}

//------------------------------------------------------------------------//
bool PropertySet::OnUnload()
{

	if(!m_pPropertySet)
		return true;

	// Retrieve any buffer property sets
	PropertyList::iterator itr;
	for(itr = m_PropertyList.begin(); itr != m_PropertyList.end(); ++itr)
	{
		uint32 nBytesReturned;
		m_pPropertySet->Get(
			(*itr).m_guid, 
			(*itr).m_nID, 
			(*itr).m_pInstanceData, 
			(*itr).m_nInstanceLength, 
			(*itr).m_pPropData, 
			(*itr).m_nPropLength, 
			&nBytesReturned);
	}

	_XASAFE_RELEASE(m_pPropertySet);
	return true;
}

//------------------------------------------------------------------------//
// Generic property support (for driver-specific extensions)
bool PropertySet::QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport)
{
	FN("PropertySet::QuerySupport()");
	if(!m_pPropertySet)
		return false;
	HRESULT hr = m_pPropertySet->QuerySupport(guid, nID, pTypeSupport);
	if(FAILED(hr))
		return Error::Handle("Error = %s", DXGetErrorString(hr));
	return true;
}


//------------------------------------------------------------------------//
bool PropertySet::Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
	uint32 nInstanceLength, void* pPropData, uint32 nPropLength, uint32* pBytesReturned)
{
	FN("PropertySet::Get()");

	if(m_pPropertySet)
	{
		HRESULT hr = m_pPropertySet->Get(guidProperty, nID, pInstanceData, nInstanceLength, 
			pPropData, nPropLength, pBytesReturned);
		if(FAILED(hr))
			return Error::Handle("Error = %s", DXGetErrorString(hr));
		return true;
	}
	// If we can't retrieve the information from the buffer, we'll
	// retrieve it from our stored data.
	PropertyList::iterator itr;
	for(itr = m_PropertyList.begin(); itr != m_PropertyList.end(); ++itr)
	{
		// Set the data if we have an exact match on both GUID and ID
		if((guidProperty == (*itr).m_guid) && (nID == (*itr).m_nID))
		{
			if(pInstanceData && (nInstanceLength <= (*itr).m_nInstanceLength))
				memcpy(pInstanceData, (*itr).m_pInstanceData, (*itr).m_nInstanceLength);
			if(pPropData && (nPropLength <= (*itr).m_nPropLength))
				memcpy(pPropData, (*itr).m_pPropData, (*itr).m_nPropLength);
			*pBytesReturned = (*itr).m_nPropLength;
			return true;
		}
	}

	return false;
}


//------------------------------------------------------------------------//
bool PropertySet::Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
	uint32 nInstanceLength, void* pPropData, uint32 nPropLength, bool bStoreProperty)
{
	FN("PropertySet::Set()");

	if(m_pPropertySet)
	{	
		HRESULT hr = m_pPropertySet->Set(guidProperty, nID, pInstanceData, 
			nInstanceLength, pPropData, nPropLength);
		if(FAILED(hr))
			// We still need to store the data, so don't return an error
			Error::Handle("Error = %s", DXGetErrorString(hr));
	}
	
	// Don't bother continuing if the user doesn't wish to store the property
	if(!bStoreProperty)
		return true;

	bool bFoundProperty = false;

	// Iterate through all the properties and set the on the buffer
	PropertyList::iterator itr;
	for(itr = m_PropertyList.begin(); itr != m_PropertyList.end(); ++itr)
	{
		// Set the data if we have a match on the property set GUID and
		// the ID
		if((guidProperty == (*itr).m_guid) && (nID == (*itr).m_nID))
		{
			uint32 nBytesReturned = 0;
			// Update instance data if needed
			if(pInstanceData)
			{
				// Generally, we will not have to realloc, but just in case...
				if(nInstanceLength > (*itr).m_nInstanceLength)
				{
					delete[] (*itr).m_pInstanceData;
					(*itr).m_pInstanceData = new uint8[nInstanceLength];
				}
				memcpy((*itr).m_pInstanceData, pInstanceData, nInstanceLength);
				(*itr).m_nInstanceLength = nInstanceLength;
			}
			// Update prop data if needed
			if(pPropData)
			{
				// Generally, we will not have to realloc, but just in case...
				if(nPropLength > (*itr).m_nPropLength)
				{
					delete[] (*itr).m_pPropData;
					(*itr).m_pPropData = new uint8[nPropLength];
				}
				memcpy((*itr).m_pPropData, pPropData, nPropLength);
				(*itr).m_nPropLength = nPropLength;
			}
			bFoundProperty = true;
			break;
		}
	}

	// If we haven't found an existing property (GUID + ID as key), 
	// allocate storage for the data and create a new entry in the 
	// property list
	if(!bFoundProperty)
	{
		GenericProperty prop;
		prop.m_guid = guidProperty;
		prop.m_nID = nID;
		if(pInstanceData)
		{
			prop.m_pInstanceData = new uint8[nInstanceLength];
			memcpy(prop.m_pInstanceData, pInstanceData, nInstanceLength);
			prop.m_nInstanceLength = nInstanceLength;
		}
		if(pPropData)
		{
			prop.m_pPropData = new uint8[nPropLength];
			memcpy(prop.m_pPropData, pPropData, nPropLength);
			prop.m_nPropLength = nPropLength;
		}
		m_PropertyList.push_back(prop);
	}

	return true;
}


