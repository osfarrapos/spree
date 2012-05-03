#pragma once

#include "..\CS\Organization.h"
#include "../MyLibrary/FileLog.h"

#pragma warning(disable:4267)

/*
class CAccerrorBaseInterface
{
	~virtual void ResetAccessor() = 0 ;
};
*/

/*
#define _PITEM_MULTI_YEAR_		(100000000)
#define _PITEM_MULTI_MONTH_		(1000000)
#define _PITEM_MULTI_DAY_		(10000)
#define _PITEM_MULTI_HOUR_		(100)

inline int NDF_DBTIME_TO_INT(
*/

class C_prDh_GetCharacStateAccessor
{
public:	

	LONG m_RETURN_VALUE;
	LONG m_UniqueID;	
	_CHARAC_STATE m_data ;
	LONG m_result;

/*
	CREATE PROCEDURE [_prDS_GetCharacState]
		@UniqueID 	int,			--유니크아이디	-- 1
		@vexp		smallint	OUTPUT, 			-- 2
		@jin		bigint	OUTPUT,
		@gong		bigint	OUTPUT,
		@vretribution	smallint	OUTPUT,  			-- 5
		@vhonor	int	OUTPUT,  			-- 6
		@fatigue	int	OUTPUT, 			-- 7
		@wound_value smallint  OUTPUT,
		@inside_value	smallint	OUTPUT,
		@vshowdown	smallint	OUTPUT, 			-- 10
		@fury		smallint	OUTPUT,
		@levelup_time	int	output,
		@or_index	int	output,
		@peace_mode smallint 	output,
		@mute_time	int	output,
		@honor_gave_time	int	output,
		@honor_take_time	int	output,
		@hiding			int	output,
		@blocking_end_date	int	output,
		@pk_prevdead_mode	smallint	output,
		@pk_dead_count	smallint	output,
		@pk_kill_count		smallint 	output,
		@result		int	OUTPUT	--반환값	--

		AS 
*/
	DEFINE_COMMAND_EX(C_prDh_GetCharacStateAccessor,
		L"{ ? = CALL dbo._prDS_GetCharacState(?,?,?,?,?,?,?,?,?,?,\
			?,?,?,?,?,?,?,?,?,?,\
			?,?,?,? ) }")

	BEGIN_PARAM_MAP(C_prDh_GetCharacStateAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_UniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_data.sInnerLevel)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_data.uiJin)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_data.iGong)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_data.sRetribution)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(7, m_data.iHonor)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(8, m_data.usFatigue)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(9, m_data.sWoundValue)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(10, m_data.sInsideWoundValue)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(11, m_data.sShowdown)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(12, m_data.sFuryParameter)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(13, m_data.sLevelUpGameSecond)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(14, m_data.iORIndex)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(15, m_data.sPeaceMode )		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(16, m_data.iMuteTime)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(17, m_data.iHonorGaveDate)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(18, m_data.iHonorTakeDate)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(19, m_data.iHiding)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(20, m_data.iBlockingEndDate)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(21, m_data.sPkPrevDeadMode)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(22, m_data.sPkDeadCount)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(23, m_data.sPkKillCount)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(24, m_data.sMonsterKill)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(25, m_result)
	END_PARAM_MAP()		
};


#ifdef _PD_LEVEL_RATE_SAVE_
class C_prDh_GetCharacState_v2Accessor
{
public:	

	LONG m_RETURN_VALUE;
	LONG m_UniqueID;	
	_CHARAC_STATE m_data ;
	LONG m_result;

	/*
	CREATE PROCEDURE [_prDS_GetCharacState]
	@UniqueID 	int,			--유니크아이디	-- 1
	@vexp		smallint	OUTPUT, 			-- 2
	@jin		bigint	OUTPUT,
	@gong		bigint	OUTPUT,
	@vretribution	smallint	OUTPUT,  			-- 5
	@vhonor	int	OUTPUT,  			-- 6
	@fatigue	int	OUTPUT, 			-- 7
	@wound_value smallint  OUTPUT,
	@inside_value	smallint	OUTPUT,
	@vshowdown	smallint	OUTPUT, 			-- 10
	@fury		smallint	OUTPUT,
	@levelup_time	int	output,
	@or_index	int	output,
	@peace_mode smallint 	output,
	@mute_time	int	output,
	@honor_gave_time	int	output,
	@honor_take_time	int	output,
	@hiding			int	output,
	@blocking_end_date	int	output,
	@pk_prevdead_mode	smallint	output,
	@pk_dead_count	smallint	output,
	@pk_kill_count		smallint 	output,
	@result		int	OUTPUT	--반환값	--

	AS 
	*/
	DEFINE_COMMAND_EX(C_prDh_GetCharacState_v2Accessor,
		L"{ ? = CALL dbo._prDS_GetCharacState_v2(?,?,?,?,?,?,?,?,?,?,\
		 ?,?,?,?,?,?,?,?,?,?,\
		 ?,?,?,?,?,? ) }")

		 BEGIN_PARAM_MAP(C_prDh_GetCharacState_v2Accessor)
			 SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(1, m_RETURN_VALUE)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(2, m_UniqueID)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(3, m_data.sInnerLevel)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(4, m_data.uiJin)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(5, m_data.iGong)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(6, m_data.sRetribution)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(7, m_data.iHonor)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(8, m_data.usFatigue)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(9, m_data.sWoundValue)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(10, m_data.sInsideWoundValue)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(11, m_data.sShowdown)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(12, m_data.sFuryParameter)		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(13, m_data.sLevelUpGameSecond)		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(14, m_data.iORIndex)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(15, m_data.sPeaceMode )		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(16, m_data.iMuteTime)		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(17, m_data.iHonorGaveDate)		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(18, m_data.iHonorTakeDate)		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(19, m_data.iHiding)		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(20, m_data.iBlockingEndDate)		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(21, m_data.sPkPrevDeadMode)		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(22, m_data.sPkDeadCount)		
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(23, m_data.sPkKillCount)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(24, m_data.sMonsterKill)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(25, m_data.sLevelModifyFlag)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(26, m_data.dLevelRate)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(27, m_result)
		 END_PARAM_MAP()		
};
#endif

class C_prDS_All_LogoutAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_pwd;


	DEFINE_COMMAND_EX(C_prDS_All_LogoutAccessor, L"{ ? = CALL dbo._prDS_Alllogout(?) }")


		BEGIN_PARAM_MAP(C_prDS_All_LogoutAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_pwd)
		END_PARAM_MAP()
};

class C_prDS_CharacRenameAccessor
{
public:

	LONG m_RETURN_VALUE;
	CHAR m_cCurName[13];
	CHAR m_cNewName[13] ;	

	

	DEFINE_COMMAND_EX(C_prDS_CharacRenameAccessor, L"{ ? = CALL dbo._prDS_CharacRename (?,?) }")


		BEGIN_PARAM_MAP(C_prDS_CharacRenameAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_cCurName)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_cNewName)			
		END_PARAM_MAP()
};

class C_prDS_CountCharacAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_acc_uid;
	LONG m_nCharac;
	LONG m_UniID1;
	LONG m_UniID2;
	LONG m_UniID3;

	
	DEFINE_COMMAND_EX(C_prDS_CountCharacAccessor, L"{ ? = CALL dbo._prDS_CountCharac(?,?,?,?,?) }")


	BEGIN_PARAM_MAP(C_prDS_CountCharacAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_acc_uid)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_nCharac)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_UniID1)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_UniID2)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_UniID3)
	END_PARAM_MAP()
};

class C_prDS_CreateCharacAccessor
{
public:




	LONG m_RETURN_VALUE;
	CHAR m_acc[en_max_lil+1];
	CHAR m_name[13];	
	SHORT m_vsex;

	// level
	LONG m_Constitution;				//건강  36 <= 생기
	LONG m_Zen;							//진기  40 <= 진기
	LONG m_Intelligence;				//지혜  44 <= 영기
	LONG m_Dexterity;					//민첩  48 <= 심기
	LONG m_Str;						//근력  52 <= 정기

	SHORT m_vhair;
	SHORT m_vface;
	SHORT m_vluck;
	SHORT m_vamor;
	SHORT m_vpants;

	// cur_basic
	SHORT m_zone;
	float m_x;
	float m_z;

	// level
	SHORT m_sLifePower;				//현재 체력  14
	SHORT m_sForcePower;			//현재 내력  16
	SHORT m_sConcentrationPower;	//현재 영력  18
	SHORT m_vmaxheart;
	SHORT m_vmaxstamina;
	LONG m_acc_id;
	LONG m_vunique_id;
	LONG m_result;



	DEFINE_COMMAND_EX(C_prDS_CreateCharacAccessor, L"{ ? = CALL dbo._prDS_CreateCharac(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_CreateCharacAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_acc)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_name)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_vsex)

			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_Constitution)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6, m_Zen)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7, m_Intelligence)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(8, m_Dexterity)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(9, m_Str)

			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(10, m_vhair)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(11, m_vface)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(12, m_vluck)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(13, m_vamor)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(14, m_vpants)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(15, m_zone)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(16, m_x)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(17, m_z)

			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(18, m_sLifePower)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(19, m_sForcePower)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(20, m_sConcentrationPower)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(21, m_vmaxheart)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(22, m_vmaxstamina)

			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(23, m_acc_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(24, m_vunique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(25, m_result)
		END_PARAM_MAP()
};

class C_prDS_DeleteAccessor
{
public:




	LONG m_RETURN_VALUE;	
	LONG m_uid;
	SHORT m_result;



	DEFINE_COMMAND_EX(C_prDS_DeleteAccessor, L"{ ? = CALL dbo._prDS_CharacDelete(?,?) }")


		BEGIN_PARAM_MAP(C_prDS_DeleteAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)		
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_uid)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_result)
		END_PARAM_MAP()
};

class C_prDS_GetEffectAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	_CHARAC_EFFECT m_data ;
	INT	 m_result ;

	
	DEFINE_COMMAND_EX(C_prDS_GetEffectAccessor, L"{ ? = CALL dbo._prDS_GetEffect(?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_GetEffectAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_data.byData )
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_result )

		END_PARAM_MAP()

};

class C_prDS_SetEffectAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	_CHARAC_EFFECT m_data ;


	DEFINE_COMMAND_EX(C_prDS_SetEffectAccessor, L"{ ? = CALL dbo._prDS_SetEffect(?,?) }")


		BEGIN_PARAM_MAP(C_prDS_SetEffectAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_data.byData )			
		END_PARAM_MAP()
};

#ifdef _PD_BUFF_COUNT_MODIFY_
class C_prDS_GetEffect_V2Accessor
{
public:

	LONG m_unique_id;
	_CHARAC_EFFECT m_data ;
	INT	 m_result ;


	DEFINE_COMMAND_EX(C_prDS_GetEffect_V2Accessor, L"{ CALL dbo._prDS_GetEffect_V2(?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_GetEffect_V2Accessor)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(2, m_data.byData )
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_data.bySpecial )
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_result )

		END_PARAM_MAP()
};

class C_prDS_SetEffect_V2Accessor
{
public:

	LONG m_unique_id;
	_CHARAC_EFFECT m_data ;


	DEFINE_COMMAND_EX(C_prDS_SetEffect_V2Accessor, L"{ CALL dbo._prDS_SetEffect_V2(?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_SetEffect_V2Accessor)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_data.byData )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_data.bySpecial )			
		END_PARAM_MAP()
};

#endif



class C_prDS_CheckAndChangeNameAccessor
{
public:
	char			curName[13] ;
	char			newName[13] ;
	int				result ;

	DEFINE_COMMAND_EX(C_prDS_CheckAndChangeNameAccessor, L"{ CALL dbo._prDSU_CheckAndChangeName(?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_CheckAndChangeNameAccessor)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, curName)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, newName)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, result )			
		END_PARAM_MAP()
} ;


class C_prDS_GetPKRecordAccessor
{
public:
	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	_CHARAC_PK_RECORD m_data ;


	DEFINE_COMMAND_EX(C_prDS_GetPKRecordAccessor, L"{ ? = CALL dbo._prDS_GetPkRecord(?,?,?,?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_GetPKRecordAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_data.iKillCount )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_data.iDeadCount )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_data.bWinRecord )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_data.bLoseRecord )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(7, m_data.bAbusingRecord )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(8, m_data.bMoonpaCount )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(9, m_data.bClassCount )			
		END_PARAM_MAP()
};

class C_prDS_UpdatePKRecordAccessor
{
public:
	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	_CHARAC_PK_RECORD m_data ;


	DEFINE_COMMAND_EX(C_prDS_UpdatePKRecordAccessor, L"{ ? = CALL dbo._prDS_UpdatePkRecord(?,?,?,?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_UpdatePKRecordAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_data.iKillCount )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_data.iDeadCount )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_data.bWinRecord )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6, m_data.bLoseRecord )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7, m_data.bAbusingRecord )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(8, m_data.bMoonpaCount )			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(9, m_data.bClassCount )			
		END_PARAM_MAP()
};


class C_pr_prDS_GetCharacLevelAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_UniqueID;
	
	_CHARAC_LEVEL m_data ;

	LONG m_result;


	DEFINE_COMMAND_EX(C_pr_prDS_GetCharacLevelAccessor, L"{ ? = CALL dbo._prDS_GetCharacLevel(?,?,?,?,?,?,?,?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_pr_prDS_GetCharacLevelAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_UniqueID)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_data.sConstitution)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_data.sZen)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_data.sIntelligence)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_data.sDexterity)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(7, m_data.sStr)				
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)			
			COLUMN_ENTRY(8, m_data.sMaxLifePower)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(9, m_data.sMaxForcePower)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(10, m_data.sMaxConcentrationPower)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)			
			COLUMN_ENTRY(11, m_data.sLeftPoint)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(12, m_data.sMasteryPoint)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(13, m_result)
		END_PARAM_MAP()
};

class C_prDs_GetAccAccessor 
{
public:


	LONG m_RETURN_VALUE;
	CHAR m_cCharacName[13];
	CHAR m_cAccName[en_max_lil+1] ;	


	DEFINE_COMMAND_EX(C_prDs_GetAccAccessor, L"{ ? = CALL dbo._prDS_GetAcc(?,?) }")


		BEGIN_PARAM_MAP(C_prDs_GetAccAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_cCharacName)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_cAccName)		
		END_PARAM_MAP()
};

class C_prDS_GetCharacInfoAccessor
{
public:

	// basic
	LONG m_RETURN_VALUE;
	LONG m_UniID;
	CHAR m_name[13];
	SHORT m_vsex;
	SHORT m_vparty;
	CHAR m_vgamete[13];
	SHORT m_vhair;
	SHORT m_vface;
	SHORT m_vluck;	

	// state
	SHORT m_vexp;
	UINT m_jin;
	UINT m_gong;
	SHORT m_vretribution;
	INT m_honor;
	LONG m_fatigue;
	SHORT m_wound_value ;
	SHORT m_inside_value ;
	SHORT m_showdown;
	SHORT m_sFury ;	

	// level
	SHORT m_sConstitution ;
	SHORT m_sZen;
	SHORT m_sIntelligence;
	SHORT m_sDexterity;
	SHORT m_sStr;
	SHORT m_sMaxLifePower;
	SHORT m_sMaxForcePower;
	SHORT m_sMaxConcentrationPower;
	SHORT m_max_heart;	// delete
	SHORT m_max_stamina;// delete
	SHORT m_left_point;
	SHORT m_master_point ;

	// curbasic
	SHORT m_zone;
	float m_x;
	SHORT m_y;
	float m_z;
	SHORT m_sLifePower;
	SHORT m_sForcePower;
	SHORT m_sConcentrationPower;
	SHORT m_heart_power;
	SHORT m_stamina_power;
	
	SHORT m_respawnServerNo ;
	char  m_respawnPosName[13] ;
	float m_respawnPosx ;
	float m_respawnPosz ;


	// basic
	SHORT m_class ;				// 역할 : 기본값 0
	SHORT m_classGrade ;			// 역할 등급 : 기본값 0		: 2004/11/10 추가. Ray
	INT	  m_iContribution ;		// 문파 기여도 : 기본값 0	: 2004/11/10 추가. Ray

	SHORT  m_gm ;				// gm 여부..
	INT   m_levelup_time ;		// levelup 한 시간..
	DWORD	  m_dwPlay_time ;

	SHORT	m_ChangeName ;


	SHORT m_result;

#ifdef CHARAC_CREATE_DATE_20070309
	DBTIMESTAMP			m_CharacCreateDate ;	//캐릭터생성날짜

	DEFINE_COMMAND_EX(C_prDS_GetCharacInfoAccessor, L"{ ? = CALL dbo._prDS_GetCharacInfo(?,?,?,?,?,?,?,?,?,?,\
													 ?,?,?,?,?,?,?,?,?,?,\
													 ?,?,?,?,?,?,?,?,?,?,\
													 ?,?,?,?,?,?,?,?,?,?,\
													 ?,?,?,?,?,?,?,?,?,?,?,?) }")
#else

	DEFINE_COMMAND_EX(C_prDS_GetCharacInfoAccessor, L"{ ? = CALL dbo._prDS_GetCharacInfo(?,?,?,?,?,?,?,?,?,?,\
													 ?,?,?,?,?,?,?,?,?,?,\
													 ?,?,?,?,?,?,?,?,?,?,\
													 ?,?,?,?,?,?,?,?,?,?,\
													 ?,?,?,?,?,?,?,?,?,?,?) }")

#endif

	BEGIN_PARAM_MAP(C_prDS_GetCharacInfoAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)

		// unique id
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_UniID)
		
		// charac
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_name)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_vsex)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_vparty)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_vgamete)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(7, m_vhair)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(8, m_vface)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(9, m_vluck)

		// state
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(10, m_vexp)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(11, m_jin)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(12, m_gong)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(13, m_vretribution)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(14, m_honor)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(15, m_fatigue)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(16, m_wound_value)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(17, m_inside_value)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(18, m_showdown)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(19, m_sFury)

		// level
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(20, m_sConstitution)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(21, m_sZen)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(22, m_sIntelligence)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(23, m_sDexterity)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(24, m_sStr)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(25, m_sMaxLifePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(26, m_sMaxForcePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(27, m_sMaxConcentrationPower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(28, m_max_heart)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(29, m_max_stamina)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(30, m_left_point)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(31, m_master_point)

		// curbasic
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(32, m_zone)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(33, m_x)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(34, m_y)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(35, m_z)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(36, m_sLifePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(37, m_sForcePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(38, m_sConcentrationPower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(39, m_heart_power)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(40, m_stamina_power)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(41, m_respawnServerNo)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(42, m_respawnPosName)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(43, m_respawnPosx)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(44, m_respawnPosz)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)

		// charac
		COLUMN_ENTRY(45, m_class)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(46, m_classGrade)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(47, m_iContribution)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(48, m_gm)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(49, m_levelup_time)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(50, m_dwPlay_time)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(51, m_ChangeName)

		// result 
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(52, m_result)
#ifdef CHARAC_CREATE_DATE_20070309
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(53, m_CharacCreateDate)
#endif
	END_PARAM_MAP()
};

class C_prDS_GetCurBasicInfoAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_UniqueID;
	SHORT m_vzone;
	float m_vx;
	SHORT m_vy;
	float m_vz;
	SHORT m_sLifePower;
	SHORT m_sForcePower;
	SHORT m_sConcentrationPower;

	SHORT m_sRespawnServerNo ;
	CHAR m_sRespawnPosName[13] ;
	float m_fRespawnPosX ;
	float m_fRespawnPosZ ;


	LONG m_result;

	
	DEFINE_COMMAND_EX(C_prDS_GetCurBasicInfoAccessor, L"{ ? = CALL dbo._prDS_GetCurBasicInfo(?,?,?,?,?,?,?,?,?,?,?,?,?) }")


	BEGIN_PARAM_MAP(C_prDS_GetCurBasicInfoAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_UniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_vzone)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_vx)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_vy)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_vz)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(7, m_sLifePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(8, m_sForcePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(9, m_sConcentrationPower)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(10, m_sRespawnServerNo)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(11, m_sRespawnPosName)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(12, m_fRespawnPosX)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(13, m_fRespawnPosZ)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(14, m_result)
	END_PARAM_MAP()
};


#ifdef _PD_CASH_INVEN_

class C_prDs_GetDepotAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	__int64  m_d_money;
	BYTE m_depot[_DEPOT_BYTE_SIZE_];		// 40 Slot
	BYTE m_extend_depot[_EXT_DEPOT_BYTE_SIZE_];
	BYTE m_check_count ;
	SHORT m_check_year ;
	BYTE m_check_month ;
	BYTE m_check_day ;
	BYTE m_check_hour ;	

	SHORT m_version ;



	DEFINE_COMMAND_EX(C_prDs_GetDepotAccessor, L"{ ? = CALL dbo._prDs_GetDepot_v2(?,?,?,?,?,?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDs_GetDepotAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_d_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_depot)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_extend_depot)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_check_count)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(7, m_check_year)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(8, m_check_month)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(9, m_check_day)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(10, m_check_hour)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(11, m_version)
		END_PARAM_MAP()	

};

#else	// _PD_CASH_INVEN_

class C_prDs_GetDepotAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	__int64  m_d_money;
	BYTE m_depot[_DEPOT_BYTE_SIZE_];		// 40 Slot
	BYTE m_check_count ;
	SHORT m_check_year ;
	BYTE m_check_month ;
	BYTE m_check_day ;
	BYTE m_check_hour ;	

	SHORT m_version ;



	DEFINE_COMMAND_EX(C_prDs_GetDepotAccessor, L"{ ? = CALL dbo._prDs_GetDepot_v1(?,?,?,?,?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDs_GetDepotAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_d_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_depot)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_check_count)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_check_year)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(7, m_check_month)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(8, m_check_day)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(9, m_check_hour)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(10, m_version)
		END_PARAM_MAP()	

};

#endif


#ifdef _PD_CASH_INVEN_

#ifdef _PD_ADD_EVENT_COLLECTION_

class C_prDs_GetInvenAccessor
{
	/*	CREATE PROCEDURE _prDs_GetInven_v02
	@unique_id 	int ,			-- 캐릭터의 유니크 아이디
	@money	bigint OUTPUT,			-- 캐릭터의 돈
	@select_weapon smallint OUTPUT,
	@inventory 	binary(3888) 	OUTPUT,	-- 아이템 구조체(바이너리값)
	@med_check	binary(110)	OUTPUT,
	@version	smallint		OUTPUT,
	@collection	binary(1024)	OUTPUT,
	@bloodpoint	smallint	output,
	@monster_kill	smallint	output
	AS
	*/
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	_CHARAC_INVENTORY m_data ;

	__int64		m_money;

	DEFINE_COMMAND_EX(C_prDs_GetInvenAccessor, L"{ ? = CALL dbo._prDs_GetInven_v06(?,?,?,?,?,?,?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_GetInvenAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_data.sSelectWeaponID)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_data.cInventory)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_data.cCashInven)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(7, m_data.cElixirData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(8, m_data.sVersion)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(9, m_data.cCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(10, m_data.cEventCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(11, m_data.sBloodPoint)
		END_PARAM_MAP()

};

#else	// _PD_ADD_EVENT_COLLECTION_

class C_prDs_GetInvenAccessor
{
	/*	CREATE PROCEDURE _prDs_GetInven_v02
	@unique_id 	int ,			-- 캐릭터의 유니크 아이디
	@money	bigint OUTPUT,			-- 캐릭터의 돈
	@select_weapon smallint OUTPUT,
	@inventory 	binary(3888) 	OUTPUT,	-- 아이템 구조체(바이너리값)
	@med_check	binary(110)	OUTPUT,
	@version	smallint		OUTPUT,
	@collection	binary(1024)	OUTPUT,
	@bloodpoint	smallint	output,
	@monster_kill	smallint	output
	AS
	*/
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	_CHARAC_INVENTORY m_data ;

	__int64		m_money;

	DEFINE_COMMAND_EX(C_prDs_GetInvenAccessor, L"{ ? = CALL dbo._prDs_GetInven_v05(?,?,?,?,?,?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_GetInvenAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_data.sSelectWeaponID)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_data.cInventory)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_data.cCashInven)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(7, m_data.cElixirData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(8, m_data.sVersion)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(9, m_data.cCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(10, m_data.sBloodPoint)
		END_PARAM_MAP()

};

#endif	// _PD_ADD_EVENT_COLLECTION_




#else // _PD_CASH_INVEN_

#ifdef _PD_ADD_EVENT_COLLECTION_

class C_prDs_GetInvenAccessor
{
	/*	CREATE PROCEDURE _prDs_GetInven_v02
	@unique_id 	int ,			-- 캐릭터의 유니크 아이디
	@money	bigint OUTPUT,			-- 캐릭터의 돈
	@select_weapon smallint OUTPUT,
	@inventory 	binary(3888) 	OUTPUT,	-- 아이템 구조체(바이너리값)
	@med_check	binary(110)	OUTPUT,
	@version	smallint		OUTPUT,
	@collection	binary(1024)	OUTPUT,
	@bloodpoint	smallint	output,
	@monster_kill	smallint	output
	AS
	*/
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	_CHARAC_INVENTORY m_data ;

	__int64		m_money;

	DEFINE_COMMAND_EX(C_prDs_GetInvenAccessor, L"{ ? = CALL dbo._prDs_GetInven_v04(?,?,?,?,?,?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_GetInvenAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_data.sSelectWeaponID)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_data.cInventory)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_data.cElixirData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(7, m_data.sVersion)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(8, m_data.cCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(9, m_data.cEventCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(10, m_data.sBloodPoint)
		END_PARAM_MAP()

};

#else	// _PD_ADD_EVENT_COLLECTION_

class C_prDs_GetInvenAccessor
{
/*	CREATE PROCEDURE _prDs_GetInven_v02
		@unique_id 	int ,			-- 캐릭터의 유니크 아이디
		@money	bigint OUTPUT,			-- 캐릭터의 돈
		@select_weapon smallint OUTPUT,
		@inventory 	binary(3888) 	OUTPUT,	-- 아이템 구조체(바이너리값)
		@med_check	binary(110)	OUTPUT,
		@version	smallint		OUTPUT,
		@collection	binary(1024)	OUTPUT,
		@bloodpoint	smallint	output,
		@monster_kill	smallint	output
		AS
*/
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	_CHARAC_INVENTORY m_data ;

	__int64		m_money;

	DEFINE_COMMAND_EX(C_prDs_GetInvenAccessor, L"{ ? = CALL dbo._prDs_GetInven_v02(?,?,?,?,?,?,?,?) }")

	BEGIN_PARAM_MAP(C_prDs_GetInvenAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_unique_id)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_money)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_data.sSelectWeaponID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_data.cInventory)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_data.cElixirData)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(7, m_data.sVersion)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(8, m_data.cCollectionData)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(9, m_data.sBloodPoint)
	END_PARAM_MAP()

};

#endif	// _PD_ADD_EVENT_COLLECTION_


#endif // _PD_CASH_INVEN_

class C_prDS_GetMartialAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	_CHARAC_MARTIAL_DATA m_data ;

#ifdef _PD_SKILL_EXTEND_

	DEFINE_COMMAND_EX(C_prDS_GetMartialAccessor, L"{ ? = CALL dbo._prDS_GetMartial_v2(?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDS_GetMartialAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_data.cData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_data.cBookData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_data.cExtendMartial)
		END_PARAM_MAP()

#else
	
	DEFINE_COMMAND_EX(C_prDS_GetMartialAccessor, L"{ ? = CALL dbo._prDS_GetMartial(?,?,?) }")

		BEGIN_PARAM_MAP(C_prDS_GetMartialAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_data.cData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_data.cBookData)
		END_PARAM_MAP()
#endif
};


#ifdef _PD_QUEST_MAX_600_

class C_prDS_GetQuestAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	BYTE m_QuestData[sizeof(_CHARAC_QUEST_DATA)-sizeof(_CHARAC_QUEST_DATA::__quest_data2)] ;
	BYTE m_QuestData2[sizeof(_CHARAC_QUEST_DATA::__quest_data2)];

	LONG m_result;


	DEFINE_COMMAND_EX(C_prDS_GetQuestAccessor, L"{ ? = CALL dbo._prDS_GetQuest2(?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_GetQuestAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)		
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_QuestData)		
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_QuestData2)		
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_result)
		END_PARAM_MAP()
};

#else

class C_prDS_GetQuestAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	
	BYTE m_QuestData[sizeof(_CHARAC_QUEST_DATA)] ;

	LONG m_result;


	DEFINE_COMMAND_EX(C_prDS_GetQuestAccessor, L"{ ? = CALL dbo._prDS_GetQuest(?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_GetQuestAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)		
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_QuestData)		
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_result)
		END_PARAM_MAP()
};

#endif

class C_prDS_LoginAccessor
{
public:

	LONG		m_RETURN_VALUE;
	char		m_acc[en_max_lil+1];
	short		m_forest ;
	int			m_temp_uid ;
	int			m_uid ;


	DEFINE_COMMAND_EX(C_prDS_LoginAccessor, L"{ ? = CALL dbo._prDS_Login(?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_LoginAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
			COLUMN_ENTRY(2, m_acc)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_forest)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_temp_uid)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_uid)			
		END_PARAM_MAP()
};

class C_prDS_NexonLoginAccessor
{
public:

	inline void SetAccUid( int uid )			
	{ 
		m_uid = uid ; 
	}
	inline void SetAccount( const char * acc )	{ strncpy( m_acc, acc, en_max_lil ) ; m_acc[en_max_lil] = 0 ; }
	inline void SetForest( short forest )		{ m_forest = forest ; }
	inline void SetTempUid( u_short tempuid )	{ m_temp_uid = static_cast<int>(tempuid) ; }

	inline char * GetAccount()	{ return m_acc ; }
	

	// Result : 0 - Success, else fail;
	DEFINE_COMMAND_EX(C_prDS_NexonLoginAccessor, L"{ CALL dbo._prDS_NexonLogin(?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDS_NexonLoginAccessor)
			SET_PARAM_TYPE(DBPARAMIO_INPUT )
			COLUMN_ENTRY(1, m_uid)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
			COLUMN_ENTRY(2, m_acc)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_forest)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_temp_uid)
		END_PARAM_MAP()

protected:
	int			m_uid ;
	char		m_acc[en_max_lil+1];
	short		m_forest ;
	int			m_temp_uid ;
};

class C_prDS_GetAccountUIDAccessor
{
public:
	inline void SetAccount( char * acc ) 
	{
		strncpy( m_acc, acc, en_max_lil+1 ) ;
		m_uid = 0 ;
	}
	inline int GetAccUID()		{ return m_uid ; }


	DEFINE_COMMAND_EX(C_prDS_GetAccountUIDAccessor, L"{ CALL dbo._prDS_GetAccountUID(?,?) }")

		BEGIN_PARAM_MAP(C_prDS_GetAccountUIDAccessor)
			SET_PARAM_TYPE(DBPARAMIO_INPUT )
			COLUMN_ENTRY(1, m_acc)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(2, m_uid)			
		END_PARAM_MAP()
protected:
	char		m_acc[en_max_lil+1];
	int			m_uid ;
};

class C_prDS_LogoutAccessor
{
public:

	LONG		m_RETURN_VALUE;
	char		m_acc[en_max_lil+1];

	DEFINE_COMMAND_EX(C_prDS_LogoutAccessor, L"{ ? = CALL dbo._prDS_Logout(?) }")


		BEGIN_PARAM_MAP(C_prDS_LogoutAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_acc)			
		END_PARAM_MAP()
};

class C_prDS_EnterCheckAccessor
{
public:

	LONG		m_RETURN_VALUE;
	char		m_acc[en_max_lil+1];
	int			m_forest ;
	int			m_state ;
	u_short		m_temp_uid ;
	int			m_result ;

	DEFINE_COMMAND_EX(C_prDS_EnterCheckAccessor, L"{ ? = CALL dbo._prDS_EnterCheck(?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_EnterCheckAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_acc)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_forest)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_state)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_temp_uid)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_result)			
		END_PARAM_MAP()
};


class C_prDS_NexonEnterCheckAccessor
{
public:

	LONG		m_RETURN_VALUE;
	char		m_acc[en_max_lil+1];
	int			m_forest ;
	int			m_state ;
	u_short		m_temp_uid ;
	int			m_result ;

	DEFINE_COMMAND_EX(C_prDS_NexonEnterCheckAccessor, L"{ ? = CALL dbo._prDS_NexonEnterCheck(?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_NexonEnterCheckAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_acc)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_forest)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_state)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_temp_uid)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_result)			
		END_PARAM_MAP()
};


class C_prDS_PW_CHECKAccessor
{
public:

	LONG		m_RETURN_VALUE;
	int			m_unique_id ;
	char		m_pw[13] ;
	int			m_result ;


	DEFINE_COMMAND_EX(C_prDS_PW_CHECKAccessor, L"{ ? = CALL dbo._prDS_PW_CHECK(?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_PW_CHECKAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_pw)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_result)			
		END_PARAM_MAP()
};

class C_prDS_GetMatchAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	_CHARAC_MATCH m_data ;
	int	m_result ;


	DEFINE_COMMAND_EX(C_prDS_GetMatchAccessor, L"{ ? = CALL dbo._prDS_GetMatch(?,?,?,?,?,?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_GetMatchAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_data.iWinScore)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_data.iLostScore)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_data.iTieScore)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_data.iSeriesCount)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(7, m_data.iMaxSeriesWinCount)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(8, m_data.iMaxSeriesLoseCount)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(9, m_data.iAbuseCount)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(10, m_data.cPrevMatchResult)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(11, m_result)
		END_PARAM_MAP()

};


class C_prDS_SetMatchAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	_CHARAC_MATCH m_data ;
	int	m_result ;

	DEFINE_COMMAND_EX(C_prDS_SetMatchAccessor, L"{ ? = CALL dbo._prDS_SetMatch(?,?,?,?,?,?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_SetMatchAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_data.iWinScore)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_data.iLostScore)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_data.iTieScore)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6, m_data.iSeriesCount)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7, m_data.iMaxSeriesWinCount)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(8, m_data.iMaxSeriesLoseCount)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(9, m_data.iAbuseCount)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(10, m_data.cPrevMatchResult)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(11, m_result)
		END_PARAM_MAP()

};


class C_prDS_SelectCharacAccessor
{
public:

	LONG m_RETURN_VALUE;	
	CHAR m_select_charac[13];

    DEFINE_COMMAND_EX(C_prDS_SelectCharacAccessor, L"{ ? = CALL dbo._prDS_SelectCharac(?) }")


		BEGIN_PARAM_MAP(C_prDS_SelectCharacAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_select_charac)			
		END_PARAM_MAP()
};


class C_prDS_UpdateCharacAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_uniqueID;
	CHAR m_chr_nic[13];
	SHORT m_party;
	CHAR m_gamete[13];
	SHORT m_hair;
	SHORT m_face;
	SHORT m_class ;				// 역할 : 기본값 0
	SHORT m_classGrade ;			// 역할 등급 : 기본값 0		: 2004/11/10 추가. Ray
	DWORD m_iContribution ;		// 문파 기여도 : 기본값 0	: 2004/11/10 추가. Ray
	SHORT  m_gm ;					// gm 여부
	DWORD m_dwPlayTime ;
	SHORT m_state;

	DEFINE_COMMAND_EX(C_prDS_UpdateCharacAccessor, L"{ ? = CALL dbo._prDS_UpdateCharac(?,?,?,?,?,?,?,?,?,?,?,?) }")
	BEGIN_PARAM_MAP(C_prDS_UpdateCharacAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_uniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_chr_nic)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_party)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(5, m_gamete)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(6, m_hair)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(7, m_face)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(8, m_class)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(9, m_classGrade)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(10, m_iContribution)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(11, m_gm)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(12, m_dwPlayTime)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(13, m_state)
	END_PARAM_MAP()
};

class C_prDs_UpdateCharacStateAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_uniqueID;	

	_CHARAC_STATE m_data ;

	LONG m_result;

	DEFINE_COMMAND_EX(C_prDs_UpdateCharacStateAccessor, 
		L"{ ? = CALL dbo._prDs_UpdateCharacState(?,?,?,?,?,?,?,?,?,?,\
		 ?,?,?,?,?,?,?,?,?,?,\
		 ?,?,?,?) }")

	BEGIN_PARAM_MAP(C_prDs_UpdateCharacStateAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_uniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_data.sInnerLevel)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_data.uiJin)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(5, m_data.iGong)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(6, m_data.sRetribution)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(7, m_data.iHonor)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(8, m_data.usFatigue)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(9, m_data.sWoundValue)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(10, m_data.sInsideWoundValue)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(11, m_data.sShowdown)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(12, m_data.sFuryParameter)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(13, m_data.sLevelUpGameSecond)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(14, m_data.iORIndex)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(15, m_data.sPeaceMode)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(16, m_data.iMuteTime)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(17, m_data.iHonorGaveDate)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(18, m_data.iHonorTakeDate)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(19, m_data.iHiding)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(20, m_data.iBlockingEndDate)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(21, m_data.sPkPrevDeadMode)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(22, m_data.sPkDeadCount)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(23, m_data.sPkKillCount)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(24, m_data.sMonsterKill)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(25, m_result)
	END_PARAM_MAP()
};

#ifdef _PD_LEVEL_RATE_SAVE_
class C_prDs_UpdateCharacState_v2Accessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_uniqueID;	

	_CHARAC_STATE m_data ;

	LONG m_result;

	DEFINE_COMMAND_EX(C_prDs_UpdateCharacState_v2Accessor, 
		L"{ ? = CALL dbo._prDs_UpdateCharacState_v2(?,?,?,?,?,?,?,?,?,?,\
		 ?,?,?,?,?,?,?,?,?,?,\
		 ?,?,?,?,?,?) }")

		 BEGIN_PARAM_MAP(C_prDs_UpdateCharacState_v2Accessor)
			 SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(1, m_RETURN_VALUE)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(2, m_uniqueID)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(3, m_data.sInnerLevel)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(4, m_data.uiJin)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(5, m_data.iGong)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(6, m_data.sRetribution)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(7, m_data.iHonor)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(8, m_data.usFatigue)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(9, m_data.sWoundValue)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(10, m_data.sInsideWoundValue)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(11, m_data.sShowdown)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(12, m_data.sFuryParameter)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(13, m_data.sLevelUpGameSecond)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(14, m_data.iORIndex)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(15, m_data.sPeaceMode)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(16, m_data.iMuteTime)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(17, m_data.iHonorGaveDate)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(18, m_data.iHonorTakeDate)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(19, m_data.iHiding)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(20, m_data.iBlockingEndDate)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(21, m_data.sPkPrevDeadMode)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(22, m_data.sPkDeadCount)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(23, m_data.sPkKillCount)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(24, m_data.sMonsterKill)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(25, m_data.sLevelModifyFlag)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT)
			 COLUMN_ENTRY(26, m_data.dLevelRate)
			 SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			 COLUMN_ENTRY(27, m_result)
		 END_PARAM_MAP()
};
#endif


class C_prDS_UpdateCurBasicInfoAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_UniqueID;
	SHORT m_vzone;
	float m_vx;
	SHORT m_vy;
	float m_vz;
	SHORT m_sLifePower;
	SHORT m_sForcePower;
	SHORT m_sConcentrationPower;

	SHORT m_sRespawnServerNo ;
	CHAR m_sRespawnPosName[13] ;
	float m_fRespawnPosX ;
	float m_fRespawnPosZ ;

	LONG m_result;

	DEFINE_COMMAND_EX(C_prDS_UpdateCurBasicInfoAccessor, L"{ ? = CALL dbo._prDS_UpdateCurBasicInfo(?,?,?,?,?,?,?,?,?,?,?,?,?) }")

	BEGIN_PARAM_MAP(C_prDS_UpdateCurBasicInfoAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_UniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_vzone)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_vx)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(5, m_vy)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(6, m_vz)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(7, m_sLifePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(8, m_sForcePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(9, m_sConcentrationPower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(10, m_sRespawnServerNo)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(11, m_sRespawnPosName)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(12, m_fRespawnPosX)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(13, m_fRespawnPosZ)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(14, m_result)
	END_PARAM_MAP()
};



#ifdef _PD_CASH_INVEN_

class C_prDs_UpdateDepotAccessor
{
public:
	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	__int64 m_d_money;
	BYTE m_depot[_DEPOT_BYTE_SIZE_];
	BYTE m_extend_depot[_EXT_DEPOT_BYTE_SIZE_];
	BYTE m_check_count;
	SHORT m_check_year;
	BYTE m_check_month;
	BYTE m_check_day;
	BYTE m_check_hour;


	short m_version ;

	DEFINE_COMMAND_EX(C_prDs_UpdateDepotAccessor, L"{ ? = CALL dbo._prDs_UpdateDepot_v2(?,?,?,?,?,?,?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_UpdateDepotAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_d_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_depot)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_extend_depot)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6, m_check_count)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7, m_check_year)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(8, m_check_month)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(9, m_check_day)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(10, m_check_hour)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(11, m_version)
		END_PARAM_MAP()

};

#else	// _PD_CASH_INVEN_

class C_prDs_UpdateDepotAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_unique_id;	
	__int64 m_d_money;
	BYTE m_depot[_DEPOT_BYTE_SIZE_];
	BYTE m_check_count ;
	SHORT m_check_year ;
	BYTE m_check_month ;
	BYTE m_check_day ;
	BYTE m_check_hour ;	


	short m_version ;

	DEFINE_COMMAND_EX(C_prDs_UpdateDepotAccessor, L"{ ? = CALL dbo._prDs_UpdateDepot_v1(?,?,?,?,?,?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_UpdateDepotAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_d_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_depot)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_check_count)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6, m_check_year)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7, m_check_month)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(8, m_check_day)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(9, m_check_hour)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(10, m_version)
		END_PARAM_MAP()

};

#endif


#ifdef _PD_CASH_INVEN_

#ifdef _PD_ADD_EVENT_COLLECTION_

class C_prDs_UpdateInvenAccessor
{
public:	
	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	_CHARAC_INVENTORY m_data ;

	__int64		m_money;

	/*
	CREATE PROCEDURE _prDs_UpdateInven_v02
	@unique_id 	int ,			-- 캐릭터의 유니크 아이디
	@money	bigint,			-- 캐릭터의 돈
	@select_weapon smallint,
	@inventory 	binary(3888),			-- 아이템 구조체(바이너리값)
	@med_check 	binary(110),
	@version	smallint,
	@collection	binary(1024),
	@bloodpoint	smallint,
	@monster_kill	smallint
	AS
	*/


	DEFINE_COMMAND_EX(C_prDs_UpdateInvenAccessor, L"{ ? = CALL dbo._prDs_UpdateInven_v06(?,?,?,?,?,?,?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_UpdateInvenAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_data.sSelectWeaponID)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_data.cInventory)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6, m_data.cCashInven)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7, m_data.cElixirData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(8, m_data.sVersion)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(9, m_data.cCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(10, m_data.cEventCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(11, m_data.sBloodPoint)
		END_PARAM_MAP()
};

#else	//_PD_ADD_EVENT_COLLECTION_

class C_prDs_UpdateInvenAccessor
{
public:	


	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	_CHARAC_INVENTORY m_data ;

	__int64		m_money;

	/*
	CREATE PROCEDURE _prDs_UpdateInven_v02
	@unique_id 	int ,			-- 캐릭터의 유니크 아이디
	@money	bigint,			-- 캐릭터의 돈
	@select_weapon smallint,
	@inventory 	binary(3888),			-- 아이템 구조체(바이너리값)
	@med_check 	binary(110),
	@version	smallint,
	@collection	binary(1024),
	@bloodpoint	smallint,
	@monster_kill	smallint
	AS
	*/


	DEFINE_COMMAND_EX(C_prDs_UpdateInvenAccessor, L"{ ? = CALL dbo._prDs_UpdateInven_v05(?,?,?,?,?,?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_UpdateInvenAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_data.sSelectWeaponID)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_data.cInventory)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6, m_data.cCashInven)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7, m_data.cElixirData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(8, m_data.sVersion)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(9, m_data.cCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(10, m_data.sBloodPoint)
		END_PARAM_MAP()

};

#endif	// _PD_ADD_EVENT_COLLECTION_


#else	// _PD_CASH_INVEN_


#ifdef _PD_ADD_EVENT_COLLECTION_

class C_prDs_UpdateInvenAccessor
{
public:	
	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	_CHARAC_INVENTORY m_data ;

	__int64		m_money;

	/*
	CREATE PROCEDURE _prDs_UpdateInven_v02
	@unique_id 	int ,			-- 캐릭터의 유니크 아이디
	@money	bigint,			-- 캐릭터의 돈
	@select_weapon smallint,
	@inventory 	binary(3888),			-- 아이템 구조체(바이너리값)
	@med_check 	binary(110),
	@version	smallint,
	@collection	binary(1024),
	@bloodpoint	smallint,
	@monster_kill	smallint
	AS
	*/


	DEFINE_COMMAND_EX(C_prDs_UpdateInvenAccessor, L"{ ? = CALL dbo._prDs_UpdateInven_v04(?,?,?,?,?,?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_UpdateInvenAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_data.sSelectWeaponID)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_data.cInventory)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6, m_data.cElixirData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7, m_data.sVersion)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(8, m_data.cCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(9, m_data.cEventCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(10, m_data.sBloodPoint)
		END_PARAM_MAP()
};

#else	//_PD_ADD_EVENT_COLLECTION_

class C_prDs_UpdateInvenAccessor
{
public:	


	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	
	_CHARAC_INVENTORY m_data ;

	__int64		m_money;
    
/*
CREATE PROCEDURE _prDs_UpdateInven_v02
@unique_id 	int ,			-- 캐릭터의 유니크 아이디
@money	bigint,			-- 캐릭터의 돈
@select_weapon smallint,
@inventory 	binary(3888),			-- 아이템 구조체(바이너리값)
@med_check 	binary(110),
@version	smallint,
@collection	binary(1024),
@bloodpoint	smallint,
@monster_kill	smallint
AS
*/

	
	DEFINE_COMMAND_EX(C_prDs_UpdateInvenAccessor, L"{ ? = CALL dbo._prDs_UpdateInven_v02(?,?,?,?,?,?,?,?) }")

	BEGIN_PARAM_MAP(C_prDs_UpdateInvenAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_unique_id)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_money)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_data.sSelectWeaponID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(5, m_data.cInventory)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(6, m_data.cElixirData)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(7, m_data.sVersion)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(8, m_data.cCollectionData)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(9, m_data.sBloodPoint)
	END_PARAM_MAP()

};

#endif	// _PD_ADD_EVENT_COLLECTION_


#endif // _PD_CASH_INVEN_

/*
class C_prDs_UpdateInvenAccessor_v3
{
public:	


	LONG m_RETURN_VALUE;
	LONG m_unique_id;

	_CHARAC_INVENTORY m_data ;

	__int64		m_money;

	DEFINE_COMMAND_EX(C_prDs_UpdateInvenAccessor_v3, L"{ ? = CALL dbo._prDs_UpdateInven_v03(?,?,?,?,?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_UpdateInvenAccessor_v3)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_money)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_data.sSelectWeaponID)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_data.cInventory)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6, m_data.cElixirData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7, m_data.sVersion)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(8, m_data.cCollectionData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(9, m_data.sBloodPoint)
		END_PARAM_MAP()

};
*/

class C_prDs_UpdateLevelAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_uniqueID;
	
	_CHARAC_LEVEL m_data ;

	LONG m_result;


	DEFINE_COMMAND_EX(C_prDs_UpdateLevelAccessor, L"{ ? = CALL dbo._prDs_UpdateLevel(?,?,?,?,?,?,?,?,?,?,?,?) }")


	BEGIN_PARAM_MAP(C_prDs_UpdateLevelAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_uniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_data.sMaxLifePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_data.sMaxForcePower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(5, m_data.sMaxConcentrationPower)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(6, m_data.sConstitution)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(7, m_data.sZen)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(8, m_data.sIntelligence)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(9, m_data.sDexterity)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(10, m_data.sStr)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(11, m_data.sLeftPoint)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(12, m_data.sMasteryPoint)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(13, m_result)
	END_PARAM_MAP()
};

class C_prDs_UpdateMartialAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;	
	_CHARAC_MARTIAL_DATA m_data ;

#ifdef _PD_SKILL_EXTEND_
	DEFINE_COMMAND_EX(C_prDs_UpdateMartialAccessor, L"{ ? = CALL dbo._prDs_UpdateMartial_v2(?,?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_UpdateMartialAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_data.cData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_data.cBookData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5, m_data.cExtendMartial)
		END_PARAM_MAP()

#else

	DEFINE_COMMAND_EX(C_prDs_UpdateMartialAccessor, L"{ ? = CALL dbo._prDs_UpdateMartial(?,?,?) }")

		BEGIN_PARAM_MAP(C_prDs_UpdateMartialAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_data.cData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_data.cBookData)
		END_PARAM_MAP()
#endif
};


#ifdef _PD_QUEST_MAX_600_

class C_prDS_UpdateQuestAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_unique_id;	
	BYTE m_QuestData[sizeof(_CHARAC_QUEST_DATA) - sizeof(_CHARAC_QUEST_DATA::__quest_data2)];
	BYTE m_QuestData2[sizeof(_CHARAC_QUEST_DATA::__quest_data2)];
	LONG m_result;

	DEFINE_COMMAND_EX(C_prDS_UpdateQuestAccessor, L"{ ? = CALL dbo._prDS_UpdateQuest2(?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_UpdateQuestAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)		
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_QuestData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_QuestData2)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_result)
		END_PARAM_MAP()
};

#else

class C_prDS_UpdateQuestAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_unique_id;	
	BYTE m_QuestData[sizeof(_CHARAC_QUEST_DATA)] ;
	LONG m_result;

	DEFINE_COMMAND_EX(C_prDS_UpdateQuestAccessor, L"{ ? = CALL dbo._prDS_UpdateQuest(?,?,?) }")


		BEGIN_PARAM_MAP(C_prDS_UpdateQuestAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)		
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_QuestData)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_result)
		END_PARAM_MAP()
};

#endif

class C_prDS_GetLastServerAccessor
{
public:

	LONG		m_RETURN_VALUE;
	int			m_unique_id ;
	int			m_last_server ;


	DEFINE_COMMAND_EX(C_prDS_GetLastServerAccessor, L"{ ? = CALL dbo._prDS_GetLastServer(?,?) }")


		BEGIN_PARAM_MAP(C_prDS_GetLastServerAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_last_server)
		END_PARAM_MAP()
};


// 마지막 접속 서버 저장하기.
class C_prDS_SetLastServerAccessor
{
public:

	LONG		m_RETURN_VALUE;
	int			m_unique_id ;
	int			m_last_server ;

	DEFINE_COMMAND_EX(C_prDS_SetLastServerAccessor, L"{ ? = CALL dbo._prDS_SetLastServer(?,?) }")


		BEGIN_PARAM_MAP(C_prDS_SetLastServerAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_last_server)
		END_PARAM_MAP()
};


class C_prDS_GetNicAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;	
	BYTE m_data[sizeof(_CHARAC_NIC)] ;



	DEFINE_COMMAND_EX(C_prDS_GetNicAccessor, L"{ ? = CALL dbo._prDS_GetNics(?,?) }")


		BEGIN_PARAM_MAP(C_prDS_GetNicAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_data)
		END_PARAM_MAP()

	
};


class C_prDS_SetNicAccessor
{
public:

	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	BYTE m_data[sizeof(_CHARAC_NIC)] ;


	DEFINE_COMMAND_EX(C_prDS_SetNicAccessor, L"{ ? = CALL dbo._prDS_SetNics(?,?) }")


		BEGIN_PARAM_MAP(C_prDS_SetNicAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_data)
		END_PARAM_MAP()

	
};



class C_prDs_GetBackupDataAccessor
{
public:


	LONG m_RETURN_VALUE;
	LONG m_unique_id;
	char m_date_from[16] ;
	char m_date_to[16] ;
	BYTE m_inven[1160] ;
	BYTE m_martial[480] ;
	BYTE m_quest[940] ;


	DEFINE_COMMAND_EX(C_prDs_GetBackupDataAccessor, L"{ ? = CALL dbo._prGMS_GetBACKUPInven(?,?,?,?,?,?) }")


		BEGIN_PARAM_MAP(C_prDs_GetBackupDataAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_RETURN_VALUE)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_date_from)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_date_to)			
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_inven)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(6, m_martial)
			SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(7, m_quest)
	END_PARAM_MAP()
	
};

class C_prDSInsertOrganizationAccessor
{
public:
	int			m_index ;
	char		m_name[en_or_name_length+1] ;
	short		m_sProp ;
	short		m_sBaseParty ;
	u_int		m_uiExp ;
	u_int		m_uiContribution ;
	u_short		m_usCurMem ;	
	char		m_cCaptainName[13] ;
	BYTE		m_MemListData[_SIZE_OF_OR_MEM_LIST_DATA] ;

	DEFINE_COMMAND_EX( C_prDSInsertOrganizationAccessor, L"{ CALL dbo._prDSInsertOrganization(?,?,?,?,?,?,?,?,?) }" )

	BEGIN_PARAM_MAP( C_prDSInsertOrganizationAccessor )
		SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 1, m_index )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 2, m_name )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 3, m_sProp )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 4, m_sBaseParty )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 5, m_uiExp )		
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 6, m_uiContribution )		
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 7, m_usCurMem )		
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 8, m_cCaptainName )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 9, m_MemListData )
	END_PARAM_MAP()
	

};

class C_prDSUpdateOrganizationAccessor
{
public:
	int			m_index ;	
	u_int		m_uiExp ;
	u_int		m_uiContribution ;
	u_short		m_usCurMem ;	
	char		m_cCaptainName[13] ;
	BYTE		m_MemListData[_SIZE_OF_OR_MEM_LIST_DATA] ;

	char		m_cNotice[_OR_NOTICE_SIZE] ;
	BYTE		m_outTime[_SIZE_OF_OR_OUTTIME_LIST_DATA] ;

	DEFINE_COMMAND_EX( C_prDSUpdateOrganizationAccessor, L"{ CALL _prDSUpdatetOrganization_(?,?,?,?,?,?,?,?) }" )
		BEGIN_PARAM_MAP( C_prDSUpdateOrganizationAccessor )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_index ) ;
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 2, m_uiExp ) ;
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 3, m_uiContribution ) ;
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 4, m_usCurMem ) ;		
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 5, m_cCaptainName )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 6, m_MemListData )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 7, m_cNotice )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 8, m_outTime )
	END_PARAM_MAP()

	
};

class C_prDSDeleteOrganizationAccessor
{
public:
	int		m_index ;

	DEFINE_COMMAND_EX( C_prDSDeleteOrganizationAccessor, L"{ CALL _prDSDeleteOrganization(?) }")
	BEGIN_PARAM_MAP( C_prDSDeleteOrganizationAccessor )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_index )
	END_PARAM_MAP()
	
};

class C_prDS_NewJoiner_CHECKAccessor
{
public:
	int		m_uid ;
	int		m_iResult ;

	DEFINE_COMMAND_EX( C_prDS_NewJoiner_CHECKAccessor, L"{ CALL _prDS_NewJoiner_CHECK(?,?) }")
	BEGIN_PARAM_MAP( C_prDS_NewJoiner_CHECKAccessor )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_uid )
		SET_PARAM_TYPE( DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 2, m_iResult )
	END_PARAM_MAP()

	
};

class C_pr_prCharacLogoutAccessor
{
public:
	int		m_uid ;

	DEFINE_COMMAND_EX( C_pr_prCharacLogoutAccessor, L"{ CALL _prCharacLogout(?) } ")
	BEGIN_PARAM_MAP( C_pr_prCharacLogoutAccessor )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_uid ) 
	END_PARAM_MAP()

};



class C_prLS_GetIsPcbangAccessor
{
public:
	char		m_ip[15] ;
	short		m_result ;

	DEFINE_COMMAND_EX( C_prLS_GetIsPcbangAccessor, L"{ CALL _prLS_GetIsPcbang(?,?) }")
	BEGIN_PARAM_MAP( C_prLS_GetIsPcbangAccessor )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_ip )
		SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 2, m_result )
	END_PARAM_MAP()

};

class C_prDSGetCountLimitAccessor
{
public:
	int			m_id ;
	short		m_type ;
	int			m_count ;
	short		m_flag ;

	DEFINE_COMMAND_EX(C_prDSGetCountLimitAccessor, L"{ CALL _prDSGetCountLimit(?,?,?,?) }" )
	BEGIN_PARAM_MAP(C_prDSGetCountLimitAccessor)
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_id )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 2, m_type )
		SET_PARAM_TYPE( DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 3, m_count )
		SET_PARAM_TYPE( DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 4, m_flag )
	END_PARAM_MAP()
};

class C_prDSSetCountLimitAccessor
{
public:
	int			m_id ;
	short		m_type ;
	int			m_count ;
	short		m_flag ;

	DEFINE_COMMAND_EX(C_prDSSetCountLimitAccessor, L"{ CALL _prDSSetCountLimit(?,?,?,?) }" )
		BEGIN_PARAM_MAP(C_prDSSetCountLimitAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_id )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_type )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 3, m_count )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 4, m_flag )
	END_PARAM_MAP()
};




class C_prReflushedCheckAccessor
{
public:
	int		m_rank_type ;
	int		m_rank_version ;	

	
	BEGIN_COLUMN_MAP( C_prReflushedCheckAccessor )	
		COLUMN_ENTRY( 1, m_rank_type )
		COLUMN_ENTRY( 2, m_rank_version )
	END_COLUMN_MAP()

};

// 쿼리 엑세서..
class CRankSelectAccessor
{
public:
	char m_cCharacName[13] ;

	BEGIN_COLUMN_MAP(CRankSelectAccessor)
		COLUMN_ENTRY(1, m_cCharacName )
	END_COLUMN_MAP()

};



class CRank_LevelAccessor
{
public:
	char		m_cCharacName[13] ;
	short		m_inner_level ;
	u_int		m_exp ;
	short		m_moonpa ;
	short		m_class ;
	

	BEGIN_COLUMN_MAP( CRank_LevelAccessor )
		COLUMN_ENTRY(1, m_cCharacName )
		COLUMN_ENTRY(2, m_inner_level )
		COLUMN_ENTRY(3, m_exp )
		COLUMN_ENTRY(4, m_moonpa )
		COLUMN_ENTRY(5, m_class )
	END_COLUMN_MAP()

};



class COrSelectAccessor
{
public:
	int			m_index ;	
	char		m_name[en_or_name_length+1] ;
	short		m_sProp ;
	short		m_sBaseParty ;
	u_int		m_uiExp ;
	u_int		m_uiContribution ;
	u_short		m_usCurMem ;	
	char		m_cCaptainName[13] ;
	BYTE		m_MemListData[_SIZE_OF_OR_MEM_LIST_DATA] ;
	char		m_cNotice[_OR_NOTICE_SIZE] ;
	BYTE		m_outTime[_SIZE_OF_OR_OUTTIME_LIST_DATA] ;
#ifdef _PD_ALLY_ORG_STEP_1_
	int			m_iAllyIndex;
#endif
	

	BEGIN_COLUMN_MAP( COrSelectAccessor )
		COLUMN_ENTRY( 1, m_index ) 	
		COLUMN_ENTRY( 2, m_name )
		COLUMN_ENTRY( 3, m_sProp )
		COLUMN_ENTRY( 4, m_sBaseParty )
		COLUMN_ENTRY( 5, m_uiExp )
		COLUMN_ENTRY( 6, m_uiContribution ) 
		COLUMN_ENTRY( 7, m_usCurMem )		
		COLUMN_ENTRY( 8, m_cCaptainName )
		COLUMN_ENTRY( 9, m_MemListData )	
		COLUMN_ENTRY( 10, m_cNotice )
		COLUMN_ENTRY( 11, m_outTime )	
#ifdef _PD_ALLY_ORG_STEP_1_
		COLUMN_ENTRY( 12, m_iAllyIndex )	
#endif
	END_COLUMN_MAP()
};

#ifdef _PD_ALLY_ORG_STEP_1_
class CAllyReadAccessor
{
	/*
	ND_V01_ALLY_ORG
	3	ally_index			int	4	0
	0	ally_org_1			int	4	0
	0	ally_org_2			int	4	0
	0	ally_org_3			int	4	0
	0	ally_org_4			int	4	0
	0	ally_org_5			int	4	0
	0	ally_name			varchar	24	0
	0	ally_base_property	smallint	2	0
	0	ally_money			bigint	8	0
	0	ally_state			smallint	2	0
	0	ally_date			smalldatetime	4	0
	0	ally_breakup_date	smalldatetime	4	0
	*/
public:
	inline int GetAllyIndex()	{ return iAllyIndex; }
	inline int GetOrg1()		{ return iOrg1; }
	inline int GetOrg2()		{ return iOrg2; }
	inline int GetOrg3()		{ return iOrg3; }
	inline int GetOrg4()		{ return iOrg4; }
	inline int GetOrg5()		{ return iOrg5; }
	inline char * GetName()		{ return Name; }
	inline short GetProperty()	{ return sProperty; }
	inline u_int GetMoney()		{ return uiMoney; }
	inline short GetState()		{ return sState; }

	BEGIN_COLUMN_MAP( CAllyReadAccessor )
		COLUMN_ENTRY( 1, iAllyIndex )
		COLUMN_ENTRY( 2, iOrg1 )
		COLUMN_ENTRY( 3, iOrg2 )
		COLUMN_ENTRY( 4, iOrg3 )
		COLUMN_ENTRY( 5, iOrg4 )
		COLUMN_ENTRY( 6, iOrg5 )
		COLUMN_ENTRY( 7, Name )
		COLUMN_ENTRY( 8, sProperty )
		COLUMN_ENTRY( 9, uiMoney )
		COLUMN_ENTRY( 10, sState )
	END_COLUMN_MAP()

protected:
	int			iAllyIndex;
	int			iOrg1;
	int			iOrg2;
	int			iOrg3;
	int			iOrg4;
	int			iOrg5;
	char		Name[en_or_name_length+1];
	short		sProperty;
	u_int		uiMoney;
	short		sState;
};


/*
CREATE PROCEDURE [dbo].[_prDS_AllyCreate] 
@org1		INT,			-- First Organization ( Hero Group ) Index : This is Master Group of Ally.
@org2		INT,			-- Second Organization
@orgName	VARCHAR(24),		-- Ally Name == First Organization's name
@baseProperty	SMALLINT,		-- Ally Property : Default(0), White(1), Black(-1)
@index 		INT	OUTPUT	-- Ally Index : Return Value.
*/
class CAllyCreateAccessor
{
public:
	inline void SetOrg1( int org )		{ iOrg1 = org; }
	inline void SetOrg2( int org )		{ iOrg2 = org; }
	inline void SetName( char * name )	{ strncpy( Name, name, en_or_name_length ); Name[en_or_name_length] = 0; }
	inline void SetProperty( short pr )	{ sProperty = pr; }
	inline int GetIndex()				{ return iIndex; }

	DEFINE_COMMAND_EX(CAllyCreateAccessor, L"{ CALL _prDS_AllyCreate(?,?,?,?,?) }" )
		BEGIN_PARAM_MAP(CAllyCreateAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, iOrg1 )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, iOrg2 )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 3, Name )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 4, sProperty )
			SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
			COLUMN_ENTRY( 5, iIndex )
		END_PARAM_MAP()

protected:
	
	int				iOrg1;
	int				iOrg2;
	char			Name[en_or_name_length+1];
	short			sProperty;
	int				iIndex;		// output
};


/*
CREATE PROCEDURE [dbo].[_prDS_AllyUpdate] 
@index		INT,			-- Target Ally's index
@org2		INT,			-- Second Organization
@org3		INT,			-- Third
@org4		INT,			-- 4th
@org5		INT,			-- 5th
@baseProperty	SMALLINT		-- Ally Property : Default(0), White(1), Black(-1)
*/
class CAllyUpdateAccessor
{
public:
	inline void SetIndex( int index )	{ iIndex = index; }
	inline void SetOrg2( int org )		{ iOrg2 = org; }
	inline void SetOrg3( int org )		{ iOrg3 = org; }
	inline void SetOrg4( int org )		{ iOrg4 = org; }
	inline void SetOrg5( int org )		{ iOrg5 = org; }
	inline void SetProperty( short pr )	{ sProperty = pr; }

	DEFINE_COMMAND_EX(CAllyUpdateAccessor, L"{ CALL _prDS_AllyUpdate(?,?,?,?,?,?) }" )
		BEGIN_PARAM_MAP(CAllyUpdateAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, iIndex )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, iOrg2 )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 3, iOrg3 )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 4, iOrg4 )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 5, iOrg5 )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 6, sProperty )
		END_PARAM_MAP()

protected:
	int				iIndex;
	int				iOrg2;
	int				iOrg3;
	int				iOrg4;
	int				iOrg5;
	short			sProperty;
};



/*
CREATE PROCEDURE [dbo].[_prDS_AllyBreakup]
@index		INT,
@reason	SMALLINT
*/
class CAllyBreakupAccessor
{
public:
	inline void SetIndex( int index )	{ iIndex = index; }
	inline void SetState( short state )	{ sState = state; }

	DEFINE_COMMAND_EX(CAllyBreakupAccessor, L"{ CALL _prDS_AllyBreakup(?,?) }" )
		BEGIN_PARAM_MAP(CAllyBreakupAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, iIndex )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, sState )
		END_PARAM_MAP()

protected:
	int				iIndex;
	short			sState;
};


class CAllyUpdateOfOrgAccessor
{
public:
	inline void SetIndex( int index )		{ iIndex = index; }
	inline void SetAllyIndex( int ally_index )	{ iAllyIndex = ally_index; }

	DEFINE_COMMAND_EX(CAllyUpdateOfOrgAccessor, L"{ CALL _prDS_UpdateAllyIndexOfOrg(?,?) }" )
		BEGIN_PARAM_MAP(CAllyUpdateOfOrgAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, iIndex )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, iAllyIndex )
		END_PARAM_MAP()

protected:
	int				iIndex;
	int				iAllyIndex;
};

#ifdef _PD_CASTLE_STEP_1_
class CAllyGetFirstOrgAccessor
{
public:
	inline void SetAllyIndex( int i )	{ iAllyIndex = i; iFirstOrgIndex = 0; }
	inline int GetFirstOrgIndex()		{ return iFirstOrgIndex; }

	DEFINE_COMMAND_EX(CAllyGetFirstOrgAccessor, L"{ CALL _prDS_AllyGetFirstOrg(?,?) }" )
		BEGIN_PARAM_MAP(CAllyGetFirstOrgAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, iAllyIndex );
			SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
			COLUMN_ENTRY( 2, iFirstOrgIndex );
		END_PARAM_MAP()	

protected:
	int				iAllyIndex;
	int				iFirstOrgIndex;
};
#endif

#endif

class CInvenAccessor
{
public:
	int		m_index ;
	char	m_name[13] ;
	BYTE	m_inven[_INVEN_SIZE_] ;
	short	m_select_weapon ;
	u_int	m_uiMoney;
	BYTE	cElixirData[_MED_CHECK_SIZE] ;
	short	sVersion;
	BYTE	cCollectionData[_COLLECTION_DATA_SIZE];		// char => byte 로. char 로 할 경우, 문자열 '0' 이 들어온다.
	short	sBloodPoint;

	BEGIN_COLUMN_MAP( CInvenAccessor )
		COLUMN_ENTRY( 1, m_index )
		COLUMN_ENTRY( 2, m_name )
		COLUMN_ENTRY( 3, m_inven )
		COLUMN_ENTRY( 4, m_select_weapon )
		COLUMN_ENTRY( 5, m_uiMoney )
		COLUMN_ENTRY( 6, cElixirData )
		COLUMN_ENTRY( 7, sVersion )
		COLUMN_ENTRY( 8, cCollectionData )
		COLUMN_ENTRY( 9, sBloodPoint )
	END_COLUMN_MAP()

};

class CDepotAccessor
{
public:		
	LONG m_unique_id;	
	BYTE m_depot[_DEPOT_BYTE_SIZE_];
	__int64 m_d_money;
	BYTE m_check_count ;
	SHORT m_check_year ;
	BYTE m_check_month ;
	BYTE m_check_day ;
	BYTE m_check_hour ;	
	short m_version ;

	BEGIN_COLUMN_MAP(CDepotAccessor)				
		COLUMN_ENTRY(1, m_unique_id)		
		COLUMN_ENTRY(2, m_depot)		
		COLUMN_ENTRY(3, m_d_money)
		COLUMN_ENTRY(4, m_check_count)		
		COLUMN_ENTRY(5, m_check_year)		
		COLUMN_ENTRY(6, m_check_month)		
		COLUMN_ENTRY(7, m_check_day)		
		COLUMN_ENTRY(8, m_check_hour)
		COLUMN_ENTRY(9, m_version)
	END_COLUMN_MAP()
};


class _prEvent_CheckAndGiveAccessor
{
public:
	short	m_sEventID;
	char	m_key[_EVENT_KEY_MAX_LENGTH_];
	char	m_account[50];
	char	m_chrName[en_charac_name_length+1];
	short	m_sGroupNo;
	int		m_idx;				// 인덱스.
	short	m_packageID;		// 보상 패키지 아이디.
	short	m_result;			// 결과 : Result 0 - Key invalid, 1 - Used, 2 - Success
	/************************************************************************/
	/*  
	CREATE PROCEDURE [dbo].[_prEvent_CheckAndGive] 
	@eid		smallint,	
	@skey		varchar(31),
	@acc		varchar(50),
	@cname		varchar(12),
	@group		smallint,
	@index		int				output,
	@packageID	smallint		output,
	@Result		smallint		output
	*/
	/************************************************************************/
	DEFINE_COMMAND_EX( _prEvent_CheckAndGiveAccessor, L"{ CALL _prEvent_CheckAndGive(?,?,?,?,?,?,?,?) }" ) 

		BEGIN_PARAM_MAP(_prEvent_CheckAndGiveAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_sEventID )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_key )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 3, m_account )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 4, m_chrName )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 5, m_sGroupNo )
			SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
			COLUMN_ENTRY( 6, m_idx )
			SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
			COLUMN_ENTRY( 7, m_packageID )
			SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
			COLUMN_ENTRY( 8, m_result )
		END_PARAM_MAP()
};


class _prEvent_PreCheck
{
public:
	short	m_sEventID;
	char	m_account[50];
	char	m_chrName[en_charac_name_length+1];
	short	m_sGroupNo;
	short	m_result;
	
	/************************************************************************/
	/*  
	CREATE PROCEDURE [dbo].[_prEvent_CheckAndGive] 
	@eid		smallint,	
	@skey		varchar(31),
	@acc		varchar(50),
	@cname		varchar(12),
	@group		smallint,
	@index		int				output,
	@packageID	smallint		output,
	@Result		smallint		output
	*/
	/************************************************************************/
	DEFINE_COMMAND_EX( _prEvent_PreCheck, L"{ CALL _prEvent_PreCheck(?,?,?,?,?) }" ) 

		BEGIN_PARAM_MAP(_prEvent_PreCheck)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_sEventID )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_account )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 3, m_chrName )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 4, m_sGroupNo )
			SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
			COLUMN_ENTRY( 5, m_result )
		END_PARAM_MAP()
};


class _prEvent_CancelAccessor
{
public:
	short	m_sEventID;
	int		m_idx;				// 인덱스.
	/************************************************************************/
	/*  
	CREATE PROCEDURE [dbo].[_prEvent_Cancel] 
	@eid		smallint,	
	@index		int
	*/
	/************************************************************************/
	DEFINE_COMMAND_EX( _prEvent_CancelAccessor, L"{ CALL _prEvent_Cancel(?,?) }" ) 

		BEGIN_PARAM_MAP(_prEvent_CancelAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_sEventID )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_idx )
		END_PARAM_MAP()
};

class _prEvent_SuccessAccessor
{
public:
	short	m_sEventID;
	int		m_idx;				// 인덱스.
	/************************************************************************/
	/*  
	CREATE PROCEDURE [dbo].[_prEvent_Cancel] 
	@eid		smallint,	
	@index		int
	*/
	/************************************************************************/
	DEFINE_COMMAND_EX( _prEvent_SuccessAccessor, L"{ CALL _prEvent_Success(?,?) }" ) 

		BEGIN_PARAM_MAP(_prEvent_SuccessAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_sEventID )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_idx )
		END_PARAM_MAP()
};


class C_prSetForestStateFlagAccessor
{
public :
	BYTE	m_flag[4] ;

	

	DEFINE_COMMAND_EX( C_prSetForestStateFlagAccessor, L"{ CALL _prSetForestStateFlag(?) }" )

	BEGIN_PARAM_MAP( C_prSetForestStateFlagAccessor )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_flag )
	END_PARAM_MAP()
};


class C_prGetForestStateFlagAccessor
{
public :
	BYTE	m_flag[4] ;

	
	DEFINE_COMMAND_EX( C_prGetForestStateFlagAccessor, L"{ CALL _prGetForestStateFlag(?) }" )

	BEGIN_PARAM_MAP( C_prGetForestStateFlagAccessor )
		SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 1, m_flag )
	END_PARAM_MAP()
};


class C_prQuestRankAccessor
{
public:
	u_short				m_questID ;
	BYTE				m_questRankData[sizeof(_QUEST_RANK_DATA)] ;
	short				m_flag ;	// 0 : select, 1, update | insert

	DEFINE_COMMAND_EX( C_prQuestRankAccessor, L"{ CALL _prQuestRank(?,?,?) }" )
	BEGIN_PARAM_MAP( C_prQuestRankAccessor )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_questID )
		SET_PARAM_TYPE( DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 2, m_questRankData ) 
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 3, m_flag )
	END_PARAM_MAP()
};



class CCharacState
{
public:
	u_long	m_uid ;	
	int		m_inner_level ;
	u_long	m_jin ;
	int		m_gong ;


	BEGIN_COLUMN_MAP( CCharacState )
		COLUMN_ENTRY( 1, m_uid )
		COLUMN_ENTRY( 2, m_inner_level )
		COLUMN_ENTRY( 3, m_jin )
		COLUMN_ENTRY( 4, m_gong )		
	END_COLUMN_MAP()

};


//////////////////////////////////////////////////////////////////////////
// Simple Info : _prDSU_GetUnionState
//   서버군의 캐릭터 이동상태를 얻는 함수.
//////////////////////////////////////////////////////////////////////////
class _db_pr_DSU_GetUnionState
{
public:
	inline int GetState()		{ return m_state ; }
	inline void SetUniqueID( int uid )	{ m_acc_uid = uid ; }

	DEFINE_COMMAND_EX( _db_pr_DSU_GetUnionState, L"{ CALL dbo._prDSU_GetUnionState(?,?) }" )

		BEGIN_PARAM_MAP(_db_pr_DSU_GetUnionState)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1,m_acc_uid)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(2,m_state)
		END_PARAM_MAP()

protected:
	INT			m_acc_uid ;
	INT			m_state ;
} ;

//////////////////////////////////////////////////////////////////////////
// Simple Info : _prDSU_GetUnionState
//   서버군의 캐릭터 이동상태를 얻는 함수.
//////////////////////////////////////////////////////////////////////////
class _db_pr_DSU_UpdateUnionState_One
{
public:
	inline int GetState()		{ return m_state ; }
	inline void SetUniqueID( int uid )	{ m_acc_uid = uid ; }
	inline void SetState( int state )	{ m_state = state ; }

	DEFINE_COMMAND_EX( _db_pr_DSU_UpdateUnionState_One, L"{ CALL dbo._prDSU_UpdateUnionState_One(?,?) }" )

		BEGIN_PARAM_MAP(_db_pr_DSU_UpdateUnionState_One)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1,m_acc_uid)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2,m_state)
		END_PARAM_MAP()

protected:
	INT			m_acc_uid ;
	INT			m_state ;
} ;


class _db_pr_DSU_UpdateUnionState_All
{
public:
	inline int GetState()		{ return m_state ; }
	inline void SetUniqueID( int uid )	
	{ 
		m_acc_uid = uid ; 
		m_group = 3 ;		// 무의미한 값 : 0,1,2 가 아니면 아무것도 안한다.
	}
	inline void SetState( int state )	{ m_state = state ; }
	inline void SetGroup( int group )	{ m_group = group ;	}


	DEFINE_COMMAND_EX( _db_pr_DSU_UpdateUnionState_All, L"{ CALL dbo._prDSU_UpdateUnionState_All(?,?,?) }" )

		BEGIN_PARAM_MAP(_db_pr_DSU_UpdateUnionState_All)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1,m_acc_uid)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2,m_state)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3,m_group)
		END_PARAM_MAP()

protected:
	INT			m_acc_uid ;
	INT			m_state ;
	INT			m_group ;
} ;


class _db_pr_DSU_InsertUnionLog
{
	// 0 - success, 1 - fail
public:
	inline void SetAccUID( int uid )		
	{ 
		m_acc_uid = uid ;
		ZeroMemory( m_acc, en_max_lil+1 ) ;
		m_code = 0 ;		// 0, 1, 2
		ZeroMemory( m_charac, 13 ) ;
		ZeroMemory( m_to_charac, 13 ) ;
		m_from_server = 0 ;
		m_result = 0 ;
	}

	inline void SetAccount( char * acc )
	{
		strncpy( m_acc, acc, en_max_lil ) ; 
	}
	inline void SetCode( short code )		{ m_code = code ; }
	inline void SetCharac( char * chr )		{ CopyMemory( m_charac, chr, 13 ) ; }
	inline void SetToCharac( char * chr )	{ CopyMemory( m_to_charac, chr, 13 ) ; }
	inline void SetFromServer( short svr )	{ m_from_server = svr ; }
	inline void SetResult( short result )	{ m_result = result ; }

	DEFINE_COMMAND_EX( _db_pr_DSU_InsertUnionLog, L"{ CALL dbo._prDSU_InsertUnionLog(?,?,?,?,?,?,?) }" )

		BEGIN_PARAM_MAP(_db_pr_DSU_InsertUnionLog)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1,m_acc_uid)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2,m_acc)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3,m_code)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4,m_charac)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(5,m_to_charac)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(6,m_from_server)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(7,m_result)

		END_PARAM_MAP()

protected:
	INT		m_acc_uid ;
	char	m_acc[en_max_lil+1] ;
	SHORT	m_code ;		// 0, 1, 2
	char	m_charac[13] ;
	char	m_to_charac[13] ;
	SHORT	m_from_server ;
	SHORT	m_result ;
} ;


//////////////////////////////////////////////////////////////////////////
//  2006/07/18 Ray. Get/Update Charac Item Use Status.
//////////////////////////////////////////////////////////////////////////
/*
CREATE PROCEDURE _prDS_GetCharacItemUseStatus
	@unique_id 	int ,			-- unique_id of charactor
	@shout_left_time	int OUTPUT,	-- left time of shouting
	@shout_type 		smallint OUTPUT -- type of shouting
*/
class _db_pr_DS_GetCharacItemUseStatus
{
public:
	inline void SetUniqueID( int id )			{ m_charac_unique_id = id ; }
	inline void SetShoutLeftTime( int time )	{ m_shout_left_time = time ; }
	inline void SetShoutType( short type )		{ m_shout_type = type ; }

	inline int GetShoutLeftTime()	{ return m_shout_left_time ; }
	inline short GetShoutType()		{ return m_shout_type ; }


	DEFINE_COMMAND_EX( _db_pr_DS_GetCharacItemUseStatus, L"{ CALL dbo._prDS_GetCharacItemUseStatus(?,?,?) }" )

		BEGIN_PARAM_MAP(_db_pr_DS_GetCharacItemUseStatus)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1,m_charac_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(2,m_shout_left_time)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3,m_shout_type)
		END_PARAM_MAP()

protected:
	INT		m_charac_unique_id ;
	INT		m_shout_left_time ;
	SHORT	m_shout_type ;
} ;
/*
CREATE PROCEDURE _prDS_UpdateCharacItemUseStatus
	@unique_id 	int ,			-- unique_id of charactor
	@shout_left_time	int,		-- left time of shouting
	@shout_type 		smallint  	-- type of shouting
*/
class _db_pr_DS_UpdateCharacItemUseStatus
{
public:
	inline void SetUniqueID( int id )			{ m_charac_unique_id = id ; }
	inline void SetShoutLeftTime( int time )	{ m_shout_left_time = time ; }
	inline void SetShoutType( short type )		{ m_shout_type = type ; }

	DEFINE_COMMAND_EX( _db_pr_DS_UpdateCharacItemUseStatus, L"{ CALL dbo._prDS_UpdateCharacItemUseStatus(?,?,?) }" )

		BEGIN_PARAM_MAP(_db_pr_DS_UpdateCharacItemUseStatus)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1,m_charac_unique_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2,m_shout_left_time)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3,m_shout_type)
		END_PARAM_MAP()

protected:
	INT		m_charac_unique_id ;
	INT		m_shout_left_time ;
	SHORT	m_shout_type ;
} ;

/*
CREATE PROCEDURE _prDS_GetAccountItemUseStatus 
		@acc_uid	int,	-- Account Unique ID, int
		@reserve	tinyint,	-- Not used
		@type		tinyint,	-- Item Type 
		@id		smallint,	-- Item ID
		@end_time	bigint	-- End Time : return value of time() function
*/

class _db_pr_DS_GetAccountItemUseStatus
{
public:
	inline void SetUniqueID( int aid )		
	{ 
		m_account_unique_id = aid ; 
		m_reserve = 0 ;
		m_item_type = 0 ;
		m_item_id = 0 ;
		m_end_time_sec = 0 ;
	}

	inline BYTE GetReserve()				{ return m_reserve ; }
	inline char GetItemType()				{ return static_cast<char>( m_item_type ) ; }
	inline short GetItemID()				{ return m_item_id ; }
	inline DWORD GetEndTime()				{ return m_end_time_sec ; }

	DEFINE_COMMAND_EX( _db_pr_DS_GetAccountItemUseStatus, L"{ CALL dbo._prDS_GetAccountItemUseStatus(?,?,?,?,?) }" )

	BEGIN_PARAM_MAP( _db_pr_DS_GetAccountItemUseStatus )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 1, m_account_unique_id )
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY( 2, m_reserve )
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY( 3, m_item_type )
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY( 4, m_item_id )
		SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
		COLUMN_ENTRY( 5, m_end_time_sec )
	END_PARAM_MAP()

protected:
	INT			m_account_unique_id ;
	BYTE		m_reserve ;
	BYTE		m_item_type ;
	SHORT		m_item_id ;
	DWORD		m_end_time_sec ;	// time 함수의 리턴 값.
} ;

/*
CREATE PROCEDURE _prDS_UpdateAccountItemUseStatus 
		@acc_uid	int,	-- Account Unique ID, int
		@reserve	tinyint,	-- Not used
		@type		tinyint,	-- Item Type 
		@id		smallint,	-- Item ID
		@end_time	bigint,	-- End Time : return value of time() function
		@flag		smallint,	-- date Flag : Flag 0 = no update date, 1 = update date.
		@date_string	varchar(16)	-- end_time_date update 
*/

class _db_pr_DS_UpdateAccountItemUseStatus
{
public:
	inline void SetUniqueID( int aid )		{ m_account_unique_id = aid ; }
	inline void SetReserve( BYTE res )		{ m_reserve = res ; }
	inline void SetItemType( char type )	{ m_item_type = static_cast<BYTE>( type ) ; }
	inline void SetItemID( short id )		{ m_item_id = id ; }
	inline void SetEndTime( DWORD time )	{ m_end_time_sec = time ; }
	inline void SetFlag( short flag )		{ m_flag = flag ; }
	inline void SetDateString( char * date_string )
	{
		CopyMemory( m_date_string, date_string, 16 ) ;
		m_date_string[16] = 0 ;
	}

	DEFINE_COMMAND_EX( _db_pr_DS_UpdateAccountItemUseStatus, L"{ CALL dbo._prDS_UpdateAccountItemUseStatus(?,?,?,?,?,?,?) }" )

	BEGIN_PARAM_MAP( _db_pr_DS_UpdateAccountItemUseStatus )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 1, m_account_unique_id )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 2, m_reserve )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 3, m_item_type )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 4, m_item_id )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 5, m_end_time_sec )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 6, m_flag )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 7, m_date_string )
		END_PARAM_MAP()

protected:
	INT			m_account_unique_id ;
	BYTE		m_reserve ;
	BYTE		m_item_type ;
	SHORT		m_item_id ;
	DWORD		m_end_time_sec ;	// time 함수의 리턴 값.
	SHORT		m_flag ;			// 0 - date_string no update, 1 - date_string update.
	char		m_date_string[17] ;
} ;


class _db_pr_DS_GetQuickSlotAccessor
{
public:
	inline void SetCharacUID( int cid )	{ m_charac_uid = cid; }
	inline void GetQuickSlot( void * p )	{ CopyMemory( p, &m_quickslot[0], sizeof(m_quickslot) ); }
	inline void GetHelpLog( void * p )		{ CopyMemory( p, &m_helplog[0], sizeof(m_helplog) ); }
	DEFINE_COMMAND_EX( _db_pr_DS_GetQuickSlotAccessor, L"{ CALL dbo._prDS_GetQuickSlot(?,?,?) }" )

	BEGIN_PARAM_MAP( _db_pr_DS_GetQuickSlotAccessor )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(1, m_charac_uid )
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(2,m_quickslot)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3,m_helplog)
		END_PARAM_MAP()

protected:
	INT			m_charac_uid;
	BYTE		m_quickslot[sizeof(__client_quickslot)*_QUICKSLOT_COUNT_];
	BYTE		m_helplog[_HELPLOG_MAX_SIZE_];
};

class _db_pr_DS_UpdateQuickSlotAccessor
{
public:
	inline void SetCharacUID( int cid )	{ m_charac_uid = cid ; }
	inline void SetQuickSlot( void * p )	{ CopyMemory( &m_quickslot[0], p, sizeof(m_quickslot) ) ; }
	inline void SetHelplog( void * p )		{ CopyMemory( &m_helplog[0], p, sizeof(m_helplog) ); }

	DEFINE_COMMAND_EX( _db_pr_DS_UpdateQuickSlotAccessor, L"{ CALL dbo._prDS_UpdateQuickSlot(?,?,?) }" )

		BEGIN_PARAM_MAP( _db_pr_DS_UpdateQuickSlotAccessor )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_charac_uid )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2,m_quickslot)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3,m_helplog)
		END_PARAM_MAP()

protected:
	INT			m_charac_uid;
	BYTE		m_quickslot[sizeof(__client_quickslot)*_QUICKSLOT_COUNT_];
	BYTE		m_helplog[_HELPLOG_MAX_SIZE_];
};


#ifdef _PD_CHARAC_LTS_
//////////////////////////////////////////////////////////////////////////
// LTS Accessor
//////////////////////////////////////////////////////////////////////////
class _db_pr_DS_Get_LTSAccessor
{
public:
	inline void set_charac_uid( int uid )
	{
		m_iResult = 0;
		m_charac_uid = uid;
		ZeroMemory( m_lvpkList, sizeof(m_lvpkList) );
		ZeroMemory( m_pkList, sizeof(m_pkList) );
		ZeroMemory( m_spList, sizeof(m_spList) );
	}
	inline void get_lts( _CHARAC_LTS * pLTS )
	{
		CopyMemory( pLTS->m_pLvPkData, m_lvpkList, sizeof(m_lvpkList) );
		CopyMemory( pLTS->m_pPkData, m_pkList, sizeof(m_pkList) );
		CopyMemory( pLTS->m_pSpData, m_spList, sizeof(m_spList) );
	}
	inline int get_result()	{ return m_iResult; }

	DEFINE_COMMAND_EX( _db_pr_DS_Get_LTSAccessor, L"{ ? = CALL dbo._prDS_GetLts(?,?,?,?) }" )
		BEGIN_PARAM_MAP(_db_pr_DS_Get_LTSAccessor)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(1, m_iResult)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_charac_uid)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_lvpkList)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(4, m_pkList)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_spList)
		END_PARAM_MAP()

protected:
	INT			m_iResult;				// 리턴 값. 기본 0, 실패시 1.
	INT			m_charac_uid;
	BYTE		m_lvpkList[720];		// Lv 차 PK 범.
	BYTE		m_pkList[240];			// PK 범 리스트
	BYTE		m_spList[24];			// 특정인 리스트
};


class _db_pr_DS_Set_LTSAccessor
{
public:
	inline void set_list( int uid, _CHARAC_LTS * pLTS )
	{
		m_charac_uid = uid;
		CopyMemory( m_lvpkList, pLTS->m_pLvPkData, sizeof(m_lvpkList) );
		CopyMemory( m_pkList, pLTS->m_pPkData, sizeof(m_pkList) );
		CopyMemory( m_spList, pLTS->m_pSpData, sizeof(m_spList) );
	}

	DEFINE_COMMAND_EX( _db_pr_DS_Set_LTSAccessor, L"{ CALL dbo._prDS_SetLts(?,?,?,?) }" )
		BEGIN_PARAM_MAP(_db_pr_DS_Set_LTSAccessor)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_charac_uid)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_lvpkList)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_pkList)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_spList)
		END_PARAM_MAP()

protected:
	INT			m_charac_uid;
	BYTE		m_lvpkList[720];		// Lv 차 PK 범.
	BYTE		m_pkList[240];			// PK 범 리스트
	BYTE		m_spList[24];			// 특정인 리스트
};

class _db_pr_DS_GetCUIDAccessor
{
public:
	inline void set_charac_name( char * name )
	{
		CopyMemory( m_chrName, name, en_charac_name_length );
		m_chrName[en_charac_name_length] = 0;
		m_cuid = 0;
	}

	inline int get_charac_uid()	{ return m_cuid; }

	DEFINE_COMMAND_EX( _db_pr_DS_GetCUIDAccessor, L"{CALL dbo._prDS_GetCUID(?,?) }" )
		BEGIN_PARAM_MAP(_db_pr_DS_GetCUIDAccessor)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_chrName)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(2, m_cuid)
		END_PARAM_MAP()

protected:
	char		m_chrName[en_charac_name_length+1];
	INT			m_cuid;
};

#endif

#ifdef ITEMSHOP_PATCH_200607
class C_prTestAccessor {
public:
	int iVar1;
	int iVar2;
	int iRet;

	DEFINE_COMMAND_EX( C_prTestAccessor, L"{ ? = CALL WebItemShop.dbo.sp_Test(?,?) }" )
	BEGIN_PARAM_MAP( C_prTestAccessor )
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, iRet)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, iVar1)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, iVar2)
	END_PARAM_MAP()
};

class CTestAccessor2 {
public:
	int iIdx;
	int iColumn1;
	int iColumn2;

	BEGIN_COLUMN_MAP(CTestAccessor2)
		COLUMN_ENTRY(1, iIdx)
		COLUMN_ENTRY(2, iColumn1)
		COLUMN_ENTRY(3, iColumn2)
	END_COLUMN_MAP()
};
#endif

/*
#ifdef _CIS_20070326_
class C_prGetCashItemListAccessor {
public:
	BYTE	byPageSize;				// 페이지 당 자료수
	BYTE	bySetPage;				// 페이지 번호
	SHORT	shGameServerIdx;		// 게임서버 인덱스
	char	szAcc[en_max_lil+1];	// 유저 계정
	BYTE	byPresent;				// 선물여부
	INT		iTotalRec;				// 총 반환 열 개수

	DEFINE_COMMAND_EX( C_prGetCashItemListAccessor, L"{ CALL Item_mall_global.dbo.isp_get_cash_item_list(?,?,?,?,?,?) }" )
	BEGIN_PARAM_MAP( C_prGetCashItemListAccessor )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(1, byPageSize)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, bySetPage)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, shGameServerIdx)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, szAcc)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(5, byPresent)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, iTotalRec)
	END_PARAM_MAP()
};
#endif
*/

class _db_pr_DS_AddPlayTime
{
public:
	inline void SetUniqueID( int aid )		{ m_account_unique_id = aid ; }
	inline void SetAccountName( char * acc ){ strncpy( m_account_name, acc, en_max_lil+1 ); m_account_name[en_max_lil] = 0; }
	inline void SetTime( int time )			{ m_time = time; }
	inline void SetGroup( short gn )		{ m_group = gn; }
	
	DEFINE_COMMAND_EX( _db_pr_DS_AddPlayTime, L"{ CALL dbo._prDS_AddPlayTime(?,?,?,?) }" )

		BEGIN_PARAM_MAP( _db_pr_DS_AddPlayTime )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_account_unique_id )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_account_name )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 3, m_time )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 4, m_group )
		END_PARAM_MAP()

protected:
	int			m_account_unique_id ;
	char		m_account_name[en_max_lil+1];
	int			m_time;
	short		m_group;
} ;

#ifdef _VINA_SHUTDOWN_20061023_
//extern int		g_iShutdownHour;
class _db_pr_DS_GetFatigueInfoAccessor
{ 
public:
	inline void SetUniqueID( int uid )		{ m_account_unique_id = uid ; }
	inline short GetFatigueWeight()			{ return m_sFatigueWeight; }
	inline int	GetLogoutTime()				
	{
		//int iSetupHour = g_iShutdownHour;
		int iLogoutHour = (m_LogoutTime.year-1)*365*24 + (m_LogoutTime.month-1)*31*24 + (m_LogoutTime.day-1)*24 + m_LogoutTime.hour;
		Logout("LogoutTime=%d/%d/%d/%d iLogoutHour=%d sFatigueWeight=%d",
			m_LogoutTime.year,
			m_LogoutTime.month,
			m_LogoutTime.day,
			m_LogoutTime.hour,
			iLogoutHour,
			m_sFatigueWeight);
		return iLogoutHour;

		/*
		int iLogoutTime = 0 ;
		iLogoutTime = (m_LogoutTime.year - 1) * 365 + (m_LogoutTime.month - 1) * 31 + m_LogoutTime.day ;
		return iLogoutTime ;
		*/
	}
	
	DEFINE_COMMAND_EX( _db_pr_DS_GetFatigueInfoAccessor, L"{ CALL dbo._prDS_GetFatigueInfo(?,?,?) }" )

	BEGIN_PARAM_MAP( _db_pr_DS_GetFatigueInfoAccessor )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_account_unique_id )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(2, m_sFatigueWeight)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(3, m_LogoutTime)
	END_PARAM_MAP()
protected:
	int					m_account_unique_id ;
	short				m_sFatigueWeight ; //피로도
	DBTIMESTAMP			m_LogoutTime ;	//로그아웃시간
} ;

class _db_pr_DS_UpdateFatigueInfoAccessor
{
public:
	inline void SetUniqueID( int uid )		{ m_account_unique_id = uid ; }
	inline void	UpdateFatigueWeight( short sFatigueWeight )			{m_sFatigueWeight = sFatigueWeight; }
	inline short GetFatigueWeight()			{return m_sFatigueWeight; }
	/*inline void UpdateLogoutTime()
	{	
		SYSTEMTIME	LogoutTime ;
		GetLocalTime(&LogoutTime) ;
		
		m_LogoutTime = LogoutTime ;
		//iCurTime = (curTime.wYear - 1) * 365 + (curTime.wMonth - 1) * 31 + curTime.wDay ;
	}*/
	
	DEFINE_COMMAND_EX( _db_pr_DS_UpdateFatigueInfoAccessor, L"{ CALL dbo._prDS_UpdateFatigueInfo(?,?) }" )

	BEGIN_PARAM_MAP( _db_pr_DS_UpdateFatigueInfoAccessor )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_account_unique_id )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_sFatigueWeight)
	END_PARAM_MAP()
protected:
	int					m_account_unique_id ;
	short				m_sFatigueWeight ; //피로도
	//SYSTEMTIME			m_LogoutTime2 ;	//로그아웃시간
	//DBTIMESTAMP			m_LogoutTime ;	//로그아웃시간
};
#endif

#ifdef _PD_CASTLE_STEP_1_
/*
manor system 수성
*/
class _cSelectDefendCastleAccessor
{
public:
	inline int GetDefendCastleUniqueId()		{return m_iDefendCastleUniqueId;}
	inline int GetOwenerUniqueId()				{return m_iOwenerUniqueId;}
	inline char* GetOwenerName()				{return m_szOwenerName;}
	inline char* GetAllyOrgName()				{return m_szAllyOrgName;}
	inline int GetOwnerShipFirstDate()			
	{
		char szOwnerShipFirstDate[16];
		sprintf( szOwnerShipFirstDate, "%04d%02d%02d%02d", m_OwnerShipFirstDate.year, m_OwnerShipFirstDate.month, m_OwnerShipFirstDate.day, m_OwnerShipFirstDate.hour);
		return atoi(szOwnerShipFirstDate);
	}
	inline int GetDefendSuccessNum()			{return m_iDefendSuccessNum;}
	inline int GetMatchCount()					{return m_iDefendMatchNum;}
	inline int GetManorLevel()					{return m_iManorLevel;}
	inline int GetManorPartMoney()				{return m_iManorPartMoney;}
	inline int GetManorTotalMoney()				{return m_iManorTotalMoney;}
	inline int GetFirstCombatDate()				
	{
		char szFirstCombatDate[20];
		sprintf( szFirstCombatDate, "%04d%02d%02d%02d", m_FirstCombatDate.year, m_FirstCombatDate.month, m_FirstCombatDate.day, m_FirstCombatDate.hour);
		return atoi(szFirstCombatDate);
	}
	inline int GetSecondCombatDate()			
	{
		char szSecondCombatDate[20];
		sprintf( szSecondCombatDate, "%04d%02d%02d%02d", m_SecondCombatDate.year, m_SecondCombatDate.month, m_SecondCombatDate.day, m_SecondCombatDate.hour);
		return atoi(szSecondCombatDate);
	}
	inline int GetThirdCombatDate()				
	{
		char szThirdCombatDate[20];
		sprintf( szThirdCombatDate, "%04d%02d%02d%02d", m_ThirdCombatDate.year, m_ThirdCombatDate.month, m_ThirdCombatDate.day, m_ThirdCombatDate.hour);
		return atoi(szThirdCombatDate);
	}
	inline char* GetManorUniqueId()					{return m_szManorUniqueId;}
	inline char* GetDanType()						{return m_szDanType;}

	BEGIN_COLUMN_MAP( _cSelectDefendCastleAccessor )
		COLUMN_ENTRY( 1, m_iDefendCastleUniqueId )
		COLUMN_ENTRY( 2, m_iOwenerUniqueId )
		COLUMN_ENTRY( 3, m_szOwenerName )
		COLUMN_ENTRY( 4, m_szAllyOrgName )
		COLUMN_ENTRY( 5, m_OwnerShipFirstDate )
		COLUMN_ENTRY( 6, m_iDefendSuccessNum )
		COLUMN_ENTRY( 7, m_iDefendMatchNum )
		COLUMN_ENTRY( 8, m_iManorLevel )
		COLUMN_ENTRY( 9, m_iManorPartMoney )
		COLUMN_ENTRY( 10, m_iManorTotalMoney )
		COLUMN_ENTRY( 11, m_FirstCombatDate )
		COLUMN_ENTRY( 12, m_SecondCombatDate )
		COLUMN_ENTRY( 13, m_ThirdCombatDate )
		COLUMN_ENTRY( 14, m_szManorUniqueId )
		COLUMN_ENTRY( 15, m_szDanType )
	END_COLUMN_MAP()

protected:
	int					m_iDefendCastleUniqueId;		//수성 번호
	int					m_iOwenerUniqueId;				//소유자  uid
	char				m_szOwenerName[en_charac_name_length+1];				//소유자 캐릭 명
	char				m_szAllyOrgName[32+1];				//소유자 영웅단명
	DBTIMESTAMP			m_OwnerShipFirstDate;			//소유시작일자
	int					m_iDefendSuccessNum;			//소유 성공 횟수?
	int					m_iDefendMatchNum;
	int					m_iManorLevel;					//장원 레벨
	int					m_iManorPartMoney;				//장원 보유금액
	int					m_iManorTotalMoney;				//장원 누적금액
	DBTIMESTAMP			m_FirstCombatDate;
	DBTIMESTAMP			m_SecondCombatDate;
	DBTIMESTAMP			m_ThirdCombatDate;
	char				m_szManorUniqueId[2];				//장원 코드
	char				m_szDanType[2];				//단회 코드
};

	//int				m_iAllyOrgTotalNumber;			//신청단 인원수

/*
manor system 공성
*/
class _cSelectAttackCastleAccessor
{
public:
	inline int GetAttackCastleUniqueId()			{return m_iAttackCastleUniqueId;}
	inline int GetChallengerUniqueId()				{return m_iChallengerUniqueId;}
	inline char* GetChallengerName()				{return m_szChallengerName;}
	inline char* GetAllyOrgName()					{return m_szAllyOrgName;}
	inline char* GetAtkConfirm()					{return m_szAtkConfirm;}
	inline char* GetManorUniqueId()					{return m_szManorUniqueId;}
	inline char* GetDanType()						{return m_szDanType;}

	BEGIN_COLUMN_MAP( _cSelectAttackCastleAccessor )
		COLUMN_ENTRY( 1, m_iAttackCastleUniqueId )
		COLUMN_ENTRY( 2, m_iChallengerUniqueId )
		COLUMN_ENTRY( 3, m_szChallengerName )
		COLUMN_ENTRY( 4, m_szAllyOrgName )
		COLUMN_ENTRY( 5, m_szAtkConfirm )
		COLUMN_ENTRY( 6, m_szManorUniqueId )
		COLUMN_ENTRY( 7, m_szDanType )
	END_COLUMN_MAP()

protected:
	int					m_iAttackCastleUniqueId;		//공성번호
	int					m_iChallengerUniqueId;			//도전자  uid(캐릭)
	char				m_szChallengerName[en_charac_name_length+1];			//공성자 캐릭 명
	char				m_szAllyOrgName[32+1];				//공성 신청단 명
	char				m_szAtkConfirm[2];			//공성 신청 여부 1)true(default) 0) false;
	char				m_szManorUniqueId[3];			//장원 코드
	char				m_szDanType[3];					//단회 코드	
};

/*
수성_공성
*/
class _cSelectManorCombatAccessor
{
public:
	inline int GetDefendCastleUniqueId()		{return m_iDefendCastleUniqueId;}
	inline int GetAttackCastleUniqueId()		{return m_iAttackCastleUniqueId;}
	inline int GetChallengerNumber()				{return m_iChallengerNumber;}
	inline char* GetDefendCastleResult()			{return m_szDefendCastleResult;}
	inline char* GetAttackCastleResult()			{return m_szAttackCastleResult;}

	BEGIN_COLUMN_MAP( _cSelectManorCombatAccessor )
		COLUMN_ENTRY( 1, m_iDefendCastleUniqueId )
		COLUMN_ENTRY( 2, m_iAttackCastleUniqueId )
		COLUMN_ENTRY( 3, m_iChallengerNumber )
		COLUMN_ENTRY( 4, m_szDefendCastleResult )
		COLUMN_ENTRY( 5, m_szAttackCastleResult )
	END_COLUMN_MAP()

protected:
	int					m_iDefendCastleUniqueId;		//수성 번호
	int					m_iAttackCastleUniqueId;		//공성 번호
	int					m_iChallengerNumber;				//도전 차수
	char				m_szDefendCastleResult[2];				//승패 여부 ( 0)None, 1)도전자 승 2)도전자 패
	char				m_szAttackCastleResult[2];
	//DBTIMESTAMP			m_ApplyDate;					//등록 일자
	
};

/*
수성 정보 업데이트
*/
class _cUpdateDefendCastleAccessor
{
public:
	inline void SetOwnerUniqueId(int iOwnerUniqueId)			{ m_iOwnerUniqueId = iOwnerUniqueId;}
	inline void SetOwnerName(char* szOwnerName)							
	{ 
		strncpy(m_szOwnerName, szOwnerName, en_charac_name_length);
		m_szOwnerName[en_charac_name_length] = 0;
	}
	inline void SetAllyOrgName(char* szAllyOrgName)								
	{ 
		strncpy(m_szAllyOrgName, szAllyOrgName, 32);
		m_szAllyOrgName[32] = 0;
	}
	inline void SetOwnerShipFirstDate( _sND_TimeUnit * pTime )
	{
		sprintf(m_szOwnerShipFirstDate, "%04d-%02d-%02d %02d:00:00", 
			pTime->uiYear+2000, pTime->uiMonth, pTime->uiDay, pTime->uiHour );
	}
	inline void SetDefendSuccessNum( u_char ucDefendSuccessNum)				{ m_ucDefendSuccessNum = ucDefendSuccessNum; }
	inline void SetDefendMatchNum( u_char ucDefendMatchNum )				{ m_ucDefendChallengeNum = ucDefendMatchNum; }
	inline void SetManorLevel( u_char ucManorLevel)							{ m_ucManorLevel = ucManorLevel;}
	inline void SetManorPartMoney(int iManorPartMoney)						{ m_iManorPartMoney = iManorPartMoney;}
	inline void SetManorTotalMoney(int iManorTotalMoney)					{ m_iManorTotalMoney = iManorTotalMoney;}

	inline void SetFirstCombatDate( _sND_TimeUnit * pTime )	
	{ 
		sprintf(m_szFirstCombatDate, "%04u-%02u-%02u %02u:00:00", 
			pTime->uiYear+2000, pTime->uiMonth, pTime->uiDay, pTime->uiHour );
	}
	inline void SetSecondCombatDate( _sND_TimeUnit * pTime )
	{
		sprintf(m_szSecondCombatDate, "%04u-%02u-%02u %02u:00:00", 
			pTime->uiYear+2000, pTime->uiMonth, pTime->uiDay, pTime->uiHour );
	}
	inline void SetThirdCombatDate( _sND_TimeUnit * pTime )
	{ 
		sprintf(m_szThirdCombatDate, "%04u-%02u-%02u %02u:00:00", 
			pTime->uiYear+2000, pTime->uiMonth, pTime->uiDay, pTime->uiHour );
	}
	inline void SetManorCode( short map_id )
	{ 
		sprintf( m_szManorCode, "%02d", map_id );
		m_szManorCode[2] = 0;
	}
	inline void SetTypeCode( u_char  type)
	{ 
		switch( type )
		{
		case 0:		m_szTypeCode[0] = '0';		break;
		case 1:		m_szTypeCode[0] = '1';		break;
		default:	m_szTypeCode[0] = '2';
		}
		
		m_szTypeCode[1] = 0;
	}
	inline void SetFlag(short sFlag)		{ m_sFlag = sFlag; }
	inline void SetDfdNum( int dfd )		{ m_iDfdNumber = dfd; }
	inline int GetDfdNum()					{ return m_iDfdNumber; }

	DEFINE_COMMAND_EX(_cUpdateDefendCastleAccessor, L"{ CALL _prDS_UpdateDefendCastle(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) }" )
		BEGIN_PARAM_MAP(_cUpdateDefendCastleAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_iOwnerUniqueId )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_szOwnerName )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 3, m_szAllyOrgName )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 4, m_szOwnerShipFirstDate )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 5, m_ucDefendSuccessNum )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 6, m_ucDefendChallengeNum )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 7, m_ucManorLevel )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 8, m_iManorPartMoney )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 9, m_iManorTotalMoney )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 10, m_szFirstCombatDate )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 11, m_szSecondCombatDate )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 12, m_szThirdCombatDate )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 13, m_szManorCode )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 14, m_szTypeCode )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 15, m_sFlag )
			SET_PARAM_TYPE( DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
			COLUMN_ENTRY( 16, m_iDfdNumber )
		END_PARAM_MAP()

protected:
	int					m_iOwnerUniqueId;					//소유자  uid
	char				m_szOwnerName[en_charac_name_length+1];			//소유자 캐릭 명
	char				m_szAllyOrgName[32+1];				//소유자 영웅단명

	char				m_szOwnerShipFirstDate[20];			//소유시작일자

	u_char				m_ucDefendSuccessNum;				//소유 성공 횟수?
	u_char				m_ucDefendChallengeNum;				// 도전 수???

	u_char				m_ucManorLevel;						//장원 레벨

	int					m_iManorPartMoney;					//장원 보유금액
	int					m_iManorTotalMoney;					//장원 누적금액

	char				m_szFirstCombatDate[20];
	char				m_szSecondCombatDate[20];
	char				m_szThirdCombatDate[20];

	char				m_szManorCode[2+1];					//장원 코드

	char				m_szTypeCode[1+1];					//타입 코드

	short				m_sFlag;							//0)insert, 1)update
	int					m_iDfdNumber;						// Sequence Number.
};

/*
공성 정보 업데이트
*/
class _cUpdateAttackCastleAccessor
{
public:
	inline void SetChallengerUniqueId(int iChallengerUniqueId)	
	{ m_iChallengerUniqueId = iChallengerUniqueId;}
	
	inline void SetDBSequence( int db_seq )	{ m_iDB_Seq = db_seq; }
	inline int GetDBSequence()	{ return m_iDB_Seq; }

	inline void SetFlag_Insert()		{ m_sFlag = 0; }
	inline void SetFlag_Update()		{ m_sFlag = 1; }
	
	inline void SetChallengerName(char* szChallengerName)							
	{ 
		strncpy(m_szChallengerName, szChallengerName, en_charac_name_length);
		m_szChallengerName[en_charac_name_length] = 0;
	}
	inline void SetAllyOrgName(char* szAllyOrgName)							
	{ 
		strncpy(m_szAllyOrgName, szAllyOrgName, 32+1);
		m_szAllyOrgName[32] = 0;
	}
	inline void SetApplyTrueOrFalse( int result )
	{ 
		// 0 : 취소, 1 : 신청, 2 : 도전완료되었음.
		// Delete 일 때는 이값과, Flag, db_seq 값만 사용 : 나머지 값은 셋팅 필요 없음.
		m_szApplyTrueOrFalse[0] = '0' + result ;
		m_szApplyTrueOrFalse[1] = 0;
	}
	inline void SetManorUniqueId( int map_id )
	{ 
		sprintf( m_szManorUniqueId, "%02d", map_id );
		m_szManorUniqueId[2] = 0;
	}
	inline void SetAllyOrgUniqueId( u_char group_type )					
	{ 
		m_szAllyOrgUniqueId[0] = '0' + group_type;
		m_szAllyOrgUniqueId[1] = 0;
	}

	DEFINE_COMMAND_EX(_cUpdateAttackCastleAccessor, L"{ CALL _prDS_UpdateAttackCastle(?,?,?,?,?,?,?,?) }" )
		BEGIN_PARAM_MAP(_cUpdateAttackCastleAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_iChallengerUniqueId )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_szChallengerName )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 3, m_szAllyOrgName )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 4, m_szApplyTrueOrFalse )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 5, m_szManorUniqueId )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 6, m_szAllyOrgUniqueId )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 7, m_sFlag )
			SET_PARAM_TYPE( DBPARAMIO_INPUT | DBPARAMIO_OUTPUT )
			COLUMN_ENTRY( 8, m_iDB_Seq )
		END_PARAM_MAP()

protected:
	int					m_iChallengerUniqueId;			//도전자  uid(캐릭)
	int					m_iDB_Seq;
	short				m_sFlag; //0)insert 1)update
	char				m_szChallengerName[13];			//공성자 캐릭 명
	char				m_szAllyOrgName[33];				//공성 신청단 명
	char				m_szApplyTrueOrFalse[2];			//공성 신청 여부 1)true(default) 0) false;
	char				m_szManorUniqueId[3];				//장원 코드
	char				m_szAllyOrgUniqueId[2];				//단회 코드	
};

/*
수성_공성
*/
class _cUpdateManorCombatAccessor
{
public:
	inline void SetOwenerUniqueId(int iOwenerUniqueId)		{ m_iOwenerUniqueId = iOwenerUniqueId;}
	inline void SetChallengerUniqueId(int iChallengerUniqueId)		{ m_iChallengerUniqueId = iChallengerUniqueId;}
	inline void SetCallengerNumber(int iCallengerNumber)				{ m_iCallengerNumber = iCallengerNumber;}
	inline void SetDefendCastleResult(char* szDefendCastleResult)		
	{ 
		strncpy(m_szDefendCastleResult, szDefendCastleResult, 1);
	}
	inline void SetAttackCastleResult(char* szAttackCastleResult)		
	{ 
		strncpy(m_szAttackCastleResult, szAttackCastleResult, 1);
	}
	inline void SetManorUniqueId(char* szManorUniqueId)
	{
		strncpy(m_szManorUniqueId, szManorUniqueId, 2);
	}
	inline void SetFlag(short sFlag)			{ m_sFlag = sFlag; }
	/*inline void SetApplyDate(m_ApplyDate)					
	{
		char szApplyDate[16];
		sprintf( szApplyDate, "%04d%02d%02d%02d", m_ApplyDate.year, m_ApplyDate.month, m_ApplyDate.day, m_ApplyDate.hour);
		return atoi(szApplyDate);
	}*/

	DEFINE_COMMAND_EX(_cUpdateManorCombatAccessor, L"{ CALL _prDS_UpdateBattle(?,?,?,?,?,?,?) }" )
		BEGIN_PARAM_MAP(_cUpdateManorCombatAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_iOwenerUniqueId )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_iChallengerUniqueId )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 3, m_iCallengerNumber )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 4, m_szDefendCastleResult )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 5, m_szAttackCastleResult )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 6, m_szManorUniqueId )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 7, m_sFlag )
		END_PARAM_MAP()

protected:
	int					m_iOwenerUniqueId;		//수성자 번호
	int					m_iChallengerUniqueId;		//공성자 번호
	int					m_iCallengerNumber;				//도전 차수
	char				m_szDefendCastleResult[1];				//승패 여부 ( 0)None, 1)도전자 승 2)도전자 패
	char				m_szAttackCastleResult[1];				//승패 여부 ( 0)None, 1)도전자 승 2)도전자 패
	char				m_szManorUniqueId[2];
	short				m_sFlag;
	//DBTIMESTAMP			m_ApplyDate;					//등록 일자	
};

class _cCastleInsertAccessor
{
public:
	void SetUniqueID( short map_id )
	{
		ZeroMemory( m_szManorUniqueID, sizeof(m_szManorUniqueID) );
		ZeroMemory( m_szManorName, sizeof(m_szManorName) );
		m_iResult = 0;

		sprintf( m_szManorUniqueID, "%02d", map_id );
		sprintf( m_szManorName, "%02d", map_id );
	}
	int GetResult()	{ return m_iResult; }

	DEFINE_COMMAND_EX(_cCastleInsertAccessor, L"{ CALL _prDS_CastleInsert(?,?,?) }" )
		BEGIN_PARAM_MAP(_cCastleInsertAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_szManorUniqueID )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_szManorName )
			SET_PARAM_TYPE( DBPARAMIO_OUTPUT )
			COLUMN_ENTRY( 3, m_iResult )
		END_PARAM_MAP()

protected:
	char				m_szManorUniqueID[2+1];
	char				m_szManorName[12+1];
	int					m_iResult;
};


/*
수성_공성
*/
class _cSelectDefendDespotAccessor
{
public:
	inline char* GetManorUniqueId()				{ return m_szManorUniqueId;}
	inline int GetDefendCastleUniqueId()		{ return m_iDefendCastleUniqueId; }
	
	BEGIN_COLUMN_MAP( _cSelectDefendDespotAccessor )
		COLUMN_ENTRY( 1, m_szManorUniqueId )
		COLUMN_ENTRY( 2, m_iDefendCastleUniqueId )
	END_COLUMN_MAP()

protected:
	char				m_szManorUniqueId[2+1];
	int					m_iDefendCastleUniqueId;				//수성번호
};
#endif
#ifdef _PD_CASTLE_STEP_2_
/// \class	_cGetAllCombatRecord
/// \brief	ND_V01_Dan_standings_bi 테이블에 있는 내용 
class _cSelectAllCombatRecord
{
public:
	inline	int		GetMyIndex(){return m_dan_index;}
	inline	BYTE*	GetCombatRecomd(){return m_combatRecord;}
	inline	BYTE	GetIsValid(){return m_valid;}
	inline	char*	GetManorCode(){return m_manor_code;}
	inline	int		GetRecordCount(){return m_record_count;}
public:
	BEGIN_COLUMN_MAP( _cSelectAllCombatRecord )
		COLUMN_ENTRY(1, m_record_no )
		COLUMN_ENTRY(2, m_dan_index )
		COLUMN_ENTRY(3, m_combatRecord )
		COLUMN_ENTRY(4, m_valid )
		COLUMN_ENTRY(5, m_manor_code )
		COLUMN_ENTRY(6, m_record_count )
	END_COLUMN_MAP()
protected:
	int		m_record_no;
	int		m_dan_index;			// 자기 단
	BYTE	m_combatRecord[4000];	//타단인덱스,승,패,타단이름	
	BYTE	m_valid;				// 단이 유효한지
	char	m_manor_code[2+1];		// 장원전 번호
	int		m_record_count;			//바이너리에 있는 타단 카운트
};

/// \class	_cSelectCombatRecordInManor
/// \brief	ND_V01_ManorWarRecord 로드 하기 위해
class _cSelectCombatRecordInManor
{
public:
	inline	int		GetMyDanIndex()	{return m_dan_index;}
	inline	char*	GetManorCode()	{return m_manor_code;}
	inline	int		GetCombatCount(){return m_combat_count;}
	inline	int		GetTotalCount()	{return m_total_count;}
	inline	int		GetWinCount()	{return m_win_count;}
	inline	int		GetLoseCount()	{return m_lose_count;}
	inline DBTIMESTAMP GetCombatDateDBTime() { return m_combat_date; }
	inline  int		GetCombatDate()			
	{
		char szSecondCombatDate[20];
		sprintf( szSecondCombatDate, "%04d%02d%02d%02d", m_combat_date.year, m_combat_date.month, m_combat_date.day, m_combat_date.hour);
		return atoi(szSecondCombatDate);
	}
	



public:
	BEGIN_COLUMN_MAP( _cSelectCombatRecordInManor )
		COLUMN_ENTRY(1, m_record_no )
		COLUMN_ENTRY(2, m_dan_index )
		COLUMN_ENTRY(3, m_manor_code )
		COLUMN_ENTRY(4, m_combat_count )
		COLUMN_ENTRY(5, m_total_count )
		COLUMN_ENTRY(6, m_win_count )
		COLUMN_ENTRY(7, m_lose_count )
		COLUMN_ENTRY(8, m_combat_date )
	END_COLUMN_MAP()
protected:
	int		m_record_no;
	int		m_dan_index;		// 자기 단
	char	m_manor_code[2+1];	// 장원전 번호
	int		m_combat_count;		//참전횟수
	int		m_total_count;		// winCount+lostCount
	int		m_win_count;
	int		m_lose_count;
	DBTIMESTAMP	m_combat_date;   // 마지막 싸운 날짜

};

/// \brief 타단 전적 인서트
class	_cORgStandingsInsertAccessor
{
public:
	inline void SetOrgIndex(int index)			{m_org_index = index;}
	inline void SetAgainstOrgIndex(int index)	{m_against_org = index;}
	inline void SetWinCount(int count)			{m_win_count = count;}
	inline void SetLoseCount(int count)			{m_lose_count = count;}
	inline void SetManorIndex( int map_id )
	{ 
		sprintf( m_manor_index, "%02d", map_id );
		m_manor_index[2] = 0;
	}
public:
	DEFINE_COMMAND_EX(_cORgStandingsInsertAccessor, L"{ CALL _prDs_SetOrgRecord(?,?,?,?,?) }" )
	BEGIN_PARAM_MAP(_cORgStandingsInsertAccessor)
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_org_index )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 2, m_against_org )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 3, m_win_count )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 4, m_lose_count )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 5, m_manor_index )
	END_PARAM_MAP()
protected:
	int		m_org_index;		// 자기단 인덱스	
	int		m_against_org;		// 타단 인덱스 
	int		m_win_count;		// 승
	int		m_lose_count;		// 패 
	char	m_manor_index[3];	//장원 인덱스     	
};
/// \class	_cUpdateOrgStandingsAccessor
/// \brief	타단과의 전적 업데이트시키기(바이너리 값이 있는 테이블)
class _cUpdateOrgStandingsAccessor
{
public:
	inline	void	SetOrgIndex(int index){m_dan_index =  index;}
	inline	void	SetCombatRecord(BYTE* record){ 	memcpy(m_combatRecord,record,4000);}
	inline	void	SetValid(BYTE valid){ m_valid = valid;}
	inline	void	SetManorIndex( int map_id )
	{ 
		sprintf( m_manor_index, "%02d", map_id );
		m_manor_index[2] = 0;
	}
	inline	void	SetRecordCount(int count){m_record_count = count;}
public:
	DEFINE_COMMAND_EX(_cUpdateOrgStandingsAccessor, L"{ CALL _prDs_SetDan_bi(?,?,?,?,?) }" )
		BEGIN_PARAM_MAP(_cUpdateOrgStandingsAccessor)
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 1, m_dan_index )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 2, m_combatRecord )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 3, m_valid )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 4, m_manor_index )
			SET_PARAM_TYPE( DBPARAMIO_INPUT )
			COLUMN_ENTRY( 5, m_record_count )			
		END_PARAM_MAP()
protected:
	int		m_dan_index;			// 자기 단
	BYTE	m_combatRecord[4000];	//타단인덱스,승,패,타단이름	
	BYTE	m_valid;				// 단이 유효한지
	char	m_manor_index[2+1];		// 장원전 번호
	int		m_record_count;			//바이너리에 있는 타단 카운트	
	
};
/// \brief 단의 장원별 전적 업데이트
class	_cUpdateOrgRecordInManorAccessor
{
public:
	inline void SetOrgIndex(int index)			{m_org_index = index;}
	inline void SetCombatCount(int count)		{m_combat_count = count;}
	inline void SetTotalCount(int count)		{m_total_count = count;}
	inline void SetWinCount(int count)			{m_win_count = count;}
	inline void SetLoseCount(int count)			{m_lose_count = count;}
	inline void SetManorIndex( int map_id )
	{ 
		sprintf( m_manor_index, "%02d", map_id );
		m_manor_index[2] = 0;
	}

public:
	DEFINE_COMMAND_EX(_cUpdateOrgRecordInManorAccessor, L"{ CALL _prDS_UpdateOrgRecordOfCastle(?,?,?,?,?,?) }" )
	BEGIN_PARAM_MAP(_cUpdateOrgRecordInManorAccessor)
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 1, m_org_index )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 2, m_manor_index )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 3, m_combat_count )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 4, m_total_count )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 5, m_win_count )
		SET_PARAM_TYPE( DBPARAMIO_INPUT )
		COLUMN_ENTRY( 6, m_lose_count )
	END_PARAM_MAP()
protected:
	int		m_org_index;		// 자기단 인덱스	
	char	m_manor_index[3];	//장원 인덱스     	
	int		m_combat_count;		// 장원전 참가횟수
	int		m_total_count;		//토탈 전적
	int		m_win_count;		// 승
	int		m_lose_count;		// 패 	
};
#endif //_PD_CASTLE_STEP_2_

#ifdef _PD_GAME_EVENT_
class _db_pr_DS_FMSelectWinnerInfo
{
public:
	int GetRank()			{ return m_fm_rank; }
	int GetNickID()			{ return m_fm_nick_id; }
	int GetWinnerCUID()		{ return m_fm_winner_cuid; }

public:
	BEGIN_COLUMN_MAP( _db_pr_DS_FMSelectWinnerInfo )
		COLUMN_ENTRY( 1, m_fm_rank ) 	
		COLUMN_ENTRY( 2, m_fm_nick_id )
		COLUMN_ENTRY( 3, m_fm_winner_cuid )
	END_COLUMN_MAP()

protected:
	int			m_fm_rank;
	int			m_fm_nick_id;
	int			m_fm_winner_cuid;
};

class _db_pr_DS_FMUpdateWinner
{
public:
	void SetRank( u_short rank)				{ m_fm_rank = static_cast<int>(rank); }
	void SetNickID( u_short nick_id )		{ m_fm_nick_id = static_cast<int>(nick_id); }
	void SetWinnerInfo( int cuid, char * name )
	{
		m_fm_winner_cuid = cuid;
		strncpy( m_fm_winner_name, name, en_charac_name_length );
		m_fm_winner_name[en_charac_name_length] = 0;
	}
public:
	DEFINE_COMMAND_EX(_db_pr_DS_FMUpdateWinner, L"{CALL dbo._prDS_FMUpdateWinner(?,?,?,?) }" )
	BEGIN_PARAM_MAP(_db_pr_DS_FMUpdateWinner)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(1, m_fm_rank)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_fm_nick_id)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_fm_winner_cuid)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_fm_winner_name)
	END_PARAM_MAP()

protected:
	int		m_fm_rank;
	int		m_fm_nick_id;
	int		m_fm_winner_cuid;
	char	m_fm_winner_name[en_charac_name_length+1];
};

#endif	// _PD_GAME_EVENT_

#ifdef _PD_LEVEL_PRESENT_

class _db_pr_DS_LevelPresent_GetNotRecv
{
public:	// Acc DB
	void SetAcc( char * acc)	{ 
		strncpy( m_acc_name, acc, en_max_lil );
		m_acc_name[en_max_lil] = 0;
		m_event_id = -1; 
	}
	short GetEventID()			{ return m_event_id; }

public:
	DEFINE_COMMAND_EX(_db_pr_DS_LevelPresent_GetNotRecv, L"{CALL dbo._prDS_LevelPresent_GetNotRecv(?,?) }" )
	BEGIN_PARAM_MAP(_db_pr_DS_LevelPresent_GetNotRecv)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(1, m_acc_name)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(2, m_event_id)
	END_PARAM_MAP()

protected:
	char		m_acc_name[en_max_lil+1];
	short		m_event_id;
};

class _db_pr_DS_LevelPresent_Check
{
public:	// Acc DB
	void Set( int uid, char * acc, short eid, short level, char * chrName )	
	{ 
		m_acc_uid = uid; 
		strncpy( m_account, acc, en_max_lil );
		m_account[en_max_lil] = 0;
		m_event_id = eid;
		m_check_level = level;
		strncpy( m_chr_name, chrName, en_charac_name_length );
		m_chr_name[en_charac_name_length] = 0;
		m_state = 1;		// 실패로 초기화
	}
	int GetState()			{ return m_state; }

public:
	DEFINE_COMMAND_EX(_db_pr_DS_LevelPresent_Check, L"{CALL dbo._prDS_LevelPresent_Check(?,?,?,?,?,?) }" )
	BEGIN_PARAM_MAP(_db_pr_DS_LevelPresent_Check)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(1, m_acc_uid)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_account)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_event_id)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_check_level)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(5, m_chr_name)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_state)
	END_PARAM_MAP()
	
protected:
	int			m_acc_uid;
	char		m_account[en_max_lil+1];
	short		m_event_id;
	short		m_check_level;
	char		m_chr_name[en_charac_name_length+1];
	short		m_state;		// 0 - success.( not received ), 1 - fail.
};


class _db_pr_DS_LevelPresent_Recv
{
public:	// Acc DB
	void Set( char * acc, short eid, short level, char * name )
	{
		strncpy( m_acc_name, acc, en_max_lil );
		m_acc_name[en_max_lil] = 0;
		m_event_id = eid;
		m_level = level;
		strncpy( m_chr_name, name, en_charac_name_length );
		m_chr_name[en_charac_name_length] = 0;
		m_recv_flag = 1;		// 실패로 초기화.
	}
	short GetRecvFlag()	{ return m_recv_flag; }

public:
	DEFINE_COMMAND_EX(_db_pr_DS_LevelPresent_Recv, L"{CALL dbo._prDS_LevelPresent_Recv(?,?,?,?,?) }" )
		BEGIN_PARAM_MAP(_db_pr_DS_LevelPresent_Recv)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_acc_name)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_event_id)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(3, m_level)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(4, m_chr_name)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY(5, m_recv_flag)
		END_PARAM_MAP()
	

protected:
	char		m_acc_name[en_max_lil+1];
	short		m_event_id;
	short		m_level;
	char		m_chr_name[en_charac_name_length+1];
	short		m_recv_flag;		// 1 - fail(already), 0 - success.
};


class _db_pr_DS_LevelPresent_Cancel
{
public:	// Acc DB
	void Set( char * acc, short eid )	{ 
		strncpy( m_acc_name, acc, en_max_lil );
		m_acc_name[en_max_lil] = 0;
		m_event_id = eid; 
	}

public:
	DEFINE_COMMAND_EX(_db_pr_DS_LevelPresent_Cancel, L"{CALL dbo._prDS_LevelPresent_Cancel(?,?) }" )
		BEGIN_PARAM_MAP(_db_pr_DS_LevelPresent_Cancel)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(1, m_acc_name)
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY(2, m_event_id)
		END_PARAM_MAP()
	
protected:
	char		m_acc_name[en_max_lil+1];
	short		m_event_id;
};


#endif // _PD_LEVEL_PRESENT_

#ifdef _PD_NEW_ACCOUNT_PRESENT_
#endif // _PD_NEW_ACCOUNT_PRESENT_

#ifdef _PD_PKEVENT_BOX_SPAWN_

class _db_prDS_GetBWWarWinTime
{
public:
	void SetCUID( int chr_uid )	{ m_chr_uid = chr_uid; m_win_time = 0 ; }
	int GetWinTime()			{ return m_win_time; }

	DEFINE_COMMAND_EX(_db_prDS_GetBWWarWinTime, L"{CALL dbo._prDS_GetBWWarWinTime(?,?) }" )
	BEGIN_PARAM_MAP( _db_prDS_GetBWWarWinTime )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 1, m_chr_uid )
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY( 2, m_win_time )
	END_PARAM_MAP()

protected:
	int			m_chr_uid;
	int			m_win_time;		// time() 함수.
};


class _db_prDS_UpdateBWWarWinTime
{
public:
	void SetWinTime( int chr_uid, int win_time )
	{ 
		m_chr_uid = chr_uid; 
		m_win_time = win_time; 
	}

	DEFINE_COMMAND_EX(_db_prDS_UpdateBWWarWinTime, L"{CALL dbo._prDS_UpdateBWWarWinTime(?,?) }" )
	BEGIN_PARAM_MAP( _db_prDS_UpdateBWWarWinTime )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 1, m_chr_uid )
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY( 2, m_win_time )
	END_PARAM_MAP()

protected:
	int			m_chr_uid;
	int			m_win_time;		// time() 함수.
};

#endif	// _PD_PKEVENT_BOX_SPAWN_

#ifdef _PD_EVENT_INVITE_FRIEND_

class _db_prDS_GetInviteFriendState
{
public:
	void Set( char * acc )
	{
		strncpy( m_acc_name, acc, en_max_lil );
		m_acc_name[en_max_lil] = 0;
		ZeroMemory( m_whom_name, sizeof(m_whom_name) );
		m_whom_name[en_max_lil] = 0;

		m_index = 0;
		m_check = 0;
		m_end_time = 0;
	}
	char * GetWhom()	{ return m_whom_name; }
	int GetIndex()		{ return m_index; }
	int GetEndTime()	{ return m_end_time; }
	short GetState()	{ return m_check; }

	DEFINE_COMMAND_EX(_db_prDS_GetInviteFriendState, L"{CALL dbo._prDS_GetInviteFriendState(?,?,?,?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_GetInviteFriendState )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 1, m_index )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_acc_name )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 3, m_whom_name )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 4, m_check)
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 5, m_end_time )
		END_PARAM_MAP()

protected:
	int				m_index;		// output
	char			m_acc_name[en_max_lil+1];
	char			m_whom_name[en_max_lil+1];
	short			m_check;
	int				m_end_time;
};

class _db_prDS_UpdateInviteFriendState
{
public:
	void SetStartTime( int index, int end_time, int gno )
	{
		m_index = index;
		m_mode = 0;
		m_check = 0;
		m_end_time = end_time;
		m_group_no = gno;
	}
	void SetEnd( int index )
	{
		m_index = index;
		m_mode = 1;
		m_check = 1;
		m_end_time = 0;
		m_group_no = 0;
	}

	DEFINE_COMMAND_EX(_db_prDS_UpdateInviteFriendState, L"{CALL dbo._prDS_UpdateInviteFriendState(?,?,?,?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_UpdateInviteFriendState )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_index )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_mode )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 3, m_check )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 4, m_end_time )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 5, m_group_no )
		END_PARAM_MAP()

protected:
	int				m_index;
	short			m_mode;			// 0 - start use. 1 - end.
	short			m_check;		// 끝날 때만, 1 
	int				m_end_time;		// mode 0 에서만 셋팅.
	short			m_group_no;		// 서버군 인덱스.

};

#endif // _PD_EVENT_INVITE_FRIEND_



#ifdef _PD_MASTER_PUPIL_

// Pupil 의 인덱스로 마스터의 인덱스를 얻는다.
// ND_V01_Pupil : pupil_index -> master_index
class _db_prDS_MP_GetMasterFromPupil
{
public:
	void SetPupilIndex( int idx )
	{
		m_pupil_index = idx;
		m_master_index = 0;
	}

	int GetMasterIndex()	{ return m_master_index; }
	
	DEFINE_COMMAND_EX(_db_prDS_MP_GetMasterFromPupil, L"{CALL dbo._prDS_MP_GetMasterFromPupil(?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_GetMasterFromPupil )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_pupil_index )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 2, m_master_index )
			
		END_PARAM_MAP()

protected:
	int				m_pupil_index;
	int				m_master_index;

};


// Master Index 로 마스터 정보를 얻는다.
// ND_V01_Master : master_index
class _db_prDS_MP_GetMasterInfo
{
public:
	void SetMasterIndex( int idx )
	{
		m_index = idx;
		m_total_point = -1;		// -1 은 정보가 없다는 것을 의미한다. 
	}

	DEFINE_COMMAND_EX(_db_prDS_MP_GetMasterInfo, 
		L"{CALL dbo._prDS_MP_GetMasterInfo(?,?,?,?,?,?,?,?,?,?,?,?,?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_GetMasterInfo )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_index )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 2, m_name )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 3, m_total_point )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 4, m_cur_point )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 5, m_total_killcount )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 6, m_master_rank )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 7, m_nick )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 8, m_gender )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 9, m_cur_level )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 10, m_moonpa )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 11, m_class )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 12, m_classgrade )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 13, m_create )
			SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
			COLUMN_ENTRY( 14, m_last_con )
		END_PARAM_MAP()

public:		// 변수가 많아서 그냥 public 으로 접근.
	int				m_index;
	__int64			m_total_point;
	__int64			m_cur_point;
	int				m_total_killcount;
	short			m_master_rank;			// 스승 레벨
	short			m_gender;				// 성별
	short			m_cur_level;			// 내공 수위.
	short			m_moonpa;				// 문파
	short			m_class;				// 직책
	short			m_classgrade;			// 직책 등급
	DBTIMESTAMP		m_create;			// 생성 날짜.
	DBTIMESTAMP		m_last_con;			// 

	char			m_name[en_charac_name_length+1];
	char			m_nick[NDD_MP_MASTERNICK_LENGTH+1];

};



// Master Index 로 제자 정보를 구성한다.
// ND_V01_Pupil : master_index
class _db_prDS_MP_GetPupilInfo
{
public:
	
	BEGIN_COLUMN_MAP( _db_prDS_MP_GetPupilInfo)
		COLUMN_ENTRY( 1, pupil_index )
		COLUMN_ENTRY( 2, pupil_name )
		COLUMN_ENTRY( 3, pupil_pos )
		COLUMN_ENTRY( 4, total_point )
		COLUMN_ENTRY( 5, cur_point )
		COLUMN_ENTRY( 6, total_killcount )
		COLUMN_ENTRY( 7, gender )
		COLUMN_ENTRY( 8, cur_level )
		COLUMN_ENTRY( 9, moonpa )
		COLUMN_ENTRY( 10, _class )
		COLUMN_ENTRY( 11, classgrade )
		COLUMN_ENTRY( 12, join )
		COLUMN_ENTRY( 13, last_con )
	END_COLUMN_MAP()

public:		// 변수가 많아서 그냥 public 으로 접근.
	int				pupil_index;

	short			pupil_pos;
    __int64			total_point;
	__int64			cur_point;

	int				total_killcount;

	short			gender;				// 성별
	short			cur_level;			// 내공 수위.
	short			moonpa;				// 문파
	short			_class;				// 직책
	short			classgrade;			// 직책 등급
	DBTIMESTAMP		join;			// 생성 날짜.
	DBTIMESTAMP		last_con;			// 

	char			pupil_name[en_charac_name_length+1];
};

class _db_prDS_MP_CreateMasterInfo
{
public:
	void SetMasterIndex( int idx )
	{
		m_index = idx;
	}

	void SetMasterName( char * name )
	{
		ZeroMemory( m_name, sizeof(m_name) );
		strncpy( m_name, name, en_charac_name_length );
	}

	void SetCreateDate( _sND_TimeUnit * time )
	{
		m_create_time.year = time->uiYear + 2000;
		m_create_time.month = time->uiMonth;
		m_create_time.day = time->uiDay;
		m_create_time.hour = time->uiHour;
		m_create_time.minute = time->uiMinute;
		m_create_time.second = time->uiSecond;
	}

	DEFINE_COMMAND_EX(_db_prDS_MP_CreateMasterInfo, 
		L"{CALL dbo._prDS_MP_CreateMasterInfo(?,?,?,?,?,?,?,?,?,?,?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_CreateMasterInfo )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_index )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_name )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 3, m_total_point )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 4, m_cur_point )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 5, m_total_killcount )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 6, m_master_rank )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 7, m_gender )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 8, m_cur_level )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 9, m_moonpa )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 10, m_class )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 11, m_classgrade )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 12, m_create_time )

		END_PARAM_MAP()

public:		// 변수가 많아서 그냥 public 으로 접근.
	int				m_index;
	__int64			m_total_point;
	__int64			m_cur_point;
	int				m_total_killcount;
	short			m_master_rank;			// 스승 레벨
	short			m_gender;				// 성별
	short			m_cur_level;			// 내공 수위.
	short			m_moonpa;				// 문파
	short			m_class;				// 직책
	short			m_classgrade;			// 직책 등급

	DBTIMESTAMP		m_create_time;

	char			m_name[en_charac_name_length+1];
};


// Master 정보. 모든 업데이트. 
class _db_prDS_MP_UpdateMasterInfo
{
public:
	void SetMasterIndex( int idx )
	{
		m_index = idx;
	}
	void SetConDate( _sND_TimeUnit * time )
	{
		m_last_con.year = time->uiYear + 2000;
		m_last_con.month = time->uiMonth;
		m_last_con.day = time->uiDay;
		m_last_con.hour = time->uiHour;
		m_last_con.minute = time->uiMinute;
		m_last_con.second = time->uiSecond;
	}

	DEFINE_COMMAND_EX(_db_prDS_MP_UpdateMasterInfo, 
		L"{CALL dbo._prDS_MP_UpdateMasterInfo(?,?,?,?,?,?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_UpdateMasterInfo )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_index )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_total_point )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 3, m_cur_point )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 4, m_total_killcount )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 5, m_master_rank )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 6, m_cur_level )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 7, m_last_con )
		END_PARAM_MAP()

public:		// 변수가 많아서 그냥 public 으로 접근.
	int				m_index;
	__int64			m_total_point;
	__int64			m_cur_point;
	int				m_total_killcount;
	short			m_master_rank;			// 스승 레벨
	short			m_cur_level;			// 내공 수위.

	DBTIMESTAMP		m_last_con;			// 
};


class _db_prDS_MP_UpdateMasterClass
{
public:
	void SetMasterIndex( int idx )
	{
		m_index = idx;
	}

	DEFINE_COMMAND_EX(_db_prDS_MP_UpdateMasterClass, 
		L"{CALL dbo._prDS_MP_UpdateMasterClass(?,?,?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_UpdateMasterClass )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_index )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_moonpa )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 3, m_class )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 4, m_classgrade )
		END_PARAM_MAP()

public:		// 변수가 많아서 그냥 public 으로 접근.
	int				m_index;
	short			m_moonpa;				// 문파
	short			m_class;				// 직책
	short			m_classgrade;			// 직책 등급
};



class _db_prDS_MP_UpdateMasterNick
{
public:
	void SetMasterIndex( int idx )
	{
		m_index = idx;
	}
	void SetNick( char * nick )
	{
		ZeroMemory( m_nick, sizeof(m_nick) );
		strncpy( m_nick, nick, NDD_MP_MASTERNICK_LENGTH );
	}

	DEFINE_COMMAND_EX(_db_prDS_MP_UpdateMasterNick, 
		L"{CALL dbo._prDS_MP_UpdateMasterNick(?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_UpdateMasterNick )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_index )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_nick )
		END_PARAM_MAP()

public:		// 변수가 많아서 그냥 public 으로 접근.
	int				m_index;
	char			m_nick[NDD_MP_MASTERNICK_LENGTH+1];
};





// 제자를 추가한다.
class _db_prDS_MP_AddNewPupil
{
public: 

	void SetName( char * name )
	{
		ZeroMemory( m_cPupilName, sizeof(m_cPupilName) );
		strncpy( m_cPupilName, name, en_charac_name_length );
	}

	DEFINE_COMMAND_EX(_db_prDS_MP_AddNewPupil, 
		L"{CALL dbo._prDS_MP_AddNewPupilInfo(?,?,?,?,?,?,?,?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_AddNewPupil )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_iPupilIndex )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_iMasterIndex )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 3, m_cPupilName )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 4, m_sPosition )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 5, m_sGender )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 6, m_sCurLevel )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 7, m_sMoonpa )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 8, m_sClass )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 9, m_sClassGrade )

		END_PARAM_MAP()

public:
	int			m_iPupilIndex;
	int			m_iMasterIndex;
	
	short		m_sPosition;
	short		m_sGender;
	short		m_sCurLevel;
	short		m_sMoonpa;
	short		m_sClass;
	short		m_sClassGrade;

	char		m_cPupilName[en_charac_name_length+1];
};


// Pupil 정보. 변화가 거의 없는 부분.
class _db_prDS_MP_UpdatePupilBase
{
public:
	void SetPupilIndex( int idx )
	{
		m_pupil_index = idx;
	}

	DEFINE_COMMAND_EX(_db_prDS_MP_UpdatePupilBase, 
		L"{CALL dbo._prDS_MP_UpdatePupilBase(?,?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_UpdatePupilBase )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_pupil_index )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_where )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 3, m_value )
		END_PARAM_MAP()

public:	
	int				m_pupil_index;
	short			m_where;
	short			m_value;
	// where
	//	0 - Pupil position
	//	1 - cur_level
	//	2 - class
	//  3 - classgrade
	//  4 - moonpa
	//  5 - gender
};


// Pupil 정보. 자주 바뀌는 부분 업데이트
class _db_prDS_MP_UpdatePupilPoint
{
public:
	void SetPupilIndex( int idx )
	{
		m_pupil_index = idx;
	}
	
	DEFINE_COMMAND_EX(_db_prDS_MP_UpdatePupilPoint, 
		L"{CALL dbo._prDS_MP_UpdatePupilPoint(?,?,?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_UpdatePupilPoint )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_pupil_index )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_total_point )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 3, m_cur_point )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 4, m_my_killcount )
		END_PARAM_MAP()

public:		// 변수가 많아서 그냥 public 으로 접근.
	int				m_pupil_index;

	__int64			m_total_point;
	__int64			m_cur_point;
	int				m_my_killcount;
};


// 파문, 하산시에 데이타를 백업쪽으로 옮긴다.
class _db_prDS_MP_BackupPupil
{
	// ND_V01_PUPIL 에서, ND_V01_PUPIL_HISTORY 에 옮기고, 지운다.
public:
	void SetPupilIndex( int idx, short reason )
	{
		m_index = idx;
		m_withdraw = reason;		// 파문 = 1, 강제추방 = 2, 하산 = 3
	}

	DEFINE_COMMAND_EX(_db_prDS_MP_BackupPupil, 
		L"{CALL dbo._prDS_MP_BackupPupil(?,?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_BackupPupil )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_index )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 2, m_withdraw )
		END_PARAM_MAP()

public:		// 변수가 많아서 그냥 public 으로 접근.
	int				m_index;
	short			m_withdraw;
};


// 최종 접속시간을 업데이트한다.
class _db_prDS_MP_UpdatePupilLastCon
{
	// ND_V01_PUPIL 에서, ND_V01_PUPIL_HISTORY 에 옮기고, 지운다.
public:
	void SetPupilIndex( int idx, short reason )
	{
		m_index = idx;
	}

	DEFINE_COMMAND_EX(_db_prDS_MP_UpdatePupilLastCon, 
		L"{CALL dbo._prDS_MP_UpdatePupilLastCon(?) }" )
		BEGIN_PARAM_MAP( _db_prDS_MP_UpdatePupilLastCon )
			SET_PARAM_TYPE(DBPARAMIO_INPUT)
			COLUMN_ENTRY( 1, m_index )
		END_PARAM_MAP()

public:		// 변수가 많아서 그냥 public 으로 접근.
	int				m_index;
};

#endif