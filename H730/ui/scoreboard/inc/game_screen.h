#pragma once

#include "common.h"
#include "main_menu.h"
#include "game.h"

typedef struct {
    main_menu_state_t* main_menu_state;
    const game_data_t* game;
    enum : char { GAME_SCR_STAY, GAME_SCR_GO_MENU, GAME_SCR_GO_SCORE } ready_state;
} game_screen_state_t ;

// event code registered by main code, triggered on wearable packets
extern uint32_t SC_EVENT_WEARABLE;
// event code registered by main code, triggered on button presses
extern uint32_t SC_EVENT_BUTTON;

void game_screen_build(lv_obj_t* scr);
game_screen_state_t* game_screen_ready(void);
// tell the game screen game parameters
void game_screen_init(main_menu_state_t*);