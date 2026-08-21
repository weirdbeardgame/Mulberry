#ifndef TEX_H
#define TEX_H

// total size: 0x8
typedef struct /* @anon16 */ 
{    
    // Members    
    u_long ATE : 1; // offset 0x0, size 0x4    
    u_long ATST : 3; // offset 0x0, size 0x4    
    u_long AREF : 8; // offset 0x0, size 0x4    
    u_long AFAIL : 2; // offset 0x0, size 0x4    
    u_long DATE : 1; // offset 0x0, size 0x4    
    u_long DATM : 1; // offset 0x0, size 0x4    
    u_long ZTE : 1; // offset 0x0, size 0x4    
    u_long ZTST : 2; // offset 0x0, size 0x4    
    u_long pad19 : 45; // offset 0x0, size 0x4    
} TEST; // offset 0xE0, size 0x8

// total size: 0x8    
typedef struct /* @anon21 */ {    
    // Members
    u_long TBP0 : 14; // offset 0x0, size 0x4
    u_long TBW : 6; // offset 0x0, size 0x4
    u_long PSM : 6; // offset 0x0, size 0x4
    u_long TW : 4; // offset 0x0, size 0x4
    u_long TH : 4; // offset 0x0, size 0x4
    u_long TCC : 1; // offset 0x0, size 0x4
    u_long TFX : 2; // offset 0x0, size 0x4
    u_long CBP : 14; // offset 0x0, size 0x4
    u_long CPSM : 4; // offset 0x0, size 0x4
    u_long CSM : 1; // offset 0x0, size 0x4
    u_long CSA : 5; // offset 0x0, size 0x4
    u_long CLD : 3; // offset 0x0, size 0x4    
} 
TEX0; // offset 0xD0, size 0x8

// total size: 0x8    
typedef struct /* @anon25 */
{    
    // Members    
    u_long LCM : 1; // offset 0x0, size 0x4    
    u_long pad01 : 1; // offset 0x0, size 0x4    
    u_long MXL : 3; // offset 0x0, size 0x4    
    u_long MMAG : 1; // offset 0x0, size 0x4    
    u_long MMIN : 3; // offset 0x0, size 0x4    
    u_long MTBA : 1; // offset 0x0, size 0x4    
    u_long pad10 : 9; // offset 0x0, size 0x4    
    u_long L : 2; // offset 0x0, size 0x4    
    u_long pad21 : 11; // offset 0x0, size 0x4
    u_long K : 12; // offset 0x0, size 0x4    
    u_long pad44 : 20; // offset 0x0, size 0x4
} TEX1; // offset 0xD8, size 0x8


void ReadTexList(char * filename /* r2 */, void * * lplpBuf /* r2 */, signed int filetype /* r2 */, signed int index /* r2 */);


#endif // TEX_H
