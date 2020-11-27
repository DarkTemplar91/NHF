#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cards.h"
#include "player.h"


static cards Card(faceValue f, suits suitEnum)
{
    cards c;
    c.suitIndex=suitEnum;
    c.faceValueIndex=f;
    (f>=10) ? (c.value=10) : (c.value=f);
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
void switchCards(cards *a, cards *b)
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

//qsort-hoz segéd fv
int cardcmpf(const void* a, const void* b)
{
    cards a1=*(const cards*)a;
    cards b1=*(const cards*)b;

    if(a1.suitIndex<b1.suitIndex||((a1.suitIndex==b1.suitIndex) && (a1.faceValueIndex<b1.faceValueIndex)))
        return -1;

    return 1;

}



