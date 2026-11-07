#include "bot.h"
#include "maze.h"
#include "constants.h"
#include "graphics.h"
#include <math.h>
#include <stdio.h>

// M_PI is not standard C so define our own
#define PI 3.1415926535

// polygon can be customised
#define POLYGON 3

static Position rotate(float length, float angle, Position pos) {
    angle = angle * (PI / 180.0);
    float x = pos.x + cos(angle) * length;
    float y = pos.y + sin(angle) * length;
    return (Position){x, y};
}

static void set_coords(float angle_offset, int coords[2][POLYGON], Position centre) {
    Position buf;
    float current_angle = angle_offset;
    for (int i = 0; i < POLYGON; i++) {
        buf = rotate(BLOCKSIZE/2.0, current_angle, centre);
        coords[0][i] = lround(buf.x);
        coords[1][i] = lround(buf.y);
        current_angle += (360.0 / POLYGON);
    }
}

// preparation for drawing the bot
void bot_prepare() {
    foreground();
    setColour(green);
}

// draw the bot given the block position.
// assume only one bot exists at a time.
void draw_bot(Bot bot) {
    // foreground() should be called before this to avoid redundancy
    clear();

    int coords[2][POLYGON];   // x coords and y coords
    Position centre = {BLOCKSIZE*(bot.p.x+0.5), BLOCKSIZE*(bot.p.y+0.5)};
    switch (bot.dir) {
        case RIGHT:
            set_coords(0.0, coords, centre);
            break;
        case LEFT:
            set_coords(180.0, coords, centre);
            break;
        case UP:
            set_coords(270.0, coords, centre);
            break;
        case DOWN:
            set_coords(90.0, coords, centre);
            break;
        default:
            set_coords(0.0, coords, centre);
    }

    fillPolygon(POLYGON, coords[0], coords[1]);
}

// update bot status by redrawing and sleeping
void update_bot(Bot *bot, int mili) {
    draw_bot(*bot);
    sleep(mili);
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
