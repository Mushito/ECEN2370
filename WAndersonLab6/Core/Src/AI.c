/*
 * AI.c
 *
 *  Created on: Apr 30, 2025
 *      Author: willa
 */

// Core/Src/AI.c
#include "AI.h"

static const int16_t SCORE_FOUR = 100000;
static const int16_t SCORE_THREE = 1000;
static const int16_t SCORE_TWO = 100;
static const int16_t CENTER_WEIGHT = 3;

bool isBoardFull(uint8_t board[6][7]) {
    for (uint8_t r = 0; r < 6; r++){
        for (uint8_t c = 0; c < 7; c++){
            if (board[r][c] == 0)
            	return false;
        }
    return true;
    }
}

uint8_t checkWin(uint8_t board[6][7], uint8_t player) {
    for (uint8_t r = 0; r < 6; r++) {
        for (uint8_t c = 0; c < 7; c++) {
            if (c <= 3 && board[r][c] == player && board[r][c + 1] == player && board[r][c + 2] == player && board[r][c + 3] == player)
            {
                return player;
            }

            if (r <= 2 && board[r][c] == player && board[r + 1][c] == player && board[r + 2][c] == player && board[r + 3][c] == player)
            {
                return player;
            }

            if (r <= 2 && c <= 3 && board[r][c] == player && board[r + 1][c + 1] == player && board[r + 2][c + 2] == player && board[r + 3][c + 3] == player)
            {
                return player;
            }

            if (r >= 3 && c <= 3 && board[r][c] == player && board[r - 1][c + 1] == player && board[r - 2][c + 2] == player && board[r - 3][c + 3] == player)
            {
                return player;
            }
        }
    }

    return 0;
}

int8_t getNextOpenRow(uint8_t board[6][7], uint8_t c) {
    for (int8_t r = 5; r >= 0; r--)
        if (board[r][c] == 0) return r;
    return -1;
}

int16_t scoreWindow(uint8_t window[4], uint8_t player) {
    uint8_t countP = 0;
    uint8_t countE = 0;
	uint8_t countO = 0;
    uint8_t op;

    if (player == AI_PLAYER) {
        op = HUMAN_PLAYER;
    } else {
        op = AI_PLAYER;
    }

    for (uint8_t i = 0; i < 4; i++) {
        if (window[i] == player) countP++;
        else if (window[i] == op) countO++;
        else countE++;
    }

    if (countP == 4)
    	return SCORE_FOUR;

    if (countP == 3 && countE == 1)
    	return SCORE_THREE;

    if (countP == 2 && countE == 2)
    	return SCORE_TWO;

    if (countO == 3 && countE == 1)
    	return -SCORE_THREE;

    return 0;
}

int16_t scorePosition(uint8_t board[6][7], uint8_t player) {
    int16_t score = 0;
    uint8_t centerCnt = 0;
    for (uint8_t r = 0; r < 6; r++)
        if (board[r][3] == player) centerCnt++;
    score += centerCnt * CENTER_WEIGHT;

    uint8_t w[4];
    for (uint8_t r = 0; r < 6; r++){
      for (uint8_t c = 0; c < 4; c++) {
        for (uint8_t i = 0; i < 4; i++)
          w[i] = board[r][c + i];
        score += scoreWindow(w, player);
      }
    }

    for (uint8_t c = 0; c < 7; c++){
      for (uint8_t r = 0; r < 3; r++) {
        for (uint8_t i = 0; i < 4; i++)
          w[i] = board[r + i][c];
        score += scoreWindow(w, player);
      }
    }

    for (uint8_t r = 0; r < 3; r++){
      for (uint8_t c = 0; c < 4; c++) {
        for (uint8_t i = 0; i < 4; i++)
          w[i] = board[r + i][c + i];
        score += scoreWindow(w, player);
      }
    }

    for (uint8_t r = 3; r < 6; r++){
      for (uint8_t c = 0; c < 4; c++) {
        for (uint8_t i = 0; i < 4; i++)
          w[i] = board[r - i][c + i];
        score += scoreWindow(w, player);
      }
    }

    return score;
}

int16_t minimax(uint8_t board[6][7], uint8_t depth, int16_t alpha, int16_t beta, bool maxP) {
    uint8_t win;
    win = checkWin(board, AI_PLAYER);
    if (win != 0) {
        if (win == AI_PLAYER)
            return INT16_MAX;
        else
            return INT16_MIN;
    }

    win = checkWin(board, HUMAN_PLAYER);
    if (win != 0) {
        if (win == HUMAN_PLAYER)
            return INT16_MIN;
        else
            return INT16_MAX;
    }

    if (isBoardFull(board) || depth == 0)
    	return scorePosition(board, AI_PLAYER);

    if (maxP) {
        int16_t val = INT16_MIN;
        for (uint8_t c = 0; c < 7; c++) {
            int8_t r = getNextOpenRow(board, c);

            if (r < 0)
            	continue;

            board[r][c] = AI_PLAYER;
            val = MAX(val, minimax(board, depth - 1, alpha, beta, false)); //////////////TO CHANGE DIFFICULTY, INCREASE DEPTH IN AI.h.
            board[r][c] = 0;
            alpha = MAX(alpha, val);

            if (alpha >= beta)
            	break;
        }
        return val;
    } else {
        int16_t val = INT16_MAX;
        for (uint8_t c = 0; c < 7; c++) {
            int8_t r = getNextOpenRow(board, c);

            if (r < 0)
            	continue;
            board[r][c] = HUMAN_PLAYER;
            val = MIN(val, minimax(board, depth - 1, alpha, beta, true));
            board[r][c] = 0;
            beta = MIN(beta, val);

            if (alpha >= beta)
            	break;
        }
        return val;
    }
}

uint8_t getBestMove(uint8_t board[6][7]) {
    int16_t bestScore = INT16_MIN;
    uint8_t bestC = 0;

    for (uint8_t c = 0; c < 7; c++) {

        int8_t r = getNextOpenRow(board, c);
        if (r < 0) //COLUMN FULL CHECK
        	continue;

        board[r][c] = AI_PLAYER;
        int16_t score = minimax(board, MAX_DEPTH - 1, INT16_MIN, INT16_MAX, false);
        board[r][c] = 0;
        if (score > bestScore) {
            bestScore = score;
            bestC = c;
        }
    }
    return bestC;
}
