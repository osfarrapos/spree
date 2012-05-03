#ifndef _XWINDOWOBJECTDEFINE

#include "XWindow_MainFrame.h"
#include "XWindow_CInfo.h"
#include "XWindow_QuestWindow.h"
#include "XWindow_MasterySkill.h"
#include "XWindow_LifeSkill.h"
#include "XWindow_QuickSlot.h"
#include "XWindow_Skill.h"
#include "XWindow_DefaultTooltip.h"
#include "XWindow_MiniTooltip.h"
#include "Xwindow_State.h"
#include "XWindow_Group.h"
#include "XWindow_AreaSelect.h"
#include "Xwindow_PKRecord.h"
#include "XWindow_Mark.h"
#include "XWindow_JinStatus.h"
#include "XWindow_Exp.h"
#include "XWindow_RewardNick.h"
#include "XWindow_CashMall.h"
#include "XWindow_WorldMap.h"

extern _XWindow_MainFrame*		g_pMainFrame_Window;
extern _XWindow_QuestWindow*	g_pQuest_Window;
extern _XWindow_CInfo*			g_pCInfo_Window;
extern _XWindow_MasterySkill*	g_pMasterySkill_Window;
extern _XWindow_Inventory*		g_pInventory_Window;
extern _XWindow_QuickSlot*		g_pQuickSlot_Window;
extern _XWindow_Skill*			g_pSkill_Window;
extern _XWindow_LifeSkill*		g_pLifeSkill_Window;
extern _XWindow_DefaultTooltip* g_pDefaultTooltip_Window;
extern _XWindow_MiniTooltip*	g_pMiniTooltip_Window;
extern _XWindow_State*			g_pState_Window;
extern _XWindow_Group*			g_pGroup_Window;
extern _XWindow_AreaSelect*		g_pBlackClanAreaSelect_Window;
extern _XWindow_AreaSelect*		g_pWhiteClanAreaSelect_Window;
extern _XWindow_AreaSelect*		g_pMultiAreaSelect_Window;
extern _XWindow_PKRecord*		g_pPKRecord_Window;
extern _XWindow_Mark*			g_pMark_Window;
extern _XWindow_WorldMinimap*	g_pMinimap_Window;
extern _XWindow_JinStatus*		g_pJinStatus_Window;
extern _XWindow_Exp*			g_pExp_Window;
extern _XWindow_RewardNick*		g_pRewardNick_Window;
extern XWindow_WorldMap*		g_pWorldMap_Window;

#ifdef _XTS_ITEMMALLBROWSER
extern _XWindow_CashMall*		g_pCashMall_Window;
#endif
#endif