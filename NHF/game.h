#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include "linkedlist.h"

void game(gameState* p, SDL_Window** window, SDL_Renderer** renderer,char* pName,bool initState, linkedList* list);
void refreshBoard(gameState* g,SDL_Window** window, SDL_Renderer** renderer);
void drawGameBoard(SDL_Window** window,SDL_Renderer** renderer);
char* infoStrip(gameState* g);
void roundInfo(char* text,SDL_Renderer** renderer,SDL_Window** window);

//EventHandlers
int clickCardEvent(gameState* g, SDL_Window** window, SDL_Event* ev, int yOffset, int* indexes);


int* selectCards(gameState* state,SDL_Window** window, SDL_Renderer** renderer,SDL_Event* ev);
int** cpSelecCards(gameState* state);
void passCards(gameState* state,int*** indexes);
int indexOfElement(int* array, int size,int element);
bool containsElement(int* array, int size,int element);
int indexOfCard(cards* array, int size, faceValue fv, suits s);
bool hasSuit(cards* array, int size, suits s);

#endif // GAME_H_INCLUDED
