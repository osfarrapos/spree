#ifdef	_SMALL_NETWORK_

#pragma once
#pragma pack(1)

typedef struct {
	TCHAR pszEcho[194];
} PKT_C2S_ECHO_REQ, PKT_S2C_ECHO_ACK;

#pragma pack()
#endif //_SMALL_NETWORK_