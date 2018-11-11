#ifndef MENU_STATE_H_
#define MENU_STATE_H_

#include               <stdbool.h>
#include                "states.h"
#include         "../util/tween.h"
#include    "../resourceManager.h"
#include "../util/buttonManager.h"

typedef struct MenuState_ {
    StateType nextState;
    StateType pushState;
    bool redraw;

    int cover_w, cover_h;
    int title_w, title_h;
    TweenValue* coverSlide;
    TweenValue* titleSlide;
    
    ButtonManager* buttonManager;
} MenuState;

MenuState* initialiseMenuState(Display* d);
void wakeMenuState(MenuState* ms);
void sleepMenuState(MenuState* ms);
void updateMenuState(MenuState* menu);
void drawMenuState(MenuState* menu, Display* display);
void MenuState_mouseMotionEvent(MenuState* ms, SDL_MouseMotionEvent e);
void MenuState_mouseButtonEvent(MenuState* ms, SDL_MouseButtonEvent e);
void MenuState_keyEvent(MenuState* ms, SDL_KeyboardEvent e);
bool MenuState_dropRedraw(MenuState* ms);
void freeMenuState(MenuState* ms);

#endif
