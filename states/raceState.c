#include                <SDL.h>
#include            <stdbool.h>
#include             "states.h"
#include          "raceState.h"
#include "../resourceManager.h"

// Creates a RaceState and returns the pointer
RaceState* initialiseRaceState(Display* d) {
    RaceState* rs = malloc(sizeof(RaceState));
    rs->redraw = true;
    rs->nextState = NIL;

    rs->holdGrid = false;

    GridKeyboardLayout gkl_left;
    gkl_left.LEFT         = SDLK_a;
    gkl_left.RIGHT        = SDLK_d;
    gkl_left.ROTATE       = SDLK_w;
    gkl_left.FAST_DROP    = SDLK_s;
    gkl_left.INSTANT_DROP = SDLK_SPACE;
    gkl_left.SWITCH_HELD  = SDLK_e;
    rs->gridLeft = initialiseGrid(LEFT_GRID_INIT_X, 
            LEFT_GRID_INIT_Y, gkl_left, d);
    
    GridKeyboardLayout gkl_right;
    gkl_right.LEFT         = SDLK_LEFT;
    gkl_right.RIGHT        = SDLK_RIGHT;
    gkl_right.ROTATE       = SDLK_UP;
    gkl_right.FAST_DROP    = SDLK_DOWN;
    gkl_right.INSTANT_DROP = SDLK_PERIOD;
    gkl_left.SWITCH_HELD   = SDLK_RETURN;
    rs->gridRight = initialiseGrid(RIGHT_GRID_INIT_X, 
            RIGHT_GRID_INIT_Y, gkl_right, d);

    rs->sc_y = initialiseTween(SCORECARD_INIT_Y);
    SDL_QueryTexture(d->resMan->score_card, NULL, NULL, &rs->sc_w, &rs->sc_h);
    rs->sc_x = (SCREEN_WIDTH - rs->sc_w) / 2;    

    char sc_title[] = "The winning score is:";
    char wn_title[] = "The winner is:";
    char en_title[] = "Press enter to continue...";
    rs->score_text = createSmallText(sc_title, &rs->score_text_w,
            &rs->score_text_h, *d->resMan->text_colour, d);
    rs->win_text = createSmallText(wn_title, &rs->win_text_w,
            &rs->win_text_h, *d->resMan->text_colour, d);
    rs->enter_text = createSmallText(en_title, &rs->enter_text_w,
            &rs->enter_text_h, *d->resMan->text_colour, d);
    return rs;
}

// Wakes the RaceState from sleep
void wakeRaceState(RaceState* rs) {
    if (!rs->holdGrid) {
        wakeGrid(rs->gridLeft);
        wakeGrid(rs->gridRight);
        moveTweenValue(rs->gridLeft->y, EASE_OUT, 0, 60, 0);
        moveTweenValue(rs->gridRight->y, EASE_OUT, 0, 60, 0);
    }
    rs->holdGrid = false;
}  

// Sleeps the RaceState so it can be stored
void sleepRaceState(RaceState* rs) {
    rs->nextState = NIL;
    rs->sc_y->id = 0;
    if (!rs->holdGrid) {
        sleepGrid(rs->gridLeft);
        sleepGrid(rs->gridRight);
        setGridPosition(rs->gridLeft, LEFT_GRID_INIT_X, LEFT_GRID_INIT_Y);
        setGridPosition(rs->gridRight, RIGHT_GRID_INIT_X, RIGHT_GRID_INIT_Y);
    }
}

// Updates the RaceState
void updateRaceState(RaceState* rs) {
    updateGrid(rs->gridLeft);
    rs->redraw |= Grid_dropRedraw(rs->gridLeft);
    updateGrid(rs->gridRight);
    rs->redraw |= Grid_dropRedraw(rs->gridRight);
    updateTweenValue(rs->sc_y);
    rs->redraw |= TweenValue_dropRedraw(rs->sc_y);
    // If the game is over...
    if (rs->gridLeft->gameover && rs->gridRight->gameover) {
        // If the tweening has finished, show the score and end. 
        // Otherwise, if it hasn't started, tween.
        if (arrived(rs->gridLeft->y) && rs->gridLeft->y->id == 1) {
            if (rs->sc_y->id == 0) {
                moveTweenValue(rs->sc_y, ELASTIC, 100, 40, 5);
                rs->sc_y->id = 1;
            }
            if (arrived(rs->sc_y) && rs->sc_y->id == 2) {
                rs->nextState = MENU;
            } 
        } else if (rs->gridLeft->y->id == 0) {
            rs->gridLeft->y->id = 1;
            moveTweenValue(rs->gridLeft->y, EASE_OUT, -SCREEN_HEIGHT, 60, 20);
            moveTweenValue(rs->gridRight->y, EASE_OUT, -SCREEN_HEIGHT, 60, 20);
        }
    }
}

// Draws the RaceState
void drawRaceState(RaceState* rs, Display* display) {
    SDL_RenderCopy(display->renderer, display->resMan->background, NULL, NULL);
    drawGrid(rs->gridLeft, display);
    drawGrid(rs->gridRight, display);

    int sc_y = getTweenValue(rs->sc_y);
    if (sc_y + rs->sc_h >= 0) {
        SDL_Rect dstrect = (SDL_Rect) { rs->sc_x, sc_y, rs->sc_w, rs->sc_h };
        SDL_RenderCopy(display->renderer, display->resMan->score_card,
                NULL, &dstrect);

        dstrect.x += 20;
        dstrect.y += 15;
        dstrect.w = rs->score_text_w;
        dstrect.h = rs->score_text_h;
        SDL_RenderCopy(display->renderer, rs->score_text, NULL, &dstrect);

        SDL_Texture* win_score;
        SDL_Texture* winner;
        char title[24];
        if (rs->gridLeft->score > rs->gridRight->score) {
            win_score = rs->gridLeft->score_tex;
            strcpy(title, "Player One (Left)");
        } else if (rs->gridLeft->score < rs->gridRight->score) {
            win_score = rs->gridRight->score_tex;
            strcpy(title, "Player Two (Right)");
        } else {
            win_score = rs->gridLeft->score_tex;
            strcpy(title, "Both Players");
        }

        dstrect.x += 50;
        dstrect.y += dstrect.h + 10;
        SDL_QueryTexture(win_score, NULL, NULL, &dstrect.w, &dstrect.h);
        SDL_RenderCopy(display->renderer, win_score, NULL, &dstrect);
        dstrect.x -= 50;       

        dstrect.y += dstrect.h + 10;  
        dstrect.w = rs->win_text_w;
        dstrect.h = rs->win_text_h; 
        SDL_RenderCopy(display->renderer, rs->win_text, NULL, &dstrect);

        dstrect.x += 50;       
        dstrect.y += dstrect.h + 10;  
        winner = createSmallText(title, &dstrect.w, &dstrect.h,
                *display->resMan->text_colour, display);
        SDL_RenderCopy(display->renderer, winner, NULL, &dstrect);        
        SDL_DestroyTexture(winner);
        dstrect.x -= 50;       

        dstrect.w = rs->enter_text_w;
        dstrect.h = rs->enter_text_h;
        dstrect.y = (sc_y + rs->sc_h) - (dstrect.h + 15);
        SDL_RenderCopy(display->renderer, rs->enter_text, NULL, &dstrect);

    }
}

// Notifies the RaceState of mouse motion
void RaceState_mouseMotionEvent(RaceState* rs, SDL_MouseMotionEvent e) {
}

// Notifies the RaceState of mouse button presses
void RaceState_mouseButtonEvent(RaceState* rs, SDL_MouseButtonEvent e) {
}

// Notifies the RaceState of key presses
void RaceState_keyEvent(RaceState* rs, SDL_KeyboardEvent e) {
    switch (e.keysym.sym) {
        case SDLK_RETURN:
            if (arrived(rs->sc_y) && rs->sc_y->id == 1) {
                moveTweenValue(rs->sc_y, EASE_OUT, SCORECARD_INIT_Y, 40, 0);
                rs->sc_y->id = 2;
            }
        break;
        case SDLK_p:
            if (rs->gridLeft->gameover && rs->gridRight->gameover) break;
            rs->holdGrid = true;
            rs->nextState = PAUSE;
            Grid_pause(rs->gridLeft);
            Grid_pause(rs->gridRight);
        break;
    }
    Grid_keyEvent(rs->gridLeft, e);
    Grid_keyEvent(rs->gridRight, e);
}

// Returns the redraw value and sets it to false
bool RaceState_dropRedraw(RaceState* rs) {
    if (rs->redraw) {
        rs->redraw = false;
        return true;
    }
    return false;
}

void RaceState_runTests(RaceState* rs) {}

// Frees the SoloState and its components
void freeRaceState(RaceState* rs) {
    SDL_DestroyTexture(rs->score_text);
    SDL_DestroyTexture(rs->win_text);
    SDL_DestroyTexture(rs->enter_text);
    freeGrid(rs->gridLeft);
    freeGrid(rs->gridRight);
    free(rs);
}
