#include "game.h"
#include "game_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define NB_MONSTERS 4 


/**
 * @brief call intialise function
 * @return random game 4x4 created by initalise function
 **/
game default_game() {
	game game1 = new_game_ext(4, 4);
	initialise(game1); // grid generator function
	return game1;
}


/**
 * @brief check the option entered by the user
 * @param take an array of char
 * @return integer
 * return 1 : the choice is find_one
 * return 2 : nb_sol
 * return 3 : find_all
 * return -1 : unvalid choice
 **/
int choice (char* choicename) {
	char find_one[] = "FIND_ONE";
	char nb_sol[] = "NB_SOL";
	char find_all[] = "FIND_ALL";

	if (strcmp(choicename,find_one) == 0) return 1;
	if (strcmp(choicename,nb_sol) == 0) return 2;
	if (strcmp(choicename,find_all) == 0) return 3;
	else return -1;
}

/**
 * @brief check label only on the side of board
 * @param a game and coordinates x & y of the square
 * @return true if current_nb_seen <= required_nb_seen else false
 **/
bool check_label (game g, int x, int y) { 
	//label South
	if (y == 0) {
		if (current_nb_seen(g, S, x) <= required_nb_seen(g, S, x)) return true;
		return false;
	}

	//label North
	if (y == game_height(g) - 1){
		if (current_nb_seen(g, N, x) <= required_nb_seen(g, N, x)) return true;
		return false;
	}

	//label West
	if (x == 0){
		if (current_nb_seen(g, W, y) <= required_nb_seen(g, W, y)) return true;
		return false;
	}

	//label Est
	if (x == game_width(g) - 1){
		if (current_nb_seen(g, E, y) <= required_nb_seen(g, E, y)) return true;
		return false;
	}

	//other case
	return true;
}



/**
 * @brief: find one solution of the problem
 * @param: a game, number of the square, the filename, number of current ghost (mg) ,number of current zombie (mz), number of current vampire (mv), number of current spirit (ms), width, height, number of zombie required (nbz), number of ghost required (nbg), number of vampire required (nbv), number of spirit required (nbs)
 * @return: the game finded
 **/
game solver_one (game game1, int num_case, char* filename, int mg, int mz, int mv, int ms, int width, int height, int nbz, int nbg, int nbv, int nbs) {
	//get x and y 
    int y = num_case / width;
    int x = num_case - (y * width);

	//test if num_case is the last square (=filled grid)
    if (num_case == (height * width)) {
        if (is_game_over(game1)) {
			//solution -> save the game
			save_game(game1, filename);
            return game1;
        }

        else {
            return NULL;
        }
    }
    
	//test if there is a mirror in the square
	content M = get_content(game1, x,y);

    if (M == MIRROR || M == ANTIMIRROR || M == VMIRROR || M == HMIRROR) {
        return solver_one(game1, num_case+1, filename, mg, mz, mv, ms, width, height, nbz, nbg, nbv, nbs);
    }

    else {
		//saved the monster in the square
        int last_monster = M;
        game solution;

		//placed each monster
		int i = 0;
        while (i < NB_MONSTERS) {
            switch (i) {
                case 0:
					//test if we have placed all zombies in the map
					if (mz < nbz) {
						add_monster(game1, ZOMBIE, x, y);
						mz++;

						//check labels
						if (check_label(game1, x, y)) {
							solution = solver_one(game1, num_case+1, filename, mg, mz, mv, ms, width, height, nbz, nbg, nbv, nbs);
							if (solution != NULL) return solution; //case where the solution is finded
						}

						//replace the last monster 
						add_monster(game1, last_monster, x, y);
						mz--;
					}
                    i++;
                    break;

				//same work for other case			
                case 1:
					if (mg < nbg) {
						add_monster(game1, GHOST, x, y);
						mg++;
						if (check_label(game1, x, y)) {
							solution = solver_one(game1, num_case+1, filename, mg, mz, mv, ms, width, height, nbz, nbg, nbv, nbs);
							if (solution != NULL) return solution;
						}
						add_monster(game1, last_monster, x, y);
						mg--;
					}
                    i++;
                    break;

                case 2:
					if (mv < nbv) {
						add_monster(game1, VAMPIRE, x, y);
						mv++;
						if (check_label(game1, x, y)) {
							solution = solver_one(game1, num_case+1, filename, mg, mz, mv, ms, width, height, nbz, nbg, nbv, nbs);
							if (solution != NULL) return solution;
						}
						add_monster(game1, last_monster, x, y);
						mv--;
					}
                    i++;
                    break;

                case 3:
					if (ms < nbs) {
						add_monster(game1, SPIRIT, x, y);
						ms++;
						if (check_label(game1, x, y)) {
							solution = solver_one(game1, num_case+1, filename, mg, mz, mv, ms, width, height, nbz, nbg, nbv, nbs);
							if (solution != NULL) return solution;
						}
						add_monster(game1, last_monster, x, y);
						ms--;
					}
                    i++;
                    break;

                default:
                    break;
            }
        }
    }
    return NULL;
}

/**
 * @brief call function solver_one with all parameters
 * @param a game and a filename
 * @return the game returned by solver_one
 **/
game s_find_one (game game1, char* fname) {
	strcat(fname, ".sol"); //add '.sol' in the end of file saved name
	game g = solver_one(game1, 0, fname, 0, 0, 0, 0, game_width(game1), game_height(game1), required_nb_monsters(game1, ZOMBIE), required_nb_monsters(game1, GHOST), required_nb_monsters(game1, VAMPIRE), required_nb_monsters(game1, SPIRIT));
	if (g == NULL) {
		FILE* f = fopen(fname, "w");
		fprintf(f, "NO SOLUTION");
		fclose(f);
	}
	return g;
}





/**
 * @brief count all solutions of the probleme 
 * @param a game, number of the square, the file name, number of current ghost (mg) ,number of current zombie (mz), number of current vampire (mv), number of current spirit (ms), accumulator, width, height, number of zombie required (nbz), number of ghost required (nbg), number of vampire required (nbv), number of spirit required (nbs)
 * @return the game finded  
 **/
game solver_nbsol (game game1, int num_case, char* filename, int mg, int mz, int mv, int ms, int* cpt, int width, int height, int nbz, int nbg, int nbv, int nbs) {
	//get x and y
    int y = num_case / width;
    int x = num_case - (y * width);

	//test if num_case is the last square (=filled grid)
    if (num_case == (height * width)) {
        if (is_game_over(game1)) {
			//add 1 to the acculator
			*cpt = (*cpt) + 1;
            return game1;
        }

        else {
            return NULL;
        }
    }

    //same end function as solver_one function
	//test if there is a mirror in the square
	content M = get_content(game1, x, y);
    if (M == MIRROR || M == ANTIMIRROR || M == VMIRROR || M == HMIRROR) {
        return solver_nbsol(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs);
    }

    else {
        int i = 0;
		//saved the monster in the square
        int last_monster = M;
        //game solution;

		//placed each monster
        while (i < NB_MONSTERS) {
            switch (i) {
                case 0:
					//test if we have placed all zombies in the map
					if(mz < nbz) {
						add_monster(game1, ZOMBIE, x, y);
						mz++;
						//check labels
						if (check_label(game1, x, y)) {
							solver_nbsol(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs);
						}
						add_monster(game1, last_monster, x, y);
						mz--;
					}
                    i++;
                    break;

                case 1:
					if (mg < nbg) {
						add_monster(game1, GHOST, x, y);
						mg++;
						if (check_label(game1, x, y)) {
							solver_nbsol(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs);
						}
						add_monster(game1, last_monster, x, y);
						mg--;
					}
                    i++;
                    break;

                case 2:
					if (mv < nbv) {
						add_monster(game1, VAMPIRE, x, y);
						mv++;
						if (check_label(game1, x, y)) {
							solver_nbsol(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs);
						}
						add_monster(game1, last_monster, x, y);
						mv--;
					}
                    i++;
                    break;

                case 3:
					if (ms < nbs) {
						add_monster(game1, SPIRIT, x, y);
						ms++;
						if (check_label(game1, x, y)) {
							solver_nbsol(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs);
						}
						add_monster(game1, last_monster, x, y);
						ms--;
					}
                    i++;
                    break;

                default:
                    break;
            }
        }
    }
    return NULL;
}

/**
 * @brief call function solver_nbsol with all parameters
 * @param a game and a filename
 **/
void s_nb_sol (game game1, char* fname) {
	int nb_sol = 0; //number of solution
	strcat(fname, ".nbsol"); //add ".nbsol" in the end of filename
	solver_nbsol(game1, 0, fname, 0, 0, 0, 0, &nb_sol, game_width(game1), game_height(game1), required_nb_monsters(game1, ZOMBIE), required_nb_monsters(game1, GHOST), required_nb_monsters(game1, VAMPIRE), required_nb_monsters(game1, SPIRIT));
	FILE* f = fopen(fname, "w");
	fprintf(f, "%d\n", nb_sol);
	fclose(f);
}




/**
 * @brief write all solutions of the problem in file
 * @param a game, number of the square, the file name, number of current ghost (mg) ,number of current zombie (mz), number of current vampire (mv), number of current spirit (ms), accumulator, width, height, number of zombie required (nbz), number of ghost required (nbg), number of vampire required (nbv), number of spirit required (nbs), a booleen that check if we seen a soluton grid
 * @return the game last tested  
 **/
game solver_all(game game1, int num_case, char* filename, int mg, int mz, int mv, int ms, int* cpt, int width, int height, int nbz, int nbg, int nbv, int nbs, bool* view) {
	//get x and y 
    int y = num_case / width;
    int x = num_case - (y * width);
	game solution=NULL;

	//test if num_case is the last square (=filled grid)
    if (num_case == (height * width)) {
        if (is_game_over(game1)) {
			*view = true; //we see a solution 
			*cpt = *cpt + 1;; //add 1 to accumulator
			char n[10]; //created a array of char
			sprintf(n, "%d", *cpt); //convert the number of solution to char
			char tmp[100]; //create a tempory array of char in order to save the filename
			strcpy(tmp, filename); //copy the filename 
			strcat(tmp, n);	//concat the number of solution to the filename
			save_game(game1, tmp);	//save the game finded with the tempory name
            return game1;
        }

        else {
            return NULL;
        }
    }
    
	//same end function that solve_one and solve_nbsol
	//test if there is a mirror in the square
	content M = get_content(game1, x, y);
    if (M == MIRROR || M == ANTIMIRROR || M == VMIRROR || M == HMIRROR) {
        return solver_all(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs, view);
    }

    else {
        int i = 0;
		//saved the monster in the square
        int last_monster = M;
		//placed each monster
        while (i < NB_MONSTERS) {
            switch(i) {
                case 0:
					if (mz < nbz) {
						add_monster(game1, ZOMBIE, x, y);
						mz++;
						if (check_label(game1, x, y)) {
							solution = solver_all(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs, view);
						}
						add_monster(game1, last_monster, x, y);
						mz--;
					}
                    i++;
                    break;

                case 1:
					if (mg < nbg) {
						add_monster(game1, GHOST, x, y);
						mg++;
						if (check_label(game1, x, y)) {
							solution = solver_all(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs, view);
						}
						add_monster(game1, last_monster, x, y);
						mg--;
					}
                    i++;
                    break;

                case 2:
					if (mv < nbv) {
						add_monster(game1, VAMPIRE, x, y);
						mv++;
						if (check_label(game1, x, y)) {
							solution = solver_all(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs, view);
						}
						add_monster(game1, last_monster, x, y);
						mv--;
					}
                    i++;
                    break;

                case 3:
					if (ms < nbs) {
						add_monster(game1, SPIRIT, x, y);
						ms++;
						if (check_label(game1, x, y)) {
							solution = solver_all(game1, num_case+1, filename, mg, mz, mv, ms, cpt, width, height, nbz, nbg, nbv, nbs, view);
						}
						add_monster(game1, last_monster, x, y);
						ms--;
					}
                    i++;
                    break;

                default:
                    break;
            }
        }
    }
    return solution;
}

/**
 * @brief call function solver_all with all parameters
 * @param a game and a file name
 * @return the game returned by solver_all
 **/
game s_find_all (game game1, char* fname) {
	int nb_sol = 0;
	bool view = false; //initialize variable view
	strcat(fname, ".sol"); //concat ".sol" to the filename
	game g = solver_all(game1, 0, fname, 0, 0, 0, 0, &nb_sol, game_width(game1), game_height(game1), required_nb_monsters(game1, ZOMBIE), required_nb_monsters(game1, GHOST), required_nb_monsters(game1, VAMPIRE), required_nb_monsters(game1, SPIRIT), &view);
	//check if the function solver_all has seen a solution
	if (!view) {
		FILE* f = fopen(fname, "w");
		fprintf(f, "NO SOLUTION");
		fclose(f);
	}
	return g;
}



int main (int argc, char* argv[]) {
	game game1;
	//check arguments
	if (argc == 4) {
		
		int choix = choice(argv[1]); //return the choice to process
		game1 = load_game(argv[2]);    //load the game where the file name is passed in 2nd argument
		char* fname = argv[3];		 //get the file saved name in the variable fname

		switch (choix) {
			//case FIND_ONE
			case 1:
				s_find_one(game1, fname);
				break;
			//case NB_SOL
			case 2:
				s_nb_sol(game1, fname);
				break;
			//case FIND_ALL
			case 3:
				s_find_all(game1, fname);
				break;
			default:
				printf("Choose a valid option: FIND_ONE|NB_SOL|FIND_ALL\n");
				break;
		}
	}

	else {
		//create a map where the file name is passed in 1st argument
		if (argc == 2) {
			game1 = default_game();
			save_game(game1, argv[1]);
		}

		else {
			fprintf(stderr, "Usage:\nTo create a random game: ./undead_solve <nom_fichier_destination>\nTo solve a game: ./undead_solve FIND_ONE|NB_SOL|FIND_ALL <nom_fichier_pb> <prefix_fichier_sol>\n");
			exit(EXIT_FAILURE);
		}
	}
	delete_game(game1);
	return EXIT_SUCCESS;
}