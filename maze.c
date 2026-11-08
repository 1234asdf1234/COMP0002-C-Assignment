#include "maze.h"
#include "bot.h"
#include "constants.h"
#include "graphics.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Blocktype maze[SIDE][SIDE] = {WALL};

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

static int near_border(Point p, Direction dir) {
    switch(dir) {
        case LEFT:
            return p.x == 1;
        case RIGHT:
            return p.x == SIDE-2;
        case UP:
            return p.y == 1;
        case DOWN:
            return p.y == SIDE-2;
        default:
            return 0;
    }
}

static Direction start_direction(Point p) {
    if (p.x == 0) {
        return LEFT;
    }
    else if (p.x == SIDE-1) {
        return RIGHT;
    }
    else if (p.y == 0) {
        return UP;
    }
    else if (p.y == SIDE-1) {
        return DOWN;
    }
    fprintf(stderr, "ERROR: invalid starting point");
    exit(1);
}

static Direction opposite_direction(Direction dir) {
    return (Direction)((dir + 2) % 4);
}

static void add_walls(Point p, Point *walls, int *wall_size) {
    // this function will not be called on the corners of map thus safe to use
    int no_left = near_border(p, LEFT);
    int no_right = near_border(p, RIGHT);
    int no_up = near_border(p, UP);
    int no_down = near_border(p, DOWN);
    int add_counter = 0;
    if (p.x == 0) { // left edge
        no_left = 1;
        no_up = 1;
        no_down = 1;
    }
    else if (p.x == SIDE-1) {   // right edge
        no_right = 1;
        no_up = 1;
        no_down = 1;
    }
    else if (p.y == 0) {    // top edge
        no_up = 1;
        no_left = 1;
        no_right = 1;
    }
    else if (p.y == SIDE-1) {   // bottom edge
        no_down = 1;
        no_left = 1;
        no_right = 1;
    }
    if (!no_up && (maze[p.y-1][p.x] == WALL)) {
        walls[*wall_size+add_counter] = (Point){p.x, p.y-1};
        add_counter++;
    }
    if (!no_down && (maze[p.y+1][p.x] == WALL)) {
        walls[*wall_size+add_counter] = (Point){p.x, p.y+1};
        add_counter++;
    }
    if (!no_left && (maze[p.y][p.x-1] == WALL)) {
        walls[*wall_size+add_counter] = (Point){p.x-1, p.y};
        add_counter++;
    }
    if (!no_right && (maze[p.y][p.x+1] == WALL)) {
        walls[*wall_size+add_counter] = (Point){p.x+1, p.y};
        add_counter++;
    }
    *wall_size += add_counter;
}

static Point towards_border(Point p, Direction dir) {
    switch(dir) {
        case UP:
            return (Point){p.x, p.y-1};
        case DOWN:
            return (Point){p.x, p.y+1};
        case LEFT:
            return (Point){p.x-1, p.y};
        case RIGHT:
            return (Point){p.x+1, p.y};
        default:
            return p;
    }
}

static int only_one_visited(Point p) {
    int is_path[4] = {maze[p.y][p.x-1] != WALL,
        maze[p.y][p.x+1] != WALL,
        maze[p.y-1][p.x] != WALL,
        maze[p.y+1][p.x] != WALL};
    int success = 0;
    // puts("thinking...");
    for (int i = 0; i < 4; i++) {
        if (is_path[i]) {
            if (success) {
                return 0;
            }
            success = 1;
        }
    }
    return success;
}

// Iterative randomized Prim's algorithm (without stack, without sets)
/* caveat: when shrinking array of walls, move the last element to the element
   to be deleted, then move array_end index back by one
 */
void generate_maze(Point start) {
    // reset seed
    srand(time(NULL));

    Point *walls = malloc(sizeof(Point) * SIDE * SIDE / 2);
    Point *possible_ends = malloc(sizeof(Point) * SIDE);
    int wall_size = 0;
    Point current_wall;
    int wall_index;
    int ends_index = 0;

    Direction start_dir = start_direction(start);
    Direction opposite_dir = opposite_direction(start_dir);

    maze[start.y][start.x] = START;



    add_walls(start, walls, &wall_size);

    while(wall_size > 0) {
        wall_index = rand()%wall_size;
        current_wall = walls[wall_index]; // safe since everything % 1 == 0
        if (only_one_visited(current_wall)) {
            maze[current_wall.y][current_wall.x] = PATH;
            add_walls(current_wall, walls, &wall_size);
            if (near_border(current_wall, opposite_dir)) {
                possible_ends[ends_index] = current_wall;
                ends_index++;
            }
        }
        if (wall_size == 1) {
            // no more walls to add, terminate
            wall_size--;
            continue;
        }
        // else replace the current wall with the element in the end
        walls[wall_index] = walls[wall_size-1];
        wall_size--;
    }

    // randomly choose one eligible point to be the end
    Point end = possible_ends[rand()%ends_index];
    end = towards_border(end, opposite_dir);
    maze[end.y][end.x] = END;

    free(walls);
    free(possible_ends);     
}

 
