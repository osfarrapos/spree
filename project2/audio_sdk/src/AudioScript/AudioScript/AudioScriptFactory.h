/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __AUDIO_SCRIPT_FACTORY_H
#define __AUDIO_SCRIPT_FACTORY_H

namespace Audio
{

class IMusicManager;
class ISoundManager;

class AudioScriptFactory
{
public:
	static IMusicManager* GetMusicMgr();
	static ISoundManager* GetSoundMgr();

};

}; // namespace Audio

#endif // __AUDIO_SCRIPT_FACTORY_H