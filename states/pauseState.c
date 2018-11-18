#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "pauseState.h"
#include "../resourceManager.h"

PauseState* initialisePauseState(Display* d) {
    PauseState* ps = malloc(sizeof(PauseState));
    ps->redraw = true;
    ps->nextState = NIL;
    ps->pushState = NIL;

    ps->win_x = initialiseTween(0);
    ps->win_y = initialiseTween(0);
    ps->win_w = initialiseTween(SCREEN_WIDTH);
    ps->win_h = initialiseTween(SCREEN_HEIGHT);

    ps->display = d;
    ps->win = SDL_CreateTexture(d->renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            SCREEN_WIDTH, SCREEN_HEIGHT);

    char* ts[NO_BUTTONS] = { BUTTON_NAME_1, BUTTON_NAME_2 };
    int xs[] = {BUTTON_X, BUTTON_X, BUTTON_X};
    int ys[] = {BUTTON_Y_1, BUTTON_Y_2};
    ps->buttonManager = initialiseButtonManager(NO_BUTTONS, ts, xs, ys, d);
    return ps;
}


void wakePauseState(PauseState* ps) {
    SDL_SetRenderTarget(ps->display->renderer, ps->win);
    SDL_RenderCopy(ps->display->renderer, ps->display->lastPresent, NULL, NULL);
    SDL_SetRenderTarget(ps->display->renderer, ps->display->lastPresent);

    moveTweenValue(ps->win_x, EASE_OUT, WINDOW_X, 40, 0);
    moveTweenValue(ps->win_y, EASE_OUT, WINDOW_Y, 40, 0);
    moveTweenValue(ps->win_w, EASE_OUT, WINDOW_W, 40, 0);
    moveTweenValue(ps->win_h, EASE_OUT, WINDOW_H, 40, 0);
}  

void sleepPauseState(PauseState* ps) {
    ps->nextState = NIL;

    ps->win_x->id = 0;
    setTweenValue(ps->win_x, 0);
    setTweenValue(ps->win_y, 0);
    setTweenValue(ps->win_w, SCREEN_WIDTH);
    setTweenValue(ps->win_h, SCREEN_HEIGHT);
}

void PauseState_pushState(PauseState* ps, StateType st) {
    ps->pushState = st;
    ps->win_x->id = 1;
    moveTweenValue(ps->win_x, EASE_OUT, 0, 40, 0);
    moveTweenValue(ps->win_y, EASE_OUT, 0, 40, 0);
    moveTweenValue(ps->win_w, EASE_OUT, SCREEN_WIDTH, 40, 0);
    moveTweenValue(ps->win_h, EASE_OUT, SCREEN_HEIGHT, 40, 0);
}

// Sifts through the possible button presses and acts accordingly
void PauseState_buttonEvent(PauseState* ps, int i) {
    if (ps->win_x->id == 1) return;
    switch (i) {
        case 0:
            PauseState_pushState(ps, LAST_STATE);
        break;
        case 1:
            PauseState_pushState(ps, CLOSE);
        break;
    }
}

void updatePauseState(PauseState* ps) {
    updateTweenValue(ps->win_x);
    updateTweenValue(ps->win_y);
    updateTweenValue(ps->win_w);
    updateTweenValue(ps->win_h);
    ps->redraw |= TweenValue_dropRedraw(ps->win_x);
    ps->redraw |= TweenValue_dropRedraw(ps->win_y);
    ps->redraw |= TweenValue_dropRedraw(ps->win_w);
    ps->redraw |= TweenValue_dropRedraw(ps->win_h);

    updateButtonManager(ps->buttonManager);
    ps->redraw |= ButtonManager_dropRedraw(ps->buttonManager);
    PauseState_buttonEvent(ps, ButtonManager_dropClicked(ps->buttonManager));

    if (arrived(ps->win_x) && ps->win_x->id == 1) {
        ps->nextState = ps->pushState;
    }
}

void drawPauseState(PauseState* ps, Display* display) {
    SDL_SetRenderDrawColor(display->renderer, 40, 40, 40, 255);
    
    drawButtonManager(ps->buttonManager, display);

    SDL_Rect r = {
        getTweenValue(ps->win_x),
        getTweenValue(ps->win_y),
        getTweenValue(ps->win_w),
        getTweenValue(ps->win_h)
    };
    SDL_RenderCopy(display->renderer, ps->win, NULL, &r);
}

void PauseState_mouseMotionEvent(PauseState* ps, SDL_MouseMotionEvent e) {
    ButtonManager_mouseMotionEvent(ps->buttonManager, e);
}

void PauseState_mouseButtonEvent(PauseState* ps, SDL_MouseButtonEvent e) {
    ButtonManager_mouseButtonEvent(ps->buttonManager, e);
}

void PauseState_keyEvent(PauseState* ps, SDL_KeyboardEvent e) {
}

bool PauseState_dropRedraw(PauseState* ps) {
    if (ps->redraw) {
        ps->redraw = false;
        return true;
    }
    return false;
}

void PauseState_runTests(PauseState* ps) {}

void freePauseState(PauseState* ps) {
    freeButtonManager(ps->buttonManager);
    SDL_DestroyTexture(ps->win);
    freeTweenValue(ps->win_x);
    freeTweenValue(ps->win_y);
    freeTweenValue(ps->win_w);
    freeTweenValue(ps->win_h);
    free(ps);
}
