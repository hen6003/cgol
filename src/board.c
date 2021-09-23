#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>

#include "board.h"

board *alloc_board(size_t width, size_t height, board *local_board)
{
	local_board->cells = malloc(width * sizeof((bool *)0));

	for (size_t i = 0; i < width; i++)
		local_board->cells[i] = malloc(height * sizeof(bool));

	local_board->width = width;
	local_board->height = height;

	for (size_t x = 0; x < width; x++)
		for (size_t y = 0; y < height; y++)
			local_board->cells[x][y] = false; 
					

	TraceLog(LOG_INFO, "CGOL: Board initialized");
	TraceLog(LOG_INFO, "    > Size: %zu x %zu", width, height);
		
	return local_board;
}

void free_board(board *local_board)
{
	for (size_t i = 0; i < local_board->width; i++)
		free(local_board->cells[i]);

	free(local_board->cells);
}

bool copy_board(board src_board, board dest_board)
{
	for (size_t x = 0; x < src_board.width; x++)
		for (size_t y = 0; y < src_board.height; y++)
		{
			if (x >= dest_board.width ||
					y >= dest_board.height)
			{
				TraceLog(LOG_ERROR, "CGOL: Failed to copy board");
				return false;
			}

			dest_board.cells[x][y] = src_board.cells[x][y];
		}

	TraceLog(LOG_INFO, "CGOL: Copied board");
	return true;
}
