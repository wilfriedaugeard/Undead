#include "game.h"
#include "game_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define WIDTH 4
#define HEIGHT 4

bool test_new_game() {
  printf("TESTING NEW GAME FUNCTION.\n");
  game newGame = new_game();
  assert(newGame);
  bool ret = true;

  if (newGame == NULL){
    ret = false;
    printf("newGame not created\n");
  }

  else if (required_nb_monsters(newGame, GHOST) != 0 ||
           required_nb_monsters(newGame, VAMPIRE) != 0 ||
           required_nb_monsters(newGame, ZOMBIE) != 0 ||
           required_nb_monsters(newGame, SPIRIT) != 0) {
    ret = false;
    printf("We shouldn't have monsters\n");
  }

  // Teste nombre de monstres
  else if (current_nb_monsters(newGame, GHOST) != 0 ||
           current_nb_monsters(newGame, VAMPIRE) != 0 ||
           current_nb_monsters(newGame, ZOMBIE) != 0 ||
           current_nb_monsters(newGame, SPIRIT) != 0) {
    ret = false;
    printf("We shouldn't have monsters\n");
  }

  direction dirN = N;
  direction dirS = S;
  direction dirW = W;
  direction dirE = E;

  // Teste les labels et monstres presents N/S
  for (int i = 0; i < game_height(newGame); i++) {
    if (required_nb_seen(newGame, dirN, i) != 0 ||
        required_nb_seen(newGame, dirS, i) != 0 ||
        current_nb_seen(newGame, dirN, i) != 0 ||
        current_nb_seen(newGame, dirS, i) != 0) {
        ret = false;
    }
  }

  // Teste les labels et monstres presents E/W
  for (int i = 0; i < game_width(newGame); i++) {
    if (required_nb_seen(newGame, dirE, i) != 0 ||
        required_nb_seen(newGame, dirW, i) != 0 ||
        current_nb_seen(newGame, dirE, i) != 0 ||
        current_nb_seen(newGame, dirW, i) != 0) {
        ret = false;
        printf("We should see any monsters on the board on East label or West label\n");
    }
  }

  // Teste si la grille générée est vide (ne retourne rien si case vide (0), retourne false sinon)
  for (int line = game_width(newGame)-1; line >= 0; line--) {
    for (int col = 0; col < game_height(newGame); col++) {
      if (get_content(newGame, col, line) != EMPTY) {
        ret = false;
        printf("The board should be empty\n");
      }
    }
  }

  //printf("Test on Test_new_game successfull.\n");
  delete_game(newGame);
  return ret;
}

bool test_setup_new_game() {
  printf("TESTING SETUP NEW GAME FUNCTION.\n");
  bool res = true;

  int **labels = (int **) malloc(NB_DIR*sizeof(int *));
	if(labels == NULL) {
		fprintf(stderr, "Error in \"new_game_ext\" the labels is NULL");
		exit(EXIT_FAILURE);
  }
  
  for (int i = 0; i < NB_DIR; i++) {
    labels[i] = (int *) calloc(4, sizeof(int)); // 4 -> 4 labels par direction

    if (labels[i] == NULL) {
      fprintf(stderr, "labels[%d] alloc failed.", i);
      return false;
    }

    labels[i][0] = 1;
    labels[i][1] = 1;
    labels[i][2] = 1;
    labels[i][3] = 1;
  }

  content* board = (content*) calloc(16, sizeof(int)); // 16 -> 16 cases si grille 4*4
  if (board == NULL) {
    fprintf(stderr, "board alloc failed.");
    return false;
  }

  board[0] = MIRROR;
  board[1] = GHOST;
  board[2] = ZOMBIE;
  board[3] = ANTIMIRROR;

  game newGame = setup_new_game(labels, board, 2, 2, 5);
  assert(newGame);

  if (newGame == NULL) {
    fprintf(stderr,"test on setup_new_game failed: NULL game\n");
    res = false;
  }

  if (required_nb_monsters(newGame, ZOMBIE) != 5) {
    fprintf(stderr,"test on setup_new_game failed: incorrect nb zombies\n");
    res = false;
  }

  if (required_nb_monsters(newGame, GHOST) != 2) {
    fprintf(stderr,"test on setup_new_game failed: incorrect nb ghosts\n");
    res = false;
  }

  if (required_nb_monsters(newGame, VAMPIRE) != 2) {
    fprintf(stderr,"test on setup_new_game failed: incorrect nb vampires\n");
    res = false;
  }

  if (required_nb_seen(newGame, N, 0) != 1 ||
      required_nb_seen(newGame, N, 1) != 1 ||
      required_nb_seen(newGame, N, 2) != 1 ||
      required_nb_seen(newGame, N, 3) != 1) {
    fprintf(stderr,"test on setup_new_game failed : error on N\n");
    res = false;
  }

  if (required_nb_seen(newGame, S, 0) != 1 ||
      required_nb_seen(newGame, S, 1) != 1 ||
      required_nb_seen(newGame, S, 2) != 1 ||
      required_nb_seen(newGame, S, 3) != 1) {
    fprintf(stderr,"test on setup_new_game failed : error on S\n");
    res = false;
  }

  if (required_nb_seen(newGame, E, 0) != 1 ||
      required_nb_seen(newGame, E, 1) != 1 ||
      required_nb_seen(newGame, E, 2) != 1 ||
      required_nb_seen(newGame, E, 3) != 1) {
    fprintf(stderr,"test on setup_new_game failed : error on E\n");
    res = false;
  }

  if (required_nb_seen(newGame, W, 0) != 1 ||
      required_nb_seen(newGame, W, 1) != 1 ||
      required_nb_seen(newGame, W, 2) != 1 ||
      required_nb_seen(newGame, W, 3) != 1) {
    fprintf(stderr,"test on setup_new_game failed : error on W\n");
    res = false;
  }

  if (get_content(newGame, 0, 0) != MIRROR ||
      get_content(newGame, 1, 0) != GHOST ||
      get_content(newGame, 2, 0) != ZOMBIE ||
      get_content(newGame, 3, 0) != ANTIMIRROR) {
    printf("test on setup_new_game failed\n");
    res = false;
  }

  for(int i = 0; i < NB_DIR; i++) {
    free(labels[i]);
    labels[i] = NULL;
  }

  free(labels);

  free(board);
  board = NULL;

  //delete_game(newGame);

  return res;
}

bool test_is_game_over() {
  printf("TESTING IS GAME OVER FUNCTION.\n");
  game newGame = new_game();
  assert(newGame);

  bool ret = true;

  direction dirN = N;
  direction dirS = S;
  direction dirW = W;
  direction dirE = E;


  // Teste les labels et monstres presents N/S
  for (int i = 0; i < WIDTH; i++) {
    if ((required_nb_seen(newGame, dirN, i) != current_nb_seen(newGame, dirN, i) || required_nb_seen(newGame, dirS, i) != current_nb_seen(newGame, dirS, i))
         && is_game_over(newGame) == true)
        ret = false;
  }

  // Teste les labels et monstres presents E/W
  for (int i = 0; i < HEIGHT; i++) {
    if ((required_nb_seen(newGame, dirE, i) != current_nb_seen(newGame, dirE, i) || required_nb_seen(newGame, dirW, i) != current_nb_seen(newGame, dirW, i))
        && is_game_over(newGame) == true)
        ret = false;
  }

  delete_game(newGame);
  return ret;
}

int main(void) {

  //bool result1 = test_new_game();
  bool result2 = test_setup_new_game();
  //bool result3 = test_is_game_over();

  if(/*result1 &&*/ result2 /*&& result3*/ && true)
    printf(" \nAll tests in tests_axel are successfull.\n");

  else
    printf("\nOne or more tests have failed in tests_axel.\n");

  return EXIT_SUCCESS;
}
