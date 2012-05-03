#ifndef _XPROC_MAINGAMEDEFINE_
#define _XPROC_MAINGAMEDEFINE_

#ifdef _XTESTSERVER	
	#define _XDEF_TEXTUREFILE_A						"tsTR_ENVIRONMENT.XP"
#else
	#define _XDEF_TEXTUREFILE_A						"TR_ENVIRONMENT.XP"
#endif

#define _XDEF_DEFAULT_GAME_BGM					"Data\\Sound\\Bgm\\field.mp3"

#define _XDEF_MOVE_SPEED						0.01f
#define _XDEF_LOOK_SPEED						0.01f
#define _XDEF_RAPID_RATE						5.0f

#define _XDEF_MINIMUMWALKDISTANCE				0.01f
//#define _XDEF_MINIMUMRUNDISTANCE				0.25f
#define _XDEF_MINIMUMRUNDISTANCE				0.8f	// Not fix
#define _XDEF_DISTANCEOFSTARTNPCSCRIPT			3.5f

// Rendering radius
#define _XDEF_USER_CULLINGRADIUS				30.0f
#define _XDEF_MOB_CULLINGRADIUS					30.0f
#define _XDEF_NPC_CULLINGRADIUS					60.0f
#define _XDEF_EFFECT_CULLINGRADIUS				12.0f

//////////////////////////////////////////////////////////////////
// Game timer
#define XTIMER_SENDMOVEMESSAGE					0
#define XTIMER_SENDMOVEMESSAGETIME				0.5f

#define XTIMER_FRAMESKIP						1

#define XTIMER_USERPROCESS						2
#define XTIMER_USERPROCESSTIME					1.0f

#define _XDEF_KEYDOWNTICK						700

//////////////////////////////////////////////////////////////////
// Object < Character, Mob, NPC > define

//////////////////////////////////////////////////////////////////
// Interface macro & pre.. define

#define _XDEF_MAXCHATLENGTH						256

#define _XDEF_CHATLIST_QUESIZE					150
#define _XDEF_CHATLIST_STARTX					13
//#define _XDEF_CHATLIST_STARTY					606
#define _XDEF_CHATLIST_MAXVIEWLINE				11

#define _XDEF_CHATLIST_WHEELAREALEFT			0
#define _XDEF_CHATLIST_WHEELAREATOP				602
#define _XDEF_CHATLIST_WHEELAREARIGHT			500
#define _XDEF_CHATLIST_WHEELAREABOTTOM			767

#define _XDEF_SLIDEWINDOW_RIGHT_GAB				10
#define _XDEF_SLIDEWINDOWMOVERATE				50
#define _XDEF_QUICKSLOTSLIDEMOVERATE			8
#define _XDEF_MINIMAPSLIDEMOVERATE				18

#define _XDEF_NOTIFYMESSAGEVIEWTIME				20000

#define _XDEF_NORMALNOTIFYMESSAGEVIEWTIME		5000


#ifdef _XDWDEBUG

#define _XDEF_PROCESSTOP_DELAYTIME				1000
#define _XDEF_GAMEEXIT_DELAYTIME				1000

#else

#ifndef _XMANAGER

#define _XDEF_PROCESSTOP_DELAYTIME				8000
#define _XDEF_GAMEEXIT_DELAYTIME				5000

#endif

#endif

#ifndef _XDWDEBUG

#ifdef _XMANAGER

#define _XDEF_PROCESSTOP_DELAYTIME				2000
#define _XDEF_GAMEEXIT_DELAYTIME				2000

#else

#define _XDEF_PROCESSTOP_DELAYTIME				8000
#define _XDEF_GAMEEXIT_DELAYTIME				5000

#endif

#endif


typedef enum
{
	_XSLIDE_STOP = 0,
	_XSLIDE_IN,
	_XSLIDE_OUT
} _XWINDOW_SLIDEMODE;

typedef enum _XMOUSETYPE
{
	_XMOUSETYPE_PEACE	= 0,
	_XMOUSETYPE_PEACE_CLICK,
	_XMOUSETYPE_BATTLE,	
	_XMOUSETYPE_BATTLE_CLICK,	
	
	_XMOUSETYPE_SELECT,
	_XMOUSETYPE_SELECT_CLICK,
	
	_XMOUSETYPE_PICKUP,
	_XMOUSETYPE_PICKUP_CLICK,
	
	_XMOUSETYPE_RESIZE_DIAGONAL_LEFT,
	_XMOUSETYPE_RESIZE_DIAGONAL_RIGHT,
	_XMOUSETYPE_RESIZE_HORIZ,
	_XMOUSETYPE_RESIZE_VERT,
	
	_XMOUSETYPE_SANDGLASS1,
	_XMOUSETYPE_SANDGLASS2,
	_XMOUSETYPE_SANDGLASS3,
	_XMOUSETYPE_SANDGLASS4,
	_XMOUSETYPE_SANDGLASS5,
	_XMOUSETYPE_SANDGLASS6,
	
	_XMOUSETYPE_SCRSCROLL_UPLEFT,
	_XMOUSETYPE_SCRSCROLL_UP,
	_XMOUSETYPE_SCRSCROLL_UPRIGHT,
	_XMOUSETYPE_SCRSCROLL_RIGHT,
	_XMOUSETYPE_SCRSCROLL_DOWNRIGHT,
	_XMOUSETYPE_SCRSCROLL_DOWN,
	_XMOUSETYPE_SCRSCROLL_DOWNLEFT,
	_XMOUSETYPE_SCRSCROLL_LEFT,

	_XMOUSETYPE_CROSSPOINTER,
	
	_XMOUSETYPE_CHARACTORSELECT,
	_XMOUSETYPE_GOTOGATE,
	_XMOUSETYPE_NOWAY,
	_XMOUSETYPE_CONVERSATION,	

	_XMOUSETYPE_MOBOVER,
	_XMOUSETYPE_MOBOVER_CLICK,

	_XMOUSETYPE_LPSOVER,
	_XMOUSETYPE_LPSOVER_CLICK,

	_XMOUSETYPE_CHATRESIZE,
	_XMOUSETYPE_CHATRESIZE_CLICK,	

	_XMOUSETYPE_STATUS,
	_XMOUSETYPE_STATUS_CLICK,	
	
	_XMOUSETYPE_PERSONALSTORE,
	_XMOUSETYPE_PERSONALSTORE_CLICK,	

	_XMOUSETYPE_REBIRTH,
	_XMOUSETYPE_REBIRTH_CLICK,	

	_XMOUSETYPE_TOTALCOUNT
};

//////////////////////////////////////////////////////////////////
// Performance check macros...
//#ifdef _XDWDEBUG
#define _XDEF_PERFORMANCE_CHECK
//#endif

#ifdef  _XDEF_PERFORMANCE_CHECK

#define _PERFORMANCECHECKSTART   LARGE_INTEGER frequency, count1, count2;									\
                                 if (!QueryPerformanceFrequency(&frequency))								\
	                                  MessageBox( NULL, "No performancefreq", "Error !", MB_OK );			\
							     QueryPerformanceCounter(&count1);

#define _PERFORMANCECHECKEND	 QueryPerformanceCounter(&count2);											\
								 double timeinmicroseconds =												\
								 (double)( (double)((count2.QuadPart - count1.QuadPart) * 1000 * 1000)		\
											/ (double)frequency.QuadPart );								 
#define _PERFORMANCEDISPLAY_MOUSEPICKING  _XDWINPRINT( "Mouse picking time : %f second", timeinmicroseconds/1000000 );
#define _PERFORMANCEDISPLAY_PATHFINDING   _XDWINPRINT( "Path finding  time : %f second", timeinmicroseconds/1000000 );
#define _PERFORMANCEDISPLAY_RENDERTERRAIN g_XBaseFont->Print( gnWidth-320, gnHeight-38, 1.0f, "Terrain render time : %fsec face [%d] FPS: %d\n", timeinmicroseconds/1000000, m_FacesDrawn, gnFPS ); g_XBaseFont->Flush();

#else   

#define _PERFORMANCECHECKSTART
#define _PERFORMANCECHECKEND
#define _PERFORMANCEDISPLAY_MOUSEPICKING
#define _PERFORMANCEDISPLAY_PATHFINDING
#define _PERFORMANCEDISPLAY_RENDERTERRAIN
#endif


// Virtual key define..
#define VK_PAGE_UP   0x21
#define VK_PAGE_DOWN 0x22

#define VK_0    0x30
#define VK_1    0x31
#define VK_2    0x32
#define VK_3    0x33
#define VK_4    0x34
#define VK_5    0x35
#define VK_6    0x36
#define VK_7    0x37
#define VK_8    0x38
#define VK_9    0x39


#define VK_A    0x41
#define VK_B    0x42
#define VK_C    0x43
#define VK_D    0x44
#define VK_E    0x45
#define VK_F    0x46
#define VK_G    0x47
#define VK_H    0x48
#define VK_I    0x49
#define VK_J    0x4A
#define VK_K    0x4B
#define VK_L    0x4C
#define VK_M    0x4D
#define VK_N    0x4E
#define VK_O    0x4F
#define VK_P    0x50
#define VK_Q    0x51
#define VK_R    0x52
#define VK_S    0x53
#define VK_T    0x54
#define VK_U    0x55
#define VK_V    0x56
#define VK_W    0x57
#define VK_X    0x58
#define VK_Y    0x59
#define VK_Z    0x5A


#endif
