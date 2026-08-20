#ifndef FONT_H
#define FONT_H
#include "Unk.h"

void FontInit();
void FontDrawEnd();
void FontDrawBegin(int font);
void SetFontColor(COLOR col);
void FontPrintXY(float x /* r21 */, float y /* r20 */, char * lpFormat /* r29+0x348 */);

#endif // FONT_H