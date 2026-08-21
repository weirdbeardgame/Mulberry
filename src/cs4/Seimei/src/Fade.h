#ifndef FADE_H
#define FADE_H
#include "common.h"

void InitFadeControl();
void SetFadeMng(unsigned char Type, signed int * Col, signed int Timer, void (* lpMidFunc)(), void (* lpEndFunc)(), signed int OutEnd, signed int StartIn);
void UpdateFadeMng();

#endif // FADE_H
