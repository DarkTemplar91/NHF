﻿#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "gamestate.h"
#include "sdlTypes.h"
#include "game.h"

//A main menu gombjai
button_t m_newGameBTN;
button_t m_prevGameBTN;
button_t m_highScrBTN;
button_t m_rulesBTN;
gameState state;

//megrajzolja a főmenüt
void drawMainMenu(SDL_Window** window, SDL_Renderer** renderer)
{
    IMG_Init(0);
    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    //betölti a hátteret
    SDL_Texture* texture=SDL_CreateTexture(*renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,626,365);
    texture=IMG_LoadTexture(*renderer,"background.jpg");
    SDL_RenderCopy(*renderer, texture,NULL,NULL);


    //gombok változóinak megadása
    m_newGameBTN=(button_t){
        .color={.r=225,.g=255,.b=230,.a=255},
        .draw_rect= {.x=235, .y=150,.w=170,.h=30}
    };
    m_prevGameBTN=(button_t)
    {
        .color={.r=225,.g=255,.b=230,.a=255},
        .draw_rect= {.x=235, .y=m_newGameBTN.draw_rect.y+35,.w=170,.h=30}
    };

    m_highScrBTN=(button_t)
    {
        .color={.r=225,.g=255,.b=230,.a=255},
        .draw_rect= {.x=235, .y=m_prevGameBTN.draw_rect.y+35,.w=170,.h=30}
    };

    m_rulesBTN=(button_t)
    {
        .color={.r=225,.g=255,.b=230,.a=255},
        .draw_rect= {.x=235, .y=m_highScrBTN.draw_rect.y+35,.w=170,.h=30}
    };

    //felrajzolja a gombokat
    button(*renderer,&m_newGameBTN);
    button(*renderer, &m_prevGameBTN);
    button(*renderer,&m_highScrBTN);
    button(*renderer, &m_rulesBTN);
    writeText("New Game", renderer,&m_newGameBTN.draw_rect,24);
    writeText("Continue Game", renderer,&m_prevGameBTN.draw_rect,24);
    writeText("TOP LIST", renderer,&m_highScrBTN.draw_rect,24);
    writeText("Rules", renderer,&m_rulesBTN.draw_rect,24);
    SDL_RenderPresent(*renderer);

    IMG_Quit();
}
void sdl_init(int w, int h, SDL_Window **pwindow, SDL_Renderer **prenderer)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    SDL_Window* window=SDL_CreateWindow("Pikk Dáma",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(window==NULL||renderer==NULL)
    {
        exit(1);
    }
    *pwindow = window;
    *prenderer = renderer;

}
int main(int argc, char *argv[])
{
    //főablak létrehozása
    SDL_Window* window;
    SDL_Renderer* renderer;
    sdl_init(640,480,&window,&renderer);

    //main
    drawMainMenu(&window,&renderer);
    //

    //event handler
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT)
    {
        if(button_process_event(&m_newGameBTN, &ev))
        {
            gameState newGame;
            //TODO -> Ask for player name in popup
            game(&newGame,&window,&renderer,"Jack");
            drawMainMenu(&window,&renderer);


        }
        else if(button_process_event(&m_prevGameBTN, &ev))
        {
            //TODO -> prevGame();
        }
        else if(button_process_event(&m_highScrBTN, &ev))
        {
            //TODO -> leaderBoard();
        }
        else if(button_process_event(&m_rulesBTN, &ev))
        {
            //TODO -> displayRules();
        }

    }

    //exit
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
