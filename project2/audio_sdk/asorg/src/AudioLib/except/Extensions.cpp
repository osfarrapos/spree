/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#include "Extensions.h"
#include "AudioMgr.h"

using namespace Audio;


EAXListener::EAXListener()
{
	m_pPropertySet = 0;
}



bool EAXListener::OnLoad(IUnknown* pUnknown)
{
	if(!DXAudioMgr()->GetInit()->m_bUseEAX)
		return true;
	if(!pUnknown)
		return false;
	HRESULT hr = pUnknown->QueryInterface(IID_IKsPropertySet, (void**)&m_pPropertySet);
	if(FAILED(hr))
		return false;

	uint32 nSupport = 0;
	hr = m_pPropertySet->QuerySupport(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
		&nSupport);
	if((nSupport != (KSPROPERTY_SUPPORT_GET | 
		KSPROPERTY_SUPPORT_SET)))
	{
		_XASAFE_RELEASE(m_pPropertySet);
		return false;
	}

	EAXListenerProps props;
	hr = m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
		0,
		0,
		(void*)&props,
		sizeof(EAXListenerProps));
	if(FAILED(hr))
	{
		_XASAFE_RELEASE(m_pPropertySet);
		return false;
	}
	return true;
}

bool EAXListener::OnUnload()
{
	_XASAFE_RELEASE(m_pPropertySet);
	return true;
}

void EAXListener::Term()
{
	_XASAFE_RELEASE(m_pPropertySet);
}

void EAXListener::SetRoom(int32 iRoom)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ROOM,
		0,
		0,
		&iRoom,
		sizeof(int32));
}

void EAXListener::SetRoomHF(int32 iRoomHF)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ROOMHF,
		0,
		0,
		&iRoomHF,
		sizeof(int32));
}

void EAXListener::SetRoomRolloffFactor(float fRoomRolloffFactor)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR,
		0,
		0,
		&fRoomRolloffFactor,
		sizeof(float));
}

void EAXListener::SetDecayTime(float fDecayTime)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_DECAYTIME,
		0,
		0,
		&fDecayTime,
		sizeof(float));
}

void EAXListener::SetDecayHFRatio(float fDecayHFRatio)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_DECAYHFRATIO,
		0,
		0,
		&fDecayHFRatio,
		sizeof(float));
}

void EAXListener::SetReflections(int32 iReflections)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_REFLECTIONS,
		0,
		0,
		&iReflections,
		sizeof(int32));
}

void EAXListener::SetReflectionsDelay(float fReflectionsDelay)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_REFLECTIONSDELAY,
		0,
		0,
		&fReflectionsDelay,
		sizeof(float));
}

void EAXListener::SetReverb(int32 iReverb)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_REVERB,
		0,
		0,
		&iReverb,
		sizeof(int32));
}

void EAXListener::SetReverbDelay(float fReverbDelay)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_REVERBDELAY,
		0,
		0,
		&fReverbDelay,
		sizeof(float));
}

void EAXListener::SetEnvironment(uint32 nEnvironment)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ENVIRONMENT,
		0,
		0,
		&nEnvironment,
		sizeof(uint32));
}

void EAXListener::SetEnvironmentSize(float fEnvironmentSize)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ENVIRONMENTSIZE,
		0,
		0,
		&fEnvironmentSize,
		sizeof(float));
}

void EAXListener::SetEnvironmentDiffusion(float fEnvironmentDiffusion)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ENVIRONMENTDIFFUSION,
		0,
		0,
		&fEnvironmentDiffusion,
		sizeof(float));
}

void EAXListener::SetAirAbsorptionHF(float fAirAbsorptionHF)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF,
		0,
		0,
		&fAirAbsorptionHF,
		sizeof(float));
}

void EAXListener::SetFlags(uint32 nFlags)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_FLAGS,
		0,
		0,
		&nFlags,
		sizeof(uint32));
}

void EAXListener::SetProperties(const EAXListenerProps& props)
{
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
		0,
		0,
		(void*)&props,
		sizeof(EAXListenerProps));
}

void EAXListener::GetProperties(EAXListenerProps& props)
{
	if(!m_pPropertySet) return;
	uint32 nSize;
	m_pPropertySet->Get(
		DSPROPSETID_EAX20_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
		0,
		0,
		&props,
		sizeof(EAXListenerProps),
		&nSize);
}




EAXBuffer::EAXBuffer()
{
	Clear();
}


void EAXBuffer::Clear()
{
	m_pPropertySet = 0;
	m_Props.Clear();
};


bool EAXBuffer::OnLoad(IUnknown* pUnknown)
{
	if(!pUnknown)
		return false;
	HRESULT hr = pUnknown->QueryInterface(IID_IKsPropertySet, (void**)&m_pPropertySet);
	if(FAILED(hr))
		return false;

	uint32 nSupport = 0;
	hr = m_pPropertySet->QuerySupport(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
		&nSupport);
	if((nSupport != (KSPROPERTY_SUPPORT_GET | 
		KSPROPERTY_SUPPORT_SET)))
	{
		_XASAFE_RELEASE(m_pPropertySet);
		return false;
	}

	hr = m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
		0,
		0,
		(void*)(&m_Props),
		sizeof(EAXBufferProps));
	if(FAILED(hr))
	{
		_XASAFE_RELEASE(m_pPropertySet);
		return false;
	}
	return true;
}

bool EAXBuffer::OnUnload()
{
	if(!m_pPropertySet)
		return false;

	uint32 nSize;
	m_pPropertySet->Get(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
		0,
		0,
		&m_Props,
		sizeof(EAXBufferProps),
		&nSize);

	_XASAFE_RELEASE(m_pPropertySet);
	return true;
}

void EAXBuffer::Term()
{
	_XASAFE_RELEASE(m_pPropertySet);
}

void EAXBuffer::SetDirect(int32 iDirect)
{
	m_Props.lDirect = iDirect;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_DIRECT,
		0,
		0,
		&iDirect,
		sizeof(int32));
}

void EAXBuffer::SetDirectHF(int32 iDirectHF)
{
	m_Props.lDirectHF = iDirectHF;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_DIRECTHF,
		0,
		0,
		&iDirectHF,
		sizeof(int32));
}

void EAXBuffer::SetRoom(int32 iRoom)
{
	m_Props.lRoom = iRoom;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_ROOM,
		0,
		0,
		&iRoom,
		sizeof(int32));
}

void EAXBuffer::SetRoomHF(int32 iRoomHF)
{
	m_Props.lRoomHF = iRoomHF;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_ROOMHF,
		0,
		0,
		&iRoomHF,
		sizeof(int32));
}

void EAXBuffer::SetRoomRolloffFactor(float fRoomRolloffFactor)
{
	m_Props.flRoomRolloffFactor = fRoomRolloffFactor;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR,
		0,
		0,
		&fRoomRolloffFactor,
		sizeof(float));
}

void EAXBuffer::SetObstruction(int32 iObstruction)
{
	m_Props.lObstruction = iObstruction;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_OBSTRUCTION,
		0,
		0,
		&iObstruction,
		sizeof(int32));
}

void EAXBuffer::SetObstructionLFRatio(float fObstructionLFRatio)
{
	m_Props.flObstructionLFRatio = fObstructionLFRatio;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_OBSTRUCTIONLFRATIO,
		0,
		0,
		&fObstructionLFRatio,
		sizeof(float));
}

void EAXBuffer::SetOcclusion(int32 iOcclusion)
{
	m_Props.lOcclusion = iOcclusion;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_OCCLUSION,
		0,
		0,
		&iOcclusion,
		sizeof(int32));
}

void EAXBuffer::SetOcclusionLFRatio(float fOcclusionLFRatio)
{
	m_Props.flOcclusionLFRatio = fOcclusionLFRatio;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_OCCLUSIONLFRATIO,
		0,
		0,
		&fOcclusionLFRatio,
		sizeof(float));
}

void EAXBuffer::SetOcclusionRoomRatio(float fOcclusionRoomRatio)
{
	m_Props.flOcclusionRoomRatio = fOcclusionRoomRatio;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_OCCLUSIONROOMRATIO,
		0,
		0,
		&fOcclusionRoomRatio,
		sizeof(float));
}

void EAXBuffer::SetOutsideVolumeHF(int32 iOutsideVolumeHF)
{
	m_Props.lOutsideVolumeHF = iOutsideVolumeHF;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_OUTSIDEVOLUMEHF,
		0,
		0,
		&iOutsideVolumeHF,
		sizeof(int32));
}

void EAXBuffer::SetAirAbsorptionFactor(float fAirAbsorptionFactor)
{
	m_Props.flAirAbsorptionFactor = fAirAbsorptionFactor;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_AIRABSORPTIONFACTOR,
		0,
		0,
		&fAirAbsorptionFactor,
		sizeof(float));
}

void EAXBuffer::SetFlags(uint32 nFlags)
{
	m_Props.dwFlags = nFlags;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_FLAGS,
		0,
		0,
		&nFlags,
		sizeof(uint32));
}

void EAXBuffer::SetProperties(const EAXBufferProps& props)
{
	m_Props = props;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_EAX20_BufferProperties,
		DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
		0,
		0,
		(void*)(&props),
		sizeof(EAXBufferProps));
}

void EAXBuffer::GetProperties(EAXBufferProps& props)
{
	uint32 nSize;
	if(m_pPropertySet)
	{
		m_pPropertySet->Get(
			DSPROPSETID_EAX20_BufferProperties,
			DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
			0,
			0,
			&m_Props,
			sizeof(EAXBufferProps),
			&nSize);
	}
	props = m_Props;
}



ZoomFx::ZoomFx()
{
	Clear();
}

void ZoomFx::Clear()
{
	m_pPropertySet = 0;
	m_Props.Clear();
}

bool ZoomFx::OnLoad(IUnknown* pUnknown)
{
	if(!DXAudioMgr()->GetInit()->m_bUseZoomFX)
		return true;
	if(!pUnknown)
		return false;
	HRESULT hr = pUnknown->QueryInterface(IID_IKsPropertySet, (void**)&m_pPropertySet);
	if(FAILED(hr))
		return false;

	uint32 nSupport = 0;
	hr = m_pPropertySet->QuerySupport(
		DSPROPSETID_ZOOMFX_BufferProperties,
		DSPROPERTY_ZOOMFXBUFFER_ALL,
		&nSupport);
	if((nSupport != (KSPROPERTY_SUPPORT_GET | 
		KSPROPERTY_SUPPORT_SET)))
	{
		_XASAFE_RELEASE(m_pPropertySet);
		return false;
	}

	ZoomFXProps props;
	hr = m_pPropertySet->Set(
		DSPROPSETID_ZOOMFX_BufferProperties,
		DSPROPERTY_ZOOMFXBUFFER_ALL,
		0,
		0,
		(void*)&props,
		sizeof(ZoomFXProps));
	if(FAILED(hr))
	{
		_XASAFE_RELEASE(m_pPropertySet);
		return false;
	}
	return true;
}

bool ZoomFx::OnUnload()
{
	if(!m_pPropertySet)
		return false;

	uint32 nSize;
	m_pPropertySet->Get(
		DSPROPSETID_ZOOMFX_BufferProperties,
		DSPROPERTY_ZOOMFXBUFFER_ALL,
		0,
		0,
		&m_Props,
		sizeof(ZoomFXProps),
		&nSize);

	_XASAFE_RELEASE(m_pPropertySet);
	return true;
}

void ZoomFx::Term()
{
	_XASAFE_RELEASE(m_pPropertySet);
}


void ZoomFx::SetBox(const ZoomFXBox& box)
{
	m_Props.box = box;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_ZOOMFX_BufferProperties,
		DSPROPERTY_ZOOMFXBUFFER_BOX,
		0,
		0,
		(void*)&box,
		sizeof(ZoomFXBox));
}

void ZoomFx::SetOrientation(const ZoomFXOrientation& orientation)
{
	m_Props.orientation = orientation;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_ZOOMFX_BufferProperties,
		DSPROPERTY_ZOOMFXBUFFER_ORIENTATION,
		0,
		0,
		(void*)&orientation,
		sizeof(ZoomFXOrientation));
}

void ZoomFx::SetMacroFX(uint32 nMacroFX)
{
	m_Props.lMacroFx = nMacroFX;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_ZOOMFX_BufferProperties,
		DSPROPERTY_ZOOMFXBUFFER_MACROFX_EFFECT,
		0,
		0,
		&nMacroFX,
		sizeof(uint32));
}

void ZoomFx::SetProperties(const ZoomFXProps& props)
{
	m_Props = props;
	if(!m_pPropertySet) return;
	m_pPropertySet->Set(
		DSPROPSETID_ZOOMFX_BufferProperties,
		DSPROPERTY_ZOOMFXBUFFER_ALL,
		0,
		0,
		(void*)&props,
		sizeof(ZoomFXProps));
}

void ZoomFx::GetProperties(ZoomFXProps& props)
{
	props = m_Props;
	if(!m_pPropertySet) return;
	uint32 nSize;
	m_pPropertySet->Get(
		DSPROPSETID_ZOOMFX_BufferProperties,
		DSPROPERTY_ZOOMFXBUFFER_ALL,
		0,
		0,
		&m_Props,
		sizeof(ZoomFXProps),
		&nSize);
}


