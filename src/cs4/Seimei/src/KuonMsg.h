#ifndef KUON_MSG_H
#define KUON_MSG_H
#include "common.h"

int KnMsgMng_Update();
int KnMsgMng_Draw();
u_short * * KnMsgMng_GetMapNameMsg();
u_short * KnMsgMng_GetMsgStr(int msg_id /* r16 */);
int KnMsgMng_MsgAllClear();
#endif // KUON_MSG_H