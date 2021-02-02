#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"

#define N 10

struct game_s {
  square *squares;
  uint *nb_tents_row;
  uint *nb_tents_col;
  uint nb_rows;
  uint nb_cols;
  bool wrapping;
  bool diagadj;
};

int main(void) {
  assert(N > 3);

  // queue new
  printf("queue new\n");
  queue *q = queue_new();

  // N push
  for (int i = 0; i < N; i++) {
    printf("queue push head: %d \n", i);
    if (i == N - 1) {
      game data = game_default_solution();
      assert(data);
      queue_push_head(q, data);
    } else {
      game data = game_default();
      assert(data);
      queue_push_head(q, data);
    }
  }
  printf("queue length: %d\n", queue_length(q));

  game head = (game)queue_peek_head(q);
  printf("queue peek head: \n");
  game_print(head);

  game tail = (game)queue_peek_tail(q);
  printf("queue peek tail: \n");
  game_print(tail);

  // // 1 first pop
  game data = (game)queue_pop_tail(q);
  printf("queue pop tail: \n");
  game_print(data);
  free(data);
  printf("queue length: %d\n", queue_length(q));

  // // N-3 pop
  // for (int i = 0; i < N - 3; i++) {
  //   int *data = (int *)queue_pop_tail(q);
  //   printf("queue pop tail: %d\n", *data);
  //   free(data);
  // }
  // printf("queue length: %d\n", queue_length(q));

  // // clear (2 last pop)
  // printf("queue clear full\n");
  // queue_clear_full(q, free);
  // printf("queue length: %d\n", queue_length(q));

  // // free
  printf("queue free\n");
  queue_free(q);
  q = NULL;

  return EXIT_SUCCESS;
}