cmake_minimum_required(VERSION 2.6)

project(Haunted)

option(ENABLE_DEBUG "ON|OFF debug mode" ON)
if(ENABLE_DEBUG)
  set(CMAKE_C_FLAGS "-std=c99 -g -Wall")
  message("-- DEBUG MODE ON")
else()
  set(CMAKE_C_FLAGS "-std=c99 -O3")
  message("-- DEBUG MODE OFF")
endif(ENABLE_DEBUG)

# Sets the CMP0037 policy to the old one which accept "test" as an executable name
cmake_policy(SET CMP0037 OLD)

include(CTest)
enable_testing()

add_library(game game.c)
# Allows the game library to be generate with .o files
set_target_properties(game PROPERTIES LINKER_LANGUAGE C)

add_executable(undead_text undead_text.c)
target_link_libraries(undead_text game)

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
