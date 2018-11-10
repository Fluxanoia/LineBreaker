#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "soloState.h"
#include "../resourceManager.h"

SoloState* initialiseSoloState(Display* d) {
    SoloState* ss = malloc(sizeof(SoloState));
    ss->redraw = true;
    ss->nextState = NIL;
    ss->grid = initialiseGrid();
    return ss;
}

void wakeSoloState(SoloState* ss) {
}  

void sleepSoloState(SoloState* ss) {
}

void updateSoloState(SoloState* ss) {
    updateGrid(ss->grid);
    ss->redraw |= Grid_dropRedraw(ss->grid);
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
