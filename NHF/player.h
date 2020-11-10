#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "cards.h"
#include "list.h"

typedef struct{
    int score;
    char name[10];
    cards hand[13];
    DynamicArray currentHand;
}player;

void initPlayer(player* p,char* name);

#endif // PLAYER_H_INCLUDED
