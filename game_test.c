#include "game.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_aux.h"

//* Usage
void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> \n", argv[0]);
  exit(EXIT_FAILURE);
}

//* Aux Functions

uint game_get_nb_adj_test(cgame g, uint i, uint j, square s) {
  assert(g);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);
  assert(s >= 0 && s <= 3);

  uint count = 0;
  if ((int)(i - 1) >= 0) {
    if (game_get_square(g, i - 1, j) == s) {
      count++;
    }
  }
  if ((int)(j - 1) >= 0) {
    if (game_get_square(g, i, j - 1) == s) {
      count++;
    }
  }
  if ((j + 1) < nb_cols) {
    if (game_get_square(g, i, j + 1) == s) {
      count++;
    }
  }
  if ((i + 1) < nb_rows) {
    if (game_get_square(g, i + 1, j) == s) {
      count++;
    }
  }
  return count;
}

uint game_get_nb_tents_diag_adj_test(cgame g, uint i, uint j) {
  assert(g);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);

  uint count = 0;
  if ((int)(i - 1) >= 0 && (int)(j - 1) >= 0) {
    if (game_get_square(g, i - 1, j - 1) == TENT) {
      count++;
    }
  }
  if ((int)(i - 1) >= 0 && (j + 1) < nb_cols) {
    if (game_get_square(g, i - 1, j + 1) == TENT) {
      count++;
    }
  }
  if ((i + 1) < nb_rows && (int)(j - 1) >= 0) {
    if (game_get_square(g, i + 1, j - 1) == TENT) {
      count++;
    }
  }
  if ((i + 1) < nb_rows && (j + 1) < nb_cols) {
    if (game_get_square(g, i + 1, j + 1) == TENT) {
      count++;
    }
  }
  return count;
}

uint game_get_current_nb_emptys_row_test(cgame g, uint i) {
  assert(g);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(i >= 0 && i < nb_rows);

  uint count = 0;
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      count++;
    }
  }
  return count;
}

uint game_get_current_nb_emptys_col_test(cgame g, uint j) {
  assert(g);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(j >= 0 && j < nb_cols);

  uint count = 0;
  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_square(g, i, j) == EMPTY) {
      count++;
    }
  }
  return count;
}

uint game_get_current_nb_emptys_all_test(cgame g) {
  assert(g);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;

  uint count = 0;
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        count++;
      }
    }
  }
  return count;
}

//* Tests

bool test_game_new() {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  extern square squares_default[DEFAULT_SIZE * DEFAULT_SIZE];
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = game_new(squares_default, nb_tents_row_default, nb_tents_col_default);
  assert(g);

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_default[i]) {
      return false;
    }
  }
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_default[j]) {
      return false;
    }
  }
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != squares_default[i * nb_cols + j]) {
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_new_empty() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = game_new_empty();
  assert(g);

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != 0) {
      return false;
    }
  }
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != 0) {
      return false;
    }
  }
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != EMPTY) {
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_copy() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g1 = game_default();
  game g2 = game_copy(g1);
  assert(g1 && g2);

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g1, i) != game_get_expected_nb_tents_row(g2, i)) {
      return false;
    }
  }
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g1, j) != game_get_expected_nb_tents_col(g2, j)) {
      return false;
    }
  }
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) {
        return false;
      }
    }
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_equal() {
  uint nb_tents_row[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
  uint nb_tents_col[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
  square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, TREE, TREE, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
      EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game g1 = game_default();
  game g2 = game_default();
  assert(g1 && g2);

  // Totally Equal games
  if (!game_equal(g1, g2)) {
    return false;
  }

  // Equal only nb_tents_row[]
  nb_tents_col[0] = 0;  // change default values of nb_tents_col[] and squares[]
  squares[0] = TREE;
  game_delete(g2);
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) {
    return false;
  }
  nb_tents_col[0] = 4;  // return default value for nb_tents_col[] and squares[]
  squares[0] = EMPTY;

  // Equal only nb_tents_col[]
  nb_tents_row[0] = 0;  // change default value of nb_tents_row[] and squares[]
  squares[0] = TREE;
  game_delete(g2);
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) {
    return false;
  }
  nb_tents_row[0] = 3;  // return default value for nb_tents_row[] and squares[]
  squares[0] = EMPTY;

  // Equal only nb_tents_row and nb_tents_all
  squares[0] = TREE;  // change default value of squares[]
  game_delete(g2);
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) {
    return false;
  }
  squares[0] = EMPTY;  // return default value for squares[]

  // Equal only squares
  nb_tents_row[0] = 0;  // change default values of nb_tents_row[] and nb_tents_col[]
  nb_tents_col[0] = 0;
  game_delete(g2);
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) {
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_set_square() {
  extern square squares_default_solution[DEFAULT_SIZE * DEFAULT_SIZE];
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  square squares_empty[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game g1 = game_default();
  game g2 = game_default_solution();
  assert(g1 && g2);

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      game_set_square(g1, i, j, squares_default_solution[i * nb_cols + j]);
    }
  }
  if (!game_equal(g1, g2)) {
    return false;
  }

  game_delete(g1);
  g1 = game_new(squares_empty, nb_tents_row_default, nb_tents_col_default);
  assert(g1);
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      game_set_square(g2, i, j, EMPTY);
    }
  }
  if (!game_equal(g1, g2)) {
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_get_square() {
  extern square squares_default_solution[DEFAULT_SIZE * DEFAULT_SIZE];
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = game_default_solution();
  assert(g);

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != squares_default_solution[i * nb_cols + j]) {
        return false;
      }
    }
  }

  game_delete(g);
  g = game_new_empty();
  assert(g);
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != EMPTY) {
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_set_expected_nb_tents_row() {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  extern square squares_default[DEFAULT_SIZE * DEFAULT_SIZE];
  uint nb_rows = DEFAULT_SIZE;
  uint nb_tents_row_new[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  game g1 = game_default();
  game g2 = game_new(squares_default, nb_tents_row_new, nb_tents_col_default);
  assert(g1 && g2);

  for (uint i = 0; i < nb_rows; i++) {
    game_set_expected_nb_tents_row(g2, i, nb_tents_row_default[i]);
  }
  if (!game_equal(g1, g2)) {
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_set_expected_nb_tents_col() {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  extern square squares_default[DEFAULT_SIZE * DEFAULT_SIZE];
  uint nb_cols = DEFAULT_SIZE;
  uint nb_tents_col_new[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  game g1 = game_default();
  game g2 = game_new(squares_default, nb_tents_row_default, nb_tents_col_new);
  assert(g1 && g2);

  for (uint j = 0; j < nb_cols; j++) {
    game_set_expected_nb_tents_col(g2, j, nb_tents_col_default[j]);
  }
  if (!game_equal(g1, g2)) {
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_get_expected_nb_tents_row() {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  uint nb_rows = DEFAULT_SIZE;
  game g = game_default();
  assert(g);

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_default[i]) {
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_game_get_expected_nb_tents_col() {
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  uint nb_cols = DEFAULT_SIZE;
  game g = game_default();
  assert(g);

  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_default[j]) {
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_game_get_expected_nb_tents_all() {
  game g = game_default();
  assert(g);

  if (game_get_expected_nb_tents_all(g) != 12) {
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_get_current_nb_tents_row() {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  uint nb_rows = DEFAULT_SIZE;
  game g = game_default();
  assert(g);

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_current_nb_tents_row(g, i) != 0) {
      return false;
    }
  }

  game_delete(g);
  g = game_default_solution();
  assert(g);
  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_current_nb_tents_row(g, i) != nb_tents_row_default[i]) {
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_game_get_current_nb_tents_col() {
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  uint nb_cols = DEFAULT_SIZE;
  game g = game_default();
  assert(g);

  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_current_nb_tents_col(g, j) != 0) {
      return false;
    }
  }

  game_delete(g);
  g = game_default_solution();
  assert(g);
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_current_nb_tents_col(g, j) != nb_tents_col_default[j]) {
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_game_get_current_nb_tents_all() {
  game g = game_default();
  assert(g);

  if (game_get_current_nb_tents_all(g) != 0) {
    return false;
  }

  game_delete(g);
  g = game_default_solution();
  assert(g);
  if (game_get_current_nb_tents_all(g) != 12) {
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_play_move() {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  square squares_full_grass[DEFAULT_SIZE * DEFAULT_SIZE] = {
      GRASS, GRASS, GRASS, GRASS, TREE, TREE, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
      GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS,
      GRASS, TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, TREE, GRASS, TREE, GRASS,
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};
  square squares_full_tents[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, TENT, TENT, TENT, TREE, TREE, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TENT, TENT, TREE,
      TENT, TENT, TENT, TENT, TREE, TENT, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TREE, TENT, TENT,
      TENT, TREE, TENT, TENT, TENT, TENT, TENT, TENT,
      TREE, TENT, TENT, TENT, TREE, TENT, TREE, TENT,
      TENT, TENT, TENT, TENT, TENT, TENT, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TENT, TENT, TENT};
  game g1 = game_default();
  assert(g1);

  // g1: play GRASS on all squares except TREE
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (squares_full_grass[i * nb_cols + j] != TREE) {
        game_play_move(g1, i, j, GRASS);
      }
    }
  }

  game g2 = game_new(squares_full_grass, nb_tents_row_default, nb_tents_col_default);
  assert(g2);
  // compare g1 with game filled with GRASS
  if (!game_equal(g1, g2)) {
    return false;
  }

  // g1: play TENT on all squares except TREE
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (squares_full_grass[i * nb_cols + j] != TREE) {
        game_play_move(g1, i, j, TENT);
      }
    }
  }

  game_delete(g2);
  g2 = game_new(squares_full_tents, nb_tents_row_default, nb_tents_col_default);
  assert(g2);
  // compare g1 with game filled with TENT
  if (!game_equal(g1, g2)) {
    return false;
  }

  // g1: play EMPTY on all squares except TREE
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (squares_full_grass[i * nb_cols + j] != TREE) {
        game_play_move(g1, i, j, EMPTY);
      }
    }
  }

  game_delete(g2);
  g2 = game_default();
  assert(g2);
  // compare g1 with game_default
  if (!game_equal(g1, g2)) {
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  return true;
}

//TODO
bool test_game_check_move_1() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  //LOSING
  game g_empty = game_new_empty();
  assert(g_empty);
  for (uint i = 0; i < nb_rows; i++) {
    game_set_expected_nb_tents_row(g_empty, i, 1);
  }
  for (uint j = 0; j < nb_cols; j++) {
    game_set_expected_nb_tents_col(g_empty, j, 1);
  }

  game_set_square(g_empty, 0, 0, TREE);
  game_set_square(g_empty, 1, 1, TREE);
  game_set_square(g_empty, 1, 2, TREE);
  game_set_square(g_empty, 1, 3, TREE);
  game_set_square(g_empty, 4, 2, TREE);
  game_set_square(g_empty, 7, 5, TENT);
  game_set_square(g_empty, 7, 6, TENT);
  game_set_square(g_empty, 6, 7, TENT);
  game_set_square(g_empty, 7, 7, TENT);
  for (uint j = 1; j < (nb_cols - 1); j++) {
    game_set_square(g_empty, 0, j, GRASS);
  }

  // REGULAR move
  if (game_check_move(g_empty, 2, 1, TENT) != REGULAR) {
    return false;
  }
  game_play_move(g_empty, 2, 1, TENT);
  // DIAG TENT
  if (game_check_move(g_empty, 1, 0, TENT) != LOSING) {
    return false;
  }
  // ADJ TENT
  if (game_check_move(g_empty, 2, 2, TENT) != LOSING) {
    return false;
  }
  // MORE TENTS THAN EXPECTED
  if (game_check_move(g_empty, 2, 3, TENT) != LOSING) {
    return false;
  }
  // TOO MANY GRASS --> NOT ENOUGH EMPTY FOR TENT
  if (game_check_move(g_empty, 0, 7, GRASS) != LOSING) {
    return false;
  }
  // MORE TENTS THAN TREES
  if (game_check_move(g_empty, 0, 3, TENT) != LOSING) {
    return false;
  }
  // TENT WITH NO TREE AROUND
  if (game_check_move(g_empty, 5, 5, TENT) != LOSING) {
    return false;
  }

  game_delete(g_empty);
  return true;
}

bool test_game_check_move_2() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = game_default();
  assert(g);

  bool is_losing = false;
  uint nb_moves = 48000;
  uint i, j, s;
  while (nb_moves > 0) {
    i = (uint)(rand() % 8);
    j = (uint)(rand() % 8);
    s = (uint)(rand() % 4);
    if (game_get_square(g, i, j) == TREE || s == TREE) {
      if (game_check_move(g, i, j, s) != ILLEGAL) {
        return false;
      }
    } else {
      game_play_move(g, i, j, s);

      if (s == EMPTY) {
        if (game_check_move(g, i, j, s) != REGULAR) {
          return false;
        }
      }

      else if (s == GRASS) {
        //* LOSING case: too many GRASS: play GRASS on EMPTY / TENT / GRASS square
        // on EMPTY square
        if (game_get_square(g, i, j) == EMPTY) {
          if (((int)game_get_current_nb_emptys_row_test(g, i) - 1) <
              (int)(game_get_expected_nb_tents_row(g, i) -
                    game_get_current_nb_tents_row(g, i))) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          } else if (((int)game_get_current_nb_emptys_col_test(g, j) - 1) <
                     (int)(game_get_expected_nb_tents_col(g, j) -
                           game_get_current_nb_tents_col(g, j))) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          } else if (((int)game_get_current_nb_emptys_all_test(g) - 1) <
                     (int)(game_get_expected_nb_tents_all(g) -
                           game_get_current_nb_tents_all(g))) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          }
        }
        // on TENT square
        else if (game_get_square(g, i, j) == TENT) {
          if ((int)game_get_current_nb_emptys_row_test(g, i) <
              (int)(game_get_expected_nb_tents_row(g, i) -
                    game_get_current_nb_tents_row(g, i) + 1)) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          } else if ((int)game_get_current_nb_emptys_col_test(g, j) <
                     (int)(game_get_expected_nb_tents_col(g, j) -
                           game_get_current_nb_tents_col(g, j) + 1)) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          } else if ((int)game_get_current_nb_emptys_all_test(g) <
                     (int)(game_get_expected_nb_tents_all(g) -
                           game_get_current_nb_tents_all(g) + 1)) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          }
        }
        // on GRASS square
        else if (game_get_square(g, i, j) == GRASS) {
          if ((int)game_get_current_nb_emptys_row_test(g, i) <
              (int)(game_get_expected_nb_tents_row(g, i) -
                    game_get_current_nb_tents_row(g, i))) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          } else if ((int)game_get_current_nb_emptys_col_test(g, j) <
                     (int)(game_get_expected_nb_tents_col(g, j) -
                           game_get_current_nb_tents_col(g, j))) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          } else if ((int)game_get_current_nb_emptys_all_test(g) <
                     (int)(game_get_expected_nb_tents_all(g) -
                           game_get_current_nb_tents_all(g))) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          }
        }

        //* LOSING case: block a tree from receving a tent
        if ((int)(i - 1) >= 0) {
          if (game_get_square(g, i - 1, j) == TREE) {
            if (game_get_square(g, i, j) == EMPTY) {
              if (game_get_nb_adj_test(g, i - 1, j, EMPTY) == 1 &&
                  game_get_nb_adj_test(g, i - 1, j, TENT) == 0) {
                is_losing = true;
                if (game_check_move(g, i, j, s) != LOSING) {
                  return false;
                }
              }
            } else if (game_get_square(g, i, j) == TENT) {
              if (game_get_nb_adj_test(g, i - 1, j, EMPTY) == 0 &&
                  game_get_nb_adj_test(g, i - 1, j, TENT) == 1) {
                is_losing = true;
                if (game_check_move(g, i, j, s) != LOSING) {
                  return false;
                }
              }
            }
          }
        }
        if ((int)(j - 1) >= 0) {
          if (game_get_square(g, i, j - 1) == TREE) {
            if (game_get_square(g, i, j) == EMPTY) {
              if (game_get_nb_adj_test(g, i, j - 1, EMPTY) == 1 &&
                  game_get_nb_adj_test(g, i, j - 1, TENT) == 0) {
                is_losing = true;
                if (game_check_move(g, i, j, s) != LOSING) {
                  return false;
                }
              }
            } else if (game_get_square(g, i, j) == TENT) {
              if (game_get_nb_adj_test(g, i, j - 1, EMPTY) == 0 &&
                  game_get_nb_adj_test(g, i, j - 1, TENT) == 1) {
                is_losing = true;
                if (game_check_move(g, i, j, s) != LOSING) {
                  return false;
                }
              }
            }
          }
        }
        if (j + 1 < nb_cols) {
          if (game_get_square(g, i, j + 1) == TREE) {
            if (game_get_square(g, i, j) == EMPTY) {
              if (game_get_nb_adj_test(g, i, j + 1, EMPTY) == 1 &&
                  game_get_nb_adj_test(g, i, j + 1, TENT) == 0) {
                is_losing = true;
                if (game_check_move(g, i, j, s) != LOSING) {
                  return false;
                }
              }
            } else if (game_get_square(g, i, j) == TENT) {
              if (game_get_nb_adj_test(g, i, j + 1, EMPTY) == 0 &&
                  game_get_nb_adj_test(g, i, j + 1, TENT) == 1) {
                is_losing = true;
                if (game_check_move(g, i, j, s) != LOSING) {
                  return false;
                }
              }
            }
          }
        }
        if (i + 1 < nb_rows) {
          if (game_get_square(g, i + 1, j) == TREE) {
            if (game_get_square(g, i, j) == EMPTY) {
              if (game_get_nb_adj_test(g, i + 1, j, EMPTY) == 1 &&
                  game_get_nb_adj_test(g, i + 1, j, TENT) == 0) {
                is_losing = true;
                if (game_check_move(g, i, j, s) != LOSING) {
                  return false;
                }
              }
            } else if (game_get_square(g, i, j) == TENT) {
              if (game_get_nb_adj_test(g, i + 1, j, EMPTY) == 0 &&
                  game_get_nb_adj_test(g, i + 1, j, TENT) == 1) {
                is_losing = true;
                if (game_check_move(g, i, j, s) != LOSING) {
                  return false;
                }
              }
            }
          }
        }

      } else if (s == TENT) {
        //! LOSING IF TOO MANY TENTS

        // play TENT on EMPTY / GRASS
        if (game_get_square(g, i, j) == EMPTY ||
            game_get_square(g, i, j) == GRASS) {
          if (game_get_expected_nb_tents_row(g, i) <
                  (game_get_current_nb_tents_row(g, i) + 1) ||
              game_get_expected_nb_tents_col(g, j) <
                  (game_get_current_nb_tents_col(g, j) + 1) ||
              game_get_expected_nb_tents_all(g) <
                  (game_get_current_nb_tents_all(g) + 1)) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          }
        }
        // play TENT on TENT
        else if (game_get_square(g, i, j) == TENT) {
          if (game_get_expected_nb_tents_row(g, i) <
                  game_get_current_nb_tents_row(g, i) ||
              game_get_expected_nb_tents_col(g, j) <
                  game_get_current_nb_tents_col(g, j) ||
              game_get_expected_nb_tents_all(g) <
                  game_get_current_nb_tents_all(g)) {
            is_losing = true;
            if (game_check_move(g, i, j, s) != LOSING) {
              return false;
            }
          }
        }

        //! LOSING IF THERE ARE (DIAGONALLY) ADJACENT TENTS
        if ((game_get_nb_adj_test(g, i, j, TENT) +
             game_get_nb_tents_diag_adj_test(g, i, j)) > 0) {
          is_losing = true;
          if (game_check_move(g, i, j, s) != LOSING) {
            return false;
          }
        }

        //! LOSING IF THERE IS NO ADJACENT TREE
        if (game_get_nb_adj_test(g, i, j, TREE) == 0) {
          is_losing = true;
          if (game_check_move(g, i, j, s) != LOSING) {
            return false;
          }
        }

        if (game_check_move(g, i, j, s) != REGULAR && !is_losing) {
          return false;
        }
      }
    }

    nb_moves--;
  }

  game_delete(g);
  return true;
}

bool test_game_is_over() {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  game g = game_default();
  assert(g);

  // Only Rule 1 is true
  square squares_1[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, TENT, EMPTY, EMPTY, TREE, TREE, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      TENT, EMPTY, EMPTY, TENT, TREE, TENT, EMPTY, TENT,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
      TENT, TREE, TENT, EMPTY, TENT, EMPTY, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
      TENT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game_delete(g);
  g = game_new(squares_1, nb_tents_row_default, nb_tents_col_default);
  assert(g);
  if (game_is_over(g)) {
    return false;
  }

  // Only Rule 2,3 are true
  square squares_2[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, EMPTY, EMPTY, TENT, TREE, TREE, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      TENT, EMPTY, TENT, EMPTY, TREE, TENT, EMPTY, TENT,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
      TENT, TREE, EMPTY, TENT, TENT, EMPTY, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
      TENT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game_delete(g);
  g = game_new(squares_2, nb_tents_row_default, nb_tents_col_default);
  assert(g);
  if (game_is_over(g)) {
    return false;
  }

  // Only Rule 3 is true
  square squares_3[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, TENT, TENT, TENT, TREE, TREE, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TENT, TENT, TREE,
      EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
      EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game_delete(g);
  g = game_new(squares_3, nb_tents_row_default, nb_tents_col_default);
  assert(g);
  if (game_is_over(g)) {
    return false;
  }

  // Only Rule 4 is true
  square squares_4[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, TENT, TENT, TENT, TREE, TREE, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TENT, TENT, TREE,
      TENT, TENT, TENT, TENT, TREE, TENT, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TREE, TENT, TENT,
      TENT, TREE, TENT, TENT, TENT, TENT, TENT, TENT,
      TREE, TENT, TENT, TENT, TREE, TENT, TREE, TENT,
      TENT, TENT, TENT, TENT, TENT, TENT, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TENT, TENT, TENT};
  game_delete(g);
  g = game_new(squares_4, nb_tents_row_default, nb_tents_col_default);
  assert(g);
  if (game_is_over(g)) {
    return false;
  }

  // Only Rule 1 is false
  uint nb_tents_row_5[DEFAULT_SIZE] = {2, 1, 3, 1, 2, 1, 2, 0};
  uint nb_tents_col_5[DEFAULT_SIZE] = {1, 3, 0, 3, 0, 2, 2, 1};
  square squares_5[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TREE, TENT, EMPTY, EMPTY, EMPTY, EMPTY, TENT, TREE,
      EMPTY, EMPTY, EMPTY, TENT, TREE, EMPTY, EMPTY, EMPTY,
      TENT, EMPTY, EMPTY, TENT, TREE, EMPTY, EMPTY, TENT,
      TREE, EMPTY, EMPTY, EMPTY, TREE, TENT, EMPTY, TREE,
      EMPTY, TENT, TREE, TREE, EMPTY, EMPTY, TENT, TREE,
      EMPTY, EMPTY, EMPTY, TENT, EMPTY, TREE, EMPTY, EMPTY,
      EMPTY, TENT, EMPTY, EMPTY, EMPTY, TENT, EMPTY, EMPTY,
      EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game_delete(g);
  g = game_new(squares_5, nb_tents_row_5, nb_tents_col_5);
  assert(g);
  if (game_is_over(g)) {
    return false;
  }

  // Only Rule 2 is false
  square squares_6[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, EMPTY, EMPTY, TENT, TREE, TREE, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      TENT, EMPTY, EMPTY, TENT, TREE, TENT, EMPTY, TENT,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
      TENT, TREE, TENT, EMPTY, EMPTY, EMPTY, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
      TENT, EMPTY, EMPTY, EMPTY, TENT, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game_delete(g);
  g = game_new(squares_6, nb_tents_row_default, nb_tents_col_default);
  assert(g);
  if (game_is_over(g)) {
    return false;
  }

  // Only Rule 2,3 are false
  square squares_7[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, TENT, TREE, TREE, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      TENT, EMPTY, EMPTY, TENT, TREE, TENT, EMPTY, TENT,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
      TENT, TREE, TENT, EMPTY, EMPTY, EMPTY, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
      TENT, EMPTY, EMPTY, EMPTY, TENT, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game_delete(g);
  g = game_new(squares_7, nb_tents_row_default, nb_tents_col_default);
  assert(g);
  if (game_is_over(g)) {
    return false;
  }

  // Only Rule 4 is false
  uint nb_tents_row_8[DEFAULT_SIZE] = {0, 3, 0, 4, 0, 4, 0, 1};
  uint nb_tents_col_8[DEFAULT_SIZE] = {3, 1, 1, 2, 1, 1, 1, 2};
  square squares_8[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TENT, EMPTY, EMPTY, TENT, TREE, TREE, EMPTY, TENT,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TENT, TREE, TENT, EMPTY, TENT, EMPTY, TENT, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY, TREE,
      EMPTY, TENT, TREE, TENT, EMPTY, TENT, TREE, TENT,
      EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      TENT, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game_delete(g);
  g = game_new(squares_8, nb_tents_row_8, nb_tents_col_8);
  assert(g);
  if (game_is_over(g)) {
    return false;
  }

  // Game is already over with all grass
  square squares_9[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, GRASS, GRASS, TENT, TREE, TREE, TENT, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
      TENT, GRASS, GRASS, TENT, TREE, TENT, GRASS, TENT,
      TREE, GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS,
      TENT, TREE, TENT, GRASS, TENT, GRASS, TENT, GRASS,
      TREE, GRASS, GRASS, GRASS, TREE, GRASS, TREE, GRASS,
      TENT, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};
  game_delete(g);
  g = game_new(squares_9, nb_tents_row_default, nb_tents_col_default);
  assert(g);
  if (!game_is_over(g)) {
    return false;
  }

  // Game is already over with no grass
  square squares_10[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, EMPTY, EMPTY, TENT, TREE, TREE, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      TENT, EMPTY, EMPTY, TENT, TREE, TENT, EMPTY, TENT,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
      TENT, TREE, TENT, EMPTY, TENT, EMPTY, TENT, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
      TENT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  game_delete(g);
  g = game_new(squares_10, nb_tents_row_default, nb_tents_col_default);
  assert(g);
  if (!game_is_over(g)) {
    return false;
  }

  game_delete(g);
  return true;
}

bool test_game_fill_grass_row() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g1 = game_default();
  assert(g1);

  for (uint i = 0; i < nb_rows; i++) {
    game_fill_grass_row(g1, i);
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g1, i, j) == EMPTY) {
        return false;
      }
    }
  }

  game_delete(g1);
  g1 = game_default_solution();
  game g2 = game_default_solution();
  assert(g1 && g2);

  for (uint i = 0; i < nb_rows; i++) {
    game_fill_grass_row(g1, i);
  }
  if (!game_equal(g1, g2)) {
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_fill_grass_col() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g1 = game_default();
  assert(g1);

  for (uint j = 0; j < nb_cols; j++) {
    game_fill_grass_col(g1, j);
    for (uint i = 0; i < nb_rows; i++) {
      if (game_get_square(g1, i, j) == EMPTY) {
        return false;
      }
    }
  }

  game_delete(g1);
  g1 = game_default_solution();
  game g2 = game_default_solution();
  assert(g1 && g2);

  for (uint j = 0; j < nb_cols; j++) {
    game_fill_grass_row(g1, j);
  }
  if (!game_equal(g1, g2)) {
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_restart() {
  game g1 = game_default();
  game g2 = game_default();
  game g3 = game_default_solution();
  assert(g1 && g2 && g3);

  game_restart(g1);
  if (!game_equal(g1, g2)) {
    return false;
  }

  game_restart(g3);
  if (!game_equal(g2, g3)) {
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return true;
}

bool test_game_print() {
  game g = game_default();
  assert(g);
  game_print(g);
  assert(g);

  game_delete(g);
  return true;
}

bool test_game_default() {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  extern square squares_default[DEFAULT_SIZE * DEFAULT_SIZE];
  game g1 = game_default();
  game g2 = game_new(squares_default, nb_tents_row_default, nb_tents_col_default);
  assert(g1 && g2);

  if (!game_equal(g1, g2)) {
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_default_solution() {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  extern square squares_default_solution[DEFAULT_SIZE * DEFAULT_SIZE];
  game g1 = game_default_solution();
  game g2 = game_new(squares_default_solution, nb_tents_row_default, nb_tents_col_default);
  assert(g1 && g2);

  if (!game_equal(g1, g2)) {
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

//* Main
int main(int argc, char *argv[]) {
  if (argc == 1) {
    usage(argc, argv);
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;

  if (strcmp(argv[1], "game_new") == 0) {
    ok = test_game_new();
  } else if (strcmp(argv[1], "game_new_empty") == 0) {
    ok = test_game_new_empty();
  } else if (strcmp(argv[1], "game_copy") == 0) {
    ok = test_game_copy();
  } else if (strcmp(argv[1], "game_equal") == 0) {
    ok = test_game_equal();
  } else if (strcmp(argv[1], "game_set_square") == 0) {
    ok = test_game_set_square();
  } else if (strcmp(argv[1], "game_get_square") == 0) {
    ok = test_game_get_square();
  } else if (strcmp(argv[1], "game_set_expected_nb_tents_row") == 0) {
    ok = test_game_set_expected_nb_tents_row();
  } else if (strcmp(argv[1], "game_set_expected_nb_tents_col") == 0) {
    ok = test_game_set_expected_nb_tents_col();
  } else if (strcmp(argv[1], "game_get_expected_nb_tents_row") == 0) {
    ok = test_game_get_expected_nb_tents_row();
  } else if (strcmp(argv[1], "game_get_expected_nb_tents_col") == 0) {
    ok = test_game_get_expected_nb_tents_col();
  } else if (strcmp(argv[1], "game_get_expected_nb_tents_all") == 0) {
    ok = test_game_get_expected_nb_tents_all();
  } else if (strcmp(argv[1], "game_get_current_nb_tents_row") == 0) {
    ok = test_game_get_current_nb_tents_row();
  } else if (strcmp(argv[1], "game_get_current_nb_tents_col") == 0) {
    ok = test_game_get_current_nb_tents_col();
  } else if (strcmp(argv[1], "game_get_current_nb_tents_all") == 0) {
    ok = test_game_get_current_nb_tents_all();
  } else if (strcmp(argv[1], "game_play_move") == 0) {
    ok = test_game_play_move();
  } else if (strcmp(argv[1], "game_check_move_1") == 0) {
    ok = test_game_check_move_1();
  } else if (strcmp(argv[1], "game_check_move_2") == 0) {
    ok = test_game_check_move_2();
  } else if (strcmp(argv[1], "game_is_over") == 0) {
    ok = test_game_is_over();
  } else if (strcmp(argv[1], "game_fill_grass_row") == 0) {
    ok = test_game_fill_grass_row();
  } else if (strcmp(argv[1], "game_fill_grass_col") == 0) {
    ok = test_game_fill_grass_col();
  } else if (strcmp(argv[1], "game_restart") == 0) {
    ok = test_game_restart();
  } else if (strcmp(argv[1], "game_print") == 0) {
    ok = test_game_print();
  } else if (strcmp(argv[1], "game_default") == 0) {
    ok = test_game_default();
  } else if (strcmp(argv[1], "game_default_solution") == 0) {
    ok = test_game_default_solution();
  }

  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}