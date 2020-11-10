#ifndef SDLTYPES_H_INCLUDED
#define SDLTYPES_H_INCLUDED
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect draw_rect;    // dimensions of button
    struct {
        uint16_t r, g, b, a;
    } color;

} button_t;

void button(SDL_Renderer* renderer, button_t* btn);
void writeText(const char* text, TTF_Font font, SDL_Color color,SDL_Rect messBox, SDL_Renderer renderer);
bool button_process_event(button_t *btn, const SDL_Event *ev);
#endif // SDLTYPES_H_INCLUDED
