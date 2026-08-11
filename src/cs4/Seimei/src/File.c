#include "File.h"
#include "Binder.h"
#include "Cdvd.h"
#include "Game.h"
#include "Model.h"
#include "My.h"
#include "Tex.h"
#include "Unk.h"
#include "common.h"
#include "lib/FsFont.h"
#include "menu/KuonMenu.h"
#include "menu/Menu.h"

unsigned char RootPath[128]; // size: 0x80, address: 0x325530
unsigned char ScePath[128];  // size: 0x80, address: 0x3254B0

// Belongs in CDVD.c
unsigned int GlobalLoadTimer;

void AfterFileReadFunc(u8 *lpAddr, s32 index, s32 fileType, s32 size) {
  switch (fileType) {
  case 0x0:
  case 0x80:
    RemapBinderFile(lpAddr);
    SetMdlBndFile(index);
    if (GameSt.GameMode != 4) {
      if (index == 0) {
        MyAppear(0, &MdlMng[0xA0], VW, 0.0f);
      }
      if ((GameSt.PlayerId != 2) && (index == 0x10E)) {
        InitMyWeapon(GameSt.PlayerId);
        break;
      }
    }
    break;
  case 0x1:
    RemapCamData(lpAddr);
    break;

  case 0x2:
    RemapMapFile(lpAddr);
    ReadMapMdlFile(lpAddr);
    break;

  case 0x3:
    break;

  case 0x4:
    break;

  case 0x5:
    RemapMdlLink(lpAddr);
    SetMdlData(index, lpAddr);
    break;

  case 0x6:
    RemapMdlLink(lpAddr);
    SetTexList(lpAddr, 2);
    break;

  case 0x7:
    RemapMdlLink(lpAddr);
    SetTexList(lpAddr, 2);
    FsFontSetTexBuf(2,
                    GetTexMngByName(GetGameTexList(), "iconfont.tm2")->lpTm2);
    ItemMng_CallBack_FinishLoadFile_ItemList();
    break;

  case 0x8:
    RemapMdlLink(lpAddr);
    SetTexList(lpAddr, 1);
    SetMapTexListFile(index);
    break;

  case 0x9:
    RemapMdlLink(lpAddr);
    SetAniData(index, lpAddr);
    break;

  case 0xA:
    RemapMdlLink(lpAddr);
    SetHdlData(index, lpAddr);
    break;

  case 0xB:
    RemapMdlLink(lpAddr);
    SetSdlData(index, lpAddr);
    break;

  case 0xC:
    break;

  case 0xD:
    RemapBinderFile(lpAddr);
    Rta_BndReadEndCallBack();
    break;

  case 0xE:
    break;

  case 0x11:
    RemapMdlLink(lpAddr);
    SetTexList(lpAddr, 1);
    break;

  case 0xF:
    break;

  case 0x10:
    RemapBinderFile(lpAddr);

    if ((Menu_GetLastReloadType() & 0xFFFF) == 0x8000) {
      Menu_FileReadCallBack(0x8000);
      KnMenuMng_ReMakeMenu(KUON_MENU__MAKE_TITLE);
      break;
    }

    if ((Menu_GetLastReloadType(0x8000) & 0xFFFF) == 0x4000) {
      Menu_FileReadCallBack(KUON_MENU_RELOAD_FLAG_GAME);
      KnMenuMng_ReMakeMenu(KUON_MENU__MAKE_GAME);
      SetFixMemoryEndIndex();
      break;
    }
    break;

  case 0x12:
    SetScript(size, lpAddr);
    break;

  case 0x13:
    break;

  case 0x14:
    break;

  case 0x81:
    break;

  case 0x15:
    RemapDramaData(lpAddr);
    if (index != 0) {
      SetDramaData();
      break;
    }
    break;

  case 0x16:
    SND_LoadSecFromFileInMemory(index & 0xFF, lpAddr);
    break;

  case 0x21:
    SND_LoadRandSeFromFileInMemory(lpAddr);
    break;

  case 0x17:
    RemapBinderFile(lpAddr);
    break;

  case 0x18:
    SND_SqPrepare();
    break;

  case 0x19:
    SetSceneBndFile();
    break;

  case 0x1A:
    break;

  case 0x1B:
    SfxFIO_CallBack_FinishLoadSfxBindFile();
    break;

  case 0x1C:
    break;

  case 0x1D:
    SetRiddleData();
    break;

  case 0x1E:
    break;

  case 0x1F:
    RemapMiz(lpAddr);
    break;

  case 0x20:
    break;

  case 0x82:
    break;
  }
  return;
}

int FileCheckAsyncBusy(void) { return CdvdCheckAsyncBusy(); }

void FileBackReadControl(void) { CdvdBackReadProc(); }

void FileReadControl(int bDraw) { CdvdAsyncProc(bDraw); }

signed int FileRead(char *filename /* r2 */, signed int size /* r2 */,
                    unsigned char **lpBuf /* r2 */,
                    unsigned char bSynch /* r2 */, signed int index /* r2 */,
                    signed int filetype /* r2 */, unsigned int align /* r2 */) {
  return CdvdRead(filename, size, lpBuf, bSynch, index, filetype, align);
}

void InitFileRead(void) { InitCdvdRead(); }

unsigned char *GetRootPath() { return RootPath; }

void ReadRootPath() {
  memset(RootPath, 0, 0x80);
  memset(ScePath, 0, 0x80);
  sprintf((char *)RootPath, "cdrom0:\\");
  sprintf((char *)ScePath, "cdrom0:\\");
}

void InitFileSystem() {
  /* anonymous block */
  // Range: 0x138C20 -> 0x138C78

  sceSifInitRpc(0);
  sceSifInitIopHeap();
  sceSifLoadFileReset();
  sceFsReset();
  while (!sceCdInit(0)) {
  }
  sceCdMmode(2);
}