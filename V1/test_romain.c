#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "game_io.h"
#include "game.h"

#define HEIGHT 4
#define WIDTH 4

bool test_add_mirror(){
	bool res=true;
	game game1 = new_game();
	assert(game1);
	int m = 0; //mirror
	int a = 1; //antimirror

	for (int x=0; x<WIDTH; x++){
		for (int y=0; y<HEIGHT; y++){
			add_mirror(game1,m,x,y);
			if (get_content(game1,x,y)!=MIRROR){
				res = false;
				printf("Problem on add mirror (mirror) at %d,%d\n",x,y);
			}
		}
	}
	delete_game(game1);
	game game2 = new_game();
	assert(game2);

	for (int x=0; x<WIDTH; x++){
		for (int y=0; y<HEIGHT; y++){
			add_mirror(game2,a,x,y);
			if (get_content(game2,x,y)!=ANTIMIRROR){
				res = false;
				printf("Problem add mirror (antimirror) at %d,%d\n",x,y);
			}
		}
	}
	if (res==true){
		printf("test_add_mirror successfull\n");
	
	}
	delete_game(game2);
	return res;
}
bool test_add_monster(){
	bool res=true;
	game game1 = new_game();
	assert(game1);

	for (int x=0; x<WIDTH; x++){
		for (int y=0; y<HEIGHT; y++){
			add_monster(game1,ZOMBIE,x,y);
			if (get_content(game1,x,y)!=ZOMBIE){
				res = false;
				printf("Problem on add monster (ZOMBIE) at %d,%d\n",x,y);
			}
		}
	}
	for (int x=0; x<WIDTH; x++){
			for (int y=0; y<HEIGHT; y++){
				add_monster(game1,VAMPIRE,x,y);
				if (get_content(game1,x,y)!=VAMPIRE){
					res = false;
					printf("Problem on add monster (VAMPIRE) at %d,%d\n",x,y);
				}
			}
		}
	for (int x=0; x<WIDTH; x++){
			for (int y=0; y<HEIGHT; y++){
				add_monster(game1,GHOST,x,y);
				if (get_content(game1,x,y)!=GHOST){
					res = false;
					printf("Problem on add monster (GHOST) at %d,%d\n",x,y);
				}
			}
		}
	if (res==true){
		printf("test_add_monster successfull\n");
		
	}
	delete_game(game1);
	return res;
}


bool test_get_content(){
	bool res = true;
	game game1 = new_game();
	assert(game1);
	int m = 0; //mirror
	int a = 1; //antimirror
	assert (game1);
	for (int x=0; x<WIDTH; x++){
		for (int y=0; y<HEIGHT; y++){
			add_monster(game1,ZOMBIE,x,y);
			if (get_content(game1,x,y)!=ZOMBIE){
				res = false;
				printf("Problem on get content (ZOMBIE) at %d,%d\n",x,y);
			}
		}
	}
	for (int x=0; x<WIDTH; x++){
			for (int y=0; y<HEIGHT; y++){
				add_monster(game1,VAMPIRE,x,y);
				if (get_content(game1,x,y)!=VAMPIRE){
					res = false;
					printf("Problem on get content (VAMPIRE) at %d,%d\n",x,y);
				}
			}
		}
	for (int x=0; x<WIDTH; x++){
			for (int y=0; y<HEIGHT; y++){
				add_monster(game1,GHOST,x,y);
				if (get_content(game1,x,y)!=GHOST){
					res = false;
					printf("Problem on get content (GHOST) at %d,%d\n",x,y);
				}
			}
		}
		for (int x=0; x<WIDTH; x++){
			for (int y=0; y<HEIGHT; y++){
				add_mirror(game1,m,x,y);
				if (get_content(game1,x,y)!=MIRROR){
					res = false;
					printf("Problem on get content (mirror) at %d,%d\n",x,y);
				}
			}
		}
		for (int x=0; x<WIDTH; x++){
			for (int y=0; y<HEIGHT; y++){
				add_mirror(game1,a,x,y);
				if (get_content(game1,x,y)!=ANTIMIRROR){
					res = false;
					printf("Problem on get content (antimirror) at %d,%d\n",x,y);
				}
			}
		}
		if (res==true){
		printf("test_get_content successfull\n");
		
	}
	delete_game(game1);
	return res;
}

bool test_current_nb_monsters(){
	bool res = true;
	game game1 = new_game();
	assert(game1);
	for (int x=0; x<WIDTH; x++){
		for (int y=0; y<HEIGHT; y++){
			add_monster(game1,ZOMBIE,x,y);
		}
	}
	if (current_nb_monsters(game1, ZOMBIE)!=(HEIGHT*WIDTH)){
		res = false;
		printf("Problem on current_nb_monster (ZOMBIE)\n");
	}
	for (int x=0; x<WIDTH; x++){
		for (int y=0; y<HEIGHT; y++){
			add_monster(game1,GHOST,x,y);
		}
	}
	if (current_nb_monsters(game1, GHOST)!=(HEIGHT*WIDTH)){
		res = false;
		printf("Problem on current_nb_monster (GHOST)\n");
	}
	for (int x=0; x<WIDTH; x++){
		for (int y=0; y<HEIGHT; y++){
			add_monster(game1,VAMPIRE,x,y);
		}
	}
	if (current_nb_monsters(game1, VAMPIRE)!=(HEIGHT*WIDTH)){
		res = false;
		printf("Problem on current_nb_monster (VAMPIRE)\n");
	}
	if (res==true){
		printf("test_current_nb_monsters successfull\n");
	}
	delete_game(game1);
	return res;
}

bool test_required_nb_seen(){

	bool res=true;
	game game1 = new_game();
	assert(game1);
	set_required_nb_seen(game1,N,0,2);
	set_required_nb_seen(game1,S,0,1);
	set_required_nb_seen(game1,E,0,3);
	set_required_nb_seen(game1,W,0,0);

	if (required_nb_seen(game1,N,0)!=2){
		res=false;
		printf("Problem on the north label\n");
	}
	else if (required_nb_seen(game1,S,0)!=1){
		res=false;
		printf("Problem on the south label\n");
	}
	else if (required_nb_seen(game1,E,0)!=3){
		res=false;
		printf("Problem on the east label\n");
	}
	else if (required_nb_seen(game1,W,0)!=0){
		printf("Problem on the west label\n");
		res=false;
	}
	else{
		printf("test_required_nb_seen successfull\n");
	}
	delete_game(game1);
	return res;
}

int main() {

	bool result = true;
	
	result = test_add_mirror() && result;
	result = test_add_monster() && result;
	result = test_get_content () && result;
	result = test_current_nb_monsters() && result;
	result = test_required_nb_seen() && result;

	if (result == true)
		printf(" \nAll tests are successfull.\n");
	else
		printf("\nOne or more tests have failed.\n");

	return EXIT_SUCCESS;
}
