#ifndef __CONV_PICK_H__
#define __CONV_PICK_H__

void ResetPickAfter(void);
void ResetPick(void);

void PickAt(LONG X, LONG Y);
void TagObjForPick(LPMYOBJ x);
void UntagObjForPick(LPMYOBJ x);
void TagFrameForPick(LPMYFRAME x);
LPMYOBJ MyGetPick(void);
void GetPickCoordinates(MYVECTOR v);


void PGEPickResetRect(void);
void PGEPickObject(LPMYOBJ lpOb);

/////////Collision List
void PGEPickResetCollisionList(void);
void PGEPickCheckAndAddToCollisionList(LPMYOBJ lpObj, MYMATRIX lpMatrix);
//////picking for collision verification////////////
D3DMATRIXHANDLE LowVerifyCollision (D3DMATRIX *Vertices, D3DMATRIX *Normals, 
						 D3DMATRIX *Transform, LPMYOBJ lpOb,
						 LPMYOBJ RefObj, BOOL *CollisionFound);
BOOL MObjectsVerifyCollision (PREAL Displ, D3DMATRIX *Camera, D3DMATRIX *Verticess, 
							  D3DMATRIX *Normals, unsigned char *property, void **Coll);


#endif