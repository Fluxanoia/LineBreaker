#include  <stdlib.h>
#include <stdbool.h>
#include    "grid.h"

// Initialise the Grid and return its pointer
Grid* initialiseGrid(int x, int y, GridKeyboardLayout gkl, Display* d) {
    // Allocate the memory and set default values
    Grid* g = malloc(sizeof(Grid));
    g->redraw = true;
    
    // Add the keymaps
    g->gkl = gkl;

    // Create the necessary text textures
    g->score_title = createSmallText("Score:", &g->scoreTitleWidth,
            &g->scoreTitleHeight, *d->resMan->text_colour, d);
    g->next_title = createSmallText("Next piece:", &g->nextTitleWidth,
            &g->nextTitleHeight, *d->resMan->text_colour, d);    
    g->held_title = createSmallText("Held piece:", &g->heldTitleWidth,
            &g->heldTitleHeight, *d->resMan->text_colour, d); 
    // Grab the width and height of textures
    SDL_QueryTexture(d->resMan->board_back, NULL, NULL,
            &g->boardBackWidth, &g->boardBackHeight);
    SDL_QueryTexture(d->resMan->board_tab, NULL, NULL,
            &g->tabWidth, &g->tabHeight);

    // Initialise the tween values
    g->shadow = initialiseTween(0);
    g->tabSlide = initialiseTween(0);
    if (x == GRID_CENTER) {
        g->x = initialiseTween((SCREEN_WIDTH - g->boardBackWidth) / 2);
    } else {
        g->x = initialiseTween(x);
    }
    if (y == GRID_CENTER) {
        g->y = initialiseTween((SCREEN_HEIGHT - g->boardBackHeight) / 2);
    } else {
        g->y = initialiseTween(y);
    } 

    // Initialise all the particles
    for (int i = 0; i < GRID_HEIGHT; i++) for (int j = 0; j < GRID_WIDTH; j++) {
        // No. particles, their size, max x/y speed, gravity and, variance
        g->particles[i][j] = initialiseParticles(8, 8, 16, 1, 16);
    }

    return g;
}

// Returns the x coordinate of the grid column
int getGridBoxX(Grid* g, int j) {
    return (g->boardBackWidth - GRID_WIDTH * GRID_BOX_SIZE) / 2
            + getTweenValue(g->x) + j * GRID_BOX_SIZE;
}

// Returns the y coordinate of the grid row
int getGridBoxY(Grid* g, int i) {
    return (g->boardBackHeight - BACKING_DIP - (GRID_HEIGHT - HIDDEN_ROWS)
            * GRID_BOX_SIZE) / 2 + getTweenValue(g->y) + (i - HIDDEN_ROWS)
            * GRID_BOX_SIZE;
}

// Returns the string length required to contain an int
int reqStrLenFor(int i) {
    if (i == 0) return 2;
    int count = floor(log10(i)) + 1;
    // For negative sign
    if (i < 0) count++;
    // For null terminator
    count++;
    return count;
}

// ==================================================================
// === TETROMINO FUNCTIONS ==========================================
// ==================================================================

// Fills a tetromino with the default grid references for its
// Tetromino type
void setTetrominoBoxes(Tetromino* t) {
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        t->box_xs[i] = 0;
        t->box_ys[i] = 0;
    }
    switch (t->tet_type) {
        case I:
            for (int i = 0; i < 4; i++) {
                t->box_xs[i] = i;
                t->box_ys[i] = 1;
            }
        break;
        case O:
            t->box_xs[0] = 0;
            t->box_xs[1] = 1;
            t->box_xs[2] = 0;
            t->box_xs[3] = 1;
            
            t->box_ys[0] = 0;
            t->box_ys[1] = 1;
            t->box_ys[2] = 1;
            t->box_ys[3] = 0;
        break;
        case T:
            for (int i = 0; i < 3; i++) {
                t->box_xs[i] = i;
                t->box_ys[i] = 1;
            }
            t->box_xs[3] = 1;
            t->box_ys[3] = 0;
        break;
        case J:
            for (int i = 0; i < 3; i++) {
                t->box_xs[i] = i;
                t->box_ys[i] = 1;
            }
            t->box_xs[3] = 0;
            t->box_ys[3] = 0;
        break;
        case L:
            for (int i = 0; i < 3; i++) {
                t->box_xs[i] = i;
                t->box_ys[i] = 1;
            }
            t->box_xs[3] = 2;
            t->box_ys[3] = 0;
        break;
        case S:
            for (int i = 0; i < 2; i++) {
                t->box_xs[i] = 1;
                t->box_ys[i] = i;
            }
            t->box_xs[2] = 0;
            t->box_ys[2] = 1;
            t->box_xs[3] = 2;
            t->box_ys[3] = 0;
        break;
        case Z:
            for (int i = 0; i < 2; i++) {
                t->box_xs[i] = 1;
                t->box_ys[i] = i;
            }
            t->box_xs[2] = 2;
            t->box_ys[2] = 1;
            t->box_xs[3] = 0;
            t->box_ys[3] = 0;
        break;
        case NO_PIECE:
        break;
    }
}

// Resets the Tetromino to default values
void resetTetromino(Tetromino* t) {
    t->tet_type = NO_PIECE;
    t->bound_x = 0;
    t->bound_y = 0;
    t->colour = 0;
    setTetrominoBoxes(t);
}

// Gives the size in grid boxes of the smallest bounding square that
// would encapsulate a tetromino of a particular type
int getBoundSize(TetrominoType tt) {
    Tetromino t;
    resetTetromino(&t);
    t.tet_type = tt;
    setTetrominoBoxes(&t);
    int min = TETROMINO_SIZE, max = -1;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        if (t.box_xs[i] > max) max = t.box_xs[i];
        if (t.box_ys[i] > max) max = t.box_ys[i];
        if (t.box_xs[i] < min) min = t.box_xs[i];
        if (t.box_ys[i] < min) min = t.box_ys[i];
    }
    return max - min + 1;
}

// Rotates the tetromino
bool rotateTetromino(Grid* g, Tetromino* t) {
    // The x and y values to pivot around
    float p_x = ((float) getBoundSize(t->tet_type) - 1.0) / 2.0;
    float p_y = p_x;
    // The rotated boxes
    int r_xs[TETROMINO_SIZE], r_ys[TETROMINO_SIZE];
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        r_xs[i] = p_x - (((float) t->box_ys[i]) - p_y);
        r_ys[i] = p_y + (((float) t->box_xs[i]) - p_x);
    }
    
    // The new x and y
    int n_x, n_y;
    // How much the piece has to move to be valid
    int c_x = 0, c_y = 0;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        n_x = t->bound_x + r_xs[i];
        n_y = t->bound_y + r_ys[i];
        while (n_y + c_y < 0) c_y++;
        while (n_y + c_y >= GRID_HEIGHT) c_y--;
        while (n_x + c_x < 0) c_x++;
        while (n_x + c_x >= GRID_WIDTH) c_x--;
    }
    // Check one final time if this organisation is valid
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        n_x = t->bound_x + r_xs[i] + c_x;
        n_y = t->bound_y + r_ys[i] + c_y;
        if (n_y < 0) return false;
        if (n_y >= GRID_HEIGHT) return false;
        if (n_x < 0) return false;
        if (n_x >= GRID_WIDTH) return false;

        if (g->grid[n_y][n_x] != 0) return false;
    }

    // Update the tetromino
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        t->box_xs[i] = r_xs[i] + c_x;
        t->box_ys[i] = r_ys[i] + c_y;
    }
    return true;
} 

// Fills the next tetromino with a new piece if necessary
void fillNextTetromino(Grid* g) {
    if (g->next == NO_PIECE) {
        g->next = (rand() % TETROMINO_COUNT) + 1;
        g->nextColour = (rand() % GRID_MAX) + 1;
    }
}

// Fills a tetromino with new data regardless of its state
void forceFillTetromino(Tetromino* t, TetrominoType tt, int colour) {
    t->tet_type = tt;
    t->bound_x = (GRID_WIDTH - getBoundSize(tt)) >> 1;
    t->bound_y = START_ROW;
    t->colour = colour;
    setTetrominoBoxes(t);
}

// Updates the next and current tetrominoes if necessary
void updateGridTetrominoes(Grid* g) {
    if (g->current.tet_type == NO_PIECE) {
        // Push the next piece in
        forceFillTetromino(&g->current, g->next, g->nextColour);
        // Update the next piece
        g->next = NO_PIECE;
        fillNextTetromino(g);
        // Reset the piece shadow for smooth transitions
        setTweenValue(g->shadow, 0);
        moveTweenValue(g->shadow, EASE_IN, SHADOW_OPACITY, 20, 0);
    }    
}

// Checks whether a move would be possible
bool queryMoveTetromino(Grid* g, Tetromino* t, int c_x, int c_y) {
    int bi, bj;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        // The new grid references
        bi = t->bound_y + t->box_ys[i] + c_y;
        bj = t->bound_x + t->box_xs[i] + c_x;
        // Check if it's out of bounds
        if (bi < 0) return false;
        if (bi >= GRID_HEIGHT) return false;
        if (bj < 0) return false;
        if (bj >= GRID_WIDTH) return false;  
        // Check if it intersects
        if (g->grid[bi][bj] != 0) return false;
    }
    return true;
}

// Move the tetromino on the grid returns True on success, false
// otherwise
bool moveTetromino(Grid* g, Tetromino* t, int c_x, int c_y) {
    if (queryMoveTetromino(g, t, c_x, c_y)) {
        // Update the piece
        t->bound_y += c_y;
        t->bound_x += c_x;
        return true;
    }
    return false;
}

// Copies the information from src to dst
void copyTetromino(Tetromino* dst, Tetromino* src) {
    dst->tet_type = src->tet_type;
    dst->bound_x = src->bound_x;
    dst->bound_y = src->bound_y;
    dst->colour = src->colour;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        dst->box_xs[i] = src->box_xs[i];
        dst->box_ys[i] = src->box_ys[i];
    }
}

// Clears the line and drops the above pieces
void clearLine(Grid* g, int c_i) {
    for (int j = 0; j < GRID_WIDTH; j++) {
        // Update the colour grid and reset the particles
        g->colours[c_i][j] = g->grid[c_i][j];
        resetParticles(g->particles[c_i][j],
                getGridBoxX(g, j) + GRID_BOX_SIZE / 2,
                getGridBoxY(g, c_i) + GRID_BOX_SIZE / 2);
    }
    // Moves all the rows down
    for (int i = c_i; i > 0; i--) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            g->grid[i][j] = g->grid[i - 1][j];
        }      
    }
    // Clears the top row
    for (int j = 0; j < GRID_WIDTH; j++) {
        g->grid[0][j] = 0;
    }
    g->redraw = true;
}

// Locks in the current piece and does the related calculations
void lockInCurrent(Grid* g) {
    // Fills the grid with the piece
    int bi, bj;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        bi = g->current.bound_y + g->current.box_ys[i];
        bj = g->current.bound_x + g->current.box_xs[i];                
        g->grid[bi][bj] = g->current.colour;
    }
    // Allow the held piece to be switched again
    g->switchedHeld = false;
    // Turn off double time
    g->double_time = false;

    // Clear the lines
    int linesCleared = 0;
    for (int i = GRID_HEIGHT - 1; i > -1; i--) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            // Check if the line isn't full
            if (g->grid[i][j] == 0) break;
            // If all the spaces are full, clear
            if (j == GRID_WIDTH - 1) {
                clearLine(g, i);
                linesCleared++;
                i++;
            }
        }
    }
    // Adds to the score based on lines cleared
    g->score += (pow(BASE_SCORE, linesCleared)) *
            (GRID_HEIGHT - g->current.bound_y) * 10;
    // Checks for gameovers
    for (int j = 0; j < GRID_WIDTH; j++) {
        // If there's a piece in the hidden area
        if (g->grid[HIDDEN_ROWS - 1][j] != 0) {
            g->gameover = true;
            // Reset particles on all grid pieces and
            // clear the board
            for (int i = 0; i < GRID_HEIGHT; i++) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    if (g->grid[i][j] == 0) continue;
                    g->colours[i][j] = g->grid[i][j];
                    resetParticles(g->particles[i][j],
                        getGridBoxX(g, j) + GRID_BOX_SIZE / 2,
                        getGridBoxY(g, i) + GRID_BOX_SIZE / 2);
                    g->grid[i][j] = 0;
                }
            }
            break;
        }
    }
    // Reset the tetromino for reallocation
    resetTetromino(&g->current);
}

// Check and move the current piece laterally if needed
void lateralMove(Grid* g) {
    // If moving in one direction
    if (g->moveLeft ^ g->moveRight) {
        // Get the correct direction
        int c_x = 1;
        if (g->moveLeft) c_x *= -1;
        // Move the piece
        if (moveTetromino(g, &g->current, c_x, 0)) {
            // If moved, redraw and reset the lock hold
            g->redraw = true;
            g->lockHold = 0;
        }
    }
}

// ==================================================================
// === GRID FUNCTIONS ===============================================
// ==================================================================

// Wakes the grid
void wakeGrid(Grid* g) {
    // Reset the tetrominoes
    g->next = NO_PIECE;
    g->held = NO_PIECE;
    fillNextTetromino(g);
    resetTetromino(&g->current);    
    // Allow the play to hold a piece
    g->switchedHeld = false;
    // Make the game playable again
    g->gameover = false;
    // Reset the ticks
    g->ticks = 0;
    // Reset the lock hold
    g->lockHold = 0;
    // Reset the score and game speed
    g->score = 0;
    g->speed = 30;
    g->double_time = false;
    // Reset the movement variables
    g->movePause = 0;
    g->moveLeft = false;
    g->moveRight = false;
    g->rotated = false;
    g->fastDropped = false;
    // Start moving the tab
    moveTweenValue(g->tabSlide, EASE_OUT, TAB_MAX, 20, 0);
    // Reset the grid and its colours
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            g->grid[i][j] = 0;
            g->colours[i][j] = 0;
        }
    }
}

// Sleep the grid for storage
void sleepGrid(Grid* g) {
    // Reset the tetromino shadow and the tab
    setTweenValue(g->shadow, 0);
    setTweenValue(g->tabSlide, 0);
    // Reset the ids of the tweens
    g->x->id = 0;
    g->y->id = 0;
}

// Update the grid
void updateGrid(Grid* g) {
    // Update the ticks
    g->ticks = g->ticks + 1;
    // Update all the tween values
    updateTweenValue(g->x);
    g->redraw |= TweenValue_dropRedraw(g->x);
    updateTweenValue(g->y);
    g->redraw |= TweenValue_dropRedraw(g->y);
    updateTweenValue(g->shadow);
    g->redraw |= TweenValue_dropRedraw(g->shadow);
    updateTweenValue(g->tabSlide);
    g->redraw |= TweenValue_dropRedraw(g->tabSlide);
    // Update the particles
    for (int i = 0; i < GRID_HEIGHT; i++) for (int j = 0; j < GRID_WIDTH; j++) {
        updateParticles(g->particles[i][j]);
        g->redraw |= Particles_dropRedraw(g->particles[i][j]);
    }
    // Stop here if the game is over
    if (g->gameover) return;
    // Update the tetrominoes
    updateGridTetrominoes(g);
    // Move laterally if possible
    if (g->movePause > 0) {
        g->movePause--;
    } else if (g->ticks % LATERAL_DELAY == 0) {
        lateralMove(g);
    }
    // Process whether the piece should lock in or not
    bool lockin = false;
    if (g->lockHold > 0) {
        g->lockHold--;
        if (g->lockHold == 0) {
            lockin = true;
        } else {
            return;
        }
    }
    // If it can be locked in, lock it in
    if (lockin && !queryMoveTetromino(g, &g->current, 0, 1)) {
        lockInCurrent(g);
        setTweenValue(g->y, 10);
        moveTweenValue(g->y, EASE_IN, 0, 20, 0);
        return;
    }
    // Move the piece down if possible
    if (g->ticks % g->speed == 0 || g->double_time) {
        if (moveTetromino(g, &g->current, 0, 1)) {
            g->redraw = true;
            if (g->double_time) g->score += 5;
        } else if (!lockin && g->lockHold == 0) {
            g->lockHold = LOCK_PAUSE;
        }
    }
}

// Draw the tetromino
void drawTetromino(Grid* g, Tetromino* t, Display* d) {
    // If the tetromino isn't a piece, return
    if (t->tet_type == NO_PIECE) return;
    // Set up variables to use
    SDL_Rect srcrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    SDL_Rect dstrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    int draw_x, draw_y;
    // Prepare the shadow
    Tetromino bottom;
    copyTetromino(&bottom, t);
    while (moveTetromino(g, &bottom, 0, 1)) {}
    // Draw the tetromino
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        draw_x = t->box_xs[i] + t->bound_x;
        draw_y = t->box_ys[i] + t->bound_y;
        if (draw_y < HIDDEN_ROWS) continue;
        srcrect.x = t->colour * GRID_BOX_SIZE;
        dstrect.x = getGridBoxX(g, draw_x);
        dstrect.y = getGridBoxY(g, draw_y);
        SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
    }
    // Draw the shadow
    SDL_SetTextureAlphaMod(d->resMan->boxes, (Uint8) getTweenValue(g->shadow));
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        draw_x = bottom.box_xs[i] + bottom.bound_x;
        draw_y = bottom.box_ys[i] + bottom.bound_y;
        if (draw_y < HIDDEN_ROWS) continue;
        srcrect.x = bottom.colour * GRID_BOX_SIZE;
        dstrect.x = getGridBoxX(g, draw_x);
        dstrect.y = getGridBoxY(g, draw_y);
        SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
    }
    SDL_SetTextureAlphaMod(d->resMan->boxes, 255);    
}

// Draw a small tetromino
void drawSmallTetromino(Grid* g, Tetromino* t, int x, int y, Display* d) {
    // If the tetromino isn't a piece, return    
    if (t->tet_type == NO_PIECE) return;
    // Set up variables to use
    int draw_x, draw_y;
    SDL_Rect srcrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    SDL_Rect dstrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE / 2, GRID_BOX_SIZE / 2 };
    // Draw the tetromino
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        draw_x = t->box_xs[i];
        draw_y = t->box_ys[i];
        srcrect.x = t->colour * GRID_BOX_SIZE;
        dstrect.x = x + draw_x * (GRID_BOX_SIZE / 2);
        dstrect.y = y + draw_y * (GRID_BOX_SIZE / 2);
        SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
    }
}

// Draw the grid
void drawGrid(Grid* g, Display* d) {
    // Create some variables to use
    Tetromino t;
    SDL_Rect srcrect, dstrect;
    // Draw the tab for the score, held and next pieces
    dstrect = (SDL_Rect) { getTweenValue(g->x) + getTweenValue(g->tabSlide),
            getTweenValue(g->y) + 50, g->tabWidth, g->tabHeight };
    SDL_RenderCopy(d->renderer, d->resMan->board_tab, NULL, &dstrect);
    // Draw the score title
    dstrect.x += 150;
    dstrect.y += 5;
    dstrect.w = g->scoreTitleWidth;
    dstrect.h = g->scoreTitleHeight;
    SDL_RenderCopy(d->renderer, g->score_title, NULL, &dstrect);
    // Draw the score
    char score_str[reqStrLenFor(g->score)];
    sprintf(score_str, "%d", g->score);
    dstrect.y += 5 + dstrect.h;
    SDL_Texture* score = createSmallText(score_str, &dstrect.w, &dstrect.h,
            *d->resMan->text_colour, d);
    if (dstrect.w > TAB_MAX - 10) dstrect.w = TAB_MAX - 10;
    SDL_RenderCopy(d->renderer, score, NULL, &dstrect);
    SDL_DestroyTexture(score);
    // Draw the next title
    dstrect.y += 5 + dstrect.h;
    dstrect.w = g->nextTitleWidth;
    dstrect.h = g->nextTitleHeight;
    SDL_RenderCopy(d->renderer, g->next_title, NULL, &dstrect);
    // Draw the next tetromino
    t.tet_type = g->next;
    t.colour = g->nextColour;
    setTetrominoBoxes(&t);
    drawSmallTetromino(g, &t, dstrect.x + 20, dstrect.y + dstrect.h + 10, d);
    // Draw the held title
    dstrect.y += 5 + dstrect.h + 60;
    dstrect.w = g->heldTitleWidth;
    dstrect.h = g->heldTitleHeight;
    SDL_RenderCopy(d->renderer, g->held_title, NULL, &dstrect); 
    // Draw the held tetromino   
    t.tet_type = g->held;
    t.colour = g->heldColour;
    setTetrominoBoxes(&t);
    drawSmallTetromino(g, &t, dstrect.x + 20, dstrect.y + dstrect.h + 10, d);
    // Draw the board backing
    dstrect = (SDL_Rect) { getTweenValue(g->x), getTweenValue(g->y)
        - BACKING_DIP, g->boardBackWidth, g->boardBackHeight };
    SDL_RenderCopy(d->renderer, d->resMan->board_back, NULL, &dstrect);
    // Draw the grid
    srcrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    dstrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    for (int i = HIDDEN_ROWS; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            srcrect.x = g->grid[i][j] * GRID_BOX_SIZE;
            dstrect.x = getGridBoxX(g, j);
            dstrect.y = getGridBoxY(g, i);
            SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
        }
    }
    // Draw the tetromino
    drawTetromino(g, &g->current, d);
    // Draw the particles
    for (int i = 0; i < GRID_HEIGHT; i++) for (int j = 0; j < GRID_WIDTH; j++) {
        srcrect.x = g->colours[i][j] * GRID_BOX_SIZE;
        drawParticles(g->particles[i][j], d, d->resMan->boxes, srcrect);
    }
}

// React to key events
void Grid_keyEvent(Grid* g, SDL_KeyboardEvent e) {
    // If the game is over, ignore
    if (g->gameover) return;
    // Moving left
    if (e.keysym.sym == g->gkl.LEFT) {
        if (e.type == SDL_KEYUP) {
            g->moveLeft = false; 
            g->movePause = 0;
        }
        if (e.type == SDL_KEYDOWN) {
            g->moveLeft = true;
            lateralMove(g);
            g->movePause = LATERAL_PAUSE;
        }
    }
    // Moving right
    if (e.keysym.sym == g->gkl.RIGHT) {
        if (e.type == SDL_KEYUP) {
            g->moveRight = false;
            g->movePause = 0;
        }
        if (e.type == SDL_KEYDOWN) { 
            g->moveRight = true;
            lateralMove(g);
            g->movePause = LATERAL_PAUSE;
        }
    }
    // Speed up (double time)
    if (e.keysym.sym == g->gkl.FAST_DROP) {
        if (e.type == SDL_KEYUP) g->double_time = false;
        if (e.type == SDL_KEYDOWN) g->double_time = true;
    }
    // Rotate
    if (e.keysym.sym == g->gkl.ROTATE) {
        if (e.type == SDL_KEYUP) g->rotated = false;
        if (e.type == SDL_KEYDOWN && !g->rotated) {
            g->rotated = true;
            g->redraw |= rotateTetromino(g, &g->current);
        }
    }
    // Instant drop
    if (e.keysym.sym == g->gkl.INSTANT_DROP) {
        if (e.type == SDL_KEYUP) g->fastDropped = false;
        if (e.type == SDL_KEYDOWN && !g->fastDropped) {
            int fallen = 0;
            while (moveTetromino(g, &g->current, 0, 1)) { fallen++; }
            lockInCurrent(g);
            setTweenValue(g->y, 50);
            moveTweenValue(g->y, EASE_IN, 0, 20, 0);
            g->fastDropped = true;
            g->score += fallen * 10;
            g->redraw = true;
        }
    }
    // Hold the current piece
    if (e.keysym.sym == g->gkl.SWITCH_HELD) {
        if (!g->switchedHeld) {
            g->switchedHeld = true;
            TetrominoType tempt = g->held;
            int tempc = g->heldColour;
            g->heldColour = g->current.colour;
            g->held = g->current.tet_type;
            forceFillTetromino(&g->current, tempt, tempc);
            g->redraw = true;
        }
    }
}

// Return the redraw value and set it to false
bool Grid_dropRedraw(Grid* g) {
    if (g->redraw) {
        g->redraw = false;
        return true;
    }
    return false;
}

// Frees the Grid's resources
void freeGrid(Grid* g) {
    freeTweenValue(g->x);
    freeTweenValue(g->y);
    freeTweenValue(g->shadow);
    freeTweenValue(g->tabSlide);
    SDL_DestroyTexture(g->score_title);
    SDL_DestroyTexture(g->next_title);
    SDL_DestroyTexture(g->held_title);
    for (int i = 0; i < GRID_HEIGHT; i++) for (int j = 0; j < GRID_WIDTH; j++) {
        freeParticles(g->particles[i][j]);
    }
    free(g);
}
