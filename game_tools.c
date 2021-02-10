#include "game_tools.h"
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_ext.h"
#include "game_struct.h"

game game_load(char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) exit(EXIT_FAILURE);
  uint nb_rows, nb_cols, wrapping, diagadj;
  fscanf(f, "%u %u %u %u", &nb_rows, &nb_cols, &wrapping, &diagadj);
  uint nb_tents_row[nb_rows];
  uint nb_tents_col[nb_cols];
  square squares[nb_rows * nb_cols];
  char type[100];
  for (uint i = 0; i < nb_rows; i++) fscanf(f, "%u", &nb_tents_row[i]);
  for (uint j = 0; j < nb_cols; j++) fscanf(f, "%u", &nb_tents_col[j]);
  fgets(type, 100, f);
  for (uint i = 0; i < nb_rows; i++) {
    fgets(type, 100, f);
    for (uint j = 0; j < nb_cols; j++) {
      if (type[j] == ' ')
        squares[i * nb_cols + j] = EMPTY;
      else if (type[j] == '-')
        squares[i * nb_cols + j] = GRASS;
      else if (type[j] == 'x')
        squares[i * nb_cols + j] = TREE;
      else if (type[j] == '*')
        squares[i * nb_cols + j] = TENT;
      else
        exit(EXIT_FAILURE);
    }
  }
  game g = game_new_ext(nb_rows, nb_cols, squares, nb_tents_row, nb_tents_col,
                        wrapping, diagadj);
  fclose(f);
  return g;
}

void game_save(cgame g, char *filename) {
  FILE *f = fopen(filename, "w");
  if (f == NULL) exit(EXIT_FAILURE);

  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  bool wrapping = game_is_wrapping(g);
  bool diagadj = game_is_diagadj(g);

  fprintf(f, "%d %d %d %d\n", nb_rows, nb_cols, wrapping, diagadj);
  for (uint i = 0; i < nb_rows; i++)
    fprintf(f, "%d ", game_get_expected_nb_tents_row(g, i));
  fprintf(f, "\n");
  for (uint j = 0; j < nb_cols; j++)
    fprintf(f, "%d ", game_get_expected_nb_tents_col(g, j));
  fprintf(f, "\n");

  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) == EMPTY)
        fprintf(f, " ");
      else if (game_get_square(g, i, j) == GRASS)
        fprintf(f, "-");
      else if (game_get_square(g, i, j) == TREE)
        fprintf(f, "x");
      else if (game_get_square(g, i, j) == TENT)
        fprintf(f, "*");
    }
    fprintf(f, "\n");
  }
  fclose(f);
}