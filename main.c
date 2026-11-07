#include "search.h"
#include "constants.h"
#include "maze.h"

int main() {
    read_maze("sample_maze.txt");
    Bot bot = {(Point){0, 1}, RIGHT};
    draw_maze_cmd(&bot);
    recursive_search(&bot);
    return 0;
}
