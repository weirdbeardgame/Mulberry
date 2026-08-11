#ifndef BINDER_H
#define BINDER_H
#include "typedefs.h"

// Range: 0x15E440 -> 0x15E760
        // total size: 0x10
typedef struct /* @anon0 */ {
    char appname[4]; // offset 0x0, size 0x4
    unsigned int version; // offset 0x4, size 0x4        
    unsigned int filesize; // offset 0x8, size 0x4        
    unsigned int filenum; // offset 0xC, size 0x4        
} LPBINDHEAD; // r18
        
// total size: 0x10        
typedef struct /* @anon6 */ 
{
    unsigned short fileID; // offset 0x0, size 0x2
    unsigned short groupID; // offset 0x2, size 0x2
    unsigned int dataoffset; // offset 0x4, size 0x4        
    unsigned int filesize; // offset 0x8, size 0x4        
    unsigned int nameoffset; // offset 0xC, size 0x4        
} LPOFFSET; // r17
        
// total size: 0x8C        
typedef struct /* @anon5 */
{      
    char filename[128]; // offset 0x0, size 0x80        
    unsigned int lsn; // offset 0x80, size 0x4        
    unsigned int size; // offset 0x84, size 0x4        
    unsigned int hash; // offset 0x88, size 0x4        
} LPALLBINDINFO; // r16

typedef struct {
    // Members
    unsigned short fileID; // offset 0x0, size 0x2
    unsigned short groupID; // offset 0x2, size 0x2
    unsigned int dataoffset; // offset 0x4, size 0x4
    unsigned int nameoffset; // offset 0x8, size 0x4
} SHORT_OFFSET; // r16

typedef struct /* @anon5 */ {
    // Members
    char filename[128]; // offset 0x0, size 0x80
    unsigned int lsn; // offset 0x80, size 0x4
    unsigned int size; // offset 0x84, size 0x4
    unsigned int hash; // offset 0x88, size 0x4
} BINDINFO;

static unsigned int AllBindDataNum; // size: 0x4, address: 0x257964

static BINDINFO AllBindInfo[3200];

static inline unsigned int foo_fn(char* fname) {
    unsigned int hash = 0;
    int i = 0;
    int foo[32] = {
        0x02,           0x03,           0x05,           0x07,
        0x0B,           0x0D,           0x11,           0x13,
        0x17,           0x1D,           0x1F,           0x25,
        0x29,           0x2B,           0x2F,           0x35,
        0x3B,           0x3D,           0x43,           0x47,
        0x49,           0x4F,           0x53,           0x59,
        0x61,           0x65,           0x67,           0x6B,
        0x6D,           0x71,           0x7F,           0x83
    };
    
    int bar[32] = {
        0x007F,         0x0161,         0x025F,         0x036D,
        0x0481,         0x05AD,         0x06CD,         0x080F,
        0x0943,         0x0A81,         0x0BB9,         0x0D0F,
        0x0E4B,         0x0FA1,         0x10E7,         0x1237,
        0x1391,         0x1511,         0x1645,         0x17B3,
        0x18E5,         0x1A6B,         0x1BC5,         0x1D53,
        0x1EA1,         0x201D,         0x2197,         0x22E5,
        0x244D,         0x25AB,         0x2719,         0x2875
    };
    
    for (; *fname != 0; fname++) {
        
        if (++i >= 32u) {
            do { } while( 1 );
        }
        hash += bar[i] * foo[i] * *fname;
    }
    return hash;
}



void GetAllBindInfo(char * fname /* r2 */, unsigned int * lsn /* r2 */, unsigned int * size /* r2 */);
void SetAllBindInfo(void);
unsigned int GetBindNum(LPBINDHEAD *lpBindHead /* r2 */);
unsigned int GetBindOffsetByName(LPBINDHEAD *lpBindHead, char *fname);
unsigned int GetBindOffsetByIndex(LPBINDHEAD *lpBindHead, u_short Index);
void RemapBinderFile(void *lpBuf /* r2 */);

#endif // BINDER_H