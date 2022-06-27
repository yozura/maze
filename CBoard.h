#pragma once
#include "define.h"

enum class MAZE_TYPE
{
	DEFAULT,
	BINARY_TREE,
	SIDE_WINDER,

};

class Player;
class Board
{
private:
	vector<vector<TILE_TYPE>> m_tiles;		// 미로
	int m_size;								// 미로 사이즈 size * size
	MAZE_TYPE m_mazeType;					// 미로 알고리즘 타입.
	const wchar_t RECT;						// 미로 그래픽

	Pos* m_destPos;							// 목적지 포지션

	Player* m_player;						// 플레이어
public:
	Board(int _size, MAZE_TYPE _mazeType = MAZE_TYPE::DEFAULT);
	~Board();
private:
	void Init();
	void GenerateByDefault();
	void GenerateByBinaryTree();
	void GenerateBySideWinder();
public:
	vector<vector<TILE_TYPE>> GetTiles() { return m_tiles; }
	Pos& GetDestPos() const { return *m_destPos; }
	int GetSize() { return m_size; }
	
	void SetPlayer(Player* _player) { m_player = _player; }
public:
	void ChangeMaze(MAZE_TYPE _mazeType);
	void SetColor(WORD _text);
	void Render();
	void Reset();
};