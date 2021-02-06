#include "game_tools.h"
#include <assert.h>
#include <stdio.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

game game_load(char *filename);

void game_save(cgame g, char *filename);
