#include <stddef.h>
#include <stdbool.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"

board main_board; 

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
	ret += live_cell(x-1, y);	 // 2
	ret += live_cell(x-1, y+1); // 3
	ret += live_cell(x,	 y+1); // 4
	ret += live_cell(x+1, y+1); // 5
	ret += live_cell(x+1, y);	 // 6
	ret += live_cell(x+1, y-1); // 7
	ret += live_cell(x,	 y-1); // 8

	return ret;
}

void start()
{
	int frameCounter = 0;

	// Init window
	InitWindow(500, 500, "Conway's game of life");
	SetTargetFPS(60);
	
	while(!WindowShouldClose())
	{
		/*** Update main_board ***/
		if ((frameCounter % 10) == 0)
		//if (IsKeyDown(KEY_SPACE) && ((frameCounter % 10) == 0))
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
			
		/*** Drawing ***/
		int size = 500 / MAX_BOARD_SIZE;
		
		BeginDrawing();
		{
			ClearBackground(WHITE);

			// Draw main_board
			for (int x = 0; x < MAX_BOARD_SIZE; x++)
				for (int y = 0; y < MAX_BOARD_SIZE; y++)
				{
					Rectangle tile = {x*size, y*size, size, size};

					if (main_board.cells[x][y])
						DrawRectangleRec(tile, RED);

					DrawRectangleLinesEx(tile, 0.5, GRAY);
				}
		}
		EndDrawing();

		frameCounter++;
	}

	CloseWindow();
}

int main(int argc, char **argv)
{		
	// Init main_board
	alloc_board(MAX_BOARD_SIZE, MAX_BOARD_SIZE, &main_board);
	TraceLog(LOG_INFO, "CGOL: Main board initialized successfully");

	if (argc < 2)
	{
default_board:
		main_board.cells[0][1] = true;
		main_board.cells[1][2] = true;
		main_board.cells[2][0] = true;
		main_board.cells[2][1] = true;
		main_board.cells[2][2] = true;
	}
	else
	{
		board loaded_pattern;
		const char *file_extension = GetFileExtension(argv[1]);

		if (!strcmp(file_extension, ".cells"))
			loaded_pattern = read_plaintext(argv[1]);
		else if (!strcmp(file_extension, ".rle"))
			loaded_pattern = read_rle(argv[1]);
		else
		{
			TraceLog(LOG_ERROR, "CGOL: Filetype unsupported");
			goto default_board;
		}
		
		if (loaded_pattern.width == 0 &&
				loaded_pattern.height == 0)
		{
			TraceLog(LOG_ERROR, "CGOL: Failed to load pattern");
			goto default_board;
		}
		else
		{
			TraceLog(LOG_INFO, "CGOL: Pattern loaded successfully");

			if (!copy_board(loaded_pattern, main_board))
				goto default_board;
			
			free_board(&loaded_pattern);
			TraceLog(LOG_INFO, "CGOL: Pattern unloaded successfully");
		}
	}

	start();
	free_board(&main_board);
	TraceLog(LOG_INFO, "CGOL: Board unloaded successfully");
}
