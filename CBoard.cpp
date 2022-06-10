#include "CBoard.h"

CBoard::CBoard(int _size, MAZE_TYPE _mazeType)
	: m_size(_size)
	, m_mazeType(_mazeType)
{
	if (m_size % 2 == 0)
		++m_size;

	init();
}

CBoard::~CBoard()
{
}

void CBoard::setColor(WORD _text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _text);
}

void CBoard::setMaze(MAZE_TYPE _mazeType)
{
	m_tiles.clear();
	m_mazeType = _mazeType;

	switch (m_mazeType) {
	case MAZE_TYPE::DEFAULT: GenerateByDefault(); break;
	case MAZE_TYPE::BINARY_TREE: GenerateByBinaryTree(); break;
	case MAZE_TYPE::SIDE_WINDER: GenerateBySideWinder(); break;
	}
}

void CBoard::GenerateByDefault()
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

void CBoard::GenerateByBinaryTree()
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

void CBoard::GenerateBySideWinder()
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
				m_tiles[x][y + 1] = TILE_TYPE::EMPTY;
				++count;
			}
			else
			{
				uniform_int_distribution<int> distribution(0, count);
				auto countGenerator = std::bind(distribution, engine);
				int rand = countGenerator();
				m_tiles[x + 1][y - rand * 2] = TILE_TYPE::EMPTY;
				count = 1;
			}
		}
	}
}

void CBoard::init()
{
	switch (m_mazeType)
	{
	case MAZE_TYPE::DEFAULT: GenerateByDefault(); break;
	case MAZE_TYPE::BINARY_TREE: GenerateByBinaryTree(); break;
	case MAZE_TYPE::SIDE_WINDER: GenerateBySideWinder(); break;
	}
}

void CBoard::render()
{
	// Mazes for Programmers
	std::locale::global(std::locale("kor"));
	const wchar_t CIRCLE = L'■';
	for (int x = 0; x < m_size; ++x)
	{
		for (int y = 0; y < m_size; ++y)
		{
			if (m_tiles[x][y] == TILE_TYPE::WALL)
			{
				setColor(12);
				std::wcout << CIRCLE;
			}
			else
			{
				setColor(10);
				std::wcout << CIRCLE;
			}
		}
		setColor(15);
		std::wcout << '\n';
	}
}

void CBoard::reset()
{
	m_tiles.clear();
	switch (m_mazeType)
	{
	case MAZE_TYPE::DEFAULT: GenerateByDefault(); break;
	case MAZE_TYPE::BINARY_TREE: GenerateByBinaryTree(); break;
	case MAZE_TYPE::SIDE_WINDER: GenerateBySideWinder(); break;
	}
}