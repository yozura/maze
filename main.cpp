#include "define.h"
#include "CBoard.h"
#include "CPlayer.h"
#include "CGraph.h"
#include "CPriorityQueue.h"

void set_cursor(bool visible);
int maze();

int main()
{
	int** matrix = new int* [6];
	matrix[0] = new int[6]{ 0, 1, 0, 1, 0, 0 };
	matrix[1] = new int[6]{ 1, 0, 1, 1, 0, 0 };
	matrix[2] = new int[6]{ 0, 1, 0, 0, 0, 0 };
	matrix[3] = new int[6]{ 1, 1, 0, 0, 1, 0 };
	matrix[4] = new int[6]{ 0, 0, 0, 1, 0, 1 };
	matrix[5] = new int[6]{ 0, 0, 0, 0, 1, 0 };

	Graph<int> graph(6, 6);
	graph.SetMatrix(matrix);

	maze();
	return 0;
}

int maze()
{
	// System
	set_cursor(false);
	COORD pos = { 0, 0 };

	// Player
	Pos playerPos(1, 1);
	Player player(playerPos);

	// Board
	Board board(25, MAZE_TYPE::SIDE_WINDER);

	// Player Setting
	player.SetBoard(&board);
	player.SetSize(board.GetSize());
	player.ChangePathFinder(PATH_FINDER::ASTAR);

	// Board Setting
	board.SetPlayer(&player);

	const ULONGLONG WAIT_TICK = 1000 / 60;
	ULONGLONG lastTick = 0;
	while (true)
	{
#pragma region Frame
		ULONGLONG curTick = GetTickCount64();
		if (curTick - lastTick < WAIT_TICK)
			continue;
		int deltaTick = curTick - lastTick;
		lastTick = curTick;
#pragma endregion

		// Update
		player.Update(deltaTick);

		// Draw
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		board.Render();
	}
	return 0;
}

void set_cursor(bool visible) {
	CONSOLE_CURSOR_INFO info = { 1, visible };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}