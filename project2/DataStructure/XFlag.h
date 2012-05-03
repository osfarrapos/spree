// XFlag.h: interface for the _XFlag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFLAG_H__2A7DF9EC_5EDA_468D_8EC1_DD4CA978A931__INCLUDED_)
#define AFX_XFLAG_H__2A7DF9EC_5EDA_468D_8EC1_DD4CA978A931__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XUser.h"
#include "EODEXTDEF_ModelData.h"

#define _XDEF_FLAG_DELETETIME			10000

enum _XFlagType
{
	_XFLAG_TYPE_MATCH = 0,		// ºñ¹«
	_XFLAG_TYPE_SEGA			// Ç÷¸Í
};

class _XFlag : public _XItem
{
public :
	_XModelDescriptor			m_ModelDescriptor;

	char						m_FlagType;
	unsigned short				m_FlagID;			// match id or sega id

	D3DXVECTOR3					m_Position;
	FLOAT						m_RotateAngle;

	BOOL						m_bInsideFrustum;

	char						m_MatchType;
	unsigned short				m_Applicant1;
	unsigned short				m_Applicant2;

	DWORD						m_LastMessageReceiveTime;

public:
	_XFlag();
	virtual ~_XFlag();

	void						Create(unsigned short id, char type, D3DXVECTOR3 position);
	
	BOOL						CheckInsideFrustum(void);

	void						Render(void);
	void						Process(void);

	void						UpdatePosition(void);

};

class _XFlagList : public _XList
{

public :
	_XFlagList();
	~_XFlagList();

	BOOL						Add(_XFlag* flag);
	_XFlag*						Find(unsigned short flagid);
	void						Delete(unsigned short flagid);
	void						Delete(_XFlag* flag);

	void						Process(void);
	void						Render(void);

	void						CheckInsideFrustum(void);
	void						RemoveEffectInstance(int templateid);
};

#endif // !defined(AFX_XFLAG_H__2A7DF9EC_5EDA_468D_8EC1_DD4CA978A931__INCLUDED_)
