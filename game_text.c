#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"

int main(void)
{
  game g = game_default();
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  while (game_is_over(g) == false)
  {
    game_print(g);
    char c;
    uint i, j;
    scanf(" %c", &c);
    if (c == 'h')
    {
      printf("> action: help\n");
      printf("- press 't <i> <j>' to put a tent at square (i,j)\n");
      printf("- press 'g <i> <j>' to put grass at square (i,j)\n");
      printf("- press 'e <i> <j>' to erase square (i,j)\n");
      printf("- press 'r' to restart\n");
      printf("- press 'q' to quit\n");
    }
    else if (c == 'r')
    {
      printf("> action: restart\n");
      game_restart(g);
    }
    else if (c == 'q')
    {
      printf("> action: quit\n");
      printf("What a shame, you gave up :-(\n");
      return EXIT_SUCCESS;
    }
    else if (c == 't')
    {
      scanf(" %u %u", &i, &j);
      printf("> action: play move 't' into square %u %u\n", i, j);
      if ((i >= nb_rows) || (j >= nb_cols) ||
          (game_check_move(g, i, j, TENT) == ILLEGAL))
      {
        printf("Illegal / Wrong parameters\n");
      }
      else if (game_check_move(g, i, j, TENT) == LOSING)
      {
        printf("Warning: losing move in square (%u, %u)\n", i, j);
        game_play_move(g, i, j, TENT);
      }
      else
      {
        game_play_move(g, i, j, TENT);
      }
    }
    else if (c == 'g')
    {
      scanf(" %u %u", &i, &j);
      printf("> action: play move 'g' into square %u %u\n", i, j);
      if ((i >= nb_rows) || (j >= nb_cols) ||
          (game_check_move(g, i, j, GRASS) == ILLEGAL))
      {
        printf("Illegal / Wrong parameters\n");
      }
      else if (game_check_move(g, i, j, GRASS) == LOSING)
      {
        printf("Warning: losing move in square (%u, %u)\n", i, j);
        game_play_move(g, i, j, GRASS);
      }
      else
      {
        game_play_move(g, i, j, GRASS);
      }
    }
    else if (c == 'e')
    {
      scanf(" %u %u", &i, &j);
      printf("> action: play move 'e' into square %u %u\n", i, j);
      if ((i >= nb_rows) || (j >= nb_cols) ||
          (game_check_move(g, i, j, EMPTY) == ILLEGAL))
      {
        printf("Illegal / Wrong parameters\n");
      }
      else if (game_check_move(g, i, j, EMPTY) == LOSING)
      {
        printf("Warning: losing move in square (%u, %u)\n", i, j);
        game_play_move(g, i, j, EMPTY);
      }
      else
      {
        game_play_move(g, i, j, EMPTY);
      }
    }
  }
  game_print(g);
  printf("Congratulations, you won! :-)\n");
  return EXIT_SUCCESS;
}
