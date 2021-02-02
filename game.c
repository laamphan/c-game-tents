#include "game.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"

struct game_s {
  square *squares;
  uint *nb_tents_row;
  uint *nb_tents_col;
  uint nb_rows;
  uint nb_cols;
  bool wrapping;
  bool diagadj;
  queue *undo;
  queue *redo;
};

typedef enum {
  FILLSQUARE = 0,  // fill one square move
  FILLROW = 1,     // fill one row move
  FILLCOL = 2,     // fill one column move
} move_type;

// struct history_move_s {
//   move_type type;
//   uint row_nb;
//   uint col_nb;
//   square *squares;
// };

// typedef struct history_move_s *hmove;

//* Aux functions prototypes
uint game_get_nb_adj(cgame g, uint i, uint j, square s);
uint game_get_nb_tents_diag_adj(cgame g, uint i, uint j);
uint game_get_current_nb_emptys_row(cgame g, uint i);
uint game_get_current_nb_emptys_col(cgame g, uint j);
uint game_get_current_nb_emptys_all(cgame g);

//* Required functions
game game_new(square *squares, uint *nb_tents_row, uint *nb_tents_col) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = malloc(sizeof(struct game_s));
  g->squares = malloc(nb_rows * nb_cols * sizeof(square));
  g->nb_tents_row = malloc(nb_rows * sizeof(uint));
  g->nb_tents_col = malloc(nb_cols * sizeof(uint));
  g->undo = queue_new();
  g->redo = queue_new();
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = false;
  g->diagadj = false;

  for (uint i = 0; i < nb_rows; i++) {
    g->nb_tents_row[i] = nb_tents_row[i];
  }
  for (uint j = 0; j < nb_cols; j++) {
    g->nb_tents_col[j] = nb_tents_col[j];
  }
  for (uint k = 0; k < (nb_rows * nb_cols); k++) {
    g->squares[k] = squares[k];
  }

  return g;
}

game game_new_empty(void) {
  uint nb_rows = DEFAULT_SIZE;
  uint nb_cols = DEFAULT_SIZE;
  game g = malloc(sizeof(struct game_s));
  g->squares = malloc(nb_rows * nb_cols * sizeof(square));
  g->nb_tents_row = malloc(nb_rows * sizeof(uint));
  g->nb_tents_col = malloc(nb_cols * sizeof(uint));
  g->undo = queue_new();
  g->redo = queue_new();
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = false;
  g->diagadj = false;

  for (uint i = 0; i < nb_rows; i++) {
    g->nb_tents_row[i] = 0;
  }
  for (uint j = 0; j < nb_cols; j++) {
    g->nb_tents_col[j] = 0;
  }
  for (uint k = 0; k < (nb_rows * nb_cols); k++) {
    g->squares[k] = EMPTY;
  }

  return g;
}

game game_copy(cgame g) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  game g_copy = game_new_empty();
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(g_copy);
  assert(g_copy->squares && g_copy->nb_tents_row && g_copy->nb_tents_col);
  assert(g_copy->undo && g_copy->redo);

  g_copy->nb_rows = g->nb_rows;
  g_copy->nb_cols = g->nb_cols;
  g_copy->wrapping = g->wrapping;
  g_copy->diagadj = g->diagadj;

  for (uint i = 0; i < nb_rows; i++) {
    g_copy->nb_tents_row[i] = g->nb_tents_row[i];
  }
  for (uint j = 0; j < nb_cols; j++) {
    g_copy->nb_tents_col[j] = g->nb_tents_col[j];
  }
  for (uint k = 0; k < (nb_rows * nb_cols); k++) {
    g_copy->squares[k] = g->squares[k];
  }

  return g_copy;
}

bool game_equal(cgame g1, cgame g2) {
  uint nb_rows = g1->nb_rows;
  uint nb_cols = g1->nb_cols;
  assert(g1);
  assert(g1->squares && g1->nb_tents_row && g1->nb_tents_col);
  assert(g1->undo && g1->redo);
  assert(g2);
  assert(g2->squares && g2->nb_tents_row && g2->nb_tents_col);
  assert(g2->undo && g2->redo);

  if (g1->nb_rows != g2->nb_rows || g1->nb_cols != g2->nb_cols ||
      g1->wrapping != g2->wrapping || g1->diagadj != g2->diagadj) {
    return false;
  }
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

  return true;
}

void game_delete(game g) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  free(g->squares);
  free(g->nb_tents_row);
  free(g->nb_tents_col);
  queue_free_full(g->undo, free);
  queue_free_full(g->redo, free);
  free(g);
}

void game_set_square(game g, uint i, uint j, square s) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);
  assert(s >= 0 && s <= 3);

  g->squares[i * nb_cols + j] = s;
}

square game_get_square(cgame g, uint i, uint j) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);

  return g->squares[i * nb_cols + j];
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {
  uint nb_rows = g->nb_rows;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);

  g->nb_tents_row[i] = nb_tents;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(j >= 0 && j < nb_cols);

  g->nb_tents_col[j] = nb_tents;
}

uint game_get_expected_nb_tents_row(cgame g, uint i) {
  uint nb_rows = g->nb_rows;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);

  return g->nb_tents_row[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j) {
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(j >= 0 && j < nb_cols);

  return g->nb_tents_col[j];
}

uint game_get_expected_nb_tents_all(cgame g) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  uint count = 0;

  for (uint k = 0; k < nb_rows * nb_cols; k++) {
    if (g->squares[k] == TREE) {
      count++;
    }
  }

  return count;
}

uint game_get_current_nb_tents_row(cgame g, uint i) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);

  uint count = 0;

  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_square(g, i, j) == TENT) {
      count++;
    }
  }

  return count;
}

uint game_get_current_nb_tents_col(cgame g, uint j) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(j >= 0 && j < nb_cols);

  uint count = 0;

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_square(g, i, j) == TENT) {
      count++;
    }
  }

  return count;
}

uint game_get_current_nb_tents_all(cgame g) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  uint count = 0;

  for (uint k = 0; k < nb_rows * nb_cols; k++) {
    if (g->squares[k] == TENT) {
      count++;
    }
  }

  return count;
}

void game_play_move(game g, uint i, uint j, square s) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);
  assert(s >= 0 && s <= 3);
  assert(s != TREE);
  assert(game_get_square(g, i, j) != TREE);

  if (!queue_is_empty(g->redo)) {
    queue_clear_full(g->redo, free);
  }

  /* save a table of 3 containing:
	move_tab[0] = move type (FILLSQUARE/FILLROW/FILLCOL)
	*/
  uint *move_tab = malloc(4 * sizeof(uint));
  assert(move_tab);
  move_tab[0] = FILLSQUARE;
  move_tab[1] = i;
  move_tab[2] = j;
  move_tab[3] = game_get_square(g, i, j);
  queue_push_head(g->undo, move_tab);

  g->squares[i * nb_cols + j] = s;
}

int game_check_move(cgame g, uint i, uint j, square s) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);
  assert(s >= 0 && s <= 3);

  bool diagadj = g->diagadj;
  bool wrapping = g->wrapping;

  if (s == TREE || game_get_square(g, i, j) == TREE) {
    return ILLEGAL;
  }

  else if (s == EMPTY) {
    return REGULAR;
  }

  else if (s == TENT) {
    if (game_get_square(g, i, j) == EMPTY ||
        game_get_square(g, i, j) == GRASS) {
      if (game_get_expected_nb_tents_row(g, i) <
              (game_get_current_nb_tents_row(g, i) + 1) ||
          game_get_expected_nb_tents_col(g, j) <
              (game_get_current_nb_tents_col(g, j) + 1) ||
          game_get_expected_nb_tents_all(g) <
              (game_get_current_nb_tents_all(g) + 1)) {
        return LOSING;
      }
    } else if (game_get_square(g, i, j) == TENT) {
      if (game_get_expected_nb_tents_row(g, i) <
              game_get_current_nb_tents_row(g, i) ||
          game_get_expected_nb_tents_col(g, j) <
              game_get_current_nb_tents_col(g, j) ||
          game_get_expected_nb_tents_all(g) <
              game_get_current_nb_tents_all(g)) {
        return LOSING;
      }
    }
    if (game_get_nb_adj(g, i, j, TENT) > 0 ||
        game_get_nb_adj(g, i, j, TREE) == 0) {
      return LOSING;
    }
    if (!diagadj) {
      if (game_get_nb_tents_diag_adj(g, i, j) > 0) {
        return LOSING;
      }
    }
  }

  else if (s == GRASS) {
    //* LOSING case: too many GRASS: play GRASS on EMPTY / TENT / GRASS square
    // on EMPTY square
    if (game_get_square(g, i, j) == EMPTY) {
      if ((game_get_current_nb_emptys_row(g, i) - 1) <
          (game_get_expected_nb_tents_row(g, i) -
           game_get_current_nb_tents_row(g, i))) {
        return LOSING;
      }
      if ((game_get_current_nb_emptys_col(g, j) - 1) <
          (game_get_expected_nb_tents_col(g, j) -
           game_get_current_nb_tents_col(g, j))) {
        return LOSING;
      }
      if ((game_get_current_nb_emptys_all(g) - 1) <
          (game_get_expected_nb_tents_all(g) -
           game_get_current_nb_tents_all(g))) {
        return LOSING;
      }
    }
    // on TENT square
    else if (game_get_square(g, i, j) == TENT) {
      if (game_get_current_nb_emptys_row(g, i) <
          (game_get_expected_nb_tents_row(g, i) -
           game_get_current_nb_tents_row(g, i) + 1)) {
        return LOSING;
      }
      if (game_get_current_nb_emptys_col(g, j) <
          (game_get_expected_nb_tents_col(g, j) -
           game_get_current_nb_tents_col(g, j) + 1)) {
        return LOSING;
      }
      if (game_get_current_nb_emptys_all(g) <
          (game_get_expected_nb_tents_all(g) -
           game_get_current_nb_tents_all(g) + 1)) {
        return LOSING;
      }
    }
    // on GRASS square
    else if (game_get_square(g, i, j) == GRASS) {
      if (game_get_current_nb_emptys_row(g, i) <
          (game_get_expected_nb_tents_row(g, i) -
           game_get_current_nb_tents_row(g, i))) {
        return LOSING;
      }
      if (game_get_current_nb_emptys_col(g, j) <
          (game_get_expected_nb_tents_col(g, j) -
           game_get_current_nb_tents_col(g, j))) {
        return LOSING;
      }
      if (game_get_current_nb_emptys_all(g) <
          (game_get_expected_nb_tents_all(g) -
           game_get_current_nb_tents_all(g))) {
        return LOSING;
      }
    }

    //* LOSING case: block a tree from receiving a tent
    if (!wrapping) {
      if ((int)(i - 1) >= 0) {
        if (game_get_square(g, i - 1, j) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i - 1, j, EMPTY) == 1 &&
                game_get_nb_adj(g, i - 1, j, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i - 1, j, EMPTY) == 0 &&
                game_get_nb_adj(g, i - 1, j, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }
      if ((int)(j - 1) >= 0) {
        if (game_get_square(g, i, j - 1) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i, j - 1, EMPTY) == 1 &&
                game_get_nb_adj(g, i, j - 1, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i, j - 1, EMPTY) == 0 &&
                game_get_nb_adj(g, i, j - 1, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }
      if (j + 1 < nb_cols) {
        if (game_get_square(g, i, j + 1) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i, j + 1, EMPTY) == 1 &&
                game_get_nb_adj(g, i, j + 1, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i, j + 1, EMPTY) == 0 &&
                game_get_nb_adj(g, i, j + 1, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }
      if (i + 1 < nb_rows) {
        if (game_get_square(g, i + 1, j) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i + 1, j, EMPTY) == 1 &&
                game_get_nb_adj(g, i + 1, j, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i + 1, j, EMPTY) == 0 &&
                game_get_nb_adj(g, i + 1, j, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }
    }
    //
    else {
      if (i != 0) {
        if (game_get_square(g, i - 1, j) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i - 1, j, EMPTY) == 1 &&
                game_get_nb_adj(g, i - 1, j, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i - 1, j, EMPTY) == 0 &&
                game_get_nb_adj(g, i - 1, j, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }
      //
      else {
        if (game_get_square(g, nb_rows - 1, j) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, nb_rows - 1, j, EMPTY) == 1 &&
                game_get_nb_adj(g, nb_rows - 1, j, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, nb_rows - 1, j, EMPTY) == 0 &&
                game_get_nb_adj(g, nb_rows - 1, j, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }

      //
      if (j != 0) {
        if (game_get_square(g, i, j - 1) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i, j - 1, EMPTY) == 1 &&
                game_get_nb_adj(g, i, j - 1, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i, j - 1, EMPTY) == 0 &&
                game_get_nb_adj(g, i, j - 1, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }
      //
      else {
        if (game_get_square(g, i, nb_cols - 1) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i, nb_cols - 1, EMPTY) == 1 &&
                game_get_nb_adj(g, i, nb_cols - 1, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i, nb_cols - 1, EMPTY) == 0 &&
                game_get_nb_adj(g, i, nb_cols - 1, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }

      //
      if (j + 1 < nb_cols) {
        if (game_get_square(g, i, j + 1) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i, j + 1, EMPTY) == 1 &&
                game_get_nb_adj(g, i, j + 1, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i, j + 1, EMPTY) == 0 &&
                game_get_nb_adj(g, i, j + 1, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }
      //
      else {
        if (game_get_square(g, i, -1 + 1) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i, -1 + 1, EMPTY) == 1 &&
                game_get_nb_adj(g, i, -1 + 1, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i, -1 + 1, EMPTY) == 0 &&
                game_get_nb_adj(g, i, -1 + 1, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }

      //
      if (i + 1 < nb_rows) {
        if (game_get_square(g, i + 1, j) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, i + 1, j, EMPTY) == 1 &&
                game_get_nb_adj(g, i + 1, j, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, i + 1, j, EMPTY) == 0 &&
                game_get_nb_adj(g, i + 1, j, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }
      //
      else {
        if (game_get_square(g, -1 + 1, j) == TREE) {
          if (game_get_square(g, i, j) == EMPTY) {
            if (game_get_nb_adj(g, -1 + 1, j, EMPTY) == 1 &&
                game_get_nb_adj(g, -1 + 1, j, TENT) == 0) {
              return LOSING;
            }
          } else if (game_get_square(g, i, j) == TENT) {
            if (game_get_nb_adj(g, -1 + 1, j, EMPTY) == 0 &&
                game_get_nb_adj(g, -1 + 1, j, TENT) == 1) {
              return LOSING;
            }
          }
        }
      }
    }
  }

  return REGULAR;
}
//!
bool game_is_over(cgame g) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  bool diagadj = g->diagadj;

  if (game_get_current_nb_tents_all(g) != game_get_expected_nb_tents_all(g)) {
    return false;
  }
  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_current_nb_tents_row(g, i) !=
        game_get_expected_nb_tents_row(g, i)) {
      return false;
    }
  }
  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_current_nb_tents_col(g, j) !=
        game_get_expected_nb_tents_col(g, j)) {
      return false;
    }
  }
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) == TREE) {
        if (game_get_nb_adj(g, i, j, TENT) == 0) {
          return false;
        }
      } else if (game_get_square(g, i, j) == TENT) {
        if (game_get_nb_adj(g, i, j, TREE) == 0) {
          return false;
        }
        if (game_get_nb_adj(g, i, j, TENT) > 0) {
          return false;
        }
        if (!diagadj) {
          if (game_get_nb_tents_diag_adj(g, i, j) > 0) {
            return false;
          }
        }
      }
    }
  }

  return true;
}
//!
void game_fill_grass_row(game g, uint i) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);

  uint *move_tab = malloc((2 + nb_cols) * sizeof(uint));
  assert(move_tab);
  move_tab[0] = FILLROW;
  move_tab[1] = i;
  for (uint j = 0; j < nb_cols; j++) {
    move_tab[2 + j] = game_get_square(g, i, j);
  }
  queue_push_head(g->undo, move_tab);

  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      game_set_square(g, i, j, GRASS);
    }
  }
}
//!
void game_fill_grass_col(game g, uint j) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(j >= 0 && j < nb_cols);

  uint *move_tab = malloc((2 + nb_rows) * sizeof(uint));
  assert(move_tab);
  move_tab[0] = FILLCOL;
  move_tab[1] = j;
  for (uint i = 0; i < nb_rows; i++) {
    move_tab[2 + i] = game_get_square(g, i, j);
  }
  queue_push_head(g->undo, move_tab);

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_square(g, i, j) == EMPTY) {
      game_set_square(g, i, j, GRASS);
    }
  }
}
//!
void game_restart(game g) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  for (uint k = 0; k < nb_rows * nb_cols; k++) {
    if (g->squares[k] != TREE) {
      g->squares[k] = EMPTY;
    }
  }
  if (!queue_is_empty(g->undo)) {
    queue_clear_full(g->undo, free);
  }
  if (!queue_is_empty(g->redo)) {
    queue_clear_full(g->redo, free);
  }
}

//* Ext functions

game game_new_ext(uint nb_rows, uint nb_cols, square *squares, uint *nb_tents_row, uint *nb_tents_col,
                  bool wrapping, bool diagadj) {
  game g = malloc(sizeof(struct game_s));
  g->squares = malloc(nb_rows * nb_cols * sizeof(square));
  g->nb_tents_row = malloc(nb_rows * sizeof(uint));
  g->nb_tents_col = malloc(nb_cols * sizeof(uint));
  g->undo = queue_new();
  g->redo = queue_new();
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = wrapping;
  g->diagadj = diagadj;

  for (uint i = 0; i < nb_rows; i++) {
    g->nb_tents_row[i] = nb_tents_row[i];
  }
  for (uint j = 0; j < nb_cols; j++) {
    g->nb_tents_col[j] = nb_tents_col[j];
  }
  for (uint k = 0; k < (nb_rows * nb_cols); k++) {
    g->squares[k] = squares[k];
  }

  return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping, bool diagadj) {
  game g = malloc(sizeof(struct game_s));
  g->squares = malloc(nb_rows * nb_cols * sizeof(square));
  g->nb_tents_row = malloc(nb_rows * sizeof(uint));
  g->nb_tents_col = malloc(nb_cols * sizeof(uint));
  g->undo = queue_new();
  g->redo = queue_new();
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = wrapping;
  g->diagadj = diagadj;

  for (uint i = 0; i < nb_rows; i++) {
    g->nb_tents_row[i] = 0;
  }
  for (uint j = 0; j < nb_cols; j++) {
    g->nb_tents_col[j] = 0;
  }
  for (uint k = 0; k < (nb_rows * nb_cols); k++) {
    g->squares[k] = EMPTY;
  }

  return g;
}

uint game_nb_rows(cgame g) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  return g->nb_rows;
}

uint game_nb_cols(cgame g) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  return g->nb_cols;
}

bool game_is_wrapping(cgame g) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  return g->wrapping;
}

bool game_is_diagadj(cgame g) {
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  return g->diagadj;
}
//!
void game_undo(game g) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  if (queue_is_empty(g->undo)) {
    return;
  }

  uint *move_undo = queue_peek_head(g->undo);
  if (move_undo[0] == FILLSQUARE) {
    uint i = move_undo[1];
    uint j = move_undo[2];
    uint s = move_undo[3];
    square square_save = game_get_square(g, i, j);
    game_set_square(g, i, j, s);
    move_undo[3] = square_save;
  }
  //
  else if (move_undo[0] == FILLROW) {
    uint i = move_undo[1];
    uint row_save[nb_cols];
    for (uint j = 0; j < nb_cols; j++) {
      row_save[j] = game_get_square(g, i, j);
      game_set_square(g, i, j, move_undo[2 + j]);
      move_undo[2 + j] = row_save[j];
    }
  }
  //
  else if (move_undo[0] == FILLCOL) {
    uint j = move_undo[1];
    uint col_save[nb_rows];
    for (uint i = 0; i < nb_rows; i++) {
      col_save[i] = game_get_square(g, i, j);
      game_set_square(g, i, j, move_undo[2 + i]);
      move_undo[2 + i] = col_save[i];
    }
  }

  queue_push_head(g->redo, move_undo);
  queue_pop_head(g->undo);
}
//!
void game_redo(game g) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

  if (queue_is_empty(g->redo)) {
    return;
  }

  uint *move_redo = queue_peek_head(g->redo);
  if (move_redo[0] == FILLSQUARE) {
    uint i = move_redo[1];
    uint j = move_redo[2];
    uint s = move_redo[3];
    square square_save = game_get_square(g, i, j);
    game_set_square(g, i, j, s);
    move_redo[3] = square_save;
  }
  //
  else if (move_redo[0] == FILLROW) {
    uint i = move_redo[1];
    uint row_save[nb_cols];
    for (uint j = 0; j < nb_cols; j++) {
      row_save[j] = game_get_square(g, i, j);
      game_set_square(g, i, j, move_redo[2 + j]);
      move_redo[2 + j] = row_save[j];
    }
  }
  //
  else if (move_redo[0] == FILLCOL) {
    uint j = move_redo[1];
    uint col_save[nb_rows];
    for (uint i = 0; i < nb_rows; i++) {
      col_save[i] = game_get_square(g, i, j);
      game_set_square(g, i, j, move_redo[2 + i]);
      move_redo[2 + i] = col_save[i];
    }
  }

  queue_push_head(g->undo, move_redo);
  queue_pop_head(g->redo);
}

//* Aux functions

uint game_get_nb_adj(cgame g, uint i, uint j, square s) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);
  assert(s >= 0 && s <= 3);

  uint count = 0;
  int i_wrap = i;
  int j_wrap = j;
  bool wrapping = g->wrapping;

  if (!wrapping) {
    if (i != 0) {
      if (game_get_square(g, i - 1, j) == s) {
        count++;
      }
    }
    if (j != 0) {
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
  }
  // wrapping
  else {
    // i == 0
    if (i == 0) {
      i_wrap = nb_rows;
      if (game_get_square(g, i_wrap - 1, j_wrap) == s) {
        count++;
      }
      i_wrap = i;
    }
    // i != 0
    else {
      if (game_get_square(g, i_wrap - 1, j_wrap) == s) {
        count++;
      }
    }
    // j == 0
    if (j == 0) {
      j_wrap = nb_cols;
      if (game_get_square(g, i_wrap, j_wrap - 1) == s) {
        count++;
      }
      j_wrap = j;
    }
    // j != 0
    else {
      if (game_get_square(g, i_wrap, j_wrap - 1) == s) {
        count++;
      }
    }

    // prevent counting duplicates
    // nb_cols > 2
    if (nb_cols > 2) {
      // j == nb_cols - 1
      if (j == nb_cols - 1) {
        j_wrap = -1;
        if (game_get_square(g, i_wrap, j_wrap + 1) == s) {
          count++;
        }
        j_wrap = j;
      }
      // j != nb_cols - 1
      else {
        if (game_get_square(g, i_wrap, j_wrap + 1) == s) {
          count++;
        }
      }
    }
    // nb_rows > 2
    if (nb_rows > 2) {
      // i == nb_rows -1
      if (i == nb_rows - 1) {
        i_wrap = -1;
        if (game_get_square(g, i_wrap + 1, j_wrap) == s) {
          count++;
        }
        i_wrap = i;
      }
      // i != nb_rows -1
      else {
        if (game_get_square(g, i_wrap + 1, j_wrap) == s) {
          count++;
        }
      }
    }
  }

  return count;
}

uint game_get_nb_tents_diag_adj(cgame g, uint i, uint j) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);
  assert(j >= 0 && j < nb_cols);

  uint count = 0;
  int i_wrap = i;
  int j_wrap = j;
  bool wrapping = g->wrapping;

  // no wrapping
  if (!wrapping) {
    if (i >= 1 && j >= 1) {
      if (game_get_square(g, i - 1, j - 1) == TENT) {
        count++;
      }
    }
    if (i >= 1 && (j + 1) < nb_cols) {
      if (game_get_square(g, i - 1, j + 1) == TENT) {
        count++;
      }
    }
    if ((i + 1) < nb_rows && j >= 1) {
      if (game_get_square(g, i + 1, j - 1) == TENT) {
        count++;
      }
    }
    if ((i + 1) < nb_rows && (j + 1) < nb_cols) {
      if (game_get_square(g, i + 1, j + 1) == TENT) {
        count++;
      }
    }
  }
  // wrapping
  else {
    // top left
    // i == 0
    if (i == 0) {
      i_wrap = nb_rows;
      if (j == 0) {
        j_wrap = nb_cols;
      }
      if (game_get_square(g, i_wrap - 1, j_wrap - 1) == TENT) {
        count++;
      }
      i_wrap = i;
      j_wrap = j;
    }
    // i != 0
    else {
      if (j == 0) {
        j_wrap = nb_cols;
      }
      if (game_get_square(g, i_wrap - 1, j_wrap - 1) == TENT) {
        count++;
      }
      j_wrap = j;
    }

    // bot right
    //
    if (i == nb_rows - 1) {
      i_wrap = -1;
      if (j == nb_cols - 1) {
        j_wrap = -1;
      }
      if (game_get_square(g, i_wrap + 1, j_wrap + 1) == TENT) {
        count++;
      }
      i_wrap = i;
      j_wrap = j;
    }
    //
    else {
      if (j == nb_cols - 1) {
        j_wrap = -1;
      }
      if (game_get_square(g, i_wrap + 1, j_wrap + 1) == TENT) {
        count++;
      }
      j_wrap = j;
    }

    if (nb_rows > 2 && nb_cols > 2) {
      // j == 0
      if (j == 0) {
        j_wrap = nb_cols;
        if (i == (nb_rows - 1)) {
          i_wrap = -1;
        }
        if (game_get_square(g, i_wrap + 1, j_wrap - 1) == TENT) {
          count++;
        }
        i_wrap = i;
        j_wrap = j;
      }
      //
      else {
        if (i == (nb_rows - 1)) {
          i_wrap = -1;
        }
        if (game_get_square(g, i_wrap + 1, j_wrap - 1) == TENT) {
          count++;
        }
        i_wrap = i;
      }
      // j == nb_cols - 1
      if (j == (nb_cols - 1)) {
        j_wrap = -1;
        if (i == 0) {
          i_wrap = nb_rows;
        }
        if (game_get_square(g, i_wrap - 1, j_wrap + 1) == TENT) {
          count++;
        }
        i_wrap = i;
        j_wrap = j;
      }
      //
      else {
        if (i == 0) {
          i_wrap = nb_rows;
        }
        if (game_get_square(g, i_wrap - 1, j_wrap + 1) == TENT) {
          count++;
        }
        i_wrap = i;
      }
    }
  }

  return count;
}

uint game_get_current_nb_emptys_row(cgame g, uint i) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(i >= 0 && i < nb_rows);

  uint count = 0;

  for (uint j = 0; j < nb_cols; j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      count++;
    }
  }

  return count;
}

uint game_get_current_nb_emptys_col(cgame g, uint j) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);
  assert(j >= 0 && j < nb_cols);

  uint count = 0;

  for (uint i = 0; i < nb_rows; i++) {
    if (game_get_square(g, i, j) == EMPTY) {
      count++;
    }
  }

  return count;
}

uint game_get_current_nb_emptys_all(cgame g) {
  uint nb_rows = g->nb_rows;
  uint nb_cols = g->nb_cols;
  assert(g);
  assert(g->squares && g->nb_tents_row && g->nb_tents_col);
  assert(g->undo && g->redo);

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
