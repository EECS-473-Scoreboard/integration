#pragma once

#include "common.h"

typedef struct {
    char p1_name[MAX_NAME_LEN + 1];
    char p2_name[MAX_NAME_LEN + 1];
    wearable_id_t wearable_1;
    wearable_id_t wearable_2;
} main_menu_state_t;

// main code handles memory for screen
void main_menu_build(lv_obj_t* scr);
// index can be 1 or 2
void main_menu_incoming(wearable_id_t, int index);