#include <raylib.h>

#include "board.h"
#include "render.h"

void render_board()
{
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
}
