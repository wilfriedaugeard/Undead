#include "game.h"
#include "game_io.h"
#include "UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#ifndef _UNDEAD_TEXT_
#define _UNDEAD_TEXT_

#define WIDTH 4
#define HEIGHT 4
#define MAXLINELEN 4096

void display_game(game game1);
int play(game game1);
game default_game();




#endif