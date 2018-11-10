#include  <stdlib.h>
#include <stdbool.h>
#include    "grid.h"

void setTetrominoBoxes(Tetromino* t);

Grid* initialiseGrid() {
    Grid* g = malloc(sizeof(Grid));
    g->redraw = true;
    g->current = (Tetromino) { NO_PIECE, 0, 0, 0, 0 };
    setTetrominoBoxes(&g->current);
    g->next = NO_PIECE;
    g->ticks = 0;
    g->speed = 30;
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            g->grid[i][j] = 0;
        }
    }
    return g;
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

void rotateTetromino(Tetromino* t) {
    float r_x = 0, r_y = 0, p_x = 0, p_y = 0;
    p_x = ((float) getBoundSize(t->tet_type)) / 2.0;
    p_y = p_x;
    for (int i = 0; i < 4; i++) {
        r_x = -(((float) t->box_ys[i]) - p_y);
        r_y = (((float) t->box_xs[i]) - p_x);
        t->box_xs[i] = p_x + r_x;
        t->box_ys[i] = p_y + r_y;
    }
} 

void setTetrominoBoxes(Tetromino* t) {
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
            for (int i = 0; i < 4; i++) {
                t->box_xs[i] = 0;
                t->box_ys[i] = 0;
            }
        break;
    }
}

void fillCurrentTetromino(Grid* grid) {
    if (grid->current.tet_type == NO_PIECE) {
        grid->current.tet_type = grid->next;
        grid->current.bound_x = (GRID_WIDTH - getBoundSize(grid->next)) >> 1;
        grid->current.bound_y = 3;
        grid->current.colour = (rand() % GRID_MAX) + 1;
        grid->current.rotations = 0;
        setTetrominoBoxes(&grid->current);
        grid->next = NO_PIECE;
    }
}

void fillNextTetromino(Grid* grid) {
    if (grid->next == NO_PIECE) grid->next = (rand() % TETROMINO_COUNT) + 1;
}

bool pushTetromino(Tetromino* t) {
    int bys[4];
    for (int i = 0; i < 4; i++) {
        bys[i] = t->box_ys[i] + 1;
    }
    for (int i = 0; i < 4; i++) {
        t->box_ys[i] = bys[i];
    }
    return true;
}

void updateGrid(Grid* grid) {
    grid->ticks = grid->ticks + 1;
    fillNextTetromino(grid);
    fillCurrentTetromino(grid);
    fillNextTetromino(grid);
    if (grid->ticks % grid->speed == 0) grid->redraw |= pushTetromino(&grid->current);
}

void drawTetromino(Tetromino* t, Display* d) {
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
                + draw_x * GRID_BOX_SIZE;
        dstrect.y = (SCREEN_HEIGHT - (GRID_HEIGHT - HIDDEN_ROWS) * GRID_BOX_SIZE)
                / 2 + (draw_y - HIDDEN_ROWS) * GRID_BOX_SIZE;
        SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
    }
}

void drawGrid(Grid* grid, Display* d) {
    SDL_Rect srcrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    SDL_Rect dstrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    for (int i = HIDDEN_ROWS; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            srcrect.x = grid->grid[i][j] * GRID_BOX_SIZE;
            dstrect.x = (SCREEN_WIDTH - GRID_WIDTH * GRID_BOX_SIZE) / 2
                    + j * GRID_BOX_SIZE;
            dstrect.y = (SCREEN_HEIGHT - (GRID_HEIGHT - HIDDEN_ROWS) * GRID_BOX_SIZE)
                    / 2 + (i - HIDDEN_ROWS) * GRID_BOX_SIZE;
            SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
        }
    }
    drawTetromino(&grid->current, d);
}

bool Grid_dropRedraw(Grid* g) {
    if (g->redraw) {
        g->redraw = false;
        return true;
    }
    return false;
}

void freeGrid(Grid* grid) {
    free(grid);
}
