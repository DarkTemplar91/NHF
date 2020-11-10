#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED
#include "player.h"

typedef struct
{
    player Players[4];
    int roundNo;
    int playerTurn;
    int currentTrumpSuit;

}gameState;

#endif // GAMESTATE_H_INCLUDED
