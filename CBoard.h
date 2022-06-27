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
	vector<vector<TILE_TYPE>> m_tiles;		// �̷�
	int m_size;								// �̷� ������ size * size
	MAZE_TYPE m_mazeType;					// �̷� �˰��� Ÿ��.
	const wchar_t RECT;						// �̷� �׷���

	Pos* m_destPos;							// ������ ������

	Player* m_player;						// �÷��̾�
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