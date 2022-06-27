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
		// 위쪽이 비어있을 경우...
		if (m_playerPos.x - 1 >= 0
			&& m_board->GetTiles()[m_playerPos.x - 1][m_playerPos.y] == TILE_TYPE::EMPTY)
		{
			// 위쪽으로 이동
			m_playerPos.x -= 1;
		}
		break;
	}
	case (int)DIRECTION::DOWN:
	{
		// 아래쪽이 비어있을 경우...
		if (m_playerPos.x + 1 < m_size
			&& m_board->GetTiles()[m_playerPos.x + 1][m_playerPos.y] == TILE_TYPE::EMPTY)
		{
			// 아래로 이동
			m_playerPos.x += 1;
		}
		break;
	}
	case (int)DIRECTION::LEFT:
	{
		// 왼쪽이 비어있을 경우...
		if (m_playerPos.y - 1 >= 0
			&& m_board->GetTiles()[m_playerPos.x][m_playerPos.y - 1] == TILE_TYPE::EMPTY)
		{
			// 왼쪽으로 이동
			m_playerPos.y -= 1;
		}
		break;
	}
	case (int)DIRECTION::RIGHT:
	{
		// 오른쪽이 비어있을 경우...
		if (m_playerPos.y + 1 < m_size
			&& m_board->GetTiles()[m_playerPos.x][m_playerPos.y + 1] == TILE_TYPE::EMPTY)
		{
			// 오른쪽으로 이동
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
		// 현재 바라보는 방향의 오른쪽이 비어있다면. 
		if (m_board->GetTiles()[m_playerPos.x + rightX[m_dir]][m_playerPos.y + rightY[m_dir]] == TILE_TYPE::EMPTY)
		{
			// 오른쪽으로 90도 회전
			m_dir = (m_dir - 1 + 4) % 4;

			// 전진
			m_playerPos.x = m_playerPos.x + frontX[m_dir];
			m_playerPos.y = m_playerPos.y + frontY[m_dir];

			// 저장
			m_path.push_back(m_playerPos);
		}
		// 앞이 비어있다면 전진
		else if (m_board->GetTiles()[m_playerPos.x + frontX[m_dir]][m_playerPos.y + frontY[m_dir]] == TILE_TYPE::EMPTY)
		{
			// 한보 전진
			m_playerPos.x = m_playerPos.x + frontX[m_dir];
			m_playerPos.y = m_playerPos.y + frontY[m_dir];

			// 저장
			m_path.push_back(m_playerPos);
		}
		else
		{
			// 왼쪽으로 90도 회전
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
	// 점수 매기기
	// F = G + H;
	// F = 최종 점수 ( 작을 수록 좋음, 경로에 따라 달라짐 )
	// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용 (작을 수록 좋음, 경로에 따라 달라짐)
	// H = 목적지에서 얼마나 가까운지 (작을 수록 좋음, 고정)

	// Up, Left, Down, Right, UpLeft, DownLeft, DownRight, UpRight
	const int deltaX[] = { -1, 0, 1, 0 };
	const int deltaY[] = { 0, -1, 0, 1 };
	const int cost[] = { 10, 10, 10, 10 };

	// x, y 이미 방문했는지 여부 ( closed)
	bool** closed = new bool* [m_size];
	for (int i = 0; i < m_size; ++i)
	{
		closed[i] = new bool[m_size];
		std::fill_n(closed[i], m_size, false);
	}

	// x, y 가는 길을 한번이라도 발견 했는지
	int** open = new int* [m_size];
	for (int i = 0; i < m_size; ++i)
	{
		open[i] = new int[m_size];
		std::fill_n(open[i], m_size, INT32_MAX);
	}

	Pos** parent = new Pos* [m_size];
	for (int i = 0; i < m_size; ++i)
		parent[i] = new Pos[m_size];


	// 우선순위 큐 생성
	priority_queue<PQNode> pq;
	
	// 시작점 발견
	int start = 10 * abs(m_board->GetDestPos().x - m_playerPos.x) + abs(m_board->GetDestPos().y - m_playerPos.y);
	open[m_playerPos.x][m_playerPos.y] = start;
	pq.push(PQNode(start, 0, m_playerPos.x , m_playerPos.y));
	parent[m_playerPos.x][m_playerPos.y] = m_playerPos;

	while (pq.size() > 0)
	{
		// 제일 좋은 후보를 찾는다
		PQNode node = pq.top();
		pq.pop();

		// 동일한 좌표를 여러 경로로 찾아서, 더 빠른 경로로 인해서 이미 방문된 경우 스킵
		if (closed[node.y][node.x] == true)
			continue;

		// 방문한다
		closed[node.y][node.x] = true;
		
		// 목적지 도착 시 종료
		if (node.x == m_board->GetDestPos().x && node.y == m_board->GetDestPos().y)
			break;

		// 상하좌우 등 이동할 수 있는 좌표인지 확인해서 예약한다.
		for (int i = 0; i < 4; ++i) 
		{
			int nextX = node.x + deltaX[i];
			int nextY = node.y + deltaY[i];

			// 유효범위 벗어날시 스킵
			if (nextX < 0 || nextX >= m_size || nextY < 0 || nextY >= m_size)
				continue;

			// 벽이라면 스킵
			if (m_board->GetTiles()[nextX][nextY] == TILE_TYPE::WALL)
				continue;

			// 이미 방문한 곳이면 스킵
			if (closed[nextX][nextY] == true)
				continue;

			// 비용 계산
			int g = node.g + cost[i];
			int h = 10 * abs(m_board->GetDestPos().x - nextX) + abs(m_board->GetDestPos().y - nextY);

			// 다른 경로에서 이미 빠른 길을 찾았다면 스킵
			if (open[nextX][nextY] < g + h)
				continue;

			// 예약 진행
			open[nextX][nextY] = g + h;
			pq.push(PQNode(g + h, g, nextX, nextY));
			parent[nextX][nextY] = Pos(node.x, node.y);
		}
	}

	CalculatePathFromParent(parent);

	// 삭제
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
