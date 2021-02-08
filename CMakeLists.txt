cmake_minimum_required(VERSION 2.6)
project(c-game-tents)

enable_testing()
include(CTest)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c99 -g --coverage")
set(CMAKE_C_COMPILER gcc)

set (
  CTEST_CUSTOM_COVERAGE_EXCLUDE
	"game_text.c"
)

add_library(game game.c game_aux.c game_ext.c game_tools.c game_private.c queue.c)

add_executable(game_text game_text.c) 
target_link_libraries(game_text game)

add_executable(game_test game_test.c)
target_link_libraries(game_test game)

add_test(game_new ./game_test game_new)
add_test(game_new_empty ./game_test game_new_empty)
add_test(game_copy ./game_test game_copy)
add_test(game_equal ./game_test game_equal)
add_test(game_set_square ./game_test game_set_square)
add_test(game_get_square ./game_test game_get_square)
add_test(game_set_expected_nb_tents_row ./game_test game_set_expected_nb_tents_row)
add_test(game_set_expected_nb_tents_col ./game_test game_set_expected_nb_tents_col)
add_test(game_get_expected_nb_tents_row ./game_test game_get_expected_nb_tents_row)
add_test(game_get_expected_nb_tents_col ./game_test game_get_expected_nb_tents_col)
add_test(game_get_expected_nb_tents_all ./game_test game_get_expected_nb_tents_all)
add_test(game_get_current_nb_tents_row ./game_test game_get_current_nb_tents_row)
add_test(game_get_current_nb_tents_col ./game_test game_get_current_nb_tents_col)
add_test(game_get_current_nb_tents_all ./game_test game_get_current_nb_tents_all)
add_test(game_play_move ./game_test game_play_move)
add_test(game_check_move ./game_test game_check_move)
add_test(game_check_move_1 ./game_test game_check_move_1)
add_test(game_check_move_2 ./game_test game_check_move_2)
add_test(game_check_move_3 ./game_test game_check_move_3)
add_test(game_check_move_4 ./game_test game_check_move_4)
add_test(game_is_over ./game_test game_is_over)
add_test(game_is_over_1 ./game_test game_is_over_1)
add_test(game_is_over_2 ./game_test game_is_over_2)
add_test(game_is_over_3 ./game_test game_is_over_3)
add_test(game_is_over_4 ./game_test game_is_over_4)
add_test(game_fill_grass_row ./game_test game_fill_grass_row)
add_test(game_fill_grass_col ./game_test game_fill_grass_col)
add_test(game_restart ./game_test game_restart)
add_test(game_print ./game_test game_print)
add_test(game_default ./game_test game_default)
add_test(game_default_solution ./game_test game_default_solution)

add_test(game_new_ext ./game_test game_new_ext)
add_test(game_new_empty_ext ./game_test game_new_empty_ext)
add_test(game_nb_rows ./game_test game_nb_rows)
add_test(game_nb_cols ./game_test game_nb_cols)
add_test(game_is_wrapping ./game_test game_is_wrapping)
add_test(game_is_diagadj ./game_test game_is_diagadj)
add_test(game_undo ./game_test game_undo)
add_test(game_redo ./game_test game_redo)

add_test(game_extra ./game_test game_extra)


add_library(queue queue.c)
add_executable(test_queue test_queue.c)
target_link_libraries(test_queue queue)

add_test(test_queue_new_free ./test_queue init_free)
add_test(test_queue_push_head ./test_queue push_head)
add_test(test_queue_pop_head ./test_queue pop_head)
add_test(test_queue_push_tail ./test_queue push_tail)
add_test(test_queue_pop_tail ./test_queue pop_tail)
add_test(test_queue_length ./test_queue length)
add_test(test_queue_empty ./test_queue empty)
add_test(test_queue_clear ./test_queue clear)

