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
	bool gameRunning = true;
	SDL_Event event;


	if (initSDL(SDLvariables) < 0) return -1;
	if (initBall(SDLvariables, ball) < 0) return -1;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(SDLvariables.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	while (gameRunning)
	{
		timeVariables.frameStart = SDL_GetTicks();

		SDL_SetRenderDrawColor(SDLvariables.renderer, 0, 135, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(SDLvariables.renderer);
		SDL_RenderCopy(SDLvariables.renderer, ball.ballTexture, NULL, &ball.boxModel);
		SDL_RenderPresent(SDLvariables.renderer);
		
		//update times
		timeVariables.frameTime = SDL_GetTicks() - timeVariables.frameStart;

		//update ball
		updateBall(ball, timeVariables.frameTime);
		calculateCollisions(ball);

		//handle events
		while (SDL_PollEvent(&event))
		{
			switch (event.type) {
				case SDL_QUIT:
					gameRunning = false;
					break;
			}
		}
	}


	return 0;
}