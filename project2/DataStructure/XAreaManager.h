// _XAreaManager.h: interface for the _XAreaManager class.
//
// Created Date : 2004.01.28 
// Author : Oneway Yang (oneway48)
// Desc : 기존의 지형 Format의 변화로 인해서 새로운 클래스를 만들었는데   
//		 기존의 Manager역할을 하는 클래스와 이름이 유사해서 새로 만들었음.
//       기존의 Manager는 Hashing을 통해 Load하였지만 새로운 Manager는 List를 사용하여
//        카테고리 정보를 이용해서 검색함. 
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__XAREAMANAGER_H__C2A42019_50FE_4D94_B88E_218A81B574B7__INCLUDED_)
#define AFX__XAREAMANAGER_H__C2A42019_50FE_4D94_B88E_218A81B574B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
#include "XArea.h"


#include <list>
using namespace std;

class _XOBBData;

typedef struct {
	list <_XArea *> AreaList;
}_XAREALISTTABLE;

class _XAreaManager  
{
public :
	_XAREALISTTABLE		*m_AreaTable;	

	int					m_PotalOBBCount;
	_XOBBData*			m_PortalOBBDataList;

public:
	BOOL CheckInArea(D3DXVECTOR3 position,  _XArea* currentarea, BOOL questareacheck = FALSE );

	_XAreaManager();
	virtual ~_XAreaManager();

	void Release();
	
	BOOL LoadAreaList(FILE *fileptr);
	
	void BuildTable(_XArea* currentarea);
	
	BOOL IsEmpty(D3DXVECTOR3 position);
	BOOL FindPositionInArea(D3DXVECTOR3 position, int areanumber, int areacategory, BOOL questareacheck = FALSE);
	BOOL FindAreaCenterPosition(D3DXVECTOR3& position, int areanumber, int areacategory);
	BOOL FindAreaNumber(D3DXVECTOR3 position, list <int>& arealist);
	BOOL CheckInsideArea(D3DXVECTOR3 position, int category, int& areanumber);

	_XArea* GetArea(int areacategory, int areaindex);
	TCHAR* GetAreaName(int areacategory, int areaindex);
	
	BOOL GetMyAreaCenterPosition(D3DXVECTOR3 myposition, D3DXVECTOR3& centerposition);
	
//	int  GetAreaCount( void ){ return m_TotalTileCount;	}
};

#endif // !defined(AFX__XAREAMANAGER_H__C2A42019_50FE_4D94_B88E_218A81B574B7__INCLUDED_)
