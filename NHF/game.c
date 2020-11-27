#include "gamestate.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "sdlTypes.h"
#include "game.h"
#include "cards.h"
#include "player.h"
#include <unistd.h>
#include <time.h>

button_t backToMain;
SDL_Texture* backTexture;

void game(gameState* p, SDL_Window** window, SDL_Renderer** renderer,char* pName, bool initState, linkedList* list)
{
    //gamestate létrehozása
    if(initState)
        gameStateInit(p,pName);

    for(int n=0;n<4;n++)
        callRec(&(p->Players[n]),n,window);

    drawGameBoard(window,renderer);
    int h,w;
    SDL_GetWindowSize(*window,&w,&h);
    SDL_Rect pRect={.x=w-79-10,.y=h-123-20,.w=79,.h=123};
    playerRec(p->Players,&pRect);
    SDL_RectEmpty(&pRect);


    saveGameState(p);
    initCardTexture(renderer);
    SDL_Event ev;

     //Kör ciklus
    for(int round=p->inCurrentTrumpSuit;round<=5;round++)
    {
        //13-szor hívnak a játékosok

        for(int play=p->inRoundNo;play<=13;play++)
        {
            //első hívás esetén passzolniuk kell
            if(play==1)
            {
                    qsort(&(p->Players[0].hand),p->Players[0].crntHandSize,sizeof(cards),cardcmpf);
                    int* pl1;
                    //Jtékos választ 3 lapot
                    pl1=selectCards(p,window,renderer,&ev);
                    if(pl1==NULL)
                        exit(0);
                    //gép választ
                    int** passArr=cpSelecCards(p);
                    //átmásoljuk a játékos által választott indexeket
                    memcpy(passArr[0],pl1,sizeof(int)*3);
                    //"jobb oldalukra" adják tovább a lapokat
                    passCards(p,&passArr);
                    free(passArr);

            }
            qsort(&(p->Players[0].hand),p->Players[0].crntHandSize,sizeof(cards),cardcmpf);

            bool firstCall=true;
            //Minden játékos lerak egy kártyát

            for(int calls=p->inPlayerTurn+p->offSet;calls<4+p->offSet;calls++)
            {
                int m=calls/4;
                int r=calls%4;
                calls%=4;

                updateGameState(p,round,play,calls,p->offSet);
                saveGameState(p);
                //a gamestate adatait frissíti
                refreshBoard(p,window,renderer);

                int cIndex=-1;
                //Ha a játékos következik

                if(calls%4==0)
                {
                    //kéz rendezése
                    qsort(&(p->Players[0].hand),p->Players[0].crntHandSize,sizeof(cards),cardcmpf);
                    //várunk egy eventre
                    while(SDL_PollEvent(&ev))
                        SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
                    while(SDL_WaitEvent(&ev))
                    {
                        //ha kilépünk
                        if(ev.type==SDL_QUIT)
                        {
                            SDL_DestroyWindow(*window);
                            SDL_Quit();
                            return;
                        }
                        //ha back gombra nyomunk, visszatér a main-be
                        if(button_process_event(&backToMain,&ev))
                            return;

                        //kattintás esetén
                        if(ev.type==SDL_MOUSEBUTTONDOWN && ev.button.clicks>0)
                        {
                            //megnézi, hogy melyik kártya az
                            cIndex=clickCardEvent(p,window,&ev,0,NULL);
                            if(cIndex!=-1 && validCard(p,calls,cIndex)==true)
                            {
                                //ha ez volt az első hívás, akkor a kötelező szín megadása
                                if(firstCall)
                                {
                                    memcpy(&p->firstSuitCalled,&p->Players[calls].hand[cIndex].suitIndex,sizeof(int));
                                    firstCall=false;
                                }
                                //kártya hívása
                                callCard(p,renderer, calls,cIndex);
                                updateGameState(p,round,play,calls,p->offSet);
                                qsort(&(p->Players[0].hand),p->Players[0].crntHandSize,sizeof(cards),cardcmpf);
                                break;
                            }
                            else if(!validCard(p,calls,cIndex)&&cIndex!=-1)
                            {
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Pikk Dáma","The card selected is considered an invalid move.\nTry and pick a card that has the same suit as the card that was called first!",*window);
                                refreshBoard(p,window,renderer);
                            }
                        }

                    }
                }
                //Ha a gép következik
                else
                {
                    //random kártya kiválasztása
                    //-->haladhasson a játék
                    cIndex=-1;
                    cIndex=pickRandom(p,calls);
                    if(firstCall)
                    {
                        memcpy(&p->firstSuitCalled,&p->Players[calls].hand[cIndex].suitIndex,sizeof(int));
                        firstCall=false;
                    }

                    callCard(p,renderer,calls,cIndex);
                    updateGameState(p,round,play,calls,p->offSet);
                    sleep(1);

                }
                cIndex=-1;
                //frissíti a gameState-t és újra rajzolja a játék területet
                updateGameState(p,round,play,calls,p->offSet);

                if(calls<4)
                    calls=m*4+r;

                refreshBoard(p,window,renderer);

            }
            //Kiírja ki vitte el a 4 lapot
            char* temp=(char*)malloc(30*sizeof(char));
            int n=takeCallStack(p);
            p->offSet=n;
            strcpy(temp,p->Players[n].name);
            strcat(temp," takes the stack");
            p->firstSuitCalled=-1;
            resetCalledStack(p);
            roundInfo(temp,renderer,window);
            free(temp);
            usleep(10000*120);



        }
        //Ha betöltött játék, akkor visszaállítja a betöltött konstanskat az alapértelmezett
        //értékeikre
        if(!initState)
        {
            *(int*)&p->inRoundNo=1;
            *(int*)&p->inPlayerTurn=0;
        }

        //A körben elvitt kártyák pontjainak kiszámítása
        calcCallScr(p);
        shuffleDeck(p->deck);
        dealCards(p->Players,p->deck);
        resetAllDynamic(p);
    }
    //Játék vége
    int index=0;
    for(int n=0;n<3;n++)
        if(p->Players[index+1].score < p->Players[index].score)
            index++;

    char temp[16];
    strcpy(temp,p->Players[index].name);
    strcat(temp," won!");
    roundInfo("Game Over!",renderer,window);
    SDL_Rect rec={80,.y=240,.h=640,.w=480};
    SDL_Color white={255,255,255};
    writeText(temp,renderer,&rec,25,white);
    rec.y=300;
    writeText("Press any key to continue...",renderer,&rec,12,white);
    SDL_RenderPresent(*renderer);
    while(SDL_WaitEvent(&ev))
    {
        if(ev.type==SDL_KEYDOWN)
            break;
    }
    if(index==0)
    {
        insertNode(list,p->Players[0].name,p->Players[0].score);
        saveLinkedList("toplist.csv",list);
    }
    remove("savefile.txt");
}
void drawGameBoard(SDL_Window** window,SDL_Renderer** renderer)
{

    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    backTexture=SDL_CreateTexture(*renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,606,365);
    backTexture=(SDL_Texture*)IMG_LoadTexture(*renderer,"background.jpg");
    int wW,wH;
    SDL_GetWindowSize(*window,&wW,&wH);
    SDL_RenderCopy(*renderer, backTexture,NULL,NULL);



    backToMain=(button_t)
    {
        .color={.r=230,.g=230,.b=230,.a=255},
        .draw_rect= {.x=0, .y=0,.w=65,.h=20}
    };
    button(*renderer,&backToMain);
    SDL_Color black={0,0,0};
    writeText("Back",renderer,&backToMain.draw_rect,12,black);


}
void drawCards(gameState* g,SDL_Window** window,SDL_Renderer** renderer,int yOffset,int* indexes)
{
    int x,y;
    SDL_GetWindowSize(*window,&x, &y);

    SDL_Rect rect={.x=0,.y=20,.h=y-20,.w=x};
    SDL_RenderCopy(*renderer,backTexture,NULL,&rect);

    for(int n=0; n < (g->Players[0].crntHandSize); n++)
    {
        SDL_Rect target={.x=40+n*30,.y=y-20-123,.w=79,.h=123};
        //Passzolandó lapok megjelenítése
        if(indexes!=NULL)
        {

            for(int i=0;i<3;i++)
            {
                if (indexes[i]==n)
                    target.y-=yOffset;
            }
        }
        loadCardPic(renderer,g->Players[0].hand[n].suitIndex,g->Players[0].hand[n].faceValueIndex,&target);

    }
    if(&(g->Players[0].discardRect)!=NULL && (g->Players[0].discardedCards.used!=0))
    {
            loadCardPic(renderer,5,2,&(g->Players[0].discardRect));

    }
}
void drawUpper(gameState* g, SDL_Window** window, SDL_Renderer** renderer)
{
    int x,y;
    SDL_GetWindowSize(*window,&x, &y);

    char *upperInfo=infoStrip(g);
    char *string="Score: ";
    char num[4];
    sprintf(num,"%d",g->Players[0].score);
    char* finalString=(char*)malloc(sizeof(char)*13);
    strcpy(finalString,string);
    strcat(finalString,num);
    SDL_Color black={0,0,0};
    SDL_Rect info={65,0,.h=20,.w=x};
    SDL_Rect scoreRec={.x=x/2-20,20,.w=60,.h=20};
    SDL_SetRenderDrawColor(*renderer,250,250,250,255);
    SDL_RenderFillRect(*renderer,&info);
    SDL_RenderFillRect(*renderer,&scoreRec);
    info.x=0;
    writeText(upperInfo,renderer,&info,12,black);
    writeText(finalString,renderer,&scoreRec,12,black);
    free(finalString);
    free(upperInfo);

}
void refreshBoard(gameState* g,SDL_Window** window, SDL_Renderer** renderer)
{

    drawCards(g,window,renderer,0,NULL);
    drawUpper(g,window,renderer);
    for(int n=0+g->offSet;n<4+g->offSet;n++)
    {
        int m=n/4;
        int r=n%4;
        n%=4;
        if(g->calledCards[n].suitIndex!=-1)
        {
            loadCardPic(renderer,
                        g->calledCards[n].suitIndex,
                        g->calledCards[n].faceValueIndex,
                        &(g->Players[n].callRect));

        }
        n=m*4+r;

    }
    SDL_RenderPresent(*renderer);

}
//felső információk stringje
char* infoStrip(gameState* g)
{
    char *round="Round: ";
    char *turn="Turn: ";
    char *call="Call No.: ";
    char rn[3];
    char tn[4];
    char cn[3];
    char* upperInfoStrip=(char*)malloc(sizeof(char)*31);
    sprintf(rn,"%d ",g->currentTrumpSuit);
    sprintf(tn,"%d ",g->roundNo);
    sprintf(cn, "%d ",g->playerTurn+1);
    strcpy(upperInfoStrip,round);
    strcat(upperInfoStrip,rn);
    strcat(upperInfoStrip,turn);
    strcat(upperInfoStrip,tn);
    strcat(upperInfoStrip,call);
    strcat(upperInfoStrip,cn);
    return upperInfoStrip;
}
//Hívott kártya indexét adja vissza
int clickCardEvent(gameState* g, SDL_Window** window, SDL_Event* ev,int yOffset, int* indexes)
{
    if(ev->button.button==SDL_BUTTON_LEFT && ev->button.clicks==2)
    {
        int y;
        SDL_GetWindowSize(*window,NULL, &y);
        for(int n=0; n< (g->Players[0].crntHandSize); n++)
        {
            //kiszámolja a kártyák helyét az ablakon belül
            //Ha a klikk és kártya helye megyegyezik akkor visszatér az indexxel
            SDL_Rect target={.x=40+n*30,.y=y-20-123,.w=79,.h=123};
            //Passzolt káryták esetén magasabban vannak
            if(indexes!=NULL)
            {
                for(int i=0;i<3;i++)
                {
                    if(indexes[i]==n)
                        target.y-=yOffset;
                }
            }

            //ha az utolsó indexű kártya a kézben
            if(n==g->Players[0].crntHandSize-1 &&
               ev->button.x>=target.x && ev->button.x<=target.x+target.w &&
               ev->button.y>=target.y && ev->button.y<=target.y+target.h)
            {
                return n;
            }
            //normál esetben
            else if(ev->button.x>=target.x && ev->button.x<=target.x+30 &&
               ev->button.y>=target.y && ev->button.y<=target.y+target.h)
            {
                return n;
            }
            //Ha kiválasztott kártya passzolásnál, akkor a felső részét is érzékelje
            else if(indexes!=NULL && containsElement(indexes,3,n) &&
               ev->button.x>=target.x && ev->button.x<=target.x+target.w &&
               ev->button.y>target.y-yOffset && ev->button.y<=target.y)
            {
                return n;
            }

        }
    }

    return -1;
}

void roundInfo(char* text,SDL_Renderer** renderer,SDL_Window** window)
{
    int h,w;
    SDL_GetWindowSize(*window,&w,&h);
    SDL_Rect rec={.x=0,.y=20,.w=w,.h=h};
    SDL_Rect textRec={.x=0,.y=h/2-50,.w=w,.h=h};
    SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);
    //fokozatosan besötétití a képernyőt
    for(int n=0;n<15;n++)
    {
        SDL_SetRenderDrawColor(*renderer,20,20,20,n);
        SDL_RenderFillRect(*renderer,&rec);
        SDL_RenderPresent(*renderer);
        usleep(1000*50);
    }
    SDL_Color c={255,255,255};
    writeText(text,renderer,&textRec,30,c);
    SDL_RenderPresent(*renderer);
}

int** cpSelecCards(gameState* state)
{

    int** passedArr=(int**) malloc(4*sizeof(int *));
    for(int i=0;i<4;i++)
        passedArr[i]=(int*) malloc(3*sizeof(int));

    for(int n=1;n<4;n++)
    {
        for(int i=0;i<3;i++)
        {
            int ind=-1;
            //ha nála van a pikk dáma, akkor azt passzolja, vagy az annál nagyobbértékű pikk lapokat
            if((ind=indexOfCard(state->Players[n].hand,13,12,4))!=-1 && !containsElement((int*)passedArr[n],3,ind))
                passedArr[n][i]=ind;
            else if((ind=indexOfCard(state->Players[n].hand,13,13,4))!=-1 && !containsElement((int*)passedArr[n],3,ind))
                 passedArr[n][i]=ind;
            else if((ind=indexOfCard(state->Players[n].hand,13,14,4))!=-1 && !containsElement((int*)passedArr[n],3,ind))
                passedArr[n][i]=ind;
            else
            {
                //ha van köre, akkor a legnagyobbakat passzolja passzolja
                if(hasSuit(state->Players[n].hand,13,3))
                {
                    int iter=14;
                    while(true)
                    {
                        //kis körlapokat felesleges passzolnia
                        if(iter==4)
                        {
                            //átugrik a következő else if-be
                            goto kisKor;
                        }

                        if((ind=indexOfCard(state->Players[n].hand,13,iter,3))!=-1 && !containsElement((int*)passedArr[n],3,ind))
                        {

                            passedArr[n][i]=ind;
                            break;
                        }
                        iter--;
                    }
                }
                else if(state->currentTrumpSuit!=5)
                {
                    kisKor: ;
                    int iter=14;
                    while(true)
                    {
                        ind=-1;
                        if((ind=indexOfCard(state->Players[n].hand,13,iter,state->currentTrumpSuit))!=-1 &&
                           !containsElement((int*)passedArr[n],3,ind))
                        {
                            passedArr[n][i]=ind;
                            break;
                        }
                        iter--;
                    }

                }
                else
                {
                    int r=14;
                    while(containsElement((int*)passedArr[n],3,(r=pickRandom(state,n)))&&r!=-1);
                    passedArr[n][i]=r;
                }
            }
        }
    }
    return passedArr;

}
int* selectCards(gameState* state,SDL_Window** window, SDL_Renderer** renderer,SDL_Event* ev)
{
    button_t okBTN=
    {
        .color={.r=101,.g=67,.b=33,.a=255},
        .draw_rect= {.x=640/2-40, .y=240-15,.w=70,.h=25}
    };
    SDL_Color white={255,255,255};
    SDL_Rect rec={0,100,640,380};
    bool filled=false;
    bool redraw=true;
    int* indexes=(int*)malloc(sizeof(int)*3);
    for(int i=0;i<3;i++)
        indexes[i]=-1;
    int iter=0;
    while(true)
    {
        SDL_WaitEvent(ev);
        if(button_process_event(&okBTN,ev) && filled==true)
        {
            return indexes;
        }
        if(button_process_event(&backToMain,ev))
            return NULL;
        if(ev->type==SDL_QUIT)
        {
            SDL_DestroyWindow(*window);
            SDL_Quit();
            exit(0);
        }
        if(redraw)
        {
            drawGameBoard(window,renderer);
            drawCards(state,window,renderer,40,indexes);
            drawUpper(state,window,renderer);
            button(*renderer,&okBTN);
            writeText("Pass Cards",renderer,&okBTN.draw_rect,14,white);
            writeText("Select three cards to pass on!",renderer,&rec,40,white);
            SDL_SetRenderDrawColor(*renderer,255,223,0,255);
            SDL_RenderDrawRect(*renderer,&okBTN.draw_rect);
            SDL_RenderPresent(*renderer);
            if(!containsElement(indexes,3,-1))
                filled=true;
            else
                filled=false;
            redraw=false;
        }
        else if(ev->type==SDL_MOUSEBUTTONDOWN && ev->button.clicks>1)
        {


            int n=clickCardEvent(state,window,ev,40,indexes);
            if(n!=-1 && !containsElement(indexes,3,n))
            {
                int ind;
                if((ind=indexOfElement(indexes,3,-1))!=-1)
                    indexes[ind]=n;
                else
                {
                    indexes[(iter<3) ? (iter++) : (iter=0)]=n;
                    if(iter==0)
                        iter++;
                }
                redraw=true;
            }
            else if(n!=-1 && containsElement(indexes,3,n))
            {
                int ind=indexOfElement(indexes,3,n);
                indexes[ind]=-1;
                redraw=true;
            }
        }
    }
}
void passCards(gameState* state, int*** indexes)
{
    for(int n=1;n<4;n++)
    {
        for(int i=0;i<3;i++)
        {
            switchCards(&state->Players[0].hand[(*indexes)[0][i]],&state->Players[n].hand[(*indexes)[n][i]]);

        }
    }
}
//lineáris keresés segéd fv
bool containsElement(int* array, int size,int element)
{
    for(int n=0;n<size;n++)
        if(array[n]==element)
            return true;
    return false;
}
int indexOfCard(cards* array, int size, faceValue fv, suits s)
{
    for(int n=0;n<size;n++)
    {
        if(array[n].faceValueIndex==(int)fv && array[n].suitIndex==(int)s)
        {
            return n;
        }
    }
    return -1;
}
int indexOfElement(int* array, int size,int element)
{
    for(int n=0;n<size;n++)
    {
        if(array[n]==element)
            return n;
    }
    return -1;
}
bool hasSuit(cards* array, int size, suits s)
{
    for(int n=0;n<13;n++)
        if(array[n].suitIndex==(int)s)
            return true;
    return false;
}
