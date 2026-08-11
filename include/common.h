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

// #define PI 3.1415928f

f32 cosf(f32);
f32 atan2f(f32, f32);

#define BSS __attribute__ ((section (".bss")))

#endif /* COMMON_H */
