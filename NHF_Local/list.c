#include "cards.h"
#include "list.h"
#include <stdlib.h>

void newDynamicArray(DynamicArray* a)
{
    a->array=(cards*)malloc(sizeof(cards));
    a->size=1;
    a->used=0;
}
void addToArray(DynamicArray* a, cards newElement)
{
    if(a->used==a->size)
    {
        a->size*=2;
        a=(cards*)realloc(a->array,a->size*sizeof(cards));
    }
    a->array[a->used++]=newElement;
}

void discardArray(DynamicArray* a)
{
    free(a->array);
    a->used=a->size=0;
    a->array=NULL;

}
void removeElement(DynamicArray* a, cards element)
{
    int index=findIndex(a, element);



}
//binary search
int findIndex(DynamicArray* a, cards element)
{
    return 0;

}
