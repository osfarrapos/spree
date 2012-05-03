//vedi anche oldpezzi_conv.c
/*
Object Name: l1,l2,l3,... la numerazione delle lettere (l) inizia da 1!
l1=EMCHAR0[=E] e l2=EMCHAR[=0], object emissivo
l1=AEMCHAR0[=E] e l2=AEMCHAR1[=1] object aemissivo
l1=DUMMYFRAMECHAR0[=_] e l2=DUMMYFRAMECHAR1[=0] : ???

l3=SPRITECHAR[='S'] && l4=SPRITECHAR0[='1']
l3=SPRITECHAR[='S'] && l4=SPRITECHAR1[='0']

l1=COLLCHAR0[=@]&& l2=COLLCHAR1[=@]: object ONLY_FOR_COLLISIONS, l'oggetto non viene renderizzato
l1=AFRAMECHAR0[=A] && l2=0,...,9
l1=AFRAMECHAR1[=C] && l2=AFRAMECHAR2[=1]
l3=BACKCHAR0[=<] && l4=0 BACK_TRANSPARENT
l3=BACKCHAR0[=<] && l4=1 BACK_M_TRANSPARENT
l1=NOPICK0[=B] && l2=NOPICK1[=0] no picking object

	///////////////////////////
	AddObjToZbuffer			TZBufferB+NTZBufferB,TZBufferM+NTZBufferM, TZBuffer+NTZBuffer,(CZBuffer+NCZBuffer),ZBuffer+NZBuffer
	AddObjToSZbuffer		TSZBuffer+NTSZBuffer, SZBuffer+NSZBuffer
	AddObjToSZbuffer3d	TSZBuffer3d+NTSZBuffer3d, SZBuffer3d+NSZBuffer3d

	Render Order
		Execute lpD3DExCmdBuf
		Render ZBuffer
		Render  CZBuffer, not used
		Render TZBufferB
		Render TZBufferM
		Render TZBuffer
		solo se ci sono screen object: execute lpD3DExCmdBufNull
		solo screen object:  clear viewport
		Render TSZBuffer3D
		Render SZBuffer3D
		disable D3DRENDERSTATE_ZENABLE
		Render TSZBuffer
		Render SZBuffer
		enable D3DRENDERSTATE_ZENABLE
		se ci sono screen object execute lpD3DExCmdBuf
*/	

#include "conv_common.h"
#include "conv_shadow.h"

#include "dprintf.h" // aegis

typedef struct _MVERTEX{
	D3DVECTOR coord;//D3DVALUE x, y, z;
	D3DVECTOR Normal;
	LPDIRECT3DRMMATERIAL Material;
	D3DCOLOR Color;
	D3DVALUE u, v;
	int VNormal;   //the normal and texture coord are valid
	int NextVertex;
	int NextInGroup;
	int ExecuteBIndex;
} MVERTEX, *LPMVERTEX;

typedef struct _MFACE{
	int FIndex;
	LPDIRECT3DRMTEXTURE Texture;
	BOOL Wrapu, Wrapv;
	int Vertices[VxPF];
	int NextFace;
} MFACE, *LPMFACE;

typedef struct _OBJINFO{
	int FirstVGroup; 
	D3DMATRIXHANDLE Matrix;
	int VertexNumber;
	char *FrameName;
}OBJINFO, *LPOBJINFO;

/////////////////Main Sytructures:frames and objects /////////////
static LPMYFRAME AllFrames[MAX_FRAMES];
static int NAllFrames;

static LPMYOBJ AllObjs[MAX_VISUALS];
static int NAllObjs;

static OBJINFO ObjTable[MAX_OBJS];
static int NObjs; //number of objects

///////Ojects Properties//////////////
static BOOL Transparency;   //curr obj is transparent
static BOOL ColorKey;            //curr obj use colorkey
BOOL MYEmissive,		//obj is purely emissive
			MYAEmissive, FMYAEmissive;
/////////////////////////////////////

/////////////Vertices Properties//////////////////
static MVERTEX VBuffer[MAX_VERTEXES];
static int NVertex, NVertexExt; //Current number of vertices of an obj
static int Displacement; //displacement in the table of the curr obj vertices

///                          
static int VGroups[MAX_GOUPS];  /*index in VBuffer of the first
                                Vertex of a group of Vertices with
                                the same properties*/
static int NVGroups; //Number of groups
static int VGroupDisplacement; //displacement in the group table
                                //of the curr obj
                                
static int VGroupsDim[MAX_GOUPS];//dimensions of groups
                          
static D3DMATERIALHANDLE GroupsMaterials[MAX_GOUPS];// material of each group

///////////////////////////////////////////////


///////////////Faces Properties//////////////////////////////
static MFACE FBuffer[MAX_FACES];
static int FDisplacement;//displacement in the table of the curr obj
                          //Faces
static  int NFaces;//current number of faces

/////groups of faces
static int Groups[MAX_GOUPS];  /*index in FBuffer of the first
                                face of a group of faces with the same
                                properties*/
static int NGroups; //number of groups
static int GroupDisplacement; //displacement in the group table
                                //of the curr obj

static int GroupsDim[MAX_GOUPS]; //dimension of each group

//texture groups
static D3DTEXTUREHANDLE GroupsTextures[MAX_GOUPS];//texture of each group
static BOOL GroupsWrapu[MAX_GOUPS], GroupsWrapv[MAX_GOUPS],
	   GroupsTransp[MAX_GOUPS];
        //texture wrap properties of groups
///////////////////////////////////////////////////////////
/////////////Obj being built/////////////
static LPMYOBJ BuildingObj;
static char *CurrExecuteBufferStart;
//////////////////////////////////////////
//////////////////////

//////////////////////                           
static int NNormals; //number of normals of the curr obj 
static D3DEXECUTEBUFFERDESC Descr;
static LPDIRECT3DRMFRAME2 FramesToMove[MAXSUBPARTS];
static int NFramesToMove;
/////////Current execution matrix and reference matrix                                            
D3DMATRIXHANDLE ExecuteMatrix, RMatrix;

D3DMATRIX MSprite3D, MSprite3D1;
D3DMATRIX TempM; //temporary for computations (register)
///////////////////matrix setting buffer//////////
//static 
LPDIRECT3DEXECUTEBUFFER lpD3DExCmdBuf, lpD3DExCmdBufNull;
//////////////////


static BOOL TranslatingShadow=FALSE;
static BOOL CollidingObj=FALSE;
static LPMYOBJ ReferenceObj;

////////////////////////////////
/*Extracting data from meshbuliders and frame herarchies*/
void InitConversion(void);

void PrepareFrameHerarchy(LPDIRECT3DRMFRAME2 lpFrame);
void PrepareBuffers(LPDIRECT3DRMMESHBUILDER2 lpMB);
void PrepareFrameRec(LPDIRECT3DRMFRAME2 lpFrame, D3DMATRIX *lpMF);


////////////////utilitiesfor building intermediate data structures
void AddVertex(LPD3DVECTOR v);
void AddToVGroup(int v);
int VerifyVertex(int vertex, LPD3DVECTOR normal,
                  D3DCOLOR Color,
                  LPDIRECT3DRMMATERIAL Material,
                  D3DVALUE Cu, D3DVALUE Cv);
void GroupFaces(LPDIRECT3DRMFACEARRAY Faces);
////////////////////////////


///////////////Prepare Material, Texture, and matrices//////
void CopyMatrixPointers(LPMYOBJ Ob);
void PrepareTexturesAndMaterials(void);
///////////////////////////////////////////////////////////

///////Computing dimension of ExecuteBuffer///////////////
int FacesOverHead(void);
int VerticesProcessing(void);
int ComputeExecuteBufferDim(void);
/////////////////////////////////

/*ExecuteBuffer Creation*/
void AllocateExecuteBuffer(void);
/*Execute Buffer Filling*/
int CopyVertices(void **p);
void GenerateVerticesInstructions(void **p);
void CopyTriangles (void **p, int g1, int g2);
void GenerateTrianglesInstructions(void **p);
void FillExecuteBuffer(void *p, D3DEXECUTEDATA *x);

/////////////////////////////////////////
void PGEComputeCenterTable(LPDIRECT3DRMMESHBUILDER2 lpMB);
/////////////////////////////////////////////////////////////////////////
/* TD 13-09-00
		Nota: Ho creato questa funzione per testare quando e perchè Execute non funzionava sull'ATI 3D 128
		come risultato ho ottenuto che sembra che se dwFlags=D3DEXECUTE_UNCLIPPED, la funzione non viene eseguita
		correttamente non disegnando l'oggetto. Per il momento non mi spiego il motivo, dato che tale asserzione è vera
		solo nle caso in cui l'oggetto è un giocatore
*/

#ifdef PGE_EXECUTE_DEBUG
	void RunAndCheckExecuteBufferDebug(LPDIRECT3DDEVICE lpD3DDev,LPDIRECT3DEXECUTEBUFFER lpD3DExBuf,  
																											LPDIRECT3DVIEWPORT lpD3DViewport,  DWORD dwFlags){
		
		HRESULT hr=lpD3DDev->lpVtbl->Execute(lpD3DDev, lpD3DExBuf, lpD3DViewport, dwFlags);
		if(hr!=D3D_OK)
			SaveLogTrace("\n TEDDY: WARNING: EXECUTE BUFFER ERROR, dwFlags=%d ",dwFlags);
}
#endif

////////////////////////////////////////////////////////////


	//// *lpMatrix= Matrix * (*lpMatrixf) , prodotto righe per colonne
__inline void PGE_D3DRMMATRIX4D_x_D3DMATRIX_eq_D3DMATRIX(D3DRMMATRIX4D Matrix, 
																													D3DMATRIX *lpMatrixf, D3DMATRIX *lpMatrix){
	//1 riga
	lpMatrix->_11=	Matrix[0][0]*lpMatrixf->_11+Matrix[0][1]*lpMatrixf->_21+
									Matrix[0][2]*lpMatrixf->_31+ Matrix[0][3]*lpMatrixf->_41;

	lpMatrix->_12=	Matrix[0][0]*lpMatrixf->_12+ Matrix[0][1]*lpMatrixf->_22+
									Matrix[0][2]*lpMatrixf->_32+ Matrix[0][3]*lpMatrixf->_42;

	lpMatrix->_13=	Matrix[0][0]*lpMatrixf->_13+ Matrix[0][1]*lpMatrixf->_23+
									Matrix[0][2]*lpMatrixf->_33+ Matrix[0][3]*lpMatrixf->_43;

	lpMatrix->_14=	Matrix[0][0]*lpMatrixf->_14+ Matrix[0][1]*lpMatrixf->_24+
									Matrix[0][2]*lpMatrixf->_34+ Matrix[0][3]*lpMatrixf->_44;
	//2 riga
	lpMatrix->_21=	Matrix[1][0]*lpMatrixf->_11+ Matrix[1][1]*lpMatrixf->_21+
									Matrix[1][2]*lpMatrixf->_31+ Matrix[1][3]*lpMatrixf->_41;

	lpMatrix->_22=	Matrix[1][0]*lpMatrixf->_12+ Matrix[1][1]*lpMatrixf->_22+
									Matrix[1][2]*lpMatrixf->_32+ Matrix[1][3]*lpMatrixf->_42;

	lpMatrix->_23=	Matrix[1][0]*lpMatrixf->_13+ Matrix[1][1]*lpMatrixf->_23+
									Matrix[1][2]*lpMatrixf->_33+ Matrix[1][3]*lpMatrixf->_43;

	lpMatrix->_24=	Matrix[1][0]*lpMatrixf->_14+ Matrix[1][1]*lpMatrixf->_24+
									Matrix[1][2]*lpMatrixf->_34+ Matrix[1][3]*lpMatrixf->_44;
	//3 riga
	lpMatrix->_31=	Matrix[2][0]*lpMatrixf->_11+ Matrix[2][1]*lpMatrixf->_21+
									Matrix[2][2]*lpMatrixf->_31+ Matrix[2][3]*lpMatrixf->_41;

	lpMatrix->_32=	Matrix[2][0]*lpMatrixf->_12+ Matrix[2][1]*lpMatrixf->_22+
									Matrix[2][2]*lpMatrixf->_32+ Matrix[2][3]*lpMatrixf->_42;

	lpMatrix->_33=	Matrix[2][0]*lpMatrixf->_13+ Matrix[2][1]*lpMatrixf->_23+
									Matrix[2][2]*lpMatrixf->_33+ Matrix[2][3]*lpMatrixf->_43;

	lpMatrix->_34=	Matrix[2][0]*lpMatrixf->_14+ Matrix[2][1]*lpMatrixf->_24+
									Matrix[2][2]*lpMatrixf->_34+ Matrix[2][3]*lpMatrixf->_44;                     
	//4 riga
	lpMatrix->_41=	Matrix[3][0]*lpMatrixf->_11+ Matrix[3][1]*lpMatrixf->_21+
									Matrix[3][2]*lpMatrixf->_31+ Matrix[3][3]*lpMatrixf->_41;

	lpMatrix->_42=	Matrix[3][0]*lpMatrixf->_12+ Matrix[3][1]*lpMatrixf->_22+
									Matrix[3][2]*lpMatrixf->_32+ Matrix[3][3]*lpMatrixf->_42;

	lpMatrix->_43=	Matrix[3][0]*lpMatrixf->_13+ Matrix[3][1]*lpMatrixf->_23+
									Matrix[3][2]*lpMatrixf->_33+ Matrix[3][3]*lpMatrixf->_43;

	lpMatrix->_44=	Matrix[3][0]*lpMatrixf->_14+ Matrix[3][1]*lpMatrixf->_24+
									Matrix[3][2]*lpMatrixf->_34+ Matrix[3][3]*lpMatrixf->_44;
}

void FindTransform(LPDIRECT3DRMFRAME lpReference,  LPDIRECT3DRMFRAME2 lpFrame,
              D3DMATRIX *lpMatrix, D3DMATRIX *lpMatrixf){
	D3DRMMATRIX4D Matrix;
	lpFrame->lpVtbl->GetTransform(lpFrame, Matrix);
	// *lpMatrix= Matrix * (*lpMatrixf) , prodotto righe per colonne
	PGE_D3DRMMATRIX4D_x_D3DMATRIX_eq_D3DMATRIX(Matrix, lpMatrixf, lpMatrix);
}

void FindTransform1(LPDIRECT3DRMFRAME lpReference, LPDIRECT3DRMFRAME2 lpFrame,  D3DMATRIX *lpMatrix){    
	D3DRMMATRIX4D Matrix;

	lpFrame->lpVtbl->GetTransform(lpFrame, Matrix);
	memcpy(lpMatrix,Matrix, sizeof(Matrix));	
}

void FindTransformb(LPDIRECT3DRMFRAME lpReference, LPDIRECT3DRMFRAME2 lpFrame,
              D3DMATRIX *lpMatrix, D3DMATRIX *lpMatrixf){
	
	FindTransform(lpReference, lpFrame,lpMatrix, lpMatrixf);
	
	lpMatrix->_13=-lpMatrix->_13;
	lpMatrix->_23=-lpMatrix->_23;
	lpMatrix->_33=-lpMatrix->_33;
	lpMatrix->_43=-lpMatrix->_43;
}

D3DMATRIXHANDLE CreateMatrix(void){
	D3DMATRIXHANDLE H;
	if (lpD3DDevice->lpVtbl->CreateMatrix(lpD3DDevice, &H)==D3D_OK)
		return H;	
	return PGE_DX_NOHANDLE;
}

/////////////////////////////////////////////////
static D3DMATRIX RefD3D;

void PGEInitRefD3D(void){
	memset(&RefD3D, 0, sizeof(D3DMATRIX));
	RefD3D._44=PVALUE(1.);
}

void PGESetObjMatrices(LPMYOBJ lpRefObj){
	int m;
	HRESULT H;

	for (m=0; m<lpRefObj->NMatrices; m++){
		memcpy((float *)&RefD3D,  (float *)(lpRefObj->init+m+1), 3*sizeof(float));
		memcpy(((float *)&RefD3D)+4,  ((float *)(lpRefObj->init+m+1))+3, 3*sizeof(float));
		memcpy(((float *)&RefD3D)+8,  ((float *)(lpRefObj->init+m+1))+6, 3*sizeof(float));
		memcpy(((float *)&RefD3D)+12,  ((float *)(lpRefObj->init+m+1))+9, 3*sizeof(float));
		H=lpD3DDevice->lpVtbl->SetMatrix(lpD3DDevice, lpRefObj->Matrices[m],	&RefD3D);
	}
}
///////////////////////////////////////////////////////////
//AllObjs fns
LPMYOBJ NewObj(char *szName){
	LPMYOBJ MyObj=MyMalloc(MYOBJ, 1);
	myassert(MyObj!=NULL, MemoryError, 0);
	memset(MyObj, 0, sizeof(MYOBJ));
	MyObj->Name=MyMalloc(char, strlen(szName)+1);
	strcpy(MyObj->Name, szName);
	MyObj->RefCounter++;
	myassert(NAllObjs<MAX_VISUALS, "Maximum number of objects reached", 0);
	AllObjs[NAllObjs]=MyObj;
	NAllObjs++;
	return MyObj;
}

#undef MyFree
#define MyFree(x) { gfree(x) ; x = 0; }

int ReleaseObj(LPMYOBJ Obj)
{
	int i;

	if (Obj==NULL) 
		return 0;
	if (Obj->RefCounter>0) 
		Obj->RefCounter--;
	if (Obj->RefCounter<=0) {
		if (Obj->Name!=NULL) {
			MyFree(Obj->Name);
		}
		if (!(Obj->Flags&ISSHADOW)){
			if (Obj->Matrices!=NULL) {			 
				//aegis
				for (i=Obj->NMatrices-1; i>=0; i--) 
					MyFree(Obj->FrameNames[i]);
				MyFree(Obj->FrameNames);
				MyFree(Obj->Matrices); 
			}
		}
		if (!(Obj->Flags&CLONE)) {
			if (Obj->OMatrices!=NULL) 
				MyFree(Obj->OMatrices);
			if (Obj->Vertices!=NULL) 
				MyFree(Obj->Vertices);
			if (Obj->Textures!=NULL) 
				MyFree(Obj->Textures);
			if (Obj->Triangles!=NULL) 
				MyFree(Obj->Triangles);
		}
		if (Obj->VariableTextures!=NULL) 
			MyFree(Obj->VariableTextures);
		if ((!(Obj->Flags&CLONE))&&(Obj->lpExecuteBuffer!=NULL))
			Obj->lpExecuteBuffer->lpVtbl->Release(Obj->lpExecuteBuffer);
		if (Obj->MBlare!=NULL) 
			MyFree(Obj->MBlare);
		MyFree(Obj);
		return 0;
	}
	else 
		return Obj->RefCounter;
}

void ReleaseAllObjs(void)
{
	int i;
	for(i=NAllObjs-1; i>=0; i--) {
		if (i==9) {
			int j;
			j = 0;
		}
		ReleaseObj(AllObjs[i]);	
		AllObjs[i] = 0;
	}
	NAllObjs=0;
}

LPMYOBJ FindObj(char *szName){
	int i;
	for (i=0; i<NAllObjs; i++)
		if (strcmp(AllObjs[i]->Name, szName)==0) return AllObjs[i];
	return NULL;
}

int RemoveFromObjTable(LPMYOBJ Obj){
	int i, n;
	for(i=0; i<NAllObjs; i++)
		if (Obj==AllObjs[i]) n=i;
	if (n<NAllObjs){
		for(i=n+1; i<NAllObjs; i++)
			AllObjs[i-1]=AllObjs[i];
		NAllObjs--;
		return  ReleaseObj(Obj);
	}
	return 0;   
}

LPMYFRAME GetObjFather(LPMYOBJ Obj){
	return Obj->Father;
}

LPMYFRAME GetFrameFather(LPMYOBJ Frame){
	return Frame->Father;
}

LPMYOBJ CloneObj(LPMYOBJ Ob, char *szName){
	LPMYOBJ Ob1;
	Ob1=NewObj(szName);
	memcpy(Ob1, Ob, sizeof(MYOBJ));
	Ob1->Flags=Ob->Flags|CLONE;
	if (Ob1->NMatrices>0) {
		Ob1->Matrices= MyMalloc(D3DMATRIXHANDLE, Ob1->NMatrices);
		Ob1->FrameNames=MyMalloc(char *, Ob1->NMatrices);
	}
	if (Ob1->NVariableTextures>0) 
		Ob1->VariableTextures=MyMalloc(TEXTURE_INSTANCE, Ob1->NVariableTextures);
	memcpy(Ob1->Matrices, Ob->Matrices, sizeof(D3DMATRIXHANDLE)*Ob1->NMatrices);
	memcpy(Ob1->FrameNames, Ob->FrameNames, Ob1->NMatrices*sizeof(char *));
	memcpy(Ob1->VariableTextures, Ob->VariableTextures,sizeof(TEXTURE_INSTANCE)*Ob1->NVariableTextures);
	Ob1->MBlare=NULL;
	if(Ob->MBlare!=NULL) DeclareMBlare(Ob1);
    return Ob1;
}

D3DMATRIXHANDLE FindJoint(char *szName, LPMYOBJ lpOb){
	int i;
	for (i=0; i<lpOb->NMatrices; i++)
		if (strcmp(lpOb->FrameNames[i], szName)==0) 
			return lpOb->Matrices[i];
	return PGE_DX_NOHANDLE;
}

// TD 6-10-00: it's not used in puma, BeforeRender is directly setted 
void SetBeforeRender(LPMYOBJ lpOb, void(*x)(LPMYOBJ)){
	if (lpOb==NULL) return;
	lpOb->BeforeRender=x;
}

// TD 6-10-00: it's not used in puma, AfterRender is directly setted 
void SetAfterRender(LPMYOBJ lpOb, void(*x)(LPMYOBJ)){
	if (lpOb==NULL) return;
	lpOb->AfterRender=x;
}

//BoundingBox//////////////
void EnableBBoxTransform(LPMYOBJ Ob){
	if (Ob==NULL) return;
	Ob->Flags|=COLLISIONNEEDED;
}
void DisableBBoxTransform(LPMYOBJ Ob){
	int temp=COLLISIONNEEDED;
	if (Ob==NULL) return;
	Ob->Flags&=(!COLLISIONNEEDED);
}

//Sprite3D////////////////
void DeclareSprite3D(LPMYOBJ Fr){
	Fr->Flags|=SPRITE3D;
	Fr->ValidBox=FALSE;
}

void DeclareSprite3D1(LPMYOBJ Fr){
	Fr->Flags|=SPRITE3D1;
	Fr->ValidBox=FALSE;
}


/////texture handling of an obj
//used in loadpl	
void ChangeTexture(LPMYOBJ lpOb, int NText, char **Old, NEWNAME *New){
	int i;
	LPDIRECTDRAWSURFACE lpSurf;
	myassert(lpOb!=NULL, "Wrong Obj Selected for Texture Change", 0);
	myassert(lpOb->VariableTextures==NULL, "Texture Changes Already Selected in:",
			 lpOb->Name);
	myassert(NText>0, "Wrong Number of texture Changes in:", lpOb->Name);
	lpOb->VariableTextures=MyMalloc(TEXTURE_INSTANCE, NText);
	lpOb->NVariableTextures=NText;
	myassert(lpOb->VariableTextures!=NULL, MemoryError, 0);
	for(i=0; i<NText; i++){
		NewTexture(New[i], &lpSurf,&(lpOb->VariableTextures[i].Value));
		lpOb->VariableTextures[i].Variable=FindTexturePointer(Old[i]);
		myassert(lpOb->VariableTextures[i].Variable!=NULL,
				"Wrong Name For Texture To Change in:", lpOb->Name);
	}
	return;	
}

//used in mblare.c
void UpdateTexture(LPMYOBJ lpOb, char *Old, char *New){
	int i;
	LPDIRECTDRAWSURFACE lpSurf;
	myassert(lpOb!=NULL, "Wrong Obj Selected for Texture Update", 0);
	myassert(lpOb->VariableTextures!=NULL, "Texture Changes NOT Already Selected in:",
			 lpOb->Name);
	for (i=0; i<lpOb->NVariableTextures; i++){
		if (FindTexturePointer(Old)==lpOb->VariableTextures[i].Variable){
			NewTexture(New, &lpSurf,&(lpOb->VariableTextures[i].Value));
			return;
		}
	}
	myassert(FALSE,"Wrong Name For Texture To Change in:", lpOb->Name);
	return;
}

///////////////////////////////////////////////////////////
//AllFrames fns
//create a NewFrame and record it in AllFrames
LPMYFRAME NewFrame(char *szName)
{
	LPMYFRAME MyObj=MyMalloc(MYFRAME, 1);
	myassert(MyObj!=NULL, MemoryError,0);
	memset(MyObj, 0, sizeof(MYFRAME));
	MyObj->Name=MyMalloc(char, strlen(szName)+1);
	strcpy(MyObj->Name, szName);
	MyObj->RefCounter++;
	myassert(NAllFrames<MAX_FRAMES,"Maximum number of frames reached",0);
	AllFrames[NAllFrames]=MyObj;
	NAllFrames++;
	return MyObj;
}

void ReleaseAllFrames(void){
	int i;
	for(i=NAllFrames-1; i>=0; i--) {
		ReleaseFrame(AllFrames[i]);
	}
	NAllFrames=0;
}

int RemoveFromFrameTable(LPMYFRAME Obj){
	int i, n;
	for(i=0; i<NAllFrames; i++){
		if (Obj==AllFrames[i]) n=i;
	}
	if (n<NAllFrames){
		for(i=n+1; i<NAllFrames; i++){
			AllFrames[i-1]=AllFrames[i];
		}
		NAllFrames--;
		return  ReleaseFrame(Obj);
	}
	return 0;  
}

LPMYFRAME FindFrame(char *szName)
{
	int i;
	for (i=0; i<NAllFrames; i++)
		if (strcmp(AllFrames[i]->Name, szName)==0) return AllFrames[i];
	return NULL;
}  

//NOT USED
/*
int DeleteObjFromFrame(LPMYOBJ Obj, LPMYFRAME Frame){
	LPMYOBJ *lpCurrObj;
	if(Obj==NULL||Frame==NULL) return -1;
	Obj->Father=NULL;
	lpCurrObj=&(Frame->ObList);
	while(((*lpCurrObj)!=NULL)&&((*lpCurrObj)!=Obj)){
		lpCurrObj=&((*lpCurrObj)->Next);
	}
	if ((*lpCurrObj)!=NULL){
		*lpCurrObj=Obj->Next;
		Obj->Next=NULL;
		return ReleaseObj(Obj); 
	}else 
		return Obj->RefCounter;
}
*/
int DeleteFrameFromFrame(LPMYFRAME Obj, LPMYFRAME Frame){
	LPMYFRAME *lpCurrObj;
	if(Obj==NULL||Frame==NULL) return -1;
	Obj->Father=NULL;
	lpCurrObj=&(Frame->FrameList);
	while(((*lpCurrObj)!=NULL)&&((*lpCurrObj)!=Obj)){
		lpCurrObj=&((*lpCurrObj)->Next);
	}
	if ((*lpCurrObj)!=NULL){
		*lpCurrObj=Obj->Next;
		Obj->Next=NULL;
		return ReleaseFrame(Obj); 
	}	else 
		return Obj->RefCounter;
}


void AddFrameToFrame(LPMYFRAME Obj, LPMYFRAME Frame){
	if(Obj==NULL||Frame==NULL||Obj->Next!=NULL) return;
	Obj->Next=Frame->FrameList;
	Frame->FrameList=Obj;
	Obj->RefCounter++;
	Obj->Father=Frame;
}

int ReleaseFrame(LPMYFRAME Obj){
	LPMYFRAME NextF;
	LPMYOBJ   NextO;
	if (Obj==NULL) return 0;
	if(Obj->RefCounter>0) Obj->RefCounter--;
	if(Obj->RefCounter<=0) {
		while (Obj->ObList!=NULL){
			NextO=Obj->ObList;
			Obj->ObList=NextO->Next;
			NextO->Next=NULL;
			ReleaseObj(NextO);
		}
		while (Obj->FrameList!=NULL){
			NextF=Obj->FrameList;
			Obj->FrameList=NextF->Next;
			NextF->Next=NULL;
			ReleaseFrame(NextF);
		}
		if (Obj->Name!=NULL) MyFree(Obj->Name);
		MyFree(Obj);
		return 0;
	}	else 
		return Obj->RefCounter;
}

void AddObjToFrame(LPMYOBJ Obj, LPMYFRAME Frame){
	if(Obj==NULL||Frame==NULL||Obj->Next!=NULL) return;
	Obj->Next=Frame->ObList;
	Frame->ObList=Obj;
	Obj->RefCounter++;
	Obj->Father=Frame;
}


/////////////////////////////////////////////////////////////////////////
//MVERTEX fns, i.e. handling of VBuffer and VGroups, VGroupsDim

//build a D3DVERTEX from a MVERTEX
#define PGE_MAKE_D3DVERTEX_FROM_MVERTEX(lpD3dv, lpMv) {\
			lpD3dv->x=lpMv->coord.x,	lpD3dv->y=lpMv->coord.y,	lpD3dv->z=lpMv->coord.z;\
			lpD3dv->nx=lpMv->Normal.x,	lpD3dv->ny=lpMv->Normal.y,	lpD3dv->nz=lpMv->Normal.z;\
			lpD3dv->tu=lpMv->u,lpD3dv->tv=lpMv->v;}

//reset 
void PGEVBufferReset(void){
	NVertex=0;
	NVertexExt=0;
	Displacement=0;
}

void PGEVGroupReset(void){
	NVGroups=0;
	VGroupDisplacement=0;
}

void AddVertex(LPD3DVECTOR V){    
	myassert(NVertex<MAX_VERTEXES, "Maimumm number of vertices reached", 0);
	VBuffer[NVertex].coord=*V;
	VBuffer[NVertex].VNormal=0;
	NVertexExt=VBuffer[NVertex].NextVertex=NVertex+1;
	NVertex++;
}

void CloseAddition(void){
	if (VBuffer[NVertex-1].NextVertex==NVertex)
		VBuffer[NVertex-1].NextVertex=-1;
}

//used only in VerifyVertex
void AddToVGroup(int v){
	int j;
	BOOL bFound=FALSE;
	for (j=VGroupDisplacement; j<NVGroups;j++){
		if ((VBuffer[VGroups[j]].Color==VBuffer[v].Color)&&
							(VBuffer[VGroups[j]].Material==VBuffer[v].Material)){ 
			VBuffer[v].NextInGroup=VGroups[j];
			VGroups[j]=v;
			VGroupsDim[j]++;
			bFound=TRUE;
		}
	}
	if (!bFound){
		myassert(NVGroups<MAX_GOUPS, "Maximum number of groups reached", 0);
		VBuffer[v].NextInGroup=-1;
		VGroupsDim[NVGroups]=1;
		VGroups[NVGroups]=v;
		NVGroups++;                  
	}
}

int VerifyVertex(int vertex, LPD3DVECTOR normal,
                  D3DCOLOR Color, LPDIRECT3DRMMATERIAL Material,
                  D3DVALUE Cu, D3DVALUE Cv){
	int j=vertex+Displacement;
	char finished=FALSE;
	vertex=j;
	while (!finished){
		if (VBuffer[j].VNormal==0) {
			VBuffer[j].Normal=*normal;
			VBuffer[j].u=Cu, VBuffer[j].v=Cv;
			VBuffer[j].Color=Color;
			VBuffer[j].Material=Material;
			VBuffer[j].VNormal=1;
			AddToVGroup(j);
			myassert(j>=Displacement, 0, 0);
			return j;
		}
		if ((VBuffer[j].Color==Color)&&
					(VBuffer[j].Material==Material)&&
					SameNormals((*normal), VBuffer[j].Normal)&&
					SameFloat( VBuffer[j].u, Cu)&&
					SameFloat( VBuffer[j].v, Cv))
			return j;
		if (VBuffer[j].NextVertex<NVertex) {
			myassert(NVertexExt<MAX_VERTEXES, "Maximum number vertices reached", 0);
			VBuffer[NVertexExt].coord= VBuffer[vertex].coord;
			VBuffer[NVertexExt].Normal=*normal; 
			VBuffer[NVertexExt].u=Cu,	VBuffer[NVertexExt].v=Cv;
			VBuffer[NVertexExt].Color=Color;
			VBuffer[NVertexExt].Material=Material;
			VBuffer[NVertexExt].VNormal=1;
			VBuffer[NVertexExt].NextVertex=VBuffer[j].NextVertex;
			VBuffer[j].NextVertex=NVertexExt;
			AddToVGroup(NVertexExt);
			NVertexExt++;
			myassert(NVertexExt-1>=Displacement, 0, 0);
			return(NVertexExt-1);
		}	else {
			myassert(VBuffer[j].NextVertex!=j,"FATAL ERROR","No valid NextVertex");
			j=VBuffer[j].NextVertex;
		}
	}

    return 0;
}

/////////////////////////////////////////////////////////////////////////
//Loading fns
void CopyMatrixPointers(LPMYOBJ Ob){
	int i;
	Ob->NMatrices=0;
	for (i=0; i<NObjs; i++){
		if (ObjTable[i].Matrix!=0)
			Ob->NMatrices++;
	}
	if (Ob->NMatrices==0) return;
	if(TranslatingShadow){
		if (CollidingObj){
			Ob->Vertices=MyMalloc(DWORD, Ob->NMatrices);
			Ob->OMatrices=MyMalloc(D3DMATERIALHANDLE, Ob->NMatrices);
			myassert(Ob->OMatrices!=NULL, 0, 0);
			Ob->ONMatrices=0;
			for (i=0; i<NObjs; i++){
				if (ObjTable[i].Matrix!=0){
					Ob->OMatrices[Ob->ONMatrices]=ObjTable[i].Matrix;
					Ob->Vertices[Ob->ONMatrices]=ObjTable[i].VertexNumber;
					Ob->ONMatrices++;
				}
			}
		}
		Ob->Matrices=ReferenceObj->Matrices;
		Ob->FrameNames=ReferenceObj->FrameNames;
		Ob->NMatrices=ReferenceObj->NMatrices;
	}	else{
		Ob->Vertices=MyMalloc(DWORD, Ob->NMatrices);
		Ob->Matrices=MyMalloc(D3DMATERIALHANDLE, Ob->NMatrices);
		Ob->FrameNames=MyMalloc(char *, Ob->NMatrices);
		myassert(Ob->Matrices!=NULL, 0, 0);
		Ob->NMatrices=0;
		for (i=0; i<NObjs; i++){
			if (ObjTable[i].Matrix!=0){
				Ob->Vertices[Ob->NMatrices]=ObjTable[i].VertexNumber;
				Ob->Matrices[Ob->NMatrices]=ObjTable[i].Matrix;
				Ob->FrameNames[Ob->NMatrices]=ObjTable[i].FrameName;
				Ob->NMatrices++;
			}
		}
	}
}

//TD 11-10-00 see also old_pezzi to old versions of it
void PGESetTransformMatrices(LPDIRECT3DEXECUTEBUFFER *lplpD3DExCmdBuf, BOOL bNull){	
	D3DMATRIXHANDLE IM;
	D3DMATRIX MM;	
	void *lpPointer, *lpBufStart, *lpInsStart;    
	int size;
	D3DEXECUTEDATA d3dExData;
	int nState;
	
	if(bNull){
		//make identity matrix
		PGE_MAKE_IDENTITY_D3DMATRIX(MM);
		IM=CreateMatrix();
		lpD3DDevice->lpVtbl->SetMatrix(lpD3DDevice, IM,	 &MM);
		nState=2;
	}else
		nState=3;
	size = 0;
	size += sizeof(D3DINSTRUCTION) * 2;
	size += sizeof(D3DSTATE) * nState; 
	memset(&Descr, 0, sizeof(D3DEXECUTEBUFFERDESC));
	Descr.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	Descr.dwFlags = D3DDEB_BUFSIZE;
	Descr.dwBufferSize = size;
	if (lpD3DDevice->lpVtbl->CreateExecuteBuffer(
						lpD3DDevice, &Descr, lplpD3DExCmdBuf, NULL) != D3D_OK)
		return;
	while ((*lplpD3DExCmdBuf)->lpVtbl->Lock(*lplpD3DExCmdBuf, &Descr) ==DDERR_WASSTILLDRAWING);
	lpBufStart = Descr.lpData;
	memset(lpBufStart, 0, size);
	lpPointer = lpBufStart;

	lpInsStart = lpPointer;
	OP_STATE_TRANSFORM(nState, lpPointer);

	if(bNull){
		STATE_DATA(D3DTRANSFORMSTATE_VIEW, IM, lpPointer);
		STATE_DATA(D3DTRANSFORMSTATE_PROJECTION, IM, lpPointer);
	}else{
		STATE_DATA(D3DTRANSFORMSTATE_WORLD, ExecuteMatrix, lpPointer);
		STATE_DATA(D3DTRANSFORMSTATE_VIEW, CurrCameraM, lpPointer);
		STATE_DATA(D3DTRANSFORMSTATE_PROJECTION, CurrProjectionM, lpPointer);
	}
	OP_EXIT(lpPointer);
	/*
	 * Setup the execute data describing the buffer
	 */
	(*lplpD3DExCmdBuf)->lpVtbl->Unlock(*lplpD3DExCmdBuf);
	memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
	d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
	d3dExData.dwInstructionOffset = (ULONG) 0;
	d3dExData.dwInstructionLength = (ULONG) ((char*)lpPointer - (char*)lpInsStart);
	(*lplpD3DExCmdBuf)->lpVtbl->SetExecuteData(*lplpD3DExCmdBuf, &d3dExData);

	if(!bNull){
		lpD3DDevice->lpVtbl->BeginScene(lpD3DDevice);	
		RunAndCheckExecuteBuffer(lpD3DDevice, *lplpD3DExCmdBuf, CurrViewport, D3DEXECUTE_UNCLIPPED);
		lpD3DDevice->lpVtbl->EndScene(lpD3DDevice);		
	}
	return;
}


void PGEReleaseOneCommandBuffer(LPDIRECT3DEXECUTEBUFFER *lp_lpCmdBuff){
	ULONG X;
	if (*lp_lpCmdBuff) {
		X=(*lp_lpCmdBuff)->lpVtbl->Release(*lp_lpCmdBuff);	
		*lp_lpCmdBuff=NULL;
		myassert(X==0, 0, 0);
	}
}

void ReleaseCommandBuffer(void){
	PGEReleaseOneCommandBuffer(&lpD3DExCmdBuf);
	PGEReleaseOneCommandBuffer(&lpD3DExCmdBufNull);
}


void InitD3DPatch(void){
	FMYAEmissive=	MYAEmissive=FALSE;
	
	PGEInitRefD3D();
	
	//ResetCenterTable();
	///
	PGEPickResetRect();
	ResetPickAfter();//PickRequired=FALSE;
	///
	PGEResetMainLight();
	ExecuteMatrix=CreateMatrix();
	RMatrix=CreateMatrix();
	CurrProjectionM=CreateMatrix();
	CurrCameraM=CreateMatrix();
	PGESetTransformMatrices(&lpD3DExCmdBuf,FALSE);//SetTransformMatrices();
	PGESetTransformMatrices(&lpD3DExCmdBufNull,TRUE);//PrepareNullTransformMatrices();
	NAllObjs=0;
	NAllFrames=0;
	NAllAnimations=AnimationTag;
	InitDictionary();		
}
 
void InitConversion(void){
	Transparency=FALSE;
	ColorKey=FALSE;
	PGEVBufferReset();	
	
	NGroups=NFaces=0;
	NObjs=0;
	
	GroupDisplacement=0;
	FDisplacement=0;
	
	PGEVGroupReset();
	
	NFramesToMove=0;
}

///////////////////////////////////////////  
//use in PrepareBuffers  
void GroupFaces(LPDIRECT3DRMFACEARRAY Faces){
	int i, j, l;
	LPDIRECT3DRMTEXTURE TT;

	NFaces=FDisplacement+Faces->lpVtbl->GetSize(Faces);
	myassert(NFaces<MAX_FACES, "Maximum number of vertices reached",0);
	for (i=FDisplacement, l=0; i<NFaces; i++, l++){
		char found=FALSE;
		LPDIRECT3DRMFACE Face;
		Faces->lpVtbl->GetElement(Faces, l, &Face);
		Face->lpVtbl->GetTextureTopology(Face, &(FBuffer[i].Wrapu), &(FBuffer[i].Wrapv));
		Face->lpVtbl->GetTexture(Face, &TT);
		/*
		if (TT!=NULL){
			TT->lpVtbl->QueryInterface(TT, &IID_IDirect3DTexture2, &(FBuffer[i].Texture));	
			TT->lpVtbl->Release( TT);
		}	else
			FBuffer[i].Texture=NULL;
		*/
		FBuffer[i].Texture=TT;
		if (FBuffer[i].Texture!=NULL)
			FBuffer[i].Texture->lpVtbl->Release(FBuffer[i].Texture);

		for (j=GroupDisplacement; j<NGroups;j++){
			if (SameFeatures(FBuffer[Groups[j]], FBuffer[i])){
				FBuffer[i].NextFace=Groups[j];
				Groups[j]=i;
				GroupsDim[j]++;
				found=TRUE;
			}
		}
		if (!found){
			myassert(NGroups<MAX_GOUPS, "Maximum number of vertices reached", 0);
			FBuffer[i].NextFace=-1;
			GroupsDim[NGroups]=1; 
			Groups[NGroups]=i;
			NGroups++;
		}
		Face->lpVtbl->Release(Face);
	}  
}

void PrepareBuffers(LPDIRECT3DRMMESHBUILDER2 lpMB){
	int i;
	int dummy;
	int TNVertex;
	D3DVALUE Curru, Currv;
	LPDIRECT3DRMFACEARRAY Faces;
	LPD3DVECTOR vertexdata;
	//InitConversion();
	myassert(NObjs<MAX_OBJS, "Maximun number of component objects reached",0);

	FDisplacement=NFaces;
	NVertex=NVertexExt=Displacement;
	lpMB->lpVtbl->GetFaces(lpMB, &Faces);
	GroupFaces(Faces);
	lpMB->lpVtbl->GetVertices(lpMB, (DWORD *)&TNVertex, NULL, (DWORD *)&NNormals,
								NULL, (DWORD *)&dummy, NULL);
	vertexdata=(D3DVECTOR *)malloc(TNVertex*sizeof(D3DVECTOR));
	lpMB->lpVtbl->GetVertices(lpMB, (DWORD *)&TNVertex, vertexdata, (DWORD *)&NNormals,
								NULL, (DWORD *)&dummy, NULL);
	for(i=0;i<TNVertex; i++){
		AddVertex(&vertexdata[i]);
	}
	CloseAddition();
	free((void *)vertexdata);
	for (i=0; i<NFaces-FDisplacement;i++){
		int NVertices, j;
		D3DVECTOR CNormal, dummy;
		LPDIRECT3DRMFACE Face;
		D3DCOLOR Color;
		LPDIRECT3DRMMATERIAL Material;
		Faces->lpVtbl->GetElement(Faces, i, &Face);
		NVertices=Face->lpVtbl->GetVertexCount(Face);
		myassert(NVertices<= VxPF, 0, 0);
		for (j=0;j<NVertices; j++){
			Face->lpVtbl->GetVertex(Face, j, &dummy, &CNormal);
			Face->lpVtbl->GetTextureCoordinates(Face, j, &Curru, &Currv);
			Color=Face->lpVtbl->GetColor(Face);
			Face->lpVtbl->GetMaterial(Face, &Material);
			if (Material!=NULL)
				Material->lpVtbl->Release(Material);
			FBuffer[i+FDisplacement].Vertices[j]=
					VerifyVertex(Face->lpVtbl->GetVertexIndex(Face, j),
																			&CNormal,Color,Material,Curru, Currv);
		}
		Face->lpVtbl->Release(Face);
	}
	Faces->lpVtbl->Release(Faces);
	Displacement=NVertexExt;
	ObjTable[NObjs].FirstVGroup=VGroupDisplacement;
	GroupDisplacement=NGroups;
	VGroupDisplacement=NVGroups;  
	NObjs++;

	//PGEComputeCenterTable(lpMB);
	
}

///////////////////////////////////////
//PrepareTexturesAndMaterials fns 
void ReorderFGroups(void){
   int GBuffer[MAX_GOUPS], i, j, l;
   D3DTEXTUREHANDLE CurrHandle, GBufferT[MAX_GOUPS], GBufferDim[MAX_GOUPS];
   BOOL GBufferWu[MAX_GOUPS], GBufferWv[MAX_GOUPS];    
   BOOL AlreadyCopied[MAX_GOUPS];
   l=0;
   memset(AlreadyCopied, 0, NGroups*sizeof(BOOL));
   for (i=0; i<NGroups; i++){
       if (AlreadyCopied[i]) continue;
	   if (GroupsTransp[i]) continue;
       CurrHandle=GroupsTextures[i];
       for (j=i; j<NGroups; j++){
           if (CurrHandle!=GroupsTextures[j]) continue;
           GBuffer[l]=Groups[j];
           GBufferT[l]=CurrHandle;
           GBufferWu[l]=GroupsWrapu[j];
           GBufferWv[l]=GroupsWrapv[j];
           GBufferDim[l]=GroupsDim[j];
           l++;
           AlreadyCopied[j]=TRUE;
       }
   }
   for (i=0; i<NGroups; i++){
       if (AlreadyCopied[i]) continue;
       CurrHandle=GroupsTextures[i];
       for (j=i; j<NGroups; j++){
           if (CurrHandle!=GroupsTextures[j]) continue;
           GBuffer[l]=Groups[j];
           GBufferT[l]=CurrHandle;
           GBufferWu[l]=GroupsWrapu[j];
           GBufferWv[l]=GroupsWrapv[j];
           GBufferDim[l]=GroupsDim[j];
           l++;
           AlreadyCopied[j]=TRUE;
       }
   }
   memcpy(Groups, GBuffer, sizeof(int)*NGroups);
   memcpy(GroupsDim, GBufferDim, sizeof(int)*NGroups);
   memcpy(GroupsTextures, GBufferT, sizeof(D3DTEXTUREHANDLE)*NGroups);
   memcpy(GroupsWrapu, GBufferWu, sizeof(BOOL)*NGroups);
   memcpy(GroupsWrapv, GBufferWv, sizeof(BOOL)*NGroups);
}

BOOL TextureTransparent(LPDIRECTDRAWSURFACE x){
	DDSURFACEDESC DDSurfDesc;
	memset(&DDSurfDesc, 0, sizeof(DDSURFACEDESC));
	DDSurfDesc.dwSize = sizeof(DDSURFACEDESC);
	x->lpVtbl->GetSurfaceDesc(x, &DDSurfDesc);
	return DDSurfDesc.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS;
}

BOOL TextureColorKey(char *Name){
	return  (ColorkeySupported	&&
				((Name[0]=='_')||((Name[0]>='0')&&(Name[0]<='9')))&&
				((Name[1]=='T')||(Name[1]=='t')));
}

void PrepareTexturesAndMaterials(void){
	int i;
	LPDIRECTDRAWSURFACE Surf;
	LPDIRECT3DTEXTURE2 ComObj;
	char *Name;
	D3DCOLOR colVertex;
	D3DVALUE RSpec,	GSpec, BSpec;
	D3DVALUE REm,	GEm, BEm;
	D3DVALUE SExp;
	LPDIRECT3DRMMATERIAL lpM;
	for (i=0; i<NVGroups; i++){
		colVertex=VBuffer[VGroups[i]].Color;
		if (RGBA_GETALPHA(colVertex)!=255) 
			Transparency=TRUE;
		lpM=VBuffer[VGroups[i]].Material;
		if (lpM!=NULL){
			lpM->lpVtbl->GetEmissive(lpM, &REm, &GEm, &BEm);
			lpM->lpVtbl->GetSpecular(lpM, &RSpec, &GSpec, &BSpec);
			SExp=lpM->lpVtbl->GetPower(lpM);
			GroupsMaterials[i]= NewMaterial(colVertex,RSpec,GSpec,BSpec,REm,GEm,BEm,SExp);
		} else 
			GroupsMaterials[i]= NewMaterial(colVertex,0,0,0,0,0,0,0);
	}
	for (i=0; i<NGroups; i++){
		if (FBuffer[Groups[i]].Texture!=NULL){
			GroupsWrapu[i]=FBuffer[Groups[i]].Wrapu;
			GroupsWrapv[i]=FBuffer[Groups[i]].Wrapv;
			GroupsTextures[i]=GetTextureHandle(FBuffer[Groups[i]].Texture, &Surf, &ComObj, &Name);
			if (TextureTransparent(Surf)) {
				Transparency=TRUE;
				GroupsTransp[i]=TRUE;
			}	else 
				GroupsTransp[i]=FALSE;
		if (TextureColorKey(Name)) 
			ColorKey=TRUE;
		}	else{
			GroupsWrapu[i]=FALSE;
			GroupsWrapv[i]=FALSE;
			GroupsTextures[i]=PGE_DX_NOHANDLE;
		}
	}
	ReorderFGroups();
}

///////////////////////
//PrepareFrame
static LPDIRECT3DRMFRAME2 RootFrame;
static LPDIRECT3DRMFRAME RootFrame1;

void PrepareFrameRec(LPDIRECT3DRMFRAME2 lpFrame, D3DMATRIX *lpMF){
	BOOL MatrixCopied=FALSE;	
	DWORD i,Size;//int i;
	LPDIRECT3DRMVISUALARRAY lpVisuals;
	LPDIRECT3DRMVISUAL Visual;
	LPDIRECT3DRMMESHBUILDER2 lpMB;
	//	LPDIRECT3DRMMESHBUILDER lpMB1;
	LPDIRECT3DRMFRAMEARRAY lpChildrens;
	LPDIRECT3DRMFRAME2 lpCFrame2;
	LPDIRECT3DRMFRAME lpCFrame;
	D3DMATRIX M;     
	LPDIRECT3DRMOBJECT lpD3DO;
	DWORD SizeName=PGE_NAME_SIZE;
	char Name[PGE_NAME_SIZE];
	int rcount;
	if (lpFrame==NULL) return;

	lpFrame->lpVtbl->QueryInterface(lpFrame, &IID_IDirect3DRMObject, &lpD3DO);
	rcount=lpD3DO->lpVtbl->Release(lpD3DO);
	lpD3DO->lpVtbl->GetName(lpD3DO, &SizeName, Name);
	MYEmissive=  ((strlen(Name)>1)&&(Name[0]==EMCHAR0)&&(Name[1]==EMCHAR1));
	MYAEmissive= ((strlen(Name)>1)&&(Name[0]==AEMCHAR0)&&(Name[1]==AEMCHAR1));
		
	if ((strlen(Name)>DUMMYFRAMEPOS1)&&
				Name[DUMMYFRAMEPOS0]==DUMMYFRAMECHAR0&&
			Name[DUMMYFRAMEPOS1]==DUMMYFRAMECHAR1) 
		return;
	ObjTable[NObjs].Matrix=PGE_DX_NOHANDLE;
	if (RootFrame==lpFrame) 
		memcpy(&M, lpMF, sizeof(D3DMATRIX));
	else   
		FindTransform(RootFrame1, lpFrame, &M, lpMF);

	lpFrame->lpVtbl->GetVisuals(lpFrame, &lpVisuals);
	Size=lpVisuals->lpVtbl->GetSize(lpVisuals);
	for (i=0; i<Size; i++){
		//		HRESULT ddrval;
		lpVisuals->lpVtbl->GetElement(lpVisuals, i , &Visual);

		////////
		/*ddrval=Visual->lpVtbl->QueryInterface(Visual,
		&IID_IDirect3DRMMeshBuilder, &lpMB1);
		MYERROR(7000,ddrval,__FILE__,__LINE__,OK);*/
		if (S_OK==Visual->lpVtbl->QueryInterface(Visual,
											&IID_IDirect3DRMMeshBuilder2, &lpMB))	{
			if(lpMB->lpVtbl->GetFaceCount(lpMB)) {
				if (!MatrixCopied&&!(RootFrame==lpFrame)){
					ObjTable[NObjs].Matrix=CreateMatrix();
					lpD3DDevice->lpVtbl->SetMatrix(lpD3DDevice,
													 ObjTable[NObjs].Matrix,
													 &M);
					ObjTable[NObjs].FrameName=MyMalloc(char, 1+strlen(Name));
					strcpy(ObjTable[NObjs].FrameName, Name);
					FramesToMove[NFramesToMove]=lpFrame;
					NFramesToMove++;
					MatrixCopied=TRUE;
				}	else 
					ObjTable[NObjs].Matrix=PGE_DX_NOHANDLE;
				PrepareBuffers(lpMB);
			}
			rcount=lpMB->lpVtbl->Release(lpMB);
		}
		rcount=Visual->lpVtbl->Release(Visual);
	}
	rcount=lpVisuals->lpVtbl->Release(lpVisuals);

	lpFrame->lpVtbl->GetChildren(lpFrame, &lpChildrens);
	Size=lpChildrens->lpVtbl->GetSize(lpChildrens);
	for (i=0; i<Size; i++){
		lpChildrens->lpVtbl->GetElement(lpChildrens, i , &lpCFrame);
		lpCFrame->lpVtbl->QueryInterface(lpCFrame,
											&IID_IDirect3DRMFrame2, &lpCFrame2);
		PrepareFrameRec(lpCFrame2, &M);
		lpCFrame->lpVtbl->Release(lpCFrame);
		rcount=lpCFrame2->lpVtbl->Release(lpCFrame2);
	}
	rcount=lpChildrens->lpVtbl->Release(lpChildrens);
}

void PrepareFrameHerarchy(LPDIRECT3DRMFRAME2 lpFrame){
	D3DMATRIX M;
	int temp;
	//make identity matrix
	PGE_MAKE_IDENTITY_D3DMATRIX(M); 
	RootFrame=lpFrame;
	RootFrame->lpVtbl->QueryInterface(RootFrame, &IID_IDirect3DRMFrame, &RootFrame1);
	temp=RootFrame1->lpVtbl->Release(RootFrame1);
	InitConversion();    
	PrepareFrameRec(lpFrame, &M);
}



/////////////////////Building execute Buffer//////////////////
int VerticesProcessing(void){
	int NMatrices=0;
	int NVertices=VGroupsDim[0];
	int NMaterials=1;
	int NVertexProcInstructions=1;
	char NewDataGroup;
	D3DMATERIALHANDLE PrevMaterial=GroupsMaterials[0];
	int Obj, Group;
	if (ObjTable[0].Matrix!=0) NMatrices++;
	Obj=1;
	for (Group=1; Group<NVGroups; Group++){
		NVertices+=VGroupsDim[Group];
		NewDataGroup=FALSE;
		if(GroupsMaterials[Group]!=PrevMaterial){
			NewDataGroup=TRUE;
			NMaterials++;
			PrevMaterial=GroupsMaterials[Group];
		}
		if ((Obj<NObjs)&&(Group==ObjTable[Obj].FirstVGroup)){
			NewDataGroup=TRUE;
			if (ObjTable[Obj].Matrix!=0) NMatrices++;
			Obj++;
		}
		if (NewDataGroup) NVertexProcInstructions++;
	}
	return NMatrices*(sizeof (D3DINSTRUCTION)+sizeof (D3DMATRIXMULTIPLY))+
			NMaterials*(sizeof (D3DINSTRUCTION)+sizeof (D3DSTATE))+
			NVertexProcInstructions*(sizeof(D3DINSTRUCTION)+sizeof(D3DPROCESSVERTICES))+
			NVertices*sizeof(D3DVERTEX);
}   

int FacesOverHead(void){    
	int TextureChanges=1;
	int i, NWrap;
	D3DTEXTUREHANDLE PrevTexture;
	BOOL PrevWrapu=GroupsWrapu[0];
	BOOL PrevWrapv=GroupsWrapv[0];

	PrevTexture=GroupsTextures[0];
	NWrap=2;
	for(i=1; i<NGroups; i++){      
		if (PrevTexture!=GroupsTextures[i]) {
			PrevTexture=GroupsTextures[i];
			TextureChanges++;
			if (PrevWrapu!=GroupsWrapu[i]){
				NWrap++;
				PrevWrapu=GroupsWrapu[i];
			}
			if (PrevWrapv!=GroupsWrapv[i]){
				NWrap++;
				PrevWrapv=GroupsWrapv[i];
			}         
		}
	}
	return
			TextureChanges*(3*sizeof(D3DINSTRUCTION)+sizeof(D3DSTATE))+
				NWrap*sizeof(D3DSTATE);	//one D3DINSTRUCTION more to account for possible qword align
}

int ComputeExecuteBufferDim(void){
	return NFaces*sizeof (D3DTRIANGLE)+
					FacesOverHead()+ //material, and texture changes
													 //triangles processing instructions
													 //triangles alignement
					VerticesProcessing()+	//matrix multiply and groups of vertices
																//processing instructions;
					sizeof (D3DINSTRUCTION); //Dimension of Exit instruction
}


int CopyVertices(void **p){
	int i, j;
	D3DVERTEX *pv=(D3DVERTEX *)*p;
	D3DVERTEX *pvi=(D3DVERTEX *)*p;
	D3DVERTEX *cv;
	LPMVERTEX lpCVertex;
	int CurrIndex;
	for(i=0; i<NVGroups; i++){
		CurrIndex=VGroups[i];
		for(j=VGroupsDim[i]-1; j>=0; j--){
			cv=pv+j;
			lpCVertex=VBuffer+CurrIndex;
			PGE_MAKE_D3DVERTEX_FROM_MVERTEX(cv, lpCVertex);
			CurrIndex=lpCVertex->NextInGroup;
			lpCVertex->ExecuteBIndex=cv-pvi;
		}
		pv+=VGroupsDim[i];
	}
	*p=( void *)pv;
	return (pv-pvi);
}

void GenerateVerticesInstructions(void **p){
	char NewDataGroup;
	int Group, Obj;
	int PrevVIndex=0;
	int NVertices=VGroupsDim[0];
	D3DMATERIALHANDLE PrevMaterial=GroupsMaterials[0];
	if (ObjTable[0].Matrix!=0){
		ObjTable[0].VertexNumber=0;
		OP_MATRIX_MULTIPLY(1, *p);
		MATRIX_MULTIPLY_DATA(ObjTable[0].Matrix, RMatrix, ExecuteMatrix, *p);
	}
	OP_STATE_LIGHT(1, *p);
	STATE_DATA(D3DLIGHTSTATE_MATERIAL, PrevMaterial, *p);
	Obj=1;
	for (Group=1; Group<NVGroups; Group++){
		NewDataGroup=FALSE;
		if(GroupsMaterials[Group]!=PrevMaterial){
			OP_PROCESS_VERTICES(1, *p);
			PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT,
					PrevVIndex, NVertices, *p);
			PrevVIndex+=NVertices;
			NVertices=0;
			NewDataGroup=TRUE;
			PrevMaterial=GroupsMaterials[Group];
			OP_STATE_LIGHT(1, *p);
			STATE_DATA(D3DLIGHTSTATE_MATERIAL, PrevMaterial, *p);

		}
		if ((Obj<NObjs)&&(Group==ObjTable[Obj].FirstVGroup)){
			if (!NewDataGroup){
				OP_PROCESS_VERTICES(1, *p);
				PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT|
									D3DPROCESSVERTICES_UPDATEEXTENTS ,
									PrevVIndex, NVertices, *p);
				PrevVIndex+=NVertices;
				NVertices=0;
			}
			if (ObjTable[Obj].Matrix!=0) {
				ObjTable[Obj].VertexNumber=PrevVIndex;
				OP_MATRIX_MULTIPLY(1, *p);
				MATRIX_MULTIPLY_DATA(ObjTable[Obj].Matrix, RMatrix, ExecuteMatrix, *p);
			}
			Obj++;
		}
		NVertices+=VGroupsDim[Group];
	}
	OP_PROCESS_VERTICES(1, *p);
	PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORMLIGHT, PrevVIndex, NVertices, *p);
}

//returns in ctt->v? the indexes in the execute buffer of mface vertexes
__inline PGEMakeD3DTRIANGLEFromMFACE(LPMFACE  lpCTriangle, D3DTRIANGLE *lp_ctt){	
	lp_ctt->v1=VBuffer[lpCTriangle->Vertices[0]].ExecuteBIndex;
	lp_ctt->v2=VBuffer[lpCTriangle->Vertices[1]].ExecuteBIndex;
	lp_ctt->v3=VBuffer[lpCTriangle->Vertices[2]].ExecuteBIndex;
	lp_ctt->wFlags=0;
}

void CopyTriangles(void **p, int g1, int g2){
	int j;
	D3DTRIANGLE *ct=(D3DTRIANGLE *)*p;
	LPMFACE lpCTriangle;
	int Group, CurrIndex;
	for(Group=g1; Group<g2; Group++){
		CurrIndex=Groups[Group];
		for(j=GroupsDim[Group]-1; j>=0; j--){
			lpCTriangle=FBuffer+CurrIndex;
			PGEMakeD3DTRIANGLEFromMFACE(lpCTriangle, ct+j);
			CurrIndex=lpCTriangle->NextFace;
		}
		ct+=GroupsDim[Group];
	}
	*p=(void *)ct;
}

__inline PGEBuildingObjAllocate(void){
	BuildingObj->Triangles=MyMalloc(int, NGroups);
	BuildingObj->Textures=MyMalloc(unsigned char, NGroups);
	BuildingObj->NTextures=0;
}

__inline PGEBuildingObjAdd(void **p, D3DTEXTUREHANDLE PrevTexture){
	BuildingObj->Triangles[BuildingObj->NTextures]=(char *)(*p)-CurrExecuteBufferStart;
	BuildingObj->Textures[BuildingObj->NTextures]=FindTextureProperty(PrevTexture);
	BuildingObj->NTextures++;
}

void GenerateTrianglesInstructions(void **p){
	int NTriangles=GroupsDim[0];
	int Group;
	int PrevVIndex=0;
	int NStateI;
	D3DTEXTUREHANDLE PrevTexture=GroupsTextures[0];
	BOOL PrevWrapu=GroupsWrapu[0];
	BOOL PrevWrapv=GroupsWrapv[0];

	PGEBuildingObjAllocate();

	OP_STATE_RENDER(3, *p);
	STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, PrevTexture, *p);
	STATE_DATA(D3DRENDERSTATE_WRAPU, PrevWrapu, *p);
	STATE_DATA(D3DRENDERSTATE_WRAPV, PrevWrapv, *p);
	for(Group=1; Group<NGroups; Group++){
		if (PrevTexture!=GroupsTextures[Group]){
			if (QWORD_ALIGNED(*p)){//curly brace must be there because OP_NOP is bad implemented
				OP_NOP(*p);
			}
			OP_TRIANGLE_LIST(NTriangles, *p);
			CopyTriangles(p, PrevVIndex, Group);
			PGEBuildingObjAdd(p,PrevTexture);
			PrevVIndex=Group;
			NTriangles=0;
			NStateI=1;
			if (GroupsWrapu[Group]!=PrevWrapu) NStateI++;
			if (GroupsWrapv[Group]!=PrevWrapv) NStateI++;
			OP_STATE_RENDER(NStateI, *p);
			PrevTexture=GroupsTextures[Group] ;
			STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, PrevTexture, *p);
			if (GroupsWrapu[Group]!=PrevWrapu) {
				PrevWrapu=GroupsWrapu[Group];
				STATE_DATA(D3DRENDERSTATE_WRAPU, PrevWrapu, *p);
			}
			if (GroupsWrapv[Group]!=PrevWrapv) {
				PrevWrapv=GroupsWrapv[Group];
				STATE_DATA(D3DRENDERSTATE_WRAPV, PrevWrapv, *p);
			}
		}
		NTriangles+=GroupsDim[Group];
	}
	if (QWORD_ALIGNED(*p)) {//curly brace must be there because OP_NOP is bad implemented
		OP_NOP(*p);
	}
	OP_TRIANGLE_LIST(NTriangles, *p);
	CopyTriangles(p, PrevVIndex, Group);
	PGEBuildingObjAdd(p,PrevTexture);
}


void SetTransparencyType(LPMYOBJ lpOb){
	if ((lpOb->Name[2]==BACKCHAR0)&&	(strlen(lpOb->Name)>3)){
		if (lpOb->Name[3]=='0') 
			lpOb->Flags|=BACK_TRANSPARENT;
		else if (lpOb->Name[3]=='1') 
			lpOb->Flags|=BACK_M_TRANSPARENT;
	}
}

void FillExecuteBuffer(void *p, D3DEXECUTEDATA *ExecuteData){
	void *pin=p;
	void *StartInstr;
	CurrExecuteBufferStart=(char *)p;
	memset(p, 0, Descr.dwBufferSize);
	ExecuteData->dwSize=sizeof(D3DEXECUTEDATA);
	ExecuteData->dwVertexOffset=ExecuteData->dwHVertexOffset=0;
	ExecuteData->dwVertexCount=CopyVertices(&p);
	StartInstr=p;
	ExecuteData->dwInstructionOffset=(char *)p-(char *)pin;
	GenerateVerticesInstructions(&p);
	GenerateTrianglesInstructions(&p);
	OP_EXIT(p);
	ExecuteData->dwInstructionLength=(char *)p-(char *)StartInstr;      
}

LPDIRECT3DEXECUTEBUFFER CurrExecuteBuffer;

void AllocateExecuteBuffer(void){
	HRESULT Res;
	memset(&Descr, 0, sizeof(D3DEXECUTEBUFFERDESC));
	Descr.dwSize=sizeof(D3DEXECUTEBUFFERDESC);
	Descr.dwBufferSize=ComputeExecuteBufferDim();
	Descr.dwFlags=D3DDEB_BUFSIZE;
	Res=lpD3DDevice->lpVtbl->CreateExecuteBuffer(lpD3DDevice,
																 &Descr, &CurrExecuteBuffer, NULL);
	myassert(Res==D3D_OK, 0, 0);
}

void DoExecuteBuffer(LPMYOBJ Ob){
	BuildingObj=Ob;
	AllocateExecuteBuffer();
	Ob->lpExecuteBuffer=CurrExecuteBuffer;
	while (CurrExecuteBuffer->lpVtbl->Lock(CurrExecuteBuffer, &Descr)==	DDERR_WASSTILLDRAWING);
	// FillExecuteBuffer(executedata, &(Ob->ExecuteData));
	FillExecuteBuffer((Descr.lpData), &(Ob->ExecuteData));
	CurrExecuteBuffer->lpVtbl->Unlock(CurrExecuteBuffer);
	CurrExecuteBuffer->lpVtbl->SetExecuteData(CurrExecuteBuffer, &(Ob->ExecuteData));
	CopyMatrixPointers(Ob);
	if (strlen(Ob->Name)>3){
		if(Ob->Name[2]==SPRITECHAR&&Ob->Name[3]==SPRITECHAR0) DeclareSprite3D(Ob);
		if(Ob->Name[2]==SPRITECHAR&&Ob->Name[3]==SPRITECHAR1) DeclareSprite3D1(Ob);
	}	
	if (strlen(Ob->Name)>1){
		if(Ob->Name[0]==COLLCHAR0&&Ob->Name[1]==COLLCHAR1)
			Ob->Flags|=ONLY_FOR_COLLISIONS;
	}
	SetTransparencyType(Ob);	
}

//TD 6-10-00 only for shadow ??
void PrepareMatrices(LPMYOBJ refob){
	int i, j;
	BOOL found;
	for (i=0; i<NObjs; i++){
		found=FALSE;
		if(ObjTable[i].Matrix!=PGE_DX_NOHANDLE){
			for (j=0; j<refob->NMatrices; j++){
				if (strcmp(ObjTable[i].FrameName, refob->FrameNames[j])==0){
					found=TRUE;
					ObjTable[i].Matrix=refob->Matrices[j];
					break;
				}
			}
			myassert(found, 0, 0);
		}
	}
}

//------
#define  PGETranslateExecuteObj(Ob, GETBOX){\
	PrepareTexturesAndMaterials();\
	DoExecuteBuffer(Ob);\
	Ob->ValidBox=(Ob->ExecuteData.dwVertexCount>MIN_VERTICES_BBOX/*||Transparency*/);\
	GETBOX;\
	if (Transparency) Ob->Flags|=TRANSPARENTOBJ;\
	if (ColorKey) Ob->Flags|=COLORKEY;\
}

void TranslateFrameHerarchy(LPMYOBJ Ob, LPDIRECT3DRMFRAME2 lpFrame){
	PrepareFrameHerarchy(lpFrame);
	//PrepareTexturesAndMaterials();///???TD 20 -09-00 l'ho postposto da controllare
	if (TranslatingShadow) PrepareMatrices(ReferenceObj);
	PGETranslateExecuteObj(Ob,
			(lpFrame->lpVtbl->GetHierarchyBox(lpFrame, &(Ob->BoundingBox))));
}

void TranslateMeshBuilder(LPMYOBJ Ob, LPDIRECT3DRMMESHBUILDER2 lpMB){
	InitConversion();
	PrepareBuffers(lpMB);
	ObjTable[0].Matrix=PGE_DX_NOHANDLE;
		
	PGETranslateExecuteObj(Ob,
				(lpMB->lpVtbl->GetBox(lpMB, &(Ob->BoundingBox))));	
}

LPMYOBJ LoadObjFromMesh(LPDIRECT3DRMMESHBUILDER2 lpMB){
	LPMYOBJ Ob;
	LPDIRECT3DRMOBJECT lpD3DO;
	DWORD Size=PGE_NAME_SIZE;
	char Name[PGE_NAME_SIZE];
	lpMB->lpVtbl->QueryInterface(lpMB, &IID_IDirect3DRMObject, &lpD3DO);
	lpD3DO->lpVtbl->Release(lpD3DO);
	lpD3DO->lpVtbl->GetName(lpD3DO, &Size, Name);
	Ob=NewObj(Name);
	MYEmissive= ((strlen(Name)>1)&&(Name[0]==EMCHAR0)&&(Name[1]==EMCHAR1)) ;		
	MYAEmissive=((strlen(Name)>1)&&(Name[0]==AEMCHAR0)&&(Name[1]==AEMCHAR1));		
	TranslateMeshBuilder(Ob, lpMB);
	return Ob;
}

LPMYOBJ LoadObjFromFrame(LPDIRECT3DRMFRAME2 lpFrame, char *Name){
	LPMYOBJ Ob;
	Ob=NewObj(Name);
	TranslateFrameHerarchy(Ob, lpFrame);
	return Ob;
}

static LPDIRECT3DRMFRAME2 lpRootLoading2;
static LPDIRECT3DRMFRAME  lpRootLoading;

LPMYFRAME LoadFrameHerarchyRec(LPDIRECT3DRMFRAME2 lpFrame, LPDIRECT3DRMFRAME lpFather){	
	DWORD i,Size;//int i;
	LPDIRECT3DRMVISUALARRAY lpVisuals;
	LPDIRECT3DRMVISUAL Visual;
	LPDIRECT3DRMMESHBUILDER2 lpMB;
	LPDIRECT3DRMFRAMEARRAY lpChildrens;
	LPDIRECT3DRMFRAME2 lpCFrame2;
	LPDIRECT3DRMFRAME lpCFrame, lpFrame1;
	LPMYFRAME lpMyFrame, lpMyFrameT;
	LPDIRECT3DRMOBJECT lpD3DO;
	DWORD SizeName=PGE_NAME_SIZE;
	char Name[PGE_NAME_SIZE];
	int rcount;
	if (lpFrame==NULL) return NULL;
	lpMyFrame=NULL;
	lpFrame->lpVtbl->QueryInterface(lpFrame, &IID_IDirect3DRMFrame, &lpFrame1);
	lpFrame->lpVtbl->Release(lpFrame);
	lpFrame->lpVtbl->QueryInterface(lpFrame, &IID_IDirect3DRMObject, &lpD3DO);
	lpD3DO->lpVtbl->Release(lpD3DO);
	lpD3DO->lpVtbl->GetName(lpD3DO, &SizeName, Name);
	if ((strlen(Name)>DUMMYFRAMEPOS1)&&	Name[DUMMYFRAMEPOS0]==DUMMYFRAMECHAR0 &&	
				Name[DUMMYFRAMEPOS1]==DUMMYFRAMECHAR1) {
		lpMyFrame=NULL;
		return NULL;
	}

	if (((strlen(Name)>1)&&	(Name[0]==AFRAMECHAR0)&&	((Name[1]<='9')&&(Name[1]>='0')))	||
						((lpD3DO->lpVtbl->GetAppData(lpD3DO))&FRAME_COMPILE)){
		lpMyFrame=NewFrame(Name);
		AddObjToFrame(LoadObjFromFrame(lpFrame, Name), lpMyFrame);
		if (lpRootLoading2!=lpFrame){
			FindTransform1(lpFather, lpFrame, &(lpMyFrame->Matrix));
		}
		return lpMyFrame;
	}
	if (((strlen(Name)>1)&&	Name[0]==AFRAMECHAR1&&Name[1]==AFRAMECHAR2)	){
		LPMYFRAME TFrame;
		char Name1[30];
		strcpy(Name1, Name);
		Name1[0]='B';
		lpMyFrame=NewFrame(Name1);
		TFrame=NewFrame(Name);
		//make identity matrix
		PGE_MAKE_IDENTITY_D3DMATRIX(lpMyFrame->Matrix);
		AddFrameToFrame(TFrame, lpMyFrame);
		AddObjToFrame(LoadObjFromFrame(lpFrame, Name), TFrame);

		if (lpRootLoading2!=lpFrame){
			FindTransform1(lpFather, lpFrame, &(TFrame->Matrix));
		}
		return lpMyFrame;
	}
	lpFrame->lpVtbl->GetVisuals(lpFrame, &lpVisuals);
	Size=lpVisuals->lpVtbl->GetSize(lpVisuals);
	for (i=0; i<Size; i++){
		lpVisuals->lpVtbl->GetElement(lpVisuals, i , &Visual);
		if (S_OK==Visual->lpVtbl->QueryInterface(Visual,
														&IID_IDirect3DRMMeshBuilder2, &lpMB)){
    
			if(lpMB->lpVtbl->GetFaceCount(lpMB)) {
				if (lpMyFrame==NULL)lpMyFrame=NewFrame(Name);
					AddObjToFrame(LoadObjFromMesh(lpMB), lpMyFrame);
			}
			lpMB->lpVtbl->Release(lpMB);
		}
		rcount=Visual->lpVtbl->Release(Visual);
	}
	lpVisuals->lpVtbl->Release(lpVisuals);

	lpFrame->lpVtbl->GetChildren(lpFrame, &lpChildrens);
	Size=lpChildrens->lpVtbl->GetSize(lpChildrens);
	for (i=0; i<Size; i++){
		lpChildrens->lpVtbl->GetElement(lpChildrens, i , &lpCFrame);
		lpCFrame->lpVtbl->QueryInterface(lpCFrame,
														&IID_IDirect3DRMFrame2, &lpCFrame2);
		lpMyFrameT=LoadFrameHerarchyRec(lpCFrame2, lpFrame1);
		if (lpMyFrameT){
			if (lpMyFrame==NULL)lpMyFrame=NewFrame(Name);
				AddFrameToFrame(lpMyFrameT, lpMyFrame);
		}
		lpCFrame->lpVtbl->Release(lpCFrame);  
		lpCFrame2->lpVtbl->Release(lpCFrame2);    
	}
	lpChildrens->lpVtbl->Release(lpChildrens);
	if (lpMyFrame!=NULL){
		if (lpRootLoading2!=lpFrame){
			FindTransform1(lpFather, lpFrame, &(lpMyFrame->Matrix));
		}
	}
	return lpMyFrame;   
}

LPMYFRAME LoadFrameHerarchy(LPDIRECT3DRMFRAME2 lpFrame){
	lpRootLoading2=lpFrame;
	lpRootLoading2->lpVtbl->QueryInterface(lpRootLoading2, &IID_IDirect3DRMFrame, &lpRootLoading);
	lpRootLoading->lpVtbl->Release(lpRootLoading);
	return LoadFrameHerarchyRec(lpFrame, lpRootLoading);
}

void TagForHerarchyCompile(LPDIRECT3DRM2 lpD3DRM2, char *Name){
	LPDIRECT3DRMOBJECT lpObj;    
	lpD3DRM2->lpVtbl->GetNamedObject(lpD3DRM2, Name, &lpObj);
	lpObj->lpVtbl->SetAppData(lpObj, FRAME_COMPILE);
	lpObj->lpVtbl->Release(lpObj);
}



void InitLoad(void){
	BOOL res;
	res=Initialize_Conversion(LoadTextures);
	myassert(res, 0, 0);
}

void CloseLoad(void){Close_Conversion();}


LPMYOBJ FileLoadObj(char *filename){
	char tname[_MAX_PATH]=MYMODELPATH;
	LPDIRECT3DRMMESHBUILDER2 meshb;
	LPMYOBJ res;
	int temp;
#ifdef MYVIEWER
	strcpy(tname, PathModels);
#endif
	if (strlen(tname)>0)tname[0]=drive_context;
	strcat(tname, filename);
	
	/*lpD3DRMRoot->lpVtbl->CreateMeshBuilder(lpD3DRMRoot, &meshb);
	meshb->lpVtbl->Load(meshb, tname, NULL, D3DRMLOAD_FROMFILE, LoadTextures,NULL);			*/
	meshb=Load_Mesh(lpD3DRM1Root, tname);
	res=LoadObjFromMesh(meshb);
	temp=meshb->lpVtbl->Release(meshb);
	return res;
}

LPMYOBJ FileLoadComplexObj(char *filename, char *Name){
	char tname[_MAX_PATH]=MYMODELPATH;
	LPDIRECT3DRMFRAME2 lpFrame2;
	LPMYOBJ res;
	int temp;
	#ifdef MYVIEWER
		strcpy(tname, PathModels);
	#endif
	if (strlen(tname)>0)tname[0]=drive_context;
	strcat(tname, filename);
	lpD3DRMRoot->lpVtbl->CreateFrame(lpD3DRMRoot, NULL, &lpFrame2);
	/*lpFrame2->lpVtbl->Load(lpFrame2, tname, NULL, D3DRMLOAD_FROMFILE, LoadTextures,   NULL);*/
	Load_Frame(lpD3DRM1Root, lpFrame2, tname);
	res=LoadObjFromFrame(lpFrame2, Name);
	temp=lpFrame2->lpVtbl->Release(lpFrame2);
	return res;
}

LPMYOBJ FileLoadComplexShadow(char *filename, char *Name, LPMYOBJ Ref){
	LPMYOBJ Tobj;
	TranslatingShadow=TRUE;
	ReferenceObj=Ref;
	Tobj=FileLoadComplexObj(filename, Name);
	TranslatingShadow=FALSE;
	return Tobj;
}

LPMYOBJ FileLoadComplexCollidingObj(char *filename, char *Name, LPMYOBJ Ref){
	LPMYOBJ Tobj;
	TranslatingShadow=TRUE;
	CollidingObj=TRUE;
	ReferenceObj=Ref;
	Tobj=FileLoadComplexObj(filename, Name);
	TranslatingShadow=FALSE;
	CollidingObj=FALSE;
	return Tobj;
}

 
LPMYFRAME FileLoadFrameHerarchy(char *filename){
	char tname[_MAX_PATH]=MYMODELPATH;
	LPDIRECT3DRMFRAME2 lpFrame2;
	LPMYFRAME res;
	#ifdef MYVIEWER
	strcpy(tname, PathModels);
	#endif
	if (strlen(tname)>0)tname[0]=drive_context;
	strcat(tname, filename);
	lpD3DRMRoot->lpVtbl->CreateFrame(lpD3DRMRoot, NULL, &lpFrame2);
	/*lpFrame2->lpVtbl->Load(lpFrame2, tname, NULL, D3DRMLOAD_FROMFILE, LoadTextures,
                                                 NULL);*/
	Load_Frame(lpD3DRM1Root, lpFrame2, tname);
	res=LoadFrameHerarchy(lpFrame2);
	lpFrame2->lpVtbl->Release(lpFrame2);
	return res;

}



/////////////////////////////////////////////////////

void PGEReleaseScene(int animTag){//void
	//ResetCenterTable();	
	ReleaseAllTAnimations();
	ReleaseAllAnimations(animTag);//0
	ReleaseAllTextures();
	ReleaseAllMaterials();
	ReleaseAllFrames();
	ReleaseAllObjs();
	ReleaseCommandBuffer();
	PGEReleaseMainLight();
}

void Release3d(void){
	PGEReleaseMainLight();
	ReleaseScene();
}

////////////////////SCREEN LAYER////////////////

void DeclareScreenLayer(LPMYFRAME x, BOOL CanBeClipped){
	x->Flags=x->Flags|SCREENLAYER;
	if (!CanBeClipped ) x->Flags=x->Flags|ALWAYSINTO;
}

void DeclareScreenLayer3d(LPMYFRAME x, BOOL CanBeClipped){
	x->Flags|=SCREENLAYER | IS3DLAYER;	
	if (!CanBeClipped) 
		x->Flags|=ALWAYSINTO;
}
/////////////////////////////////////////////
//////////////////////////////////////////

////////////////////////////////////////

////////finding object centers/////////
/*
//NOT USED
static CENTERTABLE *CurrCenterTable;

void FindCenters(CENTERTABLE *T){
	CurrCenterTable=T;
}

void ResetCenterTable(void){
	CurrCenterTable=NULL;
}

void StopFindCenters(void){
	CurrCenterTable=NULL;
}

void PGEComputeCenterTable(LPDIRECT3DRMMESHBUILDER2 lpMB){
	if (CurrCenterTable){
		int Is;
		LPDIRECT3DRMOBJECT lpD3DO;
		DWORD Size=PGE_NAME_SIZE;
		char Name[PGE_NAME_SIZE];
		lpMB->lpVtbl->QueryInterface(lpMB, &IID_IDirect3DRMObject, &lpD3DO);
		lpD3DO->lpVtbl->Release(lpD3DO);
		lpD3DO->lpVtbl->GetName(lpD3DO, &Size, Name);
		for (Is=0; Is<CurrCenterTable->N; Is++){ 
			if (strcmp(Name, CurrCenterTable->Name[Is])==0){
				D3DRMBOX BoundingBox;
				lpMB->lpVtbl->GetBox(lpMB, &BoundingBox);
				CurrCenterTable->Center[Is].x=(BoundingBox.min.x+BoundingBox.max.x)*0.5;
				CurrCenterTable->Center[Is].y=(BoundingBox.min.y+BoundingBox.max.y)*0.5;
				CurrCenterTable->Center[Is].z=(BoundingBox.min.z+BoundingBox.max.z)*0.5;
				break;
			}
		}
	}
}
*/
//////////////////////////////////////////function for a temptative Morphing not ended/////////////////
/*
void PGEMorphSetVertices(LPMYOBJ lpOb, D3DVERTEX *lpV, int Start, int End){
	HRESULT H;
	LPDIRECT3DEXECUTEBUFFER EB;
	if (lpOb==NULL) return;
	memset(&Descr, 0, sizeof(D3DEXECUTEBUFFERDESC));
	Descr.dwSize=sizeof(D3DEXECUTEBUFFERDESC);
	EB=lpOb->lpExecuteBuffer;
	while((H=EB->lpVtbl->Lock(EB, &Descr))==DDERR_WASSTILLDRAWING) ;
	memcpy(((char *)Descr.lpData)+Start*sizeof(D3DVERTEX), (void *)lpV, (End-Start+1)*sizeof(D3DVERTEX));
	EB->lpVtbl->Unlock(EB);
}

void PGEMorphGetVertices(LPMYOBJ lpOb, D3DVERTEX *lpV, int Start, int End){
	HRESULT H;
	LPDIRECT3DEXECUTEBUFFER EB;
	if (lpOb==NULL) return;
	memset(&Descr, 0, sizeof(D3DEXECUTEBUFFERDESC));
	Descr.dwSize=sizeof(D3DEXECUTEBUFFERDESC);
	EB=lpOb->lpExecuteBuffer;
	while((H=EB->lpVtbl->Lock(EB, &Descr))==DDERR_WASSTILLDRAWING);
	memcpy((void *)lpV, ((char *)Descr.lpData)+Start*sizeof(D3DVERTEX), (End-Start+1)*sizeof(D3DVERTEX));
	EB->lpVtbl->Unlock(EB);
}*/
/////////////////////////////////////////////////////////////////
