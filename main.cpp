#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "config.h"
#include "functions.h"

using namespace std;

int main(int argc, char* argv[])
{
	SDL_Variables SDLvariables;
	Time_Variables timeVariables = {0,0};
	Ball ball;
	Hole hole;
	Level level = { 1 };
	Player player = { 0,0,0 };
	bool gameRunning = true;

	if (gameStart(SDLvariables, ball, hole, level) < 0) return -1;

	while (gameRunning)
	{
		timeVariables.frameStart = SDL_GetTicks();
		
		//update ball
		gameUpdate(ball,timeVariables.frameTime, hole,level, player);
		
		//handle event
		gameHandleEvents(ball, gameRunning,player);

		//render
		gameRender(SDLvariables, ball, hole,level);

		//update time
		timeVariables.frameTime = SDL_GetTicks() - timeVariables.frameStart;
	}

	SDL_DestroyWindow(SDLvariables.window);
	SDL_DestroyRenderer(SDLvariables.renderer);
	SDL_Quit();
	return 0;
}