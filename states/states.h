#ifndef STATES_H_
#define STATES_H_

// Defines all the different game states 
typedef enum StateType_ { 
    NIL,                // No state
    CLOSE,              // Tell the GSM to close the game
    LAST_STATE,         // Tell the GSM to go back to the last state
    LOADING, 
    MENU, 
    SINGLEPLAYER, 
    MULTIPLAYER, 
    PAUSE
} StateType;

#endif
