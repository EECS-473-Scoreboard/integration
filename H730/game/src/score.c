// #include "7Seg.h"
#include "score.h"
#include "scoreboard_cfg.h"

static uint8_t score[NUM_PLAYERS];

override_t overrides[NUM_PLAYERS];

/* Initialize Score Private Members */
void init_score() {
    for (uint8_t i = 0; i < NUM_PLAYERS; i++) {
        score[i] = 0;
        overrides[i].overridden = 0;
        overrides[i].left_bitmap = 0;
        overrides[i].right_bitmap = 0;
    }
}

void display_score() {
    /* check for overrides */
    for (int8_t i = NUM_PLAYERS - 1; i > 0; i--) {
        if (overrides[i].overridden) {
            // send_data(overrides[i].right_bitmap);
            // send_data(overrides[i].left_bitmap);
        } else {
            uint8_t right_digit = score[i] % 10;
            uint8_t left_digit = score[i] / 10;
            // display_number(right_digit);
            // display_number(left_digit);
        }
    }
    /* Flush the additional driver on scoreboard pcb for now */
    // send_data(0x00);
}

/* Display a value on display temporarily */
void display_value(uint8_t val, uint32_t ms) {
    uint8_t right_digit = val % 10;
    uint8_t left_digit = val / 10;

    /* TODO: Do some sort of efficient sleep while waiting to disable the
     * temporary number */
    for (int8_t i = 0; i < NUM_PLAYERS; i++) {
        // display_number(right_digit);
        // display_number(left_digit);
    }
    // send_data(0x00);
}

/* Get the current score */
int get_score(player_t player) {
    return player < NUM_PLAYERS ? score[player] : -1;
}

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
    for (uint8_t i = 0; i < NUM_PLAYERS; i++)
        score[i] = 0;
}

/* TODO: play the provided win sound effect, notify the UI of completed game, */
/* update SD card database, 'dance' the display */
void reset_score() {
    for (uint8_t i = 0; i < NUM_PLAYERS; i++)
        score[i] = 0;
}
