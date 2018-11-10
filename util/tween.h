#ifndef TWEEN_H_
#define TWEEN_H_

#include <stdbool.h>

typedef enum TweenType_ { LINEAR, EASE_IN, EASE_OUT, ELASTIC } TweenType; 

typedef struct TweenValue_ {
    int id;
    TweenType type;
    int time;
    int duration;
    int hold;
    double start;
    double end;
    bool finished;
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
