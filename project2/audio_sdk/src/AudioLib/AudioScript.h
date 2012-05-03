/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef AUDIO_SCRIPT_H__
#define AUDIO_SCRIPT_H__

#include "Audio.h"

struct IDirectMusicScript;

namespace Audio
{


class AudioScript : public IAudioScript
{
DEFINE_POOL(AudioScript);

public:
	bool Init(const AudioScriptInit& init);
	void Destroy();

	bool IsInitialized() const	{  return m_bInitialized;  }

	bool Load();
	bool Unload();
	bool IsLoaded()	const	{  return (m_pScript) ? true : false;  }

	bool SetVariable(std::string sVarName, int32 iVal);
	bool GetVariable(std::string sVarName, int32& iVal);

	bool CallRoutine(std::string sRoutineName);

private:
	AudioScript();
	virtual ~AudioScript();

	void Clear();
	void Term();

private:

	bool					m_bInitialized;
	AudioScriptInit			m_Init;

	IDirectMusicScript*		m_pScript;

	// Load and Play status flags
	bool					m_bQueuePlayback;
	bool					m_bLoading;
	bool					m_bLoaded;

};


}; // namespace Audio

#endif // AUDIO_SCRIPT_H__