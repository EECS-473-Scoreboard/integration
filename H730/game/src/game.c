/*************************************************************************************************/
/* File: game.c                                                                                  */
/* Authors: mcprisk                                                                              */
/* Defines behavior of each game                                                                 */
/*************************************************************************************************/

#include "game.h"
#include "Tennis.h"
#include "Cornhole.h"
#include "Default.h"

#define str(s) #s
#define game_data(n) { n##_button1, n##_button2, n##_button3, n##_button4, str(n) }

const game_data_t games[] = {
    game_data(Tennis),
    game_data(Cornhole),
    game_data(Default),
};

const uint32_t num_games = sizeof(games) / sizeof(games[0]);

