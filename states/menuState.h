#ifndef MENU_STATE_H_
#define MENU_STATE_H_

#include               <stdbool.h>
#include                "states.h"
#include         "../util/tween.h"
#include    "../resourceManager.h"
#include "../util/buttonManager.h"

// Defines the MenuState - a game state
typedef struct MenuState_ {
    // A value holding the state to be changed to
    StateType nextState;
    // A value holding the state that will be changed to after
    // tweening happens
    StateType pushState;
    // Whether the container should redraw or not
    bool redraw;
    // The widths and heights of the menu cover and the title
    int cover_w, cover_h;
    int title_w, title_h;
    // The position of the menu cover and title
    TweenValue* coverSlide;
    TweenValue* titleSlide;
    // The button manager    
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
