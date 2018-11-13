#include                     <SDL.h>
#include                    <time.h>
#include                   <stdio.h>
#include                 <stdbool.h>
#include                 <SDL_ttf.h>
#include                "handling.h"
#include         "resourceManager.h"
#include "states/gameStateManager.h"

#define SDL_MAIN_HANDLED
// Defines the game title
#define GAME_TITLE         "Line Breaker"
// Defines the games updates per second
#define UPDATES_PER_SECOND 50
// Calculates the time inbetween updates in milliseconds
#define UPDATE_DELTA_TIME  (1000 / UPDATES_PER_SECOND)

// A variable tracking whether the program should be running
bool running = false;
// A variable tracking whether the program should be printing
// the amount of updates occuring in every second
bool PRINT_APS = false;

// A pointer holding the Display struct
Display* display = NULL;
// A pointer holding the GSM struct
GSM* gsm = NULL;

// Update components with a mouse motion
void mouseMotionEvent(SDL_MouseMotionEvent e) {
    if (gsm != NULL) GSM_mouseMotionEvent(gsm, e);
}

// Update components with a mouse button press
void mouseButtonEvent(SDL_MouseButtonEvent e) {
    if (gsm != NULL) GSM_mouseButtonEvent(gsm, e);
}

// Update components with keyEvents
void keyEvent(SDL_KeyboardEvent e) {
    if (gsm != NULL) GSM_keyEvent(gsm, e);
}

// Poll for SDL events
void pollEvents() {
    // If there's no events, return
    if (SDL_PollEvent(NULL) == 0) return;
    // Event types :: https://wiki.libsdl.org/SDL_EventType
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch(e.type) {
            // Quitting the game (on exit, ALT+F4, etc.)
            case SDL_QUIT:
                running = false;
                continue;
            // When a key is pressed or released
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                keyEvent(e.key);
                continue;
            break;
            // When mouse motion occurs
            case SDL_MOUSEMOTION:
                mouseMotionEvent(e.motion);
                continue;
            break;
            // When the mouse buttons are pressed
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                mouseButtonEvent(e.button);
                continue;
            break;
        }
    }
}

// Update components of the program
void update() {
    if (gsm != NULL) {
        updateGSM(gsm);
        if (gsm->closed) running = false;
    }
}

// Draw components
void draw() {
    // Draw the GSM
    if (gsm != NULL && drawGSM(gsm)) {
        // Update the window
	    SDL_RenderPresent(display->renderer);
    }
}

// The main game loop
void run() {
    running = true;
    unsigned int lastSecond = SDL_GetTicks();
    unsigned int lastUpdate = SDL_GetTicks();
    unsigned int APS = 0;

    printf("\nStarting the main game loop...\n");
    while (running) {
        // Poll events
        pollEvents();
        // Update and draw as many times as necessary
        while (SDL_GetTicks() - lastUpdate >= UPDATE_DELTA_TIME) {
            lastUpdate += UPDATE_DELTA_TIME;
            update();
            draw();
            APS++;
        }
        // Print out the actions per second if allowed
        if (PRINT_APS) {
            while (SDL_GetTicks() - lastSecond >= 1000) {
                lastSecond += 1000;
                printf("Actions in the last seconds : %d\n", APS);
                APS = 0;
            }
        }
    }
    printf("Main game loop exited.\n\n");
}

// Initialise the game
int main(int argc, char const *argv[]) {
    printf("\n%s, starting up...\n", GAME_TITLE);
    
    // Read in the command line arguments
    StateType startingState = LOADING;
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-aps", 4) == 0) PRINT_APS = true;
        if (strncmp(argv[i], "-l", 2) == 0)   startingState = MENU;
    }

    printf("Initialising SDL...\n");
    I_(SDL_Init(SDL_INIT_VIDEO));
    I_(TTF_Init());

    printf("Allocating memory for the display struct...\n");
    display = malloc(sizeof(struct Display_));

    printf("Creating a window...\n");
    display->window = P_(SDL_CreateWindow(
            GAME_TITLE,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_BORDERLESS));

    printf("Creating the renderer from the window...\n");
    display->renderer = P_(SDL_CreateRenderer(display->window, 
            -1, SDL_RENDERER_ACCELERATED));
    I_(SDL_SetRenderDrawBlendMode(display->renderer, SDL_BLENDMODE_BLEND));

    printf("Initialising resource manager...\n");
    display->resMan = initialiseResourceManager(display);

    printf("Seeding the random number generator...\n");
    srand(time(NULL));

    printf("Initialising the GSM...\n");
    gsm = initialiseGSM(display, startingState); 

    printf("Start up complete, running the game...\n");
    run();

    printf("Freeing memory...\n");
    freeGSM(gsm);

    printf("Freeing the display...\n");
    freeDisplay(display);

    printf("Quitting SDL...\n");
    TTF_Quit();
    SDL_Quit();

    printf("Exiting...\n\n");
    return 0;
}
