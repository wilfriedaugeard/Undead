#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "game_io.h"
#include "game.h"



/**
* @brief : test the set_required_nb_seen (game.h) function.
* @return : return the test result as a boolean.
**/
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




/**
* @brief : test the set_required_nb_monsters (game.h) function.
* @return : return the test result as a boolean.
**/
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


/**
 * @brief : test the required_nb_seen (game.h) function.
 * @return : return the test result as a boolean.
 **/
bool test_required_nb_seen(){
  game game1 = new_game();
  assert(game1);
  bool ret = true;
  int m = 0; //mirror
  int v = 2; //HMIRROR
  int h = 3; //VMIRROR

  /*unvalid monster after a mirror*/
  add_monster(game1,10,1,0);
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
  /*spirit before mirror*/
  add_monster(game1,SPIRIT,0,0);
  if(current_nb_seen(game1,S,0)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a spirit is placed.\n");
    ret= false;
  }


  /*
    mirror placed on [0,0]
  */
  add_mirror(game1,m,0,0);

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
   /*spirit after a mirror*/
  add_monster(game1,SPIRIT,1,0);
  if(current_nb_seen(game1,S,0)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see a monster when a spirit is placed.\n");
    ret= false;
  }
  delete_game(game1);


  game game2 = new_game();
  assert(game1);

  /*
    Hmirror placed on [0,1]
  */
  add_mirror(game2,h,0,1);

  /*zombie before a Hmirror*/
  add_monster(game2,ZOMBIE,0,0);
  if(current_nb_seen(game2,S,0)!=2){
    printf("%d\n",current_nb_seen(game2,S,0));
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see 2 monsters when a zombie is placed before Hmirrors.\n");
    ret= false;
  }
  add_monster(game2,ZOMBIE,1,1);
  if(current_nb_seen(game2,E,1)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see 1 monster when a zombie is placed.\n");
    ret= false;
  }

  /*ghost before a Hmirror*/
  add_monster(game2,GHOST,0,0);
  if(current_nb_seen(game2,S,0)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see a monster when a ghost is placed before Hmirrors.\n");
    ret= false;
  }
  add_monster(game2,GHOST,1,1);
  if(current_nb_seen(game2,E,1)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a ghost is placed before Hmirrors.\n");
    ret= false;
  }
  /*vampire before a Hmirror*/
  add_monster(game2,VAMPIRE,0,0);
  if(current_nb_seen(game2,S,0)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see a monster when a vampire is placed before Hmirrors.\n");
    ret= false;
  }
  add_monster(game2,VAMPIRE,1,1);
  if(current_nb_seen(game2,E,1)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see a monster when a vampire is placed before Hmirrors.\n");
    ret= false;
  }
   /*spirit before a Hmirror*/
  add_monster(game2,SPIRIT,0,0);
  if(current_nb_seen(game2,S,0)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a spirit is placed.\n");
    ret= false;
  }
  add_monster(game2,SPIRIT,1,1);
  if(current_nb_seen(game2,E,1)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a spirit is placed.\n");
    ret= false;
  }


  /*
    Vmirror placed on [0,1]
  */
  add_mirror(game2,v,0,1);

  /*zombie before a Hmirror*/
  add_monster(game2,ZOMBIE,0,0);
  if(current_nb_seen(game2,S,0)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see 1 monster when a zombie is placed before Vmirrors.\n");
    ret= false;
  }
  add_monster(game2,ZOMBIE,1,1);
  if(current_nb_seen(game2,E,1)!=2){
    fprintf(stderr,"\nTest on required_nb_seen failed: we should see 2 monsters when a zombie is placed.\n");
    ret= false;
  }

  /*ghost before a Vmirror*/
  add_monster(game2,GHOST,0,0);
  if(current_nb_seen(game2,S,0)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a ghost is placed before Vmirrors.\n");
    ret= false;
  }
  add_monster(game2,GHOST,1,1);
  if(current_nb_seen(game2,E,1)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn see a monster when a ghost is placed before Vmirrors.\n");
    ret= false;
  }

  /*vampire before a Vmirror*/
  add_monster(game2,VAMPIRE,0,0);
  if(current_nb_seen(game2,S,0)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a vampire is placed before Vmirrors.\n");
    ret= false;
  }
  
  add_monster(game2,VAMPIRE,1,1);
  if(current_nb_seen(game2,E,1)!=1){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a vampire is placed before Vmirrors.\n");
    ret= false;
  }

   /*spirit before a Vmirror*/
  add_monster(game2,SPIRIT,0,0);
  if(current_nb_seen(game2,S,0)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a spirit is placed.\n");
    ret= false;
  }
  add_monster(game2,SPIRIT,1,1);
  if(current_nb_seen(game2,E,1)!=0){
    fprintf(stderr,"\nTest on required_nb_seen failed: we shouldn't see a monster when a spirit is placed.\n");
    ret= false;
  }

  if(ret)
    printf("Test on required_nb_seen successfull.\n");
  delete_game(game2);
  return ret;
}



/**
 * @brief : test the required_nb_monsters (game.h) function.
 * @return : return the test result as a boolean.
 **/
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
  /*set_required_nb_monsters(game1, GHOST,-1);
  if(required_nb_monsters(game1, GHOST)!=-1){
      fprintf(stderr,"\nTest on set_required_nb_monsters failed: we should have -1 ghost (strange).\n");
      ret= false;
  }*/
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
