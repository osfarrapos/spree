#define MAX_STRIPS 200
#define MIN_SCALE .01
//////////////Status values
#define MB_SLEEPING 0
#define MB_STARTED 1
#define MB_ONE_POINT_VALID 2
#define MB_TWO_POINTS_VALIDS 3
#define MB_DISAPPEARING 4
#define MB_DISAPPEARING1 5
////////////////////
typedef struct _MBLARESTATE{
	int Status;			//animation has finished and its track is 
						//slowly disappearing;
	BOOL NoTors;
	D3DMATRIXHANDLE Joint;
	D3DVALUE CTime;
	D3DVALUE LTime;
	D3DVALUE IncTime;
	SMALLTIMEINTERVAL DeltaD;
	SMALLTIMEINTERVAL PrevD;
	D3DVECTOR LastPoints[2];
	int StripsDrawn;
	int VertexDrawn;
	D3DLVERTEX Vertices[(MAX_STRIPS+1)*2];
	int FirstVertex;
	PREAL TextureInc;
	D3DCOLOR Color;
	D3DTEXTUREHANDLE Texture;
	float StripWidth;
	PREAL *Displ;
	D3DVECTOR CV1;
	BOOL MBOK;
}MBLARESTATE, *LPMBLARESTATE;


void DeclareMBlare(LPMYOBJ);
void ReleaseMBlare(LPMYOBJ);
void AttachMBlare(LPMYANIMATION lpAn, PREAL Start, PREAL Stop, 
					int NStrips, D3DMATRIXHANDLE Joint,
					D3DCOLOR Color, D3DTEXTUREHANDLE Texture, PREAL Width,
					PREAL *Displ);
D3DTEXTUREHANDLE LoadMBlareTexture(char *Name);