#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "game_io.h"
#include "game.h"
#define WIDTH 4
#define HEIGHT 4

/*
 * Function : test the set_required_nb_seen (game.h) function.
 * Return : return the test result as a boolean.
 */
bool test_set_required_nb_seen(){
  game game1 = new_game();
  assert(game1);
  bool ret = true;
  set_required_nb_seen(game1, N,0,0);
  if(current_nb_seen(game1, N,0)!=0){
      fprintf(stderr,"\nTest on set_required_nb_seen failed: square on [0,0] is not empty.\n");
      ret = false;
  }
  else{
    printf("Test on set_required_nb_seen successfull.\n");
  }
  delete_game(game1);
  return ret;
}




/*
 * Function : test the set_required_nb_monsters (game.h) function.
 * Return : return the test result as a boolean.
 */
bool test_set_required_nb_monsters(){
  game game1 = new_game();
  assert(game1);
  bool ret = true;
  set_required_nb_monsters(game1, GHOST,3);
  if(required_nb_monsters(game1, GHOST)!=3){
      fprintf(stderr,"\nTest on set_required_nb_monsters failed: we should see 3 Ghosts.\n");
      ret= false;
  }
  else{
    printf("Test on set_required_nb_monsters successfull.\n");
  }
  delete_game(game1);
  return ret;
}


/*
 * Function : test the required_nb_seen (game.h) function.
 * Return : return the test result as a boolean.
 */
bool test_required_nb_seen(){
  game game1 = new_game();
  assert(game1);
  bool ret = true;
  /*unvalid monster after a mirror*/
  add_monster(game1,4,1,0);
  if(current_nb_seen(game1,S,0)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when an unvalid monster is placed.\n");
    ret= false;
  }
  /*zombie before mirror*/
  add_monster(game1,ZOMBIE,0,0);
  if(current_nb_seen(game1,S,0)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see a monster when a zombie is placed before mirrors.\n");
    ret= false;
  }
  /*ghost before mirror*/
  add_monster(game1,GHOST,0,0);
  if(current_nb_seen(game1,S,0)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see monsters when a ghost is placed before mirrors.\n");
    ret= false;
  }
  /*vampire before mirror*/
  add_monster(game1,VAMPIRE,0,0);
  if(current_nb_seen(game1,S,0)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see a monster when a vampire is placed before mirrors.\n");
    ret= false;
  }

  /*mirror placed on [0,0]*/
  add_mirror(game1,0,0,0);

  /*zombie after a mirror*/
  add_monster(game1,ZOMBIE,1,0);
  if(current_nb_seen(game1,S,0)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see a monster when a zombie is placed after mirrors.\n");
    ret= false;
  }
  /*ghost after a mirror*/
  add_monster(game1,GHOST,1,0);
  if(current_nb_seen(game1,S,0)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a ghost is placed after mirrors.\n");
    ret= false;
  }
  /*vampire after a mirror*/
  add_monster(game1,VAMPIRE,1,0);
  if(current_nb_seen(game1,S,0)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see a monster when a vampire is placed after mirrors.\n");
    ret= false;
  }
  if(ret)
    printf("Test on required_nb_seen successfull.\n");
  delete_game(game1);
  return ret;
}



/*
 * Function : test the required_nb_monsters (game.h) function.
 * Return : return the test result as a boolean.
 */
bool test_required_nb_monsters(){
  game game1=new_game();
  assert(game1);
  bool ret = true;
  /*test with uninitialized monsters*/
  if(required_nb_monsters(game1, GHOST)!=0){
      fprintf(stderr,"\nTest on set_required_nb_monsters failed: we should have 0 ghost.\n");
      ret= false;
  }
  if(required_nb_monsters(game1, VAMPIRE)!=0){
      fprintf(stderr,"\nTest on set_required_nb_monsters failed: we should have 0 vampire.\n");
      ret= false;
  }
  if(required_nb_monsters(game1, ZOMBIE)!=0){
      fprintf(stderr,"\nTest on set_required_nb_monsters failed: we should have 0 zombie.\n");
      ret= false;
  }

  /*test with initialized monsters*/
  set_required_nb_monsters(game1, GHOST,1);
  set_required_nb_monsters(game1, ZOMBIE,1);
  set_required_nb_monsters(game1,VAMPIRE,1);

  if(required_nb_monsters(game1, GHOST)!=1){
      fprintf(stderr,"\nTest on set_required_nb_monsters failed: we should have 1 ghost.\n");
      ret= false;
  }
  if(required_nb_monsters(game1, ZOMBIE)!=1){
      fprintf(stderr,"\nTest on set_required_nb_monsters failed: we should have 1 zombie.\n");
      ret= false;
  }
  if(required_nb_monsters(game1, VAMPIRE)!=1){
      fprintf(stderr,"\nTest on set_required_nb_monsters failed: we should have 1 vampire.\n");
      ret= false;
  }
  /*test with negativ number*/
  set_required_nb_monsters(game1, GHOST,-1);
  if(required_nb_monsters(game1, GHOST)!=-1){
      fprintf(stderr,"\nTest on set_required_nb_monsters failed: we should have -1 ghost (strange).\n");
      ret= false;
  }
  if(ret)
    printf("Test on required_nb_monsters successfull.\n");
  delete_game(game1);
  return ret;
}



int main(void){

  bool result1 = test_set_required_nb_seen();
  bool result2 = test_set_required_nb_monsters();
  bool result3 = test_required_nb_seen();
  bool result4 = test_required_nb_monsters();
  if(result1 && result2 && result3 && result4 && true)
    printf(" \nAll tests in test_wil are successfull.\n");
  else
    printf("\nOne or more tests have failed in test_wil.\n");
  return EXIT_SUCCESS;
}
