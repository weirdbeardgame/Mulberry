#include "Binder.h"
#include "Cdvd.h"
#include "File.h"
#include "Memory.h"
#include "common.h"
#include "sce/libcdvd.h"

void GetAllBindInfo(char *fname /* r2 */, unsigned int *lsn /* r2 */,
                    unsigned int *size /* r2 */) {
  // Range: 0x15E2E0 -> 0x15E440
  unsigned int hash = 0; // r7
  signed int i = 0;      // r9

  hash = foo_fn(fname);

  *lsn = 0x0;
  *size = 0x0;
  for (i = 0; i < AllBindDataNum; i++) {
    if (hash == AllBindInfo[i].hash) {
      *lsn = AllBindInfo[i].lsn;
      *size = AllBindInfo[i].size;
      break;
    }
  }
}

void SetAllBindInfo(void) {
  signed int i;                                                // r19
  LPBINDHEAD *lpBindHead;                                      // s2
  LPOFFSET *lpOffset;                                          // s1
  LPALLBINDINFO *lpAllBindInfo = (LPALLBINDINFO *)AllBindInfo; // s0
  sceCdRMode CdRMode = {0, 1, 0, 0};

  MakeCdvdReadPath(lpCdvd->fname, (char *)GetRootPath(), "All.bnd");
  MakeCdvdSearchPath(lpCdvd->sname, "All.bnd");
  StringCorrectPath4Disc(lpCdvd->fname, 1);
  StringCorrectPath4Disc(lpCdvd->sname, 1);

  lpCdvd->size = 0x18000;

  lpCdvd->sector = (lpCdvd->size + 0x7FF) / 2048;
  lpCdvd->lpDmyData = MemAllocAlignReal(lpCdvd->sector << 0xB, 0x800U,
                                        (char *)__FUNCTION__, "");
  if (lpCdvd->lpDmyData == NULL) {
    while (1)
      ;
  }

  while (sceCdSearchFile((sceCdlFILE *)&lpCdvd->cdlfile,
                         (void *)&lpCdvd->sname) == 0)
    ;
  FlushCache(0);
  while (sceCdRead(lpCdvd->cdlfile.lsn, lpCdvd->sector, lpCdvd->lpDmyData,
                   &CdRMode) == 0)
    ;
  sceCdSync(0);
  memset(AllBindInfo, 0, sizeof(AllBindInfo));

  lpBindHead = (void *)lpCdvd->lpDmyData;
  lpOffset = (void *)(lpBindHead + 0x1);
  if (lpBindHead->filenum > ARRAY_COUNT(AllBindInfo)) {
    while (1)
      ;
  }
  AllBindDataNum = lpBindHead->filenum;

  for (i = 0; i < lpBindHead->filenum; i++) {
    lpOffset->nameoffset += (int)lpBindHead;
    strcpy((char *)&lpAllBindInfo->filename, (char *)lpOffset->nameoffset);
    lpAllBindInfo->lsn = (lpOffset->dataoffset >> 0xB) + lpCdvd->cdlfile.lsn;
    lpAllBindInfo->size = lpOffset->filesize;
    lpAllBindInfo->hash = foo_fn((char *)&lpAllBindInfo->filename);
    lpOffset++;
    lpAllBindInfo++;
  }
  MemFree(lpCdvd->lpDmyData);
}

unsigned int GetBindNum(LPBINDHEAD *lpBindHead /* r2 */) {

  return lpBindHead->filenum;
}

unsigned int GetBindOffsetByName(LPBINDHEAD *lpBindHead, char *fname) {
  unsigned int i;         // r17
  SHORT_OFFSET *lpOffset; // r16

  if (lpBindHead == NULL) {
    return 0;
  }

  lpOffset = (SHORT_OFFSET *)(lpBindHead + 1);

  for (i = 0; i < lpBindHead->filenum; i++, lpOffset++) {
    if (strcmp((char *)lpOffset->nameoffset, fname) == 0) {
      return lpOffset->dataoffset;
    }
  }

  return 0;
}

unsigned int GetBindOffsetByIndex(LPBINDHEAD *lpBindHead, u_short Index) {
  unsigned int i;         // r17
  SHORT_OFFSET *lpOffset; // r16

  if (lpBindHead == NULL) {
    return 0;
  }

  lpOffset = (SHORT_OFFSET *)(lpBindHead + 1);

  for (i = 0; i < lpBindHead->filenum; i++, lpOffset++) {
    if (lpOffset->fileID == Index) {
      return lpOffset->dataoffset;
    }
  }
  return 0;
}

void RemapBinderFile(void *lpBuf /* r2 */) {
  u_int i;                // r6
  LPBINDHEAD *lpBindHead; // r2
  SHORT_OFFSET *lpOffset; // r5

  if (lpBuf == NULL) {
    return;
  }

  lpBindHead = lpBuf;
  lpOffset = (SHORT_OFFSET *)(lpBindHead + 1);

  for (i = 0; i < lpBindHead->filenum; i++) {
    lpOffset[i].dataoffset += (u_int)lpBuf;
    lpOffset[i].nameoffset += (u_int)lpBuf;
  }
}

void ReadBinderFile(char *filename /* r2 */, void **lplpBuf /* r5 */,
                    signed int dataindex /* r2 */,
                    signed int filetype /* r2 */) {
  FileRead(filename, -1, (u_char **)lplpBuf, 0, dataindex, filetype, 0);
}
