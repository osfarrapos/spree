// XBattleLog.h: interface for the _XBattleLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XBATTLELOG_H__94FAC4E8_8522_4086_B63E_1713C0E20C1B__INCLUDED_)
#define AFX_XBATTLELOG_H__94FAC4E8_8522_4086_B63E_1713C0E20C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

typedef struct _BATTLELOGSTRUCT
{
	BOOL			bWrite;

	SYSTEMTIME		systemtime;
	int				targetgroup;
	int				targettype;

	int				skillid;
	int				skilllevel;
	int				attackseq;

	int				atkinfo;
	int				damage[3];

	int				effid[8];
	int				effvalue[8];
	int				efftime[8];

	int				addeffid[4];
	int				addeffvalue[4];
	int				addefftime[4];

	TCHAR			targetname[256];

}_XBATTLELOGSTRUCT;

class _XBattleLog  
{
private :
	TCHAR	m_FileName[512];
	FILE*	m_pBattleLogFile;

	_XBATTLELOGSTRUCT		m_BattleLog;
	
	BOOL	m_bWriteLog;

public:
	_XBattleLog();
	virtual ~_XBattleLog();

	BOOL			Initialize(TCHAR* charactername);
	void			Release(void);

	void			Print(char* argv, ...);
	void			Flush(void);

	void			SetTime(void);
	void			SetTargetGroup(int targetgroup);
	void			SetTargetType(int targettype);
	void			SetSkillId(int skillid, int skilllevel = 0);
	void			SetAttackSeq(int seq);

	void			SetAtkInfo(int atkinfo);
	void			SetDamage(int damage_1, int damage_2 = 0, int damage_3 = 0);
	
	void			SetEffect(int id, int level);

	void			DumpLogResult(void);
	void			DumpLogEffect(void);

	void			SetWriteLog(BOOL bWrite)	{	m_bWriteLog = bWrite;	};
	BOOL			GetWriteLog(void)			{	return m_bWriteLog;		};

};

#endif // !defined(AFX_XBATTLELOG_H__94FAC4E8_8522_4086_B63E_1713C0E20C1B__INCLUDED_)
