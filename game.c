#include "game.h"
#include "game_aux.h"

struct game_s {
  square *squares;
  uint *nb_tents_row;
  uint *nb_tents_col;
  uint nb_rows;
  uint nb_cols;
};

game game_new(square *squares, uint *nb_tents_row, uint *nb_tents_col) {}

game game_new_empty(void) {}

game game_copy(cgame g) {}

bool game_equal(cgame g1, cgame g2) {}

void game_delete(game g) {}

void game_set_square(game g, uint i, uint j, square s) {}

square game_get_square(cgame g, uint i, uint j) {}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {}

uint game_get_expected_nb_tents_row(cgame g, uint i) {}

uint game_get_expected_nb_tents_col(cgame g, uint j) {}

uint game_get_expected_nb_tents_all(cgame g) {}

uint game_get_current_nb_tents_row(cgame g, uint i) {}

uint game_get_current_nb_tents_col(cgame g, uint j) {}

uint game_get_current_nb_tents_all(cgame g) {}

void game_play_move(game g, uint i, uint j, square s) {}

int game_check_move(cgame g, uint i, uint j, square s) {}

bool game_is_over(cgame g) {}

void game_fill_grass_row(game g, uint i) {}

void game_fill_grass_col(game g, uint j) {}

void game_restart(game g) {}
