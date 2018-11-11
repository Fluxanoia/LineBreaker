#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "menuState.h"
#include "../resourceManager.h"

#define SLIDE_OFF     -600
#define SLIDE_ON      -100
#define BUTTON_OFF_X  -450
#define BUTTON_ON_X   22
#define TITLE_OFF_X   -450
#define TITLE_ON_X    22

#define BUTTON_NAME_1 "Singleplayer"
#define BUTTON_NAME_2 "Multiplayer"
#define BUTTON_NAME_3 "Quit"

#define BUTTON_Y_1    325
#define BUTTON_Y_2    425
#define BUTTON_Y_3    525
 
#define NO_BUTTONS    3

MenuState* initialiseMenuState(Display* d) {
    MenuState* ms = malloc(sizeof(MenuState));
    ms->redraw = true;
    ms->nextState = NIL;
    ms->pushState = NIL;

    SDL_QueryTexture(d->resMan->menu_cover, NULL, NULL,
            &(ms->cover_w), &(ms->cover_h));
    ms->coverSlide = initialiseTween(SLIDE_OFF);

    SDL_QueryTexture(d->resMan->title_small, NULL, NULL,
            &(ms->title_w), &(ms->title_h));
    ms->titleSlide = initialiseTween(TITLE_OFF_X);

    char* ts[NO_BUTTONS] = { BUTTON_NAME_1, BUTTON_NAME_2, BUTTON_NAME_3 };
    int xs[] = {BUTTON_OFF_X, BUTTON_OFF_X, BUTTON_OFF_X};
    int ys[] = {BUTTON_Y_1, BUTTON_Y_2, BUTTON_Y_3};
    ms->buttonManager = initialiseButtonManager(3, ts, xs, ys, d);

    return ms;
}


void wakeMenuState(MenuState* ms) {
    moveTweenValue(ms->coverSlide, EASE_OUT, SLIDE_ON, 50, 10);
    moveTweenValue(ms->titleSlide, EASE_OUT, TITLE_ON_X, 70, 45);
    for (int i = 0; i < ms->buttonManager->numberOf; i++) {
        moveTweenValue(ms->buttonManager->buttons[i]->x,
                EASE_OUT, BUTTON_ON_X, 70, 55 + 10 * i);
    }
}  

void sleepMenuState(MenuState* ms) {
    ms->nextState = NIL;
    ms->coverSlide->id = 0;
    setTweenValue(ms->coverSlide, SLIDE_OFF);
    setTweenValue(ms->titleSlide, TITLE_OFF_X);
    for (int i = 0; i < ms->buttonManager->numberOf; i++) {
        setTweenValue(ms->buttonManager->buttons[i]->x, BUTTON_OFF_X);
    }
}

void pushState(MenuState* ms, StateType st) {
    ms->pushState = st;
    ms->coverSlide->id = 1;
    moveTweenValue(ms->coverSlide, EASE_OUT, SLIDE_OFF, 50, 55);
    moveTweenValue(ms->titleSlide, EASE_OUT, TITLE_OFF_X, 70, 0);
    for (int i = 0; i < ms->buttonManager->numberOf; i++) {
        moveTweenValue(ms->buttonManager->buttons[i]->x,
                EASE_OUT, BUTTON_OFF_X, 70, 10 * (i + 1));
    }
}

void buttonEvent(MenuState* ms, int i) {
    switch (i) {
        case 0:
            pushState(ms, SINGLEPLAYER);
        break;
        case 1:
            pushState(ms, MULTIPLAYER);
        break;
        case 2:
            pushState(ms, CLOSE);
        break;
    }
}

void updateMenuState(MenuState* ms) {
    updateTweenValue(ms->coverSlide);
    ms->redraw |= TweenValue_dropRedraw(ms->coverSlide);

    updateTweenValue(ms->titleSlide);
    ms->redraw |= TweenValue_dropRedraw(ms->titleSlide);

    updateButtonManager(ms->buttonManager);
    ms->redraw |= ButtonManager_dropRedraw(ms->buttonManager);
    buttonEvent(ms, ButtonManager_dropClicked(ms->buttonManager));

    if (arrived(ms->coverSlide) && ms->coverSlide->id == 1) {
        ms->nextState = ms->pushState;
    }
}

void drawMenuState(MenuState* ms, Display* display) {
    SDL_RenderCopy(display->renderer, display->resMan->background, NULL, NULL);

    double x;
    SDL_Rect dstrect;

    x = getTweenValue(ms->coverSlide);
    dstrect = (SDL_Rect) { x, 0, ms->cover_w, ms->cover_h };
    SDL_RenderCopy(display->renderer, display->resMan->menu_cover,
            NULL, &dstrect);

    x = getTweenValue(ms->titleSlide);
    dstrect = (SDL_Rect) { x, 10, ms->title_w, ms->title_h };
    SDL_RenderCopy(display->renderer, display->resMan->title_small, NULL, &dstrect);

    drawButtonManager(ms->buttonManager, display);
}

void MenuState_mouseMotionEvent(MenuState* ms, SDL_MouseMotionEvent e) {
    ButtonManager_mouseMotionEvent(ms->buttonManager, e);
}

void MenuState_mouseButtonEvent(MenuState* ms, SDL_MouseButtonEvent e) {
    ButtonManager_mouseButtonEvent(ms->buttonManager, e);
}

void MenuState_keyEvent(MenuState* ms, SDL_KeyboardEvent e) {

}

bool MenuState_dropRedraw(MenuState* ms) {
    if (ms->redraw) {
        ms->redraw = false;
        return true;
    }
    return false;
}

void freeMenuState(MenuState* ms) {
    freeTweenValue(ms->coverSlide);
    freeButtonManager(ms->buttonManager);
    free(ms);
}
