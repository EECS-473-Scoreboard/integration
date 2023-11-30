#include "game_screen.h"
#include "common.h"
#include "score.h"

static game_screen_state_t state;

static lv_obj_t *game_name_lbl;
static lv_obj_t *p1_name_lbl;
static lv_obj_t *p2_name_lbl;
static lv_obj_t *p1_score_table;
static lv_obj_t *p2_score_table;
static lv_obj_t *ScoreScreenBtn;
static lv_obj_t *RetBtn;
static lv_obj_t *BTN_1;
static lv_obj_t *BTN_2;
static lv_obj_t *BTN_3;
static lv_obj_t *BTN_4;
static lv_obj_t *BTN_5;

static void render_scores() {
    static char p1_chars[3] = {0};
    static char p2_chars[3] = {0};

    if (overrides[PLAYER_1].overridden) {
        if (overrides[PLAYER_1].left_bitmap == ADVANTAGE_BITMAP_L) {
            p1_chars[0] = 'A';
        } else if (overrides[PLAYER_1].left_bitmap == CLEAR_BITMAP_L) {
            p1_chars[0] = ' ';
        } else {
            p1_chars[0] = '0';
        }

        if (overrides[PLAYER_1].right_bitmap == ADVANTAGE_BITMAP_R) {
            p1_chars[1] = 'd';
        } else if (overrides[PLAYER_1].right_bitmap == CLEAR_BITMAP_R) {
            p1_chars[1] = ' ';
        } else {
            p1_chars[1] = '0';
        }
    } else {
        int score = get_score(PLAYER_1);
        p1_chars[0] = '0' + (score / 10);
        p1_chars[1] = '0' + (score % 10);
    }

    if (overrides[PLAYER_2].overridden) {
        if (overrides[PLAYER_2].left_bitmap == ADVANTAGE_BITMAP_L) {
            p2_chars[0] = 'A';
        } else if (overrides[PLAYER_2].left_bitmap == CLEAR_BITMAP_L) {
            p2_chars[0] = ' ';
        } else {
            p2_chars[0] = '0';
        }

        if (overrides[PLAYER_2].right_bitmap == ADVANTAGE_BITMAP_R) {
            p2_chars[1] = 'd';
        } else if (overrides[PLAYER_2].right_bitmap == CLEAR_BITMAP_R) {
            p2_chars[1] = ' ';
        } else {
            p2_chars[1] = '0';
        }
    } else {
        int score = get_score(PLAYER_2);
        p2_chars[0] = '0' + (score / 10);
        p2_chars[1] = '0' + (score % 10);
    }

    lv_table_set_cell_value(p1_score_table, 0, 0, p1_chars);
    lv_table_set_cell_value(p2_score_table, 0, 0, p2_chars);
}

static void wearable_packet_rcvd(lv_event_t *e) {
    wearable_event_t event = (wearable_event_t)(e->param);

    player_t player = event.fields.id == state.main_menu_state->wearable_1
                          ? PLAYER_1
                          : PLAYER_2;
    switch (event.fields.act) {
    case WEARABLE_ACT_0:
        state.game->button1(player);
        break;
    case WEARABLE_ACT_1:
        state.game->button2(player);
        break;
    case WEARABLE_ACT_2:
        state.game->button3(player);
        break;
    case WEARABLE_ACT_3:
        state.game->button4(player);
        break;
    }

    render_scores();
}

void game_screen_init(main_menu_state_t *menu_state) {
    state.main_menu_state = menu_state;
    state.ready_state = GAME_SCR_STAY;

    switch (menu_state->game) {
    case PINGPONG:
        lv_label_set_text(game_name_lbl, "PING PONG");
        state.game = &games[2];
        break;
    case CORNHOLE:
        lv_label_set_text(game_name_lbl, "CORNHOLE");
        state.game = &games[1];
        break;
    case TENNIS:
        lv_label_set_text(game_name_lbl, "TENNIS");
        state.game = &games[0];
        break;
    }
    lv_label_set_text(p1_name_lbl, menu_state->p1_name);
    lv_label_set_text(p2_name_lbl, menu_state->p2_name);

    init_score();
    render_scores();
}

static void btn_pressed(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);

    if (btn == RetBtn) {
        state.ready_state = GAME_SCR_GO_MENU;
    }
}

inline static void set_ret_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 150);
    lv_obj_set_height(btn, 50);

    lv_obj_t *retLabel = lv_label_create(btn);
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, "Return");
}

inline static void set_act_btn_style(lv_obj_t *btn, char *name) {
    lv_obj_set_width(btn, 150);
    lv_obj_set_height(btn, 50);

    lv_obj_t *retLabel = lv_label_create(btn);
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, name);
}

inline static void set_scor_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 150);
    lv_obj_set_height(btn, 50);

    lv_obj_t *retLabel = lv_label_create(btn);
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, "Scores");
}

static void align_table_cb(lv_event_t *e) {
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
    if (dsc->part == LV_PART_ITEMS) {
        dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
    }
}

void game_screen_build(lv_obj_t *scr) {
    lv_obj_set_style_bg_color(scr, COLOR_GREY, LV_PART_MAIN);

    lv_obj_add_event_cb(scr, wearable_packet_rcvd, SC_EVENT_WEARABLE, NULL);

    game_name_lbl = lv_label_create(scr);
    lv_obj_set_style_text_align(game_name_lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(game_name_lbl, LV_ALIGN_CENTER, 0, -360);

    p1_name_lbl = lv_label_create(scr);
    lv_obj_align(p1_name_lbl, LV_ALIGN_CENTER, 0, -300);
    p1_score_table = lv_table_create(scr);
    lv_obj_align(p1_score_table, LV_ALIGN_CENTER, 0, -260);

    p2_name_lbl = lv_label_create(scr);
    lv_obj_align(p2_name_lbl, LV_ALIGN_CENTER, 0, -200);
    p2_score_table = lv_table_create(scr);
    lv_obj_align(p2_score_table, LV_ALIGN_CENTER, 0, -160);

    RetBtn = lv_btn_create(scr);
    lv_obj_align(RetBtn, LV_ALIGN_CENTER, -80, 40);
    set_ret_btn_style(RetBtn);

    ScoreScreenBtn = lv_btn_create(scr);
    lv_obj_align(ScoreScreenBtn, LV_ALIGN_CENTER, 80, 40);
    set_scor_btn_style(ScoreScreenBtn);

    BTN_1 = lv_btn_create(scr);
    lv_obj_align(BTN_1, LV_ALIGN_CENTER, -80, 100);
    set_act_btn_style(BTN_1, "P1+");

    BTN_2 = lv_btn_create(scr);
    lv_obj_align(BTN_2, LV_ALIGN_CENTER, -80, 160);
    set_act_btn_style(BTN_2, "P1-");

    BTN_3 = lv_btn_create(scr);
    lv_obj_align(BTN_3, LV_ALIGN_CENTER, 80, 100);
    set_act_btn_style(BTN_3, "P2+");

    BTN_4 = lv_btn_create(scr);
    lv_obj_align(BTN_4, LV_ALIGN_CENTER, 80, 160);
    set_act_btn_style(BTN_4, "P2-");

    BTN_5 = lv_btn_create(scr);
    lv_obj_align(BTN_5, LV_ALIGN_CENTER, 0, 220);
    set_act_btn_style(BTN_5, "Reset");

    lv_obj_add_event_cb(p1_score_table, align_table_cb,
                        LV_EVENT_DRAW_PART_BEGIN, NULL);
    lv_obj_add_event_cb(p2_score_table, align_table_cb,
                        LV_EVENT_DRAW_PART_BEGIN, NULL);

    lv_obj_add_event_cb(RetBtn, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ScoreScreenBtn, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_1, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_2, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_3, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_4, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_5, btn_pressed, LV_EVENT_CLICKED, NULL);
}

game_screen_state_t *game_screen_ready() { return &state; }