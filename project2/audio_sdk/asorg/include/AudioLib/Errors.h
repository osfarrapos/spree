/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __ERRORS_H__
#define __ERRORS_H__

namespace Audio
{

class Error
{
public:
	// Use HandleError to log error messages and strings for fatal errors.
	// This function can be overridden to provide more complete diagnostics
	// as needed per class.  The default function simply sets the error
	// code and error message for later retrieval, then returns the error code
	// passed in.
	static bool Handle(const char* lpszFormat, ...);

	// Error logging functions
	// The error logging is designed to store every error message passed to
	// Error::Handle for later reference.  It can also be used as a central 
	// repository for logging debug messages for later retrieval in the event
	// of a run-time error.  Because a global list is stored, and this is used
	// in release mode, care must be taken to record only relevant information,
	// preferrably mostly at startup.
	static void Log(const char* lpszFormat, ...);
	static void ClearLog();
	static const char* GetFirst();
	static const char* GetNext();
	static const char* GetLast();

protected:

	static std::list<std::string> m_ErrorLog;
	static std::list<std::string>::iterator m_ErrorLogItor;
};

}; // namespace Audio

#endif // __ERRORS_H__