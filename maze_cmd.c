#include "maze.h"
#include "bot.h"
#include "constants.h"
#include <locale.h> // square workaround from https://stackoverflow.com/a/72810263

#include <stdio.h>

Blocktype maze[SIDE][SIDE];

// characters for each type of block stored in strings
const char* blocks[4] = {"■", " ", "*", "%"};

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

int same_point(Point p1, Point p2) {
    return (p1.x == p2.x) && (p1.y == p2.y);
}

const char* bot_char(Bot *bot) {
    switch(bot->dir) {
        case UP:
            return "^";
        case DOWN:
            return "v";
        case LEFT:
            return "<";
        case RIGHT:
            return ">";
        default:
            return "?";
    }
}

// test function to print maze in command line.
void draw_maze_cmd(Bot *bot) {
    Blocktype buf;
    setlocale(LC_CTYPE, "en_US.UTF-8"); // 
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (same_point(bot->p, (Point){j, i})) {
                printf("%s", bot_char(bot));
                continue;
            }
            buf = maze[i][j];
            printf("%s", blocks[buf]);
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

