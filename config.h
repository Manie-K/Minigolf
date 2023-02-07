#pragma once
#include <SDL.h>
#include <SDL_image.h>

#define BALL_FRICTION 0//0.001

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 700

struct SDL_Variables {
	SDL_Window* window;
	SDL_Renderer* renderer;
};

struct Ball {
	int x, y;
	double xPosBuffer, yPosBuffer;
	double Xspeed, Yspeed;
	short int size;
	short int radius;
	SDL_Rect boxModel;
	SDL_Texture* ballTexture;

	const double friction = BALL_FRICTION;
};

struct Time_Variables {
	Uint32 frameStart;
	double frameTime;
};
