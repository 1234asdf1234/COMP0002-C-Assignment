#ifndef BOT_H
#define BOT_H

#include "constants.h"

// drawing functions for bot
void draw_bot(Bot bot);
void bot_prepare();
void update_bot(Bot *bot, int mili);

// control functions
void forward(Bot *bot);
void left(Bot *bot);
void right(Bot *bot);
int at_marker(Bot *bot);
int can_move_forward(Bot *bot);

// non-standard helper for recursive search
int at_start(Bot *bot);

#endif