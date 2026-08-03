#ifndef TEX_H
#define TEX_H

/*
    Compile unit: C:\cs4\Seimei\src\Tex.c
    Producer: MW MIPS C Compiler
    Language: C
    Code range: 0x0013FA80 -> 0x0013FB18
*/
// Range: 0x13FA80 -> 0x13FB18
// total size: 0x60
struct /* @anon0 */ {
    // Members
    unsigned char bUse; // offset 0x0, size 0x1
    unsigned char unused0; // offset 0x1, size 0x1
    unsigned short id; // offset 0x2, size 0x2
    // total size: 0x20
    struct /* @anon1 */ {
        // Members
        unsigned int size; // offset 0x0, size 0x4
        unsigned int wnum; // offset 0x4, size 0x4
        unsigned char dummy[5]; // offset 0x8, size 0x5
        unsigned char compare; // offset 0xD, size 0x1
        unsigned short tex_id; // offset 0xE, size 0x2
        // total size: 0x8
        struct /* @anon5 */ {
            // Members
            unsigned long TBP0 : 14; // offset 0x0, size 0x4
            unsigned long TBW : 6; // offset 0x0, size 0x4
            unsigned long PSM : 6; // offset 0x0, size 0x4
            unsigned long TW : 4; // offset 0x0, size 0x4
            unsigned long TH : 4; // offset 0x0, size 0x4
            unsigned long TCC : 1; // offset 0x0, size 0x4
            unsigned long TFX : 2; // offset 0x0, size 0x4
            unsigned long CBP : 14; // offset 0x0, size 0x4
            unsigned long CPSM : 4; // offset 0x0, size 0x4
            unsigned long CSM : 1; // offset 0x0, size 0x4
            unsigned long CSA : 5; // offset 0x0, size 0x4
            unsigned long CLD : 3; // offset 0x0, size 0x4
        } tex0; // offset 0x10, size 0x8
        // total size: 0x8
        struct /* @anon7 */ {
            // Members
            unsigned long LCM : 1; // offset 0x0, size 0x4
            unsigned long pad01 : 1; // offset 0x0, size 0x4
            unsigned long MXL : 3; // offset 0x0, size 0x4
            unsigned long MMAG : 1; // offset 0x0, size 0x4
            unsigned long MMIN : 3; // offset 0x0, size 0x4
            unsigned long MTBA : 1; // offset 0x0, size 0x4
            unsigned long pad10 : 9; // offset 0x0, size 0x4
            unsigned long L : 2; // offset 0x0, size 0x4
            unsigned long pad21 : 11; // offset 0x0, size 0x4
            unsigned long K : 12; // offset 0x0, size 0x4
            unsigned long pad44 : 20; // offset 0x0, size 0x4
        } tex1; // offset 0x18, size 0x8
    } * lpTm2; // offset 0x4, size 0x4
    void * lpTexList; // offset 0x8, size 0x4
    signed int unused1[1]; // offset 0xC, size 0x4
    unsigned char texName[32]; // offset 0x10, size 0x20
    unsigned char texNameBase[32]; // offset 0x30, size 0x20
    signed int texW; // offset 0x50, size 0x4
    signed int texH; // offset 0x54, size 0x4
    signed int texTBW; // offset 0x58, size 0x4
    signed int texPSM; // offset 0x5C, size 0x4
} * GetTexMngByName(void * lpTexList /* r18 */, char * texname /* r17 */);


#endif // TEX_H
