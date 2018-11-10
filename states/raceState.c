#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "raceState.h"
#include "../resourceManager.h"

RaceState* initialiseRaceState(Display* d) {
    RaceState* rs = malloc(sizeof(RaceState));
    rs->redraw = true;
    rs->nextState = NIL;
    return rs;
}


void wakeRaceState(RaceState* rs) {
}  

void sleepRaceState(RaceState* rs) {
}

void updateRaceState(RaceState* rs) {
}

void drawRaceState(RaceState* rs, Display* display) {
    SDL_RenderCopy(display->renderer, display->resMan->background, NULL, NULL);
}

void RaceState_mouseMotionEvent(RaceState* rs, SDL_MouseMotionEvent e) {
}

void RaceState_mouseButtonEvent(RaceState* rs, SDL_MouseButtonEvent e) {
}

void RaceState_keyEvent(RaceState* rs, SDL_KeyboardEvent e) {
}

bool RaceState_dropRedraw(RaceState* rs) {
    if (rs->redraw) {
        rs->redraw = false;
        return true;
    }
    return false;
}

void freeRaceState(RaceState* rs) {
    free(rs);
}
