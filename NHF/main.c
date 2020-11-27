#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "gamestate.h"
#include "sdlTypes.h"
#include "game.h"
#include "popup.h"


//A main menu gombjai
static button_t m_newGameBTN;
static button_t m_prevGameBTN;
static button_t m_highScrBTN;
static button_t m_rulesBTN;
static button_t backB;
gameState state;
SDL_Color black={0,0,0};

//betölti a szövegfájlt
char* loadText(char* path)
{
    //file pointer létrehozása
    char c;
    FILE *file;
    char* string=(char*)malloc(sizeof(char));
    //fájl megnyitása
    file = fopen(path, "r");
    int i=0;
    if (file)
    {
        while ((c = getc(file)) != EOF)
        {
            //karakterenként beolvassa a fájlból
            //a szöveget és lefoglalja annak helyét
            string[i++]=c;
            string=realloc(string, (strlen(string)+2)*sizeof(char));

        }
    }
    string[strlen(string)+1]='\0';
    fclose(file);
    return string;

}

//megrajzolja a főmenüt
void drawMainMenu(SDL_Window** window, SDL_Renderer** renderer)
{
    IMG_Init(0);
    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    //betölti a hátteret
    SDL_Texture* texture=SDL_CreateTexture(*renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,626,365);
    texture=IMG_LoadTexture(*renderer,"background.jpg");
    SDL_RenderCopy(*renderer, texture,NULL,NULL);


    //gombok változóinak megadása
    m_newGameBTN=(button_t){
        .color={.r=225,.g=255,.b=230,.a=255},
        .draw_rect= {.x=235, .y=150,.w=170,.h=30}
    };
    m_prevGameBTN=(button_t)
    {
        .color={.r=225,.g=255,.b=230,.a=255},
        .draw_rect= {.x=235, .y=m_newGameBTN.draw_rect.y+35,.w=170,.h=30}
    };

    m_highScrBTN=(button_t)
    {
        .color={.r=225,.g=255,.b=230,.a=255},
        .draw_rect= {.x=235, .y=m_prevGameBTN.draw_rect.y+35,.w=170,.h=30}
    };

    m_rulesBTN=(button_t)
    {
        .color={.r=225,.g=255,.b=230,.a=255},
        .draw_rect= {.x=235, .y=m_highScrBTN.draw_rect.y+35,.w=170,.h=30}
    };

    //felrajzolja a gombokat
    button(*renderer,&m_newGameBTN);
    button(*renderer, &m_prevGameBTN);
    button(*renderer,&m_highScrBTN);
    button(*renderer, &m_rulesBTN);
    writeText("New Game", renderer,&m_newGameBTN.draw_rect,24,black);
    writeText("Continue Game", renderer,&m_prevGameBTN.draw_rect,24,black);
    writeText("TOP LIST", renderer,&m_highScrBTN.draw_rect,24,black);
    writeText("Rules", renderer,&m_rulesBTN.draw_rect,24,black);
    SDL_RenderPresent(*renderer);

    IMG_Quit();
}

void drawTopList(SDL_Window** window, SDL_Renderer** renderer, linkedList* list)
{
    IMG_Init(0);
    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    //betölti a hátteret
    SDL_Texture* texture=SDL_CreateTexture(*renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,626,365);
    texture=IMG_LoadTexture(*renderer,"background.jpg");
    SDL_RenderCopy(*renderer, texture,NULL,NULL);

    backB=(button_t)
    {
        .color={.r=230,.g=230,.b=230,.a=255},
        .draw_rect= {.x=0, .y=0,.w=65,.h=20}
    };
    button(*renderer,&backB);
    writeText("Back",renderer,&backB.draw_rect,12,black);

    SDL_Rect rect={.x=0,.y=50,.w=640,.h=480};
    SDL_Color white={255,255,255};
    char buff[50];
    listNode* current=list->first->next;
    writeText("Top List:",renderer,&rect,30,white);
    rect.y=100;
    while(current!=list->last)
    {
        sprintf(buff,"* %s - %-4d",current->name,current->score);
        writeText(buff,renderer,&rect,20,white);
        rect.y+=30;
        current=current->next;
    }
    SDL_RenderPresent(*renderer);
    SDL_Event ev;
    while(SDL_WaitEvent(&ev))
    {
        if(button_process_event(&backB,&ev))
            break;
        else if(ev.type==SDL_QUIT)
        {
            SDL_DestroyWindow(*window);
            SDL_DestroyRenderer(*renderer);
            freelist(list);
            exit(0);
        }
    }

}
//Rules menüpont megrajzolása
void drawRules(SDL_Renderer** renderer, SDL_Window** window)
{
    IMG_Init(0);
    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    //betölti a hátteret
    SDL_Texture* texture=SDL_CreateTexture(*renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,626,365);
    texture=IMG_LoadTexture(*renderer,"background.jpg");
    SDL_RenderCopy(*renderer, texture,NULL,NULL);

    char* string=loadText("rules.txt");
    SDL_Color white={255,255,255};
    SDL_Rect rect={20,30,640,480};
    writeText(string, renderer, &rect,15,white);
    backB=(button_t)
    {
        .color={.r=230,.g=230,.b=230,.a=255},
        .draw_rect= {.x=0, .y=0,.w=65,.h=20}
    };
    button(*renderer,&backB);
    writeText("Back",renderer,&backB.draw_rect,12,black);
    SDL_RenderPresent(*renderer);
    SDL_Event ev;
    while(SDL_WaitEvent(&ev))
    {
        if(button_process_event(&backB,&ev))
        {
            break;
        }
        else if(ev.type==SDL_QUIT)
        {
            SDL_DestroyWindow(*window);
            SDL_Quit();
        }
    }
    free(string);


}
void sdl_init(int w, int h, SDL_Window **pwindow, SDL_Renderer **prenderer)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    SDL_Window* window=SDL_CreateWindow("Pikk Dáma",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(window==NULL||renderer==NULL)
    {
        exit(1);
    }
    *pwindow = window;
    *prenderer = renderer;

}
int main(int argc, char *argv[])
{
    //főablak létrehozása
    SDL_Window* window;
    SDL_Renderer* renderer;
    sdl_init(640,480,&window,&renderer);
    linkedList topList;
    linkedListInit(&topList);
    loadTopList("toplist.csv",&topList);
    //main
    drawMainMenu(&window,&renderer);
    //

    //event handler
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT)
    {
        if(button_process_event(&m_newGameBTN, &ev))
        {
            char* name=namePopUp();
            if(name!=NULL)
            {
                gameState newGame={.inRoundNo=1,.inPlayerTurn=0,.inCurrentTrumpSuit=1};
                game(&newGame,&window,&renderer,name,true,&topList);
                free(name);
                drawMainMenu(&window,&renderer);
                exitGameState(&newGame);
            }


        }
        else if(button_process_event(&m_prevGameBTN, &ev))
        {
            //betölt egy elmentett gamestatet
            gameState* prevGame=loadGameState();
            if(prevGame->currentTrumpSuit!=-1)
            {
                game(prevGame,&window,&renderer,prevGame->Players[0].name,false,&topList);
                exitGameState(prevGame);
                free(prevGame);
                drawMainMenu(&window, &renderer);
            }
            else
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Pikk Dáma","No save game found!",window);
            }


        }
        else if(button_process_event(&m_highScrBTN, &ev))
        {
            drawTopList(&window,&renderer,&topList);
            drawMainMenu(&window,&renderer);
        }
        else if(button_process_event(&m_rulesBTN, &ev))
        {

            drawRules(&renderer, &window);
            drawMainMenu(&window,&renderer);
            //TODO - angol
        }

    }

    freelist(&topList);
    //exit
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
