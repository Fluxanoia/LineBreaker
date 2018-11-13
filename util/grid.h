#ifndef GRID_H_
#define GRID_H_

#define GRID_BOX_SIZE   32
#define GRID_MAX        4
#define START_ROW       2
#define HIDDEN_ROWS     4
#define GRID_WIDTH      10
#define GRID_HEIGHT     24
#define TAB_MAX         250

#define LOCK_PAUSE      10
#define LATERAL_DELAY   3
#define LATERAL_PAUSE   12

#define BASE_SCORE      5

#define TETROMINO_SIZE  4
#define TETROMINO_COUNT 7

#define SHADOW_OPACITY  64

#include                   "tween.h"
#include      "../resourceManager.h"
#include "../aesthetics/particles.h"

typedef enum TetrominoType_ { NO_PIECE, I, O, T, J, L, S, Z } TetrominoType;

typedef struct Tetromino_ {
    TetrominoType tet_type;
    int bound_x;
    int bound_y;
    int colour;

    int box_xs[TETROMINO_SIZE];
    int box_ys[TETROMINO_SIZE];
} Tetromino;

typedef struct Grid_ {
    bool redraw;
    int grid[GRID_HEIGHT][GRID_WIDTH];
    
    TweenValue* x;
    TweenValue* y;
    TweenValue* tabSlide;

    int score;
    int scoreTitleWidth;
    int scoreTitleHeight;
    int nextTitleWidth;
    int nextTitleHeight;
    int heldTitleWidth;
    int heldTitleHeight;
    SDL_Texture* score_title;
    SDL_Texture* next_title;
    SDL_Texture* held_title;

    bool gameover;

    int ticks;
    int lockHold;

    int speed;
    bool double_time;

    int movePause;
    bool moveLeft;
    bool moveRight;
    bool rotated;
    bool fastDropped;

    int tabWidth;
    int tabHeight;
    int boardBackWidth;
    int boardBackHeight;

    Tetromino current;
    int nextColour;
    TetrominoType next;
    int heldColour;
    TetrominoType held;
    bool switchedHeld;
    TweenValue* shadow;

    Particles* particles[GRID_HEIGHT][GRID_WIDTH];
    int colours[GRID_HEIGHT][GRID_WIDTH];
} Grid;

Grid* initialiseGrid();
void wakeGrid(Grid* grid);
void sleepGrid(Grid* grid);
void updateGrid(Grid* grid);
void drawGrid(Grid* grid, Display* d);
void Grid_keyEvent(Grid* grid, SDL_KeyboardEvent e);
bool Grid_dropRedraw(Grid* grid);
void freeGrid(Grid* grid);

#endif
