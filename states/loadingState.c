#include                <SDL.h>
#include            <SDL_ttf.h>
#include             "states.h"
#include       "loadingState.h"
#include "../resourceManager.h"

// Initialises the LoadingState and returns its pointer
LoadingState* initialiseLoadingState(Display* d) {
    LoadingState* ls = malloc(sizeof(LoadingState));
    ls->redraw = true;
    ls->nextState = NIL;
    ls->bgAlpha = initialiseTween(0);
    
    SDL_QueryTexture(d->resMan->title_line, NULL, NULL,
            &(ls->lineX_width), &(ls->lineX_height));
    ls->lineX = initialiseTween(-(ls->lineX_width + 10));

    SDL_QueryTexture(d->resMan->title_breaker, NULL, NULL,
            &(ls->breakerX_width), &(ls->breakerX_height));
    ls->breakerX = initialiseTween(SCREEN_WIDTH + 10);

    return ls;
}

// Wakes the LoadingState from sleep
void wakeLoadingState(LoadingState* ls) {
    moveTweenValue(ls->bgAlpha, EASE_OUT, 255, 100, 10);
    moveTweenValue(ls->lineX, ELASTIC,
            (SCREEN_WIDTH - ls->lineX_width) / 2, 60, 100);
    moveTweenValue(ls->breakerX, ELASTIC,
            (SCREEN_WIDTH - ls->breakerX_width) / 2, 60, 130);
}

// Sleeps the LoadingState so it can be stored
void sleepLoadingState(LoadingState* ls) {
    ls->nextState = NIL;
    setTweenValue(ls->bgAlpha, 0);
    setTweenValue(ls->lineX, -(ls->lineX_width + 10));
    setTweenValue(ls->breakerX, SCREEN_WIDTH + 10);
}

// Updates the LoadingState
void updateLoadingState(LoadingState* ls) {
    updateTweenValue(ls->bgAlpha);
    ls->redraw |= TweenValue_dropRedraw(ls->bgAlpha);
    updateTweenValue(ls->lineX);
    ls->redraw |= TweenValue_dropRedraw(ls->lineX);
    updateTweenValue(ls->breakerX);
    ls->redraw |= TweenValue_dropRedraw(ls->breakerX);

    if (arrived(ls->lineX)) {
        if (ls->lineX->id == 0) {
            moveTweenValue(ls->lineX, EASE_IN, SCREEN_WIDTH + 10, 50, 20);
            ls->lineX->id = 1;
        }
    }
    if (arrived(ls->breakerX)) {
        if (ls->breakerX->id == 0) {
            moveTweenValue(ls->breakerX, EASE_IN, SCREEN_WIDTH + 10, 50, 10);
            ls->breakerX->id = 1;
        } else {
            ls->nextState = MENU;
        }
    }
}

// Draws the LoadingState
void drawLoadingState(LoadingState* load, Display* display) {
    SDL_SetTextureAlphaMod(display->resMan->background,
            (Uint8) getTweenValue(load->bgAlpha));
    SDL_RenderCopy(display->renderer, display->resMan->background, NULL, NULL);
    SDL_SetTextureAlphaMod(display->resMan->background, 255);

    SDL_Rect line_rect = (SDL_Rect) { (int) getTweenValue(load->lineX),
            TITLE_HEIGHT, load->lineX_width, load->lineX_height};
    SDL_RenderCopy(display->renderer, display->resMan->title_line,
            NULL, &line_rect);

    SDL_Rect break_rect = (SDL_Rect) { (int) getTweenValue(load->breakerX),
            TITLE_HEIGHT + load->breakerX_height + 10, load->breakerX_width,
            load->breakerX_height};
    SDL_RenderCopy(display->renderer, display->resMan->title_breaker,
            NULL, &break_rect);
}

// Notifies the LoadingState of mouse and key events
void LoadingState_mouseMotionEvent(LoadingState* ls, SDL_MouseMotionEvent e) {
}
void LoadingState_mouseButtonEvent(LoadingState* ls, SDL_MouseButtonEvent e) {
}
void LoadingState_keyEvent(LoadingState* ls, SDL_KeyboardEvent e) {
    switch (e.keysym.sym) {
        case SDLK_RETURN:
            ls->nextState = MENU;
        break;
    }
}

// Returns the redraw value and sets it to false
bool LoadingState_dropRedraw(LoadingState* ls) {
    if (ls->redraw) {
        ls->redraw = false;
        return true;
    }
    return false;
}

void LoadingState_runTests(LoadingState* ls) {}

// Frees the LoadingState
void freeLoadingState(LoadingState* ls) {
    freeTweenValue(ls->bgAlpha);
    freeTweenValue(ls->lineX);
    freeTweenValue(ls->breakerX);
    free(ls);
}
