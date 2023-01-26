#include "functions.h"
using namespace std;

bool initilize(SDLvariables_T& SDLvars)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))return false;
	
	SDLvars.window = SDL_CreateWindow("OKNO!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (SDLvars.window == NULL) {
		cout << "Blad: " << SDL_GetError() << "\n";
		return false;
	}
	SDLvars.renderer = SDL_CreateRenderer(SDLvars.window, -1, 0);
	if (SDLvars.renderer == NULL) {
		cout << "Blad: " << SDL_GetError() << "\n";
		return false;
	}
	SDL_RenderSetScale(SDLvars.renderer, 10, 10);
	return true;
}