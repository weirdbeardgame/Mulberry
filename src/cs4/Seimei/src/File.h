#ifndef FILE_H
#define FILE_H
#include "common.h"

extern unsigned char RootPath[128]; // size: 0x80, address: 0x325530
extern unsigned char ScePath[128]; // size: 0x80, address: 0x3254B0

void InitFileRead();
void InitFileSystem();
void ReadRootPath();
unsigned char * GetRootPath();
void AfterFileReadFunc(u8 *lpAddr, s32 index, s32 fileType, s32 size);
int FileCheckAsyncBusy(void);
void FileBackReadControl();
void FileReadControl(int bDraw);
int FileRead(char *filename /* r2 */, signed int size /* r2 */,
                    u_char **lpBuf /* r2 */,
                    u_char bSynch /* r2 */, int index /* r2 */,
                    int filetype /* r2 */, u_int align /* r2 */);

#endif // FILE_H
