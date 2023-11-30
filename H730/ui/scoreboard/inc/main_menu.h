#pragma once

#include "common.h"

typedef struct {
    // allocated by LVGL
    const char* p1_name;
    const char* p2_name;
    wearable_id_t wearable_1;
    wearable_id_t wearable_2;
    game_t game;
    // main code reset to STAY before returning to main menu
    enum : char {
        MAIN_MENU_STAY,
        MAIN_MENU_GO_GAME,
        MAIN_MENU_GO_SOUND,
    } ready_state;
} main_menu_state_t;

// event code registered by main code, triggered on wearable packets
extern uint32_t SC_EVENT_WEARABLE;

// main code handles memory for screen
void main_menu_build(lv_obj_t* scr);
// main code poll this. If ->ready_state == MAIN_MENU_GO_GAME, get the data
main_menu_state_t* main_menu_ready(void);
// reset main menu to initial state
void main_menu_reset(void);