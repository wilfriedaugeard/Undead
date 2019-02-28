#include "game.h"
#include "game_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define WIDTH 4
#define HEIGHT 4


// initialise la grille
void initialise (game game1){
	set_required_nb_seen(game1, N, 0, 0);
	set_required_nb_seen(game1, N, 1, 3);
	set_required_nb_seen(game1, N, 2, 3);
	set_required_nb_seen(game1, N, 3, 0);
	set_required_nb_seen(game1, S, 0, 0);
	set_required_nb_seen(game1, S, 1, 3);
	set_required_nb_seen(game1, S, 2, 2);
	set_required_nb_seen(game1, S, 3, 3);
	set_required_nb_seen(game1, E, 0, 0); //error
	set_required_nb_seen(game1, E, 1, 0); //error
	set_required_nb_seen(game1, E, 2, 3); //error
	set_required_nb_seen(game1, E, 3, 2); //error
	set_required_nb_seen(game1, W, 0, 0);
	set_required_nb_seen(game1, W, 1, 2);
	set_required_nb_seen(game1, W, 2, 3);
	set_required_nb_seen(game1, W, 3, 3);

	// cree les miroirs
	add_mirror(game1,1,0,0);
	add_mirror(game1,1,0,2);
	add_mirror(game1,1,0,3);
	add_mirror(game1,1,2,1);
	add_mirror(game1,1,3,0);
	add_mirror(game1,0,3,1);
	add_mirror(game1,0,3,3);
}

// display_game affiche la grille du jeu ainsi que le nombre de monstres étant sur la grille.
void display_game(game game1){
	printf("\nG: %d | Z: %d | V: %d\n \n", required_nb_monsters(game1, GHOST),
																			 required_nb_monsters(game1, ZOMBIE),
																			 required_nb_monsters(game1, VAMPIRE));
	// on affiche le jeu
	printf("    ");
	for (int labN = 0; labN < WIDTH; labN++){
		printf("%d  ", required_nb_seen(game1, N, labN));
	}
	printf("\n");
	for (int line = WIDTH-1; line >= 0; line--){							// lignes
		printf("\n");
		printf("%d   ", required_nb_seen(game1, W, line));
		for (int col = 0; col < HEIGHT; col++) {								// colonnes
			int val = get_content(game1, col, line); 									// element pour chaque position
			switch(val){
				case 0:
					printf("  ");
					break;
				case 1:
					printf("/ ");
					break;
				case 2:
					printf("\\ ");
					break;
				case GHOST:
					printf("G ");
					break;
				case VAMPIRE:
					printf("V ");
					break;
				case ZOMBIE:
					printf("Z ");
					break;
				//default:
					//printf("#");
			}
			printf(" ");
		}
		printf(" %d", required_nb_seen(game1, E, line));
	}
	printf("\n\n    ");
	for (int labS = 0; labS < WIDTH; labS++){
		printf("%d  ", required_nb_seen(game1, S, labS));
	}
	printf("\n");
}

// setup_monsters definit le nombre de chaque monstre.
void setup_monsters (game ga, int z, int g, int v){
	set_required_nb_monsters(ga, ZOMBIE, z);
	set_required_nb_monsters(ga, GHOST, g);
	set_required_nb_monsters(ga, VAMPIRE, v);
}

// play prend en compte l'entrée de l'utilisateur, ajoute les monstres.
int  play(game ga){
	//position (x,y)
	int x;
	int y;

	// initiale du monstre
	char m;

	// entree utilisateur
	printf("\nWaiting an input : <x> <y> <G|V|Z>\n");
	int e = scanf("%d %d %c", &x, &y, &m);
	int tmp = write(1,"\E[H\E[2J",7); // clean le terminal a chaque coup
	if(tmp <= 0)
		fprintf(stderr, "%s\n", strerror(errno));

	// Clean le buffer ce qui permet d'eviter le defilment lorsque l'entree utilisateur est invalide
	getchar();
	if(e==3){
			// verification des coordonnees
		if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
			printf("Unvalid input (x,y)\n");
		//}
			// verification de la place ou modification dun monstre deja place
		else if (get_content (ga, x, y) != 0 && get_content (ga, x, y) != GHOST && get_content (ga, x, y) != VAMPIRE && get_content (ga, x, y) != ZOMBIE)
			printf("Not empty !!\n");
		// tout est bon, on ajoute le monstre a la position demandee
		else{
			switch(m){
				case 'G':
					add_monster(ga,GHOST,x,y);
					break;
				case 'V':
					add_monster(ga,VAMPIRE,x,y);
					break;
				case 'Z':
					add_monster(ga,ZOMBIE,x,y);
					break;
				default:
					printf("Unvalid monster\n");
			}
		}
	}

	else if (e == EOF)
			return EOF; //erreur

	else
			printf("Invalid input.\n");

	return 0;
}


int main(void){
	game game1 = new_game();
	setup_monsters (game1, 5,2,2);
	int north[WIDTH], south[WIDTH], east[HEIGHT], west[HEIGHT];
	int *labels[NB_DIR] = {north, south, east, west};
	content board[] = {EMPTY, MIRROR, ANTIMIRROR, VAMPIRE, GHOST, ZOMBIE};



	game g=setup_new_game(labels,
								 board,
								 required_nb_monsters(game1, GHOST),
								 required_nb_monsters(game1, ZOMBIE),
								 required_nb_monsters(game1, VAMPIRE));
  initialise(game1);
	display_game(game1);

	// Boucle de jeu
	while(!is_game_over(game1)){
		if(play(game1)==EOF)
			break;
		display_game(game1);
	}

	if(is_game_over(game1))
		printf ("\n\nYou won\n\n");
		delete_game(game1);
		delete_game(g);

	return EXIT_SUCCESS;
}
