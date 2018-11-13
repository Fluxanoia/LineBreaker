#include                <SDL.h>
#include            <stdbool.h>
#include          "particles.h"

// Initialises a Particles struct and returns the pointer
Particles* initialiseParticles(int count, int size,
        double speed, double grav, double variance) {
    Particles* p = malloc(sizeof(Particles));
    p->redraw = false;

    p->variance = variance;
    p->gravity = grav;
    p->speed = speed;
    p->size = size;
    p->count = count;

    // Initialise the particles far enough away that they won't be drawn
    for (int i = 0; i < count; i++) {
        p->xs[i] = -size * 2;
        p->ys[i] = -size * 2;
        p->xvs[i] = 0;
        p->yvs[i] = 0;
    }

    return p;
}

// Returns whether the particle is offscreen or not
bool offscreen(Particles* p, int i) {
    if (p->xs[i] + p->size < 0) return true;
    if (p->xs[i] > SCREEN_WIDTH) return true;
    if (p->ys[i] + p->size < 0) return true;
    if (p->ys[i] > SCREEN_HEIGHT) return true;
    return false;
}

// Returns a random number from 0 to 1
double modifierRand() {
    return (double) rand() / (double) RAND_MAX;
}

// Returns -1 or 1 at random
double flipRand() {
    double d = modifierRand();
    if (rand() % 2 == 0) d *= -1;
    return d;
}

// Reset all the particles to fall from a point
void resetParticles(Particles* p, int x, int y) {
    for (int i = 0; i < p->count; i++) {
        p->xs[i] = (double) x + p->variance * flipRand();
        p->ys[i] = (double) y + p->variance * flipRand();
        p->xvs[i] = p->speed * flipRand();
        p->yvs[i] = p->speed * modifierRand() * -1;
    }
}

// Update the particles
void updateParticles(Particles* p) {
    for (int i = 0; i < p->count; i++) {
        if (!offscreen(p, i)) {
            p->xs[i] += p->xvs[i];
            p->ys[i] += p->yvs[i];
            p->yvs[i] += p->gravity;
            p->redraw = true;
        }
    }
}

// Draw the particles
void drawParticles(Particles* p, Display* d, SDL_Texture* texture, SDL_Rect srcrect) {
    for (int i = 0; i < p->count; i++) {
        if (!offscreen(p, i)) {
            break;
        }
        if (i == p->count - 1) return;
    }

    SDL_Rect dstrect = (SDL_Rect) { 0, 0, 0, 0 };
    for (int i = 0; i < p->count; i++) {
        if (!offscreen(p, i)) {
            dstrect.x = p->xs[i];
            dstrect.y = p->ys[i];
            dstrect.w = p->size;
            dstrect.h = p->size;
            SDL_RenderCopy(d->renderer, texture, &srcrect, &dstrect);
        }
    }
}

// Returns the redraw value and sets it to false
bool Particles_dropRedraw(Particles* p) {
    if (p->redraw) {
        p->redraw = false;
        return true;
    }    
    return false;
}

// Frees the particles
void freeParticles(Particles* p) {
    free(p);
}
