/*************************************************************************************************/
/* File: game.h                                                                                  */
/* Authors: mcprisk                                                                              */
/* All of the types necessary for game-related files                                             */
/*************************************************************************************************/

#ifndef __GAME_TYPES_H__
#define __GAME_TYPES_H__

#include "types.h"

/* a button_func should update the game state as needed for the specific game.  These are        */
/* currently defined in per-game files; however, this should be migrated to a user-editable      */
/* format in the future.  Each button func is passed the player calling the function and returns */
/* whether or not the score needs to be updated from a UI perspective.                           */
typedef void (*button_func_t)(player_t);

typedef struct {
    button_func_t button1;
    button_func_t button2;
    button_func_t button3;
    button_func_t button4;
    /* TODO: Add pointers to audio files for inc, dec, rst, win */
    const char* game_name;
} game_data_t;

/* 2 player specific macros */
#define OTHER_PLAYER (player == PLAYER_1) ? PLAYER_2 : PLAYER_1
#define ENSURE_TWO_PLAYERS() if (player != PLAYER_1 && player != PLAYER_2) return;

#endif /* __GAME_TYPES_H__ */
