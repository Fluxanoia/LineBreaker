#ifndef LOAD_STATE_H_
#define LOAD_STATE_H_

#include      "../util/tween.h"
#include "../resourceManager.h"

typedef struct LoadingState_ {
    StateType nextState;
    bool redraw;
    
    TweenValue* bgAlpha;
    TweenValue* lineX;
    TweenValue* breakerX;

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
void freeLoadingState(LoadingState* ls);

#endif
