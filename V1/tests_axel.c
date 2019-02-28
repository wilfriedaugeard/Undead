#include "game.h"
#include "game_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define WIDTH 4
#define HEIGHT 4

bool test_new_game() {
  game newGame = new_game();
  assert(newGame);
  bool ret = true;

  if (newGame == NULL){
    ret = false;
    printf("newGame not created\n");
  }

  else if (required_nb_monsters(newGame, GHOST) != 0 ||
           required_nb_monsters(newGame, VAMPIRE) != 0 ||
           required_nb_monsters(newGame, ZOMBIE) != 0){
    ret = false;
    printf("We shouldn't have monsters\n");
  }

  // Teste nombre de monstres
  else if (current_nb_monsters(newGame, GHOST) != 0 ||
           current_nb_monsters(newGame, VAMPIRE) != 0 ||
           current_nb_monsters(newGame, ZOMBIE) != 0){
    ret = false;
    printf("We shouldn't have monsters\n");
  }
  direction dirN = N;
  direction dirS = S;
  direction dirW = W;
  direction dirE = E;
  // Teste les labels et monstres presents N/S
  for (int i = 0; i < HEIGHT; i++) {
    if (required_nb_seen(newGame, dirN, i) != 0 ||
        required_nb_seen(newGame, dirS, i) != 0 ||
        current_nb_seen(newGame, dirN, i) != 0 ||
        current_nb_seen(newGame, dirS, i) != 0){
        ret = false;
      }
  }

  // Teste les labels et monstres presents E/W
  for (int i = 0; i < WIDTH; i++) {
    if (required_nb_seen(newGame, dirE, i) != 0 ||
        required_nb_seen(newGame, dirW, i) != 0 ||
        current_nb_seen(newGame, dirE, i) != 0 ||
        current_nb_seen(newGame, dirW, i) != 0){
        ret = false;
        printf("We should see any monsters on the board on East label or West label\n");
      }
  }

  // Teste si la grille générée est vide (ne retourne rien si case vide (0), retourne false sinon)
  for (int line = WIDTH-1; line >= 0; line--) {
    for (int col = 0; col < HEIGHT; col++) {

      int val = get_content(newGame, col, line);

      switch(val) {

        case 1:
          ret = false;
          printf("The board should be empty\n");
          break;

        case 2:
          ret = false;
          printf("The board should be empty\n");
          break;

        case GHOST:
          ret = false;
          printf("The board should be empty\n");
          break;

        case VAMPIRE:
          ret = false;
          printf("The board should be empty\n");
          break;

        case ZOMBIE:
          ret = false;
          printf("The board should be empty\n");
          break;
      }
    }
  }
  //printf("Test on Test_new_game successfull.\n");
  delete_game(newGame);
  return ret;
}

bool test_setup_new_game()
{
  game g=new_game();
  game newGame=g;
  bool res= true;
  int north[] = {0,0};
  int south[] = {0,1};
  int east[] = {1,0};
  int west[] = {0,0};

  int *labels[] = {north,south,east,west};
  content board[] = {EMPTY, MIRROR, ANTIMIRROR, VAMPIRE, GHOST, ZOMBIE};
  newGame = setup_new_game(labels, board, 2, 2, 5);
  assert(newGame);
  add_mirror(newGame, 0, 1, 0);
  add_mirror(newGame, 1, 1, 1);
  add_mirror(newGame, 1, 0, 1);
  add_monster(newGame, GHOST, 0, 0);

  if (!newGame)
  {
    fprintf(stderr,"test on setup_new_game failed: NULL game\n");
    res=false;
  }
  if (required_nb_monsters(newGame, ZOMBIE) != 5)
  {
    fprintf(stderr,"test on setup_new_game failed: incorrect nb zombies\n");
    res=false;
  }
  if (required_nb_monsters(newGame, GHOST) != 2)
  {
    fprintf(stderr,"test on setup_new_game failed: incorrect nb ghosts\n");
    res=false;
  }
  if (required_nb_monsters(newGame, VAMPIRE) != 2)
  {
    fprintf(stderr,"test on setup_new_game failed: incorrect nb vampires\n");
    res=false;
  }

  if (required_nb_seen(newGame, N, 0) != 0 ||
      required_nb_seen(newGame, N, 1) != 0)
  {
    fprintf(stderr,"test on setup_new_game failed : error on N\n");
    res=false;
  }
  if (required_nb_seen(newGame, S, 0) != 0 ||
      required_nb_seen(newGame, S, 1) != 1)
  {
    fprintf(stderr,"test on setup_new_game failed : error on S\n");
    res=false;
  }
  if (required_nb_seen(newGame, E, 0) != 1 ||
      required_nb_seen(newGame, E, 1) != 0)
  {
    fprintf(stderr,"test on setup_new_game failed : error on E\n");
    res=false;
  }
  if (required_nb_seen(newGame, W, 0) != 0 ||
      required_nb_seen(newGame, W, 1) != 0)
  {
    fprintf(stderr,"test on setup_new_game failed : error on W\n");
    res=false;
  }

  if (get_content(newGame, 0, 0) != GHOST ||
      get_content(newGame, 0, 1) != ANTIMIRROR ||
      get_content(newGame, 1, 0) != MIRROR ||
      get_content(newGame, 1, 1) != ANTIMIRROR)
  {
    printf("test on setup_new_game failed\n");
    res=false;
  }
  delete_game(newGame);
  delete_game(g);
  return res;
}


/*bool test_setup_new_game()
{
  game newGame=new_game();
  game g=newGame;
  bool res= true;
  int north[] = {0,0};
  int south[] = {0,1};
  int east[] = {1,0};
  int west[] = {0,0};

  int *labels[] = {north,south,east,west};
  content board[]={EMPTY, MIRROR, ANTIMIRROR, VAMPIRE, GHOST, ZOMBIE};
  g = setup_new_game(labels, board, 2, 2, 5);
  add_mirror(g, 0, 1, 0);
  add_mirror(g, 1, 1, 1);
  add_mirror(g, 1, 0, 1);
  add_monster(g, GHOST, 0, 0);

  if (!g)
  {
    fprintf(stderr,"Basic test on setup_new_game failed, NULL game\n");
    res=false;
  }
  if (required_nb_monsters(g, ZOMBIE) != 5)
  {
    fprintf(stderr,"Basic test on setup_new_game failed, incorrect nb zombies\n");
    res=false;
  }
  if (required_nb_monsters(g, GHOST) != 2)
  {
    fprintf(stderr,"Basic test on setup_new_game failed, incorrect nb ghosts\n");
    res=false;
  }
  if (required_nb_monsters(g, VAMPIRE) != 2)
  {
    fprintf(stderr,"Basic test on setup_new_game failed, incorrect nb vampires\n");
    res=false;
  }

  if (required_nb_seen(g, N, 0) != 0 ||
      required_nb_seen(g, N, 1) != 0)
  {
    fprintf(stderr,"Basic test on setup_new_game failed : error on N\n");
    res=false;
  }
  if (required_nb_seen(g, S, 0) != 0 ||
      required_nb_seen(g, S, 1) != 1)
  {
    fprintf(stderr,"Basic test on setup_new_game failed : error on S\n");
    res=false;
  }
  if (required_nb_seen(g, E, 0) != 1 ||
      required_nb_seen(g, E, 1) != 0)
  {
    fprintf(stderr,"Basic test on setup_new_game failed : error on E\n");
    res=false;
  }
  if (required_nb_seen(g, W, 0) != 0 ||
      required_nb_seen(g, W, 1) != 0)
  {
    fprintf(stderr,"Basic test on setup_new_game failed : error on W\n");
    res=false;
  }

  if (get_content(g, 0, 0) != GHOST ||
      get_content(g, 0, 1) != ANTIMIRROR ||
      get_content(g, 1, 0) != MIRROR ||
      get_content(g, 1, 1) != ANTIMIRROR)
  {
    fprintf(stderr,"Basic test on setup_new_game failed, false grid\n");
    res=false;
  }
  delete_game(g);
  delete_game(newGame);
  return res;
}*/



bool test_is_game_over() {
  game newGame = new_game();
  assert(newGame);
  bool ret = true;
  direction dirN = N;
  direction dirS = S;
  direction dirW = W;
  direction dirE = E;


  // Teste les labels et monstres presents N/S
  for (int i = 0; i < HEIGHT; i++) {
    if ((required_nb_seen(newGame, dirN, i) != current_nb_seen(newGame, dirN, i) ||
        required_nb_seen(newGame, dirS, i) != current_nb_seen(newGame, dirS, i)) &&
        is_game_over(newGame) == true)
        ret = false;
  }

  // Teste les labels et monstres presents E/W
  for (int i = 0; i < WIDTH; i++) {
    if ((required_nb_seen(newGame, dirE, i) != current_nb_seen(newGame, dirE, i) ||
        required_nb_seen(newGame, dirW, i) != current_nb_seen(newGame, dirW, i)) &&
        is_game_over(newGame) == true)
        ret = false;
  }

  delete_game(newGame);
  return ret;
}

int main(void) {

  bool result1 = test_new_game();

  bool result2 = test_setup_new_game();

  bool result3 = test_is_game_over();
  if(result1 && result2 && result3 && true)
    printf(" \nAll tests in tests_axel are successfull.\n");
  else
    printf("\nOne or more tests have failed in tests_axel.\n");
  return EXIT_SUCCESS;
}
