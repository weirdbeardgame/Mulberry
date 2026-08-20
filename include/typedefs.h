#ifndef TYPEDEFS_H 
#define TYPEDEFS_H

typedef long s_long;
typedef short s_short;
typedef char s_char;
typedef int s_int;
typedef int s_long128 __attribute__ ((mode (TI)));

typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned char u_char;
typedef unsigned int u_int;
typedef unsigned int u_long128 __attribute__ ((mode (TI)));

typedef long int dword[2] __attribute__ ((aligned(16)));;
typedef int qword[4] __attribute__ ((aligned(16)));
typedef int sceVu0IVECTOR[4] __attribute__((aligned (16)));
typedef float sceVu0FVECTOR[4] __attribute__((aligned (16)));
typedef float sceVu0FMATRIX[4][4] __attribute__((aligned (16)));

#define false (u_char)0x0
#define true (u_char)0x1

typedef char s8;
typedef short s16;
typedef int s32;
typedef long s64;

typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef float f32;

typedef union {
    vs8 s8;
    vs16 s16;
    vs32 s32;
    vs64 s64;
    vu8 u8;
    vu16 u16;
    vu32 u32;
    vu64 u64;
} _VX;

typedef union {
	int ui32;
	float fl32;
} U32DATA;

typedef union {
	u_long128 ul128;
	u_long ul64[2];
	u_int ui32[4];
	float fl32[4];
	u_short us16[8];
	u_char uc8[16];
	sceVu0FVECTOR fv;
	sceVu0IVECTOR iv;
} Q_WORDDATA;

typedef union {
	u_char *pu8;
	u_short *pu16;
	u_int *pu32;
	u_long *pu64;
	s_char *ps8;
	s_short *ps16;
	int *ps32;
	s_long *ps64;
	long int wrk;
} P_INT;

#endif /* TYPEDEFS_H */
