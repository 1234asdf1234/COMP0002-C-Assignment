#include "bot.h"
#include "maze.h"
#include "constants.h"
#include <unistd.h>


// M_PI is not standard C so define our own
#define PI 3.1415926535

// polygon can be customised
#define POLYGON 3

// in command line, redraw the whole board
void update_bot(Bot *bot, int mili) {
    draw_maze_cmd(bot);
    usleep(mili*1000);
}

// should only be called if can_move_forward() returns True
void forward(Bot *bot) {
    switch (bot->dir) {
        case UP:
            bot->p.y -= 1;
            break;
        case DOWN:
            bot->p.y += 1;
            break;
        case LEFT:
            bot->p.x -= 1;
            break;
        case RIGHT:
            bot->p.x += 1;
            break;
        default:
            bot->p.y += 1;
    }
    // fprintf(stderr, "moved to {%d %d}", bot->p.x, bot->p.y);
    update_bot(bot, 50);
}

void left(Bot *bot) {
    bot->dir = (bot->dir-1) % 4;
    update_bot(bot, 50);
}

void right(Bot *bot) {
    bot->dir = (bot->dir+1) % 4;
    update_bot(bot, 50);
}

int at_marker(Bot *bot) {
    return maze[bot->p.y][bot->p.x] == END;
}

int can_move_forward(Bot *bot) {
    // fprintf(stderr, "Place {%d %d} is block %d", bot->p.y, bot->p.x+1, maze[bot->p.y][bot->p.x+1]);
    switch (bot->dir) {
        case LEFT:
            if (bot->p.x == 0)
                return 0;
            return maze[bot->p.y][bot->p.x-1] != WALL;
        case RIGHT:
            if (bot->p.x == SIDE-1)
                return 0;
            return maze[bot->p.y][bot->p.x+1] != WALL;
        case UP:
            if (bot->p.y == 0) 
                return 0;
            return maze[bot->p.y-1][bot->p.x] != WALL;
        case DOWN:
            if (bot->p.y == SIDE-1) 
                return 0;
            return maze[bot->p.y+1][bot->p.x] != WALL;  
        default:
            return 0; 
    }
}
