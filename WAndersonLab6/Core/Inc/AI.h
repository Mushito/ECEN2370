/*
 * AI.h
 *
 *  Created on: Apr 30, 2025
 *      Author: willa
 */

#ifndef INC_AI_H_
#define INC_AI_H_

#include <limits.h>
#include "game_state.h"
#include <stdint.h>
#include <stdbool.h>

#define AI_PLAYER     2
#define HUMAN_PLAYER  1
#define MAX_DEPTH     2 ////(EASIEST)1-4(HARDEST)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

uint8_t getBestMove(uint8_t board[6][7]);



#endif /* INC_AI_H_ */
