#include "game.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game_aux.h"

struct game_s {
  square *squares;
  uint *nb_tents_row;
  uint *nb_tents_col;
  uint nb_rows;
  uint nb_cols;
};

uint nb_tents_row_default[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
uint nb_tents_col_default[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};

square squares_default[DEFAULT_SIZE * DEFAULT_SIZE] = {
    EMPTY, EMPTY, EMPTY, EMPTY, TREE, TREE, EMPTY, EMPTY,
    TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
    EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY,
    TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
    EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};

square squares_default_solution[DEFAULT_SIZE * DEFAULT_SIZE] = {
    TENT, GRASS, GRASS, TENT, TREE, TREE, TENT, GRASS,
    TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
    TENT, GRASS, GRASS, TENT, TREE, TENT, GRASS, TENT,
    TREE, GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS,
    TENT, TREE, TENT, GRASS, TENT, GRASS, TENT, GRASS,
    TREE, GRASS, GRASS, GRASS, TREE, GRASS, TREE, GRASS,
    TENT, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
    TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};

game game_new(square *squares, uint *nb_tents_row, uint *nb_tents_col) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = malloc(sizeof(struct game_s));
  g->squares = malloc(nb_rows * nb_cols * sizeof(square));
  g->nb_tents_row = malloc(nb_rows * sizeof(uint));
  g->nb_tents_col = malloc(nb_cols * sizeof(uint));
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);

  for (uint i = 0; i < nb_rows; i++) {
    g->nb_tents_row[i] = nb_tents_row[i];
  }
  for (uint j = 0; j < nb_cols; j++) {
    g->nb_tents_col[j] = nb_tents_col[j];
  }
  for (uint k = 0; k < (nb_rows * nb_cols); k++) {
    g->squares[k] = squares[k];
  }
  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;

  return g;
}

game game_new_empty(void) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = malloc(sizeof(struct game_s));
  g->squares = malloc(nb_rows * nb_cols * sizeof(square));
  g->nb_tents_row = malloc(nb_rows * sizeof(uint));
  g->nb_tents_col = malloc(nb_cols * sizeof(uint));
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);

  for (uint i = 0; i < nb_rows; i++) {
    g->nb_tents_row[i] = 0;
  }
  for (uint j = 0; j < nb_cols; j++) {
    g->nb_tents_col[j] = 0;
  }
  for (uint k = 0; k < (nb_rows * nb_cols); k++) {
    g->squares[k] = EMPTY;
  }
  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;

  return g;
}

game game_copy(cgame g) {
  game g_copy = game_new_empty();
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g_copy);
  assert(g_copy->squares && g_copy->nb_tents_row && g_copy->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;

  for (uint i = 0; i < nb_rows; i++) {
    g_copy->nb_tents_row[i] = g->nb_tents_row[i];
  }
  for (uint j = 0; j < nb_cols; j++) {
    g_copy->nb_tents_col[j] = g->nb_tents_col[j];
  }
  for (uint k = 0; k < (nb_rows * nb_cols); k++) {
    g_copy->squares[k] = g->squares[k];
  }
  g_copy->nb_rows = nb_rows;
  g_copy->nb_cols = nb_cols;

  return g_copy;
}

bool game_equal(cgame g1, cgame g2) {
  assert(g1);
  assert(g1->squares && g1->nb_tents_row && g1->nb_tents_col);
  assert(g2);
  assert(g2->squares && g2->nb_tents_row && g2->nb_tents_col);
  uint nb_rows = g1->nb_rows;
  uint nb_cols = g1->nb_cols;

  for (uint i = 0; i < nb_rows; i++) {
    if (g1->nb_tents_row[i] != g2->nb_tents_row[i]) {
      return false;
    }
  }
  for (uint j = 0; j < nb_cols; j++) {
    if (g1->nb_tents_col[j] != g2->nb_tents_col[j]) {
      return false;
    }
  }
  for (uint k = 0; k < (nb_rows * nb_cols); k++) {
    if (g1->squares[k] != g2->squares[k]) {
      return false;
    }
  }
  if (g1->nb_rows != g2->nb_rows || g1->nb_cols != g2->nb_cols) {
    return false;
  }
  return true;
}

void game_delete(game g) {
  assert(g);
  free(g->squares);
  free(g->nb_tents_row);
  free(g->nb_tents_col);
  free(g);
}

void game_set_square(game g, uint i, uint j, square s) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);
  assert(s >= 0 && s <= 3);

  g->squares[i * nb_cols + j] = s;
}

square game_get_square(cgame g, uint i, uint j) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);

  return g->squares[i * nb_cols + j];
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(i >= 0 && i < nb_rows);
  assert(nb_tents <= (nb_cols / 2 + nb_cols % 2));

  g->nb_tents_row[i] = nb_tents;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(j >= 0 && j < nb_cols);
  assert(nb_tents <= (nb_rows / 2 + nb_rows % 2));

  g->nb_tents_col[j] = nb_tents;
}

uint game_get_expected_nb_tents_row(cgame g, uint i) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  // uint nb_cols = g->nb_cols;
  assert(i >= 0 && i < nb_rows);

  return g->nb_tents_row[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  // uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(j >= 0 && j < nb_cols);

  return g->nb_tents_col[j];
}

uint game_get_expected_nb_tents_all(cgame g) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  uint count = 0;

  for (uint k = 0; k < nb_rows * nb_cols; k++) {
    if (g->squares[k] == TREE) {
      count++;
    }
  }
  return count;
}

uint game_get_current_nb_tents_row(cgame g, uint i) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(i >= 0 && i < nb_rows);
  uint count = 0;

  for (uint j = 0; j < nb_cols; j++) {
    if (g->squares[i * nb_cols + j] == TENT) {
      count++;
    }
  }
  return count;
}

uint game_get_current_nb_tents_col(cgame g, uint j) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(j >= 0 && j < nb_cols);
  uint count = 0;

  for (uint i = 0; i < nb_rows; i++) {
    if (g->squares[i * nb_cols + j] == TENT) {
      count++;
    }
  }
  return count;
}

uint game_get_current_nb_tents_all(cgame g) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  uint count = 0;

  for (uint k = 0; k < nb_rows * nb_cols; k++) {
    if (g->squares[k] == TENT) {
      count++;
    }
  }
  return count;
}

void game_play_move(game g, uint i, uint j, square s) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);
  assert(s >= 0 && s <= 3);
  assert(s != TREE);
  assert(g->squares[i * nb_cols + j] != TREE);

  g->squares[i * nb_cols + j] = s;
}

// TODO
int game_check_move(cgame g, uint i, uint j, square s) {
  return REGULAR;
}

bool game_is_over(cgame g) {
  return false;
}

void game_fill_grass_row(game g, uint i) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(i >= 0 && i < nb_rows);

  for (uint j = 0; j < nb_cols; j++) {
    if (g->squares[i * nb_cols + j] == EMPTY) {
      g->squares[i * nb_cols + j] = GRASS;
    }
  }
}

void game_fill_grass_col(game g, uint j) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  // uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(j >= 0 && j < nb_cols);

  for (uint i = 0; i < nb_cols; i++) {
    if (g->squares[i * nb_cols + j] == EMPTY) {
      g->squares[i * nb_cols + j] = GRASS;
    }
  }
}

void game_restart(game g) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;

  for (uint k = 0; k < nb_rows * nb_cols; k++) {
    if (g->squares[k] != TREE) {
      g->squares[k] = EMPTY;
    }
  }
}
