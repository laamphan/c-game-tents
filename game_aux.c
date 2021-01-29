#include "game_aux.h"
#include <assert.h>
#include <stdio.h>
#include "game.h"
#include "game_ext.h"

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

void game_print(cgame g) {
  assert(g);
  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  uint nb_tent_rows[nb_rows];
  uint nb_tent_cols[nb_cols];
  square s;

  for (uint i = 0; i < nb_rows; i++) {
    nb_tent_rows[i] = game_get_expected_nb_tents_row(g, i);
  }
  for (uint j = 0; j < nb_cols; j++) {
    nb_tent_cols[j] = game_get_expected_nb_tents_col(g, j);
  }

  printf("   ");
  for (uint j = 0; j < nb_cols; j++) {
    printf("%u", j);
  }
  printf("\n   ");
  for (uint j = 0; j < nb_cols; j++) {
    printf("-");
  }
  printf("\n");

  for (uint i = 0; i < nb_rows; i++) {
    printf("%u |", i);
    for (uint j = 0; j < nb_cols; j++) {
      s = game_get_square(g, i, j);
      if (s == EMPTY) {
        printf(" ");
      } else if (s == TREE) {
        printf("X");
      } else if (s == TENT) {
        printf("*");
      } else if (s == GRASS) {
        printf("-");
      }
    }
    printf("| %u", nb_tent_rows[i]);
    printf("\n");
  }

  printf("   ");
  for (uint j = 0; j < nb_cols; j++) {
    printf("-");
  }
  printf("\n   ");
  for (uint j = 0; j < nb_cols; j++) {
    printf("%u", nb_tent_cols[j]);
  }
  printf("\n> ? [h for help]\n");
}

game game_default(void) {
  game g = game_new(squares_default, nb_tents_row_default, nb_tents_col_default);
  return g;
}

game game_default_solution(void) {
  game g = game_new(squares_default_solution, nb_tents_row_default, nb_tents_col_default);
  return g;
}