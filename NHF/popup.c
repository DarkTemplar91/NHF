#include <SDL2/SDL.h>
#include <ctype.h>
#include "sdlTypes.h"
#include "popup.h"

button_t ok;
button_t back;
SDL_Window* popUp;
SDL_Renderer* renderer;

void createPopUp()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }
    popUp=(SDL_Window*)SDL_CreateWindow(u8"Pikk Dáma",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED ,240, 135,SDL_WINDOW_OPENGL);
    renderer=SDL_CreateRenderer(popUp,-1,SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer,240,240,240,255);
    if(popUp==NULL||renderer==NULL)
    {
        exit(1);
    }
    SDL_RenderClear(renderer);


    ok=(button_t)
    {
        .color={.r=200,.g=200,.b=200,.a=255},
        .draw_rect= {.x=45, .y=80,.w=70,.h=20}
    };
    back=(button_t)
    {
        .color={.r=200,.g=200,.b=200,.a=255},
        .draw_rect= {.x=125, .y=80,.w=70,.h=20}
    };
    button(renderer,&ok);
    button(renderer,&back);

    SDL_Color black={0,0,0};
    writeText("Ok",&renderer,&ok.draw_rect,12,black);
    writeText("Back",&renderer,&back.draw_rect,12,black);

    SDL_RenderPresent(renderer);
}
char* namePopUp()
{

    createPopUp();
    SDL_Rect rec={.x=0,.y=10,.w=240,.h=135};
    SDL_Color black={0,0,0};
    writeText("Enter Name:",&renderer,&rec,14,black);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_Rect recTextBox={.x=60,.y=40,.w=120,.h=20};
    drawTextBox(&renderer);
    char* text=(char*)malloc((10+1)*sizeof(char));
    bool refreshBox=false;

    SDL_Event event;
    SDL_StartTextInput();
    while(SDL_WaitEvent(&event))
    {
        SDL_RaiseWindow(popUp);
        if(event.window.windowID>1)
        {

            if(event.type==SDL_WINDOWEVENT&&event.window.event==SDL_WINDOWEVENT_CLOSE)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(popUp);
                return NULL;
            }
            else if(button_process_event(&ok,&event))
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(popUp);
                SDL_StopTextInput();
                return text;
            }
            else if(button_process_event(&back,&event))
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(popUp);
                return NULL;
            }
            else if(event.type==SDL_KEYDOWN)
            {
                if(event.key.keysym.sym==SDLK_RETURN && strlen(text)>0)
                {
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(popUp);
                    SDL_StopTextInput();
                    return text;
                }
                if(strlen(text)>0 && event.key.keysym.sym==SDLK_BACKSPACE)
                {
                    if(strlen(text)==1)
                    {
                        text[0]='\0';
                        drawTextBox(&renderer);
                    }
                    else
                    {
                        text[strlen(text)-1]='\0';
                        refreshBox=true;
                    }

                }
            }
            else if(event.type==SDL_TEXTINPUT)
            {
                if(strlen(text)>=10)
                {
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,"Warning","The name is longer than the allowed number of characters (10)!",popUp);
                }
                else
                {
                    strcat(text,event.text.text);
                    refreshBox=true;

                }
            }
            if(refreshBox==true)
            {
                drawTextBox(&renderer);
                writeText(text,&renderer,&recTextBox,12,black);
                SDL_RenderPresent(renderer);
                refreshBox=false;
            }
        }
    }
    return NULL;
}
void drawTextBox(SDL_Renderer** renderer)
{
    SDL_Rect recTextBox={.x=60,.y=40,.w=120,.h=20};
    SDL_SetRenderDrawColor(*renderer,255,255,255,255);
    SDL_RenderFillRect(*renderer,&recTextBox);
    SDL_SetRenderDrawColor(*renderer,0,0,0,255);
    SDL_RenderDrawRect(*renderer, &recTextBox);
    SDL_RenderPresent(*renderer);
}
