#ifndef SOLO_H_
#define SOLO_H_

#include            <stdbool.h>
#include             "states.h"
#include       "../util/grid.h"
#include "../resourceManager.h"

typedef struct SoloState_ {
    StateType nextState;
    bool redraw;

    Grid* grid;
} SoloState;

SoloState* initialiseSoloState(Display* d);
void wakeSoloState(SoloState* ss);
void sleepSoloState(SoloState* ss);
void updateSoloState(SoloState* ss);
void drawSoloState(SoloState* ss, Display* display);
void SoloState_mouseMotionEvent(SoloState* ss, SDL_MouseMotionEvent e);
void SoloState_mouseButtonEvent(SoloState* ss, SDL_MouseButtonEvent e);
void SoloState_keyEvent(SoloState* ss, SDL_KeyboardEvent e);
bool SoloState_dropRedraw(SoloState* ss);
void freeSoloState(SoloState* ss);

#endif
