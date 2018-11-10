#include               <math.h>
#include             <stdlib.h>
#include              "tween.h"
#include "../resourceManager.h"

TweenValue* initialiseTween(double s) {
    TweenValue* tween = malloc(sizeof(TweenValue));
    tween->type = LINEAR;
    tween->id = 0;
    setTweenValue(tween, s);
    return tween;
}

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

void setTweenValue(TweenValue* t, double s) {
    t->time = 0;
    t->duration = 0;
    t->hold = 0;
    t->start = s;
    t->end = s;
    t->redraw = true;
    t->finished = true;
}

bool arrived(TweenValue* t) {
    if (t->time == t->duration) return true;
    return false;
}

bool TweenValue_dropRedraw(TweenValue* t) {
    if (t->redraw) {
        t->redraw = false;
        return true;
    }
    return false;
}

void freeTweenValue(TweenValue* t) {
    free(t);
}

// Each following function takes params: start, end, time, duration

double linearTween(double s, double e, int t, int d) {
    return s + ((double) t / (double) d) * (e - s);
}

double easeInTween(double s, double e, int t, int d) {
    return s + pow((double) t / (double) d, 3) * (e - s);
}

double easeOutTween(double s, double e, int t, int d) {
    return s + (1 - pow(1 - ((double) t / (double) d), 3)) * (e - s);
}

double elasticTween(double s, double e, int t, int d) {
    double p = 0.3;
    double inter = (double) t / (double) d;
    return s + (pow(2,-10*inter) * sin((inter-p/4)*(2*M_PI)/p) + 1) * (e - s);
}

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
