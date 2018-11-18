#ifndef LOAD_STATE_H_
#define LOAD_STATE_H_

#include      "../util/tween.h"
#include "../resourceManager.h"

// Defines the height that the title is shown at
#define TITLE_HEIGHT 220

// Defines the LoadingState struct - a game state
typedef struct LoadingState_ {
    // A variable containing the state to be changed to
    StateType nextState;
    // A variable containing whether the container should redraw
    bool redraw;
    // TweenValues holding values of:
    // Background opacity
    TweenValue* bgAlpha;
    // The 'LINE' X value
    TweenValue* lineX;
    // The 'BREAKER' Y value
    TweenValue* breakerX;
    // The width and height of the title pieces
    int lineX_width;
    int lineX_height;
    int breakerX_width;
    int breakerX_height;
} LoadingState;

LoadingState* initialiseLoadingState(Display* d);
void wakeLoadingState(LoadingState* ls);
void sleepLoadingState(LoadingState* ls);
void updateLoadingState(LoadingState* ls);
void drawLoadingState(LoadingState* ls, Display* display);
void LoadingState_mouseMotionEvent(LoadingState* ls, SDL_MouseMotionEvent e);
void LoadingState_mouseButtonEvent(LoadingState* ls, SDL_MouseButtonEvent e);
void LoadingState_keyEvent(LoadingState* ls, SDL_KeyboardEvent e);
bool LoadingState_dropRedraw(LoadingState* ls);
void LoadingState_runTests(LoadingState* ls);
void freeLoadingState(LoadingState* ls);

#endif
