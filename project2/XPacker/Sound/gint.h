#ifndef GINTHEADER
#define GINTHEADER 
#include "conv.h" 
#include "cplayer.h"
#include "collfun.h"

extern BOOL IsIn3d;
////////Geometric Utilities////////////////////

////////////////////////////////////////////FRAMES AND ANIMATION HANDLE/////////
#define MAX_RFRAMES 100 
#define MAX_RANIMATIONS 300  
#define MAX_RTANIMATIONS 100
extern MYVECTOR tempvect;
extern LPMYFRAME RFrames[MAX_RFRAMES];
extern LPMYANIMATION RAnimations[MAX_RANIMATIONS];
extern LPMYTANIMATION RTAnimations[MAX_RTANIMATIONS];
extern int NRFrames, NRAnimations, NRTAnimations;

/////////////////////STRUTTURE DATI PER FUNZIONI PAINT GENERAL PURPOSE///////////////
typedef struct _TEXTURE_FRAME{
	PREAL AnimationTime;
	unsigned char Animation;
} TEXTURE_FRAME;

typedef struct _ANIMATION_FRAME{
	PREAL AnimationTime;
	unsigned char Animation;
	unsigned char FrameHandle;
} ANIMATION_FRAME;

///////////////////////////ADDITIONNAL INFORMATION PASSED TO THE PAINTPLAYER FUNCTION BY HANIM.C
extern PREAL *CurrPrevX;
extern PREAL *CurrPrevZ;
extern int CurrPlayer;

//SCENE MANAGEMENT////////////////////////
void StartPrintScene(void);
void FinishPrintScene(void);
void CloseSubScene(void);
void CloseScene(void);
void CloseSceneDisplay(void);
void InitScene(void);
void InitSceneDisplay(void);
void PaintScene(void);//paint backgroung environment////////

///////////////INTERFACE INITIALIZATION AND CLOSURE//////////////////////////////////////////////
void InitGInt(void);
void CloseGInt(void);
void GoTo3D(void);//NO_PLAYSTATION(partial initialization when entering 3d)
void ReleaseCurrScene(void);//NO_PLAYSTATION (partial closure when exiting 3d)

//////////////////CARICAMENTO DIFFERENZIATO ANIMAZIONI NO_PLAYSTATION////////////
extern int NPendingMBlare;
void PReleaseCurrScene(void); //Textures in sospeso da caricare
void TagAnimations(void);
void ResetAnimationsTag(void);

///////////////////MODELS AND ANIMATION LOADING//////////////
void LoadScene(char *Name); //load background scene
void HLoadAnimation(char *Name); //scene animations
void HLoadTextureAnimation(char *Name); //scene texture animations
int LoadAnimationHandle(char *Name); //player loading
int LoadTAnimationHandle(char *Name); //not used
int FindFrameHandle(char *);
int CreateAlias(int FrameHandle, int Index); //utilizzate per animazioni ripetute
//Properties of animations
PREAL CAnimationTime(int x);

BOOL IsCiclicAnimation(int x);
char AnimationCType(int x);
void DeclareNotPaint(int x);

/////////////////////////MODELS HANDLING///////////////////////
void UpdatePlayerPosition(int i); // aggiorna matrice del giocatore con posizione in PlayerPosition 
void SetFramePosition(int i, MYVECTOR v); //Setta posizione per animazioni la cui posizione 
void EnablePlayerMBlare(int i, BOOL Enabled); //enable player motion-blurr.

/////////////////////////3D SOUNDS HANDLING///////////////////////////
void GetPosForSound(char *Name, PREAL **Pos);
void SetListernerFromCamera(void);

//////////////////////////ANIMATIONS AND MODELS INFORMATION/////////
void StartAnimationSound(int Handle);
void StartTAnimationSound(int Handle);
PREAL AnimationTime(int x);
PREAL TAnimationTime(int x);

/////////////////Player models main matrices informations
PREAL *GetFirstAnimationMatrix(int Anim,PREAL Time);//Not Used
PREAL *GetMatrix(int Handle);
PREAL *GetTotMatrix(int Handle);
PREAL GetLastFrameParams(MYVECTOR Transl, int Handle);

////////////////////////Reference Object informations
PREAL *AnimationRefPos(int Animations, PREAL Time, PREAL *InputVect);
PREAL *GetTotRef(int Animations	);
PREAL *GetTotRefPos(int Animations);
PREAL *GetGenericTotRefPos(int Animations, PREAL Time);
ANGLE GetTotRefAngle(int hAnim);
PREAL GetIntTheta(void *cmodel);
PREAL GetRelevantTime(int Animations);
PREAL GetRelevantTime2(int Animations);

/////////////CAMERA HANDLING (CControl e FCamera)///////////////
void GetCameraMatrix(PREAL *MM, MYVECTOR Position, 
					 PREAL *xc, PREAL *yc, PREAL *zc);
void GetCameraLens(PREAL *x, PREAL *y, PREAL *H);
void SetCameraLens(D3DVALUE FrontClipping,
				   D3DVALUE BackClipping,
				   D3DVALUE Height);
void SetCamera (D3DVECTOR *Position,
				D3DVALUE xc, D3DVALUE yc, D3DVALUE zc,
                D3DVECTOR *LookAt);
PREAL *SetCameraAngles (PREAL EulerX, PREAL EulerY);
void SetCameraPosition(D3DVECTOR *Position,
					   D3DVALUE xc, D3DVALUE yc, D3DVALUE zc);
void SetCameraMatrix(PREAL *M);

/* CHOEUNS SHOULD FIX */
// Key/Joy input direction conversion from screen to world //
#define AngleFromScreenDirection(dirX, dirY) \
		(ConvertScreenDirection(dirX,dirY, tempvect), GETANGLE(tempvect[2], tempvect[0]))
void ConvertScreenDirection(PREAL dirX, PREAL dirY, MYVECTOR Out);
/* CHOEUNS SHOULD FIX TILL HERE */

//////////////////////Only For 3d Sounds/////////////////////////
PREAL *GetCameraPos(void);
PREAL *GetCameraLookAt(void);
PREAL *GetCameraTop(void);

/////////BALL COLLISIONS////////////////
extern int BackSceneHandle; ///Framehandle of the whole scene background
BOOL MObjectsVerifyCollision (PREAL Displ, D3DMATRIX *Camera, D3DMATRIX *Verticess, 
							  D3DMATRIX *Normals, unsigned char *property, void **Coll);
void EnableCollisionList(BOOL x, int FrameHandle); 
int HighPlayerBallCollision(int i, PREAL *Vertices, PREAL *Normals,
							PREAL *Transform);

/////////////SCENETTE/////////////////////////////////////////
#define MAX_ACTIONS 10 //max actions associated to collisions
typedef struct _ACTION_COLLISION{
	void *CollOb;
	void(*CollFun)(void);
} ACTION_COLLISION;

extern ACTION_COLLISION TabColl[MAX_ACTIONS];
extern int NCollFun;

////////collision functions/////////
void InitCollision0(void);
void InitCollision1(void);
void InitCollision2(void);
void InitCollision3(void);
void InitCollision4(void);
void InitCollision5(void);

#endif