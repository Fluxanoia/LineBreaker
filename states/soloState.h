#ifndef SOLO_H_
#define SOLO_H_

#include            <stdbool.h>
#include             "states.h"
#include       "../util/grid.h"
#include "../resourceManager.h"

// Defines the grid's starting x and y values
#define GRID_INIT_X -1
#define GRID_INIT_Y -720
// Defines the score card start x
#define SCORECARD_INIT_Y -500

// Defines the SoloState - a game state
typedef struct SoloState_ {
    // A value holding the state to be changed to
    StateType nextState;
    // Whether the container should redraw or not
    bool redraw;

    // Whether to not wake/sleep the grid for pausing
    bool holdGrid;
    // The game grid to be played on
    Grid* grid;

    // The scorecard values
    int sc_w;
    int sc_h;
    int sc_x;
    TweenValue* sc_y;
    // "Your score was:"
    int score_text_w;
    int score_text_h;
    SDL_Texture* score_text;
    // "Press enter to continue"
    int enter_text_w;
    int enter_text_h;
    SDL_Texture* enter_text;
} SoloState;

SoloState* initialiseSoloState(Display* d);
void wakeSoloState(SoloState* ss);
void sleepSoloState(SoloState* ss);
void updateSoloState(SoloState* ss);
void drawSoloState(SoloState* ss, Display* display);
void SoloState_mouseMotionEvent(SoloState* ss, SDL_MouseMotionEvent e);
void SoloState_mouseButtonEvent(SoloState* ss, SDL_MouseButtonEvent e);
void SoloState_keyEvent(SoloState* ss, SDL_KeyboardEvent e);
bool SoloState_dropRedraw(SoloState* ss);
void SoloState_runTests(SoloState* ss);
void freeSoloState(SoloState* ss);

#endif
