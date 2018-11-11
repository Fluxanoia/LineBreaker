#include             <SDL.h>
#include         <SDL_ttf.h>
#include       <SDL_image.h>
#include        "handling.h"
#include "resourceManager.h"

SDL_Texture* texturise(Display* d, SDL_Surface* s) {
    SDL_Texture* t = P_(SDL_CreateTextureFromSurface(d->renderer, s));
    SDL_FreeSurface(s);
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    return t;
}

SDL_Texture* loadPNGTexture(Display* d, char path[]) {
    SDL_Surface* s = P_(IMG_Load(path));
    return texturise(d, s);
}

SDL_Texture* loadBMPTexture(Display* d, char path[]) {
    SDL_Surface* s = P_(SDL_LoadBMP(path));
    return texturise(d, s);
}

ResourceManager* initialiseResourceManager(Display* d) {
    ResourceManager* rm = malloc(sizeof(ResourceManager));
    rm->base = P_(TTF_OpenFont("res/font/roboto.ttf", 72));
    rm->small = P_(TTF_OpenFont("res/font/roboto.ttf", 36));

    rm->background = loadBMPTexture(d, "res/image/bg.bmp");
    
    rm->title_line = loadPNGTexture(d, "res/image/line.png");
    rm->title_breaker = loadPNGTexture(d, "res/image/breaker.png");
    rm->title_small = loadPNGTexture(d, "res/image/title_small.png");

    rm->menu_cover = loadPNGTexture(d, "res/image/menucover.png");

    rm->button = loadPNGTexture(d, "res/image/button.png");
    rm->hoverButton = loadPNGTexture(d, "res/image/button_h.png");
    
    rm->boxes = loadBMPTexture(d, "res/image/boxes.bmp");
    rm->board_back = loadPNGTexture(d, "res/image/boardback.png");
    return rm;
}

void freeResourceManager(ResourceManager* rm) {
    TTF_CloseFont(rm->base);
    SDL_DestroyTexture(rm->background);
    SDL_DestroyTexture(rm->title_line);
    SDL_DestroyTexture(rm->title_breaker);
    SDL_DestroyTexture(rm->menu_cover);
    SDL_DestroyTexture(rm->button);
    SDL_DestroyTexture(rm->hoverButton);
    SDL_DestroyTexture(rm->boxes);
    SDL_DestroyTexture(rm->board_back);
    free(rm);
}

void freeDisplay(Display* d) {
    freeResourceManager(d->resMan);
    SDL_DestroyRenderer(d->renderer);
    SDL_DestroyWindow(d->window);
    free(d);
}
