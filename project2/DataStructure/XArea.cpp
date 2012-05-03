// _XArea.cpp: implementation of the _XArea class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "emperorofdragons.h"
#include "XArea.h"
#include "XStringDataBase.h"
#include "EODEXTDEF_GlobalObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

TCHAR g_NoneAreaString[2] = _T("");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XArea::_XArea()
{
	m_AreaCategory		=	0;
	m_Num				=	0;
	m_Radius			=	0.0f;
	m_ShapeType			=	0;
	m_TargetZone		=	0;
	m_TargetWarpzone	=	0;
	m_MonsterSpawnType	=	0;
	m_MonsterSpawnCount =   0;
	m_SpecialFunction	=	0;
	
	m_PVPZoneType = 0;
	m_indexPVPCompleteZone = m_indexPVPSafeZone[0] = m_indexPVPSafeZone[1] = m_indexPVPSafeZone[2] = m_indexPVPSafeZone[3] = -1;

#ifdef _XDEF_USENEWAREATEXTSYSTEM	
	m_Name = NULL;
	
#ifdef _XJAPANESE	
	m_HiraganaName = NULL;
#endif	

#else
	memset(m_Name, 0, sizeof(TCHAR)*21);
#endif
	memset(m_Point, 0, sizeof(_XAREAPOINTSTRUCT)*4);
	memset(&m_CenterPoint, 0, sizeof(_XAREAPOINTSTRUCT));
		
	m_MultiTargetZone1	= -1;
	m_MultiTargetWarpzone1	= -1;
	m_MultiWarpzonePassage1	= -1; // 통과 조건을 나타낸다.	
	
	m_MultiTargetZone2	= -1;
	m_MultiTargetWarpzone2	= -1;
	m_MultiWarpzonePassage2	= -1; // 통과 조건을 나타낸다.
		
	m_MultiTargetZone3	= -1;
	m_MultiTargetWarpzone3	= -1;
	m_MultiWarpzonePassage3	= -1; // 통과 조건을 나타낸다.
	
#ifdef _XDEF_USENEWAREATEXTSYSTEM
	m_WarpZoneName1 = NULL;
	m_WarpZoneName2 = NULL;
	m_WarpZoneName3 = NULL;
#else
	memset(m_WarpZoneName1, 0, sizeof(TCHAR)*21);
	memset(m_WarpZoneName2, 0, sizeof(TCHAR)*21);
	memset(m_WarpZoneName3, 0, sizeof(TCHAR)*21);
#endif
}

_XArea::~_XArea()
{
	
}

BOOL _XArea::Load(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;
	
	if(fread(&m_AreaCategory, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;
	
	if(fread(&m_Num, sizeof(int), 1, fileptr) < 1)
		return FALSE;

#ifdef _XDEF_USENEWAREATEXTSYSTEM
	TCHAR buffer[21];
	//fseek( fileptr, sizeof(TCHAR) * 21, SEEK_CUR );
	if(fread(&buffer, sizeof(TCHAR) * 21, 1, fileptr) < 1)
		return FALSE;

	if( strlen(buffer) )
	{
		if( m_AreaCategory == _XDEF_AREACATEGORY_EVENTAREA )
		{
			int resourceid = (g_CurrentZoneInfoIndex * 1000) + m_Num;
			m_Name = g_StringDataBase.GetTextString( _XSTRINGDB_SN_AREATITLE, resourceid );
#ifdef _XJAPANESE	
			m_HiraganaName = g_StringDataBase.GetSubTextString( _XSTRINGDB_SN_AREATITLE, resourceid );
#endif
		}
		else if( m_AreaCategory == _XDEF_AREACATEGORY_MAPWARP )
		{
			int resourceid = (g_CurrentZoneInfoIndex * 1000) + m_Num;
			m_Name = g_StringDataBase.GetTextString( _XSTRINGDB_SN_AREATITLE, resourceid );
#ifdef _XJAPANESE	
			m_HiraganaName = g_StringDataBase.GetSubTextString( _XSTRINGDB_SN_AREATITLE, resourceid );
#endif
		}
		else if( m_AreaCategory == _XDEF_AREACATEGORY_PVPAREA )
		{
			int resourceid = (g_CurrentZoneInfoIndex * 1000) + m_Num;
			m_Name = g_StringDataBase.GetTextString( _XSTRINGDB_SN_AREATITLE, resourceid );
#ifdef _XJAPANESE	
			m_HiraganaName = g_StringDataBase.GetSubTextString( _XSTRINGDB_SN_AREATITLE, resourceid );
#endif
		}
		else
		{
			m_Name = g_NoneAreaString;
#ifdef _XJAPANESE	
			m_HiraganaName = g_NoneAreaString;
#endif
		}
	}
	else
	{
		m_Name = g_NoneAreaString;
#ifdef _XJAPANESE	
			m_HiraganaName = g_NoneAreaString;
#endif
	}
#else	
	if(fread(m_Name, sizeof(TCHAR), 21, fileptr) < 1)
		return FALSE;
#endif	
	
	if(fread(&m_ShapeType, sizeof(BYTE), 1, fileptr) < 1)
		return FALSE;
	
	if(m_ShapeType == _XDEF_AREASHAPETYPE_BOX)
	{
		if(fread(m_Point, sizeof(_XAREAPOINTSTRUCT), 4, fileptr) < 1)
			return FALSE;
	}
	else
	{
		if(fread(&m_CenterPoint, sizeof(_XAREAPOINTSTRUCT), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&m_Radius, sizeof(FLOAT), 1, fileptr) < 1)
			return FALSE;
	}

	if(fread(&m_SpecialFunction, sizeof(int), 1, fileptr) < 1)
		return FALSE;
	
	if(m_AreaCategory == _XDEF_AREACATEGORY_MAPWARP)
	{
		if(fread(&m_TargetZone, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&m_TargetWarpzone, sizeof(int), 1, fileptr) < 1)
			return FALSE;
	}

	if(m_AreaCategory == _XDEF_AREACATEGORY_MOBSPAWN)
	{
		int monstertypecount = 0;
		if(fread(&monstertypecount, sizeof(int), 1, fileptr) < 1)
			return FALSE;

		// Skip read monster spawn area informations...
		int MonsterSpawnType, MonsterSpawnCount;
		for( int i = 0; i < monstertypecount; ++i )
		{
			if(fread(&MonsterSpawnType, sizeof(int), 1, fileptr) < 1) return FALSE;
			if(fread(&MonsterSpawnCount, sizeof(int), 1, fileptr) < 1) return FALSE;
			
			if( MonsterSpawnCount > 0 )
			{
				g_MobAttribTable[MonsterSpawnType].usecurrentzone = true;
			}

			//fseek( fileptr, sizeof(int) * 2 * monstertypecount, SEEK_CUR );
		}

		int monsterspawnpointcount = 0;
		if(fread(&monsterspawnpointcount, sizeof(int), 1, fileptr) < 1) return FALSE;

		// Skip read monster spawn point...
		fseek( fileptr, ((sizeof(FLOAT) * 2) + sizeof(int)) * monsterspawnpointcount, SEEK_CUR );
	}

	if( m_AreaCategory == _XDEF_AREACATEGORY_PVPAREA )
	{
		if(fread(&m_PVPZoneType, sizeof(BYTE), 1, fileptr) < 1)
			return FALSE;
		
		if(fread(&m_indexPVPSafeZone[0], sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_indexPVPSafeZone[1], sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_indexPVPSafeZone[2], sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_indexPVPSafeZone[3], sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_indexPVPCompleteZone, sizeof(int), 1, fileptr) < 1)
			return FALSE;
	}

	if( m_SpecialFunction == 6 ) // 다중맵 이동
	{			
		if(fread(&m_MultiTargetZone1, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_MultiTargetWarpzone1, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_MultiWarpzonePassage1, sizeof(int), 1, fileptr) < 1)
			return FALSE;

#ifdef _XDEF_USENEWAREATEXTSYSTEM
		if(fread(&buffer, sizeof(TCHAR) * 21, 1, fileptr) < 1)
			return FALSE;
		if( strlen(buffer) )
		{
			int resourceid = (g_CurrentZoneInfoIndex * 1000) + m_Num + 1000000;
			m_WarpZoneName1 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_AREATITLE, resourceid );
		}
		else
		{
			m_WarpZoneName1 = g_NoneAreaString;
		}
#else
		if(fread(m_WarpZoneName1, sizeof(TCHAR), 21, fileptr) < 1)
			return FALSE;		
#endif
		
		if(fread(&m_MultiTargetZone2, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_MultiTargetWarpzone2, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_MultiWarpzonePassage2, sizeof(int), 1, fileptr) < 1)
			return FALSE;
	
#ifdef _XDEF_USENEWAREATEXTSYSTEM
		if(fread(&buffer, sizeof(TCHAR) * 21, 1, fileptr) < 1)
			return FALSE;
		if( strlen(buffer) )
		{
			int resourceid = (g_CurrentZoneInfoIndex * 1000) + m_Num + 2000000;
			m_WarpZoneName2 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_AREATITLE, resourceid );
		}
		else
		{
			m_WarpZoneName2 = g_NoneAreaString;
		}
#else
		if(fread(m_WarpZoneName2, sizeof(TCHAR), 21, fileptr) < 1)
			return FALSE;
#endif		
		
		if(fread(&m_MultiTargetZone3, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_MultiTargetWarpzone3, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		if(fread(&m_MultiWarpzonePassage3, sizeof(int), 1, fileptr) < 1)
			return FALSE;
		
#ifdef _XDEF_USENEWAREATEXTSYSTEM
		if(fread(&buffer, sizeof(TCHAR) * 21, 1, fileptr) < 1)
			return FALSE;
		if( strlen(buffer) )
		{
			int resourceid = (g_CurrentZoneInfoIndex * 1000) + m_Num + 3000000;
			m_WarpZoneName3 = g_StringDataBase.GetTextString( _XSTRINGDB_SN_AREATITLE, resourceid );
		}
		else
		{
			m_WarpZoneName3 = g_NoneAreaString;
		}
#else
		if(fread(m_WarpZoneName3, sizeof(TCHAR), 21, fileptr) < 1)
			return FALSE;
#endif		
	}

	D3DXVECTOR3 point[4];
	for(int i = 0 ; i < 4 ; ++i)
	{
		point[i].x = m_Point[i].x;
		point[i].y = 0.0f;
		point[i].z = m_Point[i].z;
	}
	
	D3DXVECTOR3 diagonal = point[2] - point[0];
	FLOAT length = D3DXVec3Length(&diagonal);
	D3DXVECTOR3 unit;
	D3DXVec3Normalize(&unit, &diagonal);
	
	D3DXVECTOR3 position = point[0] + ((length/2)*unit);
	m_CenterPoint.x = position.x;
	m_CenterPoint.z = position.z;

	return TRUE;
}
