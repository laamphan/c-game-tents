#include <assert.h>
#include <stdio.h>
#include "game_aux.h"
#include "game.h"

void game_print(cgame g)
{
	assert(g);
	uint nb_rows = DEFAULT_SIZE;
	uint nb_cols = DEFAULT_SIZE;
	uint nb_tent_rows[DEFAULT_SIZE];
	uint nb_tent_cols[DEFAULT_SIZE];
	square s;

	for (uint i = 0; i < nb_rows; i++)
	{
		nb_tent_rows[i] = game_get_expected_nb_tents_row(g, i);
	}
	for (uint j = 0; j < nb_cols; j++)
	{
		nb_tent_cols[j] = game_get_expected_nb_tents_col(g, j);
	}

	printf("   ");
	for (uint j = 0; j < nb_cols; j++)
	{
		printf("%u", j);
	}
	printf("\n   ");
	for (uint j = 0; j < nb_cols; j++)
	{
		printf("-");
	}
	printf("\n");

	for (uint i = 0; i < nb_rows; i++)
	{
		printf("%u |", i);
		for (uint j = 0; j < nb_cols; j++)
		{
			if (game_get_square(g, i, j) == EMPTY)
			{
				printf(" ");
			}
			else if (game_get_square(g, i, j) == TREE)
			{
				printf("X");
			}
			if (game_get_square(g, i, j) == TENT)
			{
				printf("*");
			}
			if (game_get_square(g, i, j) == GRASS)
			{
				printf("-");
			}
		}
		printf("| %u", nb_tent_rows[i]);
		printf("\n");
	}
	printf("   ");

	for (uint j = 0; j < nb_cols; j++)
	{
		printf("-");
	}
	printf("\n   ");
	for (uint j = 0; j < nb_cols; j++)
	{
		printf("%u", nb_tent_cols[j]);
	}
	printf("\n");
	printf("> ? [h for help]\n");
}

game game_default(void)
{
	uint nb_tents_row[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
	uint nb_tents_col[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
	square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
			EMPTY, EMPTY, EMPTY, EMPTY, TREE, TREE, EMPTY, EMPTY,
			TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
			EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY,
			TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY,
			EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
			TREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, TREE, EMPTY,
			EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
			TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};

	game g = game_new(squares, nb_tents_row, nb_tents_col);
	return g;
}

game game_default_solution(void)
{
	uint nb_tents_row[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
	uint nb_tents_col[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
	square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
			TENT, GRASS, GRASS, TENT, TREE, TREE, TENT, GRASS,
			TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
			TENT, GRASS, GRASS, TENT, TREE, TENT, GRASS, TENT,
			TREE, GRASS, GRASS, GRASS, GRASS, TREE, GRASS, GRASS,
			TENT, TREE, TENT, GRASS, TENT, GRASS, TENT, GRASS,
			TREE, GRASS, GRASS, GRASS, TREE, GRASS, TREE, GRASS,
			TENT, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
			TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};

	game g = game_new(squares, nb_tents_row, nb_tents_col);
	return g;
}