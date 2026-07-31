#ifndef FADE_H
#define FADE_H

void InitFadeControl();

void SetFadeMng(unsigned char Type, signed int * Col, signed int Timer, void (* lpMidFunc)(), void (* lpEndFunc)(), signed int OutEnd, signed int StartIn);


#endif // FADE_H