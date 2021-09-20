#include <raylib.h>
#include <stdio.h>
#include <malloc.h>

#include "files.h"

board read_plaintext(const char *file)
{
	FILE *fp;
	char ch;
	bool comment = false;
	board local_board = {0,0,0,0};

	fp = fopen(file, "r"); // read mode
	
	if (fp == NULL)
		return local_board;

	alloc_board(MAX_BOARD_SIZE, MAX_BOARD_SIZE, &local_board);
	local_board.x = 0;
	local_board.y = 0;

	size_t x=0,y=0;
	while ((ch = fgetc(fp)) != EOF)
	{
		if (!comment)
		{
			if (ch == '!')
				comment = true;
			else if (ch == '.')
				x++;
			else if (ch == 'O')
			{
				local_board.cells[x][y] = true;
				x++;
			}
			else if (ch == '\n')
			{
				x=0;
				y++;
			}

			if (x > MAX_BOARD_SIZE ||
					y > MAX_BOARD_SIZE)
			{
				TraceLog(LOG_ERROR, "Pattern exceded max size");
				break;
			}
		}
		else if (ch == '\n')
			comment = false;
	}

	fclose(fp);

	return local_board;
}
