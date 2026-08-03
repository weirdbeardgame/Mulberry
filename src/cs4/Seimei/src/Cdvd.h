#ifndef CDVD_H
#define CDVD_H

/*
    Compile unit: C:\cs4\Seimei\src\Cdvd.c
    Producer: MW MIPS C Compiler
    Language: C
*/
static // total size: 0x14C
struct /* @anon1 */ {
    // Members
    // total size: 0x24
    struct /* @anon0 */ {
        // Members
        unsigned int lsn; // offset 0x0, size 0x4
        unsigned int size; // offset 0x4, size 0x4
        char name[16]; // offset 0x8, size 0x10
        unsigned char date[8]; // offset 0x18, size 0x8
        unsigned int flag; // offset 0x20, size 0x4
    } cdlfile; // offset 0x0, size 0x24
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
} * lpSetCdvd; // size: 0x4, address: 0x257560
static // total size: 0x14C
struct /* @anon1 */ {
    // Members
    // total size: 0x24
    struct /* @anon0 */ {
        // Members
        unsigned int lsn; // offset 0x0, size 0x4
        unsigned int size; // offset 0x4, size 0x4
        char name[16]; // offset 0x8, size 0x10
        unsigned char date[8]; // offset 0x18, size 0x8
        unsigned int flag; // offset 0x20, size 0x4
    } cdlfile; // offset 0x0, size 0x24
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
} * lpLastCdvd; // size: 0x4, address: 0x257564
static // total size: 0x14C
struct /* @anon1 */ {
    // Members
    // total size: 0x24
    struct /* @anon0 */ {
        // Members
        unsigned int lsn; // offset 0x0, size 0x4
        unsigned int size; // offset 0x4, size 0x4
        char name[16]; // offset 0x8, size 0x10
        unsigned char date[8]; // offset 0x18, size 0x8
        unsigned int flag; // offset 0x20, size 0x4
    } cdlfile; // offset 0x0, size 0x24
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
} * lpCdvdRead; // size: 0x4, address: 0x257568
// total size: 0x14C
struct /* @anon1 */ {
    // Members
    // total size: 0x24
    struct /* @anon0 */ {
        // Members
        unsigned int lsn; // offset 0x0, size 0x4
        unsigned int size; // offset 0x4, size 0x4
        char name[16]; // offset 0x8, size 0x10
        unsigned char date[8]; // offset 0x18, size 0x8
        unsigned int flag; // offset 0x20, size 0x4
    } cdlfile; // offset 0x0, size 0x24
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
} CdvdReadSt[256]; // size: 0x14C00, address: 0xA01200
unsigned int GlobalLoadTimer; // size: 0x4, address: 0x257894

#endif // CDVD_H
