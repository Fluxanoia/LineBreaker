#ifndef RES_MAN_H_
#define RES_MAN_H_

#include    <math.h>
#include <SDL_ttf.h>

// Defines PI
#define M_PI acos(-1.0)
// Defines the screen width and height
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

// Defines the ResourceManager struct
//  -- this holds pointers to all the texture and font
//  -- resources for memory management
typedef struct ResourceManager_ {
    TTF_Font* base;
    TTF_Font* small;

    SDL_Color* button_colour;
    SDL_Color* text_colour;

    SDL_Texture* board_tab;
    SDL_Texture* board_back;
    SDL_Texture* boxes;

    SDL_Texture* button;
    SDL_Texture* hoverButton;

    SDL_Texture* menu_cover;

    SDL_Texture* title_small;
    SDL_Texture* title_line;
    SDL_Texture* title_breaker;

    SDL_Texture* background;
} ResourceManager;

// Defines the Display struct
//  -- holds the SDL_Window, SDL_Renderer and,
//  -- ResourceManager for memory management
typedef struct Display_ {
    SDL_Window*  window;
    SDL_Renderer* renderer;
    
    ResourceManager* resMan;
} Display;

SDL_Texture* createSmallText(char title[], int* w, int* h, SDL_Color c, Display* d);
SDL_Texture* createBaseText(char title[], int* w, int* h, SDL_Color c, Display* d);

ResourceManager* initialiseResourceManager(Display* d);
void freeDisplay(Display* d);

#endif
