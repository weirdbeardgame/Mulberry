#ifndef SND_SQ_H
#define SND_SQ_H
#include "common.h"

void SND_SqFadeStart(PORT port, FADE fade);
int SND_SqPlay(PORT port, int iBlockNum);
#endif // SND_SQ_H
