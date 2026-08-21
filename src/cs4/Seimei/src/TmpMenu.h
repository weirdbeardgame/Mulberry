#ifndef TMP_MENU_H
#define TMP_MENU_H
#include "common.h"
#include "Model.h"

typedef enum {
    TMP_MENU__UDDRAW_ORDER__ON = 0,
    TMP_MENU__UDDRAW_ORDER__OFF = 1,
    TMP_MENU__UDDRAW_ORDER__OFF_FORCE = 2,
} _TMP_MENU__UDDRAW_ORDER_EN;

typedef struct _TMP_MENU_MNG_ST {
    // Members
    _MNG_STATE_ST mng_state; // offset 0x0, size 0x20
    int DrawState; // offset 0x20, size 0x4
    u_char** pTmpMenuFileBuf; // offset 0x24, size 0x4
    float draw_up_rate; // offset 0x28, size 0x4
    u_char dmy[12]; // offset 0x2C, size 0xC
} TMP_MENU_MNG_ST;

extern TMP_MENU_MNG_ST g_TmpMenuMng; // size: 0x38, address: 0xA00C70

int TmpMenuMng_Update(TMP_MENU_MNG_ST * pTmpMenuMng /* r2 */);
int TmpMenuMng_Draw(TMP_MENU_MNG_ST * pTmpMenuMng /* r16 */);
int TmpMenuMng_SetUDBlackDraw(TMP_MENU_MNG_ST * pTmpMenuMng /* r16 */, _TMP_MENU__UDDRAW_ORDER_EN order /* r17 */);

#endif // TMP_MENU_H