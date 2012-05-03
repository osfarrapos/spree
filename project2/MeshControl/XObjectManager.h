// XObjectManager.h: interface for the _XObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XOBJECTMANAGER_H_
#define _XOBJECTMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XMeshContainer.h"
#include "XBipedController.h"
#include "X3PCamera.h"

#ifdef _XUSEFMOD
	#include "XFMODWrapper.h"
#else
	#include "XAudioSystem.h"
	using namespace Audio;
#endif

#include <map>
#include <string>

#include <list>

#include <vector>

class _XST_AbstractObject;

//////////////////////////////////////////////////////////////////////////////////////////
// Object item
typedef struct _XObject 
{
	int		   m_ObjectIndex;
	D3DXMATRIX m_matWorldPosition;
	
	_XObject()
	{
		m_ObjectIndex = -1;
		memset( m_matWorldPosition, 0, sizeof(D3DXMATRIX) );
		m_matWorldPosition._11 = 1.0f;
		m_matWorldPosition._22 = 1.0f;
		m_matWorldPosition._33 = 1.0f;
		m_matWorldPosition._44 = 1.0f;	
	}
	~_XObject()
	{
		m_ObjectIndex = -1;		
	}
} _XObject, *_LPXObject;

struct _EXT_Object : public _XObject
{
	int				m_nEventType;
	int				m_nEventValue;
};


enum OBJECT_EVENT
{
	OBJECT_EVENT_NONE = 0,
	OBJECT_EVENT_COLLECT_COTTON,
	OBJECT_EVENT_COLLECT_TEA,
	OBJECT_EVENT_COLLECT_MUSHROOM,
	OBJECT_EVENT_COLLECT_TREE,
	OBJECT_EVENT_FIREWOOD_POINT,
	OBJECT_EVENT_WATER_TUBE,
	OBJECT_EVENT_WATER_POINT
};

class _XOctreeObject : public _XItem
{
public:
	int		   m_ObjectIndex;
	D3DXMATRIX m_matWorldPosition;
	
	int				m_nEventType;
	int				m_nEventValue;

	FLOAT				m_Radius;
	_XOctreeObject*		m_nextObject;
	FLOAT				m_Distance;		
	DWORD				m_AlphaLevel;
	BOOL				m_AboveWater;

	_XBipedController*	m_pBipedController;
	FLOAT				m_fAniTime;
public:
	_XOctreeObject()
	{
		m_nEventType = 0;
		m_nEventValue = 0;

		m_ObjectIndex = -1;
		memset( m_matWorldPosition, 0, sizeof(D3DXMATRIX) );
		m_matWorldPosition._11 = 1.0f;
		m_matWorldPosition._22 = 1.0f;
		m_matWorldPosition._33 = 1.0f;
		m_matWorldPosition._44 = 1.0f;

		m_Radius = 5.0f;

		m_Distance = 9999.0f;

		m_AlphaLevel = 255;

		m_nextObject = NULL;
		
		m_AboveWater = FALSE;
		m_pBipedController = NULL;
		m_fAniTime = 0.f;
	}
	~_XOctreeObject()
	{
		m_ObjectIndex = -1;
		m_nextObject = NULL;
	}
};

class _XRealTimeObject : public _XItem
{
public:
	int		   m_ObjectIndex;
	D3DXMATRIX m_matWorldPosition;
	FLOAT	   m_Distance;
public:
	_XRealTimeObject()
	{		
		m_ObjectIndex = -1;
		m_Distance = 9999.0f;
				
		memset( m_matWorldPosition, 0, sizeof(D3DXMATRIX) );
		m_matWorldPosition._11 = 1.0f;
		m_matWorldPosition._22 = 1.0f;
		m_matWorldPosition._33 = 1.0f;
		m_matWorldPosition._44 = 1.0f;
	}	
};

#ifdef _XUSEFMOD
class IEnvSound3D
{
public:	
	int			soundindex;
#ifdef _XDWDEBUG
	TCHAR		envsoundname[64];
#endif

	IEnvSound3D()
	{	
		soundindex = -1;
#ifdef _XDWDEBUG
		memset( envsoundname, 0, sizeof(TCHAR) * 64 );
#endif
	}
};
#else
class IEnvSound3D : public _XReference
{
public:
	ISound3D*		m_pSound3D;
	char			envsoundname[256];

	IEnvSound3D()
	{
		m_pSound3D = NULL;
	}
};
#endif

class _XSoundObject : public _XObject
{
public :

	enum
	{
		ATTRIBUTE_NORMAL = 0x00,
		ATTRIBUTE_REPEAT
	};
	enum
	{
		EVENT_ALL = 0x00,
		EVENT_DAWN,
		EVENT_DAY,
		EVENT_NIGHTFALL,
		EVENT_NIGHT
	};

	BYTE			m_byAttribute;
	BYTE			m_byEvent;
	BYTE			m_byDistance;
	BYTE			m_byInterval;
	BYTE			m_byCount;

	// Temp
	FLOAT			m_fDistance2;

	int				m_nPlayCount;
	FLOAT			m_fPlayTime;

	IEnvSound3D*	m_pSound;
	BOOL			m_bPlay;
	
public :
	_XSoundObject()
	{
		m_byAttribute	= ATTRIBUTE_NORMAL;
		m_byEvent		= EVENT_ALL;
		m_byDistance	= 10;
		m_byInterval	= 0;
		m_byCount		= 1;

		m_fDistance2	= 1.f;

		m_nPlayCount	= 0;
		m_fPlayTime		= 0.f;

		m_pSound		= NULL;

		m_bPlay			= FALSE;
	}
	virtual ~_XSoundObject()
	{
	}

	void Initial()
	{
		m_nPlayCount	= 0;
		m_fPlayTime		= 0.f;
	}

	void Stop()
	{
		if (m_bPlay == TRUE)
		{
			Initial();

			m_bPlay = FALSE;
#ifdef _XUSEFMOD
			g_FMODWrapper.FL_FadeinSound( m_pSound->soundindex );
#else
			m_pSound->ReleaseRef();
			if (m_pSound->GetRef() == 0)
			{
				m_pSound->m_pSound3D->Stop();
			}
			OutputDebugString("Release Sound Ref : ");
			OutputDebugString(m_pSound->envsoundname);
			OutputDebugString("\n");
#endif
		}
	}
};

class _XParticleGroupItem;
class _XEffectObject : public _XObject
{
public :
	enum
	{
		EVENT_ALL = 0x00,
		EVENT_DAWN,
		EVENT_DAY,
		EVENT_NIGHTFALL,
		EVENT_NIGTH
	};

	BYTE			m_byEvent;
	BYTE			m_byDistance;

	_XParticleGroupItem* m_pParticleGroupItem;

public :
	_XEffectObject()
	{
		m_byEvent		= EVENT_ALL;
		m_byDistance	= 50;
		m_pParticleGroupItem = NULL;
	}
	virtual ~_XEffectObject()
	{
	}
};


//////////////////////////////////////////////////////////////////////////////////////////
// Object list
class _XObjectManager
{
public:
	int					m_ObjectCount;
	_XOctreeObject* 	m_pObjectArray;

public:
	_XObjectManager();
	virtual ~_XObjectManager();
	
	void DisposeList( void  );

	BOOL LoadObjectList( FILE* Fileptr, LPSTR Filename = NULL );

	void RebuildAboveWaterObject( void );
	
	void PreCachingTexture( void );
};

class _XSoundObjectManager
{
public:
	typedef std::vector<_XSoundObject*>		svXObject;


	svXObject			m_svXObject;

public:
	_XSoundObjectManager()				{} 
	virtual ~_XSoundObjectManager()		{ DisposeList(); }
	void insertObject( _XSoundObject* pObject )
	{
		m_svXObject.push_back(pObject);
	}
	void DisposeList()
	{
		svXObject::iterator it;
		for (it = m_svXObject.begin(); it != m_svXObject.end(); ++ it)
		{
			delete *it;
		}
		m_svXObject.clear();
	}
	int GetCount()
	{
		return m_svXObject.size();
	}
};



//////////////////////////////////////////////////////////////////////////////////////////
// Mesh object list
#define _HEADERINDICATOR_XMESHLISTFILE            0x0000AE15
#define _HEADERINDICATOR_XMESHLISTFILEVERSION	  0xA001

#define _HEADERINDICATOR_NPCLISTFILE			 0x0000AE16
#define _HEADERINDICATOR_NPCLISTFILEVERSION		 0xA001


typedef struct 
{
	WORD HeaderIndicator;
	WORD FileVersion;	
	int  MeshFileCount;
} _XMESHLISTFILE_HEADER;

typedef struct
{
	DWORD	transparent	: 1;
	DWORD	reserved	: 31;
} _XMESH_ATTRIB;

struct CiStringCompare : public std::binary_function<std::string, std::string, bool>
{
	static bool nocase_compare ( char c1, char c2)
	{
		return toupper(c1) < toupper(c2);
	}
	bool operator () (const std::string& lhs, const std::string& rhs) const
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(),
										rhs.begin(), rhs.end(),
										nocase_compare);
	}
};


class _XFunctionalObject;

class _XMeshObjectManager
{

public:
	int						m_ModelStackMaxCount;	
	int						m_ModelStackHeight;
	_XMeshContainer**		m_ModelList;
	
	typedef std::map<std::string, _XBipedController*, CiStringCompare>	smdef_BipedController;
	smdef_BipedController		m_smBipedController;

	typedef std::list<_XOctreeObject*>	sldef_OctreeObject;
	sldef_OctreeObject			m_slRenderObject;

	sldef_OctreeObject			m_slRenderBakemapObject;

	typedef std::list<_XOctreeObject*>	sldef_XOctreeObject;

	sldef_XOctreeObject		m_slEventObjectList;

public:
	_XMeshObjectManager();
	virtual ~_XMeshObjectManager();
	
	_XMeshContainer*		GetModelContainer( int index );

	BOOL					ReserveModelStack( int modelstacksize );
	BOOL					LoadMeshList( FILE* pTerrainFile );
	void					_RenderObject( _XOctreeObject& object );
	void					_RenderObject( _XFunctionalObject& object );
	void					_RenderObject( _XRealTimeObject& object );	
	void					IntersectionEventObject(D3DXVECTOR3& vRayPos, D3DXVECTOR3& vRayDir);

	BOOL					insertMeshObject( _XMeshContainer* pMeshObject );
	void					disposeList( void );

	BOOL					LoadAniMeshList( void );
	void					RenderAniObject( FLOAT fElaspedTime );
	void					RenderBakemapObject( void );
	void					RenderReflectBakemapObject( void );

	void					ResetCount()
	{
		m_slEventObjectList.clear();
	}
};


//////////////////////////////////////////////////////////////////////////////////////////
// Object frustum culling...

typedef enum _XVisibility { NO_CLIP = 0, SOME_CLIP, NOT_VISIBLE };

typedef struct
{
	float	x, z;			
	float	nx[3], nz[3];	
} _XFrustum;


// Quad tree...
typedef struct _XQNode 
{
	_XQNode*	parent;
	_XQNode*	child[2][2];
	float	cx, cz;
	int	depth;
	_XOctreeObject*	objects;

	_XQNode(_XQNode* p, float x, float z, int d) 
	{
		parent = p;
		cx = x;
		cz = z;
		depth = d;
		
		parent = 0;
		for (int j = 0; j < 2; j++) 
		{
			for (int i = 0; i < 2; i++) 
			{
				child[j][i] = 0;
			}
		}
		objects = 0;
	}
	
	~_XQNode() 
	{
		for (int j = 0; j < 2; j++) 
		{
			for (int i = 0; i < 2; i++) 
			{
				if (child[j][i]) delete child[j][i];
			}
		}
	}
}_XQNode;


#define _XDEF_FRUSTUM_COUNT					100
#define _XDEF_DEFAULT_LOOSEFACTOR			2.0f
#define _XDEF_MAX_QTREEDEPTH				5
#define _XDEF_WORLD_SIZE					1024.0f
#define _XDEF_MAX_RADIUS					30.0f
#define _XDEF_MIN_RADIUS					30.0f

#define _XDEF_DEFAULT_ANGLE					_X_RAD(0.0f) //0.785398f
#define _XDEF_DEFAULT_FOV					_X_RAD(g_DefaultCameraFov) //0.785398f //(_X_PI / 4.0f)
#define _XDEF_DEFAULTCULLINGSQRTFARPLANE	100.0f		//160.f	* 2
#define _XDEF_DEFAULTCULLINGFARPLANE		200.0f		//160.f	* 2
#define _XDEF_MINIMUMCULLINGSQRTFARPLANE	60.0f		//60.f * 2
#define _XDEF_MINIMUMCULLINGFARPLANE		120.0f		//60.f * 2
#define _XDEF_EXPANDFRUSTUMFOV				1.35f

class _XQuadTree
{

//protected:
public:	
	_XQNode*				m_pRoot;
	float					m_LooseFactor;
	_XFrustum				m_FrustumList[_XDEF_FRUSTUM_COUNT];

	_XFrustum				m_CurrentFrustum;
	int						m_LastMouseX;
	int						m_LastMouseY;

	int						m_ObjectCount;
	_XOctreeObject*			m_pObjectList;
	_XMeshObjectManager*	m_pMeshObjectManager;

	FLOAT					m_CameraAngle;
	FLOAT					m_CameraFov;

	FLOAT					m_CullingFarPlane;

	FLOAT					m_WorldSize;

	FLOAT					m_Alpharate;

	//	Test data//////////////////////////////////////////////
	int						m_FrustumTotals[_XDEF_FRUSTUM_COUNT];
	int						m_DepthTotals[_XDEF_MAX_QTREEDEPTH];
	int						m_NodesChecked;
	int						m_ObjectsActuallyInFrustum;
	int						m_ObjectsChecked;	

#ifdef _XDWDEBUG
	BOOL					m_PlayerCulling; // 유저 시점 컬링 플래그
	BOOL					m_ViewFrustum;	 // 프러스텀 라인 표시 플래그 
	int						m_RenderedObject;
	int						m_AlphaBlendObject;
	FLOAT					m_FrustumHeight;
#endif

public:	

	_XQuadTree();
	~_XQuadTree();

	BOOL					CreateTree( FLOAT worldsize, int objectcount, _XOctreeObject* objectlist );
	void					DisposeTree( void );

	_XVisibility			CheckBoxAgainstFrustum(FLOAT cx, FLOAT cz, FLOAT HalfSize, _XFrustum* f);
	_XVisibility			CheckObjectAgainstFrustum(_XOctreeObject* o, _XFrustum* f);
	_XVisibility			CheckObjectAgainstCurrrentFrustum(D3DXVECTOR3 oVec, FLOAT radius );

	int						QuadTreeInsert( _XQNode* q, _XOctreeObject* o);
	BOOL					FitsInBox(_XOctreeObject* o, FLOAT cx, FLOAT cz, FLOAT HalfSize);
	BOOL					CheckObjectAgainstObject(_XOctreeObject* o, _XOctreeObject* p);
	int						CountQTreeNodes( _XQNode* q );
	int						CountNodes(_XQNode* q);
	int						QuadTreeCountObjectsInFrustum(_XQNode* q, _XFrustum* f, _XVisibility vis);
	int						OrdinaryCountContactingObjects(_XQNode* q, _XOctreeObject* o);
	int						LooseQuadTreeInsert(_XQNode* q, _XOctreeObject* o);
	int						LooseQuadTreeCountObjectsInFrustum(_XQNode* q, _XFrustum* f, _XVisibility vis);
	int						LooseCountContactingObjects(_XQNode* q, _XOctreeObject* o);
	
	void					SetupFrustumNormals(_XFrustum* f, FLOAT ViewAngle, FLOAT FOV);
	void					SetupFrustumNormals( void );

	void					SetObjectList( int objectcount, _XOctreeObject* objectlist )
	{  
							this->m_ObjectCount = objectcount;  
							this->m_pObjectList = objectlist;
	}
	void					SetCameraInfo( _X3PCamera& camera );
	void					UpdateCamera( _X3PCamera& camera );

	void					_Render( _XMeshObjectManager* meshobjectmanager );
	void					_Render(_XQNode* q, _XFrustum* f, _XVisibility vis);	

};



#endif // !defined(_XOBJECTMANAGER_H_)
