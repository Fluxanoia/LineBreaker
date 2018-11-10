#include                <SDL.h>
#include             <stdlib.h>
#include          "menuState.h"
#include        "../handling.h"
#include   "gameStateManager.h"
#include "../resourceManager.h"

void wakeState(GSM* gsm);

GSM* initialiseGSM(Display* d, StateType s) {
    GSM* gsm = malloc(sizeof(GSM));
    gsm->currentState = s;
    gsm->redraw = true;
    gsm->closed = false;

    gsm->display = d;

    gsm->dragX = 0;
    gsm->dragY = 0;
    gsm->dragBarAlpha = 0;
    gsm->hoverDragBar = false;
    gsm->dragging = false;

    gsm->menuState = initialiseMenuState(d);
    gsm->loadingState = initialiseLoadingState(d);
    gsm->soloState = initialiseSoloState(d);
    gsm->raceState = initialiseRaceState(d);
    gsm->pauseState = initialisePauseState(d);
    wakeState(gsm);
    return gsm;
}

void wakeState(GSM* gsm) {
    switch (gsm->currentState) {
        case LOADING:
            wakeLoadingState(gsm->loadingState);
        break;
        case MENU:
            wakeMenuState(gsm->menuState);
        break;
        case SINGLEPLAYER:
            wakeSoloState(gsm->soloState);
        break;
        case MULTIPLAYER:
            wakeRaceState(gsm->raceState);
        break;
        case PAUSE:
            wakePauseState(gsm->pauseState);
        break;
        case NIL:
        case CLOSE:
            fprintf(stderr, "State set to NIL\n");
            fail();
        break;
    }
}

void sleepState(GSM* gsm) {
    switch (gsm->currentState) {
        case LOADING:
            sleepLoadingState(gsm->loadingState);
        break;
        case MENU:
            sleepMenuState(gsm->menuState);
        break;
        case SINGLEPLAYER:
            sleepSoloState(gsm->soloState);
        break;
        case MULTIPLAYER:
            sleepRaceState(gsm->raceState);
        break;
        case PAUSE:
            sleepPauseState(gsm->pauseState);
        break;
        case NIL:
        case CLOSE:
            fprintf(stderr, "State set to NIL\n");
            fail();
        break;
    }
}

void changeState(GSM* gsm, StateType st) {
    sleepState(gsm);
    if (st == CLOSE) {
        gsm->closed = true;
        return;
    }
    gsm->currentState = st;
    wakeState(gsm);
}

void updateGSM(GSM* gsm) {
    if (gsm->hoverDragBar) {
        if (gsm->dragBarAlpha < 96) {
            gsm->dragBarAlpha = gsm->dragBarAlpha + 3;
            gsm->redraw = true;
        }
    } else {
        if (gsm->dragBarAlpha > 0) {
            gsm->dragBarAlpha = gsm->dragBarAlpha - 3;
            gsm->redraw = true;
        }
    }
    switch (gsm->currentState) {
        case LOADING:
            updateLoadingState(gsm->loadingState);
            gsm->redraw |= LoadingState_dropRedraw(gsm->loadingState);
            if (gsm->loadingState->nextState != NIL) {
                changeState(gsm, gsm->loadingState->nextState);
            }
        break;
        case MENU:
            updateMenuState(gsm->menuState);
            gsm->redraw |= MenuState_dropRedraw(gsm->menuState);
            if (gsm->menuState->nextState != NIL) {
                changeState(gsm, gsm->menuState->nextState);
            }
        break;
        case SINGLEPLAYER:
            updateSoloState(gsm->soloState);
            gsm->redraw |= SoloState_dropRedraw(gsm->soloState);
            if (gsm->soloState->nextState != NIL) {
                changeState(gsm, gsm->soloState->nextState);
            }
        break;
        case MULTIPLAYER:
            updateRaceState(gsm->raceState);
            gsm->redraw |= RaceState_dropRedraw(gsm->raceState);
            if (gsm->raceState->nextState != NIL) {
                changeState(gsm, gsm->raceState->nextState);
            }
        break;
        case PAUSE:
            updatePauseState(gsm->pauseState);
            gsm->redraw |= PauseState_dropRedraw(gsm->pauseState);
            if (gsm->pauseState->nextState != NIL) {
                changeState(gsm, gsm->pauseState->nextState);
            }
        break;
        case NIL:
        case CLOSE:
            fprintf(stderr, "State set to NIL\n");
            fail();
        break;
    }
}

void drawGSM(GSM* gsm) {
    if (gsm->redraw) {
        gsm->redraw = false;
        SDL_RenderClear(gsm->display->renderer);
    } else {
        return;
    }
    switch (gsm->currentState) {
        case LOADING:
            drawLoadingState(gsm->loadingState, gsm->display);
        break;
        case MENU:
            drawMenuState(gsm->menuState, gsm->display);
        break;
        case SINGLEPLAYER:
            drawSoloState(gsm->soloState, gsm->display);
        break;
        case MULTIPLAYER:
            drawRaceState(gsm->raceState, gsm->display);
        break;
        case PAUSE:
            drawPauseState(gsm->pauseState, gsm->display);
        break;
        case NIL:
        case CLOSE:
            fprintf(stderr, "State set to NIL\n");
            fail();
        break;
    }

    if (gsm->dragBarAlpha > 0) {
        SDL_SetRenderDrawColor(gsm->display->renderer, 255, 255, 255, gsm->dragBarAlpha);
        SDL_Rect bar = (SDL_Rect) { 0, 0, SCREEN_WIDTH, DRAG_BAR_HEIGHT };
        SDL_RenderFillRect(gsm->display->renderer, &bar);
    }
}

void GSM_mouseMotionEvent(GSM* gsm, SDL_MouseMotionEvent e) {
    if (0 < e.y && e.y < DRAG_BAR_HEIGHT) {
        gsm->hoverDragBar = true;
    } else {
        gsm->hoverDragBar = false;
    }
    if (gsm->dragging) {
        int winx = 0;
        int winy = 0;
        SDL_GetWindowPosition(gsm->display->window, &winx, &winy);
        winx += e.x - gsm->dragX;
        winy += e.y - gsm->dragY;
        SDL_SetWindowPosition(gsm->display->window, winx, winy);
    }
    switch (gsm->currentState) {
        case LOADING:
            LoadingState_mouseMotionEvent(gsm->loadingState, e);
        break;
        case MENU:
            MenuState_mouseMotionEvent(gsm->menuState, e);
        break;
        case SINGLEPLAYER:
            SoloState_mouseMotionEvent(gsm->soloState, e);
        break;
        case MULTIPLAYER:
            RaceState_mouseMotionEvent(gsm->raceState, e);
        break;
        case PAUSE:
            PauseState_mouseMotionEvent(gsm->pauseState, e);
        break;
        case NIL:
        case CLOSE:
            fprintf(stderr, "State set to NIL\n");
            fail();
        break;
    }
}

void GSM_mouseButtonEvent(GSM* gsm, SDL_MouseButtonEvent e) {
    if (e.type == SDL_MOUSEBUTTONUP && e.button == SDL_BUTTON_LEFT) {
            gsm->dragging = false;
    }
    if (gsm->hoverDragBar) {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button == SDL_BUTTON_LEFT) {
            gsm->dragging = true;
            gsm->dragX = e.x;
            gsm->dragY = e.y;
        }
    }
    switch (gsm->currentState) {
        case LOADING:
            LoadingState_mouseButtonEvent(gsm->loadingState, e);
        break;
        case MENU:
            MenuState_mouseButtonEvent(gsm->menuState, e);
        break;
        case SINGLEPLAYER:
            SoloState_mouseButtonEvent(gsm->soloState, e);
        break;
        case MULTIPLAYER:
            RaceState_mouseButtonEvent(gsm->raceState, e);
        break;
        case PAUSE:
            PauseState_mouseButtonEvent(gsm->pauseState, e);
        break;
        case NIL:
        case CLOSE:
            fprintf(stderr, "State set to NIL\n");
            fail();
        break;
    }
}

void GSM_keyEvent(GSM* gsm, SDL_KeyboardEvent e) {
    switch (gsm->currentState) {
        case LOADING:
            LoadingState_keyEvent(gsm->loadingState, e);
        break;
        case MENU:
            MenuState_keyEvent(gsm->menuState, e);
        break;
        case SINGLEPLAYER:
            SoloState_keyEvent(gsm->soloState, e);
        break;
        case MULTIPLAYER:
            RaceState_keyEvent(gsm->raceState, e);
        break;
        case PAUSE:
            PauseState_keyEvent(gsm->pauseState, e);
        break;
        case NIL:
        case CLOSE:
            fprintf(stderr, "State set to NIL\n");
            fail();
        break;
    }
}

void freeGSM(GSM* gsm) {
    freeLoadingState(gsm->loadingState);
    freeMenuState(gsm->menuState);
    freeSoloState(gsm->soloState);
    freeRaceState(gsm->raceState);
    freePauseState(gsm->pauseState);
    free(gsm);
}
