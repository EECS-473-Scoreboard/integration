#pragma once

#include "lvgl/lvgl.h"

#define MAX_NAME_LEN 12

typedef uint16_t wearable_id_t;
#define NO_WEARABLE 0

typedef uint16_t wearable_act_t;
typedef union {
    // actual content
    struct {
        wearable_id_t id;
        wearable_act_t act;
    } fields;
    // for converting to uint32_t
    uint32_t bits;
    // for converting to void*
    void* word;
} wearable_event_t;
#define NO_WEARABLE_EVENT 0

#define WEARABLE_ACT_0 0x3032
#define WEARABLE_ACT_1 0x3033
#define WEARABLE_ACT_2 0x3034
#define WEARABLE_ACT_3 0x3035

typedef enum : char {
    PINGPONG,
    CORNHOLE,
    TENNIS
} game_t;

#define COLOR_GREY lv_palette_main(LV_PALETTE_GREY)
