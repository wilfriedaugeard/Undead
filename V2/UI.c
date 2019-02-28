#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "UI.h"
#include "game.h"
#include "undead_text.h"
#include "game_io.c"
#include <time.h>
#include <string.h>


#define FONT "./font/Arial.ttf"
#define FONT_MENU "./font/AdamGorry-Inline.ttf"

/* **************************************************************** */

//Initialise structure
struct Env_t {
    SDL_Texture* ghost;
    SDL_Texture* zombie;
    SDL_Texture* vampire;
    SDL_Texture* mirror;
    SDL_Texture* antimirror;
    SDL_Texture* spirit;
    SDL_Texture* hmirror;
    SDL_Texture* vmirror;
    SDL_Texture* won;
    SDL_Texture* text;
    SDL_Texture* background;
    SDL_Texture* empty;
    SDL_Texture* reset;
    SDL_Texture* rules;
    SDL_Texture* play_buttom;

    int game_width, game_height;
    int fontsize;
    content choice;
    game game1;
    int level;
    int state; //0-> menu 1->jeu 2->regle 3->won -1->animation
    int grille_x, grille_y;//grille menu
    int nb_g, nb_z, nb_v, nb_s;
    int w, h, coef; //w: width of map, h: height of map, coef: W and height of one case
    int anim;
    int operation_anim; //-1-> -; 1-> +
    int g_anim; //monster position in animation
    int m; //monster choose in animation
};

/* **************************************************************** */

//Load textures into env structure
Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]) {
    Env * env = malloc(sizeof(struct Env_t));

    env->ghost = IMG_LoadTexture(ren, "./textures/ghost.png");
    if (!env->ghost)
        ERROR("IMG_LoadTexture: ghost.png\n");

    env->zombie = IMG_LoadTexture(ren, "./textures/zombie.png");
    if (!env->zombie)
        ERROR("IMG_LoadTexture: zombie.png");

    env->vampire = IMG_LoadTexture(ren, "./textures/vampire.png");
    if (!env->vampire)
        ERROR("IMG_LoadTexture: vampire.png");

    env->mirror = IMG_LoadTexture(ren, "./textures/mirror.png");
    if (!env->mirror)
        ERROR("IMG_LoadTexture: mirror.png");

    env->antimirror = IMG_LoadTexture(ren, "./textures/antimirror.png");
    if (!env->antimirror)
        ERROR("IMG_LoadTexture: antimirror.png");

    env->hmirror = IMG_LoadTexture(ren, "./textures/hmirror.png");
    if (!env->hmirror)
        ERROR("IMG_LoadTexture: hmirror.png");

    env->vmirror = IMG_LoadTexture(ren, "./textures/vmirror.png");
    if (!env->vmirror)
        ERROR("IMG_LoadTexture: vmirror.png");

    env->spirit = IMG_LoadTexture(ren, "./textures/spirit.png");
    if (!env->spirit)
        ERROR("IMG_LoadTexture: antimirror.png");

    env->won = IMG_LoadTexture(ren, "./textures/youwon.png");
    if (!env->won)
        ERROR("IMG_LoadTexture: youwon.png");

    env->background = IMG_LoadTexture(ren, "./textures/background.png");
    if (!env->background)
        ERROR("IMG_LoadTexture: background.png");

    env->empty = IMG_LoadTexture(ren, "./textures/empty.png");
    if (!env->empty)
        ERROR("IMG_LoadTexture: empty.png");

    env->reset = IMG_LoadTexture(ren, "./textures/reset.png");
    if (!env->reset)
        ERROR("IMG_LoadTexture: reset.png");

    env->rules = IMG_LoadTexture(ren, "./textures/rules.png");
    if (!env->rules)
        ERROR("IMG_LoadTexture: rules.png");

    env->play_buttom = IMG_LoadTexture(ren, "./textures/play.png");
    if (!env->play_buttom)
        ERROR("IMG_LoadTexture play.png");

    if(argc == 2)
    env->game1 = load_game(argv[1]);

    env->choice = EMPTY;
    env->state = -1;
    env->grille_x = 4;
    env->grille_y = 5;
    env->anim = 0;
    env->operation_anim = 1;
    env->g_anim = 0;
    env->m = 0;

    return env;
}

/* **************************************************************** */

/**
 * @brief show the animation at begin of the game
 * @param win the window of your game
 * @param ren the renderer of your game 
 * @param env an environnement which contains all textures   
 **/
void animation(SDL_Window* win,SDL_Renderer* ren,Env* env){
    //Initialise an array which contains all used textures
    SDL_Texture* textureArray[] = {
        env->ghost,
        env->zombie,
        env->vampire,
        env->spirit,
    };
    
    SDL_GetWindowSize(win, &env->w, &env->h);
    //background
    SDL_RenderCopy(ren, env->background, NULL, NULL);

    //posx, posy: coordonées du début de la grille
    int posx = env->w * 0.22;
    int posy = env->h * 0.22;

    // Coef will take a value which it depends of the minimum between height & width
    if (posx>posy)
        env->coef = (env->h * 0.6);
    else
        env->coef = (env->w * 0.6);


    //play buttom
    int play_x = env->coef/2;
    int play_y = env->coef/2;
    SDL_Rect rect_play = {env->w/2-play_x/2, env->h/3-play_y/2, 0, 0};
    SDL_QueryTexture(env->play_buttom, NULL, NULL, &rect_play.w, &rect_play.h);
    rect_play.w = play_x;
    rect_play.h = play_y;
    SDL_RenderCopy(ren, env->play_buttom, NULL, &rect_play);

    //ghost animation
    if(env->g_anim>env->w){
        env->g_anim = -env->coef/2;
        if(env->m==3) env->m=0;
        else{
            env->m++;
        }
    } 
    SDL_Rect rectg = {env->g_anim, env->h/2+play_y/2, 0, 0};
    SDL_QueryTexture(textureArray[env->m], NULL, NULL, &rectg.w, &rectg.h);
    rectg.w = env->coef/2;
    rectg.h = env->coef/2;
    SDL_RenderCopy(ren, textureArray[env->m], NULL, &rectg);
    env->g_anim+=3;

}


/**
* @brief print the image 'you won!'
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
**/
void won (SDL_Window* win, SDL_Renderer* ren, Env* env) {
    //fond
    SDL_RenderCopy(ren, env->background, NULL, NULL);
    //you won
    SDL_RenderCopy(ren, env->won, NULL, NULL);
    env->state = 3;
    env->choice = EMPTY;
}

/**
* @brief display rules
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
**/
void render_rules(SDL_Window* win,  SDL_Renderer* ren, Env * env) {
    SDL_GetWindowSize(win, &env->w, &env->h);

    //background
    SDL_RenderCopy(ren, env->background, NULL, NULL);

    if (env->w > env->h)
        env->coef = (env->h * 0.6) / (env->game_height);
    else
        env->coef = (env->w * 0.6) / (env->game_width);

    //TEXTE NUMERO LEVEL
    env->fontsize = env->coef / 4;
    SDL_Color blue = { 36, 113, 134, 255 };

    //text "RULES"
    TTF_Font * rules = TTF_OpenFont(FONT_MENU, env->fontsize + 20);
    SDL_Surface * surf = TTF_RenderText_Blended(rules, "RULES", blue);
    env->text = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect rect;
    SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
    int size_w;
    int size_h;
    TTF_SizeText(rules, "RULES", &size_w, &size_h);
    rect.x = (env->w/2) - (size_w/2);
    rect.y = 0;
    SDL_RenderCopy(ren, env->text, NULL, &rect);
    SDL_FreeSurface(surf);
    
    //text
    SDL_Rect rectangle = {0, size_h*2, 0, 0};
    SDL_QueryTexture(env->rules, NULL, NULL, &rectangle.w, &rectangle.h);
    rectangle.w = env->w;
    rectangle.h = env->h;
    SDL_RenderCopy(ren, env->rules, NULL, &rectangle);

}


/**
* @brief display the game
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
**/

void render_game(SDL_Window* win,  SDL_Renderer* ren, Env * env) {
    //Initialise an array which contains all used textures
    SDL_Texture* textureArray[] = {
        env->ghost,
        env->zombie,
        env->vampire,
        env->mirror,
        env->antimirror,
        env->spirit,
        env->hmirror,
        env->vmirror,
        env->empty,
        env->reset,
    };

    //posmx: position top monster x, posmy: position top monster y
    int posmx, posmy;
    SDL_GetWindowSize(win, &env->w, &env->h);

    //background
    SDL_RenderCopy(ren, env->background, NULL, NULL);

    //posx, posy: coordonées du début de la grille
    int posx = env->w * 0.22;
    int posy = env->h * 0.22;

    // Coef will take a value which it depends of the minimum between height & width
    if (posx>posy)
        env->coef = (env->h * 0.6) / (env->game_height);
    else
        env->coef = (env->w * 0.6) / (env->game_width);

    SDL_Rect rectArray[env->game_width * env->game_height];


    /************************
    TEXTE LABEL
    ************************/

    // Initialise all values which are about texts ( labels )

    env->fontsize = env->coef/6;

    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Color nb_b_color = { 66, 55, 50, 255 };
    SDL_Color nb_r_color = { 255, 0, 0, 255 };
    SDL_Color nb_g_color = { 0, 255, 0, 255 };
    TTF_Font * font = TTF_OpenFont(FONT, env->fontsize); //label font
    TTF_Font * font_min = TTF_OpenFont(FONT, env->fontsize-2); //required_nb_monsters font
    if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD); // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
    TTF_SetFontStyle(font_min, TTF_STYLE_ITALIC | TTF_STYLE_BOLD);

    //add label in a list
    int n = (2*(env->game_width) + 2*(env->game_height)) + 4;
    int label[n];
    for (int i = 0; i < n-4; i++) {
        //North
        if (i < env->game_width) label[i] = required_nb_seen(env->game1, N, i);
        //South
        if (i >= env->game_width && i < 2*(env->game_width)) label[i] = required_nb_seen(env->game1, S, i-env->game_width);
        //West
        if (i >= 2*(env->game_width) && i < 2*(env->game_width) + (env->game_height)) label[i] = required_nb_seen(env->game1, W, env->game_height-1 - (i - 2*(env->game_width)));
        //Est
        if (i >= 2*(env->game_width) + (env->game_height) && i < 2*(env->game_width) + 2*(env->game_height)) label[i] = required_nb_seen(env->game1, E, env->game_height-1 - (i - 2*(env->game_width) - (env->game_height)));
    }
    //add value of required_nb_monsters
    int m = 2*(env->game_width) + 2*(env->game_height);
    label[m] = env->nb_g;
    label[m+1] = env->nb_v;
    label[m+2] = env->nb_z;
    label[m+3] = env->nb_s;

    char str_x[12];
    for (int i = 0; i < n; i++) {
        sprintf(str_x, "%d", label[i]);
        SDL_Surface * surf;

        if (i >= 2*(env->game_width) + 2*(env->game_height)) {
            if (label[i] < 0)
                surf = TTF_RenderText_Blended(font_min, str_x, nb_r_color);
            else if (label[i] == 0)
                surf = TTF_RenderText_Blended(font_min, str_x, nb_g_color); //for required_nb_monster
            else
                surf = TTF_RenderText_Blended(font_min, str_x, nb_b_color); //for required_nb_monster
        }

        else
            surf = TTF_RenderText_Blended(font, str_x, black);

        env->text = SDL_CreateTextureFromSurface(ren, surf);
        SDL_Rect rect;
        SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);

        //North
        if (i < env->game_width) {
            rect.x = (env->coef*i) + (env->w*0.1) + (env->coef/2) - env->fontsize/2;
            rect.y = (env->h*0.06) + (env->h*0.1);
        }

        //South
        if (i >= env->game_width && i < 2*(env->game_width)) {
            rect.x = (env->coef * (i - env->game_width)) + (env->w*0.1) + (env->coef/2) - env->fontsize/2;
            rect.y = posy + (env->h*0.03) + (env->coef * env->game_height);
        }

        //West
        if (i >= 2*(env->game_width) && i < 2*(env->game_width) + (env->game_height)) {
            rect.x = (env->w*0.1) - (env->w*0.06);
            rect.y = (env->h*0.2) + (env->coef/2) - env->fontsize/2 + (env->coef * (i - (2*(env->game_width))));
        }

        //Est
        if (i >= 2*(env->game_width) + env->game_height && i < 2*(env->game_width) + 2*(env->game_height)) {
            rect.x = (env->w*0.1) + (env->w*0.06) + (env->coef * env->game_width);
            rect.y = (env->h*0.2) + (env->coef/2) - env->fontsize/2 + (env->coef * (i -(2*(env->game_width)) - env->game_height));
        }

        //required_nb_monsters
        if (i >= 2*(env->game_width) + 2*(env->game_height)) {
            rect.x = ((env->coef - (env->coef/4)) * (i - (2*(env->game_width) + 2*(env->game_height)))) + env->w*0.01;
            rect.y = env->coef*0.01;
        }

        SDL_RenderCopy(ren, env->text, NULL, &rect);
        SDL_FreeSurface(surf);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(font_min);

    //posmx, posmy: position of top monsters
    posmx = env->w*0.01;
    posmy = env->h*0.01;

    // Create, initialise and display textures for top monsters
    SDL_Rect rectangleG = {posmx, posmy, 0, 0};
    SDL_QueryTexture(textureArray[0], NULL, NULL, &rectangleG.w, &rectangleG.h);
    rectangleG.w = env->coef - (env->coef*0.3);
    rectangleG.h = env->coef - (env->coef*0.3);
    SDL_RenderCopy(ren, textureArray[0], NULL, &rectangleG);

    posmx += env->coef - (env->coef/4);
    SDL_Rect rectangleV = {posmx, posmy, 0, 0};
    SDL_QueryTexture(textureArray[2], NULL, NULL, &rectangleV.w, &rectangleV.h);
    rectangleV.w = env->coef - (env->coef*0.3);
    rectangleV.h = env->coef - (env->coef*0.3);
    SDL_RenderCopy(ren, textureArray[2], NULL, &rectangleV);

    posmx += env->coef - (env->coef/4);
    SDL_Rect rectangleZ = {posmx, posmy, 0, 0};
    SDL_QueryTexture(textureArray[1], NULL, NULL, &rectangleZ.w, &rectangleZ.h);
    rectangleZ.w = env->coef - (env->coef*0.3);
    rectangleZ.h = env->coef - (env->coef*0.3);
    SDL_RenderCopy(ren, textureArray[1], NULL, &rectangleZ);

    posmx += env->coef - (env->coef/4);
    SDL_Rect rectangleS = {posmx, posmy, 0, 0};
    SDL_QueryTexture(textureArray[5], NULL, NULL, &rectangleS.w, &rectangleS.h);
    rectangleS.w = env->coef - (env->coef*0.3);
    rectangleS.h = env->coef - (env->coef*0.3);
    SDL_RenderCopy(ren, textureArray[5], NULL, &rectangleS);

    // env -> choice will cointain the texture of the last monster which was selectionned by the user
    SDL_Texture* courant;
    switch(env->choice) {
        case VAMPIRE:
            courant = textureArray[2];
        break;

        case ZOMBIE:
            courant = textureArray[1];
        break;

        case GHOST:
            courant = textureArray[0];
        break;

        case SPIRIT:
            courant = textureArray[5];
        break;

        case EMPTY:
            courant = textureArray[8];

        default:
            courant = textureArray[8];
        break;
    }

    //Create, initialise and display texture for the choice monster
    posmx += env->coef - (env->coef/4);
    SDL_Rect rectangleC = {posmx, posmy, 0, 0};
    SDL_QueryTexture(courant, NULL, NULL, &rectangleC.w, &rectangleC.h);
    rectangleC.w = env->coef - (env->coef*0.3);
    rectangleC.h = env->coef - (env->coef*0.3);
    SDL_RenderCopy(ren, courant, NULL, &rectangleC);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderDrawRect(ren, &rectangleC);

    //button reset
    posmx += env->coef - (env->coef/4);
    SDL_Rect rectangleR = {posmx, posmy, 0, 0};
    SDL_QueryTexture(textureArray[9], NULL, NULL, &rectangleR.w, &rectangleR.h);
    rectangleR.w = env->coef - (env->coef*0.3);
    rectangleR.h = env->coef - (env->coef*0.3);
    SDL_RenderCopy(ren, textureArray[9], NULL, &rectangleR);




    //display of the grid
    for (int x = 0; x < env->game_width; x++) {
        for (int y = 0; y < env->game_height; y++) {
            SDL_Rect rect = { (env->coef*x) + (env->w*0.1), ((env->h*0.1) + (env->coef*y)) + (env->h*0.1), 0, 0 };
            int val = get_content(env->game1, x, env->game_height-1-y);
            switch(val) {
                case EMPTY:
                    rect.w = env->coef;
                    rect.h = env->coef;
                    rectArray[x + y*env->game_width] = rect;
                break;

                case MIRROR:
                    SDL_QueryTexture(textureArray[3], NULL, NULL, &rect.w, &rect.h);
                    rect.w = env->coef;
                    rect.h = env->coef;
                    SDL_RenderCopy(ren, textureArray[3], NULL, &rect);
                    rectArray[x + y*env->game_width] = rect;
                break;

                case ANTIMIRROR:
                    SDL_QueryTexture(textureArray[4], NULL, NULL, &rect.w, &rect.h);
                    rect.w = env->coef;
                    rect.h = env->coef;
                    SDL_RenderCopy(ren, textureArray[4], NULL, &rect);
                    rectArray[x + y*env->game_width] = rect;
                break;

                case HMIRROR:
                    SDL_QueryTexture(textureArray[6], NULL, NULL, &rect.w, &rect.h);
                    rect.w = env->coef;
                    rect.h = env->coef;
                    SDL_RenderCopy(ren, textureArray[6], NULL, &rect);
                    rectArray[x + y*env->game_width] = rect;
                break;

                case VMIRROR:
                    SDL_QueryTexture(textureArray[7], NULL, NULL, &rect.w, &rect.h);
                    rect.w = env->coef;
                    rect.h = env->coef;
                    SDL_RenderCopy(ren, textureArray[7], NULL, &rect);
                    rectArray[x + y*env->game_width] = rect;
                break;

                case GHOST:
                    SDL_QueryTexture(textureArray[0], NULL, NULL, &rect.w, &rect.h);
                    rect.w = env->coef;
                    rect.h = env->coef;
                    SDL_RenderCopy(ren, textureArray[0], NULL, &rect);
                    rectArray[x + y*env->game_width] = rect;
                break;

                case VAMPIRE:
                    SDL_QueryTexture(textureArray[2], NULL, NULL, &rect.w, &rect.h);
                    rect.w = env->coef;
                    rect.h = env->coef;
                    SDL_RenderCopy(ren, textureArray[2], NULL, &rect);
                    rectArray[x + y*env->game_width] = rect;
                break;

                case ZOMBIE:
                    SDL_QueryTexture(textureArray[1], NULL, NULL, &rect.w, &rect.h);
                    rect.w = env->coef;
                    rect.h = env->coef;
                    SDL_RenderCopy(ren, textureArray[1], NULL, &rect);
                    rectArray[x + y*env->game_width] = rect;
                break;

                case SPIRIT:
                    SDL_QueryTexture(textureArray[5], NULL, NULL, &rect.w, &rect.h);
                    rect.w = env->coef;
                    rect.h = env->coef;
                    SDL_RenderCopy(ren, textureArray[5], NULL, &rect);
                    rectArray[x + y*env->game_width] = rect;
                break;
            }
        }
    }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderDrawRects(ren, rectArray, env->game_width * env->game_height);
}

/**
* @brief display menu
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
**/
void render_menu(SDL_Window* win,  SDL_Renderer* ren, Env * env) {
    SDL_GetWindowSize(win, &env->w, &env->h);

    //background
    SDL_RenderCopy(ren, env->background, NULL, NULL);
    int grille_x = env->grille_x;
    int grille_y = env->grille_y;

    //posx, posy: coordonates of the start of the grid
    int posx = env->w*0.20;
    int posy = env->h*0.20;

    if (posx > posy)
        env->coef = (env->h*0.7) / grille_y;
    else
        env->coef = (env->w*0.7) / grille_x;

    //TEXTE NUMERO LEVEL
    env->fontsize = env->coef/4;
    SDL_Color blue = { 36, 113, 134, 255 };

    //text "MENU"
    TTF_Font * menu = TTF_OpenFont(FONT_MENU, env->fontsize + 20);
    SDL_Surface * surf = TTF_RenderText_Blended(menu, "MENU", blue);
    env->text = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect rect;
    SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
    int size_w;
    int size_h;
    TTF_SizeText(menu, "MENU", &size_w, &size_h);
    rect.x = (env->w/2) - (size_w/2);
    rect.y = 0;
    SDL_RenderCopy(ren, env->text, NULL, &rect);
    SDL_FreeSurface(surf);
    TTF_CloseFont(menu);

    TTF_Font * font = TTF_OpenFont(FONT, env->fontsize);
    if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD); // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL

    //text "RULES"
    TTF_Font * rules = TTF_OpenFont(FONT_MENU, env->fontsize + 20);
    surf = TTF_RenderText_Blended(rules, "RULES", blue);
    env->text = SDL_CreateTextureFromSurface(ren, surf);
    SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
    int size_x;
    int size_y;
    TTF_SizeText(rules, "RULES", &size_x, &size_y);
    rect.x = (env->w/2) - (size_x/2);
    rect.y = (env->h) - (size_y);
    SDL_RenderCopy(ren, env->text, NULL, &rect);
    SDL_FreeSurface(surf);
    TTF_CloseFont(rules);


    //stock grid of menu
    int n = grille_x * grille_y;
    int level[n];
    for(int i = 0; i < n; i++)
        level[i] = i+1;

    char str_x[12];
    for (int i = 0; i < n-1; i++) {
        sprintf(str_x, "%d", level[i]);
        SDL_Surface * surf = TTF_RenderText_Blended(font, str_x, blue); // blended rendering for ultra nice text
        env->text = SDL_CreateTextureFromSurface(ren, surf);
        SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);



        if (i < grille_x) {
            rect.x = (env->coef*i) + ((env->w - env->coef * grille_x)/2) + (env->coef/2) - env->fontsize/2;
            rect.y = ((env->h - env->coef * grille_y)/2) + (env->coef/2) - env->fontsize/2;
        }

        if (i >= grille_x && i < 2*grille_x) {
            rect.x = (env->coef * (i - grille_x)) + ((env->w - env->coef * grille_x)/2) + (env->coef/2) - env->fontsize/2;
            rect.y = env->coef + ((env->h - env->coef * grille_y)/2) + (env->coef/2) - env->fontsize/2;
        }

        if (i >= 2*grille_x && i < 3*grille_x) {
            rect.x = (env->coef * (i - 2*grille_x)) + ((env->w - env->coef * grille_x)/2) + (env->coef/2) - env->fontsize/2;
            rect.y = 2*env->coef + ((env->h - env->coef * grille_y)/2) + (env->coef/2) - env->fontsize/2;
        }

        if (i >= 3*grille_x && i < 4*grille_x) {
            rect.x = (env->coef * (i - 3*grille_x)) + ((env->w - env->coef * grille_x)/2) + (env->coef/2) - env->fontsize/2;
            rect.y = 3*env->coef + ((env->h - env->coef * grille_y)/2) + (env->coef/2) - env->fontsize/2;
        }

        if (i >= 4*grille_x && i < (5*grille_x)) {
            rect.x = (env->coef * (i - 4*grille_x)) + ((env->w - env->coef * grille_x)/2) + (env->coef/2) - env->fontsize/2;
            rect.y = 4*env->coef + ((env->h - env->coef * grille_y)/2) + (env->coef/2) - env->fontsize/2;
        }

        SDL_RenderCopy(ren, env->text, NULL, &rect);
        SDL_FreeSurface(surf);
    }
    surf = TTF_RenderText_Blended(font, "?", blue); // blended rendering for ultra nice text
    env->text = SDL_CreateTextureFromSurface(ren, surf);
    SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
    rect.x = (env->coef * ((n-1) - 4*grille_x)) + ((env->w - env->coef * grille_x)/2) + (env->coef/2) - env->fontsize/2;
    rect.y = 4*env->coef + ((env->h - env->coef * grille_y)/2) + (env->coef/2) - env->fontsize/2;
    SDL_RenderCopy(ren, env->text, NULL, &rect);
    SDL_FreeSurface(surf);


    TTF_CloseFont(font);

    //show grid (20 lvl)
    SDL_Rect rectArray[grille_x * grille_y+1];

    for (int x = 0; x < grille_x; x++) {
        for (int y = 0; y < grille_y; y++) {
            SDL_Rect rect = { (env->coef*x) + ((env->w - env->coef * grille_x)/2), (env->coef*y) + ((env->h - env->coef * grille_y)/2), 0, 0};
            rect.w = env->coef;
            rect.h = env->coef;
            rectArray[x + y*grille_x] = rect;
        }
    }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderDrawRects(ren, rectArray, grille_x * grille_y);
}

/**
* @brief choose the render to call
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
**/
void render(SDL_Window* win, SDL_Renderer* ren, Env * env) {
    switch (env->state) {
        case 0:
            render_menu(win, ren, env);
        break;

        case 1:
            render_game(win, ren, env);
        break;

        case 2:
            render_rules(win, ren, env);
        break;
        case 3:
            won(win,ren,env);
            break;
        case -1:
            animation(win,ren,env); 
            break;
        default: break;
    }
}

/* **************************************************************** */

/**
* @brief dectect a collision between a point and and object
* @param x coordonate on x of your point
* @param y coordonate on y of your point
* @param x2 coordonate on x of the start of the object
* @param y2 coordonate on y of the start of the object
* @param w2 width of the object
* @param h2 height of the object
* @return a boolean whi is egual to true if the point is inside the object else false
**/

bool collision (int x, int y, int x2, int y2, int w2, int h2) {
    if (x > x2 && y > y2 && x < x2 + w2 && y < y2 + h2)
        return true;
    return false;
}

/**
* @brief load the game corresponds with level
* @param env an environnement which contains all textures
**/
void load_level (Env* env) {
    switch (env->level) {
        case 0:
            env->game1 = load_game("./map/level1"); break;

        case 1:
            env->game1 = load_game("./map/level2");break;

        case 2:
            env->game1 = load_game("./map/level3"); break;

        case 3:
            env->game1 = load_game("./map/level4"); break;

        case 4:
            env->game1 = load_game("./map/level5"); break;

        case 5:
            env->game1 = load_game("./map/level6"); break;

        case 6:
            env->game1 = load_game("./map/level7"); break;

        case 7:
            env->game1 = load_game("./map/level8"); break;

        case 8:
            env->game1 = load_game("./map/level9"); break;

        case 9:
            env->game1 = load_game("./map/level10"); break;

        case 10:
            env->game1 = load_game("./map/level11"); break;

        case 11:
            env->game1 = load_game("./map/level12"); break;

        case 12:
            env->game1 = load_game("./map/level13"); break;

        case 13:
            env->game1 = load_game("./map/level14"); break;

        case 14:
            env->game1 = load_game("./map/level15"); break;

        case 15:
            env->game1 = load_game("./map/level16"); break;

        case 16:
            env->game1 = load_game("./map/level17"); break;

        case 17:
            env->game1 = load_game("./map/level18"); break;

        case 18:
            env->game1 = load_game("./map/level19"); break;

        case 19:
            env->game1 = default_game(); break;
            

        default: break;
    }

    env->game_width = game_width(env->game1);
    env->game_height = game_height(env->game1);
}


/* **************************************************************** */

/**
* @brief manage all events on your game
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
* @param e the event
* @return a boolean which is egual to true if we are quiting the game else false if we have a crash or a bug
**/

bool process_game (SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e) {
    int posmx, posmy;
    SDL_GetWindowSize(win, &env->w, &env->h);

    //posx, posy: coordonées du début de la grille
    int posx = env->w*0.22;
    int posy = env->h*0.22;

    // Coef will take a value which it depends of the minimum between height & width
    if (posx > posy)
        env->coef = (env->h*0.6) / (env->game_height);
    else
        env->coef = (env->w*0.6) / (env->game_width);

    posmx = env->w*0.01;
    posmy = env->h*0.01;

    // collision between the mouse and the choice monster
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y);

        if (collision(mouse.x, mouse.y, posmx, posmy, env->coef - (env->coef*0.3), env->coef - (env->coef*0.3)))
            env->choice = GHOST;

        if (collision(mouse.x, mouse.y, posmx + env->coef - (env->coef/4), posmy, env->coef - (env->coef*0.3), env->coef - (env->coef*0.3)))
            env->choice = VAMPIRE;

        if (collision(mouse.x, mouse.y, posmx + (2*(env->coef - (env->coef/4))), posmy, env->coef - (env->coef*0.3), env->coef - (env->coef*0.3)))
            env->choice = ZOMBIE;

        if (collision(mouse.x, mouse.y, posmx + (3*(env->coef - (env->coef/4))), posmy, env->coef - (env->coef*0.3), env->coef - (env->coef*0.3)))
            env->choice = SPIRIT;

        //reset
        if (collision(mouse.x, mouse.y, posmx + (5*(env->coef - (env->coef/4))), posmy, env->coef - (env->coef*0.3), env->coef - (env->coef*0.3))){
            load_level(env);
            env->nb_g = required_nb_monsters(env->game1, GHOST);
            env->nb_z = required_nb_monsters(env->game1, ZOMBIE);
            env->nb_v = required_nb_monsters(env->game1, VAMPIRE);
            env->nb_s = required_nb_monsters(env->game1, SPIRIT);
            env->choice = EMPTY;
        }
        // collision between the mouse and the grid
        for (int x = 0; x < env->game_width; x++) {
            for (int y = 0; y < env->game_height; y++) {
                if (collision(mouse.x, mouse.y, (env->coef*x) + (env->w*0.1), ((env->h*0.1) + (env->coef*y)) + (env->h*0.1), env->coef, env->coef)) {
                    //check the monster in the case
                    content last_monster = get_content(env->game1, x, env->game_height-1-y);

                    //update value of required_nb_monster
                    switch (last_monster) {
                        case SPIRIT:
                            env->nb_s++; break;

                        case VAMPIRE:
                            env->nb_v++; break;

                        case GHOST:
                            env->nb_g++; break;

                        case ZOMBIE:
                            env->nb_z++; break;

                        default: break;
                    }
                    if(last_monster == MIRROR || last_monster == ANTIMIRROR || last_monster == HMIRROR || last_monster == VMIRROR){
                        return false;
                    }
                    add_monster(env->game1, env->choice, x, env->game_height-1-y);

                    switch (env->choice) {
                        case SPIRIT:
                            env->nb_s--; break;

                        case VAMPIRE:
                            env->nb_v--; break;

                        case GHOST:
                            env->nb_g--; break;

                        case ZOMBIE:
                            env->nb_z--; break;

                        default: break;
                    }
                    break;
                }
            }
        }
    }

    //Quit event
    if (e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_ESCAPE:
                env->state = 0; 
                env->choice = EMPTY;
                break;
            default: break;
        }
    }
    if (e->type == SDL_QUIT)
        return true;

    return false;
}

/**
* @brief manage all events on menu
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
* @param e the event
* @return a boolean which is egual to true if we are quiting the game else false if we have a crash or a bug
**/
bool process_menu(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e) {
    SDL_GetWindowSize(win, &env->w, &env->h);

    int grille_x = env->grille_x;
    int grille_y = env->grille_y;
    //posx, posy: coordonées du début de la grille
    int posx = env->w*0.20;
    int posy = env->h*0.20;

    if (posx > posy)
        env->coef = (env->h*0.7) / grille_y;
    else
        env->coef = (env->w*0.7) / grille_x;


    //quit
    if (e->type == SDL_QUIT)
        return true;

    if (e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_ESCAPE:
                return true; break;
            default: break;
        }
    }


    //teste collision case niveau
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y);
        //rules
        int size_x;
        int size_y;
        TTF_Font * rules = TTF_OpenFont(FONT_MENU, env->fontsize + 20);
        TTF_SizeText(rules, "RULES", &size_x, &size_y);
        TTF_CloseFont(rules);
        if(collision(mouse.x, mouse.y, (env->w/2) - (size_x/2), (env->h) - (size_y), size_x, size_y)){
            //show rules
            env->state =2;
        }

        for (int x = 0; x < grille_x; x++) {
            for (int y = 0; y < grille_y; y++) {
                if (collision(mouse.x, mouse.y, (env->coef*x) + ((env->w - env->coef * grille_x)/2), (env->coef*y) + ((env->h - env->coef * grille_y)/2), env->coef, env->coef)) {
                    env->level = (x + y * grille_x);
                    env->state = 1;
                    load_level(env);
                    env->nb_g = required_nb_monsters(env->game1, GHOST);
                    env->nb_z = required_nb_monsters(env->game1, ZOMBIE);
                    env->nb_v = required_nb_monsters(env->game1, VAMPIRE);
                    env->nb_s = required_nb_monsters(env->game1, SPIRIT);
                    break;
                }
            }
        }
    }

    return false;
}

/**
* @brief manage all events on won page
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
* @param e the event
* @return a boolean which is egual to true if we are quiting the game else false if we have a crash or a bug
**/
bool process_won(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e){
    //quit
    if (e->type == SDL_QUIT)
        return true;

    if (e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_ESCAPE:
                return true; break;
            default: break;
        }
    }
    SDL_GetWindowSize(win, &env->w, &env->h);
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y);
        if(collision(mouse.x, mouse.y,0,0,env->w,env->h)){
            env->state = 0; //menu
        }

    }
    return false;
}

/**
* @brief manage events on anim page
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
* @param e the event
* @return a boolean which is egual to true if we are quiting the game else false if we have a crash or a bug
**/
bool process_anim(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e){
    //quit
    if (e->type == SDL_QUIT)
        return true;

    if (e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_ESCAPE:
                return true; break;
            default: break;
        }
    }
    SDL_GetWindowSize(win, &env->w, &env->h);
    if (env->w>env->h)
        env->coef = (env->h * 0.6);
    else
        env->coef = (env->w * 0.6);

    //buttom play position
    int buttom_x = env->coef/2;
    int buttom_y = env->coef/2;
    SDL_GetWindowSize(win, &env->w, &env->h);
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y);
        if(collision(mouse.x, mouse.y,env->w/2-buttom_x/2, env->h/3-buttom_y/2,buttom_x, buttom_y)){
            env->state = 0; //menu
        }

    }
    return false;
}

/**
* @brief divide events fonctions
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
* @param e the event
* @return a boolean which is egual to true if we are quiting the game else false if we have a crash or a bug
**/
bool process (SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e) {
    switch (env->state) {
        case -1: //animation
            return process_anim(win,ren,env,e);
            break;
        case 0: //menu
            return process_menu(win, ren, env, e);
            break;
        case 1: //game
            return process_game(win, ren, env, e);
            break;
        case 2: //rules
            return process_won(win,ren,env,e);
            break;
        case 3: //won
            return process_won(win,ren,env,e);
            break;
        default:
            return false;
            break;
    }
}


/* **************************************************************** */
/**
* @brief destroy all textures on your game
* @param win the window of your game
* @param ren the renderer of your game
* @param env an environnement which contains all textures
**/

void clean (SDL_Window* win, SDL_Renderer* ren, Env * env) {
    SDL_DestroyTexture(env->ghost);
    SDL_DestroyTexture(env->zombie);
    SDL_DestroyTexture(env->vampire);
    SDL_DestroyTexture(env->mirror);
    SDL_DestroyTexture(env->antimirror);
    SDL_DestroyTexture(env->spirit);
    SDL_DestroyTexture(env->hmirror);
    SDL_DestroyTexture(env->vmirror);
    SDL_DestroyTexture(env->background);
    SDL_DestroyTexture(env->won);
    SDL_DestroyTexture(env->empty);
    SDL_DestroyTexture(env->reset);
    SDL_DestroyTexture(env->play_buttom);
    SDL_DestroyTexture(env->rules);
    SDL_DestroyTexture(env->text);
    free(env);
}

/* **************************************************************** */


/**
* @brief return the game in environnement structure
* @param rnv an environnement which contains all textures
**/
game _game(Env* env) {
    return env->game1;
}

/**
* @brief return the state of game (menu or in a party)
* @param rnv an environnement which contains all textures
**/
void _state(Env* env, int x) {
    env->state=x;
}
/**
* @brief set the state of game (menu or in a party)
* @param rnv an environnement which contains all textures
**/
int get_state(Env* env){
    return env->state;
}


game default_game() {
    srand((unsigned) time(NULL));
	game game1 = new_game_ext((rand() % 6) + 5, (rand() % 6) + 5);
	initialise(game1); // grid generator function
	return game1;
}
