#include  <string.h>
#include  <stdlib.h>
#include <stdbool.h>
#include  "button.h"

Button* initialiseButton(char* title, double x, double y, Display* d) {
    Button* b = malloc(sizeof(Button));
    b->hovered = false;
    b->redraw = true;
    b->x = initialiseTween(x);
    b->y = initialiseTween(y);
    SDL_QueryTexture(d->resMan->button, NULL, NULL,
            &(b->width), &(b->height));
    
    SDL_Color color = (SDL_Color) { 40, 40, 40, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(d->resMan->base, title, color);
    b->text = SDL_CreateTextureFromSurface(d->renderer, surface);
    SDL_FreeSurface(surface);

    SDL_QueryTexture(b->text, NULL, NULL,
            &(b->text_width), &(b->text_height));

    return b;
}

void updateButton(Button* b) {
    updateTweenValue(b->x);
    b->redraw |= TweenValue_dropRedraw(b->x);
    updateTweenValue(b->y);
    b->redraw |= TweenValue_dropRedraw(b->y);
}

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

void Button_mousePosition(Button* b, int x, int y) {
    if (checkIntersection(b, x, y)) {
        if (!b->hovered) b->redraw = true;
        b->hovered = true;
        return;
    }
    if (b->hovered) b->redraw = true;
    b->hovered = false;
}

bool clickButton(Button* b, int x, int y) {
    if (checkIntersection(b, x, y)) {
        return true;
    }
    return false;
}

bool Button_dropRedraw(Button* b) {
    if (b->redraw) {
        b->redraw = false;
        return true;
    }
    return false;
}

void freeButton(Button* b) {
    SDL_DestroyTexture(b->text);
    freeTweenValue(b->x);
    freeTweenValue(b->y);
    free(b);
}
