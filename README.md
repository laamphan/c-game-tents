# README

<span style="color:red">Here is the main description of the game at **version @version**.</span>

## Rules

This game is a simple one-player puzzle. The game starts with a grid of squares,
some of which contain _trees_ (🌲). The goal of the game is to place _tents_ (⛺)
in the _empty_ squares (⬜) in such a way that the following conditions are met:

- Rule 1) No two tents are adjacent, even diagonally.
- Rule 2) The number of tents in each row, and in each column, matches the
  expected numbers given around the sides of the grid.
- Rule 3) There are exactly as many tents as trees.
- Rule 4) Each tent must be orthogonally adjacent (horizontally or vertically,
  but not diagonally) to at least one tree and vice versa.

Besides, it is also possible to place _grass_ (🟩) in a square, indicating that
you are sure there is no tent on this square. The _grass_ square gives a clue,
but it is not necessary to use it to win the game.

In summary, the game uses 4 different squares:

- empty (⬜ or ' ')
- tree (🌲 or 'x')
- tent (⛺ or '\*')
- grass (🟩 or '-')

By convention, the square in the _i_-th row and _j_-th column of the grid is
referred to as the coordinate (i,j), and the coordinate (0,0) corresponds to the
top left corner. The dimension of the default game is 8x8.

You can try the original game on the famous <a
href="https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/tents.html"> Simon
Tatham's Portable Puzzle Collection</a>

## Losing, Illegal and Regular Moves

During the game, a move will be said to be a _losing_ move if playing it
prevents definitively the player from winning the game. Nevertheless, it is not
forbidden to play such a move. For example, playing one tent adjacent to another
is a losing move, or playing _n+1_ tents when _n_ tents are expected is a losing
move. More subtly, if you play a grass square and the number of empty squares in
a row (or a column) is not enough to reach the expected number of tents, it's
also considered as a losing move.

Besides, it is not possible during the game to replace a tree or to add a new
tree in the square. And it is considered as an _illegal_ move.

On the other hand, a move that is neither illegal nor losing is said to be a
_regular_ move.

## Example: Default Game

As an example, let's see the instance of the game, considered thereafter as the
_default_ game:

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

The game interface is divided into 2 files:

- Basic Game Functions: @ref game.h
- Auxiliarry Game Functions: @ref game_aux.h

It manipulates the _opaque_ @ref game structure by means of pointers or const pointers (@ref cgame).

- https://en.wikipedia.org/wiki/Opaque_data_type

About Const Pointer:

- See also: http://www.geeksforgeeks.org/const-qualifier-in-c/
- See also this more technical discussion: http://stackoverflow.com/questions/8504411/typedef-pointer-const-weirdness

## Error Policy

There are many different ways to deal with program errors... Concerning the
"game" library, we suppose that function calls to these interfaces (_game.h_,
...) must systematically be made with valid arguments (see preconditions). Also,
the behavior of the library is not defined if a third-party program passes bad
arguments to it. In practice, this means that the responsibility of checking
that the arguments are valid is relieved to the high-level program (_game_text_,
...).

In order to debug your program easily, we recommand to use the `assert()` macro,
as it allows you to activate checks only in DEBUG mode and not in RELEASE mode.
More precisely, if you pass the `-DNDEBUG` option at compilation time, the macro
`assert()` generates no code, and hence does nothing at all.

For example:

```C
#include <assert.h>
#include <stdlib.h>

/** duplicate an array of integer and return it */
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

Note that the Rule 4) is not exactly formulated in the same way in the original
game. According to <a
href="https://www.chiark.greenend.org.uk/~sgtatham/puzzles/doc/tents.html">Simon
Tatham</a>, the rule 4) is defined by a _matching_ condition:

> It is possible to match tents to trees so that each tree is orthogonally
> adjacent to its own tent (but may also be adjacent to other tents).

Another way of phrasing the tree/tent matching criterion is to say that the
bipartite adjacency graph between trees and tents has a perfect matching. And
there is no trivial algorithm to check this condition (see <a
href="https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm">
Hopcroft–Karp algorithm</a>).

Here is an example that satisfies the Rule 4) as we have stated it, but does not
satisfy the _matching criterion_ formulated in the original rule.

```
⛺🟩🌲
🌲🟩⛺
⛺🟩🌲
```

The variant of the Rule 4) we have chosen is a local criterion, that is much easier to implement.

---
