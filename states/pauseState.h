#ifndef PAUSE_H_
#define PAUSE_H_

#include               <stdbool.h>
#include                "states.h"
#include         "../util/tween.h"
#include    "../resourceManager.h"
#include "../util/buttonManager.h"

#define WINDOW_H      400
#define WINDOW_W      ((double) SCREEN_WIDTH / (double) SCREEN_HEIGHT) * WINDOW_H
#define WINDOW_Y      ((SCREEN_HEIGHT - WINDOW_H) / 2)
#define WINDOW_X      400
     
#define BUTTON_X      22

#define BUTTON_NAME_1 "Back"
#define BUTTON_NAME_2 "Quit"

#define BUTTON_Y_1    500
#define BUTTON_Y_2    600
 
#define NO_BUTTONS    2

typedef struct PauseState_ {
    StateType nextState;
    StateType pushState;
    Display* display;
    bool redraw;

    SDL_Texture* win;
    TweenValue* win_x;
    TweenValue* win_y;
    TweenValue* win_w;
    TweenValue* win_h;

    ButtonManager* buttonManager;
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
void PauseState_runTests(PauseState* ps);
void freePauseState(PauseState* ps);

#endif
