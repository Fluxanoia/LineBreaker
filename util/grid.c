#include  <stdlib.h>
#include <stdbool.h>
#include    "grid.h"

void resetTetromino(Tetromino* t);

Grid* initialiseGrid(int x, int y, Display* d) {
    Grid* g = malloc(sizeof(Grid));
    g->redraw = true;

    g->init_x = x;
    g->init_y = y;
    g->x = initialiseTween(x);
    g->y = initialiseTween(y);

    SDL_QueryTexture(d->resMan->board_back, NULL, NULL,
            &g->boardBackWidth, &g->boardBackHeight);

    return g;
}

void wakeGrid(Grid* g) {
    resetTetromino(&g->current);
    g->next = NO_PIECE;

    g->gameover = false;

    g->ticks = 0;
    g->speed = 30;
    g->double_time = false;
    g->moveLeft = false;
    g->moveRight = false;
    g->rotated = false;

    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            g->grid[i][j] = 0;
        }
    }
}

void sleepGrid(Grid* g) {
    setTweenValue(g->x, g->init_x);
    setTweenValue(g->y, g->init_y);
    g->y->id = 0;
}

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

void resetTetromino(Tetromino* t) {
    t->tet_type = NO_PIECE;
    t->bound_x = 0;
    t->bound_y = 0;
    t->colour = 0;
    setTetrominoBoxes(t);
}

int getBoundSize(TetrominoType tt) {
    switch (tt) {
        case I: 
            return 4;
        case O: 
            return 2;
        case T:
        case J:
        case L:
        case S:
        case Z:
            return 3;
        case NO_PIECE:
            return 0;
    }
}

bool rotateTetromino(Grid* g, Tetromino* t) {
    float p_x = ((float) getBoundSize(t->tet_type) - 1.0) / 2.0;
    float p_y = p_x;
    int r_xs[TETROMINO_SIZE], r_ys[TETROMINO_SIZE];
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        r_xs[i] = p_x - (((float) t->box_ys[i]) - p_y);
        r_ys[i] = p_y + (((float) t->box_xs[i]) - p_x);
    }
    
    int n_x, n_y;
    int c_x = 0, c_y = 0;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        n_x = t->bound_x + r_xs[i];
        n_y = t->bound_y + r_ys[i];
        while (n_y + c_y < 0) c_y++;
        while (n_y + c_y >= GRID_HEIGHT) c_y--;
        while (n_x + c_x < 0) c_x++;
        while (n_x + c_x >= GRID_WIDTH) c_x--;
    }
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        n_x = t->bound_x + r_xs[i] + c_x;
        n_y = t->bound_y + r_ys[i] + c_y;
        if (n_y < 0) return false;
        if (n_y >= GRID_HEIGHT) return false;
        if (n_x < 0) return false;
        if (n_x >= GRID_WIDTH) return false;

        if (g->grid[n_y][n_x] != 0) return false;
    }

    for (int i = 0; i < TETROMINO_SIZE; i++) {
        t->box_xs[i] = r_xs[i] + c_x;
        t->box_ys[i] = r_ys[i] + c_y;
    }
    return true;
} 

void fillCurrentTetromino(Grid* grid) {
    if (grid->current.tet_type == NO_PIECE) {
        grid->current.tet_type = grid->next;
        grid->current.bound_x = (GRID_WIDTH - getBoundSize(grid->next)) >> 1;
        grid->current.bound_y = 2;
        grid->current.colour = (rand() % GRID_MAX) + 1;
        setTetrominoBoxes(&grid->current);
        grid->next = NO_PIECE;
    }
}

void fillNextTetromino(Grid* grid) {
    if (grid->next == NO_PIECE) grid->next = (rand() % TETROMINO_COUNT) + 1;
}

bool moveTetromino(Grid* g, Tetromino* t, int c_x, int c_y) {
    int bi, bj;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        if (t->bound_y + t->box_ys[i] + c_y < 0) return false;
        if (t->bound_y + t->box_ys[i] + c_y >= GRID_HEIGHT) return false;
        if (t->bound_x + t->box_xs[i] + c_x < 0) return false;
        if (t->bound_x + t->box_xs[i] + c_x >= GRID_WIDTH) return false;

        bi = t->bound_y + t->box_ys[i] + c_y;
        bj = t->bound_x + t->box_xs[i] + c_x;  
        if (g->grid[bi][bj] != 0) return false;
    }
    t->bound_y += c_y;
    t->bound_x += c_x;
    return true;
}

void clearLine(Grid* g, int c_i) {
    for (int i = c_i; i > 0; i--) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            g->grid[i][j] = g->grid[i - 1][j];    
        }      
    }
    g->redraw = true;
}

void updateGrid(Grid* grid) {
    grid->ticks = grid->ticks + 1;

    updateTweenValue(grid->x);
    grid->redraw |= TweenValue_dropRedraw(grid->x);
    updateTweenValue(grid->y);
    grid->redraw |= TweenValue_dropRedraw(grid->y);

    if (grid->y->id == 1) {
        if (arrived(grid->y)) grid->gameover = true;
        return;
    }

    fillNextTetromino(grid);
    fillCurrentTetromino(grid);
    fillNextTetromino(grid);

    if (grid->moveLeft ^ grid->moveRight &&
            grid->ticks % LATERAL_DELAY == 0) {
        int c_x = 1;
        if (grid->moveLeft) c_x *= -1;
        grid->redraw |= moveTetromino(grid, &grid->current, c_x, 0);
    }

    if (grid->ticks % grid->speed == 0 || grid->double_time) {
        if (moveTetromino(grid, &grid->current, 0, 1)) {
            grid->redraw = true;
        } else {
            int bi, bj;
            for (int i = 0; i < 4; i++) {
                bi = grid->current.bound_y + grid->current.box_ys[i];
                bj = grid->current.bound_x + grid->current.box_xs[i];                
                grid->grid[bi][bj] = grid->current.colour;
            }
            grid->double_time = false;
            for (int i = GRID_HEIGHT - 1; i > -1; i--) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    if (grid->grid[i][j] == 0) break;
                    if (j == GRID_WIDTH - 1) {
                        clearLine(grid, i);
                        i++;
                    }
                }
            }
            for (int j = 0; j < GRID_WIDTH; j++) {
                if (grid->grid[HIDDEN_ROWS - 1][j] != 0) {
                    grid->y->id = 1;
                    moveTweenValue(grid->y, EASE_IN, -SCREEN_HEIGHT, 60, 20);
                    break;
                }
            }
            resetTetromino(&grid->current);
        }
    }
}

void drawTetromino(Grid* g, Tetromino* t, Display* d) {
    if (t->tet_type == NO_PIECE) return;
    SDL_Rect srcrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    SDL_Rect dstrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    int draw_x, draw_y;
    for (int i = 0; i < 4; i++) {
        draw_x = t->box_xs[i] + t->bound_x;
        draw_y = t->box_ys[i] + t->bound_y;
        if (draw_y < HIDDEN_ROWS) continue;
        srcrect.x = t->colour * GRID_BOX_SIZE;
        dstrect.x = (SCREEN_WIDTH - GRID_WIDTH * GRID_BOX_SIZE) / 2
                + draw_x * GRID_BOX_SIZE +  + getTweenValue(g->x);
        dstrect.y = (SCREEN_HEIGHT - (GRID_HEIGHT - HIDDEN_ROWS) * GRID_BOX_SIZE)
                / 2 + (draw_y - HIDDEN_ROWS) * GRID_BOX_SIZE +  + getTweenValue(g->y);
        SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
    }
}

void drawGrid(Grid* grid, Display* d) {
    int g_x, g_y;
    SDL_Rect srcrect, dstrect;

    g_x = (SCREEN_WIDTH - grid->boardBackWidth) / 2 + getTweenValue(grid->x);
    g_y = (SCREEN_HEIGHT - grid->boardBackHeight) / 2 + getTweenValue(grid->y);
    
    dstrect = (SDL_Rect) { g_x, g_y - 50, grid->boardBackWidth, grid->boardBackHeight };
    SDL_RenderCopy(d->renderer, d->resMan->board_back, NULL, &dstrect);

    g_x = (SCREEN_WIDTH - GRID_WIDTH * GRID_BOX_SIZE) / 2 + getTweenValue(grid->x);
    g_y = (SCREEN_HEIGHT - (GRID_HEIGHT - HIDDEN_ROWS) * GRID_BOX_SIZE) / 2 +
            getTweenValue(grid->y);
    srcrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    dstrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    for (int i = HIDDEN_ROWS; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            srcrect.x = grid->grid[i][j] * GRID_BOX_SIZE;
            dstrect.x = g_x + j * GRID_BOX_SIZE;
            dstrect.y = g_y + (i - HIDDEN_ROWS) * GRID_BOX_SIZE;
            SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
        }
    }
    drawTetromino(grid, &grid->current, d);
}

void Grid_keyEvent(Grid* grid, SDL_KeyboardEvent e) {
    switch (e.keysym.sym) {
        case SDLK_LEFT:
            if (e.type == SDL_KEYUP) grid->moveLeft = false; 
            if (e.type == SDL_KEYDOWN) grid->moveLeft = true;
        break;
        case SDLK_RIGHT:
            if (e.type == SDL_KEYUP) grid->moveRight = false; 
            if (e.type == SDL_KEYDOWN) grid->moveRight = true;
        break;
        case SDLK_DOWN:
            if (e.type == SDL_KEYUP) grid->double_time = false;
            if (e.type == SDL_KEYDOWN) grid->double_time = true;
        break;
        case SDLK_UP:
            if (e.type == SDL_KEYUP) grid->rotated = false;
            if (e.type == SDL_KEYDOWN && !grid->rotated) {
                grid->rotated = true;
                grid->redraw |= rotateTetromino(grid, &grid->current);
            }
        break;
    }
}

bool Grid_dropRedraw(Grid* g) {
    if (g->redraw) {
        g->redraw = false;
        return true;
    }
    return false;
}

void freeGrid(Grid* grid) {
    freeTweenValue(grid->x);
    freeTweenValue(grid->y);
    free(grid);
}
