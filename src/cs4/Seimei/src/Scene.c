#include "common.h"

void ClearLoadMovieId() {}

unsigned char GetLoadMovieId() { return 0; }

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", SetLoadMovieId);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", SetLoadCamReserve);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", SetMapMdlFile);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", SetSceneBndFile);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", FreeSceneData);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", LoadSceneData);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", GetCamIdAfterLoad);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", CheckSceneLoad);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", SetSceneLoad);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", InitSceneLoadFlag);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/Scene", InitSceneLoad);
