#ifndef TEX_H
#define TEX_H

#include "common.h"
#include "ee/eestruct.h"

// total size: 0x20
typedef struct /* @anon1 */ {
    // Members
    unsigned int size; // offset 0x0, size 0x4
    unsigned int wnum; // offset 0x4, size 0x4
    unsigned char dummy[5]; // offset 0x8, size 0x5
    unsigned char compare; // offset 0xD, size 0x1
    unsigned short tex_id; // offset 0xE, size 0x2
    // total size: 0x8
    sceGsTex0 tex0; // offset 0x10, size 0x8
    // total size: 0x8
    sceGsTex1 tex1; // offset 0x18, size 0x8
} TM2;

typedef struct /* @anon0 */ {
    // Members
    unsigned char bUse; // offset 0x0, size 0x1
    unsigned char unused0; // offset 0x1, size 0x1
    unsigned short id; // offset 0x2, size 0x2
    // total size: 0x20
    TM2 * lpTm2; // offset 0x4, size 0x4
    void * lpTexList; // offset 0x8, size 0x4
    signed int unused1[1]; // offset 0xC, size 0x4
    unsigned char texName[32]; // offset 0x10, size 0x20
    unsigned char texNameBase[32]; // offset 0x30, size 0x20
    signed int texW; // offset 0x50, size 0x4
    signed int texH; // offset 0x54, size 0x4
    signed int texTBW; // offset 0x58, size 0x4
    signed int texPSM; // offset 0x5C, size 0x4
} TEXMNG;

TEXMNG* GetTexMngByName(void * lpTexList /* r18 */, char * texname /* r17 */);

#endif // TEX_H
