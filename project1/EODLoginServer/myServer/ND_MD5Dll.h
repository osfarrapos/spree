#ifndef _INDY_ND_SERVER__ND_MD5DLL_H_20070720__
#define _INDY_ND_SERVER__ND_MD5DLL_H_20070720__

#ifdef ND_MD5_EXPORTS
#define ND_MD5_API		__declspec(dllexport)
#else
#define ND_MD5_API		__declspec(dllimport)
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifdef __cplusplus
extern "C" {
#endif
	ND_MD5_API BOOL MD5Hash( IN char * in_data, IN int length, OUT char * out_data );
#ifdef __cplusplus
}
#endif


#endif