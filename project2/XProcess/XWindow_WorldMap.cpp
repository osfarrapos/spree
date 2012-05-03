// XWindow_WorldMap.cpp: implementation of the XWindow_WorldMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "xbasedefs.h"
#include "xsr_stringheader.h"
#include "XMLPaser.h"

#include "XWindow_WorldMap.h"

XWindow_WorldMap*	g_pWorldMap_Window = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XWindow_WorldMap::XWindow_WorldMap() : m_pWorldmapTex(NULL), m_pWorldMapImage(NULL), m_pCharinfo(NULL), m_pBoard(NULL),
									   m_mpData(), m_nMaxLinkMapCount(0), m_nFindKey(-1), m_bAlphaProcess(FALSE)
{

}

XWindow_WorldMap::~XWindow_WorldMap()
{

}

BOOL XWindow_WorldMap::Initialize(XMLPaser& m_xXML)
{
	//윈도우의 크기 //사용안함
	RECT curwindowrect = { m_WindowPosition.x, m_WindowPosition.y, 
						   m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy };
	FLOAT fWidth  = curwindowrect.right  - curwindowrect.left;
	FLOAT fHeight = curwindowrect.bottom - curwindowrect.top;

	//텍스쳐 로딩
	_XPackageArchive TextureArchive;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Texture\\") );
	strcat( packagefilename, _XDEF_TEXTUREFILE_A );

	//저장될 메모리
	LPBYTE pImageVFMemory = NULL;

	try
	{
		int nRes = g_MainInterfaceTextureArchive.FindResource( "worldmap_icon.tga" );
		
		if( nRes < 0 )	
		{
			_XLog( "Can't found worldmap_icon.tga file" );
			throw(0);
		}


		m_pBoard = new _XImageStatic;
		m_pBoard->Create( 20, 30, 20 + 341, 30 + 68, &g_MainInterfaceTextureArchive, nRes );	
		m_pBoard->SetClipRect( 0, 422, 341, 422 + 68 );
		m_pBoard->m_FColor = D3DCOLOR_ARGB( 129, 255,255,255 );
		InsertChildObject( m_pBoard );

		//열기
		if( !TextureArchive.OpenPackage( packagefilename ) ) 
			throw(0);
	
		TCHAR messagestring[256];
		memset( messagestring, 0, sizeof(TCHAR)*256 );
		
#ifdef _XVIETNAMESE
		sprintf( messagestring, "Vn_");
		strcat( messagestring, "worldmap.tga" );
#elif _XTAIWANESE
		sprintf( messagestring, "Tw_");
		strcat( messagestring, "worldmap.tga" );
#elif _XENGLISH
		sprintf( messagestring, "Us_");
		strcat( messagestring, "worldmap.tga" );
#elif _XJAPANESE
		sprintf( messagestring, "Jp_");
		strcat( messagestring, "worldmap.tga" );
#elif _XRUSSIAN
		sprintf( messagestring, "Rs_");
		strcat( messagestring, "worldmap.tga" );
#else
		sprintf( messagestring, "worldmap_kr.tga");
#endif
		//월드맵을 찾는다
		nRes = TextureArchive.FindResource( messagestring );

		if( nRes < 0 )
			throw(0);


		int   filesize = TextureArchive.GetPackedFileSize(nRes);
		FILE* pFP = TextureArchive.GetPackedFile(nRes);
		
		pImageVFMemory = new BYTE[ filesize ];

		if( !pImageVFMemory )
			throw(0);
		
		if( fread( pImageVFMemory, filesize, 1, pFP ) < 1 )
			throw(0);

		TextureArchive.FinalizePackage();
		
		SAFE_RELEASE( m_pWorldmapTex );

		if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, pImageVFMemory, filesize, 
			D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
			D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
			D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
			0, NULL, NULL, &m_pWorldmapTex) ) )
		{
			throw(0);
		}
		
		delete[] pImageVFMemory;
		
		//닫기
		TextureArchive.FinalizePackage();

		//이미지 저장
		m_pWorldMapImage = new _XImageStatic;
		if( !m_pWorldMapImage )
			throw(0);

		m_pWorldMapImage->Create( 0, 0, 1024, 768, &g_MainInterfaceTextureArchive, -1 );
		m_pWorldMapImage->SetClipRect( 0, 0, 1024, 768 );

		/*if( gnWidth < 1024 && gnHeight < 768 )
		{
			float fWidthScale = (float)gnWidth/1024.f;
			float fHeightScale = (float)gnHeight/768.f;
			m_pWorldMapImage->SetScale( fWidthScale, fHeightScale );
		}
		else*/
			m_pWorldMapImage->SetScale( 1.0f, 1.0f );

		m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 0, 255,255,255 );

		nRes = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
		_XBTN_STRUCTURE Closebtnstruct = 
		{ 
			TRUE, { m_pWorldMapImage->m_WindowRect.right - 90, m_pWorldMapImage->m_WindowRect.bottom - 64 }, { 60, 24 }, 
			_XDEF_WORLDMAP_CLOSEBUTTON, nRes, nRes, nRes, &g_MainInterfaceTextureArchive 
		};
		//돌아가기 버튼
		_XButton* pButton = new _XButton;
		pButton->Create( Closebtnstruct  );	
		pButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
		pButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CLOSE); //닫기
		pButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
		pButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
		pButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
		pButton->ShowWindow( TRUE );
		InsertChildObject( pButton );

		//텍스쳐 셋팅
		nRes  = g_MainInterfaceTextureArchive.FindResource( "worldmap_icon.tga" );
		
		m_pCharinfo = new _XTextureAnimation( 0, 0, 64, 64, this, &g_MainInterfaceTextureArchive, nRes,
											  7, 1, 7, 0, 192 );
		m_pCharinfo->ShowWindow( FALSE );
		//m_pCharinfo.Create(0, 0, 64, 64, &g_MainInterfaceTextureArchive, nRes);
		//m_pCharinfo.SetWindow( this );
		//m_pCharinfo.InitLocationA( -1, 7, 1, 7, 0, 192 );

		LoadXMLTable(m_xXML, _T("world_map") );
	}
	catch(...)
	{
		if(pImageVFMemory)
			delete[] pImageVFMemory;

		SAFE_RELEASE( m_pWorldmapTex );
		SAFE_DELETE( m_pWorldMapImage );

		//닫기
		TextureArchive.FinalizePackage();

		return FALSE;
	}

	return TRUE;
}

BOOL XWindow_WorldMap::LoadXMLTable(XMLPaser& m_xXML, LPTSTR pTableName )
{
	map<int, MapInfo*>::iterator iter;

	//버튼의 시작아이디..
	int nBtnID = _XDEF_WORLDMAP_MAPBUTTON_01;

	//토큰을 위함..
	char szTemp[256];
	char* szToken;
	memset( szTemp, 0, sizeof(szTemp));

	//엑셀을 로드 하기 전에... 메모리로 저장하여야 한다
	//테스트용   차후 팩 으로 교체
	TCHAR szBuffer[MAX_PATH];
	memset( szBuffer, 0, sizeof(szBuffer) );
	strcpy( szBuffer, gModulePath );
	strcat( szBuffer, "\\GameInfo.xml" );
	//FILE* fp = fopen( szBuffer, "rb"  );

#define _XDEF_BLOCKREAD
	
/*#ifdef _XDEF_BLOCKREAD
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	int resid = g_ScriptArchive.FindResource(_T("GameInfo.xml"));	
	FILE* pFile = g_ScriptArchive.GetPackedFile(_T("GameInfo.xml"));

	if( resid >= 0 && pFile )
	{
		int filesize = g_ScriptArchive.GetPackedFileSize(resid);
		
		LPTSTR pBuffer = NULL;
		pBuffer = new TCHAR[ filesize+1 ];

		if( pBuffer )
		{
			memset( pBuffer, 0, sizeof(TCHAR) * (filesize + 1) );
			fread(  pBuffer, filesize, 1, pFile );
			
			if( E_FAIL == m_xXML.ND_hrOpenMemory(pBuffer) )
			{
				_XFatalError( "Can't read GameInfo.xml file" );
				
				delete[] pBuffer;
				g_ScriptArchive.ClosePackage();

				return FALSE;
			}		
			delete[] pBuffer;
		}
		else
		{
			g_ScriptArchive.ClosePackage();
			
			_XFatalError( "Can't allocate xml buffer" );			
			return FALSE;
		}
	}
	else
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	g_ScriptArchive.ClosePackage();

#else
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	FILE* fp = g_ScriptArchive.GetPackedFile(_T("GameInfo.xml"));

	//엑셀 파일을 불러오자
	if( !LoadFile( fp ) )
	{
		_XLog( "GameInfo.xml 파일로드 실패" );
		return FALSE;
	}
#endif*/
	//엑셀 파일 로드
	//if( E_FAIL == m_xXML.ND_hrOpen( _T("system.xml")) )
	//	return FALSE;

	//해당 테이블이 있는지 검사
	if( ! m_xXML.ND_isThereTable( pTableName ) )
	{
		return FALSE;
	}

	int TableRowCount = m_xXML.ND_nGetTableRowCount( pTableName );

	if(TableRowCount <=0)	
		return FALSE;

	TableRowCount -= 1;

	//텍스쳐 로딩
	_XPackageArchive TextureArchive;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Texture\\") );
	strcat( packagefilename, _XDEF_TEXTUREFILE_A );

	//열기
	if( !TextureArchive.OpenPackage( packagefilename ) ) 
		return FALSE;

	for( int i = 0; i < TableRowCount; i++ )
	{
		iter = m_mpData.find( i );
		
		//같은 키값이 있는경우
		if( iter != m_mpData.end() )
			continue;

		MapInfo* pData = new MapInfo;

		m_xXML.ND_hrGetTableValue( pTableName, "ID", i, &pData->nID ); // 아이디 저장
		m_xXML.ND_hrGetTableValue( pTableName, "Name", i, &pData->nName ); // 맵 이름 저장

		//좌표를 저장하기 위해..
		memset( szTemp, 0, sizeof(szTemp));
		m_xXML.ND_hrGetTableValue( pTableName, "Icon_position", i, szTemp ); // 아이콘의 좌표를 저장

		//, 구분, 반드시 x,y 좌표로 이루어져 있을것, 실제 아이콘의 좌표를 저장한다
		szToken = strtok( szTemp, "," );
		if( szToken )
		{
			pData->IconPos.x = atol(szToken);
		}
		szToken = strtok( NULL, "," );
		if( szToken )
		{
			pData->IconPos.y = atol(szToken);
		}

		m_xXML.ND_hrGetTableValue( pTableName, "Icon_Index", i, &pData->nIconIndex ); //아이콘 종류 저장

		m_xXML.ND_hrGetTableValue( pTableName, "MAP_Info_1", i, &pData->nMapInfo_1 ); // 맵 첫번째 텍스트 저장
		
		memset( szTemp, 0, sizeof(szTemp));
		m_xXML.ND_hrGetTableValue( pTableName, "MAP_Info_Color_1", i, szTemp ); // 텍스트 컬러 저장
		SetStringColor( szTemp, pData->dwMapInfoColor_1 );

		m_xXML.ND_hrGetTableValue( pTableName, "MAP_Info_2", i, &pData->nMapInfo_2 ); // 맵 두번째 텍스트 저장

		memset( szTemp, 0, sizeof(szTemp));
		m_xXML.ND_hrGetTableValue( pTableName, "MAP_Info_Color_2", i, szTemp); //텍스트 컬러 저장
		SetStringColor( szTemp, pData->dwMapInfoColor_2);

		memset( szTemp, 0, sizeof(szTemp));
		m_xXML.ND_hrGetTableValue( pTableName, "Link_Map_ID", i, szTemp ); //링크 되어있는 아이디 저장
		
		int nLinkMapCheckCount = 0;
		int nTemp = 0;
		szToken = strtok( szTemp, "," );	
		if( szToken )
		{
			nTemp = atoi(szToken);
			pData->vtLinkID.push_back(nTemp);
			nLinkMapCheckCount++;
		}

		while( szToken != NULL )
		{
			szToken = strtok( NULL, "," );
			if( szToken )
			{
				nTemp = atoi(szToken);
				pData->vtLinkID.push_back(nTemp);
				nLinkMapCheckCount++;
			}
		}

		if( nLinkMapCheckCount > m_nMaxLinkMapCount )
			m_nMaxLinkMapCount = nLinkMapCheckCount;

		m_xXML.ND_hrGetTableValue( pTableName, "Application", i, &pData->bEnable ); //사용 여부 저장

		//이미지를 저장
		SetMiniMapImage(TextureArchive, *pData );

		//해당 버튼을 생성한다
		SetButton(nBtnID, *pData );
		nBtnID++; //버튼 아이디를 증가 시켜 줘야함

		m_mpData[i] = pData;
	}

	//닫기
	TextureArchive.FinalizePackage();

	//에니메이션을 저장
	for( int j = 0; j < m_nMaxLinkMapCount; j++ )
	{
		_XTextureAnimation* xTexAni = new _XTextureAnimation( 0, 0, 64, 64, this, &g_MainInterfaceTextureArchive, 
															g_MainInterfaceTextureArchive.FindResource( "worldmap_icon.tga" ),
															8, 1, 8 );
		xTexAni->ShowWindow( FALSE );
		vtTextureAni.push_back( xTexAni );
	}

	return TRUE;
}

BOOL XWindow_WorldMap::LoadFile( FILE* fp )
{
	if( !fp )
		return FALSE;

	//파일의 사이즈
	long length = 0;
	long count = 0;
	//fseek( fp, 0, SEEK_END );
	//length = ftell( fp );
	//fseek( fp, 0, SEEK_SET );

	int nRes = g_ScriptArchive.FindResource( "GameInfo.xml" );
	length = g_ScriptArchive.GetPackedFileSize( nRes ) - 1;

	if ( length <= 0 )
		return FALSE;

	string strData = "";

	char* buf = new char[ length+1 ];
	buf[0] = 0;
	
	//다양한 플랫폼도 가능하게끔 바꾸어야한다
	while( fgets( buf, sizeof(buf), fp ) )
	{
		if( length <= count )
			break;
		strData += buf;
		count = strData.size();
	}

	//다른 플렛폼을 위해..
	/*char* buf = new char[ length+1 ];
	buf[0] = 0;

	if ( fread( buf, length, 1, fp ) != 1 )
	{
		delete [] buf;
		return FALSE;
	}

	const char* lastPos = buf;
	const char* p = buf;

	buf[length] = 0;
	while( *p ) 
	{
		assert( p < (buf+length) );
		if ( *p == 0xa )
		{
			// Newline character. No special rules for this. Append all the characters
			// since the last string, and include the newline.
			data.append( lastPos, (p-lastPos+1) );	// append, include the newline
			++p;									// move past the newline
			lastPos = p;							// and point to the new buffer (may be 0)
			assert( p <= (buf+length) );
		}
		else if ( *p == 0xd )
		{
			// Carriage return. Append what we have so far, then
			// handle moving forward in the buffer.
			if ( (p-lastPos) > 0 ) {
				data.append( lastPos, p-lastPos );	// do not add the CR
			}
			data += (char)0xa;						// a proper newline

			if ( *(p+1) == 0xa )
			{
				// Carriage return - new line sequence
				p += 2;
				lastPos = p;
				assert( p <= (buf+length) );
			}
			else 
			{
				// it was followed by something else...that is presumably characters again.
				++p;
				lastPos = p;
				assert( p <= (buf+length) );
			}
		}
		else
		{
			++p;
		}
	}
	// Handle any left over characters.
	if ( p-lastPos )
	{
		data.append( lastPos, p-lastPos );
	}*/

	delete [] buf;
	buf = 0;

	g_ScriptArchive.ClosePackage();

/*	if( E_FAIL == m_xXML.ND_hrOpenMemory(strData.c_str()) )
	{
		_XLog( "GameInfo.xml 엑셀에서 메모리 로드 실패" );
		return FALSE;
	}*/

	return TRUE;
}

void XWindow_WorldMap::SetButton( const int nID, const MapInfo& MapData )
{
	//이미지의 사이즈
	RECT sizerect = {0,};

	sizerect.left = 64 * MapData.nIconIndex;
	sizerect.top = 64;
	sizerect.bottom = 128;
	sizerect.right = 64 + (64 * MapData.nIconIndex);

	//그림이미지
	int Imageindex = g_MainInterfaceTextureArchive.FindResource( "worldmap_icon.tga" );

	//텍스쳐가 없다
	if( Imageindex < 0)
		return;

	//버튼으로 ..
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, 
		{ MapData.IconPos.x, MapData.IconPos.y }, 
		{ sizerect.right - sizerect.left, sizerect.bottom - sizerect.top },
		nID, Imageindex, Imageindex, Imageindex, &g_MainInterfaceTextureArchive 
	};

	_XButton* pButton = new _XButton;
	pButton->Create( btnstruct );	
	pButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, sizerect.left, sizerect.top, sizerect.right, sizerect.bottom);
	pButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, sizerect.left, sizerect.top + 64, sizerect.right, sizerect.bottom + 64);
	pButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  sizerect.left, sizerect.top + 64, sizerect.right, sizerect.bottom + 64);
	pButton->ShowWindow( MapData.bEnable );

	InsertChildObject( pButton );	
}

BOOL XWindow_WorldMap::SetMiniMapImage(_XPackageArchive& TextureArchive, MapInfo& MapData )
{
	//맵이름 지정
	TCHAR minimapnamestr[_MAX_PATH];
	memset( minimapnamestr, 0, _MAX_PATH );

	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{			
		strcpy( minimapnamestr, "Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy( minimapnamestr, "vn_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		strcpy( minimapnamestr, "us_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy( minimapnamestr, "tw_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		strcpy( minimapnamestr, "jp_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		strcpy( minimapnamestr, "rs_Mini" );
	}
	else
	{
		strcpy( minimapnamestr, "Mini" );
	}

	switch( MapData.nID )
	{
	case 0 : //개방
		strcat( minimapnamestr, "map_gaebang.tga" );
		break;
	case 1 : //소림사
		strcat( minimapnamestr, "map_sorim.tga" );
		break;
	case 2 : //무당
		strcat( minimapnamestr, "map_moodang.tga" );
		break;
	case 3 : //녹림
		strcat( minimapnamestr, "map_noklim.tga" );
		break;
	case 4 : //비궁
		strcat( minimapnamestr, "map_beegoong.tga" );
		break;
	case 5 : //마교
		strcat( minimapnamestr, "map_magyo.tga" );
		break;
	case 6 : //합비
		strcat( minimapnamestr, "map_hapbi.tga" );
		break;
	case 7 : //북경
		strcat( minimapnamestr, ".tga" );
		break;
	case 8 : //정주
		strcat( minimapnamestr, "map_jungju.tga" );
		break;
	case 9 : //석가장
		strcat( minimapnamestr, "map_seokgaja.tga" );
		break;
	case 10 : //제남
		strcat( minimapnamestr, "map_jaenam.tga" );
		break;
	case 11 : //대동
		strcat( minimapnamestr, "map_daedong.tga" );
		break;
	case 12 : //남창
		strcat( minimapnamestr, "map_namchang.tga" );
		break;
	case 13 : //항주
		strcat( minimapnamestr, "map_hangjoo.tga" );
		break;
	case 14 : //요동
		strcat( minimapnamestr, "map_Yodong.tga" );
		break;
	case 15 : //북해빙궁
		strcat( minimapnamestr, "map_BookHae.tga" );
		break;
	case 16 : //혈사평
		strcat( minimapnamestr, "map_hyulsapyung.tga" );
		break;
	case 17 : //패왕동
		strcat( minimapnamestr, "map_Paewangdong.tga" );
		break;
	case 18 : //포달랍궁
		strcat( minimapnamestr, ".tga" );
		break;
	}

	//저장될 메모리
	LPBYTE pImageVFMemory = NULL;

	//월드맵을 찾는다
	int nRes = TextureArchive.FindResource( minimapnamestr );

	if( nRes < 0 )
		return FALSE;

	int   filesize = TextureArchive.GetPackedFileSize(nRes);
	FILE* pFP = TextureArchive.GetPackedFile(nRes);
	
	pImageVFMemory = new BYTE[ filesize ];

	if( !pImageVFMemory )
		return FALSE;
	
	if( fread( pImageVFMemory, filesize, 1, pFP ) < 1 )
		return FALSE;

	if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, pImageVFMemory, filesize, 
		D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		0, NULL, NULL, &MapData.pImageTex) ) )
	{
		return FALSE;
	}
	
	delete[] pImageVFMemory;

	MapData.pMapImage = new _XImageStatic;

	if( !MapData.pMapImage )
		return FALSE;

	MapData.pMapImage->Create( 0, 0, 512, 512, &g_MainInterfaceTextureArchive, -1 );
	MapData.pMapImage->SetClipRect( 0, 0, 512, 512 );
	MapData.pMapImage->SetScale( 0.58f, 0.58f );

	return TRUE;
}

void XWindow_WorldMap::SetStringColor( const char* szString, DWORD& dwColor )
{
	dwColor = 0;
	//각자리의 문자를 숫자로 변환
	int nNum[255] = {0,};
	int nStrlen = strlen(szString);

	for( int i = 0; i < nStrlen; i++ )
	{
		if( szString[i] >= '0' && szString[i] <= '9' )
		{
			nNum[i] = szString[i] - '0';
		}
		else if( szString[i] >= 'a' && szString[i] <= 'f' )
		{
			nNum[i] = szString[i] - 'a' + 10;
		}
		else if( szString[i] >= 'A' && szString[i] <= 'F' )
		{
			nNum[i] = szString[i] - 'A' + 10;
		}
		else
		{
			//16진수를 나타내는 문자가 아닐경우
			nNum[i] = 0;
		}
	}

	//각자리의 숫자를 승수를 곱해서.. 10진수로 만든다
	i = 0;
	int j = nStrlen;
	for( ; i < nStrlen; i++, j-- )
	{
		//16의 j승 // 숫자는 앞에서 부터 대입 
		int multiplier = 1;
		for( int h = j-1; h >= 1; h-- )
			multiplier *= 16;

		dwColor = dwColor + ( nNum[i] * multiplier );
	}
}

void XWindow_WorldMap::DestroyWindow()
{
	_XWindow::DestroyWindow();

	if(m_pCharinfo)
	{
		delete m_pCharinfo;
		m_pCharinfo = NULL;
	}

	map<int, MapInfo*>::iterator iter;

	if(m_mpData.size() > 0)
	{
		for( iter = m_mpData.begin(); iter != m_mpData.end(); ++iter )
		{
			MapInfo* pData = iter->second;
			pData->vtLinkID.clear();
			
			if( pData->pMapImage )
			{
				delete pData->pMapImage;
				pData->pMapImage = NULL;
			}

			if(pData->pImageTex )
			{
				pData->pImageTex->Release();
				pData->pImageTex = NULL;
			}
			
			delete iter->second;
		}
		m_mpData.clear();
	}

	vector<_XTextureAnimation*>::iterator vtTexiter = vtTextureAni.begin();
	for( ; vtTexiter != vtTextureAni.end(); ++vtTexiter )
	{
		delete (*vtTexiter);
	}

	vtTextureAni.clear();

	SAFE_RELEASE( m_pWorldmapTex );
	SAFE_DELETE( m_pWorldMapImage );
}

void XWindow_WorldMap::ShowWindow( BOOL show )
{
	m_nFindKey = -1;
	m_pBoard->ShowWindow(FALSE);
	
#ifdef _ACCLAIM_RUBICONADSYSTEM
		if( (g_pLocalUser->m_AccountItemcType >= 0 && g_pLocalUser->m_AccountItemsID > 0) || 
			g_RubiconADFreeLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
		{
			g_ViewRubiconAD = FALSE;
			::ShowWindow(g_hRubiconADWindow, SW_HIDE);
		}
		else
		{
			g_ViewRubiconAD = TRUE;
			::ShowWindow(g_hRubiconADWindow, SW_SHOW);
		}
#endif
	
	vector<_XTextureAnimation*>::iterator vtTexiter = vtTextureAni.begin();
	for( ; vtTexiter != vtTextureAni.end(); ++vtTexiter  )
	{
		(*vtTexiter)->ShowWindow(FALSE);
	}

	m_pCharinfo->ShowWindow( show );

	if(show)
	{
		SetDrawPostion( m_nFirstPos.x, m_nFirstPos.y );

		switch(g_CurrentZoneInfoIndex)
		{
		case 1 : //소림
			SetCharPostion(1);
			break;
		case 2 : //개방
			SetCharPostion(0);
			break;
		case 3 : //녹림맹
			SetCharPostion(3);
			break;
		case 4 : //비궁
			SetCharPostion(4);
			break;
		case 5 : //무당
			SetCharPostion(2);
			break;
		case 6 : //마교
			SetCharPostion(5);
			break;
		case 7 : //정주
			SetCharPostion(8);
			break;
		case 8 : //석가장
			SetCharPostion(9);
			break;
		case 9 : //제남
			SetCharPostion(10);
			break;
		case 10 : //대동
			SetCharPostion(11);
			break;
		case 11 : //남창
			SetCharPostion(12);
			break;
		case 13 : //항주
			SetCharPostion(13);
			break;
		case 16 : //요동
			SetCharPostion(14);
			break;
		case 17 : //합비
			SetCharPostion(6);
			break;
		case 20 : //혈사평
			SetCharPostion(16);
			break;
		case 21 : //패왕동
			SetCharPostion(17);
			break;
		case 23 : //북해빙궁
			SetCharPostion(15);
			break;
		default :
			m_pCharinfo->ShowWindow(FALSE);
		}
	}

	_XWindow::ShowWindow( show );
}

void XWindow_WorldMap::SetCharPostion(int nKey )
{
	map<int, MapInfo*>::iterator iter;
	
	// 인덱스번호로 찾는다
	iter = m_mpData.find( nKey );

	// 없으면 리턴
	if( iter == m_mpData.end() )
	{
		m_pCharinfo->ShowWindow(FALSE);
		return;
	}

	POINT pos = iter->second->IconPos;
	m_pCharinfo->SetDrawPostion( m_nPos.x + pos.x, m_nPos.y + (pos.y - 32) );
}

void XWindow_WorldMap::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );
}

void XWindow_WorldMap::Draw( _XGUIObject*& pfocusobject )
{
	//그리는 순서
	if( !this->m_ShowWindow ) return;

	m_pWorldMapImage->Draw( m_nPos.x, m_nPos.y, m_pWorldmapTex );

	if(SetAlphaProcess(m_bAlphaProcess))
		return;

	m_pCharinfo->Update(0.1f);

	_XWindow::Draw(pfocusobject);

	if( m_nFindKey == -1 )
		return;

	map<int, MapInfo*>::iterator iter;
	iter = m_mpData.find( m_nFindKey );
	if( iter == m_mpData.end() )
		return;

	if( !(iter->second->bEnable) )
		return;

	_XDrawSolidBar( m_nPos.x + 30, m_nPos.y + 68, 
					m_nPos.x + 30 + 320, m_nPos.y + 68 + 550, 0x60000000 );
	//_XDrawRectAngle( m_WindowPosition.x + 30, m_WindowPosition.y + 118, m_WindowPosition.x + 30 + 320, m_WindowPosition.y + 118 + 388, 1.0f, 0xFF000000 );

	g_XLargeFont->SetColor( _XSC_DEFAULT_WHITE );
	g_XLargeFont->PutsAlign(m_nPos.x + 180, m_nPos.y + 60, 
							_XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(iter->second->nName));
	g_XLargeFont->Flush();

	if(iter->second->pImageTex)
		iter->second->pMapImage->Draw( m_nPos.x + 37, m_nPos.y + 90, iter->second->pImageTex );

	g_XBaseFont->SetColor( iter->second->dwMapInfoColor_1 );
	g_XBaseFont->Puts_SeparateAlign( m_nPos.x + 40, m_nPos.y + 400, 
									_XGETINTERFACETEXT(iter->second->nMapInfo_1), 305, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

	g_XBaseFont->SetColor( iter->second->dwMapInfoColor_2 );
	g_XBaseFont->Puts_SeparateAlign( m_nPos.x + 40, m_nPos.y + 520, 
									_XGETINTERFACETEXT(iter->second->nMapInfo_2), 250, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );

	g_XBaseFont->Flush();

	//링크맵 에니메이션 텍스쳐 만
	vector<_XTextureAnimation*>::iterator vtTexiter = vtTextureAni.begin();
	for( ; vtTexiter != vtTextureAni.end(); ++vtTexiter )
	{
		(*vtTexiter)->Update(0.1f);
	}
}

BOOL XWindow_WorldMap::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	if( m_nFindKey == -1 )
		return FALSE;

	map<int, MapInfo*>::iterator iter;
	iter = m_mpData.find( m_nFindKey );
	if( iter == m_mpData.end() )
		return FALSE;

	return TRUE;
}

void XWindow_WorldMap::SetKey( const int nKey )
{
	map<int, MapInfo*>::iterator iter;
	
	// 인덱스번호로 찾는다
	iter = m_mpData.find( nKey );

	// 없으면 리턴
	if( iter == m_mpData.end() )
		return;

	//저장된 링크 아이디의 지정된 포지션을 찾아서 에니메이션의 위치를 바꿔준다 
	vector<_XTextureAnimation*>::iterator vtTexiter = vtTextureAni.begin();
	vector<int>::iterator vtiter = iter->second->vtLinkID.begin();

	for( ; vtTexiter != vtTextureAni.end(); ++vtTexiter  )
	{
		map<int, MapInfo*>::iterator tempiter = m_mpData.find( (*vtiter) );

		if( tempiter == m_mpData.end() )
		{
			//해당 맵정보가 존재 하지 않을때
			(*vtTexiter)->ShowWindow(FALSE); 
			if( vtiter != iter->second->vtLinkID.end() )
				++vtiter;

			continue;
		}

		if( vtiter != iter->second->vtLinkID.end() )
			++vtiter;
		else
		{
			(*vtTexiter)->ShowWindow(FALSE);
			continue;
		}

		POINT pos = tempiter->second->IconPos;
		(*vtTexiter)->SetDrawPostion( m_nPos.x + pos.x, m_nPos.y + pos.y );
		(*vtTexiter)->ShowWindow(TRUE);
	}

	m_nFindKey = nKey;
	m_pBoard->ShowWindow(TRUE);
}

BOOL XWindow_WorldMap::SetAlphaProcess(BOOL bShow)
{
	if( bShow )
	{
		int nAlpha = m_pWorldMapImage->m_FColor >> 24;

		nAlpha += 15;

		if( nAlpha >= 255 )
		{
			m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
			return FALSE;
		}
		else
			m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );		
	}
	else
	{
		int nAlpha = m_pWorldMapImage->m_FColor >> 24;

		nAlpha -= 15;

		if( nAlpha <= 0 )
		{
			m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 0, 255, 255, 255 );
			ShowWindow(FALSE);
			return FALSE;
		}
		else
			m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( nAlpha, 255, 255, 255 );
	}

	return TRUE;
}