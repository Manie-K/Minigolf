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
	bool gameRunning = true;

	if (gameStart(SDLvariables, ball, hole) < 0) return -1;

	while (gameRunning)
	{
		timeVariables.frameStart = SDL_GetTicks();
		
		//update ball
		updateBall(ball,timeVariables.frameTime, hole);
		
		//handle event
		gameHandleEvents(ball, gameRunning);

		//render
		gameRender(SDLvariables, ball, hole);

		//update time
		timeVariables.frameTime = SDL_GetTicks() - timeVariables.frameStart;
	}


	return 0;
}