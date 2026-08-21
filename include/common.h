#ifndef COMMON_H
#define COMMON_H

#include "typedefs.h"

#define UNK_TYPE s32
#define UNK_PTR void*
#define UNK_RET void
#define UNK_FUN_ARG void(*)(void)
#define UNK_FUN_PTR(name) void(*name)(void)
#define UNK_ARGS

#define NULL 0
#define TRUE 1
#define FALSE 0
#define b32 s32 // 32-bit boolean (TRUE/FALSE)

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ARRAY_COUNTU(arr) (u32)(sizeof(arr) / sizeof(arr[0]))

#define PI 3.1415927f
#define HALF_PI 1.5707964f
#define TWO_PI 6.2831855f

float atan2f(float, float);
float atanf(float);
float cosf(float);
float sinf(float);
float sqrtf(float);

#define BSS __attribute__ ((section (".bss")))

#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME)
#endif
#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME)
#endif

#endif /* COMMON_H */
