/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef EXTENSIONS_H__
#define EXTENSIONS_H__

#define INITGUID
#include "Audio.h"
#undef INITGUID

namespace Audio
{

class EAXListener : public IEAXListener
{
public:
	void SetRoom(int32 iRoom);
	void SetRoomHF(int32 iRoomHF);
	void SetRoomRolloffFactor(float fRoomRolloffFactor);
	void SetDecayTime(float fDecayTime);
	void SetDecayHFRatio(float fDecayHFRatio);
	void SetReflections(int32 iReflections);
	void SetReflectionsDelay(float fReflectionsDelay);
	void SetReverb(int32 iReverb);
	void SetReverbDelay(float fReverbDelay);
	void SetEnvironment(uint32 nEnvironment);
	void SetEnvironmentSize(float fEnvironmentSize);
	void SetEnvironmentDiffusion(float fEnvironmentDiffusion);
	void SetAirAbsorptionHF(float fAirAbsorption);
	void SetFlags(uint32 nFlags);
	void SetProperties(const EAXListenerProps& props);
	void GetProperties(EAXListenerProps& props);

public:
	EAXListener();
	bool OnLoad(IUnknown* pUnknown);
	bool OnUnload();
	void Term();

private:
	IKsPropertySet*		m_pPropertySet;
};

class EAXBuffer : public IEAXBuffer
{
public:
	void SetDirect(int32 iDirect);
	void SetDirectHF(int32 iDirectHF);
	void SetRoom(int32 iRoom);
	void SetRoomHF(int32 iRoomHF);
	void SetRoomRolloffFactor(float fRoomRolloffFactor);
	void SetObstruction(int32 iObstruction);
	void SetObstructionLFRatio(float fObstructionLFRatio);
	void SetOcclusion(int32 iOcclusion);
	void SetOcclusionLFRatio(float fOcclusionLFRatio);
	void SetOcclusionRoomRatio(float fOcclusionRoomRatio);
	void SetOutsideVolumeHF(int32 iOutsideVolumeHF);
	void SetAirAbsorptionFactor(float fAirAbsorptionFactor);
	void SetFlags(uint32 nFlags);
	void SetProperties(const EAXBufferProps& props);
	void GetProperties(EAXBufferProps& props);

public:
	EAXBuffer();
	void Clear();
	bool OnLoad(IUnknown* pUnknown);
	bool OnUnload();
	void Term();

private:
	IKsPropertySet*	m_pPropertySet;
	EAXBufferProps	m_Props;
};



class ZoomFx : public IZoomFX
{
public:
	void SetBox(const ZoomFXBox& box);
	void SetOrientation(const ZoomFXOrientation& orientation);
	void SetMacroFX(uint32 nMacroFX);
	void SetProperties(const ZoomFXProps& props);
	void GetProperties(ZoomFXProps& props);
public:
	ZoomFx();
	void Clear();
	bool OnLoad(IUnknown* pUnknown);
	bool OnUnload();
	void Term();

private:
	IKsPropertySet*	m_pPropertySet;
	ZoomFXProps	m_Props;

};



}; // namespace Audio

#endif // EXTENSIONS_H__
