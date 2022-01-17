/*
Joongyeon Cho
ECE469: Proj1
game.h
*/

#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

namespace Game {
	static char columnOutput[8] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

	struct Coordinate
	{
		int x;
		int y;
		Coordinate() {}
		Coordinate(int y, int x) : x(x), y(y) {}
		friend std::ostream& operator<<(std::ostream& os, const Coordinate& c)
		{
			os << "(" << columnOutput[c.x] << " , " << c.y << ") ";
			return os;
		}
	};
}

#endif