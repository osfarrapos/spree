/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __IAUDIOLOADER_H__
#define __IAUDIOLOADER_H__

#include "Audio.h"

namespace Audio
{

class IAudioLoader
{
public:
	
	// This Open function is used by the current audio
	// system.  It is expected that it will acquire
	// an IAudioStream interface using the filename,
	// which technically could simply be a string ID
	// for a custom file system as well.
    virtual bool Open(std::string sFileName) = 0;

	// This decodes data directly to a memory buffer in
	// a single batch.
	virtual bool Open(BYTE* pbData, uint32 dwDataSize) = 0;

	// Close the input stream
    virtual bool Close() = 0;

	// Read from the input stream
    virtual bool Read( BYTE* pBuffer, uint32 dwSizeToRead, uint32* pdwSizeRead ) = 0;

	// Get the total size of the input stream
    virtual uint32 GetSize() = 0;

	// Reset the input stream to the beginning
    virtual bool Reset() = 0;

	// Get the PCM format of the input stream
    virtual WAVEFORMATEX* GetFormat() = 0;

	// Return true if the end of the stream is reached
	virtual bool IsEOF() = 0;

	// Function used to destroy pool-managed objects
	virtual void Destroy() = 0;

protected:
	virtual ~IAudioLoader()	{}
};

}; // namespace Audio

#endif // __IAUDIOLOADER_H__