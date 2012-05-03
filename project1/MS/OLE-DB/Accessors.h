#pragma once

#include "../MyLib/MY_OLE_DB.H"

class C_prMS_GetFriendList_N_MailListAccessor
{
public:
	LONG m_RETURN_VALUE;
	char m_chr_name[13] ;
	int	m_iUniqueID ;
	BYTE m_friend_list[760];	
	BYTE m_mail_list[100];
	short m_gm ;
	LONG m_result;
	
	DEFINE_COMMAND(C_prMS_GetFriendList_N_MailListAccessor, "{ ? = CALL dbo._prMS_GetFriendList_N_MailList(?,?,?,?,?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMS_GetFriendList_N_MailListAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_chr_name)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_iUniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_friend_list)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_mail_list)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_gm)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(7, m_result)
	END_PARAM_MAP()
	
};

class C_prMS_GetFriendList_N_MailList_by_UIDAccessor
{
public:
	LONG m_RETURN_VALUE;	
	int	m_iUniqueID ;
	BYTE m_friend_list[760];	
	BYTE m_mail_list[100];
	short m_gm ;
	LONG m_result;
	
	DEFINE_COMMAND(C_prMS_GetFriendList_N_MailList_by_UIDAccessor, "{ ? = CALL dbo._prMS_GetFriendList_N_MailList_by_UID(?,?,?,?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMS_GetFriendList_N_MailList_by_UIDAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_iUniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_friend_list)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_mail_list)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_gm)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_result)
		END_PARAM_MAP()
		
};


class C_prMs_UpdateFriendList_N_MailListAccessor
{
public:
	LONG m_RETURN_VALUE;	
	int	m_iUniqueID ;
	BYTE m_friend_list[760];	
	BYTE m_mail_list[100];	
	
	DEFINE_COMMAND(C_prMs_UpdateFriendList_N_MailListAccessor, "{ ? = CALL dbo._prMs_UpdateFriendList_N_MailList(?,?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_UpdateFriendList_N_MailListAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_iUniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_friend_list)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_mail_list)		
	END_PARAM_MAP()

};


class C_prMs_GetOffFriendListAccessor
{
public:	
	
	LONG m_RETURN_VALUE;
	CHAR m_chr_name[13] ;
	int	m_iUniqueID ;
	BYTE m_list_data[760];	
	short m_gm ;
	LONG m_result ;	
	
	
	DEFINE_COMMAND(C_prMs_GetOffFriendListAccessor, "{ ? = CALL dbo._prMs_GetOffFriendList(?,?,?,?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_GetOffFriendListAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_chr_name)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_iUniqueID)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_list_data)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_gm)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_result)
	END_PARAM_MAP()

};


class C_prMs_GetOffMailListAccessor
{
public:
		
	LONG m_RETURN_VALUE;
	char m_chr_name[13] ;
	int	m_iUniqueID ;
	BYTE m_mail_list[100];
	short m_gm ;
	LONG m_result;
	
	
	
	
	DEFINE_COMMAND(C_prMs_GetOffMailListAccessor, "{ ? = CALL dbo._prMs_GetOffMailList(?,?,?,?,?) }")		
		
		BEGIN_PARAM_MAP(C_prMs_GetOffMailListAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_chr_name)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_iUniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_mail_list)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_gm)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_result)
	END_PARAM_MAP()

};



class C_prMs_UpdateFriendListAccessor
{
public:
	
	
	LONG m_RETURN_VALUE;
	int	m_iUniqueID ;
	BYTE m_list_data[760];
	
	
	DEFINE_COMMAND(C_prMs_UpdateFriendListAccessor, "{ ? = CALL dbo._prMs_UpdateFriendList(?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_UpdateFriendListAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_iUniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_list_data)
	END_PARAM_MAP()

};


class C_prMs_UpdateMailListAccessor
{
public:	
	
	LONG m_RETURN_VALUE;
	int	m_iUniqueID ;
	BYTE m_mail_list[100];
	
	
	
	DEFINE_COMMAND(C_prMs_UpdateMailListAccessor, "{ ? = CALL dbo._prMs_UpdateMailList(?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_UpdateMailListAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_iUniqueID)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_mail_list)
	END_PARAM_MAP()

};




class C_prMs_GetUIDAccessor
{
public:	
	
	LONG m_RETURN_VALUE;
	CHAR m_chr_name[13] ;
	int	m_iUniqueID ;	
	LONG m_result ;
	
	
	
	DEFINE_COMMAND(C_prMs_GetUIDAccessor, "{ ? = CALL dbo._prMs_GetUID(?,?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_GetUIDAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_chr_name)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)		
		COLUMN_ENTRY(3, m_iUniqueID)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)		
		COLUMN_ENTRY(4, m_result)
	END_PARAM_MAP()

};

class C_prMs_MailBoxAccessor
{
public:
	
	LONG m_RETURN_VALUE;
	LONG m_index;
	CHAR m_from[13];
	DBTIMESTAMP m_date;
	CHAR m_text[101];
	short m_mode ;
	LONG m_result;
	

	DEFINE_COMMAND(C_prMs_MailBoxAccessor, "{ ? = CALL dbo._prMs_MailBox_(?,?,?,?,?,?,?,?,?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_MailBoxAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_index)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(3, m_from)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(4, m_date.year)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_date.month)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(6, m_date.day)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(7, m_date.hour)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(8, m_date.minute)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(9, m_text)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(10, m_mode)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(11, m_result)
	END_PARAM_MAP()

};


class C_prMs_SendMailAccessor
{
public:
	
	
	
	
	LONG m_RETURN_VALUE;
	LONG m_index;
	CHAR m_from[13];
	CHAR m_text[101];

	short m_mode ;	
	
	
	DEFINE_COMMAND(C_prMs_SendMailAccessor, "{ ? = CALL dbo._prMs_SendMail_(?,?,?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_SendMailAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)		
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_from)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_text)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_mode)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(5, m_index)
	END_PARAM_MAP()

};



class C_prMs_DropMailAccessor
{
public:
	
	
	
	
	LONG m_RETURN_VALUE;
	LONG m_index;
	
	
	DEFINE_COMMAND(C_prMs_DropMailAccessor, "{ ? = CALL dbo._prMs_DropMail(?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_DropMailAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_index)		
	END_PARAM_MAP()

};



class C_prMs_SendMSMessageAccessor
{
public:
	
	
	LONG m_RETURN_VALUE;
	LONG m_index;
	CHAR m_from[13];
	SHORT m_mode ;
	
	DEFINE_COMMAND(C_prMs_SendMSMessageAccessor, "{ ? = CALL dbo._prMs_SendMSMessage(?,?,?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_SendMSMessageAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT | DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(2, m_index)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(3, m_from)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(4, m_mode)
	END_PARAM_MAP()

};



class C_prMs_DeleteCharacAccessor
{
public:
	
	
	LONG m_RETURN_VALUE;
	int	m_iUniqueID ;
	
	DEFINE_COMMAND(C_prMs_DeleteCharacAccessor, "{ ? = CALL dbo._prMS_DeleteCharac(?) }")
		
		
		BEGIN_PARAM_MAP(C_prMs_DeleteCharacAccessor)
		SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
		COLUMN_ENTRY(1, m_RETURN_VALUE)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, m_iUniqueID)		
	END_PARAM_MAP()
	
};



typedef CDB_Command<C_prMs_DeleteCharacAccessor >							C_prMs_DeleteCharac ;
typedef CDB_Command<C_prMs_GetOffFriendListAccessor >						C_prMs_GetOffFriendList ;
typedef CDB_Command<C_prMs_GetOffMailListAccessor >							C_prMs_GetOffMailList ;
typedef CDB_Command<C_prMs_GetUIDAccessor >									C_prMs_GetUID ;
typedef CDB_Command<C_prMs_MailBoxAccessor >								C_prMs_MailBox ;

typedef CDB_Command<C_prMs_SendMailAccessor >								C_prMs_SendMail ;
typedef CDB_Command<C_prMs_DropMailAccessor >								C_prMs_DropMail ;
typedef CDB_Command<C_prMs_SendMSMessageAccessor >							C_prMs_SendMSMessage ;
typedef CDB_Command<C_prMs_UpdateFriendListAccessor >						C_prMs_UpdateFriendList ;
typedef CDB_Command<C_prMs_UpdateMailListAccessor >							C_prMs_UpdateMailList ;

typedef CDB_Command<C_prMS_GetFriendList_N_MailListAccessor >				C_prMS_GetFriendList_N_MailList ;
typedef CDB_Command<C_prMs_UpdateFriendList_N_MailListAccessor >			C_prMS_UpdateFriendList_N_MailList ;

typedef CDB_Command<C_prMS_GetFriendList_N_MailList_by_UIDAccessor>			C_prMS_GetFriendList_N_MailList_by_UID ;

