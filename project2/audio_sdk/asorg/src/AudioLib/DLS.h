/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __DLS_H
#define __DLS_H

#include "Audio.h"
#include "FileStream.h"

struct IDirectMusicCollection;

namespace Audio
{

class DLS : public IDLS
{
DEFINE_POOL(DLS);
public:

	bool Init(const DLSInit& init);
	void Destroy();

	bool IsInitialized() const	{  return m_bInitialized;  }
	bool Lock();
	bool Unlock();

	bool IsLoaded()	const	{  return (m_pCollection) ? true : false;  }
	
private:
	DLS();
	virtual ~DLS();
	IDirectMusicCollection* Obj()
	{  return m_pCollection;  }

	void Clear();
	void Term();
	bool Load();
	bool Unload();


	bool						m_bInitialized;
	IDirectMusicCollection*		m_pCollection; 
	DLSInit						m_Init;
	int32						m_iRefCount;
};

}; // namespace Audio


#endif // __DLS_H