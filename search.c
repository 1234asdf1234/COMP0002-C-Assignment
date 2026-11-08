#include "bot.h"
#include "constants.h"
#include "maze.h"
#include <stdio.h>

int last_status;

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

/* althought this is guranteed to work,
   it is inefficient as a lot of turning 
   takes place for each block visited  */

/* I deliberately repeated the code for three branches
   in order not to fill up the call stack with twice as much
   of function calls */
int recursive_search(Bot *bot) {
    if (at_marker(bot)) {
        return 1;
    }
    if (at_start(bot)) {
        return -1;   // no possible route to marker, declare failure
    }
    if (can_move_forward(bot)) {
        forward(bot);
        last_status = recursive_search(bot);
        switch(last_status) {
            case 1:
                return 1;   // success
            case -1:
                return -1;  // overall fail
            default:
                right(bot); // fail for this node
        }
    }
    else {
    // check left branch
    left(bot);
    }
    if (can_move_forward(bot)) {
        forward(bot);
        last_status = recursive_search(bot);
        switch(last_status) {
            case 1:
                return 1;   // success
            case -1:
                return -1;  // overall fail
            default:
                ;           // do nothing, bot already at correct direction
        }
    }
    else {
    // check right branch
    right(bot);
    right(bot);
    }
    if (can_move_forward(bot)) {
        forward(bot);
        last_status = recursive_search(bot);
        switch(last_status) {
            case 1:
                return 1;   // success
            case -1:
                return -1;  // overall fail
            default:
                right(bot); // fail for this node
                right(bot);
        }
    }
    // dead end, go to previous position
    right(bot);
    forward(bot);
    return 0;
}