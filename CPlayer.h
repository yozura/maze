#pragma once
#include "define.h"

enum class PATH_FINDER
{
	RANDOM,				// �����¿� ����
	RIGHT_HAND,			// �����
	BFS,				// �ʺ� ��� Ž��
	DIJKSTRA,			// ���ͽ�Ʈ�� �ִ� ���
	ASTAR,				// A* �ִ� ���
};

class Board;
class Player
{
private:
	// Player
	Pos m_playerPos;
	int m_dir;
	int m_accTick;
	int m_moveTick;
	PATH_FINDER m_pathFinder;
	vector<Pos> m_path;

	bool m_goal;

	// Board Refer
	Board* m_board;
	int m_size;

public:
	Player(Pos _pos, PATH_FINDER _pathFinder = PATH_FINDER::RANDOM);
	~Player();

public:
	Pos GetPlayerPos() { return m_playerPos; }
	bool GetGoal() { return m_goal; }

	void SetBoard(Board* _board) { m_board = _board; }
	void SetSize(int _size) { m_size = _size; }
	void ChangePathFinder(PATH_FINDER _pathFinder);
public:
	void Update(int deltaTick);
	void Reset();

private:
	void PathFindByRandom();
	void PathFindByRightHand();
	void PathFindByBFS();
	void PathFindByDijkstra();
	void PathFindByAStar();

	void CalculatePathFromParent(Pos** parent);
};
