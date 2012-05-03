#ifndef _XMODELCONTAINERDEFINE_H_
#define _XMODELCONTAINERDEFINE_H_

#include "XMeshMath.h"

///////////////////////////////////////////////////////////////////////////////////////
// Define of XModel file
#define _XDEF_X3DFILEINDICATOR		0x0000AE01
#define _XDEF_X3DFILEVERSION		0.72f
#define _XDEF_X3DFILEVERSION_0_73	0.73f

#define _XDEF_XAFILEINDICATOR		0x0000AE00
#define _XDEF_XAFILEVERSION			0.6f

#define _XDEF_MAX_SKINNINGCOUNT		8
#define _XDEF_TEMPSKINNINGBUFFER	2048

#include "XObb.h"

typedef struct 
{
	DWORD		HeaderIndicator; 
	FLOAT		Fileversion;
	
	// Obb
	D3DXVECTOR3 OBB_Center;
    D3DXVECTOR3 OBB_Axis[3];
	float		OBB_Extent[3];
} _XMESHHEADER;


typedef struct 
{
	DWORD	HeaderIndicator; 
	FLOAT	Fileversion;
	int		FirstFrame;
	int		LastFrame;
	int		FrameSpeed;
	int		TickSpeed;	
} _XAHEADER;



///////////////////////////////////////////////////////////////////////////////////////
// temporary key buffer size for loading key data...
#define _XDEF_XKEYBUFFERSIZE			1024 


///////////////////////////////////////////////////////////////////////////////////////
// Rotate & Position animation key structure
typedef struct _XM_POSITIONKEY
{
	FLOAT			nTime;
	D3DXVECTOR3		d3dVKeyVal;			 // Poistion, Scale
	_XM_POSITIONKEY(){ nTime = 0;	}
}_XM_POSITIONKEY, *_LPXM_POSITIONKEY;

typedef struct _XM_ROTATIONKEY
{
	FLOAT			nTime;
	D3DXQUATERNION	Qt;					 // Rotation
	_XM_ROTATIONKEY(){ nTime = 0;	}
}_XM_ROTATIONKEY, *_LPXM_ROTATIONKEY;


///////////////////////////////////////////////////////////////////////////////////////
// Physique informations buffer
typedef struct 
{				 
	FLOAT	fBlendFactor;				 // weight
	WORD	wKeyIdx;					 // Linked object index
}_XM_FACTOR, *_LPXM_FACTOR;


///////////////////////////////////////////////////////////////////////////////////////
// Key factor containner
class _XM_KEYFACTOR
{
public:
	int				iFactorCnt;			// how many object effect to current vertex 
	_LPXM_FACTOR	pFactorData;		// Matrix buffer

public:
	_XM_KEYFACTOR()
	{
		iFactorCnt = 0;
		pFactorData = NULL;
	}

	~_XM_KEYFACTOR()
	{
		iFactorCnt = 0;
		SAFE_DELETE_ARRAY( pFactorData);
	}

	_XM_KEYFACTOR& operator=(const _XM_KEYFACTOR& keyfactor)
	{
		if (this == &keyfactor) return *this;

		iFactorCnt = 0;
		SAFE_DELETE_ARRAY( pFactorData );

		pFactorData = new _XM_FACTOR[ keyfactor.iFactorCnt ];
		if( pFactorData )
		{
			memcpy( pFactorData, keyfactor.pFactorData, sizeof(_XM_FACTOR) * keyfactor.iFactorCnt );
			iFactorCnt = keyfactor.iFactorCnt;
		}

		return *this;
	}

};
typedef _XM_KEYFACTOR* _LPXM_KEYFACTOR;

///////////////////////////////////////////////////////////////////////////////////////
// Mesh object
class _XM_MESHOBJECT
{
public:
	int						nId;					
	int						nParentId;				

	LPDIRECT3DVERTEXBUFFER9 pVertex;
	LPDIRECT3DVERTEXBUFFER9 pSecondVertexBuffer;
	int						nVertexCnt;

	LPDIRECT3DINDEXBUFFER9	pIndex;
	DWORD					nIndexCnt;			

	D3DXMATRIX				Matrix;						// Object matrix
	D3DXMATRIX				MatrixTM;					// Object matrix
	D3DXMATRIX				Inverse;					// Inverse object matrix
	
	D3DXMATRIX				LocalTM;					// Local object matrix
	D3DXMATRIX				LocalAniTM;					// Local animation matrix
	D3DXMATRIX				PosMatrixTM;				// Object matrix from TM_ROW
	
	int						nTextureId;	
	
	int						nKeyFactorCnt;
	_LPXM_KEYFACTOR			pKeyFactor;

	// Bone을 메쉬별로 분할 렌더링 할때 사용할 멤버들
	int						BoneIndexCnt;				//현재 이 메쉬에 적용된 본의 카운트
	WORD*					pBoneIndexArray;			//본의 인덱스

	BOOL					bBakemap;
	int						nBakemapId;	
	
public:
	_XM_MESHOBJECT() 
	{
		nId					=  0;
		nParentId			= -1;
			
		nVertexCnt			=  0;
		pVertex				=  NULL;		
		pSecondVertexBuffer =  NULL;

		nIndexCnt			=  0;
		pIndex				=  NULL;

		pKeyFactor			=  NULL;
		nKeyFactorCnt		=  0;
	
		nTextureId			= -1;		

		BoneIndexCnt		= 0;
		pBoneIndexArray		= NULL;

		bBakemap			= FALSE;
		nBakemapId			= -1;	

		D3DXMatrixIdentity(&Matrix);
		D3DXMatrixIdentity(&Inverse);
		D3DXMatrixIdentity(&LocalTM);
		D3DXMatrixIdentity(&LocalAniTM);		
	} 
	
	~_XM_MESHOBJECT()
	{
		DestroyObject();
	}

	void DestroyObject( void )
	{	
		SAFE_RELEASE( pVertex  );	
		SAFE_RELEASE( pSecondVertexBuffer );
		SAFE_RELEASE( pIndex );

		SAFE_DELETE_ARRAY( pKeyFactor );		
		SAFE_DELETE_ARRAY( pBoneIndexArray );
	}
};

typedef _XM_MESHOBJECT* _LPXM_MESHOBJECT;


///////////////////////////////////////////////////////////////////////////////////////
// Mesh object
class _XM_BIPEDOBJECT
{
public:
	int						nId;					
	int						nParentId;
	
	LPTSTR					strName;

	D3DXMATRIX				m_matTM;					//로컬좌표의 버텍스를 월드 좌표로 변환 하는 매트릭스
	D3DXMATRIX				m_matInvTM;					//TM의 인버스
	D3DXMATRIX				m_matAniTM;
	D3DXMATRIX				m_localTM;					//하이러키 구조에서의 본에서 로컬 TM = TM * 부모 TM의 인버스
	D3DXMATRIX				m_matLocalAni;				//버텍스의 에니메이션이 되는 에니메이션의 매트릭스 
	
	int						m_keystacksize;

	int*					m_pnPosKey;
	_LPXM_POSITIONKEY*		m_pPosKey;

	int*					m_pnRotKey;
	_LPXM_ROTATIONKEY*		m_pRotKey;	
	
public:
	_XM_BIPEDOBJECT() 
	{
		nId					=  0;
		nParentId			= -1;

		strName				=  NULL;

		m_keystacksize		=  0;

		m_pnPosKey			=  NULL;
		m_pPosKey			=  NULL;
		m_pnRotKey			=  NULL;
		m_pRotKey			=  NULL;

		D3DXMatrixIdentity(&m_matTM);
		D3DXMatrixIdentity(&m_matInvTM);
		D3DXMatrixIdentity(&m_localTM);
		D3DXMatrixIdentity(&m_matLocalAni);
	} 
	
	~_XM_BIPEDOBJECT()
	{
		DestroyObject();
	}

	void ReserveAnimationKeyStack( int stacksize )
	{	
		for( int i = 0; i < m_keystacksize; i++)
		{
			if( m_pPosKey ) SAFE_DELETE_ARRAY(m_pPosKey[i]);
			if( m_pRotKey ) SAFE_DELETE_ARRAY(m_pRotKey[i]);
		}		
		
		SAFE_DELETE_ARRAY( m_pnPosKey );
		SAFE_DELETE_ARRAY( m_pPosKey  );
		SAFE_DELETE_ARRAY( m_pnRotKey );
		SAFE_DELETE_ARRAY( m_pRotKey  );

		m_keystacksize = stacksize;

		m_pnPosKey = new int[ m_keystacksize ];		
		m_pPosKey  = new _LPXM_POSITIONKEY[ m_keystacksize ];

		m_pnRotKey = new int[ m_keystacksize ];
		
		m_pRotKey  = new _LPXM_ROTATIONKEY[ m_keystacksize ];

		memset( m_pnPosKey, 0, sizeof(int) * m_keystacksize );
		memset( m_pnRotKey, 0, sizeof(int) * m_keystacksize );

		for( i = 0; i < m_keystacksize; i++)
		{
			m_pPosKey[i] = NULL;
			m_pRotKey[i] = NULL;
		}		
	}

	void DestroyObject( void )
	{	
		SAFE_DELETE_ARRAY( strName );

		for( int i = 0; i < m_keystacksize; i++)
		{
			if( m_pPosKey ) SAFE_DELETE_ARRAY(m_pPosKey[i]);
			if( m_pRotKey ) SAFE_DELETE_ARRAY(m_pRotKey[i]);
		}		

		SAFE_DELETE_ARRAY( m_pnPosKey );
		SAFE_DELETE_ARRAY( m_pPosKey  );

		SAFE_DELETE_ARRAY( m_pnRotKey );
		SAFE_DELETE_ARRAY( m_pRotKey  );

		m_keystacksize = 0;
	}
};

typedef _XM_BIPEDOBJECT* _LPXM_BIPEDOBJECT;

#define _XDEF_ACI_NONE					0
#define _XDEF_ACI_FIRSTFRAME			1
#define _XDEF_ACI_COMMANDFRAME			2
#define _XDEF_ACI_LASTFRAME				4
#define _XDEF_ACI_SECONDFRAME			8
#define _XDEF_ACI_THIRDFRAME			16

#define _XDEF_NOEFFECTVALUE				255
#define _XDEF_NOEFFECTWORDVALUE			65535
#define _XDEF_MAXFRAMEEVENT				9

#define _XDEF_ATTACKCLASS_LOW			0	
#define _XDEF_ATTACKCLASS_MIDIUM		1
#define _XDEF_ATTACKCLASS_HIGH			2
#define _XDEF_ATTACKCLASS_CRITICAL		3

#define _XDEF_SECONDSEQUENCE_FRAME		36
#define _XDEF_THIRDSEQUENCE_FRAME		71

typedef struct
{
	bool	attackcmd_sended[_XDEF_MAXFRAMEEVENT];	
	bool	soundplayed[_XDEF_MAXFRAMEEVENT];
	bool	effectplayed[_XDEF_MAXFRAMEEVENT];
	bool	firstframe;
	bool	lastframe;
	bool	secondframe;
	bool	thirdframe;
} _XANIEVENT_STRUCTURE;

typedef struct _XM_MOTION
{	
	int		MotionNum;

	int		StartFrame;					
	int		EndFrame;					

	int		FrameSpeed;					
	int		orgFrameSpeed;	

	BYTE	AttackClass[_XDEF_MAXFRAMEEVENT];
	BYTE	AttackCmdFrame[_XDEF_MAXFRAMEEVENT];
	BYTE	WeaponClass[_XDEF_MAXFRAMEEVENT];		// 0 이면 메인 무기 1이면 발차기

	WORD	SoundIndex[_XDEF_MAXFRAMEEVENT];
	BYTE	SoundOutFrame[_XDEF_MAXFRAMEEVENT];	
	
	WORD	EffectIndex[_XDEF_MAXFRAMEEVENT];
	BYTE	EffectFrame[_XDEF_MAXFRAMEEVENT];	
	BYTE	EffectPosition[_XDEF_MAXFRAMEEVENT];		

	int		UseItemIndex;
	
	_XM_MOTION()
	{
		ResetMotionData();
	}
	
	void ResetMotionData( void )
	{
		MotionNum		= 0;		
		StartFrame		= 0;
		EndFrame		= 0;
		FrameSpeed		= 0;
		
		UseItemIndex	= -1;

		for( int i = 0; i < _XDEF_MAXFRAMEEVENT; i++  )
		{
			WeaponClass[i]		= 0;
			AttackClass[i]		= _XDEF_ATTACKCLASS_LOW;
			AttackCmdFrame[i]	= _XDEF_NOEFFECTVALUE;
			SoundIndex[i]		= _XDEF_NOEFFECTWORDVALUE;
			SoundOutFrame[i]	= _XDEF_NOEFFECTVALUE;
			EffectIndex[i]		= _XDEF_NOEFFECTWORDVALUE;
			EffectFrame[i]		= _XDEF_NOEFFECTVALUE;
			EffectPosition[i]	= _XDEF_NOEFFECTVALUE;
		}		
	}

	void SetAttackClass( DWORD index, BYTE attackclass = _XDEF_NOEFFECTVALUE )
	{ 
		if( index < _XDEF_MAXFRAMEEVENT )
		{
			AttackClass[index]	  = attackclass;			
		}
	};	
	void SetWeaponClass( DWORD index, BYTE weaponclass = 0 )
	{ 
		if( index < _XDEF_MAXFRAMEEVENT )
		{
			WeaponClass[index]	  = weaponclass;
		}
	};	

	void SetAttackFrame( DWORD index, BYTE attackcmdframe = _XDEF_NOEFFECTVALUE )
	{ 
		if( index < _XDEF_MAXFRAMEEVENT )
		{			
			AttackCmdFrame[index] = attackcmdframe; 
		}
	};
	
	void SetSoundEffectFrame( DWORD index, BYTE soundoutframe = _XDEF_NOEFFECTVALUE, WORD soundindex = _XDEF_NOEFFECTWORDVALUE )
	{ 
		if( index < _XDEF_MAXFRAMEEVENT )
		{
			SoundOutFrame[index] = soundoutframe;
			SoundIndex[index]	 = soundindex;
		}
	};

	void SetEffectFrame( DWORD index, BYTE effectoutframe = _XDEF_NOEFFECTVALUE, WORD effectindex = _XDEF_NOEFFECTWORDVALUE )	 					 
	{
		if( index < _XDEF_MAXFRAMEEVENT )
		{
			EffectFrame[index]	= effectoutframe; 
			EffectIndex[index]	= effectindex; 		
		}
	}

	void SetEffectPosition( DWORD index, BYTE effectposition = _XDEF_NOEFFECTVALUE )
	{
		if( index < _XDEF_MAXFRAMEEVENT )
		{
			EffectPosition[index]	= effectposition;
		}
	}

} _XM_MOTION, *_LPXM_MOTION;


typedef struct 
{
	int ObjectIndex;
	int VertexIndex;
} _XM_SKINNINGVERTEX;


typedef struct _XM_SKINNING
{
	int	  SkCnt;
	_XM_SKINNINGVERTEX SkIndex[_XDEF_MAX_SKINNINGCOUNT];

	_XM_SKINNING() { SkCnt = 0; }
	~_XM_SKINNING(){ SkCnt = 0; }
} _XM_SKINNING, *_LPXM_SKINNING;


typedef struct _XM_SKINNINGDATA
{
		float VertexX;
		float VertexY;
		float VertexZ;

		int  SX;
		int  SY;
		int  SZ;

		int	  SkCnt;
		_XM_SKINNINGVERTEX SkIndex[_XDEF_MAX_SKINNINGCOUNT];

		_XM_SKINNINGDATA()
		{
			VertexX = 0.0f;
			VertexY = 0.0f;
			VertexZ = 0.0f;
			SkCnt = 0;
		}

		~_XM_SKINNINGDATA()
		{
			VertexX = 0.0f;
			VertexY = 0.0f;
			VertexZ = 0.0f;
			SkCnt = 0;
		}
} _XM_SKINNINGDATA;



#define _XM_SET_ALPHABLEND_ON		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			\
									gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  TRUE );			\
									gpDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE );			
									/*gpDev->SetRenderState(D3DRS_ZENABLE, FALSE);*/
									//gpDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

#define _XM_SET_ALPHABLEND_OFF		gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,   FALSE );		\
									gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,    FALSE );		\
									gpDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				
									/*gpDev->SetRenderState(D3DRS_ZENABLE, g_RS_UseZBuffer);	*/
									//gpDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

#define _XM_SET_ALPHABLENDNOZ_ON	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			\
									gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  TRUE );			\
									gpDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE );

#define _XM_SET_ALPHABLENDNOZ_OFF	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE,   FALSE );		\
									gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,    FALSE );		\
									gpDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#define _XM_SET_ALPHATEST_ON		gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  TRUE  );
#define _XM_SET_ALPHATEST_OFF		gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE );


#endif