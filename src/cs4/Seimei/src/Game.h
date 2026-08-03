#ifndef GAME_H
#define GAME_H

#include "typedefs.h"

typedef struct {
    // Members
    u_int SystemCounter; // offset 0x0, size 0x4
    u_char ControlMode; // offset 0x4, size 0x1
    u_char GameMode; // offset 0x5, size 0x1
    u_char GameModePrev; // offset 0x6, size 0x1
    u_char DebugMode; // offset 0x7, size 0x1
    u_char StopMode; // offset 0x8, size 0x1
    u_char sceneNo; // offset 0x9, size 0x1
    u_char padport; // offset 0xA, size 0x1
    u_char ShadowTest; // offset 0xB, size 0x1
    u_char DepthTest; // offset 0xC, size 0x1
    u_char ScriptMode; // offset 0xD, size 0x1
    u_char DizzyTest; // offset 0xE, size 0x1
    u_char Pause; // offset 0xF, size 0x1
    u_char EnePause; // offset 0x10, size 0x1
    u_char EnemyOff; // offset 0x11, size 0x1
    u_char FadedTest; // offset 0x12, size 0x1
    u_char PeepMask; // offset 0x13, size 0x1
    u_char PlayerId; // offset 0x14, size 0x1
    u_char PadVib; // offset 0x15, size 0x1
    u_char KeyConfType; // offset 0x16, size 0x1
    u_char EneSearchOff; // offset 0x17, size 0x1
    u_char GameLevel; // offset 0x18, size 0x1
    u_char StartEquip; // offset 0x19, size 0x1
    u_char PenanceMode; // offset 0x1A, size 0x1
    u_char NineHitNum; // offset 0x1B, size 0x1
    u_char NegaPosi; // offset 0x1C, size 0x1
    u_char LoadStart; // offset 0x1D, size 0x1
    char Brightness; // offset 0x1E, size 0x1
    u_char GlareId; // offset 0x1F, size 0x1
    u_char MyAllAttack; // offset 0x20, size 0x1
    u_char DebugMenuOn; // offset 0x21, size 0x1
    u_char Sugoroku; // offset 0x22, size 0x1
    u_char TutorialOff; // offset 0x23, size 0x1
    float AmbRate; // offset 0x24, size 0x4
    u_char ClearCount[3]; // offset 0x28, size 0x3
    u_char ExtraClear; // offset 0x2B, size 0x1
    u_char VoiceLanguage; // offset 0x2C, size 0x1
    u_char LastLanguage; // offset 0x2D, size 0x1
    u_char unused[2]; // offset 0x2E, size 0x2
} GameState;

extern GameState GameSt; // size: 0x30, address: 0x3A5E10


int GameLoop();
void InitGameData();
void InitGameDataAfterLoad();
void * GetGameTexList();
#endif // GAME_H
