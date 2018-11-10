#ifndef GSM_H_
#define GSM_H_

#include       "states.h"
#include    "menuState.h"
#include    "soloState.h"
#include    "raceState.h"
#include   "pauseState.h"
#include "loadingState.h"

#define DRAG_BAR_HEIGHT 40

typedef struct GameStateManager_ {
    bool redraw;
    bool closed;
    StateType currentState;

    int dragX;
    int dragY;
    int dragBarAlpha;
    bool hoverDragBar;
    bool dragging;

    Display* display;

    MenuState* menuState;
    LoadingState* loadingState;
    SoloState* soloState;
    RaceState* raceState;
    PauseState* pauseState;
} GSM;

GSM* initialiseGSM(Display* d, StateType s);
void updateGSM(GSM* gsm);
bool drawGSM(GSM* gsm);
void GSM_mouseMotionEvent(GSM* gsm, SDL_MouseMotionEvent e);
void GSM_mouseButtonEvent(GSM* gsm, SDL_MouseButtonEvent e);
void GSM_keyEvent(GSM* gsm, SDL_KeyboardEvent e);
void freeGSM(GSM* gsm);

#endif
