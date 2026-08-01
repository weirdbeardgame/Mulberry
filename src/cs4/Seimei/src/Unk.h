#ifndef UNK_H
#define UNK_H

struct _MNG_STATE_ST {
    // Members
    signed int state; // offset 0x0, size 0x4
    unsigned int flag; // offset 0x4, size 0x4
    unsigned int calc_total_time; // offset 0x8, size 0x4
    unsigned int total_time; // offset 0xC, size 0x4
    unsigned int get_time; // offset 0x10, size 0x4
    unsigned int dmy[3]; // offset 0x14, size 0xC
};

struct _TMP_MENU_MNG_ST {
    // Members
    struct _MNG_STATE_ST mng_state; // offset 0x0, size 0x20
    signed int DrawState; // offset 0x20, size 0x4
    unsigned char * * pTmpMenuFileBuf; // offset 0x24, size 0x4
    float draw_up_rate; // offset 0x28, size 0x4
    unsigned char dmy[12]; // offset 0x2C, size 0xC
};

extern struct _TMP_MENU_MNG_ST g_TmpMenuMng; // size: 0x38, address: 0xA00C70

struct /* @anon0 */ {
    // Members
    unsigned char bLoad; // offset 0x0, size 0x1
    unsigned char movie_id; // offset 0x1, size 0x1
    unsigned char unused[2]; // offset 0x2, size 0x2
    unsigned short scene_id; // offset 0x4, size 0x2
    unsigned short cam_id; // offset 0x6, size 0x2
} * lpSceneLoad; // size: 0x4, address: 0x257530



#endif // UNK_H
