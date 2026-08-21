#ifndef FSFONT_H
#define FSFONT_H

int FsFontSetTexBuf(signed int id /* r2 */, void * pTexFileBuf /* r2 */);
int FsFontSetScale(float x /* r29 */, float y /* r29 */);
int FsFontGetSize(float * pWidth /* r2 */, float * pHeight /* r2 */, u_short * str /* r2 */, u_short lastCode /* r2 */);

#endif // FSFONT_H
