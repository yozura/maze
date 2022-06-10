#pragma once
#include "define.h"

enum class TILE_TYPE
{
	EMPTY,
	WALL,
};

enum class MAZE_TYPE
{
	DEFAULT,
	BINARY_TREE,
	SIDE_WINDER,

};

class CBoard
{
private:
	vector<vector<TILE_TYPE>> m_tiles;		// 미로
	int m_size;								// 미로 사이즈 size * size
	MAZE_TYPE m_mazeType;					// 미로 알고리즘 타입.
public:
	CBoard(int _size, MAZE_TYPE _mazeType = MAZE_TYPE::DEFAULT);
	~CBoard();
private:
	void init();
	void GenerateByDefault();
	void GenerateByBinaryTree();
	void GenerateBySideWinder();
public:
	void setMaze(MAZE_TYPE _mazeType);
	void setColor(WORD _text);
	void render();
	void reset();
};