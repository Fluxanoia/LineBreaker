#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "soloState.h"
#include "../resourceManager.h"

// Creates a SoloState and returns the pointer
SoloState* initialiseSoloState(Display* d) {
    SoloState* ss = malloc(sizeof(SoloState));
    ss->redraw = true;
    ss->nextState = NIL;

    GridKeyboardLayout gkl;
    gkl.LEFT         = SDLK_LEFT;
    gkl.RIGHT        = SDLK_RIGHT;
    gkl.ROTATE       = SDLK_UP;
    gkl.FAST_DROP    = SDLK_DOWN;
    gkl.INSTANT_DROP = SDLK_PERIOD;
    gkl.SWITCH_HELD  = SDLK_RETURN;
    ss->grid = initialiseGrid(GRID_INIT_X, GRID_INIT_Y, gkl, d);
    return ss;
}

// Wakes the SoloState from sleep
void wakeSoloState(SoloState* ss) {
    wakeGrid(ss->grid);
    moveTweenValue(ss->grid->y, EASE_OUT, 0, 60, 0);
}  

// Sleeps the SoloState so it can be stored
void sleepSoloState(SoloState* ss) {
    ss->nextState = NIL;
    sleepGrid(ss->grid);
    setGridPosition(ss->grid, GRID_INIT_X, GRID_INIT_Y);
}

// Updates the SoloState
void updateSoloState(SoloState* ss) {
    updateGrid(ss->grid);
    ss->redraw |= Grid_dropRedraw(ss->grid);
    // If the game is over...
    if (ss->grid->gameover) {
        // If the tweening has finished, leave. Otherwise, if it hasn't started,
        // tween.
        if (arrived(ss->grid->y) && ss->grid->y->id == 1) {
            ss->nextState = MENU;
        } else if (ss->grid->y->id == 0) {
            ss->grid->y->id = 1;
            moveTweenValue(ss->grid->y, EASE_OUT, -SCREEN_HEIGHT, 60, 20);
        }
    }
}

// Draws the SoloState
void drawSoloState(SoloState* ss, Display* display) {
    SDL_RenderCopy(display->renderer, display->resMan->background, NULL, NULL);
    drawGrid(ss->grid, display);
}

// Notifies the SoloState of mouse motion
void SoloState_mouseMotionEvent(SoloState* ss, SDL_MouseMotionEvent e) {
}

// Notifies the SoloState of mouse button presses
void SoloState_mouseButtonEvent(SoloState* ss, SDL_MouseButtonEvent e) {
}

// Notifies the SoloState of key presses
void SoloState_keyEvent(SoloState* ss, SDL_KeyboardEvent e) {
    Grid_keyEvent(ss->grid, e);
}

// Returns the redraw value and sets it to false
bool SoloState_dropRedraw(SoloState* ss) {
    if (ss->redraw) {
        ss->redraw = false;
        return true;
    }
    return false;
}

// Frees the SoloState and its components
void freeSoloState(SoloState* ss) {
    freeGrid(ss->grid);
    free(ss);
}
