#ifndef MAIN_H
#define MAIN_H

void SetGoToEndingFade();
int CheckGoToEndingFlag();
void SetGoToEndingFlag();
void SetGoToTitleFade();
int CheckGoToTitleFlag();
void SetGoToTitleFlag();
void SetKeyConfig(unsigned char Type);
void SifLoadModule();
void ClearTime();
volatile int GetTime();
void Timer0Handler();
int main(int argc, char **argv);

#endif // MAIN_H
