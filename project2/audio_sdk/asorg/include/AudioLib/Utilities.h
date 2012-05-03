/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef __UTILITIES_H__
#define __UTILITIES_H__


namespace Audio
{

// This template will only work with pointers to single objects
template <class T>
inline void _XASAFE_DELETE(T& pObj)
{
	if(pObj)
	{
		delete pObj;
		pObj = 0;
	}
}

// This template function will work with dynamically allocated arrays
template <class T>
inline void _XASAFE_DELETE_ARRAY(T& pObjArray)
{
	if(pObjArray)
	{
		delete[] pObjArray;
		pObjArray = 0;
	}
}

// Safely releases COM objects and sets the interface ptr to null
template <class T>
inline void _XASAFE_RELEASE(T& pCOM)
{
	if(pCOM)
	{
		pCOM->Release();
		pCOM = 0;
	}
}

// Allows a cleaner way to clear out an STL queue
#define CLEAR_STL_QUEUE(_1) \
	while(!_1.empty()) \
	_1.pop();

// These classes allow an STL object to compare object values instead of
// comparing the value of the objects' pointers.
template<class _Ty>
struct ptr_less : std::binary_function<_Ty, _Ty, bool> {
	bool operator()(const _Ty& _X, const _Ty& _Y) const
		{return (*_X < *_Y); }
	};

// Handy critical section object ensures that LeaveCriticalSection()
// is always called no matter how the function exits.
class CritSectObj
{
public:
	CritSectObj(LPCRITICAL_SECTION lpCriticalSection)
	{
		m_pCriticalSection = lpCriticalSection;
		EnterCriticalSection(m_pCriticalSection);  
	}
	~CritSectObj()
	{
		LeaveCriticalSection(m_pCriticalSection);
	}
private:
	LPCRITICAL_SECTION m_pCriticalSection;
};

#define CRITICAL_FUNCTION(lpCriticalSection) \
	CritSectObj __CritSectObj(lpCriticalSection);

class IAudioStream;
class CreateAudioStream
{
public:
	CreateAudioStream(IAudioStream*& pStream);
	~CreateAudioStream();
private:
	IAudioStream*		m_pStream;
};



// convinient conversion from bytes to megabytes
inline size_t Megabytes(size_t MB)	
{  return (MB*1048576);  }


// Handy string conversion classes
static void StringToUpper(std::string& sString)
{
	for(int i = 0; i < sString.length(); i++)
		sString[i] = toupper(sString[i]);
}

static void StringToLower(std::string& sString)
{
	for(int i = 0; i < sString.length(); i++)
		sString[i] = tolower(sString[i]);
}

// Random number utility classes
static float GetRandom(float fMin, float fMax)
{
	return (float(rand() % RAND_MAX) / float(RAND_MAX)) * (fMax - fMin) + fMin;
}

static int GetRandom(int iMin, int iMax)
{
	return (rand() % (iMax - iMin)) + iMin;
}

// Value clamping utility functions
template <class T>
inline T ClampMin(T val, T min)
{  return (val < min) ? min : val;  }

template <class T>
inline T ClampMax(T val, T max)
{  return (val > max) ? max : val;  }

template <class T>
inline T Clamp(T val, T min, T max)
{  return ClampMin(ClampMax(val, max), min);  }

//-----------------------------------------------------------------------------
// UNICODE support for converting between CHAR, TCHAR, and WCHAR strings
//-----------------------------------------------------------------------------
void ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar = -1 );
void ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar = -1 );
void ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
void ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
void ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, int cchDestChar = -1 );
void ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, int cchDestChar = -1 );


// class used for dynamically adjusting a variable over time
class VarAdjust
{
public:
	VarAdjust();
	void Clear();
	void Init(float fStartVal, float fEndVal, float fTime);
	float GetVar();
	bool IsFinished();

private:
	float	m_fStartTime;
	float	m_fEndTime;
	float	m_fStartVal;
	float	m_fEndVal;
	bool	m_bFirstCheck;
};


}; // namespace Audio

#endif // __UTILITIES_H__