#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "cards.h"
#include "list.h"
#include <SDL2/SDL.h>

typedef struct{
    int score;
    int crntHandSize;
    char name[10];
    cards hand[13];
    DynamicArray discardedCards;
    SDL_Rect discardRect;
}player;

void initPlayer(player* p,char* name,SDL_Rect* rec);

#endif // PLAYER_H_INCLUDED
