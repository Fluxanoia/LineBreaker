#include  <stdlib.h>
#include <stdbool.h>
#include    "grid.h"

Grid* initialiseGrid(int x, int y, Display* d) {
    Grid* g = malloc(sizeof(Grid));
    g->redraw = true;

    g->x = initialiseTween(x);
    g->y = initialiseTween(y);

    g->shadow = initialiseTween(0);
    g->tabSlide = initialiseTween(0);

    SDL_QueryTexture(d->resMan->board_back, NULL, NULL,
            &g->boardBackWidth, &g->boardBackHeight);
    SDL_QueryTexture(d->resMan->board_tab, NULL, NULL,
            &g->tabWidth, &g->tabHeight);

    g->score_title = createSmallText("Score:", &g->scoreTitleWidth,
            &g->scoreTitleHeight, *d->resMan->text_colour, d);
    g->next_title = createSmallText("Next piece:", &g->nextTitleWidth,
            &g->nextTitleHeight, *d->resMan->text_colour, d);    
    g->held_title = createSmallText("Held piece:", &g->heldTitleWidth,
            &g->heldTitleHeight, *d->resMan->text_colour, d);    

    for (int i = 0; i < GRID_HEIGHT; i++) for (int j = 0; j < GRID_WIDTH; j++) {
        g->particles[i][j] = initialiseParticles(8, 8, 16, 1, 16);
    }

    return g;
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

void wakeGrid(Grid* g) {
    resetTetromino(&g->current);
    g->next = NO_PIECE;
    g->held = NO_PIECE;
    g->switchedHeld = false;

    g->gameover = false;

    g->ticks = 0;
    g->lockHold = 0;

    g->score = 0;
    g->speed = 30;
    g->double_time = false;

    g->movePause = 0;
    g->moveLeft = false;
    g->moveRight = false;
    g->rotated = false;
    g->fastDropped = false;

    moveTweenValue(g->tabSlide, EASE_OUT, TAB_MAX, 20, 0);

    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            g->grid[i][j] = 0;
            g->colours[i][j] = 0;
        }
    }
}

void sleepGrid(Grid* g) {
    setTweenValue(g->shadow, 0);
    setTweenValue(g->tabSlide, 0);
    g->x->id = 0;
    g->y->id = 0;
}

int getGridBoxX(Grid* g, int j) {
    return (SCREEN_WIDTH - GRID_WIDTH * GRID_BOX_SIZE) / 2
            + getTweenValue(g->x) + j * GRID_BOX_SIZE;
}

int getGridBoxY(Grid* g, int i) {
    return (SCREEN_HEIGHT - (GRID_HEIGHT - HIDDEN_ROWS) * GRID_BOX_SIZE) / 2
            + getTweenValue(g->y) + (i - HIDDEN_ROWS) * GRID_BOX_SIZE;
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

void fillTetromino(Tetromino* t, TetrominoType tt) {
    
}

void fillNextTetromino(Grid* grid) {
    if (grid->next == NO_PIECE) {
        grid->next = (rand() % TETROMINO_COUNT) + 1;
        grid->nextColour = (rand() % GRID_MAX) + 1;
    }
}

void forceFillTetromino(Tetromino* t, TetrominoType tt, int colour) {
    t->tet_type = tt;
    t->bound_x = (GRID_WIDTH - getBoundSize(tt)) >> 1;
    t->bound_y = START_ROW;
    t->colour = colour;
    setTetrominoBoxes(t);
}

void fillGridTetromino(Grid* grid) {
    fillNextTetromino(grid);    
    if (grid->current.tet_type == NO_PIECE) {
        forceFillTetromino(&grid->current, grid->next, grid->nextColour);
        
        grid->next = NO_PIECE;
        fillNextTetromino(grid);
        
        setTweenValue(grid->shadow, 0);
        moveTweenValue(grid->shadow, EASE_IN, SHADOW_OPACITY, 20, 0);
    }    
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

bool queryMoveTetromino(Grid* g, Tetromino* t, int c_x, int c_y) {
    Tetromino tt;
    copyTetromino(&tt, t);
    return moveTetromino(g, &tt, c_x, c_y);
}

void clearLine(Grid* g, int c_i) {
    for (int j = 0; j < GRID_WIDTH; j++) {
        g->colours[c_i][j] = g->grid[c_i][j];
        resetParticles(g->particles[c_i][j],
                getGridBoxX(g, j) + GRID_BOX_SIZE / 2,
                getGridBoxY(g, c_i) + GRID_BOX_SIZE / 2);
    }
    for (int i = c_i; i > 0; i--) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            g->grid[i][j] = g->grid[i - 1][j];
        }      
    }
    g->redraw = true;
}

void lockInCurrent(Grid* grid) {
    int bi, bj;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        bi = grid->current.bound_y + grid->current.box_ys[i];
        bj = grid->current.bound_x + grid->current.box_xs[i];                
        grid->grid[bi][bj] = grid->current.colour;
    }
    grid->switchedHeld = false;
    grid->double_time = false;

    int linesCleared = 0;
    for (int i = GRID_HEIGHT - 1; i > -1; i--) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (grid->grid[i][j] == 0) break;
            if (j == GRID_WIDTH - 1) {
                clearLine(grid, i);
                linesCleared++;
                i++;
            }
        }
    }
    grid->score += (pow(BASE_SCORE, linesCleared)) *
            (GRID_HEIGHT - grid->current.bound_y) * 10;
    for (int j = 0; j < GRID_WIDTH; j++) {
        if (grid->grid[HIDDEN_ROWS - 1][j] != 0) {
            grid->gameover = true;
            for (int i = 0; i < GRID_HEIGHT; i++) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    if (grid->grid[i][j] == 0) continue;
                    grid->colours[i][j] = grid->grid[i][j];
                    resetParticles(grid->particles[i][j],
                        getGridBoxX(grid, j) + GRID_BOX_SIZE / 2,
                        getGridBoxY(grid, i) + GRID_BOX_SIZE / 2);
                    grid->grid[i][j] = 0;
                }
            }
            break;
        }
    }
    resetTetromino(&grid->current);
}

void lateralMove(Grid* grid) {
    if (grid->moveLeft ^ grid->moveRight) {
        int c_x = 1;
        if (grid->moveLeft) c_x *= -1;
        if (moveTetromino(grid, &grid->current, c_x, 0)) {
            grid->redraw = true;
            grid->lockHold = 0;
        }
    }
}

void updateGrid(Grid* grid) {
    grid->ticks = grid->ticks + 1;

    updateTweenValue(grid->x);
    grid->redraw |= TweenValue_dropRedraw(grid->x);
    updateTweenValue(grid->y);
    grid->redraw |= TweenValue_dropRedraw(grid->y);
    updateTweenValue(grid->shadow);
    grid->redraw |= TweenValue_dropRedraw(grid->shadow);
    updateTweenValue(grid->tabSlide);
    grid->redraw |= TweenValue_dropRedraw(grid->tabSlide);

    for (int i = 0; i < GRID_HEIGHT; i++) for (int j = 0; j < GRID_WIDTH; j++) {
        updateParticles(grid->particles[i][j]);
        grid->redraw |= Particles_dropRedraw(grid->particles[i][j]);
    }

    if (grid->gameover) return;

    fillGridTetromino(grid);

    if (grid->movePause > 0) {
        grid->movePause--;
    } else if (grid->ticks % LATERAL_DELAY == 0) {
        lateralMove(grid);
    }

    bool lockin = false;
    if (grid->lockHold > 0) {
        grid->lockHold--;
        if (grid->lockHold == 0) {
            lockin = true;
        } else {
            return;
        }
    }
    if (lockin && !queryMoveTetromino(grid, &grid->current, 0, 1)) {
        lockInCurrent(grid);
        setTweenValue(grid->y, 10);
        moveTweenValue(grid->y, EASE_IN, 0, 20, 0);
        return;
    }

    if (grid->ticks % grid->speed == 0 || grid->double_time) {
        if (moveTetromino(grid, &grid->current, 0, 1)) {
            grid->redraw = true;
            if (grid->double_time) grid->score += 5;
        } else if (!lockin && grid->lockHold == 0) {
            grid->lockHold = LOCK_PAUSE;
        }
    }
}

void drawTetromino(Grid* g, Tetromino* t, Display* d) {
    if (t->tet_type == NO_PIECE) return;
    SDL_Rect srcrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    SDL_Rect dstrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    int draw_x, draw_y;

    Tetromino bottom;
    copyTetromino(&bottom, t);
    while (moveTetromino(g, &bottom, 0, 1)) {}

    int g_x = (SCREEN_WIDTH - GRID_WIDTH * GRID_BOX_SIZE) / 2;
    int g_y = (SCREEN_HEIGHT - (GRID_HEIGHT - HIDDEN_ROWS) * GRID_BOX_SIZE) / 2;

    for (int i = 0; i < TETROMINO_SIZE; i++) {
        draw_x = t->box_xs[i] + t->bound_x;
        draw_y = t->box_ys[i] + t->bound_y;
        if (draw_y < HIDDEN_ROWS) continue;
        srcrect.x = t->colour * GRID_BOX_SIZE;
        dstrect.x = g_x + draw_x * GRID_BOX_SIZE + getTweenValue(g->x);
        dstrect.y = g_y + (draw_y - HIDDEN_ROWS) * GRID_BOX_SIZE + getTweenValue(g->y);
        SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
    }

    SDL_SetTextureAlphaMod(d->resMan->boxes, (Uint8) getTweenValue(g->shadow));
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        draw_x = bottom.box_xs[i] + bottom.bound_x;
        draw_y = bottom.box_ys[i] + bottom.bound_y;
        if (draw_y < HIDDEN_ROWS) continue;
        srcrect.x = bottom.colour * GRID_BOX_SIZE;
        dstrect.x = g_x + draw_x * GRID_BOX_SIZE + getTweenValue(g->x);
        dstrect.y = g_y + (draw_y - HIDDEN_ROWS) * GRID_BOX_SIZE + getTweenValue(g->y);
        SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
    }
    SDL_SetTextureAlphaMod(d->resMan->boxes, 255);    
}

void drawSmallTetromino(Grid* g, Tetromino* t, int x, int y, Display* d) {
    if (t->tet_type == NO_PIECE) return;
    int draw_x, draw_y;
    SDL_Rect srcrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    SDL_Rect dstrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE / 2, GRID_BOX_SIZE / 2 };
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        draw_x = t->box_xs[i];
        draw_y = t->box_ys[i];
        srcrect.x = t->colour * GRID_BOX_SIZE;
        dstrect.x = x + draw_x * (GRID_BOX_SIZE / 2);
        dstrect.y = y + draw_y * (GRID_BOX_SIZE / 2);
        SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
    }
}

int charsInInt(int i) {
  int count = 0;
  while (i != 0) {
        i /= 10;
        count++;
  }
  if (i < 0) i++;
  return count + 1;
}

void drawGrid(Grid* grid, Display* d) {
    int g_x;
    SDL_Rect srcrect, dstrect;

    g_x = (SCREEN_WIDTH + grid->boardBackWidth) / 2 - grid->tabWidth
            + getTweenValue(grid->x) + getTweenValue(grid->tabSlide);
    dstrect = (SDL_Rect) { g_x, getTweenValue(grid->y) + 50, grid->tabWidth,
            grid->tabHeight };
    SDL_RenderCopy(d->renderer, d->resMan->board_tab, NULL, &dstrect);
    dstrect.x += 150;
    dstrect.y += 5;
    dstrect.w = grid->scoreTitleWidth;
    dstrect.h = grid->scoreTitleHeight;
    SDL_RenderCopy(d->renderer, grid->score_title, NULL, &dstrect);

    char score_str[charsInInt(grid->score) + 1];
    sprintf(score_str, "%d", grid->score);
    dstrect.y += 5 + dstrect.h;
    SDL_Texture* score = createSmallText(score_str, &dstrect.w, &dstrect.h,
            *d->resMan->text_colour, d);
    if (dstrect.w > TAB_MAX - 10) dstrect.w = TAB_MAX - 10;
    SDL_RenderCopy(d->renderer, score, NULL, &dstrect);
    SDL_DestroyTexture(score);

    Tetromino t;
    dstrect.y += 5 + dstrect.h;
    dstrect.w = grid->nextTitleWidth;
    dstrect.h = grid->nextTitleHeight;
    SDL_RenderCopy(d->renderer, grid->next_title, NULL, &dstrect);
    t.tet_type = grid->next;
    t.colour = grid->nextColour;
    setTetrominoBoxes(&t);
    drawSmallTetromino(grid, &t, dstrect.x + 20, dstrect.y + dstrect.h + 10, d);

    dstrect.y += 5 + dstrect.h + 60;
    dstrect.w = grid->heldTitleWidth;
    dstrect.h = grid->heldTitleHeight;
    SDL_RenderCopy(d->renderer, grid->held_title, NULL, &dstrect);    
    t.tet_type = grid->held;
    t.colour = grid->heldColour;
    setTetrominoBoxes(&t);
    drawSmallTetromino(grid, &t, dstrect.x + 20, dstrect.y + dstrect.h + 10, d);

    g_x = (SCREEN_WIDTH - grid->boardBackWidth) / 2 + getTweenValue(grid->x);
    dstrect = (SDL_Rect) { g_x, getTweenValue(grid->y) - 100, grid->boardBackWidth,
            grid->boardBackHeight };
    SDL_RenderCopy(d->renderer, d->resMan->board_back, NULL, &dstrect);

    srcrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    dstrect = (SDL_Rect) { 0, 0, GRID_BOX_SIZE, GRID_BOX_SIZE };
    for (int i = HIDDEN_ROWS; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            srcrect.x = grid->grid[i][j] * GRID_BOX_SIZE;
            dstrect.x = getGridBoxX(grid, j);
            dstrect.y = getGridBoxY(grid, i);
            SDL_RenderCopy(d->renderer, d->resMan->boxes, &srcrect, &dstrect);
        }
    }
    drawTetromino(grid, &grid->current, d);

    for (int i = 0; i < GRID_HEIGHT; i++) for (int j = 0; j < GRID_WIDTH; j++) {
        srcrect.x = grid->colours[i][j] * GRID_BOX_SIZE;
        drawParticles(grid->particles[i][j], d, d->resMan->boxes, srcrect);
    }
}

void Grid_keyEvent(Grid* grid, SDL_KeyboardEvent e) {
    if (grid->gameover) return;
    switch (e.keysym.sym) {
        case SDLK_LEFT:
            if (e.type == SDL_KEYUP) {
                grid->moveLeft = false; 
                grid->movePause = 0;
            }
            if (e.type == SDL_KEYDOWN) {
                grid->moveLeft = true;
                lateralMove(grid);
                grid->movePause = LATERAL_PAUSE;
            }
        break;
        case SDLK_RIGHT:
            if (e.type == SDL_KEYUP) {
                grid->moveRight = false;
                grid->movePause = 0;
            }
            if (e.type == SDL_KEYDOWN) { 
                grid->moveRight = true;
                lateralMove(grid);
                grid->movePause = LATERAL_PAUSE;
            }
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
        case SDLK_SPACE:
            if (e.type == SDL_KEYUP) grid->fastDropped = false;
            if (e.type == SDL_KEYDOWN && !grid->fastDropped) {
                int fallen = 0;
                while (moveTetromino(grid, &grid->current, 0, 1)) { fallen++; }
                lockInCurrent(grid);
                setTweenValue(grid->y, 50);
                moveTweenValue(grid->y, EASE_IN, 0, 20, 0);
                grid->fastDropped = true;
                grid->score += fallen * 10;
                grid->redraw = true;
            }
        break;
        case SDLK_RETURN:
        if (!grid->switchedHeld) {
            grid->switchedHeld = true;
            TetrominoType tempt = grid->held;
            int tempc = grid->heldColour;
            grid->heldColour = grid->current.colour;
            grid->held = grid->current.tet_type;
            forceFillTetromino(&grid->current, tempt, tempc);
            grid->redraw = true;
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
    freeTweenValue(grid->shadow);
    freeTweenValue(grid->tabSlide);
    SDL_DestroyTexture(grid->score_title);
    SDL_DestroyTexture(grid->next_title);
    SDL_DestroyTexture(grid->held_title);
    for (int i = 0; i < GRID_HEIGHT; i++) for (int j = 0; j < GRID_WIDTH; j++) {
        freeParticles(grid->particles[i][j]);
    }
    free(grid);
}
