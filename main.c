#include "search.h"
#include "bot.h"
#include "constants.h"
#include "maze.h"
#include "graphics.h"

int main() {
    setWindowSize(BLOCKSIZE*(SIDE+2), BLOCKSIZE*(SIDE+2));
    generate_maze((Point){0, 1});
    draw_maze();
    bot_prepare();
    Bot bot = {(Point){0, 1}, RIGHT};
    recursive_search(&bot);
    return 0;
}
