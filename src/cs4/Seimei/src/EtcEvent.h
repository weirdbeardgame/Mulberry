#ifndef ETC_EVENT_H
#define ETC_EVENT_H
#include "common.h"
#include "Model.h"
#include "Unk.h"

struct _ETC_EVENT_MNSG__ENE_EXT_GENE_DATA_ST {
    // Members
    signed int state; // offset 0x0, size 0x4
    MDLMNG * pMdlMng; // offset 0x4, size 0x4
    PENECTRL * pEneCtrl;
    unsigned int ene_num; // offset 0xC, size 0x4
    unsigned int counter; // offset 0x10, size 0x4
    unsigned int dmy[3]; // offset 0x14, size 0xC
};

typedef struct  {
    // Members
    _MNG_STATE_ST mng_state; // offset 0x0, size 0x20
    signed int DrawState; // offset 0x20, size 0x4
    unsigned char * * pEtcEventFileBuf; // offset 0x24, size 0x4
    struct _ETC_EVENT_MNSG__ENE_EXT_GENE_DATA_ST EneExtGeneEventData; // offset 0x28, size 0x20
} _ETC_EVENT_MNG_ST;
    
extern _ETC_EVENT_MNG_ST g_EtcEventMng;

int EtcEventMng_EneExtGeneEvent_Update( _ETC_EVENT_MNG_ST * pEtcEventMng /* r2 */);
int EtcEventMng_EneExtGeneEvent_End(_ETC_EVENT_MNG_ST * pEtcEventMng /* r2 */);

#endif // ETC_EVENT_H