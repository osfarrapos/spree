#ifndef __RAD_H_
#define __RAD_H_

#ifdef _ACCLAIM_IGAADSYSTEM

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define radInt64		__int64
#else
#define radInt64		long long
#endif

/* api return codes */
#define RAD_SUCCESS					0       /* call was successful */
#define RAD_ERROR_SOCKETERROR		-1      /* a socket error occured */
#define RAD_ERROR_NOSERVER			-2      /* error connecting to session server */
#define RAD_ERROR_UNSUPPORTED		-3      /* the server did not recognize a packet */
#define RAD_ERROR_INVALIDPARAMETER	-4      /* one or more parameters to a function were invalid */
#define RAD_ERROR_INTERNALERROR		-5      /* something bad happened internally */
#define RAD_ERROR_COMMUNICATION		-6      /* a communication error occured(connection dropped?) */
#define RAD_ERROR_BUFFERSIZE		-7      /* the buffer passed to getcontent was too small to fit the content */
#define RAD_ERROR_NOCONTENT			-8      /* a request for a non-existent content id occured, or a search revealed no content */
#define RAD_ERROR_CACHE				-9      /* there was a problem with the local cache(disk read-protected or file corrupted) */
#define RAD_ERROR_SYSTEM			-10     /* couldn't create a thread for background data transfer */
#define RAD_ERROR_CANCELLED			-11     /* a transfer operation was cancelled by a callback function */
#define RAD_ERROR_NOTINITIALIZED	-12     /* an api function was called before radInit() or radStart() */

#define RAD_ERROR_DISABLED			-100	/* the advertising subsystem has been globally disabled for this application  */

/****************************************
 *
 * Key-Value Utility functions
 *
 ****************************************/

/* a single key/value node */
typedef struct _radKeyValue
{
	char*					szKey;
	char*					szValue;
	struct _radKeyValue*	pNext;
	struct _radKeyValue*	pPrev;
} radKeyValue;

/* linked list of key/value pairs */
typedef struct _radKeyValueList
{
	radKeyValue*				pKeyValues;
	struct _radKeyValueList*	pNext;
	struct _radKeyValueList*	pPrev;
} radKeyValueList;

/* Key-Value Functions */
void radSetKeyValue(radKeyValue **kv, char *key, char *value);
void radRemoveKeyValue(radKeyValue **kv, char *key);
void radMakeKeyValueString(radKeyValue *kv, char *str, unsigned int strLength);
void radMakeKeyValues(char *str, radKeyValue **kv);
void radFreeKeyValues(radKeyValue *kv);
void radFreeKeyValueList(radKeyValueList* kvl);
const char* radGetValue(radKeyValue* kv, const char* key, const char* def_value);

/****************************************
 *
 * High-level API functions
 *
 ****************************************/

/* Initialize high-level API and begin content caching */
int radStart(unsigned int appId, unsigned int zoneId, unsigned int* contentTypes, int ncontentTypes, const char* defaultDir);

/* Shutdown high-level API */
void radStop();

/* Get a content item from the local content cache
 *
 * Parameters:
 *   Same as radGetContent(), see documentation
 *   contentId is [in/out] and should be set to 0 on entry unless a specific contentid is required
 * Returns:
 *   RAD_SUCCESS on success
 *   RAD_ERROR_NOCONTENT if no content of <contentType> is defined for this zone
 *   RAD_ERROR_* for other errors, see documentation for error descriptions
 */
int radGetLocalContent(unsigned int zone, unsigned int contentType, radInt64* contentId, int *length, void *buf, char* props);

/* Get/Set a parameter
 *
 * Note that set value should be cast to (void*), and get value is pointer to result value
 *
 * Parameters:
 *   name          set/get    type            usage
 *   ----------------------------------------------
 *   "bandwidth"   set        unsigned int    Maximum number of bytes/second to transfer in the background
 *                                            Note that this is approximate and could differ by +/- 20%
 *
 *   "progress"    get        unsigned int    Current background file transfer progress
 *                                            hiword is progress in kilobytes, loword is total size in kilobytes
 *
 * Returns:
 *   RAD_SUCCESS on success
 *   RAD_ERROR_INVALIDPARAMETER on error
 */
int radSetting(const char* setting_name, void* set_value, void* get_value);

/*  Report an impression and queue to the local cache */
int radReportLocalImpression(unsigned short zone, unsigned int cellId, radInt64 contentId, unsigned int duration, unsigned int timestamp, float avgSize, float avgAngle, const char* keyvalues);

#ifdef __cplusplus
}
#endif

#endif //_ACCLAIM_IGAADSYSTEM

#endif