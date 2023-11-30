#include "Default.h"
#include "game_types.h"
#include "score.h"

static const uint8_t target_scores[] = {7, 11, 21, 99};
static const uint8_t num_target_scores = sizeof(target_scores) / sizeof(target_scores[0]);
static uint8_t target_score_idx = 0;

/* Increment Score by one */
void Default_button1(player_t player) {
    ENSURE_TWO_PLAYERS();

    /* TODO: Play increment sound effect */

    modify_score(player, 1);
    display_score();

    if (get_score(player) >= target_scores[target_score_idx]) {
        set_winner(player);
    }
}

/* decrement score by one*/
void Default_button2(player_t player) {
    ENSURE_TWO_PLAYERS();

    /* TODO: Play decrement sound effect */

    modify_score(player, -1);
    display_score();
}

/* Change target score */
void Default_button3(player_t player) {
    if (target_score_idx >= num_target_scores - 1)
        target_score_idx = 0;
    else
        target_score_idx++;

    /* TODO: Play score target change sound effect */

    /* Print new value to 7Seg Displays for 1 second */
    display_value(target_scores[target_score_idx], 1000);

    /* Reset scores */
    set_score(player, 0);
    set_score(OTHER_PLAYER, 0);
    display_score();
}

/* Reset score */
void Default_button4(player_t player) {
    reset_score();
}
