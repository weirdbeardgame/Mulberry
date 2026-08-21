#include "Event.h"
#include "Event0001.h"
#include "Event0002.h"
#include "Event0006.h"
#include "Event0008.h"
#include "Event0009.h"
#include "Event0010.h"
#include "Event0031.h"
#include "Event0032.h"
#include "Event0033.h"
#include "Event0034.h"
#include "Event0035.h"
#include "Event0040.h"
#include "Event0041.h"
#include "Event0051.h"
#include "Event0083.h"
#include "Event0085.h"
#include "Ene.h"
#include "Game.h"
#include "Map.h"
#include "menu/MenuTD.h"
#include "common.h"

// bss
EVENT EventMngSt[50];

void SetEventFunc(void) {
    memset(EventMngSt, 0, sizeof(EventMngSt));
    
    switch (GameSt.sceneNo) {
        case 1: SetEventFuncS0001(); break;                        
        case 2: SetEventFuncS0002(); break;
        case 6: SetEventFuncS0006(); break;
        case 8: SetEventFuncS0008(); break;
        case 9: SetEventFuncS0009(); break;
        case 10: SetEventFuncS0010(); break;
        case 31: SetEventFuncS0031(); break;
        case 32: SetEventFuncS0032(); break;
        case 33: SetEventFuncS0033(); break;
        case 34: SetEventFuncS0034(); break;
        case 35: SetEventFuncS0035(); break;
        case 40: SetEventFuncS0040(); break;
        case 41: SetEventFuncS0041(); break;
        case 51: SetEventFuncS0051(); break;
        case 83: SetEventFuncS0083(); break;
        case 85: SetEventFuncS0085();
    }
}

void InitEventFunc(void) { // not line matched
    EVENT* lpEvent; // r16
    
    lpEvent = EventMngSt;
    do {
        if (lpEvent->InitFunc != NULL) lpEvent->InitFunc(lpEvent);
    } while (++lpEvent != &EventMngSt[50]);
}

void PlayEventFunc(void) { // not line matched
    EVENT* lpEvent;
        
    if (GameSt.GameMode == 0) {
        if (GameSt.Pause == 0) {
            
            if (Menu_TD_IsPlay() != 0) return;
            
                lpEvent = EventMngSt;
                do  
                {
                    if (lpEvent->PlayFunc != NULL) lpEvent->PlayFunc(lpEvent); 
                        
                    
                    
                } while (++lpEvent != &EventMngSt[50]);
            
        }
    }
}

void ActiEventFunc(u_int EventNo, u_int Id) {
    EVENT* lpEvent = &EventMngSt[EventNo];

    if (lpEvent->ActiFunc != NULL) lpEvent->ActiFunc(lpEvent, Id);     
}

int CheckSquareRange(float* Pos, float* Range) {























    
    if ((Pos[0] < Range[0]) || (Pos[0] > Range[2]) || (Pos[2] < Range[1]) || (Pos[2] > Range[3])) return 0;    
    else return 1;
}

void EasyEneAppear(int CtrlId) {
    PARTS* lpParts = GetMapPartsInfoByCtrlId(2, CtrlId);
    int MdlMngIndex = GetMdlMngIndexByCtrlId(2, CtrlId);
    MDLMNG* lpMdlMng = &MdlMng[MdlMngIndex + 161];
    int EneNo, EneDataIndex;      
    EneNo = lpParts->MdlKindId;
    
    
    EneDataIndex = GetMdlDataIndexFromMapMdlInfo(2, EneNo);

    if ((lpParts->Pos != NULL) && (EneDataIndex != -1)) 
        EneAppear(EneNo, EneDataIndex, lpMdlMng, lpParts->Pos, lpParts->Ang[1], lpParts->Scl, 1, 0xFFFF, 0,NULL, NULL);

    
}

void EasyEneAppearEx(int CtrlId, u_short AnimId, u_char bLogicFree) {
    PARTS* lpParts = GetMapPartsInfoByCtrlId(2, CtrlId);
    int MdlMngIndex = GetMdlMngIndexByCtrlId(2, CtrlId);
    MDLMNG* lpMdlMng = &MdlMng[MdlMngIndex + 161];    
    int EneNo, EneDataIndex;     
    EneNo = lpParts->MdlKindId;

    
    EneDataIndex = GetMdlDataIndexFromMapMdlInfo(2, EneNo);

    if ((lpParts->Pos != NULL) && (EneDataIndex != -1)) 
        EneAppear(EneNo, EneDataIndex, lpMdlMng, lpParts->Pos, lpParts->Ang[1], lpParts->Scl, 1, AnimId, bLogicFree, NULL, NULL);


}

void EventEneHitCheck(MDLMNG* lpEne) {
    
    
    
    if (lpEne->GravOff == 0) lpEne->Pos[1] += 0.1f;
    
    if (lpEne->MapHitOff == 0) lpEne->HitFlag = CheckHitMapGetMdlMng(lpEne, lpEne->lpHitMap);    
}

void SetEventModelDrawAndHit(MDLMNG* lpMdl, u_char OnOff) {
    lpMdl->bErase = OnOff;
    lpMdl->HitOff = OnOff;
}
