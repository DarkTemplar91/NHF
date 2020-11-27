#include "gamestate.h"
#include <string.h>
#include <stdlib.h>
#include "cards.h"
#include "time.h"


void gameStateInit(gameState* state, char* playerName)
{
    initPlayer(&(state->Players[0]),playerName,NULL);
    for(int n=1;n<4;n++)
    {
        char* temp="CPlayer";
        char num[2];
        char finals[10];
        sprintf(num,"%d",n);
        strcpy(finals,temp);
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
    state->firstSuitCalled=-1;
    state->offSet=4;
    resetCalledStack(state);


}
void dealCards(player* players, cards* deck)
{
    for(int h=0;h<4;h++)
        players[h].crntHandSize=13;
    for(int n=0;n<13;n++)
    {
        for(int i=0;i<4;i++)
        {
            players[i].hand[n]=deck[(i)*13+n];
        }
    }
}
void callCard(gameState* state, SDL_Renderer** renderer, int pIndex, int cIndex)
{
    state->calledCards[pIndex]=state->Players[pIndex].hand[cIndex];
    switchCards(&(state->Players[pIndex].hand[cIndex]),&(state->Players[pIndex].hand[state->Players[pIndex].crntHandSize-- -1]));
}
bool validCard(gameState* state, int pIndex, int cIndex)
{
    if(state->firstSuitCalled==-1)
        return true;
    bool hasSuit=false;
    for(int n=0;n<state->Players[pIndex].crntHandSize;n++)
    {
        if(state->Players[pIndex].hand[n].suitIndex==state->firstSuitCalled)
        {
             hasSuit=true;
             break;
        }
    }

    if(state->firstSuitCalled!=state->Players[pIndex].hand[cIndex].suitIndex && hasSuit)
        return false;

    return true;
}
int pickRandom(gameState* state, int pIndex)
{
    srand(time(0));
    int cIndex=rand()%state->Players[pIndex].crntHandSize;
    do
    {
        cIndex=rand()%state->Players[pIndex].crntHandSize;

    }while(validCard(state,pIndex,cIndex)==false);
    return cIndex;

}

void calcCallScr(gameState* state)
{
    int temp[4];
    bool takenAll=false;
    int index=-1;
    for(int n=0;n<4;n++)
    {
        for(int i=0;i<state->Players[n].discardedCards.used;i++)
        {
            if(state->Players[n].discardedCards.array[i].suitIndex==3 || (state->Players[n].discardedCards.array[i].suitIndex==4 && state->Players[n].discardedCards.array[i].faceValueIndex==12))
            {
                temp[n]+=state->Players[n].discardedCards.array[i].value;
            }
        }
        if(temp[n]==114)
        {
            takenAll=true;
            index=n;
            break;
        }
    }

    for(int n=0;n<4;n++)
    {
        if(takenAll)
            (index==n) ? (state->Players[n].score=0) : (state->Players[n].score=114);
        else
            state->Players[n].score+=temp[n];
    }


}


int takeCallStack(gameState* state)
{
    int maxIndex=0;
    for(int n=1;n<4;n++)
    {
        if((state->calledCards[maxIndex].suitIndex==state->calledCards[n].suitIndex &&
            state->calledCards[maxIndex].faceValueIndex<state->calledCards[n].faceValueIndex) ||
            (state->calledCards[maxIndex].suitIndex!=state->currentTrumpSuit &&
            state->calledCards[n].suitIndex==state->currentTrumpSuit))
        {
            maxIndex=n;
        }
    }
    for(int n=0;n<4;n++)
    {
        addToArray(&state->Players[maxIndex].discardedCards,state->calledCards[n]);
    }
    return maxIndex;
}
void resetCalledStack(gameState* state)
{
    for(int n=0;n<4;n++)
        state->calledCards[n].suitIndex=-1;
}
void resetAllDynamic(gameState* state)
{
    for(int n=0;n<4;n++)
    {
        if(state->Players[n].discardedCards.used!=0)
        {
            discardArray(&(state->Players[n].discardedCards));
        }
    }
}

void updateGameState(gameState* state, int round, int play, int call,int off)
{
    state->currentTrumpSuit=round;
    state->roundNo=play;
    state->playerTurn=call;
    state->offSet=off;
}
void exitGameState(gameState* state)
{
    for(int n=0;n<4;n++)
    {
        if(state->Players[n].discardedCards.array!=NULL)
        {
            free(state->Players[n].discardedCards.array);
        }

        state->Players[n].discardedCards.used=state->Players[n].discardedCards.size=0;
        state->Players[n].discardedCards.array=NULL;
    }

}
void saveGameState(gameState* state)
{
    FILE* savefile;
    savefile=fopen("lastgame.txt","w");
    if(!savefile)
        exit(1);

    fprintf(savefile,"Round: %d Play: %d Call: %d",state->currentTrumpSuit,state->roundNo,state->playerTurn);
    fprintf(savefile,"\nOFFSET: %d;",state->offSet);
    for(int n=0;n<4;n++)
    {
        fprintf(savefile,"\nName:%s %d %d;",state->Players[n].name,state->Players[n].score,state->Players[n].crntHandSize);
        fprintf(savefile,"\nDiscardRect: x=%d, y=%d, w=%d, h=%d;",
                state->Players[n].discardRect.x,state->Players[n].discardRect.y,state->Players[n].discardRect.w,state->Players[n].discardRect.h);
        for(int i=0;i<state->Players[n].crntHandSize;i++)
        {
            fprintf(savefile,"\nS: %d, R: %d;",state->Players[n].hand[i].suitIndex,state->Players[n].hand[i].faceValueIndex);
        }
        fprintf(savefile,"\nDiscard Array:\nSize: %zu, Used: %zu;",(state->Players[n].discardedCards.size),(state->Players[n].discardedCards.used));
        for(int i=0;i<state->Players[n].discardedCards.used;i++)
        {
            fprintf(savefile,"\nS: %d, R: %d;",state->Players[n].discardedCards.array[i].suitIndex,state->Players[n].discardedCards.array[i].faceValueIndex);
        }
    }
    fclose(savefile);
}
gameState* loadGameState()
{
    gameState* state=(gameState*)malloc(sizeof(gameState));
    FILE* savefile;
    savefile=fopen("lastgame.txt","r");
    state->currentTrumpSuit=-1;
    if(!savefile)
        state;
    fscanf(savefile,"Round: %d Play: %d Call: %d",&state->currentTrumpSuit,&state->roundNo,&state->playerTurn);
    fscanf(savefile,"\nOFFSET: %d;",&state->offSet);
    *(int*)&state->inCurrentTrumpSuit=state->currentTrumpSuit;
    *(int*)&state->inRoundNo=state->roundNo;
    *(int*)&state->inPlayerTurn=state->playerTurn;

    for(int n=0;n<4;n++)
    {
        initPlayer(&(state->Players[n]),"",NULL);
        fscanf(savefile,"\nName:%s%d %d;",state->Players[n].name,&state->Players[n].score,&state->Players[n].crntHandSize);
        SDL_Rect tempRec;
        fscanf(savefile,"\nDiscardRect: x=%d, y=%d, w=%d, h=%d;",&tempRec.x,&tempRec.y,&tempRec.w,&tempRec.h);
        playerRec(&state->Players[n],&tempRec);
        tempRec.h=tempRec.w=tempRec.x=tempRec.y=0;

        for(int i=0;i<(state->Players[n].crntHandSize);i++)
        {
            int sv,fv;
            fscanf(savefile,"\nS: %d, R: %d;",&sv,&fv);
            state->Players[n].hand[i].faceValueIndex=fv;
            state->Players[n].hand[i].suitIndex=sv;
            (fv>=10) ? (state->Players[n].hand[i].value=10) : (state->Players[n].hand[i].value=fv);
        }
        fscanf(savefile,"\nDiscard Array:\nSize: %zu, Used: %zu;",&state->Players[n].discardedCards.size,&state->Players[n].discardedCards.used);
        if(state->Players[n].discardedCards.used>0)
        {
            /*
            //az addToArray fv-vel növeljük a
            state->Players[n].discardedCards.used
            értékét ezért át kell másolni annak értékét,
            hogy a következõben ne végtelenségig fusson a loop.
            */
            int max;
            memcpy(&max,&state->Players[n].discardedCards.used,sizeof(int));


            for(int j=0;j<max;j++)
            {
                int sv,fv;
                fscanf(savefile,"\nS: %d, R: %d;",&sv,&fv);
                cards newCard;
                newCard.faceValueIndex=fv;
                newCard.suitIndex=sv;
                (fv>=10) ? (newCard.value=10) : (newCard.value=fv);

                if(j<state->Players[n].discardedCards.size)
                {
                    state->Players[n].discardedCards.array[j]=newCard;
                }
                else
                {
                    addToArray(&state->Players[n].discardedCards,newCard);
                }
            }
        }
    }

    fillDeck(state->deck);
    shuffleDeck(state->deck);
    fclose(savefile);
    return state;
}
