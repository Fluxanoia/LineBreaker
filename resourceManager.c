#include             <SDL.h>
#include         <SDL_ttf.h>
#include       <SDL_image.h>
#include        "handling.h"
#include "resourceManager.h"

// Converts an SDL_Surface into a texture
SDL_Texture* texturise(Display* d, SDL_Surface* s) {
    SDL_Texture* t = P_(SDL_CreateTextureFromSurface(d->renderer, s));
    SDL_FreeSurface(s);
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    return t;
}

// Loads a PNG image
SDL_Texture* loadPNGTexture(Display* d, char path[]) {
    SDL_Surface* s = P_(IMG_Load(path));
    return texturise(d, s);
}

// Loads a BMP image
SDL_Texture* loadBMPTexture(Display* d, char path[]) {
    SDL_Surface* s = P_(SDL_LoadBMP(path));
    return texturise(d, s);
}

// Sets the colour of a SDL_Color
void fillColour(SDL_Color* c, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    c->r = r;
    c->g = g;
    c->b = b;
    c->a = a;
}

// Creates a texture using the small font
SDL_Texture* createSmallText(char title[], int* w, int* h, SDL_Color c, Display* d) {
    SDL_Surface* surface = TTF_RenderText_Solid(d->resMan->small, title, c);
    SDL_Texture* t = SDL_CreateTextureFromSurface(d->renderer, surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(t, NULL, NULL, w, h);
    return t;
}

// Creates a texture using the base font
SDL_Texture* createBaseText(char title[], int* w, int* h, SDL_Color c, Display* d) {
    SDL_Surface* surface = TTF_RenderText_Solid(d->resMan->base, title, c);
    SDL_Texture* t = SDL_CreateTextureFromSurface(d->renderer, surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(t, NULL, NULL, w, h);
    return t;
}


// Initialises a ResourceManager struct and fills it with
// pointers to its textures and fonts
ResourceManager* initialiseResourceManager(Display* d) {
    ResourceManager* rm = malloc(sizeof(ResourceManager));
    rm->base = P_(TTF_OpenFont("res/font/roboto.ttf", 72));
    rm->small = P_(TTF_OpenFont("res/font/roboto.ttf", 36));

    rm->button_colour = malloc(sizeof(SDL_Color));
    rm->text_colour = malloc(sizeof(SDL_Color));
    fillColour(rm->button_colour, 40, 40, 40, 255);
    fillColour(rm->text_colour, 40, 40, 40, 255);

    rm->background = loadBMPTexture(d, "res/image/bg.bmp");
    
    rm->title_line = loadPNGTexture(d, "res/image/line.png");
    rm->title_breaker = loadPNGTexture(d, "res/image/breaker.png");
    rm->title_small = loadPNGTexture(d, "res/image/title_small.png");

    rm->menu_cover = loadPNGTexture(d, "res/image/menucover.png");

    rm->button = loadPNGTexture(d, "res/image/button.png");
    rm->hoverButton = loadPNGTexture(d, "res/image/button_h.png");
    
    rm->boxes = loadBMPTexture(d, "res/image/boxes.bmp");
    rm->board_back = loadPNGTexture(d, "res/image/boardback.png");
    rm->board_tab = loadPNGTexture(d, "res/image/boardtab.png");
    rm->score_card = loadPNGTexture(d, "res/image/scorecard.png");
    return rm;
}

// Frees all the resources in the ResourceManager
void freeResourceManager(ResourceManager* rm) {
    TTF_CloseFont(rm->base);
    TTF_CloseFont(rm->small);
    SDL_DestroyTexture(rm->background);
    SDL_DestroyTexture(rm->title_line);
    SDL_DestroyTexture(rm->title_breaker);
    SDL_DestroyTexture(rm->title_small);
    SDL_DestroyTexture(rm->menu_cover);
    SDL_DestroyTexture(rm->button);
    SDL_DestroyTexture(rm->hoverButton);
    SDL_DestroyTexture(rm->boxes);
    SDL_DestroyTexture(rm->board_back);
    SDL_DestroyTexture(rm->board_tab);
    SDL_DestroyTexture(rm->score_card);
    free(rm->button_colour);
    free(rm->text_colour);
    free(rm);
}

// Frees all the resources in the Display
void freeDisplay(Display* d) {
    freeResourceManager(d->resMan);
    SDL_DestroyRenderer(d->renderer);
    SDL_DestroyWindow(d->window);
    SDL_DestroyTexture(d->lastPresent);
    free(d);
}
