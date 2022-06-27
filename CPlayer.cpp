#include "CPlayer.h"
#include "CBoard.h"
#include "CPriorityQueue.h"

Player::Player(Pos _pos, PATH_FINDER _pathFinder)
	: m_playerPos(_pos)
	, m_dir(0)
	, m_pathFinder(_pathFinder)
	, m_accTick(0)
	, m_moveTick(30)
	, m_board(nullptr)
	, m_size(0)
	, m_goal(false)
{
}

Player::~Player()
{
}

void Player::ChangePathFinder(PATH_FINDER _pathFinder)
{
	m_path.clear();
	m_pathFinder = _pathFinder;
	switch (m_pathFinder)
	{
	case PATH_FINDER::RANDOM: PathFindByRandom(); break;
	case PATH_FINDER::RIGHT_HAND: PathFindByRightHand(); break;
	case PATH_FINDER::BFS: PathFindByBFS(); break;
	case PATH_FINDER::DIJKSTRA: PathFindByDijkstra(); break;
	case PATH_FINDER::ASTAR: PathFindByAStar(); break;
	}
}

void Player::Update(int deltaTick)
{
	m_accTick += deltaTick;
	if (m_accTick < m_moveTick)
		return;
	
	m_accTick = 0;

	if (m_path.size() > 0 && !m_goal)
	{
		m_playerPos = m_path.back();
		m_path.pop_back();
	}

	if (m_path.size() <= 0)
	{
		m_goal = true;
		m_board->Reset();
		Reset();
	}
}

void Player::Reset()
{
	m_goal = false;
	m_path.clear();
	m_playerPos.x = 1;
	m_playerPos.y = 1;
	switch (m_pathFinder)
	{
	case PATH_FINDER::RANDOM: PathFindByRandom(); break;
	case PATH_FINDER::RIGHT_HAND: PathFindByRightHand(); break;
	case PATH_FINDER::BFS: PathFindByBFS(); break;
	case PATH_FINDER::DIJKSTRA: PathFindByDijkstra(); break;
	case PATH_FINDER::ASTAR: PathFindByAStar(); break;
	}
}

void Player::PathFindByRandom() 
{
	mt19937 engine((unsigned int)time(NULL));
	uniform_int_distribution<int> distribution(0, 4);
	auto generator = std::bind(distribution, engine);
	int rand = generator();

	switch (rand)
	{
	case (int)DIRECTION::UP:
	{
		// ������ ������� ���...
		if (m_playerPos.x - 1 >= 0
			&& m_board->GetTiles()[m_playerPos.x - 1][m_playerPos.y] == TILE_TYPE::EMPTY)
		{
			// �������� �̵�
			m_playerPos.x -= 1;
		}
		break;
	}
	case (int)DIRECTION::DOWN:
	{
		// �Ʒ����� ������� ���...
		if (m_playerPos.x + 1 < m_size
			&& m_board->GetTiles()[m_playerPos.x + 1][m_playerPos.y] == TILE_TYPE::EMPTY)
		{
			// �Ʒ��� �̵�
			m_playerPos.x += 1;
		}
		break;
	}
	case (int)DIRECTION::LEFT:
	{
		// ������ ������� ���...
		if (m_playerPos.y - 1 >= 0
			&& m_board->GetTiles()[m_playerPos.x][m_playerPos.y - 1] == TILE_TYPE::EMPTY)
		{
			// �������� �̵�
			m_playerPos.y -= 1;
		}
		break;
	}
	case (int)DIRECTION::RIGHT:
	{
		// �������� ������� ���...
		if (m_playerPos.y + 1 < m_size
			&& m_board->GetTiles()[m_playerPos.x][m_playerPos.y + 1] == TILE_TYPE::EMPTY)
		{
			// ���������� �̵�
			m_playerPos.y += 1;
		}
		break;
	}
	}
}

void Player::PathFindByRightHand()
{
	const int frontX[4] = { -1, 0, 1, 0 };
	const int frontY[4] = { 0, -1, 0, 1 };
	const int rightX[4] = { 0, -1, 0, 1 };
	const int rightY[4] = { 1, 0, -1, 0 };

	while (m_playerPos.x != m_board->GetDestPos().x || m_playerPos.y != m_board->GetDestPos().y)
	{
		// ���� �ٶ󺸴� ������ �������� ����ִٸ�. 
		if (m_board->GetTiles()[m_playerPos.x + rightX[m_dir]][m_playerPos.y + rightY[m_dir]] == TILE_TYPE::EMPTY)
		{
			// ���������� 90�� ȸ��
			m_dir = (m_dir - 1 + 4) % 4;

			// ����
			m_playerPos.x = m_playerPos.x + frontX[m_dir];
			m_playerPos.y = m_playerPos.y + frontY[m_dir];

			// ����
			m_path.push_back(m_playerPos);
		}
		// ���� ����ִٸ� ����
		else if (m_board->GetTiles()[m_playerPos.x + frontX[m_dir]][m_playerPos.y + frontY[m_dir]] == TILE_TYPE::EMPTY)
		{
			// �Ѻ� ����
			m_playerPos.x = m_playerPos.x + frontX[m_dir];
			m_playerPos.y = m_playerPos.y + frontY[m_dir];

			// ����
			m_path.push_back(m_playerPos);
		}
		else
		{
			// �������� 90�� ȸ��
			m_dir = (m_dir + 1 + 4) % 4;
		}
	}
}

void Player::PathFindByBFS()
{
	const int deltaX[] = { -1, 0, 1, 0 };
	const int deltaY[] = { 0, -1, 0, 1 };

	bool** found = new bool* [m_size];
	for (int i = 0; i < m_size; ++i)
	{
		found[i] = new bool[m_size];
		std::fill_n(found[i], m_size, false);
	}

	Pos** parent = new Pos* [m_size];
	for (int i = 0; i < m_size; ++i)
		parent[i] = new Pos[m_size];

	queue<Pos> q;
	q.push(m_playerPos);
	found[m_playerPos.x][m_playerPos.y] = true;
	parent[m_playerPos.x][m_playerPos.y] = m_playerPos;
	
	while (q.size() > 0)
	{
		Pos pos = q.front();
		q.pop();

		for (int i = 0; i < 4; ++i)
		{
			int nextX = pos.x + deltaX[i];
			int nextY = pos.y + deltaY[i];

			if (nextX < 0 || nextX >= m_size || nextY < 0 || nextY >= m_size)
				continue;

			if (m_board->GetTiles()[nextX][nextY] == TILE_TYPE::WALL)
				continue;

			if (found[nextX][nextY] == true)
				continue;

			q.push(Pos(nextX, nextY));
			found[nextX][nextY] = true;
			parent[nextX][nextY] = pos;
		}
	}

	CalculatePathFromParent(parent);

	for (int i = 0; i < m_size; ++i)
	{
		delete[] found[i];
		delete[] parent[i];
	}
	delete[] found;
	delete[] parent;
}

void Player::PathFindByDijkstra()
{

}

void Player::PathFindByAStar()
{
	// ���� �ű��
	// F = G + H;
	// F = ���� ���� ( ���� ���� ����, ��ο� ���� �޶��� )
	// G = ���������� �ش� ��ǥ���� �̵��ϴµ� ��� ��� (���� ���� ����, ��ο� ���� �޶���)
	// H = ���������� �󸶳� ������� (���� ���� ����, ����)

	// Up, Left, Down, Right, UpLeft, DownLeft, DownRight, UpRight
	const int deltaX[] = { -1, 0, 1, 0 };
	const int deltaY[] = { 0, -1, 0, 1 };
	const int cost[] = { 10, 10, 10, 10 };

	// x, y �̹� �湮�ߴ��� ���� ( closed)
	bool** closed = new bool* [m_size];
	for (int i = 0; i < m_size; ++i)
	{
		closed[i] = new bool[m_size];
		std::fill_n(closed[i], m_size, false);
	}

	// x, y ���� ���� �ѹ��̶� �߰� �ߴ���
	int** open = new int* [m_size];
	for (int i = 0; i < m_size; ++i)
	{
		open[i] = new int[m_size];
		std::fill_n(open[i], m_size, INT32_MAX);
	}

	Pos** parent = new Pos* [m_size];
	for (int i = 0; i < m_size; ++i)
		parent[i] = new Pos[m_size];


	// �켱���� ť ����
	priority_queue<PQNode> pq;
	
	// ������ �߰�
	int start = 10 * abs(m_board->GetDestPos().x - m_playerPos.x) + abs(m_board->GetDestPos().y - m_playerPos.y);
	open[m_playerPos.x][m_playerPos.y] = start;
	pq.push(PQNode(start, 0, m_playerPos.x , m_playerPos.y));
	parent[m_playerPos.x][m_playerPos.y] = m_playerPos;

	while (pq.size() > 0)
	{
		// ���� ���� �ĺ��� ã�´�
		PQNode node = pq.top();
		pq.pop();

		// ������ ��ǥ�� ���� ��η� ã�Ƽ�, �� ���� ��η� ���ؼ� �̹� �湮�� ��� ��ŵ
		if (closed[node.y][node.x] == true)
			continue;

		// �湮�Ѵ�
		closed[node.y][node.x] = true;
		
		// ������ ���� �� ����
		if (node.x == m_board->GetDestPos().x && node.y == m_board->GetDestPos().y)
			break;

		// �����¿� �� �̵��� �� �ִ� ��ǥ���� Ȯ���ؼ� �����Ѵ�.
		for (int i = 0; i < 4; ++i) 
		{
			int nextX = node.x + deltaX[i];
			int nextY = node.y + deltaY[i];

			// ��ȿ���� ����� ��ŵ
			if (nextX < 0 || nextX >= m_size || nextY < 0 || nextY >= m_size)
				continue;

			// ���̶�� ��ŵ
			if (m_board->GetTiles()[nextX][nextY] == TILE_TYPE::WALL)
				continue;

			// �̹� �湮�� ���̸� ��ŵ
			if (closed[nextX][nextY] == true)
				continue;

			// ��� ���
			int g = node.g + cost[i];
			int h = 10 * abs(m_board->GetDestPos().x - nextX) + abs(m_board->GetDestPos().y - nextY);

			// �ٸ� ��ο��� �̹� ���� ���� ã�Ҵٸ� ��ŵ
			if (open[nextX][nextY] < g + h)
				continue;

			// ���� ����
			open[nextX][nextY] = g + h;
			pq.push(PQNode(g + h, g, nextX, nextY));
			parent[nextX][nextY] = Pos(node.x, node.y);
		}
	}

	CalculatePathFromParent(parent);

	// ����
	for (int i = 0; i < m_size; ++i)
	{
		delete[] parent[i];
		delete[] closed[i];
		delete[] open[i];
	}
	delete[] parent;
	delete[] closed;
	delete[] open;
}

void Player::CalculatePathFromParent(Pos** parent)
{
	int x = m_board->GetDestPos().x;
	int y = m_board->GetDestPos().y;
	while (parent[x][y].y != y || parent[x][y].x != x)
	{
		m_path.push_back(Pos(x, y));
		Pos pos = parent[x][y];
		x = pos.x;
		y = pos.y;
	}
	m_path.push_back(Pos(x, y));
}
