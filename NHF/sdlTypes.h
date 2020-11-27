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
void writeText(const char* text, SDL_Renderer** renderer,SDL_Rect* destin,uint8_t fontSize, SDL_Color color);
bool button_process_event(button_t *btn, const SDL_Event *ev);
void initCardTexture(SDL_Renderer** renderer);
void loadCardPic(SDL_Renderer** renderer, int suitNo, int cardNo, SDL_Rect* target);
#endif // SDLTYPES_H_INCLUDED
