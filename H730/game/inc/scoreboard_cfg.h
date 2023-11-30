#ifndef __SCOREBOARD_CFG_H__
#define __SCOREBOARD_CFG_H__

/* The number of score displays on the scoreboard */
#define NUM_PLAYERS 2

/* Any other required global configuration options */

/*************************************************************************************************/
/* CONFIGURATION CHECKER                                                                         */
/*************************************************************************************************/

#if NUM_PLAYERS != 4 && NUM_PLAYERS != 2
# error "NUM_PLAYERS IS AN INVALID VALUE"
#endif /* NUM_PLAYERS */

#endif /* __SCOREBOARD_CFG_H__ */
