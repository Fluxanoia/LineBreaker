#ifndef RACE_H_
#define RACE_H_

#include            <stdbool.h>
#include             "states.h"
#include       "../util/grid.h"
#include "../resourceManager.h"

// Defines the grids' starting x and y values
#define LEFT_GRID_INIT_X 50
#define LEFT_GRID_INIT_Y -720
#define RIGHT_GRID_INIT_X 690
#define RIGHT_GRID_INIT_Y -720

// Defines the SoloState - a game state
typedef struct RaceState_ {
    // A value holding the state to be changed to
    StateType nextState;
    // Whether the container should redraw or not
    bool redraw;

    // The game grid to be played on
    Grid* gridLeft;
    Grid* gridRight;
} RaceState;

RaceState* initialiseRaceState(Display* d);
void wakeRaceState(RaceState* rs);
void sleepRaceState(RaceState* rs);
void updateRaceState(RaceState* rs);
void drawRaceState(RaceState* rs, Display* display);
void RaceState_mouseMotionEvent(RaceState* rs, SDL_MouseMotionEvent e);
void RaceState_mouseButtonEvent(RaceState* rs, SDL_MouseButtonEvent e);
void RaceState_keyEvent(RaceState* rs, SDL_KeyboardEvent e);
bool RaceState_dropRedraw(RaceState* rs);
void freeRaceState(RaceState* rs);

#endif
