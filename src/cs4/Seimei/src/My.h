#ifndef MY_H
#define MY_H
#include "common.h"
#include "Model.h"

void MyAppear(signed int MyNo /* r2 */, struct _mdlmng * lpMy /* r18 */, float * Pos /* r16 */, float AngY /* r20 */);

void CheckMyEquipUpdate();
void MyPlay(MDLMNG * lpMy /* r18 */);
void MyReset(MDLMNG * lpMy /* r17 */);
void StabilizeMyCloth();


#endif // MY_H