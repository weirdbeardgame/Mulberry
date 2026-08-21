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


extern struct /* @anon0 */ {
    // Members
    unsigned char bLoad; // offset 0x0, size 0x1
    unsigned char movie_id; // offset 0x1, size 0x1
    unsigned char unused[2]; // offset 0x2, size 0x2
    unsigned short scene_id; // offset 0x4, size 0x2
    unsigned short cam_id; // offset 0x6, size 0x2
}* lpSceneLoad; // size: 0x4, address: 0x257530

extern float VZ[4]; // size: 0x10, address: 0x228720
extern float VW[4]; // size: 0x10, address: 0x228730

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

typedef struct /* @anon6 */
{
    // Members
    u_int EN1 : 1; // offset 0x0, size 0x4
    u_int EN2 : 1; // offset 0x0, size 0x4
    u_int CRTMD : 3; // offset 0x0, size 0x4
    u_int MMOD : 1; // offset 0x0, size 0x4
    u_int AMOD : 1; // offset 0x0, size 0x4
    u_int SLBG : 1; // offset 0x0, size 0x4
    u_int ALP : 8; // offset 0x0, size 0x4
    u_int p0 : 16; // offset 0x0, size 0x4
    u_int p1; // offset 0x4, size 0x4
} PMODE; // offset 0x0, size 0x8

typedef struct /* @anon17 */ {    
    u_int INT : 1; // offset 0x0, size 0x4
    u_int FFMD : 1; // offset 0x0, size 0x4    
    u_int DPMS : 2; // offset 0x0, size 0x4    
    u_int p0 : 28; // offset 0x0, size 0x4
    u_int p1; // offset 0x4, size 0x4
} SMODE2; // offset 0x8, size 0x8

typedef struct /* @anon7 */ {
    u_int FBP : 9; // offset 0x0, size 0x4
    u_int FBW : 6; // offset 0x0, size 0x4
    u_int PSM : 5; // offset 0x0, size 0x4
    u_int p0 : 12; // offset 0x0, size 0x4
    u_int DBX : 11; // offset 0x4, size 0x4
    u_int DBY : 11; // offset 0x4, size 0x4
    u_int p1 : 10; // offset 0x4, size 0x4    
} DISPFB; // offset 0x10, size 0x8

typedef struct /* @anon20 */
{
    u_int DX : 12; // offset 0x0, size 0x4
    u_int DY : 11; // offset 0x0, size 0x4
    u_int MAGH : 4; // offset 0x0, size 0x4
    u_int MAGV : 2; // offset 0x0, size 0x4
    u_int p0 : 3; // offset 0x0, size 0x4
    u_int DW : 12; // offset 0x4, size 0x4
    u_int DH : 11; // offset 0x4, size 0x4
    u_int p1 : 9; // offset 0x4, size 0x4    
} DISPLAY; // offset 0x18, size 0x8
            
// total size: 0x8           
typedef struct /* @anon18 */
{
    // Members
    u_int R : 8; // offset 0x0, size 0x4
    u_int G : 8; // offset 0x0, size 0x4
    u_int B : 8; // offset 0x0, size 0x4
    u_int p0 : 8; // offset 0x0, size 0x4
    u_int p1; // offset 0x4, size 0x4    
} BG_COLOR; // offset 0x20, size 0x8

typedef struct /* @anon0 */
{    
    PMODE pmode;
    SMODE2 smode2;
    DISPFB dispfb;
    DISPLAY display;
    BG_COLOR bgcolor;    
} DISP; // offset 0x0, size 0x50
        
typedef struct /* @anon3 */
{
    u_long NLOOP : 15; // offset 0x0, size 0x4
    u_long EOP : 1; // offset 0x0, size 0x4
    u_long pad16 : 16; // offset 0x0, size 0x4
    u_long id : 14; // offset 0x0, size 0x4
    u_long PRE : 1; // offset 0x0, size 0x4
    u_long PRIM : 11; // offset 0x0, size 0x4
    u_long FLG : 2; // offset 0x0, size 0x4
    u_long NREG : 4; // offset 0x0, size 0x4
    u_long REGS0 : 4; // offset 0x8, size 0x4
    u_long REGS1 : 4; // offset 0x8, size 0x4
    u_long REGS2 : 4; // offset 0x8, size 0x4
    u_long REGS3 : 4; // offset 0x8, size 0x4
    u_long REGS4 : 4; // offset 0x8, size 0x4
    u_long REGS5 : 4; // offset 0x8, size 0x4
    u_long REGS6 : 4; // offset 0x8, size 0x4
    u_long REGS7 : 4; // offset 0x8, size 0x4
    u_long REGS8 : 4; // offset 0x8, size 0x4
    u_long REGS9 : 4; // offset 0x8, size 0x4
    u_long REGS10 : 4; // offset 0x8, size 0x4
    u_long REGS11 : 4; // offset 0x8, size 0x4
    u_long REGS12 : 4; // offset 0x8, size 0x4
    u_long REGS13 : 4; // offset 0x8, size 0x4
    u_long REGS14 : 4; // offset 0x8, size 0x4
    u_long REGS15 : 4; // offset 0x8, size 0x4
} GIFTAG0; // offset 0x50, size 0x10

typedef struct /* @anon15 */   
{    
    u_long FBP : 9; // offset 0x0, size 0x4
    u_long pad09 : 7; // offset 0x0, size 0x4
    u_long FBW : 6; // offset 0x0, size 0x4
    u_long pad22 : 2; // offset 0x0, size 0x4
    u_long PSM : 6; // offset 0x0, size 0x4
    u_long pad30 : 2; // offset 0x0, size 0x4
    u_long FBMSK : 32; // offset 0x0, size 0x4    
} FRAME1; // offset 0x0, size 0x8


typedef struct /* @anon26 */ {
    u_long ZBP : 9; // offset 0x0, size 0x4
    u_long pad09 : 15; // offset 0x0, size 0x4
    u_long PSM : 4; // offset 0x0, size 0x4
    u_long pad28 : 4; // offset 0x0, size 0x4
    u_long ZMSK : 1; // offset 0x0, size 0x4
    u_long pad33 : 31; // offset 0x0, size 0x4    
} ZBUF1; // offset 0x10, size 0x8


typedef struct /* @anon22 */ 
{
    u_long OFX : 16; // offset 0x0, size 0x4
    u_long pad16 : 16; // offset 0x0, size 0x4
    u_long OFY : 16; // offset 0x0, size 0x4
    u_long pad48 : 16; // offset 0x0, size 0x4    
} XYOFFSET1; // offset 0x20, size 0x8
    
typedef struct /* @anon10 */ {
    u_long SCAX0 : 11; // offset 0x0, size 0x4
    u_long pad11 : 5; // offset 0x0, size 0x4
    u_long SCAX1 : 11; // offset 0x0, size 0x4
    u_long pad27 : 5; // offset 0x0, size 0x4
    u_long SCAY0 : 11; // offset 0x0, size 0x4
    u_long pad43 : 5; // offset 0x0, size 0x4
    u_long SCAY1 : 11; // offset 0x0, size 0x4
    u_long pad59 : 5; // offset 0x0, size 0x4    
} SCISSOR1; // offset 0x30, size 0x8

typedef struct /* @anon1 */ {
    u_long AC : 1; // offset 0x0, size 0x4
    u_long pad01 : 63; // offset 0x0, size 0x4    
} PRMODECONT; // offset 0x40, size 0x8

typedef struct /* @anon24 */ {
    u_long CLAMP : 1; // offset 0x0, size 0x4
    u_long pad01 : 63; // offset 0x0, size 0x4    
} COL_CLAMP; // offset 0x50, size 0x8


typedef struct /* @anon5 */
{
    u_long DTHE : 1; // offset 0x0, size 0x4
    u_long pad01 : 63; // offset 0x0, size 0x4    
} DTHE; // offset 0x60, size 0x8
    
typedef struct /* @anon19 */
{
    u_long ATE : 1; // offset 0x0, size 0x4
    u_long ATST : 3; // offset 0x0, size 0x4
    u_long AREF : 8; // offset 0x0, size 0x4
    u_long AFAIL : 2; // offset 0x0, size 0x4
    u_long DATE : 1; // offset 0x0, size 0x4
    u_long DATM : 1; // offset 0x0, size 0x4
    u_long ZTE : 1; // offset 0x0, size 0x4
    u_long ZTST : 2; // offset 0x0, size 0x4
    u_long pad19 : 45; // offset 0x0, size 0x4    
} TEST1; // offset 0x70, size 0x8

typedef struct /* @anon2 */
{
    FRAME1 frame1;
    u_long frame1addr; // offset 0x8, size 0x4
    long zbuf1addr; // offset 0x18, size 0x4
    ZBUF1 zbuf1;
    XYOFFSET1 xyoffset1;
    long xyoffset1addr; // offset 0x28, size 0x4
    SCISSOR1 scissor1;
    long scissor1addr; // offset 0x38, size 0x4
    PRMODECONT prmodecont;
    long prmodecontaddr; // offset 0x48, size 0x4
    COL_CLAMP colclamp;
    long colclampaddr; // offset 0x58, size 0x4
    DTHE dthe;
    long dtheaddr; // offset 0x68, size 0x4
    TEST1 test1;
    long test1addr; // offset 0x78, size 0x4
    
} DRAW0; // offset 0x60, size 0x80

typedef struct /* @anon19 */ {
    u_long ATE : 1; // offset 0x0, size 0x4
    u_long ATST : 3; // offset 0x0, size 0x4
    u_long AREF : 8; // offset 0x0, size 0x4
    u_long AFAIL : 2; // offset 0x0, size 0x4
    u_long DATE : 1; // offset 0x0, size 0x4
    u_long DATM : 1; // offset 0x0, size 0x4
    u_long ZTE : 1; // offset 0x0, size 0x4
    u_long ZTST : 2; // offset 0x0, size 0x4
    u_long pad19 : 45; // offset 0x0, size 0x4            
} TESTA; // offset 0x0, size 0x8

            
typedef struct /* @anon27 */
{
    u_long PRIM : 3; // offset 0x0, size 0x4
    u_long IIP : 1; // offset 0x0, size 0x4
    u_long TME : 1; // offset 0x0, size 0x4
    u_long FGE : 1; // offset 0x0, size 0x4
    u_long ABE : 1; // offset 0x0, size 0x4
    u_long AA1 : 1; // offset 0x0, size 0x4
    u_long FST : 1; // offset 0x0, size 0x4
    u_long CTXT : 1; // offset 0x0, size 0x4
    u_long FIX : 1; // offset 0x0, size 0x4
    u_long pad11 : 53; // offset 0x0, size 0x4
} PRIM; // offset 0x10, size 0x8
 
typedef struct /* @anon4 */ 
{
    u_int R : 8; // offset 0x0, size 0x4
    u_int G : 8; // offset 0x0, size 0x4
    u_int B : 8; // offset 0x0, size 0x4
    u_int A : 8; // offset 0x0, size 0x4
    float Q; // offset 0x4, size 0x4            
} RGBAQ; // offset 0x20, size 0x8

typedef struct /* @anon23 */
{
    // Members
    u_long X : 16; // offset 0x0, size 0x4
    u_long Y : 16; // offset 0x0, size 0x4
    u_long Z : 32; // offset 0x0, size 0x4
} XYZ2A; // offset 0x30, size 0x8

typedef struct /* @anon23 */ 
{
    // Members
    u_long X : 16; // offset 0x0, size 0x4
    u_long Y : 16; // offset 0x0, size 0x4
    u_long Z : 32; // offset 0x0, size 0x4    
} XYZ2B; // offset 0x40, size 0x8

            
typedef struct /* @anon19 */
{
    u_long ATE : 1; // offset 0x0, size 0x4
    u_long ATST : 3; // offset 0x0, size 0x4
    u_long AREF : 8; // offset 0x0, size 0x4
    u_long AFAIL : 2; // offset 0x0, size 0x4
    u_long DATE : 1; // offset 0x0, size 0x4
    u_long DATM : 1; // offset 0x0, size 0x4
    u_long ZTE : 1; // offset 0x0, size 0x4
    u_long ZTST : 2; // offset 0x0, size 0x4
    u_long pad19 : 45; // offset 0x0, size 0x4    
} TESTB; // offset 0x50, size 0x8

typedef struct /* @anon21 */
{
    TESTA testa;
    long testaaddr; // offset 0x8, size 0x4
    PRIM prim;
    long primaddr; // offset 0x18, size 0x4
    RGBAQ rgbaq;
    long rgbaqaddr; // offset 0x28, size 0x4
    XYZ2A xyz2a;
    long xyz2aaddr; // offset 0x38, size 0x4
    XYZ2B xyz2b;            
    long xyz2baddr; // offset 0x48, size 0x4
    TESTB testb;
    long testbaddr; // offset 0x58, size 0x4        
} CLEAR0; // offset 0xE0, size 0x60

typedef struct /* @anon3 */
{
    u_long NLOOP : 15; // offset 0x0, size 0x4
    u_long EOP : 1; // offset 0x0, size 0x4
    u_long pad16 : 16; // offset 0x0, size 0x4
    u_long id : 14; // offset 0x0, size 0x4
    u_long PRE : 1; // offset 0x0, size 0x4
    u_long PRIM : 11; // offset 0x0, size 0x4
    u_long FLG : 2; // offset 0x0, size 0x4
    u_long NREG : 4; // offset 0x0, size 0x4
    u_long REGS0 : 4; // offset 0x8, size 0x4
    u_long REGS1 : 4; // offset 0x8, size 0x4
    u_long REGS2 : 4; // offset 0x8, size 0x4
    u_long REGS3 : 4; // offset 0x8, size 0x4
    u_long REGS4 : 4; // offset 0x8, size 0x4
    u_long REGS5 : 4; // offset 0x8, size 0x4
    u_long REGS6 : 4; // offset 0x8, size 0x4
    u_long REGS7 : 4; // offset 0x8, size 0x4
    u_long REGS8 : 4; // offset 0x8, size 0x4
    u_long REGS9 : 4; // offset 0x8, size 0x4
    u_long REGS10 : 4; // offset 0x8, size 0x4
    u_long REGS11 : 4; // offset 0x8, size 0x4
    u_long REGS12 : 4; // offset 0x8, size 0x4
    u_long REGS13 : 4; // offset 0x8, size 0x4
    u_long REGS14 : 4; // offset 0x8, size 0x4    
    u_long REGS15 : 4; // offset 0x8, size 0x4    
} GIFTAG1; // offset 0x140, size 0x10

typedef struct /* @anon2 */
{
    FRAME1 frame1;
    u_long frame1addr;
    ZBUF1 zbuf1;
    long zbuf1addr;
    XYOFFSET1 xyoffset1;
    long xyoffset1addr;
    SCISSOR1 scissor1;
    long scissor1addr;
    PRMODECONT prmodecont;
    long prmodecontaddr;
    COL_CLAMP colclamp;
    long colclampaddr;
    DTHE dthe;
    long dtheaddr;
    TEST1 test1;     
    long test1addr;      
} DRAW1; // offset 0x150, size 0x80
        
// total size: 0x60        
typedef struct /* @anon21 */ 
{
    TESTA testa;
    long testaaddr;
    PRIM prim;
    long primaddr;
    RGBAQ rgbaq;
    long rgbaqaddr;
    XYZ2A xyz2a;
    long xyz2aaddr;
    XYZ2B xyz2b;     
    long xyz2baddr;
    TESTB testb;
    long testbaddr;
} CLEAR1; // offset 0x1D0, size 0x60   

typedef struct /* @anon25 */
{
    DISP disp[2];
    GIFTAG0 giftag0;
    DRAW0 draw0;
    CLEAR0 clear0;
    GIFTAG1 giftag1;
    DRAW1 draw1;
    CLEAR1 clear1;

} DB_BUF; // offset 0x0, size 0x230

// total size: 0x470
typedef struct /* @anon8 */
{
    DB_BUF dbBuf;    
    float camPos[4]; // offset 0x230, size 0x10
    float camAng[4]; // offset 0x240, size 0x10
    float camPosB[4]; // offset 0x250, size 0x10
    float camAngB[4]; // offset 0x260, size 0x10
    float vsMat[4][4]; // offset 0x270, size 0x40
    float wsMat[4][4]; // offset 0x2B0, size 0x40
    float wvMat[4][4]; // offset 0x2F0, size 0x40
    float lcMat[4][4]; // offset 0x330, size 0x40
    float wcMat[4][4]; // offset 0x370, size 0x40
    float csMat[4][4]; // offset 0x3B0, size 0x40
    float scrDepth; // offset 0x3F0, size 0x4
    u_int * sprBuf[2]; // offset 0x3F4, size 0x8
    signed int sprBufID; // offset 0x3FC, size 0x4
    u_int DMATag; // offset 0x400, size 0x4
    signed int ctxt; // offset 0x404, size 0x4
    signed int dbBufID; // offset 0x408, size 0x4
    signed int oddeven; // offset 0x40C, size 0x4
    u_int calcTime; // offset 0x410, size 0x4
    u_int drawTime; // offset 0x414, size 0x4
    u_int frameTime; // offset 0x418, size 0x4
    float fov; // offset 0x41C, size 0x4
    signed int TotalPolyNum; // offset 0x420, size 0x4
    signed int TotalPrimNum; // offset 0x424, size 0x4
    float WaterLevel; // offset 0x428, size 0x4
    u_char DrawFlag; // offset 0x42C, size 0x1
    u_char DrawHitModel; // offset 0x42D, size 0x1
    u_char BlurMode; // offset 0x42E, size 0x1
    u_char BlurRate; // offset 0x42F, size 0x1
    u_char ProjectionType; // offset 0x430, size 0x1
    u_char unused[15]; // offset 0x431, size 0xF
    float PlaneParam[4]; // offset 0x440, size 0x10
    float FogParam[4]; // offset 0x450, size 0x10
    float FogParam16[4]; // offset 0x460, size 0x10
} DRAW_MNG; // size: 0x4, address: 0x257500

typedef struct /* @anon0 */ 
{
    // Members
    u_char r; // offset 0x0, size 0x1
    u_char g; // offset 0x1, size 0x1
    u_char b; // offset 0x2, size 0x1
    u_char a; // offset 0x3, size 0x1
} COLOR;

extern TMP_MENU_MNG_ST g_TmpMenuMng; // size: 0x38, address: 0xA00C70
extern u_int * lpMdlPacketMem; // size: 0x4, address: 0x2578B0
extern DRAW_MNG *lpDrawMng;

#endif // UNK_H
