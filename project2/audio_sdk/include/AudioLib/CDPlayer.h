/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/


/*
Notes:

In the window's message handler, you should check for the case 
MM_MCINOTIFY, and call the OnMCINotify() function in response.

This is an example of what a typical Win32 program's message 
handler might look like:

long WINAPI _export WindowProc(HWND window, UINT message,
	UINT wParam, LONG lParam)
{
	switch (message)
	{

	.
	.
	.

	case MM_MCINOTIFY:

		CDPlayer()->OnMCINotify(wParam, lParam);
		break;
	}

*/


#ifndef __CDPLAYER_H
#define __CDPLAYER_H

namespace Audio
{

class CDPlay
{
private:

enum CD_STATUS_TYPE
{
	CDSTATUS_STOPPED,
	CDSTATUS_PAUSED,
	CDSTATUS_PLAY,
};


public:
	CDPlay();
	virtual ~CDPlay();

	void Clear();

	bool Init(HWND hWindow, const char* pDrive = NULL);
	void Term();

	bool Play();
	bool Pause();
	bool Stop();

	bool OnMCINotify(WPARAM wParam, LPARAM lParam);

	void SetLooping(bool loop)			{  m_bLoop = loop;  }
	UINT GetCurrentTrack()				{  return m_nCurrentTrack;  }
	void SetCurrentTrack(UINT track)	{  m_nCurrentTrack = track;  }
	bool IsPlaying()					{  return (m_Status == CDSTATUS_PLAY) ? true : false;  }
	bool IsPaused()						{  return (m_Status == CDSTATUS_PAUSED) ? true : false;  }

private:
	// Window handle is required for the callback function
	HWND m_hWindow;
	// The MCI function requires this device ID
	MCIDEVICEID m_iDeviceID;
	// The currently playing or selected track
	uint32 m_nCurrentTrack;
	// Indicates whether the CD Player should loop the tracks
	bool m_bLoop;
	// Current status of the CD Player
	CD_STATUS_TYPE m_Status;
	// CD player is initialized
	bool m_bInitialized;
};

static CDPlay* CDPlayer()
{  static CDPlay cdplyr;  return &cdplyr;  }


}; // namespace Audio



#endif // __CDPLAYER_H