#ifndef MAZE_H
#define MAZE_H

#include "constants.h"
#include "bot.h"

#define SIDE 11
#define BLOCKSIZE 20

extern Blocktype maze[SIDE][SIDE];

void read_maze(const char *path);
void draw_maze();
void draw_maze_cmd(Bot *bot);


#endif