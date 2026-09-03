#ifndef MEMORY_H
#define MEMORY_H
#include "common.h"

typedef struct _allocmng {
    // Members
    unsigned char bUse; // offset 0x0, size 0x1
    unsigned char bFree; // offset 0x1, size 0x1
    unsigned short no; // offset 0x2, size 0x2
    unsigned int addr; // offset 0x4, size 0x4
    unsigned int addrB; // offset 0x8, size 0x4
    unsigned int size; // offset 0xC, size 0x4
    struct _allocmng * prev; // offset 0x10, size 0x4
    struct _allocmng * next; // offset 0x14, size 0x4
    char * lpfuncname; // offset 0x18, size 0x4
} allocmng;

extern u_int LastAddr; // size: 0x4, address: 0x257944

extern u_int UseMemSize; // size: 0x4, address: 0x25794C
extern u_int MaxHeapSize; // size: 0x4, address: 0x257948

__declspec(data) extern int _stack_size; // size: 0x4, address: 0x100000
__declspec(data) extern int _end; // size: 0x4, address: 0xA63380
extern allocmng AllocMng[2048]; // size: 0xE000, address: 0x784BC0



void MemInit();
static u_int FreeMemory(void * lpMem /* r2 */);
static void * AllocateMemory(unsigned int size /* r2 */, unsigned int align /* r2 */, char * lpFuncName /* r2 */, char * lpUseName);
void SetFixMemoryEndIndex();
void MemFree(void * lpMem /* r2 */);
void * MemAllocAlignReal(unsigned int size /* r11 */, unsigned int align /* r2 */, char * lpFuncName /* r2 */, char * lpUseName /* r2 */);
void * MemAllocReal(unsigned int size /* r11 */, char * lpFuncName /* r5 */, char * lpUseName /* r2 */);

#endif
