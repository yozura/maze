#pragma once

#include <Windows.h>
#include <iostream>
#include <random>
#include <functional>
#include <ctime>
using std::mt19937;
using std::mt19937_64;
using std::uniform_int_distribution;

#include <vector>
using std::vector;

#include <stack>
using std::stack;

#include <queue>
using std::queue;
using std::priority_queue;

struct Pos
{
	int x;
	int y;

	Pos()
		: x (0)
		, y (0)
	{}

	Pos(int _x, int _y)
		: x(_x)
		, y(_y)
	{}
};

enum class TILE_TYPE
{
	EMPTY,
	WALL,
};

enum class DIRECTION
{
	UP = 0,
	LEFT,
	DOWN,
	RIGHT
};

struct PQNode
{
	int f;
	int g;
	int x;
	int y;

	PQNode(int _f, int _g, int _x, int _y)
		: f(_f)
		, g(_g)
		, x(_x)
		, y(_y)
	{}

	bool operator<(const PQNode& other) const
	{
		return f < other.f;
	}
};