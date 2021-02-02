#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

int main(void) {
  game g = game_default();
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;

  // extern uint nb_tents_row_default[DEFAULT_SIZE];
  // extern uint nb_tents_col_default[DEFAULT_SIZE];
  // extern square squares_default[DEFAULT_SIZE * DEFAULT_SIZE];
  // game g = game_new_ext(8, 8, squares_default, nb_tents_row_default, nb_tents_col_default, true, true);
  // uint nb_rows = DEFAULT_SIZE;
  // uint nb_cols = DEFAULT_SIZE;

  // uint nb_tents_row_2x4[2] = {1, 1};
  // uint nb_tents_col_2x4[4] = {0, 1, 0, 1};
  // square squares_2x4[2 * 4] = {TREE, EMPTY, EMPTY, EMPTY,
  //                              EMPTY, EMPTY, TREE, EMPTY};
  // game g = game_new_ext(2, 4, squares_2x4, nb_tents_row_2x4, nb_tents_col_2x4, false, false);
  // uint nb_rows = 2;
  // uint nb_cols = 4;

  // uint nb_tents_row_3x3_diag[3] = {1, 1, 0};
  // uint nb_tents_col_3x3_diag[3] = {1, 1, 0};
  // square squares_3x3_diag[3 * 3] = {TREE, EMPTY, EMPTY,
  //                                   EMPTY, TREE, EMPTY,
  //                                   EMPTY, EMPTY, EMPTY};
  // game g = game_new_ext(3, 3, squares_3x3_diag, nb_tents_row_3x3_diag, nb_tents_col_3x3_diag, false, true);
  // uint nb_rows = 3;
  // uint nb_cols = 3;

  // uint nb_tents_row_3x3_wrap[3] = {1, 0, 0};
  // uint nb_tents_col_3x3_wrap[3] = {0, 0, 1};
  // square squares_3x3_diag[3 * 3] = {TREE, EMPTY, EMPTY,
  //                                   EMPTY, EMPTY, EMPTY,
  //                                   EMPTY, EMPTY, EMPTY};
  // game g = game_new_ext(3, 3, squares_3x3_diag, nb_tents_row_3x3_wrap, nb_tents_col_3x3_wrap, true, false);
  // uint nb_rows = 3;
  // uint nb_cols = 3;

  // uint nb_tents_row_3x3_wrap_diag[3] = {1, 0, 1};
  // uint nb_tents_col_3x3_wrap_diag[3] = {1, 0, 1};
  // square squares_3x3_diag[3 * 3] = {EMPTY, EMPTY, TREE,
  //                                   EMPTY, EMPTY, EMPTY,
  //                                   TREE, EMPTY, EMPTY};
  // game g = game_new_ext(3, 3, squares_3x3_diag, nb_tents_row_3x3_wrap_diag, nb_tents_col_3x3_wrap_diag, true, true);
  // uint nb_rows = 3;
  // uint nb_cols = 3;

  while (game_is_over(g) == false) {
    game_print(g);
    char c;
    uint i, j;
    scanf(" %c", &c);
    if (c == 'h') {
      printf("> action: help\n");
      printf("- press 't <i> <j>' to put a tent at square (i,j)\n");
      printf("- press 'g <i> <j>' to put grass at square (i,j)\n");
      printf("- press 'e <i> <j>' to erase square (i,j)\n");
      printf("- press 'z' to undo your move'\n");
      printf("- press 'y' to redo your move'\n");
      printf("- press 'r' to restart\n");
      printf("- press 'q' to quit\n");
      // }
      // //
      // else if (c == 'a') {
      //   game_fill_grass_row(g, 1);
      // } else if (c == 'b') {
      //   game_fill_grass_col(g, 1);
      // }
      // //
    } else if (c == 'z') {
      printf("> action: undo\n");
      game_undo(g);
    } else if (c == 'y') {
      printf("> action: redo\n");
      game_redo(g);
    } else if (c == 'r') {
      printf("> action: restart\n");
      game_restart(g);
    } else if (c == 'q') {
      game_delete(g);
      printf("> action: quit\n");
      printf("What a shame, you gave up :-(\n");
      return EXIT_SUCCESS;
    } else if (c == 't') {
      scanf(" %u %u", &i, &j);
      printf("> action: play move 't' into square %u %u\n", i, j);
      if ((i >= nb_rows) || (j >= nb_cols) ||
          (game_check_move(g, i, j, TENT) == ILLEGAL)) {
        printf("Illegal / Wrong parameters\n");
      } else if (game_check_move(g, i, j, TENT) == LOSING) {
        printf("Warning: losing move in square (%u, %u)\n", i, j);
        game_play_move(g, i, j, TENT);
      } else {
        game_play_move(g, i, j, TENT);
      }
    } else if (c == 'g') {
      scanf(" %u %u", &i, &j);
      printf("> action: play move 'g' into square %u %u\n", i, j);
      if ((i >= nb_rows) || (j >= nb_cols) ||
          (game_check_move(g, i, j, GRASS) == ILLEGAL)) {
        printf("Illegal / Wrong parameters\n");
      } else if (game_check_move(g, i, j, GRASS) == LOSING) {
        printf("Warning: losing move in square (%u, %u)\n", i, j);
        game_play_move(g, i, j, GRASS);
      } else {
        game_play_move(g, i, j, GRASS);
      }
    } else if (c == 'e') {
      scanf(" %u %u", &i, &j);
      printf("> action: play move 'e' into square %u %u\n", i, j);
      if ((i >= nb_rows) || (j >= nb_cols) ||
          (game_check_move(g, i, j, EMPTY) == ILLEGAL)) {
        printf("Illegal / Wrong parameters\n");
      } else if (game_check_move(g, i, j, EMPTY) == LOSING) {
        printf("Warning: losing move in square (%u, %u)\n", i, j);
        game_play_move(g, i, j, EMPTY);
      } else {
        game_play_move(g, i, j, EMPTY);
      }
    }
  }
  game_print(g);
  game_delete(g);
  printf("Congratulations, you won! :-)\n");
  return EXIT_SUCCESS;
}
