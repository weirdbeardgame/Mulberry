#include "Game.h"
#include "Camera.h"
#include "Drama.h"
#include "DrawOT.h"
#include "EtcEvent.h"
#include "Event.h"
#include "Filter.h"
#include "KuonMsg.h"
#include "Main.h"
#include "Model.h"
#include "Pad.h"
#include "Sprite.h"
#include "TexEffect.h"
#include "Unk.h"
#include "common.h"
#include "ee/eeregs.h"

void InitGameData() {
  SetObjFunc();
  InitLightMat();
  DssInit();
  InitDamageMng();
  InitPointLight();
  InitEneSave();
  InitDramaMng();
  SfxPostEffect_Initialize();
  InitHitLine();
  InitMovePath();
  InitSpiritMng();
}

void InitGameDataAfterLoad(void) {
  FECard_Initialize();
  SetLoadCamReserve(0xFFFF);
}

void LoadGameTexture(void) {
  ReadTexList("tex/game.tex", &lpGameTexList, 7, 0);
}

inline void ScreenClear() {
  int color_array[4] = {};
  SetFadeMng(0, (int *)&color_array, 0x3C, 0, 0, 0, 0);
}

void ResetGameData(void) {

  GameSt.ControlMode = 0;
  GameSt.Pause = 0;

  ResetDebug();
  InitPointLight();
  InitTextureScroll();
  SetMapModel();
  SND_SqPlay(HSYN_PORT_SQBGM, 0);
  SND_SqFadeStart(HSYN_PORT_SQBGM, SND_SQSONG_FADE_IN);
  SND_SqPlay(HSYN_PORT_SQBGM1, 1);
  SND_SqFadeStart(HSYN_PORT_SQBGM1, SND_SQSONG_FADE_IN);
  SND_SqPlay(HSYN_PORT_SQBGM2, 2);
  SND_SqFadeStart(HSYN_PORT_SQBGM2, SND_SQSONG_FADE_IN);
  SND_SqPlay(HSYN_PORT_SQBGM3, 3);
  SND_SqFadeStart(HSYN_PORT_SQBGM3, SND_SQSONG_FADE_IN);

  GameSt.LastLanguage = GameSt.VoiceLanguage;

  InitMirror();
  InitCamera();
  InitSandEffect();
  MyReset(&MdlMng[160]);
  StabilizeMyCloth();
  InitObjFunc();
  InitBlast();
  InitDizzyFilter();
  InitGlareFilter();
  InitTVEffect();
  KnMsgMng_MsgAllClear();
  TmpMenuMng_SetUDBlackDraw(&g_TmpMenuMng, TMP_MENU__UDDRAW_ORDER__OFF_FORCE);
  KnMenuMng_Crean();
  Menu_TD_Init();
  ScreenClear();

  GameSt.EnePause = 0;
  GameSt.PeepMask = 0;

  if ((GameSt.sceneNo == 9) && (CheckEventFlag(0x45F) == 0)) {
    SetDramaData();
    SetEventFlag(0x45F);
  } else if ((GameSt.sceneNo == 23) && (CheckEventFlag(0xD8) == 0)) {
    SetDramaData();
    SetEventFlag(0xD8);
  } else if ((GameSt.PlayerId == 0) && (GameSt.sceneNo == 33) &&
             (CheckEventFlag(0x440) == 0)) {
    SetDramaData();
    SetEventFlag(0x440);
  } else if (GameSt.sceneNo == 36 && CheckEventFlag(0x3EF) == 0) {
    SetDramaData();
    SetEventFlag(0x3EF);
  } else if (GameSt.sceneNo == 37) {
    SetDramaData(GameSt.sceneNo);
  } else if (GameSt.sceneNo == 71 && CheckEventFlag(0x110) != 0) {
    SetDramaData();
  } else if ((GameSt.sceneNo == 78) && (CheckEventFlag(0x144) == 0)) {
    SetDramaData();
    SetEventFlag(0x144);
  } else if ((GameSt.sceneNo == 82) && (CheckEventFlag(0x143) == 0)) {
    SetDramaData();
    SetEventFlag(0x143);
  } else if ((GameSt.PlayerId == 2) && (GameSt.sceneNo == 87) &&
             (CheckEventFlag(0x1E4) == 0)) {
    SetDramaData();
    SetEventFlag(0x1E4);
  } else if (GameSt.sceneNo == 0x5A) {
    SetDramaData(GameSt.sceneNo);
    SetEventFlag(0x45F);
  }
}

int GameLoop() {
  int i;
  CAMINFO *lpCamInfo;       // v0
  int GameLoopRet = 0;      // s0
  struct _mdlmng *lpMdlMng; // s1
  while (true) {
    ClearTime();
    GameSt.SystemCounter += 1;

    GameSt.GameModePrev = GameSt.GameMode;

    SetPcrtc();
    KickMdlPacket();
    PadControl();
    FileBackReadControl();
    CheckMyEquipUpdate();
    UpdateTextureScroll();
    ClearDrawOT();
    CheckTVEffect();

    switch (GameSt.GameMode) {
    case 0:

      UpdateCamera();
      DrawCalcView();
      DamageProc();
      MyPlay(&MdlMng[0xa0]);
      EnePlayAll();
      ObjPlayAll();
      ScriptProc();
      PlayEventFunc();
      CheckSaveEvent();
      KnMsgMng_Update();
      KnMenuMng_Update(1);
      Menu_TD_Update();
      CsMngUpdate();
      TmpMenuMng_Update(&g_TmpMenuMng);

      lpMdlMng = (struct _mdlmng *)&MdlMng;

      do {
        if (lpMdlMng->bUse == 0)
          continue;

        PlayMi3(lpMdlMng);
        SetDrawOTModel(lpMdlMng, lpMdlMng->Pos, lpMdlMng->Priority);
      } while (++lpMdlMng != &MdlMng[770]);

      UpdateMapSfx();

      if ((GameSt.Pause == 0) && (Menu_TD_IsPlay() == 0))
        SFX_Action(0.0166666f);

      SfxPostEffect_Action(0.0166666f);
      EtcEventMng_EneExtGeneEvent_Update(&g_EtcEventMng);
      CsMngDraw();
      UpdatePointLight();
      break;
    case 1:
      PlayDrama();
      ObjPlayAll();
      ScriptProc();
      if (GetDramaFreeFlag() != 0) {
        lpCamInfo = GetGameCamInfo();
        SetCamEnv(lpCamInfo, lpCamInfo->nowIndex);
        UpdateCamera();
      }

      DrawCalcView();
      KnMsgMng_Update();
      KnMenuMng_Update(1);

      lpMdlMng = (struct _mdlmng *)&MdlMng;

      do {
        if (lpMdlMng->bUse == 0)
          continue;

        if ((CheckDramaDrawModel(lpMdlMng) == 0))
          continue;

        if (CheckDramaGameOver() == 0)
          PlayMi3(lpMdlMng);

        SetDrawOTModel(lpMdlMng, lpMdlMng->Pos, lpMdlMng->Priority);
      } while (++lpMdlMng != &MdlMng[770]);

      UpdateMapSfx();
      if ((GameSt.Pause == 0) && (Menu_TD_IsPlay() == 0))
        SFX_Action(0.0166666f);

      SfxPostEffect_Action(0.0166666f);
      break;

    case 2:
      KnMenuMng_Update(1);
      Menu_TD_Update();
      CsMngUpdate();
      break;

    case 3:
      UpdateCamera();
      DrawCalcView();
      KnMsgMng_Update();
      Menu_TD_Update();
      PlayRiddle();

      lpMdlMng = (struct _mdlmng *)&MdlMng;

      do {
        if ((lpMdlMng->bUse == 0))
          continue;

        if ((CheckRiddleDrawModel(lpMdlMng) == 0))
          continue;

        PlayMi3(lpMdlMng);
        SetDrawOTModel(lpMdlMng, lpMdlMng->Pos, lpMdlMng->Priority);
      } while (++lpMdlMng != &MdlMng[770]);
      break;
    }

    switch (GameSt.GameMode) {
    case 2:
      UpdateMapSe();
      SND_Update();
      KnMenuMng_Draw();
      Menu_TD_Draw();
      DrawBrightnessFilter();
      UpdateFadeMng();
      break;

    default:
      if ((GameSt.Pause == 0) && (Menu_TD_IsPlay() == 0))
        FECard_Action(0.0166666f);

      UpdateMapSe();
      SND_Update();
      SFX_Draw();
      DrawByOT();
      SFX_DrawAfterOT();
      DrawGlareFilter();
      DrawBrightnessFilter();
      DrawFrameFilter();
      DrawDizzyFilter();
      DrawGameOver();
      DrawPhantomFilter();
      DrawBlurFilter();
      SfxPostEffect_Draw();
      DrawPeepHole();

      if (GameSt.NegaPosi != 0)
        DrawNegaPosiFilter();
      if (GameSt.FadedTest != 0)
        MakeFadedFilter();

      FECard_Draw();
      DrawDizzyMask();

      if (GameSt.FadedTest != 0)
        DrawFadedFilter();

      TmpMenuMng_Draw(&g_TmpMenuMng);
      KnMenuMng_Draw();
      Menu_TD_Draw();
      KnMsgMng_Draw();
      UpdateBlast();
      UpdateFadeMng();
      DrawGamePause();

      if (GameSt.DepthTest != 0)
        DrawDepthOfField();

      break;
    }

    while ((*D1_CHCR & D_CHCR_STR_M) != 0 || (*D2_CHCR & D_CHCR_STR_M) != 0) {
    }

    DrawTVEffect();
    DrawSynch();
    lpDrawMng->frameTime = GetTime();
    FlushCache(0);
    CheckDramaFreeFlag();
    CheckRiddleFree();

    if (CheckSceneLoad() != 0) {

      if (GameSt.LoadStart != 0)
        GameLoopRet = 2;
      break;
    }

    else if ((GameLoopRet = CheckResetPad()) != 0) {
      GameLoopRet = 1;
      break;
    }

    else if (CheckGoToTitleFlag() != 0) {
      GameLoopRet = 1;
      break;
    }

    else if (!!CheckGoToEndingFlag()) {
      GameLoopRet = 3;
      break;
    }
  }

  PadVibAllClear();
  PadUpdate();
  SetVagStmFade(0, 1, 0x1E);
  SetVagStmFade(1, 1, 0x1E);
  SetVagStmFade(2, 1, 0x1E);
  SetVagStmFade(3, 1, 0x1E);
  SetVagStmFade(4, 1, 0x1E);
  SetVagStmFade(5, 1, 0x1E);
  CheckFreeDramaData();
  CheckFreeRiddleData();
  EtcEventMng_EneExtGeneEvent_End(&g_EtcEventMng);
  FECard_Enable_Draw(1);
  FreeSandEffect();
  Menu_TD_Init();

  return GameLoopRet;
}

void *GetGameTexList() { return lpGameTexList; }

COLOR g_col = {127, 127, 127, 127};

static void DrawGamePause(void) {
  u_short *lpMsg;

  volatile char padding[16];

  SPDATA SpData;

  int Col[4] = {0, 0, 0, 64};

  float w;
  float h;
  float w2;
  float h2;

  volatile char *p = padding;

  if (GameSt.Pause == 1) {
    SetSprite((LPSP *)&SpData, -256, -224, 512, 448, Col, 0, 0);
    lpMdlPacketMem = MakeSpritePacketUseSpr((LPSP *)&SpData, lpMdlPacketMem);

    FontDrawBegin(0);
    SetFontColor(g_col);
    FsFontSetScale(1.5f, 1.5f);

    if ((lpMsg = (u_short *)KnMsgMng_GetMapNameMsg()) != NULL) {
      FsFontGetSize(&w, &h, lpMsg, 0);
      FontPrintXY(0.5f * (512.0f - w), 150.0f, (char *)lpMsg);
    }

    FsFontSetScale(0.8f, 0.8f);
    if ((lpMsg = KnMsgMng_GetMsgStr(0x1d4e2)) != NULL) {

      FsFontGetSize(&w2, &h2, lpMsg, 0);
      FontPrintXY(0.5f * (512.0f - w2), 210.0f, (char *)lpMsg);
    }
    FontDrawEnd();
  }
}
