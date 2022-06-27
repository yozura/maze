#include "CBoard.h"
#include "CPlayer.h"	

Board::Board(int _size, MAZE_TYPE _mazeType)
	: m_size(_size)
	, m_mazeType(_mazeType)
	, RECT(L'■')
	, m_player(nullptr)
{
	if (m_size % 2 == 0)
		++m_size;

	m_destPos = new Pos(m_size - 2, m_size - 2);
	Init();
}

Board::~Board()
{
	delete m_destPos;
}

void Board::SetColor(WORD _text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _text);
}

void Board::ChangeMaze(MAZE_TYPE _mazeType)
{
	m_tiles.clear();
	m_mazeType = _mazeType;

	switch (m_mazeType) {
	case MAZE_TYPE::DEFAULT: GenerateByDefault(); break;
	case MAZE_TYPE::BINARY_TREE: GenerateByBinaryTree(); break;
	case MAZE_TYPE::SIDE_WINDER: GenerateBySideWinder(); break;
	}
}

void Board::GenerateByDefault()
{
	for (int x = 0; x < m_size; ++x)
	{
		vector<TILE_TYPE> tempVector;
		for (int y = 0; y < m_size; ++y)
		{
			if (x == 0 || x == m_size - 1 || y == 0 || y == m_size - 1)
				tempVector.push_back(TILE_TYPE::WALL);
			else
				tempVector.push_back(TILE_TYPE::EMPTY);
		}
		m_tiles.push_back(tempVector);
	}
}

void Board::GenerateByBinaryTree()
{
	// 1. 길을 막는 작업
	for (int x = 0; x < m_size; ++x)
	{
		vector<TILE_TYPE> tempVector;
		for (int y = 0; y < m_size; ++y)
		{
			if (x % 2 == 0 || y % 2 == 0)
				tempVector.push_back(TILE_TYPE::WALL);
			else
				tempVector.push_back(TILE_TYPE::EMPTY);
		}
		m_tiles.push_back(tempVector);
	}

	// 2. 길 뚫는 작업
	mt19937 engine((unsigned int)time(NULL));				// 메르센 트위스터 난수 엔진
	uniform_int_distribution<int> distribution(0, 1);		// 생성 범위
	auto generator = std::bind(distribution, engine);

	for (int x = 0; x < m_size; ++x)
	{
		for (int y = 0; y < m_size; ++y)
		{
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			if (x == m_size - 2 && y == m_size - 2)
				continue;

			if (x == m_size - 2)
			{
				m_tiles[x][y + 1] = TILE_TYPE::EMPTY;
				continue;
			}

			if (y == m_size - 2)
			{
				m_tiles[x + 1][y] = TILE_TYPE::EMPTY;
				continue;
			}

			if (generator() == 0)
				m_tiles[x + 1][y] = TILE_TYPE::EMPTY;
			else
				m_tiles[x][y + 1] = TILE_TYPE::EMPTY;
		}
	}
}

void Board::GenerateBySideWinder()
{
	// 알고리즘 개요
	// 1. (0, 0) 에서 시작하는 셀부터 행 방향으로 진행한다.
	// 2. 현재 셀을 실행 세트에 추가한다.
	// 3. 현재 셀의 경우 동쪽으로 갈건지 아닌지


	// 1. 길을 막는 작업
	for (int x = 0; x < m_size; ++x)
	{
		vector<TILE_TYPE> tempVector;
		for (int y = 0; y < m_size; ++y)
		{
			if (x % 2 == 0 || y % 2 == 0)
				tempVector.push_back(TILE_TYPE::WALL);
			else
				tempVector.push_back(TILE_TYPE::EMPTY);
		}
		m_tiles.push_back(tempVector);
	}

	// 2. 길 뚫는 작업
	mt19937 engine((unsigned int)time(NULL));				// 메르센 트위스터 난수 엔진
	uniform_int_distribution<int> distribution(0, 1);		// 생성 범위
	auto generator = std::bind(distribution, engine);

	for (int x = 0; x < m_size; ++x)
	{
		int count = 1;
		for (int y = 0; y < m_size; ++y)
		{
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			if (x == m_size - 2 && y == m_size - 2)
				continue;

			if (x == m_size - 2)
			{
				m_tiles[x][y + 1] = TILE_TYPE::EMPTY;
				continue;
			}

			if (y == m_size - 2)
			{
				m_tiles[x + 1][y] = TILE_TYPE::EMPTY;
				continue;
			}

			if (generator() == 0)
			{
				uniform_int_distribution<int> distribution(0, count - 1);
				auto countGenerator = std::bind(distribution, engine);
				int rand = countGenerator();
				m_tiles[x + 1][y - rand * 2] = TILE_TYPE::EMPTY;
				count = 1;
			}
			else
			{
				m_tiles[x][y + 1] = TILE_TYPE::EMPTY;
				++count;
			}
		}
	}
}

void Board::Init()
{
	switch (m_mazeType)
	{
	case MAZE_TYPE::DEFAULT: GenerateByDefault(); break;
	case MAZE_TYPE::BINARY_TREE: GenerateByBinaryTree(); break;
	case MAZE_TYPE::SIDE_WINDER: GenerateBySideWinder(); break;
	}
}

void Board::Render()
{
	// Mazes for Programmers
	std::locale::global(std::locale("kor"));
	for (int x = 0; x < m_size; ++x)
	{
		for (int y = 0; y < m_size; ++y)
		{
			if (m_player->GetPlayerPos().x == x && m_player->GetPlayerPos().y == y) SetColor(11);
			else if (x == (*m_destPos).x && y == (*m_destPos).y) SetColor(14);
			else
			{
				if (m_tiles[x][y] == TILE_TYPE::WALL) SetColor(16);
				else SetColor(17);
			}
			std::wcout << RECT;
		}
		SetColor(15);
		std::wcout << '\n';
	}
}

void Board::Reset()
{
	m_tiles.clear();
	switch (m_mazeType)
	{
	case MAZE_TYPE::DEFAULT: GenerateByDefault(); break;
	case MAZE_TYPE::BINARY_TREE: GenerateByBinaryTree(); break;
	case MAZE_TYPE::SIDE_WINDER: GenerateBySideWinder(); break;
	}
}