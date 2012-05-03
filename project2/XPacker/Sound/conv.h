#ifndef CONVHEADER
#define CONVHEADER
#include "d3dmacs.h" 

#define PGE_DX_NOHANDLE ((DWORD) 0)
//Matrix ops
#define PGE_MAKE_IDENTITY_D3DMATRIX(m)\
		(memset(&(m),0, sizeof(D3DMATRIX)),	(m)._11= (m)._22= (m)._33= (m)._44=	PVALUE(1.))
///D3DRM user data flags////////
#define FRAME_COMPILE 1   //do compile the herarchy rooted in this frame

////////////////////////////////

////MYOBJ and MYFRAME Flags
#define CLONE 1          //this is a clone
#define TRANSPARENTOBJ 2    //this object is transparent
#define COLORKEY 4
#define PICKENABLE 8
#define SCREENLAYER 16
#define COLLISIONNEEDED 32
#define	ALWAYSINTO 64
#define SPRITE3D 128
#define SPRITE3D1 256
#define ISSHADOW 512
#define EMISSIVE 1024
#define NOT_PAINT 2048
#define USER_DEFINED_PAINT 4096
#define IS_RELEVANT_FRAME 8192
#define BACK_TRANSPARENT (8192*2)
#define BACK_M_TRANSPARENT  (BACK_TRANSPARENT*2)
#define HAS_BEEN_CLIPPED (BACK_M_TRANSPARENT*2)
#define DO_NOT_CUT (HAS_BEEN_CLIPPED*2)
#define WHOLE_CUT (DO_NOT_CUT*2)
#define FAKE_SHADOW (WHOLE_CUT*2)
#define NO_D_LIGHT (FAKE_SHADOW*2)
#define ADD_TO_COLLISION_LIST (NO_D_LIGHT*2)
#define ONLY_FOR_COLLISIONS (ADD_TO_COLLISION_LIST*2)
#define ACTIVE_OUT_OF_VIDEO (2*ONLY_FOR_COLLISIONS)
#define MBLARE_ON (2*ACTIVE_OUT_OF_VIDEO)
#define IS3DLAYER (2*MBLARE_ON)
#define DECLARE_NOT_PAINT (2*IS3DLAYER)

#define ALPHAREF 6  
///////////////////////////////////////////////////////////
//LIMITS
//TD 20-09-00 Valori cambiati
#define MAX_VISUALS 1000//640//512//400//300
#define MAX_FRAMES	MAX_VISUALS//640//512//400//300
#define MAX_BACK		1000//100
#define MAX_M_BACK	1000//100
/////////////////////////

#define MAX_SCREEN_VISUALS 50

#define MAX_ANIMATIONS	300
#define MAX_TANIMATIONS 300
#define MAX_GOUPS				500

#define MAX_OBJS 100
#define MAXSUBPARTS 100
#define VxPF 5     //maximum vertexs per face

#define MAX_NORMALS		3000
#define MAX_VERTEXES	3000
#define MAX_FACES			3000

/////////////////////////////
//tollerances
#define VALUETOL .1e-07
#define ZEPSILON 0.0002	//z displacement to allow info display before scene
/////////////////////////////////
//Name notation
#define DUMMYFRAMEPOS0 0
#define DUMMYFRAMECHAR0 '_'
#define DUMMYFRAMEPOS1 1
#define DUMMYFRAMECHAR1 '0'
#define SPRITECHAR 'S' 
#define SPRITECHAR0 '1'
#define SPRITECHAR1 '0'
#define EMCHAR0 'E'
#define EMCHAR1 '0'
#define AEMCHAR0 'E'
#define AEMCHAR1 '1'
#define NOPICK0 'B'
#define NOPICK1 '0'
#define AFRAMECHAR0 'A'
#define AFRAMECHAR1 'C'
#define AFRAMECHAR2 '1'
#define BACKCHAR0 '<'
#define COLLCHAR0 '@'
#define COLLCHAR1 '@'

//////////////////////
//BBox macro
#define MIN_VERTICES_BBOX 30
typedef enum _BOXCOMPARE{
    ALL_VISIBLE=1,
    NOT_VISIBLE=2,
    PARTIALLY_VISIBLE=3,
} BOXCOMPARE;

/////////////////////////
//Main structs
typedef struct _TEXTURE_INSTANCE{
    LPDIRECT3DTEXTURE2 Variable;
    LPDIRECT3DTEXTURE2 Value;
}TEXTURE_INSTANCE, *LPTEXTURE_INSTANCE;

typedef struct _CENTERTABLE{
	int N;
	char *Name[MAXSUBPARTS];
	D3DVECTOR Center[MAXSUBPARTS];
} CENTERTABLE;

typedef struct _RD3MATRIX{
	float _11; float _12; float _13;
	float _21; float _22; float _23;
	float _31; float _32; float _33;
	float _41; float _42; float _43;
} RD3DMATRIX, *LPRD3DMATRIX;

typedef struct _MYOBJ{
	void (*BeforeRender)(struct _MYOBJ *);
	void (*AfterRender)(struct _MYOBJ *);
	int Flags;
	char *Name;
	RD3DMATRIX *init;
	LPDIRECT3DEXECUTEBUFFER lpExecuteBuffer;
	D3DEXECUTEDATA ExecuteData;
	int NMatrices;
	LPD3DMATRIXHANDLE Matrices;
	char **FrameNames;
	DWORD *Vertices;
	int ONMatrices;
	LPD3DMATRIXHANDLE OMatrices;
	BOOL ValidBox;
	D3DRMBOX BoundingBox;
	D3DRMBOX WBoundingBox;
	int RefCounter;
	struct _MYFRAME *Father;
	struct _MYOBJ *Next;
	struct _MYTANIMATION *TAnimations;
	LPTEXTURE_INSTANCE VariableTextures;
	int NVariableTextures;
	struct _MBLARESTATE *MBlare;
	int NTextures;
	int *Triangles;
	unsigned char *Textures;
	void * UserData;
} MYOBJ, *LPMYOBJ;

typedef struct _MYFRAME{
	int Flags; 
	char *Name;
	D3DMATRIX Matrix;
	D3DMATRIX GMatrix;
	LPMYOBJ ObList;
	struct _MYFRAME *Father;
	struct _MYFRAME *FrameList;
	struct _MYFRAME *Next;
	int RefCounter;
}MYFRAME, *LPMYFRAME;

////////////////////////////////////
//Animation structs
typedef struct _MILESTONE{
	int Frame;
	int Position;
} MILESTONE, *LPMILESTONE;

typedef struct _SOUNDKEY{
	int Frame;
} SOUNDKEY, *LPSOUNDKEY;
//CType values 
typedef void (*POUT_VIDEO_FUN)(PREAL , PREAL, BOOL);


#include "splines.h"
#define IS_SPLINE 1
#define AN_NO_CONTROL 0
#define AN_NORMAL_CONTROL 1
#define AN_TRAJECTORY_CONTROL 2
extern PREAL PercAnim, NextX, NextZ;

typedef struct _MYANIMATION{
	unsigned int Flags;
	BOOL Ciclic;
	int CType;
	D3DCOLOR Color;
	D3DTEXTUREHANDLE Texture;
	D3DMATRIXHANDLE Joint;
	int NMBlare;
	D3DVALUE MBlareInit;  //it is M Blare init time
	D3DVALUE MBlareStop;
	D3DVALUE InitKey;
	D3DVALUE IncKey;
	int NFrames;
	int RelevantFrame;
	int RelevantFrame2;
	int NMatrices;
	union {
		LPMILESTONE MileStones;
		LPSPLINE Splines;
	};
	int PMileStone;
	int NMileStones;
	union {
		RD3DMATRIX *Data;
		D3DMATRIX *InitMatrix;
	};
	RD3DMATRIX *Ref;
	D3DRMBOX *BBoxes;    
	float StripWidth;
	MYVECTOR Displ;
	int SoundHandle;
}MYANIMATION, *LPMYANIMATION;


#define CLONED_TANIM 1
typedef struct _MYTANIMATION{
	int Flags;
	int NFrames;
	LPDIRECT3DTEXTURE2 *Frames;
	LPMYOBJ Target;
	int AFrame;
	PREAL DeltaT;
	int FileSoundHandle;
	LPSOUNDKEY lpSounds;
	int NSounds;
	int LastSoundKey;
	LPMILESTONE MileStones;
	int PMileStone;
	int NMileStones;
	struct _MYTANIMATION *Next;
	int SoundHandle;
}MYTANIMATION, *LPMYTANIMATION;



#include "mblare.h"


extern LPMYANIMATION AllAnimations[MAX_ANIMATIONS];
extern int NAllAnimations;
extern int AnimationTag;

extern BOOL MYEmissive;
extern BOOL MYAEmissive;
extern BOOL FMYAEmissive;
#define SameFeatures(x, y) (x.Texture==y.Texture)
#define SameNormals(a, b)((fabs(a.x-b.x)<VALUETOL)&&\
                        (fabs(a.y-b.y)<VALUETOL)&&\
                        (fabs(a.z-b.z)<VALUETOL))
#define SameFloat(a, b) (fabs(a-b)<VALUETOL)

///////////initialization////////
void InitD3DPatch(void);
void ReleaseCommandBuffer(void);

void PGEReleaseScene(int animTag);
#define ReleaseScene()	PGEReleaseScene(0)
#define PReleaseScene()	PGEReleaseScene(AnimationTag)

void Configuration_3d(void);
void Release3d(void);
//////////////////////

//////////utilities//////////////////
D3DMATRIXHANDLE CreateMatrix(void);
void FindTransform(LPDIRECT3DRMFRAME lpReference,
              LPDIRECT3DRMFRAME2 lpFrame,
              D3DMATRIX *lpMatrix, D3DMATRIX *lpMatrixf);
/////////////////////////

///////////////Frames and Objects Manipulation///////////
//void ComputeObjectCenter(LPMYOBJ lpOb, MYVECTOR v);
void AddFrameToFrame(LPMYFRAME Obj, LPMYFRAME Frame);
void AddObjToFrame(LPMYOBJ Obj, LPMYFRAME Frame);
LPMYFRAME GetObjFather(LPMYOBJ Obj);
LPMYFRAME GetFrameFather(LPMYOBJ Frame);
int RemoveFromFrameTable(LPMYFRAME Obj);
int RemoveFromObjTable(LPMYOBJ Obj);
void ReleaseAllFrames(void);
void ReleaseAllObjs(void);
int ReleaseFrame(LPMYFRAME Obj);
int ReleaseObj(LPMYOBJ Obj);
int DeleteFrameFromFrame(LPMYFRAME Obj, LPMYFRAME Frame);
int DeleteObjFromFrame(LPMYOBJ Obj, LPMYFRAME Frame);
int ReleaseFrame(LPMYFRAME Obj);
int ReleaseObj(LPMYOBJ Obj);
LPMYOBJ NewObj(char *szName);
LPMYOBJ CloneObj(LPMYOBJ Ob, char *szName);
LPMYOBJ FindObj(char *szName);
LPMYFRAME NewFrame(char *szName);
LPMYFRAME FindFrame(char *szName);
D3DMATRIXHANDLE FindJoint(char *szName, LPMYOBJ lpOb);
void SetAfterRender(LPMYOBJ lpOb, void(*x)(LPMYOBJ));
void SetBeforeRender(LPMYOBJ lpOb, void(*x)(LPMYOBJ));
void SetMBlareObj (LPMYOBJ lpOb);
BOXCOMPARE CheckBoundingBox(LPMYOBJ Ob, D3DVALUE *t);
void ChangeTexture(LPMYOBJ lpOb, int Ntext, char **Old, NEWNAME *New);
void UpdateTexture(LPMYOBJ lpOb, char *Old, char *New);
/////////////////////////////////////////////

/////////loading and translation form Ret Mode///////////////
void TranslateFrameHerarchy(LPMYOBJ Ob, LPDIRECT3DRMFRAME2 lpFrame );
void TranslateMeshBuilder(LPMYOBJ Ob, LPDIRECT3DRMMESHBUILDER2 lpMB);
void InitLoad(void);
void CloseLoad(void);
LPMYOBJ LoadObjFromMesh(LPDIRECT3DRMMESHBUILDER2 lpMB);
LPMYOBJ LoadObjFromFrame(LPDIRECT3DRMFRAME2 lpFrame, char *Name);
LPMYFRAME LoadFrameHerarchy(LPDIRECT3DRMFRAME2 lpFrame);
LPMYOBJ FileLoadObj(char *filename);
LPMYOBJ FileLoadComplexObj(char *filename, char *Name);
LPMYOBJ FileLoadComplexShadow(char *filename, char *Name, LPMYOBJ Ref);
LPMYOBJ FileLoadComplexCollidingObj(char *filename, char *Name, LPMYOBJ Ref);
void FindCenters(CENTERTABLE *T);
void StopFindCenters(void);
LPMYFRAME FileLoadFrameHerarchy(char *filename);
void TagForHerarchyCompile(LPDIRECT3DRM2 lpD3DRM2, char *Name);
HRESULT CreateTexture(D3DTEXTUREHANDLE *lpHandle,
               LPDIRECT3DTEXTURE2 *lpComObj, LPDIRECTDRAWSURFACE *lpSurf, char *Name);
/////////Animations/////////////////////////////////////
void LoadAnimation(LPMYANIMATION An, char *filename);
void ConvertAnimation(char *outfile, BOOL app, LPDIRECT3DRMFRAME2 Fr,
                LPDIRECT3DRMANIMATIONSET An, D3DVALUE Firstkey,
                D3DVALUE Inckey, int N, PREAL *Trans, BOOL Normalize);
void FileConvertAnimation(char *filename, char *outfilename,
				BOOL app, 
                int Mult, PREAL *Trans, BOOL Normalize);
void AttachAnimation(LPMYANIMATION lpAn, LPMYFRAME lpOb, D3DVALUE key);
PREAL *GetAnimationRef(LPMYANIMATION lpAn, PREAL Time);
LPMYANIMATION NewAnimation(void);
void RemoveAnimation(LPMYANIMATION Obj);
//void ReleaseAllAnimations(void);
void ReleaseAllAnimations(int x);
void LoadRegisteredMBlareTextures(void);
void MBlareOnOff(LPMYOBJ lOb, BOOL Enabled);
///texture animations///
LPMYTANIMATION NewTAnimation(void);
LPMYTANIMATION LoadTAnimation(char *Name);
LPMYTANIMATION LoadSTAnimation(char *Name, int N);
void SetTAnimationFrame(LPMYTANIMATION lpAn, int N);
LPMYTANIMATION CloneTAnimation(LPMYTANIMATION x);
void AttachTAnimation(LPMYTANIMATION TAn, LPMYOBJ Ob);
void SetTAnimationTime(LPMYTANIMATION lpTAn, PREAL x);
LPMYTANIMATION CloneTAnimation(LPMYTANIMATION x);
void ReleaseAllTAnimations(void);

//////Rendering//////////////////////////////
void InitZBuffers(void);
void AddObjToZbuffer(LPMYOBJ lpOb, LPD3DMATRIX lpM);
void AddFrameToZbuffer(LPMYFRAME lpFr);
void AddFrameToZbufferAfter(LPMYFRAME lpFr);
void POPZBuffer();
void OrderZBuffers(void);
void RenderZBuffers(void);
/////////////////////////////////////////////////////

///////////////Internal camera matrix//////////
void GetCameraXZMatrix(PREAL *MM);//riportare in conv
/////////////////////////////////////////////////

///////////////lights/////////////////////////////////
void CreateDirectionalLight(D3DVALUE R, D3DVALUE G, D3DVALUE B,
                             D3DVALUE A,  //0-1 values
                             D3DVALUE x, D3DVALUE y, D3DVALUE z,
                             LPDIRECT3DLIGHT *lplpD3DLight);
                             
void SetAmbientLight(int R, int G, int B, int A);
                //0-255 values
void SetDirectionalLight(D3DVALUE R, D3DVALUE G, D3DVALUE B,
                             D3DVALUE A, //0-1 values
                             D3DVALUE x, D3DVALUE y, D3DVALUE z,
                             LPDIRECT3DLIGHT lpD3DLight);
void LinkLightToCamera(void);
                             /* Black Backgrounds (mainly for debug)*/
void RemoveLight(LPDIRECT3DLIGHT x);
void BlackBackGround(void);
void ReleaseBlackBackGround(void);
void SetScreenAmbientLight(int R, int G, int B, int A);
void SetScreenLight(D3DVALUE R, D3DVALUE G, D3DVALUE B,
                             D3DVALUE A,
                             D3DVALUE x, D3DVALUE y, D3DVALUE z
                             );
/////////////////////////////////////////PICKING////////
#include "conv_pick.h"

//////////////////////////////////////////

////////////////Morphing not used/////////////////////
void PGEMorphSetVertices(LPMYOBJ lpOb, D3DVERTEX *lpV, int Start, int End);
void PGEMorphGetVertices(LPMYOBJ lpOb, D3DVERTEX *lpV, int Start, int End);
///////////////////////////////////////////

/////////////////////SCREENLAYER///////////
void DeclareScreenLayer(LPMYFRAME x, BOOL CanBeClipped);
void DeclareScreenLayer3d(LPMYFRAME x, BOOL CanBeClipped);
/////////////////////////////

//////////////////////////////////////////////////////
          
extern D3DMATRIXHANDLE ExecuteMatrix, RMatrix;
/////////////////////////////////////////////////////
/////Bounding Box//////////
void EnableBBoxTransform(LPMYOBJ);
void DisableBBoxTransform(LPMYOBJ);
///////////////////////////

////////////////Sprite3d//////////
void DeclareSprite3D(LPMYOBJ Ob);

void DeclareSprite3D1(LPMYOBJ Ob);
///////////Shadows////////////
void TagShadow(LPMYFRAME);
void SetShadowLight(float Lx, float Ly, float Lz, float Y0);
void AttachShadow(float BBoxScale, LPMYOBJ obj, LPMYFRAME fr);
void AttachFakeShadow(LPMYFRAME orig, LPMYFRAME shadow);
//////////////////////////////
#endif