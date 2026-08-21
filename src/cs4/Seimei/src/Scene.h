#ifndef SCENE_H
#define SCENE_H
#include "common.h"

unsigned char GetLoadMovieId();
void ClearLoadMovieId();
void FreeSceneData();
u_int CheckSceneLoad();
void SetLoadCamReserve(u_short CamId /* r2 */);


#endif // SCENE_H
