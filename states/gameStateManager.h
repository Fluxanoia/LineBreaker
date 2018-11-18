#ifndef GSM_H_
#define GSM_H_

#include       "states.h"
#include    "menuState.h"
#include    "soloState.h"
#include    "raceState.h"
#include   "pauseState.h"
#include "loadingState.h"

// Defines the height of the drag bar at the top of the screen
#define DRAG_BAR_HEIGHT 40

// Defines the GameStateManager struct
typedef struct GameStateManager_ {
    // Whether to redraw or not
    bool redraw;
    // Whether the game should be closed or not
    bool closed;
    // The current, running state
    StateType currentState;
    // The state running prior to the current
    StateType lastState;

    // The X and Y coordinates where dragging began
    int dragX;
    int dragY;
    // The opacity of the drag bar
    int dragBarAlpha;
    // Whether the mouse is on the drag bar
    bool hoverDragBar;
    // Whether the mouse is dragging the drag bar
    bool dragging;
    
    // The game display
    Display* display;

    // All the game states
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
void GSM_runTests(GSM* gsm);
void freeGSM(GSM* gsm);

#endif
