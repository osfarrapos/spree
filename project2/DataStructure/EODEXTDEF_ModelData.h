#ifndef _EODEXTDEF_MODELDATA
#define _EODEXTDEF_MODELDATA

// Common object <biped & mesh>
extern _XBipedController	g_FemaleBipedObject;
extern _XBipedController	g_MaleBipedObject;
extern _XOBBData			g_CameraOBB;
extern D3DXMATRIX			g_BaseOBBAxisMatrix;

// Model type 
#define		_XDEF_MAXMODEL_COMBINATIONCOUNT		19

#define		_XDEF_MODELCLUSTER_FACE				0
#define		_XDEF_MODELCLUSTER_UPPERBODY		1
#define		_XDEF_MODELCLUSTER_LOWERBODY		2
#define		_XDEF_MODELCLUSTER_LHAND			3
#define		_XDEF_MODELCLUSTER_RHAND			4
#define		_XDEF_MODELCLUSTER_LFOOT			5
#define		_XDEF_MODELCLUSTER_RFOOT			6
#define		_XDEF_MODELCLUSTER_CAP				7
#define		_XDEF_MODELCLUSTER_ADAE				8
#define		_XDEF_MODELCLUSTER_EARRING			9
#define		_XDEF_MODELCLUSTER_BACKPACK			10
#define		_XDEF_MODELCLUSTER_ITEM1			11
#define		_XDEF_MODELCLUSTER_ITEM2			12
#define		_XDEF_MODELCLUSTER_ITEM3			13
#define		_XDEF_MODELCLUSTER_ITEM4			14
#define		_XDEF_MODELCLUSTER_ITEM5			15
#define		_XDEF_MODELCLUSTER_HAIR				16
#define		_XDEF_MODELCLUSTER_MASK				17
#define		_XDEF_MODELCLUSTER_FACEMASK			18	

#define		_XDEF_MODELCLUSTER_WEAPON			20
#define		_XDEF_MODELCLUSTER_MARTIALBOOK		21
#define		_XDEF_MODELCLUSTER_POTION			22
#define		_XDEF_MODELCLUSTER_THROW			23
#define		_XDEF_MODELCLUSTER_SOCERY			24
#define		_XDEF_MODELCLUSTER_ETC				25
#define		_XDEF_MODELCLUSTER_JEWELBOX			26
#define		_XDEF_MODELCLUSTER_ELIXIR			27


// Model type max count	
#define _XDEF_USERMODELTYPECOUNT_HAIR			600
#define _XDEF_USERMODELTYPECOUNT_FACE			600
#define _XDEF_USERMODELTYPECOUNT_UBODY			600
#define _XDEF_USERMODELTYPECOUNT_LBODY			600
#define _XDEF_USERMODELTYPECOUNT_LHAND			600
#define _XDEF_USERMODELTYPECOUNT_RHAND			600
#define _XDEF_USERMODELTYPECOUNT_LFOOT			600
#define _XDEF_USERMODELTYPECOUNT_RFOOT			600
#define _XDEF_USERMODELTYPECOUNT_CAP			800
#define _XDEF_USERMODELTYPECOUNT_ADAE			600
#define _XDEF_USERMODELTYPECOUNT_EARRING		600
#define _XDEF_USERMODELTYPECOUNT_BACKPACK		600
#define _XDEF_USERMODELTYPECOUNT_MASK			300
#define _XDEF_USERMODELTYPECOUNT_FACEMASK		10

#define _XDEF_ITEMMODELTYPECOUNT_WEAPON			500
#define _XDEF_ITEMMODELTYPECOUNT_MARTIALBOOK	100
#define _XDEF_ITEMMODELTYPECOUNT_POTION			150
#define _XDEF_ITEMMODELTYPECOUNT_THROW			100
#define _XDEF_ITEMMODELTYPECOUNT_SOCERY			100
#define _XDEF_ITEMMODELTYPECOUNT_ETC			100
#define _XDEF_ITEMMODELTYPECOUNT_JEWELBOX		20
#define _XDEF_ITEMMODELTYPECOUNT_ELIXIR			20

extern int					g_Model_GenderSeparateIndex;	// 이수치에서 성별을 구분함
extern int					g_MaskModel_GenderSeparateIndex;// 이수치에서 마스크모델 성별을 구분함
extern int					g_MaskModel_GenderSeparateIndex2;// 이수치에서 마스크모델 두번째 추가 모델 성별을 구분함
extern int					g_FaceMaskModel_GenderSeparateIndex;

extern int					g_Model_GroupSeparateIndex[9];	// 이수치에서 문파를 구분함

extern int					g_Model_SorimHairSeparateIndex;	// 이수치에서 소림사 머리를 구분함

//------------------------------------------------------------------------------------
// Model texture manager...
extern _XTextureManager		g_CharacterTextureArchive;
extern _XTextureManager		g_MobTextureArchive;

//------------------------------------------------------------------------------------
// Character model archive...
extern _XAnimMeshAbstractClass* g_UserModel_HairType	[_XDEF_USERMODELTYPECOUNT_HAIR    ];
extern _XAnimMeshAbstractClass* g_UserModel_FaceType	[_XDEF_USERMODELTYPECOUNT_FACE    ];
extern _XAnimMeshAbstractClass* g_UserModel_UBodyType	[_XDEF_USERMODELTYPECOUNT_UBODY   ];
extern _XAnimMeshAbstractClass* g_UserModel_LBodyType	[_XDEF_USERMODELTYPECOUNT_LBODY   ];
extern _XAnimMeshAbstractClass* g_UserModel_LHandType	[_XDEF_USERMODELTYPECOUNT_LHAND   ];
extern _XAnimMeshAbstractClass* g_UserModel_RHandType	[_XDEF_USERMODELTYPECOUNT_RHAND   ];
extern _XAnimMeshAbstractClass* g_UserModel_LFootType	[_XDEF_USERMODELTYPECOUNT_LFOOT   ];
extern _XAnimMeshAbstractClass* g_UserModel_RFootType	[_XDEF_USERMODELTYPECOUNT_RFOOT   ];
extern _XAnimMeshAbstractClass* g_UserModel_Cap			[_XDEF_USERMODELTYPECOUNT_CAP     ];
extern _XAnimMeshAbstractClass* g_UserModel_Adae		[_XDEF_USERMODELTYPECOUNT_ADAE    ];
extern _XAnimMeshAbstractClass* g_UserModel_Earring		[_XDEF_USERMODELTYPECOUNT_EARRING ];
extern _XAnimMeshAbstractClass* g_UserModel_BackPack    [_XDEF_USERMODELTYPECOUNT_BACKPACK];


extern _XAnimMeshAbstractClass* g_ItemModel_Weapon		[2][_XDEF_ITEMMODELTYPECOUNT_WEAPON	 ];
extern _XAnimMeshAbstractClass* g_ItemModel_MartialBook	[_XDEF_ITEMMODELTYPECOUNT_MARTIALBOOK];
extern _XAnimMeshAbstractClass* g_ItemModel_Potion		[_XDEF_ITEMMODELTYPECOUNT_POTION	 ];
extern _XAnimMeshAbstractClass* g_ItemModel_Throw		[_XDEF_ITEMMODELTYPECOUNT_THROW		 ];
extern _XAnimMeshAbstractClass* g_ItemModel_Socery		[_XDEF_ITEMMODELTYPECOUNT_SOCERY	 ];
extern _XAnimMeshAbstractClass* g_ItemModel_ETC			[_XDEF_ITEMMODELTYPECOUNT_ETC		 ];
extern _XAnimMeshAbstractClass* g_ItemModel_JewelBox	[_XDEF_ITEMMODELTYPECOUNT_JEWELBOX	 ];
extern _XAnimMeshAbstractClass* g_ItemModel_Elixir		[_XDEF_ITEMMODELTYPECOUNT_ELIXIR	 ];

extern _XAnimMeshAbstractClass* g_UserModel_Mask		[_XDEF_USERMODELTYPECOUNT_MASK		 ];
extern _XAnimMeshAbstractClass* g_UserModel_FaceMask	[_XDEF_USERMODELTYPECOUNT_FACEMASK		 ];


//=================================================================================
// Monster model archive...
#define _XDEF_MODELCLASSCOUNT_MOB			2100
extern _XBipedController		g_MOBBIPEDMODEL[_XDEF_MODELCLASSCOUNT_MOB];
extern _XAnimMeshAbstractClass* g_MOBMODEL[_XDEF_MODELCLASSCOUNT_MOB];

//=================================================================================
// NPC model archive...
#define _XDEF_MODELCLASSCOUNT_NPC			200

extern _XBipedController		g_NPCBIPEDMODEL[_XDEF_MODELCLASSCOUNT_NPC];
extern _XAnimMeshAbstractClass*	g_NPCMODEL[_XDEF_MODELCLASSCOUNT_NPC];


// ================================================================================
// Dummy model archive
#define _XDEF_MODELCLASSCOUNT_DUMMY			6
extern _XBipedController			g_DUMMYBIPEDMODEL[_XDEF_MODELCLASSCOUNT_DUMMY];
extern _XAnimMeshAbstractClass*		g_DUMMYMODEL[_XDEF_MODELCLASSCOUNT_DUMMY];


// ================================================================================
// Seat model archive
extern _XAnimMeshAbstractClass*		g_SEATMODEL[_XDEF_MODELCLASSCOUNT_DUMMY];

// ================================================================================
// Flag model archive... - 비무 홍보용 flag
#define _XDEF_MODELCLASSCOUNT_FLAG			6
extern _XAnimMeshAbstractClass*		g_FLAGMODEL[_XDEF_MODELCLASSCOUNT_FLAG];


#ifdef _XDWDEBUG	
extern _XMeshContainer		g_AxisModel;
#endif


#endif