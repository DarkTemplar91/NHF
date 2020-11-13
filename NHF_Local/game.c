#include "gamestate.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "sdlTypes.h"
#include "game.h"
#include "cards.h"
#include <unistd.h>
#include <SDL2/SDL_image.h>

button_t backToMain;

void game(gameState* p, SDL_Window** window, SDL_Renderer** renderer,char* pName)
{

    if(pName)
        gameStateInit(p,pName);
    else
        gameStateInit(p,"You");

    drawGameBoard(window,renderer);
    int h,w;
    SDL_GetWindowSize(*window,&w,&h);
    SDL_Rect pRect={.x=w-79-10,.y=h-123-20,.w=79,.h=123};
    playerRec(p->Players[0],&pRect);
    SDL_RectEmpty(&pRect);


    initCardTexture(renderer);
    SDL_Event ev;
    for(int round=p->currentTrumpSuit;round<=5;round++)
    {
        for(int part=p->roundNo;part<=13;part++)
        {
            for(int calls=p->playerTurn;calls<=4;calls++)
            {

                if(calls==0 &&part==1 &&round==1)
                    refreshBoard(p,window,renderer);

                int cIndex;

                while(true)
                {

                    if(ev.type==SDL_MOUSEBUTTONDOWN && ev.button.clicks>0)
                    {
                        cIndex=clickCardEvent(p,window,&ev);
                        if(cIndex!=-1 || validCard(cIndex)==true)
                        {
                            callCard();
                            break;

                        }


                    }

                }
                printf("Cont");


            }
        }
        shuffleDeck(p->deck);
        dealCards(p->Players,p->deck);

    }
}
void drawGameBoard(SDL_Window** window,SDL_Renderer** renderer)
{
    SDL_DestroyRenderer(*renderer);
    *renderer=SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    SDL_Texture* texture=SDL_CreateTexture(*renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,626,365);
    texture=(SDL_Texture*)IMG_LoadTexture(*renderer,"background.jpg");
    SDL_RenderCopy(*renderer, texture, NULL,NULL);


    backToMain=(button_t)
    {
        .color={.r=255,.g=255,.b=255,.a=255},
        .draw_rect= {.x=0, .y=0,.w=65,.h=15}
    };
    button(*renderer,&backToMain);
    writeText("Back",renderer,&backToMain.draw_rect,12);
    int wH, wW;
    SDL_GetWindowSize(*window,&wW,&wH);
    thickLineRGBA(*renderer,0,wH-150,wW,wH-150,5,220,145,30,255);


    SDL_RenderPresent(*renderer);

}
void refreshBoard(gameState* g,SDL_Window** window, SDL_Renderer** renderer)
{

    int y;
    SDL_GetWindowSize(*window,NULL, &y);
    for(int n=0; n < (g->Players[0].crntHandSize); n++)
    {
        SDL_Rect target={.x=40+n*30,.y=y-20-123,.w=79,.h=123};
        loadCardPic(renderer,g->Players[0].hand[n].suitIndex,g->Players[0].hand[n].faceValueIndex,&target);
        if(&(g->Players[0].discardRect)!=NULL && (g->Players[0].discardedCards.array!=NULL))
        {
            loadCardPic(renderer,5,2,&(g->Players[0].discardRect));
        }

    }
    SDL_RenderPresent(*renderer);


}

int clickCardEvent(gameState* g, SDL_Window** window, SDL_Event* ev)
{

    if(ev->button.button==SDL_BUTTON_LEFT && ev->button.clicks==2)
    {
        int y;
        SDL_GetWindowSize(*window,NULL, &y);
        for(int n=0; n< (g->Players[0].crntHandSize); n++)
        {

            SDL_Rect target={.x=40+n*30,.y=y-20-123,.w=79,.h=123};
            if(ev->button.x>=target.x && ev->button.x<=target.x+30 &&
               ev->button.y>=target.y && ev->button.y<=target.y+target.h)
            {
                return n;
            }

        }
    }

    return -1;
}
