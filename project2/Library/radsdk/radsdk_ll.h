#ifndef __RADSDK_LL_H_
#define __RADSDK_LL_H_

#ifdef _ACCLAIM_IGAADSYSTEM

#include "radsdk.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 *
 * Low-level API functions and data
 *
 ****************************************/

/* session id length in bytes */
#define RAD_SESSION_ID_LENGTH	20

/* maximum expected string length */
#define RAD_MAX_STRING			2048

/*
 * the session infomation struct
*/
#pragma pack(push,1)
typedef struct _radSession
{
	unsigned char	pSessionId[RAD_SESSION_ID_LENGTH];
	char 			szContentServer[RAD_MAX_STRING];
	unsigned short	usContentPort;
	char			szImpressionServer[RAD_MAX_STRING];
	unsigned short	usImpressionPort;
	unsigned short	usDurationThreshold;
	unsigned int	tStartTime;
	radInt64		liUserId;
	unsigned int	uiBandwidth;
	int				sock;
} radSession;
#pragma pack(pop)

/* content tracking struct */
typedef struct tag_radContentTrack
{
	char		valid;
	int			cellId;
	int			zoneId;
	radInt64	contentId;
	time_t		min_time;
	time_t		min_size;
	time_t		max_size;
	time_t		startTime;
	time_t		endTime;
	void*		pContent;
} radContentTrack;

/* callback prototype for radGetContentCB */
typedef char (*_cbcontent)(void* bytes, int progress, int total, void* userdata);

/* callback prototype for radGetContentListCB */
typedef char (*_cbcontentlist)(radKeyValue* kv, int progress, int total, void* userdata);

/* Utility Functions */
void radInit();
void radUninit();
int radGetLastSocketError();
const char* radGetExt(const char* str, char* buf); // returns the extension from a filename */

/* Authorization server functions */
int radStartSession(unsigned int appId, radSession *session);
int radEndSession(radSession *session);

/* Content server functions */
int radGetContentList(radSession *session, unsigned short zone, unsigned int contentType, unsigned short maxResults, char *defaultDir, unsigned int *count, radKeyValueList **propList);
int radGetContentListCB(radSession *session, unsigned short zone, unsigned int contentType, unsigned short maxResults, char *defaultDir, unsigned int *count, radKeyValueList **propList, _cbcontentlist cbcontentlist, void* user);
int radGetContent(radSession *session, radInt64 contentId, int offset, int *length, void *buf, char *props);
int radGetContentCB(radSession *session, radInt64 contentId, int offset, int *length, void *buf, char *props, _cbcontent cbcontent, void* user);
int radReportImpression(radSession *session, unsigned short zone, unsigned int cellId, radInt64 contentId, unsigned int duration, unsigned int timestamp, const char* keyvalues);
int radReportImpressionList(radSession *session, void *hImpressions);

#ifdef __cplusplus
}
#endif

#endif //_ACCLAIM_IGAADSYSTEM

#endif