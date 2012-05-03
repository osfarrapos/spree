#pragma once
// #pragma pack(1)
/*enum
{
	en_charac_name_length=12,
};*/

//위치추적의 기본 정보
struct _sLTS_Data
{
	int			iStartTrackingTime;			// 위치추적신청시간	
	u_short     usReserve;
	u_char		ucTimeMode;					// 의뢰한 위치추적 내용 ( 1) 24시간, 2) 48시간 ) 
	char		szCharacName[en_charac_name_length+1];
	_sLTS_Data()
		: iStartTrackingTime(0), ucTimeMode(0)
	{}
};


//위치 정보 (USER_GS : x,z 좌표가 필요)
struct _sCL_LTS_Data
{
	u_char		ucPkOrSpMode;	//1.Pk 2. 특정인
	char		szCharacName[en_charac_name_length+1];

	short sPosX;
	short sPosZ;
	
	_sCL_LTS_Data()
		:ucPkOrSpMode(0),sPosX(0),sPosZ(0)
	{}
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define MSG_NO_LTS						22		// 위치 추적.

enum EN_LTS_DATA_COUNT
{
	en_lts_lvpk_data_count = 30,
	en_lts_pk_data_count = 10,
	en_lts_sp_data_count = 1,
	en_lts_pos_info_count = 11,
};


/////////////////
//기본
//위치추적 기본 패킷
struct MSG_LTS
{
	_sHeader				header;
	u_char					ucMessage;

    u_char					ucPktMode;

//	MSG_LTS(u_short length, u_char mode)
//		: header(length),ucMessage(MSG_NO_LTS),ucPktMode(mode)
//	{}
};

enum EN_LTS_CL_GS
{
	// 레벨차가 큰 PK 범
	en_lts_lvpk_info_gs_cl=0,			//레벨차 모든정보
	en_lts_lvpk_insert_gs_cl,	
	en_lts_lvpk_del_req_cl_gs,	
	en_lts_lvpk_del_req_gs_cl,
	



	// PK 범 추적
	en_lts_pk_info_gs_cl=10,			//PK범 모든정보
	en_lts_pk_insert_req_gs_cl,			// pk범 이름을 알려준다 - 죽었을 때 서버에서 알려줌
	en_lts_pk_insert_req_cl_gs,
	en_lts_pk_insert_success_gs_cl,
	en_lts_pk_insert_failed_gs_cl,
	en_lts_pk_del_success_gs_cl,

	// 특정인 추적
	en_lts_sp_info_gs_cl=20,			//특정인 모든정보
	en_lts_sp_insert_req_cl_gs,
	en_lts_sp_insert_success_gs_cl,
	en_lts_sp_insert_failed_gs_cl,
	en_lts_sp_del_success_gs_cl,
		
	en_lts_pos_info_gs_cl =30, //3초마다 PK범 특정인 (캐릭명 + 위치정보)
};


// 레벨차가 큰 PK 범
// en_lts_lvpk_info_gs_cl=0,
struct MSG_LTS_LVPK_INFO_GS_CL : public MSG_LTS
{
	u_char ucCount; //레벨차 갯수
	_sLTS_Data LvPkData[en_lts_lvpk_data_count];
	
//	MSG_LTS_LVPK_INFO_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_LVPK_INFO_GS_CL), en_lts_lvpk_info_gs_cl)
//	{}

	void set_length()
	{
		if( ucCount > en_lts_lvpk_data_count )
			ucCount = en_lts_lvpk_data_count;

		header.sLength = sizeof(MSG_LTS_LVPK_INFO_GS_CL) - ( en_lts_lvpk_data_count - ucCount ) * sizeof(_sLTS_Data);
	}
};

//	en_lts_lvpk_insert_gs_cl,	
struct MSG_LTS_LVPK_INSERT_GS_CL : public MSG_LTS
{
	_sLTS_Data	PkData;

//	MSG_LTS_LVPK_INSERT_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_LVPK_INSERT_GS_CL), en_lts_lvpk_insert_gs_cl)
//	{}
};

//	en_lts_lvpk_del_req_cl_gs,
struct MSG_LTS_LVPK_DEL_REQ_CL_GS : public MSG_LTS
{
	char szDelCharacName[en_charac_name_length+1];
	
//	MSG_LTS_LVPK_DEL_REQ_CL_GS()
//		: MSG_LTS(sizeof(MSG_LTS_LVPK_DEL_REQ_CL_GS), en_lts_lvpk_del_req_cl_gs)
//	{}
};

//en_lts_lvpk_del_req_gs_cl
struct MSG_LTS_LVPK_DEL_REQ_GS_CL : public MSG_LTS
{
	char szDelCharacName[en_charac_name_length+1];
//	MSG_LTS_LVPK_DEL_REQ_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_LVPK_DEL_REQ_GS_CL), en_lts_lvpk_del_req_gs_cl)
//	{}
};



// PK 범 추적
//	en_lts_pk_info_gs_cl=10,
struct MSG_LTS_PK_INFO_GS_CL : public MSG_LTS
{
	u_char			ucCount;//갯수
	_sLTS_Data		PkData[en_lts_pk_data_count];
	
//	MSG_LTS_PK_INFO_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_PK_INFO_GS_CL), en_lts_pk_info_gs_cl)
//	{}

	void set_length()
	{
		if( ucCount > en_lts_pk_data_count )
			ucCount = en_lts_pk_data_count;

		header.sLength = sizeof(MSG_LTS_PK_INFO_GS_CL) - ( en_lts_pk_data_count - ucCount ) * sizeof(_sLTS_Data);
	}
};

// 2007_03_30 추가 - 죽었을 때 누가 죽였는지 서버에서 알려줌 / 의뢰할 때 이 이름으로 보낸다
struct MSG_LTS_PK_INSERT_REQ_GS_CL : public MSG_LTS
{
	char szInsertCharacName[en_charac_name_length+1];
};

	
//2. Pk범 위치추적 en_lts_pk_insert_req_cl_gs,
struct MSG_LTS_PK_INSERT_REQ_CL_GS : public MSG_LTS
{
	char szInsertCharacName[en_charac_name_length+1];
	
	u_char		ucTimeMode;		//1)24시간, 2)48시간
	
//	MSG_LTS_PK_INSERT_REQ_CL_GS()
//		:MSG_LTS(sizeof(MSG_LTS_PK_INSERT_REQ_CL_GS), en_lts_pk_insert_req_cl_gs)
//	{}
};

//	en_lts_pk_insert_success_gs_cl,
struct MSG_LTS_PK_INSERT_SUCCESS_GS_CL : public MSG_LTS
{
	_sLTS_Data		PkData;
	u_int			uiRemainMoney;//사용하고 남은돈.

	
//	MSG_LTS_PK_INSERT_SUCCESS_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_PK_INSERT_SUCCESS_GS_CL), en_lts_pk_insert_success_gs_cl)
//	{}
};

//	en_lts_pk_insert_failed_gs_cl,
struct MSG_LTS_PK_INSERT_FAILED_GS_CL : public MSG_LTS
{
	u_char ucRes;
	
//	MSG_LTS_PK_INSERT_FAILED_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_PK_INSERT_FAILED_GS_CL), en_lts_pk_insert_failed_gs_cl)
//	{}
};

//	en_lts_pk_del_success_gs_cl,
//2. pk범 위치추적
struct MSG_LTS_PK_DEL_SUCCESS_GS_CL : public MSG_LTS
{
	char		szDelCharacName[en_charac_name_length+1];
	
//	MSG_LTS_PK_DEL_SUCCESS_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_PK_DEL_SUCCESS_GS_CL), en_lts_pk_del_success_gs_cl)
//	{}
};


//	en_lts_sp_info_gs_cl=20,
struct MSG_LTS_SP_INFO_GS_CL : public MSG_LTS
{
	
	u_char     ucCount;//갯수
	_sLTS_Data SpData[en_lts_sp_data_count];
	
//	MSG_LTS_SP_INFO_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_SP_INFO_GS_CL), en_lts_sp_info_gs_cl)
//	{}
};


//	en_lts_sp_insert_req_cl_gs,
//3. 특정인
struct MSG_LTS_SP_INSERT_REQ_CL_GS : public MSG_LTS
{
	char szInsertCharacName[en_charac_name_length+1];
	
	u_char ucTimeMode;
	
//	MSG_LTS_SP_INSERT_REQ_CL_GS()
//		: MSG_LTS(sizeof(MSG_LTS_SP_INSERT_REQ_CL_GS), en_lts_sp_insert_req_cl_gs)
//	{}
};

	
//	en_lts_sp_insert_success_gs_cl,
struct MSG_LTS_SP_INSERT_SUCCESS_GS_CL : public MSG_LTS
{
	_sLTS_Data SpData;
	u_int			uiRemainMoney;//사용하고 남은돈.
	
//	MSG_LTS_SP_INSERT_SUCCESS_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_SP_INSERT_SUCCESS_GS_CL), en_lts_sp_insert_success_gs_cl)
//	{}
};


//	en_lts_sp_insert_failed_gs_cl,
struct MSG_LTS_SP_INSERT_FAILED_GS_CL : public MSG_LTS
{
	u_char	ucRes;
	
//	MSG_LTS_SP_INSERT_FAILED_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_SP_INSERT_FAILED_GS_CL), en_lts_sp_insert_failed_gs_cl)
//	{}
};

//	en_lts_sp_del_success_gs_cl,
//3. 특정인 위치추적
struct MSG_LTS_SP_DEL_SUCCESS_GS_CL : public MSG_LTS
{
	char	szDelCharacName[en_charac_name_length+1];
	
//	MSG_LTS_SP_DEL_SUCCESS_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_SP_DEL_SUCCESS_GS_CL), en_lts_sp_del_success_gs_cl)
//	{}
};


//	en_lts_pos_info_gs_cl,
struct MSG_LTS_POS_INFO_GS_CL : public MSG_LTS
{
	u_char			ucPkCount;//Pk 범 갯수
	_sCL_LTS_Data	LtsPosData[en_lts_pk_data_count +  en_lts_sp_data_count];

//	MSG_LTS_POS_INFO_GS_CL()
//		: MSG_LTS(sizeof(MSG_LTS_POS_INFO_GS_CL), en_lts_pos_info_gs_cl)
//	{}

	void set_length()
	{
		if( ucPkCount > en_lts_pk_data_count + en_lts_sp_data_count)
			ucPkCount = en_lts_pk_data_count + en_lts_sp_data_count;

		header.sLength = sizeof(MSG_LTS_POS_INFO_GS_CL) - ( en_lts_pk_data_count + en_lts_sp_data_count - ucPkCount ) * sizeof(_sCL_LTS_Data);

	}
};
