#include "sdlTypes.h"



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

}


/*
void writeText(const char* text, TTF_Font* font, SDL_Color color,SDL_Rect messBox, SDL_Renderer* renderer)
{
    TFF_Init();
    SDL_Surface *surface=TTF_RenderText_Blended(*font,text,color);
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,surface);
    SDL_RenderCopy(renderer,texture,NULL,&messBox);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

}
*/
