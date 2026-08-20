#ifndef SPRITE_H
#define SPRITE_H
#include "common.h"
#include "Tex.h"

typedef struct /* @anon0 */
{
    u_short bUse : 1; // offset 0x0, size 0x2
    u_short Fst : 1; // offset 0x0, size 0x2
    u_short Alpha : 2; // offset 0x0, size 0x2
    u_short LoadOff : 1; // offset 0x0, size 0x2
    u_short unused : 11; // offset 0x0, size 0x2
    u_short texID; // offset 0x2, size 0x2
    int ver[4][4]; // offset 0x10, size 0x40
    int col[4][4]; // offset 0x50, size 0x40
    union { // inferred
        int uv[4][4]; // offset 0x90, size 0x40
        float st[4][4]; // offset 0x90, size 0x40        
    };        
    
    TEX0 tex0;        
    TEX1 tex1;
    TEST test;
} SPDATA;

typedef struct /* @anon10 */ {
    // Members
    u_short bUse : 1; // offset 0x0, size 0x2
    u_short Fst : 1; // offset 0x0, size 0x2
    u_short Alpha : 2; // offset 0x0, size 0x2
    u_short LoadOff : 1; // offset 0x0, size 0x2
    u_short unused : 11; // offset 0x0, size 0x2
    u_short texID; // offset 0x2, size 0x2
    signed int ver[4][4]; // offset 0x10, size 0x40
    signed int col[4][4]; // offset 0x50, size 0x40
    union { // inferred
        signed int uv[4][4]; // offset 0x90, size 0x40
        float st[4][4]; // offset 0x90, size 0x40
    };

    TEX0 tex0;
    TEX1 tex1;
    TEST test;
} LPSP;



    u_int * MakeSpritePacketUseSpr( LPSP * lpSp /* r2 */, u_int * lpPacket /* r2 */);
    void SetSprite(LPSP * lpSp, int PosX /* r21 */, int PosY /* r20 */, 
               int w /* r19 */, int h /* r18 */, int * col /* r17 */, 
               u_short texID /* r16 */, u_char Fst /* r23 */);
#endif // SPRITE_H