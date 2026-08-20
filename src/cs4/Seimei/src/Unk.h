#ifndef UNK_H
#define UNK_H

#include "Model.h"

typedef struct _TMP_MENU_MNG_ST {
    // Members
    _MNG_STATE_ST mng_state; // offset 0x0, size 0x20
    int DrawState; // offset 0x20, size 0x4
    u_char** pTmpMenuFileBuf; // offset 0x24, size 0x4
    float draw_up_rate; // offset 0x28, size 0x4
    u_char dmy[12]; // offset 0x2C, size 0xC
} TMP_MENU_MNG_ST;

extern TMP_MENU_MNG_ST g_TmpMenuMng; // size: 0x38, address: 0xA00C70

extern struct /* @anon0 */ {
    // Members
    unsigned char bLoad; // offset 0x0, size 0x1
    unsigned char movie_id; // offset 0x1, size 0x1
    unsigned char unused[2]; // offset 0x2, size 0x2
    unsigned short scene_id; // offset 0x4, size 0x2
    unsigned short cam_id; // offset 0x6, size 0x2
}* lpSceneLoad; // size: 0x4, address: 0x257530

static float VZ[4]; // size: 0x10, address: 0x228720
static float VW[4]; // size: 0x10, address: 0x228730

typedef enum /* @enum */ {
    HSYN_PORT_SQBGM = 0,
    HSYN_PORT_SQBGM1 = 1,
    HSYN_PORT_SQBGM2 = 2,
    HSYN_PORT_SQBGM3 = 3,
    HSYN_PORT_SE_0 = 4,
    HSYN_PORT_SE_1 = 5,
    HSYN_PORT_SE_2 = 6,
    HSYN_PORT_SE_3 = 7,
    HSYN_PORT_SE_4 = 8,
    HSYN_PORT_SE_5 = 9,
    HSYN_PORT_SE_6 = 10,
    HSYN_PORT_SE_7 = 11,
    HSYN_PORT_SE_8 = 12,
    HSYN_PORT_SE_9 = 13,
    HSYN_PORT_SE_10 = 14,
    HSYN_PORT_SE_11 = 15,
    HSYN_PORT_MAX = 16,
} PORT;

typedef enum /* @enum */ {
    SND_SQSONG_FADE_OUT = 0,
    SND_SQSONG_FADE_OUT_STATE_QUO = 1,
    SND_SQSONG_FADE_IN = 2,
    SND_SQSONG_FADE_STATE_QUO = 3,
} FADE;

typedef enum {
    TMP_MENU__UDDRAW_ORDER__ON = 0,
    TMP_MENU__UDDRAW_ORDER__OFF = 1,
    TMP_MENU__UDDRAW_ORDER__OFF_FORCE = 2,
} _TMP_MENU__UDDRAW_ORDER_EN;

// Members
// total size: 0x1A0
typedef struct /* @anon10 */
{
    // Members
    void (* ThkFunc[32])(); // offset 0x0, size 0x80
    void (* ActFunc[32])(); // offset 0x80, size 0x80
    void (* SetActFunc[32])(); // offset 0x100, size 0x80
    u_char actID; // offset 0x180, size 0x1        
    u_char actIDB; // offset 0x181, size 0x1
    u_char freeEnd; // offset 0x182, size 0x1
    u_char unused0[1]; // offset 0x183, size 0x1
    int (* GetDmgAnimFunc)(); // offset 0x184, size 0x4
    int (* CalcDmgFunc)(); // offset 0x188, size 0x4
    void (* FrameLastFunc)(); // offset 0x18C, size 0x4        
    void (* DramaStartFunc)(); // offset 0x190, size 0x4        
    void (* DramaEndFunc)(); // offset 0x194, size 0x4
    int unused[2]; // offset 0x198, size 0x8       
} FUNC; // offset 0x0, size 0x1A0
                    
typedef struct /* @anon15 */
{
    // Members
    u_short select : 1; // offset 0x0, size 0x2
    u_short L3 : 1; // offset 0x0, size 0x2
    u_short R3 : 1; // offset 0x0, size 0x2
    u_short start : 1; // offset 0x0, size 0x2
    u_short Lup : 1; // offset 0x0, size 0x2
    u_short Lright : 1; // offset 0x0, size 0x2
    u_short Ldown : 1; // offset 0x0, size 0x2                    
    u_short Lleft : 1; // offset 0x0, size 0x2                    
    u_short L2 : 1; // offset 0x0, size 0x2                    
    u_short R2 : 1; // offset 0x0, size 0x2                    
    u_short L1 : 1; // offset 0x0, size 0x2                    
    u_short R1 : 1; // offset 0x0, size 0x2                    
    u_short Rup : 1; // offset 0x0, size 0x2                    
    u_short Rdown : 1; // offset 0x0, size 0x2                    
    u_short Rright : 1; // offset 0x0, size 0x2                    
    u_short Rleft : 1; // offset 0x0, size 0x2                    
} BIT; // offset 0x0, size 0x2
     
// total size: 0xC
typedef struct /* @anon33 */ {                
    // Members
    u_char Lright; // offset 0x0, size 0x1
    u_char Lleft; // offset 0x1, size 0x1
    u_char Lup; // offset 0x2, size 0x1
    u_char Ldown; // offset 0x3, size 0x1
    u_char Rup; // offset 0x4, size 0x1
    u_char Rright; // offset 0x5, size 0x1
    u_char Rdown; // offset 0x6, size 0x1
    u_char Rleft; // offset 0x7, size 0x1
    u_char L1; // offset 0x8, size 0x1
    u_char R1; // offset 0x9, size 0x1
    u_char L2; // offset 0xA, size 0x1
    u_char R2; // offset 0xB, size 0x1
} PRESS; // offset 0x14, size 0xC

// Members        
// total size: 0x24
typedef struct /* @anon23 */
{
    // Members
    u_char success; // offset 0x0, size 0x1
    u_char data_len; // offset 0x1, size 0x1    
    union /* @anon31 */    
    {
        // total size: 0x2
        BIT bit; // offset 0x0, size 0x2
        u_short set; // offset 0x0, size 0x2
    } button; // offset 0x2, size 0x2
    u_char r3h; // offset 0x4, size 0x1
    u_char r3v; // offset 0x5, size 0x1
    u_char l3h; // offset 0x6, size 0x1
    u_char l3v; // offset 0x7, size 0x1     
    u_char apress[12]; // offset 0x8, size 0xC
    PRESS press;            
    union /* @anon31 */
    {
        BIT bit;
        u_short set; // offset 0x0, size 0x2
    } edge; // offset 0x20, size 0x2            
    
    union /* @anon31 */
    {
        BIT bit;            
        u_short set; // offset 0x0, size 0x2            
    } repeat; // offset 0x22, size 0x2            
} LPPAD; // offset 0x0, size 0x4

// total size: 0x10
typedef struct /* @anon20 */
{
    LPPAD * lpPad; // offset 0x0, size 0x4
    char L3H; // offset 0x4, size 0x1
    char L3V; // offset 0x5, size 0x1
    char R3H; // offset 0x6, size 0x1
    char R3V; // offset 0x7, size 0x1
    u_int unused[2]; // offset 0x8, size 0x8        
} KEY; // offset 0x1A0, size 0x10

typedef struct /* @anon1 */ 
{
    FUNC func;
    KEY key;
    float InitPos[4]; // offset 0x1B0, size 0x10
    float DstPos[4]; // offset 0x1C0, size 0x10
    float moveRange; // offset 0x1D0, size 0x4
    float moveAngle; // offset 0x1D4, size 0x4
    u_char bDash; // offset 0x1D8, size 0x1
    u_char Rebirth; // offset 0x1D9, size 0x1
    u_char bHitOff; // offset 0x1DA, size 0x1
    u_char bDead; // offset 0x1DB, size 0x1
    u_short stayTimer; // offset 0x1DC, size 0x2
    u_short walkTimer; // offset 0x1DE, size 0x2
    u_short approachTimer; // offset 0x1E0, size 0x2
    u_short ClingDmgMyAnim; // offset 0x1E2, size 0x2
    MDLMNG * lpCling; // offset 0x1E4, size 0x4
    int attackOff; // offset 0x1E8, size 0x4
    short RebirthTimer; // offset 0x1EC, size 0x2
    u_short PauseTime; // offset 0x1EE, size 0x2
    u_char bWeak; // offset 0x1F0, size 0x1 
    u_char UseSfxHdlNum; // offset 0x1F1, size 0x1    
    u_char bFired; // offset 0x1F2, size 0x1    
    u_char bFree; // offset 0x1F3, size 0x1    
    float rollAng; // offset 0x1F4, size 0x4    
    u_short lifecut; // offset 0x1F8, size 0x2    
    u_short noDamage; // offset 0x1FA, size 0x2    
    short DisableSearch; // offset 0x1FC, size 0x2
    short GuardFrame; // offset 0x1FE, size 0x2
    int SfxHdl[8]; // offset 0x200, size 0x20
    int SeHdl[8]; // offset 0x220, size 0x20    
    MDLMNG * lpP019; // offset 0x240, size 0x4    
    u_char lifecutscl; // offset 0x244, size 0x1    
    u_char captureLock; // offset 0x245, size 0x1    
    u_char ClingPos; // offset 0x246, size 0x1    
    u_char ClingFree; // offset 0x247, size 0x1
    u_char NoSave; // offset 0x248, size 0x1
    u_char FreeDamage; // offset 0x249, size 0x1    
    u_char bEraseDisable; // offset 0x24A, size 0x1    
    u_char NoFloorDeadTimer; // offset 0x24B, size 0x1    
    u_char bNoFloorDeadDisable; // offset 0x24C, size 0x1   
    u_char AppearTypeAlpha; // offset 0x24D, size 0x1    
    u_char unused0[2]; // offset 0x24E, size 0x2    
    float PowerScl; // offset 0x250, size 0x4
    int MaxHp; // offset 0x254, size 0x4
    int ItemId; // offset 0x258, size 0x4
    float SclLimit; // offset 0x25C, size 0x4    
} PENECTRL;

#endif // UNK_H
