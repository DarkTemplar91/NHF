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
    {
        p->discardRect.x=rec->x;
        p->discardRect.y=rec->y;
        p->discardRect.w=rec->w;
        p->discardRect.h=rec->h;
    }
    else
    {
        p->discardRect.h=p->discardRect.w=p->discardRect.x=p->discardRect.y=0;
    }


}
void playerRec(player* p, SDL_Rect* rec)
{

    if(rec)
        p->discardRect=*rec;;

}

void callRec(player* p, int index, SDL_Window** window)
{
    int wH,wW;
    SDL_GetWindowSize(*window,&wW,&wH);

    int offsetX=0;
    int offsetY=0;
    switch(index)
    {
        case 0:
            offsetY=1;
            break;
        case 1:
            offsetX=1;
            break;
        case 2:
            offsetY=-1;
            break;
        case 3:
            offsetX=-1;
    }

    SDL_Rect rec={.x=(wW-79)/2+offsetX*79*0.6,.y=(wH-79)/2+offsetY*123*0.6-75,.w=79,.h=123};
    p->callRect=rec;

}
