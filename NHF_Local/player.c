#include "player.h"
#include <string.h>
#include "list.h"

void initPlayer(player* p, char* name,SDL_Rect* rec)
{
    strcpy(p->name,name);
    p->score=0;
    p->crntHandSize=13;
    newDynamicArray(&(p->discardedCards));
    if(rec)
        p->discardRect=*rec;

}
void playerRec(player* p, SDL_Rect* rec)
{
    if(rec)
        p->discardRect=*rec;
}
