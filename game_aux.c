#include "game_aux.h"
#include <assert.h>
#include <stdio.h>
#include "game.h"

void game_print(cgame g) {
  assert(g);
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  uint nb_tent_rows[DEFAULT_SIZE];
  uint nb_tent_cols[DEFAULT_SIZE];
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
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  extern square squares_default[DEFAULT_SIZE * DEFAULT_SIZE];
  game g = game_new(squares_default, nb_tents_row_default, nb_tents_col_default);
  return g;
}

game game_default_solution(void) {
  extern uint nb_tents_row_default[DEFAULT_SIZE];
  extern uint nb_tents_col_default[DEFAULT_SIZE];
  extern square squares_default_solution[DEFAULT_SIZE * DEFAULT_SIZE];
  game g = game_new(squares_default_solution, nb_tents_row_default, nb_tents_col_default);
  return g;
}