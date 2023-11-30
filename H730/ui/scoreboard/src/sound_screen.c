#include "sound_screen.h"
#include "common.h"

static lv_obj_t *VolumeInc;
static lv_obj_t *IncText;
static lv_obj_t *VolumeDec;
static lv_obj_t *DecText;
static lv_obj_t *Ret;

inline static void set_textarea_style(lv_obj_t *ta) {
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_max_length(ta, MAX_NAME_LEN);
    lv_obj_set_width(ta, 100);
    lv_obj_set_height(ta, 100);
    lv_obj_set_style_text_align(ta, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
}

inline static void set_vol_up_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 100);
    lv_obj_set_height(btn, 100);

    lv_obj_t *PlusLabel = lv_label_create(btn);
    lv_obj_center(PlusLabel);
    lv_label_set_text(PlusLabel, "+");
}

inline static void set_vol_down_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 100);
    lv_obj_set_height(btn, 100);

    lv_obj_t *Minus_Label = lv_label_create(btn);
    lv_obj_center(Minus_Label);
    lv_label_set_text(Minus_Label, "-");
}

inline static void set_ret_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 150);
    lv_obj_set_height(btn, 50);

    lv_obj_t *ret_Label = lv_label_create(btn);
    lv_obj_center(ret_Label);
    lv_label_set_text(ret_Label, "Return");
}

void sound_screen_build(lv_obj_t *scr) {

    lv_obj_set_style_bg_color(scr, COLOR_GREY, LV_PART_MAIN);

    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Volume Change");
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -360);

    IncText = lv_textarea_create(scr);
    set_textarea_style(IncText);
    lv_obj_set_pos(IncText, 150, 84);
    lv_obj_t *IncLabel = lv_label_create(scr);
    lv_label_set_text(IncLabel, "Volume up");
    lv_obj_align_to(IncLabel, IncText, LV_ALIGN_BOTTOM_MID, 0, 40);

    VolumeInc = lv_btn_create(scr);
    // add a button event
    lv_obj_set_pos(VolumeInc, 150, 84);
    set_vol_up_btn_style(VolumeInc);

    DecText = lv_textarea_create(scr);
    set_textarea_style(DecText);
    lv_obj_set_pos(DecText, 150, 300);
    lv_obj_t *DecLabel = lv_label_create(scr);
    lv_label_set_text(DecLabel, "Volume down");
    lv_obj_align_to(DecLabel, DecText, LV_ALIGN_BOTTOM_MID, 0, 40);

    VolumeDec = lv_btn_create(scr);
    // add a button event
    lv_obj_set_pos(VolumeDec, 150, 300);
    set_vol_down_btn_style(VolumeDec);

    /*
    Ret_Text = lv_textarea_create(scr);
    set_ret_textarea_style(Ret_Text);
    lv_obj_set_pos(Ret_Text,50,600);
    lv_obj_t *retLabel = lv_label_create(scr);
    lv_label_set_text(retLabel, "Return To Main Menu");
    lv_obj_align_to(retLabel,Ret_Text,LV_ALIGN_BOTTOM_MID,0,40);
    */

    Ret = lv_btn_create(scr);
    lv_obj_set_pos(Ret, 50, 600);
    set_ret_btn_style(Ret);
}