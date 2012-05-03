/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef IAUDIOSTREAM_H__
#define IAUDIOSTREAM_H__

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace Audio
{
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/*! \file IAudioStream.h*/ 

//! IAudioStream is the internal file representation used by the audio system
/*!
IAudioStream, derived from the COM standard IStream interface is used
internally by the audio system to interface with data streams of any kind.
A default file-based stream implementation is used internally by default,
but a client may choose to create a custom implementation and use it 
instead.  
\sa IAudioStreamFactory
*/
class IAudioStream : public IStream
{
public:
	/*!
	Opens an IStream using a unique character-based identifier.  Typically,
	the string represents a file name, but alternative implementations
	may choose to utilize other methods of representing data blocks.
	\param sFileName A unique string-based identifier which can be used
	\return S_OK is returned on success, otherwise E_FAIL is returned.
	to open the data stream.
	*/
	virtual HRESULT __stdcall Open(std::string sFileName) = 0;
};


//! IAudioStreamFactory is a factory template used to create IAudioStream files
/*!
IAudioStreamFactory is an abstract template class used by the audio system
when it must create IAudioStream objects.  A default implementation of this
interface is used to create standard file-based IAudioStream objects, but
a client may choose to create a custom implementation of this class, along with
a custom implementation of IAudioStream, which will be used in place of
the internal default version of the factory.  The custom factory may be passed to
the audio system in the AudioMgrInit structure.  The client is responsible
for allocating and deallocating the object.
\sa IAudioStream, AudioMgrInit, IAudioManager::Init()
*/
class IAudioStreamFactory
{
public:
	/*!
	Creates an IAudioStream-based object.
	\param pStream Holds a pointer to an IAudioStream object which will be
	filled out by the function.
	\return true indicates success, false indicates failure.
	\sa IAudioStream
	*/
	virtual bool CreateAudioStream(IAudioStream*& pStream) = 0;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}; // namespace Audio
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif // IAUDIOSTREAM_H__
