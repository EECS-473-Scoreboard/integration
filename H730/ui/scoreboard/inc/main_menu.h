#pragma once

#include "common.h"

typedef struct {
    char p1_name[MAX_NAME_LEN + 1];
    char p2_name[MAX_NAME_LEN + 1];
    wearable_id_t wearable_1;
    wearable_id_t wearable_2;
    game_t game;
} main_menu_state_t;

extern lv_obj_t* main_menu;
// event code registered by main code, triggered on wearable packets
extern uint32_t SC_EVENT_WEARABLE;

// main code handles memory for screen
void main_menu_build(lv_obj_t* scr);
// main code poll this. If !NULL, proceed to game with settings provided
main_menu_state_t* main_menu_ready();
// reset main menu to initial state
void main_menu_reset();