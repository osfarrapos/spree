#ifdef _XDEF_NEXONLOGIN

#ifndef __NMWSWBSESSION_H_E701B5B4_EB67_4445_983D_FB5496AED8DB__
#define __NMWSWBSESSION_H_E701B5B4_EB67_4445_983D_FB5496AED8DB__

#include "NMGSGeneralSession.h"

class CNMWSWBSession : public CNMGSGeneralSession
{
public:
	CNMWSWBSession();
	virtual ~CNMWSWBSession();

	_UInt32_	GetAssocSerialKey( void );
	BOOL		SendWhiteBoardMsg( _UInt32_ uContentId, BYTE* pBuf, _SInt32_ nBufLen );
};

#endif	//	#ifndef __NMWSWBSESSION_H_E701B5B4_EB67_4445_983D_FB5496AED8DB__

#endif