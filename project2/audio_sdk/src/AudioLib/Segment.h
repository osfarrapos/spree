/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __SEGMENT_H
#define __SEGMENT_H

#include "Audio.h"
#include "FileStream.h"

struct IDirectMusicSegment8;
struct IDirectMusicSegmentState8;
struct IDirectMusicAudioPath;

namespace Audio
{



class Segment : public ISegment
{
DEFINE_POOL(Segment);

public:
	bool Init(const SegmentInit& init);
	void Destroy();

	bool IsInitialized() const	{  return m_bInitialized;  }

	bool Load();
	bool Unload();
	bool IsLoaded()	const	{  return m_bLoaded;  }

	// Required interface functions
	bool Play();
	bool Stop();
	bool Pause();

	bool IsPlaying() const;
	bool IsPaused() const;

	bool IsLooping() const;

	// Generic property support (for driver-specific extensions)
	bool QuerySupport(const GUID& guid, uint32 nID, uint32* pTypeSupport);
	bool Get(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
		uint32 nPropLength, uint32* pBytesReturned);
	bool Set(const GUID& guidProperty, uint32 nID, void* pInstanceData,
		uint32 nInstanceLength, void* pPropData, 
		uint32 nPropLength, bool bStoreProperty);

public:
	void Term();
	IDirectMusicSegment8* Obj()
	{  return m_pSegment;  }
	void Clear();
	uint32 GetLastPlayTime() const;
	bool operator < (const Segment& seg) const;
	bool IsLoading() const		{  return m_bLoading;  }
	bool DoLoad();
	bool IsMusic()  const		{  return m_Init.m_bMusic;  }

private:
	Segment();
	virtual ~Segment();
	// Actually plays the segment.  The Play() call may only queue the
	// segment to play next, if there is a segment already playing.
	bool DoPlay();


private:

	IDirectMusicSegment8*		m_pSegment;
	IDirectMusicSegmentState8*	m_pSegState;
	SegmentInit					m_Init;
	bool						m_bInitialized;
	bool						m_bPaused;
	bool						m_b3DSegment;
	int32						m_iPlayStartTime;
	int32						m_iPauseTime;

	// Last time the sound was played (used for prioritization)
	uint32						m_nLastTimePlayed;

	// Load and Play status flags
	bool						m_bQueuePlayback;
	bool						m_bLoading;
	bool						m_bLoaded;
};

}; // namespace Audio


#endif // __SEGMENT_H