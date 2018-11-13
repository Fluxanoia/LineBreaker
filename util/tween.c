#include               <math.h>
#include             <stdlib.h>
#include              "tween.h"
#include "../resourceManager.h"

// Creates a tween value and returns the pointer
TweenValue* initialiseTween(double s) {
    TweenValue* tween = malloc(sizeof(TweenValue));
    tween->type = LINEAR;
    tween->id = 0;
    setTweenValue(tween, s);
    return tween;
}

// Updates the tween valeu
void updateTweenValue(TweenValue* t) {
    if (t->hold > 0) {
        t->hold = t->hold - 1;
        return;
    }
    if (t->time == t->duration) {
        if (!t->finished) {
            t->redraw = true;
            t->finished = true;
        }
    } else {
        t->time = t->time + 1;
        t->redraw = true;
    }
} 

// Sets the tween to move toward a value
void moveTweenValue(TweenValue* t, TweenType ttype,
    double e, int duration, int holdTime) {
    t->type = ttype;
    t->time = 0;
    t->duration = duration;
    t->hold = holdTime;
    t->start = t->end;
    t->end = e;
    t->redraw = true;
    t->finished = false;
}

// Set the tween to a value
void setTweenValue(TweenValue* t, double s) {
    t->time = 0;
    t->duration = 0;
    t->hold = 0;
    t->start = s;
    t->end = s;
    t->redraw = true;
    t->finished = true;
}

// Returns whether the tween value has reached its end value
bool arrived(TweenValue* t) {
    if (t->time == t->duration) return true;
    return false;
}

// Returns the redraw value and sets it to false
bool TweenValue_dropRedraw(TweenValue* t) {
    if (t->redraw) {
        t->redraw = false;
        return true;
    }
    return false;
}

// Frees the tween value
void freeTweenValue(TweenValue* t) {
    free(t);
}

// Each following function takes params: start, end, time, duration

// A linear tween
double linearTween(double s, double e, int t, int d) {
    return s + ((double) t / (double) d) * (e - s);
}

// An ease in tween
double easeInTween(double s, double e, int t, int d) {
    return s + pow((double) t / (double) d, 3) * (e - s);
}

// An ease out tween
double easeOutTween(double s, double e, int t, int d) {
    return s + (1 - pow(1 - ((double) t / (double) d), 3)) * (e - s);
}

// A bouncy tween
double elasticTween(double s, double e, int t, int d) {
    double p = 0.3;
    double inter = (double) t / (double) d;
    return s + (pow(2,-10*inter) * sin((inter-p/4)*(2*M_PI)/p) + 1) * (e - s);
}

// Returns the value of a tween value
double getTweenValue(TweenValue* t) {
    if (t->time == t->duration) return t->end;
    if (t->hold > 0) return t->start;

    switch (t->type) {
        case LINEAR: return linearTween(t->start, t->end, t->time, t->duration);
        case EASE_IN: return easeInTween(t->start, t->end, t->time, t->duration);
        case EASE_OUT: return easeOutTween(t->start, t->end, t->time, t->duration);
        case ELASTIC: return elasticTween(t->start, t->end, t->time, t->duration);
    } 
}
