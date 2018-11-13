#ifndef TWEEN_H_
#define TWEEN_H_

#include <stdbool.h>

// Defines the enum containing the possible types of tweens
typedef enum TweenType_ { LINEAR, EASE_IN, EASE_OUT, ELASTIC } TweenType; 

// Defines the TweenValue struct
// -- this struct smoothly 'inbe-tweens' values
// -- allowing for smooth transitions and animations
typedef struct TweenValue_ {
    // The id of the TweenValue, useful for classifying
    // what made the value move
    int id;
    // The type of tween
    TweenType type;
    // The progress through the animation
    int time;
    // The total length of the animation
    int duration;
    // Time that should be waited before the animation
    // begins
    int hold;
    // The starting value
    double start;
    // The destination value
    double end;
    // Whether the animation is done
    bool finished;
    // Whether the container should redraw
    bool redraw;
} TweenValue;

TweenValue* initialiseTween(double s);
void updateTweenValue(TweenValue* t);
void moveTweenValue(TweenValue* tween, TweenType ttype, double e,
        int duration, int hold);
void setTweenValue(TweenValue* t, double s);
double getTweenValue(TweenValue* t);
bool arrived(TweenValue* t);
bool TweenValue_dropRedraw(TweenValue* t);
void freeTweenValue(TweenValue* t);

#endif
