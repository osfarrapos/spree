#pragma once

#ifdef _CIS_20070326_

// en_max_lil

class COdbc;

void CIS_ShowCashInvenReq(MSG_COMMON* pRecv, COdbc* pCIS, _h_DS_Com* pDSComm);
void CIS_MoveCashItemReq(MSG_COMMON* pRecv, COdbc* pCIS, _h_DS_Com* pDSComm);
void CIS_PresentMsgReq(MSG_COMMON* pRecv, COdbc* pCIS, _h_DS_Com* pDSComm);

#endif
