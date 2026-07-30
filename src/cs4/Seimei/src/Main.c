#include "Main.h"
#include "ClothSimulator.h"
#include "Draw.h"
#include "Fade.h"
#include "File.h"
#include "Font.h"
#include "Game.h"
#include "Load.h"
#include "Scene.h"
#include "Script.h"
#include "TexEffect.h"
#include "Title.h"
#include "common.h"
#include "ee/eekernel.h"
#include "typedefs.h"

void SetGoToEndingFade(void) {
  SetFadeMng(1, (sceVu0IVECTOR){0}, 30, NULL, SetGoToEndingFlag, 0, 0);
  SND_RemoveSound();
}

int CheckGoToEndingFlag(void) { return GoToEndingFlag; }

void SetGoToEndingFlag(void) { GoToEndingFlag = 1; }

void SetGoToTitleFade(void) {
  SetFadeMng(10, (sceVu0IVECTOR){0}, 30, NULL, SetGoToTitleFlag, 0, 0);
  GameSt.Pause = 0;
  SND_RemoveSound();
}

int CheckGoToTitleFlag(void) { return GoToTitleFlag; }

void SetGoToTitleFlag(void) { GoToTitleFlag = 1; }

void SetKeyConfig(unsigned char Type /* r2 */) { GameSt.KeyConfType = Type; }

static void InitGameSt(signed int argc /* r18 */, char **argv /* r17 */) {
  int i;
  sceCdCLOCK rtc;

  memset(&GameSt, 0, sizeof(GameSt));
  GameSt.DepthTest = 0;
  GameSt.ShadowTest = 1;
  GameSt.AmbRate = 1.0f;
  GameSt.GameLevel = 1;
  GameSt.LastLanguage = 1;
  GameSt.VoiceLanguage = 1;

  SetPadVibFlag(1);

  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "xxxxxxxx") == 0) {
      SetPadVibFlag(0);
    }
  }

  GameSt.KeyConfType = 0;

  sceCdReadClock(&rtc);
  srand(rtc.second);
}

void SifLoadModule(void) {
  char acStack_80[128];
  char acStack_100[128];
  char acStack_180[128];
  char acStack_200[128];
  char acStack_280[128];
  char acStack_300[128];
  char acStack_380[128];
  char acStack_400[128];
  char acStack_480[128];
  char acStack_500[128];
  char acStack_580[128];
  char acStack_600[128];
  char acStack_680[128];
  char acStack_700[128];

  memset(acStack_80, 0, 0x80);
  sprintf(acStack_80, "%s%s", GetRootPath(), "iop\\sio2man.irx;1");
  StringCorrectPath4Disc(acStack_80, 1);
  if (sceSifLoadModule(acStack_80, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_100, 0, 0x80);
  sprintf(acStack_100, "%s%s", GetRootPath(), "iop\\padman.irx;1");
  StringCorrectPath4Disc(acStack_100, 1);
  if (sceSifLoadModule(acStack_100, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_180, 0, 0x80);
  sprintf(acStack_180, "%s%s", GetRootPath(), "iop\\libsd.irx;1");
  StringCorrectPath4Disc(acStack_180, 1);
  if (sceSifLoadModule(acStack_180, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_200, 0, 0x80);
  sprintf(acStack_200, "%s%s", GetRootPath(), "iop\\sdrdrv.irx;1");
  StringCorrectPath4Disc(acStack_200, 1);
  if (sceSifLoadModule(acStack_200, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_280, 0, 0x80);
  sprintf(acStack_280, "%s%s", GetRootPath(), "iop\\cdvdstm.irx;1");
  StringCorrectPath4Disc(acStack_280, 1);
  if (sceSifLoadModule(acStack_280, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_300, 0, 0x80);
  sprintf(acStack_300, "%s%s", GetRootPath(), "iop\\modmidi.irx;1");
  StringCorrectPath4Disc(acStack_300, 1);
  if (sceSifLoadModule(acStack_300, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_380, 0, 0x80);
  sprintf(acStack_380, "%s%s", GetRootPath(), "iop\\modhsyn.irx;1");
  StringCorrectPath4Disc(acStack_380, 1);
  if (sceSifLoadModule(acStack_380, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_400, 0, 0x80);
  sprintf(acStack_400, "%s%s", GetRootPath(), "iop\\mcman.irx;1");
  StringCorrectPath4Disc(acStack_400, 1);
  if (sceSifLoadModule(acStack_400, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_480, 0, 0x80);
  sprintf(acStack_480, "%s%s", GetRootPath(), "iop\\mcserv.irx;1");
  StringCorrectPath4Disc(acStack_480, 1);
  if (sceSifLoadModule(acStack_480, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_500, 0, 0x80);
  sprintf(acStack_500, "%s%s", GetRootPath(), "iop\\modsein.irx;1");
  StringCorrectPath4Disc(acStack_500, 1);
  if (sceSifLoadModule(acStack_500, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_580, 0, 0x80);
  sprintf(acStack_580, "%s%s", GetRootPath(), "iop\\fssd.irx;1");
  StringCorrectPath4Disc(acStack_580, 1);
  if (sceSifLoadModule(acStack_580, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_600, 0, 0x80);
  sprintf(acStack_600, "%s%s", GetRootPath(), "iop\\fssdsyth.irx;1");
  StringCorrectPath4Disc(acStack_600, 1);
  if (sceSifLoadModule(acStack_600, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_680, 0, 0x80);
  sprintf(acStack_680, "%s%s", GetRootPath(), "iop\\fsmfstrm.irx;1");
  StringCorrectPath4Disc(acStack_680, 1);
  if (sceSifLoadModule(acStack_680, 0, "") < 0) {
    exit(0);
  }
  memset(acStack_700, 0, 0x80);
  sprintf(acStack_700, "%s%s", GetRootPath(), "iop\\fssdivag.irx;1");
  StringCorrectPath4Disc(acStack_700, 1);
  if (sceSifLoadModule(acStack_700, 0, "") < 0) {
    exit(0);
  }
}

void ClearTime(void) {
  *REG_RCNT0_COUNT = 0;
  gTime = 0;
  return;
}

volatile int GetTime(void) {
  return *(volatile int *)REG_RCNT0_COUNT + gTime * 60000;
}

void Timer0Handler(void) {
  gTime += 1;
  *REG_RCNT0_MODE |= 0x400;
}

static inline void WaitIop(void) {
  char sp50[0x80];
  memset(&sp50, 0, sizeof(sp50));
  sprintf(sp50, "xxxxxxxx1", "xxxxxxxx2");
  StringCorrectPath4Disc(sp50, 1);
  while (!sceSifRebootIop(sp50))
    ;
  while (!sceSifSyncIop())
    ;
}

int main(int argc /* r19 */, char **argv /* r18 */) {
  /* anonymous block */  //{
                         // Range: 0x13AB40 -> 0x13AF58
  int LoopRet;           // r17, s1
  int FirstRead;         // r16, s0
  unsigned char MovieId; // r2, v0
  int i;                 // v0
  //}

  LoopRet = 0;
  MemInit();
  InitFileSystem();
  WaitIop();
  InitFileSystem();
  ChangeThreadPriority(GetThreadId(), 0x40);
  ReadRootPath(argv[0]);
  sceDmaReset(1);
  sceVpu0Reset();
  sceDevVif0Reset();
  DrawInit();
  Vu1Init();
  Vu1SendProg();
  DrawClearImage();
  SifLoadModule();
  InitGameSt(argc, argv);
  InitPad();
  InitFileRead();
  InitMemoryCard();
  ResetMdlPacketMem();
  InitTexMng();
  SetAllBindInfo();
  SND_Init();
  InitMultiStm();
  InitVagStm();
  SND_InitLoad();
  *REG_RCNT0_MODE = 0x1c1;
  *REG_RCNT0_TARGET = 60000;
  AddIntcHandler(9, Timer0Handler, -1);
  EnableIntc(9);
  gTime = 0;
  InitPcrtc();
  InitMdlMng();
  {
    char spD0[0x10] = {};
    DrawSetClearColor((int *)spD0);
  }
  FontInit(0);
  SFX_Initialize();
  TmpMenuMng_Init(&g_TmpMenuMng);
  InitFadeControl();
  InitSceneLoad();
  KnMsgMng_Initialize();
  KnMsgMng_Init();
  InitSavePointID();
  KnMenuMng_Initialize();
  KnMenuMng_Init(0xC);
  Menu_TD_Init();
  CsMngInitialize();
  CsMngInit();
  SND_SecLoadCommon();
  while (1) {
    GameSt.SystemCounter = 0;
    GoToTitleFlag = 0;
    GoToEndingFlag = 0;
    LoadTitleTexture();
    SetInitLoadScene(argc, argv);
    ClearEventFlag();
    ItemMng_Clear();
    LoadPrevTitleLoop(argc, argv);
    TitleLoop(LoopRet == 3);
    FreeTitleTexture();
    do {
      if (GameSt.GameMode == 4) {
        InitGameData();
        LoadMiniGameData(argc, argv);
        LoadLoadingTexture();
        LoadingLoop();
        FreeLoadingTexture();
        InitMiniGameData();
        ResetMiniGameData();
        MiniGameLoop();
        FreeMiniGameData();
        LoopRet = 1;
        break;
      }

      FirstRead = 0;
      InitGameData();
      GameSt.sceneNo = lpSceneLoad->scene_id;
      LoadPrevGameLoop(argc, argv);
      while (1) {
        InitBloodStain();
        LoadLoadingTexture();
        LoadSceneData(GameSt.sceneNo, GameSt.PlayerId);
        LoadingLoop();
        FreeLoadingTexture();
        if (!FirstRead) {
          FirstRead = TRUE;
          InitGameDataAfterLoad();
        }
        ResetGameData();
        LoopRet = GameLoop();
        FreeSceneData();
        ResetMdlPacketMem();
        if (LoopRet != 0) {
          break;
        }
        MovieId = GetLoadMovieId();
        if (MovieId != 0) {
          SetKuonMovie((MovieId & 0xFF) - 1);
          ClearLoadMovieId();
        }
      }
      FreePrevGameLoop();
    } while (LoopRet != 1 && LoopRet != 3);

    if (LoopRet == 3) {
      SetKuonMovie(GameSt.PlayerId + 1);
    }

    FreePrevTitleLoop();
  }
}