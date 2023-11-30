#pragma once
#include "common.h"
#include "main_menu.h"
typedef struct{
    main_menu_state_t *main_menu_state;
    enum : char { SCORE_SCR_STAY, SCORE_SCR_GO_GAME } ready_state;
} score_screen_state_t;

void score_screen_build(lv_obj_t* scr);
score_screen_state_t* score_screen_ready(void);
void score_screen_init(main_menu_state_t* menu_state);