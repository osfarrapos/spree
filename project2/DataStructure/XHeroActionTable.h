#ifndef		_XHEROACTIONTABLE_DEFINE
#define		_XHEROACTIONTABLE_DEFINE

///////////////////////////////////////////////////////////////////////
// Character action number...

typedef enum _XHEROCHARACTER_MOTION
{
	_XHAN_IDLE_NONE		=	0,				
	_XHAN_STANCE,	
	_XHAN_BATTLEIDLE_NONE,	
	_XHAN_MOVE,
	_XHAN_ATTACK1,
	_XHAN_ATTACK2,
	_XHAN_ATTACK3,
	_XHAN_ATTACK4,
	_XHAN_BLOW,
	_XHAN_DIE,
	_XHAN_SPECIAL1,
	_XHAN_SPECIAL2,
	_XHAN_TALK,

	_XHAN_MAXCOUNT
}_XHEROCHARACTER_MOTION;

#endif