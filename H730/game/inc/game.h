/*************************************************************************************************/
/* File: game.h                                                                                  */
/* Authors: mcprisk                                                                              */
/* Contains the information for each game                                                        */
/*************************************************************************************************/

#ifndef __GAME_H__
#define __GAME_H__

#include "game_types.h"

/* All of the callbacks are stored in the games array which is defined in game.c */
extern const game_data_t games[];
extern const uint32_t num_games;

#endif /* __GAME_H__ */
