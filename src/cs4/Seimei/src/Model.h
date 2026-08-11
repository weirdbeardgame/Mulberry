#ifndef MODEL_H
#define MODEL_H

#include "typedefs.h"

// total size: 0x1C0
typedef struct _mdlmng {
    // Members
    unsigned char bUse; // offset 0x0, size 0x1
    unsigned char bMultiDraw; // offset 0x1, size 0x1
    unsigned char bErase; // offset 0x2, size 0x1
    unsigned char bEraseByMapDraw; // offset 0x3, size 0x1
    unsigned char bPresetMatrix; // offset 0x4, size 0x1
    unsigned char FadeType; // offset 0x5, size 0x1
    unsigned char Priority; // offset 0x6, size 0x1
    unsigned char bUseMdlLight; // offset 0x7, size 0x1
    unsigned char bUseMdlAmbient; // offset 0x8, size 0x1
    unsigned char bUseWorldMat; // offset 0x9, size 0x1
    unsigned char unused0[2]; // offset 0xA, size 0x2
    // total size: 0x680
    struct /* @anon11 */ {
        // Members
        signed int MaxAnimParam; // offset 0x0, size 0x4
        unsigned int TexInVmd; // offset 0x4, size 0x4
        unsigned short MdlNo; // offset 0x8, size 0x2
        unsigned short MdlType; // offset 0xA, size 0x2
        unsigned char bMultiDraw; // offset 0xC, size 0x1
        unsigned char DmyPolyNum; // offset 0xD, size 0x1
        unsigned char DropShadow; // offset 0xE, size 0x1
        char unused0; // offset 0xF, size 0x1
        void * lpVmdHead; // offset 0x10, size 0x4
        void * lpAniHead; // offset 0x14, size 0x4
        void * lpHmdHead; // offset 0x18, size 0x4
        void * lpSmdHead; // offset 0x1C, size 0x4
        void * lpMicHead; // offset 0x20, size 0x4
        void * lpMi3Index; // offset 0x24, size 0x4
        void * lpJcnFileHead; // offset 0x28, size 0x4
        void * lpAnimParam; // offset 0x2C, size 0x4
        void * lpBndFile; // offset 0x30, size 0x4
        void * lpMdlFile; // offset 0x34, size 0x4
        void * lpAniFile; // offset 0x38, size 0x4
        void * lpHdlFile; // offset 0x3C, size 0x4
        void * lpSdlFile; // offset 0x40, size 0x4
        void * lpTexList; // offset 0x44, size 0x4
        void * lpDss; // offset 0x48, size 0x4
        void * lpPrm; // offset 0x4C, size 0x4
        unsigned int * lpObjGroupID; // offset 0x50, size 0x4
        unsigned int * lpPacket; // offset 0x54, size 0x4
        unsigned int * lpCurrent; // offset 0x58, size 0x4
        unsigned int * lpTexPacket; // offset 0x5C, size 0x4
        unsigned int * lpTexCurrent; // offset 0x60, size 0x4
        unsigned int MdlPacketSize; // offset 0x64, size 0x4
        unsigned int TexPacketSize; // offset 0x68, size 0x4
        float aMorphRate[3]; // offset 0x6C, size 0xC
        unsigned int unused[2]; // offset 0x78, size 0x8
        // total size: 0x30
        struct /* @anon52 */ {
            // Members
            float Pos[4]; // offset 0x0, size 0x10
            float Nrm[4]; // offset 0x10, size 0x10
            signed short ObjectId; // offset 0x20, size 0x2
            unsigned char Type; // offset 0x22, size 0x1
            unsigned char Id; // offset 0x23, size 0x1
            unsigned int unused[3]; // offset 0x24, size 0xC
        } DmyPoly[32]; // offset 0x80, size 0x600
    } * lpMdlData; // offset 0xC, size 0x4
    float (* lpObjMat)[4][4]; // offset 0x10, size 0x4
    float (* lpShadowMat)[4][4]; // offset 0x14, size 0x4
    float (* lpLocalMat)[4][4]; // offset 0x18, size 0x4
    void * lpMi3Head; // offset 0x1C, size 0x4
    float Pos[4]; // offset 0x20, size 0x10
    float Ang[4]; // offset 0x30, size 0x10
    float Scl[4]; // offset 0x40, size 0x10
    float Acc[4]; // offset 0x50, size 0x10
    float Dcy[4]; // offset 0x60, size 0x10
    float PosB[4]; // offset 0x70, size 0x10
    float AngB[4]; // offset 0x80, size 0x10
    float LightDir[4]; // offset 0x90, size 0x10
    float LightCol[4]; // offset 0xA0, size 0x10
    float AmbientCol[4]; // offset 0xB0, size 0x10
    float WorldMat[4][4]; // offset 0xC0, size 0x40
    void * lpAnimCtrl; // offset 0x100, size 0x4
    signed short IndexAnim; // offset 0x104, size 0x2
    signed short AnimState; // offset 0x106, size 0x2
    float AnimSpeed; // offset 0x108, size 0x4
    float Alpha; // offset 0x10C, size 0x4
    float HitSize; // offset 0x110, size 0x4
    float HitHigh; // offset 0x114, size 0x4
    float ShadowSize; // offset 0x118, size 0x4
    float ShadowHigh; // offset 0x11C, size 0x4
    signed int Hp; // offset 0x120, size 0x4
    unsigned int timer; // offset 0x124, size 0x4
    unsigned char TeamNo; // offset 0x128, size 0x1
    unsigned char TeamNoBase; // offset 0x129, size 0x1
    unsigned char AttackNo; // offset 0x12A, size 0x1
    unsigned char Flags; // offset 0x12B, size 0x1
    signed short DmgCnt; // offset 0x12C, size 0x2
    signed short NoSearched; // offset 0x12E, size 0x2
    unsigned char HitOff; // offset 0x130, size 0x1
    unsigned char GravOff; // offset 0x131, size 0x1
    unsigned char MapHitOff; // offset 0x132, size 0x1
    unsigned char CharHitOutOff; // offset 0x133, size 0x1
    unsigned int HitFlag; // offset 0x134, size 0x4
    struct _mdlmng * lpHitMap[2]; // offset 0x138, size 0x8
    void * lpParts; // offset 0x140, size 0x4
    void * lpEventData; // offset 0x144, size 0x4
    void * lpCtrlSt; // offset 0x148, size 0x4
    float FadeStep; // offset 0x14C, size 0x4
    // total size: 0x4
    struct /* @anon51 */ {
        // Members
        unsigned int TEST : 19; // offset 0x0, size 0x4
        unsigned int unused1 : 5; // offset 0x0, size 0x4
        unsigned int ZMSK : 1; // offset 0x0, size 0x4
        unsigned int unused2 : 7; // offset 0x0, size 0x4
    } DrawRegs; // offset 0x150, size 0x4
    signed int State[10]; // offset 0x154, size 0x28
    void * lpPathMng; // offset 0x17C, size 0x4
    signed int PLightIndex; // offset 0x180, size 0x4
    float Z_Offset; // offset 0x184, size 0x4
    signed short DebugAnimState; // offset 0x188, size 0x2
    unsigned char CtrlMode; // offset 0x18A, size 0x1
    unsigned char CtrlModeB; // offset 0x18B, size 0x1
    unsigned int * lpShiftTexPacket; // offset 0x18C, size 0x4
    unsigned char sceneNo; // offset 0x190, size 0x1
    unsigned short hSfxPLight; // offset 0x192, size 0x2
    // total size: 0x1
    struct /* @anon41 */ {
        // Members
        unsigned char inhibit_mirror_sfx : 1; // offset 0x0, size 0x1
        unsigned char dmy : 7; // offset 0x0, size 0x1
    } flags; // offset 0x194, size 0x1
    // total size: 0x20
    struct /* @anon39 */ {
        // Members
        float angY; // offset 0x0, size 0x4
        float distXZ; // offset 0x4, size 0x4
        float angStep; // offset 0x8, size 0x4
        unsigned short stayAnim; // offset 0xC, size 0x2
        unsigned short walkAnim; // offset 0xE, size 0x2
        unsigned short prevAnim; // offset 0x10, size 0x2
        signed short unused[5]; // offset 0x12, size 0xA
        struct _mdlmng * lpMdl; // offset 0x1C, size 0x4
    } Target; // offset 0x198, size 0x20
    signed int remnant_HP; // offset 0x1B8, size 0x4
    unsigned short blood_pool_sfx_idx; // offset 0x1BC, size 0x2
} MDLMNG;

extern MDLMNG MdlMng[770]; // size: 0x54380, address: 0x6A0E50


void SetMdlBndFile(signed int Index /* r17 */);
#endif // MODEL_H
