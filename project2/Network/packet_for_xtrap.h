#pragma once
#pragma pack(1)

// XTrap

struct sHeader;

// Trap.h에 있는 define - 확인할 것
#define CREATEKF_IN_SIZE		264
#define CREATEKF_OUT_SIZE		544

typedef unsigned char XC_CLIENTBUF	[CREATEKF_OUT_SIZE];
typedef unsigned char XC_ADDR		[CREATEKF_IN_SIZE];

#define MSG_NO_CHECK_XTRAP		157			// Xtrap Send/Recv

enum EN_XTRAP_CHECK_MOVE	// XCM
{
	en_xtrap_sc_request = 0,	// S->C:Request client to send hash code to server
	en_xtrap_cs_hashcode,		// C->S:Hash code
	en_xtrap_sc_errno,			// S->C:send errno tk client if error exist
	en_xtrap_sc_exit			// S->C:send exit command to client
};

struct MSG_XTRAP
{
	_sHeader	header;
	u_char		ucMessage;

	u_char		ucMode;
};

struct MSG_XTRAP_SC_REQUEST : public MSG_XTRAP
{
	XC_ADDR		InputCode[2];
};

struct MSG_XTRAP_CS_HASHCODE : public MSG_XTRAP
{
//	char		hashcode[CREATEKF_OUT_SIZE*2];
	XC_CLIENTBUF	hashcode[2];
};

struct MSG_XTRAP_SC_ERRNO : public MSG_XTRAP
{
	u_char		ucError;
};

struct MSG_XTRAP_SC_EXIT : public MSG_XTRAP
{
	u_char		ucExitCode;
};