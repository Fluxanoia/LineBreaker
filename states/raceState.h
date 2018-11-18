#ifndef RACE_H_
#define RACE_H_

#include            <stdbool.h>
#include             "states.h"
#include       "../util/grid.h"
#include "../resourceManager.h"

// Defines the grids' starting x and y values
#define LEFT_GRID_INIT_X 50
#define LEFT_GRID_INIT_Y -720
#define RIGHT_GRID_INIT_X 690
#define RIGHT_GRID_INIT_Y -720
// Defines the score card start x
#define SCORECARD_INIT_Y -500

// Defines the SoloState - a game state
typedef struct RaceState_ {
    // A value holding the state to be changed to
    StateType nextState;
    // Whether the container should redraw or not
    bool redraw;

    // Whether to not wake/sleep the grid for pausing
    bool holdGrid;
    // The game grids to be played on
    Grid* gridLeft;
    Grid* gridRight;

    // The scorecard values
    int sc_w;
    int sc_h;
    int sc_x;
    TweenValue* sc_y;
    // "The winning score is:" texture and values
    int score_text_w;
    int score_text_h;
    SDL_Texture* score_text;
    // "The winner is:" texture and values
    int win_text_w;
    int win_text_h;
    SDL_Texture* win_text;
    // "Press enter to continue" texture and values
    int enter_text_w;
    int enter_text_h;
    SDL_Texture* enter_text;
} RaceState;

RaceState* initialiseRaceState(Display* d);
void wakeRaceState(RaceState* rs);
void sleepRaceState(RaceState* rs);
void updateRaceState(RaceState* rs);
void drawRaceState(RaceState* rs, Display* display);
void RaceState_mouseMotionEvent(RaceState* rs, SDL_MouseMotionEvent e);
void RaceState_mouseButtonEvent(RaceState* rs, SDL_MouseButtonEvent e);
void RaceState_keyEvent(RaceState* rs, SDL_KeyboardEvent e);
bool RaceState_dropRedraw(RaceState* rs);
void RaceState_runTests(RaceState* rs);
void freeRaceState(RaceState* rs);

#endif
