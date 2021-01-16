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

//* Aux Test Functions

uint game_get_current_nb_emptys_row(cgame g, uint i) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(g && i >= 0 && i < nb_rows);
  uint count = 0;

  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      count++;
    }
  }
  return count;
}

uint game_get_current_nb_emptys_col(cgame g, uint j) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(g && j >= 0 && j < nb_cols);
  uint count = 0;

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_square(g, i, j) == EMPTY) {
      count++;
    }
  }
  return count;
}

uint game_get_current_nb_emptys_all(cgame g) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(g);
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

uint game_get_current_nb_trees_all(cgame g) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(g);
  uint count = 0;
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) == TREE) {
        count++;
      }
    }
  }
  return count;
}

uint game_get_nb_adjacent_tents(cgame g, uint i, uint j) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(g && i >= 0 && i < nb_rows && j >= 0 && j < nb_cols);
  uint count = 0;

  // corners
  if (i == 0 && j == 0) {
    if (game_get_square(g, i + 1, j) == TENT) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TENT) {
      count++;
    }
  } else if (i == 0 && j == (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TENT) {
      count++;
    }
  } else if (i == (nb_rows - 1) && j == 0) {
    if (game_get_square(g, i - 1, j) == TENT) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TENT) {
      count++;
    }
  } else if (i == (nb_rows - 1) && j == (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i - 1, j) == TENT) {
      count++;
    }
  }

  // edges except corners
  else if (i == 0 && 0 < j && j < (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TENT) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TENT) {
      count++;
    }
  } else if (i == (nb_rows - 1) && 0 < j && j < (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TENT) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TENT) {
      count++;
    }
  } else if (0 < i && i < (nb_rows - 1) && j == 0) {
    if (game_get_square(g, i - 1, j) == TENT) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TENT) {
      count++;
    }
  } else if (0 < i && i < (nb_rows - 1) && j == (nb_cols - 1)) {
    if (game_get_square(g, i - 1, j) == TENT) {
      count++;
    }
    if (game_get_square(g, i, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TENT) {
      count++;
    }
  }

  // other cases
  else {
    if (game_get_square(g, i - 1, j) == TENT) {
      count++;
    }
    if (game_get_square(g, i, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TENT) {
      count++;
    }
  }

  return count;
}

uint game_get_nb_adjacent_trees(cgame g, uint i, uint j) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(g && i >= 0 && i < nb_rows && j >= 0 && j < nb_cols);
  uint count = 0;

  // corners
  if (i == 0 && j == 0) {
    if (game_get_square(g, i + 1, j) == TREE) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TREE) {
      count++;
    }
  } else if (i == 0 && j == (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == TREE) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TREE) {
      count++;
    }
  } else if (i == (nb_rows - 1) && j == 0) {
    if (game_get_square(g, i - 1, j) == TREE) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TREE) {
      count++;
    }
  } else if (i == (nb_rows - 1) && j == (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == TREE) {
      count++;
    }
    if (game_get_square(g, i - 1, j) == TREE) {
      count++;
    }
  }

  // edges except corners
  else if (i == 0 && 0 < j && j < (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == TREE) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TREE) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TREE) {
      count++;
    }
  } else if (i == (nb_rows - 1) && 0 < j && j < (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == TREE) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TREE) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TREE) {
      count++;
    }
  } else if (0 < i && i < (nb_rows - 1) && j == 0) {
    if (game_get_square(g, i - 1, j) == TREE) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TREE) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TREE) {
      count++;
    }
  } else if (0 < i && i < (nb_rows - 1) && j == (nb_cols - 1)) {
    if (game_get_square(g, i - 1, j) == TREE) {
      count++;
    }
    if (game_get_square(g, i, j - 1) == TREE) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TREE) {
      count++;
    }
  }

  // other cases
  else {
    if (game_get_square(g, i - 1, j) == TREE) {
      count++;
    }
    if (game_get_square(g, i, j - 1) == TREE) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == TREE) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == TREE) {
      count++;
    }
  }

  return count;
}

uint game_get_nb_adjacent_emptys(cgame g, uint i, uint j) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(g && i >= 0 && i < nb_rows && j >= 0 && j < nb_cols);
  uint count = 0;

  // corners
  if (i == 0 && j == 0) {
    if (game_get_square(g, i + 1, j) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == EMPTY) {
      count++;
    }
  } else if (i == 0 && j == (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == EMPTY) {
      count++;
    }
  } else if (i == (nb_rows - 1) && j == 0) {
    if (game_get_square(g, i - 1, j) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == EMPTY) {
      count++;
    }
  } else if (i == (nb_rows - 1) && j == (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i - 1, j) == EMPTY) {
      count++;
    }
  }

  // edges except corners
  else if (i == 0 && 0 < j && j < (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == EMPTY) {
      count++;
    }
  } else if (i == (nb_rows - 1) && 0 < j && j < (nb_cols - 1)) {
    if (game_get_square(g, i, j - 1) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == EMPTY) {
      count++;
    }
  } else if (0 < i && i < (nb_rows - 1) && j == 0) {
    if (game_get_square(g, i - 1, j) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == EMPTY) {
      count++;
    }
  } else if (0 < i && i < (nb_rows - 1) && j == (nb_cols - 1)) {
    if (game_get_square(g, i - 1, j) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i, j - 1) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == EMPTY) {
      count++;
    }
  }

  // other cases
  else {
    if (game_get_square(g, i - 1, j) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i, j - 1) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i, j + 1) == EMPTY) {
      count++;
    }
    if (game_get_square(g, i + 1, j) == EMPTY) {
      count++;
    }
  }

  return count;
}

uint game_get_nb_diag_adjacent_tents(cgame g, uint i, uint j) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  assert(g && i >= 0 && i < nb_rows && j >= 0 && j < nb_cols);
  uint count = 0;

  // corners
  if (i == 0 && j == 0) {
    if (game_get_square(g, i + 1, j + 1) == TENT) {
      count++;
    }
  } else if (i == 0 && j == (nb_cols - 1)) {
    if (game_get_square(g, i + 1, j - 1) == TENT) {
      count++;
    }
  } else if (i == (nb_rows - 1) && j == 0) {
    if (game_get_square(g, i - 1, j + 1) == TENT) {
      count++;
    }
  } else if (i == (nb_rows - 1) && j == (nb_cols - 1)) {
    if (game_get_square(g, i - 1, j - 1) == TENT) {
      count++;
    }
  }

  // edges except corners
  else if (i == 0 && 0 < j && j < (nb_cols - 1)) {
    if (game_get_square(g, i + 1, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j + 1) == TENT) {
      count++;
    }
  } else if (i == (nb_rows - 1) && 0 < j && j < (nb_cols - 1)) {
    if (game_get_square(g, i - 1, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i - 1, j + 1) == TENT) {
      count++;
    }
  } else if (0 < i && i < (nb_rows - 1) && j == 0) {
    if (game_get_square(g, i - 1, j + 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j + 1) == TENT) {
      count++;
    }
  } else if (0 < i && i < (nb_rows - 1) && j == (nb_cols - 1)) {
    if (game_get_square(g, i - 1, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j - 1) == TENT) {
      count++;
    }
  }

  // other cases
  else {
    if (game_get_square(g, i - 1, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i - 1, j + 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j - 1) == TENT) {
      count++;
    }
    if (game_get_square(g, i + 1, j + 1) == TENT) {
      count++;
    }
  }

  return count;
}

//* Tests

bool test_game_new() {
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

  game g = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g);

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row[i] ||
        game_get_expected_nb_tents_col(g, i) != nb_tents_col[i]) {
      return false;
    }
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != squares[i * DEFAULT_SIZE + j]) {
        return false;
      }
    }
  }

  game_delete(g);
  return true;
}

bool test_game_new_empty() {
  game g = game_new_empty();
  assert(g);

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_row(g, i) != 0 ||
        game_get_expected_nb_tents_col(g, i) != 0) {
      return false;
    }
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != EMPTY) {
        return false;
      }
    }
  }

  game_delete(g);
  return true;
}

bool test_game_copy() {
  game g1 = game_default();
  game g2 = game_copy(g1);
  assert(g1 && g2);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;

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
  // Equal games
  game g1 = game_default();
  game g2 = game_default();
  assert(g1 && g2);

  if (!game_equal(g1, g2)) {
    return false;
  }

  // Equal only nb_tents_row
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

  nb_tents_col[0] = 0;
  squares[0] = TREE;
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) {
    return false;
  }
  nb_tents_col[0] = 4;

  // Equal only nb_tents_col
  nb_tents_row[0] = 0;
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) {
    return false;
  }
  nb_tents_row[0] = 3;

  // Equal only nb_tents_row and nb_tents_all
  g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);
  if (game_equal(g1, g2)) {
    return false;
  }
  squares[0] = EMPTY;

  // Equal only squares
  nb_tents_row[0] = 0;
  nb_tents_col[0] = 0;
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
  game g1 = game_default();
  game g2 = game_default_solution();
  assert(g1 && g2);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, GRASS, GRASS, TENT, TREE, TREE, TENT, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
      TENT, GRASS, GRASS, TENT, TREE, TENT, GRASS, TENT,
      TREE, GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS,
      TENT, TREE, TENT, GRASS, TENT, GRASS, TENT, GRASS,
      TREE, GRASS, GRASS, GRASS, TREE, GRASS, TREE, GRASS,
      TENT, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      game_set_square(g1, i, j, squares[i * nb_cols + j]);
    }
  }

  if (!game_equal(g1, g2)) {
    return false;
  }

  uint nb_tents_row[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
  uint nb_tents_col[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
  square squares_empty[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  g1 = game_new(squares_empty, nb_tents_row, nb_tents_col);
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
  game g = game_default_solution();
  assert(g);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, GRASS, GRASS, TENT, TREE, TREE, TENT, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
      TENT, GRASS, GRASS, TENT, TREE, TENT, GRASS, TENT,
      TREE, GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS,
      TENT, TREE, TENT, GRASS, TENT, GRASS, TENT, GRASS,
      TREE, GRASS, GRASS, GRASS, TREE, GRASS, TREE, GRASS,
      TENT, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != squares[i * nb_cols + j]) {
        return false;
      }
    }
  }

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
  game g1 = game_default();
  assert(g1);
  uint nb_rows = DEFAULT_SIZE;

  uint nb_tents_row[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
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

  game g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);

  uint nb_tents_row_new[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
  for (uint i = 0; i < nb_rows; i++) {
    game_set_expected_nb_tents_row(g2, i, nb_tents_row_new[i]);
  }

  if (!game_equal(g1, g2)) {
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_set_expected_nb_tents_col() {
  game g1 = game_default();
  assert(g1);
  uint nb_cols = DEFAULT_SIZE;

  uint nb_tents_row[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
  uint nb_tents_col[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
  square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
      EMPTY, EMPTY, EMPTY, EMPTY, TREE, TREE, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
      EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
      EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};

  game g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);

  uint nb_tents_col_new[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
  for (uint j = 0; j < nb_cols; j++) {
    game_set_expected_nb_tents_col(g2, j, nb_tents_col_new[j]);
  }

  if (!game_equal(g1, g2)) {
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_get_expected_nb_tents_row() {
  game g = game_default();
  assert(g);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_tents_row[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row[i]) {
      return false;
    }
  }

  game_delete(g);
  return true;
}

bool test_game_get_expected_nb_tents_col() {
  game g = game_default();
  assert(g);
  uint nb_cols = DEFAULT_SIZE;
  uint nb_tents_col[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};

  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col[j]) {
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
  game g = game_default();
  assert(g);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_tents_row[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_current_nb_tents_row(g, i) != 0) {
      return false;
    }
  }

  g = game_default_solution();
  assert(g);

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_current_nb_tents_row(g, i) != nb_tents_row[i]) {
      return false;
    }
  }

  game_delete(g);
  return true;
}

bool test_game_get_current_nb_tents_col() {
  game g = game_default();
  assert(g);
  uint nb_cols = DEFAULT_SIZE;
  uint nb_tents_col[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};

  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_current_nb_tents_col(g, j) != 0) {
      return false;
    }
  }

  g = game_default_solution();
  assert(g);

  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_current_nb_tents_col(g, j) != nb_tents_col[j]) {
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

  g = game_default_solution();
  assert(g);

  if (game_get_current_nb_tents_all(g) != 12) {
    return false;
  }

  game_delete(g);
  return true;
}

bool test_game_play_move() {
  game g1 = game_default();
  assert(g1);
  uint nb_tents_row[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
  uint nb_tents_col[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  square squares_grass[DEFAULT_SIZE * DEFAULT_SIZE] = {
      GRASS, GRASS, GRASS, GRASS, TREE, TREE, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
      GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS,
      GRASS, TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, TREE, GRASS, TREE, GRASS,
      GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};
  square squares_tent[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, TENT, TENT, TENT, TREE, TREE, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TENT, TENT, TREE,
      TENT, TENT, TENT, TENT, TREE, TENT, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TREE, TENT, TENT,
      TENT, TREE, TENT, TENT, TENT, TENT, TENT, TENT,
      TREE, TENT, TENT, TENT, TREE, TENT, TREE, TENT,
      TENT, TENT, TENT, TENT, TENT, TENT, TENT, TENT,
      TREE, TENT, TENT, TENT, TENT, TENT, TENT, TENT};

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (squares_grass[i * nb_cols + j] != TREE) {
        game_play_move(g1, i, j, GRASS);
      }
    }
  }
  game g2 = game_new(squares_grass, nb_tents_row, nb_tents_col);
  assert(g2);
  if (!game_equal(g1, g2)) {
    return false;
  }

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (squares_grass[i * nb_cols + j] != TREE) {
        game_play_move(g1, i, j, TENT);
      }
    }
  }
  g2 = game_new(squares_tent, nb_tents_row, nb_tents_col);
  assert(g2);
  if (!game_equal(g1, g2)) {
    return false;
  }

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (squares_grass[i * nb_cols + j] != TREE) {
        game_play_move(g1, i, j, EMPTY);
      }
    }
  }
  g2 = game_default();
  assert(g2);
  if (!game_equal(g1, g2)) {
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_check_move() { return true; }
bool test_game_is_over() { return true; }

bool test_game_fill_grass_row() {
  game g1 = game_default();
  assert(g1);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;

  for (uint i = 0; i < nb_rows; i++) {
    game_fill_grass_row(g1, i);
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g1, i, j) == EMPTY) {
        return false;
      }
    }
  }

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
  game g1 = game_default();
  assert(g1);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;

  for (uint j = 0; j < nb_cols; j++) {
    game_fill_grass_col(g1, j);
    for (uint i = 0; i < nb_rows; i++) {
      if (game_get_square(g1, i, j) == EMPTY) {
        return false;
      }
    }
  }

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
  game g1 = game_default();
  assert(g1);
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

  game g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);

  if (!game_equal(g1, g2)) {
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_default_solution() {
  game g1 = game_default_solution();
  assert(g1);
  uint nb_tents_row[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
  uint nb_tents_col[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
  square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TENT, GRASS, GRASS, TENT, TREE, TREE, TENT, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
      TENT, GRASS, GRASS, TENT, TREE, TENT, GRASS, TENT,
      TREE, GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS,
      TENT, TREE, TENT, GRASS, TENT, GRASS, TENT, GRASS,
      TREE, GRASS, GRASS, GRASS, TREE, GRASS, TREE, GRASS,
      TENT, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
      TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};

  game g2 = game_new(squares, nb_tents_row, nb_tents_col);
  assert(g2);

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
  } else if (strcmp(argv[1], "game_check_move") == 0) {
    ok = test_game_check_move();
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