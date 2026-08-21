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

void GetAllBindInfo(char * fname /* r2 */, unsigned int * lsn /* r2 */, unsigned int * size /* r2 */);
void SetAllBindInfo(void);
unsigned int GetBindNum(LPBINDHEAD *lpBindHead /* r2 */);
unsigned int GetBindOffsetByName(LPBINDHEAD *lpBindHead, char *fname);
unsigned int GetBindOffsetByIndex(LPBINDHEAD *lpBindHead, u_short Index);
void RemapBinderFile(void *lpBuf /* r2 */);

#endif // BINDER_H