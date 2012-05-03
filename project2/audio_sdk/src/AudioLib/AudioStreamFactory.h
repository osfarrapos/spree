/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef AUDIO_STREAM_FACTORY_H___
#define AUDIO_STREAM_FACTORY_H___

#include "Audio.h"

namespace Audio
{


class AudioStreamFactory : public IAudioStreamFactory
{
public:
	bool CreateAudioStream(IAudioStream*& pStream);
};


}

#endif // AUDIO_STREAM_FACTORY_H___