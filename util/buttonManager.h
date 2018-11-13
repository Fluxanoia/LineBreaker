#ifndef BMAN_H_
#define BMAN_H_

#include "button.h"

// Defines the maximum amount of buttons in a single
// ButtonManager
#define MAX_BUTTONS 4

// Defines the ButtonManager struct that manages buttons
typedef struct ButtonManager_ {
    // Whether the container should redraw
    bool redraw;
    // The button that has been clicked, -1 if none
    int clicked;
    // The number of buttons created
    int numberOf;
    // The list of buttons
    Button* buttons[MAX_BUTTONS];
} ButtonManager;

ButtonManager* initialiseButtonManager(int n,
        char** titles, int xs[n], int ys[n], Display* d);
void updateButtonManager(ButtonManager* b);
void drawButtonManager(ButtonManager* b, Display* d);
void ButtonManager_mouseMotionEvent(ButtonManager* b, SDL_MouseMotionEvent e);
void ButtonManager_mouseButtonEvent(ButtonManager* b, SDL_MouseButtonEvent e);
bool ButtonManager_dropRedraw(ButtonManager* b);
int ButtonManager_dropClicked(ButtonManager* b);
void freeButtonManager(ButtonManager* b);

#endif
