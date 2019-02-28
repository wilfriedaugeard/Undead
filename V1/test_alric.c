#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "game_io.h"
#include "game.h"

/*
 * Function : test the copy_game (game.h) function.
 * Return : return the test result as a boolean.
 */
bool test_copy_game()
{
  bool res = true;

  game original = new_game();
  assert(original);
  add_monster(original, GHOST, 0, 0);
  add_monster(original, ZOMBIE, 1, 0);
  add_monster(original, VAMPIRE, 2, 0);
  add_mirror(original, 0, 0, 1);
  add_mirror(original, 1, 1, 1);
  set_required_nb_seen(original, N, 0, 1);
  set_required_nb_seen(original, S, 0, 1);
  set_required_nb_seen(original, W, 0, 1);
  set_required_nb_seen(original, E, 0, 1);

  game copy = NULL;
  copy = copy_game(original);

  if(copy != NULL)
  {
    if(get_content(copy, 0, 0) != GHOST)
    {
      fprintf(stderr, "Test on copy_game failed : the monster \"GHOST\" is not correctly copied.\n");
      res = false;
    }
    if(get_content(copy, 1, 0) != ZOMBIE)
    {
      fprintf(stderr, "Test on copy_game failed : the monster \"ZOMBIE\" is not correctly copied.\n");
      res = false;
    }
    if(get_content(copy, 2, 0) != VAMPIRE)
    {
      fprintf(stderr, "Test on copy_game failed : the monster \"VAMPIRE\" is not correctly copied.\n");
      res = false;
    }
    if(get_content(copy, 0, 1) != MIRROR)
    {
      fprintf(stderr, "Test on copy_game failed : the \"MIRROR\" is not correctly copied.\n");
      res = false;
    }
    if(get_content(copy, 1, 1) != ANTIMIRROR)
    {
      fprintf(stderr, "Test on copy_game failed : the \"ANTIMIRROR\" is not correctly copied.\n");
      res = false;
    }
    if(required_nb_seen(original, N, 0) != 1)
    {
      fprintf(stderr, "Test on copy_game failed : the northern nb_seen label is not correctly copied.\n");
      res = false;
    }
    if(required_nb_seen(original, S, 0) != 1)
    {
      fprintf(stderr, "Test on copy_game failed : the  southern nb_seen label is not correctly copied.\n");
      res = false;
    }
    if(required_nb_seen(original, W, 0) != 1)
    {
      fprintf(stderr, "Test on copy_game failed : the  western nb_seen label is not correctly copied.\n");
      res = false;
    }
    if(required_nb_seen(original, E, 0) != 1)
    {
      fprintf(stderr, "Test on copy_game failed : the  eastern nb_seen label is not correctly copied.\n");
      res = false;
    }
    delete_game(copy);
  }
  else
  {
    fprintf(stderr, "Test on copy_game failed : the copy object content is NULL.\n");
    res = false;
  }

  if(res)
  {
    printf("Test on copy_game successfull.\n");
  }

  delete_game(original);

  return res;
}

/*
 * Function : test the delete_game (game.h) function.
 * Return : return the test result as a boolean.
 */
 bool test_delete_game()
 {
   game g = new_game();
   delete_game(g);
   printf("For the delete_game test see Valgrind.\n");

   return true;
 }

/*
 * Function : test the restart_game (game.h) function.
 * Return : return the test result as a boolean.
 */
bool test_restart_game()
{
  bool res = true;

  game g = new_game();
  assert(g);
  add_monster(g, GHOST, 0, 0);
  add_mirror(g, 0, 1, 0);
  set_required_nb_seen(g, N, 0, 1);
  set_required_nb_seen(g, S, 0, 1);
  set_required_nb_seen(g, W, 0, 1);
  set_required_nb_seen(g, E, 0, 1);

  restart_game(g);

  if(get_content(g, 0, 0) != EMPTY)
  {
    fprintf(stderr, "Test on restart_game failed : the position [0;0] of the board is not empty.\n");
    res = false;
  }
  if(get_content(g, 1, 0) != MIRROR)
  {
    fprintf(stderr, "Test on restart_game failed : the position [1;0] of the board is not a \"MIRROR\"\n");
    res = false;
  }
  if(required_nb_seen(g, N, 0) != 1)
  {
    fprintf(stderr, "Test on restart_game failed : the northern nb_seen label has not correctly restart.\n");
    res = false;
  }
  if(required_nb_seen(g, S, 0) != 1)
  {
    fprintf(stderr, "Test on restart_game failed : the  southern nb_seen label has not correctly restart.\n");
    res = false;
  }
  if(required_nb_seen(g, W, 0) != 1)
  {
    fprintf(stderr, "Test on restart_game failed : the western nb_seen label has not correctly restart.\n");
    res = false;
  }
  if(required_nb_seen(g, E, 0) != 1)
  {
    fprintf(stderr, "Test on restart_game failed : the eastern nb_seen label has not correctly restart.\n");
    res = false;
  }
  if(res)
  {
    printf("Test on restart_game successfull.\n");
  }

  delete_game(g);

  return res;
}

/*
 * main
 */
int main()
{
  printf("--------------------\n");
  bool result = true;

  result = test_copy_game() && result;
  result = test_delete_game() && result;
  result = test_restart_game() && result;

  if(!result)
  {
    printf("\nOne or more tests have failed in test_alric.\n");
    printf("--------------------\n\n");
    return EXIT_SUCCESS;
  }

  printf("\nAll tests in test_alric are successfull.\n");
  printf("--------------------\n\n");

  return EXIT_SUCCESS;
}
