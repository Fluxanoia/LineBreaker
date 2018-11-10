#ifndef BMAN_H_
#define BMAN_H_

#include "button.h"

#define MAX_BUTTONS 4

typedef struct ButtonManager_ {
    bool redraw;

    int clicked;

    int numberOf;
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
