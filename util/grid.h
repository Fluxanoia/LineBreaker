#ifndef GRID_H_
#define GRID_H_

// The value that corresponds to centering the Grid x or y
// on initialisation
#define GRID_CENTER     -1

// The size of a grid box
#define GRID_BOX_SIZE   32
// The possible colours and hence, maximum grid element
#define GRID_MAX        4
// The row where tetrominoes spawn
#define START_ROW       2
// The rows not rendered
#define HIDDEN_ROWS     4
// The width of grid in elements
#define GRID_WIDTH      10
// The height of grid in elements
#define GRID_HEIGHT     24

// How much of the backing board is obscured by the top of the window
#define BACKING_DIP     100
// How far the tab sticks out
#define TAB_MAX         225

// How long the game waits before locking in a piece
#define LOCK_PAUSE      10
// The delay between lateral movements
#define LATERAL_DELAY   3
// The pause between the first and subsequent lateral movements
#define LATERAL_PAUSE   12

// The base awarded score
#define BASE_SCORE      5

// The amount of boxes that make up a tetromino
#define TETROMINO_SIZE  4
// The amount of tetrominoes
#define TETROMINO_COUNT 7

// The maximum opacity of the piece shadow
#define SHADOW_OPACITY  64

#include                   "tween.h"
#include      "../resourceManager.h"
#include "../aesthetics/particles.h"

// All the different types of tetrominoes
typedef enum TetrominoType_ { NO_PIECE, I, O, T, J, L, S, Z } TetrominoType;

// Defines the Tetromino struct - the pieces in the game
typedef struct Tetromino_ {
    // The type of piece
    TetrominoType tet_type;
    // The bounding boxes x and y values
    int bound_x;
    int bound_y;
    // The colour (grid value) of the Tetromino
    int colour;
    // The coordinates of the tetromino elements within the bounding box
    int box_xs[TETROMINO_SIZE];
    int box_ys[TETROMINO_SIZE];
} Tetromino;

typedef struct GridKeyboardLayout_ {
    int LEFT;
    int RIGHT;
    int ROTATE;
    int FAST_DROP;
    int INSTANT_DROP;
    int SWITCH_HELD;
} GridKeyboardLayout;

typedef struct Grid_ {
    // Whether the container should redraw or not
    bool redraw;
    // The grid values
    int grid[GRID_HEIGHT][GRID_WIDTH];
    // The keymaps for the grid
    GridKeyboardLayout gkl;

    // The x and y of the board
    TweenValue* x;
    TweenValue* y;
    // The slide for the tab
    TweenValue* tabSlide;

    // The score
    int score;
    // The dimensions for all the text elements
    int scoreTitleWidth;
    int scoreTitleHeight;
    int nextTitleWidth;
    int nextTitleHeight;
    int heldTitleWidth;
    int heldTitleHeight;
    // The text titles
    SDL_Texture* score_title;
    SDL_Texture* next_title;
    SDL_Texture* held_title;

    // Whether the game is over or not
    bool gameover;

    // How many updates have transpired
    int ticks;
    // How long the game should be waiting before locking the piece in
    int lockHold;

    // The updates between the piece dropping
    int speed;
    // Whether the game should drop the piece faster (not literally
    // 2x as fast)
    bool double_time;

    // How long the game should be waiting before moving the piece
    // at a normal rate
    int movePause;
    // If the game should be moving the piece left or right
    bool moveLeft;
    bool moveRight;
    // Whether the rotation key is held down
    bool rotated;
    // Whether the instant drop key is held down
    bool fastDropped;

    // The width and height of the tab and board back
    int tabWidth;
    int tabHeight;
    int boardBackWidth;
    int boardBackHeight;

    // The current tetromino
    Tetromino current;
    // The colour and type of the next and held piece
    int nextColour;
    TetrominoType next;
    int heldColour;
    TetrominoType held;
    // Whether the game can currently switch the held piece
    bool switchedHeld;
    // The opacity of the tetromino shadow
    TweenValue* shadow;

    // The particles and the colour memory for them
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
