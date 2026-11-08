#ifndef MAZE_H
#define MAZE_H

#include "constants.h"

#define SIDE 11
#define BLOCKSIZE 20

extern Blocktype maze[SIDE][SIDE];

void read_maze(const char *path);
void draw_maze();

void generate_maze(Point start);

#endif