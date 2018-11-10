#ifndef RACE_H_
#define RACE_H_

#include            <stdbool.h>
#include             "states.h"
#include "../resourceManager.h"

typedef struct RaceState_ {
    StateType nextState;
    bool redraw;
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
