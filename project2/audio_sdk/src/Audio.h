/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#pragma once

#ifndef __AUDIO_H
#define __AUDIO_H

/*! \addtogroup Audio */
//@{

/*! 
\file Audio.h 
Audio.h contains all sound and music related interface files
*/ 
//@}


#pragma warning(disable:4786)
#pragma warning(disable:4251)

#include "AudioLib/Common.h"
#include "AudioLib/Types.h"
#include "AudioLib/Diagnostics.h"
#include "AudioLib/ObjectPool.h"
#include "AudioLib/Errors.h"
#include "AudioLib/Utilities.h"
#include "AudioLib/AudioLibFactory.h"
#include "AudioLib/CDPlayer.h"
#include "AudioLib/IAudioExt.h"
#include "AudioLib/IAudioStream.h"
#include "AudioLib/IAudio.h"

#include "AudioLib/XAudioPackage.h"

#include "AudioScript/AudioScriptFactory.h"
#include "AudioScript/IMusic.h"
#include "AudioScript/ISound.h"

#endif // __AUDIO_H