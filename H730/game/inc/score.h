#ifndef __SCORE_H__
#define __SCORE_H__

/* Maintain current score and communicate with 7Seg Display */
#include <stdint.h>
#include "types.h"

/* Bitmaps for displaying 'Ad' on the 7 segment displays */
#define ADVANTAGE_BITMAP_L 0b11111001
#define ADVANTAGE_BITMAP_R 0b11010011

/* Bitmaps for displaying '  ' on the 7 segment displays */
#define CLEAR_BITMAP_L 0x00
#define CLEAR_BITMAP_R 0x00

typedef struct Override {
    uint8_t overridden;
    uint8_t left_bitmap;
    uint8_t right_bitmap;
} override_t;

extern override_t overrides[];

void init_score();

/* Print score to 7Seg displays */
void display_score();

/* Get the current score */
int get_score(player_t player);

/* Set the score */
void set_score(player_t player, uint8_t val);

/* Display a value on display temporarily */
void display_value(uint8_t val, uint32_t ms);

/* Set a player's displayed score with a custom bitmap (ie. for tennis 'Ad') */
/* The display will remain overridden until cleared */
void override_display(player_t player, uint8_t ldigit, uint8_t rdigit);

/* clear any display override */
void clear_display(player_t player);

/* Adjust a player's score */
void modify_score(player_t player, int delta);

/* declare a winner, TODO: Add sound to this call */
void set_winner(player_t player);

/* Reset the score, TODO: Add sound to this call */
void reset_score();

#endif /* __SCORE_H__ */
