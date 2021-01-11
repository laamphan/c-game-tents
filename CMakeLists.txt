cmake_minimum_required(VERSION 2.6)
project(GameText C)

set(C_MAKE_FLAGS "-std=c99 -Wall")
set(CMAKE_C_COMPILER gcc)

add_library(game game.o game_aux.c)

add_executable(game_text game_text.c) 
target_link_libraries(game_text game)
