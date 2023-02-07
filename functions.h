#pragma once
#include "config.h"
#include <math.h>

int initSDL(SDL_Variables& vars);
int initBall(SDL_Variables vars, Ball& ball);
void updateBall(Ball& ball, double frameTime);
void calculateCollisions(Ball& ball);