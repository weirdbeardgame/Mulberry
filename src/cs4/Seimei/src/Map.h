#ifndef MAP_H
#define MAP_H

#include "common.h"

typedef struct _PARTS {
    // Members
    sceVu0FVECTOR Pos; // offset 0x0, size 0x10
    sceVu0FVECTOR Ang; // offset 0x10, size 0x10
    sceVu0FVECTOR Scl; // offset 0x20, size 0x10
    int ModelID; // offset 0x30, size 0x4
    int MdlKindId; // offset 0x34, size 0x4
    u_int MapObjData; // offset 0x38, size 0x4
    int unused1[1]; // offset 0x3C, size 0x4
    char Name[16]; // offset 0x40, size 0x10
} PARTS;

int GetMdlDataIndexFromMapMdlInfo(int Type, int MdlId);
PARTS* GetMapPartsInfoByCtrlId(int Type, int CtrlId);

int GetMdlMngIndexByCtrlId(int Type, int CtrlId);    
void CheckSaveEvent();
void UpdateMapSfx();
void UpdateMapSe();    
void SetMapModel();

#endif // MAP_H
