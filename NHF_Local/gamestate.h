#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED
#include "player.h"

typedef struct
{
    player Players[4];
    int currentTrumpSuit;
    int roundNo;
    int playerTurn;
    cards deck[52];
    cards calledCards[4];
    int firstSuitCalled

}gameState;

void gameStateInit(gameState* state, char* playerName);
bool validCard(gameState* state, cards* card);
void callCard(gameState* state, int index);
void resetCalledStack(gameState* state);
#endif // GAMESTATE_H_INCLUDED
