#include "define.h"
#include "CBoard.h"

void set_cursor(bool visible);

int main()
{
	CBoard board(25, MAZE_TYPE::SIDE_WINDER);
	const int WAIT_TICK = 1000 / 30;
	COORD pos = { 0, 0 };
	set_cursor(false);
	int lastTick = 0;	
	
	mt19937 engine((unsigned int)time(NULL));				// 메르센 트위스터 난수 엔진
	uniform_int_distribution<int> distribution(0, 1);		// 생성 범위
	auto generator = std::bind(distribution, engine);
	while (true) 
	{
		#pragma region Frame
		int curTick = GetTickCount64();
		if (curTick - lastTick < WAIT_TICK)
			continue;
		lastTick = curTick;
		#pragma endregion

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		board.render();
		board.reset();
	}
	return 0;
}

void set_cursor(bool visible) {
	CONSOLE_CURSOR_INFO info = { 1, visible };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}