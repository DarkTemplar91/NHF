#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED
#include "player.h"
#include <stdbool.h>

typedef struct
{
    player Players[4];
    int const inCurrentTrumpSuit;
    int const inRoundNo;
    int const inPlayerTurn;
    int currentTrumpSuit, roundNo, playerTurn;

    cards deck[52];
    cards calledCards[4];
    int firstSuitCalled;
    int offSet;

}gameState;

void gameStateInit(gameState* state, char* playerName);
void saveGameState(gameState* state);
gameState* loadGameState();
void exitGameState(gameState* state);
void updateGameState(gameState* state, int round, int play, int call,int off);

bool validCard(gameState* state, int pIndex, int cIndex);
void callCard(gameState* state,SDL_Renderer** renderer, int pIndex ,int cindex);
int pickRandom(gameState* state, int pIndex);
void dealCards(player* players, cards* deck);

void resetCalledStack(gameState* state);
int takeCallStack(gameState* state);
void resetAllDynamic(gameState* state);

void calcCallScr(gameState* state);
#endif // GAMESTATE_H_INCLUDED
