#include        <stdlib.h>
#include       <stdbool.h>
#include "buttonManager.h"

ButtonManager* initialiseButtonManager(int n,
        char** titles, int xs[n], int ys[n], Display* d) {
    ButtonManager* bm = malloc(sizeof(ButtonManager));
    bm->redraw = true;
    bm->numberOf = n;
    bm->clicked = -1;

    for (int i = 0; i < MAX_BUTTONS; i++) {
        if (i >= n) {
            bm->buttons[i] = NULL;
            break;
        }
        bm->buttons[i] = initialiseButton(titles[i], xs[i], ys[i], d);
    }

    return bm;
}

void updateButtonManager(ButtonManager* b) {
    for (int i = 0; i < b->numberOf; i++) {
        updateButton(b->buttons[i]);
        b->redraw |= Button_dropRedraw(b->buttons[i]);
    }
}

void drawButtonManager(ButtonManager* b, Display* d) {
    for (int i = 0; i < b->numberOf; i++) {
        drawButton(b->buttons[i], d);
    }
}

void ButtonManager_mouseMotionEvent(ButtonManager* b, SDL_MouseMotionEvent e) {
    for (int i = 0; i < b->numberOf; i++) {
        Button_mousePosition(b->buttons[i], e.x, e.y);
    }
}

void ButtonManager_mouseButtonEvent(ButtonManager* b, SDL_MouseButtonEvent e) {
    if (e.type == SDL_MOUSEBUTTONUP) {
        if (e.button == SDL_BUTTON_LEFT) {
            for (int i = 0; i < b->numberOf; i++) {
                if (clickButton(b->buttons[i], e.x, e.y)) {
                    b->clicked = i;
                    return;
                }
            }
        }
    }
}


bool ButtonManager_dropRedraw(ButtonManager* b) {
    if (b->redraw) {
        b->redraw = false;
        return true;
    }
    return false;
}

int ButtonManager_dropClicked(ButtonManager* b) {
    if (b->clicked == -1) {
        return -1;
    } else {
        int c = b->clicked;
        b->clicked = -1;
        return c;
    }
}

void freeButtonManager(ButtonManager* b) {
    for (int i = 0; i < b->numberOf; i++) {
        freeButton(b->buttons[i]);
    }
    free(b);
}
