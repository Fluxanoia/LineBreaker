#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "raceState.h"
#include "../resourceManager.h"

// Creates a RaceState and returns the pointer
RaceState* initialiseRaceState(Display* d) {
    RaceState* rs = malloc(sizeof(RaceState));
    rs->redraw = true;
    rs->nextState = NIL;

    GridKeyboardLayout gkl_left;
    gkl_left.LEFT         = SDLK_a;
    gkl_left.RIGHT        = SDLK_d;
    gkl_left.ROTATE       = SDLK_w;
    gkl_left.FAST_DROP    = SDLK_s;
    gkl_left.INSTANT_DROP = SDLK_e;
    gkl_left.SWITCH_HELD  = SDLK_r;
    rs->gridLeft = initialiseGrid(LEFT_GRID_INIT_X, 
            LEFT_GRID_INIT_Y, gkl_left, d);
    
    GridKeyboardLayout gkl_right;
    gkl_right.LEFT         = SDLK_LEFT;
    gkl_right.RIGHT        = SDLK_RIGHT;
    gkl_right.ROTATE       = SDLK_UP;
    gkl_right.FAST_DROP    = SDLK_DOWN;
    gkl_right.INSTANT_DROP = SDLK_SPACE;
    gkl_left.SWITCH_HELD   = SDLK_RETURN;
    rs->gridRight = initialiseGrid(RIGHT_GRID_INIT_X, 
            RIGHT_GRID_INIT_Y, gkl_right, d);
    return rs;
}

// Wakes the RaceState from sleep
void wakeRaceState(RaceState* rs) {
    wakeGrid(rs->gridLeft);
    wakeGrid(rs->gridRight);
    moveTweenValue(rs->gridLeft->y, EASE_OUT, 0, 60, 0);
    moveTweenValue(rs->gridRight->y, EASE_OUT, 0, 60, 0);
}  

// Sleeps the RaceState so it can be stored
void sleepRaceState(RaceState* rs) {
    rs->nextState = NIL;
    sleepGrid(rs->gridLeft);
    sleepGrid(rs->gridRight);
    setTweenValue(rs->gridLeft->x, LEFT_GRID_INIT_X);
    setTweenValue(rs->gridLeft->y, LEFT_GRID_INIT_Y);
    setTweenValue(rs->gridRight->x, RIGHT_GRID_INIT_X);
    setTweenValue(rs->gridRight->y, RIGHT_GRID_INIT_Y);
}

// Updates the RaceState
void updateRaceState(RaceState* rs) {
    updateGrid(rs->gridLeft);
    rs->redraw |= Grid_dropRedraw(rs->gridLeft);
    updateGrid(rs->gridRight);
    rs->redraw |= Grid_dropRedraw(rs->gridRight);
    // If the game is over...
    if (rs->gridLeft->gameover && rs->gridRight->gameover) {
        // If the tweening has finished, leave. Otherwise, if it hasn't started,
        // tween.
        if (arrived(rs->gridLeft->y) && rs->gridLeft->y->id == 1) {
            rs->nextState = MENU;
        } else if (rs->gridLeft->y->id == 0) {
            rs->gridLeft->y->id = 1;
            moveTweenValue(rs->gridLeft->y, EASE_OUT, -SCREEN_HEIGHT, 60, 20);
            moveTweenValue(rs->gridRight->y, EASE_OUT, -SCREEN_HEIGHT, 60, 20);
        }
    }
}

// Draws the RaceState
void drawRaceState(RaceState* rs, Display* display) {
    SDL_RenderCopy(display->renderer, display->resMan->background, NULL, NULL);
    drawGrid(rs->gridLeft, display);
    drawGrid(rs->gridRight, display);
}

// Notifies the RaceState of mouse motion
void RaceState_mouseMotionEvent(RaceState* rs, SDL_MouseMotionEvent e) {
}

// Notifies the RaceState of mouse button presses
void RaceState_mouseButtonEvent(RaceState* rs, SDL_MouseButtonEvent e) {
}

// Notifies the RaceState of key presses
void RaceState_keyEvent(RaceState* rs, SDL_KeyboardEvent e) {
    Grid_keyEvent(rs->gridLeft, e);
    Grid_keyEvent(rs->gridRight, e);
}

// Returns the redraw value and sets it to false
bool RaceState_dropRedraw(RaceState* rs) {
    if (rs->redraw) {
        rs->redraw = false;
        return true;
    }
    return false;
}

// Frees the SoloState and its components
void freeRaceState(RaceState* rs) {
    freeGrid(rs->gridLeft);
    freeGrid(rs->gridRight);
    free(rs);
}
