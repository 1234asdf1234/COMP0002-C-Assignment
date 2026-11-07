#include "bot.h"
#include "constants.h"
#include "maze.h"
#include <stdio.h>

// naive right-wall algorithm to test bot controlling functions
int search_right_hand(Bot *bot) {
    while(!at_marker(bot)) {
        while(can_move_forward(bot)) {
            forward(bot);
            // fprintf(stderr, "current direction: %d\n", bot->dir);
        }
        // fprintf(stderr, "No road on %d", bot->dir);
        right(bot);
        if (maze[bot->p.y][bot->p.x] == START) {
            return 0;   // failed, bot returned to starting point
        }
    }
    return 1;   // success
}

int recursive_search(Bot *bot) {
    if (at_marker(bot)) {
        return 1;
    }
    if (can_move_forward(bot)) {
        forward(bot);
        if (recursive_search(bot)) {
            return 1;
        }
        else {
            // turn back
            right(bot);
            right(bot);
        }
    }
    // check left branch
    left(bot);
    if (can_move_forward(bot)) {
        forward(bot);
        if (recursive_search(bot)) {
            return 1;
        }
        else {
            // turn back
            right(bot);
            right(bot);
        }
    }
    // check right branch
    right(bot);
    right(bot);
    if (can_move_forward(bot)) {
        forward(bot);
        if (recursive_search(bot)) {
            return 1;
        }
        else {
            // turn back
            right(bot);
            right(bot);
        }
    }
    // dead end, go to previous position
    right(bot);
    forward(bot);
    return 0;
}