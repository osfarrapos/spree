#include "myenv.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gset.h"
#include "tmdict.h"
#include "fastmat.h"
#include "conv.h"
#include "imcode.h"
#define DISAPPARITION_TIME 100


static WORD MainIndex[(MAX_STRIPS+1)*2-1]; 
static D3DMATRIX CameraMat;
void HandleMBlare(LPMYOBJ);



void PrepareMainIndex(int N){
	static int i, j;
	for (i=0; i<N; i++) 
		MainIndex[i]=i;
	for (i=N, j=N-2; j>=0; i++, j--)  MainIndex[i]=j; 	
}


void AttachMBlare(LPMYANIMATION lpAn, PREAL Start, PREAL Stop, 
					int NStrips, D3DMATRIXHANDLE Joint,
					D3DCOLOR Color, D3DTEXTUREHANDLE Texture, float Width,
					PREAL *Displ){
	D3DVALUE FinishKey;
	myassert(NStrips<MAX_STRIPS, 0, 0);
	lpAn->Joint=Joint;
	lpAn->NMBlare=NStrips;
	lpAn->Color=Color;
	lpAn->Texture=Texture;
	lpAn->StripWidth=Width;
	memcpy(lpAn->Displ, Displ, sizeof(MYVECTOR));
	if (Start>=lpAn->InitKey)
		lpAn->MBlareInit=Start;
	else 
		lpAn->MBlareInit=lpAn->InitKey;
	FinishKey=lpAn->InitKey+lpAn->IncKey*(float)(lpAn->NFrames-1);
	if (Stop<=FinishKey)
		lpAn->MBlareStop=Stop;
	else 
		lpAn->MBlareStop=FinishKey;
}

void DeclareMBlare(LPMYOBJ lpOb){
	if (lpOb==NULL) return;
	if (lpOb->MBlare!=NULL) return;
	lpOb->MBlare=MyMalloc(MBLARESTATE, 1);
	myassert(lpOb->MBlare!=NULL, MemoryError,0); 
	memset(lpOb->MBlare, 0, sizeof(MBLARESTATE));
	if ((strlen(lpOb->Name)>=5)&&(lpOb->Name[4]=='<'))
		lpOb->BeforeRender=HandleMBlare;
	else
		lpOb->AfterRender=HandleMBlare;
	if ((strlen(lpOb->Name)>=6)&&(lpOb->Name[5]=='<'))
		lpOb->MBlare->NoTors=TRUE;
	memset(lpOb->MBlare->Vertices, 0, (MAX_STRIPS+1)*2*sizeof(D3DLVERTEX));
/*	for (i=0; i<(MAX_STRIPS+1)*2; i+=2){
		lpOb->MBlare->Vertices[i].tu=0.;
	}
	for (i=1; i<(MAX_STRIPS+1)*2; i+=2){
		lpOb->MBlare->Vertices[i].tu=1.;
	}*/
}

void ReleaseMBlare(LPMYOBJ lpOb){
	if (lpOb==NULL) return;
	if (lpOb->MBlare!=NULL) MyFree(lpOb->MBlare);
	lpOb->MBlare=NULL;
}


static D3DVECTOR CV;
static D3DMATRIX TempMatrix, TempMatrix1, TempMatrix2;
static D3DMATRIX UnitMatrix={ 1., 0., 0., 0.,
							  0., 1., 0., 0.,
							  0., 0., 1., 0.,
							  0., 0., 0., 1.
					};
static MYVECTOR TempVect;
#define GET_NEXT_POINT(v) if (State->Joint==0)\
							 lpD3DDevice->lpVtbl->GetMatrix(lpD3DDevice, ExecuteMatrix,\
							 &TempMatrix2);\
						 else{\
							 lpD3DDevice->lpVtbl->GetMatrix(lpD3DDevice, State->Joint,\
							 &TempMatrix);\
							 lpD3DDevice->lpVtbl->GetMatrix(lpD3DDevice, ExecuteMatrix,\
							 &TempMatrix1);\
							 MatrixMultiply((PREAL *)&TempMatrix, (PREAL *)&TempMatrix1, (PREAL *)&TempMatrix2);\
						}\
						 VByMatrixMultiply(State->Displ, (PREAL *)&TempMatrix2, TempVect);\
						 memcpy(&(v), &TempVect, sizeof(D3DVECTOR))

/*#define GET_NEXT_POINT(v) \
							 lpD3DDevice->lpVtbl->GetMatrix(lpD3DDevice, State->Joint,\
							 &TempMatrix);\
							 lpD3DDevice->lpVtbl->GetMatrix(lpD3DDevice, ExecuteMatrix,\
							 &TempMatrix1);\
							 MatrixMultiply((PREAL *)&TempMatrix, (PREAL *)&TempMatrix1, (PREAL *)&TempMatrix2);\
						 VByMatrixMultiply(State->Displ, (PREAL *)&TempMatrix2, TempVect);\
						 memcpy(&(v), &TempVect, sizeof(D3DVECTOR))*/
#define GET_CURR_POINT() GET_NEXT_POINT(CV)
#define PUSH_NEW_POINT() State->LastPoints[0]=State->LastPoints[1];\
						  State->LastPoints[1]=CV

#define RENDERSTRIP()\
	lpD3DDevice->lpVtbl->SetMatrix(lpD3DDevice, ExecuteMatrix, &UnitMatrix);\
	PrepareMainIndex(State->VertexDrawn-State->FirstVertex);\
	lpD3DDevice2->lpVtbl->SetRenderState(lpD3DDevice2, \
						  D3DRENDERSTATE_TEXTUREHANDLE,\
						  State->Texture);\
	lpD3DDevice2->lpVtbl->DrawIndexedPrimitive(lpD3DDevice2, \
						  D3DPT_TRIANGLESTRIP, D3DVT_LVERTEX,\
						  (State->Vertices+State->FirstVertex),\
						  State->VertexDrawn-State->FirstVertex,\
						  MainIndex,\
						  State->VertexDrawn+State->VertexDrawn-1\
						  -State->FirstVertex-State->FirstVertex,\
						  D3DDP_DONOTUPDATEEXTENTS)

void UpdateTextureCoordinates(LPMBLARESTATE State){
	static int i;
	static float TU;
	for(i=State->VertexDrawn-1, TU=((State->FirstVertex)>>1)*State->TextureInc; 
					i>=State->FirstVertex; i-=2, TU+=State->TextureInc){
		State->Vertices[i].tv=TU;
		State->Vertices[i-1].tv=TU;
		State->Vertices[i].tu=.95;
		State->Vertices[i-1].tu=.05;
	}
}

#define CROSS_PRODUCT(VI1, VI2, VO)\
	VO.x=-VI1.y*VI2.z+VI1.z*VI2.y;\
	VO.y=-VI1.z*VI2.x+VI1.x*VI2.z;\
	VO.z=-VI1.x*VI2.y+VI1.y*VI2.x


static D3DVECTOR CV1, CV2, CV3;

BOOL ComputeNewStrip(LPMBLARESTATE State){
	
#define VSSUB(X, Y, Z) Z.x=Y.x-X.x; Z.y=Y.y-X.y; Z.z=Y.z-X.z	
	
		VSSUB(State->LastPoints[0], State->LastPoints[1], CV1);
		VSSUB(State->LastPoints[1], CV, CV2);
		if (State->NoTors){
			CV3.x=CameraMat._13;
			CV3.y=CameraMat._23;
			CV3.z=CameraMat._33;

		}
		else{
			CROSS_PRODUCT(CV1, CV2, CV3);
		}
		if ((MODULO3D((&CV3))<MIN_SCALE*(MODULO3D(&CV1)*MODULO3D(&CV2)))) {
									;			
			
		}
		else{
			CROSS_PRODUCT(CV3, CV2, (State->CV1));
			NORMALIZE3D(&(State->CV1));
		}	
	State->Vertices[State->VertexDrawn].x=State->CV1.x*State->StripWidth;
	State->Vertices[State->VertexDrawn].y=State->CV1.y*State->StripWidth;
	State->Vertices[State->VertexDrawn].z=State->CV1.z*State->StripWidth;
	State->Vertices[State->VertexDrawn].color=State->Color;

	State->Vertices[State->VertexDrawn+1].x=
		-State->Vertices[State->VertexDrawn].x;
	State->Vertices[State->VertexDrawn+1].y=
		-State->Vertices[State->VertexDrawn].y;
	State->Vertices[State->VertexDrawn+1].z=
		-State->Vertices[State->VertexDrawn].z;
	State->Vertices[State->VertexDrawn+1].color=State->Color;
	if (State->VertexDrawn<=2){
		memcpy((char *)State->Vertices, ((char *)State->Vertices)+2*sizeof(D3DLVERTEX), 
								2*sizeof(D3DLVERTEX));
		State->Vertices[0].x+=State->LastPoints[0].x;
		State->Vertices[0].y+=State->LastPoints[0].y;
		State->Vertices[0].z+=State->LastPoints[0].z;
		State->Vertices[1].x+=State->LastPoints[0].x;
		State->Vertices[1].y+=State->LastPoints[0].y;
		State->Vertices[1].z+=State->LastPoints[0].z;
	}
	State->Vertices[State->VertexDrawn].x+=CV.x;
	State->Vertices[State->VertexDrawn].y+=CV.y;
	State->Vertices[State->VertexDrawn].z+=CV.z;
	State->Vertices[State->VertexDrawn+1].x+=CV.x;
	State->Vertices[State->VertexDrawn+1].y+=CV.y;
	State->Vertices[State->VertexDrawn+1].z+=CV.z;
	return TRUE;

}
void HandleMBlare(LPMYOBJ lpOb){
	LPMBLARESTATE State=lpOb->MBlare;
	int Status, N;
	static ActTime;
	if (GameFrozen||NowReplaying) return;
	lpD3DDevice->lpVtbl->GetMatrix(lpD3DDevice, CurrCameraM, &CameraMat);
	Status=State->Status;
	if (Status==MB_SLEEPING) {
		State->MBOK=FALSE;
		return;
	}
	if (Status==MB_STARTED) {
		GET_NEXT_POINT(State->LastPoints[0]);
		State->Status=MB_ONE_POINT_VALID;
	//	lpOb->Flags=TRANSPARENTOBJ|lpOb->Flags; //OBROBRIOOOOO!!!!!!!!
		State->FirstVertex=0;
		State->CV1.x=PVALUE(0.);
		State->CV1.y=PVALUE(1.);
		State->CV1.z=PVALUE(0.);
		if (!(State->MBOK)) State->Status=MB_SLEEPING;
		State->MBOK=FALSE;
	}
	else if (Status==MB_ONE_POINT_VALID) {
		GET_NEXT_POINT(State->LastPoints[1]);
		State->Status=MB_TWO_POINTS_VALIDS;
		State->VertexDrawn=2;
		State->StripsDrawn=0;
		if (!(State->MBOK)) State->Status=MB_SLEEPING;
		State->MBOK=FALSE;

	}
	else if (Status==MB_TWO_POINTS_VALIDS) {
		State->PrevD=MyTime();
		if ((State->CTime<=State->LTime)||
			(!(State->MBOK))) {
			State->Status=MB_SLEEPING;
			RENDERSTRIP();
			State->MBOK=FALSE;
			return;
		}
		if (State->CTime-State->LTime>=State->IncTime){
			GET_CURR_POINT();
			if (ComputeNewStrip(State)) {
				
				State->StripsDrawn++;
				State->VertexDrawn+=2;
				UpdateTextureCoordinates(State);
				State->LTime+=State->IncTime;
				PUSH_NEW_POINT();
			}
		}
		RENDERSTRIP();
		State->MBOK=FALSE;
	}
	else if (State->Status==MB_DISAPPEARING){
		if (State->StripsDrawn>0){
			State->DeltaD=DISAPPARITION_TIME/State->StripsDrawn;
			if (State->DeltaD<=1) State->DeltaD=1;
			ActTime=MyTime();
			//ActTime=State->PrevD+State->DeltaD/10;
			N=(ActTime-State->PrevD)/State->DeltaD;
			if (N>0) 
				State->PrevD+=N*State->DeltaD;
			State->StripsDrawn-=N;
			State->FirstVertex+=(N+N);
		}
		
		
		if (State->StripsDrawn<=0) {
		//	lpOb->Flags=lpOb->Flags&(~TRANSPARENTOBJ);
			State->Status=MB_SLEEPING;
			return;
		}
		UpdateTextureCoordinates(State);
		RENDERSTRIP();
		State->Status=MB_DISAPPEARING1;
	}
	else if (State->Status==MB_DISAPPEARING1){
		//ActTime+=State->DeltaD/10;
		ActTime=MyTime();
		N=(ActTime-State->PrevD)/State->DeltaD;
		if (N>0){ 
			State->PrevD+=N*State->DeltaD;
			State->StripsDrawn-=N;
			State->FirstVertex+=(N+N);
		}
		if (State->StripsDrawn<=1) {
			//lpOb->Flags=lpOb->Flags&(~TRANSPARENTOBJ);
			State->Status=MB_SLEEPING;
			return;
		}
		UpdateTextureCoordinates(State);
		RENDERSTRIP();
	}
	
}

D3DTEXTUREHANDLE LoadMBlareTexture(char *Name){
	static LPDIRECTDRAWSURFACE lpSurf;
	LPDIRECT3DTEXTURE2 lpComObj;
	return NewTexture(Name, &lpSurf,
                                  &lpComObj);
}





