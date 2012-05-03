#pragma once

void RF_RequestRecall( CMSG_RF_REQUEST_RECALL * pRecall, _h_DS_Com * pFromCom );
void RF_Answer( CMSG_RF_ANSWER * pAnswer );
void RF_ForceMove( CMSG_RF_FORCE_MOVE * pMove );
void RF_Fail( CMSG_RF_FAIL * pFail );