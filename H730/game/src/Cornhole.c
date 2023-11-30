#include "Cornhole.h"
#include "game_types.h"
#include "score.h"

/* Increment Score by one */
void Cornhole_button1(player_t player) {
    ENSURE_TWO_PLAYERS();

    /* TODO: Play increment sound effect */

    modify_score(player, 1);
    display_score();

    if (get_score(player) == 21) {
        set_winner(player);
    }
}

/* Increment Score by three */
void Cornhole_button2(player_t player) {
    ENSURE_TWO_PLAYERS();

    /* TODO: Play increment sound effect */

    modify_score(player, 3);
    display_score();

    if (get_score(player) == 21) {
        set_winner(player);
    } else if (get_score(player) > 21) {
        set_score(player, 14);
    }
}

/* Decrement Score by one */
void Cornhole_button3(player_t player) {

    /* TODO: Play decrement sound effect */

    modify_score(player, -1);
    display_score();
}

/* Reset score to 14 */
void Cornhole_button4(player_t player) {
    reset_score();
}
