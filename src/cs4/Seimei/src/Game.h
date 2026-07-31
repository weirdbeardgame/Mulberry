#ifndef GAME_H
#define GAME_H

extern struct {
    // Members
    unsigned int SystemCounter; // offset 0x0, size 0x4
    unsigned char ControlMode; // offset 0x4, size 0x1
    unsigned char GameMode; // offset 0x5, size 0x1
    unsigned char GameModePrev; // offset 0x6, size 0x1
    unsigned char DebugMode; // offset 0x7, size 0x1
    unsigned char StopMode; // offset 0x8, size 0x1
    unsigned char sceneNo; // offset 0x9, size 0x1
    unsigned char padport; // offset 0xA, size 0x1
    unsigned char ShadowTest; // offset 0xB, size 0x1
    unsigned char DepthTest; // offset 0xC, size 0x1
    unsigned char ScriptMode; // offset 0xD, size 0x1
    unsigned char DizzyTest; // offset 0xE, size 0x1
    unsigned char Pause; // offset 0xF, size 0x1
    unsigned char EnePause; // offset 0x10, size 0x1
    unsigned char EnemyOff; // offset 0x11, size 0x1
    unsigned char FadedTest; // offset 0x12, size 0x1
    unsigned char PeepMask; // offset 0x13, size 0x1
    unsigned char PlayerId; // offset 0x14, size 0x1
    unsigned char PadVib; // offset 0x15, size 0x1
    unsigned char KeyConfType; // offset 0x16, size 0x1
    unsigned char EneSearchOff; // offset 0x17, size 0x1
    unsigned char GameLevel; // offset 0x18, size 0x1
    unsigned char StartEquip; // offset 0x19, size 0x1
    unsigned char PenanceMode; // offset 0x1A, size 0x1
    unsigned char NineHitNum; // offset 0x1B, size 0x1
    unsigned char NegaPosi; // offset 0x1C, size 0x1
    unsigned char LoadStart; // offset 0x1D, size 0x1
    char Brightness; // offset 0x1E, size 0x1
    unsigned char GlareId; // offset 0x1F, size 0x1
    unsigned char MyAllAttack; // offset 0x20, size 0x1
    unsigned char DebugMenuOn; // offset 0x21, size 0x1
    unsigned char Sugoroku; // offset 0x22, size 0x1
    unsigned char TutorialOff; // offset 0x23, size 0x1
    float AmbRate; // offset 0x24, size 0x4
    unsigned char ClearCount[3]; // offset 0x28, size 0x3
    unsigned char ExtraClear; // offset 0x2B, size 0x1
    unsigned char VoiceLanguage; // offset 0x2C, size 0x1
    unsigned char LastLanguage; // offset 0x2D, size 0x1
    unsigned char unused[2]; // offset 0x2E, size 0x2
} GameSt; // size: 0x30, address: 0x3A5E10


int GameLoop();
void InitGameData();
void InitGameDataAfterLoad();
#endif // GAME_H
