#ifndef BUTTON_H_
#define BUTTON_H_

#include              "tween.h"
#include "../resourceManager.h"

typedef struct Button_ {
    bool redraw;
    TweenValue* x;
    TweenValue* y;
    int width;
    int height;
    int text_width;
    int text_height;
    SDL_Texture* text;
    bool hovered;
} Button;

Button* initialiseButton(char* title, double x, double y, Display* d);
void updateButton(Button *b);
void drawButton(Button* b, Display* d);
void Button_mousePosition(Button* b, int x, int y);
bool clickButton(Button* b, int x, int y);
bool Button_dropRedraw(Button* b);
void freeButton(Button* b);

#endif
