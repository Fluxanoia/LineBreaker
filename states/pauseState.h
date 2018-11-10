#ifndef PAUSE_H_
#define PAUSE_H_

#include            <stdbool.h>
#include             "states.h"
#include "../resourceManager.h"

typedef struct PauseState_ {
    StateType nextState;
    bool redraw;
} PauseState;

PauseState* initialisePauseState(Display* d);
void wakePauseState(PauseState* ps);
void sleepPauseState(PauseState* ps);
void updatePauseState(PauseState* ps);
void drawPauseState(PauseState* ps, Display* display);
void PauseState_mouseMotionEvent(PauseState* ps, SDL_MouseMotionEvent e);
void PauseState_mouseButtonEvent(PauseState* ps, SDL_MouseButtonEvent e);
void PauseState_keyEvent(PauseState* ps, SDL_KeyboardEvent e);
bool PauseState_dropRedraw(PauseState* ps);
void freePauseState(PauseState* ps);

#endif
