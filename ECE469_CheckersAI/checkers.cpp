#include "checkers.h"

int white = 1;
int black = 2;
bool timeUp = false;

int thisTotal = 0;
int thisWTotal = 0;
int thisBTotal = 0;

using namespace Game;

checkers::checkers()
{
	vMoves.resize(50);
	tMoves.resize(100);
	jMoves.resize(100);
}

bool checkers::isRepeatJump(int y_from, int x_from, int y_to, int x_to, std::vector<Coordinate>& jumpPath)
{
	int xJump = (x_from + x_to) / 2;
	int yJump = (y_from + y_to) / 2;
	for (int i = 0; i < jumpPath.size(); i++)
	{
		if (i + 1 == jumpPath.size())
		{
			return false;
		}
		if ((((jumpPath[i].x + jumpPath[i + 1].x) / 2) == xJump) && (((jumpPath[i].y + jumpPath[i + 1].y) / 2) == yJump))
		{
			return true;
		}
	}

	return false;
}

// Check if more jumps exist
void checkers::getJumps(int board[8][8], int turn, int y_to, int x_to, bool isKing, std::vector<std::vector<Coordinate>*>& jumpMoves, std::vector<Coordinate>& tempPath, std::vector<std::vector<Coordinate>>& tempMoves)
{
	int opponent = (turn == white) ? black : white;
	bool multiJumps = false;
	int tempMovesSize;
	std::vector<Coordinate> copyVec = tempPath;

	if ((y_to == 7 && turn == white) || (y_to == 0 && turn == black) && !isKing)
	{
		return;
	}
	if (y_to < 6)
	{
		if (x_to > 1 && (turn == white || isKing) && (board[y_to + 1][x_to - 1] == opponent || board[y_to + 1][x_to - 1] == opponent + 2) && board[y_to + 2][x_to - 2] == 0)
		{
			if (!isRepeatJump(y_to, x_to, y_to + 2, x_to - 2, copyVec))
			{
				multiJumps = true;

				tempPath.push_back(Coordinate(y_to + 2, x_to - 2));
				getJumps(board, turn, y_to + 2, x_to - 2, isKing, jumpMoves, tempPath, tempMoves);
			}
		}
		if (x_to < 6 && (turn == white || isKing) && (board[y_to + 1][x_to + 1] == opponent || board[y_to + 1][x_to + 1] == opponent + 2) && board[y_to + 2][x_to + 2] == 0)
		{
			if (!isRepeatJump(y_to, x_to, y_to + 2, x_to + 2, copyVec))
			{
				if (multiJumps)
				{
					tempMovesSize = tempMoves.size();
					tempMoves.push_back(copyVec);
					tempMoves[tempMovesSize].push_back(Coordinate(y_to + 2, x_to + 2));
					jumpMoves.push_back(&tempMoves[tempMovesSize]);

					getJumps(board, turn, y_to + 2, x_to + 2, isKing, jumpMoves, tempMoves[tempMovesSize], tempMoves);
				}
				else
				{
					multiJumps = true;

					tempPath.push_back(Coordinate(y_to + 2, x_to + 2));
					getJumps(board, turn, y_to + 2, x_to + 2, isKing, jumpMoves, tempPath, tempMoves);
				}
			}
		}
	}
	if (y_to > 1)
	{
		if (x_to > 1 && (turn == black || isKing) && (board[y_to - 1][x_to - 1] == opponent || board[y_to - 1][x_to - 1] == opponent + 2) && board[y_to - 2][x_to - 2] == 0)
		{
			if (!isRepeatJump(y_to, x_to, y_to - 2, x_to - 2, copyVec))
			{
				if (multiJumps)
				{
					tempMovesSize = tempMoves.size();
					tempMoves.push_back(copyVec);
					tempMoves[tempMovesSize].push_back(Coordinate(y_to - 2, x_to - 2));
					jumpMoves.push_back(&tempMoves[tempMovesSize]);

					getJumps(board, turn, y_to - 2, x_to - 2, isKing, jumpMoves, tempMoves[tempMovesSize], tempMoves);
				}
				else
				{
					multiJumps = true;

					tempPath.push_back(Coordinate(y_to - 2, x_to - 2));
					getJumps(board, turn, y_to - 2, x_to - 2, isKing, jumpMoves, tempPath, tempMoves);
				}
			}
		}
		if (x_to < 6 && (turn == black || isKing) && (board[y_to - 1][x_to + 1] == opponent || board[y_to - 1][x_to + 1] == opponent + 2) && board[y_to - 2][x_to + 2] == 0)
		{
			if (!isRepeatJump(y_to, x_to, y_to - 2, x_to + 2, copyVec))
			{
				if (multiJumps)
				{
					tempMovesSize = tempMoves.size();
					tempMoves.push_back(copyVec);
					tempMoves[tempMovesSize].push_back(Coordinate(y_to - 2, x_to + 2));
					jumpMoves.push_back(&tempMoves[tempMovesSize]);

					getJumps(board, turn, y_to - 2, x_to + 2, isKing, jumpMoves, tempMoves[tempMovesSize], tempMoves);
				}
				else
				{
					multiJumps = true;

					tempPath.push_back(Coordinate(y_to - 2, x_to + 2));
					getJumps(board, turn, y_to - 2, x_to + 2, isKing, jumpMoves, tempPath, tempMoves);
				}
			}
		}
	}
}

void checkers::getLegalMoves(int turn, int board[8][8], std::vector<std::vector<Coordinate>>& vecMoves, std::vector<std::vector<Coordinate>*>& jumpMoves, std::vector<std::vector<Coordinate>>& tempMoves)
{
	jumpMoves.clear();
	vecMoves.clear();
	tempMoves.clear();

	std::vector<Coordinate> tempPath;
	int tempMovesSize;
	bool mustJump = false;
	bool isKing;

	int opponent = (turn == white) ? black : white;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if ((x + y) % 2 == 0)
			{
				if (board[y][x] == turn || board[y][x] == turn + 2)
				{
					isKing = board[y][x] > 2;
					if ((board[y][x] == white || board[y][x] > 2) && y != 7)
					{
						if (x != 0)
						{
							if (board[y + 1][x - 1] == 0)
							{
								vecMoves.push_back({ Coordinate(y, x), Coordinate(y + 1, x - 1) });
							}
							else if ((board[y + 1][x - 1] == opponent || board[y + 1][x - 1] == opponent + 2) && board[y + 2][x - 2] == 0 && x > 1 && y != 6)
							{
								mustJump = true;

								tempMovesSize = tempMoves.size();
								tempMoves.push_back({ Coordinate(y, x), Coordinate(y + 2, x - 2) });
								jumpMoves.push_back(&tempMoves[tempMovesSize]);

								getJumps(board, turn, y + 2, x - 2, isKing, jumpMoves, tempMoves[tempMovesSize], tempMoves);
							}
						}

						if (x != 7)
						{
							if (!mustJump && board[y + 1][x + 1] == 0)
							{
								vecMoves.push_back({ Coordinate(y, x), Coordinate(y + 1, x + 1) });
							}
							else if ((board[y + 1][x + 1] == opponent || board[y + 1][x + 1] == opponent + 2) && board[y + 2][x + 2] == 0 && x < 6 && y != 6)
							{
								mustJump = true;

								tempMovesSize = tempMoves.size();
								tempMoves.push_back({ Coordinate(y, x), Coordinate(y + 2, x + 2) });
								jumpMoves.push_back(&tempMoves[tempMovesSize]);

								getJumps(board, turn, y + 2, x + 2, isKing, jumpMoves, tempMoves[tempMovesSize], tempMoves);
							}
						}
					}
					if ((board[y][x] == black || board[y][x] > 2) && y != 0)
					{
						if (x != 0)
						{
							if (!mustJump && board[y - 1][x - 1] == 0)
							{
								vecMoves.push_back({ Coordinate(y, x), Coordinate(y - 1, x - 1) });
							}
							else if ((board[y - 1][x - 1] == opponent || board[y - 1][x - 1] == opponent + 2) && board[y - 2][x - 2] == 0 && y > 1 && x != 1)
							{
								mustJump = true;

								tempMovesSize = tempMoves.size();
								tempMoves.push_back({ Coordinate(y, x), Coordinate(y - 2, x - 2) });
								jumpMoves.push_back(&tempMoves[tempMovesSize]);

								getJumps(board, turn, y - 2, x - 2, isKing, jumpMoves, tempMoves[tempMovesSize], tempMoves);
							}
						}
						if (x != 7)
						{
							if (!mustJump && board[y - 1][x + 1] == 0)
							{
								vecMoves.push_back({ Coordinate(y, x), Coordinate(y - 1, x + 1) });
							}
							else if ((board[y - 1][x + 1] == opponent || board[y - 1][x + 1] == opponent + 2) && board[y - 2][x + 2] == 0 && x < 6 && y != 1)
							{
								mustJump = true;

								tempMovesSize = tempMoves.size();
								tempMoves.push_back({ Coordinate(y, x), Coordinate(y - 2, x + 2) });
								jumpMoves.push_back(&tempMoves[tempMovesSize]);

								getJumps(board, turn, y - 2, x + 2, isKing, jumpMoves, tempMoves[tempMovesSize], tempMoves);
							}
						}
					}
				}
			}
		}
	}
}

void checkers::getLegalMoves(int turn)
{
	getLegalMoves(turn, _thisBoard, vMoves, jMoves, tMoves);
}

void checkers::getCopyBoard(int originalBoard[8][8], int copyBoard[8][8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			copyBoard[i][j] = originalBoard[i][j];
		}
	}
}

void checkers::printBoard()
{
	Color::Modifier greenBG(Color::BG_GREEN);
	Color::Modifier defBG(Color::BG_DEFAULT);
	Color::Modifier redFG(Color::FG_RED);
	Color::Modifier defFG(Color::FG_DEFAULT);

	std::cout << "\n|---|   |---|---|---|---|---|---|---|---|\n";
	for (int row = 7; row >= 0; row--)
	{
		for (int col = 0; col < 8; col++)
		{
			if (col == 0)
			{
				std::cout << "| " << row << " |   ";
			}
			std::cout << "|";

			if ((col + row) & 1)
			{
				std::cout << greenBG << "   " << defBG;
			}
			else
			{
				if (_thisBoard[row][col] == 2 || _thisBoard[row][col] == 4)
					std::cout << " " << redFG << _thisBoard[row][col] << " " << defFG;
				else if (_thisBoard[row][col] == 0) {
					std::cout << "   ";
				}
				else
					std::cout << " " << _thisBoard[row][col] << " ";
			}
			if (col == 7)
			{
				std::cout << "|\n|---|   |---|---|---|---|---|---|---|---|\n";
			}
		}
	}
	std::cout << "\n        | A | B | C | D | E | F | G | H |\n";
}

void checkers::makeMove(int Choice, int board[8][8], std::vector<std::vector<Coordinate>>& vecMoves, std::vector<std::vector<Coordinate>*>& jumpMoves)
{
	int choice = Choice - 1;
	bool jump = jumpMoves.size() > 0;

	int initialX, initialY, toX, toY;
	if (!jump)
	{
		initialX = vecMoves[choice][0].x;
		initialY = vecMoves[choice][0].y;
		toX = vecMoves[choice][1].x;
		toY = vecMoves[choice][1].y;

		//promote
		if (board[initialY][initialX] == 1 && toY == 7)
		{
			board[initialY][initialX] = 3;
		}
		else if (board[initialY][initialX] == 2 && toY == 0)
		{
			board[initialY][initialX] = 4;
		}

		int tmp = board[initialY][initialX];
		board[initialY][initialX] = board[toY][toX];
		board[toY][toX] = tmp;
	}
	else
	{
		for (int i = 0; i < jumpMoves[choice]->size(); i++)
		{
			if (i + 1 == jumpMoves[choice]->size())
			{
				break;
			}

			initialX = jumpMoves[choice]->at(i).x;
			initialY = jumpMoves[choice]->at(i).y;
			toX = jumpMoves[choice]->at(i + 1).x;
			toY = jumpMoves[choice]->at(i + 1).y;

			//promote
			if (board[initialY][initialX] == 1 && toY == 7)
			{
				board[initialY][initialX] = 3;
			}
			else if (board[initialY][initialX] == 2 && toY == 0)
			{
				board[initialY][initialX] = 4;
			}

			int middleX = (jumpMoves[choice]->at(i).x + jumpMoves[choice]->at(i + 1).x) / 2;
			int middleY = (jumpMoves[choice]->at(i).y + jumpMoves[choice]->at(i + 1).y) / 2;
			if (board == _thisBoard) {
				if (board[middleY][middleX] == 1) {
					thisTotal--;
					thisWTotal--;
				}
				if (board[middleY][middleX] == 2) {
					thisTotal--;
					thisBTotal--;
				}
				if (board[middleY][middleX] == 3) {
					thisTotal--;
					thisWTotal--;
				}
				if (board[middleY][middleX] == 4) {
					thisTotal--;
					thisBTotal--;
				}
			}

			board[middleY][middleX] = 0;

			int tmp = board[initialY][initialX];
			board[initialY][initialX] = board[toY][toX];
			board[toY][toX] = tmp;
		}
	}
}

void checkers::makeMove(int Choice)
{
	makeMove(Choice, _thisBoard, vMoves, jMoves);
}

int checkers::heuristic(int board[8][8], int turn, int currentDepth, std::vector<std::vector<Coordinate>> vecMoves, bool isJump)
{
	int value = 0;
	int wPawn = 0, bPawn = 0, wKing = 0, bKing = 0,
		safeWhitePawn = 0, safeBlackPawn = 0, safeWhiteKing = 0, safeBlackKing = 0, wTotal = 0, bTotal = 0, total = 0, wMidBox = 0, bMidBox = 0, wBackRow = 0, bBackRow = 0;
	int wAttackingRow = 0, bAttackingRow = 0, wMidRow = 0, bMidRow = 0;

	int wQ1 = 0, wQ2 = 0, wQ3 = 0, wQ4 = 0, bQ1 = 0, bQ2 = 0, bQ3 = 0, bQ4 = 0;  // {Q1 = NE Quadrant | Q2 = NW Quadrant | Q3 = SW Quadrant | Q4 = SE Quadrant}
	int wTop = 0, wBottom = 0, wLeft = 0, wRight = 0, bTop = 0, bBottom = 0, bLeft = 0, bRight = 0;
	int wKingDC_Top = 0, wKingDC_Bottom = 0, bKingDC_Top = 0, bKingDC_Bottom = 0;

	int wRow = 0, wCol = 0, bRow = 0, bCol = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++) {
			if (!((i + j) & 1)) {
				if (board[j][i] == 1)
				{
					if (j > 4) {
						wTop++;
					}
					else if (j < 3)
					{
						wBottom++;
					}
					else if (i > 4) {
						wRight++;
					}
					else if (i < 3) {
						wLeft++;
					}

					if (j > 3) {
						if (i > 3) {
							wQ1++;
						}
						else {
							wQ2++;
						}
					}
					else {
						if (i > 3) {
							wQ4++;
						}
						else
						{
							wQ3++;
						}
					}

					if (j == 0) {
						wBackRow++;
					}
					//mid row / box
					if ((j > 3 && j < 6)) {
						if ((i > 1 && i < 6))
							wMidBox++;
						wMidRow++;
					}
					// attacking row
					if (j > 4) {
						wAttackingRow++;
					}
					// try to promote
					if (j != 0)
						value += 7 * j;

					wPawn++;
					wTotal++;
					total++;
					wRow = j;
					wCol = i;
				}
				else if (board[j][i] == 2)
				{
					if (j > 4) {
						bTop++;
					}
					else if (j < 3)
					{
						bBottom++;
					}
					else if (i > 4) {
						bRight++;
					}
					else if (i < 3) {
						bLeft++;
					}
					if (j > 3) {
						if (i > 3) {
							bQ1++;
						}
						else {
							bQ2++;
						}
					}
					else {
						if (i > 3) {
							bQ4++;
						}
						else
						{
							bQ3++;
						}
					}
					if (j == 7) {
						bBackRow++;
					}
					if ((j > 3 && j < 6)) {
						if ((i > 1 && i < 6))
							bMidBox++;
						bMidRow++;
					}
					if (j < 3) {
						bAttackingRow++;
					}

					// try to promote
					if (j != 7)
						value -= 7 * (7 - j);

					bPawn++;
					bTotal++;
					total++;
					bRow = j;
					bCol = i;
				}
				else if (board[j][i] == 3)
				{
					if (j > 5) {
						wTop++;
						if (i < 2) {
							wKingDC_Top++;
						}
					}
					else if (j < 2)
					{
						wBottom++;
						if (i > 5) {
							wKingDC_Bottom++;
						}
					}
					else if (i > 4) {
						wRight++;
					}
					else if (i < 3) {
						wLeft++;
					}
					if (j > 3) {
						if (i > 3) {
							wQ1++;
						}
						else {
							wQ2++;
						}
					}
					else {
						if (i > 3) {
							wQ4++;
						}
						else
						{
							wQ3++;
						}
					}
					if (i == 0 || i == 7 || j == 0 || j == 7) {
						safeWhiteKing++;
					}
					if ((j > 3 && j < 6)) {
						if (i > 1 && i < 6)
							wMidBox++;
						wMidRow++;
					}
					if (j > 4) {
						wAttackingRow++;
					}
					wKing++;
					wTotal++;
					total++;
					wRow = j;
					wCol = i;
				}
				else if (board[j][i] == 4)
				{
					if (j > 5) {
						bTop++;
						if (i < 2) {
							bKingDC_Top++;
						}
					}
					else if (j < 2)
					{
						bBottom++;

						if (i > 5) {
							bKingDC_Bottom++;
						}
					}
					else if (i > 4) {
						bRight++;
					}
					else if (i < 3) {
						bLeft++;
					}

					if (j > 3) {
						if (i > 3) {
							bQ1++;
						}
						else {
							bQ2++;
						}
					}
					else {
						if (i > 3) {
							bQ4++;
						}
						else
						{
							bQ3++;
						}
					}

					if (i == 0 || i == 7 || j == 0 || j == 7) {
						if (j == 0 && i == 0) {
						}

						safeBlackKing++;
					}
					if ((j > 3 && j < 6)) {
						if ((i > 1 && i < 6))
							bMidBox++;
						bMidRow++;
					}
					if (j < 3) {
						bAttackingRow++;
					}
					bKing++;
					bTotal++;
					total++;
					bRow = j;
					bCol = i;
				}
			}
		}
	}
	// pieces
	value += (wPawn * 100) + (wKing * 160) - ((bPawn * 100) + (bKing * 160));
	//position
	value += (wBackRow * 20) + (wMidBox * 10) + (wAttackingRow * 10) - ((bBackRow * 20) + (bMidBox * 10) + (bAttackingRow * 10));

	bool wMore = (wTotal - bTotal) > 2;
	bool bMore = (bTotal - wTotal) > 2;

	//If there is clear win
	if (total < 6 || wMore || bMore) {
		if (wMore)
			value -= (safeBlackPawn * 10) + (bBackRow * 20);
		else if (bMore)
			value += (safeWhitePawn * 10) + (wBackRow * 20);

		if (vecMoves.size() == 0 && !isJump) {
			if (turn == 1)
				value += (currentDepth * 50) - 500;
			else
				value += 500 - (currentDepth * 50);
		}
		if (wTotal != bTotal) {
			value += (wPawn * 20) + (wKing * 30) - ((bPawn * 20) + (bKing * 30));

			int winningPlayer = wTotal == 1 ? black : white;
			if (wTotal == 1 || bTotal == 1) {
				int losingPieceRow, losingPieceCol;
				if (wTotal == 1) {
					losingPieceRow = wRow;
					losingPieceCol = wCol;
				}
				else {
					losingPieceRow = bRow;
					losingPieceCol = bCol;
				}
				for (int i = 0; i < 7; i++) {
					for (int j = 0; j < 7; j++) {
						if (board[j][i] == winningPlayer || board[j][i] == winningPlayer + 2) {
							if (abs(losingPieceRow - j) > 4 || abs(losingPieceCol - i) > 4) {
								if (thisWTotal > thisBTotal && wTotal > bTotal) {
									value -= 3 * (abs(losingPieceRow - j) + abs(losingPieceCol - i));
								}
								else if (thisWTotal < thisBTotal && wTotal < bTotal) {
									value += 3 * (abs(losingPieceRow - j) + abs(losingPieceCol - i));
								}
							}
						}
					}
				}
			}

			if (bTotal < wTotal) {
				value -= currentDepth * 2;
				if (thisWTotal > thisBTotal) {
					value += (thisTotal - total) * 70;
				}
				//
				/*if (bKingDC_Top || bKingDC_Bottom) {
					value -= 70 * (bKingDC_Bottom + bKingDC_Top);
					if (bKingDC_Top) {
						value += wKingDC_Top * 30;
					}
					else {
						value += (wBottom * 10) + (wRight * 10);
					}
				}*/
			}

			else if (wTotal < bTotal) {
				value += currentDepth * 2;
				if (thisWTotal < thisBTotal) {
					value -= (thisTotal - total) * 70;
				}

				/*if (wKingDC_Top || wKingDC_Bottom) {
					value += 70 * (wKingDC_Bottom + wKingDC_Top);

					if (wKingDC_Top) {
						value -= bKingDC_Top * 30;
					}
					else {
						value -= bKingDC_Bottom * 30;
					}
				}*/
			}
		}
	}
	//Middle Game
	else if (total < 20) {
		value += (wPawn * 20) + (wKing * 30) - ((bPawn * 20) + (bKing * 30)); // pieces have more value
		value -= (safeWhiteKing * 10) - (safeBlackKing * 10); // king activity

		//side with more pieces should try to trade
		if (thisWTotal > thisBTotal && wTotal > bTotal) {
			value += (120 - (5 * total)) + ((thisTotal - total) * 30);
		}
		else if (thisWTotal < thisBTotal && wTotal < bTotal) {
			value -= (120 - (5 * total)) + ((thisTotal - total) * 30);
		}

		if (wTotal > bTotal)
			value -= currentDepth * 3;
		if (bTotal > wTotal)
			value += currentDepth * 3;
	}
	return value;
}

int checkers::alpha_beta(int board[8][8], int turn, int depth, int currentDepth, int alpha, int beta, time_t end, bool& timeUp)
{
	if (time(nullptr) >= end) {
		timeUp = true;
		return 0;
	}

	int moveChoice = 1;
	int value, tmp;
	int copyBoard[8][8] = {};
	bool isJump;
	int numMoves;

	std::vector<std::vector<Coordinate>> vecMoves;
	std::vector<std::vector<Coordinate>*> jumpMoves;
	std::vector<std::vector<Coordinate>> tempMoves;
	vecMoves.resize(20);
	jumpMoves.resize(100);
	tempMoves.resize(100);

	getLegalMoves(turn, board, vecMoves, jumpMoves, tempMoves);
	isJump = jumpMoves.size() > 0;
	numMoves = isJump ? jumpMoves.size() : vecMoves.size();

	if (currentDepth == depth || numMoves == 0) {
		return heuristic(board, turn, currentDepth, vecMoves, isJump);
	}
	// maximize
	if (turn == white)
	{
		value = INT_MIN;
		for (unsigned int i = 1; i <= numMoves; i++)
		{
			getCopyBoard(board, copyBoard);
			makeMove(i, copyBoard, vecMoves, jumpMoves);
			tmp = alpha_beta(copyBoard, black, depth, currentDepth + 1, alpha, beta, end, timeUp);

			if (tmp > value)
			{
				value = tmp;
				moveChoice = i;
			}
			alpha = std::max(alpha, value);
			if (beta <= alpha)
				break;
		}
	}

	//minimize
	else
	{
		value = INT_MAX;
		for (unsigned int i = 1; i <= numMoves; i++)
		{
			getCopyBoard(board, copyBoard);
			makeMove(i, copyBoard, vecMoves, jumpMoves);
			tmp = alpha_beta(copyBoard, white, depth, currentDepth + 1, alpha, beta, end, timeUp);

			if (tmp < value)
			{
				value = tmp;
				moveChoice = i;
			}
			beta = std::min(beta, value);
			if (beta <= alpha)
				break;
		}
	}

	if (currentDepth == 0)
		return moveChoice;
	else
		return value;
}

int checkers::iterative_deepening(int time_per_move, int turn)
{
	if (jMoves.size() == 1 || vMoves.size() == 1) {
		std::cout << "Only legal move. Searched to depth 0\n";
		return 1;
	}

	time_t start = time(nullptr);
	time_t end = start + time_per_move;
	int moveChoice = 1;
	int move;
	int depth = 1;
	bool timeUp = false;

	do
	{
		move = alpha_beta(_thisBoard, turn, depth, 0, INT_MIN, INT_MAX, end, timeUp);
		if (!timeUp) {
			moveChoice = move;
			depth++;
		}
	} while (time(nullptr) <= end - 1 && depth < 20);
	std::cout << "Searched for: " << (time(nullptr) - start) << "s\n";
	std::cout << "Depth: " << depth - 1 << std::endl;

	return moveChoice;
}

void checkers::printMoves()
{
	if (jMoves.size() > 0)
	{
		for (unsigned int i = 0; i < jMoves.size(); i++)
		{
			std::cout << "[" << i + 1 << "]: ";

			for (int j = 0; j < jMoves[i]->size(); j++)
			{
				std::cout << jMoves[i]->at(j) << "  ";
			}
			std::cout << std::endl;
		}
	}
	else
	{
		for (unsigned int i = 0; i < vMoves.size(); i++)
		{
			std::cout << "[" << i + 1 << "]: ";
			for (Coordinate c : vMoves[i])
			{
				std::cout << c << "  ";
			}
			std::cout << std::endl;
		}
	}
}

void checkers::loadGame(const std::string& lFile)
{
	int idx1 = 0, idx2 = 0;
	std::ifstream loadFile;
	loadFile.open(lFile);

	if (!loadFile.is_open())
	{
		std::cout << "Couldn't open file.."
			<< "\n";
		exit(-1);
	}
	else
	{
		int tmp = 0;
		for (int row = 7; row >= 0; row--)
		{
			for (int col = 0; col < 8; col++)
			{
				if ((row + col) % 2 == 1)
				{
					_thisBoard[row][col] = 0;
				}
				else
				{
					loadFile >> tmp;
					if (static_cast<int>(tmp) == 1 || static_cast<int>(tmp) == 3) {
						thisWTotal++;
						thisTotal++;
					}
					else if (static_cast<int>(tmp) == 2 || static_cast<int>(tmp) == 4) {
						thisBTotal++;
						thisTotal++;
					}

					_thisBoard[row][col] = static_cast<int>(tmp);
				}
			}
		}

		loadFile >> tmp;
		loadTurn = tmp;
		loadFile >> tmp;
		load_timeLimit = tmp;
		loadFile.close();
	}
}

void checkers::initNewGame()
{
	int idx1 = 0, idx2 = 0;
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if ((j + i) & 1)
				_thisBoard[j][i] = 0;
			else if ((j < 3) && !((i + j) & 1))
			{
				_thisBoard[j][i] = 1;
			}
			else if ((j > 4) && !((i + j) & 1))
			{
				_thisBoard[j][i] = 2;
			}
			else
				_thisBoard[j][i] = 0;
		}
	}
	thisTotal = 24;
	thisWTotal = 12;
	thisBTotal = 12;
}
void checkers::startGame(int turn, int timeLimit, bool isWhiteAI, bool isBlackAI)
{
	int choice = 0;
	int size;
	while (true)
	{
		printBoard();

		getLegalMoves(turn);
		if ((turn == white && !isWhiteAI) || (turn == black && !isBlackAI))
			printMoves();

		size = jMoves.size() == 0 ? vMoves.size() : jMoves.size();
		if (vMoves.size() > 0 || jMoves.size() > 0)
		{
			if ((turn == white && !isWhiteAI) || (turn == black && !isBlackAI))
			{
				std::cout << "What move would you like to play?" << std::endl;
				std::cin >> choice;
				while (1)
				{
					if (std::cin.fail() || choice < 0 || choice > size)
					{
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "You have entered wrong input. Try again" << std::endl;
						std::cin >> choice;
					}
					if (!std::cin.fail())
						break;
				}
				makeMove(choice);
			}
			else
			{
				choice = iterative_deepening(timeLimit, turn);
				if (jMoves.size() > 0 || vMoves.size() > 0)
				{
					std::cout << "\nPlayer " << turn << " played move: ";
					if (jMoves.size() > 0)
					{
						for (int i = 0; i < jMoves[choice - 1]->size(); i++)
						{
							std::cout << jMoves[choice - 1]->at(i);
						}
					}
					else
					{
						for (Coordinate c : vMoves[choice - 1])
						{
							std::cout << c;
						}
					}
					std::cout << "\n";
				}
				makeMove(choice);
			}

			if (turn == 1)
				turn = 2;
			else
				turn = 1;
		}

		else {
			break;
		}
	}
	std::cout << "\nNo moves available. Game Over" << std::endl;
}