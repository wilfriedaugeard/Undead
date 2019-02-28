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
#include "UI.h"
#include "undead_text.h"

#define WIDTH 4
#define HEIGHT 4
#define MAXLINELEN 4096



/**
 * @brief display the board an the labels of the game
 * @param game1 game we want to display the grid
 **/
void display_game(game game1){
	printf("\nG: %d | Z: %d | V: %d | S: %d\n \n", required_nb_monsters(game1, GHOST),
	required_nb_monsters(game1, ZOMBIE),
	required_nb_monsters(game1, VAMPIRE),
	required_nb_monsters(game1, SPIRIT));
	
	// dislaying the game
	printf("    ");
	for (int label_north = 0; label_north < game_width(game1); label_north++){
		printf("%d  ", required_nb_seen(game1, N, label_north));
	}
	printf("\n");

	for (int line = game_height(game1)-1; line >= 0; line--){							// lines
		printf("\n");
		
		// labels for West side (not a story lol)
		printf("%d   ", required_nb_seen(game1, W, line));
		for (int col = 0; col < game_width(game1); col++) {								// cols
			int val = get_content(game1, col, line); 									// content for each position
			switch(val){
				case EMPTY:
				printf("  ");
				break;
				case MIRROR:
				printf("/ ");
				break;
				case ANTIMIRROR:
				printf("\\ ");
				break;
				case HMIRROR:
				printf("- ");
				break;
				case VMIRROR:
				printf("| ");
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
				case SPIRIT:
				printf("S ");
				break;
			}
			printf(" ");
		}

		// label for East side
		printf(" %d", required_nb_seen(game1, E, line));
	}
	printf("\n\n    ");

	for (int label_south = 0; label_south < game_width(game1); label_south++){
		printf("%d  ", required_nb_seen(game1, S, label_south));
	}
	printf("\n");
}

/**
 * @brief user input for adding monsters on the board
 * @param game1 the game we want to play
 * @return 0 if all is right, EOF if there's a problem.
 **/
int play(game game1){
	//position (x,y)

	int x;
	int y;

	char monster;

	printf("\nWaiting an input : <x> <y> <G|V|Z|S>\n");
	int correct_inputs = scanf("%d %d %c", &x, &y, &monster);

	//clean terminal
	int tmp = write(1,"\E[H\E[2J",7);
	if(tmp <= 0) fprintf(stderr, "%s\n", strerror(errno));
	
	// clean buffer
	getchar();


	if(correct_inputs == 3){
		if (x < 0 || x >= game_width(game1) || y < 0 || y >= game_height(game1)) printf("Unvalid coordinates (x,y)\n");
		
		else if (get_content(game1, x, y) != EMPTY &&
				 get_content(game1, x, y) != GHOST &&
				 get_content(game1, x, y) != VAMPIRE &&
				 get_content(game1, x, y) != ZOMBIE &&
				 get_content(game1, x, y) != SPIRIT)
			printf("This case is already taken by a mirror !\n");

		else {
			switch(monster) {
				case 'G':
				add_monster(game1,GHOST,x,y);
				break;
				case 'V':
				add_monster(game1,VAMPIRE,x,y);
				break;
				case 'Z':
				add_monster(game1,ZOMBIE,x,y);
				break;
				case 'S':
				add_monster(game1, SPIRIT, x, y);
				break;
				default:
				printf("Unvalid monster.\n");
			}
		}
	}
	
	else if (correct_inputs == EOF) return EOF; //error
	else printf("Invalid input.\n");
	return 0;
}






game default_game(){
	game game1 = new_game();
	initialise(game1); // grid generator function
	return game1;
}





int main(int argc,char* argv[]) {
	game game1;
	if (argc==2){
		game1 = load_game(argv[1]);
	}
	else{
		game1 = default_game();
	}
	display_game(game1);
	while(!is_game_over(game1)) {
		
		// play : function with user input
		if(play(game1)==EOF)
		break;
		//save the current game
		save_game(game1,"saved_game");
		display_game(game1);
	}
	
	printf("\n\nYou won\n\n");
	delete_game(game1);
	
	
	return EXIT_SUCCESS;
}
