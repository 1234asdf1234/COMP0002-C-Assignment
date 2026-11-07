#include "maze.h"
#include "bot.h"
#include "constants.h"
#include "graphics.h"

#include <stdio.h>

Blocktype maze[SIDE][SIDE];

const colour block_colours[4] = {black, black, yellow, gray};

// paths to be drawn with drawRect() and others with fillRect()

static void draw_path(int x, int y) {
    drawRect(x, y, BLOCKSIZE, BLOCKSIZE);
}

static void draw_rest(int x, int y) {
    fillRect(x, y, BLOCKSIZE, BLOCKSIZE);
}

// change the colour of next block
static void change_colour(Blocktype b) {
    setColour(block_colours[b]);
}

// draw the board, change colours only if necessary
void draw_maze() {
    // only the bot is redrawn, then draw the maze on background
    background();

    Blocktype last_block = WALL;    // assume top-left corner is always black
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            // change colour if next block is different
            if (maze[i][j] != last_block) {
                last_block = maze[i][j];
                change_colour(last_block);
            }
            if (maze[i][j] == PATH) {
                draw_path(BLOCKSIZE*j, BLOCKSIZE*i);
            }
            else {
                draw_rest(BLOCKSIZE*j, BLOCKSIZE*i);
            }
        }
    }
}

/* 
suppose the text file contains a square of numbers,
with side length of SIDE,
each corresponds to the Blocktype enum in constants.h.
*/

// write contents of maze file into the 2D array.
void read_maze(const char* path) {
    // open the file
    FILE *p = fopen(path, "r");
    int ch;
    int row = 0, col = 0;
    while((ch = fgetc(p)) != EOF) {
        if (ch == '\n') {
            row++;
            col = 0;
            continue;
        }
        maze[row][col] = (Blocktype)(ch - '0'); // convert to blocktype enum
        col++;
    }
    fclose(p);
}

// test function to print maze in command line.
void print_maze() {
    Blocktype buf;
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            buf = maze[i][j];
            switch (buf) {
                case WALL:
                    putchar('W');
                    break;
                case PATH:
                    putchar(' ');
                    break;
                case START:
                    putchar('S');
                    break;
                case END:
                    putchar('E');
                    break;
                default:
                    puts("???");
            }
        }
        puts("");
    }
}

static int test_maze(void) {
    read_maze("sample_maze.txt");
    draw_maze();
    bot_prepare();
    draw_bot((Bot){(Point){0, 1}, RIGHT});
    return 0;
}

