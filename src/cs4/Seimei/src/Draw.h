#ifndef DRAW_H
#define DRAW_H
#include "common.h"
#include "Model.h"

void DrawClearImage();
void DrawInit();
void DrawSetClearColor(int * col);
void DrawCalcView();
int CheckDramaDrawModel(MDLMNG * lpMdlMng /* r16 */);
void DrawSynch();
int CheckRiddleDrawModel(MDLMNG * lpMdlMng /* r16 */);
void InitLightMat();



#endif // DRAW_H
