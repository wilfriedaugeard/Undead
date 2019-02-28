cmake_minimum_required(VERSION 2.6)

project(Haunted)

option(ENABLE_DEBUG "debug options" ON)
if(ENABLE_DEBUG)
    set(CMAKE_C_FLAGS "-std=c99 -g -fprofile-arcs -ftest-coverage -Wall")
    set(CMAKE_LD_FLAGS "-fprofile-arcs -ftest-coverage")
else()
    set(CMAKE_C_FLAGS "-std=c99 -O3 -Wall")
endif()

# Sets the CMP0037 policy to the old one which accept "test" as an executable name
#cmake_policy(SET CMP0037 OLD)

include(CTest)
enable_testing()

# -------- SDL + undead_graphic -------- #
# pkg-config --list-all  | grep -i sdl2
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMG REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} ${SDL2_IMG_LIBRARIES} ${SDL2_TTF_LIBRARIES})
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS} ${SDL2_IMG_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

include_directories(${SDL2_ALL_INC})
add_executable(undead_sdl undead_graphic.c UI.c game.c)
target_link_libraries(undead_sdl ${SDL2_ALL_LIBS} m)
# -------------------------------------- #

add_library(game game_io.c game.c undead_text.c)
# Allows the game library to be generate with .o files
set_target_properties(game PROPERTIES LINKER_LANGUAGE C)


add_executable(undead_text undead_text.c)
target_link_libraries(undead_text game)

add_executable(undead_solve solver.c)
target_link_libraries(undead_solve game)

add_executable(test_romain test_romain.c)
target_link_libraries(test_romain game)

add_executable(test_axel tests_axel.c)
target_link_libraries(test_axel game)

add_executable(test_wil test_wil.c)
target_link_libraries(test_wil game)

add_executable(test_alric test_alric.c)
target_link_libraries(test_alric game)

add_test(test_romain test_romain)
add_test(test_axel test_axel)
add_test(test_wil test_wil)
add_test(test_alric test_alric)

set_tests_properties(test_romain test_axel test_wil test_alric PROPERTIES FAIL_REGULAR_EXPRESSION "failed")
