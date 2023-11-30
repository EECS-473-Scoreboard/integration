#include "Tennis.h"
#include "game_types.h"
#include "score.h"

/* Increment Score by One */
void Tennis_button1(player_t player) {
    ENSURE_TWO_PLAYERS();

    /* TODO: Play increment sound effect */

    switch (get_score(player)) {
    case 0: /* Intentional Fallthrough */
    case 15:
        modify_score(player, 15);
        display_score();
        break;
    case 30:
        set_score(player, 40);
        display_score();
        break;
    case 40:
        if (get_score(OTHER_PLAYER) == 40) {
            set_score(player, 45);
            /* Display 'Ad' on player's display */
            override_display(player, ADVANTAGE_BITMAP_L, ADVANTAGE_BITMAP_R);
            /* Display '  ' on opponent's display */
            override_display(OTHER_PLAYER, CLEAR_BITMAP_L, CLEAR_BITMAP_R);
            display_score();
        } else if (get_score(OTHER_PLAYER) == 45) {
            /* Remove display overrides if present */
            clear_display(player);
            clear_display(OTHER_PLAYER);
            /* Update Scores */
            set_score(player, 40);
            set_score(OTHER_PLAYER, 40);
            display_score();
        } else {
            set_winner(player);
        }
        break;
    case 45:
        set_winner(player);
        break;
    default:
        return;
    }
}

/* Decrement Score */
void Tennis_button2(player_t player) {
    ENSURE_TWO_PLAYERS();

    /* TODO: Play decrement sound effect */

    switch (get_score(player)) {
    case 15: /* Intentional Fallthrough */
    case 30:
        modify_score(player, -15);
        break;
    case 40:
        set_score(player, 30);
        break;
    case 45:
        set_score(player, 40);
        break;
    }
    display_score();
}

/* Reset Score */
void Tennis_button3(player_t player) {
    reset_score();
    display_score();
}

/* Reset Score */
void Tennis_button4(player_t player) {
    reset_score();
    display_score();
}
