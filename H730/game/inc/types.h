#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include "scoreboard_cfg.h"

typedef enum {E_OK, E_NOT_OK} sb_error_t;

#if NUM_PLAYERS == 4
typedef enum {PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4, UNSPECIFIED} player_t
#else
typedef enum {PLAYER_1, PLAYER_2, UNSPECIFIED} player_t;
#endif

#endif /* __TYPES_H__ */
