#include "cards.h"
#include "list.h"
#include <stdlib.h>

void newDynamicArray(DynamicArray* a)
{
    a->array=(cards*)malloc(sizeof(cards)*4);
    if(!a->array)
    {
        exit(1);
        SDL_Log("No memory");
    }
    a->size=4;
    a->used=0;
}
void addToArray(DynamicArray* a, cards newElement)
{
    if(a->used==a->size)
    {
        a->size+=4;
        a->array=(cards*)realloc(a->array,a->size*sizeof(cards));
    }
    a->array[a->used++]=newElement;
}
void discardArray(DynamicArray* a)
{
    if(a->array!=NULL)
    {
        free(a->array);
    }

    a->used=a->size=0;
    a->array=NULL;
    newDynamicArray(a);
}
