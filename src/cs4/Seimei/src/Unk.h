#ifndef UNK_H
#define UNK_H

typedef struct _MNG_STATE_ST {
    // Members
    int state; // offset 0x0, size 0x4
    u_int flag; // offset 0x4, size 0x4
    u_int calc_total_time; // offset 0x8, size 0x4
    u_int total_time; // offset 0xC, size 0x4
    u_int get_time; // offset 0x10, size 0x4
    u_int dmy[3]; // offset 0x14, size 0xC
} MNG_STATE_ST;

typedef struct _TMP_MENU_MNG_ST {
    // Members
    MNG_STATE_ST mng_state; // offset 0x0, size 0x20
    int DrawState; // offset 0x20, size 0x4
    u_char** pTmpMenuFileBuf; // offset 0x24, size 0x4
    float draw_up_rate; // offset 0x28, size 0x4
    u_char dmy[12]; // offset 0x2C, size 0xC
} TMP_MENU_MNG_ST;

extern TMP_MENU_MNG_ST g_TmpMenuMng; // size: 0x38, address: 0xA00C70

extern struct /* @anon0 */ {
    // Members
    unsigned char bLoad; // offset 0x0, size 0x1
    unsigned char movie_id; // offset 0x1, size 0x1
    unsigned char unused[2]; // offset 0x2, size 0x2
    unsigned short scene_id; // offset 0x4, size 0x2
    unsigned short cam_id; // offset 0x6, size 0x2
}* lpSceneLoad; // size: 0x4, address: 0x257530

static float VZ[4]; // size: 0x10, address: 0x228720
static float VW[4]; // size: 0x10, address: 0x228730


#endif // UNK_H
