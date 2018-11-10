#ifndef RES_MAN_H_
#define RES_MAN_H_

#include    <math.h>
#include <SDL_ttf.h>

#define M_PI acos(-1.0)

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

typedef struct ResourceManager_ {
    TTF_Font* base;

    SDL_Texture* board_back;
    SDL_Texture* boxes;

    SDL_Texture* button;
    SDL_Texture* hoverButton;

    SDL_Texture* menu_cover;

    SDL_Texture* title_line;
    SDL_Texture* title_breaker;

    SDL_Texture* background;
} ResourceManager;

typedef struct Display_ {
    SDL_Window*  window;
    SDL_Renderer* renderer;
    
    ResourceManager* resMan;
} Display;

ResourceManager* initialiseResourceManager(Display* d);
void freeDisplay(Display* d);

#endif
