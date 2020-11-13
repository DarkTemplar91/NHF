#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cards.h"
#include "player.h"


static cards Card(faceValue f, suits suitEnum)
{
    cards c;
    switch(f)
    {
        case 2:
            c.value=2;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 3:
            c.value=3;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 4:
            c.value=4;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 5:
            c.value=5;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 6:
            c.value=6;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 7:
            c.value=7;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 8:
            c.value=8;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 9:
            c.value=9;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 10:
            c.value=10;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 11:
            c.value=10;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 12:
            c.value=10;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 13:
            c.value=10;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
        case 14:
            c.value=10;
            c.suitIndex=suitEnum;
            c.faceValueIndex=f;
            break;
    }
    return c;
}
void fillDeck(cards *deck)
{
        int n=0;
        for(int s=1;s<=4;s++)
        {
            for(int c=2;c<=14;c++, n++)
            {

                deck[n]=Card(c, s);
            }
        }
}
static void switchCards(cards *a, cards *b)
{
    cards temp=*a;
    *a=*b;
    *b=temp;
}

void shuffleDeck(cards* deck)
{
    int index;
    srand(time(0));
    for(int n=0;n<52;n++)
    {
        index=((rand()%(52-n))+n);
        switchCards(&deck[n],&deck[index]);
    }
}
void dealCards(player* players, cards* deck)
{
    for(int n=0;n<13;n++)
    {
        for(int i=0;i<4;i++)
        {
            players[i].hand[n]=deck[(i)*13+n];
        }
    }
}
//qsort-hoz segéd fv
int cardcmpf(const void* a, const void* b)
{
    cards a1=*(const cards*)a;
    cards b1=*(const cards*)b;

    if(a1.suitIndex<b1.suitIndex||((a1.suitIndex==b1.suitIndex) && (a1.faceValueIndex<b1.faceValueIndex)))
        return -1;

    return 1;

}



