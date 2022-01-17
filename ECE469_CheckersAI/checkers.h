/*
Joongyeon Cho
ECE469: Proj1
checkers.h
*/

#ifndef CHECKER_H
#define CHECKER_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>
#include <limits>
#include <climits>
#include "colormod.h"
#include "game.h"

namespace Game
{
	class checkers
	{
	public:
		checkers();

		bool isRepeatJump(int row, int col, int row_to, int col_to, std::vector<Coordinate>& jumpPath);
		void getJumps(int board[8][8], int turn, int x_to, int y_to, bool isKing, std::vector<std::vector<Coordinate>*>& jumpMoves, std::vector<Coordinate>& tempPath, std::vector<std::vector<Coordinate>>& tempMoves);
		void getLegalMoves(int turn);
		void getLegalMoves(int turn, int board[8][8], std::vector<std::vector<Coordinate>>& vecMoves, std::vector<std::vector<Coordinate>*>& jumpMoves, std::vector<std::vector<Coordinate>>& tempMoves);

		void getCopyBoard(int[8][8], int[8][8]);
		void printBoard();

		void makeMove(int choice, int board[8][8], std::vector<std::vector<Coordinate>>& vecMoves, std::vector<std::vector<Coordinate>*>& jumpMoves);
		void makeMove(int choice);
		int heuristic(int board[8][8], int turn, int currentDepth, std::vector<std::vector<Coordinate>> vecMoves, bool isJump);

		int alpha_beta(int board[8][8], int turn, int depth, int currentDepth, int alpha, int beta, time_t end, bool& timeUp);
		int iterative_deepening(int time_per_move, int turn);

		void printMoves();

		void initNewGame();
		void loadGame(const std::string& lFile);
		void startGame(int, int, bool, bool);

		std::vector<std::vector<Coordinate>> vMoves;
		std::vector<std::vector<Coordinate>*> jMoves;
		std::vector<std::vector<Coordinate>> tMoves; // for storing jumps in case there are multiple jumps available

		int _thisBoard[8][8];
		bool gameOver;
		int loadTurn;
		int load_timeLimit;
		int gameOverDepth;
	};
}

#endif