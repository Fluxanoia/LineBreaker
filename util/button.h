#ifndef BUTTON_H_
#define BUTTON_H_

#include              "tween.h"
#include "../resourceManager.h"

// Defines the button struct
typedef struct Button_ {
    // A variable that holds whether the component should redraw or not
    bool redraw;
    // Variables holding the x and y of the button
    TweenValue* x;
    TweenValue* y;
    // The width and height of the button and its text
    int width;
    int height;
    int text_width;
    int text_height;
    // The text texture
    SDL_Texture* text;
    // A variable holding whether the button is hovered or not
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
