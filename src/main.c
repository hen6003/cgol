#include <stddef.h>
#include <stdbool.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "cgol.h"
#include "render.h"

board main_board; 

void start()
{
	int frame_counter = 0;

	// Init window
	InitWindow(500, 500, "Conway's game of life");
	SetTargetFPS(60);
	
	while(!WindowShouldClose())
	{
		/*** Update main_board ***/
		if ((frame_counter % 10) == 0)
			cgol_update();
			
		/*** Drawing ***/
		render_board();

		/* Count frames for timing */
		frame_counter++;
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
