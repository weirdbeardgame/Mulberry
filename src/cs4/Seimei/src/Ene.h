#ifndef ENE_H
#define ENE_H

#include "typedefs.h"

#include "Model.h"

// I will clean this up later
void EneAppear(signed int EneId /* r18 */, signed int EneDataIndex /* r2 */, struct _mdlmng * lpEne /* r16 */, float * Pos /* r19 */, float AngY /* r20 */, float * Scl /* r20 */, unsigned char TeamNo /* r17 */, unsigned short AnimId /* r23 */, unsigned char ActFree /* r30 */, // total size: 0x260
struct /* @anon0 */ {
    // Members
    // total size: 0x1A0
    struct /* @anon20 */ {
        // Members
        void (* ThkFunc[32])(); // offset 0x0, size 0x80
        void (* ActFunc[32])(); // offset 0x80, size 0x80
        void (* SetActFunc[32])(); // offset 0x100, size 0x80
        unsigned char actID; // offset 0x180, size 0x1
        unsigned char actIDB; // offset 0x181, size 0x1
        unsigned char freeEnd; // offset 0x182, size 0x1
        unsigned char unused0[1]; // offset 0x183, size 0x1
        signed int (* GetDmgAnimFunc)(); // offset 0x184, size 0x4
        signed int (* CalcDmgFunc)(); // offset 0x188, size 0x4
        void (* FrameLastFunc)(); // offset 0x18C, size 0x4
        void (* DramaStartFunc)(); // offset 0x190, size 0x4
        void (* DramaEndFunc)(); // offset 0x194, size 0x4
        signed int unused[2]; // offset 0x198, size 0x8
    } func; // offset 0x0, size 0x1A0
    // total size: 0x10
    struct /* @anon2 */ {
        // Members
        // total size: 0x24
        struct /* @anon3 */ {
            // Members
            unsigned char success; // offset 0x0, size 0x1
            unsigned char data_len; // offset 0x1, size 0x1
            union /* @anon16 */ {
                // total size: 0x2
                struct /* @anon7 */ {
                    // Members
                    unsigned short select : 1; // offset 0x0, size 0x2
                    unsigned short L3 : 1; // offset 0x0, size 0x2
                    unsigned short R3 : 1; // offset 0x0, size 0x2
                    unsigned short start : 1; // offset 0x0, size 0x2
                    unsigned short Lup : 1; // offset 0x0, size 0x2
                    unsigned short Lright : 1; // offset 0x0, size 0x2
                    unsigned short Ldown : 1; // offset 0x0, size 0x2
                    unsigned short Lleft : 1; // offset 0x0, size 0x2
                    unsigned short L2 : 1; // offset 0x0, size 0x2
                    unsigned short R2 : 1; // offset 0x0, size 0x2
                    unsigned short L1 : 1; // offset 0x0, size 0x2
                    unsigned short R1 : 1; // offset 0x0, size 0x2
                    unsigned short Rup : 1; // offset 0x0, size 0x2
                    unsigned short Rdown : 1; // offset 0x0, size 0x2
                    unsigned short Rright : 1; // offset 0x0, size 0x2
                    unsigned short Rleft : 1; // offset 0x0, size 0x2
                } bit; // offset 0x0, size 0x2
                unsigned short set; // offset 0x0, size 0x2
            } button; // offset 0x2, size 0x2
            unsigned char r3h; // offset 0x4, size 0x1
            unsigned char r3v; // offset 0x5, size 0x1
            unsigned char l3h; // offset 0x6, size 0x1
            unsigned char l3v; // offset 0x7, size 0x1
            unsigned char apress[12]; // offset 0x8, size 0xC
            // total size: 0xC
            struct /* @anon17 */ {
                // Members
                unsigned char Lright; // offset 0x0, size 0x1
                unsigned char Lleft; // offset 0x1, size 0x1
                unsigned char Lup; // offset 0x2, size 0x1
                unsigned char Ldown; // offset 0x3, size 0x1
                unsigned char Rup; // offset 0x4, size 0x1
                unsigned char Rright; // offset 0x5, size 0x1
                unsigned char Rdown; // offset 0x6, size 0x1
                unsigned char Rleft; // offset 0x7, size 0x1
                unsigned char L1; // offset 0x8, size 0x1
                unsigned char R1; // offset 0x9, size 0x1
                unsigned char L2; // offset 0xA, size 0x1
                unsigned char R2; // offset 0xB, size 0x1
            } press; // offset 0x14, size 0xC
            union /* @anon16 */ {
                // total size: 0x2
                struct /* @anon7 */ {
                    // Members
                    unsigned short select : 1; // offset 0x0, size 0x2
                    unsigned short L3 : 1; // offset 0x0, size 0x2
                    unsigned short R3 : 1; // offset 0x0, size 0x2
                    unsigned short start : 1; // offset 0x0, size 0x2
                    unsigned short Lup : 1; // offset 0x0, size 0x2
                    unsigned short Lright : 1; // offset 0x0, size 0x2
                    unsigned short Ldown : 1; // offset 0x0, size 0x2
                    unsigned short Lleft : 1; // offset 0x0, size 0x2
                    unsigned short L2 : 1; // offset 0x0, size 0x2
                    unsigned short R2 : 1; // offset 0x0, size 0x2
                    unsigned short L1 : 1; // offset 0x0, size 0x2
                    unsigned short R1 : 1; // offset 0x0, size 0x2
                    unsigned short Rup : 1; // offset 0x0, size 0x2
                    unsigned short Rdown : 1; // offset 0x0, size 0x2
                    unsigned short Rright : 1; // offset 0x0, size 0x2
                    unsigned short Rleft : 1; // offset 0x0, size 0x2
                } bit; // offset 0x0, size 0x2
                unsigned short set; // offset 0x0, size 0x2
            } edge; // offset 0x20, size 0x2
            union /* @anon16 */ {
                // total size: 0x2
                struct /* @anon7 */ {
                    // Members
                    unsigned short select : 1; // offset 0x0, size 0x2
                    unsigned short L3 : 1; // offset 0x0, size 0x2
                    unsigned short R3 : 1; // offset 0x0, size 0x2
                    unsigned short start : 1; // offset 0x0, size 0x2
                    unsigned short Lup : 1; // offset 0x0, size 0x2
                    unsigned short Lright : 1; // offset 0x0, size 0x2
                    unsigned short Ldown : 1; // offset 0x0, size 0x2
                    unsigned short Lleft : 1; // offset 0x0, size 0x2
                    unsigned short L2 : 1; // offset 0x0, size 0x2
                    unsigned short R2 : 1; // offset 0x0, size 0x2
                    unsigned short L1 : 1; // offset 0x0, size 0x2
                    unsigned short R1 : 1; // offset 0x0, size 0x2
                    unsigned short Rup : 1; // offset 0x0, size 0x2
                    unsigned short Rdown : 1; // offset 0x0, size 0x2
                    unsigned short Rright : 1; // offset 0x0, size 0x2
                    unsigned short Rleft : 1; // offset 0x0, size 0x2
                } bit; // offset 0x0, size 0x2
                unsigned short set; // offset 0x0, size 0x2
            } repeat; // offset 0x22, size 0x2
        } * lpPad; // offset 0x0, size 0x4
        char L3H; // offset 0x4, size 0x1
        char L3V; // offset 0x5, size 0x1
        char R3H; // offset 0x6, size 0x1
        char R3V; // offset 0x7, size 0x1
        unsigned int unused[2]; // offset 0x8, size 0x8
    } key; // offset 0x1A0, size 0x10
    float InitPos[4]; // offset 0x1B0, size 0x10
    float DstPos[4]; // offset 0x1C0, size 0x10
    float moveRange; // offset 0x1D0, size 0x4
    float moveAngle; // offset 0x1D4, size 0x4
    unsigned char bDash; // offset 0x1D8, size 0x1
    unsigned char Rebirth; // offset 0x1D9, size 0x1
    unsigned char bHitOff; // offset 0x1DA, size 0x1
    unsigned char bDead; // offset 0x1DB, size 0x1
    unsigned short stayTimer; // offset 0x1DC, size 0x2
    unsigned short walkTimer; // offset 0x1DE, size 0x2
    unsigned short approachTimer; // offset 0x1E0, size 0x2
    unsigned short ClingDmgMyAnim; // offset 0x1E2, size 0x2
    struct _mdlmng * lpCling; // offset 0x1E4, size 0x4
    signed int attackOff; // offset 0x1E8, size 0x4
    signed short RebirthTimer; // offset 0x1EC, size 0x2
    unsigned short PauseTime; // offset 0x1EE, size 0x2
    unsigned char bWeak; // offset 0x1F0, size 0x1
    unsigned char UseSfxHdlNum; // offset 0x1F1, size 0x1
    unsigned char bFired; // offset 0x1F2, size 0x1
    unsigned char bFree; // offset 0x1F3, size 0x1
    float rollAng; // offset 0x1F4, size 0x4
    unsigned short lifecut; // offset 0x1F8, size 0x2
    unsigned short noDamage; // offset 0x1FA, size 0x2
    signed short DisableSearch; // offset 0x1FC, size 0x2
    signed short GuardFrame; // offset 0x1FE, size 0x2
    signed int SfxHdl[8]; // offset 0x200, size 0x20
    signed int SeHdl[8]; // offset 0x220, size 0x20
    struct _mdlmng * lpP019; // offset 0x240, size 0x4
    unsigned char lifecutscl; // offset 0x244, size 0x1
    unsigned char captureLock; // offset 0x245, size 0x1
    unsigned char ClingPos; // offset 0x246, size 0x1
    unsigned char ClingFree; // offset 0x247, size 0x1
    unsigned char NoSave; // offset 0x248, size 0x1
    unsigned char FreeDamage; // offset 0x249, size 0x1
    unsigned char bEraseDisable; // offset 0x24A, size 0x1
    unsigned char NoFloorDeadTimer; // offset 0x24B, size 0x1
    unsigned char bNoFloorDeadDisable; // offset 0x24C, size 0x1
    unsigned char AppearTypeAlpha; // offset 0x24D, size 0x1
    unsigned char unused0[2]; // offset 0x24E, size 0x2
    float PowerScl; // offset 0x250, size 0x4
    signed int MaxHp; // offset 0x254, size 0x4
    signed int ItemId; // offset 0x258, size 0x4
    float SclLimit; // offset 0x25C, size 0x4
} * lpCtrlSt /* r29+0xC0 */, // total size: 0x50
struct /* @anon5 */ {
    // Members
    unsigned char bUse; // offset 0x0, size 0x1
    unsigned char SceneNo; // offset 0x1, size 0x1
    unsigned char MdlMngIndex; // offset 0x2, size 0x1
    unsigned char ActID; // offset 0x3, size 0x1
    unsigned short AnimID; // offset 0x4, size 0x2
    unsigned short blood_pool_sfx_idx; // offset 0x6, size 0x2
    float Frame; // offset 0x8, size 0x4
    signed int Hp; // offset 0xC, size 0x4
    float Pos[4]; // offset 0x10, size 0x10
    float Ang[4]; // offset 0x20, size 0x10
    float Scl[4]; // offset 0x30, size 0x10
    signed short RebirthTimer; // offset 0x40, size 0x2
    unsigned char Rebirth; // offset 0x42, size 0x1
    unsigned char HitOff; // offset 0x43, size 0x1
    unsigned char GravOff; // offset 0x44, size 0x1
    unsigned char MapHitOff; // offset 0x45, size 0x1
    unsigned char Weak; // offset 0x46, size 0x1
    unsigned char bFired; // offset 0x47, size 0x1
    signed int ItemId; // offset 0x48, size 0x4
} * lpEneSave /* r21 */);

    void EnePlayAll();
    void InitEneSave();

#endif // ENE_H
