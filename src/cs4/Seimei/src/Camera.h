#ifndef CAMERA_H
#define CAMERA_H

typedef struct /* @anon0 */ {
    // Members
    unsigned char * lpCamData; // offset 0x0, size 0x4
    signed int nowIndex; // offset 0x4, size 0x4
    float frame; // offset 0x8, size 0x4
    unsigned char LockDirFlag; // offset 0xC, size 0x1
    char LockL3V; // offset 0xD, size 0x1
    char LockL3H; // offset 0xE, size 0x1
    unsigned char noneUpdate; // offset 0xF, size 0x1
    unsigned char eraseMap[256][48]; // offset 0x10, size 0x3000
} CAMINFO;

void UpdateCamera();
void SetCamEnv(CAMINFO * CAMINFO, int Index);
CAMINFO * GetGameCamInfo();
void InitCamera();

#endif // CAMERA_H