#include "sdlTypes.h"
#include <SDL2/SDL_image.h>

SDL_Texture* cardTexture;

bool button_process_event(button_t *btn, const SDL_Event *ev)
{
    if(ev->type == SDL_MOUSEBUTTONDOWN)
    {

        if(ev->button.button == SDL_BUTTON_LEFT &&
                ev->button.x >= btn->draw_rect.x &&
                ev->button.x <= (btn->draw_rect.x + btn->draw_rect.w) &&
                ev->button.y >= btn->draw_rect.y &&
                ev->button.y <= (btn->draw_rect.y + btn->draw_rect.h))
        {
            return true;

        }
    }
    return false;
}

void button(SDL_Renderer* renderer, button_t* btn)
{
    SDL_SetRenderDrawColor(renderer, btn->color.r, btn->color.g, btn->color.b, btn->color.a);
    SDL_RenderFillRect(renderer, &btn->draw_rect);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderDrawRect(renderer, &btn->draw_rect);

}

void initCardTexture(SDL_Renderer** renderer)
{
    IMG_Init(0);
    cardTexture=(SDL_Texture*)SDL_CreateTexture(*renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,1027,615);
    //123x79
    if(cardTexture==NULL)
    {
        SDL_Log("Failed to init texture");
        exit(1);
    }
    cardTexture=(SDL_Texture*)IMG_LoadTexture(*renderer,"kartya.png");
    IMG_Quit();
}

void loadCardPic(SDL_Renderer** renderer, int suitNo, int cardNo, SDL_Rect* target)
{
    if(cardTexture)
    {
        SDL_Rect source={.x=(cardNo-1)*79,.y=(suitNo-1)*123,.w=79,.h=123};
        if(cardNo==14)
            source.x=0;
        SDL_RenderCopy(*renderer,cardTexture,&source,target);
    }
}

void writeText(const char* text, SDL_Renderer** renderer,SDL_Rect* destin, uint8_t fontSize, SDL_Color color)
{
    TTF_Init();
    TTF_Font* font=TTF_OpenFont("LiberationSerif-Regular.ttf",fontSize);
    if(!font)
    {
        SDL_Log("Opening the font failed");
        exit(1);
    }

    SDL_Surface* textSurface=TTF_RenderUTF8_Blended_Wrapped(font,text,color,640);
    SDL_Texture* textTexture=SDL_CreateTextureFromSurface(*renderer,textSurface);
    SDL_Rect source={.x=textSurface->clip_rect.x,.y=textSurface->clip_rect.y,.w=textSurface->clip_rect.w,.h=textSurface->clip_rect.h};
    SDL_Rect destination={.x=destin->x+(((destin->w)-(textSurface->clip_rect.w))/2),.y=destin->y+2,.w=textSurface->clip_rect.w,.h=textSurface->clip_rect.h};

    SDL_RenderCopy(*renderer,textTexture,&source,&destination);
    //SDL_RenderPresent(*renderer);


    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit();

}

