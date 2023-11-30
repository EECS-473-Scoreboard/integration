#ifndef __CORNHOLE_H__
#define __CORNHOLE_H__

#include "score.h"
#include "game_types.h"

/* Increment Score by one */
void Cornhole_button1(player_t player);

/* Increment Score by three */
void Cornhole_button2(player_t player);

/* Decrement Score by one */
void Cornhole_button3(player_t player);

/* Reset score to 14 */
void Cornhole_button4(player_t player);

#endif /* __CORNHOLE_H__ */
