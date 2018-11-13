#include             <string.h>
#include             <stdlib.h>
#include            <stdbool.h>
#include             "button.h"
#include "../resourceManager.h"

// Initialises a button and returns the pointer
Button* initialiseButton(char* title, double x, double y, Display* d) {
    // Create the pointer and assign default values
    Button* b = malloc(sizeof(Button));
    b->hovered = false;
    b->redraw = true;
    b->x = initialiseTween(x);
    b->y = initialiseTween(y);
    // Saves the size of the button image
    SDL_QueryTexture(d->resMan->button, NULL, NULL,
            &(b->width), &(b->height));
    // Turns the text into a texture
    b->text = createSmallText(title, &(b->text_width), &(b->text_height), 
            *d->resMan->button_colour, d);
    return b;
}

// Updates the button
void updateButton(Button* b) {
    // Updates the tween values
    updateTweenValue(b->x);
    b->redraw |= TweenValue_dropRedraw(b->x);
    updateTweenValue(b->y);
    b->redraw |= TweenValue_dropRedraw(b->y);
}

// Draws the button
void drawButton(Button* b, Display* d) {
    SDL_Rect rect = (SDL_Rect) { getTweenValue(b->x), getTweenValue(b->y),
            b->width, b->height };
    if (b->hovered) {
        SDL_RenderCopy(d->renderer, d->resMan->hoverButton,
            NULL, &rect);
    } else {
        SDL_RenderCopy(d->renderer, d->resMan->button,
            NULL, &rect);
    }

    rect = (SDL_Rect) { getTweenValue(b->x) + (b->width - b->text_width) / 2,
            getTweenValue(b->y) + (b->height - b->text_height) / 2,
            b->text_width,
            b->text_height };
    SDL_RenderCopy(d->renderer, b->text, NULL, &rect);
}

// Checks whether a point is inside the button
bool checkIntersection(Button* b, int x, int y) {
    SDL_Rect rect = (SDL_Rect) { getTweenValue(b->x), getTweenValue(b->y),
            b->width, b->height };
    if (rect.x < x && rect.x + rect.w > x) {
        if (rect.y < y && rect.y + rect.h > y) {
            return true;
        }
    }
    return false;
}

// Updates the button with the mouse position
void Button_mousePosition(Button* b, int x, int y) {
    if (checkIntersection(b, x, y)) {
        if (!b->hovered) b->redraw = true;
        b->hovered = true;
        return;
    }
    if (b->hovered) b->redraw = true;
    b->hovered = false;
}

// Checks if the button has been clicked
bool clickButton(Button* b, int x, int y) {
    if (checkIntersection(b, x, y)) {
        return true;
    }
    return false;
}

// Returns the redraw value and sets it to false
bool Button_dropRedraw(Button* b) {
    if (b->redraw) {
        b->redraw = false;
        return true;
    }
    return false;
}

// Frees the button
void freeButton(Button* b) {
    SDL_DestroyTexture(b->text);
    freeTweenValue(b->x);
    freeTweenValue(b->y);
    free(b);
}
