#ifndef GRID_H_
#define GRID_H_

#define GRID_BOX_SIZE   32
#define GRID_MAX        4
#define HIDDEN_ROWS     4
#define GRID_WIDTH      10
#define GRID_HEIGHT     24

#define LATERAL_DELAY   3

#define TETROMINO_SIZE  4
#define TETROMINO_COUNT 7

#include              "tween.h"
#include "../resourceManager.h"

typedef enum TetrominoType_ { NO_PIECE, I, O, T, J, L, S, Z } TetrominoType;

typedef struct Tetromino_ {
    TetrominoType tet_type;
    int bound_x;
    int bound_y;
    int colour;

    int box_xs[4];
    int box_ys[4];
} Tetromino;

typedef struct Grid_ {
    bool redraw;
    int grid[GRID_HEIGHT][GRID_WIDTH];
    
    int init_x;
    int init_y;
    TweenValue* x;
    TweenValue* y;

    bool gameover;

    int ticks;
    int speed;
    bool double_time;

    bool moveLeft;
    bool moveRight;
    bool rotated;

    int boardBackWidth;
    int boardBackHeight;

    Tetromino current;
    TetrominoType next;
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
