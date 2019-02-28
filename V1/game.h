#ifndef _GAME_H_
#define _GAME_H_
#include <stdbool.h>

/**
 * @file game.h
 *
 * @brief This files describes the programming interface of a puzzle
 * game, named 'Haunted Mirror'.
 *
 * @mainpage
 *
 * This game was originally invented by David Millar, under the name
 * 'Haunted Mirror Maze'.  A demo of this game is provided in the
 * Simon Tatham's Portable Puzzle Collection, under the name <a
 * href="https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/undead.html">'Undead'</a>.
 *
 * The game board is a grid of 4x4 squares, that initially contains
 * some diagonal mirrors (/ or \\).
 * The aim of the game is to place a ghost (G), a vampire (V) or a zombie (Z)
 * on each empty square of the board, so that the expected number of monsters
 * (given by labels on the four sides) can be seen in each direction, and the
 * total number of monsters of each type match the indicated number.
 *
 * Starting from the edge of the board (on the N, S, E or W side) at a
 * given position, your line of sight draws a path that is reflected
 * successively by mirrors before it exits through another side.
 * On this line of sight, Ghosts are only visible when reflected in
 * at least one mirror, vampires are visible unless they are reflected in a
 * mirror, while Zombies are always visible.
 * For further details on the rules, see <a
 * href="https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/undead.html">here</a>.
 *
 * We use coordinates for the squares of the grid, (0,0) being the bottom
 * left corner, and the first coordinate indicating the column.
 * *
 * Here is an example of a game at start and its solution at end.
 *
 @verbatim
 |   Z:5 V:2 G:2   |   Z:5 V:2 G:2   |
 |                 |                 |
 |     0 3 3 0     |     0 3 3 0     |
 |                 |                 |
 |  3  \     /  2  |  3  \ V V /  2  |
 |  3  \        3  |  3  \ Z Z Z  3  |
 |  2      \ /  0  |  2  Z G \ /  0  | 
 |  0  \     \  0  |  0  \ Z G \  0  |
 |                 |                 |
 |     0 3 2 3     |     0 3 2 3     |
 |                 |                 |
 |     start       |     solution    |
 @endverbatim
 * Observe for example that the label 0 on the left of North line
 * says that no monster should be visible from there, thus squares
 * (1,3) and (2,3) must contain monsters not visible when reflected in a mirror,
 * that is Vampires.
 *
 **/

/** 
 * @brief possible contents of a square on the board.
 * MIRROR stands for a mirror at +45 degrees (/), ANTIMIRROR at -45 degrees (\\) (mnemonic is slash and antislash).
 **/
typedef enum content_e {EMPTY, MIRROR, ANTIMIRROR, VAMPIRE, GHOST, ZOMBIE} content;

#define NB_POSSIBLE_FILLINGS 6

/** 
 * @brief the fourth cardinal directions
 **/

typedef enum direction_e {N,S,E,W} direction;

#define NB_DIR 4


/**
 * @brief The structure pointer that stores the game
 **/

typedef struct game_s* game;

/**
 * @brief The structure constant pointer that stores the game
 * That means that it is not possible to modify the game using this pointer.
 * See also: http://www.geeksforgeeks.org/const-qualifier-in-c/
 * See also this more technical discussion: http://stackoverflow.com/questions/8504411/typedef-pointer-const-weirdness
 **/

typedef const struct game_s* cgame;


/** 
 * @name Version 1 of Game API 
 * 
 **/


///@{

/**
 * @brief creates an empty game with all squares set to empty, all labels and the number of required monsters set to 0.
 * @return the created game
 **/

game new_game();

/**
 * @brief creates a game from a board description.
 * It uses a given board possibly with its mirrors, and a list of labels.
 * @param labels an array of four pointers towards array of labels (namely, labels[N], labels[S], labels[E], labels[W]).
 * The labels are given according to increasing coordinate value (from S to N or from W to E)
 * @param board an array of all the board squares, stored sequentially line by line starting from coordinate (0,0)
 * @param required_nb_ghosts the number of ghosts required in the game.
 * @param required_nb_vampires the number of vampires required in the game.
 * @param required_nb_zombies the number of zombies required in the game.
 * @return the created game
 **/

game setup_new_game(int *labels[NB_DIR], content * board,
		    int required_nb_ghosts,  int required_nb_vampires, int required_nb_zombies);


/**
 * @brief adds a mirror on the game board
 * @param game the game board where to add the mirror
 * @param dir the direction on which to add the mirror: 0 for +45 degrees (/), 1 for -45 degrees (\\)
 * @param col which column to insert the mirror on (0<=col<4)
 * @param line which line to insert the mirror on (0<=line<4)
 **/

void add_mirror(game game, int dir, int col, int line);

/**
 * @brief sets the label of a game
 * @param game the game to modify
 * @param side the side of the game where the label should be modified
 * @param pos the position that the label should be modified (from S to N or from W to E)
 * @param value the new value to give to the label
 **/

void set_required_nb_seen(game game, direction side, int pos, int value);


/**
 * @brief modifies the aimed number of a given monster on the board
 * @param game the game that you want to modify the information about
 * @param monster the type of monster (should be GHOST, VAMPIRE or ZOMBIE)
 * @param value the value you want to set to the parameter.
 **/

void set_required_nb_monsters(game game, content monster, int value);

/**
 * @brief Clone the game g_src
 * @param g_src the game to clone
 * @return the clone of g_src
 **/
game copy_game (cgame g_src);


/**
 * @brief Destroy the game and free allocated memory
 * @param g the game to destroy
 **/
void delete_game (game g);

/**
 * @brief return the label on the side of the board
 * @param game the game we consider
 * @param side the side of the board we want the label from
 * @param pos the position of the label you want (from S to N or from W to E)
 **/

int required_nb_seen(cgame game, direction side, int pos);

/**
 * @brief get the content of a square in the board
 * @param game the game we consider
 * @param col the column of the square
 * @param line the line of the square
 * @return the content of the square
 **/

content get_content(cgame game, int col, int line);

/**
 * @brief indicates the aimed number of a given monster on the board
 * @param game the game that you want to collect the information about
 * @param monster the type of monster (should be GHOST, VAMPIRE or ZOMBIE)
 * @return the number of monsters of that type that should be placed on the game.
 **/

int required_nb_monsters(cgame game, content monster);

/**
 * @brief Test if the game is over (that is the grid is filled according to the requirements).
 * @return true if all the constraints are satisfied
 **/

bool is_game_over (cgame g);

/**
 * @brief Restart a game by cleaning monsters from the board.
 **/

void restart_game(game g);


/**
 * @brief adds a monster on the game board.
 * Can also be used to remove any monster by adding EMPTY. 
 * This function does not have effect on mirrors so it can be used safely in the course of the game.
 * @param game the game board where to add the monster
 * @param monster the type of monster to add
 * @param col the column where to insert the monster
 * @param line and the line where to insert the monster
 * @return false if the monster was not placed since the square was occupied by a mirror.
 **/

bool add_monster(game game, content monster, int col, int line);


/**
 * @brief says how many monsters can be seen on the current game board
 * @param game the game board to look at
 * @param side the side of the board we consider (N, S, W, or E)
 * @param pos the coordinate on that side (from S to N or from W to E)
 * @return the number of monsters that can be seen through all the mirrors from a given side at position x 
 **/

int current_nb_seen(cgame game, direction side, int pos);


/**
 * @brief counts the current number of monsters of a given type
 * @param game the game board
 * @param monster the type of monster
 * @return the total number of monsters of the given type on the board.
 **/

int current_nb_monsters(cgame game, content monster);



///@}

#endif // _GAME_H_
