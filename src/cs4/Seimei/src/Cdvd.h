#ifndef CDVD_H
#define CDVD_H
#include "common.h"
#define SECTOR_SIZE (2048)

typedef struct /* @anon4 */ {
    // Members
    unsigned int lsn; // offset 0x0, size 0x4
    unsigned int size; // offset 0x4, size 0x4
    char name[16]; // offset 0x8, size 0x10
    unsigned char date[8]; // offset 0x18, size 0x8
    unsigned int flag; // offset 0x20, size 0x4
} CDLFILE;

/*
    Compile unit: C:\cs4\Seimei\src\Cdvd.c
    Producer: MW MIPS C Compiler
    Language: C
*/
typedef struct /* @anon1 */ {
    // Members
    // total size: 0x24
    CDLFILE cdlfile;
    char fname[128]; // offset 0x24, size 0x80
    char sname[128]; // offset 0xA4, size 0x80
    signed int state; // offset 0x124, size 0x4
    signed int fileType; // offset 0x128, size 0x4
    signed int index; // offset 0x12C, size 0x4
    signed int fhd; // offset 0x130, size 0x4
    signed int size; // offset 0x134, size 0x4
    signed int sector; // offset 0x138, size 0x4
    unsigned int align; // offset 0x13C, size 0x4
    unsigned char retry; // offset 0x140, size 0x1
    unsigned char unused[3]; // offset 0x141, size 0x3
    unsigned char * * lplpAddr; // offset 0x144, size 0x4
    unsigned char * lpDmyData; // offset 0x148, size 0x4
} LPSETCDVD ; // size: 0x4, address: 0x257560

typedef struct /* @anon1 */ {
    CDLFILE cdlfile;
    char fname[128]; // offset 0x24, size 0x80
    char sname[128]; // offset 0xA4, size 0x80
    signed int state; // offset 0x124, size 0x4
    signed int fileType; // offset 0x128, size 0x4
    signed int index; // offset 0x12C, size 0x4
    signed int fhd; // offset 0x130, size 0x4
    signed int size; // offset 0x134, size 0x4
    signed int sector; // offset 0x138, size 0x4
    unsigned int align; // offset 0x13C, size 0x4
    unsigned char retry; // offset 0x140, size 0x1
    unsigned char unused[3]; // offset 0x141, size 0x3
    unsigned char * * lplpAddr; // offset 0x144, size 0x4
    unsigned char * lpDmyData; // offset 0x148, size 0x4
} LPLASTCDVD; // size: 0x4, address: 0x257564

typedef struct /* @anon1 */ {

    CDLFILE cdlfile;

    char fname[128]; // offset 0x24, size 0x80
    char sname[128]; // offset 0xA4, size 0x80
    signed int state; // offset 0x124, size 0x4
    signed int fileType; // offset 0x128, size 0x4
    signed int index; // offset 0x12C, size 0x4
    signed int fhd; // offset 0x130, size 0x4
    signed int size; // offset 0x134, size 0x4
    signed int sector; // offset 0x138, size 0x4
    unsigned int align; // offset 0x13C, size 0x4
    unsigned char retry; // offset 0x140, size 0x1
    unsigned char unused[3]; // offset 0x141, size 0x3
    unsigned char * * lplpAddr; // offset 0x144, size 0x4
    unsigned char * lpDmyData; // offset 0x148, size 0x4
} LPCDVDREAD; // size: 0x4, address: 0x257568

typedef struct /* @anon1 */ {
    // Members
    // total size: 0x24
    CDLFILE cdlfile;
    char fname[128]; // offset 0x24, size 0x80
    char sname[128]; // offset 0xA4, size 0x80
    signed int state; // offset 0x124, size 0x4
    signed int fileType; // offset 0x128, size 0x4
    signed int index; // offset 0x12C, size 0x4
    signed int fhd; // offset 0x130, size 0x4
    signed int size; // offset 0x134, size 0x4
    signed int sector; // offset 0x138, size 0x4
    unsigned int align; // offset 0x13C, size 0x4
    unsigned char retry; // offset 0x140, size 0x1
    unsigned char unused[3]; // offset 0x141, size 0x3
    unsigned char * * lplpAddr; // offset 0x144, size 0x4
    unsigned char * lpDmyData; // offset 0x148, size 0x4
} CDVDREADST; // size: 0x14C00, address: 0xA01200

/*
    Compile unit: C:\cs4\Seimei\src\Binder.c
    Producer: MW MIPS C Compiler
    Language: C
*/
typedef struct /* @anon3 */ {
    // Members
    // total size: 0x24
// offset 0x0, size 0x24
    CDLFILE cdlfile;   
    char fname[128]; // offset 0x24, size 0x80
    char sname[128]; // offset 0xA4, size 0x80
    signed int state; // offset 0x124, size 0x4
    signed int fileType; // offset 0x128, size 0x4
    signed int index; // offset 0x12C, size 0x4
    signed int fhd; // offset 0x130, size 0x4
    signed int size; // offset 0x134, size 0x4
    signed int sector; // offset 0x138, size 0x4
    unsigned int align; // offset 0x13C, size 0x4
    unsigned char retry; // offset 0x140, size 0x1
    unsigned char unused[3]; // offset 0x141, size 0x3
    unsigned char * * lplpAddr; // offset 0x144, size 0x4
    unsigned char * lpDmyData; // offset 0x148, size 0x4
} LPCDVD; // size: 0x4, address: 0x257528

extern unsigned int GlobalLoadTimer; // size: 0x4, address: 0x257894
static CDVDREADST CdvdReadSt[256];
static LPSETCDVD* lpSetCdvd = (LPSETCDVD* )CdvdReadSt;
static LPLASTCDVD* lpLastCdvd = (LPLASTCDVD* )CdvdReadSt;
static LPCDVDREAD* lpCdvdRead = (LPCDVDREAD* )CdvdReadSt;

void InitCdvdRead(void);
void MakeCdvdReadPath(s8* full, s8* root, s8* fname);
void MakeCdvdSearchPath(s8* full, s8* fname);
int CdvdRead(char * filename, int size, u_char * * lpBuf, u_char bSynch, int index, int filetype, u_int align);
u_int CdvdAsyncProc(int bDraw);
void CdvdBackReadProc(void);
int CdvdCheckAsyncBusy(void);

#endif // CDVD_H
