#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "soloState.h"
#include "../resourceManager.h"

SoloState* initialiseSoloState(Display* d) {
    SoloState* ss = malloc(sizeof(SoloState));
    ss->redraw = true;
    ss->nextState = NIL;
    ss->grid = initialiseGrid(GRID_INIT_X, GRID_INIT_Y, d);
    return ss;
}

void wakeSoloState(SoloState* ss) {
    wakeGrid(ss->grid);
    moveTweenValue(ss->grid->y, EASE_OUT, 0, 60, 0);
}  

void sleepSoloState(SoloState* ss) {
    ss->nextState = NIL;
    sleepGrid(ss->grid);
    setTweenValue(ss->grid->x, GRID_INIT_X);
    setTweenValue(ss->grid->y, GRID_INIT_Y);
}

void updateSoloState(SoloState* ss) {
    updateGrid(ss->grid);
    ss->redraw |= Grid_dropRedraw(ss->grid);
    if (ss->grid->gameover) {
        if (arrived(ss->grid->y) && ss->grid->y->id == 1) {
            ss->nextState = MENU;
        } else if (ss->grid->y->id == 0) {
            ss->grid->y->id = 1;
            moveTweenValue(ss->grid->y, EASE_OUT, -720, 60, 0);
        }
    }
}

void drawSoloState(SoloState* ss, Display* display) {
    SDL_RenderCopy(display->renderer, display->resMan->background, NULL, NULL);
    drawGrid(ss->grid, display);
}

void SoloState_mouseMotionEvent(SoloState* ss, SDL_MouseMotionEvent e) {
}

void SoloState_mouseButtonEvent(SoloState* ss, SDL_MouseButtonEvent e) {
}

void SoloState_keyEvent(SoloState* ss, SDL_KeyboardEvent e) {
    Grid_keyEvent(ss->grid, e);
}

bool SoloState_dropRedraw(SoloState* ss) {
    if (ss->redraw) {
        ss->redraw = false;
        return true;
    }
    return false;
}

void freeSoloState(SoloState* ss) {
    freeGrid(ss->grid);
    free(ss);
}
