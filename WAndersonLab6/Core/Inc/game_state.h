/*
 * game_state.h
 *
 *  Created on: Apr 18, 2025
 *      Author: willa
 */

#ifndef INC_GAME_STATE_H_
#define INC_GAME_STATE_H_

#include <stdint.h>
#include "LCD_Driver.h"///////////////// MAYBE NEED TO DELETE


int placeCoin(uint8_t board[6][7], uint8_t col, uint8_t player);

int checkWinOrTie(uint8_t board[6][7], uint8_t row, uint8_t col, uint8_t player);


#endif /* INC_GAME_STATE_H_ */
