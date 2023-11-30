/*************************************************************************************************/
/* File: template.h                                                                              */
/* Authors: mcprisk                                                                              */
/* Macros for quickly creating new game rulesets                                                 */
/*************************************************************************************************/

/* Template for making games */
#define GAME_TEMPLATE(name) \
    static void button1(player_t), button2(player_t), button3(player_t), button4(player_t); \
    const game_data_t name##_game_data = \
    { button1, button2, button3, button4 };

/* Shorthand for button functions */
#define BUTTON(n) static void button##n(player_t player)

/* 2 player specific macros */
#define OTHER_PLAYER (player == PLAYER_1) ? PLAYER_2 : PLAYER_1
#define ENSURE_TWO_PLAYERS() if (player != PLAYER_1 && player != PLAYER_2) return;
