#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

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
				TraceLog(LOG_ERROR, "FILE: Pattern exceded max size");
				break;
			}
		}
		else if (ch == '\n')
			comment = false;
	}

	fclose(fp);

	return local_board;
}

enum rle_line_states
{
	HASH_LINE,
	HEADER_LINE,
	PATTERN_LINE,
};

board read_rle(const char *file)
{
	FILE *fp;
	char ch;
	bool line_start = true;
	board local_board = {0,0,0,0};
	enum rle_line_states line_state = HASH_LINE;
	size_t pattern_width, pattern_height;
	char pattern_width_s[10] = "", pattern_height_s[10] = "", run_count[10] = "", header_tag = '\0';
	size_t x=0,y=0;

	fp = fopen(file, "r"); // read mode
	
	if (fp == NULL)
		return local_board;

	while ((ch = fgetc(fp)) != EOF)
	{
		if (line_start && line_state == HASH_LINE)
		{
			line_start = false;
			if (line_state == HASH_LINE && ch != '#')
			{
				line_state = HEADER_LINE;
				goto parse;
			}
		}
		else
		{
parse:
			if (line_state == HEADER_LINE)
			{
				if (!header_tag && ch != ' ')
					header_tag = ch;
				else if (ch <= '9' && ch >= '0')
				{
					switch (header_tag)
					{
						case 'x':
							pattern_width_s[strlen(pattern_width_s)+1] = '\0';
							pattern_width_s[strlen(pattern_width_s)]   = ch;
							break;
						
						case 'y':
							pattern_height_s[strlen(pattern_height_s)+1] = '\0';
							pattern_height_s[strlen(pattern_height_s)]   = ch;
							break;
					}
				}
				else if (ch == ',')
					header_tag = '\0';
				else if (ch == '\n')
				{
					line_state = PATTERN_LINE;

					pattern_width = atoi(pattern_width_s);
					pattern_height = atoi(pattern_height_s);

					alloc_board(pattern_width, pattern_height, &local_board);
				}
			}
			else if (line_state == PATTERN_LINE)
			{
				if (ch >= '0' && ch <= '9')
				{
					run_count[strlen(run_count)+1] = '\0';
					run_count[strlen(run_count)]   = ch;
				}
				else if (ch == '!')
					break;
				else
				{
					int times = 1;

					if (run_count[0] != '\0')
						times = atoi(run_count);

					if (ch == 'o')
					{
						while (times--)
						{
							local_board.cells[x][y] = true;
							x++;
						}
					}
					else if (ch == 'b')
					{
						while (times--)
							x++;
					}
					else if (ch == '$')
					{
						x=0;
						while (times--)
							y++;
					}

					strcpy(run_count, "");
				}

				if (x > pattern_width ||
						y > pattern_height)
				{
					TraceLog(LOG_ERROR, "FILE: Pattern exceded allocated size");
					break;
				}
				else if (x > MAX_BOARD_SIZE ||
								 y > MAX_BOARD_SIZE)
				{
					TraceLog(LOG_ERROR, "FILE: Pattern exceded max size");
					break;
				}
			}

			if (ch == '\n')
			{
				line_start = true;
				continue;
			}
		}
	}
	
	fclose(fp);

	return local_board;
}
