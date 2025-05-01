/*
 * game_state.c
 *
 *  Created on: Apr 18, 2025
 *      Author: willa
 */

#include "game_state.h"

int8_t placeCoin(uint8_t board[6][7], uint8_t col, uint8_t player) {
    for (int8_t row = 5; row >= 0; row--) {
        if (board[row][col] == 0) {
            board[row][col] = player;
            return row;
        }
    }
    return -1;
}

static uint8_t countDirection(uint8_t board[6][7], uint8_t startRow, uint8_t startCol, uint8_t chngRow, uint8_t chngCol, uint8_t player) {
	uint8_t count = 0;
	uint8_t r = startRow + chngRow;
	uint8_t c = startCol + chngCol;

    while ((r >= 0) && (r < 6) && (c >= 0) && (c < 7) && (board[r][c] == player)) {
        count++;
        r += chngRow;
        c += chngCol;
    }

    return count;
}


// 1 IS PLAYER WIN
// 2 IS TIE
// 0 NO WIN OR TIE, GAME STILL GOING
uint8_t checkWinOrTie(uint8_t board[6][7], uint8_t row, uint8_t col, uint8_t player) {
    if (1 + countDirection(board, row, col, 0, 1, player) + countDirection(board, row, col, 0, -1, player) >= 4)
        return 1;

    if (1 + countDirection(board, row, col, 1, 0, player) + countDirection(board, row, col, -1, 0, player) >= 4)
        return 1;

    if (1 + countDirection(board, row, col, 1, 1, player) + countDirection(board, row, col, -1, -1, player) >= 4)
        return 1;

    if (1 + countDirection(board, row, col, -1, 1, player) + countDirection(board, row, col, 1, -1, player) >= 4)
        return 1;

    for (uint8_t r = 0; r < 6; r++) {
        for (uint8_t c = 0; c < 7; c++) {
            if (board[r][c] == 0)
                return 0; // GAME STILL RUNNING
        }
    }

    return 2;
}



