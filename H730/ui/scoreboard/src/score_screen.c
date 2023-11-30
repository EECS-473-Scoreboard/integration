#include "score_screen.h"
#include "common.h"

static lv_obj_t *Ret;

static void draw_part_event_cb(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
    /*If the cells are drawn...*/
    if (dsc->part == LV_PART_ITEMS) {
        uint32_t row = dsc->id / lv_table_get_col_cnt(obj);
        uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);

        /*Make the texts in the first cell center aligned*/
        if (row == 0) {
            dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
            dsc->rect_dsc->bg_color =
                lv_color_mix(lv_palette_main(LV_PALETTE_BLUE),
                             dsc->rect_dsc->bg_color, LV_OPA_20);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
        /*In the first column align the texts to the right*/
        else if (col == 0) {
            dsc->label_dsc->align = LV_TEXT_ALIGN_RIGHT;
        }

        /*MAke every 2nd row grayish*/
        if ((row != 0 && row % 2) == 0) {
            dsc->rect_dsc->bg_color =
                lv_color_mix(lv_palette_main(LV_PALETTE_GREY),
                             dsc->rect_dsc->bg_color, LV_OPA_10);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
    }
}

inline static void set_ret_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 150);
    lv_obj_set_height(btn, 50);

    lv_obj_t *retLabel = lv_label_create(btn);
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, "Return");
}

void score_screen_build(lv_obj_t *scr) {
    lv_obj_set_style_bg_color(scr, COLOR_GREY, LV_PART_MAIN);
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Scores");
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -360);

    lv_obj_t *table = lv_table_create(scr);
    lv_table_set_cell_value(table, 0, 0, "Name");
    lv_table_set_cell_value(table, 0, 1, "W/L \%");
    lv_table_set_cell_value(table, 0, 2, "# won");
    lv_table_set_cell_value(table, 1, 0, "Zach");
    lv_table_set_cell_value(table, 1, 1, "0xDEAD");
    lv_table_set_cell_value(table, 2, 0, "Richard");
    lv_table_set_cell_value(table, 1, 2, "0xBEEF");
    // lv_obj_set_height(table,400);
    lv_obj_set_pos(table, 3, 100);
    lv_obj_set_height(table, 500);

    lv_obj_add_event_cb(table, draw_part_event_cb, LV_EVENT_DRAW_PART_BEGIN,
                        NULL);

    Ret = lv_btn_create(scr);
    lv_obj_set_pos(Ret, 50, 600);
    set_ret_btn_style(Ret);
}