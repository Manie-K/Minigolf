#include <iostream>
#include "config.h"
#include "functions.h"

using namespace std;

int main(int argc, char* argv[])
{
	SDL_Variables SDLvariables;
	Time_Variables timeVariables = {0,0};
	Ball ball;
	Hole hole;
	TextContainer text;
	Level level = { 1 };
	Player player = { 0,0,0 };
	bool gameRunning = true;

	if (gameStart(SDLvariables, ball, hole, level,text) < 0) return -1;
	
	while (gameRunning)
	{
		timeVariables.frameStart = SDL_GetTicks();
		
		//update ball
		gameUpdate(ball,timeVariables.frameTime, hole,level, player,text,SDLvariables.renderer);
		
		//handle event
		gameHandleEvents(ball, gameRunning,player,text,SDLvariables.renderer);

		//render
		gameRender(SDLvariables, ball, hole,level,text);

		//update time
		timeVariables.frameTime = SDL_GetTicks() - timeVariables.frameStart;
	}

	SDL_DestroyWindow(SDLvariables.window);
	SDL_DestroyRenderer(SDLvariables.renderer);
	TTF_Quit();
	SDL_Quit();
	return 0;
}