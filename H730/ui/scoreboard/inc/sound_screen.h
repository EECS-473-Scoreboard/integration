#pragma once
#include "common.h"
#include "main_menu.h"

typedef struct {
    main_menu_state_t* main_menu_state;
    enum : char { SOUND_SCR_STAY, SOUND_SCR_GO_MENU, SOUND_SCR_GO_SCORE } ready_state;
} sound_screen_state_t ;
// main code handles memory for screen
void sound_screen_build(lv_obj_t* scr);
sound_screen_state_t* sound_screen_ready(void);
