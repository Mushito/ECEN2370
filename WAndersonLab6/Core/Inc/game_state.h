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


int placeCoin(int board[6][7], int col, int player);

int checkWinOrTie(int board[6][7], int row, int col, int player);


#endif /* INC_GAME_STATE_H_ */
