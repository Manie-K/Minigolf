#include <iostream>
#include <SDL.h>
#include "config.h"
#include "functions.h"

using namespace std;

int main(int argc, char* args[])
{
	//variables 
	SDLvariables_T SDLvariables;
	SDL_Event event;
	bool quitGame = false;


	if (!initilize(SDLvariables)) return -1;


	SDL_SetRenderDrawColor(SDLvariables.renderer, 0, 0, 0, 255);
	SDL_RenderClear(SDLvariables.renderer);
	SDL_SetRenderDrawColor(SDLvariables.renderer, 255, 255, 255, 255);
	SDL_RenderDrawPoint(SDLvariables.renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	SDL_RenderPresent(SDLvariables.renderer);
	SDL_Delay(5000);

	while (!quitGame)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quitGame = true;
			}
		}
	}
	SDL_Quit();
	return 0;
}