#include <stddef.h>

#include "cgol.h"
#include "board.h"

bool live_cell(size_t x, size_t y)
{
	if (x >= MAX_BOARD_SIZE ||
			y >= MAX_BOARD_SIZE)
		return false;

	return main_board.cells[x][y];
}

int live_neighbours(size_t x, size_t y)
{
	int ret = 0;

	/*
	 * 1 8 7
	 * 2 0 6
	 * 3 4 5
	 */

	ret += live_cell(x-1, y-1); // 1
	ret += live_cell(x-1, y);	  // 2
	ret += live_cell(x-1, y+1); // 3
	ret += live_cell(x,	 y+1);  // 4
	ret += live_cell(x+1, y+1); // 5
	ret += live_cell(x+1, y);	  // 6
	ret += live_cell(x+1, y-1); // 7
	ret += live_cell(x,	 y-1);  // 8

	return ret;
}

void cgol_update()
{
	bool next_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

	for (size_t x = 0; x < MAX_BOARD_SIZE; x++)
		for (size_t y = 0; y < MAX_BOARD_SIZE; y++)
		{
			// Rules
			int ret = live_neighbours(x,y);

			if (ret == 3)
				next_board[x][y] = true;
			else if (ret == 2)
				next_board[x][y] = main_board.cells[x][y];
			else
				next_board[x][y] = false;
		}

	for (size_t x = 0; x < MAX_BOARD_SIZE; x++)
		for (size_t y = 0; y < MAX_BOARD_SIZE; y++)
			main_board.cells[x][y] = next_board[x][y];
}

