// _XArea.h: interface for the _XArea class.
//
// Created Date : 2004.01.28 
// Author : Oneway Yang (oneway48)
// Desc : 기존의 지형 Format의 변화로 인해서 변화된 Format을 저장하는 클래스를 구현   
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX__XAREA_H__42F1B703_0C0C_40E0_8DC8_7A99C4CCE1A7__INCLUDED_)
#define AFX__XAREA_H__42F1B703_0C0C_40E0_8DC8_7A99C4CCE1A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>


#define _XDEF_AREACATEGORY_COUNT			12

#define _XDEF_AREACATEGORY_SAFE				0
#define _XDEF_AREACATEGORY_PLAYERSPAWN		1
#define _XDEF_AREACATEGORY_MOBSPAWN			2
#define _XDEF_AREACATEGORY_NPCAREA			3
#define _XDEF_AREACATEGORY_QUESTAREA		4
#define _XDEF_AREACATEGORY_EVENTAREA		5
#define _XDEF_AREACATEGORY_MAPWARP			6
#define _XDEF_AREACATEGORY_PVPAREA			7
#define _XDEF_AREACATEGORY_CASTLE			8
#define _XDEF_AREACATEGORY_LOGOUT			9
#define _XDEF_AREACATEGORY_NOTUSESPEEDSKILL	10
#define _XDEF_AREACATEGORY_PUBLICPVPAREA	11	//Author : 양희왕 //breif : 추가

#define _XDEF_AREASHAPETYPE_BOX				0
#define _XDEF_AREASHAPETYPE_SPHERE			1

typedef struct
{
	FLOAT	x;
	FLOAT	z;
}_XAREAPOINTSTRUCT;

class _XArea  
{
public :
	BYTE				m_AreaCategory;
	int					m_Num;

#ifdef _XDEF_USENEWAREATEXTSYSTEM
	LPTSTR				m_Name;

#ifdef _XJAPANESE	
	LPTSTR				m_HiraganaName;
#endif	

#else
	TCHAR				m_Name[21];
#endif
	
	_XAREAPOINTSTRUCT	m_Point[4];
	_XAREAPOINTSTRUCT	m_CenterPoint;
	FLOAT				m_Radius;
	
	BYTE				m_ShapeType;
	
	int					m_TargetZone;
	int					m_TargetWarpzone;
	
	int					m_MonsterSpawnType;
	int					m_MonsterSpawnCount;

	BYTE				m_PVPZoneType;	//0:필드비무장 1:공식비무장소(개인/집단-단승,연승) 2:공식비무장대(집단-전멸,행수)
										//3:비무대회용 비무장 4:비무대회장 5:비무안전지역 6:비무완료지역
	int					m_indexPVPSafeZone[4];
	int					m_indexPVPCompleteZone;

	int					m_SpecialFunction;
	
	// 다중맵 이동일 때 필요한 정보 
	int					m_MultiTargetZone1;
	int					m_MultiTargetWarpzone1;
	int					m_MultiWarpzonePassage1; // 통과 조건을 나타낸다.
	
#ifdef _XDEF_USENEWAREATEXTSYSTEM
	LPTSTR				m_WarpZoneName1;
#else
	TCHAR				m_WarpZoneName1[21];
#endif
	
	int					m_MultiTargetZone2;
	int					m_MultiTargetWarpzone2;
	int					m_MultiWarpzonePassage2; // 통과 조건을 나타낸다.
#ifdef _XDEF_USENEWAREATEXTSYSTEM
	LPTSTR				m_WarpZoneName2;
#else
	TCHAR				m_WarpZoneName2[21];
#endif
	
	int					m_MultiTargetZone3;
	int					m_MultiTargetWarpzone3;
	int					m_MultiWarpzonePassage3; // 통과 조건을 나타낸다.
#ifdef _XDEF_USENEWAREATEXTSYSTEM
	LPTSTR				m_WarpZoneName3;
#else
	TCHAR				m_WarpZoneName3[21];
#endif
	
public:
	_XArea();
	virtual ~_XArea();
	
	BOOL				Load(FILE* fileptr);
};

#endif // !defined(AFX__XAREA_H__42F1B703_0C0C_40E0_8DC8_7A99C4CCE1A7__INCLUDED_)
