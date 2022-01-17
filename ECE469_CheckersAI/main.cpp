#include <iostream>
#include <climits>
#include <limits>
#include <string>
#include "game.h"
#include "checkers.h"

using namespace std;

int main()
{
	Game::checkers Checkers = Game::checkers();
	int turn = 1, isLoad, gameType, timeLimit = 20;

	cout << "1. Start a new game" << endl;
	cout << "2. Load existing game" << endl;
	cin >> isLoad;
	while (1)
	{
		if (cin.fail() || isLoad < 1 || isLoad > 2)
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "You have entered wrong input. Try again" << std::endl;
			cin >> isLoad;
		}
		if (!cin.fail())
			break;
	}
	if (isLoad == 2)
	{
		// load game
		string fileName;
		cout << "Please enter the name of the loadFile" << endl;
		cin >> fileName;
		Checkers.loadGame(fileName);
		turn = Checkers.loadTurn;
		timeLimit = Checkers.load_timeLimit;

		std::cout << "turn is " << turn << " and timelimit is " << timeLimit;
	}
	else
	{
		Checkers.initNewGame();
	}

	cout << "Choose one of the options below" << endl;
	cout << "[1]: Player vs Player\n[2]: Player vs AI\n[3]: AI vs AI" << endl;
	cin >> gameType;
	while (1)
	{
		if (cin.fail() || gameType < 1 || gameType > 4)
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "You have entered wrong input. Try again" << std::endl;
			cin >> gameType;
		}
		if (!cin.fail())
			break;
	}
	if (isLoad != 2 && gameType != 1)
	{
		cout << "Choose the time limit for the AI" << endl;
		cin >> timeLimit;
	}

	if (gameType == 2)
	{
		char tmp;
		cout << "Would you like to play white? (y/n)" << endl;
		cin >> tmp;
		if (tmp == 'n')
			gameType = 4;
		while (1)
		{
			if (cin.fail() || (tmp != 'y' && tmp != 'n'))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				std::cout << "You have entered wrong input. Try again" << std::endl;
				cin >> tmp;
				if (tmp == 'n')
					gameType = 4;
			}
			if (!cin.fail())
				break;
		}
	}
	switch (gameType)
	{
	case 1:
		Checkers.startGame(turn, 0, false, false);
		break;
	case 2:
		Checkers.startGame(turn, timeLimit, false, true);
		break;
	case 3:
		Checkers.startGame(turn, timeLimit, true, true);
		break;
	case 4:
		Checkers.startGame(turn, timeLimit, true, false);
		break;
	}

	return 0;
}