#include "game.h"
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 4
#define HEIGHT 4

typedef struct game_s
{
  int nb_ghosts, nb_zombies, nb_vampires;
  int* labels[NB_DIR];
  content* board;
  int width;
  int height;
} game_s;

/**
  * Implementation of the "new_game" function. See game.h for more infos.
  **/
game new_game ()
{
  game g = (game) malloc(sizeof(game_s));
	if(g == NULL)
	{
		fprintf(stderr, "Error in \"new_game\" the game is NULL.");
		exit(EXIT_FAILURE);
	}

	g->board = (content*) calloc(HEIGHT*WIDTH, sizeof(int));
	if(g->board == NULL)
	{
		fprintf(stderr, "Error in \"new_game\" the board is NULL");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < NB_DIR; i++)
	{
		if(i == N || i == S)
		{
			g->labels[i] = (int*) calloc(WIDTH, sizeof(int));
		}
		else
		{
			g->labels[i] = (int*) calloc(HEIGHT, sizeof(int));
		}

		if(g->labels[i] == NULL)
		{
			fprintf(stderr, "Error in \"new_game\" the labels[%d] is NULL.", i);
			exit(EXIT_FAILURE);
		}
	}

  g->nb_ghosts = 0;
	g->nb_vampires = 0;
	g->nb_zombies = 0;

	g->width = WIDTH;
	g->height = HEIGHT;

  return g;
}

/**
	* Implementation of the "copy_game" function. See game.h for more infos.
	**/
game copy_game (cgame g_src)
{
  game copy = new_game();

  copy->nb_ghosts = g_src->nb_ghosts;
  copy->nb_vampires = g_src->nb_vampires;
  copy->nb_zombies = g_src->nb_zombies;

	copy->width = g_src->width;
	copy->height = g_src->height;

  for(int i = 0; i < g_src->width*g_src->height; i++)
	{
    copy->board[i] = g_src->board[i];
  }

	for(int i = 0; i < NB_DIR; i++)
	{
		if(i == N || i == S)
		{
			for(int j = 0; j < g_src->width; j++)
			{
				copy->labels[i][j] = g_src->labels[i][j];
			}
		}
		else
		{
			for(int j = 0; j < g_src->height; j++)
			{
				copy->labels[i][j] = g_src->labels[i][j];
			}
		}
	}

  return copy;
}

/**
	* Implementation of the "delete_game" function. See game.h for more infos.
	**/
void delete_game(game g)
{
  if(g==NULL){
    return;
  }
  if(g->board!=NULL){
    free(g->board);
    g->board = NULL;
  }


  for(int i = 0; i < NB_DIR; i++)
	{
    if(g->labels[i]!=NULL){
      free(g->labels[i]);
      g->labels[i] = NULL;
    }
  }

	free(g);
	g= NULL;
}

/**
	* Implementation of the "stup_new_game" function. See game.h for more infos.
	**/
game setup_new_game(int* labels[NB_DIR], content* board, int required_nb_ghosts, int required_nb_vampires, int required_nb_zombies)
{
	game g = new_game();

	g->nb_ghosts = required_nb_ghosts;
	g->nb_vampires = required_nb_vampires;
	g->nb_zombies = required_nb_zombies;

	for(int i = 0; i < g->width*g->height; i++)
	{
			g->board[i] = board[i];
	}

	for(int i = 0; i < NB_DIR; i++)
	{
		g->labels[i] = labels[i];
	}

	return g;
}

/**
  * Implementation of the "restart_game" function. See game.h for more infos.
  **/
void restart_game(game g)
{
  if(g == NULL)
  {
    fprintf(stderr, "Unvalid pointer g in \"restart_game\".\n");
    exit(EXIT_FAILURE);
  }

  for(int i = 0; i < g->height*g->width; i++)
  {
    if(g->board[i] != ANTIMIRROR && g->board[i] != MIRROR)
      g->board[i] = EMPTY;
  }
}

/**
  * Implementation of "get_content" function. See game.h for more infos.
  **/
content get_content(cgame game, int col, int line)
{
  if(game == NULL)
  {
    fprintf(stderr, "Unvalid pointer game in \"get_content\".\n");
    exit(EXIT_FAILURE);
  }

  if(col < 0 || col >= WIDTH || line < 0 || line >= HEIGHT)
  {
    printf("Value of col or line should be positive.\n");
    exit(EXIT_FAILURE);
  }
  int position = WIDTH * line + col;
  return (game->board[position]);
}

/**
  * Implementation of the "is_game_over" function. See game.h for more infos.
  **/
bool is_game_over(cgame g)
{
  if(g == NULL)
  {
    fprintf(stderr, "Unvalid pointer g in \"is_game_over\".\n");
    exit(EXIT_FAILURE);
  }

  // Tests if the board is fulfilled
  for(int i = 0; i < HEIGHT*WIDTH; i++)
  {
    if(g->board[i] == EMPTY)
      return false;
  }

  direction dirN = N;
  direction dirS = S;
  direction dirW = W;
  direction dirE = E;

  // Teste les labels et monstres presents N/S
  for (int i = 0; i < HEIGHT; i++)
  {
    if ((required_nb_seen(g, dirN, i) != current_nb_seen(g, dirN, i) ||
        required_nb_seen(g, dirS, i) != current_nb_seen(g, dirS, i)))
        return false;
  }

  // Teste les labels et monstres presents E/W
  for (int i = 0; i < WIDTH; i++)
  {
    if ((required_nb_seen(g, dirE, i) != current_nb_seen(g, dirE, i) ||
        required_nb_seen(g, dirW, i) != current_nb_seen(g, dirW, i)))
        return false;
  }

  return true;
}

/**
	* Implementation of "add_monster" function. See game.h for more infos.
	**/
bool add_monster(game game, content monster, int col, int line)
{
	if (game == NULL) {
		fprintf(stderr, "Error in \"add_monster\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if (monster != GHOST && monster != VAMPIRE && monster != ZOMBIE && monster != EMPTY) {
		printf("Not a monster.\n");
		return false;
	}

	if (col >= game->width || line >= game->height || col < 0 || line < 0) {
		printf("Out of the board.\n");
		return false;
	}

	unsigned int position = line * game->width + col;

	if (game->board[position] == MIRROR || game->board[position] == ANTIMIRROR) {
		printf("Position already used by MIRROR or ANTIMIRROR.\n");
		return false;
	}

	game->board[position] = monster;

	return true;
}

/**
	* Implementation of the "add_mirror" function. See game.h for more infos.
	**/
void add_mirror(game game, int dir, int col, int line)
{
	if (game == NULL)
	{
		fprintf(stderr, "Error in \"add_mirror\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if (dir > 1 || dir < 0)
	{
		printf("Not a mirror.\n");
		return;
	}

	unsigned int position = line * game->width + col;
	game->board[position] = (dir) ? ANTIMIRROR : MIRROR; // if dir = 1 (true) => ANTIMIRROR - if dir = 0 (false) => MIRROR
}

/**
	* Implementation of the "current_nb_monsters" function. See game.h for more infos.
	**/
int current_nb_monsters(cgame game, content monster)
{
	if (game == NULL)
	{
		fprintf(stderr, "Error in \"current_nb_monsters\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if (monster != GHOST && monster != VAMPIRE && monster != ZOMBIE)
	{
		printf ("Not a monster.\n");
		return -1;
	}

	int cpt = 0;

	for (int i = 0; i < game->width*game->height; i++)
	{
		if (game->board[i] == monster) cpt++;
	}

	return cpt;
}

/**
	* Implementation of the "current_nb_seen" function. See game.h for more infos.
	**/
int current_nb_seen(cgame game, direction side, int pos)
{
	if(game == NULL)
	{
		fprintf(stderr, "Error in \"current_nb_seen\" the game is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if(pos < 0)
	{
		if(side == N || side == S)
		{
			if(pos > game->width)
				return -1;
		}
		else
		{
			if(pos > game->height)
				return -1;
		}
	}

  int x = 0;
  int y = 0;
  if(side == N || side == S)
  {
    x = pos;
    y = (side == N) ? game->height-1 : 0;
  }
  else
  {
    x = (side == E) ? game->width-1 : 0;
    y = pos;
  }

  int cptMonster = 0;
  content currentContent;
  bool mirrorPassed = false;
  direction currentDir = (side == N || side == E) ? side+1 : side-1;

  bool outOfBound = false;
  while(!outOfBound)
  {
    currentContent = get_content(game, x, y);

    switch(currentContent)
    {
      case ZOMBIE: { cptMonster++; break; }
      case GHOST:
      {
        if(mirrorPassed)
          cptMonster++;
        break;
      }
      case VAMPIRE:
      {
        if(!mirrorPassed)
          cptMonster++;
        break;
      }
      case MIRROR:
      case ANTIMIRROR: mirrorPassed = true;
      default: break;
    }

    if(currentContent == ZOMBIE || currentContent == VAMPIRE || currentContent == GHOST || currentContent == EMPTY)
    {
      switch(currentDir)
      {
        case N: y++; break;
        case S: y--; break;
        case E: x++; break;
        case W: x--; break;
        default: break;
      }
    }
    else
    {
      if(currentContent == MIRROR)
      {
        switch(currentDir)
        {
          case N: { currentDir = E; x++; break; }
          case E: { currentDir = N; y++; break; }
          case S: { currentDir = W; x--; break; }
          case W: { currentDir = S; y--; break; }
          default: break;
        }
      }
      else
      {
        switch(currentDir)
        {
          case N: { currentDir = W; x--; break; }
          case W: { currentDir = N; y++; break; }
          case S: { currentDir = E; x++; break; }
          case E: { currentDir = S; y--; break; }
          default: break;
        }
      }
    }

    if(x < 0 || x > game->width-1 || y < 0 || y > game->height-1)
      outOfBound = true;
  }

  return cptMonster;
}

/**
  * Implementation of the "set_required_nb_seen" function. See game.h for more infos.
  **/
void set_required_nb_seen(game game, direction side, int pos, int value)
{
  if(game == NULL)
  {
    fprintf(stderr, "Error in \"set_required_nb_seen\" the game is NULL.\n");
    exit(EXIT_FAILURE);
  }

  if(value >= 0)
  {
    if(side == N || side == S)
    {
      if(pos < game->width && pos >= 0)
        game->labels[side][pos] = value;
    }

    if(side == E || side == W)
    {
      if(pos < game->height && pos >= 0)
        game->labels[side][pos] = value;
    }
  }
}

/**
  * Implementation of the "set_required_nb_monsters" function. See game.h for mor infos.
  **/
void set_required_nb_monsters(game game, content monster, int value)
{
  if(game == NULL)
  {
    fprintf(stderr, "Error in \"set_required_nb_monsters\" the game is NULL.\n");
    exit(EXIT_FAILURE);
  }

  // Tests if the value is negative.
  if(value >= 0)
  {
    // For each case, tests if the number of monsters (with the new value) is bigger than the number of squares board.
    switch(monster)
    {
      case GHOST:
      {
        if((value + game->nb_zombies + game->nb_vampires) <= (game->width * game->height))
          game->nb_ghosts = value;
        break;
      }
      case ZOMBIE:
      {
        if((value + game->nb_ghosts + game->nb_vampires) <= (game->width * game->height))
          game->nb_zombies = value;
        break;
      }
      case VAMPIRE:
      {
        if((value + game->nb_zombies + game->nb_ghosts) <= (game->width * game->height))
          game->nb_vampires = value;
        break;
      }
      default: break;
    }
  }
}

/**
  * Implementation of the "required_nb_seen" function. See game.h for mor infos.
  **/
int required_nb_seen(cgame game, direction side, int pos)
{
  if(game == NULL)
  {
    fprintf(stderr, "Error in \"required_nb_seen\" the game is NULL.\n");
    exit(EXIT_FAILURE);
  }

  if(side == N || side == S)
  {
    if(pos < game->width && pos >= 0)
      return game->labels[side][pos];
  }

  if(side == E || side == W)
  {
    if(pos < game->height && pos >= 0)
      return game->labels[side][pos];
  }

  // Error code
  return -1;
}

/**
  * Implementation of the "required_nb_monsters" function. See game.h for more infos.
  **/
int required_nb_monsters(cgame game, content monster)
{
  if(game == NULL)
  {
    fprintf(stderr, "Error in \"required_nb_monsters\" the game is NULL.\n");
    exit(EXIT_FAILURE);
  }

  switch(monster)
  {
    case GHOST: return game->nb_ghosts; break;
    case ZOMBIE: return game->nb_zombies; break;
    case VAMPIRE: return game->nb_vampires; break;
    default: break;
  }

  // Error code
  return -1;
}
