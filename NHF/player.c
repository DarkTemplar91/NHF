#include "player.h"
#include <string.h>
#include "list.h"

void initPlayer(player* p, char* name)
{
    strcpy(p->name,name);
    p->score=0;
    //newDynamicArray(p->currentHand);

}
