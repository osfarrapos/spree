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
#include "AudioStreamFactory.h"
#include "FileStream.h"

using namespace Audio;



//------------------------------------------------------------------------//
bool AudioStreamFactory::CreateAudioStream(IAudioStream*& pStream)
{
	pStream = FileStream::CreateObject();
	if(!pStream)
		return false;
	pStream->AddRef();
	return true;
}

