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

#include "Audio.h"

using namespace Audio;

//------------------------------------------------------------------------//
// Constructor for the cd player.  
CDPlay::CDPlay()
{
	Clear();
}


//------------------------------------------------------------------------//
CDPlay::~CDPlay()
{
	Term();
}


//------------------------------------------------------------------------//
void CDPlay::Clear()
{
	m_bLoop = false;
	m_nCurrentTrack = 1;
	m_Status = CDSTATUS_STOPPED;
	m_hWindow = NULL;
	m_iDeviceID = 0;
	m_bInitialized = false;
}


//------------------------------------------------------------------------//
// The open function prepares the CD player for playing and
// checks to see if it is ready
bool CDPlay::Init(HWND hWindow, const char* pDrive)
{
	if(m_bInitialized)
		return Error::Handle("CD Player is already initialized");

	// This tells which window should receive the MCI messages
	m_hWindow = hWindow;

	// Open the MCI cdaudio device
	MCI_OPEN_PARMS OpenParms;
	OpenParms.lpstrDeviceType = "cdaudio";
	DWORD dwFlags = MCI_OPEN_TYPE | MCI_WAIT;
	// If a user has specified a drive letter, then try
	// to initialize that device by name.
	if(pDrive)
	{
		TCHAR	szElementName[4];
		if((strlen(pDrive) > 1) && (pDrive[1] == ':'))
			wsprintf(szElementName, TEXT("%s"), pDrive);
		else
			wsprintf(szElementName, TEXT("%s:"), pDrive);
		OpenParms.lpstrElementName = szElementName;
		dwFlags |= MCI_OPEN_ELEMENT;
	}
	MCIERROR error = mciSendCommand(0, MCI_OPEN, dwFlags,
		(DWORD)(LPVOID)&OpenParms);
	if(error)
		return Error::Handle("Could not initialize CD Player");

	// Store this value for later comparison when checking
	// MCI Windows messages.
	m_iDeviceID = OpenParms.wDeviceID;

	// Set the proper time format
	MCI_SET_PARMS SetParms;
	SetParms.dwTimeFormat = MCI_FORMAT_TMSF;
	error = mciSendCommand(m_iDeviceID, MCI_SET, MCI_SET_TIME_FORMAT,
		(DWORD)(LPVOID)&SetParms);
	if(error)
		return Error::Handle("Could not open CD Player.");

	m_bInitialized = true;

	return true;
}



//------------------------------------------------------------------------//
// Closes the CD Audio device.  Make sure this function is
// called before the program exits, or it may leave the device
// in limbo.
void CDPlay::Term()
{
	// make sure the CD stops before closing it.
	if(IsPlaying())
		Stop();

	mciSendCommand(m_iDeviceID, MCI_CLOSE, 0, NULL);

	Clear();
}


//------------------------------------------------------------------------//
// This play function plays a single track.  If the looping
// variable is set then it will play this track repeatedly.
bool CDPlay::Play()
{
	if(!m_bInitialized)
		return Error::Handle("CD Player is not initialized");
	MCIERROR error;
	// Depending on whether we're paused or not, either send
	// a Resume or a Play command to the CD device.
	if(IsPaused())
	{
		MCI_GENERIC_PARMS DefaultParms;
		DefaultParms.dwCallback = MAKELONG(m_hWindow, 0);
		error = mciSendCommand(m_iDeviceID, MCI_RESUME, 0, 
			(DWORD)(LPVOID)&DefaultParms);
	}
	else
	{
		MCI_PLAY_PARMS PlayParms;
		PlayParms.dwFrom = m_nCurrentTrack;
		PlayParms.dwTo = m_nCurrentTrack + 1;
		PlayParms.dwCallback = DWORD(m_hWindow);
		error = mciSendCommand(m_iDeviceID, MCI_PLAY, 
			MCI_FROM | MCI_TO | MCI_NOTIFY, (DWORD)(LPVOID)&PlayParms);
	}
	if(error)
	{
		m_Status = CDSTATUS_STOPPED;
		return Error::Handle("Could not play CD Track %d", m_nCurrentTrack);
	}
	m_Status = CDSTATUS_PLAY;
	return true;
}


//------------------------------------------------------------------------//
bool CDPlay::Pause()
{
	if(!m_bInitialized)
		return Error::Handle("CD Player is not initialized");
	MCIERROR error;
	MCI_GENERIC_PARMS DefaultParms;
	DefaultParms.dwCallback = MAKELONG(m_hWindow, 0);
	error = mciSendCommand(m_iDeviceID, MCI_PAUSE, 
		MCI_NOTIFY, (DWORD)(LPVOID)&DefaultParms);
	if(error)
		return Error::Handle("Could not pause CD Audio");
	m_Status = CDSTATUS_PAUSED;
	return true;
}

//------------------------------------------------------------------------//
bool CDPlay::Stop()
{
	if(!m_bInitialized)
		return Error::Handle("CD Player is not initialized");
	MCIERROR error;
	MCI_GENERIC_PARMS DefaultParms;
	DefaultParms.dwCallback = MAKELONG(m_hWindow, 0);
	error = mciSendCommand(m_iDeviceID, MCI_STOP, 
		0, (DWORD)(LPVOID)&DefaultParms);
	if(error)
		return Error::Handle("Could not stop CD player");
	m_Status = CDSTATUS_STOPPED;
	return true;
}

//------------------------------------------------------------------------//
// The Continue function should be called from a window's
// message handling loop.  See above notes for details
// on how to check for the appropriate messages.
bool CDPlay::OnMCINotify(WPARAM wParam, LPARAM lParam)
{
	if(!m_bInitialized)
		return Error::Handle("CD Player is not initialized");
	if(long(lParam) == m_iDeviceID)
	{
		switch(wParam)
		{
		// An MCI command has successfully completed
		case MCI_NOTIFY_SUCCESSFUL:
			switch(m_Status)
			{
			case CDSTATUS_STOPPED:
				break;
			case CDSTATUS_PLAY:
				if(m_bLoop)
				{
					if(!Play())
						return Error::Handle("Could not play selected CD track");
				}
				else
					m_Status = CDSTATUS_STOPPED;
				break;
			break;
			};
		// An MCI command has superceded a previously executed command
		case MCI_NOTIFY_SUPERSEDED:
			break;
		};
	}

	return true;
}


