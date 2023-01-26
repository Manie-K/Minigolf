#pragma once
#include <SDL.h>

struct SDLvariables_T
{
	SDL_Window* window;
	SDL_Renderer* renderer;
};

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 400