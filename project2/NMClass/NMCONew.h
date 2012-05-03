#ifdef _XDEF_NEXONLOGIN

#ifndef __NMCONEW_H_7E9B922A_A6D1_4865_93DD_69B59E27CF1C__
#define __NMCONEW_H_7E9B922A_A6D1_4865_93DD_69B59E27CF1C__

#ifdef _DLL_EXPORT
#define DllDecl	__declspec( dllexport )
#else
#define DllDecl	__declspec( dllimport )
#endif

extern "C"
{
	/*
	uFuncCode		: 호출하는 CNMFunc의 코드
	pCallingData	: Serialize된 호출 데이터
	ppReturnData	: 결과를 돌려받을 버퍼의 포인터, 버퍼가 불충분한 경우 새로할당해서 리턴됨
	uReturnDataLen	: 호출시 결과 버퍼의 사이즈
					리턴시 결과값 리턴에 사용한 버퍼의사이즈
	참고			: 리턴 버퍼의 크기가 실제 리턴 데이터에 비하여 작은 경우
					새로 메모리를 할당해 결과를 리턴한다. 이때 ppReturnData
					값이 처음 호출시와 달라지게 되면, 이렇게 새로 할당되어
					넘어간 메모리는 NMCO_MemoryFree를 이용해 해제해 주어야 한다.
	*/
	BOOL DllDecl NMCO_CallNMFunc(	IN		UINT32	uFuncCode,
									IN		BYTE*	pCallingData,
									IN OUT	BYTE**	ppReturnData,
									IN OUT	UINT32&	uReturnDataLen
									);

	void DllDecl NMCO_MemoryFree( IN LPVOID pBuffer );

	//
	//	모듈 패치 여부를 결정합니다
	//	(기본적으로는 패치를 합니다)
	//
	BOOL DllDecl NMCO_SetPatchOption( IN BOOL bPatch );

	//
	//	친구 서버 사용 여부를 결정합니다
	//	(기본적으로는 켜져 있습니다)
	//
	BOOL DllDecl NMCO_SetUseFriendModuleOption( IN BOOL bUserFriendModule );

	//
	//	패치 할 경우, 버전 파일을 지정합니다
	//	(기본적으로는 한국 버전이 지정됩니다)
	//
	BOOL DllDecl NMCO_SetVersionFileUrl( IN LPCTSTR pszVersionUrl );

	//
	//	NGM 을 이용해서 패치 합니다
	//	(기본적으로는 꺼져 있습니다)
	//
	BOOL DllDecl NMCO_SetUseNGMOption( IN BOOL bUseNGM );

	//
	//	메신저가 접속할 국가와 지역 코드를 설정합니다
	//
	//		-	기본적으로는 한국으로 접속하도록 설정되어 있습니다
	//
	BOOL DllDecl NMCO_SetLocale( IN UINT32 uLocaleID, IN UINT32 uRegionCode );
}

#endif	//	#ifndef __NMCONEW_H_7E9B922A_A6D1_4865_93DD_69B59E27CF1C__

#endif