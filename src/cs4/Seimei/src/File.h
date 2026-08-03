#ifndef FILE_H
#define FILE_H

extern unsigned char ScePath[128]; // size: 0x80, address: 0x3254B0
extern unsigned char RootPath[128]; // size: 0x80, address: 0x325530

void InitFileRead();
void InitFileSystem();
void ReadRootPath();
unsigned char * GetRootPath();

#endif // FILE_H
