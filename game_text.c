#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char *argv[]) {
  game g = game_default();
  if (argc == 2) {
    game_delete(g);
    g = game_load(argv[1]);
  }

  while (game_is_over(g) == false) {
    game_print(g);
    char c;
    int i, j;
    scanf(" %c", &c);
    if (c == 'h') {
      printf("> action: help\n");
      printf("- press 't <i> <j>' to put a tent at square (i,j)\n");
      printf("- press 'g <i> <j>' to put grass at square (i,j)\n");
      printf("- press 'e <i> <j>' to erase square (i,j)\n");
      printf("- press 'z' to undo your move'\n");
      printf("- press 'y' to redo your move'\n");
      printf("- press 's' to save current state\n");
      printf("- press 'r' to restart\n");
      printf("- press 'q' to quit\n");
    } else if (c == 't' || c == 'g' || c == 'e') {
      scanf(" %d %d", &i, &j);
      printf("> action: play move '%c' into square %d %d\n", c, i, j);
      square s;
      if (c == 't') s = TENT;
      if (c == 'g') s = GRASS;
      if (c == 'e') s = EMPTY;
      if ((i >= game_nb_rows(g)) || (j >= game_nb_cols(g)) || i < 0 || j < 0 ||
          (game_check_move(g, i, j, s) == ILLEGAL)) {
        printf("Illegal / Wrong parameters\n");
      } else if (game_check_move(g, i, j, s) == LOSING) {
        printf("Warning: losing move in square (%d, %d)\n", i, j);
        game_play_move(g, i, j, s);
      } else {
        game_play_move(g, i, j, s);
      }
    } else if (c == 'z') {
      printf("> action: undo\n");
      game_undo(g);
    } else if (c == 'y') {
      printf("> action: redo\n");
      game_redo(g);
    } else if (c == 's') {
      printf("> action: save\n");
      game_save(g, "./data/current.tnt");
    } else if (c == 'r') {
      printf("> action: restart\n");
      game_restart(g);
    } else if (c == 'q') {
      printf("> action: quit\n");
      printf("What a shame, you gave up :-(\n");
      game_delete(g);
      return EXIT_SUCCESS;
    }
  }
  game_print(g);
  game_delete(g);
  printf("Congratulations, you won! :-)\n");
  return EXIT_SUCCESS;
}
