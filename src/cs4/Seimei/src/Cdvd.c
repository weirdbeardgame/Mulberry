#include "common.h"
#include "Cdvd.h"
#include "ee/eeregs.h"
#include "Memory.h"

static u_int CdvdAsync(void);




/*? fsmfstrm_AllocStreamBuffer(?, u8*, s32, ?);
s32 fsmfstrm_AttachSector(?, u32, s32, ?);
s32 fsmfstrm_DmaReadSyncBlock(?, ?);
? fsmfstrm_SetReadStart(?, ?);*/


#define SECTOR_SIZE (2048)

















































void InitCdvdRead(void) {
    memset(CdvdReadSt, 0, sizeof(CdvdReadSt));

    lpSetCdvd = (LPSETCDVD* ) CdvdReadSt;
    lpLastCdvd = (LPLASTCDVD* ) CdvdReadSt;
    lpCdvdRead = (LPCDVDREAD* ) CdvdReadSt;


    AioInit(0x14);
}







void MakeCdvdReadPath(s8* full, s8* root, s8* fname) {
    sprintf(full, "%s%s;1", root, fname);
}







void MakeCdvdSearchPath(s8* full, s8* fname) {
    sprintf(full, "\\%s;1", fname);
}































int CdvdRead(char * filename, int size, u_char * * lpBuf, u_char bSynch, int index, int filetype, u_int align) 
{
    char TmpStr[128]; if (lpSetCdvd->state != 0) return -1;




    strcpy(TmpStr, filename);

    StringCorrectPath4Disc(TmpStr, 0);

    strcpy(lpSetCdvd->fname, SearchFileName(TmpStr));












    lpSetCdvd->lplpAddr = lpBuf;
    lpSetCdvd->size = size;
    lpSetCdvd->index = index;
    lpSetCdvd->fileType = filetype;
    lpSetCdvd->align = align;
    lpSetCdvd->retry = 0;
    lpSetCdvd->lpDmyData = NULL;
    if (lpSetCdvd->fileType < 0x80) *lpSetCdvd->lplpAddr = NULL;
    if (lpSetCdvd->align == 0) lpSetCdvd->align = 0x10;

    if (bSynch) {




        GetAllBindInfo(lpSetCdvd->fname, &lpSetCdvd->cdlfile.lsn, &lpSetCdvd->cdlfile.size);
        if (lpSetCdvd->cdlfile.size == 0) return 0;






        if (lpSetCdvd->size <= 0) lpSetCdvd->size = lpSetCdvd->cdlfile.size;

            lpCdvdRead->sector = (lpCdvdRead->size + SECTOR_SIZE - 1) / SECTOR_SIZE;



































        if (lpSetCdvd->fileType < 0x80) 

            *lpSetCdvd->lplpAddr = MemAllocAlignReal(lpSetCdvd->sector << 0xB, 0x40U, (char*)__FUNCTION__, "");


        
        
        fsmfstrm_SetReadStart(0, 0);

        if (fsmfstrm_AttachSector(0, lpSetCdvd->cdlfile.lsn, lpSetCdvd->size, 0x30) == 0) {

            MemFree(*lpSetCdvd->lplpAddr);

            return 0;
        }


        fsmfstrm_AllocStreamBuffer(0, *lpSetCdvd->lplpAddr, lpSetCdvd->sector << 0xB, 1);


        fsmfstrm_SetReadStart(0, 1);

        fsmfstrm_DmaReadSyncBlock(0, 1);


        FlushCache(0);
        return lpSetCdvd->size;
    }

    lpSetCdvd->state = 1;
    lpLastCdvd = (LPLASTCDVD* ) lpSetCdvd;
    if (lpSetCdvd < (LPSETCDVD *) &CdvdReadSt[0xFF]) lpSetCdvd++; 
    else lpSetCdvd = (LPSETCDVD* ) CdvdReadSt;
    return 1;


}







u_int CdvdAsyncProc(int bDraw /* r16 */) {

    GlobalLoadTimer = 0;

    while (1) 
    {
        if (lpLastCdvd->state == 0) break;

        if (bDraw != 0) {

            FontDrawBegin(1);
            FontPrintXY(350.0f, 430.0f, "Now Loading...[%d]\n", GlobalLoadTimer++);
            FontDrawEnd();

            while ((*D1_CHCR & D_CHCR_STR_M) != 0 || (*D2_CHCR & D_CHCR_STR_M) != 0) {}
            DrawSynch();
            FlushCache(0);
        }


        CdvdAsync();    
    }

    return 0;
}








void CdvdBackReadProc(void) {
    CdvdAsync();
}







static u_int CdvdAsync(void) {
    int aiocmd = 0; /* r29+0x1C */
    int aiores = 0; /* r29+0x18 */
    int size;
    if ((void*)lpCdvdRead == (void*)lpSetCdvd)  return 0;

    switch (lpCdvdRead->state) {
    case 1:
        if (lpCdvdRead->size <= 0) {
            GetAllBindInfo(lpCdvdRead->fname, &lpCdvdRead->cdlfile.lsn, &lpCdvdRead->cdlfile.size);
            
            size = lpCdvdRead->cdlfile.size;
            if (size == 0) break;

                lpCdvdRead->size = size;

                lpCdvdRead->sector = (lpCdvdRead->size + SECTOR_SIZE - 1) / SECTOR_SIZE;
                lpCdvdRead->state = 4;



        } else {


            lpCdvdRead->sector = (lpCdvdRead->size + SECTOR_SIZE - 1) / SECTOR_SIZE;
            lpCdvdRead->state = 4;
        }
        return 1;
    case 2:
        if (AioCdSearchFile(lpCdvdRead->sname, &lpCdvdRead->cdlfile) != 0) {
            break;
        }
        lpCdvdRead->state = 3;
        return 0;
    case 3:
        if (AioCmdSync(1, &aiocmd, &aiores) != 0) {
            if (aiores < 0) {
                break;
            }

            lpCdvdRead->size = (s32) lpCdvdRead->cdlfile.size;
            
            lpCdvdRead->sector = (lpCdvdRead->size + SECTOR_SIZE - 1) / SECTOR_SIZE;
            lpCdvdRead->state = 4;
            return 1;
        }
        return 0;
    case 4:




















        if (lpCdvdRead->fileType < 0x80) {

            *lpCdvdRead->lplpAddr = MemAllocAlignReal(lpCdvdRead->sector * SECTOR_SIZE, 0x40, (char *)__FUNCTION__, "");

        }


        fsmfstrm_SetReadStart(0, 0);

        if (fsmfstrm_AttachSector(0, lpCdvdRead->cdlfile.lsn, lpCdvdRead->size, 0x30) == 0) {
        
            MemFree(*lpCdvdRead->lplpAddr);
            break;
        }

        fsmfstrm_AllocStreamBuffer(0, *lpCdvdRead->lplpAddr, lpCdvdRead->sector * SECTOR_SIZE, 1);


        fsmfstrm_SetReadStart(0, 1);

        lpCdvdRead->state = 5;
        return 0;
    case 5:
        if (fsmfstrm_DmaReadSyncBlock(0, 0) == 0) {













            AfterFileReadFunc(*lpCdvdRead->lplpAddr, lpCdvdRead->index, lpCdvdRead->fileType, lpCdvdRead->size);


            if ((SND_SqPrepareStatus() == 1) || !(fssdsynthTransBankStatus() & 0xFF)) {
                lpCdvdRead->state = 6;
                return 0;
            }








            lpCdvdRead->state = 0;
            if (lpCdvdRead < (LPCDVDREAD* )&CdvdReadSt[0xFF]) lpCdvdRead += 1;
            else lpCdvdRead = (LPCDVDREAD* ) CdvdReadSt;
            FlushCache(0);
        }
        return 0;
    case 6:
        if ((SND_SqPrepareStatus() == 0) && (fssdsynthTransBankStatus() & 0xFF)) {

            lpCdvdRead->state = 0;
            if (lpCdvdRead < (LPCDVDREAD* )&CdvdReadSt[0xFF]) lpCdvdRead += 1;
            else lpCdvdRead = (LPCDVDREAD* ) CdvdReadSt;
            FlushCache(0);
            SND_ReleaseBunk();
        }
        return 0;
    }


















    lpCdvdRead->state = 0;
    if (lpCdvdRead < (LPCDVDREAD* )&CdvdReadSt[0xFF]) lpCdvdRead += 1;
    else lpCdvdRead = (LPCDVDREAD* ) CdvdReadSt;
    return 0;
}


static inline char* foo() {
    printf("\\%s;1");
}



int CdvdCheckAsyncBusy(void) { return lpLastCdvd->state != 0;

}