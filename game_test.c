#include "game.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "queue.h"

uint nb_tents_row_default_test[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
uint nb_tents_col_default_test[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};

square squares_default_test[DEFAULT_SIZE * DEFAULT_SIZE] = {
    EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY,  // row 1
    TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,   // row 2
    EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,  // row 3
    TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,  // row 4
    EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
    TREE,  EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY,  // row 6
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
    TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
};

square squares_default_test_solution[DEFAULT_SIZE * DEFAULT_SIZE] = {
    TENT, GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS,  // row 1
    TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,   // row 2
    TENT, GRASS, GRASS, TENT,  TREE,  TENT,  GRASS, TENT,   // row 3
    TREE, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS,  // row 4
    TENT, TREE,  TENT,  GRASS, TENT,  GRASS, TENT,  GRASS,  // row 5
    TREE, GRASS, GRASS, GRASS, TREE,  GRASS, TREE,  GRASS,  // row 6
    TENT, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 7
    TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS   // row 8
};

//* Usage

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> \n", argv[0]);
  exit(EXIT_FAILURE);
}

//* Tests

bool test_game_new() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = game_new(squares_default_test, nb_tents_row_default_test,
                    nb_tents_col_default_test);
  assert(g);

  if (game_nb_rows(g) != nb_rows || game_nb_cols(g) != nb_cols ||
      game_is_wrapping(g) || game_is_diagadj(g))
    return false;

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_default_test[i])
      return false;
  }
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_default_test[j])
      return false;
  }
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != squares_default_test[i * nb_cols + j])
        return false;
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

  if (game_nb_rows(g) != nb_rows || game_nb_cols(g) != nb_cols ||
      game_is_wrapping(g) || game_is_diagadj(g))
    return false;

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != 0) return false;
  }
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != 0) return false;
  }
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != EMPTY) return false;
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

  if (game_nb_rows(g1) != game_nb_rows(g2) ||
      game_nb_cols(g1) != game_nb_cols(g2) ||
      game_is_wrapping(g1) != game_is_wrapping(g2) ||
      game_is_diagadj(g1) != game_is_diagadj(g2)) {
    return false;
  }

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g1, i) !=
        game_get_expected_nb_tents_row(g2, i))
      return false;
  }
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g1, j) !=
        game_get_expected_nb_tents_col(g2, j))
      return false;
  }
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) return false;
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
      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY,  // row 1
      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,   // row 2
      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,  // row 3
      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,  // row 4
      EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
      TREE,  EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY,  // row 6
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
  };
  game g1 = game_default();
  game g2 = game_default();
  assert(g1 && g2);

  //* Totally Equal games
  if (!game_equal(g1, g2)) return false;

  //* Equal only nb_tents_row[]
  nb_tents_col[0] = 0;  // change default values of nb_tents_col[] and squares[]
  squares[0] = TREE;
  game_delete(g2);
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) return false;
  nb_tents_col[0] = 4;  // return default values
  squares[0] = EMPTY;

  //* Equal only nb_tents_col[]
  nb_tents_row[0] = 0;  // change default values of nb_tents_row[] and squares[]
  squares[0] = TREE;
  game_delete(g2);
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) return false;
  nb_tents_row[0] = 3;  // return default values
  squares[0] = EMPTY;

  //* Equal only nb_tents_row and nb_tents_all
  squares[0] = TREE;  // change default value of squares[]
  game_delete(g2);
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) return false;
  squares[0] = EMPTY;  // return default value

  //* Equal only squares
  nb_tents_row[0] = 0;  // change default values of nb_tents_row/col[]
  nb_tents_col[0] = 0;
  game_delete(g2);
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) return false;
  nb_tents_row[0] = 3;
  nb_tents_col[0] = 4;  // return default values

  //* Equal all but game_is_wrapping
  game_delete(g2);
  g2 = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares, nb_tents_row,
                    nb_tents_col, true, false);
  if (game_equal(g1, g2)) return false;

  //* Equal all but game_is_diagadj
  game_delete(g2);
  g2 = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares, nb_tents_row,
                    nb_tents_col, false, true);
  if (game_equal(g1, g2)) return false;
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_set_square() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  square squares_empty[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 3
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 4
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 6
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
  };
  game g1 = game_default();
  game g2 = game_default_solution();
  assert(g1 && g2);
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      game_set_square(g1, i, j, squares_default_test_solution[i * nb_cols + j]);
    }
  }
  if (!game_equal(g1, g2)) return false;
  game_delete(g1);

  g1 = game_new(squares_empty, nb_tents_row_default_test,
                nb_tents_col_default_test);
  assert(g1);
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      game_set_square(g2, i, j, EMPTY);
    }
  }
  if (!game_equal(g1, g2)) return false;
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_get_square() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = game_default_solution();
  assert(g);

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) !=
          squares_default_test_solution[i * nb_cols + j])
        return false;
    }
  }

  game_delete(g);
  g = game_new_empty();
  assert(g);
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != EMPTY) return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_game_set_expected_nb_tents_row() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_tents_row_new[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  game g1 = game_default();
  game g2 = game_new(squares_default_test, nb_tents_row_new,
                     nb_tents_col_default_test);
  assert(g1 && g2);

  for (uint i = 0; i < nb_rows; i++) {
    game_set_expected_nb_tents_row(g2, i, nb_tents_row_default_test[i]);
  }
  if (!game_equal(g1, g2)) return false;
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_set_expected_nb_tents_col() {
  uint nb_cols = DEFAULT_SIZE;
  uint nb_tents_col_new[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  game g1 = game_default();
  game g2 = game_new(squares_default_test, nb_tents_row_default_test,
                     nb_tents_col_new);
  assert(g1 && g2);

  for (uint j = 0; j < nb_cols; j++) {
    game_set_expected_nb_tents_col(g2, j, nb_tents_col_default_test[j]);
  }
  if (!game_equal(g1, g2)) return false;
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_get_expected_nb_tents_row() {
  uint nb_rows = DEFAULT_SIZE;
  game g = game_default();
  assert(g);

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_default_test[i])
      return false;
  }
  game_delete(g);
  return true;
}

bool test_game_get_expected_nb_tents_col() {
  uint nb_cols = DEFAULT_SIZE;
  game g = game_default();
  assert(g);
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_default_test[j])
      return false;
  }
  game_delete(g);
  return true;
}

bool test_game_get_expected_nb_tents_all() {
  game g = game_default();
  assert(g);
  if (game_get_expected_nb_tents_all(g) != 12) return false;
  game_delete(g);
  return true;
}

bool test_game_get_current_nb_tents_row() {
  uint nb_rows = DEFAULT_SIZE;
  game g = game_default();
  assert(g);
  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_current_nb_tents_row(g, i) != 0) return false;
  }
  game_delete(g);

  g = game_default_solution();
  assert(g);
  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_current_nb_tents_row(g, i) != nb_tents_row_default_test[i])
      return false;
  }
  game_delete(g);
  return true;
}

bool test_game_get_current_nb_tents_col() {
  uint nb_cols = DEFAULT_SIZE;
  game g = game_default();
  assert(g);
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_current_nb_tents_col(g, j) != 0) return false;
  }
  game_delete(g);

  g = game_default_solution();
  assert(g);
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_current_nb_tents_col(g, j) != nb_tents_col_default_test[j])
      return false;
  }
  game_delete(g);
  return true;
}

bool test_game_get_current_nb_tents_all() {
  game g = game_default();
  assert(g);
  if (game_get_current_nb_tents_all(g) != 0) return false;
  game_delete(g);

  g = game_default_solution();
  assert(g);
  if (game_get_current_nb_tents_all(g) != 12) return false;
  game_delete(g);
  return true;
}

bool test_game_play_move() {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  square squares_full_grass[DEFAULT_SIZE * DEFAULT_SIZE] = {
      GRASS, GRASS, GRASS, GRASS, TREE,  TREE,  GRASS, GRASS,  // row 1
      TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,   // row 2
      GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS,  // row 3
      TREE,  GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS,  // row 4
      GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 5
      TREE,  GRASS, GRASS, GRASS, TREE,  GRASS, TREE,  GRASS,  // row 6
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 7
      TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS   // row 8
  };
  square squares_full_tents[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, TENT, TENT, TENT, TREE, TREE, TENT, TENT,  // row 1
      TREE, TENT, TENT, TENT, TENT, TENT, TENT, TREE,  // row 2
      TENT, TENT, TENT, TENT, TREE, TENT, TENT, TENT,  // row 3
      TREE, TENT, TENT, TENT, TENT, TREE, TENT, TENT,  // row 4
      TENT, TREE, TENT, TENT, TENT, TENT, TENT, TENT,  // row 5
      TREE, TENT, TENT, TENT, TREE, TENT, TREE, TENT,  // row 6
      TENT, TENT, TENT, TENT, TENT, TENT, TENT, TENT,  // row 7
      TREE, TENT, TENT, TENT, TENT, TENT, TENT, TENT   // row 8
  };
  game g1 = game_default();
  assert(g1);

  // g1: play GRASS on all squares except TREE
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (squares_full_grass[i * nb_cols + j] != TREE)
        game_play_move(g1, i, j, GRASS);
    }
  }

  game g2 = game_new(squares_full_grass, nb_tents_row_default_test,
                     nb_tents_col_default_test);
  assert(g2);
  // compare g1 with game filled with GRASS
  if (!game_equal(g1, g2)) return false;

  // g1: play TENT on all squares except TREE
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (squares_full_grass[i * nb_cols + j] != TREE)
        game_play_move(g1, i, j, TENT);
    }
  }
  game_delete(g2);

  g2 = game_new(squares_full_tents, nb_tents_row_default_test,
                nb_tents_col_default_test);
  assert(g2);
  // compare g1 with game filled with TENT
  if (!game_equal(g1, g2)) return false;

  // g1: play EMPTY on all squares except TREE
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (squares_full_grass[i * nb_cols + j] != TREE)
        game_play_move(g1, i, j, EMPTY);
    }
  }
  game_delete(g2);

  g2 = game_default();
  assert(g2);
  // compare g1 with game_default
  if (!game_equal(g1, g2)) return false;

  // test free redo queue
  game_play_move(g2, 0, 0, TENT);
  game_undo(g2);
  game_play_move(g2, 0, 1, TENT);
  game_redo(g2);
  if (game_get_square(g2, 0, 0) == TENT) return false;

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_check_move() {
  game g = game_default();
  assert(g);

  if (game_check_move(g, 1, 0, TENT) != ILLEGAL ||
      game_check_move(g, 0, 0, TREE) != ILLEGAL)
    return false;
  if (game_check_move(g, 0, 0, EMPTY) != REGULAR) return false;
  game_delete(g);
  return true;
}

bool test_game_check_move_1() {
  square squares_1[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TREE, TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY,  // row 1
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 2
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 3
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 4
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 6
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
  };
  uint nb_tents_row_1[DEFAULT_SIZE] = {2, 2, 2, 2, 2, 2, 2, 2};
  uint nb_tents_col_1[DEFAULT_SIZE] = {2, 2, 2, 2, 2, 2, 2, 2};

  game g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                        nb_tents_col_1, false, false);
  assert(g);
  game_set_square(g, 0, 2, TENT);
  if (game_check_move(g, 1, 2, TENT) != LOSING ||
      game_check_move(g, 1, 3, TENT) != LOSING)
    return false;
  game_delete(g);

  g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                   nb_tents_col_1, false, true);
  assert(g);
  game_set_square(g, 0, 2, TENT);
  if (game_check_move(g, 1, 2, TENT) != LOSING ||
      game_check_move(g, 1, 3, TENT) != REGULAR)
    return false;
  game_delete(g);

  g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                   nb_tents_col_1, true, false);
  assert(g);
  game_set_square(g, 0, 2, TENT);
  if (game_check_move(g, 7, 2, TENT) != LOSING ||
      game_check_move(g, 7, 3, TENT) != LOSING)
    return false;
  game_delete(g);

  g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                   nb_tents_col_1, true, true);
  game_set_square(g, 0, 2, TENT);
  if (game_check_move(g, 7, 2, TENT) != LOSING ||
      game_check_move(g, 7, 3, TENT) != REGULAR)
    return false;
  game_delete(g);
  return true;
}

bool test_game_check_move_2() {
  game g = game_default();
  assert(g);

  if (game_check_move(g, 1, 5, TENT) != LOSING) return false;
  game_set_square(g, 1, 5, TENT);
  if (game_check_move(g, 1, 5, TENT) != LOSING) return false;
  game_set_square(g, 4, 4, TENT);
  if (game_check_move(g, 6, 4, TENT) != LOSING) return false;
  game_set_square(g, 6, 4, TENT);
  if (game_check_move(g, 6, 4, TENT) != LOSING) return false;
  game_delete(g);

  uint nb_tents_row_1[DEFAULT_SIZE] = {7, 0, 0, 0, 0, 0, 0, 0};
  uint nb_tents_col_1[DEFAULT_SIZE] = {7, 0, 0, 0, 0, 0, 0, 0};
  square squares_1[DEFAULT_SIZE * DEFAULT_SIZE] = {
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, EMPTY,  // row 1
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 2
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 3
      GRASS, GRASS, TREE,  TREE,  TREE,  TREE,  GRASS, GRASS,  // row 4
      GRASS, GRASS, TREE,  TREE,  TREE,  GRASS, GRASS, GRASS,  // row 5
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 6
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 7
      EMPTY, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, EMPTY   // row 8
  };

  // play GRASS on EMPTY
  g = game_new(squares_1, nb_tents_row_1, nb_tents_col_1);
  if (game_check_move(g, 0, 7, GRASS) != LOSING ||
      game_check_move(g, 7, 0, GRASS) != LOSING)
    return false;

  // play GRASS on TENT
  game_play_move(g, 0, 7, TENT);
  game_play_move(g, 7, 0, TENT);
  game_play_move(g, 7, 7, TENT);
  if (game_check_move(g, 0, 7, GRASS) != LOSING ||
      game_check_move(g, 7, 0, GRASS) != LOSING)
    return false;

  game_play_move(g, 0, 7, GRASS);
  game_play_move(g, 7, 0, GRASS);
  game_play_move(g, 7, 7, GRASS);
  if (game_check_move(g, 0, 7, GRASS) != LOSING ||
      game_check_move(g, 7, 0, GRASS) != LOSING)
    return false;
  game_delete(g);
  return true;
}

bool test_game_check_move_3() {
  game g = game_default();
  assert(g);
  uint nb_cols = game_nb_cols(g);

  for (uint i = 0; i < 2; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != TREE) game_set_square(g, i, j, TENT);
    }
  }
  if (game_check_move(g, 6, 0, TENT) != LOSING) return false;
  game_set_square(g, 6, 0, TENT);
  if (game_check_move(g, 6, 0, TENT) != LOSING) return false;
  game_delete(g);

  uint nb_tents_row_1[DEFAULT_SIZE] = {7, 0, 0, 0, 0, 0, 0, 0};
  uint nb_tents_col_1[DEFAULT_SIZE] = {7, 0, 0, 0, 0, 0, 0, 0};
  square squares_1[DEFAULT_SIZE * DEFAULT_SIZE] = {
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, EMPTY,  // row 1
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 2
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 3
      GRASS, GRASS, TREE,  TREE,  TREE,  TREE,  GRASS, GRASS,  // row 4
      GRASS, GRASS, TREE,  TREE,  TREE,  GRASS, GRASS, GRASS,  // row 5
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 6
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,  // row 7
      EMPTY, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, EMPTY   // row 8
  };

  // play GRASS on EMPTY
  g = game_new(squares_1, nb_tents_row_1, nb_tents_col_1);
  if (game_check_move(g, 7, 7, GRASS) != LOSING) return false;
  // play GRASS on TENT
  game_play_move(g, 7, 7, TENT);
  if (game_check_move(g, 7, 7, GRASS) != LOSING) return false;
  game_play_move(g, 7, 7, GRASS);
  if (game_check_move(g, 7, 7, GRASS) != LOSING) return false;

  game_delete(g);
  return true;
}

bool test_game_check_move_4() {
  game g = game_default();
  assert(g);
  // TENT
  if (game_check_move(g, 6, 5, TENT) != LOSING) return false;
  game_delete(g);

  square squares_1[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 3
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 4
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 6
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
  };
  uint nb_tents_row_1[DEFAULT_SIZE] = {1, 1, 1, 1, 1, 1, 1, 1};
  uint nb_tents_col_1[DEFAULT_SIZE] = {1, 1, 1, 1, 1, 1, 1, 1};
  g = game_new(squares_1, nb_tents_row_1, nb_tents_col_1);
  assert(g);
  // GRASS
  // i>1
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 0, 1, GRASS);
  if (game_check_move(g, 1, 0, GRASS) != LOSING) return false;
  game_set_square(g, 1, 0, TENT);
  if (game_check_move(g, 1, 0, GRASS) != LOSING) return false;

  // j>1
  game_set_square(g, 7, 0, TREE);
  game_set_square(g, 6, 0, GRASS);
  if (game_check_move(g, 7, 1, GRASS) != LOSING) return false;
  game_set_square(g, 7, 1, TENT);
  if (game_check_move(g, 7, 1, GRASS) != LOSING) return false;

  // j<max
  game_set_square(g, 0, 7, TREE);
  game_set_square(g, 1, 7, GRASS);
  if (game_check_move(g, 0, 6, GRASS) != LOSING) return false;
  game_set_square(g, 0, 6, TENT);
  if (game_check_move(g, 0, 6, GRASS) != LOSING) return false;

  // i<max
  game_set_square(g, 7, 7, TREE);
  game_set_square(g, 7, 6, GRASS);
  if (game_check_move(g, 6, 7, GRASS) != LOSING) return false;
  game_set_square(g, 6, 7, TENT);
  if (game_check_move(g, 6, 7, GRASS) != LOSING) return false;
  game_delete(g);

  g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                   nb_tents_col_1, true, false);
  assert(g);

  game_set_square(g, 1, 1, TREE);
  game_set_square(g, 1, 0, GRASS);
  game_set_square(g, 0, 1, GRASS);
  game_set_square(g, 1, 2, GRASS);
  if (game_check_move(g, 2, 1, GRASS) != LOSING) return false;
  game_set_square(g, 2, 1, TENT);
  if (game_check_move(g, 2, 1, GRASS) != LOSING) return false;

  game_set_square(g, 6, 1, TREE);
  game_set_square(g, 5, 1, GRASS);
  game_set_square(g, 6, 0, GRASS);
  game_set_square(g, 7, 1, GRASS);
  if (game_check_move(g, 6, 2, GRASS) != LOSING) return false;
  game_set_square(g, 6, 2, TENT);
  if (game_check_move(g, 6, 2, GRASS) != LOSING) return false;

  game_set_square(g, 6, 6, TREE);
  game_set_square(g, 6, 5, GRASS);
  game_set_square(g, 6, 7, GRASS);
  game_set_square(g, 7, 6, GRASS);
  if (game_check_move(g, 5, 6, GRASS) != LOSING) return false;
  game_set_square(g, 5, 6, TENT);
  if (game_check_move(g, 5, 6, GRASS) != LOSING) return false;

  game_set_square(g, 1, 6, TREE);
  game_set_square(g, 0, 6, GRASS);
  game_set_square(g, 1, 7, GRASS);
  game_set_square(g, 2, 6, GRASS);
  if (game_check_move(g, 1, 5, GRASS) != LOSING) return false;
  game_set_square(g, 1, 5, TENT);
  if (game_check_move(g, 1, 5, GRASS) != LOSING) return false;

  game_delete(g);
  g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                   nb_tents_col_1, true, false);
  assert(g);

  game_set_square(g, 7, 1, TREE);
  game_set_square(g, 6, 1, GRASS);
  game_set_square(g, 7, 0, GRASS);
  game_set_square(g, 7, 2, GRASS);
  if (game_check_move(g, 0, 1, GRASS) != LOSING) return false;
  game_set_square(g, 0, 1, TENT);
  if (game_check_move(g, 0, 1, GRASS) != LOSING) return false;

  game_set_square(g, 6, 7, TREE);
  game_set_square(g, 5, 7, GRASS);
  game_set_square(g, 6, 6, GRASS);
  game_set_square(g, 7, 7, GRASS);
  if (game_check_move(g, 6, 0, GRASS) != LOSING) return false;
  game_set_square(g, 6, 0, TENT);
  if (game_check_move(g, 6, 0, GRASS) != LOSING) return false;

  game_set_square(g, 1, 0, TREE);
  game_set_square(g, 0, 0, GRASS);
  game_set_square(g, 1, 1, GRASS);
  game_set_square(g, 2, 0, GRASS);
  if (game_check_move(g, 1, 7, GRASS) != LOSING) return false;
  game_set_square(g, 1, 7, TENT);
  if (game_check_move(g, 1, 7, GRASS) != LOSING) return false;

  game_set_square(g, 0, 6, TREE);
  game_set_square(g, 0, 5, GRASS);
  game_set_square(g, 0, 7, GRASS);
  game_set_square(g, 1, 6, GRASS);
  if (game_check_move(g, 7, 6, GRASS) != LOSING) return false;
  game_set_square(g, 7, 6, TENT);
  if (game_check_move(g, 7, 6, GRASS) != LOSING) return false;

  game_delete(g);
  return true;
}

bool test_game_is_over() {
  game g = game_default();
  assert(g);
  if (game_is_over(g)) return false;
  game_delete(g);
  return true;
}

bool test_game_is_over_1() {
  square squares_1[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TREE, TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY,  // row 1
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 2
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 3
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 4
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 6
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
      TREE, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
  };
  uint nb_tents_row_1[DEFAULT_SIZE] = {2, 2, 2, 2, 2, 2, 2, 2};
  uint nb_tents_col_1[DEFAULT_SIZE] = {2, 2, 2, 2, 2, 2, 2, 2};

  game g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                        nb_tents_col_1, false, false);
  assert(g);

  game_set_square(g, 0, 2, TENT);
  game_set_square(g, 1, 2, TENT);
  if (game_is_over(g)) return false;
  game_set_square(g, 1, 2, EMPTY);
  game_set_square(g, 1, 3, TENT);
  if (game_is_over(g)) return false;
  game_delete(g);

  g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                   nb_tents_col_1, false, true);
  game_set_square(g, 0, 2, TENT);
  game_set_square(g, 1, 3, TENT);
  if (game_is_over(g)) return false;
  game_delete(g);
  return true;
}

bool test_game_is_over_2() {
  square squares_empty[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 3
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 4
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 6
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
  };
  uint nb_tents_row_1[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint nb_tents_col_1[DEFAULT_SIZE] = {1, 0, 0, 0, 0, 0, 0, 0};
  uint nb_tents_row_2[DEFAULT_SIZE] = {1, 0, 0, 0, 0, 0, 0, 0};
  uint nb_tents_col_2[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  game g = game_new(squares_empty, nb_tents_row_1, nb_tents_col_1);
  if (game_is_over(g)) return false;
  game_delete(g);

  g = game_new(squares_empty, nb_tents_row_2, nb_tents_col_2);
  if (game_is_over(g)) return false;
  game_delete(g);

  g = game_default_solution();
  if (!game_is_over(g)) return false;
  game_delete(g);
  return true;
}

bool test_game_is_over_3() {
  square squares_empty[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 3
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 4
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 6
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
  };
  uint nb_tents_row[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint nb_tents_col[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};

  game g = game_new(squares_empty, nb_tents_row, nb_tents_col);
  assert(g);
  game_set_square(g, 0, 0, TREE);
  if (game_is_over(g)) return false;
  game_delete(g);
  return true;
}

bool test_game_is_over_4() {
  square squares_empty[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 3
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 4
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 6
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
  };
  uint nb_tents_row_1[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 1};
  uint nb_tents_col_1[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 1};
  uint nb_tents_row_2[DEFAULT_SIZE] = {1, 0, 0, 0, 0, 0, 0, 0};
  uint nb_tents_col_2[DEFAULT_SIZE] = {1, 0, 0, 0, 0, 0, 0, 0};

  game g = game_new(squares_empty, nb_tents_row_1, nb_tents_col_1);
  assert(g);
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 7, 7, TENT);
  if (game_is_over(g)) return false;
  game_delete(g);

  g = game_new(squares_empty, nb_tents_row_2, nb_tents_col_2);
  assert(g);
  game_set_square(g, 0, 0, TENT);
  game_set_square(g, 7, 7, TREE);
  if (game_is_over(g)) return false;
  game_delete(g);
  return true;
}

bool test_game_fill_grass_row() {
  uint nb_rows = DEFAULT_SIZE;
  game g1 = game_default();
  game g2 = game_default();
  assert(g1 && g2);

  game_fill_grass_row(g1, 0);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  if (!game_equal(g1, g2)) return false;
  game_delete(g1);
  game_delete(g2);

  g1 = game_default_solution();
  g2 = game_default_solution();
  assert(g1 && g2);
  for (uint i = 0; i < nb_rows; i++) game_fill_grass_row(g1, i);
  if (!game_equal(g1, g2)) return false;
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_fill_grass_col() {
  uint nb_cols = DEFAULT_SIZE;
  game g1 = game_default();
  game g2 = game_default();
  assert(g1 && g2);

  game_fill_grass_col(g1, 0);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  if (!game_equal(g1, g2)) return false;
  game_delete(g1);
  game_delete(g2);

  g1 = game_default_solution();
  g2 = game_default_solution();
  assert(g1 && g2);
  for (uint j = 0; j < nb_cols; j++) game_fill_grass_row(g1, j);
  if (!game_equal(g1, g2)) return false;
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
  if (!game_equal(g1, g2)) return false;
  game_restart(g3);
  if (!game_equal(g2, g3)) return false;
  game_play_move(g1, 0, 0, 3);
  game_restart(g1);
  game_undo(g1);
  if (!game_equal(g1, g2)) return false;

  game_play_move(g1, 0, 0, 3);
  game_undo(g1);
  game_restart(g1);
  game_redo(g1);
  if (!game_equal(g1, g2)) return false;

  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return true;
}

bool test_game_print() {
  game g = game_default_solution();
  assert(g);
  game_print(g);
  assert(g);
  game_delete(g);
  g = game_default();
  assert(g);
  game_print(g);
  assert(g);

  game_delete(g);
  return true;
}

bool test_game_default() {
  game g1 = game_default();
  game g2 = game_new(squares_default_test, nb_tents_row_default_test,
                     nb_tents_col_default_test);
  assert(g1 && g2);

  if (game_nb_rows(g1) != DEFAULT_SIZE || game_nb_cols(g1) != DEFAULT_SIZE ||
      game_is_wrapping(g1) || game_is_diagadj(g1))
    return false;
  if (!game_equal(g1, g2)) return false;

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_default_solution() {
  game g1 = game_default_solution();
  game g2 = game_new(squares_default_test_solution, nb_tents_row_default_test,
                     nb_tents_col_default_test);
  assert(g1 && g2);

  if (game_nb_rows(g1) != DEFAULT_SIZE || game_nb_cols(g1) != DEFAULT_SIZE ||
      game_is_wrapping(g1) || game_is_diagadj(g1))
    return false;
  if (!game_equal(g1, g2)) return false;

  game_delete(g1);
  game_delete(g2);
  return true;
}

//* Tests Ext

bool test_game_new_ext(void) {
  square squares[2 * 4] = {
      TREE,  EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, TREE,  EMPTY   // row 2
  };
  uint nb_tents_row[2] = {1, 1};
  uint nb_tents_col[4] = {0, 1, 0, 1};
  game g1 =
      game_new_ext(2, 4, squares, nb_tents_row, nb_tents_col, false, false);
  game g2 = game_new_empty_ext(2, 4, false, false);
  assert(g1 && g2);

  for (uint i = 0; i < 2; i++)
    game_set_expected_nb_tents_row(g2, i, nb_tents_row[i]);
  for (uint j = 0; j < 4; j++)
    game_set_expected_nb_tents_col(g2, j, nb_tents_col[j]);
  for (uint i = 0; i < 2; i++) {
    for (uint j = 0; j < 4; j++) {
      game_set_square(g2, i, j, squares[i * 4 + j]);
    }
  }

  if (game_nb_rows(g1) != 2 || game_nb_cols(g1) != 4 || game_is_wrapping(g1) ||
      game_is_diagadj(g1))
    return false;

  for (uint i = 0; i < 2; i++) {
    if (game_get_expected_nb_tents_row(g1, i) !=
        game_get_expected_nb_tents_row(g2, i))
      return false;
  }
  for (uint j = 0; j < 4; j++) {
    if (game_get_expected_nb_tents_col(g1, j) !=
        game_get_expected_nb_tents_col(g2, j))
      return false;
  }
  for (uint i = 0; i < 2; i++) {
    for (uint j = 0; j < 4; j++) {
      if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) return false;
    }
  }

  if (!(game_equal(g1, g2)) || game_nb_rows(g1) != game_nb_rows(g2) ||
      game_nb_cols(g1) != game_nb_cols(g2) ||
      game_is_diagadj(g1) != game_is_diagadj(g2) ||
      game_is_wrapping(g1) != game_is_wrapping(g2))
    return false;

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_new_empty_ext() {
  game g = game_new_empty_ext(8, 8, true, true);
  assert(g);
  uint nb_rows = 8;
  uint nb_cols = 8;
  bool diagadj = true;
  bool wrapping = true;

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != EMPTY) return false;
    }
  }
  if (game_nb_rows(g) != nb_rows || game_nb_cols(g) != nb_cols ||
      game_is_diagadj(g) != diagadj || game_is_wrapping(g) != wrapping)
    return false;

  game_delete(g);
  return true;
}

bool test_game_nb_rows() {
  square squares[2 * 4] = {
      TREE,  EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, TREE,  EMPTY   // row 2
  };
  uint nb_tents_row[2] = {1, 1};
  uint nb_tents_col[4] = {0, 1, 0, 1};
  game g =
      game_new_ext(2, 4, squares, nb_tents_row, nb_tents_col, false, false);
  if (game_nb_rows(g) != 2) return false;
  game_delete(g);
  return true;
}

bool test_game_nb_cols() {
  square squares[2 * 4] = {
      TREE,  EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, TREE,  EMPTY   // row 2
  };
  uint nb_tents_row[2] = {1, 1};
  uint nb_tents_col[4] = {0, 1, 0, 1};
  game g =
      game_new_ext(2, 4, squares, nb_tents_row, nb_tents_col, false, false);
  if (game_nb_cols(g) != 4) return false;
  game_delete(g);
  return true;
}

bool test_game_is_wrapping() {
  game g1 = game_default();
  game g2 = game_new_ext(8, 8, squares_default_test, nb_tents_row_default_test,
                         nb_tents_col_default_test, true, false);
  assert(g1 && g2);
  if (game_is_wrapping(g1)) return false;
  if (!game_is_wrapping(g2)) return false;
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_is_diagadj() {
  game g1 = game_default();
  game g2 = game_new_ext(8, 8, squares_default_test, nb_tents_row_default_test,
                         nb_tents_col_default_test, false, true);
  assert(g1 && g2);
  if (game_is_diagadj(g1)) return false;
  if (!game_is_diagadj(g2)) return false;
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_undo() {
  game g1 = game_default();
  game g2 = game_default();
  assert(g1 && g2);
  game_play_move(g1, 0, 0, TENT);
  game_undo(g1);
  if (!game_equal(g1, g2)) return false;

  game_fill_grass_row(g1, 0);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  if (!game_equal(g1, g2)) return false;

  game_fill_grass_col(g1, 0);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  if (!game_equal(g1, g2)) return false;

  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  game_undo(g1);
  if (!game_equal(g1, g2)) return false;

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_redo() {
  game g1 = game_default();
  game g2 = game_default();

  assert(g1 && g2);
  game_play_move(g1, 0, 0, TENT);
  game_play_move(g2, 0, 0, TENT);
  game_undo(g1);
  game_redo(g1);
  if (!game_equal(g1, g2)) return false;

  game_fill_grass_row(g1, 0);
  game_fill_grass_row(g2, 0);
  game_undo(g1);
  game_redo(g1);
  if (!game_equal(g1, g2)) return false;

  game_fill_grass_col(g1, 0);
  game_fill_grass_col(g2, 0);
  game_undo(g1);
  game_redo(g1);
  if (!game_equal(g1, g2)) return false;

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_load() {
  square squares_2x2[2 * 4] = {
      TREE,  EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, TREE,  EMPTY   // row 2
  };
  uint nb_tents_row_2x2[2] = {1, 1};
  uint nb_tents_col_2x2[4] = {0, 1, 0, 1};
  game g = game_load("./data/game_2x4.tnt");
  assert(g);
  if (game_nb_rows(g) != 2 || game_nb_cols(g) != 4 || game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 2; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_2x2[i])
      return false;
  }
  for (uint j = 0; j < 4; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_2x2[j])
      return false;
  }
  for (uint i = 0; i < 2; i++) {
    for (uint j = 0; j < 4; j++) {
      if (game_get_square(g, i, j) != squares_2x2[i * 4 + j]) return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_3x3d[3] = {1, 1, 0};
  uint nb_tents_col_3x3d[3] = {1, 1, 0};
  square squares_3x3d[3 * 3] = {
      TREE,  EMPTY, EMPTY,  // row 1
      EMPTY, TREE,  EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY   // row 3
  };
  g = game_load("./data/game_3x3d.tnt");
  assert(g);
  if (game_nb_rows(g) != 3 || game_nb_cols(g) != 3 || game_is_wrapping(g) ||
      !game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 3; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_3x3d[i])
      return false;
  }
  for (uint j = 0; j < 3; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_3x3d[j])
      return false;
  }
  for (uint i = 0; i < 3; i++) {
    for (uint j = 0; j < 3; j++) {
      if (game_get_square(g, i, j) != squares_3x3d[i * 3 + j]) return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_3x3w[3] = {1, 0, 0};
  uint nb_tents_col_3x3w[3] = {0, 0, 1};
  square squares_3x3w[3 * 3] = {
      TREE,  EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY   // row 3
  };
  g = game_load("./data/game_3x3w.tnt");
  assert(g);
  if (game_nb_rows(g) != 3 || game_nb_cols(g) != 3 || !game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 3; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_3x3w[i])
      return false;
  }
  for (uint j = 0; j < 3; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_3x3w[j])
      return false;
  }
  for (uint i = 0; i < 3; i++) {
    for (uint j = 0; j < 3; j++) {
      if (game_get_square(g, i, j) != squares_3x3w[i * 3 + j]) return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_3x3wd[3] = {1, 0, 1};
  uint nb_tents_col_3x3wd[3] = {1, 0, 1};
  square squares_3x3wd[3 * 3] = {
      EMPTY, EMPTY, TREE,   // row 1
      EMPTY, EMPTY, EMPTY,  // row 2
      TREE,  EMPTY, EMPTY   // row 3
  };
  g = game_load("./data/game_3x3wd.tnt");
  assert(g);
  if (game_nb_rows(g) != 3 || game_nb_cols(g) != 3 || !game_is_wrapping(g) ||
      !game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 3; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_3x3wd[i])
      return false;
  }
  for (uint j = 0; j < 3; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_3x3wd[j])
      return false;
  }
  for (uint i = 0; i < 3; i++) {
    for (uint j = 0; j < 3; j++) {
      if (game_get_square(g, i, j) != squares_3x3wd[i * 3 + j]) return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_4x4w[4] = {1, 1, 1, 1};
  uint nb_tents_col_4x4w[4] = {2, 0, 2, 0};
  square squares_4x4w[4 * 4] = {
      EMPTY, TREE,  EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY, TREE,   // row 2
      EMPTY, EMPTY, TREE,  TREE,   // row 3
      EMPTY, EMPTY, EMPTY, EMPTY   // row 4
  };
  g = game_load("./data/game_4x4w.tnt");
  assert(g);
  if (game_nb_rows(g) != 4 || game_nb_cols(g) != 4 || !game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 4; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_4x4w[i])
      return false;
  }
  for (uint j = 0; j < 4; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_4x4w[j])
      return false;
  }
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++) {
      if (game_get_square(g, i, j) != squares_4x4w[i * 4 + j]) return false;
    }
  }
  game_delete(g);

  g = game_load("./data/game_default.tnt");
  assert(g);
  if (game_nb_rows(g) != 8 || game_nb_cols(g) != 8 || game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 8; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_default_test[i])
      return false;
  }
  for (uint j = 0; j < 8; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_default_test[j])
      return false;
  }
  for (uint i = 0; i < 8; i++) {
    for (uint j = 0; j < 8; j++) {
      if (game_get_square(g, i, j) != squares_default_test[i * 8 + j])
        return false;
    }
  }
  game_delete(g);

  g = game_load("./data/game_default_solution.tnt");
  assert(g);
  if (game_nb_rows(g) != 8 || game_nb_cols(g) != 8 || game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 8; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_default_test[i])
      return false;
  }
  for (uint j = 0; j < 8; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_default_test[j])
      return false;
  }
  for (uint i = 0; i < 8; i++) {
    for (uint j = 0; j < 8; j++) {
      if (game_get_square(g, i, j) != squares_default_test_solution[i * 8 + j])
        return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_20x20[20] = {1, 1, 0, 0, 2, 2, 1, 1, 1, 3,
                                 0, 2, 0, 1, 1, 1, 1, 1, 1, 0};
  uint nb_tents_col_20x20[20] = {1, 1, 1, 0, 2, 1, 1, 2, 1, 0,
                                 2, 0, 0, 1, 3, 0, 0, 2, 1, 1};
  square squares_20x20[20 * 20] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 1
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 2
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 3
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 4
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE,  EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 5
      TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 6
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  // 7
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 8
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 9
      EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 10
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE,  EMPTY, EMPTY, EMPTY, TREE,  EMPTY,  // 11
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 12
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 13
      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY,  // 14
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 15
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,  // 16
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 17
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 18
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,  // 19
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};  // 20
  g = game_load("./data/random_20_20.tnt");
  assert(g);
  if (game_nb_rows(g) != 20 || game_nb_cols(g) != 20 || game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 20; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_20x20[i])
      return false;
  }
  for (uint j = 0; j < 20; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_20x20[j])
      return false;
  }
  for (uint i = 0; i < 20; i++) {
    for (uint j = 0; j < 20; j++) {
      if (game_get_square(g, i, j) != squares_20x20[i * 20 + j]) return false;
    }
  }
  game_delete(g);

  return true;
}

bool test_game_save() {
  square squares_2x2[2 * 4] = {
      TREE,  EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, TREE,  EMPTY   // row 2
  };
  uint nb_tents_row_2x2[2] = {1, 1};
  uint nb_tents_col_2x2[4] = {0, 1, 0, 1};
  game g = game_load("./data/game_2x4.tnt");
  assert(g);
  game_save(g, "./data/current.tnt");  // saved in build folder
  game_delete(g);
  g = game_load("./data/current.tnt");
  if (game_nb_rows(g) != 2 || game_nb_cols(g) != 4 || game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 2; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_2x2[i])
      return false;
  }
  for (uint j = 0; j < 4; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_2x2[j])
      return false;
  }
  for (uint i = 0; i < 2; i++) {
    for (uint j = 0; j < 4; j++) {
      if (game_get_square(g, i, j) != squares_2x2[i * 4 + j]) return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_3x3d[3] = {1, 1, 0};
  uint nb_tents_col_3x3d[3] = {1, 1, 0};
  square squares_3x3d[3 * 3] = {
      TREE,  EMPTY, EMPTY,  // row 1
      EMPTY, TREE,  EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY   // row 3
  };
  g = game_load("./data/game_3x3d.tnt");
  assert(g);
  game_save(g, "./data/current.tnt");  // saved in build folder
  game_delete(g);
  g = game_load("./data/current.tnt");
  if (game_nb_rows(g) != 3 || game_nb_cols(g) != 3 || game_is_wrapping(g) ||
      !game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 3; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_3x3d[i])
      return false;
  }
  for (uint j = 0; j < 3; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_3x3d[j])
      return false;
  }
  for (uint i = 0; i < 3; i++) {
    for (uint j = 0; j < 3; j++) {
      if (game_get_square(g, i, j) != squares_3x3d[i * 3 + j]) return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_3x3w[3] = {1, 0, 0};
  uint nb_tents_col_3x3w[3] = {0, 0, 1};
  square squares_3x3w[3 * 3] = {
      TREE,  EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY   // row 3
  };
  g = game_load("./data/game_3x3w.tnt");
  assert(g);
  game_save(g, "./data/current.tnt");  // saved in build folder
  game_delete(g);
  g = game_load("./data/current.tnt");
  if (game_nb_rows(g) != 3 || game_nb_cols(g) != 3 || !game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 3; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_3x3w[i])
      return false;
  }
  for (uint j = 0; j < 3; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_3x3w[j])
      return false;
  }
  for (uint i = 0; i < 3; i++) {
    for (uint j = 0; j < 3; j++) {
      if (game_get_square(g, i, j) != squares_3x3w[i * 3 + j]) return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_3x3wd[3] = {1, 0, 1};
  uint nb_tents_col_3x3wd[3] = {1, 0, 1};
  square squares_3x3wd[3 * 3] = {
      EMPTY, EMPTY, TREE,   // row 1
      EMPTY, EMPTY, EMPTY,  // row 2
      TREE,  EMPTY, EMPTY   // row 3
  };
  g = game_load("./data/game_3x3wd.tnt");
  assert(g);
  game_save(g, "./data/current.tnt");  // saved in build folder
  game_delete(g);
  g = game_load("./data/current.tnt");
  if (game_nb_rows(g) != 3 || game_nb_cols(g) != 3 || !game_is_wrapping(g) ||
      !game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 3; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_3x3wd[i])
      return false;
  }
  for (uint j = 0; j < 3; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_3x3wd[j])
      return false;
  }
  for (uint i = 0; i < 3; i++) {
    for (uint j = 0; j < 3; j++) {
      if (game_get_square(g, i, j) != squares_3x3wd[i * 3 + j]) return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_4x4w[4] = {1, 1, 1, 1};
  uint nb_tents_col_4x4w[4] = {2, 0, 2, 0};
  square squares_4x4w[4 * 4] = {
      EMPTY, TREE,  EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY, TREE,   // row 2
      EMPTY, EMPTY, TREE,  TREE,   // row 3
      EMPTY, EMPTY, EMPTY, EMPTY   // row 4
  };
  g = game_load("./data/game_4x4w.tnt");
  assert(g);
  game_save(g, "./data/current.tnt");  // saved in build folder
  game_delete(g);
  g = game_load("./data/current.tnt");
  if (game_nb_rows(g) != 4 || game_nb_cols(g) != 4 || !game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 4; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_4x4w[i])
      return false;
  }
  for (uint j = 0; j < 4; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_4x4w[j])
      return false;
  }
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++) {
      if (game_get_square(g, i, j) != squares_4x4w[i * 4 + j]) return false;
    }
  }
  game_delete(g);

  g = game_load("./data/game_default.tnt");
  assert(g);
  game_save(g, "./data/current.tnt");  // saved in build folder
  game_delete(g);
  g = game_load("./data/current.tnt");
  if (game_nb_rows(g) != 8 || game_nb_cols(g) != 8 || game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 8; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_default_test[i])
      return false;
  }
  for (uint j = 0; j < 8; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_default_test[j])
      return false;
  }
  for (uint i = 0; i < 8; i++) {
    for (uint j = 0; j < 8; j++) {
      if (game_get_square(g, i, j) != squares_default_test[i * 8 + j])
        return false;
    }
  }
  game_delete(g);

  g = game_load("./data/game_default_solution.tnt");
  assert(g);
  game_save(g, "./data/current.tnt");  // saved in build folder
  game_delete(g);
  g = game_load("./data/current.tnt");
  if (game_nb_rows(g) != 8 || game_nb_cols(g) != 8 || game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 8; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_default_test[i])
      return false;
  }
  for (uint j = 0; j < 8; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_default_test[j])
      return false;
  }
  for (uint i = 0; i < 8; i++) {
    for (uint j = 0; j < 8; j++) {
      if (game_get_square(g, i, j) != squares_default_test_solution[i * 8 + j])
        return false;
    }
  }
  game_delete(g);

  uint nb_tents_row_20x20[20] = {1, 1, 0, 0, 2, 2, 1, 1, 1, 3,
                                 0, 2, 0, 1, 1, 1, 1, 1, 1, 0};
  uint nb_tents_col_20x20[20] = {1, 1, 1, 0, 2, 1, 1, 2, 1, 0,
                                 2, 0, 0, 1, 3, 0, 0, 2, 1, 1};
  square squares_20x20[20 * 20] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 1
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 2
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 3
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 4
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE,  EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 5
      TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 6
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  // 7
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 8
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 9
      EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 10
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE,  EMPTY, EMPTY, EMPTY, TREE,  EMPTY,  // 11
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 12
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 13
      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY,  // 14
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 15
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,  // 16
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 17
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // 18
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,  // 19
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  g = game_load("./data/random_20_20.tnt");
  assert(g);
  game_save(g, "./data/current.tnt");  // saved in build folder
  game_delete(g);
  g = game_load("./data/current.tnt");
  if (game_nb_rows(g) != 20 || game_nb_cols(g) != 20 || game_is_wrapping(g) ||
      game_is_diagadj(g))
    return false;
  for (uint i = 0; i < 20; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row_20x20[i])
      return false;
  }
  for (uint j = 0; j < 20; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col_20x20[j])
      return false;
  }
  for (uint i = 0; i < 20; i++) {
    for (uint j = 0; j < 20; j++) {
      if (game_get_square(g, i, j) != squares_20x20[i * 20 + j]) return false;
    }
  }
  game_delete(g);

  return true;
}

bool test_game_extra() {
  square squares_1[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 1
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 2
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 3
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 4
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 5
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 6
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  // row 7
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY   // row 8
  };
  uint nb_tents_row_1[DEFAULT_SIZE] = {7, 7, 7, 7, 7, 7, 7, 7};
  uint nb_tents_col_1[DEFAULT_SIZE] = {7, 7, 7, 7, 7, 7, 7, 7};
  game g1 = game_new(squares_1, nb_tents_row_1, nb_tents_col_1);

  game_set_square(g1, 0, 0, TENT);
  game_set_square(g1, 0, 7, TENT);
  game_set_square(g1, 7, 0, TENT);
  game_set_square(g1, 7, 7, TENT);
  if (game_check_move(g1, 1, 1, TENT) != LOSING) return false;
  if (game_check_move(g1, 1, 6, TENT) != LOSING) return false;
  if (game_check_move(g1, 6, 1, TENT) != LOSING) return false;
  if (game_check_move(g1, 6, 6, TENT) != LOSING) return false;
  game_delete(g1);

  // wrapping
  g1 = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                    nb_tents_col_1, true, false);
  if (game_check_move(g1, 0, 0, TENT) != LOSING) return false;
  game_set_square(g1, 7, 7, TENT);
  if (game_check_move(g1, 0, 0, TENT) != LOSING) return false;
  game_set_square(g1, 0, 7, TENT);
  if (game_check_move(g1, 1, 0, TENT) != LOSING) return false;
  game_delete(g1);

  g1 = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                    nb_tents_col_1, true, false);
  if (game_check_move(g1, 7, 7, TENT) != LOSING) return false;
  game_set_square(g1, 0, 0, TENT);
  if (game_check_move(g1, 7, 7, TENT) != LOSING) return false;
  game_set_square(g1, 7, 0, TENT);
  if (game_check_move(g1, 6, 7, TENT) != LOSING) return false;
  game_delete(g1);

  g1 = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                    nb_tents_col_1, true, false);
  if (game_check_move(g1, 7, 0, TENT) != LOSING) return false;
  game_set_square(g1, 0, 7, TENT);
  if (game_check_move(g1, 7, 0, TENT) != LOSING) return false;
  game_set_square(g1, 0, 0, TENT);
  if (game_check_move(g1, 7, 1, TENT) != LOSING) return false;

  game_delete(g1);
  g1 = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares_1, nb_tents_row_1,
                    nb_tents_col_1, true, false);
  if (game_check_move(g1, 0, 7, TENT) != LOSING) return false;
  game_set_square(g1, 7, 0, TENT);
  if (game_check_move(g1, 0, 7, TENT) != LOSING) return false;
  game_set_square(g1, 7, 7, TENT);
  if (game_check_move(g1, 0, 6, TENT) != LOSING) return false;

  game_delete(g1);
  return true;
}

//* Main
int main(int argc, char *argv[]) {
  if (argc == 1) {
    usage(argc, argv);
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp(argv[1], "dummy") == 0) {
    ok = true;
  } else if (strcmp(argv[1], "game_new") == 0) {
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
  } else if (strcmp(argv[1], "game_check_move") == 0) {
    ok = test_game_check_move();
  } else if (strcmp(argv[1], "game_check_move_1") == 0) {
    ok = test_game_check_move_1();
  } else if (strcmp(argv[1], "game_check_move_2") == 0) {
    ok = test_game_check_move_2();
  } else if (strcmp(argv[1], "game_check_move_3") == 0) {
    ok = test_game_check_move_3();
  } else if (strcmp(argv[1], "game_check_move_4") == 0) {
    ok = test_game_check_move_4();
  } else if (strcmp(argv[1], "game_is_over") == 0) {
    ok = test_game_is_over();
  } else if (strcmp(argv[1], "game_is_over_1") == 0) {
    ok = test_game_is_over_1();
  } else if (strcmp(argv[1], "game_is_over_2") == 0) {
    ok = test_game_is_over_2();
  } else if (strcmp(argv[1], "game_is_over_3") == 0) {
    ok = test_game_is_over_3();
  } else if (strcmp(argv[1], "game_is_over_4") == 0) {
    ok = test_game_is_over_4();
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
  // ext
  else if (strcmp(argv[1], "game_new_ext") == 0) {
    ok = test_game_new_ext();
  } else if (strcmp(argv[1], "game_new_empty_ext") == 0) {
    ok = test_game_new_empty_ext();
  } else if (strcmp(argv[1], "game_nb_rows") == 0) {
    ok = test_game_nb_rows();
  } else if (strcmp(argv[1], "game_nb_cols") == 0) {
    ok = test_game_nb_cols();
  } else if (strcmp(argv[1], "game_is_wrapping") == 0) {
    ok = test_game_is_wrapping();
  } else if (strcmp(argv[1], "game_is_diagadj") == 0) {
    ok = test_game_is_diagadj();
  } else if (strcmp(argv[1], "game_undo") == 0) {
    ok = test_game_undo();
  } else if (strcmp(argv[1], "game_redo") == 0) {
    ok = test_game_redo();
  }
  // tools
  else if (strcmp(argv[1], "game_load") == 0) {
    ok = test_game_load();
  } else if (strcmp(argv[1], "game_save") == 0) {
    ok = test_game_save();
  }
  // extra
  else if (strcmp(argv[1], "game_extra") == 0) {
    ok = test_game_extra();
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