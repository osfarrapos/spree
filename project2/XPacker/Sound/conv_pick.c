/* //////////////////////////////////////////////////////////
TD 15-09-00  WORKING IN PROGRESS
Devo simulare le funzioni Pick & GetPickRecord, UAU!
		HRESULT H=lpD3DDevice->lpVtbl->Pick(lpD3DDevice, lpOb->lpExecuteBuffer, CurrViewport, 0, &PickRect);
		H=lpD3DDevice->lpVtbl->GetPickRecords(lpD3DDevice, &NumberPick, NULL);	

////////////////////////////////////////////////////////// */
////////////Picking/////////////
#include "conv_common.h"
#include "pick.h"

static D3DRECT PickRect;
static D3DRECT CollPickRect;
static BOOL PickRequired;
//static DWORD NumberPick;
static LPMYOBJ SelectedPick, IntPick;
static PREAL PickedZ;
//////////////////////

//////////Collisions via Pick//////
static LPMYOBJ CollisionList[MAX_VISUALS];
static D3DMATRIX *MCollisionList[MAX_VISUALS];
static int NCollisionList;

#define SOFTWARE_PICK

void PGEPickResetCollisionList(void){
	NCollisionList=0;
}

void PGEPickCheckAndAddToCollisionList(LPMYOBJ lpObj, MYMATRIX lpMatrix){
	CollisionList[NCollisionList]=lpObj;
	MCollisionList[NCollisionList]=lpMatrix;
	NCollisionList++;
}

void PGEPickResetRect(void){
	CollPickRect.x1= CollPickRect.x2=W_WIDTH/2;
	CollPickRect.y1= CollPickRect.y2=W_HEIGHT/2;
}

void PGEPickObject(LPMYOBJ lpOb){	
	if(PickRequired&&((lpOb->Flags)&PICKENABLE)){
		D3DPICKRECORD PRec;
		DWORD NumberPick;

#ifdef SOFTWARE_PICK
		HRESULT H=_Pick( lpD3DDevice2, lpD3DDevice, lpOb->lpExecuteBuffer, CurrViewport, CurrViewport2, 0, &PickRect );
		        H=_GetPickRecords( &NumberPick, NULL ); 
#else
    HRESULT H=lpD3DDevice->lpVtbl->Pick(lpD3DDevice, lpOb->lpExecuteBuffer, CurrViewport, 0, &PickRect);
    		    H=lpD3DDevice->lpVtbl->GetPickRecords(lpD3DDevice, &NumberPick, NULL);
#endif

		if( NumberPick > 0 )
		{
			IntPick    = SelectedPick = lpOb;
			NumberPick = 1;
			PickedZ    = PRec.dvZ = 0;
#ifdef SOFTWARE_PICK
			H=_GetPickRecords( &NumberPick, &PRec );
#else
			H=lpD3DDevice->lpVtbl->GetPickRecords( lpD3DDevice, &NumberPick, &PRec );
#endif
			PickedZ      = 2 * PRec.dvZ;
			PickRequired = FALSE;
			//////////////////DEBUG ONLY////////////				
			//////////////////////////////////////////////////
		}
	}
}

DWORD GetPickedVertices(D3DMATRIX *Verticess, D3DMATRIX *Normals, 
					   D3DPICKRECORD *PRec, LPDIRECT3DEXECUTEBUFFER EB,   DWORD Displ){
	HRESULT H;
	LPD3DTRIANGLE Tr;
	LPD3DVERTEX Vert;
	WORD CurrV;
	
	D3DEXECUTEBUFFERDESC Descr;

	memset(&Descr, 0, sizeof(D3DEXECUTEBUFFERDESC));
	Descr.dwSize=sizeof(D3DEXECUTEBUFFERDESC);

	while((H=EB->lpVtbl->Lock(EB, &Descr))!=D3D_OK);

	Vert=(LPD3DVERTEX)(Descr.lpData);

	Tr=(LPD3DTRIANGLE)(((char *)Descr.lpData)+Displ+PRec->dwOffset);

	////////////////First Vertex//////////////////////////
	CurrV=Tr->v1;
		
	Verticess->_11=Vert[CurrV].x;
	Verticess->_12=Vert[CurrV].y;
	Verticess->_13=Vert[CurrV].z;
	Verticess->_14=PVALUE(1.);

	Normals->_11=Vert[CurrV].nx;
	Normals->_12=Vert[CurrV].ny;
	Normals->_13=Vert[CurrV].nz;
	Normals->_14=PVALUE(0.);
	//////////////////////////////////////////////////////////
	////////////////Second Vertex//////////////////////////
	CurrV=Tr->v2;

	Verticess->_21=Vert[CurrV].x;
	Verticess->_22=Vert[CurrV].y;
	Verticess->_23=Vert[CurrV].z;
	Verticess->_24=PVALUE(1.);

	Normals->_21=Vert[CurrV].nx;
	Normals->_22=Vert[CurrV].ny;
	Normals->_23=Vert[CurrV].nz;
	Normals->_24=PVALUE(0.);
	//////////////////////////////////////////////////////////
	////////////////ThirdVertex//////////////////////////
	CurrV=Tr->v3;

	Verticess->_31=Vert[CurrV].x;
	Verticess->_32=Vert[CurrV].y;
	Verticess->_33=Vert[CurrV].z;
	Verticess->_34=PVALUE(1.);

	Normals->_31=Vert[CurrV].nx;
	Normals->_32=Vert[CurrV].ny;
	Normals->_33=Vert[CurrV].nz;
	Normals->_34=PVALUE(0.);
	//////////////////////////////////////////////////////////

	EB->lpVtbl->Unlock(EB);

	return CurrV;
}
//////////////////////////////////////////////////
//Picking functions not used in Puma
/*
void UntagObjForPick(LPMYOBJ x){
	int inv=PICKENABLE;
	inv=~inv;
	x->Flags=inv&(x->Flags);	
}

void TagFrameForPick(LPMYFRAME x){
	LPMYFRAME NextF;
	LPMYOBJ   NextO;
	if (x==NULL) return ;
	NextO=x->ObList;
	while (NextO!=NULL){
		if ((strlen(NextO->Name)<2)||(NextO->Name[0]!=NOPICK0)||(NextO->Name[0]!=NOPICK1))
			TagObjForPick(NextO);
		NextO=NextO->Next;
	}
	NextF=x->FrameList;
	while (NextF!=NULL){
		TagFrameForPick(NextF);
		NextF=NextF->Next;
	}
}


void ComputeObjectCenter(LPMYOBJ lpOb, MYVECTOR v){
	MYVECTOR v0;

	v0[0]=(lpOb->BoundingBox.min.x+lpOb->BoundingBox.max.x)/2;
	v0[1]=(lpOb->BoundingBox.min.y+lpOb->BoundingBox.max.y)/2;
	v0[2]=(lpOb->BoundingBox.min.z+lpOb->BoundingBox.max.z)/2;
	v0[3]=1;
	VByMatrixMultiply(v0, (PREAL *)&(lpOb->Father->GMatrix), v);
}

void GetPickCoordinates(MYVECTOR v){
	ComputeObjectCenter(IntPick, v);
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//High Level picking functions

/* TD temp 15-09-00
typedef struct PGEPICK_RECORD_OBJtag{
	LPMYOBJ lpOb;
	MYMATRIX transform;
	D3DPICKRECORD pickRec;
};*/

void ResetPick(void){
	SelectedPick=NULL;
}

void ResetPickAfter(void){
	PickRequired=FALSE;
}

//MOUSE PICKING
void PickAt(LONG X, LONG Y){
	PickRequired=TRUE;
	PickRect.x1=X;
	PickRect.x2=X;
	PickRect.y1=Y;
	PickRect.y2=Y;
}


void TagObjForPick(LPMYOBJ x){
	x->Flags=PICKENABLE|(x->Flags);
}

LPMYOBJ MyGetPick(void){
	LPMYOBJ temp;
	temp=SelectedPick;
	SelectedPick=NULL;
	return temp;
} 

static D3DPICKRECORD vPRec[100];

//BALL COLLISION
// Nico: dovrebbe verificare le collisioni con i giocatori ( non funziona in software )
extern LPDIRECT3DEXECUTEBUFFER lpD3DExCmdBuf, lpD3DExCmdBufNull; //TEMP ???!!
D3DMATRIXHANDLE LowVerifyCollision (D3DMATRIX *Verticess, D3DMATRIX *Normals, 
						 D3DMATRIX *Transform, LPMYOBJ lpOb,
						 LPMYOBJ RefObj, BOOL *CollisionFound){

	DWORD NumberPick;
	DWORD CurrV;
	D3DMATRIXHANDLE Result;
	HRESULT H;
	
	static D3DMATRIX Reg1, Reg2, RegN, RegV;
	static int Tr;
	
	lpD3DDevice->lpVtbl->BeginScene(lpD3DDevice);
	
	// nulls out the projection and view matrices
	RunAndCheckExecuteBuffer(lpD3DDevice, lpD3DExCmdBufNull, CurrViewport, D3DEXECUTE_UNCLIPPED);	
	
	// set world matrix
	H=lpD3DDevice2->lpVtbl->SetTransform(lpD3DDevice2, D3DTRANSFORMSTATE_WORLD, Transform);
	H=lpD3DDevice->lpVtbl->SetMatrix(lpD3DDevice, RMatrix, Transform);
	H=lpD3DDevice->lpVtbl->SetMatrix(lpD3DDevice, ExecuteMatrix, Transform);

	// set obj animation if any
	if( RefObj && ( lpOb->NMatrices > 0 ))
		PGESetObjMatrices( RefObj );		

	// now pick object
#ifdef SOFTWARE_PICK
	H=_Pick( lpD3DDevice2, lpD3DDevice, lpOb->lpExecuteBuffer, CurrViewport, CurrViewport2, 0, &CollPickRect );
#else
	H=lpD3DDevice->lpVtbl->Pick(lpD3DDevice, lpOb->lpExecuteBuffer, CurrViewport, 0, &CollPickRect );
#endif

	//_TestExecuteBuffer();

	RunAndCheckExecuteBuffer(lpD3DDevice,lpD3DExCmdBuf, CurrViewport, D3DEXECUTE_UNCLIPPED);

	Transform->_44=PVALUE(1);

#ifdef SOFTWARE_PICK
	H=_GetPickRecords( &NumberPick, NULL );
#else
	H=lpD3DDevice->lpVtbl->GetPickRecords(lpD3DDevice, &NumberPick, NULL);
#endif

	if( NumberPick > 0 )
	{
		int   i, j = 0;
		PREAL Min = 1000.;
		
		*CollisionFound=TRUE;//if (NumberPick>1)	*CollisionFound=TRUE;//TD 10-07-00 ????
#ifdef SOFTWARE_PICK
		H=_GetPickRecords( &NumberPick, vPRec );
#else
		H=lpD3DDevice->lpVtbl->GetPickRecords(lpD3DDevice, &NumberPick, vPRec);
#endif

		for (i=0;i<NumberPick;i++){
			if (vPRec[i].dvZ<Min){
				j=i;
				Min=vPRec[i].dvZ;
			}
		}
		(lpOb->lpExecuteBuffer)->lpVtbl->GetExecuteData((lpOb->lpExecuteBuffer), &(lpOb->ExecuteData));
		H=lpD3DDevice->lpVtbl->EndScene(lpD3DDevice);
		CurrV=GetPickedVertices(&RegV, &RegN,   vPRec+j, lpOb->lpExecuteBuffer, lpOb->ExecuteData.dwInstructionOffset);

		/////////////find Matrix handle//////////////
		for (i=0; i<lpOb->ONMatrices; i++){
			if (CurrV<lpOb->Vertices[i]) break;
		}

		i--;
		if (i>-1){
			Result=lpOb->OMatrices[i];
			H=lpD3DDevice->lpVtbl->GetMatrix(lpD3DDevice, Result,&Reg1);
			MatrixMultiply((PREAL *)(&Reg1), (PREAL *)Transform, (PREAL *)&Reg2);
			MatrixMultiply((PREAL *)(&RegV), (PREAL *)&Reg2, (PREAL *)Verticess);
			MatrixMultiply((PREAL *)(&RegN), (PREAL *)&Reg2, (PREAL *)Normals);
		}
		else {
			Result=0;
			MatrixMultiply((PREAL *)(&RegV), (PREAL *)Transform, (PREAL *)Verticess);
			MatrixMultiply((PREAL *)(&RegN), (PREAL *)Transform, (PREAL *)Normals);
		}
	}
	else {		
		H=lpD3DDevice->lpVtbl->EndScene(lpD3DDevice);
		Result=0;			
		*CollisionFound=FALSE;
	}
	return Result;
}

// Nico: verifica le collisioni con gli elementi di contorno del campo ( funziona anche in software )
BOOL MObjectsVerifyCollision (PREAL Displ, D3DMATRIX *Camera, D3DMATRIX *Verticess, 
							  D3DMATRIX *Normals, unsigned char *property, void **Coll){
	static D3DPICKRECORD vPRec[300], MinPRec;
	DWORD           NumberPick;
	DWORD           CurrV;
	BOOL            CollisionFound;
	D3DMATRIXHANDLE Result;
	HRESULT H;
	MYMATRIX Transform, MTransform;
	static D3DMATRIX Reg1, Reg2, RegN, RegV;
	int i, l;
	PREAL Min=1000.;
	LPMYOBJ lpOb, MinOb;

	lpD3DDevice->lpVtbl->BeginScene(lpD3DDevice);

	RunAndCheckExecuteBuffer(lpD3DDevice, lpD3DExCmdBufNull, CurrViewport, D3DEXECUTE_UNCLIPPED);

	lpD3DDevice2->lpVtbl->SetRenderState(lpD3DDevice2, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

	CollisionFound=FALSE;
	for (l=0; l<NCollisionList; l++){
		lpOb=CollisionList[l];
		MatrixMultiply((PREAL *)(MCollisionList[l]), (PREAL *)Camera, Transform);
		if(lpOb->Flags&ISSHADOW) continue;
		//////////////////////////SET WORLD  MATRIX////////////
		Transform[15]=PVALUE(1000);
		PINC(Transform[14], Displ);

		H=lpD3DDevice2->lpVtbl->SetTransform(lpD3DDevice2, D3DTRANSFORMSTATE_WORLD, Transform);

		H=lpD3DDevice->lpVtbl->SetMatrix(lpD3DDevice, RMatrix, Transform);
	
		H=lpD3DDevice->lpVtbl->SetMatrix(lpD3DDevice, ExecuteMatrix, Transform);

		//////////Now Pick Object///////////////////////////////////////
#ifdef SOFTWARE_PICK
		H=_Pick( lpD3DDevice2, lpD3DDevice, lpOb->lpExecuteBuffer, CurrViewport, CurrViewport2, 0, &CollPickRect ); 
    H=_GetPickRecords( &NumberPick, NULL );
#else
		H=lpD3DDevice->lpVtbl->Pick(lpD3DDevice, lpOb->lpExecuteBuffer, CurrViewport, 0,
										   &CollPickRect);
		H=lpD3DDevice->lpVtbl->GetPickRecords(lpD3DDevice, &NumberPick, NULL);	
#endif

		if(NumberPick>0){
			int k;
			CollisionFound=TRUE;
#ifdef SOFTWARE_PICK
	    H=_GetPickRecords( &NumberPick, vPRec );
#else
			H=lpD3DDevice->lpVtbl->GetPickRecords(lpD3DDevice, &NumberPick, vPRec);
#endif
			for (k=0;k<NumberPick;k++){
				if (PABS(vPRec[k].dvZ)<Min){					
					Min=PABS(vPRec[k].dvZ);
					MinOb=lpOb;					
					memcpy(&MinPRec, vPRec+k, sizeof(D3DPICKRECORD));					
					memcpy(MTransform, Transform, sizeof(MYMATRIX));
				}
			}
		}
	}
	
	MTransform[15]=PVALUE(1);
	PDEC(MTransform[14], Displ);
	///////////////////Process minimum distance element///////
	if (CollisionFound){
		int Tr;
		(MinOb->lpExecuteBuffer)->lpVtbl->GetExecuteData((MinOb->lpExecuteBuffer), 	&(MinOb->ExecuteData));
		CurrV=GetPickedVertices(&RegV, &RegN,  &MinPRec,  MinOb->lpExecuteBuffer, MinOb->ExecuteData.dwInstructionOffset);
		//find texture property/////////////////////
		Tr=MinPRec.dwOffset+MinOb->ExecuteData.dwInstructionOffset;
		for(i=0;i<MinOb->NTextures;i++){
			if (Tr<MinOb->Triangles[i]) break;
		}
		i=min(i, MinOb->NTextures-1);
		*property=MinOb->Textures[i];
		/////////////find Matrix handle//////////////
		for (i=0; i<MinOb->NMatrices; i++){
			if (CurrV<MinOb->Vertices[i]) break;
		}

		i--;		
		if (i>-1){
			Result=MinOb->Matrices[i];
			H=lpD3DDevice->lpVtbl->GetMatrix(lpD3DDevice,  
							Result,	&Reg1);
				
			MatrixMultiply((PREAL *)(&Reg1), (PREAL *)MTransform, (PREAL *)&Reg2);
			MatrixMultiply((PREAL *)(&RegV), (PREAL *)&Reg2, (PREAL *)Verticess);
			MatrixMultiply((PREAL *)(&RegN), (PREAL *)&Reg2, (PREAL *)Normals);
		}else {
			Result=0;
			MatrixMultiply((PREAL *)(&RegV), (PREAL *)MTransform, (PREAL *)Verticess);
			MatrixMultiply((PREAL *)(&RegN), (PREAL *)MTransform, (PREAL *)Normals);
		}
		*Coll=MinOb;
	}
	
	RunAndCheckExecuteBuffer(lpD3DDevice,	lpD3DExCmdBuf, CurrViewport, D3DEXECUTE_UNCLIPPED);

	lpD3DDevice2->lpVtbl->SetRenderState(lpD3DDevice2, D3DRENDERSTATE_CULLMODE, D3DCULL_CCW); 

	H=lpD3DDevice->lpVtbl->EndScene(lpD3DDevice);

	return CollisionFound;
}
