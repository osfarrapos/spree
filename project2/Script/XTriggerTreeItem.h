// XTriggerTreeItem.h: interface for the _XTriggerTreeItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTRIGGERTREEITEM_H__A8C5605A_E853_4AB0_A49B_B48DB9079B55__INCLUDED_)
#define AFX_XTRIGGERTREEITEM_H__A8C5605A_E853_4AB0_A49B_B48DB9079B55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTriggerTreeItem  
{
public:
	
	int			m_nOwnerQuestID;
	
	int			m_TreeNodeNumber;
	int			m_ParentsNodeUniqueID;		
	int			m_SelfNodeUniqueID;
	int			m_FLeftNodeUniqueID;
	int			m_TRightNodeUniqueID;
	
	int			m_tempParentsNodeUniqueID;		
	int			m_tempSelfNodeUniqueID;
	int			m_tempFLeftNodeUniqueID;
	int			m_tempTRightNodeUniquID;
	
	int			m_TriggerUniqueID;
	
	BOOL		m_bLeftNode;
	BOOL		m_bQuestBackAtCurrentNode;
	BOOL		m_bPartyResultToOneself;
	bool		m_bAutoNode;

public:
	_XTriggerTreeItem();
	virtual ~_XTriggerTreeItem();

	BOOL		Load(FILE* fileptr);
	BOOL		LoadString(FILE* fileptr, LPTSTR& string);
	
	void		SetOwnerQuestID(int questid);
	int			GetOwnerQuestID(void);
	
};

#endif // !defined(AFX_XTRIGGERTREEITEM_H__A8C5605A_E853_4AB0_A49B_B48DB9079B55__INCLUDED_)
