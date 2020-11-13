#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include "cards.h"
#include <stdio.h>

typedef struct
{
    cards *array;
    size_t used;
    size_t size;
}DynamicArray;

void newDynamicArray(DynamicArray* a);
void addToArray(DynamicArray* a, cards newElement);
void discardArray(DynamicArray* a);
void removeElement(DynamicArray* a, cards element);

#endif // LIST_H_INCLUDED
