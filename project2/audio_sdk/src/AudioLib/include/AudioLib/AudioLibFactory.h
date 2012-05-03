/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __AUDIO_LIB_FACTORY_H
#define __AUDIO_LIB_FACTORY_H

namespace Audio
{

class IAudioManager;

class AudioLibFactory
{
public:
	static IAudioManager* GetAudioMgr();

};

}; // namespace Audio

#endif // __AUDIO_LIB_FACTORY_H