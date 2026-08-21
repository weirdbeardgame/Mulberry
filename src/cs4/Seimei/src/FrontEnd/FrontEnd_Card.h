#ifndef FRONT_END_CARD_H
#define FRONT_END_CARD_H
#include "common.h"

void FECard_Action(float dt_ /* r20 */);
void FECard_Draw();
void FECard_Enable_Draw(signed int bEnable_ /* r2 */);
int FECard_Initialize();
#endif // FRONT_END_CARD_H
