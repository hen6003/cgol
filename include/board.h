#include <stdbool.h>
#include <stddef.h>

#define MAX_BOARD_SIZE 50

typedef struct board  
{
  size_t x,y,width,height;
  bool **cells;
} board;

extern board main_board;

board *alloc_board(size_t width, size_t height, board *local_board);
void free_board(board *local_board);
bool copy_board(board src_board, board dest_board);
