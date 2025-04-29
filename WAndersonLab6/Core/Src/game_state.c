/*
 * game_state.c
 *
 *  Created on: Apr 18, 2025
 *      Author: willa
 */

#include "game_state.h"

int placeCoin(int board[6][7], int col, int player) {
    for (int row = 5; row >= 0; row--) {
        if (board[row][col] == 0) {
            board[row][col] = player;
            return row;
        }
    }
    return -1; // COL FULL, IMPLIMENT THIS LATER AND DON'T FORGET
}

static int countDirection(int board[6][7], int startRow, int startCol, int chngRow, int chngCol, int player) {
    int count = 0;
    int r = startRow + chngRow;
    int c = startCol + chngCol;
    while (r >= 0 && r < 6 && c >= 0 && c < 7 && board[r][c] == player) {
        count++;
        r += chngRow;
        c += chngCol;
    }
    return count;
}


// 1 IS PLAYER WIN
// 2 IS TIE
// 0 NO WIN OR TIE, GAME STILL GOING
int checkWinOrTie(int board[6][7], int row, int col, int player) {
	return 0; //TESTING//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    if (1 + countDirection(board, row, col, 0, 1, player) + countDirection(board, row, col, 0, -1, player) >= 4) return 1;
//    if (1 + countDirection(board, row, col, 1, 0, player) + countDirection(board, row, col, -1, 0, player) >= 4) return 1;
//    if (1 + countDirection(board, row, col, 1, 1, player) + countDirection(board, row, col, -1, -1, player) >= 4) return 1;
//    if (1 + countDirection(board, row, col, -1, 1, player) + countDirection(board, row, col, 1, -1, player) >= 4) return 1;
//
//    // NO WIN CHECK TIE
//    for (int r = 0; r < 6; r++) {
//        for (int c = 0; c < 7; c++) {
//            if (board[r][c] == 0) {
//                return 0;
//            }
//        }
//    }
//    return 2;
}


