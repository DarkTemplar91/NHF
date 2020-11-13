#include "gamestate.h"
#include <string.h>
#include <stdlib.h>
#include "cards.h"

void gameStateInit(gameState* state, char* playerName)
{
    initPlayer(&(state->Players[0]),playerName,NULL);
    for(int n=1;n<4;n++)
    {
        char* temp[]={"CPlayer "};
        char num[2];
        char finals[10];
        sprintf(num,"%d",n);
        strcpy(finals,*temp);
        strcat(finals,num);
        initPlayer(&(state->Players[n]),finals,NULL);
    }
    state->roundNo=1;
    state->playerTurn=0;
    state->currentTrumpSuit=1;
    fillDeck(state->deck);
    shuffleDeck(state->deck);
    dealCards(state->Players,state->deck);
    qsort(&(state->Players[0].hand),13,sizeof(cards),cardcmpf);
    resetCalledStack(gameState* state);


}
void resetCalledStack(gameState* state)
{
    for(int n=0;n<4;n++)
        state->calledCards[n]=-1;
}
void callCard(gameState* state, int index)
{

}
bool validCard(gameState* state, cards* card)
{

}
