#include "main_menu.h"
#include "common.h"

#include "lvgl/src/core/lv_event.h"
#include "lvgl/src/core/lv_obj.h"
#include "lvgl/src/core/lv_obj_pos.h"
#include "lvgl/src/core/lv_obj_tree.h"
#include "lvgl/src/misc/lv_area.h"
#include "lvgl/src/misc/lv_color.h"
#include "lvgl/src/misc/lv_txt.h"
#include "lvgl/src/widgets/lv_btn.h"
#include "lvgl/src/widgets/lv_label.h"
#include "lvgl/src/widgets/lv_textarea.h"

static enum screen_state_t {
    IDLE,
    P1_PARING,
    P2_PARING,
} menu_state;

static lv_obj_t *p1_textarea;
static lv_obj_t *p1_pair_btn;
static lv_obj_t *p2_textarea;
static lv_obj_t *p2_pair_btn;

static void pair_btn_pressed(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    switch (menu_state) {
    case IDLE:
        lv_label_set_text(lv_obj_get_child(btn, 0), "Pairing");
        menu_state = btn == p1_pair_btn ? P1_PARING : P2_PARING;
        break;
    case P1_PARING:
        if (btn == p1_pair_btn) {
            lv_label_set_text(lv_obj_get_child(p1_pair_btn, 0), "Pair");
            menu_state = IDLE;
        } else {
            lv_label_set_text(lv_obj_get_child(p1_pair_btn, 0), "Pair");
            lv_obj_clear_state(p1_pair_btn, LV_STATE_CHECKED);
            lv_label_set_text(lv_obj_get_child(p2_pair_btn, 0), "Pairing");
            menu_state = P2_PARING;
        }
        break;
    case P2_PARING:
        if (btn == p2_pair_btn) {
            lv_label_set_text(lv_obj_get_child(p2_pair_btn, 0), "Pair");
            menu_state = IDLE;
        } else {
            lv_label_set_text(lv_obj_get_child(p2_pair_btn, 0), "Pair");
            lv_obj_clear_state(p2_pair_btn, LV_STATE_CHECKED);
            lv_label_set_text(lv_obj_get_child(p1_pair_btn, 0), "Pairing");
            menu_state = P1_PARING;
        }
        break;
    }
}

inline static void set_textarea_style(lv_obj_t *ta) {
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_max_length(ta, MAX_NAME_LEN);
    lv_obj_set_width(ta, 216);
    lv_obj_set_height(ta, 42);
    lv_obj_set_style_text_align(ta, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
}

inline static void set_pair_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 116);
    lv_obj_set_height(btn, 42);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_t *lbl = lv_label_create(btn);
    lv_obj_center(lbl);
    lv_label_set_text(lbl, "Pair");
}

void main_menu_build(lv_obj_t *scr) {
    menu_state = IDLE;

    lv_obj_set_style_bg_color(scr, COLOR_GREY, LV_PART_MAIN);

    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "SCORE++ | Scoreboard");
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -360);

    p1_textarea = lv_textarea_create(scr);
    set_textarea_style(p1_textarea);
    lv_obj_set_pos(p1_textarea, 12, 84);
    lv_obj_t *p1_label = lv_label_create(scr);
    lv_label_set_text(p1_label, "Player 1");
    lv_obj_align_to(p1_label, p1_textarea, LV_ALIGN_BOTTOM_MID, 0, 40);

    p1_pair_btn = lv_btn_create(scr);
    lv_obj_add_event_cb(p1_pair_btn, pair_btn_pressed, LV_EVENT_CLICKED, NULL);
    set_pair_btn_style(p1_pair_btn);
    lv_obj_set_pos(p1_pair_btn, 256, 84);

    p2_textarea = lv_textarea_create(scr);
    set_textarea_style(p2_textarea);
    lv_obj_set_pos(p2_textarea, 12, 186);
    lv_obj_t *p2_label = lv_label_create(scr);
    lv_label_set_text(p2_label, "Player 2");
    lv_obj_align_to(p2_label, p2_textarea, LV_ALIGN_BOTTOM_MID, 0, 40);

    p2_pair_btn = lv_btn_create(scr);
    lv_obj_add_event_cb(p2_pair_btn, pair_btn_pressed, LV_EVENT_CLICKED, NULL);
    set_pair_btn_style(p2_pair_btn);
    lv_obj_set_pos(p2_pair_btn, 256, 186);
}