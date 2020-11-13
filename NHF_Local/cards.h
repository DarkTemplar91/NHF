#ifndef CARDS_H_INCLUDED
#define CARDS_H_INCLUDED

typedef enum
{
    Two=2, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
    Jack, Queen, King, Ace
}faceValue;

typedef enum
{
    Clubs=1, Diamonds, Hearts, Spades
}suits;

typedef struct{
    int value;
    faceValue faceValueIndex;
    suits suitIndex;
}cards;

void fillDeck(cards *deck);
void shuffleDeck(cards* deck);
int cardcmpf(const void* a, const void* b);

#endif // CARDS_H_INCLUDED
