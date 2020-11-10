#ifndef CARDS_H_INCLUDED
#define CARDS_H_INCLUDED

typedef struct{
    int value;
    char sign;
    //change signs
    char suit[10];
    int faceValueIndex;


}cards;

typedef enum
{
    Two=2, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
    Jack, Queen, King, Ace
}faceValue;

typedef enum
{
    Spades=1, Hearts, Diamonds, Clubs
}suits;

void fillDeck(cards *deck);
void shuffleDeck(cards* deck);

#endif // CARDS_H_INCLUDED
