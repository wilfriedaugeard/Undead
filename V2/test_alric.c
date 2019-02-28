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

  game original = new_game_ext(5, 5);

  add_monster(original, GHOST, 0, 0);
  add_monster(original, ZOMBIE, 1, 0);
  add_monster(original, VAMPIRE, 2, 0);
  add_monster(original, SPIRIT, 3, 0);

  add_mirror_ext(original, MIRROR, 0, 1);
  add_mirror_ext(original, ANTIMIRROR, 1, 1);
  add_mirror_ext(original, HMIRROR, 2, 1);
  add_mirror_ext(original, VMIRROR, 3, 1);

  set_required_nb_seen(original, N, 0, 1);
  set_required_nb_seen(original, S, 0, 1);
  set_required_nb_seen(original, W, 0, 1);
  set_required_nb_seen(original, E, 0, 1);

  game copy = NULL;
  copy = copy_game(original);

  if(copy == NULL)
  {
    fprintf(stderr, "Test on copy_game failed : the copied game is NULL.");
    delete_game(original);
    return false;
  }

  // Tests on size attributes
  bool sizeTest = true;
  if(game_width(copy) != game_width(original))
  {
    fprintf(stderr, "Test on copy_game failed : the width attribute is not correctly copied.");
    sizeTest = false;
    res = false;
  }
  if(game_height(copy) != game_height(original))
  {
    fprintf(stderr, "Test on copy_game failed : the height attribute is not correctly copied.");
    sizeTest = false;
    res = false;
  }

  // Tests on nb_monsters attributes
  if(required_nb_monsters(copy, GHOST) != required_nb_monsters(original, GHOST))
  {
    fprintf(stderr, "Test on copy_game failed : the nb_ghosts attribute is not correctly copied.");
    res = false;
  }
  if(required_nb_monsters(copy, ZOMBIE) != required_nb_monsters(original, ZOMBIE))
  {
    fprintf(stderr, "Test on copy_game failed : the nb_zombies attribute is not correctly copied.");
    res = false;
  }
  if(required_nb_monsters(copy, VAMPIRE) != required_nb_monsters(original, VAMPIRE))
  {
    fprintf(stderr, "Test on copy_game failed : the nb_vampires attribute is not correctly copied.");
    res = false;
  }
  if(required_nb_monsters(copy, SPIRIT) != required_nb_monsters(original, SPIRIT))
  {
    fprintf(stderr, "Test on copy_game failed : the nb_spirits attribute is not correctly copied.");
    res = false;
  }

  // Tests on labels
  if(required_nb_seen(original, N, 0) != required_nb_seen(copy, N, 0))
  {
    fprintf(stderr, "Test on copy_game failed : the northern nb_seen label is not correctly copied.\n");
    res = false;
  }
  if(required_nb_seen(original, S, 0) != required_nb_seen(copy, S, 0))
  {
    fprintf(stderr, "Test on copy_game failed : the  southern nb_seen label is not correctly copied.\n");
    res = false;
  }
  if(required_nb_seen(original, W, 0) != required_nb_seen(copy, W, 0))
  {
    fprintf(stderr, "Test on copy_game failed : the  western nb_seen label is not correctly copied.\n");
    res = false;
  }
  if(required_nb_seen(original, E, 0) != required_nb_seen(copy, E, 0))
  {
    fprintf(stderr, "Test on copy_game failed : the  eastern nb_seen label is not correctly copied.\n");
    res = false;
  }

  // Test on the board
  if(sizeTest)
  {
    for(int x = 0; x < game_width(original); x++)
    {
      for(int y = 0; y < game_height(original); y++)
      {
        if(get_content(copy, x, y) != get_content(original, x, y))
        {
          fprintf(stderr, "Test on copy_game failed : the [%d;%d] position on the board is not correcty copied.", x, y);
          res = false;
        }
      }
    }
  }

  if(res)
  {
    printf("Test on copy_game successfull.\n");
  }

  delete_game(original);
  delete_game(copy);

  return res;
}

/*
 * Function : test the delete_game (game.h) function.
 * Return : return the test result as a boolean.
 */
 bool test_delete_game()
 {
   game g = new_game_ext(5, 5);
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

  game g = new_game_ext(5, 5);

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
