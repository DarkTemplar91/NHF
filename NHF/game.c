#include "gamestate.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "sdlTypes.h"

button_t backToMain;

void game(gameState p)
{
    for(int round=0;round<5;round++)
    {
        for(int part=0;part<13;part++)
        {
            for(int calls=0;calls<4;calls++)
            {

            }
        }
    }
}

 void drawGameBoard(SDL_Window** window,SDL_Renderer** renderer)
{
    printf("gameboard");
    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    SDL_Texture* texture=SDL_CreateTexture(*renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,626,365);
    texture=IMG_LoadTexture(*renderer,"background.jpg");


    backToMain=(button_t)
    {
        .color={.r=255,.g=255,.b=255,.a=255},
        .draw_rect= {.x=0, .y=0,.w=50,.h=20}
    };
    button(*renderer,&backToMain);
    SDL_RenderPresent(*renderer);

}
