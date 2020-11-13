#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


void game(gameState* p, SDL_Window** window, SDL_Renderer** renderer,char* pName);
void refreshBoard(gameState* g,SDL_Window** window, SDL_Renderer** renderer);
void drawGameBoard(SDL_Window** window,SDL_Renderer** renderer);

//EventHandlers
int clickCardEvent(gameState* g, SDL_Window** window, SDL_Event* ev);


#endif // GAME_H_INCLUDED
