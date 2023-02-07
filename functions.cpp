#include "functions.h"
#include <iostream>

using namespace std;

int initSDL(SDL_Variables& vars) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		cout << "Error while initializing SDL: " << SDL_GetError() << "\n";
		return -1;
	}

	vars.window = SDL_CreateWindow("Mini golf", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!vars.window)
	{
		cout << "Error while creating WINDOW: " << SDL_GetError() << "\n";
		return -1;
	}

	vars.renderer = SDL_CreateRenderer(vars.window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!vars.renderer)
	{
		cout << "Error while creating RENDERER: " << SDL_GetError() << "\n";
		return -1;
	}
	return 0;
}

int initBall(SDL_Variables vars, Ball &ball)
{
	const char* ballImagePath = "assets/ball.png";

	ball.x = SCREEN_WIDTH/2;
	ball.xPosBuffer = 0;
	
	ball.y = SCREEN_HEIGHT/2;
	ball.yPosBuffer = 0;

	ball.size = 24;
	ball.radius = ball.size / 2;

	ball.Xspeed = -0.5;
	ball.Yspeed = 0.5;

	SDL_Rect temp;
	temp.x = ball.x - ball.radius;
	temp.y = ball.y - ball.radius;
	temp.w = temp.h = ball.size;
	ball.boxModel = temp;

	SDL_Surface* tempSurface = IMG_Load(ballImagePath);
	ball.ballTexture = SDL_CreateTextureFromSurface(vars.renderer, tempSurface);
	if (!tempSurface || !ball.ballTexture) {
		cout << "Error while initializing BALL: " << SDL_GetError() << "\n";
		return -1;
	}
	SDL_FreeSurface(tempSurface);

	return 0;
}

void updateBall(Ball& ball, double frameTime)
{
	int temp;

	ball.xPosBuffer += ball.Xspeed * frameTime;
	if (ball.xPosBuffer >= 0)
		temp = floor(ball.xPosBuffer);
	else
		temp = ceil(ball.xPosBuffer);
	ball.x += temp;
	ball.xPosBuffer -= temp;

	ball.yPosBuffer += ball.Yspeed * frameTime;
	if (ball.yPosBuffer >= 0)
		temp = floor(ball.yPosBuffer);
	else
		temp = ceil(ball.yPosBuffer);
	ball.y += temp;
	ball.yPosBuffer -= temp;



	if (ball.Xspeed > 0.1)
		ball.Xspeed -= ball.friction;
	else if (ball.Xspeed < -0.1)
		ball.Xspeed += ball.friction;
	else
		ball.Xspeed = 0;

	if (ball.Yspeed > 0.1)
		ball.Yspeed -= ball.friction;
	else if (ball.Yspeed < -0.1)
		ball.Yspeed += ball.friction;
	else
		ball.Yspeed = 0;

	SDL_Rect tempModel;
	tempModel.w = tempModel.h = ball.size;
	tempModel.x = ball.x - ball.radius;
	tempModel.y = ball.y - ball.radius;

	ball.boxModel = tempModel;
}

void calculateCollisions(Ball& ball)
{
	if (ball.x <= ball.radius){
		ball.x = ball.size;
		ball.Xspeed *= -1;
	}
	else if (abs(SCREEN_WIDTH - ball.x) <= ball.radius) {
		ball.x = SCREEN_WIDTH-ball.size;
		ball.Xspeed *= -1;
	}
	if (ball.y <= ball.radius){
		ball.y = ball.size;
		ball.Yspeed *= -1;
	}
	else if (abs(SCREEN_HEIGHT - ball.y) <= ball.radius) {
		ball.y = SCREEN_HEIGHT - ball.size;
		ball.Yspeed *= -1;
	}
}