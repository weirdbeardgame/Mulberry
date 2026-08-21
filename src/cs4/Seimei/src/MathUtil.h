#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "typedefs.h"

extern float V0[4];
extern float V1[4];
extern float VX[4];
extern float VY[4];
extern float VZ[4];
extern float VW[4];
extern float MRX1[4][4];

float Math_GetPosToAngle(float* pos1, float* pos2);
u_int Math_GotoVector2(float* now, float* end, float per, float min, float max);
u_int Math_GotoVector(float* now, float* end, float add);
u_int Math_GotoVectorXZ(float* now, float* end, float add);
float Math_GetAngleDist(float ang1, float ang2);
float Math_GetAngle360(float ang);
float Math_GetDistXZ(float* v1, float* v2);
float Math_GetDist2(float* v1, float* v2);
float Math_GetDist(float* v1, float* v2);
float Math_GotoAngle(float FromAngle, float ToAngle, float Rate);
void Math_VectorToAngleXY(float* Ang, float* Vec);
float Math_VectorToAngle(float xx, float zz);

#endif // MATH_UTIL_H
