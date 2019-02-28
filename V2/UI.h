#ifndef MODEL_H
#define MODEL_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "game_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include <time.h>
#include "undead_text.h"

typedef struct Env_t Env;

/* **************************************************************** */

#ifdef __ANDROID__
#define PRINT(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__);  } while(0)
#define ERROR(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#else
#define PRINT(STR, ...) do { printf(STR, ##__VA_ARGS__); } while(0)
#define ERROR(STR, ...) do { fprintf(stderr, STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#endif

/* **************************************************************** */

#define APP_NAME "Undead Graphic"
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480
#define DELAY 50

/* **************************************************************** */

Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]);

void render(SDL_Window* win, SDL_Renderer* ren, Env * env);
void render_rules(SDL_Window* win, SDL_Renderer* ren, Env * env);
void render_game(SDL_Window* win, SDL_Renderer* ren, Env * env);
void render_menu(SDL_Window* win,  SDL_Renderer* ren, Env * env);
void animation(SDL_Window* win, SDL_Renderer* ren, Env* env);
void won(SDL_Window* win, SDL_Renderer* ren, Env * env);

bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e);
bool process_won(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e);
bool process_menu(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e);
bool process_game(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e);
bool process_anim(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e);

void clean(SDL_Window* win, SDL_Renderer* ren, Env * env);
bool collision (int x, int y, int x2, int y2, int w2, int h2);
game _game(Env* env);
void _state(Env* env,int x);
int get_state(Env* env);

/* **************************************************************** */

#endif
