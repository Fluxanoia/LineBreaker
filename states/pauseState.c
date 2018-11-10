#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "pauseState.h"
#include "../resourceManager.h"

PauseState* initialisePauseState(Display* d) {
    PauseState* ps = malloc(sizeof(PauseState));
    ps->redraw = true;
    ps->nextState = NIL;
    return ps;
}


void wakePauseState(PauseState* ps) {
}  

void sleepPauseState(PauseState* ps) {
}

void updatePauseState(PauseState* ps) {
}

void drawPauseState(PauseState* ps, Display* display) {
    SDL_RenderCopy(display->renderer, display->resMan->background, NULL, NULL);
}

void PauseState_mouseMotionEvent(PauseState* ps, SDL_MouseMotionEvent e) {
}

void PauseState_mouseButtonEvent(PauseState* ps, SDL_MouseButtonEvent e) {
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

void freePauseState(PauseState* ps) {
    free(ps);
}
