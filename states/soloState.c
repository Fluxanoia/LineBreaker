#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "soloState.h"
#include "../resourceManager.h"

// Creates a SoloState and returns the pointer
SoloState* initialiseSoloState(Display* d) {
    SoloState* ss = malloc(sizeof(SoloState));
    ss->redraw = true;
    ss->nextState = NIL;

    ss->holdGrid = false;

    GridKeyboardLayout gkl;
    gkl.LEFT         = SDLK_a;
    gkl.RIGHT        = SDLK_d;
    gkl.ROTATE       = SDLK_w;
    gkl.FAST_DROP    = SDLK_s;
    gkl.INSTANT_DROP = SDLK_SPACE;
    gkl.SWITCH_HELD  = SDLK_LSHIFT;
    ss->grid = initialiseGrid(GRID_INIT_X, GRID_INIT_Y, gkl, d);

    ss->sc_y = initialiseTween(SCORECARD_INIT_Y);
    SDL_QueryTexture(d->resMan->score_card, NULL, NULL, &ss->sc_w, &ss->sc_h);
    ss->sc_x = (SCREEN_WIDTH - ss->sc_w) / 2;    

    char sc_title[] = "Your score was:";
    char en_title[] = "Press enter to continue...";
    ss->score_text = createSmallText(sc_title, &ss->score_text_w,
            &ss->score_text_h, *d->resMan->text_colour, d);
    ss->enter_text = createSmallText(en_title, &ss->enter_text_w,
            &ss->enter_text_h, *d->resMan->text_colour, d);
    return ss;
}

// Wakes the SoloState from sleep
void wakeSoloState(SoloState* ss) {
    if (!ss->holdGrid) { 
        wakeGrid(ss->grid);
        moveTweenValue(ss->grid->y, EASE_OUT, 0, 60, 0);
    }
    ss->holdGrid = false;
}  

// Sleeps the SoloState so it can be stored
void sleepSoloState(SoloState* ss) {
    ss->nextState = NIL;
    ss->sc_y->id = 0;
    setTweenValue(ss->sc_y, SCORECARD_INIT_Y);
    if (!ss->holdGrid)  {
        sleepGrid(ss->grid);
        setGridPosition(ss->grid, GRID_INIT_X, GRID_INIT_Y);
    }
}

// Updates the SoloState
void updateSoloState(SoloState* ss) {
    updateGrid(ss->grid);
    ss->redraw |= Grid_dropRedraw(ss->grid);
    updateTweenValue(ss->sc_y);
    ss->redraw |= TweenValue_dropRedraw(ss->sc_y);
    // If the game is over...
    if (ss->grid->gameover) {
        // If the tweening has finished, show the score and end. 
        // Otherwise, if it hasn't started, tween.
        if (arrived(ss->grid->y) && ss->grid->y->id == 1) {
            if (ss->sc_y->id == 0) {
                moveTweenValue(ss->sc_y, ELASTIC, 100, 40, 5);
                ss->sc_y->id = 1;
            }
            if (arrived(ss->sc_y) && ss->sc_y->id == 2) {
                ss->nextState = MENU;
            } 
        } else if (ss->grid->y->id == 0) {
            ss->grid->y->id = 1;
            moveTweenValue(ss->grid->y, EASE_OUT, -SCREEN_HEIGHT, 60, 20);
        }
    }
}

// Draws the SoloState
void drawSoloState(SoloState* ss, Display* display) {
    SDL_RenderCopy(display->renderer, display->resMan->background, NULL, NULL);
    drawGrid(ss->grid, display);

    int sc_y = getTweenValue(ss->sc_y);
    if (sc_y + ss->sc_h >= 0) {
        SDL_Rect dstrect = (SDL_Rect) { ss->sc_x, sc_y, ss->sc_w, ss->sc_h };
        SDL_RenderCopy(display->renderer, display->resMan->score_card,
                NULL, &dstrect);

        dstrect.x += 20;
        dstrect.y += 15;
        dstrect.w = ss->score_text_w;
        dstrect.h = ss->score_text_h;
        SDL_RenderCopy(display->renderer, ss->score_text, NULL, &dstrect);

        dstrect.x += 50;
        dstrect.y += dstrect.h + 10;
        SDL_QueryTexture(ss->grid->score_tex, NULL, NULL, &dstrect.w, &dstrect.h);
        SDL_RenderCopy(display->renderer, ss->grid->score_tex, NULL, &dstrect);
        dstrect.x -= 50;        

        dstrect.w = ss->enter_text_w;
        dstrect.h = ss->enter_text_h;
        dstrect.y = (sc_y + ss->sc_h) - (dstrect.h + 15);
        SDL_RenderCopy(display->renderer, ss->enter_text, NULL, &dstrect);

    }
}

// Notifies the SoloState of mouse motion
void SoloState_mouseMotionEvent(SoloState* ss, SDL_MouseMotionEvent e) {
}

// Notifies the SoloState of mouse button presses
void SoloState_mouseButtonEvent(SoloState* ss, SDL_MouseButtonEvent e) {
}

// Notifies the SoloState of key presses
void SoloState_keyEvent(SoloState* ss, SDL_KeyboardEvent e) {
    switch (e.keysym.sym) {
        case SDLK_RETURN:
            if (arrived(ss->sc_y) && ss->sc_y->id == 1) {
                moveTweenValue(ss->sc_y, EASE_OUT, SCORECARD_INIT_Y, 40, 0);
                ss->sc_y->id = 2;
            }
        break;
        case SDLK_p:
            if (ss->grid->gameover) break;
            ss->holdGrid = true;
            ss->nextState = PAUSE;
            Grid_pause(ss->grid);
        break;
    }
    Grid_keyEvent(ss->grid, e);
}

// Returns the redraw value and sets it to false
bool SoloState_dropRedraw(SoloState* ss) {
    if (ss->redraw) {
        ss->redraw = false;
        return true;
    }
    return false;
}

// Run tests
void SoloState_runTests(SoloState* ss) {
    wakeGrid(ss->grid);
    Grid_runTests(ss->grid);
}

// Frees the SoloState and its components
void freeSoloState(SoloState* ss) {
    SDL_DestroyTexture(ss->score_text);
    SDL_DestroyTexture(ss->enter_text);
    freeGrid(ss->grid);
    freeTweenValue(ss->sc_y);
    free(ss);
}
