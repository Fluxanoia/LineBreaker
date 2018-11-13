#ifndef PARTICLES_H_
#define PARTICLES_H_

#define MAX_PARTICLE_COUNT 16

#include "../resourceManager.h"

// Defines the Particles struct
typedef struct Particles_ {
    // A variable holding whether the component should redraw or not
    bool redraw;
    // Variable holding how much the particle origin can vary from
    // the given value
    double variance;
    // The rate of falling of the particles
    double gravity;
    // The max possible speed in the x/y direction
    double speed;
    // The particle size in px
    int size;
    // The amount of particles
    int count;
    // Arrays with the x,y positions and the velocities
    double xs[MAX_PARTICLE_COUNT];
    double ys[MAX_PARTICLE_COUNT];
    double xvs[MAX_PARTICLE_COUNT];
    double yvs[MAX_PARTICLE_COUNT];
} Particles;

Particles* initialiseParticles(int count, int size,
        double speed, double grav, double variance);
void resetParticles(Particles* p, int x, int y);
void updateParticles(Particles* p);
void drawParticles(Particles* p, Display* d, SDL_Texture* texture, SDL_Rect srcrect);
bool Particles_dropRedraw(Particles* p);
void freeParticles(Particles* p);

#endif
