#include "score.h"
#include "7Seg.h"
#include "scoreboard_cfg.h"

static uint8_t score[NUM_PLAYERS];
static int win;

override_t overrides[NUM_PLAYERS];

/* Initialize Score Private Members */
void init_score() {
    win = 0;
    for (uint8_t i = 0; i < NUM_PLAYERS; i++) {
        score[i] = 0;
        overrides[i].overridden = 0;
        overrides[i].left_bitmap = 0;
        overrides[i].right_bitmap = 0;
    }
    display_score();
}

void display_score() {
    /* check for overrides */
    for (int8_t i = 0; i < NUM_PLAYERS; i++) {
        if (overrides[i].overridden) {
            change_digit(2 * i, overrides[i].left_bitmap);
            change_digit(2 * i + 1, overrides[i].right_bitmap);
        } else {
            change_digit(2 * i, get_bitmap(score[i] / 10));
            change_digit(2 * i + 1, get_bitmap(score[i] % 10));
        }
    }
    start_display_saved();
}

/* Get the current score */
int get_score(player_t player) {
    return player < NUM_PLAYERS ? score[player] : -1;
}

int get_win() { return win; }

/* Set the score */
void set_score(player_t player, uint8_t val) {
    if (player < NUM_PLAYERS)
        score[player] = val;
}

/* Adjust the score */
void modify_score(player_t player, int delta) {
    if (delta * -1 > score[player]) {
        score[player] = 0;
    } else {
        score[player] += delta;
    }
}

/* Set a player's displayed score with a custom bitmap (ie. for tennis 'Ad') */
/* The display will remain overridden until cleared */
void override_display(player_t player, uint8_t ldigit, uint8_t rdigit) {
    overrides[player].overridden = 1;
    overrides[player].left_bitmap = ldigit;
    overrides[player].right_bitmap = rdigit;
}

/* clear any display override */
void clear_display(player_t player) { overrides[player].overridden = 0; }

/* TODO: play the provided win sound effect, notify the UI of completed game, */
/* update SD card database, 'dance' the display, reset score values */
void set_winner(player_t player) {
    win = 1;
    start_blink(HAL_GetTick(), 5000);
}

/* TODO: play the provided win sound effect, notify the UI of completed game, */
/* update SD card database, 'dance' the display */
void reset_score() {
    for (uint8_t i = 0; i < NUM_PLAYERS; i++)
        score[i] = 0;
}
