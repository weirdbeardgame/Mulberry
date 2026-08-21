#include "common.h"

#define REVOLUTIONS(angle) (TWO_PI * (int) ((PI + angle) / TWO_PI))

float V0[4]; // size: 0x10, address: 0x3A5E40
float V1[4] = {1.0, 1.0, 1.0, 1.0}; // size: 0x10, address: 0x2286F0
float VX[4] = {1.0, 0.0, 0.0, 0.0}; // size: 0x10, address: 0x228700
float VY[4] = {0.0, 1.0, 0.0, 0.0}; // size: 0x10, address: 0x228710
float VZ[4] = {0.0, 0.0, 1.0, 0.0}; // size: 0x10, address: 0x228720
float VW[4] = {0.0, 0.0, 0.0, 1.0}; // size: 0x10, address: 0x228730
float MRX1[4][4] = {
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0},
}; // size: 0x40, address: 0x228740

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/MathUtil", Math_GetPosToAngle);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/MathUtil", Math_GotoVector2);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/MathUtil", Math_GotoVector);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/MathUtil", Math_GotoVectorXZ);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/MathUtil", Math_GetAngleDist);

float Math_GetAngle360(float ang) {
    return ang >= 0.0f ? ang : ang + TWO_PI;

}

float Math_GetDistXZ(float* v1, float* v2) {
    float dx;
    float dz;
    float dd;

    dx = v2[0] - v1[0];
    dz = v2[2] - v1[2];
    dd = sqrtf(dx * dx + dz * dz);

    return dd;
}

float Math_GetDist2(float* v1, float* v2) {
    float dx;
    float dy;
    float dz;

    dx = v2[0] - v1[0];
    dy = v2[1] - v1[1];
    dz = v2[2] - v1[2];

    return dx * dx + dy * dy + dz * dz;
}

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/MathUtil", Math_GetDist);

INCLUDE_ASM("config/asm/nonmatchings/cs4/Seimei/src/MathUtil", Math_GotoAngle);

void Math_VectorToAngleXY(float* Ang, float* Vec) {
    float dist = sqrtf(Vec[0] * Vec[0] + Vec[2] * Vec[2]);

    if (dist == 0.0f) {
        if (Vec[1] < 0.0f) Ang[0] = HALF_PI;
        else if (Vec[1] > 0.0f) Ang[0] = -HALF_PI;
        else Ang[0] = 0.0f;
    } else {
        // treat distance as x-axis
        Ang[0] = atanf(-Vec[1] / dist);
    }
    
    Ang[1] = atan2f(Vec[0], Vec[2]);
    Ang[3] = Ang[2] = 0;
}

float Math_VectorToAngle(float xx, float zz) {
    float Angle;
 
    if (zz != 0.0f) {
        Angle = atanf(xx / -zz);
        
        if (-zz > 0.0f) {
            if (xx > 0.0f) Angle -= PI;
            else Angle += PI;
        }
    }
        
    else if (xx > 0.0f) Angle = -HALF_PI;
    else Angle = HALF_PI;

    if (PI - Angle >= 0.0f) return -Angle - REVOLUTIONS(-Angle);
    return TWO_PI + (-Angle - REVOLUTIONS(-Angle));
}
