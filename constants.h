#ifndef CONSTANT_H
#define CONSTANT_H
// used to represent specific block in the maze,
// e.g. {0, 1} represents row 1, column 2
typedef struct {
    int x;
    int y;
} Point;

// point with decimal precision used for graphical functions
typedef struct {
    float x;
    float y;
} Position;

// represent type of each block, i.e.
// /path/wall/start/end
typedef enum {
    WALL,
    PATH,
    START,
    END
} Blocktype;

// direction of the bot
typedef enum {
    RIGHT,
    DOWN,
    LEFT,
    UP
} Direction;

// state of the bot
typedef struct {
    Point p;
    Direction dir;
} Bot;

#endif

