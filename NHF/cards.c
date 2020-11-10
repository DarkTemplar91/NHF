#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cards.h"


static cards Card(faceValue f, suits suitEnum)
{
    char suit[10];
    switch (suitEnum)
    {
        case 1:
            strcpy(suit,"Spades\0");
            break;
        case 2:
            strcpy(suit,"Hearts\0");
            break;
        case 3:
            strcpy(suit,"Diamonds\0");
            break;
        case 4:
            strcpy(suit,"Clubs\0");
            break;

    }
    cards c;
    switch(f)
    {
        case 2:
            c.value=2;
            c.sign='2';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 3:
            c.value=3;
            c.sign='3';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 4:
            c.value=4;
            c.sign='4';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 5:
            c.value=5;
            c.sign='5';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 6:
            c.value=6;
            c.sign='6';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 7:
            c.value=7;
            c.sign='7';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 8:
            c.value=8;
            c.sign='8';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 9:
            c.value=9;
            c.sign='9';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 10:
            c.value=10;
            c.sign='10';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 11:
            c.value=10;
            c.sign='J';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 12:
            c.value=10;
            c.sign='Q';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 13:
            c.value=10;
            c.sign='K';
            strcpy(c.suit,suit);
            c.faceValueIndex=f;
            break;
        case 14:
            c.value=10;
            c.sign='A';
            strcpy(c.suit,suit);
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



