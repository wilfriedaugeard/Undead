#include "game.h"
#include "game_io.h"
#include "UI.h"
#include "undead_text.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 4
#define HEIGHT 4


/**
 * @brief creates a random board with mirrors and labels based on a game.
 * @param game1 the game we want to initialise
 **/
void initialise (game game1){
	srand((unsigned) time(NULL));
	int nb_monsters[4] = {0,0,0,0}; //0 -> ZOMBIE | 1 -> GHOST | 2 -> VAMPIRE | 3 -> SPIRIT
	
	// scanning the grid and place randomly monsters and mirrors
	for (int x = 0; x < game_width(game1); x++) {
		for (int y = 0; y < game_height(game1); y++) {
			int random = rand() % 5; // number between 0 and 4
			switch (random) {
				case 0:
				{
					add_monster(game1, ZOMBIE, x, y);
					nb_monsters[0] += 1;
				}
				break;
				case 1:
				{
					add_monster(game1, GHOST, x, y);
					nb_monsters[1] += 1;
				}
				break;
				case 2:
				{
					add_monster(game1, VAMPIRE, x, y);
					nb_monsters[2] += 1;
				}
				break;
				case 3:
				{
					add_monster(game1, SPIRIT, x, y);
					nb_monsters[3] += 1;
				}
				break;
				//mirrors
				case 4:
				{
					int random_mirror = rand() % 4; // number between 0 and 4
					content content_mirror;
					switch (random_mirror) {
						case 0: content_mirror = MIRROR;
						break;
						case 1: content_mirror = ANTIMIRROR;
						break;
						case 2: content_mirror = HMIRROR;
						break;
						case 3: content_mirror = VMIRROR;
						break;
					}
					
					add_mirror_ext(game1, content_mirror, x, y);
				}
				break;
			}
			
			set_required_nb_monsters(game1, ZOMBIE, nb_monsters[0]);
			set_required_nb_monsters(game1, GHOST, nb_monsters[1]);
			set_required_nb_monsters(game1, VAMPIRE, nb_monsters[2]);
			set_required_nb_monsters(game1, SPIRIT, nb_monsters[3]);
		}
	}

	int nb_monsters_seen;
	
	// NORTH AND SOUTH
	for (int x = 0; x < game_width(game1); x++) {
		//nb seen by North side
		nb_monsters_seen = current_nb_seen(game1, N, x);
		set_required_nb_seen(game1, N, x, nb_monsters_seen);
		
		//nb seen by South side
		nb_monsters_seen = current_nb_seen(game1, S, x);
		set_required_nb_seen(game1, S, x, nb_monsters_seen);
	}
	
	// EAST AND WEST
	for (int y = 0; y < game_height(game1); y++) {
		//nb seen by East side
		nb_monsters_seen = current_nb_seen(game1, E, y);
		set_required_nb_seen(game1, E, y, nb_monsters_seen);
		
		//nb seen by West side
		nb_monsters_seen = current_nb_seen(game1, W, y);
		set_required_nb_seen(game1, W, y, nb_monsters_seen);
	}
	
	restart_game(game1); // remove monsters from the board
}


/**
 * @brief Implementation of "game_s" structure. See game.h for more infos.
 **/
typedef struct game_s {
	int nb_ghosts, nb_zombies, nb_vampires, nb_spirits;
	int** labels;
	content* board;
	int width;
	int height;
} game_s;

/**
 * @brief Implementation of the "new_game" function. See game.h for more infos.
 * In V2, new_game asks user for an input (size of the board) and then calls new_game_ext.
 * @return new_game_ext() to create the empty game.
 **/
game new_game() {
	int height, width;
    height = 5;
    width = 5;
	return new_game_ext(width, height);
}

/**
 * @brief Implementation of the "new_game_ext" function. See game.h for more infos.
 * @param width the chosen width of the board.
 * @param height the chosen height of the board.
 * @return the created game set to empty.
 **/
game new_game_ext (int width, int height) {

	// the new game
	game g = (game) malloc(sizeof(game_s));
	if (g == NULL) {
		fprintf(stderr, "Error in \"new_game_ext\" the game is NULL.");
		exit(EXIT_FAILURE);
	}

	g->board = (content*) calloc(width*height, sizeof(int));
	if (g->board == NULL) {
		fprintf(stderr, "Error in \"new_game_ext\" the board is NULL");
		exit(EXIT_FAILURE);
	}

	g->labels = (int **) malloc(NB_DIR*sizeof(int *));
	if (g->labels == NULL) {
		fprintf(stderr, "Error in \"new_game_ext\" the labels is NULL");
		exit(EXIT_FAILURE);
	}

	// each direction depicted by "i", memory allocation for the right number of labels.
	for(int i = 0; i < NB_DIR; i++) {
		if (i == N || i == S) g->labels[i] = (int*) calloc(width, sizeof(int));
		else g->labels[i] = (int*) calloc(height, sizeof(int));

		if (g->labels[i] == NULL) {
			fprintf(stderr, "Error in \"new_game_ext\" the labels[%d] is NULL.", i);
			exit(EXIT_FAILURE);
		}
	}

	g->nb_ghosts = 0;
	g->nb_vampires = 0;
	g->nb_zombies = 0;
	g->nb_spirits = 0;

	g->width = width;
	g->height = height;

	return g;
}

/**
 * @brief Implementation of the "copy_game" function. See game.h for more infos.
 **/
game copy_game (cgame g_src) {
	game copy = new_game_ext(g_src->width, g_src->height);

	copy->nb_ghosts = g_src->nb_ghosts;
	copy->nb_vampires = g_src->nb_vampires;
	copy->nb_zombies = g_src->nb_zombies;
	copy->nb_spirits = g_src->nb_spirits;

	// each case depicted by "i", cloned in the new game 
	for(int i = 0; i < (g_src->width * g_src->height); i++) copy->board[i] = g_src->board[i];

	// each direction depicted by "i", each label depicted by "j", cloned in the new game
	for(int i = 0; i < NB_DIR; i++) {
		if (i == N || i == S) {
			for(int j = 0; j < g_src->width; j++)
				copy->labels[i][j] = g_src->labels[i][j];
		}

		else {
			for(int j = 0; j < g_src->height; j++)
				copy->labels[i][j] = g_src->labels[i][j];
		}
	}

  return copy;
}

/**
 * @brief Implementation of the "delete_game" function. See game.h for more infos.
 **/
void delete_game (game g)
{
	if (g == NULL) return;

	if (g->board != NULL) {
		free(g->board);
    	g->board = NULL;
	}

	// each direction depicted by "i", each side free'd of its labels
	for(int i = 0; i < NB_DIR; i++) {
		if (g->labels[i] != NULL) {
			free(g->labels[i]);
    		g->labels[i] = NULL;
    	}
	}

	free(g->labels);
	g->labels = NULL;

	free(g);
	g = NULL;
}

/**
 * @brief Implementation of the "setup_new_game" function. See game.h for more infos.
 **/
game setup_new_game (int* labels[NB_DIR], content* board, int required_nb_ghosts, int required_nb_vampires, int required_nb_zombies) {
	game g = new_game_ext(HEIGHT, WIDTH);

	g->nb_ghosts = required_nb_ghosts;
	g->nb_vampires = required_nb_vampires;
	g->nb_zombies = required_nb_zombies;

	// each case depicted by "i", copied from param board to game board.
	for(int i = 0; i < (g->width * g->height); i++) g->board[i] = board[i];
	
	// each direction depicted by "i", each tab of labels copied from param labels to game labels.
	for(int i = 0; i < NB_DIR; i++) g->labels[i] = labels[i];

	return g;
}

/**
 * @brief Implementation of the "setup_new_game_ext" function. See game.h for more infos.
 **/
game setup_new_game_ext (int width, int height, int *labels[NB_DIR], content * board,
						int required_nb_ghosts, 
						int required_nb_vampires,
						int required_nb_zombies,
						int required_nb_spirits) {

    game g = new_game_ext(width, height);
    
    g->nb_ghosts = required_nb_ghosts;
    g->nb_vampires = required_nb_vampires;
    g->nb_zombies = required_nb_zombies;
    g->nb_spirits = required_nb_spirits;
	
	// each case depicted by "i", copied from param board to game board.
    for(int i = 0; i < (g->width * g->height); i++) g->board[i] = board[i];
	
	// each direction depicted by "i", each tab of labels copied from param labels to game labels.
    for(int i = 0; i < NB_DIR; i++) g->labels[i] = labels[i];
    
    return g;
}

/**
 * @brief Implementation of the "game_height" function. See game.h for more infos.
 **/
int game_height (cgame game) {
	return game->height;
}

/**
 * @brief Implementation of the "game_width" function. See game.h for more infos.
 **/
int game_width (cgame game) {
	return game->width;
}

/**
  * @brief Implementation of the "restart_game" function. See game.h for more infos.
  **/
void restart_game (game g) {
	if (g == NULL) {
		fprintf(stderr, "Unvalid pointer g in \"restart_game\".\n");
		exit(EXIT_FAILURE);
	}

	// each case depicted by "i", if there's a monster, he's removed.
	for(int i = 0; i < (g->height * g->width); i++) {
		if (g->board[i] != ANTIMIRROR && g->board[i] != MIRROR &&
		   g->board[i] != HMIRROR && g->board[i] != VMIRROR)

		   g->board[i] = EMPTY;
	}
}

/**
  * @brief Implementation of "get_content" function. See game.h for more infos.
  **/
content get_content (cgame game, int col, int line) {
	if (game == NULL) {
		fprintf(stderr, "Unvalid pointer game in \"get_content\".\n");
		exit(EXIT_FAILURE);
	}

	if (col < 0 || col >= game->width || line < 0 || line >= game->height) {
		printf("Value of col or line should be positive (respectively not greater than game height/width).\n");
		exit(EXIT_FAILURE);
	}
	
	else {
		int position = game->width * line + col;
		return game->board[position];
	}
}

/**
  * @brief Implementation of the "is_game_over" function. See game.h for more infos.
  **/
bool is_game_over (cgame g) {
	if (g == NULL) {
		fprintf(stderr, "Unvalid pointer g in \"is_game_over\".\n");
		exit(EXIT_FAILURE);
	}

	// Tests if the board is fulfilled
	for(int i = 0; i < (g->height * g->width); i++) {
		if (g->board[i] == EMPTY)
			return false;
	}

	int nb_g=0;
	int nb_z=0;
	int nb_v=0;
	int nb_s=0;

	for(int i = 0; i < (g->height * g->width); i++) {
		switch(g->board[i]) {
			case GHOST:
				nb_g++;
				break;
			case ZOMBIE:
				nb_z++;
				break;
			case VAMPIRE:
				nb_v++;
				break;
			case SPIRIT:
				nb_s++;
			default:
				break;
		}
	}

	if (nb_g != required_nb_monsters(g, GHOST) || nb_z != required_nb_monsters(g, ZOMBIE) || nb_v != required_nb_monsters(g, VAMPIRE) || nb_s != required_nb_monsters(g, SPIRIT))
		return false;

	direction dirN = N;
	direction dirS = S;
	direction dirW = W;
	direction dirE = E;

	// Tests labels and monsters on North/South sides
	for (int i = 0; i < g->width; i++) {
		if ((required_nb_seen(g, dirN, i) != current_nb_seen(g, dirN, i) ||
			 required_nb_seen(g, dirS, i) != current_nb_seen(g, dirS, i)))
			return false;
	}

  	// Tests labels and monsters on West/East sides
	for (int i = 0; i < g->height; i++) {
		if ((required_nb_seen(g, dirE, i) != current_nb_seen(g, dirE, i) ||
			 required_nb_seen(g, dirW, i) != current_nb_seen(g, dirW, i)))
			return false;
	}

	return true;
}

/**
 * @brief Implementation of "add_monster" function. See game.h for more infos.
 **/
bool add_monster (game game, content monster, int col, int line) {
	if (game == NULL) {
		fprintf(stderr, "Error in \"add_monster\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if (monster != GHOST && monster != VAMPIRE && monster != ZOMBIE && monster != SPIRIT && monster != EMPTY) {
		printf("Not a monster.\n");
		return false;
	}

	if (col >= game->width || line >= game->height || col < 0 || line < 0) {
		printf("Out of the board.\n");
		return false;
	}

	unsigned int position = line * game->width + col;

	if (game->board[position] == MIRROR || game->board[position] == ANTIMIRROR || game->board[position] == HMIRROR || game->board[position] == VMIRROR) {
		printf("Position already used by MIRROR/ANTIMIRROR/HMIRROR/VMIRROR.\n");
		return false;
	}

	game->board[position] = monster;

	return true;
}

/**
 * @brief Implementation of the "add_mirror" function. See game.h for more infos.
 **/
void add_mirror (game game, int dir, int col, int line) {
	if (game == NULL) {
		fprintf(stderr, "Error in \"add_mirror\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	// dir+1 represents the enum content_e (EMPTY = 0; MIRROR = 1, ANTIMIRROR = 2...)
	return add_mirror_ext(game, dir+1, col, line);
}

/**
 * @brief Implementation of the "add_mirror_ext" function. See game.h for more infos.
 **/
void add_mirror_ext (game game, content mirror_type, int col, int line) {
	if (game == NULL) {
		fprintf(stderr, "Error in \"add_mirror\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if (mirror_type != MIRROR && mirror_type != ANTIMIRROR && mirror_type != HMIRROR && mirror_type != VMIRROR) {
		printf("Not a mirror.\n");
		return;
	}

	unsigned int position = line * game->width + col;
	game->board[position] = mirror_type;
}

/**
 * @brief Implementation of the "current_nb_monsters" function. See game.h for more infos.
 **/
int current_nb_monsters (cgame game, content monster) {
	if (game == NULL) {
		fprintf(stderr, "Error in \"current_nb_monsters\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if (monster != GHOST && monster != VAMPIRE && monster != ZOMBIE && monster != SPIRIT)  {
		printf ("Not a monster.\n");
		return -1;
	}

	// counts monsters
	int cpt = 0;

	// each case depicted by "i", if the case is taken by the monster, cpt++
	for (int i = 0; i < game->width*game->height; i++) {
		if (game->board[i] == monster)
			cpt++;
	}

	return cpt;
}

/**
 * @brief Implementation of the "current_nb_seen" function. See game.h for more infos.
 **/
int current_nb_seen (cgame game, direction side, int pos) {
	if (game == NULL) {
		fprintf(stderr, "Error in \"current_nb_seen\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	// x width, y height
	int x = 0;
	int y = 0;

	// initialization x and y for each side
	if (side == N || side == S) {
		x = pos;
		y = (side == N) ? game->height-1 : 0;
	}

	else {
		x = (side == E) ? game->width-1 : 0;
		y = pos;
	}

	int cpt_monster = 0;
	content current_content;
	bool mirror_passed = false;
	direction current_dir = (side == N || side == E) ? side+1 : side-1;

	while(1) {
		// condition for ending function
		if (x < 0 || x > game->width-1 || y < 0 || y > game->height-1) return cpt_monster;

		current_content = get_content(game, x, y);

		// tests value of the case, increments cpt_monster in function of mirrors.
		switch(current_content) {
			case ZOMBIE: {
				cpt_monster++;
				break;
			}

			case GHOST: {
				if (mirror_passed) cpt_monster++;
				break;
			}
			
			case VAMPIRE: {
				if (!mirror_passed)
				cpt_monster++;
				break;
			}
			
			case MIRROR:
			case ANTIMIRROR: mirror_passed = true; break;
			case HMIRROR: if (current_dir == N || current_dir == S) mirror_passed = true; break;
			case VMIRROR: if (current_dir == E || current_dir == W) mirror_passed = true; break;
			default: break;
		}

		// mirror collision (if monster, we're passing through, if mirror, we make a decision in function of its type).
		if (current_content == ZOMBIE || current_content == VAMPIRE || current_content == GHOST || current_content == SPIRIT || current_content == EMPTY) {
			switch(current_dir) {
				case N: y++; break;
				case S: y--; break;
				case E: x++; break;
				case W: x--; break;
				default: break;
			}
		}

		// redirection according to the mirror (ex : case N, we're going North)
		else {
			switch (current_content) {
				case MIRROR: {
					switch(current_dir) {
						case N: { current_dir = E; x++; break; }
						case E: { current_dir = N; y++; break; }
						case S: { current_dir = W; x--; break; }
						case W: { current_dir = S; y--; break; }
						default: break;
					}
				}
				break;

				case ANTIMIRROR: {
					switch(current_dir) {
						case N: { current_dir = W; x--; break; }
						case W: { current_dir = N; y++; break; }
						case S: { current_dir = E; x++; break; }
						case E: { current_dir = S; y--; break; }
						default: break;
					}
				}
				break;

				case HMIRROR: {
					switch(current_dir) {
						case N: { current_dir = S; y--; break; }
						case W: { current_dir = W; x--; break; }
						case S: { current_dir = N; y++; break; }
						case E: { current_dir = E; x++; break; }
						default: break;
					}
				}
				break;

				case VMIRROR: {
					switch(current_dir) {
						case N: { current_dir = N; y++; break; }
						case W: { current_dir = E; x++; break; }
						case S: { current_dir = S; y--; break; }
						case E: { current_dir = W; x--; break; }
						default: break;
					}
				}
				break;

				default: break;
			}
		}
	}
}


/**
  * @brief Implementation of the "set_required_nb_seen" function. See game.h for more infos.
  **/
void set_required_nb_seen (game game, direction side, int pos, int value) {
	if (game == NULL) {
		fprintf(stderr, "Error in \"set_required_nb_seen\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}
	
	if (value >= 0) {
		if (side == N || side == S) {
			if (pos < game->width && pos >= 0)
				game->labels[side][pos] = value;
		}

		if (side == E || side == W) {
			if (pos < game->height && pos >= 0)
				game->labels[side][pos] = value;
		}
	}
}

/**
  * @brief Implementation of the "set_required_nb_monsters" function. See game.h for mor infos.
  **/
void set_required_nb_monsters (game game, content monster, int value) {
	if (game == NULL) {
		fprintf(stderr, "Error in \"set_required_nb_monsters\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	// Tests if the value is negative.
	if (value >= 0) {
		// For each case, tests if the number of monsters (with the new value) is bigger than the number of squares board.
		switch(monster) {
			case GHOST: {
				if ((value + game->nb_zombies + game->nb_vampires + game->nb_spirits) <= (game->width * game->height))
				game->nb_ghosts = value;
				break;
			}
			case ZOMBIE: {
				if ((value + game->nb_ghosts + game->nb_vampires + game->nb_spirits) <= (game->width * game->height))
				game->nb_zombies = value;
				break;
			}
			case VAMPIRE: {
				if ((value + game->nb_zombies + game->nb_ghosts + game->nb_spirits) <= (game->width * game->height))
				game->nb_vampires = value;
				break;
			}
			case SPIRIT: {
				if ((value + game->nb_zombies + game->nb_vampires + game->nb_ghosts) <= (game->width * game->height))
				game->nb_spirits = value;
			}
			default: break;
		}
	}
}

/**
  * @brief Implementation of the "required_nb_seen" function. See game.h for mor infos.
  **/
int required_nb_seen (cgame game, direction side, int pos) {
	if (game == NULL) {
		fprintf(stderr, "Error in \"required_nb_seen\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if (side == N || side == S) {
		if(pos < game->width && pos >= 0)
		return game->labels[side][pos];
	}

	if (side == E || side == W) {
		if (pos < game->height && pos >= 0)
		return game->labels[side][pos];
	}

	// Error code
	return -1;
}

/**
  * @brief Implementation of the "required_nb_monsters" function. See game.h for more infos.
  **/
int required_nb_monsters (cgame game, content monster) {
	if (game == NULL) {
		fprintf(stderr, "Error in \"required_nb_monsters\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	switch(monster) {
		case GHOST: return game->nb_ghosts; break;
		case ZOMBIE: return game->nb_zombies; break;
		case VAMPIRE: return game->nb_vampires; break;
		case SPIRIT: return game->nb_spirits; break;
		default: break;
	}

	// Error code
	return -1;
}
