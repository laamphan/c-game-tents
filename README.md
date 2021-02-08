# README

Here is the main description of the game at **version 2**.

## Rules

This game is a simple one-player puzzle. The game starts with a grid of squares, some of which contain trees (🌲). The goal of the game is to place tents (⛺) in the empty squares (⬜) in such a way that the following conditions are met:

- Rule 1) No two tents are adjacent, even diagonally.
- Rule 2) The number of tents in each row, and in each column, matches the expected numbers given around the sides of the grid.
- Rule 3) There are exactly as many tents as trees.
- Rule 4) Each tent must be orthogonally adjacent (horizontally or vertically, but not diagonally) to at least one tree and vice versa.

Besides, it is also possible to place _grass_ (🟩) in a square, indicating that you are sure there is no tent on this square. The _grass_ square gives a clue, but it is not necessary to use it to win the game.

In summary, the game uses 4 different squares:

- empty (⬜ or ' ')
- tree (🌲 or 'x')
- tent (⛺ or '\*')
- grass (🟩 or '-')

By convention, the square in the _i_-th row and _j_-th column of the grid is referred to as the coordinate (i,j), and the coordinate (0,0) corresponds to the top left corner. The dimension of the default game is 8x8.

You can try the original game on the famous [Simon Tatham's Portable Puzzle Collection](https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/tents.html)

## Losing, Illegal and Regular Moves

During the game, a move will be said to be a _losing_ move if playing it prevents definitively the player from winning the game. Nevertheless, it is not forbidden to play such a move. For example, playing one tent adjacent to another is a losing move, or playing _n+1_ tents when n tents are expected is a losing move. More subtly, if you play a grass square and the number of empty squares in a row (or a column) is not enough to reach the expected number of tents, it's also considered as a losing move.

Besides, it is not possible during the game to replace a tree or to add a new tree in the square. And it is considered as an _illegal_ move.

On the other hand, a move that is neither illegal nor losing is said to be a _regular_ move.

## Example: Default Game

As an example, let's see the instance of the game, considered thereafter as the _default_ game:

```
⬜⬜⬜⬜🌲🌲⬜⬜ 3
🌲⬜⬜⬜⬜⬜⬜🌲 0
⬜⬜⬜⬜🌲⬜⬜⬜ 4
🌲⬜⬜⬜⬜🌲⬜⬜ 0
⬜🌲⬜⬜⬜⬜⬜⬜ 4
🌲⬜⬜⬜🌲⬜🌲⬜ 0
⬜⬜⬜⬜⬜⬜⬜⬜ 1
🌲⬜⬜⬜⬜⬜⬜⬜ 0
4 0 1 2 1 1 2 1
```

Hereafter, its solution:

```
⛺🟩🟩⛺🌲🌲⛺🟩 3
🌲🟩🟩🟩🟩🟩🟩🌲 0
⛺🟩🟩⛺🌲⛺🟩⛺ 4
🌲🟩🟩🟩🟩🌲🟩🟩 0
⛺🌲⛺🟩⛺🟩⛺🟩 4
🌲🟩🟩🟩🌲🟩🌲🟩 0
⛺🟩🟩🟩🟩🟩🟩🟩 1
🌲🟩🟩🟩🟩🟩🟩🟩 0
4 0 1 2 1 1 2 1
```

Try it: https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/tents.html#8x8de#911105660705777

## Programming Interface

The game interface is divided into 3 files:

- Basic Game Functions: [**game.h**](https://pt2.gitlabpages.inria.fr/tents/v2/game_8h.html)
- Auxiliarry Game Functions: [**game_aux.h**](https://pt2.gitlabpages.inria.fr/tents/v2/game__aux_8h.html)
- Extended Game Functions: [**game_ext.h**](https://pt2.gitlabpages.inria.fr/tents/v2/game__ext_8h.html)

It manipulates the opaque [**game**](https://pt2.gitlabpages.inria.fr/tents/v2/game_8h.html#a6506ae481babb520162987a6cc2e10ed) structure by means of pointers or const pointers ([**cgame**](https://pt2.gitlabpages.inria.fr/tents/v2/game_8h.html#aa593d9745d861794dde8f808e750457b)).

- https://en.wikipedia.org/wiki/Opaque_data_type

About Const Pointer:

- See also: http://www.geeksforgeeks.org/const-qualifier-in-c/
- See also this more technical discussion: http://stackoverflow.com/questions/8504411/typedef-pointer-const-weirdness

## Error Policy

There are many different ways to deal with program errors... Concerning the "game" library, we suppose that function calls to these interfaces ([**_game.h_**](https://pt2.gitlabpages.inria.fr/tents/v2/game_8h.html), ...) must systematically be made with valid arguments (see preconditions). Also, the behavior of the library is not defined if a third-party program passes bad arguments to it. In practice, this means that the responsibility of checking that the arguments are valid is relieved to the high-level program (_game_text_, ...).

In order to debug your program easily, we recommend to use the `assert()` macro, as it allows you to activate checks only in DEBUG mode and not in RELEASE mode. More precisely, if you pass the `-DNDEBUG` option at compilation time, the macro `assert()` generates no code, and hence does nothing at all.

For example:

```c
#include <assert.h>
#include <stdlib.h>
int* copyArray(int size, int * array)
{
  assert(size > 0);                           // check size parameter
  assert(array != NULL);                      // check array parameter
  int * newarray = malloc(size*sizeof(int));  // allocate memory for a new array
  assert(newarray != NULL);                   // check memory allocation
  for(int i = 0 ; i < size ; i++)             // copy array values
    newarray[i] = array[i];
  return newarray;
}
```

## About the Rule 4

Note that the Rule 4) is not exactly formulated in the same way in the original game. According to [Simon Tatham](https://www.chiark.greenend.org.uk/~sgtatham/puzzles/doc/tents.html), the rule 4) is defined by a _matching_ condition:

> It is possible to match tents to trees so that each tree is orthogonally adjacent to its own tent (but may also be adjacent to other tents).

Another way of phrasing the tree/tent matching criterion is to say that the bipartite adjacency graph between trees and tents has a perfect matching. And there is no trivial algorithm to check this condition (see [Hopcroft–Karp algorithm](https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm)).

Here is an example that satisfies the Rule 4) as we have stated it, but does not satisfy the matching criterion formulated in the original rule.

```
⛺🟩🌲
🌲🟩⛺
⛺🟩🌲
```

The variant of the Rule 4) we have chosen is a local criterion, that is much easier to implement.

## Game Extension

Here, we will consider a generalization of the original game (or _version 1_). This new version (called _version 2_) is characterized by the following changes:

- The grid can have an arbitrary number of rows and columns. The default game dimensions were a square grid of size 8x8.
- The game grid has the possibility to have a "toric" **topology** (`wrapping` option): the rightmost column is adjacent to the leftmost column and the highest row is adjacent to the lowest row.
- The `diagadj` option modifies rule 1) and allows you to place two tents that are diagonally adjacent. By default this is not possible.
- We introduce in this version an history management of game moves ([game_play_move()](https://pt2.gitlabpages.inria.fr/tents/v2/game_8h.html#aae589406b760218d076fd02dcb42ccb9 "Plays a move in a given square. ")), in order to be able to _undo_ the last moves played ([game_undo()](https://pt2.gitlabpages.inria.fr/tents/v2/game__ext_8h.html#a645c3423245a31af908fcd3434254204 "Undoes the last move. ")), or to replay the last undone moves if necessary ([game_redo()](https://pt2.gitlabpages.inria.fr/tents/v2/game__ext_8h.html#a2f80dbffdb3d7b16e05526a29a143f14 "Redoes the last move. ")). After playing a new move with [game_play_move()](https://pt2.gitlabpages.inria.fr/tents/v2/game_8h.html#aae589406b760218d076fd02dcb42ccb9 "Plays a move in a given square. "), it is no longer possible to redo an old cancelled move. The function [game_restart()](https://pt2.gitlabpages.inria.fr/tents/v2/game_8h.html#a403c5ff67201946c3f7043d62d2abb7f "Restarts a game. ") clears the history. To simplify, it is considered that the functions [game_fill_grass_row()](https://pt2.gitlabpages.inria.fr/tents/v2/game_8h.html#a8955f771325896e0aef7d48f7029f56a "Fills a row with grass. ") and [game_fill_grass_col()](https://pt2.gitlabpages.inria.fr/tents/v2/game_8h.html#a89fea612b8fd176363ce083213fde5ed "Fills a column with grass. ") will save a set of moves in the history, which will therefore have to be cancelled in several times. The other functions are not involved in the history management.

The file [game_ext.h](https://pt2.gitlabpages.inria.fr/tents/v2/game__ext_8h.html) contains the description of these new features. Note that version 2 of the game keeps the compatibility with version 1\.

## Game IO

We now add Input/Output features to our library libgame.

For this purpose, we will implement 2 functions [game_load()](game__tools_8h.html#a8737a5878e15868dc35349fca499e640 "Creates a game by loading its description in a text file. ") and [game_save()](game__tools_8h.html#a8eaf898f66ea7d7530b981cb77245e45 "Saves a game in a text file. ") allowing respectively to load and save a game (possibly in progress, and thus possibly the solution of a game). These functions must be implemented in the files [game_tools.h](game__tools_8h.html "Game Tools. ") and game_tools.c.

The format used to save a game is the following:

```
<nb_rows> <nb_cols> <is_swap> <is_diagadj>\n
<nb_tents_row[0]> <nb_tents_row[1]> ... <nb_tents_row[nb_rows-1]> \n
<nb_tents_col[0]> <nb_tents_col[1]> ... <nb_tents_col[nb_cols-1]> \n
<squares[0][0]><squares[0][1]>...<squares[0][nb_cols-1]>\n
<squares[1][0]><squares[1][1]>...<squares[1][nb_cols-1]>\n
...
<squares[nb_rows-1][0]><squares[nb_rows-1][1]>...<squares[nb_rows-1][nb_cols-1]>\n
```

The first line describes in that order the number of grid rows `<nb_rows>`, the number of grid columns `<nb_cols>`, is the topology wrapping or not `<is_wrapping>` (0 for false and 1 for true) and do diagonals count for adjacency `<is_diagadj>` (0 for false and 1 for true). Then, the second line indicates for each line, the number of expected tents (the values are separated by spaces and there is a space at the end of the line). The third line, indicates for each column the number of expected tents (the values are separated by spaces and there is a space at the end of the line). Finally the following lines store the content of each square of the grid with one character (' ' for EMPTY, 'x' for TREE, '*' for TENT, '-' for GRASS). In particular, `<squares[i][j]>` describes the content of the square in the i-th row and j-th column.

**Nota Bene** : The characters return to the line `\n` are indicated for clarify the file format... Note that the (numeric) values in the first 3 lines are separated by spaces but that the values (as character) in the grid are not separated by spaces.

The file [game_tools.h](game__tools_8h.html) contains the description of these input-output features.