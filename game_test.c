#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"

//* Usage
void usage(int argc, char *argv[])
{
	fprintf(stderr, "Usage: %s <testname> \n", argv[0]);
	exit(EXIT_FAILURE);
}

//* Tests

bool test_game_new() { return true; }
bool test_game_new_empty() { return true; }
bool test_game_copy() { return true; }
bool test_game_equal() { return true; }
bool test_game_set_square() { return true; }
bool test_game_get_square() { return true; }
bool test_game_set_expected_nb_tents_row() { return true; }
bool test_game_set_expected_nb_tents_col() { return true; }
bool test_game_get_expected_nb_tents_row() { return true; }
bool test_game_get_expected_nb_tents_col() { return true; }
bool test_game_get_expected_nb_tents_all() { return true; }
bool test_game_get_current_nb_tents_row() { return true; }
bool test_game_get_current_nb_tents_col() { return true; }
bool test_game_get_current_nb_tents_all() { return true; }
bool test_game_play_move() { return true; }
bool test_game_check_move() { return true; }
bool test_game_is_over() { return true; }
bool test_game_fill_grass_row() { return true; }
bool test_game_fill_grass_col() { return true; }
bool test_game_restart() { return true; }
bool test_game_print() { return true; }
bool test_game_default() { return true; }
bool test_game_default_solution() { return true; }

//* Main
int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		usage(argc, argv);
	}

	fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
	bool ok = false;

	if (strcmp(argv[1], "game_new") == 0)
	{
		ok = test_game_new();
	}
	else if (strcmp(argv[1], "game_new_empty") == 0)
	{
		ok = test_game_new_empty();
	}
	else if (strcmp(argv[1], "game_copy") == 0)
	{
		ok = test_game_copy();
	}
	else if (strcmp(argv[1], "game_equal") == 0)
	{
		ok = test_game_equal();
	}
	else if (strcmp(argv[1], "game_set_square") == 0)
	{
		ok = test_game_set_square();
	}
	else if (strcmp(argv[1], "game_get_square") == 0)
	{
		ok = test_game_get_square();
	}
	else if (strcmp(argv[1], "game_set_expected_nb_tents_row") == 0)
	{
		ok = test_game_set_expected_nb_tents_row();
	}
	else if (strcmp(argv[1], "game_set_expected_nb_tents_col") == 0)
	{
		ok = test_game_set_expected_nb_tents_col();
	}
	else if (strcmp(argv[1], "game_get_expected_nb_tents_row") == 0)
	{
		ok = test_game_get_expected_nb_tents_row();
	}
	else if (strcmp(argv[1], "game_get_expected_nb_tents_col") == 0)
	{
		ok = test_game_get_expected_nb_tents_col();
	}
	else if (strcmp(argv[1], "game_get_expected_nb_tents_all") == 0)
	{
		ok = test_game_get_expected_nb_tents_all();
	}
	else if (strcmp(argv[1], "game_get_current_nb_tents_row") == 0)
	{
		ok = test_game_get_current_nb_tents_row();
	}
	else if (strcmp(argv[1], "game_get_current_nb_tents_col") == 0)
	{
		ok = test_game_get_current_nb_tents_col();
	}
	else if (strcmp(argv[1], "game_get_current_nb_tents_all") == 0)
	{
		ok = test_game_get_current_nb_tents_all();
	}
	else if (strcmp(argv[1], "game_play_move") == 0)
	{
		ok = test_game_play_move();
	}
	else if (strcmp(argv[1], "game_check_move") == 0)
	{
		ok = test_game_check_move();
	}
	else if (strcmp(argv[1], "game_is_over") == 0)
	{
		ok = test_game_is_over();
	}
	else if (strcmp(argv[1], "game_fill_grass_row") == 0)
	{
		ok = test_game_fill_grass_row();
	}
	else if (strcmp(argv[1], "game_fill_grass_col") == 0)
	{
		ok = test_game_fill_grass_col();
	}
	else if (strcmp(argv[1], "game_restart") == 0)
	{
		ok = test_game_restart();
	}
	else if (strcmp(argv[1], "game_print") == 0)
	{
		ok = test_game_print();
	}
	else if (strcmp(argv[1], "game_default") == 0)
	{
		ok = test_game_default();
	}
	else if (strcmp(argv[1], "game_default_solution") == 0)
	{
		ok = test_game_default_solution();
	}

	else
	{
		fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (ok)
	{
		fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
		return EXIT_SUCCESS;
	}
	else
	{
		fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
		return EXIT_FAILURE;
	}
}