#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x0500
#endif

#ifndef _UNICODE

#define tin				std::cin
#define tout			std::cout
typedef std::string		tstring;
#define _ttof(s)		atof(s)

#else

#define tin				std::wcin
#define tout			std::wcout
typedef std::wstring	tstring;
#define _ttof(s)		_wtof(s)

#endif

#ifndef SAFE_DEL
#define SAFE_DEL(p) if((p) != NULL) { delete (p); (p) = NULL; }
#endif
#ifndef SAFE_DEL_ARRAY
#define SAFE_DEL_ARRAY(p) if((p) != NULL) { delete [] (p); (p) = NULL; }
#endif
#ifndef SAFE_CLOSE_HANDLE
#define SAFE_CLOSE_HANDLE(h) if((h) != NULL) { CloseHandle(h); (h) = NULL; }
#endif
	
#define IPV4_BUF_SIZE		16
#define IPV6_BUF_SIZE		

class FPOINT {
public:
	float x;
	float y;

	FPOINT() : x(0), y(0) {}
	void operator = (const FPOINT& pt) { x = pt.x; y = pt.y; }
};

class POINT3 {
public:
	int x;
	int y;
	int z;

	POINT3() : x(0), y(0), z(0) {}
	void operator = (const POINT3& pt3) { x = pt3.x; y = pt3.y; z = pt3.z; }
};

class VECTOR3 {
public:
	float x, y, z;

	VECTOR3() : x(0), y(0), z(0) {}
	VECTOR3(float x, float y, float z) : x(x), y(y), z(z) {}
	void operator = (const VECTOR3& v3) { x = v3.x; y = v3.y; z = v3.z; }
	BOOL operator == (const VECTOR3& v3) {
		if(x != v3.x || y != v3.y || z != v3.z) return FALSE;
		else return TRUE;
	}
};

inline DWORD MyRand(DWORD dwSeed = 0) {
	static DWORD dwOld = GetTickCount();
	if(dwSeed != 0) dwOld = dwSeed;
	dwOld = (dwOld*dwOld + dwOld) / 10 + 1;
	return dwOld;
}

inline WORD ConvertByteOrder2(WORD w) {
	WORD	result = 0;
	char*	p = (char*)&result;
	p[0] = (char)((w & 0xFF00) >> 8);
	p[1] = (char)(w & 0x00FF);
	return result;
}

inline DWORD ConvertByteOrder4(DWORD dw) {
	DWORD	result = 0;
	char*	p = (char*)&result;
	p[0] = (char)((dw & 0xFF000000) >> 24);
	p[1] = (char)((dw & 0x00FF0000) >> 16);
	p[2] = (char)((dw & 0x0000FF00) >> 8);
	p[3] = (char)(dw & 0x000000FF);
	return result;
}

