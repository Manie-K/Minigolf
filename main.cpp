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
	Sounds sounds;
	TextContainer text;
	Menu menu;
	Level level = { 1 };
	Player player = { 0,0,0 };
	bool gameRunning = true;

	//game initial configuration
	if (gameStart(SDLvariables, ball, hole, level,text,menu,sounds) < 0) return -1;
	
	while (gameRunning)
	{
		timeVariables.frameStart = SDL_GetTicks();
		
		//update ball
		gameUpdate(ball,timeVariables.frameTime, hole,level, player,text,SDLvariables.renderer,menu,gameRunning,sounds);
		
		//handle event
		gameHandleEvents(ball, gameRunning,player,text,SDLvariables.renderer,sounds);

		//render
		gameRender(SDLvariables, ball, hole,level,text);

		//update time
		timeVariables.frameTime = SDL_GetTicks() - timeVariables.frameStart;
	}
	//end menu
	finishGame(SDLvariables.renderer,player);

	//cleaning memory
	gameCleanUp(SDLvariables, sounds,ball,hole,menu,text);
	return 0;
}