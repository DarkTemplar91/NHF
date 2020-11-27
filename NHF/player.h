#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "cards.h"
#include "list.h"
#include <SDL2/SDL.h>

typedef struct{
    int score;
    int crntHandSize;
    char name[10+1];
    cards hand[13];
    DynamicArray discardedCards;
    SDL_Rect discardRect;
    SDL_Rect callRect;
}player;

void initPlayer(player* p,char* name,SDL_Rect* rec);
void playerRec(player* p, SDL_Rect* rec);
void callRec(player* p, int index, SDL_Window** window);

#endif // PLAYER_H_INCLUDED
