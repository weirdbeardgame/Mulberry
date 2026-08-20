#ifndef DRAW_OT_H
#define DRAW_OT_H
#include "common.h"
#include "Model.h"


    void DrawByOT();
    void ClearDrawOT();
    int SetDrawOTModel(MDLMNG * lpMdlMng /* r18 */, float * WorldPos /* r17 */, u_char Priority /* r16 */);
#endif // DRAW_OT_H