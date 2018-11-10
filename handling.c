#include      <SDL.h>
#include    <stdio.h>
#include   <stdlib.h>
#include "handling.h"

// Fail, printing the SDL error message, and stop the program.
void fail() {
    fprintf(stderr, "%s\n", SDL_GetError());
    exit(1);
}

// Check the results from SDL functions for errors.
int I_(int n) { if (n < 0) fail(); return n; }
void *P_(void *p) { if (p == NULL) fail(); return p; }
